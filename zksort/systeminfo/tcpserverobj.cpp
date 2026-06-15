#include "tcpserverobj.h"
#include "globalflow.h"
#include <QDebug>
#include "basewidget.h"
TcpServerObj *mytcpServer;

TcpServerObj::TcpServerObj(QObject *parent):QObject(parent)
{
    tcpServer = new QTcpServer;
    tcpSocket = NULL;
    QSettings setting(CFG_APPSet,QSettings::IniFormat);
    setting.beginGroup("eth0");
    IPforConnect =  setting.value("ipStr","").toString();
    setting.endGroup();
    PORTforConnect  = setting.value("tcpPort","9978").toString();
    isconnected = false;

    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
//    connect(tcpServer, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(serverError(QAbstractSocket::SocketError)));
}


TcpServerObj::~TcpServerObj()
{

}
void TcpServerObj::onConnectButtonPressed()
{
    if (tcpServer->isListening()) {
        qDebug() << "Server is already running";
        isconnected = true;
    }
    QHostAddress  address;
    if(address.setAddress(IPforConnect))
    {
        if (!tcpServer->listen(address, PORTforConnect.toUShort())) {
            logger()->info("TCP connect failed:" + tcpServer->errorString() );
            isconnected = false;
        }

        QString logMessage = "TCP connect success:"+ IPforConnect + ":"+PORTforConnect;
        logger()->info(logMessage);
        isconnected =  true;
    }
    else
    {
        logger()->info("TCP connect failed");
        isconnected =  false;
    }
}


void TcpServerObj::onDisconnectButtonPressed()
{
    disconnected();
    tcpServer->close();
    isconnected= false;

}

void TcpServerObj::newConnection()
{
    // 当有新的连接时，接受连接并创建一个新的 QTcpSocket
    // clientSocket = tcpServer->nextPendingConnection();

    // 获取新连接的客户端socket
    QTcpSocket *clientSocket = tcpServer->nextPendingConnection();
    if (!clientSocket) return;
    handleContentVCommand(clientSocket);

    // 将客户端连接信息添加到界面
    QString clientInfo = clientSocket->peerAddress().toString() + ":" + QString::number(clientSocket->peerPort());
    QString logMessage = "new client: " + clientInfo;

    logger()->info(logMessage);

    // 将客户端加入列表
    m_clients.append(clientSocket);

    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(clientSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(clientSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
}

void TcpServerObj::disconnected()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    if (clientSocket != NULL) {
        QString clientInfo = clientSocket->peerAddress().toString() + ":" + QString::number(clientSocket->peerPort());
        QString logMessage = "client close: " + clientInfo;
        logger()->info(logMessage);

        // 从列表中移除客户端
        m_clients.removeOne(clientSocket);
        m_buffers.remove(clientSocket);

        clientSocket->deleteLater();
        clientSocket = NULL;
    }
}

void TcpServerObj::readData()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    // 追加到该客户端的缓冲区，处理粘包/半包
    m_buffers[clientSocket].append(clientSocket->readAll());

    const QByteArray delimiter = "\r\n\r\n";

    // 循环提取以 \r\n\r\n 结尾的完整数据帧
    while (true) {
        int index = m_buffers[clientSocket].indexOf(delimiter);
        if (index == -1)
            break;

        QByteArray data = m_buffers[clientSocket].left(index);
        m_buffers[clientSocket].remove(0, index + delimiter.size());

        if (data.isEmpty())
            continue;
        // 使用 jsoncpp 库解析 JSON 数据
        Json::CharReaderBuilder reader;
        Json::CharReader* jsonReader = reader.newCharReader();
        Json::Value jsonObject;
        std::string errors;

        if (jsonReader->parse(data.constData(), data.constData() + data.size(), &jsonObject, &errors)) {
            // 根据命令类型进行处理
            if (jsonObject.isMember("c")) {
                std::string cmd = jsonObject["c"].asString();
                if (cmd == "con")
                { // 启动色选
                    QString logMessage = "TCP Received: start sort ";
                    logger()->info(logMessage);
                    handleConCommand(clientSocket, jsonObject);
                } else if (cmd == "coff") { // 停止色选
                    QString logMessage = "TCP Received: stop sort ";
                    logger()->info(logMessage);
                    handleConCommand(clientSocket, jsonObject);
                } else if (cmd == "poff") { // 关机
                    QString logMessage = "TCP Received: shutdown ";
                    logger()->info(logMessage);
                    handlePoffCommand(clientSocket, jsonObject);
                } else if (cmd == "qs")
                { // 查询状态
                    handleQsCommand(clientSocket, jsonObject);
                } else if (cmd == "gContentV")//通讯协议版本
                {
                    handleContentVCommand(clientSocket);
                }
                else
                {
                    // 未知命令
                    sendUnknownCommandResponse(clientSocket);
                }
            }
        } else {
            QString errorLog = "Error parsing JSON: " + QString::fromStdString(errors);
        }
        delete jsonReader;
    }
    if(m_buffers.size() > 1024*1024) //1MB
        m_buffers.clear();
}


