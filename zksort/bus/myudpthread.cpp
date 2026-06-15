#include "myudpthread.h"
#include <QDebug>
#include <QHostAddress>
#include "global.h"
#include "logger.h"
#include <string>

using namespace std;

MyUdp            g_Udp;
QVector<QVector<char>> fpgadebugdata;
//static int tmpIpcHeartJudge = 0;
static int tmpIpcLoadJudge = 0;

static unsigned int udpPacketSeq = 0;


static char *p_macBuf = NULL;

MyUdp::MyUdp(QObject *parent) :
    QObject(parent)
{
    localPort = 8081;			//色选机的通讯端口（本地）

    headSize = sizeof(udp_header);
    isOpened = true;
    m_viewIndexMap.insert(0,0x02);
    m_viewIndexMap.insert(1,0x04);
    m_viewIndexMap.insert(2,0x03);
    m_viewIndexMap.insert(3,0x05);
    m_viewIndexMap.insert(4,0x13);
    m_viewIndexMap.insert(5,0x14);
    m_viewIndexMap.insert(6,0x2f);
    m_viewIndexMap.insert(7,0x30);


    connect(this,SIGNAL(sLoadIpcModel(int,int)),this,SLOT(loadIpcModelSlt(int,int)));
    connect(this,SIGNAL(sUpdateCamBadPoint()),this,SLOT(updateCamBadPoint()));
    //    connect(this,SIGNAL(sGetChannelFeedIsOn(quint8)),this,SLOT(getChannelFeedIsOn(quint8)));
}

MyUdp::~MyUdp()
{
    //udpSender->stop();
}

bool MyUdp::UDPIsOpen()
{
    return isOpened;
}

void MyUdp::initSocket()
{
    udpSender = new SendThread(this);
}

