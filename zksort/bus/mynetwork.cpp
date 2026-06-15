#include "mynetwork.h"
#include <QDebug>
#include <algorithm>
#include<fcntl.h>

static QMutex qtmutex;
static QWaitCondition qtcondition;
MyNetCptureThread *myNetCpture;

MyNetWorkThread::MyNetWorkThread(QThread *parent) : QThread(parent)
{
    curTotal = 0;
    retNum = 0;
    isrunning = false;
    dim = 3;
    curPixels = 2048;
    m_nNumOfImg = 0;
    currenttype = 0;
    mlayer = 0;
    mview = 0;
    munit = 0;
    rowPacketTotal = 0;
    waveBufSize = 0;
    sockfd = 0;
    imageRawBufSize = 0;
    imgCorrectHight = 0;
    materCnt = 0;
    totalCnt = 0;
    nBlowQueryFreq = 0;

    memset(waveDataBuf,0,sizeof(waveDataBuf));
    memset(imagePacketBuf,0,sizeof(imagePacketBuf));
    memset(imageDataBuf,0,sizeof(imageDataBuf));
    memset(blowCountsPacketBuf,0,sizeof(blowCountsPacketBuf));
    memset(blowCountsTempDataBuf,0,sizeof(blowCountsTempDataBuf));
}

int MyNetWorkThread::myGetTickCount()
{
#ifdef Q_OS_UNIX
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return(ts.tv_sec*1000+ts.tv_nsec/1000000);
#else
    return(GetTickCount());
#endif
}

void MyNetWorkThread::setSocket(int fd)
{
    sockfd = fd;
}
void MyNetWorkThread::setAddr_remote(sockaddr_in remote)
{
    memcpy(&r_addr,&remote,sizeof(sockaddr_in));
}

void MyNetWorkThread::setRecvType(int type,int layer,int view,int unit)
{
    currenttype = type;
    mlayer = layer;
    mview = view;
    munit = unit;
}

void MyNetWorkThread::updateCameraInfo(int view)
{
    if(view < 0) view = struGsh.nView;
    if (struCnfp.nImageSplice[struGsh.nLayer][view] >= 2){
        curImageRowCnt = myFlow.imgHeight*2;
    }else{
        curImageRowCnt = myFlow.imgHeight;
    }

    if(struGsh.nRemoteImageCaptureIsRun){
        dim =struCnfp.nSensorDim[lockedLayer][lockedview] ;
        curPixels = myFlow.getSensorPixels(lockedLayer, lockedview);
    }
    else{
        dim = struCnfp.nSensorDim[struGsh.nLayer][struGsh.nView];
        curPixels = myFlow.getSensorPixels(struGsh.nLayer, struGsh.nView);
    }

    myFlow.updateCameraTimeDivisionMode(struGsh.nLayer,struGsh.nView);
    td_dim = (struGsh.nCamTdMode > 0)? 2 : 1;
    waveBufSize = td_dim * dim * curPixels + 6;

    if(struCnfp.nSensorPixel[0][struGsh.nView] == SENSOR_PIXELS_2048_TD
            && struCnfp.nViewSensorType[0][struGsh.nView] == SENSOR_G3)
    {
        dim = struCnfp.nSensorDim[struGsh.nLayer][struGsh.nView];
        curPixels = 1024;
        td_dim = 5;
        waveBufSize = td_dim * dim * curPixels + 6;//2048*2+1024
        myFlow.imgHeight = 416;
        curImageRowCnt = myFlow.imgHeight;
    }
    else if(struCnfp.nSensorPixel[0][struGsh.nView] == SENSOR_PIXELS_2048_XTD
            && struCnfp.nViewSensorType[0][struGsh.nView] == SENSOR_G2)
    {
        dim = struCnfp.nSensorDim[struGsh.nLayer][struGsh.nView];
        curPixels = 2048;
        waveBufSize = dim * curPixels + 6;//2048
        myFlow.imgHeight = 832;
        curImageRowCnt = myFlow.imgHeight;
    }
    else if(struCnfp.nSensorPixel[0][struGsh.nView] == SENSOR_PIXELS_2048_GTD)
    {
        td_dim = 2;
        waveBufSize = dim * curPixels + 6;
        if(currenttype == CAPTURE_SINGLE_WAVE)
            waveBufSize *= 2;
        curImageRowCnt = myFlow.imgHeight*2; //2048*832*2
    }

  //  qDebug()<<"dim"<<dim <<"td_dim"<<td_dim<<"curImageRowCnt"<<curImageRowCnt<<"curPixels" <<curPixels<<"rowPacketTotal"<<rowPacketTotal<<"imageRawBufSize"<<imageRawBufSize <<"waveBufSize"<<waveBufSize;
}

