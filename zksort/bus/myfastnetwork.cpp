#include "myfastnetwork.h"
#include <QDebug>
#include <time.h>
#include <algorithm>
#include <fcntl.h>
#include "systeminfo/targetimagewidget.h"

MyRingQueue myRingQueue;

bool saveImageLoop(uchar* pData, int width, int height, int bpp, QString strFile)
{
    if(struCnfg.nFreeSpace < 100.0)
        return false;
    QImage::Format fmt;
    QVector<QRgb>  colorTable;
    switch (bpp)
    {
    case 1:
        fmt = QImage::Format_Indexed8;
        for (int k = 0; k < 256; ++k) {
            colorTable.push_back(qRgb(k, k, k));
        }
        break;
    case 3:
        fmt = QImage::Format_RGB888;
        break;
    case 4:
        fmt = QImage::Format_RGB32;
        break;
    default:
        return false;
    }
#if 0
    myFlow.saveBmp(pData, strFile, width, height, bpp ,false);
#else
    QImage im(pData, width, height, fmt);
    if (bpp == 1) {
        im.setColorTable(colorTable);
    }

    QStringList list = strFile.split("_");
    if(list.length() != 9) return false;
    int v = list.at(1).toInt(),u = list.at(2).toInt(),c = list.at(7).toInt();
    QString text = struIpcShare.struIpcModelClass[v][u].className[c];
    QPainter painter;
    if (!painter.begin(&im)) { // 绑定 painter 到 image，失败则提示
        qDebug() << "绘制初始化失败！";
        return false;
    }
    QFont font;
    font.setFamily("SimSun");    // 设置字体（宋体）
    font.setPointSize(12);       // 字体大小
    font.setBold(false);          // 加粗
    painter.setFont(font);

    // 设置文字颜色（黑色）
//    painter.setPen(QColor(0, 0, 0));
    painter.setPen(QColor(Qt::white));
    QRect rect(0, 0, width, height); // 绘制区域矩形
    painter.drawText(rect, Qt::AlignLeft | Qt::AlignTop, text);

    painter.end();
    bool saveRes = im.save(strFile, "BMP");
    if(!saveRes) {
        printf("save again\n");
        im.save(strFile, "BMP");
    }

#endif
    struCnfg.nFreeSpace -=((54+width*height*bpp)/1024.0/1024.0);
    return true;
}

void loopPngSaveThread(int index)
{
    std::cout<<"loopPngSaveThread start index = " << index << std::endl;
//    unsigned char img[2048*832*4];
    unsigned char pImage[2048*832*4];
    struct img_stream_head head;
    Packet imgPacket;
    imgPacket.data = new unsigned char[2048 * 832 * 4];
    imgPacket.length = 0;
    while(1)
    {
#if 1
        if(myRingQueue.dequeue(imgPacket)){
            memset(pImage,0,sizeof(pImage));
            memcpy(pImage,imgPacket.data,imgPacket.length);
            memcpy(&head,pImage,sizeof(head));

            if(!head.imgWidth ||!head.imgHeight || !head.imgDim){
                qDebug()<<"\033[31m"<<index<<"packed data err! "<<"\033[0m";
                free(imgPacket.data);
                imgPacket.data = NULL;
                continue;
            }
            QString fileName = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
            QString filePath = QString("/sdcard/bmp/%1_%2_%3_%4_%5_%6_%7_%8_%9.bmp").
                    arg(struGsh.nLayer).arg(head.imgView).arg(head.imgUnit)
                    .arg(struCnfp.nSensorDataType[struGsh.nLayer][head.imgView])
                    .arg(struCnfp.nSenseBrightSelection[struGsh.nLayer][head.imgView])
                    .arg(struCnfp.bColorRestoreEnable)
                    .arg(head.imgSN)
                    .arg(head.imgClass)
                    .arg(fileName);

            if(g_Runtime().checkUsbExist() && struCnfg.autoTargetImage == 1){
                QString path = g_Runtime().getUsbPath();
                if (path != ""){
                    QString imagePath = path+"IMG/"+fileName.left(8);
                    g_Runtime().dirExist(imagePath);
                    filePath = QString("%1/%2_%3_%4_%5_%6_%7_%8_%9_%10.bmp").arg(imagePath).
                            arg(struGsh.nLayer).arg(head.imgView).arg(head.imgUnit)
                            .arg(struCnfp.nSensorDataType[struGsh.nLayer][head.imgView])
                            .arg(struCnfp.nSenseBrightSelection[struGsh.nLayer][head.imgView])
                            .arg(struCnfp.bColorRestoreEnable)
                            .arg(head.imgSN)
                            .arg(head.imgClass)
                            .arg(fileName);
                }
            }
#if 0
            for(int i=0; i<head.imgHeight; i++){
                for(int j=0; j<head.imgWidth; j++){
                    img[i*head.imgWidth*3+j*3+0] = pImage[sizeof(head)+i*head.imgWidth*3+j*3+2];
                    img[i*head.imgWidth*3+j*3+1] = pImage[sizeof(head)+i*head.imgWidth*3+j*3+1];
                    img[i*head.imgWidth*3+j*3+2] = pImage[sizeof(head)+i*head.imgWidth*3+j*3+0];
                }
            }
            if(saveImageLoop(img,head.imgWidth, head.imgHeight, head.imgDim, filePath))
#else
            if(saveImageLoop(&pImage[sizeof(head)],head.imgWidth, head.imgHeight, head.imgDim, filePath))
#endif
                if(struCnfg.bMainpageShow)
                    myCircularQueue.enqueue(filePath);


            system("sync");
            free(imgPacket.data);
            system("sysctl -w vm.drop_caches=3 > /dev/zero");
            imgPacket.data = NULL;





               // emit saveTargetImg(filePath);
//                qDebug()<<"\033[32m"<<index<<"save okk! "<<head.imgSN<<head.imgWidth<<head.imgHeight<<head.imgDim<<filePath<<imgPacket.length<<"\033[0m";
//            else
//                qDebug()<<"\033[31m"<<index<<"save err! "<<head.imgSN<<head.imgWidth<<head.imgHeight<<head.imgDim<<filePath<<imgPacket.length<<"\033[0m";

#endif
#if 0
        if(myRingQueue.pop(img)){
            pImage = (unsigned char*)img;
            memset(&head,0,sizeof(head));
            memcpy(&head,pImage,sizeof(head));
            qDebug()<<"dequeue"<<head.imgSN<<head.imgWidth<<head.imgHeight<<head.imgDim;

            QString fileName = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
            QString filePath = QString("/sdcard/bmp/%1_%2_%3_%4_%5_%6_%7_%8.bmp").
                    arg(struGsh.nLayer).arg(head.imgView).arg(head.imgUnit)
                    .arg(struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[head.imgView].nSensorDataType)
                    .arg(struCnfp.nSenseBrightSelection[struGsh.nLayer][head.imgView])
                    .arg(struCnfp.bColorRestoreEnable)
                    .arg(head.imgSN)
                    .arg(fileName);
            if(saveImageLoop(&pImage[sizeof(head)],head.imgWidth, head.imgHeight, head.imgDim, filePath))
                qDebug()<<index<<"save okk! "<<head.imgSN<<head.imgWidth<<head.imgHeight<<head.imgDim<<filePath;
            else
                qDebug()<<index<<"save err! "<<head.imgSN<<head.imgWidth<<head.imgHeight<<head.imgDim<<filePath;;
            system("sync");
            qDebug()<<"\n";
#endif
        }
        else{
            usleep(1000);
        }
    }
}

MyFastNetWorkThread::MyFastNetWorkThread(QThread *parent) : QThread(parent)
{
    isrunning = false;
    currenttype = 0;
    sockfd = 0;

    m_fpgaSendPacketSeq = 0;
    imgBufHasNew = false;

    m_imageDim = 3;
    m_imagePixels = 2048;
    m_imageHeight = 832;

    switch(m_imagePixels)
    {
    case 1024:
        m_packetPerLine = 3;
        break;
    case 2048:
        m_packetPerLine = 6;
        break;
    default:
        m_packetPerLine = 3;
        break;
    }
    m_recvRowCount = 0;
    m_errCount = 0;
    m_recvImgCount = 0;

    memset(imagePacketBuf,0,sizeof(imagePacketBuf));
    memset(imageDataBuf,0,sizeof(imageDataBuf));

    udpSocketInit();
//    udpServerInit();
    initData();
}

int MyFastNetWorkThread::myGetTickCount()
{
#ifdef Q_OS_UNIX
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return(ts.tv_sec*1000+ts.tv_nsec/1000000);
#else
    return(GetTickCount());
#endif
}

void MyFastNetWorkThread::udpSocketInit(QString ip,QString port)
{
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
        printf("Obtain Socket 192.168.1.100 Err\n");
    } else {
        printf ("Obtain Socket 192.168.1.100 Ok\n");
    }

    static int valueTemp = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&valueTemp, sizeof(valueTemp));
    valueTemp=32 * 1024 * 1024;
    setsockopt(sockfd,SOL_SOCKET, SO_RCVBUF,  (const char *)&valueTemp, sizeof(valueTemp));

    memset(&addr_local, 0, sizeof(addr_local));
    addr_local.sin_family = AF_INET;
    addr_local.sin_port = htons(20001);
    const char* fip = QString(myFlow.ipForFastNetwork).toLocal8Bit().constData();
    addr_local.sin_addr.s_addr = inet_addr(fip);

    if(bind(sockfd, (struct sockaddr*)&addr_local, sizeof(struct sockaddr)) == -1 ){
        printf ("Bind Port 20001 Err.\n");
    } else {
        printf("Bind Port 20001 Ok.\n");
    }
}