void MyUdp::processTheDatagram(QByteArray datagram,quint8 ipOffset)
{
    udp_header head;
    QVector<char> tmpdata;
    int i, j, k, nPacketLen, nCmd, nCmdMinus,nRestart;
    int ipcId = 0, camId = 0, camCount = 0, classCount = 0;
    int packetIndex = 0, packetModelCounts = 0;
    int tmpDataAddr;
    int t_bAlarmIpcStatus[MAX_IPC_NUM];
    int tmpIpcHeartJudge =0;
    ofstream fout;

    int view = 0, unit = 0,group = 0;

    int tmpTestCameraCount = 0,err =0;
    unsigned char testNetdata[16];
    memset(testNetdata, 0, 16);

    for(i = 0; i < MAX_IPC_NUM; i++)
    {
        t_bAlarmIpcStatus[i] = 0;
    }

    headSize=sizeof(udp_header);
    memcpy(&head,datagram.data(),headSize);

    view = m_viewIndexMap.key(head.board_h);
    unit = head.board_l-1;
    nPacketLen = head.len_h*256+head.len_l-6;
    unsigned char* buf = (unsigned char*)malloc(nPacketLen);
    memset(buf, 0, nPacketLen);
    memcpy(buf, datagram.data()+headSize, nPacketLen);

    char* timeStr = QDateTime::currentDateTime().toString("hh:mm:ss").toLatin1().data();

    udpSender->RecvPacket(head);
    nCmd = (head.cmd_h*256+head.cmd_l)&0x7fff;
    nCmdMinus = ((head.cmd_h*256+head.cmd_l)&0x8000)>>15;

    if(struGsh.debugMode && nCmd != 0x01&& nCmd != 0xC0)
    {
        if(unit == 0)
        {
            printf("\n222 recv[%d][%d] cmd[%02x] data[",view,unit,nCmd);
            for(int i=0;i<nPacketLen;i++)
            {
                printf("%02x ",buf[i]);
            }
            printf("]\n");
        }
    }

    tmpDataAddr = 0;

    switch(nCmd)
    {
    case CMD_UDP_IPC_REQ_INFO:
        struIpcShare.struIpcInfo[view][unit].aliveNum = (buf[tmpDataAddr]<<8)|buf[tmpDataAddr+1];
        tmpDataAddr += 2;
        memcpy(struIpcShare.struIpcInfo[view][unit].version,buf+tmpDataAddr,32);
        tmpDataAddr += 32;
        memcpy(struIpcShare.struIpcInfo[view][unit].versionLib,buf+tmpDataAddr,32);
        tmpDataAddr += 32;
        struIpcShare.struIpcInfo[view][unit].sortStat = buf[tmpDataAddr]&0x01;
        tmpDataAddr += 1;
        struIpcShare.struIpcInfo[view][unit].modelIndex = buf[tmpDataAddr];
        tmpDataAddr += 1;
        struIpcShare.struIpcInfo[view][unit].modelStat = buf[tmpDataAddr];
        tmpDataAddr += 1;
        struIpcShare.struIpcInfo[view][unit].roiHeight = (buf[tmpDataAddr]<<8)|buf[tmpDataAddr+1];
        tmpDataAddr += 2;
        struIpcShare.struIpcInfo[view][unit].roiWidth = (buf[tmpDataAddr]<<8)|buf[tmpDataAddr+1];
        struCnfp.nIpcRoiHeight[view] = struIpcShare.struIpcInfo[view][0].roiHeight;
        struCnfp.nIpcRoiWidth[view] = struIpcShare.struIpcInfo[view][0].roiWidth;
        tmpDataAddr += 2;
        struIpcShare.struIpcInfo[view][unit].recvCamTotalLines = (buf[tmpDataAddr]<<24)|(buf[tmpDataAddr+1]<<16)|(buf[tmpDataAddr+2]<<8)|buf[tmpDataAddr+3];
        tmpDataAddr += 4;
        struIpcShare.struIpcInfo[view][unit].loseCamTotalLines = (buf[tmpDataAddr]<<24)|(buf[tmpDataAddr+1]<<16)|buf[tmpDataAddr+2]<<8|buf[tmpDataAddr+3];
        tmpDataAddr += 4;
        struIpcShare.struIpcInfo[view][unit].freeSize = (buf[tmpDataAddr]<<24)|(buf[tmpDataAddr+1]<<16)|buf[tmpDataAddr+2]<<8|buf[tmpDataAddr+3];
        tmpDataAddr += 4;
        struIpcShare.struIpcInfo[view][unit].npuUsed = buf[tmpDataAddr];
        tmpDataAddr += 1;
        struIpcShare.struIpcInfo[view][unit].cpuUsed = buf[tmpDataAddr];
        tmpDataAddr += 1;
        struIpcShare.struIpcInfo[view][unit].memUsed = buf[tmpDataAddr];
        tmpDataAddr += 1;
        struIpcShare.struIpcInfo[view][unit].chipTemp = (buf[tmpDataAddr] > 125) ? (buf[tmpDataAddr]-256) : buf[tmpDataAddr];
        tmpDataAddr += 1;
        struIpcShare.struIpcInfo[view][unit].lineFreq = (buf[tmpDataAddr]<<8)|buf[tmpDataAddr+1];
        tmpDataAddr += 2;
        struIpcShare.struIpcInfo[view][unit].freeSize = (buf[tmpDataAddr]<<8)|buf[tmpDataAddr+1];
        tmpDataAddr += 2;
        struIpcShare.struIpcInfo[view][unit].picCnt = (buf[tmpDataAddr]<<8)|buf[tmpDataAddr+1];
        tmpDataAddr += 2;

        tmpDataAddr += 2;
        if(struCnfg.nXsystemBoard[view] == 1)  //X系统模型升级状态
        {
            struIpcShare.struIpcInfo[view][unit].modelUploadStat = buf[tmpDataAddr];
            tmpDataAddr += 1;
        }

        //rk重启
        if( struIpcShare.struIpcInfo[view][unit].modelIndex == 0
                && struIpcShare.struIpcInfo[view][unit].modelStat == 0
                && struIpcShare.struIpcInfo[view][unit].restartStat == 0
                && struIpcShare.struIpcInfo[view][unit].aliveNum != struIpcShare.struIpcInfo[view][unit].aliveNumLast)
        {
            struIpcShare.struIpcInfo[view][unit].restartStat = 1;

            if(struCnfg.nXsystemBoard[view] == 0)  //非X系统
            {
                myFlow.setRKInfo(false,view,unit,0);
            }
        }

        //AI心跳机制
        if(struIpcShare.struIpcInfo[view][unit].aliveNumLast == struIpcShare.struIpcInfo[view][unit].aliveNum)
        {
            struIpcShare.struIpcInfo[view][unit].aliveRetryCnt++;
        }
        else{
            struIpcShare.struIpcInfo[view][unit].aliveNumLast = struIpcShare.struIpcInfo[view][unit].aliveNum;
            struIpcShare.struIpcInfo[view][unit].aliveRetryCnt = 1;
        }

        //重启机制
        if( struIpcShare.struIpcInfo[view][unit].restartStat == 1)
        {
            if(struIpcShare.allIpcModelLoadOk != 1)//防止重复加载
            {
                struIpcShare.struIpcInfo[view][unit].restartStat = 2;
                logger()->info("view%1,unit%2 start",view,unit);

                err = 0;
                for(i = 0; i < struCnfg.nLayerTotal; i++)
                {
                    for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
                    {
                        if(struCnfg.struLayerInfo[i].nViewIsUsed[j] == 1)
                        {
                            for(k = 0; k <struCnfg.struLayerInfo[i].nViewUnitTotal[j]; k++)
                            {
                                if(struCnfg.struIpcCameraParams[i][j][k].ipcType == IPC_CAMERA_TYPE_NULL) continue;
                                if( struIpcShare.struIpcInfo[j][k].restartStat != 2)
                                {
                                    err = 1;
                                    goto ERR;
                                }
                            }
                        }
                    }
                }
ERR:
                if(err == 0)
                {
                    qDebug()<<"\n----------------start load Ipc model!----------------------";
                    emit sLoadIpcModel(0xff,0xff);
                }
            }
        }
        //接口板保活机制
        struGsh.intefaceAlive = 0;

        if(struGsh.debugMode)
        {
            //            qDebug("Ipc[%d][%d] version is %d.%d", view,unit,struIpcShare.struIpcInfo[view][unit].version[0],struIpcShare.struIpcInfo[view][unit].version[1]);
        }
        struIpcShare.struIpcReqStauts[view][unit].ipcInfoReq = 1;
        if(struGsh.bSortStart && struGsh.bChuteOverRatio[view][unit] == 0)
        {
            myFlow.setIpcCamOverRatio(false,0,view,unit);
            struGsh.bChuteOverRatio[view][unit] = 1;
        }
        break;
    case CMD_UDP_IPC_REQ_MODEL_ABLE:
        packetModelCounts = buf[tmpDataAddr];
        tmpDataAddr += 1;
        packetIndex = buf[tmpDataAddr];
        tmpDataAddr += 1;
        struIpcShare.struIpcModelAble[view][unit].modelCount += buf[tmpDataAddr];
        tmpDataAddr += 1;
        for(i = packetIndex*14; i <(packetIndex+1)*14; i++)
        {
            if(i >= struIpcShare.struIpcModelAble[view][unit].modelCount || i >= MAX_IPC_MODEL_NUM) break;
            memcpy(struIpcShare.struIpcModelAble[view][unit].modelInfo[i].modelname, buf+tmpDataAddr, MAX_IPC_MODEL_NAME_LENGTH);
            tmpDataAddr += MAX_IPC_MODEL_NAME_LENGTH;
            memcpy(struIpcShare.struIpcModelAble[view][unit].modelInfo[i].modelDate, buf+tmpDataAddr, MAX_IPC_MODEL_DATE_LENGTH);
            tmpDataAddr += MAX_IPC_MODEL_DATE_LENGTH;
            if(struGsh.debugMode && unit == 0)
                qDebug("%d/%d ipc[%d][%d], model total:%d, model%d name is %s, date is %s", packetIndex+1,packetModelCounts,view,unit, struIpcShare.struIpcModelAble[view][unit].modelCount,
                       i+1, struIpcShare.struIpcModelAble[view][unit].modelInfo[i].modelname,
                       struIpcShare.struIpcModelAble[view][unit].modelInfo[i].modelDate);
        }
        packetIndex++;
        if (packetIndex == packetModelCounts){
            struIpcShare.struIpcReqStauts[view][unit].modelEnableReq = 1;
        }

        break;

    case CMD_UDP_IPC_REQ_MODEL_INFO:
        packetModelCounts = buf[tmpDataAddr];
        tmpDataAddr += 1;
        packetIndex = buf[tmpDataAddr];
        tmpDataAddr += 1;
        memcpy(struIpcShare.struIpcReqValue[view][unit].modelLoadName, buf+tmpDataAddr, MAX_IPC_MODEL_NAME_LENGTH);
        tmpDataAddr += MAX_IPC_MODEL_NAME_LENGTH;
        if(buf[tmpDataAddr] > 52) logger()->info("classCount err:%1",buf[tmpDataAddr]);
        struIpcShare.struIpcModelClass[view][unit].classCount = buf[tmpDataAddr];
        for(group = 0; group < struCnfg.struLayerInfo[0].stuViewInfo[view].nIdentifyGroupTotal; group++)
        {
            if(myFlow.getUnitGroup(view,unit,group))
            {
                struCnfp.ipcModelClassCount[view][group] = struIpcShare.struIpcModelClass[view][unit].classCount;
//                qDebug()<<"view"<<view<<"group"<<group<<"unit"<<unit<< struCnfp.ipcModelClassCount[view][group];
            }
        }
        //  struCnfp.ipcModelClassCount[view][group] = struIpcShare.struIpcModelClass[view][unit].classCount;
        tmpDataAddr += 1;
        for(i = packetIndex*35; i <(packetIndex+1)*35; i++)
        {
            if(i >= struIpcShare.struIpcModelClass[view][unit].classCount) break;
            memcpy(struIpcShare.struIpcModelClass[view][unit].className[i], buf+tmpDataAddr, MAX_IPC_CLASS_NAME_LENGTH);
            tmpDataAddr += MAX_IPC_CLASS_NAME_LENGTH;
            if(struGsh.debugMode && unit == 0)
            {
                qDebug("ipc[%d][%d], class count is %d, class%d name:%s",  view,unit, struIpcShare.struIpcModelClass[view][unit].classCount,
                       i, struIpcShare.struIpcModelClass[view][unit].className[i]);
            }
        }
        if (packetIndex == (packetModelCounts-1))
            struIpcShare.struIpcReqStauts[view][unit].modelInfoReq = 1;
        break;
#if 0
    case CMD_UDP_IPC_REQ_MODEL_LOAD://无了无了
        memcpy(struIpcShare.struIpcReqValue[view][unit].modelLoadName, buf+tmpDataAddr, MAX_IPC_MODEL_NAME_LENGTH);
        tmpDataAddr += MAX_IPC_MODEL_NAME_LENGTH;
        struIpcShare.struIpcReqValue[view][unit].modelLoadValue = buf[tmpDataAddr];

        if(struGsh.debugMode && unit == 0)
        {
            qDebug("modelLoadReqName[%d][%d] is %s", view,unit, struIpcShare.struIpcReqValue[view][unit].modelLoadName);
        }
        break;
#endif
    case CMD_UDP_IPC_MODEL_ADD_HEAD:
        //      struIpcShare.struIpcReqValue[view][unit].modelAddHeadRes  = buf[tmpDataAddr];
        struIpcShare.struIpcReqStauts[view][unit].modelAddHeadReq = 1;
        break;
    case CMD_UDP_IPC_MODEL_ADD_DATA:
        struIpcShare.struIpcReqValue[view][unit].modelAddDataRes  = buf[tmpDataAddr];
        struIpcShare.struIpcReqStauts[view][unit].modelAddDataReq = 1;
        break;
    case CMD_UDP_IPC_REQ_BAD_MATERIAL_COUNTS:
        //
        //        classCount = buf[tmpDataAddr];
        //        tmpDataAddr += 1;
        //        for(i = 0; i < classCount; i++)
        //        {
        //            struIpcShare.sCurModelClassBadPointsCountPerCam[view][unit][i]
        //                    = (buf[tmpDataAddr]<<24)|(buf[tmpDataAddr+1]<<16)|(buf[tmpDataAddr+2]<<8)|buf[tmpDataAddr+3];
        //            tmpDataAddr += 4;
        //            //           qDebug()<<"IPC:"<<ipOffset<<"CAM:"<<camId<<struIpcShare.sCurModelClassBadPointsCountPerCam[view][unit][camId][i];
        //        }
        //        camBadPoiontRet[view][unit] = 1;
        //        for( i = 0; i < struCnfg.nLayerTotal; i++)
        //        {
        //            for( j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        //            {
        //                if(struCnfg.struLayerInfo[i].nViewIsUsed[j] == 1)
        //                {
        //                    for( k = 0; k <struCnfg.struLayerInfo[i].nViewUnitTotal[j]; k++)
        //                    {
        //                        if(camBadPoiontRet[j][k] == 0)
        //                        {
        //                            tmpDataAddr = 0; //借用
        //                            break;
        //                        }
        //                    }
        //                    if(camBadPoiontRet[i][j] == 0)
        //                        break;
        //                }
        //            }
        //        }
        //        if(tmpDataAddr > 0)
        //        {
        //            memset(camBadPoiontRet,0,sizeof(camBadPoiontRet));
        //            emit sUpdateCamBadPoint();
        //        }
        break;

    case CMD_UDP_IPC_REQ_BAD_MATERIAL_COUNTS_2:
        packetModelCounts = buf[tmpDataAddr]; // 总包数
        tmpDataAddr += 1;
        packetIndex = buf[tmpDataAddr];  // 子包序号
        tmpDataAddr += 1;
        classCount = buf[tmpDataAddr]*256 + buf[tmpDataAddr+1];  // 当前包类别个数 一个包最多70个类被
        tmpDataAddr += 2;
        // qDebug()<<"CMD_UDP_IPC_REQ_BAD_MATERIAL_COUNTS_2"<<"view"<<view<<"unit"<<unit<<packetModelCounts<<packetIndex<<classCount;
        if(packetIndex == 0)
        {
            struGsh.RkFrameNum[view][unit] =buf[tmpDataAddr]*256*256*256 + buf[tmpDataAddr+1]*256*256 +  buf[tmpDataAddr+2]*256 + buf[tmpDataAddr+3]; // RK框数据
            tmpDataAddr += 4;
            struGsh.StaticsFrameNum[view][unit]  = buf[tmpDataAddr]*256*256*256 + buf[tmpDataAddr+1]*256*256 +  buf[tmpDataAddr+2]*256 + buf[tmpDataAddr+3]; // 统计框数据
            tmpDataAddr += 4;
            // qDebug()<< "CMD_UDP_IPC_REQ_BAD_MATERIAL_COUNTS_2" <<struGsh.RkFrameNum[view][unit] << struGsh.StaticsFrameNum[view][unit] ;
        }
        else
        {
            tmpDataAddr += 8;
        }

        for(i = packetIndex*70; i <packetIndex*70+classCount; i++)
        {
            struIpcShare.sCurModelClassBadPointsAreaPerCam[view][unit][i] =
                    (buf[tmpDataAddr]*256*256*256)+(buf[tmpDataAddr+1]*256*256)+(buf[tmpDataAddr+2]*256)+buf[tmpDataAddr+3];

            tmpDataAddr += 4;
            struIpcShare.sCurModelClassBadPointsFrameAreaPerCam[view][unit][i] =
                    (buf[tmpDataAddr]*256*256*256)+(buf[tmpDataAddr+1]*256*256)+(buf[tmpDataAddr+2]*256)+buf[tmpDataAddr+3];
            tmpDataAddr += 4;
            struIpcShare.sCurModelClassBadPointsCountPerCam[view][unit][i] =
                    (buf[tmpDataAddr]*256*256*256)+(buf[tmpDataAddr+1]*256*256)+(buf[tmpDataAddr+2]*256)+buf[tmpDataAddr+3];
            tmpDataAddr += 8;

//            qDebug()<<"CMD_UDP_IPC_REQ_BAD_MATERIAL_COUNTS_2"<<"view"<<view<<"unit"<<unit<<"i"<<i<< struIpcShare.sCurModelClassBadPointsAreaPerCam[view][unit][i]
//                      <<struIpcShare.sCurModelClassBadPointsFrameAreaPerCam[view][unit][i]
//                        <<struIpcShare.sCurModelClassBadPointsCountPerCam[view][unit][i];
        }
        if( (packetIndex+1) == packetModelCounts) //所有类别完成传输
            struGsh.camBadPoiontRet[view][unit] = 1;

        for( j = 0; j < struCnfg.struLayerInfo[0].nViewChainTotal; j++)
        {
            if(struCnfg.struLayerInfo[0].nViewIsUsed[j] == 1) //
            {
                for( k = 0; k <struCnfg.struLayerInfo[0].nViewUnitTotal[j]; k++)
                {
                    if(struGsh.camBadPoiontRet[j][k] == 0 && struCnfg.struIpcCameraParams[0][j][k].ipcType != IPC_CAMERA_TYPE_NULL)
                    {
                        free(buf);
                        return;
                    }
                }
            }
        }
        emit sUpdateCamBadPoint();

        break;

    case CMD_UDP_IPC_GET_TARGER_VALUE:
        struIpcShare.sCurModelTargetValue[view][unit] = buf[tmpDataAddr];
        struIpcShare.struIpcReqStauts[view][unit].modelTargetValueReq = 1;
        break;
    case CMD_UDP_GET_FPGA_PARAMS:
        tmpdata.clear();
        i = buf[tmpDataAddr]*256 + buf[tmpDataAddr+1];  // 总包
        j = buf[tmpDataAddr+2]*256 + buf[tmpDataAddr+3]; // 当前包
        tmpDataAddr+=4;
        tmpdata<<view<<unit;
        for(i = 0; i < nPacketLen;i++)
        {
            tmpdata<<buf[tmpDataAddr + i];
        }
        fpgadebugdata.append(tmpdata);

        break;
    case CMD_UDP_REQ_CAMERA_ARITH:

        tmpdata<<view<<unit;
        struGsh.fpgaArithCount[view] =  buf[tmpDataAddr]*256 + buf[tmpDataAddr+1];;  // 相机支持算法个数;
        tmpDataAddr += 2;

        for(i=0; i<struGsh.fpgaArithCount[view]*2; i++)
        {
            if(unit == 0) //只接受每个视的第一个相机数据
                struGsh.fpgaAirthAdd[view][i] = buf[tmpDataAddr+i];
            tmpdata<< buf[tmpDataAddr+i];
        }
        qDebug()<<"view"<<view<<"airth count"<<struGsh.fpgaArithCount[view];
        qDebug()<<QByteArray(struGsh.fpgaAirthAdd[view],struGsh.fpgaArithCount[view]*2).toHex();
        fpgadebugdata.append(tmpdata);

        break;
    case CMD_UDP_HT_GET_CAM_IMAGE_INFO_ANS://0x5101
        struGsh.nCamMaxImgCnt[view][unit] = buf[tmpDataAddr] <<16 |buf[tmpDataAddr+1]<< 8 |buf[tmpDataAddr+2];
        tmpDataAddr += 3;
        struGsh.nCamCurImgCnt[view][unit] = buf[tmpDataAddr] <<16 |buf[tmpDataAddr+1]<< 8 |buf[tmpDataAddr+2];
        tmpDataAddr += 3;
        struIpcShare.struIpcReqStauts[view][unit].camImgInfoReq = 1;

        if(struGsh.debugMode)
        {
            QString str = QString("%1/%2").arg(struGsh.nCamCurImgCnt[view][unit]).arg(struGsh.nCamMaxImgCnt[view][unit]);
            qDebug()<<"view"<<view<<"unit"<<unit<<"ddrImg"<< str;
        }
        break;
    case CMD_CAMERA_AI_MISC_STATUS:
        memcpy(struIpcShare.struIpcInfo[view][unit].ugDate,buf+tmpDataAddr,6);
        tmpDataAddr += 6;
        memcpy(struIpcShare.struIpcInfo[view][unit].ugVer,buf+tmpDataAddr,2);
        tmpDataAddr += 2;
        struIpcShare.struIpcInfo[view][unit].modelUploadStat = buf[tmpDataAddr];
        tmpDataAddr += 1;
        //        printf("view%d-unit%d ug date:%2d-%2d-%2d %2d:%2d:%2d, ver:%d-%d, pushStat:%d\n",view,unit,
        //               struIpcShare.struIpcInfo[view][unit].ugDate[0],
        //                struIpcShare.struIpcInfo[view][unit].ugDate[1],
        //                struIpcShare.struIpcInfo[view][unit].ugDate[2],
        //                struIpcShare.struIpcInfo[view][unit].ugDate[3],
        //                struIpcShare.struIpcInfo[view][unit].ugDate[4],
        //                struIpcShare.struIpcInfo[view][unit].ugDate[5],
        //                struIpcShare.struIpcInfo[view][unit].ugVer[0],
        //                struIpcShare.struIpcInfo[view][unit].ugVer[1],
        //                struIpcShare.struIpcInfo[view][unit].modelUploadStat);
        struIpcShare.struIpcReqStauts[view][unit].keepAlive = 1;
        break;

    case CMD_SCREEN_GET_FILE:
        packetModelCounts = buf[tmpDataAddr];
        tmpDataAddr += 1;
        packetIndex = buf[tmpDataAddr];
        tmpDataAddr += 1;

        if(packetIndex == 0){
            if(p_macBuf){
                free(p_macBuf);
            }
            p_macBuf = ( char *)malloc(packetModelCounts*getFileDataPacketLen);
            memset(p_macBuf,0,packetModelCounts*getFileDataPacketLen);
        }
        if(!p_macBuf){
            qDebug()<<"p is not null"<<packetIndex;
            p_macBuf = ( char *)malloc(packetModelCounts*getFileDataPacketLen);
            memset(p_macBuf,0,packetModelCounts*getFileDataPacketLen);
        }
        if(!p_macBuf) return;

        memcpy(&p_macBuf[packetIndex*getFileDataPacketLen],&buf[tmpDataAddr],nPacketLen-2);

        //        qDebug()<<packetModelCounts<<" recv file data:"<<packetIndex+1<<nPacketLen-2;
        if(packetIndex+1 == packetModelCounts){
            qDebug()<<"recv whole file data!";

            fout.open("/sdcard/cnf/.res", ios::binary|ios::out);
            if (fout.is_open())
            {
                fout.write((const char*)p_macBuf, packetIndex*getFileDataPacketLen+nPacketLen-2);     // 写入数据
                fout.close();
                sleep(1);
                system("sync");
                sleep(1);
                system("sync");
                printf("start write file ok!\n");
                struIpcShare.struIpcReqStauts[view][unit].getFileReq = 1;
            }
            free(p_macBuf);
            p_macBuf = NULL;
        }
        break;

    case CMD_SCREEN_PUSH_FILE:
        break;

    case CMD_SCREEN_FILE_DEL:
        break;

    case CMD_SCREEN_SHELL_CMD:
        struIpcShare.struIpcReqStauts[view][unit].finshShellCmdReq = 1;
        break;
        // case CMD_UDP_IPC_TURN_ON:
        //     qDebug("view%1,unit%2 start",view,unit);
        //     logger()->info("view%1,unit%2 start",view,unit);
        //     emit sLoadIpcModel(view,unit);
        //     break;
    default:
        break;
    }
    free(buf);
}