int MyNetWorkThread::saveBMPFile(QString path,int w,int h)
{
    if(struCnfg.nFreeSpace < 100.0)
        return 0;

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
    size_t imageBuffSize= (((size_t)wWidth) * ((size_t)wHeight)) *dim;
    //设置BMP文件头
    bmfh.bfType = 0x4D42;//'BM'
    if(dim == 1){
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
    bmih.biBitCount = dim*8;
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
    if(dim == 1){
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
    return 1;
}

void MyNetWorkThread::waveDataPacketed(bool needFocus)
{
    int totalRed = 0, totalGreen=0, totalBlue=0, totalInf=0, curScale=0;
    QColor hsvColor;
    int h,s,v;
    int tempdata;

    switch(dim)
    {
    case 1:
        for(int i = 0; i < curPixels; i++)
        {
            struGsh.sRowRed[i]      = waveDataBuf[i*dim];
            struGsh.sRowTotal[i]    = struGsh.sRowRed[i];
            struGsh.sRowAveForFocusData[i] = struGsh.sRowRed[i];
        }
        break;
    case 2:
        for(int i = 0; i < curPixels; i++)
        {
            struGsh.sRowRed[i]      = waveDataBuf[i*dim];
            struGsh.sRowGreen[i]    = waveDataBuf[i*dim+1];

            struGsh.sRowTotal[i]    = waveDataBuf[i*2]*256+waveDataBuf[i*2+1];
            struGsh.sRowAveForFocusData[i] = (struGsh.sRowRed[i]+struGsh.sRowGreen[i])/2;
        }
        break;
    case 3:
        if(struGsh.nCamTdMode == 0) //非分时
        {
            for(int i = 0; i < curPixels; i++)
            {
                struGsh.sRowRed[i]      = waveDataBuf[i*dim];
                struGsh.sRowGreen[i]    = waveDataBuf[i*dim+1];
                struGsh.sRowBlue[i]     = waveDataBuf[i*dim+2];

                struGsh.sRowTotal[i]    = waveDataBuf[i*2]*256+waveDataBuf[i*2+1];
                struGsh.sRowAveForFocusData[i] = (struGsh.sRowRed[i]+struGsh.sRowGreen[i]+struGsh.sRowBlue[i])/3;
                myFlow.getHsv(struGsh.sRowRed[i],struGsh.sRowGreen[i],struGsh.sRowBlue[i], h, s, v);
                struGsh.sRowHue[i] = h/10;
                struGsh.sRowVaule[i] = v;
            }
        }
        else if(struGsh.nCamTdMode == 1) //分时可见
        {
            if(td_dim == 2) //1024-TD可见
            {
                for(int i = 0; i < curPixels; i++)
                {
                    struGsh.sRowRed[i]      = waveDataBuf[i*dim];
                    struGsh.sRowGreen[i]    = waveDataBuf[i*dim+1];
                    struGsh.sRowBlue[i]     = waveDataBuf[i*dim+2];

                    struGsh.sRowTotal[i]    = waveDataBuf[i*2]*256+waveDataBuf[i*2+1];
                    struGsh.sRowAveForFocusData[i] = (struGsh.sRowRed[i]+struGsh.sRowGreen[i]+struGsh.sRowBlue[i])/3;
                    myFlow.getHsv(struGsh.sRowRed[i],struGsh.sRowGreen[i],struGsh.sRowBlue[i], h, s, v);
                    struGsh.sRowHue[i] = h/10;
                    struGsh.sRowVaule[i] = v;
                }
            }
            else if(td_dim == 5) //2048-TD可见
            {
                for(int i = 0; i < curPixels*2; i++)
                {
                    struGsh.sRowRed[i]      = waveDataBuf[i*dim];
                    struGsh.sRowGreen[i]    = waveDataBuf[i*dim+1];
                    struGsh.sRowBlue[i]     = waveDataBuf[i*dim+2];

                    struGsh.sRowTotal[i]    = waveDataBuf[i*2]*256+waveDataBuf[i*2+1];
                    struGsh.sRowAveForFocusData[i] = (struGsh.sRowRed[i]+struGsh.sRowGreen[i]+struGsh.sRowBlue[i])/3;
                    myFlow.getHsv(struGsh.sRowRed[i],struGsh.sRowGreen[i],struGsh.sRowBlue[i], h, s, v);
                    struGsh.sRowHue[i] = h/10;
                    struGsh.sRowVaule[i] = v;
                }
            }
        }
        else if(struGsh.nCamTdMode == 2) //分时紫外
        {
            if(td_dim == 2) //1024-TD紫外
            {
                if(struCnfp.nSensorPixel[0][struGsh.nView] == SENSOR_PIXELS_2048_XTD
                        && struCnfp.nViewSensorType[0][struGsh.nView] == SENSOR_G2)
                {
                    for(int i = 0; i < curPixels; i=i+2)
                    {
                        struGsh.sRowRed[i/2]      = (waveDataBuf[dim*curPixels+i*dim]+waveDataBuf[dim*curPixels+(i+1)*dim])/2;
                        struGsh.sRowGreen[i/2]    = (waveDataBuf[dim*curPixels+i*dim+1]+waveDataBuf[dim*curPixels+(i+1)*dim+1])/2;
                        struGsh.sRowBlue[i/2]     = (waveDataBuf[dim*curPixels+i*dim+2]+waveDataBuf[dim*curPixels+(i+1)*dim+2])/2;

                        struGsh.sRowTotal[i/2]    = waveDataBuf[i/2*2]*256+waveDataBuf[i/2*2+1];
                        struGsh.sRowAveForFocusData[i/2] = (struGsh.sRowRed[i/2]+struGsh.sRowGreen[i/2]+struGsh.sRowBlue[i/2])/3;
                        myFlow.getHsv(struGsh.sRowRed[i/2],struGsh.sRowGreen[i/2],struGsh.sRowBlue[i/2], h, s, v);
                        struGsh.sRowHue[i/2] = h/10;
                        struGsh.sRowVaule[i/2] = v;
                    }
                    break;
                }

                for(int i = 0; i < curPixels; i++)
                {
                    struGsh.sRowRed[i]      = waveDataBuf[dim*curPixels+i*dim];
                    struGsh.sRowGreen[i]    = waveDataBuf[dim*curPixels+i*dim+1];
                    struGsh.sRowBlue[i]     = waveDataBuf[dim*curPixels+i*dim+2];

                    struGsh.sRowTotal[i]    = waveDataBuf[i*2]*256+waveDataBuf[i*2+1];
                    struGsh.sRowAveForFocusData[i] = (struGsh.sRowRed[i]+struGsh.sRowGreen[i]+struGsh.sRowBlue[i])/3;
                    myFlow.getHsv(struGsh.sRowRed[i],struGsh.sRowGreen[i],struGsh.sRowBlue[i], h, s, v);
                    struGsh.sRowHue[i] = h/10;
                    struGsh.sRowVaule[i] = v;
                }
            }
            else if(td_dim == 5)  //2048-TD紫外
            {
                for(int i = 0; i < curPixels; i++)
                {
                    struGsh.sRowRed[i]      = waveDataBuf[dim*curPixels*4+i*dim];
                    struGsh.sRowGreen[i]    = waveDataBuf[dim*curPixels*4+i*dim+1];
                    struGsh.sRowBlue[i]     = waveDataBuf[dim*curPixels*4+i*dim+2];

                    struGsh.sRowTotal[i]    = waveDataBuf[i*2]*256+waveDataBuf[i*2+1];
                    struGsh.sRowAveForFocusData[i] = (struGsh.sRowRed[i]+struGsh.sRowGreen[i]+struGsh.sRowBlue[i])/3;
                    myFlow.getHsv(struGsh.sRowRed[i],struGsh.sRowGreen[i],struGsh.sRowBlue[i], h, s, v);
                    struGsh.sRowHue[i] = h/10;
                    struGsh.sRowVaule[i] = v;
                }
            }
        }
        else if(struGsh.nCamTdMode == TD_2048_A || struGsh.nCamTdMode == TD_2048_B)
        {
            for(int i = 0; i < curPixels; i++)
            {
                if(struGsh.nTdMode == TD_1024_A )
                {
                    struGsh.sRowRed[i]      = waveDataBuf[i*dim];
                    struGsh.sRowGreen[i]    = waveDataBuf[i*dim+1];
                    struGsh.sRowBlue[i]     = waveDataBuf[i*dim+2];
                }
                else
                {
                    struGsh.sRowRed[i]      = waveDataBuf[dim*curPixels+i*dim];
                    struGsh.sRowGreen[i]    = waveDataBuf[dim*curPixels+i*dim+1];
                    struGsh.sRowBlue[i]     = waveDataBuf[dim*curPixels+i*dim+2];
                }

                struGsh.sRowTotal[i]    = waveDataBuf[i*2]*256+waveDataBuf[i*2+1];
                struGsh.sRowAveForFocusData[i] = (struGsh.sRowRed[i]+struGsh.sRowGreen[i]+struGsh.sRowBlue[i])/3;
                myFlow.getHsv(struGsh.sRowRed[i],struGsh.sRowGreen[i],struGsh.sRowBlue[i], h, s, v);
                struGsh.sRowHue[i] = h/10;
                struGsh.sRowVaule[i] = v;
            }
        }

        break;
    case 4:
        for(int i = 0; i < curPixels; i++)
        {
            struGsh.sRowInf[i]      = waveDataBuf[i*dim];
            struGsh.sRowRed[i]      = waveDataBuf[i*dim+1];
            struGsh.sRowGreen[i]    = waveDataBuf[i*dim+2];
            struGsh.sRowBlue[i]     = waveDataBuf[i*dim+3];

            struGsh.sRowTotal[i]    = waveDataBuf[i*2]*256+waveDataBuf[i*2+1];
            struGsh.sRowAveForFocusData[i] = (struGsh.sRowRed[i]+struGsh.sRowGreen[i]+struGsh.sRowBlue[i]+struGsh.sRowInf[i])/4;
            myFlow.getHsv(struGsh.sRowRed[i],struGsh.sRowGreen[i],struGsh.sRowBlue[i], h, s, v);
            struGsh.sRowHue[i] = h/10;
            struGsh.sRowVaule[i] = v;
        }
        break;
    default:
        for(int i = 0; i < curPixels; i++)
        {
            struGsh.sRowRed[i]      = waveDataBuf[i*dim];
            struGsh.sRowGreen[i]    = waveDataBuf[i*dim+1];
            struGsh.sRowBlue[i]     = waveDataBuf[i*dim+2];
            struGsh.sRowTotal[i]    = waveDataBuf[i*2]*256+waveDataBuf[i*2+1];;
            struGsh.sRowAveForFocusData[i] = (struGsh.sRowRed[i]+struGsh.sRowGreen[i]+struGsh.sRowBlue[i])/3;
            myFlow.getHsv(struGsh.sRowRed[i],struGsh.sRowGreen[i],struGsh.sRowBlue[i], h, s, v);
            struGsh.sRowHue[i] = h/10;
            struGsh.sRowVaule[i] = v;
        }
        break;
    }

    int rat = 1;
    /* 计算各颜色平均值 */
    if(struGsh.nCamTdMode == 2 && td_dim == 2 && struCnfp.nSensorPixel[0][struGsh.nView] == SENSOR_PIXELS_2048_XTD
            && struCnfp.nViewSensorType[0][struGsh.nView] == SENSOR_G2)
    {
        rat = 2;
    }

    curScale = abs(struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nChannelEnd[struGsh.nUnit]-struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nChannelBegin[struGsh.nUnit]);
    curScale = (curScale == 0) ? 1 : curScale/rat;
    for(int i = struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nChannelBegin[struGsh.nUnit]/rat; i <= struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nChannelEnd[struGsh.nUnit]/rat; i++)
    {
        totalRed += struGsh.sRowRed[i];
        totalGreen += struGsh.sRowGreen[i];
        totalBlue += struGsh.sRowBlue[i];
        totalInf += struGsh.sRowInf[i];
    }

    struGsh.fRowAveRed = (float)totalRed/curScale;
    struGsh.fRowAveGreen = (float)totalGreen/curScale;
    struGsh.fRowAveBlue = (float)totalBlue/curScale;
    struGsh.fRowAveInf = (float)totalInf/curScale;

#if 1
    if(needFocus)
    {
        int highDataLen = 0;
        int pixelCount = 0;

        if(struGsh.focusAveEndPixel >= struGsh.focusAveBeginPixel)
        {
            pixelCount = struGsh.focusAveEndPixel-struGsh.focusAveBeginPixel;
            for(int i = 0;  i < pixelCount; i++)
            {
                struGsh.sRowAveForFocusData[i] = abs(struGsh.sRowAveForFocusData[struGsh.focusAveBeginPixel+i]-struGsh.sRowAveForFocusData[struGsh.focusAveBeginPixel+i+1]);
            }

            for(int i = 0; i < SENSOR_MAX_PIXELS; i++)
            {
                struGsh.sRowFocusHighData[i]=0;
            }
            myFlow.dataSort(struGsh.sRowAveForFocusData, pixelCount, 1, 0);

            highDataLen = SENSOR_FOCUS_GRADE_DADA_LEN*pixelCount;

            memcpy(struGsh.sRowFocusHighData, struGsh.sRowAveForFocusData, highDataLen);
            struGsh.focusAveVavle = 0;
            for(int i = 0; i < highDataLen; i++)
            {
                struGsh.focusAveVavle += struGsh.sRowFocusHighData[i];
            }

            struGsh.focusAveVavle = (float)struGsh.focusAveVavle/highDataLen;
        }

        if(struGsh.focusAveEndPixel2 >= struGsh.focusAveBeginPixel2)
        {
            pixelCount = struGsh.focusAveEndPixel2-struGsh.focusAveBeginPixel2;
            for(int i = 0;  i < pixelCount; i++)
            {
                struGsh.sRowAveForFocusData[i] = abs(struGsh.sRowAveForFocusData[struGsh.focusAveBeginPixel2+i]-struGsh.sRowAveForFocusData[struGsh.focusAveBeginPixel2+i+1]);
            }

            for(int i = 0; i < SENSOR_MAX_PIXELS; i++)
            {
                struGsh.sRowFocusHighData[i]=0;
            }
            myFlow.dataSort(struGsh.sRowAveForFocusData, pixelCount, 1, 0);

            highDataLen = SENSOR_FOCUS_GRADE_DADA_LEN*pixelCount;

            memcpy(struGsh.sRowFocusHighData, struGsh.sRowAveForFocusData, highDataLen);
            struGsh.focusAveVavle2 = 0;
            for(int i = 0; i < highDataLen; i++)
            {
                struGsh.focusAveVavle2 += struGsh.sRowFocusHighData[i];
            }

            struGsh.focusAveVavle2 = (float)struGsh.focusAveVavle2/highDataLen;
        }
    }
#endif

}

/* 根据维度进行图像数据包的组包 */
void MyNetWorkThread::imageDataPacketed(int correctHeight)
{
    memset(imageDataBuf,0,sizeof(imageDataBuf));
    memset(tmpBuf,0,sizeof(tmpBuf));
    //i: 包数
    for(int i = 0 ;i < correctHeight; i++)
    {
        memcpy(&tmpBuf[i*(waveBufSize-6)], &imagePacketBuf[i*waveBufSize +6], waveBufSize - 6);
    }
    float datafloat;
    int tempdata = 0;

    if (!struCnfp.nImageSplice[struGsh.nLayer][struGsh.nView]){
        switch(dim)
        {
        case 1:
            for(int i = 0;i < correctHeight;i++)
            {
                for(int j =0;j< curPixels;j++)
                {
                    imageDataBuf[i*curPixels*4 +j*4]   = tmpBuf[i*curPixels*dim+j];
                    imageDataBuf[i*curPixels*4 +j*4+1] = tmpBuf[i*curPixels*dim+j];
                    imageDataBuf[i*curPixels*4 +j*4+2] = tmpBuf[i*curPixels*dim+j];
                    imageDataBuf[i*curPixels*4 +j*4+3]   = 255;
                }
            }
            break;
        case 2:
            for(int i = 0;i < correctHeight;i++)
            {
                for(int j =0;j< curPixels;j++)  //512*2
                {
                    //                    imageDataBuf[i*curPixels*4 +j*4]   = 0;
                    //                    imageDataBuf[i*curPixels*4 +j*4+1] = tmpBuf[i*curPixels*dim+j*dim+1];
                    //                    imageDataBuf[i*curPixels*4 +j*4+2] = tmpBuf[i*curPixels*dim+j*dim];
                    //                    imageDataBuf[i*curPixels*4 +j*4+3]   = 255;
                    imageDataBuf[i*curPixels*3 +j*3]   = 0;
                    imageDataBuf[i*curPixels*3 +j*3+1] = tmpBuf[i*curPixels*dim+j*dim+1];
                    imageDataBuf[i*curPixels*3 +j*3+2] = tmpBuf[i*curPixels*dim+j*dim];
                }
            }
            break;
        case 3:
            if(struCnfp.nSensorPixel[0][struGsh.nView] == SENSOR_PIXELS_2048_XTD
                    && struCnfp.nViewSensorType[0][struGsh.nView] == SENSOR_G2)
            {
                for(int i = 0;i < correctHeight;i++)
                {
                    for(int j = 0;j < curPixels;j++)  //512*2
                    {
                        imageDataBuf[i*curPixels*3+j*3]   = tmpBuf[i*curPixels*dim+j*dim+2];
                        imageDataBuf[i*curPixels*3+j*3+1] = tmpBuf[i*curPixels*dim+j*dim+1];
                        imageDataBuf[i*curPixels*3+j*3+2] = tmpBuf[i*curPixels*dim+j*dim];
                    }
                }
                memcpy(tmpBuf,imageDataBuf,sizeof(imageDataBuf));
                for(int i = 0;i < correctHeight;i++)
                {
                    for(int j =0;j< curPixels;j++)
                    {
                        if(i % 2 == 0)//可见
                        {
                            imageDataBuf[(i+1)*curPixels*3+j*3]   = tmpBuf[i*curPixels*3+j*3];
                            imageDataBuf[(i+1)*curPixels*3+j*3+1] = tmpBuf[i*curPixels*3+j*3+1];
                            imageDataBuf[(i+1)*curPixels*3+j*3+2] = tmpBuf[i*curPixels*3+j*3+2];
                        }
                        else//紫外
                        {
                            if(j%2 == 0)
                            {
                                imageDataBuf[correctHeight*curPixels*3+i/2*curPixels/2*3+j/2*3]   = (tmpBuf[i*curPixels*dim+j*dim]+tmpBuf[i*curPixels*dim+(j+1)*dim])/2;
                                imageDataBuf[correctHeight*curPixels*3+i/2*curPixels/2*3+j/2*3+1] = (tmpBuf[i*curPixels*dim+j*dim+1]+tmpBuf[i*curPixels*dim+(j+1)*dim+1])/2;
                                imageDataBuf[correctHeight*curPixels*3+i/2*curPixels/2*3+j/2*3+2] = (tmpBuf[i*curPixels*dim+j*dim+2]+tmpBuf[i*curPixels*dim+(j+1)*dim+2])/2;
                            }
                        }
                    }
                    //                    if(i % 2 == 0)//可见需要将416个偶数行double成832行常规图像
                    //                    {
                    //                        memcpy(&imageDataBuf[(i+1)*curPixels*3],&tmpBuf[i*curPixels*3],curPixels*3)
                    //                    }
                    //                    if(i % 2 == 1)//紫外需要将416*2048的奇数行合并成416*1024的紫外图像
                    //                    {
                    //                        memcpy(&imageDataBuf[correctHeight*curPixels*3+(i-1)*curPixels*3],&tmpBuf[i*curPixels*3],curPixels*3)
                    //                    }
                }
                break;
            }
            else if(struGsh.nCamTdMode == TD_2048_A || struGsh.nCamTdMode == TD_2048_B) // 2048GTD
            {
                for(int i = 0;i < correctHeight;i++) //1664
                {
                    for(int j =0;j< curPixels;j++)
                    {
                        if(i % 2 == 0)//可见
                        {
                            imageDataBuf[i/2*curPixels*3 +j*3]   = tmpBuf[i*curPixels*dim+j*dim+2];
                            imageDataBuf[i/2*curPixels*3 +j*3+1] = tmpBuf[i*curPixels*dim+j*dim+1];
                            imageDataBuf[i/2*curPixels*3 +j*3+2] = tmpBuf[i*curPixels*dim+j*dim];
                        }
                        else//紫外
                        {
                            imageDataBuf[(i/2+correctHeight/2)*curPixels*3 +j*3]   = tmpBuf[i*curPixels*dim+j*dim+2];
                            imageDataBuf[(i/2+correctHeight/2)*curPixels*3 +j*3+1] = tmpBuf[i*curPixels*dim+j*dim+1];
                            imageDataBuf[(i/2+correctHeight/2)*curPixels*3 +j*3+2] = tmpBuf[i*curPixels*dim+j*dim];
                        }
                    }
                }
                break;
            }
            else
            {


                for(int i = 0;i < correctHeight;i++)
                {
                    for(int j = 0;j < curPixels*td_dim;j = j+td_dim)  //512*2
                    {
                        for(int k = 0; k < td_dim; k++)
                        {
                            imageDataBuf[i*curPixels*td_dim*3+(j/td_dim*td_dim+k)*3]   = tmpBuf[i*curPixels*td_dim*dim+(j/td_dim*td_dim+k)*dim+2];
                            imageDataBuf[i*curPixels*td_dim*3+(j/td_dim*td_dim+k)*3+1] = tmpBuf[i*curPixels*td_dim*dim+(j/td_dim*td_dim+k)*dim+1];
                            imageDataBuf[i*curPixels*td_dim*3+(j/td_dim*td_dim+k)*3+2] = tmpBuf[i*curPixels*td_dim*dim+(j/td_dim*td_dim+k)*dim];
                        }
                    }
                }
                memcpy(tmpBuf,imageDataBuf,sizeof(imageDataBuf));
                for(int i = 0;i < correctHeight*td_dim;i=i+td_dim)
                {
                    memcpy(&imageDataBuf[i/td_dim*(td_dim-1)*curPixels*3],&tmpBuf[(i/td_dim*td_dim+0)*curPixels*3], curPixels*3*(td_dim-1));
                    memcpy(&imageDataBuf[(i/td_dim+correctHeight*(td_dim-1))*curPixels*3],&tmpBuf[(i/td_dim*td_dim+td_dim-1)*curPixels*3], curPixels*3);
                }
                break;
            }
        case 4:
            for(int i = 0;i < correctHeight;i++)
            {
                for(int j =0;j< curPixels;j++)
                {
                    imageDataBuf[i*curPixels*4 +j*4]   = tmpBuf[i*curPixels*dim+j*dim+3];
                    imageDataBuf[i*curPixels*4 +j*4+1] = tmpBuf[i*curPixels*dim+j*dim+2];
                    imageDataBuf[i*curPixels*4 +j*4+2] = tmpBuf[i*curPixels*dim+j*dim+1];
                    imageDataBuf[i*curPixels*4 +j*4+3] = tmpBuf[i*curPixels*dim+j*dim+0];   //NRGB格式
                }
            }
            break;
        }
    }
    else{//图像拼接
        switch(dim)
        {
        case 1:
            for(int i = 0;i < correctHeight;i++)
            {
                for(int j =0;j< curPixels;j++)
                {
                    if( j % 2 != 0) continue;
                    imageDataBuf[i*curPixels/2*4 +j/2*4]   = (tmpBuf[i*curPixels*dim+j]+tmpBuf[i*curPixels*dim+j+1])/2;
                    imageDataBuf[i*curPixels/2*4 +j/2*4+1] = (tmpBuf[i*curPixels*dim+j]+tmpBuf[i*curPixels*dim+j+1])/2;
                    imageDataBuf[i*curPixels/2*4 +j/2*4+2] = (tmpBuf[i*curPixels*dim+j]+tmpBuf[i*curPixels*dim+j+1])/2;
                    imageDataBuf[i*curPixels/2*4 +j/2*4+3]   = 255;
                }
            }
            break;
        case 2:
            for(int i = 0;i < correctHeight;i++)
            {
                for(int j =0;j< curPixels;j++)  //512*2
                {
                    if( j % 2 != 0) continue;
                    imageDataBuf[i*curPixels/2*4 +j/2*4]   = 0;
                    imageDataBuf[i*curPixels/2*4 +j/2*4+1] = (tmpBuf[i*curPixels*dim+j*dim+1]+tmpBuf[i*curPixels*dim+(j+1)*dim+1])/2;
                    imageDataBuf[i*curPixels/2*4 +j/2*4+2] = (tmpBuf[i*curPixels*dim+j*dim]+tmpBuf[i*curPixels*dim+(j+1)*dim])/2;
                    imageDataBuf[i*curPixels/2*4 +j/2*4+3]   = 255;
                }
            }
            break;
        case 3:
            for(int i = 0;i < correctHeight;i++)
            {
                if (struCnfp.nImageSplice[struGsh.nLayer][struGsh.nView] == 2){
                    for(int j =0;j< curPixels;j++)
                    {
                        if(( j % 2 != 0) || (i % 2 != 0)) continue;
                        tempdata = tmpBuf[i*curPixels*dim+j*dim+2]+tmpBuf[i*curPixels*dim+(j+1)*dim+2]+tmpBuf[(i+1)*curPixels*dim+j*dim+2]+tmpBuf[(i+1)*curPixels*dim+(j+1)*dim+2];
                        datafloat = tempdata/4.0;
                        tempdata = (int)(datafloat+0.5);
                        tempdata = tempdata > 255?255:tempdata;
                        imageDataBuf[i/2*curPixels/2*4 +j/2*4]  = tempdata;
                        tempdata = tmpBuf[i*curPixels*dim+j*dim+1]+tmpBuf[i*curPixels*dim+(j+1)*dim+1]+tmpBuf[(i+1)*curPixels*dim+j*dim+1]+tmpBuf[(i+1)*curPixels*dim+(j+1)*dim+1];
                        datafloat = tempdata/4.0;
                        tempdata = (int)(datafloat+0.5);
                        tempdata = tempdata > 255?255:tempdata;
                        imageDataBuf[i/2*curPixels/2*4 +j/2*4+1] = tempdata;
                        tempdata = tmpBuf[i*curPixels*dim+j*dim]+tmpBuf[i*curPixels*dim+(j+1)*dim]+tmpBuf[(i+1)*curPixels*dim+j*dim]+tmpBuf[(i+1)*curPixels*dim+(j+1)*dim];
                        datafloat = tempdata/4.0;
                        tempdata = (int)(datafloat+0.5);
                        tempdata = tempdata > 255?255:tempdata;
                        imageDataBuf[i/2*curPixels/2*4 +j/2*4+2]  = tempdata;
                        imageDataBuf[i/2*curPixels/2*4 +j/2*4+3]   = 255;
                    }
                }
                else if (struCnfp.nImageSplice[struGsh.nLayer][struGsh.nView] == 3){
                    for(int j =0;j< curPixels;j++)
                    {
                        if(( j % 2 != 0) || (i % 2 != 0)) continue;
                        tempdata = tmpBuf[i*curPixels*dim+j*dim+2]+tmpBuf[i*curPixels*dim+(j+1)*dim+2]+tmpBuf[(i+1)*curPixels*dim+j*dim+2]+tmpBuf[(i+1)*curPixels*dim+(j+1)*dim+2];
                        datafloat = tempdata/2.0;
                        tempdata = (int)(datafloat+0.5);
                        tempdata = tempdata > 255?255:tempdata;
                        imageDataBuf[i/2*curPixels/2*4 +j/2*4]  = tempdata;
                        tempdata = tmpBuf[i*curPixels*dim+j*dim+1]+tmpBuf[i*curPixels*dim+(j+1)*dim+1]+tmpBuf[(i+1)*curPixels*dim+j*dim+1]+tmpBuf[(i+1)*curPixels*dim+(j+1)*dim+1];
                        datafloat = tempdata/2.0;
                        tempdata = (int)(datafloat+0.5);
                        tempdata = tempdata > 255?255:tempdata;
                        imageDataBuf[i/2*curPixels/2*4 +j/2*4+1] = tempdata;
                        tempdata = tmpBuf[i*curPixels*dim+j*dim]+tmpBuf[i*curPixels*dim+(j+1)*dim]+tmpBuf[(i+1)*curPixels*dim+j*dim]+tmpBuf[(i+1)*curPixels*dim+(j+1)*dim];
                        datafloat = tempdata/2.0;
                        tempdata = (int)(datafloat+0.5);
                        tempdata = tempdata > 255?255:tempdata;
                        imageDataBuf[i/2*curPixels/2*4 +j/2*4+2]  = tempdata;
                        imageDataBuf[i/2*curPixels/2*4 +j/2*4+3]   = 255;
                    }
                }
                else{
                    for(int j =0;j< curPixels;j++)  //512*2
                    {
                        if( j % 2 != 0) continue;
                        imageDataBuf[i*curPixels/2*4 +j/2*4]   = (tmpBuf[i*curPixels*dim+j*dim+2]+tmpBuf[i*curPixels*dim+(j+1)*dim+2])/2;
                        imageDataBuf[i*curPixels/2*4 +j/2*4+1] = (tmpBuf[i*curPixels*dim+j*dim+1]+tmpBuf[i*curPixels*dim+(j+1)*dim+1])/2;
                        imageDataBuf[i*curPixels/2*4 +j/2*4+2] = (tmpBuf[i*curPixels*dim+j*dim]+tmpBuf[i*curPixels*dim+(j+1)*dim])/2;
                        imageDataBuf[i*curPixels/2*4 +j/2*4+3]   = 255;
                    }
                }
            }
            break;
        case 4:
            for(int i = 0;i < correctHeight;i++)
            {
                for(int j =0;j< curPixels;j++)
                {
                    if( j % 2 != 0) continue;
                    imageDataBuf[i*curPixels/2*4 +j/2*4]   = (tmpBuf[i*curPixels*dim+j*dim+3]+tmpBuf[i*curPixels*dim+(j+1)*dim+3])/2;
                    imageDataBuf[i*curPixels/2*4 +j/2*4+1] = (tmpBuf[i*curPixels*dim+j*dim+2]+tmpBuf[i*curPixels*dim+(j+1)*dim+2])/2;
                    imageDataBuf[i*curPixels/2*4 +j/2*4+2] = (tmpBuf[i*curPixels*dim+j*dim+1]+tmpBuf[i*curPixels*dim+(j+1)*dim+1])/2;
                    imageDataBuf[i*curPixels/2*4 +j/2*4+3] = (tmpBuf[i*curPixels*dim+j*dim]+tmpBuf[i*curPixels*dim+(j+1)*dim])/2;;   //NRGB格式
                }
            }
            break;
        }
    }

    //远程采图时像元划分用红色标记
    //    if (struGsh.nRemoteImageCaptureIsRun){
    //        int begin = struCnfg.struLayerInfo[lockedLayer].stuCameraInfo[lockedview].nChannelBegin[lockedunit];
    //        int end = struCnfg.struLayerInfo[lockedLayer].stuCameraInfo[lockedview].nChannelEnd[lockedunit];
    //        if(struCnfp.nImageSplice[lockedLayer][lockedview])
    //        {
    //            begin /= 2;
    //            end /= 2;
    //        }
    //        for(int i = 0;i < correctHeight;i++)
    //        {
    //            imageDataBuf[i*curPixels*4 +begin*4]   = 0;
    //            imageDataBuf[i*curPixels*4 +begin*4+1] = 255;
    //            imageDataBuf[i*curPixels*4 +begin*4+2] = 0;
    //            imageDataBuf[i*curPixels*4 +begin*4+3] = 255;

    //            imageDataBuf[i*curPixels*4 +end*4]   = 0;
    //            imageDataBuf[i*curPixels*4 +end*4+1] = 255;
    //            imageDataBuf[i*curPixels*4 +end*4+2] = 0;
    //            imageDataBuf[i*curPixels*4 +end*4+3] = 255;
    //        }
    //    }
}


/*
 * 校验网络获取的数据包，准备可靠的图像数据
 */
int MyNetWorkThread::imageCheckSum(int curHeight)
{
    int lastIndex;

    if (curHeight == 0)
        return 0;

    for(int i = 0; i < curHeight; i++) //图像行号
    {
        int tmp = i * waveBufSize; //i行头

        if( (imagePacketBuf[tmp] != 0xFF) || (imagePacketBuf[tmp + 1] != 0x00) || (imagePacketBuf[tmp + 2] != 0xA5)
                || (imagePacketBuf[tmp + 3] != 0x5A)|| (imagePacketBuf[tmp + 4] != 0xC3)|| (imagePacketBuf[tmp + 5] != 0x3C))// 判断包头、包尾、包序号
        {
            memset(&imagePacketBuf[tmp],0,sizeof(imagePacketBuf)- tmp);
            imageDataPacketed(i);
            return i;           //返回异常行号
        }
    }
    imageDataPacketed(curHeight);
    return curHeight;
}

/* 在线程中每次读取前调用，对超时进行处理 */
int MyNetWorkThread::checkUdpSocketStatues(int seconds)
{
    struct timeval timeout;
    timeout.tv_sec  = seconds;
    timeout.tv_usec = 500*1000;
    fd_set rds;
    FD_ZERO (&rds);
    FD_SET(sockfd, &rds);
    int ret;
    ret = select(sockfd+1,&rds,NULL,NULL,&timeout);
    if(ret == -1)
    {
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

// flag 0获取图像，1上传图像，仅在同步图像获取有效
// view 本次获取哪一视
void MyNetWorkThread::serialSendTransStartStop(int flag,int view)
{
    udpSendTransStartStop(flag,view);
    return;

    //    if(struCnfg.nDPMachine){
    //        udpSendTransStartStop(flag,view);
    //        return;
    //    }

    int board;  // 0x00表示当前 视的所有相机板
    int nInt=0, nUnitAddr;
    char data[SEND_PACKET_DATA_SIZE];

    memset(data, 0, SEND_PACKET_DATA_SIZE);
    data[8] = struGsh.bFilterBackground;
    data[9] = struGsh.nImageCapTimeout;

    switch(currenttype)
    {
    case CAPTURE_SINGLE_WAVE:
        data[0] = 0;
        break;
    case CAPTURE_IMAGE:
    case CAPTURE_IAMGE_CONTINUOUS:
    case CAPTURE_IMAGE_TEMP:
        if(struCnfp.nImageSplice[struGsh.nLayer][struGsh.nView]  >= 2){
            data[0] = 3;
        }else
            data[0] = 1;
        break;
    case CAPTURE_IMAGE_SYNC:
        data[0] = (flag == 0)? 0 : 1;
        break;
    case CAPTURE_BACKGROUND_WAVE:
        data[0] = 0;
        data[3] = 1;
        break;
    case CAPTURE_IPC_EJECT:
        data[0] = 6;
        break;
    default:
        data[0] = 0;
        break;
    }

    switch (currenttype) {
    case CAPTURE_SINGLE_WAVE:
    case CAPTURE_BACKGROUND_WAVE:
    case CAPTURE_IPC_EJECT:
        //正在清灰，不请求波形数据
        if (struGsh.bFlagWiping == 1) return;
    case CAPTURE_IMAGE:
    case CAPTURE_IAMGE_CONTINUOUS:
    case CAPTURE_IMAGE_TEMP:
        data[1] = struGsh.nImageCapTrigValue/256;
        data[2] = struGsh.nImageCapTrigValue%256;

        //远程采图时支持指定层视通道
        if (struGsh.nRemoteImageCaptureIsRun){
            data[1] = 0;
            data[2] = 0;
            board = struCnfg.struLayerInfo[lockedLayer].nViewBoardType[lockedview];
            nUnitAddr = struCnfg.struLayerInfo[lockedLayer].stuViewInfo[lockedview].nViewUnitId[lockedunit];
        }
        else{
            if(struCnfp.nImageSplice[struGsh.nLayer][struGsh.nView] >= 2)
            {
                data[4] = 0;
                data[5] = ((myFlow.imgHeight*2)&0xff00)>>8;
                data[6] =  (myFlow.imgHeight*2)&0xff;
            }
            board = struCnfg.struLayerInfo[struGsh.nLayer].nViewBoardType[struGsh.nView];
            nUnitAddr = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].nViewUnitId[struGsh.nUnit];
        }
        // if(struGsh.nPageSm == SM_IMAGE_CAPTURE_WIDGET)
        mySerial.pushCom1CmdData(CMD_CAMERA_WAVE_CAPTURE, nInt, board, nUnitAddr, data, 1);

        break;
    case CAPTURE_IMAGE_SYNC:
        if(flag == 0){
            mySerial.pushCom1CmdData(CMD_CAMERA_IMAGE_CAPTURE_SYNC, nInt, BOARD_TYPE_ALL_CAMERA, BOARD_TYPE_ALL_IN_CHAIN, data, 1);
        }
        else{
            if (struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[view])
            {
                board = struCnfg.struLayerInfo[struGsh.nLayer].nViewBoardType[view];
                nUnitAddr = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[view].nViewUnitId[struGsh.nUnit];

                mySerial.pushCom1CmdData(CMD_CAMERA_IMAGE_CAPTURE_SYNC, nInt, board, nUnitAddr, data, 1);
            }
        }
        break;

    case CAPTURE_BLOW_COUNTS:
        mySerial.pushCom1CmdData(CMD_INT_BLOW_COUNTS_CAPTURE, nInt, BOARD_TYPE_INTERFACE, BOARD_TYPE_ALL_IN_CHAIN, data, 1);
        break;

    case CAPTURE_MULTI_IMAGE:
        data[0] = 4;
        data[1] = struGsh.nImageCapTrigValue/256;
        data[2] = struGsh.nImageCapTrigValue%256;
        data[4] = (myFlow.imgHeight*struGsh.imgCaptureCnt &0xff0000)>>16;
        data[5] = (myFlow.imgHeight*struGsh.imgCaptureCnt &0xff00)>>8;
        data[6] =  myFlow.imgHeight*struGsh.imgCaptureCnt &0xff;

        board = struCnfg.struLayerInfo[struGsh.nLayer].nViewBoardType[struGsh.nView];
        nUnitAddr = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].nViewUnitId[struGsh.nUnit];
        mySerial.pushCom1CmdData(CMD_CAMERA_WAVE_CAPTURE, nInt, board, nUnitAddr, data, 1);
        break;

    case CAPTURE_MULTI_IMAGE_GET:
        data[0] = 5;
        data[1] = 0;
        data[2] = 0;
        data[4] = 0;
        data[5] = myFlow.imgHeight/256;
        data[6] = myFlow.imgHeight%256;

        board = struCnfg.struLayerInfo[struGsh.nLayer].nViewBoardType[struGsh.nView];
        nUnitAddr = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].nViewUnitId[struGsh.nUnit];
        mySerial.pushCom1CmdData(CMD_CAMERA_WAVE_CAPTURE, nInt, board, nUnitAddr, data, 1);
        break;

    case CAPTURE_MULTI_IMAGE_SYNC:
    case CAPTURE_IAMGE_CONTINUOUS_SYNC:
        data[0] = 4;
        data[1] = struGsh.nImageCapTrigValue/256;
        data[2] = struGsh.nImageCapTrigValue%256;
        data[4] = (myFlow.imgHeight*struGsh.imgCaptureCnt &0xff0000)>>16;
        data[5] = (myFlow.imgHeight*struGsh.imgCaptureCnt &0xff00)>>8;
        data[6] =  myFlow.imgHeight*struGsh.imgCaptureCnt &0xff;

        mySerial.pushCom1CmdData(CMD_CAMERA_IMAGE_CAPTURE_SYNC, nInt, BOARD_TYPE_ALL_CAMERA, BOARD_TYPE_ALL_IN_CHAIN, data, 1);
        break;

    case CAPTURE_MULTI_IMAGE_GET_SYNC:
        data[0] = 5;
        data[1] = 0;
        data[2] = 0;
        data[4] = 0;
        data[5] = myFlow.imgHeight/256;
        data[6] = myFlow.imgHeight%256;

        if (struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[view])
        {
            board = struCnfg.struLayerInfo[struGsh.nLayer].nViewBoardType[view];
            nUnitAddr = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[view].nViewUnitId[struGsh.nUnit];
            mySerial.pushCom1CmdData(CMD_CAMERA_IMAGE_CAPTURE_SYNC, nInt, board, nUnitAddr, data, 1);
        }
        break;
    default:
        break;
    }
}


void MyNetWorkThread::sendTransIncreaseBlowCounts()
{
    int nInt=0;
    char data[SEND_PACKET_DATA_SIZE];
    memset(data, 0, SEND_PACKET_DATA_SIZE);

    if(struCnfg.nDPMachine)
    {
        udpSendCmdData(CMD_INT_BLOW_COUNTS_CAPTURE_2, nInt, BOARD_TYPE_INTERFACE, BOARD_TYPE_ALL_IN_CHAIN, data, 1);
    }
    else
    {
        mySerial.pushCom1CmdData(CMD_INT_BLOW_COUNTS_CAPTURE_2, nInt, BOARD_TYPE_INTERFACE, BOARD_TYPE_ALL_IN_CHAIN, data, 1);
    }
}

/*
 *  用在下发串口指令前清除本地缓存
 */
void MyNetWorkThread::cleanUdpSocket(int seconds)
{
#if 0
    int num;
    struct sockaddr_in client;
    socklen_t sin_size = sizeof(struct sockaddr_in);
    setUdpRecvTimeOut(0,10);
    int e;
    while(1)
    {
        num = recv(sockfd,(char *)dirtydatabuf,sizeof(dirtydatabuf),0);
#ifdef Q_OS_WIN32
        e = GetTickCount();
#endif
        num = recv(sockfd,(char *)dirtydatabuf,sizeof(dirtydatabuf),0);

        if(num < 520) break;
    }
#endif


#if 1 //采用非阻塞方式判断套接字缓冲区是否有数据可读
    struct timeval timeout;
    fd_set rds;
    int ret = -1;
    while(1)
    {
        FD_ZERO (&rds);
        FD_SET(sockfd, &rds);
        timeout.tv_sec  = 0;
        timeout.tv_usec = 0;
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
        if(FD_ISSET(sockfd,&rds))
        {
            ret = recv(sockfd,(char *)dirtydatabuf,sizeof(dirtydatabuf),0);
            if(ret == -1){
                logger()->info("recv err");
                emit SignalSocketFailed();
                myFlow.msleep(200);
                return;
            }
            if (ret == 0)
            {
                return;
            }
        }
    }
#endif

}


void MyNetWorkThread::udpSendTransStartStop(int flag,int view)
{
    int board;  // 0x00表示当前 视的所有相机板
    int nInt=0, nUnitAddr;
    char data[SEND_PACKET_DATA_SIZE];
    int imgRatio = 1;

    memset(data, 0, SEND_PACKET_DATA_SIZE);
    data[8] = struGsh.bFilterBackground;
    data[9] = struGsh.nImageCapTimeout;

    imgRatio = struCnfp.nImageSplice[struGsh.nLayer][struGsh.nView] < 2 ? 1: 2;

    if(struGsh.nCamTdMode == TD_2048_A ||struGsh.nCamTdMode == TD_2048_B ) imgRatio *= 2; //分时的直接double


    switch(currenttype)
    {
    case CAPTURE_SINGLE_WAVE:
        data[0] = 0;
        if(struCnfp.nSensorPixel[0][struGsh.nView] == SENSOR_PIXELS_2048_XTD
                && struCnfp.nViewSensorType[0][struGsh.nView] == SENSOR_G2)
        {
            data[0] = 3;//自定义行数
            data[1] = 0;
            data[2] = 0;
            data[6] = 2;//XFS波形单次获取2行
        }
        else if(struGsh.nCamTdMode == TD_2048_A ||struGsh.nCamTdMode == TD_2048_B )
        {
            data[0] = 3;
            data[4] = 0;
            data[5] = ((1*imgRatio)&0xff00)>>8;
            data[6] =  (1*imgRatio)&0xff;

        }
        break;
    case CAPTURE_IMAGE:
    case CAPTURE_IAMGE_CONTINUOUS:
    case CAPTURE_IMAGE_TEMP:
        if(struCnfp.nImageSplice[struGsh.nLayer][struGsh.nView] >= 2){
            data[0] = 3;
        }else
            data[0] = 1;
        break;
    case CAPTURE_IMAGE_SYNC:
        data[0] = (flag == 0)? 0 : 1;
        break;
    case CAPTURE_BACKGROUND_WAVE:
        data[0] = 0;
        data[3] = 1;
        break;
    case CAPTURE_IPC_EJECT:
        data[0] = 6;
        data[1] = struGsh.nImageCapTrigValue/256;
        data[2] = struGsh.nImageCapTrigValue%256;
        break;
    default:
        data[0] = 0;
        break;
    }

    switch (currenttype) {
    case CAPTURE_SINGLE_WAVE:
    case CAPTURE_BACKGROUND_WAVE:
    case CAPTURE_IPC_EJECT:
        //正在清灰，不请求波形数据
        if (struGsh.bFlagWiping == 1) return;
        //        data[1] = struGsh.nImageCapTrigValue/256;
        //        data[2] = struGsh.nImageCapTrigValue%256;



        board = struCnfg.struLayerInfo[struGsh.nLayer].nViewBoardType[struGsh.nView];

        nUnitAddr = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].nViewUnitId[struGsh.nUnit];
        //远程采图时支持指定层视通道
        if (struGsh.nRemoteImageCaptureIsRun){
            data[0] = 0;
            data[1] = 0;
            data[2] = 0;
            data[6] = 0;
            board = struCnfg.struLayerInfo[lockedLayer].nViewBoardType[lockedview];
            nUnitAddr = struCnfg.struLayerInfo[lockedLayer].stuViewInfo[lockedview].nViewUnitId[lockedunit];
        }
        udpSendCmdData(CMD_CAMERA_WAVE_CAPTURE, nInt, board, nUnitAddr, data, 1);
        break;
    case CAPTURE_IMAGE:
    case CAPTURE_IMAGE_TEMP:
        if (struGsh.nPageSm == SM_IMAGE_CAPTURE_WIDGET && struGsh.nImageCaptureIsRun){
            data[1] = struGsh.nImageCapTrigValue/256;
            data[2] = struGsh.nImageCapTrigValue%256;
            data[4] = 0;
            data[5] = ((myFlow.imgHeight*imgRatio)&0xff00)>>8;
            data[6] =  (myFlow.imgHeight*imgRatio)&0xff;
            board = struCnfg.struLayerInfo[struGsh.nLayer].nViewBoardType[struGsh.nView];
            nUnitAddr = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].nViewUnitId[struGsh.nUnit];
        }else{
            data[1] = 0;
            data[2] = 0;
            data[4] = 0;
            data[5] = ((myFlow.imgHeight*imgRatio)&0xff00)>>8;
            data[6] =  (myFlow.imgHeight*imgRatio)&0xff;
            board = struCnfg.struLayerInfo[lockedLayer].nViewBoardType[lockedview];
            nUnitAddr = struCnfg.struLayerInfo[lockedLayer].stuViewInfo[lockedview].nViewUnitId[lockedunit];
        }
        udpSendCmdData(CMD_CAMERA_WAVE_CAPTURE, nInt, board, nUnitAddr, data, 1);
        break;

    case CAPTURE_IAMGE_CONTINUOUS:
        if (struGsh.nPageSm == SM_IMAGE_CAPTURE_WIDGET){
            data[1] = struGsh.nImageCapTrigValue/256;
            data[2] = struGsh.nImageCapTrigValue%256;
            data[4] = 0;
            data[5] = ((myFlow.imgHeight*imgRatio)&0xff00)>>8;
            data[6] =  (myFlow.imgHeight*imgRatio)&0xff;
            board = struCnfg.struLayerInfo[struGsh.nLayer].nViewBoardType[struGsh.nView];
            nUnitAddr = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].nViewUnitId[struGsh.nUnit];
        }
        else{
            data[1] = 0;
            data[2] = 0;
            data[4] = 0;
            data[5] = ((myFlow.imgHeight*imgRatio)&0xff00)>>8;
            data[6] =  (myFlow.imgHeight*imgRatio)&0xff;
            board = struCnfg.struLayerInfo[lockedLayer].nViewBoardType[lockedview];
            nUnitAddr = struCnfg.struLayerInfo[lockedLayer].stuViewInfo[lockedview].nViewUnitId[lockedunit];
        }
        udpSendCmdData(CMD_CAMERA_WAVE_CAPTURE, nInt, board, nUnitAddr, data, 1);

        break;
    case CAPTURE_IMAGE_SYNC:
        if(flag == 0){
            udpSendCmdData(CMD_CAMERA_IMAGE_CAPTURE_SYNC, nInt, BOARD_TYPE_ALL_CAMERA, BOARD_TYPE_ALL_IN_CHAIN, data, 1);
        }
        else{
            if (struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[view])
            {
                board = struCnfg.struLayerInfo[struGsh.nLayer].nViewBoardType[view];
                nUnitAddr = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[view].nViewUnitId[struGsh.nUnit];

                udpSendCmdData(CMD_CAMERA_IMAGE_CAPTURE_SYNC, nInt, board, nUnitAddr, data, 1);
            }
        }
        break;

    case CAPTURE_BLOW_COUNTS:
        udpSendCmdData(CMD_INT_BLOW_COUNTS_CAPTURE, nInt, BOARD_TYPE_INTERFACE, BOARD_TYPE_ALL_IN_CHAIN, data, 1);
        break;

    case CAPTURE_MULTI_IMAGE:
        data[0] = 4;
        data[1] = struGsh.nImageCapTrigValue/256;
        data[2] = struGsh.nImageCapTrigValue%256;
        data[4] = (myFlow.imgHeight*imgRatio*struGsh.imgCaptureCnt &0xff0000)>>16;
        data[5] = (myFlow.imgHeight*imgRatio*struGsh.imgCaptureCnt &0xff00)>>8;
        data[6] =  myFlow.imgHeight*imgRatio*struGsh.imgCaptureCnt &0xff;

        board = struCnfg.struLayerInfo[struGsh.nLayer].nViewBoardType[struGsh.nView];
        nUnitAddr = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].nViewUnitId[struGsh.nUnit];
        if(struGsh.nRemoteImageCaptureIsRun)
        {
            board = struCnfg.struLayerInfo[lockedLayer].nViewBoardType[lockedview];
            nUnitAddr = struCnfg.struLayerInfo[lockedLayer].stuViewInfo[lockedview].nViewUnitId[lockedunit];
        }
        udpSendCmdData(CMD_CAMERA_WAVE_CAPTURE, nInt, board, nUnitAddr, data, 1);
        break;

    case CAPTURE_MULTI_IMAGE_GET:
        data[0] = 5;
        data[1] = struGsh.nImageCapTrigValue/256;
        data[2] = struGsh.nImageCapTrigValue%256;
        data[4] = 0;
        data[5] = myFlow.imgHeight*imgRatio/256;
        data[6] = myFlow.imgHeight*imgRatio%256;

        board = struCnfg.struLayerInfo[struGsh.nLayer].nViewBoardType[struGsh.nView];
        nUnitAddr = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].nViewUnitId[struGsh.nUnit];
        if(struGsh.nRemoteImageCaptureIsRun)
        {
            board = struCnfg.struLayerInfo[lockedLayer].nViewBoardType[lockedview];
            nUnitAddr = struCnfg.struLayerInfo[lockedLayer].stuViewInfo[lockedview].nViewUnitId[lockedunit];
        }
        udpSendCmdData(CMD_CAMERA_WAVE_CAPTURE, nInt, board, nUnitAddr, data, 1);
        break;

    case CAPTURE_MULTI_IMAGE_SYNC:
    case CAPTURE_IAMGE_CONTINUOUS_SYNC:
        data[0] = 4;
        data[1] = struGsh.nImageCapTrigValue/256;
        data[2] = struGsh.nImageCapTrigValue%256;
        data[4] = (myFlow.imgHeight*imgRatio*struGsh.imgCaptureCnt &0xff0000)>>16;
        data[5] = (myFlow.imgHeight*imgRatio*struGsh.imgCaptureCnt &0xff00)>>8;
        data[6] =  myFlow.imgHeight*imgRatio*struGsh.imgCaptureCnt &0xff;

        // nUnitAddr = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].nViewUnitId[struGsh.nUnit];
        // udpSendCmdData(CMD_CAMERA_WAVE_CAPTURE, nInt, BOARD_TYPE_ALL_CAMERA, nUnitAddr, data, 1);
        udpSendCmdData(CMD_CAMERA_WAVE_CAPTURE, nInt, BOARD_TYPE_ALL_CAMERA, BOARD_TYPE_ALL_IN_CHAIN, data, 1);
        break;

    case CAPTURE_MULTI_IMAGE_GET_SYNC:
        data[0] = 5;
        data[1] = struGsh.nImageCapTrigValue/256;
        data[2] = struGsh.nImageCapTrigValue%256;
        data[4] = 0;
        data[5] = myFlow.imgHeight*imgRatio/256;
        data[6] = myFlow.imgHeight*imgRatio%256;

        if (struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[view])
        {
            board = struCnfg.struLayerInfo[struGsh.nLayer].nViewBoardType[view];
            nUnitAddr = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[view].nViewUnitId[struGsh.nUnit];

            udpSendCmdData(CMD_CAMERA_WAVE_CAPTURE, nInt, board, nUnitAddr, data, 1);
        }
        break;
    default:
        break;
    }
}
void MyNetWorkThread::udpSendCmdData(int nCmd, char intAddr, char boardType, char boardAddr, char *data, int nCount)
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

    udpSendTo(queData,SEND_PACKET_SIZE);
}
int MyNetWorkThread::udpSendTo(char *buf, int len)
{
    socklen_t addrlen;
    addrlen=sizeof(r_addr);

    return sendto(sockfd,buf,len,0,(struct sockaddr *)&r_addr,addrlen);
}