void TcpServerObj::sendResponse(QTcpSocket *clientSocket, const Json::Value &response)
{
    try {
        // 设置 JSON 写入属性，移除缩进和换行
        Json::StreamWriterBuilder writer;
        writer["indentation"] = "";

        std::string jsonStr = Json::writeString(writer, response);
        QByteArray jsonBytes = QByteArray(jsonStr.c_str(), jsonStr.length());

        jsonBytes.append("\r\n\r\n");
        // 发送数据到客户端
        qint64 bytesWritten = clientSocket->write(jsonBytes);
        if (bytesWritten == -1) {
            QString errorMessage = "Error sending data: "+  clientSocket->errorString();

            //  logger()->info(errorMessage);
            return;
        }
        clientSocket->flush();
        QString logMessage = "Sent: " + QString::fromStdString(jsonStr);
        //  logger()->info(logMessage);
        emit messageSent(jsonBytes);

    } catch (const std::exception& e) {
        QString errorMessage = "Exception occurred: " + QString::fromStdString(e.what());
        // logger()->info(errorMessage);
    }
}

void TcpServerObj::handleConCommand(QTcpSocket *clientSocket, const Json::Value &request)
{
    // 处理启动色选的逻辑
    Json::Value response;
    response["t"] = "r";
    response["c"] = "con";
    response["p"]["r"] = 1;

    sendResponse(clientSocket, response);
    if(struCnfg.nUseIpcEnable == 1)
    {
        if(struIpcShare.allIpcModelLoadOk != 2 )
        {
            return;
        }
    }
    if(struCnfg.nPlcControl) {
        struCnfg.remoteCtrlIsRunning = 0;
        return;
    }

    if(struGsh.bSortStart == 0)
    {
        if(struGsh.bAlarmStatus == 1)
        {
            struCnfg.remoteCtrlIsRunning = 0;
            return;
        }
        logger()->info("sort start");
        g_infoWidget().setLabelText(g_myLan().msg_turning_on);
        g_infoWidget().delayShow();
        myFlow.onOff();

        myFlow.sleep(1);
        g_infoWidget().hide();
    }
    else
    {
        logger()->info("sort stop");
        g_infoWidget().setLabelText(g_myLan().msg_turning_off);
        g_infoWidget().delayShow();
        myFlow.onOff();

        myFlow.sleep(1);
        g_infoWidget().hide();

    }
    g_MainManager().RefreshBottomStatus();
    emit updateui();
}

void TcpServerObj::handleCoffCommand(QTcpSocket *clientSocket, const Json::Value &request)
{
    // 处理停止色选的逻辑
    Json::Value response;
    response["t"] = "r";
    response["c"] = "coff";
    response["p"]["r"] = 1;

    sendResponse(clientSocket, response);

    g_infoWidget().setLabelText(g_myLan().msg_turning_off);

    g_infoWidget().delayShow();
    myFlow.onOff();

    myFlow.sleep(1);
    g_infoWidget().hide();
    g_MainManager().RefreshBottomStatus();
}