void MyUdp::Stop()
{
    udpSender->stop();
}


void MyUdp::pushUdpCmdData(int nCmd, bool needReturn, int viewId, int unitId, int dataLen, unsigned char *data)
{
    unsigned int sum=0;
    int nInt = 0;
    int nUnitAddr,board;
    if(viewId == BOARD_TYPE_ALL_CAMERA) //all cam board
        board = 0;
    else
        board = struCnfg.struLayerInfo[nInt].nViewBoardType[viewId];

    if(unitId == 0xff)
        nUnitAddr = 0;
    else
        nUnitAddr = struCnfg.struLayerInfo[nInt].stuViewInfo[viewId].nViewUnitId[unitId]+1;
    udp_header head;
    head.identify[0] = 0xa5;
    head.identify[1] = 0x5a;

    head.verSion[0]     = 0x00;
    head.verSion[1]     = 0x02;
    head.board_h = board;
    head.board_l = nUnitAddr;
    head.len_h = (dataLen+6)/256;
    head.len_l = (dataLen+6)%256;
    head.seq_h = udpPacketSeq/256;
    head.seq_l = udpPacketSeq%256;
    if(udpPacketSeq++ > 65530)
    {
        udpPacketSeq = 0;
    }

    if(needReturn == true)
    {
        //        nCmd |= 0x8000;     //需要回包的命令，bit15置1,不需要判断
    }

    head.cmd_h = nCmd/256;
    head.cmd_l = nCmd%256;

    sum = head.identify[0]+head.identify[1]+head.verSion[0]+head.verSion[1]+ head.board_h+ head.board_l
            + head.len_h+head.len_l + head.seq_h+head.seq_l+head.cmd_h+head.cmd_l;

    head.crc_h = sum/256;
    head.crc_l = sum%256;

    udpSender->addPacket(head, data, viewId, unitId, dataLen);
}