void MyNetWorkThread::run()
{
    int k = 0;                          // 波形计数

    int imageRowCnt  = 0;               // 图像采集使用，每幅图像的波形行数计数

    socklen_t sin_size = sizeof(struct sockaddr_in);
    int retrycount = 0;                 // 接收异常处理最多5
    int dataStart = 6;                  // 数据起始字节
    int index = 6;                      // 数据起始字节
    int e;
    int ejectornum = 0;
    int packetLen = 0;
    int imageCorrectWaveCnt = 0;            // 每幅图像前正确波形数（0行~第一个出错包的行数）
    char sCrcData[WAVE_FRAME_SINGLE_LEN-5];
    unsigned int nCrcItt=0;

    int test_count = 0;
    QString tmpStr;
    QString curFilePath;            //同步采图使用
    int curImageCorrectWaveCnt;     //同步采图使用
    int totalTimeCost;
    int thisTimeCost;
    bool pingPangFlag;               //连续采图使用
    imgCorrectHight = 0;
    int curPicHeight;
    QString tmp;
    /* 更新图像传感器信息  */
    updateCameraInfo();

    curTotal = 0;
    pingPangFlag = true;
    memset(wavePacketBuf,0,sizeof(wavePacketBuf));
    memset(waveDataBuf,0,sizeof(waveDataBuf));
    memset(imagePacketBuf,0,sizeof(imagePacketBuf));
    memset(imageDataBuf,0,sizeof(imageDataBuf));
    memset(struGsh.sRgbRow,0,WAVE_FRAME_SINGLE_LEN);

    while (isrunning){
        lockedLayer = mlayer;
        lockedview  = mview;
        lockedunit  = munit;
        switch (currenttype)
        {
        case CAPTURE_SINGLE_WAVE:
        case CAPTURE_BACKGROUND_WAVE:
        case CAPTURE_IPC_EJECT:
            retNum = 0;
            curTotal = 0;
            memset (wavePacketBuf,0,sizeof(wavePacketBuf));
            memset(waveDataBuf,0,sizeof(waveDataBuf));
            updateCameraInfo();

            if(currenttype != CAPTURE_IPC_EJECT
                    && struCnfp.nSensorPixel[0][struGsh.nView] == SENSOR_PIXELS_2048_XTD
                    && struCnfp.nViewSensorType[0][struGsh.nView] == SENSOR_G2)
                waveBufSize *=2; //XFS波形直接采两行2048,显示成2048的可见和合并后的1024的紫外

            cleanUdpSocket();

            /* 发送获取单行波形命令 */
            serialSendTransStartStop();
            while (isrunning)
            {
                if( checkUdpSocketStatues(0) < 0)
                {
                    break;
                }
                retNum = recvfrom(sockfd, (char *)&wavePacketBuf[curTotal], waveBufSize-curTotal,0,(struct sockaddr*)&client,&sin_size);
                curTotal += retNum;
                if (curTotal == waveBufSize)
                {
                    break;
                }
            }
            if (curTotal != waveBufSize)
            {
                myFlow.msleep(100);
                continue;
            }

            if( (wavePacketBuf[0] != 0xFF) || (wavePacketBuf[1] != 0x00) ||(wavePacketBuf[2] != 0xA5)
                    || (wavePacketBuf[3] != 0x5A) || (wavePacketBuf[4] != 0xC3) ||(wavePacketBuf[5] != 0x3C ))
            {
                /* 包头异常*/
                qDebug()<<"wave head error!";
                continue;
            }

            if(struCnfp.nSensorPixel[0][struGsh.nView] == SENSOR_PIXELS_2048_XTD
                    &&  struCnfp.nViewSensorType[0][struGsh.nView] == SENSOR_G2)
            {
                memcpy(waveDataBuf, &wavePacketBuf[dataStart], dim * curPixels);
                memcpy(&waveDataBuf[dim * curPixels], &wavePacketBuf[dataStart+ dim * curPixels+dataStart], dim * curPixels);
            }
            else if(struGsh.nCamTdMode == TD_2048_A || struGsh.nCamTdMode== TD_2048_B){

                memcpy(waveDataBuf, &wavePacketBuf[dataStart], dim * curPixels);
                memcpy(&waveDataBuf[dim * curPixels], &wavePacketBuf[dataStart+dim * curPixels], dim * curPixels);
            }
            else
            {
                memcpy(waveDataBuf, &wavePacketBuf[dataStart], dim * curPixels* td_dim);
            }

            waveDataPacketed(true);
            if(struGsh.nPageSm == SM_NIR_DOT_CORRECTIVE_LIST_WIDGET)
            {
                if (curTotal == waveBufSize)
                {
                    //当前找红外相机要的是传感器上还是下的数据
                    int updown = struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nNirUpdownEnable;
                    for(int i = 0; i < 1024; i++)
                    {
                        struGsh.nNirDotCorrectiveRed[struGsh.nView][struGsh.nUnit][updown][i] = 0;
                        struGsh.nNirDotCorrectiveGreen[struGsh.nView][struGsh.nUnit][updown][i] = 0;
                        struGsh.nNirDotCorrectiveBlue[struGsh.nView][struGsh.nUnit][updown][i] = 0;
                        struGsh.nNirDotCorrectiveN[struGsh.nView][struGsh.nUnit][updown][i] = 0;
                    }

                    for(int i = 0; i < 1024; i++)
                    {
                        struGsh.nNirDotCorrectiveRed[struGsh.nView][struGsh.nUnit][updown][i] = struGsh.sRowRed[i];
                        struGsh.nNirDotCorrectiveGreen[struGsh.nView][struGsh.nUnit][updown][i] = struGsh.sRowGreen[i];
                        struGsh.nNirDotCorrectiveBlue[struGsh.nView][struGsh.nUnit][updown][i] = struGsh.sRowBlue[i];
                        struGsh.nNirDotCorrectiveN[struGsh.nView][struGsh.nUnit][updown][i] = struGsh.sRowInf[i];
                    }
                }
            }
            else
            {
                emit SignalWaveRecvFinished();
            }
            if (struGsh.nRemoteImageCaptureIsRun) return;
            myFlow.msleep(300);
            break;
#if 0
        case CAPTURE_IMAGE:
            cleanUdpSocket();
            /* 发送获取单幅图像命令 */
            if (struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].nImageSplice >= 2){
                curImageRowCnt = myFlow.imgHeight*2;
            }else{
                curImageRowCnt = myFlow.imgHeight;
            }
            serialSendTransStartStop();
            while (isrunning)
            {
                if(struGsh.nPageSm == SM_IMAGE_CAPTURE_WIDGET){
                    if( checkUdpSocketStatues(struGsh.nImageCapTimeout) < 0 ){
                        imageCorrectWaveCnt = imageCheckSum(imageRowCnt);
                        emit SignalImageRecvFinished(imageCorrectWaveCnt,QString(""));
                        isrunning = 0;
                        return;
                    }
                }else{
                    if( checkUdpSocketStatues(1) < 0 ){
                        struGsh.bNetContinuousCapture = -1;
                        isrunning = 0;
                        return;
                    }
                }
                retNum = recvfrom(sockfd, (char *)&imagePacketBuf[curTotal], sizeof(imagePacketBuf) - curTotal, 0,(struct sockaddr*)&client,&sin_size);

                curTotal += retNum;

                imageRowCnt = curTotal / waveBufSize;

                if(curTotal >= waveBufSize * curImageRowCnt)
                    break; //一张图片已经读完
            }
            imageRowCnt = curTotal/waveBufSize;
            imageCorrectWaveCnt = imageCheckSum(imageRowCnt);

            if(imageCorrectWaveCnt == curImageRowCnt)
            {
                if (struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].nImageSplice){
                    curPixels = curPixels/2;
                }

                QString fileName = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
                QString filePath;
#ifdef Q_OS_UNIX
                if (struGsh.nRemoteImageCaptureIsRun){
                    filePath = QString("/sdcard/ts/%1_%2_%3_%4_%5_%6_%7.bmp").arg(lockedLayer+1).arg(lockedview+1).arg(lockedunit+1)
                            .arg(struCnfg.struLayerInfo[lockedLayer].stuViewInfo[lockedview].nSensorDataType)
                            .arg(struCnfp.nSenseBrightSelection[lockedLayer][lockedview])
                            .arg(struCnfp.bColorRestoreEnable)
                            .arg(fileName);
                }else{
                    filePath = QString("/sdcard/bmp/%1_%2_%3_%4_%5_%6_%7.bmp").arg(struGsh.nLayer).arg(struGsh.nView).arg(struGsh.nUnit)
                            .arg(struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].nSensorDataType)
                            .arg(struCnfp.nSenseBrightSelection[struGsh.nLayer][struGsh.nView])
                            .arg(struCnfp.bColorRestoreEnable)
                            .arg(fileName);
                }
#else
                if (struGsh.nRemoteImageCaptureIsRun){
                    filePath = QString(qApp->applicationDirPath()+"%1_%2_%3_%4_%5_%6_%7.bmp").arg(lockedLayer+1).arg(lockedview+1).arg(lockedunit+1)
                            .arg(struCnfg.struLayerInfo[lockedLayer].stuViewInfo[lockedview].nSensorDataType)
                            .arg(struCnfg.struLayerInfo[lockedLayer].stuViewInfo[lockedview].nSenseBrightSelection)
                            .arg(struCnfp.bColorRestoreEnable)
                            .arg(fileName);
                }else{
                    filePath = QString(qApp->applicationDirPath()+"/%1_%2_%3_%4_%5_%6_%7.bmp").arg(struGsh.nLayer).arg(struGsh.nView).arg(struGsh.nUnit)
                            .arg(struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].nSensorDataType)
                            .arg(struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].nSenseBrightSelection)
                            .arg(struCnfp.bColorRestoreEnable)
                            .arg(fileName);
                }