void MyFastNetWorkThread::udpServerInit(int ipIndex)
{
    for(int i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(int j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(struCnfg.struLayerInfo[i].nViewIsUsed[j] == 1)
            {
                for(int k = 0; k <struCnfg.struLayerInfo[i].nViewUnitTotal[j]; k++)
                {
                    memset(&addr_remote[j][k], 0, sizeof(addr_remote[j][k]));
                    if(struCnfg.struIpcCameraParams[i][j][k].ipcType != IPC_CAMERA_TYPE_NULL)
                    {
                        QString serverIp = QString("192.168.1.%1").arg(struCnfg.struIpcCameraParams[i][j][k].nIpAddr[4]);
                        const char* ip = serverIp.toLatin1().constData();
                        memset(&addr_remote, 0, sizeof(addr_remote));
                        addr_remote[j][k].sin_family = AF_INET;
#ifndef Q_OS_WIN
                        inet_pton(AF_INET, ip, &addr_remote[j][k].sin_addr); 	// Net Address
#endif
                        addr_remote[j][k].sin_port = htons(20001);
                        // printf("udp to ip %s port 20001 Ok.\n",ip);
                    }
                }
            }
        }
    }
}
void MyFastNetWorkThread::initData()
{
    for(int ii = 0; ii < MAX_LOOP_PNG_SAVE_THREAD; ii++)
    {
        qDebug()<<"create thread";
//        m_loop_thread_save[ii] = std::thread(loopPngSaveThread, ii);
//        m_loop_thread_save[ii].detach();
        msleep(10);
    }
}

void MyFastNetWorkThread::setUdpRecvType(int type,int mlayer,int mview,int munit)
{
    currenttype= type;
    view = mview;
    unit = munit;
}

void MyFastNetWorkThread::updateCameraInfo(int view,int unit)
{
    m_imageDim = struCnfp.nSensorDim[struGsh.nLayer][view];
    m_imagePixels = myFlow.getSensorPixels(struGsh.nLayer, view);

    switch(m_imagePixels) {
    case 1024:
        m_packetPerLine = m_imagePixels*m_imageDim/1024;
        break;
    case 2048:
        m_packetPerLine = m_imagePixels*m_imageDim/1024;
        break;
    default:
        m_packetPerLine = 3;
        break;
    }
    int quo = 0;
    switch(m_imagePixels) {
    case 1024:
        quo = 1;
        break;
    case 2048:
        quo = 2;
        break;
    default:
        quo = 1;
        break;
    }
    imgPacketLen = PACKET_HEAD_LEN + 12 + m_imagePixels/quo;
    m_imageHeight = myFlow.imgHeight;
    qDebug()<<"view:"<<view<<"dim:"<<m_imageDim<<"pixels:"<<m_imagePixels<<"height:"<<m_imageHeight<<"packlen"<<imgPacketLen;
}

int MyFastNetWorkThread::saveBMPFile(QString path,int w,int h)
{
    if(struCnfg.nFreeSpace < 100.0)
        return 0;

#if 0
    BITMAPFILEHEADER bmfh; //bmp文件头
    BITMAPINFOHEADER bmih;
    unsigned short wWidth, wHeight;//图像的分辨率，需要预先设置
    wWidth = w;
    wHeight = h;

    const char* filename = QString(path).toLatin1().constData();
    FILE *f;
    int fd = open(filename, O_WRONLY|O_CREAT, 0640);
    if(-1 == fd){
        printf("open %s failure, errno %d\n", filename, errno);
        return 0;
    }

    size_t imageBuffSize= (((size_t)wWidth) * ((size_t)wHeight)) << 2;
    //设置BMP文件头
    bmfh.bfType = 0x4D42;//'BM'
    bmfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + imageBuffSize;
    bmfh.bfReserved1 = 0;
    bmfh.bfReserved2 = 0;
    bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    //设置BMP信息头
    bmih.biSize = 0U | sizeof(BITMAPINFOHEADER);
    bmih.biWidth = wWidth;
    bmih.biHeight = -wHeight;
    bmih.biPlanes = 1;
    bmih.biBitCount = 32;
    bmih.biCompression = 0;
    bmih.biSizeImage = 0U | imageBuffSize;
    bmih.biXPelsPerMeter = 0;
    bmih.biYPelsPerMeter = 0;
    bmih.biClrUsed = 0;
    bmih.biClrImportant = 0;
    //写文件
    write(fd, &bmfh, sizeof(BITMAPFILEHEADER));
    write(fd, &bmih, sizeof(BITMAPINFOHEADER));
    write(fd, imageDataBuf, imageBuffSize);
    //完成
#ifdef Q_OS_UNIX
    fsync(fd);
#endif
    close(fd);
    return 1;

#else
    BITMAPFILEHEADER bmfh; //bmp文件头
    BITMAPINFOHEADER bmih;
    unsigned short wWidth, wHeight;//图像的分辨率，需要预先设置
    wWidth = w;
    wHeight = h;

    const char* filename = QString(path).toLatin1().constData();
    int fd = open(filename, O_WRONLY|O_CREAT, 0640);
    if(-1 == fd){
        printf("open %s failure, errno %d\n", filename, errno);
        return 0;
    }
    size_t imageBuffSize= (((size_t)wWidth) * ((size_t)wHeight)) *m_imageDim;
    //设置BMP文件头
    bmfh.bfType = 0x4D42;//'BM'
    if(m_imageDim == 1){
        bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD);
        bmfh.bfSize = sizeof(RGBQUAD) * 256 +sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + imageBuffSize;
    }else{
        bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
        bmfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + imageBuffSize;
    }
    bmfh.bfReserved1 = 0;
    bmfh.bfReserved2 = 0;
    //设置BMP信息头
    bmih.biSize = 0U | sizeof(BITMAPINFOHEADER);
    bmih.biWidth = wWidth;
    bmih.biHeight = -wHeight;
    bmih.biPlanes = 1;
    bmih.biBitCount = m_imageDim*8;
    bmih.biCompression = 0;
    bmih.biSizeImage = 0U | imageBuffSize;
    bmih.biXPelsPerMeter = 0;
    bmih.biYPelsPerMeter = 0;
    bmih.biClrUsed = 0;
    bmih.biClrImportant = 0;

    RGBQUAD rgbquad[256];
    int i;
    for(i = 0; i < 256; i++)
    {
        rgbquad[i].rgbBlue = i;
        rgbquad[i].rgbGreen = i;
        rgbquad[i].rgbRed = i;
        rgbquad[i].rgbReserved = 0;
    }
    //写文件
    write(fd, &bmfh, sizeof(BITMAPFILEHEADER));
    write(fd, &bmih, sizeof(BITMAPINFOHEADER));
    if(m_imageDim == 1){
        write(fd,&rgbquad, sizeof(RGBQUAD)*256);
        for(i = 0; i < h; i++)
        {
            write(fd,imageDataBuf+ (h- 1 - i) * w, w);
        }
    }else{
        write(fd, imageDataBuf, imageBuffSize);
    }
    //完成