#if 0
void MyUdp::WriteUDPBaseLogicRelation(int iLogic, int layer, int sorter)
{
    udp_header head;
    head.crc16 =0;
    head.boardID =BOARD_BASE+sorter+layer*g_SystemConfig.getSorterNumber();
    head.operatorID=OPERATOR_ID_CMDWRITE_FPGA;
    head.startAddress =CMD_VIEW_LOGIC_RELATION_ADDR;
    head.addressCount =1;
    head.packetID =udpSender->getPacketId(layer, sorter);
    if(head.packetID == 255)
        return;

    udpSender->addPacket(head,(uchar *)&iLogic, 4);
}
#endif

void MyUdp::loadIpcModelSlt(int view,int unit)
{
    if(view == 0xff&& unit == 0xff)
    {
        myFlow.resetIpcParams(view,unit);
        return;
    }
    //    if(!ipcThread[view][unit].isRunning())
    //    {
    //        ipcThread[view][unit].setCurrenValue(view,unit);
    //        ipcThread[view][unit].start();
    //    }
}

void MyUdp::updateCamBadPoint()
{

    QVector<QString> tablelist;
    tablelist.clear();
    int ipcViewUsed[MAX_VIEW];
    int temp= 0;
    int temp2  = 0;
    memset(ipcViewUsed,0,sizeof(ipcViewUsed));

    memset(struIpcShare.sCurModelClassBadPointsCount, 0 , sizeof(struIpcShare.sCurModelClassBadPointsCount));
    memset(struIpcShare.sCurModelClassBadPointsArea, 0 , sizeof(struIpcShare.sCurModelClassBadPointsArea));
    memset(struIpcShare.sCurModelClassBadPointsFrameArea, 0 , sizeof(struIpcShare.sCurModelClassBadPointsFrameArea));


    for(int i = 0; i < struCnfg.struLayerInfo[0].nViewChainTotal; i++)
    {
        if(struCnfg.struLayerInfo[0].nViewIsUsed[i] == 1)
        {
            for( int j = 0; j <struCnfg.struLayerInfo[0].nViewUnitTotal[i]; j++)
            {
                if(struCnfg.struIpcCameraParams[0][i][j].ipcType != IPC_CAMERA_TYPE_NULL)
                {
                    for(int m = 0; m < struCnfp.ipcModelClassCount[i][0]; m++)  // 只针对一次
                    {
                        struIpcShare.sCurModelClassBadPointsCount[i][j][m] +=
                                (int)(struIpcShare.sCurModelClassBadPointsCountPerCam[i][j][m]);
                        struIpcShare.sCurModelClassBadPointsArea[i][j][m] +=
                                (int)(struIpcShare.sCurModelClassBadPointsAreaPerCam[i][j][m]);
                        struIpcShare.sCurModelClassBadPointsFrameArea[i][j][m] +=
                                (int)(struIpcShare.sCurModelClassBadPointsFrameAreaPerCam[i][j][m]);

                        if( i == 4 && j == 0)
                            qDebug()<<struIpcShare.sCurModelClassBadPointsCount[i][j][m];
                    }

                    temp = 0;
                    for(int m = 0; m < MAX_IPC_MODEL_CLASS; m++)
                    {
                        temp += struIpcShare.sCurModelClassBadPointsCount[i][j][m];
                    }
                    if(temp <= 0) //数据为0不入库更新
                        continue;

                    temp2 = 1; //有数据更新
                    qDebug()<<"update table" + g_Runtime().modelFullNametest+ QString("_%1_%2").arg(i).arg(j);


                    tablelist.append(g_Runtime().modelFullNametest+ QString("_%1_%2").arg(i).arg(j));
//                    MySqlite mysql;
//                    mysql.updateTable(g_Runtime().modelFullNametest+ QString("_%1_%2").arg(i).arg(j), i, j, 0);
//                   //    mysql.updateTable(g_Runtime().modelFullName_Area+ QString("_%1_%2").arg(i).arg(j),i, j, 1);
//                    //  mysql.updateTable(g_Runtime().modelFullName_Frame+ QString("_%1_%2").arg(i).arg(j),i,j, 2);
//                    system("sync");

                    //                    struIpcShare.sCurModelClassBadPointsCount[0][i][j] = 0 ;
                    //                    struIpcShare.sCurModelClassBadPointsArea[0][i][j] = 0 ;
                    //                    struIpcShare.sCurModelClassBadPointsFrameArea[0][i][j] = 0 ;
                    //                    struIpcShare.sCurModelClassBadPointsCount[0][i][j] = 0 ;
                    //                    memset(struIpcShare.sCurModelClassBadPointsArea, 0 , sizeof(struIpcShare.sCurModelClassBadPointsArea));
                    //                    memset(struIpcShare.sCurModelClassBadPointsFrameArea, 0 , sizeof(struIpcShare.sCurModelClassBadPointsFrameArea));


                }
            }
        }

    }




    if(temp2==0)
        struGsh.IpcStatisdataisrun = 0;

    if(temp2 == 1 && struGsh.updateIpcStatisdata != 1)
    {

        MySqlite mysql;
        mysql.updateTable_2(tablelist, 0);
        qDebug()<<"update  all table";

       //    mysql.updateTable(g_Runtime().modelFullName_Area+ QString("_%1_%2").arg(i).arg(j),i, j, 1);
        //  mysql.updateTable(g_Runtime().modelFullName_Frame+ QString("_%1_%2").arg(i).arg(j),i,j, 2);
        system("sync");
        struGsh.updateIpcStatisdata = 1;
    }





}


