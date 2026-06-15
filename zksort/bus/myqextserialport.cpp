#include "myqextserialport.h"
#include "globalparams.h"
#include "data/mysqlite.h"

#include <sys/time.h>

#ifdef Q_OS_UNIX
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <assert.h>
#endif

#define Vref	3
#define calTemperature1(x)	((2000*(1.0/21 + x/(81920.0*Vref))/(20.0/21 - x/(81920.0*Vref)) - 100)/0.38)
#define calTemperature2(x)	(x>>4 >= 0) ? (x>>4+(x%16)) : (x>>4-(x%16))

#define calAirPressure(x)   ((x*5.0/4096-0.6)*1.001/4.4)

#define isVibratoOvercurrent(data,x) ((data&x)==1?true:false)

#ifdef Q_OS_UNIX
ZkSerialPort mySerial;
#else
MyQextSerialPort mySerial;
#endif

mySerialRecvThread *mySerialRecv;
mySerialSendThread *mySerialSend;

mySerial2RecvThread *mySerial2Recv;
mySerial2SendThread *mySerial2Send;

//const QEvent::Type customEvent_send = (QEvent::Type)5001;

//static int pressureTimes=0;
//static int levelTimes[CTRL_ALARM_MAX_FEED_SENSOR]={0};
//static int TemplAlarmTimes[CTRL_ALARM_MAX_TEMP_SENSOR]={0};

enum cmdType
{
    CMD_NO_RSP = 0,
    CMD_RSP_BY_SERIAL,
    CMD_RSP_WAVE,
    CMD_RSP_IMAGE,
    CMD_RSP_VIDIO,
    CMD_RSP_CALIBRATE,
};

unsigned int TB_CRC[256] =
{
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
    0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
    0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
    0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
    0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
    0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
    0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
    0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
    0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};

MyQextSerialPort::MyQextSerialPort()
{
    testSendTimes = 0;
    myPort1 = NULL;
    myPort2 = NULL;
}

unsigned int MyQextSerialPort::crcFpga(char *sData, int nLen)
{
    unsigned char j;
    unsigned int nCrc = 0;

    while(nLen--) {
        for(j = 0x80; j != 0; j >>= 1)
        {
            if((nCrc&0x8000)!=0) {
                nCrc <<= 1;
                nCrc ^= 0x1021;
            }
            else {
                nCrc <<= 1;
            }
            if((*sData&j) != 0) {
                nCrc ^= 0x1021;
            }
        }
        sData++;
    }

    return(nCrc);
}

void MyQextSerialPort::crcFpga2(char *sData, int nLen, uint &ret)
{
    //    unsigned char j;
    //    unsigned int nCrc = 0;

    //    while(nLen--) {
    //        for(j = 0x80; j != 0; j >>= 1)
    //        {
    //            if((nCrc&0x8000)!=0) {
    //                nCrc <<= 1;
    //                nCrc ^= 0x1021;
    //            }
    //            else {
    //                nCrc <<= 1;
    //            }
    //            if((*sData&j) != 0) {
    //                nCrc ^= 0x1021;
    //            }
    //        }
    //        sData++;
    //    }
    //    ret = nCrc;

        while (nLen--)
        {
            uchar j = (uchar)(ret / 256);
            ret <<= 8;
            ret ^= TB_CRC[j^*sData];
            sData++;
        }
}

// linux 下使用

int MyQextSerialPort::comOpen(int nPortno)
{
    QString sComName;
    int ret = -1;


#ifdef Q_OS_UNIX
    struct PortSettings myComSetting = {BAUD115200,DATA_8,PAR_NONE,STOP_1,FLOW_OFF,TIMEOUT};
    sComName.sprintf("/dev/ttySAC%d",nPortno);
# else
     struct PortSettings myComSetting = {BAUD115200,DATA_8,PAR_NONE,STOP_1,FLOW_OFF,TIMEOUT};
    sComName.sprintf("COM%d",nPortno);
#endif
    switch(nPortno) {
    case FIRST_SERIAL_NUM:
            myPort1 = new QextSerialPort(sComName,myComSetting,QextSerialPort::Polling);
//            myPort1->setTimeout(0, true);
            ret = myPort1->open(QIODevice::ReadWrite);
            break;
    case SECOND_SERIAL_NUM:
            myPort2 = new QextSerialPort(sComName,myComSetting,QextSerialPort::Polling);
            ret = myPort2->open(QIODevice::ReadWrite);
    }

    if(ret != 1) {
        qDebug("open port%d is err!", nPortno);
    }
    return 1;
}

int MyQextSerialPort::comClose(int portno)
{
    switch(portno) {
    case FIRST_SERIAL_NUM:
        myPort1->close();
        delete myPort1;
        myPort1 = NULL;
        break;
    case SECOND_SERIAL_NUM:
        myPort2->close();
        delete myPort2;
        myPort2 = NULL;
        break;
    }

    return 0;
}

int MyQextSerialPort::com1Flush()
{
#ifdef Q_OS_UNIX
    myPort1->readAll();
#endif
    return 1;
}

int MyQextSerialPort::com2Flush()
{
#ifdef Q_OS_UNIX
    myPort2->readAll();
#endif
    return 1;
}


/**********************************************************************************************
 *
 *串口发送函数，暂时不用
 *
 */
int MyQextSerialPort::com1Write(int nCmd, char intAddr, char boardType,  char boardAddr, char *data, int nCount)
{
    char sPacket[32];
    char sCrcData[20];
    unsigned int nCrcItt=0;
    int i, ret = 0;

    memset(sPacket, 0, 25);

    sPacket[0]  = 0x53;
    sPacket[1]  = 0xb9;
//    sPacket[2]   = (intAddr<<6)|(boardType%64);    // 接口板地址2bit|电路板型号6bit
    sPacket[2]   = boardType%64;    // 接口板地址2bit|电路板型号6bit，目前只支持一个接口板
    sPacket[3]   = boardAddr+1;                       // 电路板地址
    sPacket[4]   = nCmd/256;
    sPacket[5]   = nCmd%256;

    for(i = 0; i < 16; i++)
    {
        sPacket[6+i]   = data[i];
    }
//    memcpy(sPacket+5, data, 16);

    for (i = 0; i < 20; i++) {
        sCrcData[i] = sPacket[i+2];
    }

    nCrcItt = crcFpga(sCrcData, 20);

    sPacket[22] = nCrcItt/256;
    sPacket[23] = nCrcItt%256;
    sPacket[24] = 0xff;
    sPacket[0]  = 0x53;
    sPacket[1]  = 0xb9;

#if 1
    QString tmpStr;
    if((data[4]*256+data[5] != CMD_CAMERA_WAVE_CAPTURE)
            && (data[4]*256+data[5] != CMD_CAMERA_STATICS_CAPTURE)
            && (data[4]*256+data[5] != CMD_CTRL_ALARM_CAPTURE)
            && (data[4]*256+data[5] != CMD_CTRL_NEW_ALARM_CAPTURE)
            && (data[4]*256+data[5] != CMD_CTRL_ALARM_SET)){
        tmpStr.sprintf("%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x"
                       ,sPacket[0]&0xFF, sPacket[1]&0xFF, sPacket[2]&0xFF, sPacket[3]&0xFF,sPacket[4]&0xFF, sPacket[5]&0xFF, sPacket[6]&0xFF, sPacket[7]&0xFF
                ,sPacket[8]&0xFF, sPacket[9]&0xFF, sPacket[10]&0xFF, sPacket[11]&0xFF,sPacket[12]&0xFF, sPacket[13]&0xFF, sPacket[14]&0xFF, sPacket[15]&0xFF
                ,sPacket[16]&0xFF, sPacket[17]&0xFF, sPacket[18]&0xFF, sPacket[19]&0xFF, sPacket[20]&0xFF, sPacket[21]&0xFF, sPacket[22]&0xFF, sPacket[23]&0xFF
                ,sPacket[24]&0xFF);
        qDebug(qPrintable(tmpStr));
    }
#endif

#ifdef Q_OS_UNIX
    QMutexLocker lock(&m_sendLock);

    for (i = 0; i < 1; i++) {
//        ret = myPort1->write(sPacket, SEND_PACKET_SIZE);
    }
#endif
    return ret;
}


int MyQextSerialPort::com1Read(char *sBuf, int nLen)
{
    int ret = 0;
    int i;
    int nDispLen = 0;

    nDispLen = (nLen > 36) ? 36 : nLen;
    memset(sBuf, 0, nLen);

#ifdef Q_OS_UNIX
    ret = myPort1->read(sBuf, nLen);

    if(ret > 2)
    {
        if((sBuf[2]*256+sBuf[3] != CMD_CAMERA_WAVE_CAPTURE)
                && (sBuf[2]*256+sBuf[3] != CMD_CAMERA_STATICS_CAPTURE)
                && (sBuf[4]*256+sBuf[5] != CMD_CTRL_NEW_ALARM_CAPTURE)
                && (sBuf[2]*256+sBuf[3] != CMD_CTRL_ALARM_CAPTURE)
                && (sBuf[2]*256+sBuf[3] != CMD_CTRL_ALARM_SET))
        {
            printf("com1 read >> %d\n",ret);
            for (i = 0; i < nDispLen; i++) {
                printf("%02x ", sBuf[i]&0xFF);
            }
            printf("\n");
        }
        //    fflush(0);
    }
#endif
    return ret;
}

/* 针对恒流源板灯光强设置重构串口2写函数 */
int MyQextSerialPort::com2Write(int nCmd, char sModuleAddr, char arg, int nValue, int nCount)
{
    char sPacket[16];
    char sCrcData[10];
    unsigned int nCrcItt = 0;
    int i, ret = 0;

    /* pack & add checksum */
    memset(sPacket, 0, 16);

    sPacket[0]  = 0xA5;
    sPacket[1]  = 0x5A;
    sPacket[2]   = 0x01;
    sPacket[3]   = 0;
    sPacket[4]   = nCmd/256;
    sPacket[5]   = nCmd%256;
    sPacket[6]   = 0;
    sPacket[7]   = 0;
    sPacket[8]   = 0;
    sPacket[9]   = arg;
    sPacket[10]  = nValue/256;
    sPacket[11]  = nValue%256;

    for (i = 0; i < 10; i++) {
        sCrcData[i] = sPacket[i+2];
    }

    nCrcItt = crcFpga(sCrcData, 10);
    sPacket[12] = nCrcItt/256;
    sPacket[13] = nCrcItt%256;
    sPacket[14] = 0xff;
    sPacket[15] = 0xff;

#if DEBUG_OUTPUT == 2
    printf("com2 write >> ");
    for (i = 0; i < 16; i++) {
       printf("%02x ",sPacket[i]&0xFF);
    }
    printf("\n");
#endif

#ifdef Q_OS_UNIX
    for (i = 0; i < nCount; i++) {
        ret = myPort2->write(sPacket, 16);
    }
#endif
    return ret;
}

int MyQextSerialPort::com2Read(char *sBuf, int nLen)
{
    int ret = 0;

#ifdef Q_OS_UNIX
    ret = myPort2->read(sBuf, nLen);
#endif
    return ret;
}


/*************************************************************************************************************
 * 新加的消息队列收发
 * ************************************************************************************************************/
void MyQextSerialPort::pushCom1CmdData(int nCmd, char intAddr, char boardType,  char boardAddr, char *data, int nCount)
{
    char sPacket[SEND_PACKET_SIZE+7];
    char sCrcData[20];
    unsigned int nCrcItt=0;
    int i, ret = 0;

    memset(sPacket, 0, 25);

    sPacket[0]  = 0x53;
    sPacket[1]  = 0xb9;
//    sPacket[2]   = (intAddr<<6)|(boardType%64);    // 接口板地址2bit|电路板型号6bit
    sPacket[2]   = boardType%64;    // 接口板地址2bit|电路板型号6bit，目前只支持一个接口板
    sPacket[3]   = (boardAddr == BOARD_TYPE_ALL_IN_CHAIN) ? 0 : boardAddr+1;                       // 电路板地址
    sPacket[4]   = nCmd/256;
    sPacket[5]   = nCmd%256;

    for(i = 0; i < 16; i++)
    {
        sPacket[6+i]   = data[i];
    }

    for (i = 0; i < 20; i++) {
        sCrcData[i] = sPacket[i+2];
    }

    nCrcItt = crcFpga(sCrcData, 20);

    sPacket[22] = nCrcItt/256;
    sPacket[23] = nCrcItt%256;
    sPacket[24] = 0xff;

    char queData[SEND_PACKET_SIZE];
    memcpy(queData, sPacket, SEND_PACKET_SIZE);
    myQueue.push(queData);

//    QApplication::postEvent(&mySerialSend, new QEvent(customEvent_send));

//    const QEvent::Type customEvent_send = (QEvent::Type)5001;
//    PostThreadMessage(g_data.nThreadsendCom1CmdID, WM_SEND_COM_CMD, 0, 0);

    myFlow.msleep(1);//等待5秒
}

void MyQextSerialPort::pushCom2CmdData(int nCmd, char intAddr, char boardType, char boardAddr, char *data, int nCount)
{
    char sPacket[SEND_PACKET_RECTRL_BOARD_SIZE+7];
    char sCrcData[68];
    unsigned int nCrcItt=0;
    int i, ret = 0;

    memset(sPacket, 0, 25);

    sPacket[0]  = 0x53;
    sPacket[1]  = 0xb9;
//    sPacket[2]   = (intAddr<<6)|(boardType%64);    // 接口板地址2bit|电路板型号6bit
    sPacket[2]   = boardType%64;    // 接口板地址2bit|电路板型号6bit，目前只支持一个接口板
    sPacket[3]   = (boardAddr == BOARD_TYPE_ALL_IN_CHAIN) ? 0 : boardAddr+1;                       // 电路板地址
    sPacket[4]   = nCmd/256;
    sPacket[5]   = nCmd%256;

    for(i = 0; i < 64; i++)
    {
        sPacket[6+i]   = data[i];
    }

    for (i = 0; i < 68; i++) {
        sCrcData[i] = sPacket[i+2];
    }

    nCrcItt = crcFpga(sCrcData, 68);

    sPacket[70] = nCrcItt/256;
    sPacket[71] = nCrcItt%256;
    sPacket[72] = 0xff;

    char queData[SEND_PACKET_RECTRL_BOARD_SIZE];
    memcpy(queData, sPacket, SEND_PACKET_RECTRL_BOARD_SIZE);

    myQueue2.push(queData);  //串口485通信
    //myUdpQueue8083.push(queData);  //网口通信
    myFlow.msleep(1);//等待5秒
}