#endif
                if(struGsh.nCamTdMode > 0){
#if FS_IMAGE_CONBINE
                    filePath = QString("/sdcard/bmp/%1_%2_%3_%4_%5_%6_%7_FS.bmp").arg(struGsh.nLayer).arg(struGsh.nView).arg(struGsh.nUnit)
                            .arg(struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].nSensorDataType)
                            .arg(struCnfp.nSenseBrightSelection[struGsh.nLayer][struGsh.nView])
                            .arg(struCnfp.bColorRestoreEnable)
                            .arg(fileName);
                    QImage img(imageDataBuf, curPixels*TD_DIM, myFlow.imgHeight, QImage::Format_ARGB32);
                    if(saveBMPFile(filePath, curPixels*TD_DIM, myFlow.imgHeight))
                    {
                        if (!struGsh.nRemoteImageCaptureIsRun){
                            img = img.scaled(180,120);
                            img.save(filePath+"_copy","PNG",50);
                        }
                        emit SignalImageRecvFinished(myFlow.imgHeight,filePath);
                    }
                    else
                    {
                        emit SignalImageRecvFinished(imageCorrectWaveCnt,QString(""));
                    }
#else
                    filePath = QString("/sdcard/bmp/%1_%2_%3_%4_%5_%6_%7_A.bmp").arg(struGsh.nLayer).arg(struGsh.nView).arg(struGsh.nUnit)
                            .arg(struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].nSensorDataType)
                            .arg(struCnfp.nSenseBrightSelection[struGsh.nLayer][struGsh.nView])
                            .arg(struCnfp.bColorRestoreEnable)
                            .arg(fileName);

                    if(td_dim == 2){
                        QImage img(imageDataBuf, curPixels, myFlow.imgHeight , QImage::Format_ARGB32);
                        if(saveBMPFile(filePath, curPixels, myFlow.imgHeight))
                        {
                            if (!struGsh.nRemoteImageCaptureIsRun){
                                img = img.scaled(180,120);
                                img.save(filePath+"_copy","PNG",50);
                            }
                        }
                        memcpy(imageDataBuf,&imageDataBuf[myFlow.imgHeight*curPixels*4],myFlow.imgHeight*curPixels*4);
                    }
                    else if(td_dim == 5){
                        QImage img(imageDataBuf, curPixels*2, myFlow.imgHeight*2 , QImage::Format_ARGB32);
                        if(saveBMPFile(filePath, curPixels*2, myFlow.imgHeight*2))
                        {
                            if (!struGsh.nRemoteImageCaptureIsRun){
                                img = img.scaled(180,120);
                                img.save(filePath+"_copy","PNG",50);
                            }
                        }
                        memcpy(imageDataBuf,&imageDataBuf[myFlow.imgHeight*curPixels*4*4],myFlow.imgHeight*curPixels*4);
                    }

                    filePath = QString("/sdcard/bmp/%1_%2_%3_%4_%5_%6_%7_B.bmp").arg(struGsh.nLayer).arg(struGsh.nView).arg(struGsh.nUnit)
                            .arg(struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].nSensorDataType)
                            .arg(struCnfp.nSenseBrightSelection[struGsh.nLayer][struGsh.nView])
                            .arg(struCnfp.bColorRestoreEnable)
                            .arg(fileName);
                    QImage img2(imageDataBuf, curPixels, myFlow.imgHeight, QImage::Format_ARGB32);
                    if(saveBMPFile(filePath, curPixels, myFlow.imgHeight))
                    {
                        if (!struGsh.nRemoteImageCaptureIsRun){
                            img2 = img2.scaled(180,120);
                            img2.save(filePath+"_copy","PNG",50);
                        }
                        emit SignalImageRecvFinished(myFlow.imgHeight,filePath);
                    }
                    else
                    {
                        emit SignalImageRecvFinished(imageCorrectWaveCnt,QString(""));
                    }