/**************************
 *
 *SendThread
 *
 * *******/
//CommUdpOrder SendThread::m_order;
SendThread::SendThread(QObject *parent) :
    QThread(parent)
{
    m_parent = (MyUdp*)parent;
    stopped=false;

    ipcIp = "192.168.1.";
    remotePort = 5001;		//色选板的通讯端口（远端）
    remotePort2 = 20000;    //快速采图相机端命令端口（远端）
    headSize=sizeof(udp_header);
    //    memset(packetIdVector,0,30*256);


    //////////////网口通讯优化///////////////////////////////

    m_CommMode = MODE_BKGD_THD_PROC_SLOT;
    m_bInit = false;
    //启动后台线程，并等待完成初始化
    start();
    while(!m_bInit)
        myFlow.msleep(5);

    //    connect(this, SIGNAL(SendMsg(int,int,int)), &g_UiManager, SLOT(OnSlotRecvMsg(int,int,int)));
    //    connect(this, SIGNAL(SendMsgToPage(QString,int,int,int)), &g_UiManager,
    //            SLOT(OnSlotRecvMsgToPage(QString,int,int,int)));

    m_parent->isOpened = m_MySocket->m_bIsBind;
    //    g_Runtime.g_bUDPopen = m_MySocket->m_bIsBind;
    //    g_Runtime.g_thread = this;
}