int MyQextSerialPort::isCmdNeedResp(int nCmd)
{
    switch (nCmd)
    {
//    case CMD_CAMERA_CALIBRATE_BRIGHT:
//    case CMD_CAMERA_CALIBRATE_DARK:
//        return CMD_RSP_CALIBRATE;
    case CMD_INT_ENCRYPT_PARAMS_REQUIRE:
    case CMD_CAMERA_STATICS_CAPTURE:
    case CMD_CAMERA_MISC_CAPTURE:
    case CMD_CTRL_ALARM_CAPTURE:
    case CMD_CTRL_NEW_ALARM_CAPTURE:
//    case CMD_CTRL_AIR_CAPTURE:
    case CMD_ALL_VERSION_REQUIRE:
    case CMD_CAMERA_STATUS_CAPTURE:
    case CMD_CAMERA_GET_CALIINFO:
        return CMD_RSP_BY_SERIAL;
    case CMD_CAMERA_WAVE_CAPTURE:
        return CMD_RSP_WAVE;
    default:
        return CMD_NO_RSP;
    }
}

int MyQextSerialPort::sendCom1Cmd(char * data)
{
    int nCmd = data[4]*256+data[5];
    int cmdType = isCmdNeedResp(nCmd);
    int sendTimes = (cmdType == CMD_NO_RSP ? 3 : 1);

    int ret = 0;

    struct timezone tz;
    struct timeval tvafter,tvpre;
    gettimeofday (&tvpre , &tz);
    int useMsecTime = 0;

#ifdef Q_OS_UNIX
//#if 1
    QMutexLocker lock(&m_sendLock);

    for (int i = 0; i < sendTimes; i++) {
        ret = myPort1->write(data, SEND_PACKET_SIZE);
    }
#endif
    gettimeofday (&tvafter , &tz);
    useMsecTime=(tvafter.tv_sec-tvpre.tv_sec)*1000+(tvafter.tv_usec-tvpre.tv_usec)/1000;
//    qDebug("send use time:%d\n", (tvafter.tv_sec-tvpre.tv_sec)*1000+(tvafter.tv_usec-tvpre.tv_usec)/1000);

    if (struGsh.debugMode)
    {
        if((data[4]*256+data[5] != CMD_CAMERA_WAVE_CAPTURE)
                && (data[4]*256+data[5] != CMD_CAMERA_STATICS_CAPTURE)
                && (data[4]*256+data[5] != CMD_CTRL_ALARM_CAPTURE)
                && (data[4]*256+data[5] != CMD_CTRL_NEW_ALARM_CAPTURE)
                && (data[4]*256+data[5] != CMD_CTRL_ALARM_SET))
        {
            QString tmpStr;
            tmpStr.sprintf("sc:%d, use:%d ms, head:[%02x %02x] dst:[%02x %02x] cmd:[%02x %02x]--data: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x crc-%02x %02x %02x"
                           ,testSendTimes,useMsecTime, data[0]&0xFF, data[1]&0xFF, data[2]&0xFF, data[3]&0xFF,data[4]&0xFF, data[5]&0xFF, data[6]&0xFF, data[7]&0xFF
                    ,data[8]&0xFF, data[9]&0xFF, data[10]&0xFF, data[11]&0xFF,data[12]&0xFF, data[13]&0xFF, data[14]&0xFF, data[15]&0xFF
                    ,data[16]&0xFF, data[17]&0xFF, data[18]&0xFF, data[19]&0xFF, data[20]&0xFF, data[21]&0xFF, data[22]&0xFF, data[23]&0xFF
                    ,data[24]&0xFF);
            qDebug(qPrintable(tmpStr));
        }
        testSendTimes++;
    }


    if (cmdType == CMD_RSP_BY_SERIAL)
    {
        lastResCmdId = nCmd;
        for(int i = 0; i < 15; i++)    //最多超时450ms
        {
            myFlow.msleep(15);
            if(hasRecvLastCmd == true)
            {
                break;
            }
        }
        hasRecvLastCmd = false;
    }
    else if (cmdType == CMD_RSP_CALIBRATE)
    {
        lastResCmdId = nCmd;
        for(int i = 0; i < 50; i++)    //校正最多超时5秒
        {
            myFlow.msleep(200);
            if(hasRecvLastCmd == true)
            {
                break;
            }
        }
        hasRecvLastCmd = false;
    }

    return ret;
}

int MyQextSerialPort::sendCom2Cmd(char *data)
{
    int nCmd = data[4]*256+data[5];
    int cmdType = isCmdNeedResp(nCmd);
    int sendTimes = (cmdType == CMD_NO_RSP ? 3 : 1);

    int ret = 0;

    struct timezone tz;
    struct timeval tvafter,tvpre;
    gettimeofday (&tvpre , &tz);
    int useMsecTime = 0;

#ifdef Q_OS_UNIX
    QMutexLocker lock(&m_sendLock);

    for (int i = 0; i < sendTimes; i++) {
        ret = myPort2->write(data, SEND_PACKET_RECTRL_BOARD_SIZE);
    }
#endif
    gettimeofday (&tvafter , &tz);
    useMsecTime=(tvafter.tv_sec-tvpre.tv_sec)*1000+(tvafter.tv_usec-tvpre.tv_usec)/1000;
//    qDebug("send use time:%d\n", (tvafter.tv_sec-tvpre.tv_sec)*1000+(tvafter.tv_usec-tvpre.tv_usec)/1000);

    if(struGsh.debugMode)
    {
        if((data[4]*256+data[5] != CMD_CAMERA_WAVE_CAPTURE)
                && (data[4]*256+data[5] != CMD_CAMERA_STATICS_CAPTURE)
                && (data[4]*256+data[5] != CMD_CTRL_ALARM_CAPTURE))
        {
            QString tmpStr;
            tmpStr.sprintf("sc:%d, use:%d ms, head:[%02x %02x] dst:[%02x %02x] cmd:[%02x %02x]--data: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x crc-%02x %02x %02x"
                           ,testSendTimes,useMsecTime, data[0]&0xFF, data[1]&0xFF, data[2]&0xFF, data[3]&0xFF,data[4]&0xFF, data[5]&0xFF, data[6]&0xFF, data[7]&0xFF
                    ,data[8]&0xFF, data[9]&0xFF, data[10]&0xFF, data[11]&0xFF,data[12]&0xFF, data[13]&0xFF, data[14]&0xFF, data[15]&0xFF
                    ,data[16]&0xFF, data[17]&0xFF, data[18]&0xFF, data[19]&0xFF, data[20]&0xFF, data[21]&0xFF, data[22]&0xFF, data[23]&0xFF
                    ,data[24]&0xFF);
            qDebug(qPrintable(tmpStr));
        }
        testSendTimes++;
    }

    if (cmdType == CMD_RSP_BY_SERIAL)
    {
        lastResCmdId2 = nCmd;
        for(int i = 0; i < 30; i++)    //最多超时200ms
        {
            myFlow.msleep(15);
            if(hasRecvLastCmd2 == true)
            {
                break;
            }
        }
        hasRecvLastCmd2 = false;
    }
    else if (cmdType == CMD_RSP_CALIBRATE)
    {
        lastResCmdId2 = nCmd;
        for(int i = 0; i < 50; i++)    //校正最多超时5秒
        {
            myFlow.msleep(200);
            if(hasRecvLastCmd2 == true)
            {
                break;
            }
        }
        hasRecvLastCmd2 = false;
    }

    return ret;
}

void MyQextSerialPort::com1WriteDirect(int nCmd, char intAddr, char boardType,  char boardAddr, char *data, int nCount)
{
    char sPacket[SEND_PACKET_SIZE+7];
    char sCrcData[20];
    unsigned int nCrcItt=0;
    int i, ret = 0;

    memset(sPacket, 0, 25);

    sPacket[0]  = 0x53;
    sPacket[1]  = 0xb9;
//    sPacket[2]   = (intAddr<<6)|(boardType%64);    // 接口板地址2bit|电路板型号6bit
    sPacket[2]   = boardType%64;    // 接口板地址2bit|电路板型号6bit，目前只支持一个接口板
    sPacket[3]   = (boardAddr == BOARD_TYPE_ALL_IN_CHAIN) ? 0 : boardAddr+1;                       // 电路板地址
    sPacket[4]   = nCmd/256;
    sPacket[5]   = nCmd%256;

    for(i = 0; i < 16; i++)
    {
        sPacket[6+i]   = data[i];
    }

    for (i = 0; i < 20; i++) {
        sCrcData[i] = sPacket[i+2];
    }

    nCrcItt = crcFpga(sCrcData, 20);

    sPacket[22] = nCrcItt/256;
    sPacket[23] = nCrcItt%256;
    sPacket[24] = 0xff;


#if 1
    QString tmpStr;
    if((data[4]*256+data[5] != CMD_CAMERA_WAVE_CAPTURE)
            && (data[4]*256+data[5] != CMD_CAMERA_STATICS_CAPTURE)
            && (data[4]*256+data[5] != CMD_CTRL_ALARM_CAPTURE)
            && (data[4]*256+data[5] != CMD_CTRL_NEW_ALARM_CAPTURE)
            && (data[4]*256+data[5] != CMD_CTRL_ALARM_SET)){
        tmpStr.sprintf("testSendTimes is %d, HEAD:[%02x %02x] ADDR:[%02x %02x] CMD:[%02x %02x]---DATA: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x CRC-%02x %02x %02x"
                       ,testSendTimes,sPacket[0]&0xFF, sPacket[1]&0xFF, sPacket[2]&0xFF, sPacket[3]&0xFF,sPacket[4]&0xFF, sPacket[5]&0xFF, sPacket[6]&0xFF, sPacket[7]&0xFF
                ,sPacket[8]&0xFF, sPacket[9]&0xFF, sPacket[10]&0xFF, sPacket[11]&0xFF,sPacket[12]&0xFF, sPacket[13]&0xFF, sPacket[14]&0xFF, sPacket[15]&0xFF
                ,sPacket[16]&0xFF, sPacket[17]&0xFF, sPacket[18]&0xFF, sPacket[19]&0xFF, sPacket[20]&0xFF, sPacket[21]&0xFF, sPacket[22]&0xFF, sPacket[23]&0xFF
                ,sPacket[24]&0xFF);
        qDebug(qPrintable(tmpStr));
    }

    testSendTimes++;
#endif

#ifdef Q_OS_UNIX
    QMutexLocker lock(&m_sendLock);

    for (int i = 0; i < nCount; i++) {
        ret = myPort1->write(sPacket, SEND_PACKET_SIZE);
    }
#endif
}

/**********************************************************************************************************************
 *
 *
 *
 *
 * ********************************************************************************************************************/
#ifdef Q_OS_UNIX
ZkSerialPort::ZkSerialPort()
{
    memset(&ports,0,sizeof(ports));
    testSendTimes = 0;
}

long ZkSerialPort::zkGetBaudrate(long baudrate)
{
    long tmpBaudrate;

    switch (baudrate) {
    case 921600:
        tmpBaudrate = B921600;
        break;
    case 115200:
        tmpBaudrate = B115200;
        break;
    case 57600:
        tmpBaudrate = B57600;
        break;
    case 19200:
        tmpBaudrate = B19200;
        break;
    case 9600:
        tmpBaudrate = B9600;
        break;
    default:
        tmpBaudrate = B115200;
    }

    return tmpBaudrate;
}

/**
 * @brief open serial port with the specified parameters
 * @param portno handle used for further access
 * @param deviceName - the name of the device to open
 * @param baudrate - rate to open (57600 for example)
 * @param parity - 0 for no parity
 * @param dataBits - 7 or 8
 * @param stopBits - 1 or 2
 * @param iqSize - ignored
 * @param oqSize - ignored
 * @retval -1 on failure
 * */
int ZkSerialPort::zkSerialOpenConfig(int portno, char *deviceName, long baudrate, int parity, int dataBits, int stopBits, int iqSize, int oqSize)
{
    struct termios newtio;

    ports[portno].is_busy = 1;
    strcpy(ports[portno].name, deviceName);
    if ((ports[portno].handle = open(deviceName, O_RDWR, 0666)) == -1) {
        //assert(0);
        return -1;
    }

    /* set the port to raw I/O */
    newtio.c_cflag = CS8 | CLOCAL | CREAD;

    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = ~OPOST;

    newtio.c_lflag = ~(ICANON | ECHO | ECHOE | ISIG);

    newtio.c_cc[VINTR]    = 0;
    newtio.c_cc[VQUIT]    = 0;
    newtio.c_cc[VERASE]   = 0;
    newtio.c_cc[VKILL]    = 0;
    newtio.c_cc[VEOF]     = 4;
    newtio.c_cc[VTIME]    = 0;
    newtio.c_cc[VMIN]     = 1;
    newtio.c_cc[VSWTC]    = 0;
    newtio.c_cc[VSTART]   = 0;
    newtio.c_cc[VSTOP]    = 0;
    newtio.c_cc[VSUSP]    = 0;
    newtio.c_cc[VEOL]     = 0;
    newtio.c_cc[VREPRINT] = 0;
    newtio.c_cc[VDISCARD] = 0;
    newtio.c_cc[VWERASE]  = 0;
    newtio.c_cc[VLNEXT]   = 0;
    newtio.c_cc[VEOL2]    = 0;
    cfsetospeed(&newtio, zkGetBaudrate(baudrate));
    cfsetispeed(&newtio, zkGetBaudrate(baudrate));
    tcsetattr(ports[portno].handle, TCSANOW, &newtio);

    return 0;
}

/**
 * @brief opens serial port with default parameters
 * @param portno handle used for further access
 * @param deviceName - the name of the device to open
 * @retval -1 on failure
 * */
int ZkSerialPort::zkSerialOpen(int portno, char *deviceName, long baudrate)
{
    return zkSerialOpenConfig(portno, deviceName, baudrate, 1, 8, 1, 0, 0);
}

/**
 * @brief close previously opened port
 * @param portno port handle to close
 * @retval -1 on failure
 * */
int ZkSerialPort::zkSerialClose(int portno)
{
    if (ports[portno].is_busy == 1) {
        close(ports[portno].handle);
        ports[portno].is_busy = 0;
        return 0;
    }
    else {
        return -1;
    }
}

/**
 * @brief reads the specified number of bytes from the port.
 *        return when these bytes have been read, or timeout occurs.
 * @param portno the handle
 * @param buf where to store the date
 * @param maxCount the maximum number of bytes to read
 * @retval the actual number of bytes read
 * */