#endif
                }
                else{
                    QImage img(imageDataBuf, curPixels, myFlow.imgHeight, QImage::Format_ARGB32);
                    if(saveBMPFile(filePath, curPixels, myFlow.imgHeight))
                    {
                        if(dim == 4){
                            for(int i = 0;i < myFlow.imgHeight;i++)
                            {
                                for(int j =0;j< curPixels;j++)
                                {
                                    imageDataBuf[i*curPixels*4 +j*4+3] = 255;   //缩略图不带有透明度
                                }
                            }
                            img = QImage(imageDataBuf, curPixels, myFlow.imgHeight, QImage::Format_ARGB32);
                        }
                        if (!struGsh.nRemoteImageCaptureIsRun){
                            img = img.scaled(180,120);
                            img.save(filePath+"_copy","PNG",50);
                        }
                        emit SignalImageRecvFinished(myFlow.imgHeight,filePath);
                    }
                    else
                    {
                        emit SignalImageRecvFinished(imageCorrectWaveCnt,QString(""));
                    }
                }
            }
            else
            {
                emit SignalImageRecvFinished(imageCorrectWaveCnt,QString(""));
                /* 异常情形3：图像接收完成后（数据包数据异常） */
                qDebug()<<"图像异常，请重新获取！";
            }
            isrunning = 0;
            mutex.lock();
            condition.wakeAll();
            mutex.unlock();
            return;
#endif
        case CAPTURE_IAMGE_CONTINUOUS:
            cleanUdpSocket();
            updateCameraInfo();

            /* 发送获取单幅图像命令 */
            //            if (struCnfp.nImageSplice[struGsh.nLayer][struGsh.nView] >= 2){
            //                curImageRowCnt = myFlow.imgHeight*2;
            //            }else{
            //                curImageRowCnt = myFlow.imgHeight;
            //            }

            struGsh.bNetContinuousCapture = 0;
            currenttype = CAPTURE_MULTI_IMAGE;
            serialSendTransStartStop();
            currenttype = CAPTURE_MULTI_IMAGE_GET;
            curTotal = 0;
            imageRowCnt = 0;
            serialSendTransStartStop();
            while (isrunning)
            {
                if(struGsh.nPageSm == SM_IMAGE_CAPTURE_WIDGET){
                    if( checkUdpSocketStatues(struGsh.nImageCapTimeout) < 0 ){
                        struGsh.bNetContinuousCapture = -1;
                        break;
                    }
                }else{
                    if( checkUdpSocketStatues(0) < 0 ){
                        struGsh.bNetContinuousCapture = -1;
                        break;
                    }
                }
                retNum = recvfrom(sockfd, (char *)&imagePacketBuf[curTotal], sizeof(imagePacketBuf) - curTotal, 0,(struct sockaddr*)&client,&sin_size);
                curTotal += retNum;
                imageRowCnt = curTotal / waveBufSize;
                if(curTotal >= waveBufSize * curImageRowCnt)
                    break; //一张图片已经读完
            }
            imageRowCnt = curTotal/waveBufSize;
            imageCorrectWaveCnt = imageCheckSum(imageRowCnt);

            if (struCnfp.nImageSplice[struGsh.nLayer][struGsh.nView])
                curPixels = curPixels/2;

            if(imageCorrectWaveCnt == curImageRowCnt )
            {
                if(struCnfp.nSensorPixel[0][struGsh.nView] == SENSOR_PIXELS_2048_TD
                        &&  struCnfp.nViewSensorType[0][struGsh.nView] == SENSOR_G3)
                {
                    curPixels = 2048;
                    myFlow.imgHeight = 832;
                }
                //通过阈值设置决定是演示使用还是存在本地使用
                if (struGsh.nImageCapTrigValue > 0 && CUSTOM != CUSTOM_YANSI){
                    QString fileName = QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz");
                    QString filePath;
                    if (struGsh.nRemoteImageCaptureIsRun){
                        filePath = QString("/sdcard/ts/%1_%2_%3_%4_%5_%6_%7.bmp").arg(lockedLayer+1).arg(lockedview+1).arg(lockedunit+1)
                                .arg(struCnfp.nSensorDataType[lockedLayer][lockedview])
                                .arg(struCnfp.nSenseBrightSelection[lockedLayer][lockedview])
                                .arg(struCnfp.bColorRestoreEnable)
                                .arg(fileName);
                    }else{
                        filePath = QString("/sdcard/bmp/%1_%2_%3_%4_%5_%6_%7.bmp").arg(struGsh.nLayer).arg(struGsh.nView).arg(struGsh.nUnit)
                                .arg(struCnfp.nSensorDataType[struGsh.nLayer][struGsh.nView])
                                .arg(struCnfp.nSenseBrightSelection[struGsh.nLayer][struGsh.nView])
                                .arg(struCnfp.bColorRestoreEnable)
                                .arg(fileName);
                    }

                    if(myFlow.saveBmp(imageDataBuf, filePath, curPixels, myFlow.imgHeight, dim ,true)){
                        emit showContinuousImage(filePath);
                    }
                }
                else{
                    if(pingPangFlag){
                        pingPangFlag = false;
                        if(myFlow.saveBmp(imageDataBuf, "/tmp/ping.bmp", curPixels, myFlow.imgHeight, dim ,false)){
                            emit showContinuousImage("/tmp/ping.bmp");
                        }
                    }else{
                        pingPangFlag = true;
                        if(myFlow.saveBmp(imageDataBuf, "/tmp/pang.bmp", curPixels, myFlow.imgHeight, dim ,false)){
                            emit showContinuousImage("/tmp/pang.bmp");
                        }
                    }
                }
            }
            currenttype = CAPTURE_IAMGE_CONTINUOUS;
            struGsh.bNetContinuousCapture = 1;
            if(!struGsh.bContinuous) return;
            msleep(50);
            break;
#if 0
        case CAPTURE_IMAGE_SYNC:
            //群发给所有相机板
            serialSendTransStartStop(0, 0);
            totalTimeCost = struGsh.nImageCapTimeout;
            thisTimeCost = 0;
            for(int view = 0; view < MAX_VIEW+1; view++ )
            {
                if (view == MAX_VIEW) break;

                if (struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[view] == 0)
                {
                    continue;
                }
                totalTimeCost -= thisTimeCost;
                if (totalTimeCost < 0) totalTimeCost = 2;
                imageCorrectWaveCnt = 0;

                /* 更新图像传感器信息,防止前后相机类型不一样  */
                dim = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[view].nSensorDim;
                curPixels = myFlow.getSensorPixels(struGsh.nLayer, view);
                waveBufSize = dim * curPixels + 6;

                memset(imagePacketBuf,0,sizeof(imagePacketBuf));
                memset(imageDataBuf,0,sizeof(imageDataBuf));

                cleanUdpSocket();
                /* 发送同步获取图像命令 */
                if (struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].nImageSplice >= 2){
                    curImageRowCnt = myFlow.imgHeight*2;
                }else{
                    curImageRowCnt = myFlow.imgHeight;
                }
                serialSendTransStartStop(1, view);
                thisTimeCost = myGetTickCount()/1000;

                retNum = 0;
                curTotal = 0;

                while (isrunning)
                {
                    if( checkUdpSocketStatues(totalTimeCost) < 0 )
                    {
                        imageCorrectWaveCnt = imageCheckSum(imageRowCnt);
                        break;
                    }

                    retNum = recvfrom(sockfd, (char *)&imagePacketBuf[curTotal], sizeof(imagePacketBuf) - curTotal, 0,(struct sockaddr*)&client,&sin_size);

                    curTotal += retNum;

                    imageRowCnt = curTotal / waveBufSize;

                    if(curTotal >= waveBufSize *  curImageRowCnt )
                        break; //一张图片已经读完
                }
                thisTimeCost = myGetTickCount()/1000 - thisTimeCost;
                if(curTotal != waveBufSize *  curImageRowCnt )
                    continue; //本张图片异常，读取下一张

                /*********************************图片处理************************************/

                imageCorrectWaveCnt = imageCheckSum(imageRowCnt);

                if(imageCorrectWaveCnt == curImageRowCnt)
                {
                    if (struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].nImageSplice){
                        curPixels = curPixels/2;
                    }
                    QString fileName = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
                    QString filePath;