SendThread::~SendThread()
{
    stop();
}

void SendThread::stop()
{
    quit();
    stopped=true;
    wait();
}

void SendThread::RepeatSend()
{
    QTime   t;
    udp_header  header;
    int len, sendLen;
#if 0
    QMutexLocker lock(&mutexSend);
    QList<udp_packet>::iterator it = sendPacketList.begin();
    while(it != sendPacketList.end())
    {
        t = it->sendTime;
        //距离上次发送的时间是否达到超时时间段
        if(qAbs(t.msecsTo(QTime::currentTime())) >= it->sendTimeout)
        {
            header = it->header;
            //超时未收到反馈数据
            if(it->sendNum >= cDefMaxSendCount)
            {
                packetIdVector[header.boardID-BOARD_BASE][header.packetID] =no_used;
                //在删除之前，可以发送信号给需要的页面
                FailHandling(header);

                if(NULL != it->data)
                    delete[] it->data;
                it = sendPacketList.erase(it);
                continue;
            }

            //重发数据包
            it->sendNum++;
            it->sendTime=QTime::currentTime();

            QString peerIP = ipcIp+QString::number(header.boardID);

            len = it->len;
            if(len > 4)
            {
                QByteArray buf;
                buf.resize(headSize+len);
                memcpy(buf.data(), &it->header_ex, headSize);
                memcpy(buf.data()+headSize, it->data, len);
                sendLen = m_MySocket->realSendData(buf, peerIP);
            }
            else
            {
                sendLen = m_MySocket->realSendData((char*)&it->header_ex, headSize+len, peerIP);
            }

            //记录重发或发送失败的UDP数据包
            unsigned int firstVal = 0;
            if(len <= 4)
                firstVal = it->header_ex.value;
            RecordLog(sendLen, header, firstVal, len, it->sendNum);
        }
        it++;
    }
#endif
}