#ifdef Q_OS_UNIX
    fsync(fd);
#endif
    close(fd);
    if(struCnfg.nFreeSpace >100.0)
        struCnfg.nFreeSpace -=(bmfh.bfSize/1024.0/1024.0);
    g_Runtime().mySystem("sysctl -w vm.drop_caches=3 > /dev/zero");//very important!!!
    return 1;
#endif
}


/* 在线程中每次读取前调用，对超时进行处理 */
int MyFastNetWorkThread::checkUdpSocketStatues(int seconds)
{
    struct timeval timeout;
    timeout.tv_sec  = seconds;
    timeout.tv_usec = 500*1000;
    fd_set rds;
    FD_ZERO (&rds);
    FD_SET(sockfd, &rds);
    int ret;
    if(seconds > 0){
        for(int i = 0; i < seconds;i++){
            if(!isrunning){
                return -1;
            }
            FD_ZERO (&rds);
            FD_SET(sockfd, &rds);
            timeout.tv_sec  = 1;
            timeout.tv_usec = 500*1000/seconds;
            ret = select(sockfd+1,&rds,NULL,NULL,&timeout);
            if(ret == -1){
                qDebug()<<"socket fail";
                emit SignalSocketFailed();
                myFlow.msleep(200);
                return -1;
            }
            if(ret == 0){
                continue;
            }
            if(ret >= 1){
                return 1;
            }
        }
        return -1;
    }
    else{
        ret = select(sockfd+1,&rds,NULL,NULL,&timeout);
        if(ret == -1)
        {
            qDebug()<<"socket fail";
            emit SignalSocketFailed();
            myFlow.msleep(200);
            return -1;
        }
        if(ret == 0)
        {
            return -1;
        }
        return 1;
    }
}

/*
 *  用在下发串口指令前清除本地缓存
 */
void MyFastNetWorkThread::cleanUdpSocket(int seconds)
{
#if 1 //采用非阻塞方式判断套接字缓冲区是否有数据可读
    struct timeval timeout;
    timeout.tv_sec  = 0;
    timeout.tv_usec = 0;
    fd_set rds;
    FD_ZERO (&rds);
    FD_SET(sockfd, &rds);
    int ret = -1;
    while(1)
    {
        ret = select(sockfd+1,&rds,NULL,NULL,&timeout);
        if(ret == -1)
        {
            emit SignalSocketFailed();
            myFlow.msleep(200);
            return;
        }
        else if (ret == 0)
        {
            return;
        }
        ret = recv(sockfd,(char *)dirtydatabuf,sizeof(dirtydatabuf),0);
        if (ret == 0)
        {
            return;
        }
        else if(ret == -1)
        {
            qDebug()<<"recv err";
            return;
        }
    }
#endif
}


void MyFastNetWorkThread::udpSendTransStartStop(int start,int type,int view,int unit)
{
    unsigned char data[SEND_PACKET_DATA_SIZE];
    memset(data, 0, SEND_PACKET_DATA_SIZE);

    if(type == TARGET_Mode){
        data[0] = start;
        data[1] = struCnfg.trigArea/256;
        data[2] = struCnfg.trigArea%256;
        data[3] = struCnfg.cutHeight/256;
        data[4] = struCnfg.cutHeight%256;
        data[5] = struCnfg.imgDim;

        g_Udp.pushUdpCmdData(CMD_UDP_TARGET_FAST_CAPTURE, true, view,unit, 16, data);
    }
    else{
		data[0] = start;
		data[1] = struGsh.nImageCapTrigValue/256;
		data[2] = struGsh.nImageCapTrigValue%256;
		data[3] = (m_imageHeight>>16) & 0xff;
		data[4] = (m_imageHeight>>8) & 0xff;
		data[5] = m_imageHeight &0xff;
		if(struCnfp.nSensorPixel[0][struGsh.nView] == SENSOR_PIXELS_2048_XTD)
		{
		    data[3] = ( (m_imageHeight/2) >>16) & 0xff;
		    data[4] = ( (m_imageHeight/2) >>8) & 0xff;
		    data[5] = (m_imageHeight/2) &0xff;
		}
		data[6] = struGsh.bCorrectImage;
		data[7] = struGsh.bFilterBackground;
		data[8] = struGsh.nImageCapTimeout;
		data[9] = type;
		if(type == FR_Mode){
		    data[10] = 100/256;
		    data[11] = 100%256;
		}else{
		    data[10] = struGsh.nImageCapDelayTime/256;
		    data[11] = struGsh.nImageCapDelayTime%256;
		}

		g_Udp.pushUdpCmdData(CMD_UDP_HT_GET_FAST_CAPTURE, true, view,unit, 12, data);
	}
}
void MyFastNetWorkThread::udpSendCmdData(int nCmd, char intAddr, char boardType, char boardAddr, char *data, int nCount)
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

    nCrcItt = mySerial.crcFpga(sCrcData, 20);

    sPacket[22] = nCrcItt/256;
    sPacket[23] = nCrcItt%256;
    sPacket[24] = 0xff;

    char queData[SEND_PACKET_SIZE];
    memcpy(queData, sPacket, SEND_PACKET_SIZE);
    QString tmpStr;
    tmpStr.sprintf("HEAD[%02x %02x] ADDR[%02x %02x] CMD[%02x %02x]--DATA: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x CRC-%02x %02x %02x"
                   ,queData[0]&0xFF, queData[1]&0xFF, queData[2]&0xFF, queData[3]&0xFF,queData[4]&0xFF, queData[5]&0xFF, queData[6]&0xFF, queData[7]&0xFF
            ,queData[8]&0xFF, queData[9]&0xFF, queData[10]&0xFF, queData[11]&0xFF,queData[12]&0xFF, queData[13]&0xFF, queData[14]&0xFF, queData[15]&0xFF
            ,queData[16]&0xFF, queData[17]&0xFF, queData[18]&0xFF, queData[19]&0xFF, queData[20]&0xFF, queData[21]&0xFF, queData[22]&0xFF, queData[23]&0xFF
            ,queData[24]&0xFF);
    if (struGsh.debugMode && (queData[4] !=0 && queData[5] !=0x21))
        qDebug(qPrintable(tmpStr));

    //    udpSendTo(queData,SEND_PACKET_SIZE);
}
int MyFastNetWorkThread::udpSendTo(int view,int unit, char *buf, int len)
{
    socklen_t addrlen;
    addrlen=sizeof(addr_remote[view][unit]);

    return sendto(sockfd,buf,len,0,(struct sockaddr *)&addr_remote[view][unit],addrlen);
}