void TcpServerObj::handlePoffCommand(QTcpSocket *clientSocket, const Json::Value &request)
{
    // 处理关机的逻辑
    Json::Value response;
    response["t"] = "r";
    response["c"] = "poff";
    response["p"]["r"] = 1;

    sendResponse(clientSocket, response);

    logger()->info("delay power off");
    if(struGsh.bSortStart == 1)
    {
        g_infoWidget().setLabelText(g_myLan().msg_turning_off);

        g_infoWidget().delayShow();
        myFlow.onOff();
        g_infoWidget().hide();
    }

    struGsh.bUpdateState = 1;
    for(int i = 0; i < struCnfg.nIpcCount; i++)
    {
        myFlow.shutdownIpc(true,0,0,0);
    }

    g_infoWidget().setLabelText(g_myLan().msg_ipc_clean_filter_cotton);
    g_infoWidget().showSecs();

    //关机参数保存确认

    g_infoWidget().setLabelText(g_myLan().save_parameters);
    g_infoWidget().delayShow();

    bool status = g_Runtime().save();

    g_infoWidget().hide();

    if(!status) {
        g_infoWidget().setLabelText(g_myLan().msg_params_save_error);
        g_infoWidget().delayShow();
        myFlow.sleep(2);
        g_infoWidget().hide();
        struCnfg.remoteCtrlIsRunning = 0;
        return;
    }

    //通知控制板延时断电
    char data[SEND_PACKET_RECTRL_BOARD_SIZE];
    memset(data, 0, SEND_PACKET_RECTRL_BOARD_SIZE);
    data[0] = ((int)(struCnfg.nDelayedShutdownTime))/256;
    data[1] = ((int)(struCnfg.nDelayedShutdownTime))%256;
    mySerial.pushCom2CmdData(CMD_UPPER_COMPUTER_OUTAGE_REQUEST, 0, BOARD_TYPE_CONTROL, 0x0e, data, 3);
    g_infoWidget().setLabelText(g_myLan().DelayPowerCut);
    g_infoWidget().delayShow();
    myFlow.sleep(3);
    g_infoWidget().hide();

    g_infoWidget().setLabelText(g_myLan().ids_shutdown);
    g_infoWidget().delayShow();

#ifdef Q_OS_UNIX
//    myMqttMsgParaseThread->n_uploadLogFileFlag = 1;
#endif
    myFlow.sleep(2);

    g_infoWidget().hide();
    struCnfg.remoteCtrlIsRunning = 0;
    //    this->close();
#ifndef Q_OS_WIN
    logger()->info("shutdown");
    struGsh.bFlagPowerOff = 1;
    myFlow.setTsBackLight(0);
    system("shutdown -h now");
#endif

}

