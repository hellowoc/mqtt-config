#ifndef MYUDPTHREAD_H
#define MYUDPTHREAD_H

#include <QThread>
#include <QUdpSocket>
#include <QtGlobal>
#include <cstring>
#include <QList>
#include <QTime>
#include <QMutex>
#include <QMutexLocker>
#include "logger.h"
#include <constant.h>

#define CMD_UDP_IPC_REQ_INFO                    0x0001// 心跳查询
#define CMD_UDP_IPC_REQ_MODEL_ABLE              0x0002// 查询工控机可用模型
#define CMD_UDP_IPC_REQ_MODEL_INFO              0x0003// 查询模型信息
#define CMD_UDP_IPC_REQ_MODEL_LOAD              0x0004// 加载模型
#define CMD_UDP_IPC_MODEL_ADD_HEAD              0x00E0// 上传模型-1
#define CMD_UDP_IPC_MODEL_ADD_DATA              0x00E1// 上传模型-2

#define CMD_UDP_IPC_REQ_CAMERA_PARAMS           0x0005// 配置相机参数
#define CMD_UDP_IPC_REQ_ARITH_PARAMS            0x0006// 配置算法参数
#define CMD_UDP_IPC_CFG_CONFIG_FILE             0x0007// 配置AI
#define CMD_UDP_IPC_REQ_START_SORT              0x0008// 检测开关
#define CMD_UDP_IPC_REQ_SHUTDOWN                0x0009// 关机/重启
#define CMD_UDP_IPC_REQ_BAD_MATERIAL_COUNTS     0x000A// 查询各相机含杂
#define CMD_UDP_IPC_CLEAR_BAD_MATERIAL_COUNTS   0x000B// 清除各相机含杂
#define CMD_UDP_IPC_TURN_ON                     0x000E// AI上电
#define CMD_UDP_IPC_IMG_DELETE                  0x000F// IPC图片删除
//#define CMD_UDP_IPC_MODEL_CONFIG                0x000F// 模型配置
#define CMD_UDP_IPC_MODEL_DELETE                0x0012// 模型删除
#define CMD_UDP_IPC_SET_FACTORY_MODE            0x0013// 切换工厂模式
#define CMD_UDP_IPC_GET_TARGER_VALUE            0x0020
#define CMD_UDP_IPC_CFG_IP                      0x4000// AI网口设置
#define CMD_UDP_IPC_RESULT_ANDOPERATE           0x4001// 模型结果与运算
#define CMD_CAMERA_GAMMA_SET                    0X4002// gamma校正设置

#define CMD_UDP_HT_GET_FAST_CAPTURE             0x5006// 快速采图
#define CMD_UDP_HT_GET_CAM_IMAGE_INFO           0x5007// 查询相机信息数据
#define CMD_UDP_HT_CFG_SYNC_DIFF                0x5008// 同步行号偏差
#define CMD_UDP_TARGET_FAST_CAPTURE             0x5015// 快速目标采图
#define CMD_UDP_HT_GET_FAST_CAPTURE_ANS         0x5100// 快速采图数据
#define CMD_UDP_HT_GET_CAM_IMAGE_INFO_ANS       0x5101// 相机图像信息数据回包
#define CMD_UDP_IPC_REQ_BAD_MATERIAL_COUNTS_2   0x5016// 查询各相机含杂
#define CMD_UDP_IPC_REQ_BAD_MATERIAL_PARAMS     0x5017// 统计信息参数配置

#define CMD_UDP_REQ_CAMERA_ARITH       0x5018// 相机支持算法查询
#define CMD_UDP_SET_FPGA_STATUS        0x5019// FPGA状态设置
#define CMD_UDP_GET_FPGA_PARAMS        0x501A// FPGA寄存器参数读取

#define CMD_CAMERA_AI_MISC_STATUS       0x00C0  //心跳+状态
#define CMD_SCREEN_GET_FILE             0x00C1  //获取文件
#define CMD_SCREEN_PUSH_FILE            0x00C2  //升级文件
#define CMD_SCREEN_FILE_DEL             0x00C3  //删除文件
#define CMD_SCREEN_SHELL_CMD            0x00C4  //执行命令


//#define CMD_UDP_IPC_TEST_NET_CRTL        0x000C      // 传输测试控制
//#define CMD_UDP_IPC_TEST_NET_DATA        0x000D      // 接收到工控机的传输测试数据