void MyFastNetWorkThread::run()
{
    socklen_t sin_size = sizeof(struct sockaddr_in);
    int wantedColor = 0,wantedPic = 0,curPic = 0;
    unsigned int cur_rownum = 0;
    int len = 0;
    int packLen = 1024;
    int tmpIndex = 0, recvRowCount = 0;
    int curImgcnt[MAX_VIEW][MAX_VIEW_UNIT];
    int wantedPics[MAX_VIEW][MAX_VIEW_UNIT] = {0},curPics[MAX_VIEW][MAX_VIEW_UNIT] = {0},startPics[MAX_VIEW][MAX_VIEW_UNIT]={0};
    int full = 0;
    struct img_stream_head t_imgHead[96];
    QStringList t_imgIpStrList;
    QMap<int,QString> camIpMap;
    int t_curPic[96],t_wantedPic[96],t_totalPacket[96],t_curPacket[96],t_wantedPacket[96],t_recvedTotalSize[96],t_recvedDataSize[96];
    QStringList viewList,curImgcntList;

    updateCameraInfo(view,unit);

    memset(imagePacketBuf,0,sizeof(imagePacketBuf));
    memset(imageDataBuf,0,sizeof(imageDataBuf));
    //    int realSendColor[6]={0,3,1,4,2,5};

    cleanUdpSocket();
    while (isrunning){
        switch (currenttype)
        {
        case FAST_CAPTURE_IMAGE:
            m_saveImgCount = 0;
            udpSendTransStartStop(Start,S_Mode,struGsh.nView,struGsh.nUnit);
            while (isrunning)
            {
                if( checkUdpSocketStatues(1) < 0 )
                {
                    qDebug()<<"timeout";
                    //                    isrunning = 0;
                    continue;//本次图片可能已经采完了，客户再来一把料？
                }
                /********************************原始数据*********************************/
                len = recvfrom(sockfd, (char *)imagePacketBuf, sizeof(imagePacketBuf), 0,(struct sockaddr*)&addr_local,&sin_size);
                curPic = (imagePacketBuf[PACKET_HEAD_LEN]<<24|imagePacketBuf[PACKET_HEAD_LEN+1]<<16|imagePacketBuf[PACKET_HEAD_LEN+2]<<8|imagePacketBuf[PACKET_HEAD_LEN+3]);
                if(wantedPic == 0){
                    wantedPic = curPic;
                }

                //                if(imagePacketBuf[PACKET_HEAD_LEN+8] != realSendColor[wantedColor]
                if(imagePacketBuf[PACKET_HEAD_LEN+8] != wantedColor
                        ||imagePacketBuf[0] != 0xA5 ||imagePacketBuf[1] != 0x5A
                        || curPic !=wantedPic )
                {
                    //                    m_errCount++;
                    m_errCount = (imagePacketBuf[PACKET_HEAD_LEN+4]<<24|imagePacketBuf[PACKET_HEAD_LEN+5]<<16|imagePacketBuf[PACKET_HEAD_LEN+6]<<8|imagePacketBuf[PACKET_HEAD_LEN+7]);

                    if( wantedPic != curPic+1 && wantedPic != curPic )
                    {
                        qDebug()<<"m_errCount"<<m_errCount<<"wantedColor"<<wantedColor<<imagePacketBuf[PACKET_HEAD_LEN+8]<<imagePacketBuf[0] <<imagePacketBuf[1]<<curPic<<wantedPic;
                    }
                    memset(imageCombineBuf, 0, sizeof(imageCombineBuf));
                    wantedColor = 0;
                    m_recvRowCount = 0;
                    recvRowCount = 0;
                    wantedPic = curPic+1;
                    continue;
                }
                memcpy(&imageCombineBuf[wantedColor*packLen],&imagePacketBuf[PACKET_HEAD_LEN+12], packLen);
                wantedColor++;
                if(wantedColor < m_packetPerLine)
                {
                    continue;
                }
                wantedColor = 0;
                /********************************图像组包**********************************/
                qDebug()<<"m_recvRowCount before"<<m_recvRowCount;

                switch (m_imageDim) {
                case 3:
                {
                    for(int ii = 0; ii < m_imagePixels; ii++)
                    {
                        imageDataBuf[m_recvRowCount*m_imagePixels*m_imageDim+ii*m_imageDim+2] = imageCombineBuf[m_imagePixels*0+ii];
                        imageDataBuf[m_recvRowCount*m_imagePixels*m_imageDim+ii*m_imageDim+1] = imageCombineBuf[m_imagePixels*1+ii];
                        imageDataBuf[m_recvRowCount*m_imagePixels*m_imageDim+ii*m_imageDim+0] = imageCombineBuf[m_imagePixels*2+ii];

                    }

                    if(struCnfp.nSensorPixel[0][struGsh.nView] == SENSOR_PIXELS_2048_XTD) // 再复制一行
                    {
                        ++m_recvRowCount;
                        for(int ii = 0; ii < m_imagePixels; ii++)
                        {
                            imageDataBuf[m_recvRowCount*m_imagePixels*m_imageDim+ii*m_imageDim+2] = imageCombineBuf[m_imagePixels*0+ii];
                            imageDataBuf[m_recvRowCount*m_imagePixels*m_imageDim+ii*m_imageDim+1] = imageCombineBuf[m_imagePixels*1+ii];
                            imageDataBuf[m_recvRowCount*m_imagePixels*m_imageDim+ii*m_imageDim+0] = imageCombineBuf[m_imagePixels*2+ii];

                        }
                    }
//                    else if(struCnfp.nSensorPixel[0][struGsh.nView] == SENSOR_PIXELS_2048_GTD)
//                    {

//                        if(m_recvRowCount % 2 ==0) // 偶数行
//                        {
//                            for(int ii = 0; ii < m_imagePixels; ii++)
//                            {
//                                imageDataBuf[m_recvRowCount*m_imagePixels*m_imageDim+ii*m_imageDim+2] = imageCombineBuf[m_imagePixels*0+ii];
//                                imageDataBuf[m_recvRowCount*m_imagePixels*m_imageDim+ii*m_imageDim+1] = imageCombineBuf[m_imagePixels*1+ii];
//                                imageDataBuf[m_recvRowCount*m_imagePixels*m_imageDim+ii*m_imageDim+0] = imageCombineBuf[m_imagePixels*2+ii];

//                            }
//                        }
//                        else
//                        {
//                            for(int ii = 0; ii < m_imagePixels; ii++)
//                            {
//                                imageDataBuf[m_recvRowCount*m_imagePixels*m_imageDim+ii*m_imageDim+2] = imageCombineBuf[m_imagePixels*0+ii];
//                                imageDataBuf[m_recvRowCount*m_imagePixels*m_imageDim+ii*m_imageDim+1] = imageCombineBuf[m_imagePixels*1+ii];
//                                imageDataBuf[m_recvRowCount*m_imagePixels*m_imageDim+ii*m_imageDim+0] = imageCombineBuf[m_imagePixels*2+ii];

//                            }
//                        }

//                    }
                }
                    break;
                case 4:
                {
                    for(int ii = 0; ii < m_imagePixels; ii++)
                    {
                        imageDataBuf[m_recvRowCount*m_imagePixels*m_imageDim+ii*m_imageDim+2] = imageCombineBuf[m_imagePixels*0+ii];
                        imageDataBuf[m_recvRowCount*m_imagePixels*m_imageDim+ii*m_imageDim+1] = imageCombineBuf[m_imagePixels*1+ii];
                        imageDataBuf[m_recvRowCount*m_imagePixels*m_imageDim+ii*m_imageDim+0] = imageCombineBuf[m_imagePixels*2+ii];
                        imageDataBuf[m_recvRowCount*m_imagePixels*m_imageDim+ii*m_imageDim+3] = imageCombineBuf[m_imagePixels*3+ii];
                    }


                }
                    break;
                default:
                    break;
                }

                qDebug()<<"m_recvRowCount"<<m_recvRowCount;
                if(++m_recvRowCount >= m_imageHeight)
                {
                    m_recvRowCount = 0;
                }
                else
                {
                    continue;
                }
                /********************************图像存储**********************************/
                if(struCnfg.nFreeSpace <= 100.0){
                    qDebug()<<"flash full";
                    udpSendTransStartStop(Stop,S_Mode,struGsh.nView,struGsh.nUnit);
                    isrunning = false;
                    emit SignalImageRecvFull(struGsh.nView,struGsh.nUnit); //flash满了提醒用户，并停止采图
                    continue;
                }
                m_saveImgCount++;
                wantedPic++;
                QString fileName = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
                QString filePath;



                    filePath = QString("/sdcard/bmp/%1_%2_%3_%4_%5_%6_%7_%8.bmp").arg(struGsh.nLayer).arg(struGsh.nView).arg(struGsh.nUnit)
                            .arg( struCnfp.nSensorDataType[struGsh.nLayer][struGsh.nView] )
                            .arg(struCnfp.nSenseBrightSelection[struGsh.nLayer][struGsh.nView])
                            .arg(struCnfp.bColorRestoreEnable)
                            .arg(m_saveImgCount)
                            .arg(fileName);



                qDebug()<<m_saveImgCount<<m_imagePixels<<m_imageHeight;
                if(saveBMPFile(filePath,m_imagePixels, m_imageHeight))
                { //缩略图存储
                    QImage img(filePath);
                    if (!struGsh.nRemoteImageCaptureIsRun)
                    {
                        img = img.scaled(180,120);
                        img.save(filePath+"_copy","PNG",50);
                    }

                    viewList.clear();
                    curImgcntList.clear();
                    viewList.append(QString::number(struGsh.nView));
                    curImgcntList.append(QString::number(m_saveImgCount));

                    emit SignalImageRecvFinished(viewList,curImgcntList);
                }

                memset(imagePacketBuf,0,sizeof(imagePacketBuf));
                memset(imageDataBuf,0,sizeof(imageDataBuf));
                memset(imageCombineBuf, 0, sizeof(imageCombineBuf));
            }
            isrunning = 0;
            return;

        case FAST_FR_CAPTURE_IMAGE:
            memset(curImgcnt,0,sizeof(curImgcnt));

            memset(wantedPics,0,sizeof(wantedPics));
            memset(curPics,0,sizeof(curPics));
            memset(startPics,0,sizeof(startPics));

            //1.发送开始，让相机板开始缓存
            udpSendTransStartStop(Start,FR_Mode,BOARD_TYPE_ALL_CAMERA,BOARD_TYPE_ALL_IN_CHAIN);

            msleep(100);
            qDebug()<<"captured start";
            while (isrunning)
            {
                for(int v = 0; v < MAX_VIEW; v++ )
                {
                    if (struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[v])
                    {
                        for(int u = 0; u < struCnfg.struLayerInfo[struGsh.nLayer].nViewUnitTotal[v];u++)
                        {
                            if(!struCnfg.struIpcCameraParams[struGsh.nLayer][v][u].nSaveImgEnable) continue;
                            //2.第一次查询，确保本次存图下位机已经正常触发
                            myFlow.getCamImgInfo(false,v,u);
                            msleep(100);
                            if(struIpcShare.struIpcReqStauts[v][u].camImgInfoReq == 1)
                            {
                                //3.开始相机XX的图像传输
                                if(curImgcnt[v][u] < struGsh.nCamCurImgCnt[v][u] )
                                    udpSendTransStartStop(FR_Start,FR_Mode,v,u);
                                else
                                    continue;// 歇歇，下一视
                                while(isrunning)
                                {
                                    if( checkUdpSocketStatues(0) < 0 )
                                    {
                                        break;
                                    }
                                    len = recvfrom(sockfd, (char *)imagePacketBuf, sizeof(imagePacketBuf), 0,(struct sockaddr*)&addr_local,&sin_size);
                                    if(len != imgPacketLen)
                                        qDebug()<<"err imgPacketLen"<<len;
                                    curPics[v][u] = (imagePacketBuf[PACKET_HEAD_LEN]<<24|imagePacketBuf[PACKET_HEAD_LEN+1]<<16|imagePacketBuf[PACKET_HEAD_LEN+2]<<8|imagePacketBuf[PACKET_HEAD_LEN+3]);
                                    if(wantedPics[v][u] == 0)
                                    {
                                        wantedPics[v][u] = curPics[v][u];
                                        startPics[v][u] = curPics[v][u];
                                    }

                                    if(imagePacketBuf[PACKET_HEAD_LEN+8] != wantedColor
                                            ||imagePacketBuf[0] != 0xA5 ||imagePacketBuf[1] != 0x5A
                                            || wantedPics[v][u] != curPics[v][u])
                                    {
                                        m_errCount = (imagePacketBuf[PACKET_HEAD_LEN+4]<<24|imagePacketBuf[PACKET_HEAD_LEN+5]<<16|imagePacketBuf[PACKET_HEAD_LEN+6]<<8|imagePacketBuf[PACKET_HEAD_LEN+7]);
                                        qDebug()<<"view"<<v<<"m_errCount"<<m_errCount<<"wantedColor"<<wantedColor<<imagePacketBuf[PACKET_HEAD_LEN+8]<<imagePacketBuf[0] <<imagePacketBuf[1]<<curPics[v]<<wantedPics[v];

                                        if( wantedPics[v][u] != curPics[v][u]+1 && wantedPics[v][u] != curPics[v][u] ){
                                            //qDebug()<<"view"<<v<<"m_errCount"<<m_errCount<<"wantedColor"<<wantedColor<<imagePacketBuf[PACKET_HEAD_LEN+8]<<imagePacketBuf[0] <<imagePacketBuf[1]<<curPics[v]<<wantedPics[v];

                                            if(curPics[v][u] - startPics[v][u] > 0)
                                                curImgcnt[v][u] = curPics[v][u] - startPics[v][u];
                                            else
                                                curImgcnt[v][u]++;
                                        }
                                        memset(imageCombineBuf, 0, sizeof(imageCombineBuf));
                                        wantedColor = 0;
                                        m_recvRowCount = 0;
                                        recvRowCount = 0;
                                        wantedPics[v][u] = curPics[v][u]+1;
                                        continue;
                                    }

                                    memcpy(&imageCombineBuf[wantedColor*packLen],&imagePacketBuf[PACKET_HEAD_LEN+12], packLen);
                                    wantedColor++;
                                    if(wantedColor < m_packetPerLine)
                                    {
                                        continue;
                                    }
                                    wantedColor = 0;
                                    /********************************图像组包**********************************/
                                    //                                    for(int ii = 0; ii < m_imagePixels; ii++)
                                    //                                    {
                                    //                                        imageDataBuf[m_recvRowCount*m_imagePixels*m_imageDim+ii*m_imageDim+2] = imageCombineBuf[m_imagePixels*0+ii];
                                    //                                        imageDataBuf[m_recvRowCount*m_imagePixels*m_imageDim+ii*m_imageDim+1] = imageCombineBuf[m_imagePixels*1+ii];
                                    //                                        imageDataBuf[m_recvRowCount*m_imagePixels*m_imageDim+ii*m_imageDim+0] = imageCombineBuf[m_imagePixels*2+ii];
                                    //                                    }

                                    //                                    if(struCnfp.nSensorPixel[0][v] == SENSOR_PIXELS_2048_XTD) // 再复制一行
                                    //                                    {
                                    //                                        ++m_recvRowCount;
                                    //                                        for(int ii = 0; ii < m_imagePixels; ii++)
                                    //                                        {
                                    //                                            imageDataBuf[m_recvRowCount*m_imagePixels*m_imageDim+ii*m_imageDim+2] = imageCombineBuf[m_imagePixels*0+ii];
                                    //                                            imageDataBuf[m_recvRowCount*m_imagePixels*m_imageDim+ii*m_imageDim+1] = imageCombineBuf[m_imagePixels*1+ii];
                                    //                                            imageDataBuf[m_recvRowCount*m_imagePixels*m_imageDim+ii*m_imageDim+0] = imageCombineBuf[m_imagePixels*2+ii];

                                    //                                        }
                                    //                                    }


                                    switch (m_imageDim) {
                                    case 3:
                                    {
                                        for(int ii = 0; ii < m_imagePixels; ii++)
                                        {
                                            imageDataBuf[m_recvRowCount*m_imagePixels*m_imageDim+ii*m_imageDim+2] = imageCombineBuf[m_imagePixels*0+ii];
                                            imageDataBuf[m_recvRowCount*m_imagePixels*m_imageDim+ii*m_imageDim+1] = imageCombineBuf[m_imagePixels*1+ii];
                                            imageDataBuf[m_recvRowCount*m_imagePixels*m_imageDim+ii*m_imageDim+0] = imageCombineBuf[m_imagePixels*2+ii];

                                        }

                                        if(struCnfp.nSensorPixel[0][v] == SENSOR_PIXELS_2048_XTD) // 再复制一行
                                        {
                                            ++m_recvRowCount;
                                            for(int ii = 0; ii < m_imagePixels; ii++)
                                            {
                                                imageDataBuf[m_recvRowCount*m_imagePixels*m_imageDim+ii*m_imageDim+2] = imageCombineBuf[m_imagePixels*0+ii];
                                                imageDataBuf[m_recvRowCount*m_imagePixels*m_imageDim+ii*m_imageDim+1] = imageCombineBuf[m_imagePixels*1+ii];
                                                imageDataBuf[m_recvRowCount*m_imagePixels*m_imageDim+ii*m_imageDim+0] = imageCombineBuf[m_imagePixels*2+ii];

                                            }
                                        }
                                    }
                                        break;
                                    case 4:
                                    {
                                        for(int ii = 0; ii < m_imagePixels; ii++)
                                        {
                                            imageDataBuf[m_recvRowCount*m_imagePixels*m_imageDim+ii*m_imageDim+2] = imageCombineBuf[m_imagePixels*0+ii];
                                            imageDataBuf[m_recvRowCount*m_imagePixels*m_imageDim+ii*m_imageDim+1] = imageCombineBuf[m_imagePixels*1+ii];
                                            imageDataBuf[m_recvRowCount*m_imagePixels*m_imageDim+ii*m_imageDim+0] = imageCombineBuf[m_imagePixels*2+ii];
                                            imageDataBuf[m_recvRowCount*m_imagePixels*m_imageDim+ii*m_imageDim+3] = imageCombineBuf[m_imagePixels*3+ii];
                                        }


                                    }
                                        break;
                                    default:
                                        break;
                                    }


                                    if(++m_recvRowCount >= m_imageHeight)
                                    {
                                        m_recvRowCount = 0;
                                    }
                                    else
                                        continue;

                                    /********************************图像存储**********************************/
                                    if(struCnfg.nFreeSpace <= 100.0){
                                        qDebug()<<"captured terminate - flash full";
                                        udpSendTransStartStop(Stop,FR_Mode,BOARD_TYPE_ALL_CAMERA,BOARD_TYPE_ALL_IN_CHAIN);
                                        isrunning = false;
                                        emit SignalImageRecvFull(struGsh.nView,struGsh.nUnit); //flash满了提醒用户，并停止采图
                                        continue;
                                    }
                                    curImgcnt[v][u]++;
                                    wantedPics[v][u]++;
                                    //qDebug()<<"captured view"<<v<<curImgcnt[v]<<struGsh.nCamCurImgCnt[v][unit]<<struGsh.nCamMaxImgCnt[v][unit]<<"----------------";
                                    QString fileName = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
                                    QString filePath;

                                    filePath = QString("/sdcard/bmp/%1_%2_%3_%4_%5_%6_%7_%8.bmp").arg(struGsh.nLayer).arg(v).arg(u)
                                            .arg(struCnfp.nSensorDataType[struGsh.nLayer][struGsh.nView])
                                            .arg(struCnfp.nSenseBrightSelection[struGsh.nLayer][v])
                                            .arg(struCnfp.bColorRestoreEnable)
                                            .arg(curImgcnt[v][u])
                                            .arg(fileName);

                                    if(saveBMPFile(filePath,m_imagePixels, m_imageHeight)){
                                        QImage img(filePath);
                                        if (!struGsh.nRemoteImageCaptureIsRun){
                                            img = img.scaled(180,120);
                                            img.save(filePath+"_copy","PNG",50);
                                        }
                                        viewList.clear();
                                        curImgcntList.clear();
                                        for(int tv = 0; tv < MAX_VIEW; tv++ )
                                        {
                                            QString vImgcnt;
                                            bool vExistImg = false;
                                            if (struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[tv]  )
                                            {
                                                for(int tu= 0; tu < struCnfg.struLayerInfo[struGsh.nLayer].nViewUnitTotal[tv]; tu++)
                                                {
                                                    vImgcnt += QString::number(curImgcnt[tv][tu]);
                                                    vImgcnt +="-";
                                                    if(struCnfg.struIpcCameraParams[struGsh.nLayer][tv][tu].nSaveImgEnable){
                                                        vExistImg = true;
                                                    }
                                                }
                                                if(vExistImg) //防止双层机只单采上层
                                                {
                                                    // qDebug()<<"ok"<<tv;
                                                    viewList.append(QString::number(tv));
                                                    curImgcntList.append(vImgcnt);
                                                }
                                            }
                                        }
                                        emit SignalImageRecvFinished(viewList,curImgcntList);
                                    }

                                    memset(imagePacketBuf,0,sizeof(imagePacketBuf));
                                    memset(imageDataBuf,0,sizeof(imageDataBuf));
                                    memset(imageCombineBuf, 0, sizeof(imageCombineBuf));

                                    if( curImgcnt[v][u] < struGsh.nCamCurImgCnt[v][u] )
                                    {
                                        if(curImgcnt[v][u] == struGsh.nCamCurImgCnt[v][u] -1 )
                                        {
                                            if(struGsh.nCamCurImgCnt[v][u] == struGsh.nCamMaxImgCnt[v][u])
                                            {
                                                udpSendTransStartStop(FR_Start,FR_Mode,v,u);
                                                full = 1;
                                            }
                                            else{
                                                myFlow.getCamImgInfo(false,v,u);
                                                msleep(100);
                                            }
                                        }else{
                                            udpSendTransStartStop(FR_Start,FR_Mode,v,u);
                                        }
                                    }
                                }
                            }
                            else{
                                qDebug()<<"camImgInfoReq timeout";
                            }
                        }
                    }//结束一轮循环

                    if(full == 1)
                    {
                        for(int v = 0; v < MAX_VIEW; v++ )
                        {
                            if (struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[v])
                            {
                                for(int u= 0; u < struCnfg.struLayerInfo[struGsh.nLayer].nViewUnitTotal[v]; u++)
                                {
                                    if(!struCnfg.struIpcCameraParams[struGsh.nLayer][v][u].nSaveImgEnable) continue;
                                    myFlow.getCamImgInfo(false,v,u);
                                    msleep(100);
                                }
                            }
                        }
                        int allfull = 1;
                        for(int v = 0; v < MAX_VIEW; v++ )
                        {
                            if (struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[v])
                            {
                                for(int u= 0; u < struCnfg.struLayerInfo[struGsh.nLayer].nViewUnitTotal[v]; u++)
                                {
                                    if(!struCnfg.struIpcCameraParams[struGsh.nLayer][v][u].nSaveImgEnable) continue;
                                    //有一视满了，但有其他视还没满
                                    if( curImgcnt[v][u] < struGsh.nCamCurImgCnt[v][u] )
                                    {
                                        allfull = 0;
                                        break;
                                    }
                                }
                            }
                            if( allfull == 0) break;
                        }
                        if(allfull == 1){
                            udpSendTransStartStop(Stop,FR_Mode,BOARD_TYPE_ALL_CAMERA,BOARD_TYPE_ALL_IN_CHAIN);
                            isrunning = false;
                            emit SignalImageRecvFull(0,unit); //ddr满了提示用户并结束采图
                            qDebug()<<"captured end";
                            break;
                        }
                    }
                }
            }
            isrunning = 0;
            return;
            //              Byte0-3:目标框序号(从0开始)
            //              Byte4-5:高度H
            //              Byte6-7:宽度W
            //              Byte8:通道数C
            //              Byte9-10:总包数
            //              Byte11-12:子包序号(从0开始)
            //              Byte13-:图像数据(RGBN-RGBN),按照实际的通道数。数据量为 H*W*C
        case FAST_TARGET_CAPTURE_IMAGE:
            qDebug()<<"FAST_TARGET_CAPTURE_IMAGE";
//            t_imgHead[0].imgView = struGsh.nView%2?struGsh.nView-1:struGsh.nView;
//            t_imgHead[0].imgUnit= struGsh.nUnit;
//            t_imgHead[1].imgView = t_imgHead[0].imgView+1;
//            t_imgHead[1].imgUnit= struGsh.nUnit;
//            t_imgIpStrList.append(QString("%1.%2.%3.%4").arg(struCnfg.struIpcCameraParams[struGsh.nLayer][t_imgHead[0].imgView][struGsh.nUnit].nIpAddr[0])
//                    .arg(struCnfg.struIpcCameraParams[struGsh.nLayer][t_imgHead[0].imgView][struGsh.nUnit].nIpAddr[1])
//                    .arg(struCnfg.struIpcCameraParams[struGsh.nLayer][t_imgHead[0].imgView][struGsh.nUnit].nIpAddr[2])
//                    .arg(struCnfg.struIpcCameraParams[struGsh.nLayer][t_imgHead[0].imgView][struGsh.nUnit].nIpAddr[3]));
//            t_imgIpStrList.append(QString("%1.%2.%3.%4").arg(struCnfg.struIpcCameraParams[struGsh.nLayer][t_imgHead[1].imgView][struGsh.nUnit].nIpAddr[0])
//                    .arg(struCnfg.struIpcCameraParams[struGsh.nLayer][t_imgHead[1].imgView][struGsh.nUnit].nIpAddr[1])
//                    .arg(struCnfg.struIpcCameraParams[struGsh.nLayer][t_imgHead[1].imgView][struGsh.nUnit].nIpAddr[2])
//                    .arg(struCnfg.struIpcCameraParams[struGsh.nLayer][t_imgHead[1].imgView][struGsh.nUnit].nIpAddr[3]));
//            camIpMap.insert(0,t_imgIpStrList.at(0));
//            camIpMap.insert(1,t_imgIpStrList.at(1));

//            udpSendTransStartStop(Start,TARGET_Mode,t_imgHead[0].imgView ,struGsh.nUnit);
            //            udpSendTransStartStop(Start,TARGET_Mode,t_imgHead[1].imgView ,struGsh.nUnit);
            udpSendTransStartStop(Start,TARGET_Mode,BOARD_TYPE_ALL_CAMERA,BOARD_TYPE_ALL_IN_CHAIN);
            memset(curImgcnt,0,sizeof(curImgcnt));
            memset(t_wantedPic,0,sizeof(t_wantedPic));
            memset(t_wantedPacket,0,sizeof(t_wantedPacket));
            memset(t_recvedDataSize,0,sizeof(t_recvedDataSize));

            memset(imageDataBuf,0,sizeof(imageDataBuf));
            while (isrunning)
            {
                if( checkUdpSocketStatues(1) < 0 )
                {
                    qDebug()<<"timeout";
                    continue;
                }
                /********************************原始数据*********************************/
                len = recvfrom(sockfd, (char *)imagePacketBuf, sizeof(imagePacketBuf), 0,(struct sockaddr*)&addr_local,&sin_size);
                if(len < PACKET_HEAD_LEN+14){
                    perror("targert fast recv");
                    continue;
                }
                QString addrInIp = QString::fromLocal8Bit(inet_ntoa(addr_local.sin_addr));
                QString addrInPort = QString::number(htons(addr_local.sin_port));
//                qDebug()<<"recv Ip:"<<addrInIp<<"Port:"<<addrInPort<<"Length:"<<len;

                if(!myFlow.getIpcCameraViewUnitInfo(view,unit,tmpIndex,addrInIp))
                    continue; //ip出错，解析出错

                t_imgHead[tmpIndex].imgView = view;
                t_imgHead[tmpIndex].imgUnit= unit;
                t_imgHead[tmpIndex].imgSN = (imagePacketBuf[PACKET_HEAD_LEN]<<24|imagePacketBuf[PACKET_HEAD_LEN+1]<<16\
                                            |imagePacketBuf[PACKET_HEAD_LEN+2]<<8|imagePacketBuf[PACKET_HEAD_LEN+3]);
                t_imgHead[tmpIndex].imgHeight = imagePacketBuf[PACKET_HEAD_LEN+4]<<8|imagePacketBuf[PACKET_HEAD_LEN+5];
                t_imgHead[tmpIndex].imgWidth = imagePacketBuf[PACKET_HEAD_LEN+6]<<8|imagePacketBuf[PACKET_HEAD_LEN+7];
                t_imgHead[tmpIndex].imgClass = imagePacketBuf[PACKET_HEAD_LEN+8];
                t_imgHead[tmpIndex].imgDim = imagePacketBuf[PACKET_HEAD_LEN+9];
                t_totalPacket[tmpIndex] = imagePacketBuf[PACKET_HEAD_LEN+10]<<8|imagePacketBuf[PACKET_HEAD_LEN+11];
                t_curPacket[tmpIndex] = imagePacketBuf[PACKET_HEAD_LEN+12]<<8|imagePacketBuf[PACKET_HEAD_LEN+13];

//                if(t_wantedPacket[tmpIndex] == 0){
//                    printf("%d  w:%d h%d recv %d/%d: ",tmpIndex, t_imgHead[tmpIndex].imgWidth ,\
//                           t_imgHead[tmpIndex].imgHeight, len, t_recvedDataSize[tmpIndex]);
//                    for(int i = PACKET_HEAD_LEN; i < PACKET_HEAD_LEN+13+16;i ++){
//                        printf("%02x ",imagePacketBuf[i]);
//                        if(i == PACKET_HEAD_LEN +13) printf("- ");
//                    }
//                    printf("\n");
//                }
#if 1 //暂时不考虑异常
                if(t_wantedPic[tmpIndex] == 0){
                    t_wantedPic[tmpIndex] = t_imgHead[tmpIndex].imgSN;
                    t_wantedPacket[tmpIndex] = t_curPacket[tmpIndex];
                }

                if(imagePacketBuf[0] != 0xA5 ||imagePacketBuf[1] != 0x5A
                        || t_wantedPic[tmpIndex] != t_imgHead[tmpIndex].imgSN
                        || t_wantedPacket[tmpIndex] != t_curPacket[tmpIndex]
                        || t_curPacket[tmpIndex] >= t_totalPacket[tmpIndex])
                {
                    qDebug()<<"tmpIndex"<<tmpIndex<<"wantedPic:"<<t_wantedPic[tmpIndex]<<"curPic"<<t_imgHead[tmpIndex].imgSN<<"wantedPacket:"\
                           <<t_wantedPacket[tmpIndex]<<"curPacket:"<<t_curPacket[tmpIndex]<<"totalPacket:"<<t_totalPacket[tmpIndex];
                    t_wantedPic[tmpIndex] = t_imgHead[tmpIndex].imgSN+1;
                    t_wantedPacket[tmpIndex] = 0;
                    continue;
                }
#endif
                t_wantedPacket[tmpIndex]++;
                if(t_curPacket[tmpIndex] == 0 ) {//第一包
                    t_recvedDataSize[tmpIndex] = sizeof(struct img_stream_head);
//                    printf("recved first packet %d \n",t_recvedDataSize[tmpIndex] );
                }

                memcpy(&targetImageDataBuf[tmpIndex][t_recvedDataSize[tmpIndex]],&imagePacketBuf[PACKET_HEAD_LEN+14], len -PACKET_HEAD_LEN-14);
                t_recvedDataSize[tmpIndex] += (len - PACKET_HEAD_LEN- 14);

//                printf("mmcp %d/%d: ",t_recvedDataSize[tmpIndex],len -PACKET_HEAD_LEN-13);
//                for(int i = 0; i < 48;i ++){
//                    printf("%02x ",targetImageDataBuf[tmpIndex][i]);
//                }
//                printf("\n");
                if(t_curPacket[tmpIndex] < t_totalPacket[tmpIndex]-1)
                    continue;
                if(t_curPacket[tmpIndex] > t_totalPacket[tmpIndex]-1)
                    printf("err packet %d/%d\n",t_curPacket[tmpIndex], t_totalPacket[tmpIndex]);

                t_wantedPic[tmpIndex] = t_imgHead[tmpIndex].imgSN+1;
                t_wantedPacket[tmpIndex] = 0;
//                t_recvedTotalSize[tmpIndex] = sizeof(t_imgHead)+t_imgHead[tmpIndex].imgDim*t_imgHead[tmpIndex].imgHeight*t_imgHead[tmpIndex].imgWidth;

                t_recvedTotalSize[tmpIndex] = sizeof(t_imgHead[tmpIndex])+t_imgHead[tmpIndex].imgDim*t_imgHead[tmpIndex].imgHeight*t_imgHead[tmpIndex].imgWidth;
                if(t_recvedTotalSize[tmpIndex] != t_recvedDataSize[tmpIndex])
                    printf("\033[31merr totalSize %d/%d \033[0m\n",t_recvedDataSize[tmpIndex],t_recvedTotalSize[tmpIndex]);

//                printf("tmpindex%d bf cp: ",tmpIndex);
//                for(int i = 24; i < 64;i ++){
//                    printf("%02x ",targetImageDataBuf[tmpIndex][i]);
//                }
//                printf("\n");
                memcpy(&targetImageDataBuf[tmpIndex][0],&t_imgHead[tmpIndex],sizeof(struct img_stream_head));
//                qDebug()<<"enqueue"<<t_imgHead[tmpIndex].imgSN<<t_imgHead[tmpIndex].imgWidth<<t_imgHead[tmpIndex].imgHeight<<t_imgHead[tmpIndex].imgDim<<t_recvedTotalSize[tmpIndex]<<t_recvedDataSize[tmpIndex];

                curImgcnt[view][unit]++;
                    qDebug()<<"img classid"<<t_imgHead[tmpIndex].imgClass;
                    if(struCnfp.bstaticClassAlarm[t_imgHead[tmpIndex].imgClass] == 1 )
                    {
                        struGsh.nAlarmStautsCurrent[ALARM_CLASS] = struCnfg.nAlarmValid[ALARM_CLASS];
                		myRingQueue.enqueue(targetImageDataBuf[tmpIndex],t_recvedTotalSize[tmpIndex]);
                    }
                t_recvedDataSize[tmpIndex] = 0;
                t_recvedTotalSize[tmpIndex] = 0;
                memset(targetImageDataBuf[tmpIndex], 0, sizeof(targetImageDataBuf[tmpIndex]));
//                printf("\naf cp: ");
//                for(int i = 24; i < 64;i ++){
//                    printf("%02x ",targetImageDataBuf[tmpIndex][i]);
//                }
//                printf("\n");
                viewList.clear();
                curImgcntList.clear();
                for(int tv = 0; tv < MAX_VIEW; tv++ )
                {
                    QString vImgcnt;
                    bool vExistImg = false;
                    if (struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[tv]  )
                    {
                        for(int tu= 0; tu < struCnfg.struLayerInfo[struGsh.nLayer].nViewUnitTotal[tv]; tu++)
                        {
                            vImgcnt += QString::number(curImgcnt[tv][tu]);
                            vImgcnt +="-";
                            if(struCnfg.struIpcCameraParams[struGsh.nLayer][tv][tu].nSaveImgEnable){
                                vExistImg = true;
                            }
                        }
                        if(vExistImg)
                        {
                            viewList.append(QString::number(tv));
                            curImgcntList.append(vImgcnt);
                        }
                    }
                }
                emit SignalImageRecvFinished(viewList,curImgcntList);
            }
            qDebug()<<"need save"<<t_imgHead[0].imgSN<<t_imgHead[1].imgSN;
            isrunning = 0;
            emit SignalImageRecvFinished(viewList,curImgcntList);
            return;
        }
    }
}