void TcpServerObj::handleQsCommand(QTcpSocket *clientSocket, const Json::Value &request)
{
    // 处理查询状态的逻辑
    Json::Value response;
    response["t"] = "r";
    response["c"] = "qs";
    response["p"]["r"] = 1;

    // 解析状态位（st）
    int actualPowerStatus = 1 ;
    int actualColorSortingStatus = struGsh.bSortStart;
    int st =(actualColorSortingStatus << COLOR_SORTING_STATUS_BIT)| (actualPowerStatus << POWER_STATUS_BIT) |(1<< 2) |(struGsh.bFlagWiping<<3);
    int colorSortingStatus = (st >> COLOR_SORTING_STATUS_BIT) & 1;
    int powerStatus = (st >> POWER_STATUS_BIT) & 1 ;
    response["p"]["st"] = st;

    // 解析报警位（al）
    int al = 0;
    al |= (struGsh.bAlarmStatus ? 1 : 0) << 0;   // 气压报警
    //    al |= (struGsh.struCnfg.nBoardStatusAlarm ? 1 : 0) << 1;  // 电路板报警
    al |= (struGsh.bAlarmIpcStatus[0]||struGsh.bAlarmIpcStatus[1]? 1 : 0) << 2;  // 深度学习报警
    al |= (struGsh.nAlarmStautsCurrent[ALARM_FEED_LEVEL] ? 1 : 0) << 3; // 料位报警
    al |= (struGsh.bAlarmLamp ? 1 : 0) << 4;         // 灯报警
    //    al |= (struGsh.bFeederCurrentOver[CTRL_ALARM_MAX_FEED_CURRENT] ? 1 : 0) << 5;      // 振动器报警
    response["p"]["al"] = al;
    int airPressureAlarm = (al >> 0) & 1;
    int circuitBoardAlarm = (al >> 1) & 1;
    int deepLearningAlarm = (al >> 2) & 1;
    int materialLevelAlarm = (al >> 3) & 1;
    int lightAlarm = (al >> 4) & 1;
    int vibratorAlarm = (al >> 5) & 1;
    response["p"]["al"] = al;

    // 获取当前方案的索引
    QString strScheme;
    int currentProfileIndex = struCnfg.nProfile;
    if (currentProfileIndex >= 0 && currentProfileIndex < struCnfg.nProfileTotal) {
        // 获取当前方案的材料名称
        QByteArray array(struCnfg.stuProfileIndex[currentProfileIndex].sMaterialName);
        strScheme = QString::fromUtf8(array);
        response["p"]["pl"] = strScheme.toStdString();
    } else {
        response["p"]["pl"] = g_myLan().Invalid_scheme.toStdString();
    }

    response["p"]["dateTime"] = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss").toStdString();
    response["p"]["offDelay"] = 0;//关机延时时间，单位秒，默认0不起效

    sendResponse(clientSocket, response);

//    QString statusInfo = QString("色选状态: %1, 电源状态: %2")
//            .arg(colorSortingStatus  ? "开" : "关")
//            .arg(powerStatus ? "开" : "关");
//    QString alarmInfo = "报警信息: ";
//    QStringList alarmList;
//    if (airPressureAlarm) alarmList << "气压报警";
//    if (circuitBoardAlarm) alarmList << "电路板报警";
//    if (deepLearningAlarm) alarmList << "深度学习报警";
//    if (materialLevelAlarm) alarmList << "料位报警";
//    if (lightAlarm) alarmList << "灯报警";
//    if (vibratorAlarm) alarmList << "振动器报警";

//    if (!alarmList.isEmpty()) {
//        alarmInfo += alarmList.join(" ");
//    } else {
//        alarmInfo += "无报警";
   // }

}
void TcpServerObj::handleContentVCommand(QTcpSocket *clientSocket)
{
    // {
    //     "t": "r",
    //     "c": "gContentV",
    //     "p": {
    //         "r": 1,
    //         "v": "2.6"
    //     }
    // }
    Json::Value response;
    response["t"] = "r";
    response["c"] = "gContentV";
    response["p"]["r"] = 1;
    response["p"]["v"] = "2.6";

    sendResponse(clientSocket, response);
}

void TcpServerObj::sendUnknownCommandResponse(QTcpSocket *clientSocket)
{
    // 发送未知命令的响应
    Json::Value response;
    response["t"] = "r";
    response["c"] = "unk";
    response["p"]["r"] = 0;
    response["p"]["m"] = 101; // 错误码：未知命令

    sendResponse(clientSocket, response);
}

void TcpServerObj::displayError(QAbstractSocket::SocketError socketError)
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    if (clientSocket) {
        QString errorLog = "Error: " + clientSocket->errorString();
        logger()->info(errorLog);
    } else {
        QString errorLog = "Error: " + tcpServer->errorString();
        logger()->info(errorLog);
    }
}

void TcpServerObj::serverError(QAbstractSocket::SocketError socketError)
{
    QString errorLog = "Server Error: " + tcpServer->errorString();
    logger()->info(errorLog);
}


// 向所有客户端发送数据
void TcpServerObj::sendToAllClients(const Json::Value &response)
{
    foreach (QTcpSocket *socket, m_clients) {
        sendToClient(socket, response);
    }
}

// 向指定客户端发送数据
void TcpServerObj::sendToClient(QTcpSocket *socket, const Json::Value &response)
{
    if (socket && socket->state() == QTcpSocket::ConnectedState) {

        sendResponse(socket, response);

//        socket->write(data);
//        socket->flush(); // 立即发送
    }
}