#ifdef Q_OS_UNIX
                    filePath = QString("/sdcard/bmp/%1_%2_%3_%4_%5_%6_%7.bmp").arg(struGsh.nLayer).arg(view).arg(struGsh.nUnit)
                            .arg(struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[view].nSensorDataType)
                            .arg(struCnfp.nSenseBrightSelection[struGsh.nLayer][view])
                            .arg(struCnfp.bColorRestoreEnable)
                            .arg(fileName);
#else
                    filePath = QString(qApp->applicationDirPath()+"/%1_%2_%3_%4_%5_%6_%7.bmp").arg(struGsh.nLayer).arg(view).arg(struGsh.nUnit)
                            .arg(struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].nSensorDataType)
                            .arg(struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].nSenseBrightSelection)
                            .arg(struCnfp.bColorRestoreEnable)
                            .arg(fileName);
#endif
                    if(saveBMPFile(filePath, curPixels, myFlow.imgHeight))
                    {
                        QImage img(filePath);
                        qDebug()<<"save view ok:"<<view;
                        sleep(1);

                        if (!struGsh.nRemoteImageCaptureIsRun){
                            img = img.scaled(180,120);
                            img.save(filePath+"_copy","PNG",50);
                        }
                        if (view == struGsh.nView )
                        {
                            curFilePath = filePath;
                            curImageCorrectWaveCnt = imageCorrectWaveCnt;
                        }
                    }
                    else{
                        if (view == struGsh.nView )
                        {
                            curFilePath = "";
                            curImageCorrectWaveCnt = imageCorrectWaveCnt;
                        }
                    }
                }
                else
                {
                    if (view == struGsh.nView )
                    {
                        curFilePath = "";
                        curImageCorrectWaveCnt = imageCorrectWaveCnt;
                    }
                }
            }
            /* 界面默认显示还是当前视的图片 */
            emit SignalImageRecvFinished(curImageCorrectWaveCnt,curFilePath);
            return;

        case CAPTURE_BLOW_COUNTS:
            retNum = 0;
            curTotal = 0;
            memset(blowCountsPacketBuf,0,sizeof(blowCountsPacketBuf));

            cleanUdpSocket();
            /* 发送获取吹气频率命令 */
            serialSendTransStartStop();
            while (isrunning)
            {
                if( checkUdpSocketStatues(0) < 0)
                {
                    break;
                }

                retNum = recvfrom(sockfd, (char *)&blowCountsPacketBuf[curTotal], sizeof(blowCountsPacketBuf)-curTotal,0,(struct sockaddr*)&client,&sin_size);
                curTotal += retNum;

                if (curTotal == sizeof(blowCountsPacketBuf))
                {
                    break;
                }
            }

            if (curTotal != sizeof(blowCountsPacketBuf))
            {
                isrunning = false;
                return;
            }

            if((blowCountsPacketBuf[0] != 0x53)
                    || (blowCountsPacketBuf[1] != 0xb9)
                    || (blowCountsPacketBuf[2] != BOARD_TYPE_INTERFACE)
                    || (blowCountsPacketBuf[3] != 0x00)
                    || (blowCountsPacketBuf[4] != 0x00)
                    || (blowCountsPacketBuf[5] != 0x8d )
                    || (blowCountsPacketBuf[sizeof(blowCountsPacketBuf) - 2] != 0xff))
            {
                /* 数据包异常 */
                logger()->info("wave head error!");
                return;
            }

            for(int i = 0; i< struCnfg.nChuteTotal;i++){
                for(int j = 0;j< struCnfg.struLayerInfo[struGsh.nLayer].nEjectorsPerChute;j++){
                    blowCountsTempDataBuf[i*struCnfg.struLayerInfo[struGsh.nLayer].nEjectorsPerChute+j] += (blowCountsPacketBuf[6+i*MAX_EJECTOR*4+j*4] <<24) + \
                            (blowCountsPacketBuf[7+i*MAX_EJECTOR*4+j*4] <<16) +\
                            (blowCountsPacketBuf[8+i*MAX_EJECTOR*4+j*4] <<8 ) +\
                            (blowCountsPacketBuf[9+i*MAX_EJECTOR*4+j*4]);
                }
            }
            emit SignalBlowCountsRecvFinished();
            if (struGsh.nRemoteImageCaptureIsRun) return;

            break;
#endif

        case CAPTURE_BLOW_COUNTS:
            retNum = 0;
            curTotal = 0;
            memset(blowCountsPacketBuf,0,sizeof(blowCountsPacketBuf));

            cleanUdpSocket();
            /* 发送获取吹气频率命令 */
            serialSendTransStartStop();

            ejectornum = 80;
            packetLen = 6+4096+3+1;
            if(struCnfg.struLayerInfo[struGsh.nLayer].nEjectorsPerChute > 80)
            {
                ejectornum = MAX_EJECTOR;
                packetLen = 6+4608+3+1;
            }

            while (isrunning)
            {
                if( checkUdpSocketStatues(0) < 0)
                {
                    break;
                }

                retNum = recvfrom(sockfd, (char *)&blowCountsPacketBuf[curTotal], packetLen-curTotal,0,(struct sockaddr*)&client,&sin_size);
                curTotal += retNum;

                if (curTotal == packetLen)
                {
                    break;
                }
            }

            if (curTotal != packetLen)
            {
                logger()->info("curTotal:%1",curTotal);
                isrunning = false;
                return;
            }

            if((blowCountsPacketBuf[0] != 0x53)
                    || (blowCountsPacketBuf[1] != 0xb9)
                    || (blowCountsPacketBuf[2] != BOARD_TYPE_INTERFACE)
                    || (blowCountsPacketBuf[3] != 0x00)
                    || (blowCountsPacketBuf[4] != 0x00)
                    || (blowCountsPacketBuf[5] != 0x8d )
                    /*|| (blowCountsPacketBuf[packetLen - 2] != 0xff)*/)
            {
                /* 数据包异常 */
                logger()->info("blowcount head error!");
                return;
            }

            for(int i = 0; i< struCnfg.nChuteTotal;i++){
                for(int j = 0;j< struCnfg.struLayerInfo[struGsh.nLayer].nEjectorsPerChute;j++){
                    int tmpData = (blowCountsPacketBuf[6+i*ejectornum*4+j*4] <<24) + \
                            (blowCountsPacketBuf[7+i*ejectornum*4+j*4] <<16) +\
                            (blowCountsPacketBuf[8+i*ejectornum*4+j*4] <<8 ) +\
                            (blowCountsPacketBuf[9+i*ejectornum*4+j*4]);


                    struGsh.sBlowTimes[i*struCnfg.struLayerInfo[struGsh.nLayer].nEjectorsPerChute+j] = tmpData;

                    struCnfg.totalBlowCount[i*struCnfg.struLayerInfo[struGsh.nLayer].nEjectorsPerChute+j] += tmpData;


                }
            }

            //            for(int i = 0; i < struCnfg.struLayerInfo[struGsh.nLayer].nEjectorsPerChute*4; i++)
            //            {
            //                qDebug()<<QString::number(struGsh.sBlowTimes[i]);
            //            }

            for(int i = 0; i< struCnfg.nChuteTotal;i++)
            {
                if(struGsh.bFlagWiping == 1)  // 清灰状态下的喷阀保护状态不上报
                    break;

                if(i <8 ) // 前八个通道
                {

                    struGsh.valveProtectValue[i] = (blowCountsPacketBuf[6 + MAX_VIEW_UNIT_USE*ejectornum*4 + 3] & (0x01 << i) )? true:false;
                }
                else
                {
                    struGsh.valveProtectValue[i] = (blowCountsPacketBuf[6 + MAX_VIEW_UNIT_USE*ejectornum*4 + 2] & (0x01 << (i-8)) )? true:false;
                }

            }

            //Zhy:20230510 大于12通道或大于96吹嘴时需发送补充命令
            if(struCnfg.nChuteTotal > 12 || struCnfg.struLayerInfo[struGsh.nLayer].nEjectorsPerChute > 96)
            {
                retNum = 0;
                curTotal = 0;
                memset(blowCountsPacketBuf,0,sizeof(blowCountsPacketBuf));
                cleanUdpSocket();
                sendTransIncreaseBlowCounts();
                ejectornum = 128-96;
                packetLen = 6+4096+3+1;

                while (isrunning)
                {
                    if(checkUdpSocketStatues(0) < 0)
                    {
                        break;
                    }
                    retNum = recvfrom(sockfd, (char *)&blowCountsPacketBuf[curTotal], packetLen-curTotal,0,(struct sockaddr*)&client,&sin_size);
                    curTotal += retNum;

                    if (curTotal == packetLen)
                    {
                        break;
                    }
                }

                if (curTotal != packetLen)
                {
                    logger()->info("curTotal:%1",curTotal);
                    isrunning = false;
                    return;
                }

                if((blowCountsPacketBuf[0] != 0x53)
                        || (blowCountsPacketBuf[1] != 0xb9)
                        || (blowCountsPacketBuf[2] != BOARD_TYPE_INTERFACE)
                        || (blowCountsPacketBuf[3] != 0x00)
                        || (blowCountsPacketBuf[4] != 0x00)
                        || (blowCountsPacketBuf[5] != 0x8d )
                        /*|| (blowCountsPacketBuf[packetLen - 2] != 0xff)*/)
                {
                    /* 数据包异常 */
                    logger()->info("wave head error!");
                    return;
                }
                for(int i = 0; i < 12;i++){
                    for(int j = 0;j < 32;j++){
                        int tmpData = (blowCountsPacketBuf[6+i*ejectornum*4+j*4] <<24) + \
                                (blowCountsPacketBuf[7+i*ejectornum*4+j*4] <<16) +\
                                (blowCountsPacketBuf[8+i*ejectornum*4+j*4] <<8 ) +\
                                (blowCountsPacketBuf[9+i*ejectornum*4+j*4]);
                        struGsh.sBlowTimes[i*128+96+j] = tmpData;
                        struCnfg.totalBlowCount[i*128+96+j] += tmpData;
                    }
                }
                for(int i=0; i<4; i++){
                    for(int j=0; j<128; j++){
                        int tmpData = (blowCountsPacketBuf[6+128*12+i*128*4+j*4] <<24) + \
                                (blowCountsPacketBuf[7+128*12+i*128*4+j*4] <<16) +\
                                (blowCountsPacketBuf[8+128*12+i*128*4+j*4] <<8 ) +\
                                (blowCountsPacketBuf[9+128*12+i*128*4+j*4]);
                        struGsh.sBlowTimes[12*128+i*128+j] = tmpData;
                        struCnfg.totalBlowCount[12*128+i*128+j] += tmpData;
                    }
                }
            }


            emit SignalBlowCountsRecvFinished();

            if(nBlowQueryFreq <= 0){
                isrunning = 0;
                return;
            }
            else{
                sleep(nBlowQueryFreq);
                isrunning = 1;
            }
            break;


        case CAPTURE_SUSPEND:
            sleep(1);
            break;

            /* 1.发04让下位机缓存
         * 2.上位机发送05获取第一张图像，进入25s超时等待，此时下位机缓存此命令，待缓存完毕回传第一张，若超时则直接退出,
         * 3.依次获取图像，2s超时，如出现3张超时则退出
         * 4.回传成功张数界面显示 */
        case CAPTURE_MULTI_IMAGE:
            //            if ( struCnfp.nImageSplice[struGsh.nLayer][struGsh.nView] >= 2){
            //                curImageRowCnt = myFlow.imgHeight*2;
            //            }else{
            //                curImageRowCnt = myFlow.imgHeight;
            //            }
            serialSendTransStartStop();

            currenttype = CAPTURE_MULTI_IMAGE_GET;
            totalTimeCost = struGsh.nImageCapTimeout;
            thisTimeCost = 0;
            k = 0;

            for(int i = 0; i < struGsh.imgCaptureCnt; i++ )
            {
                cleanUdpSocket();
                /* 发送获取单幅图像命令 */
                serialSendTransStartStop();
                totalTimeCost -= thisTimeCost;
                if (totalTimeCost < 0){
                    totalTimeCost = 2;
                    if (i-k >=3) break; //三张采图失败就退出
                }
                imageCorrectWaveCnt = 0;

                memset(imagePacketBuf,0,sizeof(imagePacketBuf));
                memset(imageDataBuf,0,sizeof(imageDataBuf));

                /* 发送同步获取图像命令 */
                thisTimeCost = myGetTickCount()/1000;

                retNum = 0;
                curTotal = 0;

                while (isrunning)
                {
                    if(struGsh.nPageSm == SM_IMAGE_CAPTURE_WIDGET){
                        if( checkUdpSocketStatues(totalTimeCost) < 0 )
                        {
                            qDebug()<<"timeout";
                            imageCorrectWaveCnt = imageCheckSum(imageRowCnt);
                            break;
                        }
                    }else{
                        if( checkUdpSocketStatues(1) < 0 ){
                            isrunning = 0;
                            return;
                        }
                    }

                    retNum = recvfrom(sockfd, (char *)(imagePacketBuf+curTotal), curImageRowCnt*waveBufSize - curTotal, 0,(struct sockaddr*)&client,&sin_size);

                    curTotal += retNum;

                    imageRowCnt = curTotal / waveBufSize;

                    if(curTotal == waveBufSize * curImageRowCnt)
                        break; //一张图片已经读完
                }

                qDebug()<<"curTotal"<<curTotal<<"imageRowCnt"<<imageRowCnt;

                thisTimeCost = myGetTickCount()/1000 - thisTimeCost;
                imageCorrectWaveCnt = imageCheckSum(imageRowCnt);

                if(imageCorrectWaveCnt == curImageRowCnt)
                {
                    k++;
                    QString fileName = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
                    if(struGsh.imgCaptureCnt > 1)
                        fileName.append(QString("_%1").arg(i+1));
                    QString filePath;
#ifdef Q_OS_UNIX
                    if (struGsh.nRemoteImageCaptureIsRun){
                        filePath = QString("/sdcard/ts/%1_%2_%3_%4_%5_%6_%7.bmp").arg(lockedLayer+1).arg(lockedview+1).arg(lockedunit+1)
                                .arg(struCnfp.nSensorDataType[lockedLayer][lockedview])
                                .arg(struCnfp.nSenseBrightSelection[lockedLayer][lockedview])
                                .arg(struCnfp.bColorRestoreEnable)
                                .arg(fileName);
                    }else{
                        filePath = QString("/sdcard/bmp/%1_%2_%3_%4_%5_%6_%7.bmp").arg(struGsh.nLayer).arg(struGsh.nView).arg(struGsh.nUnit)
                                .arg(struCnfp.nSensorDataType[struGsh.nLayer][struGsh.nView])
                                .arg(struCnfp.nSenseBrightSelection[struGsh.nLayer][struGsh.nView])
                                .arg(struCnfp.bColorRestoreEnable)
                                .arg(fileName);
                    }
#endif
                    if(struGsh.nCamTdMode > 0){
                        filePath = QString("/sdcard/bmp/%1_%2_%3_%4_%5_%6_%7_A.bmp").arg(struGsh.nLayer).arg(struGsh.nView).arg(struGsh.nUnit)
                                .arg(struCnfp.nSensorDataType[struGsh.nLayer][struGsh.nView])
                                .arg(struCnfp.nSenseBrightSelection[struGsh.nLayer][struGsh.nView])
                                .arg(struCnfp.bColorRestoreEnable)
                                .arg(fileName);
                        //可見
                        if(td_dim == 2){
                            myFlow.saveBmp(imageDataBuf,filePath,curPixels,myFlow.imgHeight,dim,true);
                        }
                        else if(td_dim == 5){
                            myFlow.saveBmp(imageDataBuf,filePath,curPixels*2,myFlow.imgHeight*2,dim,true);
                        }
                        filePath = QString("/sdcard/bmp/%1_%2_%3_%4_%5_%6_%7_B.bmp").arg(struGsh.nLayer).arg(struGsh.nView).arg(struGsh.nUnit)
                                .arg(struCnfp.nSensorDataType[struGsh.nLayer][struGsh.nView])
                                .arg(struCnfp.nSenseBrightSelection[struGsh.nLayer][struGsh.nView])
                                .arg(struCnfp.bColorRestoreEnable)
                                .arg(fileName);

                        if(struCnfp.nSensorPixel[0][struGsh.nView] == SENSOR_PIXELS_2048_XTD
                                && struCnfp.nViewSensorType[0][struGsh.nView] == SENSOR_G2)
                        {
                            if(myFlow.saveBmp(&imageDataBuf[myFlow.imgHeight*curPixels*3],filePath,curPixels/2,myFlow.imgHeight/2,dim,true))  {
                                emit SignalImageRecvFinished(myFlow.imgHeight,filePath);
                            }
                            else {
                                emit SignalImageRecvFinished(imageCorrectWaveCnt,QString(""));
                            }
                        }
                        else{
                            if(struCnfp.nSensorPixel[0][struGsh.nView] == SENSOR_PIXELS_2048_GTD )
                            {
                                memcpy(imageDataBuf,&imageDataBuf[myFlow.imgHeight*curPixels*dim],myFlow.imgHeight*curPixels*dim);
                            }
                            else
                                memcpy(imageDataBuf,&imageDataBuf[myFlow.imgHeight*curPixels*(td_dim-1)*3],myFlow.imgHeight*curPixels*3);

                            //紫外
                            if(myFlow.saveBmp(imageDataBuf,filePath,curPixels,myFlow.imgHeight,dim,true))  {
                                emit SignalImageRecvFinished(myFlow.imgHeight,filePath);
                            }
                            else {
                                emit SignalImageRecvFinished(imageCorrectWaveCnt,QString(""));
                            }
                        }
                    }
                    else{
                        if( myFlow.saveBmp(imageDataBuf,filePath,curPixels,myFlow.imgHeight,dim,true)) {
                            emit SignalImageRecvFinished(myFlow.imgHeight,filePath);
                        }
                        else {
                            emit SignalImageRecvFinished(imageCorrectWaveCnt,QString(""));
                        }
                    }
                }
                else
                {
                    emit SignalImageRecvFinished(imageCorrectWaveCnt,QString(""));
                }
            }
            return;