int ZkSerialPort::zkSerialRead(int portno, char *buf, int maxCount, int timeout)
{
    int count = 0;
    fd_set rfds;
    struct timeval tv;
    int retval;

    if (ports[portno].is_busy == 0) {
//		assert(0);
    }

    FD_ZERO(&rfds);
    FD_SET(ports[portno].handle, &rfds);
    tv.tv_sec = timeout/1000;
    tv.tv_usec = (timeout%1000)*1000;
    retval = select(ports[portno].handle+1, &rfds, NULL, NULL, &tv);

    if (retval > 0) {
        count = read(ports[portno].handle, buf, maxCount);
    }

    return count;
}

/**
 * @brief writes out the specified bytes to the port
 * @param portno the handle
 * @param buf the bytes to write
 * @param maxCount the maximum number of bytes to write
 * @retval the actual number of bytes written
 * */
int ZkSerialPort::zkSerialWrite(int portno, char *buf, int maxCount)
{
    int count;

    if (ports[portno].is_busy == 0) {
//		assert(0);
    }

    count = write(ports[portno].handle, buf, maxCount);

    return count;
}

void ZkSerialPort::zkSerialFlushRead(int portno)
{
    tcflush(ports[portno].handle, TCIFLUSH);
}

unsigned int ZkSerialPort::crcFpga(char *sData, int nLen)
{
    unsigned char j;
    unsigned int nCrc = 0;

    while(nLen--) {
        for(j = 0x80; j != 0; j >>= 1)
        {
            if((nCrc&0x8000)!=0) {
                nCrc <<= 1;
                nCrc ^= 0x1021;
            }
            else {
                nCrc <<= 1;
            }
            if((*sData&j) != 0) {
                nCrc ^= 0x1021;
            }
        }
        sData++;
    }

    return(nCrc);
}

void ZkSerialPort::crcFpga2(char *sData, int nLen, uint &ret)
{
    while (nLen--)
    {
        uchar j = (uchar)(ret / 256);
        ret <<= 8;
        ret ^= TB_CRC[j^*sData];
        sData++;
    }
}

// linux 下使用
int ZkSerialPort::comOpen(int nPortno)
{
#ifdef Q_OS_UNIX
    char dev[64];
    int ret = 0;

    sprintf(dev, "/dev/ttySAC%d", nPortno);
    if (nPortno == FIRST_SERIAL_NUM) {
        ret = zkSerialOpen(nPortno, dev, BAUDRATE_TTYS1);
        if(ret == 0)
        {
            qDebug("zkSerialOpen %s ok", dev);
        }
        else
        {
            qDebug("zkSerialOpen %s error", dev);
            return -1;
        }
    }
    else if (nPortno == SECOND_SERIAL_NUM) {
        ret = zkSerialOpen(nPortno, dev, BAUDRATE_TTYS2);
        if(ret == 0)
        {
            qDebug("zkSerialOpen %s ok", dev);
        }
        else
        {
            qDebug("zkSerialOpen %s error", dev);
            return -1;
        }

    }
#endif
    return 1;
}

int ZkSerialPort::comClose(int portno)
{
    return zkSerialClose(portno);

}

int ZkSerialPort::com1Flush()
{
#ifdef Q_OS_UNIX
    zkSerialFlushRead(struGsh.serialNum);
    return 0;
#endif
}

int ZkSerialPort::com2Flush()
{
#ifdef Q_OS_UNIX
    zkSerialFlushRead(struGsh.serialNum_2);
    return 0;
#endif
}

int ZkSerialPort::com1Read(char *sBuf, int nLen)
{
    int ret = 0;
    int i;
    int nDispLen = 0;

    nDispLen = (nLen > 75) ? 75 : nLen;
    memset(sBuf, 0, nLen);

#ifdef Q_OS_UNIX
    ret = zkSerialRead(struGsh.serialNum, sBuf, nLen, TIMEOUT);
#endif

//    if(struGsh.debugMode)
//    {
//        if (ret > 0)
//        {
//            printf("com2 read >> %d\n",ret);
//            for (i = 0; i < nDispLen; i++) {
//                printf("%02x ", sBuf[i]&0xFF);
//            }
//        }
//    }
    return ret;
}


int ZkSerialPort::com2Read(char *sBuf, int nLen)
{
    int ret = 0;
    int i;
    int nDispLen = 0;

    nDispLen = (nLen > 75) ? 75 : nLen;
    memset(sBuf, 0, nLen);

#ifdef Q_OS_UNIX
    ret = zkSerialRead(struGsh.serialNum_2, sBuf, nLen, TIMEOUT);
#endif
    return ret;
}


/*************************************************************************************************************
 * 新加的消息队列收发
 * ************************************************************************************************************/
void ZkSerialPort::pushCom1CmdData(int nCmd, char intAddr, char boardType,  char boardAddr, char *data, int nCount)
{
    char sPacket[SEND_PACKET_SIZE+7];
    char sCrcData[20];
    unsigned int nCrcItt=0;
    int i, ret = 0;

    memset(sPacket, 0, 25);

    sPacket[0]  = 0x53;
    sPacket[1]  = 0xb9;
//    sPacket[2]   = (intAddr<<6)|(boardType%64);    // 接口板地址2bit|电路板型号6bit
    sPacket[2]   = boardType%64;    // 接口板地址2bit|电路板型号6bit，目前只支持一个接口板
    sPacket[3]   = (boardAddr == BOARD_TYPE_ALL_IN_CHAIN) ? 0 : boardAddr+1;                       // 电路板地址
    sPacket[4]   = nCmd/256;
    sPacket[5]   = nCmd%256;

    for(i = 0; i < 16; i++)
    {
        sPacket[6+i]   = data[i];
    }

    for (i = 0; i < 20; i++) {
        sCrcData[i] = sPacket[i+2];
    }

    nCrcItt = crcFpga(sCrcData, 20);

    sPacket[22] = nCrcItt/256;
    sPacket[23] = nCrcItt%256;
    sPacket[24] = 0xff;

    char queData[SEND_PACKET_SIZE];
    memcpy(queData, sPacket, SEND_PACKET_SIZE);
    myQueue.push(queData);

    myFlow.msleep(1);//等待5秒
}


void ZkSerialPort::pushCom2CmdData(int nCmd, char intAddr, char boardType,  char boardAddr, char *data, int nCount)
{
    char sPacket[SEND_PACKET_RECTRL_BOARD_SIZE+7];
    char sCrcData[68];
    unsigned int nCrcItt=0;
    int i, ret = 0;

    memset(sPacket, 0, 73);

    sPacket[0]  = 0x53;
    sPacket[1]  = 0xb9;
//    sPacket[2]   = (intAddr<<6)|(boardType%64);    // 接口板地址2bit|电路板型号6bit
    sPacket[2]   = boardType%64;    // 接口板地址2bit|电路板型号6bit，目前只支持一个接口板
    sPacket[3]   = (boardAddr == BOARD_TYPE_ALL_IN_CHAIN) ? 0 : boardAddr+1;                       // 电路板地址
    sPacket[4]   = nCmd/256;
    sPacket[5]   = nCmd%256;

    for(i = 0; i < 64; i++)
    {
        sPacket[6+i]   = data[i];
    }

    for (i = 0; i < 68; i++) {
        sCrcData[i] = sPacket[i+2];
    }

    nCrcItt = crcFpga(sCrcData, 68);

    sPacket[70] = nCrcItt/256;
    sPacket[71] = nCrcItt%256;
    sPacket[72] = 0xff;

    char queData[SEND_PACKET_RECTRL_BOARD_SIZE];
    memcpy(queData, sPacket, SEND_PACKET_RECTRL_BOARD_SIZE);

    myQueue2.push(queData);
  //  myUdpQueue8083.push(queData);

    myFlow.msleep(1);//等待5秒
}


int ZkSerialPort::isCmdNeedResp(int nCmd)
{
    switch (nCmd)
    {
//    case CMD_CAMERA_CALIBRATE_BRIGHT:
//    case CMD_CAMERA_CALIBRATE_DARK:
//        return CMD_RSP_CALIBRATE;
    case CMD_INT_ENCRYPT_PARAMS_REQUIRE:
    case CMD_CAMERA_STATICS_CAPTURE:
    case CMD_CAMERA_MISC_CAPTURE:
    case CMD_CTRL_ALARM_CAPTURE:
    case CMD_CTRL_NEW_ALARM_CAPTURE:
//    case CMD_CTRL_AIR_CAPTURE:
    case CMD_ALL_VERSION_REQUIRE:
    case CMD_CAMERA_STATUS_CAPTURE:
    case CMD_CAMERA_GET_CALIINFO:
        return CMD_RSP_BY_SERIAL;
    case CMD_CAMERA_WAVE_CAPTURE:
    case CMD_CAMERA_IMAGE_CAPTURE_SYNC:
        return CMD_RSP_WAVE;
    default:
        return CMD_NO_RSP;
    }
}

int ZkSerialPort::sendCom1Cmd(char * data)
{
    int nCmd = data[4]*256+data[5];
    int cmdType = isCmdNeedResp(nCmd);
    int sendTimes = (cmdType == CMD_NO_RSP ? 3 : 1);

    int ret = 0;

    struct timezone tz;
    struct timeval tvafter,tvpre;
    gettimeofday (&tvpre , &tz);
    int useMsecTime = 0;

#ifdef Q_OS_UNIX
    QMutexLocker lock(&m_sendLock);

    for (int i = 0; i < sendTimes; i++) {
        ret = zkSerialWrite(struGsh.serialNum, data, SEND_PACKET_SIZE);
    }
#endif
    gettimeofday (&tvafter , &tz);
    useMsecTime=(tvafter.tv_sec-tvpre.tv_sec)*1000+(tvafter.tv_usec-tvpre.tv_usec)/1000;
//    qDebug("send use time:%d\n", (tvafter.tv_sec-tvpre.tv_sec)*1000+(tvafter.tv_usec-tvpre.tv_usec)/1000);

    if(struGsh.debugMode)
    {
        if((data[4]*256+data[5] != CMD_CAMERA_WAVE_CAPTURE)
                && (data[4]*256+data[5] != CMD_CAMERA_STATICS_CAPTURE)
                && (data[4]*256+data[5] != CMD_CTRL_ALARM_CAPTURE)
                && (data[4]*256+data[5] != CMD_CTRL_NEW_ALARM_CAPTURE)
                && (data[4]*256+data[5] != CMD_CTRL_ALARM_SET))
        {
            QString tmpStr;
            tmpStr.sprintf("sc:%d, use:%d ms, head:[%02x %02x] dst:[%02x %02x] cmd:[%02x %02x]--data: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x crc-%02x %02x %02x"
                           ,testSendTimes,useMsecTime, data[0]&0xFF, data[1]&0xFF, data[2]&0xFF, data[3]&0xFF,data[4]&0xFF, data[5]&0xFF, data[6]&0xFF, data[7]&0xFF
                    ,data[8]&0xFF, data[9]&0xFF, data[10]&0xFF, data[11]&0xFF,data[12]&0xFF, data[13]&0xFF, data[14]&0xFF, data[15]&0xFF
                    ,data[16]&0xFF, data[17]&0xFF, data[18]&0xFF, data[19]&0xFF, data[20]&0xFF, data[21]&0xFF, data[22]&0xFF, data[23]&0xFF
                    ,data[24]&0xFF);
            qDebug(qPrintable(tmpStr));
        }
        testSendTimes++;
    }

    if (cmdType == CMD_RSP_BY_SERIAL)
    {
        lastResCmdId = nCmd;
        for(int i = 0; i < 30; i++)    //最多超时200ms
        {
            myFlow.msleep(15);
            if(hasRecvLastCmd == true)
            {
                break;
            }
        }
        hasRecvLastCmd = false;
    }
    else if (cmdType == CMD_RSP_CALIBRATE)
    {
        lastResCmdId = nCmd;
        for(int i = 0; i < 50; i++)    //校正最多超时5秒
        {
            myFlow.msleep(200);
            if(hasRecvLastCmd == true)
            {
                break;
            }
        }
        hasRecvLastCmd = false;
    }

    return ret;
}
int ZkSerialPort::sendCom2Cmd(char * data)
{
    int nCmd = data[4]*256+data[5];
    int cmdType = isCmdNeedResp(nCmd);
    int sendTimes = (cmdType == CMD_NO_RSP ? 3 : 1);

    int ret = 0;

    struct timezone tz;
    struct timeval tvafter,tvpre;
    gettimeofday (&tvpre , &tz);
    int useMsecTime = 0;

#ifdef Q_OS_UNIX
    QMutexLocker lock(&m_sendLock_2);

    for (int i = 0; i < sendTimes; i++) {
        ret = zkSerialWrite(struGsh.serialNum_2, data, SEND_PACKET_RECTRL_BOARD_SIZE);
    }
#endif
    gettimeofday (&tvafter , &tz);
    useMsecTime=(tvafter.tv_sec-tvpre.tv_sec)*1000+(tvafter.tv_usec-tvpre.tv_usec)/1000;
//    qDebug("send use time:%d\n", (tvafter.tv_sec-tvpre.tv_sec)*1000+(tvafter.tv_usec-tvpre.tv_usec)/1000);

    if(struGsh.debugMode)
    {
        if(data[4]*256+data[5] != CMD_UPPER_COMPUTER_STATUS_RET_PKG)
        {
            QString tmpStr;
            tmpStr.sprintf("sc:%d, use:%d ms, head:[%02x %02x] dst:[%02x %02x] cmd:[%02x %02x]--data: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x crc-%02x %02x %02x"
                           ,testSendTimes,useMsecTime, data[0]&0xFF, data[1]&0xFF, data[2]&0xFF, data[3]&0xFF,data[4]&0xFF, data[5]&0xFF, data[6]&0xFF, data[7]&0xFF
                    ,data[8]&0xFF, data[9]&0xFF, data[10]&0xFF, data[11]&0xFF,data[12]&0xFF, data[13]&0xFF, data[14]&0xFF, data[15]&0xFF
                    ,data[16]&0xFF, data[17]&0xFF, data[18]&0xFF, data[19]&0xFF, data[20]&0xFF, data[21]&0xFF, data[22]&0xFF, data[23]&0xFF
                    ,data[24]&0xFF);
            qDebug(qPrintable(tmpStr));
        }
        testSendTimes++;
    }



    if (cmdType == CMD_RSP_BY_SERIAL)
    {
        lastResCmdId2 = nCmd;
        for(int i = 0; i < 30; i++)    //最多超时200ms
        {
            myFlow.msleep(15);
            if(hasRecvLastCmd2 == true)
            {
                break;
            }
        }
        hasRecvLastCmd2 = false;
    }
    else if (cmdType == CMD_RSP_CALIBRATE)
    {
        lastResCmdId2 = nCmd;
        for(int i = 0; i < 50; i++)    //校正最多超时5秒
        {
            myFlow.msleep(200);
            if(hasRecvLastCmd2 == true)
            {
                break;
            }
        }
        hasRecvLastCmd2 = false;
    }

    return ret;
}
void ZkSerialPort::com1WriteDirect(int nCmd, char intAddr, char boardType,  char boardAddr, char *data, int nCount)
{
    char sPacket[SEND_PACKET_SIZE+7];
    char sCrcData[20];
    unsigned int nCrcItt=0;
    int i, ret = 0;

    memset(sPacket, 0, 25);

    sPacket[0]  = 0x53;
    sPacket[1]  = 0xb9;
//    sPacket[2]   = (intAddr<<6)|(boardType%64);    // 接口板地址2bit|电路板型号6bit
    sPacket[2]   = boardType%64;    // 接口板地址2bit|电路板型号6bit，目前只支持一个接口板
    sPacket[3]   = (boardAddr == BOARD_TYPE_ALL_IN_CHAIN) ? 0 : boardAddr+1;                       // 电路板地址
    sPacket[4]   = nCmd/256;
    sPacket[5]   = nCmd%256;

    for(i = 0; i < 16; i++)
    {
        sPacket[6+i]   = data[i];
    }

    for (i = 0; i < 20; i++) {
        sCrcData[i] = sPacket[i+2];
    }

    nCrcItt = crcFpga(sCrcData, 20);

    sPacket[22] = nCrcItt/256;
    sPacket[23] = nCrcItt%256;
    sPacket[24] = 0xff;

    struct timezone tz;
    struct timeval tvafter,tvpre;
    gettimeofday (&tvpre , &tz);
    int useMsecTime = 0;

#ifdef Q_OS_UNIX
    QMutexLocker lock(&m_sendLock);

    for (int i = 0; i < nCount; i++) {
        ret = zkSerialWrite(struGsh.serialNum, sPacket, SEND_PACKET_SIZE);
    }
#endif
    gettimeofday (&tvafter , &tz);
    useMsecTime=(tvafter.tv_sec-tvpre.tv_sec)*1000+(tvafter.tv_usec-tvpre.tv_usec)/1000;
//    qDebug("send use time:%d\n", (tvafter.tv_sec-tvpre.tv_sec)*1000+(tvafter.tv_usec-tvpre.tv_usec)/1000);

    if (struGsh.debugMode)
    {
        QString tmpStr;
        tmpStr.sprintf("sc:%d, use:%d ms, head:[%02x %02x] dst:[%02x %02x] cmd:[%02x %02x]--data: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x crc-%02x %02x %02x"
                       ,testSendTimes,useMsecTime, sPacket[0]&0xFF, sPacket[1]&0xFF, sPacket[2]&0xFF, sPacket[3]&0xFF,sPacket[4]&0xFF, sPacket[5]&0xFF, sPacket[6]&0xFF, sPacket[7]&0xFF
                ,sPacket[8]&0xFF, sPacket[9]&0xFF, sPacket[10]&0xFF, sPacket[11]&0xFF,sPacket[12]&0xFF, sPacket[13]&0xFF, sPacket[14]&0xFF, sPacket[15]&0xFF
                ,sPacket[16]&0xFF, sPacket[17]&0xFF, sPacket[18]&0xFF, sPacket[19]&0xFF, sPacket[20]&0xFF, sPacket[21]&0xFF, sPacket[22]&0xFF, sPacket[23]&0xFF
                ,sPacket[24]&0xFF);
        qDebug(qPrintable(tmpStr));

        testSendTimes++;
    }
}