void SendThread::run()
{
    m_MySocket = new BackgroudObj(NULL, localUdpPort, this);
    connect(m_MySocket->udpSocket, SIGNAL(readyRead()), m_MySocket,SLOT(readPendingDatagrams()));

    m_MySocket2 = new FastNetObj(NULL, localFastUdpPort, this);
    connect(m_MySocket2->udpSocket, SIGNAL(readyRead()), m_MySocket2,SLOT(readPendingDatagrams()));

    m_bInit = true;
    exec();

    //线程退出前，将后台对象删除
    if(m_MySocket)
    {
        delete m_MySocket;
        m_MySocket = NULL;
    }
    if(m_MySocket2)
    {
        delete m_MySocket2;
        m_MySocket2 = NULL;
    }
}

IpcLoadThread::IpcLoadThread(QThread *parent):QThread(parent)
{
}

IpcLoadThread::~IpcLoadThread()
{
}
void IpcLoadThread::setCurrenValue(int mView,int mUnit)
{
    view = mView;
    unit = mUnit;
}

void IpcLoadThread::run()
{
    myFlow.resetIpcParams(view,unit);
    if(struIpcShare.isIpcRestart[view][unit] == 1)//工控机发生重启
    {
        if(struGsh.bSortStart == 1) //色选状态，重新发送工控机开始色选
        {
            myFlow.onOff();
            myFlow.onOff();
        }
        g_MainManager().SetBottomEnable(true);
        struIpcShare.isIpcRestart[view][unit] = 0;
    }
    struIpcShare.staticsCount[view][unit] = 0;
    struIpcShare.badTotalPerPeriod[view][unit] = 0;

    struIpcShare.struIpcStatus[view][unit].cameraBadMaterialTotal = 0;

    struIpcShare.struIpcStatus[view][unit].cameraBadMaterialContinueLast = 0;
    struIpcShare.struIpcStatus[view][unit].cameraBadMaterialContinueCurrent = 0;

    for(int k = 0;k < MAX_IPC_MODEL_CLASS;k++){
        struIpcShare.sCurModelClassBadPointsCountPerCam[view][unit][k] = 0;
        struIpcShare.sCurModelClassBadPointsCount[view][unit][k] = 0;
        struIpcShare.sCurModelLastClassBadPointsCount[k] = 0;
    }
}


bool SendThread::addPacket(udp_header header, uchar *buf, int view, int unit, int dataLen, qint32 SendTimeout, quint16 maxSendNum)
{
    QString peerIP = "162.254.129.10";//  QString::number(ipcId, 10);
    int sendLen;

    QByteArray send_buf;
    send_buf.resize(headSize+dataLen);
    memcpy(send_buf.data(),&header,headSize);
    memcpy(send_buf.data()+headSize,buf,dataLen);

    if(header.cmd_h * 256 + header.cmd_l == CMD_UDP_HT_GET_CAM_IMAGE_INFO
            || header.cmd_h * 256 + header.cmd_l == CMD_UDP_HT_GET_FAST_CAPTURE
            || header.cmd_h * 256 + header.cmd_l == CMD_UDP_TARGET_FAST_CAPTURE){
        if(view == 0x09 || unit == 0xff){
            peerIP = "255.255.255.255"; //勉强群发待需求吧
        }else{
            peerIP = QString("%1.%2.%3.%4").arg(struCnfg.struIpcCameraParams[struGsh.nLayer][view][unit].nIpAddr[0])
                    .arg(struCnfg.struIpcCameraParams[struGsh.nLayer][view][unit].nIpAddr[1])
                    .arg(struCnfg.struIpcCameraParams[struGsh.nLayer][view][unit].nIpAddr[2])
                    .arg(struCnfg.struIpcCameraParams[struGsh.nLayer][view][unit].nIpAddr[3]);
        }
        sendLen = m_MySocket2->realSendData(send_buf, peerIP);
    }
    else{
        sendLen = m_MySocket->realSendData(send_buf, peerIP);
    }

    if(-1 == sendLen)
        return false;
    return true;
}


void SendThread::RecvPacket(udp_header &header)
{
    //检查收到的回复包，删除

    int bFind = false;
    {
        QMutexLocker lock(&mutexSend);
        for(int k=0;k<sendPacketList.count();k++)
        {

            if(header.seq_h == sendPacketList[k].header.seq_h &&
                    header.seq_l == sendPacketList[k].header.seq_l)
            {
                if(NULL != sendPacketList[k].data)
                    delete[] sendPacketList[k].data;
                sendPacketList.removeAt(k);

                //                packetIdVector[header.boardID-BOARD_BASE][header.packetID] =no_used;
                bFind = true;
                break;
            }
        }
    }
}

quint8 SendThread::getPacketId(quint8 layer, quint8 sorter)
{
#if 0
    int sortId = layer*g_SystemConfig.getSorterNumber()+sorter;
    if(sortId >= 30)
        sortId = 29;    //目前通道最大29

    QMutexLocker locker(&mutexSend);

    //get curretn packet id
    int id=packetIdVector[sortId][255];
    int i=(id+1)%255;    //(0-254)
    while(id != i)
    {
        if(packetIdVector[sortId][i] == no_used)
        {
            packetIdVector[sortId][255]= i;
            packetIdVector[sortId][i] = used;
            return i;
        }
        i = (i+1)%255;
    }

    {
        char cLog[100] = {0};
        sprintf(cLog, "send udp packet id 255 error (layer: %d, sorter: %d) !!!", layer, sorter);
        g_SelfLog->writeErrorLog(cLog);
    }
#endif
    return 255;
}


/*********************************
*读回包
*
**********************************************/
BackgroudObj::BackgroudObj(QObject *parent, int listenPort, SendThread* pAttach)
    : QObject(parent)
    , m_pAttach(pAttach)
{
    udpSocket = new QUdpSocket(this);

    m_bIsBind = udpSocket->bind(QHostAddress("162.254.129.100"), listenPort);
}