#if 0
        case CAPTURE_MULTI_IMAGE_SYNC:
            if (struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].nImageSplice >= 2){
                curImageRowCnt = myFlow.imgHeight*2;
            }else{
                curImageRowCnt = myFlow.imgHeight;
            }
            serialSendTransStartStop(0,0);
            currenttype = CAPTURE_MULTI_IMAGE_GET_SYNC;

            k = 0;
            for(int view = struGsh.nView/4*4; view <= (struGsh.nView/4+1)*4; view++ )
            {
                if (view == MAX_VIEW) break;
                totalTimeCost = struGsh.nImageCapTimeout;
                thisTimeCost = 0;
                if (struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[view] == 0)
                    continue;
                for(int i = 0; i < struGsh.imgCaptureCnt; i++ )
                {
                    cleanUdpSocket();
                    if (struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[view].nImageSplice >= 2){
                        curImageRowCnt = myFlow.imgHeight*2;
                    }else{
                        curImageRowCnt = myFlow.imgHeight;
                    }
                    /* 发送获取单幅图像命令 */
                    serialSendTransStartStop(1,view);
                    qDebug()<<"multi sync get view:"<<view<<"cnt:"<<i;
                    totalTimeCost -= thisTimeCost;
                    if (totalTimeCost < 0){
                        totalTimeCost = 2;
                        if (i-k >=3){
                            view = MAX_VIEW; //退出整个循环
                            break; //三张采图失败就退出
                        }
                    }
                    imageCorrectWaveCnt = 0;

                    memset(imagePacketBuf,0,sizeof(imagePacketBuf));
                    memset(imageDataBuf,0,sizeof(imageDataBuf));

                    /* 发送同步获取图像命令 */
                    thisTimeCost = myGetTickCount()/1000;
                    retNum = 0;
                    curTotal = 0;

                    while (isrunning)
                    {
                        if( checkUdpSocketStatues(totalTimeCost) < 0 )
                        {
                            qDebug()<<"timeout";
                            qDebug("curtotal=%d,imgrowcnt=%d,wavebufsize=%d,needTotal=%d",curTotal,imageRowCnt, waveBufSize,waveBufSize*curImageRowCnt);
                            imageCorrectWaveCnt = imageCheckSum(imageRowCnt);
                            break;
                        }

                        retNum = recvfrom(sockfd, (char *)(imagePacketBuf+curTotal), myFlow.imgHeight*waveBufSize - curTotal, 0,(struct sockaddr*)&client,&sin_size);
                        curTotal += retNum;
                        imageRowCnt = curTotal / waveBufSize;
                        if(curTotal == waveBufSize * curImageRowCnt)
                            break; //一张图片已经读完
                    }
                    thisTimeCost = myGetTickCount()/1000 - thisTimeCost;
                    imageCorrectWaveCnt = imageCheckSum(imageRowCnt);
                    if(imageCorrectWaveCnt == curImageRowCnt)
                    {
                        k++;
                        QString fileName = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
                        if(struGsh.imgCaptureCnt > 1)
                            fileName.append(QString("_%1").arg(i+1));
                        QString filePath;
#ifdef Q_OS_UNIX
                        filePath = QString("/sdcard/bmp/%1_%2_%3_%4_%5_%6_%7.bmp").arg(struGsh.nLayer).arg(view).arg(struGsh.nUnit)
                                .arg(struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[view].nSensorDataType)
                                .arg(struCnfp.nSenseBrightSelection[struGsh.nLayer][view])
                                .arg(struCnfp.bColorRestoreEnable)
                                .arg(fileName);
#else
                        filePath = QString(qApp->applicationDirPath()+"/%1_%2_%3_%4_%5_%6_%7.bmp").arg(struGsh.nLayer).arg(view).arg(struGsh.nUnit)
                                .arg(struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[view].nSensorDataType)
                                .arg(struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[view].nSenseBrightSelection)
                                .arg(struCnfp.bColorRestoreEnable)
                                .arg(fileName);
#endif
                        if(saveBMPFile(filePath, curPixels, myFlow.imgHeight))
                        {
                            QImage img(filePath);

                            if (!struGsh.nRemoteImageCaptureIsRun){
                                img = img.scaled(180,120);
                                img.save(filePath+"_copy","PNG",50);
                            }
                            curFilePath = filePath;
                            curImageCorrectWaveCnt = imageCorrectWaveCnt;
                        }
                    }
                    else
                    {
                        curFilePath = "";
                        curImageCorrectWaveCnt = imageCorrectWaveCnt;
                    }
                }
            }
            isrunning = 0;
            emit SignalImageRecvFinished(k,curFilePath); //把实际采到正常的张数回传显示
            break;
#endif
        case CAPTURE_IAMGE_CONTINUOUS_SYNC:
            cleanUdpSocket();

            /* 发送获取单幅图像命令 */
            //            if (struCnfp.nImageSplice[struGsh.nLayer][struGsh.nView] >= 2){
            //                curImageRowCnt = myFlow.imgHeight*2;
            //            }else{
            //                curImageRowCnt = myFlow.imgHeight;
            //            }

            struGsh.bNetContinuousCapture = 0;
            serialSendTransStartStop(0,0);
            curTotal = 0;
            imageRowCnt = 0;

            totalTimeCost = struGsh.nImageCapTimeout;
            currenttype = CAPTURE_MULTI_IMAGE_GET_SYNC;
            for(int view = struGsh.nView/4*4; view < (struGsh.nView/4+1)*4; view++ )
            {
                if (struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[view] == 0)
                    continue;

                updateCameraInfo(view);
                cleanUdpSocket();

                //                if (struCnfp.nImageSplice[struGsh.nLayer][view] >= 2){
                //                    curImageRowCnt = myFlow.imgHeight*2;
                //                }else{
                //                    curImageRowCnt = myFlow.imgHeight;
                //                }
                /* 发送获取单幅图像命令 */
                serialSendTransStartStop(1,view);
                imageCorrectWaveCnt = 0;

                memset(imagePacketBuf,0,sizeof(imagePacketBuf));
                memset(imageDataBuf,0,sizeof(imageDataBuf));
                retNum = 0;
                curTotal = 0;

                while (isrunning)
                {
                    if( checkUdpSocketStatues(totalTimeCost) < 0 ){
                        struGsh.bNetContinuousCapture = -1;
                        break;
                    }
                    retNum = recvfrom(sockfd, (char *)(imagePacketBuf+curTotal), myFlow.imgHeight*waveBufSize - curTotal, 0,(struct sockaddr*)&client,&sin_size);
                    curTotal += retNum;
                    if(curTotal == waveBufSize * curImageRowCnt)
                        break; //一张图片已经读完
                }
                if( struGsh.bNetContinuousCapture == -1)
                {
                    // totalTimeCost = 2;
                }
                imageRowCnt = curTotal / waveBufSize;
                imageCorrectWaveCnt = imageCheckSum(imageRowCnt);

                qDebug()<<"seq sync get view:"<<view<<"correctWavecnt"<<imageCorrectWaveCnt;
                if(imageCorrectWaveCnt == curImageRowCnt)
                {
                    QString fileName = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
                    QString filePath;

#ifdef Q_OS_UNIX
                    filePath = QString("/sdcard/bmp/%1_%2_%3_%4_%5_%6_%7.bmp").arg(struGsh.nLayer).arg(view).arg(struGsh.nUnit)
                            .arg(struCnfp.nSensorDataType[struGsh.nLayer][view])
                            .arg(struCnfp.nSenseBrightSelection[struGsh.nLayer][view])
                            .arg(struCnfp.bColorRestoreEnable)
                            .arg(fileName);
#else
                    filePath = QString(qApp->applicationDirPath()+"/%1_%2_%3_%4_%5_%6_%7.bmp").arg(struGsh.nLayer).arg(view).arg(struGsh.nUnit)
                            .arg(struCnfp.nSensorDataType[struGsh.nLayer][view])
                            .arg(struCnfp.nSenseBrightSelection[struGsh.nLayer][view])
                            .arg(struCnfp.bColorRestoreEnable)
                            .arg(fileName);
#endif
                    if(struGsh.nCamTdMode > 0){
                        filePath = QString("/sdcard/bmp/%1_%2_%3_%4_%5_%6_%7_A.bmp").arg(struGsh.nLayer).arg(view).arg(struGsh.nUnit)
                                .arg(struCnfp.nSensorDataType[struGsh.nLayer][view])
                                .arg(struCnfp.nSenseBrightSelection[struGsh.nLayer][view])
                                .arg(struCnfp.bColorRestoreEnable)
                                .arg(fileName);
                        //可見
                        if(td_dim == 2){
                            myFlow.saveBmp(imageDataBuf,filePath,curPixels,myFlow.imgHeight,dim,true);
                        }
                        else if(td_dim == 5){
                            myFlow.saveBmp(imageDataBuf,filePath,curPixels*2,myFlow.imgHeight*2,dim,true);
                        }
                        emit showContinuousImage(filePath);

                        //                        filePath = QString("/sdcard/bmp/%1_%2_%3_%4_%5_%6_%7_B.bmp").arg(struGsh.nLayer).arg(struGsh.nView).arg(struGsh.nUnit)
                        //                                .arg(struCnfp.nSensorDataType[struGsh.nLayer][struGsh.nView])
                        //                                .arg(struCnfp.nSenseBrightSelection[struGsh.nLayer][struGsh.nView])
                        //                                .arg(struCnfp.bColorRestoreEnable)
                        //                                .arg(fileName);
                        //                        if(struCnfp.nSensorPixel[0][struGsh.nView] == SENSOR_PIXELS_2048_XTD
                        //                                && struCnfp.nViewSensorType[0][struGsh.nView] == SENSOR_G2)
                        //                        {
                        //                            if(myFlow.saveBmp(&imageDataBuf[myFlow.imgHeight*curPixels*3],filePath,curPixels/2,myFlow.imgHeight/2,dim,true))  {
                        //                                emit showContinuousImage(filePath);
                        //                            }
                        //                        }
                        //                        else{
                        //                            memcpy(imageDataBuf,&imageDataBuf[myFlow.imgHeight*curPixels*(td_dim-1)*3],myFlow.imgHeight*curPixels*3);
                        //                            //紫外
                        //                            if(myFlow.saveBmp(imageDataBuf,filePath,curPixels,myFlow.imgHeight,dim,true))  {
                        //                                emit showContinuousImage(filePath);
                        //                            }
                        //                        }
                    }
                    else{
                        if( myFlow.saveBmp(imageDataBuf,filePath,curPixels,myFlow.imgHeight,dim,true)) {
                            emit showContinuousImage(filePath);
                        }
                    }
                }
            }

            struGsh.bNetContinuousCapture = 1;
            if(!struGsh.bContinuous)
            {
                isrunning = false;
                return;
            }
            msleep(50);
            currenttype = CAPTURE_IAMGE_CONTINUOUS_SYNC;
            break;
#if 0
        case CAPTURE_IMAGE_TEMP:
            cleanUdpSocket();
            if (struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].nImageSplice >= 2){
                curImageRowCnt = myFlow.imgHeight*2;
            }else{
                curImageRowCnt = myFlow.imgHeight;
            }
            serialSendTransStartStop();
            while (isrunning)
            {
                if( checkUdpSocketStatues(1) < 0 ){
                    isrunning = 0;
                    break;
                }
                retNum = recvfrom(sockfd, (char *)&imagePacketBuf[curTotal], sizeof(imagePacketBuf) - curTotal, 0,(struct sockaddr*)&client,&sin_size);
                curTotal += retNum;
                imageRowCnt = curTotal / waveBufSize;

                if(curTotal >= (waveBufSize * curImageRowCnt) )
                    break; //一张图片已经读完
            }
            imageRowCnt = curTotal/waveBufSize;
            imageCorrectWaveCnt = imageCheckSum(imageRowCnt);
            qDebug()<<imageRowCnt<<imageCorrectWaveCnt<<curTotal<<waveBufSize<<myFlow.imgHeight <<"-------";

            totalCnt = 0; materCnt = 0;materPropor = 0;
            for(int i=0; i<imageCorrectWaveCnt; i++){
                for(int j=0; j<curPixels; j++){
                    totalCnt ++;
                    uchar r = *(imageDataBuf+i*curPixels*4+j*4+2);
                    uchar g = *(imageDataBuf+i*curPixels*4+j*4+1);
                    uchar b = *(imageDataBuf+i*curPixels*4+j*4+0);
                    uchar n = *(imageDataBuf+i*curPixels*4+j*4+3);
                    if(myFlow.imagePixelIsBackground(r, g, b, n, struGsh.nLayer, lockedview))
                        continue;
                    int sensornSplice = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[lockedview].nImageSplice;
                    sensornSplice = (sensornSplice == 0)? 1 : 2;

                    if(j < struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[lockedview].nChannelBegin[lockedunit]/sensornSplice \
                            || j > struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[lockedview].nChannelEnd[lockedunit]/sensornSplice)
                        continue;//像元划分部分直接作为背景处理，节省计算时间
                    materCnt ++;
                }
            }
            if(totalCnt > 0){
                materPropor = (materCnt*100)/totalCnt;
            }
            imgCorrectHight = imageCorrectWaveCnt;

            qDebug()<<"VIEW:"<<lockedview<<"UNIT:"<<lockedunit<<"MAT:"<<materCnt<<"TOT:"<<totalCnt<<"PROPOR:"<<materPropor;

            if(materPropor > struCnfp.nIpcHasMatPixelPercert) //物料屏占比>有料下限 此时正常下料
            {
                logger()->info("MAT:%1,TOT:%2,PROP:%3",materCnt,totalCnt,materPropor);
            }
            isrunning = 0;
            return;