//#define CMD_UDP_IPC_MODEL_ADD_HEAD        0x000E      // 添加模型-头
//#define CMD_UDP_IPC_MODEL_ADD_DATA        0x000F      // 添加模型-数据
//#define CMD_UDP_IPC_REQ_HARDWARE_INFO     0x0010      // 工控机硬件信息查询
//#define CMD_UDP_IPC_REQ_CONFIG_FILE        0x0011      // 工控机配置文件查询
//#define CMD_UDP_IPC_REQ_BAD_MATERIAL        0x0012      // 当前检测到的坏物料个数
//#define CMD_UDP_IPC_TURN_ON                 0x0013      // 工控机上电启动
//#define CMD_UDP_IPC_NEED_RESTART            0x0014      // 上位机控制工控机重启

//#define CMD_UDP_IPC_REQ_CAMERA_PARAMS_2    0x0015     // 配置相机参数2
//#define CMD_UDP_IPC_REQ_BAD_MATERIAL_COUNTS     0x0016   // 查询各相机含杂
//#define CMD_UDP_IPC_CLEAR_BAD_MATERIAL_COUNTS   0x0017   // 清除各相机含杂

//#define CMD_UDP_IPC_RUN_TEST                    0x0018   // 连续运行测试


const int   cDefMaxSendCount    = 3;            //最大发送次数
const int   cDefSendTimeout     = 500;          //每次发送的默认超时时间（ms）
const int localUdpPort = 8082;
const int localFastUdpPort = 20000;
const int getFileDataPacketLen = 1210;         //1230-14-2

class QUdpSocket;
class SendThread;
class SendThread;
class MyUdp;

//udp数据包头
class udp_header
{
  public:
    unsigned char identify[2];  //0xa5, 0x5a
    unsigned char verSion[2];  //默认1.00
    unsigned char board_h;//板级，高字节
    unsigned char board_l;//板级，低字节
    unsigned char len_h; //包长-高字节
    unsigned char len_l;  //包长，低字节
    unsigned char seq_h; //序号-高字节
    unsigned char seq_l;  //序号，低字节
    unsigned char cmd_h; //命令字-高字节
    unsigned char cmd_l;  //命令字，低字节
    unsigned char crc_h; //包长-高字节   从包开始开始到“命令字”的包头Checksum
    unsigned char crc_l;  //包长，低字节

//    int ipcId;
//    int cameraId;
//    int dataLen;

    udp_header(void)
    {
        memset(this, 0, sizeof(udp_header));
    }
};

class udp_packet
{
public:
    udp_header      header;
    uchar           *data;      //当len=4时，使用header_ex.value字段数据；当len>4时，使用该数据段
    quint16         len;

    quint16         sendNum;        //已发送的次数统计（大于等于最大发送次数时，表示超时未收到反馈数据）
    QTime           sendTime;       //发送时的时间点
    qint32          sendTimeout;    //发送超时时间段（距离上次发送时的时间超过该时间段后，需重发）

    udp_packet(qint32 sdTimeout, quint16 maxSendNum)
    {
        data = NULL;
        len  = 0;
        sendTimeout = sdTimeout;

        sendNum = 0;
        if(maxSendNum > 0 && maxSendNum < cDefMaxSendCount)
            sendNum = cDefMaxSendCount-maxSendNum;
    }
};


class BackgroudObj : public QObject
{
    Q_OBJECT

public:
    explicit BackgroudObj(QObject *parent, int listenPort, SendThread* pAttach);
    ~BackgroudObj();
    qint64 realSendData(const QByteArray&, QString strPeerIP, bool bNeedLock = true);
    qint64 realSendData(const char*, int sendLen, QString strPeerIP, bool bNeedLock = true);

    QUdpSocket *udpSocket;
    bool    m_bIsBind;
    SendThread* m_pAttach;
    QMutex  m_SendLock;

public slots:
    void readPendingDatagrams();
    void Repeat();
};

class FastNetObj : public QObject
{
    Q_OBJECT

public:
    explicit FastNetObj(QObject *parent, int listenPort, SendThread* pAttach);
    ~FastNetObj();
    qint64 realSendData(const QByteArray&, QString strPeerIP, bool bNeedLock = true);
    qint64 realSendData(const char*, int sendLen, QString strPeerIP, bool bNeedLock = true);

    QUdpSocket *udpSocket;
    bool    m_bIsBind;
    SendThread* m_pAttach;
    QMutex  m_SendLock;

public slots:
    void readPendingDatagrams();
    void Repeat();
};



enum
{
    MODE_MAIN_THD_PROC,             //主线程处理接收数据的模式
    MODE_BKGD_THD_PROC_POLLING,     //后台线程以轮询方式处理接收数据的模式
    MODE_BKGD_THD_PROC_SLOT         //后台线程以槽方式处理接收数据的模式
};


//class MyUdp;
class SendThread : public QThread
{
    Q_OBJECT

public:
//    static CommUdpOrder m_order;

public slots:
    void RepeatSend();

public:
    explicit SendThread(QObject *parent = 0);
    ~SendThread();
    void stop();