void ZkSerialPort::com2WriteDirect(int nCmd, char intAddr, char boardType,  char boardAddr, char *data, int nCount)
{
    char sPacket[SEND_PACKET_RECTRL_BOARD_SIZE+7];

    int ret = 0;

    memset(sPacket, 0, 73);

    sPacket[0]  = 0x53;
    sPacket[1]  = 0xb9;
//    sPacket[2]   = (intAddr<<6)|(boardType%64);    // 接口板地址2bit|电路板型号6bit
    sPacket[2]   = boardType%64;    // 接口板地址2bit|电路板型号6bit，目前只支持一个接口板
    sPacket[3]   = (boardAddr == BOARD_TYPE_ALL_IN_CHAIN) ? 0 : boardAddr+1;                       // 电路板地址
    sPacket[4]   = nCmd/256;
    sPacket[5]   = nCmd%256;
    sPacket[6]   = data[0];

    sPacket[72] = 0xff;

    struct timezone tz;
    struct timeval tvafter,tvpre;
    gettimeofday (&tvpre , &tz);
    int useMsecTime = 0;

#ifdef Q_OS_UNIX
    QMutexLocker lock(&m_sendLock);

    for (int i = 0; i < nCount; i++) {
        ret = zkSerialWrite(struGsh.serialNum_2, sPacket, SEND_PACKET_RECTRL_BOARD_SIZE);
    }
#endif
    gettimeofday (&tvafter , &tz);
    useMsecTime=(tvafter.tv_sec-tvpre.tv_sec)*1000+(tvafter.tv_usec-tvpre.tv_usec)/1000;
//    qDebug("send use time:%d\n", (tvafter.tv_sec-tvpre.tv_sec)*1000+(tvafter.tv_usec-tvpre.tv_usec)/1000);

    if (!struGsh.debugMode)
    {
        QString tmpStr;
        tmpStr.sprintf("count:%d ,sc:%d, use:%d ms, head:[%02x %02x] dst:[%02x %02x] cmd:[%02x %02x]--data: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x crc-%02x %02x %02x"
                       ,ret,testSendTimes,useMsecTime, sPacket[0]&0xFF, sPacket[1]&0xFF, sPacket[2]&0xFF, sPacket[3]&0xFF,sPacket[4]&0xFF, sPacket[5]&0xFF, sPacket[6]&0xFF, sPacket[7]&0xFF
                ,sPacket[8]&0xFF, sPacket[9]&0xFF, sPacket[10]&0xFF, sPacket[11]&0xFF,sPacket[12]&0xFF, sPacket[13]&0xFF, sPacket[14]&0xFF, sPacket[15]&0xFF
                ,sPacket[16]&0xFF, sPacket[17]&0xFF, sPacket[18]&0xFF, sPacket[19]&0xFF, sPacket[20]&0xFF, sPacket[21]&0xFF, sPacket[22]&0xFF, sPacket[23]&0xFF
                ,sPacket[24]&0xFF);
        qDebug(qPrintable(tmpStr));

        testSendTimes++;
    }
}

#endif
/* ****************************************************************************
 *
 *                        发送串口线程
 *
 *******************************************************************************/
mySerialSendThread::mySerialSendThread()
{

}

void mySerialSendThread::run(void)
{

    int i  = 0;
    char queData[SEND_PACKET_SIZE];

    while(this->isRunning()) {
        while (!myQueue.isEmpty())
        {
            myQueue.pop(queData);
            mySerial.sendCom1Cmd(queData);
        }
        msleep(5);
    }
    exec();
}



mySerial2SendThread::mySerial2SendThread()
{

}

void mySerial2SendThread::run(void)
{

    int i  = 0;
    char queData[SEND_PACKET_RECTRL_BOARD_SIZE];

    while(this->isRunning()) {
        while (!myQueue2.isEmpty())
        {
            myQueue2.pop(queData);
            mySerial.sendCom2Cmd(queData);
        }
        msleep(5);
    }
    exec();
}



/* ****************************************************************************
 *
 *                        接收串口线程
 *
 *******************************************************************************/
mySerialRecvThread::mySerialRecvThread()
{
    wYear = 0;
    wMonth = 0;
    wDay = 0;
}

int mySerialRecvThread::getRecvPacketLength(uchar cmd_h, uchar cmd_l)
{
    int cmd = (cmd_h*256+cmd_l)&0x7fff;
    int ret = 73;
    switch (cmd) {
    case CMD_CTRL_VI_DETECT_BOARD_VERSION_QUERY:
        ret = 201;//192+9;
        break;
    case CMD_CTRL_NEW_ALARM_CAPTURE:
        ret = 265;//256+9;
        break;
    default:
        ret = 73;//64+9;
        break;
    }
    return ret;

}
int mySerialRecvThread::getRecvPacketTimeOut(uchar cmd_h, uchar cmd_l)
{
    if(getRecvPacketLength(cmd_h,cmd_l ) > 73)
        return 40;
    else
        return 20;
}

void mySerialRecvThread::run(void)
{
    int i, ret1 = 0, ret2 = 0, ret3 = 0, readTotal= 0;
    char recvBuf[1024];
    int nTmpPacketLength = 0;
    int waitTime = 0;
    unsigned int nCrcItt=0;
    int tmpNoPacketCount = 0;

    while(this->isRunning()) {
        memset(recvBuf, 0, 1024);

        ret1 = 0;
        ret2 = 0;
        ret3 = 0;

        readTotal = 0;
        ret1 = mySerial.com1Read(recvBuf, 6);   //通过包头，区分不同串口长度
        if(ret1 <= 0)
        {
            msleep(10);
            if((struGsh.bLostFpgaBoard == 1)
                    &&(struCnfg.nHasUps == 1)
                    &&(struGsh.nVersionCaptureIsRun == 0)
                    &&(struGsh.nVavleTestIsRun == 0)
                    &&(struGsh.nImageCaptureIsRun == 0)
                    &&(struGsh.nRemoteImageCaptureIsRun == 0))
            {
               tmpNoPacketCount++;
               if(tmpNoPacketCount >= 400) //210ms*400
               {
                   struGsh.bLostFpgaBoard = 2;
               }
            }
            continue;
        }
        else if(ret1 < 6)
        {
            tmpNoPacketCount = 0;
            if(recvBuf[0] != 0x53)
            {
                qDebug()<<"packed byte0 err"<<ret1;
                continue;
            }
            else
            {
                ret2 = mySerial.com1Read(recvBuf+ret1, 6-ret1);
                if(ret2 != 6-ret1)
                {
                    mySerial.com1Flush();   //清空串口缓冲区
                    qDebug("packed head err");
                    continue;
                }
            }
        }
        tmpNoPacketCount = 0;

        if((recvBuf[0] == 0x53)&&((recvBuf[1] == 0xB9)||(recvBuf[1] == 0xF0)))
        {
            nTmpPacketLength = getRecvPacketLength(recvBuf[4],recvBuf[5]);
            waitTime =  getRecvPacketTimeOut(recvBuf[4],recvBuf[5]);   //小包等待1ms, 大包等待40ms
            readTotal = 6;
            for(i = 0; i < 10; i++)  //连续读5次
            {
                myFlow.msleep(waitTime);
                ret2 = mySerial.com1Read(recvBuf+readTotal, nTmpPacketLength-readTotal);   //通过包头，区分不同串口长度
                readTotal+= ret2;
                if(readTotal >= nTmpPacketLength)
                {
                    break;
                }
                myFlow.msleep(waitTime);
            }
            if(readTotal >= nTmpPacketLength)
            {
                if(struGsh.debugMode)
                {
                    if((recvBuf[4]*256+recvBuf[5] != CMD_CAMERA_WAVE_CAPTURE)
                            && (recvBuf[4]*256+recvBuf[5] != CMD_CAMERA_STATICS_CAPTURE)
                            && (recvBuf[4]*256+recvBuf[5] != CMD_CTRL_ALARM_CAPTURE)
                            && (recvBuf[4]*256+recvBuf[5] != CMD_CTRL_NEW_ALARM_CAPTURE)
                            && (recvBuf[4]*256+recvBuf[5] != CMD_CTRL_ALARM_SET))
                    {
                        printf("com1 read >> %d\n",nTmpPacketLength);
                        for (i = 0; i < nTmpPacketLength; i++) {
                            printf("%02x ", recvBuf[i]&0xFF);
                        }
                        printf("\n");
                    }
                    //    fflush(0);
                }

//                printf("recvd %d: ",readTotal);
//                for(int i = 0 ; i < readTotal;i++)
//                {
//                    printf("%02x ",recvBuf[i]&0xff);
//                }
//                printf("\n");
                if(recvBuf[nTmpPacketLength-1] == 0xFF)
                {
                    char sCrcData[nTmpPacketLength-5];
                    nCrcItt=0;
                    for(i = 0; i < nTmpPacketLength-5; i++)
                    {
                        sCrcData[i] = recvBuf[i+2];
                    }
                    nCrcItt = mySerial.crcFpga(sCrcData, nTmpPacketLength-5);
                    nCrcItt = nCrcItt&0x0000ffff;
                    if((recvBuf[nTmpPacketLength-3] == nCrcItt/256)&&(recvBuf[nTmpPacketLength-2] == nCrcItt%256))
                       {
                        if(recvBuf[1] == 0xB9)
                        {
                            recvDataAnalysis(recvBuf, Recv_Packet_Type_64);
                        }
                        else
                        {
                            recvDataAnalysis(recvBuf, Recv_Packet_Type_1024);
                        }
                    }
                    else
                    {
                        qDebug("222 CRC error");
                    }
                }
                else
                {
                    qDebug("222 packed end is not 0xff");
                }
            }
            else
            {
                qDebug("222 packed length is not correct");
            }
        }
        else
        {
            mySerial.com1Flush();   //清空串口缓冲区
            qDebug("222 packed 1&2 is wrong");
        }
    }
    exec();
}