#endif
        }//end currenttype

    }//end whole recv loop
}

/****************************网络采图线程类*******************************/
MyNetCptureThread::MyNetCptureThread(QThread *parent) : QThread(parent)
{

}
void MyNetCptureThread::myUdpSendTransStartStop(int flag,int view)
{
    int board;  // 0x00表示当前 视的所有相机板
    int nInt=0, nUnitAddr;
    char data[SEND_PACKET_DATA_SIZE];

    memset(data, 0, SEND_PACKET_DATA_SIZE);

    switch(myFlow.myNetWork->udpthread->currenttype)
    {
    case CAPTURE_SINGLE_WAVE:
        data[0] = 0;
        break;
    case CAPTURE_IMAGE:
    case CAPTURE_IAMGE_CONTINUOUS:
    case CAPTURE_MULTI_IMAGE:
        data[0] = 1;
        break;
    case CAPTURE_IMAGE_SYNC:
        data[0] = (flag == 0)? 0 : 1;
        break;
    case CAPTURE_BACKGROUND_WAVE:
        data[0] = 0;
        data[3] = 1;
        break;
    default:
        data[0] = 0;
        break;
    }

    switch (myFlow.myNetWork->udpthread->currenttype) {
    case CAPTURE_SINGLE_WAVE:
    case CAPTURE_BACKGROUND_WAVE:
        //正在清灰，不请求波形数据
        if (struGsh.bFlagWiping == 1) return;
    case CAPTURE_IMAGE:
        data[1] = struGsh.nImageCapTrigValue/256;
        data[2] = struGsh.nImageCapTrigValue%256;

        board = struCnfg.struLayerInfo[struGsh.nLayer].nViewBoardType[struGsh.nView];
        nUnitAddr = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].nViewUnitId[struGsh.nUnit];

        myUdpSendCmdData(CMD_CAMERA_WAVE_CAPTURE, nInt, board, nUnitAddr, data, 1);
        break;

    case CAPTURE_IAMGE_CONTINUOUS:
        data[1] = 0;
        data[2] = 0;
        board = struCnfg.struLayerInfo[struGsh.nLayer].nViewBoardType[struGsh.nView];
        nUnitAddr = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].nViewUnitId[struGsh.nUnit];

        myUdpSendCmdData(CMD_CAMERA_WAVE_CAPTURE, nInt, board, nUnitAddr, data, 1);
        break;

    case CAPTURE_IMAGE_SYNC:
        if (struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[view])
        {
            board = struCnfg.struLayerInfo[struGsh.nLayer].nViewBoardType[view];
            nUnitAddr = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[view].nViewUnitId[struGsh.nUnit];

            myUdpSendCmdData(CMD_CAMERA_IMAGE_CAPTURE_SYNC, nInt, board, nUnitAddr, data, 1);
        }
        break;

    case CAPTURE_BLOW_COUNTS:
        myUdpSendCmdData(CMD_INT_BLOW_COUNTS_CAPTURE, nInt, BOARD_TYPE_INTERFACE, BOARD_TYPE_ALL_IN_CHAIN, data, 1);
        break;
    case CAPTURE_MULTI_IMAGE:
        data[0] = 4;
        data[1] = struGsh.nImageCapTrigValue/256;
        data[2] = struGsh.nImageCapTrigValue%256;
        data[4] = (myFlow.imgHeight*struGsh.imgCaptureCnt &0xff0000)>>16;
        data[5] = (myFlow.imgHeight*struGsh.imgCaptureCnt &0xff00)>>8;
        data[6] =  myFlow.imgHeight*struGsh.imgCaptureCnt &0xff;

        board = struCnfg.struLayerInfo[struGsh.nLayer].nViewBoardType[struGsh.nView];
        nUnitAddr = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].nViewUnitId[struGsh.nUnit];
        myUdpSendCmdData(CMD_CAMERA_WAVE_CAPTURE, nInt, board, nUnitAddr, data, 1);
        break;

    case CAPTURE_MULTI_IMAGE_GET:
        data[0] = 5;
        data[1] = 0;
        data[2] = 0;
        data[4] = 0;
        data[5] = myFlow.imgHeight/256;
        data[6] = myFlow.imgHeight%256;

        board = struCnfg.struLayerInfo[struGsh.nLayer].nViewBoardType[struGsh.nView];
        nUnitAddr = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].nViewUnitId[struGsh.nUnit];
        myUdpSendCmdData(CMD_CAMERA_WAVE_CAPTURE, nInt, board, nUnitAddr, data, 1);
        break;

    default:
        break;
    }
}

void MyNetCptureThread::myUdpSendCmdData(int nCmd, char intAddr, char boardType, char boardAddr, char *data, int nCount)
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

    myFlow.myNetWork->udpSendTo(queData,SEND_PACKET_SIZE);
}

void MyNetCptureThread::run()
{
    while(1){
        for(int i = 0;i< struCnfg.struLayerInfo[0].nViewChainTotal;i++){
            if (i == struCnfg.struLayerInfo[0].nViewChainTotal)
                i = 0;
            if(struCnfg.struLayerInfo[0].nViewIsUsed[i]){
                for(int j = 0;j<struCnfg.nChuteTotal;j++){
                    if (!struCnfg.nDPMachine){
                        sleep(5);
                        continue;
                    }
                    if(j == struCnfg.nChuteTotal)
                        j = 0;
                    //                    qDebug()<<"ss cap:"<<" view"<<i<<" chute"<<j;
                    //                    if(struGsh.bSortStart == 1){ //只在色选时采图
                    if((struGsh.bSortStart == 1)&&(struCnfg.nAutoImageCaptureEnable == 1) ){ //只在色选时采图
                        if(struGsh.nPageSm == SM_BACKGROUND_SET_WIDGET
                                || struGsh.nPageSm == SM_IMAGE_CAPTURE_WIDGET
                                || struGsh.nPageSm == SM_CAMERA_SIGNAL_WIDGET
                                || struGsh.nPageSm == SM_DIVIDE_PIXEL_WIDGET
                                || struGsh.nPageSm == SM_IR_BAD_POINT_WIDGET
                                || struGsh.nPageSm == SM_NIR_DOT_CORRECTIVE_LIST_WIDGET
                                || struGsh.nPageSm == SM_UPDATE_FPGA_WDIGET
                                || struGsh.nVersionCaptureIsRun)
                        {
                            switch (struGsh.nPageSm) {
                            case SM_BACKGROUND_SET_WIDGET:
                            case SM_CAMERA_SIGNAL_WIDGET:
                            case SM_DIVIDE_PIXEL_WIDGET:
                            case SM_IR_BAD_POINT_WIDGET:
                            case SM_NIR_DOT_CORRECTIVE_LIST_WIDGET:
                                myFlow.myNetWork->setUdpRecvType(CAPTURE_SINGLE_WAVE);
                                if (!myFlow.myNetWork->udpthread->isRunning())
                                    myFlow.myNetWork->udpStart();
                                break;
                            case SM_IMAGE_CAPTURE_WIDGET:
                                //                                myFlow.myNetWork->setUdpRecvType(CAPTURE_IMAGE);
                                break;
                            case SM_UPDATE_FPGA_WDIGET:
                                break;
                            default:
                                break;
                            }
                            sleep(1);
                        }
                        else
                        {
                            if(struCnfg.struIpcCameraParams[0][i][j].ipcType == IPC_CAMERA_TYPE_NULL)
                            {
                                continue;
                            }
                            if(g_Runtime().checkUsbExist()){
                                //                                QStringList sdSpace = g_Runtime().getUsbSpace("/dev/sd");
                                //                                if (sdSpace.size() > 5){
                                //                                    QString m_used = sdSpace.at(4);
                                //                                    if(QString(m_used.remove("%")).toInt() >95){
                                //                                        sleep(60);
                                //                                        continue;
                                //                                    }
                                //                                }
                                struGsh.bNetContinuousCapture = 0;
                                myFlow.myNetWork->setUdpRecvType(CAPTURE_IMAGE,0,i,j);
                                myFlow.myNetWork->udpStart();

                                qtmutex.lock();
                                qtcondition.wait(&qtmutex,3000);
                                qtmutex.unlock();
                                msleep(100);
                                continue;

                                //                                msleep(500);
                                //                                if(struGsh.bNetContinuousCapture) continue;
                                //                                msleep(500); continue;
                            }
                            sleep(60);
                        }
                    }
                    else{//关闭色选
                        if(struGsh.nPageSm == SM_BACKGROUND_SET_WIDGET
                                || struGsh.nPageSm == SM_IMAGE_CAPTURE_WIDGET
                                || struGsh.nPageSm == SM_CAMERA_SIGNAL_WIDGET
                                || struGsh.nPageSm == SM_DIVIDE_PIXEL_WIDGET
                                || struGsh.nPageSm == SM_IR_BAD_POINT_WIDGET
                                || struGsh.nPageSm == SM_NIR_DOT_CORRECTIVE_LIST_WIDGET
                                || struGsh.nPageSm == SM_UPDATE_FPGA_WDIGET
                                || struGsh.nVersionCaptureIsRun )
                        {
                            switch (struGsh.nPageSm) {
                            case SM_BACKGROUND_SET_WIDGET:
                            case SM_CAMERA_SIGNAL_WIDGET:
                            case SM_DIVIDE_PIXEL_WIDGET:
                            case SM_IR_BAD_POINT_WIDGET:
                            case SM_NIR_DOT_CORRECTIVE_LIST_WIDGET:
                                myFlow.myNetWork->setUdpRecvType(CAPTURE_SINGLE_WAVE);
                                if (!myFlow.myNetWork->udpthread->isRunning())
                                    myFlow.myNetWork->udpStart();
                                break;
                            case SM_IMAGE_CAPTURE_WIDGET:
                                //                                myFlow.myNetWork->setUdpRecvType(CAPTURE_IMAGE);
                                break;
                            case SM_UPDATE_FPGA_WDIGET:
                                break;
                            default:
                                if (myFlow.myNetWork->udpthread->isrunning || struGsh.nRemoteImageCaptureIsRun) return;
                                break;
                            }
                            sleep(1);
                        }
                        else
                        {
                            if(!struGsh.nImageCaptureIsRun && !struGsh.nRemoteImageCaptureIsRun )
                                myFlow.myNetWork->udpStop();
                            sleep(1);
                        }
                    }
                }
            }
        }
    }
}

NetWork::NetWork(QWidget *parent) : QWidget(parent)
{
    capType = CAPTURE_SINGLE_WAVE;
    //默认读取波形

    udpthread = new MyNetWorkThread();
    setUdpRecvType(capType);

    connect(udpthread,SIGNAL(SignalSocketFailed()),\
            this,SLOT(doProcessDealSocketFailed()));
}

void NetWork::udpBind(QString ip,QString port)
{
    char* netip;
    int netport;
    FILE *fp;
    int j;
    int valueTemp;

    IP = ip;
    Port = port;
    netport = port.toInt();
    netip = (char *)ip.toLocal8Bit().constData();

#ifdef Q_OS_WIN32
    WSADATA data;
    if (WSAStartup (MAKEWORD(1, 1), &data) != 0)
    {
        fputs ("Could not initialise Winsock.\n", stderr);
        exit (1);
    }
#endif

    printf("netip is %s\n",netip);
    if((host = gethostbyname("162.254.129.100")) == NULL) {

        printf ("Usage: get name error\n");
        return;
    }
    /* Get the Socket file descriptor */
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
        printf("ERROR: Failed to obtain Socket Descriptor!\n");
        return;
    } else {
        printf ("OK: Obtain Socket Despcritor sucessfully.\n");
    }
#ifdef Q_OS_UNIX
    system("echo 8388608 > /proc/sys/net/core/rmem_default");
    system("echo 33554432 > /proc/sys/net/core/rmem_max");
#endif

    valueTemp = 1;
    //设置允许地址重用
#ifdef Q_OS_UNIX
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &valueTemp, sizeof(valueTemp));
#else
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&valueTemp, sizeof(valueTemp));
#endif

    valueTemp= 8 * 1024 * 1024;
    //设置接收缓冲区
#ifdef Q_OS_UNIX
    setsockopt(sockfd,SOL_SOCKET, SO_RCVBUF,  &valueTemp, sizeof(valueTemp));
#else
    setsockopt(sockfd,SOL_SOCKET, SO_RCVBUF,  (const char*)&valueTemp, sizeof(valueTemp));
#endif

    memset(&addr_local, 0, sizeof(addr_local));
    addr_local.sin_family = AF_INET;
    addr_local.sin_port = htons(8081);
    addr_local.sin_addr.s_addr = inet_addr("162.254.129.100");

    if(bind(sockfd, (struct sockaddr*)&addr_local, sizeof(struct sockaddr)) == -1 ){
        printf ("ERROR: Failed to bind Port %d.\n",netport);
        return;
    } else {
        printf("OK: Bind the Port %d sucessfully.\n",netport);
    }

    memset(&addr_remote, 0, sizeof(addr_remote));
    addr_remote.sin_family = AF_INET;
#ifdef Q_OS_UNIX
    inet_pton(AF_INET, "162.254.129.10", &addr_remote.sin_addr); 	// Net Address
#endif
    //    addr_remote.sin_addr.s_addr = htonl(INADDR_BROADCAST);    //UDP 广播包 远端端口
    addr_remote.sin_port = htons(5000);
}


void NetWork::udpStart()
{
    udpthread->setSocket(sockfd);
    udpthread->setAddr_remote(addr_remote);

    if (udpthread->isrunning) return; //远程已经开启了采图线程
    udpthread->isrunning = true;
    udpthread->start();
}


void NetWork::udpStop()
{
    udpthread->isrunning = false;
}

void NetWork::udpRestart()
{
#ifdef Q_OS_UNIX
    ::close(sockfd);
#else
    closesocket(sockfd);
#endif
    udpBind(IP,Port);
    udpthread->setSocket(sockfd);
    udpthread->setAddr_remote(addr_remote);
    myFlow.msleep(100);
}

int NetWork::udpSendTo(char *buf, int len)
{
    socklen_t addrlen;
    addrlen=sizeof(addr_remote);

    return sendto(sockfd,buf,len,0,(struct sockaddr *)&addr_remote,addrlen);
}

void NetWork::setBlowQueryFreq(int time)
{
    udpthread->nBlowQueryFreq = time;
}


void NetWork::setUdpRecvType(int type,int layer,int view,int unit)
{
    capType = type;
    udpthread->setRecvType(type,layer,view,unit);
}

void NetWork::setUdpRecvTimeOut(int t_s, int t_ms)
{
#if 0
#ifdef Q_OS_UNIX
    struct timeval time_out={t_s, t_ms * 1000};
    //    struct timeval time_out={timeout,10000};//10s
#else
    int time_out = t_s * 1000 + t_ms;
#endif
    //设置接收超时时间
    setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(const char*)&time_out,sizeof(time_out));
#endif

}

void NetWork::doProcessDealSocketFailed()
{
    qDebug()<<"SocketFailed";
    for(int i = 0; i < SENSOR_MAX_PIXELS; i++)
    {
        struGsh.sRowRed[i]      = 0;
        struGsh.sRowGreen[i]    = 0;
        struGsh.sRowBlue[i]     = 0;
        struGsh.sRowInf[i]      = 0;
        struGsh.sRowTotal[i]    = 0;
    }

    udpRestart();

}