    //参数SendTimeout：单次发送的超时时间（ms）
    //参数maxSendNum：最多发送几次
    bool addPacket(udp_header header, uchar *buf, int ipcId, int cameraId, int dataLen, qint32 SendTimeout = cDefSendTimeout,
                   quint16 maxSendNum = cDefMaxSendCount);
    void RecvPacket(udp_header &header);
    void RecordLog(int result, udp_header header, unsigned int firstVal, quint16 len, int sendNum);

    quint8 getPacketId(quint8 layer, quint8 sorter);
    enum{no_used=0,used};
protected:
    void run();

public:
    int         m_CommMode;      //通讯模式，取值：MODE_MAIN_THD_PROC/MODE_BKGD_THD_PROC_POLLING/MODE_BKGD_THD_PROC_SLOT
    quint16     remotePort;
    quint16     remotePort2;
    bool        m_bInit;
    BackgroudObj*   m_MySocket;
    FastNetObj*   m_MySocket2;

    volatile bool stopped;
    int         isLocalTest;
    MyUdp*      m_parent;

    //处理与页面的交互
//    void procSendMsg(int msg1, int msg2 = 0, int msg3 = 0);
//    void procSendMsgToPage(QString pageName, int msg1, int msg2 = 0, int msg3 = 0); //pageName为空，表示发给当前页面

signals:
//    void SendMsg(int msg1, int msg2, int msg3);
//    void SendMsgToPage(QString pageName, int msg1, int msg2, int msg3);

private:

    quint8     packetIdVector[30][256];  //256放当前的包号
    QList<udp_packet> sendPacketList;
    QMutex mutexSend;

    QString    ipcIp;
    QString    cameraIp;
    quint16    headSize;

public:
    void FailHandling(const udp_header& header);//通讯失败处理函数

signals:
//    void CaptureImagefail();//采集图像失败
//    void OnlineUpdatefail(quint8 boardID);//在线升级失败
//    void CaptureVideoFail();//视频流采集失败
};

class IpcLoadThread : public QThread
{
    Q_OBJECT
public:
    explicit IpcLoadThread(QThread *parent = 0);
    void setCurrenValue(int mView,int mUnit);
    ~IpcLoadThread();
    int view,unit;

protected:
     void run(void);
};

class MyUdp : public QObject
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

    friend class SendThread;

public:
    explicit MyUdp(QObject *parent = 0);
    ~MyUdp();

    bool isOpened;
    bool UDPIsOpen();
    void initSocket();
    void processTheDatagram(QByteArray datagram, quint8 ipOffset);
//    void processTheDatagram(char* datagram, quint8 ipOffset);
    void Stop();

    void pushUdpCmdData(int nCmd, bool needReturn, int viewId, int unitId, int pushUdpCmdData, unsigned char *data);      //UDP发送
private:
     SendThread *udpSender;
     IpcLoadThread ipcThread[MAX_VIEW][MAX_VIEW_UNIT];
     QMap<int,int> m_viewIndexMap;
     quint16     localPort;
     quint16     headSize;
     int requestColor,requestLine;
     quint8      captureType;//0 image 1 video

signals:
    void sLoadIpcModel(int view,int unit);
    void sGetChannelFeedIsOn(quint8 ipcindex);
    void sUpdateCamBadPoint();
    void supdateIpcstatiswidget();
public slots:
    void loadIpcModelSlt(int view,int unit);
    void updateCamBadPoint();
};

#if 0
/*****
 *
 *     网络接口类
 *
 *****************************/
class myUdpNet : public QWidget
{
    Q_OBJECT
public:
    explicit myUdpNet(QWidget *parent = 0);

    void udpBind(QString ip="162.254.129.100",QString port="8081");         //绑定本地IP、端口
    void setUdpRecvType(int type,int layer=0,int view=0,int unit=0);                                          //设置接收类型
    void setUdpRecvTimeOut(int t_s,int t_ms);                               //设置接收超时时间
    void udpStart();
    void udpStop();
//    void serialSendTransStartStop(int flag);                              // 1:开始上传， 0：停止上传
    void udpRestart();

    int udpSendTo(char *buf, int len);                                     //发送网络数据包

    MyNetWorkThread *udpthread;
    int capType;

public slots:
    void doProcessDealSocketFailed();

signals:

private:
    QString IP;
    QString Port;

#ifdef Q_OS_UNIX
    int sockfd;
#else
    SOCKET sockfd;
#endif
    struct sockaddr_in addr_local;
    struct sockaddr_in addr_remote;
    struct hostent *host;

//    MyNetWorkThread *udpthread;

};
#endif

extern MyUdp            g_Udp;
extern QVector<QVector<char>> fpgadebugdata;
#endif // MYUDPTHREAD_H