void mySerialRecvThread::recvDataAnalysis(char *sBuf, int nType)
{
    int  i = 0, j, k;
    int packetCount = 0, tmpIndex = 0, offset;    //包数量
    int layer = 0, chainType = 0, view = 0,unit = 0;
    int dataByte = 6;  //回包数据内容起始字节
    int tmpAlarmStatus = 0; //临时存储报警状态
    int tmpAlarmWipe = 0;   //吹气次数需要自动清灰
    int staticsPeriodSt = 6, throughOutSt = 8, totalArithSt=12, greyASt=15,greyBSt=18,greyCSt=21,discolorASt=24,discolorBSt=27,discolorCSt=30,svmASt=33,svmBSt=36,svmCSt=39,shapeSt=42;
    int nCmd;
    int tmpAlarm = 0; // 用于温度报警、料位报警灯中间值
    int tmpPressureAlarm = 0;
    int tmpLevelAlarm[CTRL_ALARM_MAX_FEED_SENSOR] = {0};
    int tmpTemplAlarm[CTRL_ALARM_MAX_TEMP_SENSOR] = {0};
    int accelerationAlarm[MAX_ACCELERATION]={0};
    int accelerationAlarm2[MAX_ACCELERATION]={0};

    char tmpDccryptCode[30], tmpDccryptTime[10];
    unsigned int nCrcItt=0;

    bool noEncrtpt = true;
    bool dccryptBurnUse = true;
    unsigned short *pYear, *pMonth, *pDay;
    char sOnlyId[32];
    int tmp = 0;

//    pYear = wYear; pMonth = wMonth; pDay=wDay;

    chainType = sBuf[2]&0x3f;       //板级
    unit = (sBuf[3]&0xff)-1;               //板号
    if(unit > MAX_VAVLE_BOARD) unit = (sBuf[3]&0x0f)-1; //防呆

    switch(struCnfg.nLayerTotal)
    {
    case 1:
        layer = 0;
        break;
    case 2:
        if(((chainType >= BOARD_CAMERA_1_FRONT_MAIN)&&(chainType <= BOARD_CAMERA_1_REAR_VICE))
                ||(chainType == BOARD_VAVLE_LAYER_1))
        {
            layer = 0;
        }
        else if(((chainType >= BOARD_CAMERA_2_FRONT_MAIN)&&(chainType <= BOARD_CAMERA_2_REAR_MAIN))
            ||(chainType == BOARD_VAVLE_LAYER_2))
        {
            layer = 1;
        }
        else if(chainType == BOARD_TYPE_CONTROL) //控制板默认1个1层
        {
            layer = unit;
        }
        else if(chainType == BOARD_TYPE_LAMP)  //默认灯光板小于4认为是一层，此时需要定义一层的灯光板数量
        {
            layer = 0;
        }
        else if(chainType == BOARD_TYPE_INTERFACE) //默认只有1个接口板
        {
            layer = 0;
        }

        break;
    case 3:
    case 4:
    case 5:
    case 6:
        break;
    }

    if(nType == Recv_Packet_Type_64)   //普通包
    {
        nCmd = (sBuf[4]<<8)+sBuf[5];

        if(nCmd == lastResCmdId)
        {
            hasRecvLastCmd = true;
        }
        switch(nCmd)    //回包命令编码
        {
        case CMD_INT_ENCRYPT_PARAMS_REQUIRE:
            memset(tmpDccryptCode, 0, 30);
            memset(tmpDccryptTime, 0, 10);
            myFlow.interfaceEncryptAns = 1;

            for(i = 0; i < 27; i++) // 0~26加密码
            {
                if(sBuf[dataByte+i] != 0x01)
                {
                    dccryptBurnUse = false;
                    break;
                }
            }
            if(dccryptBurnUse == true)
            {
                struGsh.struDccryptData.bDccryptGetError = ENCRYPT_FPGA_BURNING; //生产烧录程序使用
                break;
            }
            for(i = 0; i < 27; i++) // 0~26加密码
            {
                if((sBuf[dataByte+i] != 0xff)&&(sBuf[dataByte+i] != 0x00))
                {
                    noEncrtpt = false;
                    break;
                }
            }
            if(noEncrtpt == true)
            {
                struGsh.struDccryptData.bDccryptGetError = ENCRYPT_FPGA_NO_ENCRYPT; //未加密过的接口板
                break;
            }

            /* 1. 加密时间是否正确 */
            dataByte = 36;
            for(i = 0; i < 9; i++)
            {
                tmpDccryptTime[i] = sBuf[dataByte++];
            }

            /* 1.1 加密时间CRC是否正确 */
            nCrcItt = mySerial.crcFpga(tmpDccryptTime, 7);
            nCrcItt = nCrcItt&0x0000ffff;

            if(nCrcItt != tmpDccryptTime[7]*256+tmpDccryptTime[8])
            {
                //                qDebug("111 struGsh.struDccryptData.bDccryptGetError is %d", struGsh.struDccryptData.bDccryptGetError);
                struGsh.struDccryptData.bDccryptGetError = ENCRYPT_FPGA_CODE_FORMAT_ERROR;
                break;
            }
            for(i = 0; i < 7; i++)
            {
                struGsh.struDccryptData.nDccryptGetTime[i] = tmpDccryptTime[i]; //年高，年底、月、日、时、分、秒
            }

            /* 2. 加密密码是否正确 */
            dataByte = 6;
            for(i = 0; i < DCCRYPT_THIS_CODE_SIZE; i++)
            {
                tmpDccryptCode[i] = sBuf[dataByte++];
            }
#ifdef Q_OS_UNIX
            memset(sOnlyId, 0, 32);
            pYear = &wYear; pMonth = &wMonth; pDay=&wDay;
//            if(decrypt_error_code_format == DecryptCode((const char*)tmpDccryptCode, pYear, pMonth, pDay, sOnlyId))
//            {
//                //                qDebug("222 struGsh.struDccryptData.bDccryptGetError is %d", struGsh.struDccryptData.bDccryptGetError);
//                struGsh.struDccryptData.bDccryptGetError = ENCRYPT_FPGA_CODE_FORMAT_ERROR;   //密码格式错误，需要重新写入
//                break;
//            }
#endif
            if(paramDelayCode.enable == 0xAA)
            {
                if(strncmp(tmpDccryptCode, paramDelayCode.delay_code_eeprom, DCCRYPT_THIS_CODE_SIZE) != 0)  //密码不匹配,判断使用上位机/下位机密码
                {
                    char tmpDccryptDate[DCCRYPT_MAX_LIMIT_DATE];
                    sprintf(tmpDccryptDate, "%04d%02d%02d", wYear, wMonth, wDay);
                    if (atoi(tmpDccryptDate) >= atoi(paramDelayCode.limit_date_cur))   // 上位机加密期限更短
                    {
                        struGsh.struDccryptData.bDccryptGetError = ENCRYPT_FPGA_CODE_MISMATCH_LCD;   //密码不匹配,需要重新写入FPGA
                    }
                    else                                                               // FPGA加密期限更短
                    {
                        memset(paramDelayCode.delay_code_eeprom, 0, DCCRYPT_MAX_STR);

                        strcpy(paramDelayCode.delay_code_eeprom, tmpDccryptCode);
                        paramDelayCode.enable = 0xAA;

                        struGsh.struDccryptData.bDccryptGetError = ENCRYPT_FPGA_CODE_MISMATCH_FPGA;   //密码不匹配,需要重新写入ARM板
                    }
                    break;
                }
                struGsh.struDccryptData.bDccryptGetError = ENCRYPT_FPGA_CODE_MATCH;       //密码匹配，需要保存时间
                break;
            }
            else
            {
                /*彻底解密*/
                if(wYear == DCCRYPT_INVALID_YEAR)
                {
                    struGsh.struDccryptData.bDccryptGetError = ENCRYPT_FPGA_HAS_DCCRYPT;   //已彻底解密，不需要其他处理
                    break;
                }
                else if((wMonth > 12)||(wDay > 31))
                {
                    //                    qDebug("333 struGsh.struDccryptData.bDccryptGetError is %d", struGsh.struDccryptData.bDccryptGetError);
                    struGsh.struDccryptData.bDccryptGetError = ENCRYPT_FPGA_CODE_FORMAT_ERROR;   //时间格式错误，不需要其他处理
                    break;
                }
                else {
                    memset(paramDelayCode.delay_code_eeprom, 0, DCCRYPT_MAX_STR);

                    strcpy(paramDelayCode.delay_code_eeprom, tmpDccryptCode);
                    paramDelayCode.enable = 0xAA;
                    struGsh.struDccryptData.bDccryptGetError = ENCRYPT_FPGA_CODE_FORMAT_OK;   //密码格式正确，,需要写入ARM板
                    break;
                }
            }
            break;
        case CMD_ALL_VERSION_REQUIRE:    //版本查询
            if(((chainType >= BOARD_CAMERA_1_FRONT_MAIN) && (chainType <= BOARD_CAMERA_1_REAR_VICE))
                    || ((chainType >= BOARD_CAMERA_2_FRONT_MAIN)&&(chainType <= BOARD_CAMERA_2_REAR_MAIN))
                    || ((chainType >= BOARD_CAMERA_2_FRONT_VICE)&&(chainType <= BOARD_CAMERA_2_REAR_VICE)))  // 相机板版本
            {
                for(i = 0; i < struCnfg.struLayerInfo[layer].nViewChainTotal; i++)
                {
                    if(chainType == struCnfg.struLayerInfo[layer].nViewBoardType[i])
                    {
                        struGsh.struVer.nUnitRes[layer][i][unit] = 1;
                        struCnfg.struVer.nUnitRes[layer][i][unit] = 1;


                        struGsh.struVer.sUnit[layer][i][unit][2] = sBuf[dataByte++];    //模式代号， 0：工厂模式，其余为用户模式
                        struGsh.struVer.sUnit[layer][i][unit][0] = sBuf[dataByte++];    //大版本
                        struGsh.struVer.sUnit[layer][i][unit][1] = sBuf[dataByte++];    //小版本
                        struGsh.struVer.sUnit[layer][i][unit][4] = sBuf[dataByte++];   //定制版本

//                        if(struCnfg.struIpcCameraParams[layer][i][unit].ipcType != IPC_CAMERA_TYPE_NULL)
                        {
                            struGsh.struVer.sUnitIpc[layer][i][unit][2] = sBuf[dataByte++];    //模式代号， 0：工厂模式，其余为用户模式
                            struGsh.struVer.sUnitIpc[layer][i][unit][0] = sBuf[dataByte++];    //大版本
                            struGsh.struVer.sUnitIpc[layer][i][unit][1] = sBuf[dataByte++];    //小版本
                            struGsh.struVer.sUnitIpc[layer][i][unit][4] = sBuf[dataByte++];   //定制版本
                        }

                        struGsh.struVer.sUnitSync[layer][i][unit][0] = sBuf[dataByte++];    //小版本
                        struGsh.struVer.sUnitSync[layer][i][unit][1] = sBuf[dataByte++];   //定制版本
//                            struGsh.struVer.sUnit[layer][i][j][3] = sBuf[dataByte++];    //svn版本，不用
                    }
                }
            }
            else if(chainType == BOARD_VAVLE_LAYER_1 ||chainType == BOARD_VAVLE_LAYER_2)  // 喷阀板版本
            {
                for(i = 0; i < struCnfg.struLayerInfo[layer].nVavleChainTotal; i++)
                {
                    if(chainType == struCnfg.struLayerInfo[layer].nVavleBoardType[i])
                    {
                        struGsh.struVer.nVavleRes[layer][i][unit] = 1;
                        struGsh.struVer.sVavle[layer][i][unit][2] = sBuf[dataByte++];    //模式代号， 0：工厂模式，其余为用户模式
                        struGsh.struVer.sVavle[layer][i][unit][0] = sBuf[dataByte++];    //大版本
                        struGsh.struVer.sVavle[layer][i][unit][1] = sBuf[dataByte++];    //小版本
                        struGsh.struVer.sVavle[layer][i][unit][4] = sBuf[dataByte++];   //定制版本
                    }
                }
            }
            else if(chainType == BOARD_TYPE_CONTROL)  // 控制板版本
            {
                struGsh.struVer.nCtrlRes[unit] = 1;

                struGsh.struVer.sCtrlBoard[unit][2] = sBuf[dataByte++];    //模式代号， 0：工厂模式，其余为用户模式
                struGsh.struVer.sCtrlBoard[unit][0] = sBuf[dataByte++];    //大版本
                struGsh.struVer.sCtrlBoard[unit][1] = sBuf[dataByte++];    //小版本
                struGsh.struVer.sCtrlBoard[unit][4] = sBuf[dataByte++];   //定制版本

                if(unit == 0)
                {
                    struGsh.struVer.sAccelerationBoard[0][0] = sBuf[6+32];      //模式代号， 0：工厂模式，其余为用户模式
                    struGsh.struVer.sAccelerationBoard[0][1] = sBuf[6+33];      //大版本
                    struGsh.struVer.sAccelerationBoard[0][2] = sBuf[6+34];      //小版本
                    struGsh.struVer.sAccelerationBoard[0][3] = sBuf[6+35];      //定制版本

                    if(struGsh.struVer.sAccelerationBoard[0][1] != 0)
                    {
                        struGsh.struVer.nAccelerationRes[0] = 1;
                    }
                }
                else
                {
                    struGsh.struVer.sAccelerationBoard[1][0] = sBuf[6+32];      //模式代号， 0：工厂模式，其余为用户模式
                    struGsh.struVer.sAccelerationBoard[1][1] = sBuf[6+33];      //大版本
                    struGsh.struVer.sAccelerationBoard[1][2] = sBuf[6+34];      //小版本
                    struGsh.struVer.sAccelerationBoard[1][3] = sBuf[6+35];      //定制版本

                    if(struGsh.struVer.sAccelerationBoard[1][1] != 0)
                    {
                        struGsh.struVer.nAccelerationRes[1] = 1;
                    }
                }
            }
            else if(chainType == BOARD_TYPE_LAMP)  // 灯光板版本
            {
                struGsh.struVer.nLampRes[unit] = 1;

                struGsh.struVer.sLampBoard[unit][2] = sBuf[dataByte++];    //模式代号， 0：工厂模式，其余为用户模式
                struGsh.struVer.sLampBoard[unit][0] = sBuf[dataByte++];    //大版本
                struGsh.struVer.sLampBoard[unit][1] = sBuf[dataByte++];    //小版本
                struGsh.struVer.sLampBoard[unit][4] = sBuf[dataByte++];   //定制版本
            }
            else if(chainType == BOARD_TYPE_INTERFACE)  // 接口板版本
            {
                struGsh.struVer.nInterfaceRes = 1;

                struGsh.struVer.sInterface[2] = sBuf[dataByte++];    //模式代号， 0：工厂模式，其余为用户模式
                struGsh.struVer.sInterface[0] = sBuf[dataByte++];    //大版本
                struGsh.struVer.sInterface[1] = sBuf[dataByte++];    //小版本
                struGsh.struVer.sInterface[4] = sBuf[dataByte++];   //定制版本
            }
            break;
        case CMD_CAMERA_STATUS_CAPTURE: //相机板状态信息查询
            for(i = 0; i < struCnfg.struLayerInfo[layer].nViewChainTotal; i++)
            {
                if(chainType == struCnfg.struLayerInfo[layer].nViewBoardType[i])
                {
                    for(j = 0; j <  struCnfg.struLayerInfo[layer].nViewUnitTotal[i]; j++)
                    {
                        struGsh.struCameraStatus[layer][i][j].nWiperPosition = sBuf[dataByte+j*4+3]&0x1;    //清灰刷位置报警
                    }
                }
            }
            break;
        case CMD_CTRL_ALARM_CAPTURE:    //控制板报警查询
            /* 气压值 */
            struGsh.struMachineStatus[layer].nPressureValue = myFlow.calPressureValue(sBuf[dataByte]*256+sBuf[dataByte+1]);
            if(struGsh.struMachineStatus[layer].nPressureValue < 0)
                struGsh.struMachineStatus[layer].nPressureValue = 0.001;

            /* 气压报警状态 */
            dataByte += 2;
            if(struCnfg.nAlarmValid[ALARM_PRESSURE] == 1)
            {
                tmpPressureAlarm = sBuf[dataByte]&0x1;
                if(tmpPressureAlarm == 1)
                {
                    struGsh.nAlarmStautsCurrent[ALARM_PRESSURE] = 1;
                }
                else
                {
                    struGsh.nAlarmStautsCurrent[ALARM_PRESSURE] = 0;
                }
            }
            else
            {
                struGsh.nAlarmStautsCurrent[ALARM_PRESSURE] = 0;
            }

            /* 料位传感器 */
            tmpAlarm = 0;
            dataByte += 1;
            for(i = 0; i < CTRL_ALARM_MAX_FEED_SENSOR; i++)
            {
                if(struCnfg.struAlarmConfig.nFeederLevelAlarmEnable[i] == 1)
                {
                    tmpLevelAlarm[i] = (sBuf[dataByte]>>i)%2;
                    tmpLevelAlarm[i] = (struCnfg.struAlarmConfig.nFeederLevelAlarmPosi[i] == 1) ? 1-tmpLevelAlarm[i] : tmpLevelAlarm[i];
                    if(tmpLevelAlarm[i] == 1)
                    {
                        tmpAlarm = 1;
                        struGsh.struMachineStatus[layer].bAlarmFeedLevel[i] = 1;
                    }
                    else
                    {
                        struGsh.struMachineStatus[layer].bAlarmFeedLevel[i] = 0;
                    }
                }
                else
                {
                    struGsh.struMachineStatus[layer].bAlarmFeedLevel[i] = 0;
                }
            }
            struGsh.nAlarmStautsCurrent[ALARM_FEED_LEVEL] = tmpAlarm & struCnfg.nAlarmValid[ALARM_FEED_LEVEL];

            /* 料位开关 模式1，使能料位1和料位2， 料位1底部，料位2顶端*/
            /*  1正常，2异常，料满；     1异常，2正常，料空；          1正常，2正常，均衡*/
            if((struCnfg.stuFeedAutoCtrl.isHasRadar == 2)&&(struCnfg.stuFeedAutoCtrl.autoFeedMode == AUTO_FEED_MODE_1)
                    &&(struCnfg.struAlarmConfig.nFeederLevelAlarmEnable[0] == 1) && ((struCnfg.struAlarmConfig.nFeederLevelAlarmEnable[1] == 1)))
            {
                if((struGsh.struMachineStatus[layer].bAlarmFeedLevel[0] == 0)&&(struGsh.struMachineStatus[layer].bAlarmFeedLevel[1] == 1))
                {
                    struGsh.struMachineStatus[struGsh.nLayer].nFeedLevelSensorSwitchStat = STAT_LEVEL_FULL_MODE_SWITCH;
                }
                else if((struGsh.struMachineStatus[layer].bAlarmFeedLevel[0] == 1)&&(struGsh.struMachineStatus[layer].bAlarmFeedLevel[1] == 0))
                {
                    struGsh.struMachineStatus[struGsh.nLayer].nFeedLevelSensorSwitchStat = STAT_LEVEL_EMPTY_MODE_SWITCH;
                }
                else if((struGsh.struMachineStatus[layer].bAlarmFeedLevel[0] == 0)&&(struGsh.struMachineStatus[layer].bAlarmFeedLevel[1] == 0))
                {
                    struGsh.struMachineStatus[struGsh.nLayer].nFeedLevelSensorSwitchStat = STAT_LEVEL_BALANCE_MODE_SWITCH;
                }
            }

            /* 耗气 */
            dataByte += 1;
            struGsh.struMachineStatus[layer].nMachineUseAir = sBuf[dataByte]*256+sBuf[dataByte+1];

            /* 温感 1~4 */            
            dataByte += 2;
            for(i = 1; i <= 4; i++)
            {
                if(struCnfg.struAlarmConfig.nTemptureAlarmEnable[i] == 1)
                {
                    struGsh.struMachineStatus[layer].nTempSensorValue[i] =  myFlow.calTemptureSens(0, sBuf[dataByte]*256+sBuf[dataByte+1]);
                }
                else
                {
                    struGsh.struMachineStatus[layer].nTempSensorValue[i] = 0;
                }
                dataByte += 2;
            }
            /* 控制板温感 */
            struGsh.struMachineStatus[layer].nTempSensorValue[0] = myFlow.calTemptureSens(1, sBuf[dataByte]*256+sBuf[dataByte+1]);

            tmpAlarm = 0;
            for(i = 0; i < CTRL_ALARM_MAX_TEMP_SENSOR; i++)
            {
                if(struCnfg.struAlarmConfig.nTemptureAlarmEnable[i] == 1)
                {
                    if(struGsh.struMachineStatus[layer].nTempSensorValue[i] > struCnfg.struAlarmConfig.nTemptureAlarmLimit[i])
                    {
                        tmpTemplAlarm[i] = 1;
                    }
                    else if(struGsh.struMachineStatus[layer].nTempSensorValue[i] < struCnfg.struAlarmConfig.nTemptureAlarmLimit[i]-1)
                    {
                        tmpTemplAlarm[i] = 0;
                    }
                }
                else
                {
                    tmpTemplAlarm[i] = 0;
                }
            }

            for(i = 0; i < CTRL_ALARM_MAX_TEMP_SENSOR; i++)
            {
                if(tmpTemplAlarm[i] == 1)
                {
                    tmpAlarm = 1;
                }
            }
            struGsh.nAlarmStautsCurrent[ALARM_TEMP_HIGH] = tmpAlarm;

            dataByte += 2;
            /* 雷达距离1~5 */
            for(i = 0; i < MAX_GROUP_TICK; i++)
            {
                /* 雷达测试 20220628 国产 无电阻校准*/
                struGsh.struMachineStatus[layer].nFeedLvelValueReal[i] = sBuf[dataByte]*256+sBuf[dataByte+1];
                if(struGsh.struMachineStatus[layer].nFeedLvelValueReal[i] <= 120)
                {
                    struGsh.struMachineStatus[layer].nFeedLvelValue[i] = 200;
                }
                else if(struGsh.struMachineStatus[layer].nFeedLvelValueReal[i] >= 4070)
                {
                    struGsh.struMachineStatus[layer].nFeedLvelValue[i] = 2000;
                }
                else
                {
                    struGsh.struMachineStatus[layer].nFeedLvelValue[i] = 200+100*(struGsh.struMachineStatus[layer].nFeedLvelValueReal[i]-120)/220;
                }

                dataByte += 2;
            }

            /* 声光报警控制*/
            if(((struCnfg.nAlarmValid[ALARM_PRESSURE] == 1)&&(struGsh.nAlarmStautsCurrent[ALARM_PRESSURE] == 1))
                   ||(struGsh.nAlarmStautsCurrent[ALARM_FEED_LEVEL] == 1)
                   ||(struGsh.nAlarmStautsCurrent[ALARM_TEMP_HIGH] == 1))
            {
                struGsh.nAlarmSetData0 = 1;
            }else{
                struGsh.nAlarmSetData0 = 0;
            }

            /*加速度传感器*/
            tmpAlarm = 0;
            for(i = 0; i < MAX_ACCELERATION; i++)
            {
                if(i == struCnfg.nChuteTotal) break;
                accelerationAlarm[i] = (i%2 == 0) ? ((sBuf[38+(i/2)]&0xf0)>>4) : (sBuf[38+(i/2)]&0x0f);

                if(accelerationAlarm[i] == 0)
                {
                    struGsh.struMachineStatus[layer].nAccelerationStatus[i] = 0;
                }
                else
                {
                    tmpAlarm = 1;
                    struGsh.struMachineStatus[layer].nAccelerationStatus[i] = accelerationAlarm[i];
                }
            }
            struGsh.nAlarmStautsCurrent[ALARM_ACCELERATION] = tmpAlarm;

            struGsh.bAlarmStatus = struGsh.nAlarmStautsCurrent[ALARM_PRESSURE] ;
            break;
        case CMD_CAMERA_MISC_CAPTURE:
            for(i = 0; i < struCnfg.struLayerInfo[layer].nViewChainTotal; i++)
            {
                if(chainType == struCnfg.struLayerInfo[layer].nViewBoardType[i])
                {
                    view = i;
                }
            }
            struGsh.struCameraStatus[layer][view][unit].nWiperPosition = (sBuf[dataByte]<<8)+sBuf[dataByte+1];    //清灰刷位置
            struGsh.struCameraStatus[layer][view][unit].nWiperGetWidth = (sBuf[dataByte+2]<<8)+sBuf[dataByte+3];    //清灰刷宽度
            if(struGsh.struCameraStatus[layer][view][unit].nWiperGetWidth > (struCnfg.struIntelWipe.wiperWidth*struCnfg.struIntelWipe.wiperDurationPerChute/100))
            {
                struGsh.nCurWiperInChute = unit;
            }

            dataByte = 16;  //查询通信信号状态

            if(struGsh.struCameraStatus[layer][view][unit].nFirstRequireCmd == 1)
            {
                struGsh.struCameraStatus[layer][view][unit].comunicationCode1=sBuf[dataByte+1];
                struGsh.struCameraStatus[layer][view][unit].comunicationCode2=sBuf[dataByte];
                struGsh.struCameraStatus[layer][view][unit].nFirstRequireCmd = 2;
            }
            dataByte = 18; //查询清灰速度
            struGsh.struCameraStatus[layer][view][unit].nWipeSpeed =(sBuf[dataByte]<<8)+sBuf[dataByte+1];
            struGsh.struCameraStatus[layer][view][unit].nWipeSpeed_backward = struGsh.struCameraStatus[layer][view][unit].nWipeSpeed;
            dataByte = 20;
            struGsh.struCameraStatus[layer][view][unit].nWipeSpeed_forward =(sBuf[dataByte]<<8)+sBuf[dataByte+1];
            dataByte = 22;
            struGsh.struCameraStatus[layer][view][unit].nWipeSpeed_pre =(sBuf[dataByte]<<16)+(sBuf[dataByte+1]<<8)+sBuf[dataByte+2];
            dataByte = 25;
            struGsh.struCameraStatus[layer][view][unit].nWipeSpeed_total =(sBuf[dataByte]<<16)+(sBuf[dataByte+1]<<8)+sBuf[dataByte+2];

            break;
        case CMD_CAMERA_STATICS_CAPTURE:    //统计信息查询查询
            if(struCnfg.nAutoStaticsEnable == 0)
            {
                break;  //智能流量标定大米重量时，清零之前的信息
            }

            struGsh.cornLastIndex++;
            struGsh.cornLastIndex %= 100;
            for(int i=0; i<LCD_VERI_VICE_VIEW_FRONT; i++){
                for(int j=0; j < MAX_VIEW_UNIT; j++){
                    struGsh.cornStatics[i][j][struGsh.cornLastIndex] = (sBuf[dataByte]<<8)+sBuf[dataByte+1];
                    dataByte += 2;
                    if(i == 0&& j== 2)
                        printf("view:%d,chute:%d,statics:%d\n",i,j,  struGsh.cornStatics[i][j][struGsh.cornLastIndex]);
                }
            }
            g_Runtime().updateCornStastic();
            break;
        case CMD_ACCELERATION_CAPTURE:  // 加速度传感器查询
            if(unit == 0)
            {
                for(i = 0; i < MAX_ACCELERATION; i++)
                {
                    struGsh.accelerationAmplitude[i] = (sBuf[6+i*4]<<8)+(sBuf[7+i*4]);
                    struGsh.accelerationDutyCycle[i] = (sBuf[8+i*4]);
                    struGsh.accelerationChangedDutyCycle[i] = (sBuf[9+i*4]);
                }
            }
            else if( unit == 1)
            {
                for(i = 0; i < MAX_ACCELERATION; i++)
                {
                    struGsh.accelerationAmplitude2[i] = (sBuf[6+i*4]<<8)+(sBuf[7+i*4]);
                    struGsh.accelerationDutyCycle2[i] = (sBuf[8+i*4]);
                    struGsh.accelerationChangedDutyCycle2[i] = (sBuf[9+i*4]);
                }
            }
            break;
        case CMD_CTRL_VI_DETECT_BOARD_VERSION_QUERY:
            if(chainType == BOARD_TYPE_CONTROL)  // 控制板版本
            {
                for(int i = 0; i < struCnfg.viBoardTotalCount;i++)
                {
                    struGsh.struVer.sVIDetectBoard[i][2] = sBuf[dataByte++];    //模式代号， 0：工厂模式，其余为用户模式
                    struGsh.struVer.sVIDetectBoard[i][0] = sBuf[dataByte++];    //大版本
                    struGsh.struVer.sVIDetectBoard[i][1] = sBuf[dataByte++];    //小版本
                    struGsh.struVer.sVIDetectBoard[i][4] = sBuf[dataByte++];   //定制版本
                }
            }
            break;
        case CMD_CTRL_NEW_ALARM_CAPTURE:
            struGsh.struMachineStatus[layer].nPressureValue =
                    myFlow.calPressureValue(sBuf[dataByte]*256+sBuf[dataByte+1]);
            if(struGsh.struMachineStatus[layer].nPressureValue < 0)
                struGsh.struMachineStatus[layer].nPressureValue = 0.001;

            dataByte += 2;
            struGsh.nAlarmStautsCurrent[ALARM_PRESSURE] = (!!(sBuf[dataByte]&0x1))&struCnfg.nAlarmValid[ALARM_PRESSURE];

            tmpAlarm = 0;
            dataByte += 1;
            for(i = 0; i < CTRL_ALARM_MAX_FEED_SENSOR; i++)
            {
                if(struCnfg.struAlarmConfig.nFeederLevelAlarmEnable[i] == 1)
                {
                    tmpLevelAlarm[i] = (sBuf[dataByte]>>i)%2;
                    tmpLevelAlarm[i] = (struCnfg.struAlarmConfig.nFeederLevelAlarmPosi[i] == 1) ? 1-tmpLevelAlarm[i] : tmpLevelAlarm[i];

                    if(tmpLevelAlarm[i] == 1)
                    {
                        tmpAlarm = 1;
//                        qDebug()<<"feeder"<<i<<"alarm"<<struGsh.struMachineStatus[layer].bAlarmFeedLevel[i];
                        if(struCnfg.bUseAutoFeeder)
                            struGsh.struMachineStatus[layer].bAlarmFeedLevel[i]++;
                        else
                            struGsh.struMachineStatus[layer].bAlarmFeedLevel[i] = 1;
                    }
                    else
                    {
                        struGsh.struMachineStatus[layer].bAlarmFeedLevel[i] = 0;
                    }
                }
                else
                {
                    struGsh.struMachineStatus[layer].bAlarmFeedLevel[i] = 0;
                }
            }
            struGsh.nAlarmStautsCurrent[ALARM_FEED_LEVEL] = tmpAlarm;

            /* 料位开关 模式1，使能料位1和料位2， 料位1底部，料位2顶端*/
            /*  1正常，2异常，料满；     1异常，2正常，料空；          1正常，2正常，均衡*/
            if((struCnfg.stuFeedAutoCtrl.isHasRadar == 2)&&(struCnfg.stuFeedAutoCtrl.autoFeedMode == AUTO_FEED_MODE_1)
                    &&(struCnfg.struAlarmConfig.nFeederLevelAlarmEnable[0] == 1) && ((struCnfg.struAlarmConfig.nFeederLevelAlarmEnable[1] == 1)))
            {
                if((struGsh.struMachineStatus[layer].bAlarmFeedLevel[0] == 0)&&(struGsh.struMachineStatus[layer].bAlarmFeedLevel[1] == 1))
                {
                    struGsh.struMachineStatus[struGsh.nLayer].nFeedLevelSensorSwitchStat = STAT_LEVEL_FULL_MODE_SWITCH;
                }
                else if((struGsh.struMachineStatus[layer].bAlarmFeedLevel[0] == 1)&&(struGsh.struMachineStatus[layer].bAlarmFeedLevel[1] == 0))
                {
                    struGsh.struMachineStatus[struGsh.nLayer].nFeedLevelSensorSwitchStat = STAT_LEVEL_EMPTY_MODE_SWITCH;
                }
                else if((struGsh.struMachineStatus[layer].bAlarmFeedLevel[0] == 0)&&(struGsh.struMachineStatus[layer].bAlarmFeedLevel[1] == 0))
                {
                    struGsh.struMachineStatus[struGsh.nLayer].nFeedLevelSensorSwitchStat = STAT_LEVEL_BALANCE_MODE_SWITCH;
                }
            }

            dataByte += 1;
            struGsh.struMachineStatus[layer].nMachineUseAir = sBuf[dataByte]*256+sBuf[dataByte+1];

            dataByte += 2;
            for(i = 1; i <= 4; i++)
            {
                if(struCnfg.struAlarmConfig.nTemptureAlarmEnable[i] == 1)
                {
                    struGsh.struMachineStatus[layer].nTempSensorValue[i] =  myFlow.calTemptureSens(0, sBuf[dataByte]*256+sBuf[dataByte+1]);
                }
                else
                {
                    struGsh.struMachineStatus[layer].nTempSensorValue[i] = 0;
                }
                dataByte += 2;
            }
            /* 控制板温感 */
            struGsh.struMachineStatus[layer].nTempSensorValue[0] = myFlow.calTemptureSens(1, sBuf[dataByte]*256+sBuf[dataByte+1]);

            tmpAlarm = 0;
            for(i = 0; i < CTRL_ALARM_MAX_TEMP_SENSOR; i++)
            {
                if(struCnfg.struAlarmConfig.nTemptureAlarmEnable[i] == 1)
                {
                    if(struGsh.struMachineStatus[layer].nTempSensorValue[i] > struCnfg.struAlarmConfig.nTemptureAlarmLimit[i])
                    {
                        tmpTemplAlarm[i] = 1;
                    }
                    else if(struGsh.struMachineStatus[layer].nTempSensorValue[i] < struCnfg.struAlarmConfig.nTemptureAlarmLimit[i]-1)
                    {
                        tmpTemplAlarm[i] = 0;
                    }
                }
                else
                {
                    tmpTemplAlarm[i] = 0;
                }
            }

            for(i = 0; i < CTRL_ALARM_MAX_TEMP_SENSOR; i++)
            {
                if(tmpTemplAlarm[i] == 1)
                {
                    tmpAlarm = 1;
                }
            }
            struGsh.nAlarmStautsCurrent[ALARM_TEMP_HIGH] = tmpAlarm;

            dataByte += 2;
            /* 雷达距离1~5 */
            for(i = 0; i < MAX_GROUP_TICK; i++)
            {
                /* 雷达测试 20220628 国产 无电阻校准*/
                struGsh.struMachineStatus[layer].nFeedLvelValueReal[i] = sBuf[dataByte]*256+sBuf[dataByte+1];
                if(struGsh.struMachineStatus[layer].nFeedLvelValueReal[i] <= 120)
                {
                    struGsh.struMachineStatus[layer].nFeedLvelValue[i] = 200;
                }
                else if(struGsh.struMachineStatus[layer].nFeedLvelValueReal[i] >= 4070)
                {
                    struGsh.struMachineStatus[layer].nFeedLvelValue[i] = 2000;
                }
                else
                {
                    struGsh.struMachineStatus[layer].nFeedLvelValue[i] = 200+100*(struGsh.struMachineStatus[layer].nFeedLvelValueReal[i]-120)/220;
                }

                dataByte += 2;
            }

//待合并-------------------------------------------------------------
            /* 声光报警控制*/
            if(((struCnfg.nAlarmValid[ALARM_PRESSURE] == 1)&&(struGsh.nAlarmStautsCurrent[ALARM_PRESSURE] == 1))
                   ||(struGsh.nAlarmStautsCurrent[ALARM_FEED_LEVEL] == 1)
                   ||(struGsh.nAlarmStautsCurrent[ALARM_TEMP_HIGH] == 1))
            {
                struGsh.nAlarmSetData0 = 1;
            }else{
                struGsh.nAlarmSetData0 = 0;
            }

            /*加速度传感器*/
            tmpAlarm = 0;
            for(i = 0; i < MAX_ACCELERATION; i++)
            {
                if(i == struCnfg.nChuteTotal) break;
                accelerationAlarm[i] = (i%2 == 0) ? ((sBuf[38+(i/2)]&0xf0)>>4) : (sBuf[38+(i/2)]&0x0f);

                if(accelerationAlarm[i] == 0)
                {
                    struGsh.struMachineStatus[layer].nAccelerationStatus[i] = 0;
                }
                else
                {
                    tmpAlarm = 1;
                    struGsh.struMachineStatus[layer].nAccelerationStatus[i] = accelerationAlarm[i];
                }
            }
            struGsh.nAlarmStautsCurrent[ALARM_ACCELERATION] = tmpAlarm;

            //新加功能-------------------------------------------------------------------------------------
            dataByte = 64+6;
            tmpAlarm = 0;
            for(int i = 0; i < struCnfg.viBoardTotalCount;i++)
            {
                struCnfg.viBoardInfo[i][3]   = (sBuf[dataByte]<<8)+sBuf[dataByte+1];
                dataByte +=2;
                struCnfg.viBoardInfo[i][2]   = (sBuf[dataByte]<<8)+sBuf[dataByte+1];
                dataByte +=2;

                //V-I报警处理
                if(struCnfg.viBoardAlarmEnable)
                {
                    if(struCnfg.viBoardInfo[i][1] == 0 || struCnfg.viBoardInfo[i][1] == 1)//sys、valve
                    {
                        if(struCnfg.viBoardInfo[i][2] < 22800 || struCnfg.viBoardInfo[i][2] > 25200 || struCnfg.viBoardInfo[i][3] > 25000)
                            tmpAlarm = 1;
                    }
                    if(struCnfg.viBoardInfo[i][1] == 2) //lamp
                    {
                        if(struCnfg.viBoardInfo[i][2] < 45600 || struCnfg.viBoardInfo[i][2] > 50400 || struCnfg.viBoardInfo[i][3] > 12500)
                            tmpAlarm = 1;
                    }
                }
            }
            struGsh.nAlarmStautsCurrent[ALARM_VI] = tmpAlarm;
            if(struGsh.nPageSm == SM_SYSSTATE_WIDGET){
                emit signalupdateVIdata();
            }

            struGsh.bAlarmStatus = struGsh.nAlarmStautsCurrent[ALARM_PRESSURE] | struGsh.nAlarmStautsCurrent[ALARM_VI];
            break;

        case CMD_CAMERA_GET_CALIINFO:

            if(((chainType >= BOARD_CAMERA_1_FRONT_MAIN) && (chainType <= BOARD_CAMERA_1_REAR_VICE))
                    || ((chainType >= BOARD_CAMERA_2_FRONT_MAIN)&&(chainType <= BOARD_CAMERA_2_REAR_MAIN))
                    || ((chainType >= BOARD_CAMERA_2_FRONT_VICE)&&(chainType <= BOARD_CAMERA_2_REAR_VICE)))  // 相机板版本
            {
                for(i = 0; i < struCnfg.struLayerInfo[layer].nViewChainTotal; i++)
                {
                    if(chainType == struCnfg.struLayerInfo[layer].nViewBoardType[i])
                    {
                        tmp = sBuf[dataByte++];
                        for(int j = 0; j < 6;j++)
                        {
                            for(int k = 0; k < 6; k++)
                            {
                                struGsh.struVer.sUnitCali[layer][i][unit][j][k] = sBuf[dataByte++];    //模式代号， 0：工厂模式，其余为用户模式
                            }
                            struGsh.struVer.sUnitCaliFSType[layer][i][unit][j] = sBuf[dataByte++];
                            struGsh.struVer.sUnitCaliRowFre[layer][i][unit][j] = (sBuf[dataByte]<<8) + sBuf[dataByte+1];
                            dataByte += 2;
                            struGsh.struVer.sUnitCaliTarget[layer][i][unit][j] = sBuf[dataByte++];
                        }

                        qDebug()<<"CMD_CAMERA_GET_CALIINFO ans!!!"<<"chainType"<<chainType<<"unit"<<unit<<struGsh.struVer.sUnitCaliRowFre[layer][i][unit][0]
                               <<struGsh.struVer.sUnitCaliTarget[layer][i][unit][0];
                    }
                }
            }
            break;
        default:
            break;
        }
    }
    else    //统计信息包 //只有2.5平台协议有，暂时用不到
    {
        nCmd = (sBuf[4]<<8)+sBuf[5];

        if(nCmd == lastResCmdId)
        {
            hasRecvLastCmd = true;
        }

        if(nCmd == 0x63)    //
        {
//            layer = sBuf[2]>>6;
//            chainType = sBuf[2]&0x3f;
            tmpAlarmWipe = 0;
            for(i = 0; i < struCnfg.struLayerInfo[layer].nViewChainTotal; i++)
            {
                if(chainType == struCnfg.struLayerInfo[layer].nViewBoardType[i])
                {
                    for(j = 0; j <  struCnfg.struLayerInfo[layer].nViewUnitTotal[i]; j++)
                    {
                        dataByte = 64*j+staticsPeriodSt;   //本次统计时间，2个字节
                        struGsh.struStatics[layer][i][j].nStaticsPeriod = (sBuf[dataByte]<<8)+sBuf[dataByte+1];

                        dataByte = 64*j+throughOutSt;   //产量统计，4个字节
                        struGsh.struStatics[layer][i][j].nThroughout = (sBuf[dataByte]<<24)+(sBuf[dataByte+1]<<16)+(sBuf[dataByte+2]<<8)+sBuf[dataByte+3];
                        if(struGsh.b_getFlow){
                            emit signalGetFlow();
                        }
                        dataByte = 64*j+totalArithSt;   //算法总吹气次数
                        struGsh.struStatics[layer][i][j].nTotalArithEjectTimes = (sBuf[dataByte]<<16)+(sBuf[dataByte+1]<<8)+sBuf[dataByte+2];
//                        if(struGsh.struStatics[layer][i][j].nTotalArithEjectTimes > 3000)
//                        {
//                            tmpAlarmWipe = 1;
//                        }

                        dataByte = 64*j+greyASt;   //灰度A吹气次数
                        struGsh.struStatics[layer][i][j].nGreyBadData[0] = (sBuf[dataByte]<<16)+(sBuf[dataByte+1]<<8)+sBuf[dataByte+2];

                        dataByte = 64*j+greyBSt;   //灰度B吹气次数
                        struGsh.struStatics[layer][i][j].nGreyBadData[1] = (sBuf[dataByte]<<16)+(sBuf[dataByte+1]<<8)+sBuf[dataByte+2];

                        dataByte = 64*j+greyCSt;   //灰度C吹气次数
                        struGsh.struStatics[layer][i][j].nGreyBadData[2] = (sBuf[dataByte]<<16)+(sBuf[dataByte+1]<<8)+sBuf[dataByte+2];

                        dataByte = 64*j+discolorASt;   //色差A吹气次数
                        struGsh.struStatics[layer][i][j].nDiscoloryBadData[0] = (sBuf[dataByte]<<16)+(sBuf[dataByte+1]<<8)+sBuf[dataByte+2];

                        dataByte = 64*j+discolorBSt;   //色差B吹气次数
                        struGsh.struStatics[layer][i][j].nDiscoloryBadData[1] = (sBuf[dataByte]<<16)+(sBuf[dataByte+1]<<8)+sBuf[dataByte+2];

                        dataByte = 64*j+discolorCSt;   //色差C吹气次数
                        struGsh.struStatics[layer][i][j].nDiscoloryBadData[2] = (sBuf[dataByte]<<16)+(sBuf[dataByte+1]<<8)+sBuf[dataByte+2];

                        dataByte = 64*j+svmASt;   //智能A吹气次数
                        struGsh.struStatics[layer][i][j].nSvmData[0] = (sBuf[dataByte]<<16)+(sBuf[dataByte+1]<<8)+sBuf[dataByte+2];

                        dataByte = 64*j+svmBSt;   //智能B吹气次数
                        struGsh.struStatics[layer][i][j].nSvmData[1] = (sBuf[dataByte]<<16)+(sBuf[dataByte+1]<<8)+sBuf[dataByte+2];

                        dataByte = 64*j+svmBSt;   //智能C吹气次数
                        struGsh.struStatics[layer][i][j].nSvmData[2] = (sBuf[dataByte]<<16)+(sBuf[dataByte+1]<<8)+sBuf[dataByte+2];

                        dataByte = 64*j+shapeSt;   //形选吹气次数
                        struGsh.struStatics[layer][i][j].nShapeData = (sBuf[dataByte]<<16)+(sBuf[dataByte+1]<<8)+sBuf[dataByte+2];
                    }
                }
            }
            struGsh.bAlarmNeedWipe = tmpAlarmWipe;
        }
    }
}