BackgroudObj::~BackgroudObj()
{

}

qint64 BackgroudObj::realSendData(const QByteArray & sendBuf, QString strPeerIP, bool bNeedLock)
{
    QString str,str1,str2;
    strPeerIP = "162.254.129.10";
    str.prepend(sendBuf.toHex());

    int len = str.length();

    if(struGsh.debugMode)
    {
        if(!(sendBuf[11] == 0x01 && sendBuf[10] == 0) && sendBuf[11] != 0xC0 )
        {
            for(int i = 0; i < len/2;i++)
            {
                str1 = str.left(2);
                str  = str.mid(2);
                if(i == 14)
                    str2.append(" -");
                str2 = str2 +" "+str1;
            }
            qDebug("peerip[%s], port[%d], sendbuf[%s]", qPrintable(strPeerIP), m_pAttach->remotePort, qPrintable(str2)/*sendBuf.data()*/ );
        }
    }

    if(bNeedLock)
    {
        QMutexLocker lock(&m_SendLock);
        return udpSocket->writeDatagram(sendBuf, QHostAddress(strPeerIP), m_pAttach->remotePort);
    }
    else
    {
        return udpSocket->writeDatagram(sendBuf, QHostAddress(strPeerIP), m_pAttach->remotePort);
    }
}

qint64 BackgroudObj::realSendData(const char* sendBuf, int sendLen, QString strPeerIP, bool bNeedLock)
{
    strPeerIP = "162.254.129.10";
    if(bNeedLock)
    {
        QMutexLocker lock(&m_SendLock);
        return udpSocket->writeDatagram(sendBuf, sendLen, QHostAddress(strPeerIP), m_pAttach->remotePort);
    }
    else
    {
        return udpSocket->writeDatagram(sendBuf, sendLen, QHostAddress(strPeerIP), m_pAttach->remotePort);
    }
}

void BackgroudObj::readPendingDatagrams()
{
    //static char cLog[200] = {0};
    //sprintf(cLog, "enter readPendingDatagrams() time : %s",
    //        QTime::currentTime().toString("mm:ss.zzz").toStdString().c_str());
    //g_SelfLog->writeWarnLog(cLog);

    while (udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        int len = udpSocket->pendingDatagramSize();
        datagram.resize(len);

        QHostAddress sender;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(),datagram.size(),
                                &sender, &senderPort);
        if(datagram.at(0) !=0xa5 || datagram.at(1) !=0x5a || len > 4096){
            qDebug()<<"##################Recv ERR###################";
            continue;
        }
        //        QString strHex;
        //        strHex.prepend(datagram.toHex());
        //        if(struGsh.debugMode && datagram.at(11) !=0x01)
        //        {
        //            qDebug("111 size is %d, udp read %s", datagram.size(), datagram.toHex().data());
        //        }
        quint8 ipOffset = 0;
        ipOffset=sender.toString().right(3).toInt()-IPC_IP_ADDR_BASE;
        m_pAttach->m_parent->processTheDatagram(datagram,ipOffset);
        //        m_pAttach->m_parent->processTheDatagram(strHex.toLatin1().data(),ipOffset);
    }
}

void BackgroudObj::Repeat()
{
    m_pAttach->RepeatSend();
}


//快速采图用
FastNetObj::FastNetObj(QObject *parent, int listenPort, SendThread* pAttach)
    : QObject(parent)
    , m_pAttach(pAttach)
{
    udpSocket = new QUdpSocket(this);
    if(myFlow.ipForFastNetwork.contains("192.168.1"))
        m_bIsBind = udpSocket->bind(QHostAddress(myFlow.ipForFastNetwork), listenPort);
    else
        m_bIsBind = udpSocket->bind(QHostAddress("192.168.1.100"), listenPort);
}

FastNetObj::~FastNetObj()
{

}

qint64 FastNetObj::realSendData(const QByteArray & sendBuf, QString strPeerIP, bool bNeedLock)
{
    QString str,str1,str2;
    str.prepend(sendBuf.toHex());

    int len = str.length();

    if(struGsh.debugMode)
    {
        for(int i = 0; i < len/2;i++)
        {
            str1 = str.left(2);
            str  = str.mid(2);
            if(i == 14)
                str2.append(" -");
            str2 = str2 +" "+str1;
        }
        qDebug("peerip[%s], port2[%d], sendbuf[%s]", qPrintable(strPeerIP), m_pAttach->remotePort2, qPrintable(str2)/*sendBuf.data()*/ );
    }

    if(bNeedLock)
    {
        QMutexLocker lock(&m_SendLock);
        return udpSocket->writeDatagram(sendBuf, QHostAddress(strPeerIP), m_pAttach->remotePort2);
    }
    else
    {
        return udpSocket->writeDatagram(sendBuf, QHostAddress(strPeerIP), m_pAttach->remotePort2);
    }
}

qint64 FastNetObj::realSendData(const char* sendBuf, int sendLen, QString strPeerIP, bool bNeedLock)
{
    if(bNeedLock)
    {
        QMutexLocker lock(&m_SendLock);
        return udpSocket->writeDatagram(sendBuf, sendLen, QHostAddress(strPeerIP), m_pAttach->remotePort2);
    }
    else
    {
        return udpSocket->writeDatagram(sendBuf, sendLen, QHostAddress(strPeerIP), m_pAttach->remotePort2);
    }
}

void FastNetObj::readPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        int len = udpSocket->pendingDatagramSize();
        datagram.resize(len);

        QHostAddress sender;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(),datagram.size(),
                                &sender, &senderPort);
        if(datagram.at(0) !=0xa5 || datagram.at(1) !=0x5a || len > 4096){
            qDebug()<<"##################FastNetObj Recv ERR###################";
            continue;
        }
        //        QString strHex;
        //        strHex.prepend(datagram.toHex());
        //        if(struGsh.debugMode && datagram.at(11) !=0x01)
        //        {
        //            qDebug("111 size is %d, udp read %s", datagram.size(), datagram.toHex().data());
        //        }
        quint8 ipOffset = 0;
        ipOffset=sender.toString().right(3).toInt()-IPC_IP_ADDR_BASE;
        m_pAttach->m_parent->processTheDatagram(datagram,ipOffset);
    }
}

void FastNetObj::Repeat()
{
    m_pAttach->RepeatSend();
}