mySerial2RecvThread::mySerial2RecvThread()
{
    wYear = 0;
    wMonth = 0;
    wDay = 0;
}

void mySerial2RecvThread::run(void)
{
    int i, ret1 = 0, ret2 = 0, ret3 = 0, readTotal= 0;
    char recvBuf[READ_PACKET_LENGTH_1024];
    int nTmpPacketLength = 0;
    int waitTime = 0;
    unsigned int nCrcItt=0;
    int tmpNoPacketCount = 0;

    while(this->isRunning()) {
        memset(recvBuf, 0, READ_PACKET_LENGTH_1024);

        ret1 = 0;
        ret2 = 0;
        ret3 = 0;

        readTotal = 0;
        ret1 = mySerial.com2Read(recvBuf, 2);   //通过包头，区分不同串口长度
        if(ret1 <= 0)
        {
            msleep(10);
            if((struGsh.bLostFpgaBoard == 1)
                    &&(struCnfg.nHasUps == 1)
                    &&(struGsh.nVersionCaptureIsRun == 0)
                    &&(struGsh.nVavleTestIsRun == 0)
                    &&(struGsh.nImageCaptureIsRun == 0))
            {
               tmpNoPacketCount++;
               if(tmpNoPacketCount >= 400) //210ms*400
               {
                   struGsh.bLostFpgaBoard = 2;
               }
            }
            continue;
        }
        else if(ret1 == 1)
        {
            tmpNoPacketCount = 0;
            if(recvBuf[0] != 0x53)
            {
                qDebug("packed 1 is a data");
                continue;
            }
            else
            {
                ret2 = mySerial.com2Read(recvBuf+1, 1);   //通过包头，区分不同串口长度
                if(ret2 == 1)
                {
                    if((recvBuf[1] == 0xB9)||(recvBuf[1] == 0xF0))  // B9:64字节长度的普通包， BA：512字节的长包，用于统计信息
                    {
                        nTmpPacketLength = (recvBuf[1] == 0xB9) ? READ_PACKET_LENGTH_64 : READ_PACKET_LENGTH_1024;
                        waitTime = (recvBuf[1] == 0xB9) ? 20 : 40;   //小包等待1ms,大包等待40ms

                        readTotal = 2;
                        myFlow.msleep(waitTime);
                        for(i = 0; i < 10; i++)  //连续读5次
                        {
                            ret2 = mySerial.com2Read(recvBuf+readTotal, nTmpPacketLength-readTotal);   //通过包头，区分不同串口长度
                            readTotal+= ret2;
                            if(readTotal >= nTmpPacketLength)
                            {
                                break;
                            }
                            myFlow.msleep(waitTime);
                        }
                        if(readTotal >= nTmpPacketLength)
                        {
                            if(recvBuf[nTmpPacketLength-1] == 0xFF)     // 判断包尾
                            {
                                char sCrcData[nTmpPacketLength-5];
                                nCrcItt=0;

                                for(i = 0; i < nTmpPacketLength-5; i++)
                                {
                                    sCrcData[i] = recvBuf[i+2];
                                }
                                nCrcItt = mySerial.crcFpga(sCrcData, nTmpPacketLength-5);
                                nCrcItt = nCrcItt&0x0000ffff;
                                if((recvBuf[nTmpPacketLength-3] == nCrcItt/256)&&(recvBuf[nTmpPacketLength-2] == nCrcItt%256))  //判断CRC
                                {
                                    if(recvBuf[1] == 0xB9)
                                    {
                                        recvDataAnalysis(recvBuf, Recv_Packet_Type_64);
                                    }
                                    else
                                    {
                                        recvDataAnalysis(recvBuf, Recv_Packet_Type_1024);
                                    }
                                }
                                else
                                {
                                    qDebug("111 CRC error");
                                }
                            }
                            else
                            {
                                qDebug("111 packed end is not 0xff");
                            }
                        }
                        else
                        {
                            qDebug("111 packed length is not correct");
                        }
                    }
                    else
                    {
                        mySerial.com1Flush();   //清空串口缓冲区
                        qDebug("111 packed 2 is not 0xB9 or 0xF0");
                    }
                }
                qDebug("111 packed 1 is a interference by packet 2");
            }
        }
        else if(ret1 == 2)
        {
            tmpNoPacketCount = 0;

            if((recvBuf[0] == 0x53)&&((recvBuf[1] == 0xB9)||(recvBuf[1] == 0xF0)))
            {
                nTmpPacketLength = (recvBuf[1] == 0xB9) ? READ_PACKET_LENGTH_64 : READ_PACKET_LENGTH_1024;
                waitTime = (recvBuf[1] == 0xB9) ? 20 : 40;   //小包等待1ms, 大包等待40ms
                readTotal = 2;
                for(i = 0; i < 10; i++)  //连续读5次
                {
                    myFlow.msleep(waitTime);
                    ret2 = mySerial.com2Read(recvBuf+readTotal, nTmpPacketLength-readTotal);   //通过包头，区分不同串口长度
                    readTotal+= ret2;
                    if(readTotal >= nTmpPacketLength)
                    {
                        break;
                    }
                    myFlow.msleep(waitTime);
                }
                if(readTotal >= nTmpPacketLength)
                {
                    if(struGsh.debugMode)
                    {
                        if((recvBuf[4]*256+recvBuf[5] != CMD_CTRL_BOARD_STATUS_QUERY_REQUEST))
                        {
                            printf("com1 read >> %d\n",nTmpPacketLength);
                            for (i = 0; i < nTmpPacketLength; i++) {
                                printf("%02x ", recvBuf[i]&0xFF);
                            }
                            printf("\n");
                        }
                    }

                    if(recvBuf[nTmpPacketLength-1] == 0xFF)
                    {
                        char sCrcData[nTmpPacketLength-5];
                        nCrcItt=0;
                        for(i = 0; i < nTmpPacketLength-5; i++)
                        {
                            sCrcData[i] = recvBuf[i+2];
                        }
                        nCrcItt = mySerial.crcFpga(sCrcData, nTmpPacketLength-5);
                        nCrcItt = nCrcItt&0x0000ffff;
                        if((recvBuf[nTmpPacketLength-3] == nCrcItt/256)&&(recvBuf[nTmpPacketLength-2] == nCrcItt%256))
                        {
                            if(recvBuf[1] == 0xB9)
                            {
                                recvDataAnalysis(recvBuf, Recv_Packet_Type_64);
                            }
                            else
                            {
                                recvDataAnalysis(recvBuf, Recv_Packet_Type_1024);
                            }
                        }
                        else
                        {
                            qDebug("222 CRC error");
                        }
                    }
                    else
                    {
                        qDebug("222 packed end is not 0xff");
                    }
                }
                else
                {
                    qDebug("222 packed length is not correct");
                }
            }
            else
            {
                mySerial.com2Flush();   //清空串口缓冲区
                qDebug("222 packed 1&2 is wrong");
            }
        }
    }
    exec();
}

void mySerial2RecvThread::recvDataAnalysis(char *sBuf, int nType)
{
    int  i = 0, j, k;
    int packetCount = 0, tmpIndex = 0, offset;    //包数量
    int layer = 0, chainType = 0, view = 0,unit = 0;
    int dataByte = 6;  //回包数据内容起始字节
    int tmpAlarmStatus = 0; //临时存储报警状态
    int tmpAlarmWipe = 0;   //吹气次数需要自动清灰
    int staticsPeriodSt = 6, throughOutSt = 8, totalArithSt=12, greyASt=15,greyBSt=18,greyCSt=21,discolorASt=24,discolorBSt=27,discolorCSt=30,svmASt=33,svmBSt=36,svmCSt=39,shapeSt=42;
    int nCmd;
    int tmpAlarm = 0; // 用于温度报警、料位报警灯中间值
    int tmpPressureAlarm = 0;
    int tmpLevelAlarm[CTRL_ALARM_MAX_FEED_SENSOR] = {0};
    int tmpTemplAlarm[CTRL_ALARM_MAX_TEMP_SENSOR] = {0};
    bool alarmCmd = false;

    char tmpDccryptCode[30], tmpDccryptTime[10];
    unsigned int nCrcItt=0;

    bool noEncrtpt = true;
    bool dccryptBurnUse = true;
    unsigned short *pYear, *pMonth, *pDay;
    char sOnlyId[32];

    chainType = sBuf[2]&0x3f;       //板级
    unit = (sBuf[3]&0xff)-1;               //板号

    if(nType == Recv_Packet_Type_64)   //普通包
    {
        nCmd = (sBuf[4]<<8)+sBuf[5];
        if(nCmd != CMD_CTRL_BOARD_STATUS_QUERY_REQUEST)
        {
            logger()->info("serial receive plc ctrl cmd:%1",nCmd);
        }

        if(nCmd == lastResCmdId2)
        {
            hasRecvLastCmd = true;
        }

        struCnfg.autoCtrlBoardAlive = 0;

        if(struCnfg.remoteCtrlIsRunning == 1)
        {
            logger()->info("plcCtrl cmd is Running, Prohibit command:%1 execution",nCmd);
            return;
        }

        switch(nCmd)    //回包命令编码
        {
        case CMD_CTRL_BOARD_OUTAGE_REQUEST: //远程控制板请求断电
            struCnfg.autoCtrlBoardAlive = 0;
            emit signalCtrlBoardOutage();
            break;
        case CMD_CTRL_BOARD_FEED_ONOFF_REQUEST:
            struCnfg.autoCtrlBoardAlive = 0;
            if(sBuf[dataByte] == 0x01)  //开下料
            {
                emit signalCtrlBoardFeedOnOffQuery(1);
            }
            if(sBuf[dataByte] == 0x00)   //关下料
            {
                emit signalCtrlBoardFeedOnOffQuery(0);
            }
            break;
        case CMD_CTRL_BOARD_STATUS_QUERY_REQUEST: //远程控制板请求查询色选机状态
            struCnfg.autoCtrlBoardAlive = 0;
            if(struCnfg.bUpdateStatus == 1)  //心跳锁
            {
                return;
            }
            if(struCnfg.nUseIpcEnable == 1)
            {
                if(struIpcShare.allIpcModelLoadOk != 2 )
                {
                    emit signalCtrlBoardStatusQuery(0);
                }
                else
                    emit signalCtrlBoardStatusQuery(1);
            }
            else
                emit signalCtrlBoardStatusQuery(1);
            break;
        case CMD_CTRL_BOARD_MACHINE_INFO_QUERY_REQUEST:
            struCnfg.autoCtrlBoardAlive = 0;
            emit signalCtrlBoardMachineInfoQuery();
            break;
        case CMD_CTRL_BOARD_IP_PORT_RET_PKG: //远程控制板和PLC IP及PORT查询回包
            struCnfg.autoCtrlBoardAlive = 0;
            struCnfg.nAutoBoardIpAddr[0] = sBuf[dataByte];
            struCnfg.nAutoBoardIpAddr[1] = sBuf[dataByte+1];
            struCnfg.nAutoBoardIpAddr[2] = sBuf[dataByte+2];
            struCnfg.nAutoBoardIpAddr[3] = sBuf[dataByte+3];

            struCnfg.nAutoBoardPort = (sBuf[dataByte+4]<<8)|sBuf[dataByte+5];

            struCnfg.nGateway[0] = sBuf[dataByte+6];                             //网关 4段IP 默认192.168.1.1
            struCnfg.nGateway[1] = sBuf[dataByte+7];
            struCnfg.nGateway[2] = sBuf[dataByte+8];
            struCnfg.nGateway[3] = sBuf[dataByte+9];

            struCnfg.nSubnetMask[0] = sBuf[dataByte+10];
            struCnfg.nSubnetMask[1] = sBuf[dataByte+11];
            struCnfg.nSubnetMask[2] = sBuf[dataByte+12];
            struCnfg.nSubnetMask[3] = sBuf[dataByte+13];

            emit signalCtrlBoardIpPortQuery();
            break;
        case CMD_CTRL_BOARD_VERSION_RET_PKG:  // 远程控制板版本
            struCnfg.autoCtrlBoardAlive = 0;
//            if((chainType == BOARD_TYPE_CONTROL) && (unit == 0x0e)){
//            if((chainType == BOARD_TYPE_CONTROL)){
                struGsh.struVer.nAutoCtrlRes = 1;

                struGsh.struVer.sAutoCtrlBoard[2] = sBuf[dataByte++];    //模式代号， 0：工厂模式，其余为用户模式
                struGsh.struVer.sAutoCtrlBoard[0] = sBuf[dataByte++];    //大版本
                struGsh.struVer.sAutoCtrlBoard[1] = sBuf[dataByte++];    //小版本
                struGsh.struVer.sAutoCtrlBoard[4] = sBuf[dataByte++];   //定制版本
//            }
            break;
        case CMD_CTRL_BOARD_FEED_MAIN_VALUE_REQUEST:  // 远程控制板供料量控制
            struCnfg.autoCtrlBoardAlive = 0;
            struCnfg.bUpdateStatus = 1;  //心跳锁
//            char pFeedValue[MAX_FEEDER];
//            memset(pFeedValue, 0, MAX_FEEDER);

            for(int i = 0; i < MAX_FEEDER; i++)
            {
//                pFeedValue[i] = sBuf[dataByte+i];
                struCnfp.struGroupCtrl[0].nFeederValueMainPlc[i] = sBuf[dataByte+i];
//                logger()->info("adjust feed chute %1:%2 %3",i,struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMainPlc[i],sBuf[dataByte+i]);
            }
            emit signalCtrlBoardFeedMainValueQuery();
            struCnfg.bUpdateStatus = 0;  //心跳开
            break;
        case CMD_CTRL_BOARD_FEED_VICE_VALUE_REQUEST:  // 远程控制板供料量控制
            struCnfg.autoCtrlBoardAlive = 0;
            struCnfg.bUpdateStatus = 1;  //心跳锁
            for(int i = 0; i < MAX_FEEDER; i++)
            {
//                QString tmpStr;
//                tmpStr.sprintf("feeed data: %02x %02x  %02x %02x"
//                               ,data[0]&0xFF, data[1]&0xFF, data[2]&0xFF, data[2]&0xFF);
//                qDebug(qPrintable(tmpStr));
                struCnfp.struGroupCtrl[1].nFeederValueMainPlc[i] = sBuf[dataByte+i];
            }
            emit signalCtrlBoardFeedViceValueQuery();
            struCnfg.bUpdateStatus = 0;  //心跳开
            break;
        default:
            break;
        }
    }
    else    //统计信息包 //只有2.5平台协议有，暂时用不到
    {

    }
}



