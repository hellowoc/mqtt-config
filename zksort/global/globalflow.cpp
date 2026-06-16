#include <QString>
#include <QThread>
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>
#include <QDataStream>
#include <QSettings>
#include <QTime>
#include "sys/types.h"
#include "sys/stat.h"
#include "unistd.h"
#include <sys/time.h>
#include <fcntl.h>

#include "globalflow.h"
//#include "global.h"
#include "global.h"
#include "data/jsondataconvert.h"
#include "data/mysqlite.h"
#include "bus/mqttsrv.h"
#include "myfscheck.h"
#include <QDesktopWidget>
#include <QApplication>

GlobalFlow myFlow;

GlobalFlow::GlobalFlow(QObject *parent) : QObject(parent)
{
    myNetWork = NULL;
    imgHeight = 416;
    h = 0;
    s = 0;
    v = 0;
}

/* 设置秒级的延迟 */
void GlobalFlow::sleep(int secs)
{
    QTime dieTime = QTime::currentTime().addSecs(secs);
    while(QTime::currentTime() < dieTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
    }
}

/* 设置秒秒级的延迟 */
void GlobalFlow::msleep(int mSecs)
{
    QTime dieTime = QTime::currentTime().addMSecs(mSecs);

    while(QTime::currentTime() < dieTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
    }
}

/* 排序比较函数 */
int dataSortCompare(const void *a, const void *b)
{
    return (*(unsigned char *)b - *(unsigned char *)a);  //降序
}

/* 排序 */
void GlobalFlow::dataSort(unsigned char *buf, int number, int byteSize, int riseDown)
{
    qsort(buf, number, byteSize, dataSortCompare);
}

/* 计算两个日期之间的差值 */
int GlobalFlow::dayDistance(int yearStart, int monthStart, int dayStart, int yearEnd, int monthEnd, int dayEnd)
{

    int y2, m2, d2;
    int y1, m1, d1;

    m1 = (monthStart + 9) % 12;
    y1 = yearStart - m1/10;
    d1 = 365*y1 + y1/4 - y1/100 + y1/400 + (m1*306 + 5)/10 + (dayStart - 1);

    m2 = (monthEnd + 9) % 12;
    y2 = yearEnd - m2/10;
    d2 = 365*y2 + y2/4 - y2/100 + y2/400 + (m2*306 + 5)/10 + (dayEnd - 1);

    return abs(d2 - d1);
}

/* 获取文件大小 */
long GlobalFlow::getFileSize(char *sFileName)
{
    struct stat buf;
    if(stat(sFileName, &buf) < 0) {
        return 0;
    }

    return (long)buf.st_size;
}

/* 计算温感温度 */
float GlobalFlow::calTemptureSens(int type, int value)
{
    float f_ret;
    int ret;
    int ref = 3.0;

    switch(type)
    {
    case 0:
        f_ret = (2000*(1.0/21.0 + value/(81920.0*ref))/(20.0/21.0 - value/(81920.0*ref)) - 100)/0.38;
        f_ret = ((int)(f_ret*10))*1.0/10.0;
        //        qDebug("f_ret is %f", f_ret);
        break;
    case 1:
        if((value&0x0800) == 0)
        {
            f_ret = value/16+(value%16)*1.0/16.0;
            f_ret = ((int)(f_ret*10))*1.0/10.0;
        }
        else
        {
            value = (value&0x07ff); //保留低11位，最高位符号位丢弃
            value = ~(value-1);     //取低11位的源码
            value = (value&0x07ff); //保留低11位

            f_ret = -1*value/16-(value%16)*1.0/16.0;
            f_ret = ((int)(f_ret*10))*1.0/10.0;
        }
        break;
    }

    return f_ret;
}

/* 计算气压压力 */
float GlobalFlow::calPressureValue(int value)
{
    float f_ret;
    //wj:2024-11-18 新数显表对应新的格式，避免显示与实际产生偏差
    //    f_ret = (value*5.0/4096.0-0.6)*1.001/4.4;
    f_ret = (value*5.0/4096.0-1.0)/4.0;

    return f_ret;
}

/* 设置触摸屏背光 */
void GlobalFlow::setTsBackLight(int flag)
{
    QFile file("/sys/class/graphics/fb0/blank");
    QTextStream io(&file);

    if(!file.open(QIODevice::WriteOnly))
        return;

    //    io<<flag;
    int t_flag = (flag == 0) ? 4 : 0 ;
    io<< t_flag;
    struGsh.nBacklightStat = flag;
    file.close();
}
static quint16 CRC16_Modbus(const QVector<quint8>& dataIn)
{
    quint16 crc = 0xFFFF;
    quint8 i, j;
    const quint8 *arr_buff = dataIn.data();
    quint8 len = dataIn.size();
    for (j = 0; j < len; j++) {
        crc = crc ^ *arr_buff++;
        for (i = 0; i < 8; i++) {

            if ((crc & 0x0001) > 0) {
                crc = crc >> 1;
                crc = crc ^ 0xa001;
            }
            else
                crc = crc >> 1;
        }
    }
    return (crc);
}
void GlobalFlow::TransportSerial(const QVector<quint8>& dataIn, uchar boardType, uchar boardNumber)
{
    uchar data[SEND_PACKET_DATA_SIZE];
    memset(data, 0, SEND_PACKET_DATA_SIZE);
    int idx = 0;
    data[idx++] = 0;
    for (; idx < dataIn.size() + 1; ++idx) {
        data[idx] = dataIn[idx - 1];
    }
    data[0] = idx - 1;
    mySerial.pushCom1CmdData(CMD_CTRL_TRANSPARENT,0,boardType,boardNumber,(char*)data,3);
    //	PushCmdData(TRANSPORT_SERIAL, data, idx, boardType, boardNumber);
}
void GlobalFlow::MotorCtrl(uchar objID, quint16 addr, quint16 val)
{
    //参考:欧瑞传动伺服驱动器sdp10手册
    QVector<quint8> data;
    data.clear();
    data.push_back(0x01);
    data.push_back(0x06);
    data.push_back(quint8(addr >> 8));
    data.push_back(quint8(addr));
    data.push_back(quint8(val >> 8));
    data.push_back(quint8(val));
    quint16 crc = CRC16_Modbus(data);
    data.push_back(crc);
    data.push_back(crc >> 8);
    TransportSerial(data, BOARD_TYPE_CONTROL, 0);
}

void GlobalFlow::MotorCtrl_Speed(quint16 speed)
{
    //参考:欧瑞传动伺服驱动器sdp10手册
    MotorCtrl(0, 115, speed);
}
void GlobalFlow::MotorCtrl_Init()
{
    //    //参考:欧瑞传动伺服驱动器sdp10手册
    //    if (struCnfg.nWheelRunType== 0)//逆时针
    //    {
    //        MotorCtrl(0, 1, 1 << 8);//01 06 00 01 01 00 D9 9A //Po001 （1 0） 从电机轴侧看逆时针旋转 内部寄存器速度模式
    //    }
    //    else//顺时针
    //    {
    //        MotorCtrl(0, 1, 0 << 8);//01 06 00 01 01 00 D9 9A //Po001 （1 0） 从电机轴侧看逆时针旋转 内部寄存器速度模式
    //    }

    MotorCtrl(0, 4, 0);//01 06 00 04 00 00 C8 0B //Po004 （0）外部端子使能
    MotorCtrl(0, 109, 10000);//01 06 00 6D 27 10 02 2B //Po109 （10000）加速时间10000ms
    MotorCtrl(0, 407, 0);//01 06 01 97 00 00 39 DA //Po407 (0 0) DI1端子功能：伺服使能输入，常闭
    MotorCtrl(0, 408, (1 << 8) + 12);//01 06 01 98 01 0C 08 4C //Po408 (1 12) DI2端子功能：紧急禁止，常开
    MotorCtrl(0, 409, 4);//01 06 01 99 00 04 59 DA //Po409 (0 4) DI3端子功能：速度选择1，常闭
    MotorCtrl(0, 410, 5);//01 06 01 9A 00 05 68 1A //Po410 (0 5) DI4端子功能：速度选择2，常闭
    MotorCtrl(0, 411, (1 << 8) + 1);//01 06 01 9B 01 01 39 89 //Po411 (1 1) DI5端子功能：报警复位，常开
    MotorCtrl(0, 421, (1 << 8) + 1);//01 06 01 A5 01 01 58 45 //Po421 (1 1) Do1端子功能：伺服使能输出，常开
    MotorCtrl_Speed(0);
}

void GlobalFlow::getHsv(int Red, int Green, int Blue, int& Hue, int& Sat, int& Val)
{
    float R = Red, G = Green, B = Blue;
    float H, S, V;
    float min, max, delta,tmp;

    tmp = R>G?G:R;
    min = tmp>B?B:tmp;

    tmp = R>G?R:G;
    max = tmp>B?tmp:B;

    V = max; // v
    delta = max - min;
    if( max != 0 )
    {
        S = 256*delta / max; // s
    }
    else
    {
        // r = g = b = 0 // s = 0, v is undefined
        S = 0;
        H = 0;
        Hue = H/1;
        Sat = S/1;
        Val = V/1;
        return;
    }
    if (delta == 0){
        H = 0;
        S = 0;
        Hue = H/1;
        Sat = S/1;
        Val = V/1;
        return;
    }
    else if(R == max){
        if (G >= B)
            H = (G - B)*10.0 / delta; // between yellow & magenta
        else
            H = (G - B)*10.0 / delta + 60;
    }
    else if( G == max )
        H = 20 + ( B - R )*10.0 / delta; // between cyan & yellow
    else if (B == max)
        H = 40 + ( R - G )*10.0 / delta; // between magenta & cyan
    H *= 60.0; // degrees

    Hue = H/1;
    Sat = S/1;
    Val = V/1;

    Hue = (Hue > 3599) ? 3599 : Hue;
    Sat = (Sat > 255) ? 255 : Sat;
    Val = (Val > 255) ? 255 : Val;

    return;
}

/* 获取预置方案名称 */
void GlobalFlow::getPrefixedName()
{
    switch (struCnfe.nMachine) {
    case MACHINE_VERTICAL_ZL_CS:
    case MACHINE_VERTICAL_ZL_CV:
        /* 预制方案文件名，即子节点文件名，用中文的汉语拼音表示*/
        memcpy(struCnfe.struParentPrefixedProfile[0].struSonProfile[0].sProfileName, "tongyong", sizeof("tongyong"));

        memcpy(struCnfe.struParentPrefixedProfile[1].struSonProfile[0].sProfileName, "baihe", sizeof("baihe"));
        memcpy(struCnfe.struParentPrefixedProfile[1].struSonProfile[1].sProfileName, "hetao", sizeof("hetao"));
        memcpy(struCnfe.struParentPrefixedProfile[1].struSonProfile[2].sProfileName, "gouqi", sizeof("gouqi"));
        memcpy(struCnfe.struParentPrefixedProfile[1].struSonProfile[3].sProfileName, "yimiren", sizeof("yimiren"));
        memcpy(struCnfe.struParentPrefixedProfile[1].struSonProfile[4].sProfileName, "mianzi", sizeof("mianzi"));
        memcpy(struCnfe.struParentPrefixedProfile[1].struSonProfile[5].sProfileName, "lianzi", sizeof("lianzi"));
        memcpy(struCnfe.struParentPrefixedProfile[1].struSonProfile[6].sProfileName, "putaogan", sizeof("putaogan"));
        memcpy(struCnfe.struParentPrefixedProfile[1].struSonProfile[7].sProfileName, "yumi", sizeof("yumi"));

        memcpy(struCnfe.struParentPrefixedProfile[2].struSonProfile[0].sProfileName, "baiyundou", sizeof("baiyundou"));
        memcpy(struCnfe.struParentPrefixedProfile[2].struSonProfile[1].sProfileName, "hongxiaodou", sizeof("hongxiaodou"));
        memcpy(struCnfe.struParentPrefixedProfile[2].struSonProfile[2].sProfileName, "candou", sizeof("candou"));
        memcpy(struCnfe.struParentPrefixedProfile[2].struSonProfile[3].sProfileName, "heiyundou", sizeof("heiyundou"));
        memcpy(struCnfe.struParentPrefixedProfile[2].struSonProfile[4].sProfileName, "naihuayundou", sizeof("naihuayundou"));
        memcpy(struCnfe.struParentPrefixedProfile[2].struSonProfile[5].sProfileName, "yingguohong", sizeof("yingguohong"));
        memcpy(struCnfe.struParentPrefixedProfile[2].struSonProfile[6].sProfileName, "lvdou", sizeof("lvdou"));

        memcpy(struCnfe.struParentPrefixedProfile[3].struSonProfile[0].sProfileName, "tuopihuasheng-sheng", sizeof("tuopihuasheng-sheng"));
        memcpy(struCnfe.struParentPrefixedProfile[3].struSonProfile[1].sProfileName, "tuopihuasheng-shu", sizeof("tuopihuasheng-shu"));
        memcpy(struCnfe.struParentPrefixedProfile[3].struSonProfile[2].sProfileName, "huashengguo", sizeof("huashengguo"));
        memcpy(struCnfe.struParentPrefixedProfile[3].struSonProfile[3].sProfileName, "hongpihuasheng", sizeof("hongpihuasheng"));
        memcpy(struCnfe.struParentPrefixedProfile[3].struSonProfile[4].sProfileName, "baishahuasheng", sizeof("baishahuasheng"));

        memcpy(struCnfe.struParentPrefixedProfile[4].struSonProfile[0].sProfileName, "baiguazi", sizeof("baiguazi"));
        memcpy(struCnfe.struParentPrefixedProfile[4].struSonProfile[1].sProfileName, "hongguazi", sizeof("hongguazi"));
        memcpy(struCnfe.struParentPrefixedProfile[4].struSonProfile[2].sProfileName, "youkuiren", sizeof("youkuiren"));
        memcpy(struCnfe.struParentPrefixedProfile[4].struSonProfile[3].sProfileName, "sandaomeikuiren", sizeof("sandaomeikuiren"));
        memcpy(struCnfe.struParentPrefixedProfile[4].struSonProfile[4].sProfileName, "jiaoguazi", sizeof("jiaoguazi"));
        memcpy(struCnfe.struParentPrefixedProfile[4].struSonProfile[5].sProfileName, "jiaoguaziren", sizeof("jiaoguaziren"));
        memcpy(struCnfe.struParentPrefixedProfile[4].struSonProfile[6].sProfileName, "nanguaziren", sizeof("nanguaziren"));


        /* 预制方案父节点和子节点名称，只有中英文*/
        if(g_myLan().g_languageIndex == LANG_CHS || g_myLan().g_languageIndex == LANG_CHT) {
            memcpy(struCnfe.struParentPrefixedProfile[0].sMaterialName, "通用类", sizeof("通用类"));
            memcpy(struCnfe.struParentPrefixedProfile[0].struSonProfile[0].sMaterialName, "通用", sizeof("通用"));

            memcpy(struCnfe.struParentPrefixedProfile[1].sMaterialName, "综合类", sizeof("综合类"));
            memcpy(struCnfe.struParentPrefixedProfile[1].struSonProfile[0].sMaterialName, "百合", sizeof("百合"));
            memcpy(struCnfe.struParentPrefixedProfile[1].struSonProfile[1].sMaterialName, "核桃", sizeof("核桃"));
            memcpy(struCnfe.struParentPrefixedProfile[1].struSonProfile[2].sMaterialName, "枸杞", sizeof("枸杞"));
            memcpy(struCnfe.struParentPrefixedProfile[1].struSonProfile[3].sMaterialName, "薏米", sizeof("薏米"));
            memcpy(struCnfe.struParentPrefixedProfile[1].struSonProfile[4].sMaterialName, "棉籽", sizeof("棉籽"));
            memcpy(struCnfe.struParentPrefixedProfile[1].struSonProfile[5].sMaterialName, "莲子", sizeof("莲子"));
            memcpy(struCnfe.struParentPrefixedProfile[1].struSonProfile[6].sMaterialName, "葡萄干", sizeof("葡萄干"));
            memcpy(struCnfe.struParentPrefixedProfile[1].struSonProfile[7].sMaterialName, "玉米", sizeof("玉米"));

            memcpy(struCnfe.struParentPrefixedProfile[2].sMaterialName, "豆类", sizeof("豆类"));
            memcpy(struCnfe.struParentPrefixedProfile[2].struSonProfile[0].sMaterialName, "白芸豆", sizeof("白芸豆"));
            memcpy(struCnfe.struParentPrefixedProfile[2].struSonProfile[1].sMaterialName, "红小豆", sizeof("红小豆"));
            memcpy(struCnfe.struParentPrefixedProfile[2].struSonProfile[2].sMaterialName, "绿豆", sizeof("绿豆"));

            memcpy(struCnfe.struParentPrefixedProfile[3].sMaterialName, "花生", sizeof("花生"));
            memcpy(struCnfe.struParentPrefixedProfile[3].struSonProfile[0].sMaterialName, "花生果", sizeof("花生果"));
            memcpy(struCnfe.struParentPrefixedProfile[3].struSonProfile[1].sMaterialName, "红皮花生", sizeof("红皮花生"));
            memcpy(struCnfe.struParentPrefixedProfile[3].struSonProfile[2].sMaterialName, "白沙花生", sizeof("白沙花生"));

            memcpy(struCnfe.struParentPrefixedProfile[4].sMaterialName, "籽仁", sizeof("籽仁"));
            memcpy(struCnfe.struParentPrefixedProfile[4].struSonProfile[0].sMaterialName, "白瓜子", sizeof("白瓜子"));
            memcpy(struCnfe.struParentPrefixedProfile[4].struSonProfile[1].sMaterialName, "红瓜子", sizeof("红瓜子"));
        }
        else {
            memcpy(struCnfe.struParentPrefixedProfile[0].sMaterialName, "General", sizeof("General"));
            memcpy(struCnfe.struParentPrefixedProfile[0].struSonProfile[0].sMaterialName, "General", sizeof("General"));

            memcpy(struCnfe.struParentPrefixedProfile[1].sMaterialName, "Misc", sizeof("Misc"));
            memcpy(struCnfe.struParentPrefixedProfile[1].struSonProfile[0].sMaterialName, "Lily", sizeof("Lily"));
            memcpy(struCnfe.struParentPrefixedProfile[1].struSonProfile[1].sMaterialName, "Walnut", sizeof("Walnut"));
            memcpy(struCnfe.struParentPrefixedProfile[1].struSonProfile[2].sMaterialName, "Wolfberry", sizeof("Wolfberry"));
            memcpy(struCnfe.struParentPrefixedProfile[1].struSonProfile[3].sMaterialName, "Pearl Barley Kernel", sizeof("Pearl Barley Kernel"));
            memcpy(struCnfe.struParentPrefixedProfile[1].struSonProfile[4].sMaterialName, "Cottonseed", sizeof("Cottonseed"));
            memcpy(struCnfe.struParentPrefixedProfile[1].struSonProfile[5].sMaterialName, "Lotus Nut", sizeof("Lotus Nut"));
            memcpy(struCnfe.struParentPrefixedProfile[1].struSonProfile[6].sMaterialName, "Raisin", sizeof("Raisin"));
            memcpy(struCnfe.struParentPrefixedProfile[1].struSonProfile[7].sMaterialName, "Corn", sizeof("Corn"));

            memcpy(struCnfe.struParentPrefixedProfile[2].sMaterialName, "Bean", sizeof("Bean"));
            memcpy(struCnfe.struParentPrefixedProfile[2].struSonProfile[0].sMaterialName, "White Kidney Bean", sizeof("White Kidney Bean"));
            memcpy(struCnfe.struParentPrefixedProfile[2].struSonProfile[1].sMaterialName, "Small Red Bean", sizeof("Small Red Bean"));
            memcpy(struCnfe.struParentPrefixedProfile[2].struSonProfile[2].sMaterialName, "Mung Bean", sizeof("Mung Bean"));

            memcpy(struCnfe.struParentPrefixedProfile[3].sMaterialName, "Peanut", sizeof("Peanut"));
            memcpy(struCnfe.struParentPrefixedProfile[3].struSonProfile[0].sMaterialName, "Peanut in Shell", sizeof("Peanut in Shell"));
            memcpy(struCnfe.struParentPrefixedProfile[3].struSonProfile[1].sMaterialName, "Red-peel Peanut", sizeof("Red-peel Peanut"));
            memcpy(struCnfe.struParentPrefixedProfile[3].struSonProfile[2].sMaterialName, "White Peel Peanut", sizeof("White Peel Peanut"));

            memcpy(struCnfe.struParentPrefixedProfile[4].sMaterialName, "Seeds", sizeof("Seeds"));
            memcpy(struCnfe.struParentPrefixedProfile[4].struSonProfile[0].sMaterialName, "White Seed", sizeof("White Seed"));
            memcpy(struCnfe.struParentPrefixedProfile[4].struSonProfile[1].sMaterialName, "Red Seed", sizeof("Red Seed"));
        }
        break;
    case MACHINE_VERTICAL_DM_RG:
    case MACHINE_VERTICAL_DM_ZK:
        memcpy(struCnfe.struParentPrefixedProfile[0].struSonProfile[0].sProfileName, "diffpositive", sizeof("diffpositive"));
        memcpy(struCnfe.struParentPrefixedProfile[0].struSonProfile[1].sProfileName, "diffnegatives", sizeof("diffnegatives"));
        memcpy(struCnfe.struParentPrefixedProfile[0].struSonProfile[2].sProfileName, "whitepositive", sizeof("whitepositive"));
        memcpy(struCnfe.struParentPrefixedProfile[0].struSonProfile[3].sProfileName, "whitenegative", sizeof("whitenegative"));
        memcpy(struCnfe.struParentPrefixedProfile[0].struSonProfile[4].sProfileName, "yellowwhite", sizeof("yellowwhite"));
        memcpy(struCnfe.struParentPrefixedProfile[0].struSonProfile[5].sProfileName, "yellowtrans", sizeof("yellowtrans"));
        memcpy(struCnfe.struParentPrefixedProfile[0].struSonProfile[6].sProfileName, "yellowyellownegative", sizeof("yellowyellownegative"));
        memcpy(struCnfe.struParentPrefixedProfile[0].struSonProfile[7].sProfileName, "whiteyellownegative", sizeof("whiteyellownegative"));
        memcpy(struCnfe.struParentPrefixedProfile[0].struSonProfile[8].sProfileName, "userdefined", sizeof("userdefined"));

        if(g_myLan().g_languageIndex == LANG_CHS || g_myLan().g_languageIndex == LANG_CHT)
        {
            memcpy(struCnfe.struParentPrefixedProfile[0].sMaterialName, "大米类", sizeof("大米类"));
            memcpy(struCnfe.struParentPrefixedProfile[0].struSonProfile[0].sMaterialName, "异色粒正选", sizeof("异色粒正选"));
            memcpy(struCnfe.struParentPrefixedProfile[0].struSonProfile[1].sMaterialName, "异色粒反选", sizeof("异色粒反选"));
            memcpy(struCnfe.struParentPrefixedProfile[0].struSonProfile[2].sMaterialName, "乳白色正选", sizeof("乳白色正选"));
            memcpy(struCnfe.struParentPrefixedProfile[0].struSonProfile[3].sMaterialName, "乳白色反选", sizeof("乳白色反选"));
            memcpy(struCnfe.struParentPrefixedProfile[0].struSonProfile[4].sMaterialName, "黄白同选", sizeof("黄白同选"));
            memcpy(struCnfe.struParentPrefixedProfile[0].struSonProfile[5].sMaterialName, "黄透同选", sizeof("黄透同选"));
            memcpy(struCnfe.struParentPrefixedProfile[0].struSonProfile[6].sMaterialName, "黄+黄反选", sizeof("黄+黄反选"));
            memcpy(struCnfe.struParentPrefixedProfile[0].struSonProfile[7].sMaterialName, "黄+白反选", sizeof("黄+白反选"));
            memcpy(struCnfe.struParentPrefixedProfile[0].struSonProfile[8].sMaterialName, "自定义模式", sizeof("自定义模式"));
        }
        else
        {
            memcpy(struCnfe.struParentPrefixedProfile[0].sMaterialName, "Rice", sizeof("Rice"));
            memcpy(struCnfe.struParentPrefixedProfile[0].struSonProfile[0].sMaterialName, "Diff Positive", sizeof("Diff Positive"));
            memcpy(struCnfe.struParentPrefixedProfile[0].struSonProfile[1].sMaterialName, "Diff Negatives", sizeof("Diff Negatives"));
            memcpy(struCnfe.struParentPrefixedProfile[0].struSonProfile[2].sMaterialName, "White Positive", sizeof("White Positive"));
            memcpy(struCnfe.struParentPrefixedProfile[0].struSonProfile[3].sMaterialName, "White Negative", sizeof("White Negative"));
            memcpy(struCnfe.struParentPrefixedProfile[0].struSonProfile[4].sMaterialName, "Yellow White", sizeof("Yellow White"));
            memcpy(struCnfe.struParentPrefixedProfile[0].struSonProfile[5].sMaterialName, "Yellow Trans", sizeof("Yellow Trans"));
            memcpy(struCnfe.struParentPrefixedProfile[0].struSonProfile[6].sMaterialName, "Yellow Yellow Negative", sizeof("Yellow Yellow Negative"));
            memcpy(struCnfe.struParentPrefixedProfile[0].struSonProfile[7].sMaterialName, "White Yellow Negative", sizeof("White Yellow Negative"));
            memcpy(struCnfe.struParentPrefixedProfile[0].struSonProfile[8].sMaterialName, "User Defined", sizeof("User Defined"));
        }
        break;
    default:
        break;
    }
}

int GlobalFlow::getParamsFileStaus()
{
    FILE *fp;
    QFile file;

    struCnfs.value = ALL_PARAMS_FILE_OK;

    if (getFileSize(CNF_PARAMS_FILE_STATUS) > 0) {
        if (!(fp = fopen(CNF_PARAMS_FILE_STATUS, "r"))) {
            qDebug("getParamsFileStatus: can not get getParamsFileStatus parameters, using default");
            return 0;
        }
        fread(&struCnfs, sizeof(struct struCnfFileStatus), 1, fp);
        fclose(fp);

        usleep(100000);
        g_Runtime().copyFileToPath(CNF_PARAMS_FILE_STATUS, CNF_PARAMS_FILE_STATUS_B,true);
        usleep(100000);
        qDebug("struCnfs.value is %d", struCnfs.value);
    }
    else if (getFileSize(CNF_PARAMS_FILE_STATUS_B) > 0) {
        if (!(fp = fopen(CNF_PARAMS_FILE_STATUS_B, "r"))) {
            qDebug("getParamsFileStatus_2: can not get getParamsFileStatus parameters, using default");
            return 0;
        }
        fread(&struCnfs, sizeof(struct struCnfFileStatus), 1, fp);
        fclose(fp);

        usleep(100000);
        g_Runtime().copyFileToPath(CNF_PARAMS_FILE_STATUS_B, CNF_PARAMS_FILE_STATUS,true);
        usleep(100000);
        qDebug("_t_cnfs.value is %d", struCnfs.value);
    }

    return 0;
}

int GlobalFlow::saveParamsFileStaus(int value)
{
    FILE *fp;
    QFile file;
    int i;
    struct struCnfFileStatus _t_Cnfs;
    bool bSaveIsOk = false;

    struCnfs.value = value;

    for(i = 0; i < 3; i++) {
        if (!(fp = fopen(CNF_PARAMS_FILE_STATUS, "w"))) {
            qDebug("saveParamsFileStatus: can not save FileStatus parameters, using default");
            return false;
        }
        fwrite(&struCnfs, sizeof(struct struCnfFileStatus), 1, fp);
        fclose(fp);
        usleep(100000);

        if (!(fp = fopen(CNF_PARAMS_FILE_STATUS, "r"))) {
            fprintf(stderr, "getParamsFileStatus: can not get FileStatus parameters, using default\n");
            return false;
        }
        memset(&_t_Cnfs, 0, sizeof(struct struCnfFileStatus));
        fread(&_t_Cnfs, sizeof(struct struCnfFileStatus), 1, fp);
        fclose(fp);
        if(struCnfs.value == _t_Cnfs.value ) {
            bSaveIsOk = true;
            g_Runtime().copyFileToPath(CNF_PARAMS_FILE_STATUS, CNF_PARAMS_FILE_STATUS_B,true);
            usleep(100000);
            break;
        }
    }
    return 0;
}

int GlobalFlow::saveMemoryDetectedFileStaus(QString path)
{
    FILE *fp;
    int i;
    int count[2];
    bool bSaveIsOk = false;

    QString filename = path+ "/.memstatus.txt";
    const char* file = QString(filename).toLocal8Bit().constData();

    count[0] = struGsh.nCounter;
    count[1] = 0;

    for(i = 0; i < 3; i++) {
        if (!(fp = fopen(file, "w"))) {
            qDebug("saveMemoryDetectedFileStaus: can not save FileStatus parameters");
            return 0;
        }
        fwrite(&count[0], sizeof(struGsh.nCounter), 1, fp);
        fclose(fp);
        usleep(100000);

        if (!(fp = fopen(file, "r"))) {
            fprintf(stderr, "getMemoryDetectedFileStaus: can not get FileStatus parameters, using default\n");
            return false;
        }
        fread(&count[1], sizeof(int), 1, fp);
        fclose(fp);
        if(count[0] == count[1] ) {
            break;
        }
    }
    return 1;
}

void GlobalFlow::initProfile()
{
    int i, j, k, m, n;
    int nWipeDuration, nWipeInterval;


    memset(&struCnfp, 0, sizeof(struCnfProfile));
    memset(&_t_struCnfp, 0, sizeof(struCnfProfile));

    switch (struCnfg.nMachineType) {
    case MACHINE_VERTICAL_DM_RG:
    case MACHINE_VERTICAL_ZL_CS:
    case MACHINE_VERTICAL_ZL_CV:
    case MACHINE_VERTICAL_DM_ZK:
        struCnfp.nViewSensorType[0][0]     = SENSOR_G2;   //前主可见
        struCnfp.nViewSensorType[0][1]     = SENSOR_G2;   //后主可见
        struCnfp.nViewSensorType[0][2]     = SENSOR_C1;   //前辅850
        struCnfp.nViewSensorType[0][3]     = SENSOR_C1;   //后辅1450
        break;
    case MACHINE_LD2_ZL_GP:
        for(i = 0; i < MAX_VIEW; i++)
        {
            struCnfp.nViewSensorType[0][i]     = SENSOR_G2;   //前主可见
        }
        break;
    default:
        struCnfp.nViewSensorType[0][0]     = SENSOR_T1;   //前主可见
        struCnfp.nViewSensorType[0][1]     = SENSOR_T1;   //后主可见
        struCnfp.nViewSensorType[0][2]     = SENSOR_C1;   //前辅850
        struCnfp.nViewSensorType[0][3]     = SENSOR_C1;   //后辅1450
        break;
    }



    /* 背景条件*/
    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(j = 0; j < MAX_VIEW; j++)
        {
            for(k = 0; k < 3; k++)
            {
                if(struCnfp.nViewSensorType[0][j] == SENSOR_C1)
                {
                    struCnfp.nBackgroundColor[i][j][k] = BACKGROUND_COLOR_BLACK_WHITE;           // 默认蓝背景
                    struCnfp.nBackgroundColorValueLow[i][j][k] = 0;    // 默认背景阈值 下阈
                    struCnfp.nBackgroundColorValueUp[i][j][k] = 20;    // 默认背景阈值，上阈
                    struCnfp.nBackgroundColorProLow[i][j][k] = 60;     // 默认背景比例，下阈
                    struCnfp.nBackgroundColorProUp[i][j][k] = 100;     // 默认背景比例，上阈
                }
                else if(struCnfp.nViewSensorType[0][j] == SENSOR_D2)
                {
                    struCnfp.nBackgroundColor[i][j][k] = BACKGROUND_COLOR_BLACK_WHITE;           // 默认蓝背景
                    struCnfp.nBackgroundColorValueLow[i][j][k] = 150;    // 默认背景阈值 下阈
                    struCnfp.nBackgroundColorValueUp[i][j][k] = 255;    // 默认背景阈值，上阈
                    struCnfp.nBackgroundColorProLow[i][j][k] = 60;     // 默认背景比例，下阈
                    struCnfp.nBackgroundColorProUp[i][j][k] = 100;     // 默认背景比例，上阈
                    struCnfp.nBackgroundColorSatUp[i][j][k] = 255;     //默认背景饱和度，上阈（整瓶机专用）
                    struCnfp.nSensorD2Mode[i][j] = 1;

                }
                else
                {
                    struCnfp.nBackgroundColor[i][j][k] = BACKGROUND_COLOR_BLUE;           // 默认蓝背景
                    struCnfp.nBackgroundColorValueLow[i][j][k] = 70;    // 默认背景阈值 下阈
                    struCnfp.nBackgroundColorValueUp[i][j][k] = 255;    // 默认背景阈值，上阈
                    struCnfp.nBackgroundColorProLow[i][j][k] = 40;     // 默认背景比例，下阈
                    struCnfp.nBackgroundColorProUp[i][j][k] = 100;     // 默认背景比例，上阈
                    struCnfp.nBackgroundColorSatUp[i][j][k] = 255;     //默认背景饱和度，上阈（整瓶机专用）
                    struCnfp.nSensorD2Mode[i][j] = 0;


                }
                struCnfp.nBackgroundColorValueVLow[i][j][k] = 195;     // 默认背景比例，下阈
                struCnfp.nBackgroundColorValueVUp[i][j][k] = 255;     // 默认背景比例，上阈
                struCnfp.nSensResizeMode[i][j] = 0;
            }
            struCnfp.nSensorRowFre[i][j] = 260;
            struCnfp.nSenseBrightSelection[i][j] = 1;
            struCnfp.nTimeDivisionScheme[i][j] = 0;
            struCnfp.nCameraBoardMode[i][j] = 1;    //
        }
    }

    for(j = 0; j < MAX_VIEW; j++)
    {
        /* 根据视传感器的类型，设置相机相关参数 */
        switch(struCnfp.nViewSensorType[0][j])
        {
        case SENSOR_T1:
        case SENSOR_T1_850:
        case SENSOR_G2:
        case SENSOR_G3:
            struCnfp.nSensorDim[0][j]     = 3;
            struCnfp.nSensBitWidth[0][j]  = 8;
            struCnfp.nSensorPixel[0][j]   = SENSOR_PIXELS_2048; //11代表2048分辨率
            struCnfp.nImageSplice[0][j] = 0;
            struCnfp.nMirror[0][j] = 0;
            struCnfp.nSensDropLine[0][j] = 0;
            struCnfp.nSensCmdSpeed[0][j] = 1;
            struCnfp.nRealTimeCorrection[0][j] = 0;
            break;
        case SENSOR_C1:
            struCnfp.nSensorDim[0][j]     = 2;
            struCnfp.nSensBitWidth[0][j]  = 8;
            struCnfp.nSensorPixel[0][j]   = SENSOR_PIXELS_512; //11代表2048分辨率
            struCnfp.nImageSplice[0][j] = 0;
            struCnfp.nSensorDataType[0][j] = 0;
            struCnfp.nMirror[0][j] = 0;
            struCnfp.nSensDropLine[0][j] = 0;
            struCnfp.nSensCmdSpeed[0][j] = 0;
            struCnfp.nRealTimeCorrection[0][j] = 0;
            break;
        case SENSOR_D2:
            struCnfp.nSensorDim[0][j]     = 4;
            struCnfp.nSensorDimD2[0][j]   = 4;
            struCnfp.nSensBitWidth[0][j]  = 8;
            struCnfp.nSensorPixel[0][j]  = SENSOR_PIXELS_2048; //11代表2048分辨率
            struCnfp.nImageSplice[0][j] = 0;
            struCnfp.nSensorDataType[0][j] = 0;
            struCnfp.nMirror[0][j] = 0;

            struCnfp.nSensDropLine[0][j] = 0;
            struCnfp.nSensCmdSpeed[0][j] = 0;

            struCnfp.nRealTimeCorrection[0][j] = 0;
            break;
        case SENSOR_H1:
            struCnfp.nSensorDim[0][j]     = 1;
            struCnfp.nSensBitWidth[0][j]  = 8;
            struCnfp.nSensorPixel[0][j]   = SENSOR_PIXELS_512; //11代表2048分辨率
            struCnfp.nImageSplice[0][j] = 0;
            struCnfp.nSensorDataType[0][j] = 0;
            struCnfp.nMirror[0][j] = 0;
            struCnfp.nSensDropLine[0][j] = 0;
            struCnfp.nSensCmdSpeed[0][j] = 0;
            struCnfp.nRealTimeCorrection[0][j] = 0;
            break;
        default:
            break;
        }
    }
    /* 灯使能 */
    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(m = 0; m < MAX_LAMP_BOARD_PER_INTERFACE; m++)   //1个灯链上最多灯光板数
        {
            for(n = 0; n < MAX_LAMP_PER_BOARD; n++)   //1个灯光板上最多灯数
            {
                if(struCnfg.stuLayerLampCfg.nLampPortEnable[m][n] != 0)
                {
                    struCnfp.nLampEnable[i][m][0]   = 0;
                    struCnfp.nLampValue[i][m][n]    = 10;
                    struCnfp.nLampSeq[i][m][n]   = 0;
                }
            }
        }
    }

    /* 算法使能 */
    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(j = 0; j < MAX_VIEW; j++)
        {
            for(m = 0; m < MAX_GROUP_IDENTIFY; m++)
            {
                for(n = 0; n < MAX_ARITH_TOTAL; n++)
                {
                    struCnfp.nGroupArithEnable[i][j][m][n] = 0;
                    struCnfp.nGroupArithEdgeEnable[i][j][m][n] = 0;
                }
            }
        }
    }

    /* 大米机型相机设置 */
    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(j = 0; j < MAX_VIEW; j++)
        {
            for(k = 0; k < MAX_VIEW_UNIT; k++)
            {
                struCnfp.nProfileAnaGainRed[i][j][k] = 200;
                struCnfp.nProfileAnaGainGreen[i][j][k] = 200;
                struCnfp.nProfileAnaGainBlue[i][j][k] = 200;
                struCnfp.nProfileAnaGainNir[i][j][k] = 200;

                struCnfp.nProfileGainTargetRed[i][j][k] = 200;
                struCnfp.nProfileGainTargetGreen[i][j][k] = 200;
                struCnfp.nProfileGainTargetBlue[i][j][k] = 200;
                struCnfp.nProfileGainTargetNir[i][j][k] = 200;

                struCnfp.nProfileDigiGainRed[i][j][k] = 2048;
                struCnfp.nProfileDigiGainGreen[i][j][k] = 2048;
                struCnfp.nProfileDigiGainBlue[i][j][k] = 2048;
                struCnfp.nProfileDigiGainNir[i][j][k] = 2048;
                struCnfp.nCameraBoardUserMode[i][j][k] = 1;
            }
        }
    }

    //d2图像模式
    for(int i = 0; i < 4;i++)
    {
        memset(struCnfp.sD2ImgModeName[i], 0, 16);
    }

    memcpy(struCnfp.sD2ImgModeName[0], "Mode1", sizeof("Mode1"));
    memcpy(struCnfp.sD2ImgModeName[1], "Mode2", sizeof("Mode2"));
    memcpy(struCnfp.sD2ImgModeName[2], "Mode3", sizeof("Mode3"));
    memcpy(struCnfp.sD2ImgModeName[3], "Mode4", sizeof("Mode4"));

    /* 识别参数设置方式1 */
    struCnfp.nMatCopySetMode = MAT_COPY_ALL_ALONE;      //全部独立
    //    if((struCnfg.struLayerInfo[0].nSplice == 1)&&(struCnfg.nChuteTotal == 1)) //1通道，且带带分割的机器
    //    {
    //        struCnfp.nMatCopySetMode = MAT_COPY_SECOND_ALIKE_FIRST;
    //    }
    //    else
    //    {
    //        struCnfp.nMatCopySetMode = MAT_COPY_ALL_ALONE;      //全部独立
    //    }

    /* 更新算法名称 */
    getArithmeticName();

    /* 识别组参数 */
    getGroupIdentify();

    /* 剔除组参数 */
    getGroupTick();

    /* 本地模型参数 */
    getLocalIpcParams();

    /* 控制板参数 */
    switch (struCnfe.nMachine) {
    case MACHINE_VERTICAL_DM_RG:
    case MACHINE_VERTICAL_ZL_CS:
    case MACHINE_VERTICAL_ZL_CV:
        nWipeDuration = 16;
        nWipeInterval = 30;
        break;
    case MACHINE_VERTICAL_DM_ZK:
        nWipeDuration = 16;
        nWipeInterval = 30;
        break;
    default:
        nWipeDuration = 16;
        nWipeInterval = 30;
        break;
    }
    for(int i = 0; i < MAX_CTRL_BOARD_PER_INTERFACE;i++){
        struCnfp.struGroupCtrl[i].nWipeUnion                  = 0;
        struCnfp.struGroupCtrl[i].nWipeDuration               = nWipeDuration;
        struCnfp.struGroupCtrl[i].nWipeInterval               = nWipeInterval;
        struCnfp.struGroupCtrl[i].nWipeTimes                  = 0;
        struCnfp.struGroupCtrl[i].nWipeWindDuration           = 3;
        struCnfp.struGroupCtrl[i].nWipeWindInterval           = 5;
        struCnfp.struGroupCtrl[i].nWipeWaterDuration          = 0;
        struCnfp.struGroupCtrl[i].nConveyorIsRun              = 0;
        struCnfp.struGroupCtrl[i].nConveyorSpeed              = 12000;

        struCnfp.struGroupCtrl[i].nBackgroundFront            = 100;
        struCnfp.struGroupCtrl[i].nBackgroundRear             = 100;
        struCnfp.struGroupCtrl[i].nFeedSetType                = 0;
        struCnfp.struGroupCtrl[i].nWipeSortOnOff              = 0;
        struCnfp.struGroupCtrl[i].nWipeFeedOnOff              = 0;
        struCnfp.struGroupCtrl[i].nFeederTea                  = 0;
        for (j = 0; j < MAX_FEEDER; j++) {
            struCnfp.struGroupCtrl[i].nFeederValueMain[j] = 0;
            struCnfp.struGroupCtrl[i].nFeederValueVice[j] = 0;
        }
    }
    memcpy(struCnfp.sCurBigstatisname[0], "AAA", sizeof("AAA"));

    memcpy(&_t_struCnfp, &struCnfp, sizeof(struCnfProfile));
}

/* 初始化相机参数 */
void GlobalFlow::getCameraParams()
{
    int i, j, k;

    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        /************************   相机参数   **********************************/
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            for(k = 0; k < MAX_VIEW_UNIT; k++)
            {
                switch(struCnfp.nViewSensorType[0][j])
                {
                case SENSOR_T1:
                case SENSOR_T1_850:
                case SENSOR_D2:
                    struCnfg.struLayerInfo[i].stuCameraInfo[j].nChannelBegin[k]     = 29;        //起始像素为0，表示像素划分无效
                    struCnfg.struLayerInfo[i].stuCameraInfo[j].nChannelEnd[k]       = 995;
                    struCnfg.struLayerInfo[i].stuCameraInfo[j].nEjectBegin[k]     = 1;
                    struCnfg.struLayerInfo[i].stuCameraInfo[j].nEjectEnd[k]       = VAVLE_EJECTORS_64;
                    break;
                case SENSOR_G2:
                    struCnfg.struLayerInfo[i].stuCameraInfo[j].nChannelBegin[k]     = 50;        //起始像素为0，表示像素划分无效
                    struCnfg.struLayerInfo[i].stuCameraInfo[j].nChannelEnd[k]       = 1998;
                    struCnfg.struLayerInfo[i].stuCameraInfo[j].nEjectBegin[k]     = 1;
                    struCnfg.struLayerInfo[i].stuCameraInfo[j].nEjectEnd[k]       = VAVLE_EJECTORS_64;
                    break;
                case SENSOR_C1:
                    struCnfg.struLayerInfo[i].stuCameraInfo[j].nChannelBegin[k]     = 50;        //起始像素为0，表示像素划分无效
                    struCnfg.struLayerInfo[i].stuCameraInfo[j].nChannelEnd[k]       = 450;
                    break;
                default:
                    struCnfg.struLayerInfo[i].stuCameraInfo[j].nChannelBegin[k]     = 20;        //起始像素为0，表示像素划分无效
                    struCnfg.struLayerInfo[i].stuCameraInfo[j].nChannelEnd[k]       = 2028;
                    struCnfg.struLayerInfo[i].stuCameraInfo[j].nEjectBegin[k]     = 1;
                    struCnfg.struLayerInfo[i].stuCameraInfo[j].nEjectEnd[k]       = VAVLE_EJECTORS_64;
                    break;
                }

                struCnfg.struLayerInfo[i].stuCameraInfo[j].nBiasingRed[k]       = 40;
                struCnfg.struLayerInfo[i].stuCameraInfo[j].nBiasingGreen[k]     = 40;
                struCnfg.struLayerInfo[i].stuCameraInfo[j].nBiasingBlue[k]      = 40;
                struCnfg.struLayerInfo[i].stuCameraInfo[j].nBiasingNir[k]       = 40;


                struCnfg.struLayerInfo[i].stuCameraInfo[j].anaGainLockStatus[k] = 0;
                struCnfg.struLayerInfo[i].stuCameraInfo[j].anaGainLockColor[k]  = 10;

                struCnfg.struLayerInfo[i].stuCameraInfo[j].nDigiGainRed[k]      = 200;
                struCnfg.struLayerInfo[i].stuCameraInfo[j].nDigiGainGreen[k]    = 200;
                struCnfg.struLayerInfo[i].stuCameraInfo[j].nDigiGainBlue[k]     = 200;
                struCnfg.struLayerInfo[i].stuCameraInfo[j].nDigiGainNir[k]      = 200;

                for(int l = 0; l < MAX_CALIBRATE_SETS;l++)
                {
                    struCnfg.struLayerInfo[i].stuCameraInfo[j].nAnaGainRed[k][l]       = 0;
                    struCnfg.struLayerInfo[i].stuCameraInfo[j].nAnaGainGreen[k][l]     = 0;
                    struCnfg.struLayerInfo[i].stuCameraInfo[j].nAnaGainBlue[k][l]      = 0;
                    struCnfg.struLayerInfo[i].stuCameraInfo[j].nAnaGainNir[k][l]       = 10;
                    if(j == 0)
                    {
                        struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibrateRed[k][l]      = 130;
                        struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibrateGreen[k][l]    = 130;
                        struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibrateBlue[k][l]     = 130;
                        struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibrateNir[k][l]      = 75;
                    }
                    else
                    {
                        struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibrateRed[k][l]      = 130;
                        struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibrateGreen[k][l]    = 130;
                        struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibrateBlue[k][l]     = 130;
                        struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibrateNir[k][l]      = 75;
                    }
                }
                struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibratePixelRed = 0;
                struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibratePixelRedVert[k] = 0;
                struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibratePixelGreen = 0;
                struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibratePixelGreenVert[k] = 0;
                struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibratePixelBlue = 0;
                struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibratePixelBlueVert[k] = 0;
                struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibratePixelNir = 0;
                struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibratePixelNirVert[k] = 0;

                struCnfg.struLayerInfo[i].stuCameraInfo[j].nNirTdiEnable = 0;
                struCnfg.struLayerInfo[i].stuCameraInfo[j].nNirUpdownEnable = 0;

                getCameraEjectorPixel(i, j, k);
            }
        }
    }
    memcpy(struCnfp.stuCameraInfo,struCnfg.struLayerInfo[0].stuCameraInfo,sizeof(struCnfp.stuCameraInfo));
}

/* 获取全局参数中的层和组信息，默认12个通道， 5次选, 4视*/
void GlobalFlow::getVertRiceGeneralLayerInfo()
{
    int i, j, k, m, n, tmpTotal = 0;
    int tmpViewNameIndex[MAX_VIEW];
    int tmpLayViewUnitCount[MAX_VIEW];  //每层每视相机单元数量/喷阀板数量

    int tmpSortTimes = MAX_GROUP_IDENTIFY;   //5次选
    int tmpTimesUnitCount[MAX_GROUP_IDENTIFY];  //每次色选的滑槽数量

    struCnfg.nMachineAttr                   = MACHINE_TYPE_VERTICAL;
    struCnfg.nMachineSpliceType             = MACHINE_SPLICE_TYPE_NULL;
    struCnfg.nLayerTotal                    = 1;
    struCnfg.nChuteTotal                    = 12;
    struCnfg.nCtrlBoardTotal                = 1;
    struCnfg.nFeedMainTotal                 = 12;   // 12通道,12个振动器
    struCnfg.nFeedViceTotal                 = 0;
    struCnfg.nVavleTimeSetMode              = 1; //22/6/27 ls,ld默认按次设置延时
    struCnfg.nValveRatio                    = 0;
    struCnfg.nCorssEjector                  = 0;

    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nVavleChainTotal; j++)
        {
            struCnfg.nVavleBoardMode[i][j] = 1;    // 每吹的模式默认都是1
        }
    }

    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(j = 0; j < MAX_VIEW; j++)
        {
            struCnfp.nCameraCompressMode[i][j] = 0;    // 每视的模式默认都是模式1
        }
    }

    /* 设置每视的名称索引 */
    tmpViewNameIndex[0] = MAIN_FRONT;
    tmpViewNameIndex[1] = MAIN_REAR;
    tmpViewNameIndex[2] = VICE_FRONT;
    tmpViewNameIndex[3] = VICE_REAR;

    //每层每视的相机板数量&喷阀板数量 和滑槽数相等
    for(i = 0; i < MAX_VIEW; i++)
    {
        tmpLayViewUnitCount[i] = struCnfg.nChuteTotal;
    }

    //每次色选的相机板数量&喷阀板数量设置
    tmpTimesUnitCount[0] = 5;
    tmpTimesUnitCount[1] = 3;
    tmpTimesUnitCount[2] = 2;
    tmpTimesUnitCount[3] = 1;
    tmpTimesUnitCount[4] = 1;

    /****************  层-->视-->次-->相机单元  *************************/
    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        struCnfg.struLayerInfo[i].nLayerIndex           = i;
        struCnfg.struLayerInfo[i].nViewChainTotal       = 4;

        struCnfg.struLayerInfo[i].nVavleChainTotal      = 1;    //喷阀吹链数


        struCnfg.struLayerInfo[i].nViewIsUsed[0]     = 1;
        struCnfg.struLayerInfo[i].nViewIsUsed[1]     = 1;
        struCnfg.struLayerInfo[i].nViewIsUsed[2]     = 1;
        struCnfg.struLayerInfo[i].nViewIsUsed[3]     = 1;

        struCnfg.struLayerInfo[i].nViewId[0]     = LCD_VERI_MAIN_VIEW_FRONT;
        struCnfg.struLayerInfo[i].nViewId[1]     = LCD_VERI_MAIN_VIEW_REAR;
        struCnfg.struLayerInfo[i].nViewId[2]     = LCD_VERI_VICE_VIEW_FRONT;
        struCnfg.struLayerInfo[i].nViewId[3]     = LCD_VERI_VICE_VIEW_REAR;

        struCnfg.struLayerInfo[i].nViewBoardType[0]     = BOARD_CAMERA_1_FRONT_MAIN;
        struCnfg.struLayerInfo[i].nViewBoardType[1]     = BOARD_CAMERA_1_REAR_MAIN;
        struCnfg.struLayerInfo[i].nViewBoardType[2]     = BOARD_CAMERA_1_FRONT_VICE;
        struCnfg.struLayerInfo[i].nViewBoardType[3]     = BOARD_CAMERA_1_REAR_VICE;

        struCnfp.nViewSensorType[0][0]     = SENSOR_T1;   //前主可见
        struCnfp.nViewSensorType[0][1]     = SENSOR_T1;   //后主可见
        struCnfp.nViewSensorType[0][2]     = SENSOR_C1;   //前辅850
        struCnfp.nViewSensorType[0][3]     = SENSOR_C1;   //后辅1450

        getCameraViewDefaultArithEnable(i);

        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            struCnfg.struLayerInfo[i].stuViewInfo[j].nLayerIndex = i;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nNameIndex = tmpViewNameIndex[j];
            struCnfg.struLayerInfo[i].stuViewInfo[j].nEjectorsPerCamera = struCnfg.struLayerInfo[i].nEjectorsPerChute;  //有问题
            struCnfg.struLayerInfo[i].nViewUnitTotal[j] = tmpLayViewUnitCount[j];

            for(k = 0; k < struCnfg.struLayerInfo[i].nViewUnitTotal[j]; k++)
            {
                struCnfg.struLayerInfo[i].stuViewInfo[j].nViewUnitId[k] = k;
            }

            /* 设置每视的色选次数 */
            struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal = tmpSortTimes;

            /* 根据视传感器的类型，设置相机相关参数 */
            switch(struCnfp.nViewSensorType[0][j])
            {
            case SENSOR_T1:
            case SENSOR_T1_850:
            case SENSOR_G2:
            case SENSOR_G3:
                //struCnfp.nSensorDim[i][j]     = 3;
                // struCnfp.nSensBitWidth[i][j]  = 8;
                //struCnfp.nSensorPixel[i][j]   = SENSOR_PIXELS_2048; //11代表2048分辨率

                //struCnfg.struLayerInfo[i].stuViewInfo[j].nSensorRowFre = 350;
                struCnfg.struLayerInfo[i].stuViewInfo[j].rowColQuo = 1;
                // struCnfp.nImageSplice[i][j] = 0;
                // struCnfp.nMirror[i][j] = 0;
                //struCnfg.struLayerInfo[i].stuViewInfo[j].nSenseBrightSelection = 0;
                //struCnfp.nSensDropLine[i][j] = 0;
                //struCnfp.nSensCmdSpeed[i][j] = 1;
                struCnfg.struLayerInfo[i].stuViewInfo[j].nSensResizeMode = 0;
                //struCnfp.nRealTimeCorrection[i][j] = 0;

                struCnfg.struLayerInfo[i].stuViewInfo[j].nBrightRefRed = 150;
                struCnfg.struLayerInfo[i].stuViewInfo[j].nBrightRefGreen = 150;
                struCnfg.struLayerInfo[i].stuViewInfo[j].nBrightRefBlue = 150;
                struCnfg.struLayerInfo[i].stuViewInfo[j].nBrightRefNir = 120;
                break;
            case SENSOR_C1:
                //struCnfp.nSensorDim[i][j]     = 2;
                //struCnfp.nSensBitWidth[i][j]  = 8;
                //struCnfp.nSensorPixel[i][j]   = SENSOR_PIXELS_512; //11代表2048分辨率

                //struCnfg.struLayerInfo[i].stuViewInfo[j].nSensorRowFre = 350;
                struCnfg.struLayerInfo[i].stuViewInfo[j].rowColQuo = 1;
                //struCnfp.nImageSplice[i][j] = 0;
                //struCnfp.nSensorDataType[i][j] = 0;
                //struCnfp.nMirror[i][j] = 0;
                //struCnfg.struLayerInfo[i].stuViewInfo[j].nSenseBrightSelection = 0;
                // struCnfp.nSensDropLine[i][j] = 0;
                //struCnfp.nSensCmdSpeed[i][j] = 0;
                struCnfg.struLayerInfo[i].stuViewInfo[j].nSensResizeMode = 0;
                //struCnfp.nRealTimeCorrection[i][j] = 0;

                struCnfg.struLayerInfo[i].stuViewInfo[j].nBrightRefRed = 100;
                struCnfg.struLayerInfo[i].stuViewInfo[j].nBrightRefGreen = 100;
                struCnfg.struLayerInfo[i].stuViewInfo[j].nBrightRefBlue = 100;
                struCnfg.struLayerInfo[i].stuViewInfo[j].nBrightRefNir = 100;
                break;
            case SENSOR_D2:
                //struCnfp.nSensorDim[i][j]     = 4;
                //struCnfp.nSensorDimD2[i][j]   = 4;
                struCnfg.struLayerInfo[i].stuViewInfo[j].nSensorD2Mode  = 0;
                // struCnfp.nSensBitWidth[i][j]  = 8;
                //struCnfp.nSensorPixel[i][j]  = SENSOR_PIXELS_2048; //11代表2048分辨率

                //struCnfg.struLayerInfo[i].stuViewInfo[j].nSensorRowFre = 70;
                struCnfg.struLayerInfo[i].stuViewInfo[j].rowColQuo = 1;
                //struCnfp.nImageSplice[i][j] = 0;
                //struCnfp.nSensorDataType[i][j] = 0;
                //struCnfp.nMirror[i][j] = 0;
                //struCnfg.struLayerInfo[i].stuViewInfo[j].nSenseBrightSelection = 0;
                //struCnfp.nSensDropLine[i][j] = 0;
                //struCnfp.nSensCmdSpeed[i][j] = 0;
                struCnfg.struLayerInfo[i].stuViewInfo[j].nSensResizeMode = 0;
                //struCnfp.nRealTimeCorrection[i][j] = 0;

                struCnfg.struLayerInfo[i].stuViewInfo[j].nBrightRefRed = 150;
                struCnfg.struLayerInfo[i].stuViewInfo[j].nBrightRefGreen = 150;
                struCnfg.struLayerInfo[i].stuViewInfo[j].nBrightRefBlue = 150;
                struCnfg.struLayerInfo[i].stuViewInfo[j].nBrightRefNir = 120;
                break;
            case SENSOR_H1:
                //struCnfp.nSensorDim[i][j]     = 1;
                //struCnfp.nSensBitWidth[i][j]  = 8;
                //struCnfp.nSensorPixel[i][j]   = SENSOR_PIXELS_512; //11代表2048分辨率

                //struCnfg.struLayerInfo[i].stuViewInfo[j].nSensorRowFre = 350;
                struCnfg.struLayerInfo[i].stuViewInfo[j].rowColQuo = 1;
                //struCnfp.nImageSplice[i][j] = 0;
                //struCnfp.nSensorDataType[i][j] = 0;
                //struCnfp.nMirror[i][j] = 0;
                //struCnfg.struLayerInfo[i].stuViewInfo[j].nSenseBrightSelection = 0;
                //struCnfp.nSensDropLine[i][j] = 0;
                //struCnfp.nSensCmdSpeed[i][j] = 0;
                struCnfg.struLayerInfo[i].stuViewInfo[j].nSensResizeMode = 0;
                // struCnfp.nRealTimeCorrection[i][j] = 0;

                struCnfg.struLayerInfo[i].stuViewInfo[j].nBrightRefRed = 100;
                struCnfg.struLayerInfo[i].stuViewInfo[j].nBrightRefGreen = 100;
                struCnfg.struLayerInfo[i].stuViewInfo[j].nBrightRefBlue = 100;
                struCnfg.struLayerInfo[i].stuViewInfo[j].nBrightRefNir = 100;
                break;
            default:
                break;
            }
        }

        /* 设置每视>>每次的界面显示显示名称， 一次前主,一次后主、一次前辅、一次后辅... */
        for(j = 0; j < struCnfg.struLayerInfo[i].stuViewInfo[0].nIdentifyGroupTotal; j++)
        {
            struCnfg.struLayerInfo[i].stuViewInfo[0].stuIdenGupInfo[j].nNameIndex = FIRST_MAIN_FRONT+j;
        }
        for(j = 0; j < struCnfg.struLayerInfo[i].stuViewInfo[1].nIdentifyGroupTotal; j++)
        {
            struCnfg.struLayerInfo[i].stuViewInfo[1].stuIdenGupInfo[j].nNameIndex = FIRST_MAIN_REAR+j;
        }
        for(j = 0; j < struCnfg.struLayerInfo[i].stuViewInfo[2].nIdentifyGroupTotal; j++)
        {
            struCnfg.struLayerInfo[i].stuViewInfo[2].stuIdenGupInfo[j].nNameIndex = FIRST_VICE_FRONT+j;
        }
        for(j = 0; j < struCnfg.struLayerInfo[i].stuViewInfo[3].nIdentifyGroupTotal; j++)
        {
            struCnfg.struLayerInfo[i].stuViewInfo[3].stuIdenGupInfo[j].nNameIndex = FIRST_VICE_REAR+j;
        }

        /* 设置每视的色选次数，以及每次的相机编号、是否分割等信息 */
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            for(m = 0; m < tmpSortTimes; m++)
            {
                struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[m].nGroupUnitTotal = tmpTimesUnitCount[m];
            }

            tmpTotal = 0;
            for(m = 0; m <struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal; m++ )
            {
                for(k = 0; k < struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[m].nGroupUnitTotal; k++)
                {
                    struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[m].nUnitGupId[k] = tmpTotal+k;
                    struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[m].nUnitGupSplice[k] = 0;
                }
                tmpTotal += k;
            }
        }

        /****************  层-->吹-->次-->喷阀板  *************************/
        for(j = 0; j < struCnfg.struLayerInfo[i].nVavleChainTotal; j++)
        {
            struCnfg.struLayerInfo[i].nVavleBoardType[j]     = BOARD_VAVLE_LAYER_1;
            struCnfg.struLayerInfo[i].nVavleType[j]     = VAVLE_TYPE_1;

            struCnfg.struLayerInfo[i].nVavleBoardTotal[j] = struCnfg.nChuteTotal; //喷阀板数量与滑槽数量相同

            struCnfg.struLayerInfo[i].stuVavleInfo[j].nLayerIndex = i;
            struCnfg.struLayerInfo[i].stuVavleInfo[j].nNameIndex = MAIN_LAYER_VAVLE;

            struCnfg.struLayerInfo[i].stuVavleInfo[j].nViewTotal = 4;

            struCnfg.struLayerInfo[i].stuVavleInfo[j].nVavleViewBoardType[0]     = BOARD_CAMERA_1_FRONT_MAIN;
            struCnfg.struLayerInfo[i].stuVavleInfo[j].nVavleViewBoardType[1]     = BOARD_CAMERA_1_REAR_MAIN;
            struCnfg.struLayerInfo[i].stuVavleInfo[j].nVavleViewBoardType[2]     = BOARD_CAMERA_1_FRONT_VICE;
            struCnfg.struLayerInfo[i].stuVavleInfo[j].nVavleViewBoardType[3]     = BOARD_CAMERA_1_REAR_VICE;

            for(k = 0; k < MAX_VAVLE_BOARD; k++)
            {
                struCnfg.struLayerInfo[i].stuVavleInfo[j].nVavleBoardId[k] = k;
            }
            struCnfg.struLayerInfo[i].nEjectorsPerChute = VAVLE_EJECTORS_64;
            struCnfg.struLayerInfo[i].nSplice = 0;
            struCnfg.struLayerInfo[i].nOnlyDispFirst = 0;
            struCnfg.struLayerInfo[i].nEjectorsPosition = 0;

            struCnfg.struLayerInfo[i].stuVavleInfo[j].nTickGroupTotal = tmpSortTimes;

        }

        /* 设置层内各喷阀板名称， 一次、二次...*/
        for(j = 0; j < struCnfg.struLayerInfo[i].stuVavleInfo[0].nTickGroupTotal; j++)
        {
            struCnfg.struLayerInfo[i].stuVavleInfo[0].stuTickGupInfo[j].nNameIndex = FIRST_SORT+j;
        }

        for(j = 0; j < struCnfg.struLayerInfo[i].nVavleChainTotal; j++)
        {
            //* 设置每层的色选次数，以及每次的喷阀板地址编号、是否分割等信息 */
            for(m = 0; m < tmpSortTimes; m++)
            {
                struCnfg.struLayerInfo[i].stuVavleInfo[j].stuTickGupInfo[m].nEjBoardGupTotal = tmpTimesUnitCount[m];
            }

            tmpTotal = 0;
            for(m = 0; m < tmpSortTimes; m++)
            {
                for(k = 0; k < struCnfg.struLayerInfo[i].stuVavleInfo[j].stuTickGupInfo[m].nEjBoardGupTotal; k++)
                {
                    struCnfg.struLayerInfo[i].stuVavleInfo[j].stuTickGupInfo[m].nEjBoardGupId[k] = tmpTotal+k;
                    struCnfg.struLayerInfo[i].stuVavleInfo[j].stuTickGupInfo[m].nEjBoardGupSplice[k] = 0;
                }
                tmpTotal += k;
            }
        }
    }
}

void GlobalFlow::getBeltLd2GeneralLayerInfo()
{
    int i, j, k, m, n, tmpTotal = 0;
    int tmpViewNameIndex[MAX_VIEW];
    int tmpLayViewUnitCount[MAX_VIEW];  //每层每视相机单元数量/喷阀板数量

    int tmpSortTimes = 1;   //1次选
    int tmpTimesUnitCount[MAX_GROUP_IDENTIFY];  //每次色选的滑槽数量

    struCnfg.nMachineAttr                   = MACHINE_TYPE_BELT_DOUBLE;
    struCnfg.nMachineSpliceType             = MACHINE_SPLICE_TYPE_NULL;
    struCnfg.nLayerTotal                    = 1;
    struCnfg.nChuteTotal                    = 6;
    struCnfg.nCtrlBoardTotal                = 1;
    struCnfg.nFeedMainTotal                 = 4;   // 4通道,12个振动器
    struCnfg.nFeedViceTotal                 = 0;
    struCnfg.nVavleTimeSetMode              = 0; //22/6/27 lpgs默认不按次设置延时
    struCnfg.nValveRatio                    = 0;
    struCnfg.nCorssEjector                  = 0;

    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(j = 0; j < MAX_VAVLE; j++)
        {
            struCnfg.nVavleBoardMode[i][j] = 1;    // 每吹的模式默认都是1
        }
    }
    /* 设置每视的名称索引 */
    tmpViewNameIndex[0] = MAIN_FRONT;
    tmpViewNameIndex[1] = MAIN_REAR;
    tmpViewNameIndex[2] = VICE_FRONT;
    tmpViewNameIndex[3] = VICE_REAR;
    tmpViewNameIndex[4] = MAIN_FRONT_2;
    tmpViewNameIndex[5] = MAIN_REAR_2;
    tmpViewNameIndex[6] = VICE_FRONT_2;
    tmpViewNameIndex[7] = VICE_REAR_2;

    //每层每视的相机板数量&喷阀板数量 和滑槽数相等
    for(i = 0; i < MAX_VIEW; i++)
    {
        tmpLayViewUnitCount[i] = struCnfg.nChuteTotal;
    }

    //每次色选的相机板数量&喷阀板数量设置
    tmpTimesUnitCount[0] = 6;
    tmpTimesUnitCount[1] = 0;
    tmpTimesUnitCount[2] = 0;
    tmpTimesUnitCount[3] = 0;
    tmpTimesUnitCount[4] = 0;

    /****************  层-->视-->次-->相机单元  *************************/
    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        struCnfg.struLayerInfo[i].nLayerIndex           = i;
        struCnfg.struLayerInfo[i].nViewChainTotal       = 8;
        struCnfg.struLayerInfo[i].nVavleChainTotal      = 2;    //喷阀吹链数

        struCnfg.struLayerInfo[i].nViewId[0]     = LCD_VERI_MAIN_VIEW_FRONT;
        struCnfg.struLayerInfo[i].nViewId[1]     = LCD_VERI_MAIN_VIEW_REAR;
        struCnfg.struLayerInfo[i].nViewId[2]     = LCD_VERI_VICE_VIEW_FRONT;
        struCnfg.struLayerInfo[i].nViewId[3]     = LCD_VERI_VICE_VIEW_REAR;
        struCnfg.struLayerInfo[i].nViewId[4]     = LCD_VERI_MAIN_VIEW_FRONT_2;
        struCnfg.struLayerInfo[i].nViewId[5]     = LCD_VERI_MAIN_VIEW_REAR_2;
        struCnfg.struLayerInfo[i].nViewId[6]     = LCD_VERI_VICE_VIEW_FRONT_2;
        struCnfg.struLayerInfo[i].nViewId[7]     = LCD_VERI_VICE_VIEW_REAR_2;


        struCnfg.struLayerInfo[i].nViewBoardType[0]     = BOARD_CAMERA_1_FRONT_MAIN;
        struCnfg.struLayerInfo[i].nViewBoardType[1]     = BOARD_CAMERA_1_REAR_MAIN;
        struCnfg.struLayerInfo[i].nViewBoardType[2]     = BOARD_CAMERA_1_FRONT_VICE;
        struCnfg.struLayerInfo[i].nViewBoardType[3]     = BOARD_CAMERA_1_REAR_VICE;
        struCnfg.struLayerInfo[i].nViewBoardType[4]     = BOARD_CAMERA_2_FRONT_MAIN;
        struCnfg.struLayerInfo[i].nViewBoardType[5]     = BOARD_CAMERA_2_REAR_MAIN;
        struCnfg.struLayerInfo[i].nViewBoardType[6]     = BOARD_CAMERA_2_FRONT_VICE;
        struCnfg.struLayerInfo[i].nViewBoardType[7]     = BOARD_CAMERA_2_REAR_VICE;

        struCnfg.struLayerInfo[i].nViewIsUsed[0] = 1;
        struCnfg.struLayerInfo[i].nViewIsUsed[1] = 1;
        struCnfg.struLayerInfo[i].nViewIsUsed[2] = 0;
        struCnfg.struLayerInfo[i].nViewIsUsed[3] = 0;
        struCnfg.struLayerInfo[i].nViewIsUsed[4] = 1;
        struCnfg.struLayerInfo[i].nViewIsUsed[5] = 1;
        struCnfg.struLayerInfo[i].nViewIsUsed[6] = 0;
        struCnfg.struLayerInfo[i].nViewIsUsed[7] = 0;

        getCameraViewDefaultArithEnable(i);

        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            //            struCnfg.struLayerInfo[i].nViewIsUsed[0] = 1;
            struCnfp.nViewSensorType[0][j]     = SENSOR_T1;   //前主可见
            struCnfg.struLayerInfo[i].stuViewInfo[j].nLayerIndex = i;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nNameIndex = tmpViewNameIndex[j];
            struCnfg.struLayerInfo[i].stuViewInfo[j].nEjectorsPerCamera = struCnfg.struLayerInfo[i].nEjectorsPerChute;  //有问题
            struCnfg.struLayerInfo[i].nViewUnitTotal[j] = tmpLayViewUnitCount[j];

            for(k = 0; k < struCnfg.struLayerInfo[i].nViewUnitTotal[j]; k++)
            {
                struCnfg.struLayerInfo[i].stuViewInfo[j].nViewUnitId[k] = k;
            }

            /* 设置每视的色选次数 */
            struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal = tmpSortTimes;

            /* 根据视传感器的类型，设置相机相关参数 */
            switch(struCnfp.nViewSensorType[0][j])
            {
            case SENSOR_T1:
            case SENSOR_T1_850:
            case SENSOR_G2:
            case SENSOR_G3:
                // struCnfp.nSensorDim[i][j]     = 3;
                // struCnfp.nSensBitWidth[i][j]  = 8;
                // struCnfp.nSensorPixel[i][j]   = SENSOR_PIXELS_2048; //11代表2048分辨率

                //struCnfg.struLayerInfo[i].stuViewInfo[j].nSensorRowFre = 350;
                struCnfg.struLayerInfo[i].stuViewInfo[j].rowColQuo = 1;
                //struCnfp.nImageSplice[i][j] = 0;
                // struCnfp.nSensorDataType[i][j] = 0;
                // struCnfp.nMirror[i][j] = 0;
                //struCnfg.struLayerInfo[i].stuViewInfo[j].nSenseBrightSelection = 0;
                //struCnfp.nSensDropLine[i][j] = 0;
                //struCnfp.nSensCmdSpeed[i][j] = 1;
                struCnfg.struLayerInfo[i].stuViewInfo[j].nSensResizeMode = 0;
                //struCnfp.nRealTimeCorrection[i][j] = 0;

                struCnfg.struLayerInfo[i].stuViewInfo[j].nBrightRefRed = 150;
                struCnfg.struLayerInfo[i].stuViewInfo[j].nBrightRefGreen = 150;
                struCnfg.struLayerInfo[i].stuViewInfo[j].nBrightRefBlue = 150;
                struCnfg.struLayerInfo[i].stuViewInfo[j].nBrightRefNir = 120;
                struCnfg.struLayerInfo[i].stuViewInfo[j].nEjectorsPerCamera  = 64;
                break;
            case SENSOR_C1:
                // struCnfp.nSensorDim[i][j]     = 2;
                // struCnfp.nSensBitWidth[i][j]  = 8;
                // struCnfp.nSensorPixel[i][j]   = SENSOR_PIXELS_512; //11代表2048分辨率

                //struCnfg.struLayerInfo[i].stuViewInfo[j].nSensorRowFre = 350;
                struCnfg.struLayerInfo[i].stuViewInfo[j].rowColQuo = 1;
                // struCnfp.nImageSplice[i][j] = 0;
                //struCnfp.nSensorDataType[i][j] = 0;
                //struCnfp.nMirror[i][j] = 0;
                //struCnfg.struLayerInfo[i].stuViewInfo[j].nSenseBrightSelection = 0;
                // struCnfp.nSensDropLine[i][j] = 0;
                // struCnfp.nSensCmdSpeed[i][j] = 0;
                struCnfg.struLayerInfo[i].stuViewInfo[j].nSensResizeMode = 0;
                // struCnfp.nRealTimeCorrection[i][j] = 0;

                struCnfg.struLayerInfo[i].stuViewInfo[j].nBrightRefRed = 100;
                struCnfg.struLayerInfo[i].stuViewInfo[j].nBrightRefGreen = 100;
                struCnfg.struLayerInfo[i].stuViewInfo[j].nBrightRefBlue = 100;
                struCnfg.struLayerInfo[i].stuViewInfo[j].nBrightRefNir = 100;
                break;
            case SENSOR_D2:
                // struCnfp.nSensorDim[i][j]     = 4;
                // struCnfp.nSensorDimD2[i][j]  = 4;
                struCnfg.struLayerInfo[i].stuViewInfo[j].nSensorD2Mode  = 4;
                // struCnfp.nSensBitWidth[i][j]  = 8;
                // struCnfp.nSensorPixel[i][j]   = SENSOR_PIXELS_2048; //11代表2048分辨率

                //struCnfg.struLayerInfo[i].stuViewInfo[j].nSensorRowFre = 70;
                struCnfg.struLayerInfo[i].stuViewInfo[j].rowColQuo = 1;
                // struCnfp.nImageSplice[i][j] = 0;
                // struCnfp.nSensorDataType[i][j] = 0;
                // struCnfp.nMirror[i][j] = 0;
                ////struCnfg.struLayerInfo[i].stuViewInfo[j].nSenseBrightSelection = 0;
                // struCnfp.nSensDropLine[i][j] = 0;
                //  struCnfp.nSensCmdSpeed[i][j] = 0;
                struCnfg.struLayerInfo[i].stuViewInfo[j].nSensResizeMode = 0;
                //  struCnfp.nRealTimeCorrection[i][j] = 0;

                struCnfg.struLayerInfo[i].stuViewInfo[j].nBrightRefRed = 150;
                struCnfg.struLayerInfo[i].stuViewInfo[j].nBrightRefGreen = 150;
                struCnfg.struLayerInfo[i].stuViewInfo[j].nBrightRefBlue = 150;
                struCnfg.struLayerInfo[i].stuViewInfo[j].nBrightRefNir = 120;
                struCnfg.struLayerInfo[i].stuViewInfo[j].nEjectorsPerCamera  = 64;
                break;
            case SENSOR_H1:
                //  struCnfp.nSensorDim[i][j]     = 1;
                //  struCnfp.nSensBitWidth[i][j]  = 8;
                // struCnfp.nSensorPixel[i][j]   = SENSOR_PIXELS_512; //11代表2048分辨率

                //struCnfg.struLayerInfo[i].stuViewInfo[j].nSensorRowFre = 350;
                struCnfg.struLayerInfo[i].stuViewInfo[j].rowColQuo = 1;
                // struCnfp.nImageSplice[i][j] = 0;
                // struCnfp.nSensorDataType[i][j] = 0;
                //  struCnfp.nMirror[i][j] = 0;
                //struCnfg.struLayerInfo[i].stuViewInfo[j].nSenseBrightSelection = 0;
                // struCnfp.nSensDropLine[i][j] = 0;
                //  struCnfp.nSensCmdSpeed[i][j] = 0;
                struCnfg.struLayerInfo[i].stuViewInfo[j].nSensResizeMode = 0;
                // struCnfp.nRealTimeCorrection[i][j] = 0;

                struCnfg.struLayerInfo[i].stuViewInfo[j].nBrightRefRed = 100;
                struCnfg.struLayerInfo[i].stuViewInfo[j].nBrightRefGreen = 100;
                struCnfg.struLayerInfo[i].stuViewInfo[j].nBrightRefBlue = 100;
                struCnfg.struLayerInfo[i].stuViewInfo[j].nBrightRefNir = 100;
                break;
            default:
                break;
            }
        }

        /* 设置每视>>每次的界面显示显示名称， 一次前主,一次后主、一次前辅、一次后辅... */
        for(j = 0; j < struCnfg.struLayerInfo[i].stuViewInfo[0].nIdentifyGroupTotal; j++)
        {
            struCnfg.struLayerInfo[i].stuViewInfo[0].stuIdenGupInfo[j].nNameIndex = FIRST_MAIN_FRONT+j;
        }
        for(j = 0; j < struCnfg.struLayerInfo[i].stuViewInfo[1].nIdentifyGroupTotal; j++)
        {
            struCnfg.struLayerInfo[i].stuViewInfo[1].stuIdenGupInfo[j].nNameIndex = FIRST_MAIN_REAR+j;
        }
        for(j = 0; j < struCnfg.struLayerInfo[i].stuViewInfo[2].nIdentifyGroupTotal; j++)
        {
            struCnfg.struLayerInfo[i].stuViewInfo[2].stuIdenGupInfo[j].nNameIndex = FIRST_VICE_FRONT+j;
        }
        for(j = 0; j < struCnfg.struLayerInfo[i].stuViewInfo[3].nIdentifyGroupTotal; j++)
        {
            struCnfg.struLayerInfo[i].stuViewInfo[3].stuIdenGupInfo[j].nNameIndex = FIRST_VICE_REAR+j;
        }
        for(j = 0; j < struCnfg.struLayerInfo[i].stuViewInfo[4].nIdentifyGroupTotal; j++)
        {
            struCnfg.struLayerInfo[i].stuViewInfo[4].stuIdenGupInfo[j].nNameIndex = FIRST_VICE_REAR+j;
        }
        for(j = 0; j < struCnfg.struLayerInfo[i].stuViewInfo[5].nIdentifyGroupTotal; j++)
        {
            struCnfg.struLayerInfo[i].stuViewInfo[5].stuIdenGupInfo[j].nNameIndex = FIRST_VICE_REAR+j;
        }
        for(j = 0; j < struCnfg.struLayerInfo[i].stuViewInfo[6].nIdentifyGroupTotal; j++)
        {
            struCnfg.struLayerInfo[i].stuViewInfo[6].stuIdenGupInfo[j].nNameIndex = FIRST_VICE_REAR+j;
        }
        for(j = 0; j < struCnfg.struLayerInfo[i].stuViewInfo[7].nIdentifyGroupTotal; j++)
        {
            struCnfg.struLayerInfo[i].stuViewInfo[7].stuIdenGupInfo[j].nNameIndex = FIRST_VICE_REAR+j;
        }

        /* 设置每视的色选次数，以及每次的相机编号、是否分割等信息 */
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            for(m = 0; m < tmpSortTimes; m++)
            {
                struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[m].nGroupUnitTotal = tmpTimesUnitCount[m];
            }

            tmpTotal = 0;
            for(m = 0; m <struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal; m++ )
            {
                for(k = 0; k < struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[m].nGroupUnitTotal; k++)
                {
                    struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[m].nUnitGupId[k] = tmpTotal+k;
                    struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[m].nUnitGupSplice[k] = 0;
                }
                tmpTotal += k;
            }
        }

        /****************  层-->吹-->次-->喷阀板  *************************/
        struCnfg.struLayerInfo[i].nVavleBoardType[0]     = BOARD_VAVLE_LAYER_1;
        struCnfg.struLayerInfo[i].nVavleBoardType[1]     = BOARD_VAVLE_LAYER_2;

        struCnfg.struLayerInfo[i].nVavleType[0]     = VAVLE_TYPE_1;
        struCnfg.struLayerInfo[i].nVavleType[1]     = VAVLE_TYPE_1;

        struCnfg.struLayerInfo[i].stuVavleInfo[0].nNameIndex = SECOND_LAYER_VAVLE;
        struCnfg.struLayerInfo[i].stuVavleInfo[1].nNameIndex = FIRST_LAYER_VAVLE;

        for(j = 0; j < struCnfg.struLayerInfo[i].nVavleChainTotal; j++)
        {
            struCnfg.struLayerInfo[i].nVavleBoardTotal[j] = struCnfg.nChuteTotal; //喷阀板数量与滑槽数量相同

            struCnfg.struLayerInfo[i].stuVavleInfo[j].nLayerIndex = i;
            //            struCnfg.struLayerInfo[i].stuVavleInfo[j].nNameIndex = MAIN_LAYER_VAVLE;

            struCnfg.struLayerInfo[i].stuVavleInfo[j].nViewTotal = 4;

            struCnfg.struLayerInfo[i].stuVavleInfo[j].nVavleViewBoardType[0]     = BOARD_CAMERA_1_FRONT_MAIN;
            struCnfg.struLayerInfo[i].stuVavleInfo[j].nVavleViewBoardType[1]     = BOARD_CAMERA_1_REAR_MAIN;
            struCnfg.struLayerInfo[i].stuVavleInfo[j].nVavleViewBoardType[2]     = BOARD_CAMERA_1_FRONT_VICE;
            struCnfg.struLayerInfo[i].stuVavleInfo[j].nVavleViewBoardType[3]     = BOARD_CAMERA_1_REAR_VICE;
            struCnfg.struLayerInfo[i].stuVavleInfo[j].nVavleViewBoardType[4]     = BOARD_CAMERA_2_FRONT_MAIN;
            struCnfg.struLayerInfo[i].stuVavleInfo[j].nVavleViewBoardType[5]     = BOARD_CAMERA_2_REAR_MAIN;
            struCnfg.struLayerInfo[i].stuVavleInfo[j].nVavleViewBoardType[6]     = BOARD_CAMERA_2_FRONT_VICE;
            struCnfg.struLayerInfo[i].stuVavleInfo[j].nVavleViewBoardType[7]     = BOARD_CAMERA_2_REAR_VICE;

            for(k = 0; k < MAX_VAVLE_BOARD; k++)
            {
                struCnfg.struLayerInfo[i].stuVavleInfo[j].nVavleBoardId[k] = k;
            }
            struCnfg.struLayerInfo[i].nEjectorsPerChute = VAVLE_EJECTORS_64;
            struCnfg.struLayerInfo[i].nSplice = 0;
            struCnfg.struLayerInfo[i].nOnlyDispFirst = 0;
            struCnfg.struLayerInfo[i].nEjectorsPosition = 0;

            struCnfg.struLayerInfo[i].stuVavleInfo[j].nTickGroupTotal = tmpSortTimes;

        }

        /* 设置层内各喷阀板名称， 一次、二次...*/
        for(j = 0; j < struCnfg.struLayerInfo[i].stuVavleInfo[0].nTickGroupTotal; j++)
        {
            struCnfg.struLayerInfo[i].stuVavleInfo[0].stuTickGupInfo[j].nNameIndex = FIRST_SORT+j;
        }

        for(j = 0; j < struCnfg.struLayerInfo[i].nVavleChainTotal; j++)
        {
            //* 设置每层的色选次数，以及每次的喷阀板地址编号、是否分割等信息 */
            for(m = 0; m < tmpSortTimes; m++)
            {
                struCnfg.struLayerInfo[i].stuVavleInfo[j].stuTickGupInfo[m].nEjBoardGupTotal = tmpTimesUnitCount[m];
            }

            tmpTotal = 0;
            for(m = 0; m < tmpSortTimes; m++)
            {
                for(k = 0; k < struCnfg.struLayerInfo[i].stuVavleInfo[j].stuTickGupInfo[m].nEjBoardGupTotal; k++)
                {
                    struCnfg.struLayerInfo[i].stuVavleInfo[j].stuTickGupInfo[m].nEjBoardGupId[k] = tmpTotal+k;
                    struCnfg.struLayerInfo[i].stuVavleInfo[j].stuTickGupInfo[m].nEjBoardGupSplice[k] = 0;
                }
                tmpTotal += k;
            }
        }
    }
}

void GlobalFlow::getCameraViewDefaultArithEnable(int nLayer)
{
    int i = nLayer;

    for(int j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
    {
        switch(struCnfp.nViewSensorType[0][j])
        {
        case SENSOR_T1:
        case SENSOR_D2:
        case SENSOR_G2:
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithTotal = 13;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_GREY_A] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_GREY_B] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_GREY_C] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_GREY_D] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_DISCOLOR_A] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_DISCOLOR_B] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_CROSS] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_SVM_A] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_SVM_B] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_SVM_C] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_SVM_D] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_SHAPE_SVM_A] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_SHAPE_SVM_B] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_SHAPE_SVM_C] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_BIG_SMALL] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_LONG_SHORT] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_LONG_SHORT_B] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_CIRCLE_LONG] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_HSV] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_HSV_B] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_HSV_C] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_SAT] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_HUE] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_SCALE] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_MILDEW] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_OUTLINE] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_LINE] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_BUD] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_BUD_B] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_ROUND] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_VALUE] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_SPL_MILDEW] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_W_PEANUT] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_BIG_SMALL_B] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_CIRCLE_LONG_B] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_ROUND_B] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_PEPPER_HANDLE] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_WHEAT_SPROUT] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[TEA_ARITH_COLOR] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[TEA_ARITH_COLOR_2] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[TEA_ARITH_COLOR_3] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[TEA_ARITH_COLOR_4] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[TEA_ARITH_COLOR_5] = 0;

            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_GREY_A] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_GREY_B] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_GREY_C] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_GREY_D] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_DISCOLOR_A] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_DISCOLOR_B] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_CROSS] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_SVM_A] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_SVM_B] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_SVM_C] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_SVM_D] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_SHAPE_SVM_A] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_SHAPE_SVM_B] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_SHAPE_SVM_C] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_BIG_SMALL] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_LONG_SHORT] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_LONG_SHORT_B] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_CIRCLE_LONG] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_HSV] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_HSV_B] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_HSV_C] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_SAT] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_HUE] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_SCALE] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_MILDEW] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_OUTLINE] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_LINE] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_BUD] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_BUD_B] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_ROUND] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_VALUE] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_SPL_MILDEW] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_W_PEANUT] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_BIG_SMALL_B] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_CIRCLE_LONG_B] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_ROUND_B] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_PEPPER_HANDLE] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_WHEAT_SPROUT] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[TEA_ARITH_COLOR] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[TEA_ARITH_COLOR_2] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[TEA_ARITH_COLOR_3] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[TEA_ARITH_COLOR_4] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[TEA_ARITH_COLOR_5] = 0;
            break;
        case SENSOR_T1_850:
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithTotal = 13;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_GREY_A] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_GREY_B] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_GREY_C] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_GREY_D] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_DISCOLOR_A] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_DISCOLOR_B] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_CROSS] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_SVM_A] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_SVM_B] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_SVM_C] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_SVM_D] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_SHAPE_SVM_A] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_SHAPE_SVM_B] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_SHAPE_SVM_C] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_BIG_SMALL] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_LONG_SHORT] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_CIRCLE_LONG] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_HSV] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_HSV_B] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_HSV_C] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_PEPPER_HANDLE] = 0;

            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_GREY_A] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_GREY_B] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_GREY_C] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_GREY_D] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_DISCOLOR_A] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_DISCOLOR_B] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_CROSS] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_SVM_A] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_SVM_B] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_SVM_C] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_SVM_D] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_SHAPE_SVM_A] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_SHAPE_SVM_B] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_SHAPE_SVM_C] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_BIG_SMALL] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_LONG_SHORT] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_CIRCLE_LONG] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_HSV] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_HSV_B] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_HSV_C] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_PEPPER_HANDLE] = 0;
            break;
        case SENSOR_C1:
        case SENSOR_H1:
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithTotal = 13;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_GREY_A] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_GREY_B] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_GREY_C] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_GREY_D] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_DISCOLOR_A] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_DISCOLOR_B] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_CROSS] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_SVM_A] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_SVM_B] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_SVM_C] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_SVM_D] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_SHAPE_SVM_A] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_SHAPE_SVM_B] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_SHAPE_SVM_C] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_BIG_SMALL] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_LONG_SHORT] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_CIRCLE_LONG] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_HSV] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_HSV_B] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_HSV_C] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_PEPPER_HANDLE] = 0;

            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_GREY_A] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_GREY_B] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_GREY_C] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_GREY_D] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_DISCOLOR_A] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_DISCOLOR_B] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_CROSS] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_SVM_A] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_SVM_B] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_SVM_C] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_SVM_D] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_SHAPE_SVM_A] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_SHAPE_SVM_B] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_SHAPE_SVM_C] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_BIG_SMALL] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_LONG_SHORT] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_CIRCLE_LONG] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_HSV] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_HSV_B] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_HSV_C] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_PEPPER_HANDLE] = 0;
            break;
        default:
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithTotal = 13;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_GREY_A] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_GREY_B] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_GREY_C] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_GREY_D] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_DISCOLOR_A] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_DISCOLOR_B] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_CROSS] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_SVM_A] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_SVM_B] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_SVM_C] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_SVM_D] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_SHAPE_SVM_A] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_SHAPE_SVM_B] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_SHAPE_SVM_C] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_BIG_SMALL] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_LONG_SHORT] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_CIRCLE_LONG] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_HSV] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_HSV_B] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_HSV_C] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithEnable[ARITH_PEPPER_HANDLE] = 0;

            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_GREY_A] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_GREY_B] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_GREY_C] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_GREY_D] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_DISCOLOR_A] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_DISCOLOR_B] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_CROSS] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_SVM_A] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_SVM_B] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_SVM_C] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_SVM_D] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_SHAPE_SVM_A] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_SHAPE_SVM_B] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_SHAPE_SVM_C] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_BIG_SMALL] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_LONG_SHORT] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_CIRCLE_LONG] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_HSV] = 1;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_HSV_B] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_HSV_C] = 0;
            struCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_PEPPER_HANDLE] = 0;
            break;
        }
    }
}

void GlobalFlow::getGlobalLampConfig()
{
    int i, j, k;
    int hasInfra1450 = 0;

    switch (struCnfg.nMachineType) {
    case MACHINE_VERTICAL_DM_RG:
    case MACHINE_VERTICAL_DM_ZK:
    case MACHINE_VERTICAL_ZL_CS:
    case MACHINE_VERTICAL_ZL_CV:
    case MACHINE_LD_ZL_GP:
        if((struCnfg.struLayerInfo[struGsh.nLayer].nViewChainTotal == 3)&&(struCnfg.struLayerInfo[struGsh.nLayer].nViewBoardType[2]) == SENSOR_C1)
        {
            hasInfra1450 = 1;
        }
        /* 区分12通道，1通道、其他通道 */
        if(struCnfg.nChuteTotal == 12)
        {
            struCnfg.stuLayerLampCfg.nLampBoardTotal = 4;
            struCnfg.stuLayerLampCfg.nLampValueQuo = 0;
            for(i = 0; i < struCnfg.stuLayerLampCfg.nLampBoardTotal; i++)
            {
                for(j = 0; j < MAX_LAMP_PER_BOARD; j++)
                {
                    struCnfg.stuLayerLampCfg.nLampLayerAddr[i][j]  = 0;

                    struCnfg.stuLayerLampCfg.fLampBoardQuo[i][j] = (struCnfg.nChuteTotal+1)/11.0;
                    for(int k = 0; k < MAX_CALIBRATE_SETS; k++)
                    {
                        struCnfg.stuLayerLampCfg.nLampValue[i][j][k]  = 0;
                        struCnfg.stuLayerLampCfg.nLampEnable[i][j][k]  = 0;
                        struCnfg.stuLayerLampCfg.nLampSeq[i][j][k]  = 0;
                    }
                }
            }

            for(j = 0; j < MAX_LAMP_PER_BOARD; j++)
            {
                struCnfg.stuLayerLampCfg.nLampViewAddr[0][j]  = 0; //前分选室
                struCnfg.stuLayerLampCfg.nLampViewAddr[1][j]  = 0; //前分选室
                struCnfg.stuLayerLampCfg.nLampViewAddr[2][j]  = 1; //后分选室
                struCnfg.stuLayerLampCfg.nLampViewAddr[3][j]  = 1; //前分选室
            }

            struCnfg.stuLayerLampCfg.nLampPortEnable[0][0]  = 1;    //前物料灯1， 白灯
            struCnfg.stuLayerLampCfg.nLampPortEnable[0][1]  = 1;    //前物料灯2,  白灯或850
            struCnfg.stuLayerLampCfg.nLampPortEnable[0][2]  = 1;    //背景
            struCnfg.stuLayerLampCfg.nLampPortEnable[0][3]  = 0;    //
            struCnfg.stuLayerLampCfg.nLampPortEnable[0][4]  = 0;    //
            struCnfg.stuLayerLampCfg.nLampPortEnable[0][5]  = 0;    //

            struCnfg.stuLayerLampCfg.nLampPortEnable[1][0]  = 0;    //
            struCnfg.stuLayerLampCfg.nLampPortEnable[1][1]  = 0;    //
            struCnfg.stuLayerLampCfg.nLampPortEnable[1][2]  = 0;    //
            struCnfg.stuLayerLampCfg.nLampPortEnable[1][3]  = 1;    //前物料灯3,  白灯或850
            struCnfg.stuLayerLampCfg.nLampPortEnable[1][4]  = 1;    //前物料灯4， 白灯
            struCnfg.stuLayerLampCfg.nLampPortEnable[1][5]  = (hasInfra1450 == 0) ? 0 : 1;    //1450红外

            struCnfg.stuLayerLampCfg.nLampPortEnable[2][0]  = 1;    //后物料灯1， 白灯
            struCnfg.stuLayerLampCfg.nLampPortEnable[2][1]  = 1;    //后物料灯2,  蓝灯
            struCnfg.stuLayerLampCfg.nLampPortEnable[2][2]  = 1;    //背景
            struCnfg.stuLayerLampCfg.nLampPortEnable[2][3]  = 0;    //
            struCnfg.stuLayerLampCfg.nLampPortEnable[2][4]  = 0;    //
            struCnfg.stuLayerLampCfg.nLampPortEnable[2][5]  = 0;    //

            struCnfg.stuLayerLampCfg.nLampPortEnable[3][0]  = 0;    //
            struCnfg.stuLayerLampCfg.nLampPortEnable[3][1]  = 0;    //
            struCnfg.stuLayerLampCfg.nLampPortEnable[3][2]  = 0;    //
            struCnfg.stuLayerLampCfg.nLampPortEnable[3][3]  = 1;    //后物料灯3,  白灯
            struCnfg.stuLayerLampCfg.nLampPortEnable[3][4]  = 1;    //后物料灯4， 蓝灯
            struCnfg.stuLayerLampCfg.nLampPortEnable[3][5]  = (hasInfra1450 == 0) ? 0 : 1;    //1450红外


            for(j = 0; j < 5; j++)
            {
                if(j == 2){ //背景灯
                    struCnfg.stuLayerLampCfg.nLampPortType[0][j] = 1;
                    struCnfg.stuLayerLampCfg.nLampPortType[1][j] = 1;
                    struCnfg.stuLayerLampCfg.nLampPortType[2][j] = 1;
                    struCnfg.stuLayerLampCfg.nLampPortType[3][j] = 1;
                }else{      //非1450物料灯
                    struCnfg.stuLayerLampCfg.nLampPortType[0][j] = 0;
                    struCnfg.stuLayerLampCfg.nLampPortType[1][j] = 0;
                    struCnfg.stuLayerLampCfg.nLampPortType[2][j] = 0;
                    struCnfg.stuLayerLampCfg.nLampPortType[3][j] = 0;
                }
            }

            for(j = 0; j < 5; j++)
            {
                struCnfg.stuLayerLampCfg.nLampBoardLimit[0][j]  = 100;    //非1450灯上限值
                struCnfg.stuLayerLampCfg.nLampBoardLimit[1][j]  = 100;    //非1450灯上限值
                struCnfg.stuLayerLampCfg.nLampBoardLimit[2][j]  = 100;    //非1450灯上限值
                struCnfg.stuLayerLampCfg.nLampBoardLimit[3][j]  = 100;    //非1450灯上限值
            }

            struCnfg.stuLayerLampCfg.nLampBoardLimit[0][5]  = 20;    //1450红外灯上限值
            struCnfg.stuLayerLampCfg.nLampBoardLimit[1][5]  = 20;    //1450红外灯上限值
            struCnfg.stuLayerLampCfg.nLampBoardLimit[2][5]  = 20;    //1450红外灯上限值
            struCnfg.stuLayerLampCfg.nLampBoardLimit[3][5]  = 20;    //1450红外灯上限值
        }
        else if(struCnfg.nChuteTotal == 1 ||struCnfg.nChuteTotal == 2 )
        {
            struCnfg.stuLayerLampCfg.nLampBoardTotal = 1;
            struCnfg.stuLayerLampCfg.nLampValueQuo = 0;
            struCnfg.stuLayerLampCfg.fLampBoardQuo[0][0] = 2.000;   //前面4个白物料灯
            struCnfg.stuLayerLampCfg.fLampBoardQuo[0][1] = 0.800;   //后面2个白物料灯
            struCnfg.stuLayerLampCfg.fLampBoardQuo[0][2] = 0.800;   //后面2个蓝物料灯
            struCnfg.stuLayerLampCfg.fLampBoardQuo[0][3] = 0.800;   //2个背景灯
            struCnfg.stuLayerLampCfg.fLampBoardQuo[0][4] = 0.800;   //不用
            struCnfg.stuLayerLampCfg.fLampBoardQuo[0][5] = 0.800;   //不用


            for(j = 0; j < 5; j++)
            {
                struCnfg.stuLayerLampCfg.nLampViewAddr[0][j]  = 0; //前分选室
            }

            struCnfg.stuLayerLampCfg.nLampPortEnable[0][0]  = 1;    //前物料灯1，2, 3, 4 白灯
            struCnfg.stuLayerLampCfg.nLampPortEnable[0][1]  = 1;    //后物料白灯1,2,  白灯
            struCnfg.stuLayerLampCfg.nLampPortEnable[0][2]  = 1;    //后物料蓝灯3,4
            struCnfg.stuLayerLampCfg.nLampPortEnable[0][3]  = 1;    //前后背景灯，蓝灯
            struCnfg.stuLayerLampCfg.nLampPortEnable[0][4]  = 0;    //物料灯4， 白灯
            struCnfg.stuLayerLampCfg.nLampPortEnable[0][5]  = 0;    //1450红外

            for(j = 0; j < 5; j++)
            {
                if(j == 3){ //背景灯,第3个是背景灯
                    struCnfg.stuLayerLampCfg.nLampPortType[0][j] = 1;
                    struCnfg.stuLayerLampCfg.nLampPortType[1][j] = 1;
                }else{      //非1450物料灯
                    struCnfg.stuLayerLampCfg.nLampPortType[0][j] = 0;
                    struCnfg.stuLayerLampCfg.nLampPortType[1][j] = 0;
                }
            }

            for(j = 0; j < 5; j++)
            {
                struCnfg.stuLayerLampCfg.nLampBoardLimit[0][j]  = 100;    //非1450灯上限值
                struCnfg.stuLayerLampCfg.nLampBoardLimit[1][j]  = 100;    //非1450灯上限值
            }

            struCnfg.stuLayerLampCfg.nLampBoardLimit[0][5]  = 20;    //1450红外灯上限值
            struCnfg.stuLayerLampCfg.nLampBoardLimit[1][5]  = 20;    //1450红外灯上限值
        }
        else
        {

            for(i = 0; i < struCnfg.stuLayerLampCfg.nLampBoardTotal; i++)
            {
                for(j = 0; j < MAX_LAMP_PER_BOARD; j++)
                {
                    struCnfg.stuLayerLampCfg.nLampLayerAddr[i][j]  = 0;
                    struCnfg.stuLayerLampCfg.fLampBoardQuo[i][j] = (struCnfg.nChuteTotal+1)/11.0;
                }
            }

            for(j = 0; j < MAX_LAMP_PER_BOARD; j++)
            {
                struCnfg.stuLayerLampCfg.nLampViewAddr[0][j]  = 0; //前分选室
                struCnfg.stuLayerLampCfg.nLampViewAddr[1][j]  = 1; //后分选室
            }
            struCnfg.stuLayerLampCfg.nLampBoardTotal = 2;
            struCnfg.stuLayerLampCfg.nLampValueQuo = 0;
            struCnfg.stuLayerLampCfg.nLampPortEnable[0][0]  = 1;    //物料灯1， 白灯
            struCnfg.stuLayerLampCfg.nLampPortEnable[0][1]  = 1;    //物料灯2,  白灯或850
            struCnfg.stuLayerLampCfg.nLampPortEnable[0][2]  = 1;    //背景
            struCnfg.stuLayerLampCfg.nLampPortEnable[0][3]  = 1;    //物料灯3,  白灯或850
            struCnfg.stuLayerLampCfg.nLampPortEnable[0][4]  = 1;    //物料灯4， 白灯
            struCnfg.stuLayerLampCfg.nLampPortEnable[0][5]  = (hasInfra1450 == 0) ? 0 : 1;    //1450红外

            struCnfg.stuLayerLampCfg.nLampPortEnable[1][0]  = 1;    //物料灯1  白灯
            struCnfg.stuLayerLampCfg.nLampPortEnable[1][1]  = 1;    //物料灯2  蓝灯
            struCnfg.stuLayerLampCfg.nLampPortEnable[1][2]  = 1;    //背景
            struCnfg.stuLayerLampCfg.nLampPortEnable[1][3]  = 1;    //物料灯3  蓝灯
            struCnfg.stuLayerLampCfg.nLampPortEnable[1][4]  = 1;    //物料灯4  白灯
            struCnfg.stuLayerLampCfg.nLampPortEnable[1][5]  = (hasInfra1450 == 0) ? 0 : 1;    //1450红外

            for(j = 0; j < 5; j++)
            {
                if(j == 2){ //背景灯
                    struCnfg.stuLayerLampCfg.nLampPortType[0][j] = 1;
                    struCnfg.stuLayerLampCfg.nLampPortType[1][j] = 1;
                }else{      //非1450物料灯
                    struCnfg.stuLayerLampCfg.nLampPortType[0][j] = 0;
                    struCnfg.stuLayerLampCfg.nLampPortType[1][j] = 0;
                }
            }

            for(j = 0; j < 5; j++)
            {
                struCnfg.stuLayerLampCfg.nLampBoardLimit[0][j]  = 100;    //非1450灯上限值
                struCnfg.stuLayerLampCfg.nLampBoardLimit[1][j]  = 100;    //非1450灯上限值
            }

            struCnfg.stuLayerLampCfg.nLampBoardLimit[0][5]  = 20;    //1450红外灯上限值
            struCnfg.stuLayerLampCfg.nLampBoardLimit[1][5]  = 20;    //1450红外灯上限值
        }

        break;
    case MACHINE_LD2_ZL_GP:
    {
        struCnfg.stuLayerLampCfg.nLampBoardTotal = 4;
        struCnfg.stuLayerLampCfg.nLampValueQuo = 0;
        for(i = 0; i < struCnfg.stuLayerLampCfg.nLampBoardTotal; i++)
        {
            for(j = 0; j < MAX_LAMP_PER_BOARD; j++)
            {
                struCnfg.stuLayerLampCfg.nLampLayerAddr[i][j]  = 0;
                struCnfg.stuLayerLampCfg.fLampBoardQuo[i][j] = (struCnfg.nChuteTotal+1)/11.0;
            }
        }

        for(j = 0; j < MAX_LAMP_PER_BOARD; j++)
        {
            struCnfg.stuLayerLampCfg.nLampViewAddr[0][j]  = 0; //前分选室
            struCnfg.stuLayerLampCfg.nLampViewAddr[1][j]  = 0; //后分选室
            struCnfg.stuLayerLampCfg.nLampViewAddr[2][j]  = 1; //前分选室
            struCnfg.stuLayerLampCfg.nLampViewAddr[3][j]  = 1; //后分选室
        }

        struCnfg.stuLayerLampCfg.nLampPortEnable[0][0]  = 1;    //物料灯1， 白灯
        struCnfg.stuLayerLampCfg.nLampPortEnable[0][1]  = 1;    //物料灯2,  白灯或850
        struCnfg.stuLayerLampCfg.nLampPortEnable[0][2]  = 1;    //背景
        struCnfg.stuLayerLampCfg.nLampPortEnable[0][3]  = 1;    //物料灯3,  白灯或850
        struCnfg.stuLayerLampCfg.nLampPortEnable[0][4]  = 1;    //物料灯4， 白灯
        struCnfg.stuLayerLampCfg.nLampPortEnable[0][5]  = 1;    //背景

        struCnfg.stuLayerLampCfg.nLampPortEnable[1][0]  = 1;    //物料灯1  白灯
        struCnfg.stuLayerLampCfg.nLampPortEnable[1][1]  = 1;    //物料灯2  蓝灯
        struCnfg.stuLayerLampCfg.nLampPortEnable[1][2]  = 1;    //背景
        struCnfg.stuLayerLampCfg.nLampPortEnable[1][3]  = 1;    //物料灯3  蓝灯
        struCnfg.stuLayerLampCfg.nLampPortEnable[1][4]  = 1;    //物料灯4  白灯
        struCnfg.stuLayerLampCfg.nLampPortEnable[1][5]  = 1;    //背景

        struCnfg.stuLayerLampCfg.nLampPortEnable[2][0]  = 1;    //物料灯1， 白灯
        struCnfg.stuLayerLampCfg.nLampPortEnable[2][1]  = 1;    //物料灯2,  白灯或850
        struCnfg.stuLayerLampCfg.nLampPortEnable[2][2]  = 1;    //背景
        struCnfg.stuLayerLampCfg.nLampPortEnable[2][3]  = 1;    //物料灯3,  白灯或850
        struCnfg.stuLayerLampCfg.nLampPortEnable[2][4]  = 1;    //物料灯4， 白灯
        struCnfg.stuLayerLampCfg.nLampPortEnable[2][5]  = 1;    //背景

        struCnfg.stuLayerLampCfg.nLampPortEnable[3][0]  = 1;    //物料灯1  白灯
        struCnfg.stuLayerLampCfg.nLampPortEnable[3][1]  = 1;    //物料灯2  蓝灯
        struCnfg.stuLayerLampCfg.nLampPortEnable[3][2]  = 1;    //背景
        struCnfg.stuLayerLampCfg.nLampPortEnable[3][3]  = 1;    //物料灯3  蓝灯
        struCnfg.stuLayerLampCfg.nLampPortEnable[3][4]  = 1;    //物料灯4  白灯
        struCnfg.stuLayerLampCfg.nLampPortEnable[4][5]  = 1;    //背景

        for(j = 0; j < MAX_LAMP_PER_BOARD; j++)
        {
            if((j == 2)||(j == 5)){ //背景灯
                struCnfg.stuLayerLampCfg.nLampPortType[0][j] = 1;
                struCnfg.stuLayerLampCfg.nLampPortType[1][j] = 1;
                struCnfg.stuLayerLampCfg.nLampPortType[2][j] = 1;
                struCnfg.stuLayerLampCfg.nLampPortType[3][j] = 1;
            }else{      //非1450物料灯
                struCnfg.stuLayerLampCfg.nLampPortType[0][j] = 0;
                struCnfg.stuLayerLampCfg.nLampPortType[1][j] = 0;
                struCnfg.stuLayerLampCfg.nLampPortType[2][j] = 0;
                struCnfg.stuLayerLampCfg.nLampPortType[3][j] = 0;
            }
        }

        for(j = 0; j < MAX_LAMP_PER_BOARD; j++)
        {
            struCnfg.stuLayerLampCfg.nLampBoardLimit[0][j]  = 100;    //非1450灯上限值
            struCnfg.stuLayerLampCfg.nLampBoardLimit[1][j]  = 100;    //非1450灯上限值
            struCnfg.stuLayerLampCfg.nLampBoardLimit[2][j]  = 100;    //非1450灯上限值
            struCnfg.stuLayerLampCfg.nLampBoardLimit[3][j]  = 100;    //非1450灯上限值
        }
    }

        break;
    default:
        break;
    }
}

QString GlobalFlow::getArithNameByIndex(int index)
{
    QMap<int,QString> totalArithMap;
    totalArithMap.insert(ARITH_GREY_A,g_myLan().arith_gray_a);
    totalArithMap.insert(ARITH_GREY_B,g_myLan().arith_gray_b);
    totalArithMap.insert(ARITH_GREY_C,g_myLan().arith_gray_c);
    totalArithMap.insert(ARITH_GREY_D,g_myLan().arith_gray_d);
    totalArithMap.insert(ARITH_DISCOLOR_A,g_myLan().arith_discolor_a);
    totalArithMap.insert(ARITH_DISCOLOR_B,g_myLan().arith_discolor_b);
    totalArithMap.insert(ARITH_CROSS,g_myLan().arith_cross);
    totalArithMap.insert(ARITH_SVM_A,"SVMA");
    totalArithMap.insert(ARITH_SVM_B,"SVMB");
    totalArithMap.insert(ARITH_SVM_C,"SVMC");
    totalArithMap.insert(ARITH_SVM_D,"SVMD");
    totalArithMap.insert(ARITH_SHAPE_SVM_A,g_myLan().shape+g_myLan().arith_svm_a);
    totalArithMap.insert(ARITH_SHAPE_SVM_B,g_myLan().shape+g_myLan().arith_svm_b);
    totalArithMap.insert(ARITH_SHAPE_SVM_C,g_myLan().shape+g_myLan().arith_svm_c);
    totalArithMap.insert(ARITH_BIG_SMALL,g_myLan().arith_big_small);
    totalArithMap.insert(ARITH_LONG_SHORT,g_myLan().arith_long_short);
    totalArithMap.insert(ARITH_LONG_SHORT_B,g_myLan().arith_long_short_b);
    totalArithMap.insert(ARITH_CIRCLE_LONG,g_myLan().arith_circle_long);
    totalArithMap.insert(ARITH_SAT,g_myLan().arith_sat);
    totalArithMap.insert(ARITH_HUE,g_myLan().arith_hue);
    totalArithMap.insert(ARITH_SCALE,g_myLan().arith_scale);
    totalArithMap.insert(ARITH_MILDEW,g_myLan().arith_mildew);
    totalArithMap.insert(ARITH_OUTLINE,g_myLan().arith_outline);
    totalArithMap.insert(ARITH_LINE,g_myLan().arith_line);
    totalArithMap.insert(ARITH_BUD,g_myLan().arith_bud);
    totalArithMap.insert(ARITH_BUD_B,g_myLan().arith_bud_b);
    totalArithMap.insert(ARITH_ROUND,g_myLan().arith_round);
    totalArithMap.insert(ARITH_HSV,"HSVA");
    totalArithMap.insert(ARITH_HSV_B,"HSVB");
    totalArithMap.insert(ARITH_HSV_C,"HSVC");
    totalArithMap.insert(ARITH_VALUE,g_myLan().arith_value);
    totalArithMap.insert(ARITH_SPL_MILDEW,g_myLan().arith_spl_mildew);
    totalArithMap.insert(ARITH_W_PEANUT,g_myLan().arith_white_peanut);
    totalArithMap.insert(ARITH_BIG_SMALL_B,g_myLan().arith_big_small_b);
    totalArithMap.insert(ARITH_CIRCLE_LONG_B,g_myLan().arith_circle_long_b);
    totalArithMap.insert(ARITH_ROUND_B,g_myLan().arith_round_b);
    totalArithMap.insert(ARITH_PEPPER_HANDLE,g_myLan().arith_pepper_handle);
    totalArithMap.insert(ARITH_SUPPRESS_LARGE_A,g_myLan().arith_suppress_large+"A");
    totalArithMap.insert(ARITH_SUPPRESS_LARGE_B,g_myLan().arith_suppress_large+"B");
    totalArithMap.insert(ARITH_SUPPRESS_LARGE_C,g_myLan().arith_suppress_large+"C");
    totalArithMap.insert(ARITH_WHEAT_SPROUT,g_myLan().arith_wheat_sprout);
    totalArithMap.insert(TEA_ARITH_COLOR,"TEA_A");
    totalArithMap.insert(TEA_ARITH_COLOR_2,"TEA_B");
    totalArithMap.insert(TEA_ARITH_COLOR_3,"TEA_C");
    totalArithMap.insert(TEA_ARITH_COLOR_4,"TEA_D");
    totalArithMap.insert(TEA_ARITH_COLOR_5,"TEA_E");

    if(totalArithMap.contains(index))
        return totalArithMap.value(index);
    return QString("out index");


    //    QStringList totalArithList;
    //    totalArithList<< g_myLan().arith_gray_a<< g_myLan().arith_gray_b<< g_myLan().arith_gray_c<< g_myLan().arith_gray_d
    //                  << g_myLan().arith_discolor_a<< g_myLan().arith_discolor_b <<g_myLan().arith_cross
    //                  << g_myLan().arith_svm_a<< g_myLan().arith_svm_b<< g_myLan().arith_svm_c
    //                  << g_myLan().arith_big_small << g_myLan().arith_long_short << g_myLan().arith_circle_long<<g_myLan().arith_hsv;
    //    if(index >= totalArithList.count())
    //        return QString("out");
    //    return totalArithList[index];
}

int GlobalFlow::getDiffArithColor(int diffColor, bool isSim)
{
    int tmpDiffColor;

    if(isSim)
    {
        switch(diffColor)
        {
        case 0: //红绿
            tmpDiffColor = (BACKGROUND_COLOR_RED<<4)|BACKGROUND_COLOR_GREEN;
            break;
        case 1: //红蓝
            tmpDiffColor = (BACKGROUND_COLOR_RED<<4)|BACKGROUND_COLOR_BLUE;
            break;
        case 2: //绿蓝
            tmpDiffColor = (BACKGROUND_COLOR_GREEN<<4)|BACKGROUND_COLOR_BLUE;
            break;
        case 3: //红红外
            tmpDiffColor = (BACKGROUND_COLOR_RED<<4)|BACKGROUND_COLOR_NIR;
            break;
        case 4: //绿红外
            tmpDiffColor = (BACKGROUND_COLOR_GREEN<<4)|BACKGROUND_COLOR_NIR;
            break;
        case 5: //蓝红外
            tmpDiffColor = (BACKGROUND_COLOR_BLUE<<4)|BACKGROUND_COLOR_NIR;
            break;
        default:
            break;
        }
    }
    else
    {
        switch(diffColor)
        {
        case 0: //红绿
            tmpDiffColor = (0<<4)|1;
            break;
        case 1: //红蓝
            tmpDiffColor = (0<<4)|2;
            break;
        case 2: //绿蓝
            tmpDiffColor = (1<<4)|2;
            break;
        case 3: //红红外
            tmpDiffColor = (0<<4)|3;
            break;
        case 4: //绿红外
            tmpDiffColor = (1<<4)|3;
            break;
        case 5: //蓝红外
            tmpDiffColor = (2<<4)|3;
            break;
        default:
            break;
        }
    }

    return tmpDiffColor;
}

int GlobalFlow::changeColorBlackWhite2Nir(int color)
{
    int tmpColor;

    switch(color)
    {
    case BACKGROUND_COLOR_RED:
    case BACKGROUND_COLOR_GREEN:
    case BACKGROUND_COLOR_BLUE:
        tmpColor = color;
        break;
    case BACKGROUND_COLOR_BLACK_WHITE:
    case BACKGROUND_COLOR_GREY:
    case BACKGROUND_COLOR_YELLOW:
        tmpColor = color+1;
        break;
    case BACKGROUND_COLOR_NIR:
        tmpColor = BACKGROUND_COLOR_BLACK_WHITE;
        break;
    }

    return tmpColor;
}

/* 获取全局文件参数 */
void GlobalFlow::getGlobal()
{
    int i, j, k;
    QFile file;

    memset(&struCnfg, 0, sizeof(struCnfGlobal));
    memset(&_t_struCnfg, 0, sizeof(struCnfGlobal));

    struCnfg.nLang                  = LANG_CHS;
    struCnfg.nCounter               = 0;
    struCnfg.nProfile               = 0;
    struCnfg.nProfileTotal          = 0;


    struCnfg.nCtrlBoardType         =   BOARD_TYPE_CONTROL;
    struCnfg.nLampBoardType         =   BOARD_TYPE_LAMP;
    struCnfg.nWipeEnable            =   1;
    struCnfg.nControlAlarmSet       =   0;
    struCnfg.nVavleTimeSetMode       =  1;
    struCnfg.nEnableDispArithSetByTimes = 0;
    struCnfg.nFeedControlType1 = 1;
    struCnfg.nHasUps = 0;
    struCnfg.nSpray = 0;
    struCnfg.nVibType = 0;
    struCnfg.nWheelRunType = 0;
    struCnfg.nVibFreq = 0;
    struCnfg.bigVibIsUse = 0;
    struCnfg.nBigVibPos = 1;
    struCnfg.nVibFreq_2 = 2;
    struCnfg.ipc_run_test_count = 0;
    struCnfg.nIpcRunTestIsRun = 0;
    struCnfg.nIpcRunTestCycle = 1;
    struCnfg.accelerationTolerance = 3;
    struCnfg.accelerationTolerance2 = 3;
    struCnfg.accelerationNum = 1;
    struCnfg.accelerationNum2 = 1;
    struCnfg.overRatio      = 50;
    struCnfg.areaThres      = 0;
    struCnfg.overRatioTime  = 300;

    struCnfg.bGetMaterialType  = 0;
    struCnfg.bMainpageShow  = 0;
    /* 报警使能 */
    struCnfg.struAlarmConfig.nAirPressureAlarmType =1;
    struCnfg.struAlarmConfig.nLightVoiceAlarmType = 0;
    struCnfg.struAlarmConfig.nFeederLevelAlarmDealMode = 0;
    for(i = 0; i < CTRL_ALARM_MAX_FEED_SENSOR; i++)
    {
        struCnfg.struAlarmConfig.nFeederLevelAlarmEnable[i] = 0;
        struCnfg.struAlarmConfig.nFeederLevelAlarmPosi[i] = 0;
        memset(struCnfg.struAlarmConfig.nFeederLevelAlarmName[i], 0, 128);
        if(g_myLan().g_languageIndex == LANG_CHS)
        {
            sprintf(struCnfg.struAlarmConfig.nFeederLevelAlarmName[i], "料位 %d", i+1);
        }
        else
        {
            sprintf(struCnfg.struAlarmConfig.nFeederLevelAlarmName[i], "Feed Level %d", i+1);
        }
    }
    struCnfg.struAlarmConfig.nTemptureAlarmDealMode = 0;
    for(i = 0; i < CTRL_ALARM_MAX_TEMP_SENSOR; i++)
    {
        struCnfg.struAlarmConfig.nTemptureAlarmEnable[i] = 0;
        struCnfg.struAlarmConfig.nTemptureAlarmLimit[i] = 60;
        memset(struCnfg.struAlarmConfig.nTemptureAlarmName[i], 0, 128);
        if(g_myLan().g_languageIndex == LANG_CHS)
        {
            sprintf(struCnfg.struAlarmConfig.nTemptureAlarmName[i], "温度 %d", i+1);
        }
        else
        {
            sprintf(struCnfg.struAlarmConfig.nTemptureAlarmName[i], "Temp %d", i+1);
        }
    }

    for(i = 0; i < MAX_ALARM; i++)
    {
        struCnfg.nAlarmValid[i] = 0;
    }
    struCnfg.nAlarmValid[ALARM_PRESSURE] = 1;          // 使能气压报警

    struCnfg.nLampBoardQuoType     = LAMP_BOARD_QUO_TYPE_M;

    memset(struCnfg.sDelayCode, 0, MAX_DELAYCODE);

    struCnfg.nMachineType = MACHINE_LD2_ZL_GP;

    for(i = 0; i < MAX_VIEW; i++)
    {
        struCnfg.struLayerInfo[0].nViewIsUsed[i] = 1;
        // struCnfp.nViewSensorType[0][i]     = SENSOR_G2;   //前主可见
    }

    switch (struCnfg.nMachineType) {
    case MACHINE_VERTICAL_DM_RG:
    case MACHINE_VERTICAL_ZL_CS:
    case MACHINE_VERTICAL_ZL_CV:
    case MACHINE_VERTICAL_DM_ZK:
        getVertRiceGeneralLayerInfo();
        break;
    case MACHINE_LD2_ZL_GP:
        getBeltLd2GeneralLayerInfo();
        break;
    default:
        getVertRiceGeneralLayerInfo();
        break;
    }

    getCameraParams();

    getGlobalLampConfig();

    struCnfg.nBigSmallArithMaxValue = ARITH_BIG_SMALL_MAX_VALUE;
    struCnfg.nEdgeCutMaxValue = 9; //xiewj:E系统要求修改
    struCnfg.nImpuredMaxValue = 16;
    struCnfg.nEjectDurationMaxValue = 6.3;
    struCnfg.ntotalFeedTime = 0;
    struCnfg.ntotalWipeCount = 0;
    struCnfg.ntotalAlarmCount = 0;
    struCnfg.nTickDirect = 0;
    struCnfg.nChuteSpliceMode = 0;
    struCnfg.nChuteSpliceAddr = 0;
    struCnfg.n_coff_user = 1.0;
    struCnfg.n_svmType = 0;
    struCnfg.nLVMachine = (PRODUCT_DEFAULT_VENDOR == PRODUCT_VENDOR_LV);
    struCnfg.nDPMachine = 0;
    struCnfg.nUseBMModule = 0;
    struCnfg.nUseLowcfgSync = 0;
    struCnfg.nIpcIpAddrBase = 1;
    struCnfg.nIpcCameraClassSameMode = 0;
    struCnfg.nPlcControl = 0;
    struCnfg.nAllInOne = 0;
    struCnfg.nValveDefendTime = 20000;
    struCnfg.nValveUndefendTime = 100000;
    struCnfg.nScreenSet = 0;
    struCnfg.nValveDefend = 1;
    struCnfg.nRealTimeCorrection = 0;
    struCnfg.bUseAutoFeeder = 0;
    struCnfg.imgSaturationFactor = 100;
    struCnfg.imgSaturationEnable = 0;
    struCnfg.bLogoLightNum =1;
    struCnfg.fpgaParamsNum =1;
    struCnfg.trigArea = 10;
    struCnfg.cutHeight = 400;
    struCnfg.imgDim = 3;
    struCnfg.autoTargetImage = 0;
    for(i = 0; i < MAX_LAMP_PER_BOARD ; i++)
    {
        struCnfg.bLogoLightMode[i]  = 4;
        struCnfg.bLogoLightLength[i] = 90;
    }
    for(i = 0;i <MAX_VIEW;i++)
    {
        for(j = 0; j<MAX_VIEW_UNIT;j++)
        {
            struCnfg.nIpcStartPixel[i][j] = 0;
            struCnfg.nIpcEndPixel[i][j] = 350;
        }
    }
    /* 统计信息配置 */
    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            struCnfg.nStaticsConfig[i][j].nStaticsTotal = 11;

            struCnfg.nStaticsConfig[i][j].nIndex[0]= 0;   //产量

            // 没有温度信息
            struCnfg.nStaticsConfig[i][j].nIndex[1]= 10;   //灰度A
            struCnfg.nStaticsConfig[i][j].nIndex[2]= 11;   //灰度B
            struCnfg.nStaticsConfig[i][j].nIndex[3]= 20;   //色差A
            struCnfg.nStaticsConfig[i][j].nIndex[4]= 21;   //色差B
            struCnfg.nStaticsConfig[i][j].nIndex[5]= 30;    //差分

            struCnfg.nStaticsConfig[i][j].nIndex[6]= 40;    //SVM A
            struCnfg.nStaticsConfig[i][j].nIndex[7]= 40;    //SVM B
            struCnfg.nStaticsConfig[i][j].nIndex[8]= 50;    //大小
            struCnfg.nStaticsConfig[i][j].nIndex[9]= 51;    //长度
            struCnfg.nStaticsConfig[i][j].nIndex[10] = 52;    //圆长


            struCnfg.nStaticsConfig[i][j].nBytes[0]= 4;
            //            for(k = 0; k < MAX_ARITH_TOTAL; k++)
            //            {
            //                struCnfg.nStaticsConfig[i][j].nBytes[k+1] = 2;
            //            }
        }
    }

    /* 智能清灰配置 */
    struCnfg.struIntelWipe.feederOnffCount = 3;
    struCnfg.struIntelWipe.wiperDurationPerChute = struCnfg.struLayerInfo[0].nEjectorsPerChute;
    struCnfg.struIntelWipe.wiperWidth = 145;
    struCnfg.struIntelWipe.wipeTimeout = 100;
    struCnfg.struIntelWipe.wiperDefaultPosi = 0;
    struCnfg.struIntelWipe.wiperConfirmThread = 60;

    struCnfg.stuUseLimitCfg.valve_start_use = 0;
    struCnfg.stuUseLimitCfg.valve_life = 60;
    struCnfg.stuUseLimitCfg.filter_start_use = 0;
    struCnfg.stuUseLimitCfg.filter_life = 60;
    struCnfg.stuUseLimitCfg.wipe_start_use = 0;
    struCnfg.stuUseLimitCfg.wipe_life = 10000;
    struCnfg.stuUseLimitCfg.lamp_start_use = 0;
    struCnfg.stuUseLimitCfg.lamp_life = 60;

    /* 统计信息获取使能的视 */
    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            struCnfg.sStaticsGetGroupEnable[i][j][0] = 1;   //使能一次所有的视
        }
    }
    /* 电路板启动模式 */
    struCnfg.nInterfaceBoardMode = 1;        // 接口板默认运行模式为1
    struCnfg.nCtrlBoardMode = 1;             // 控制板默认运行模式为1
    struCnfg.nLampBoardMode = 1;             // 灯光板默认运行模式为1

    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nVavleChainTotal; j++)
        {
            struCnfg.nVavleBoardMode[i][j] = 1;    // 每吹的模式默认都是1
        }
    }

    getPrefixedName();  // 获取预制方案父节点和子节点名称

    for(int i = 0; i < MAX_VI_BOARD;i++){
        struCnfg.viBoardInfo[i][0] = 1;
        struCnfg.viBoardInfo[i][1] = 0;
        struCnfg.viBoardInfo[i][2] = 0;
        struCnfg.viBoardInfo[i][3] = 0;
    }

    //均衡流量设置--------------------
    struCnfg.stuFeedAutoCtrl.isHasRadar = 1;    //雷达探测
    struCnfg.stuFeedAutoCtrl.riceContainerDepth = 1050;
    struCnfg.stuFeedAutoCtrl.levelLowPosi = 800;
    struCnfg.stuFeedAutoCtrl.levelEmptyPosi = 950;
    struCnfg.stuFeedAutoCtrl.levelMorePosi = 300;
    struCnfg.stuFeedAutoCtrl.levelFullPosi = 250;
    struCnfg.stuFeedAutoCtrl.feedMoreStartValue = 70;
    struCnfg.stuFeedAutoCtrl.feedEmpytLowLimitValue = 20;
    struCnfg.stuFeedAutoCtrl.feedFullToNormalContinueTime = 1;
    struCnfg.stuFeedAutoCtrl.feedBalanceClearUpValue = 95;
    struCnfg.stuFeedAutoCtrl.autoFeedMode = AUTO_FEED_MODE_3;
    struCnfg.stuFeedAutoCtrl.mode1FeedTargetPartCount = 4;
    struCnfg.stuFeedAutoCtrl.feedLevelChangeWaitTime = 5;
    struCnfg.stuFeedAutoCtrl.feedLevelPreFullWaitTime = 30;
    struCnfg.stuFeedAutoCtrl.feedPreFullConstValue = 70;
    struCnfg.stuFeedAutoCtrl.feedClearModeValue  = 70;
    struCnfg.stuFeedAutoCtrl.feedBalAdjustStep = 1;
    struCnfg.stuFeedAutoCtrl.feedBalAdjustDuration = 2;
    struCnfg.stuFeedAutoCtrl.balUpLowFlow = 1000;
    struCnfg.stuFeedAutoCtrl.balUpFeedWhileLowFlow = 90;
    struCnfg.stuFeedAutoCtrl.posiChgThres = 20;
    struCnfg.stuFeedAutoCtrl.posiChgWait = 1;
    struCnfg.stuFeedAutoCtrl.adjustPosuUnit = 50;
    struCnfg.stuFeedAutoCtrl.balUpFeedflow = 70;
    struCnfg.stuFeedAutoCtrl.balLowFeedflow = 50;

    struCnfg.nOnoffDelay = 3;
    struCnfg.nOnoffDelay_2 = 0;
    struCnfg.nAutoStaticsEnable = 0;
    struCnfg.sTotalStaticsGetDuration = 120;
    struCnfg.nCmdSendTimeout = 10;
    struCnfg.nUGSendTimeout = 25;
    struCnfg.nUgSendInterval = 10;
    struCnfg.nWaitTimeout = 100;
    struCnfg.nAutoStaticsWorking = 1;
    struCnfg.nAutoImageCaptureEnable = 0;
    struCnfg.bIpcCrossCam = 1;
    struCnfg.nIpcCount = 1;
    struCnfg.nIpcCamera = 8;
    struCnfg.nIpcModelCount = 1;
    struCnfg.nUseIpcEnable = 0;
    struCnfg.nIpcBadMatStaticsCount = 2;
    struCnfg.nIpcAlarmDealMode = 0;
    //struCnfg.nIsCameraConpress = 0;
    struCnfg.camParaInCnf = 0;
    struCnfg.nValveSite = 0;
    struCnfg.accChecked = 0;

    struCnfg.nDelayedShutdownTime   = 30;
    struCnfg.bautoplcctrlEnable = 0;
    struCnfg.nAutoBoardIpAddr[0] = 192;
    struCnfg.nAutoBoardIpAddr[1] = 168;
    struCnfg.nAutoBoardIpAddr[2] = 129;
    struCnfg.nAutoBoardIpAddr[3] = 20;
    struCnfg.nAutoBoardPort = 502;
    struCnfg.nGateway[0] = 192;                             //网关 4段IP 默认192.168.1.1
    struCnfg.nGateway[1] = 168;
    struCnfg.nGateway[2] = 1;
    struCnfg.nGateway[3] = 1;
    struCnfg.nSubnetMask[0] = 255;
    struCnfg.nSubnetMask[1] = 255;
    struCnfg.nSubnetMask[2] = 255;
    struCnfg.nSubnetMask[3] = 0;
    struCnfg.nModbusIP = 1;
    struCnfg.nBaudRate = 2;
    struCnfg.nPlcCtrl = 0;
    struCnfg.nDefaultPlcCtrl = 0;
    struCnfg.nPowerCut = 0;
    struCnfg.nBoardStatusAlarm = 0;
    struCnfg.bUpdateStatus = 0;
    struCnfg.autoCtrlBoardAlive = 0;
    struCnfg.blampctrlEnable = 0;
    struCnfg.nValveType = 1;
    for(int m= 0;m<MAX_VIEW;m++)
    {
        for(int k = 0; k < MAX_CALIBRATE_SETS; k++)
        {
            struCnfg.nSenseBrightSelection_calb[m][k] = 0;
            struCnfg.nSensorRowFre_calb[m][k] = 350;

            struCnfg.bCaliGainEnable_calb[k] = 0;
            struCnfg.bCaliBiasEnable_calb[k] = 0;
            struCnfg.bCaliTDmode_calb[m][k] = 0;

        }

        struCnfg.nXsystemBoard[m] = 0;
        for(int n= 0;n<MAX_VIEW_UNIT;n++)
        {
            for(int i =0;i<100;i++)
            {
                memset(struCnfg.struIpcRunTestInfo[m][n][i].timeStamp,0,sizeof(struCnfg.struIpcRunTestInfo[m][n][i].timeStamp));
                struCnfg.struIpcRunTestInfo[m][n][i].cpuUsed = 0;
                struCnfg.struIpcRunTestInfo[m][n][i].cpuTemp = 0;
                struCnfg.struIpcRunTestInfo[m][n][i].gpu1Used =0;
                struCnfg.struIpcRunTestInfo[m][n][i].gpuTemp = 0;
                struCnfg.struIpcRunTestInfo[m][n][i].camBadPoints = 0;
                struCnfg.struIpcRunTestInfo[m][n][i].netTransSpeed = 0;
            }
        }
    }

    getGlobalIpcConfig();
    memcpy(&_t_struCnfg, &struCnfg, sizeof(struCnfGlobal));

    if (struCnfs.value != GLOBAL_FILE_ERROR) {
        if (QFileInfo(CNF_GLOBAL).exists() && QFileInfo(CNF_GLOBAL).size() > 0)  {
            if (!getGlobalSetting(CNF_GLOBAL)) {
                qDebug("getGlobal(): can not get global parameters, using default");
                return;
            }
            file.copy(CNF_GLOBAL, CNF_GLOBAL_B);
        }
        memcpy(&_t_struCnfg, &struCnfg, sizeof(struCnfGlobal));
    }
    else {
        if (QFileInfo(CNF_GLOBAL_B).exists() && QFileInfo(CNF_GLOBAL_B).size() > 0) {
            if (!getGlobalSetting(CNF_GLOBAL_B)) {
                qDebug("getGlobal(): can not get global parameters, using default");
                return;
            }
            file.copy(CNF_GLOBAL_B, CNF_GLOBAL);
        }
        memcpy(&_t_struCnfg, &struCnfg, sizeof(struCnfGlobal));
    }
    struCnfg.nLVMachine = (PRODUCT_DEFAULT_VENDOR == PRODUCT_VENDOR_LV);
    return;
}


/* 保存全局文件参数 */
bool GlobalFlow::saveGlobal()
{
    int i;
    QFile file;

    saveParamsFileStaus(GLOBAL_FILE_ERROR);
    for (i = 0; i < 3; i++) {
        if (saveGlobalSetting(CNF_GLOBAL)) {
            saveParamsFileStaus(ALL_PARAMS_FILE_OK);
            break;
        }
    }

    if (i == 3)
        return false;

    if (QFileInfo(CNF_GLOBAL).exists() && QFileInfo(CNF_GLOBAL).size() > 0) {
        g_Runtime().copyFileToPath(CNF_GLOBAL, CNF_GLOBAL_B,true);
    }

    return true;
}

/* 清除所有算法使能 */
void GlobalFlow::resetAllArithEnableClear()
{
    int i, j, k, m;

    for(i = 0; i < struCnfg.nLayerTotal ; i++)
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            for(k = 0; k < struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal; k++)
            {
                for (m = 0; m < MAX_GREY_COLOR_ARITH_TOTAL; m++)  {	// 0: -grey A, 1: -grey B, 2: -grey C, 3: -grey D, 4: -discolor A, 5: -discolor B, 6差分A
                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].nArithEnable    = 0;
                }

                /*  智能化-SVM */
                for (m = 0; m < MAX_SVM_ARITH_TOTAL; m++) {
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nArithEnable     = 0;
                }

                /* 选圆选长算法 */
                for (m = 0; m < MAX_CIRCLE_LONG_TOTAL; m++) {
                    struCnfp.struGroupIdentify[i][j][k].struCircleLong[m].nArithEnable     = 0;
                }

                /* 长短算法 */
                for (m = 0; m < MAX_LONG_SHORT_TOTAL; m++)
                {
                    struCnfp.struGroupIdentify[i][j][k].struLongShort[m].nArithEnable      = 0;
                }
                /* 大小算法 */
                for (m = 0; m < MAX_BIG_SMALL_TOTAL; m++){
                    struCnfp.struGroupIdentify[i][j][k].struBigSmall[m].nArithEnable       = 0;
                }

                /* 饱和算法 */
                struCnfp.struGroupIdentify[i][j][k].struColorSat.nArithEnable       = 0;

                /* 比例算法 */
                struCnfp.struGroupIdentify[i][j][k].struColorScale.nArithEnable     = 0;

                /* 颜色算法 */
                struCnfp.struGroupIdentify[i][j][k].struColorHue.nArithEnable       = 0;

                /* 霉点算法 */
                struCnfp.struGroupIdentify[i][j][k].struMildew.nArithEnable         = 0;

                /* 轮廓算法 */
                struCnfp.struGroupIdentify[i][j][k].struOutline.nArithEnable        = 0;

                /* 纹理算法 */
                struCnfp.struGroupIdentify[i][j][k].struLine.nArithEnable           = 0;

                /* 选芽算法 */
                for (m = 0; m < MAX_BUD_TOTAL; m++)
                {
                    struCnfp.struGroupIdentify[i][j][k].struBud[m].nArithEnable            = 0;
                }

                /* 选圆、非圆算法 */
                for (m = 0; m < MAX_ROUND_TOTAL; m++)
                {
                    struCnfp.struGroupIdentify[i][j][k].struRound[m].nArithEnable          = 0;
                }

                /* 灰度算法 */
                struCnfp.struGroupIdentify[i][j][k].struColorValue.nArithEnable     = 0;

                /* 颜色算法 */
                for (m = 0; m < MAX_HSV_TOTAL; m++)
                {
                    struCnfp.struGroupIdentify[i][j][k].struHsv[m].nArithEnable     = 0;
                }

                /* 葫芦籽霉点算法 */
                struCnfp.struGroupIdentify[i][j][k].struSplMildew.nArithEnable = 0;

                /* 白沙果算法 */
                struCnfp.struGroupIdentify[i][j][k].struWhitePeanut.nArithEnable = 0;

                /* 辣椒选把算法 */
                struCnfp.struGroupIdentify[i][j][k].struPepperHandle.nArithEnable = 0;

                /* 压制大算法 */
                for (m = 0; m < MAX_SUPPRESS_LARGE_TOTAL; m++)
                {
                    struCnfp.struGroupIdentify[i][j][k].struSuppressLarge[m].nArithEnable = 0;
                }

                /* 小麦芽头算法 */
                struCnfp.struGroupIdentify[i][j][k].struWheatSprout.nArithEnable = 0;

                /* TEA算法 */
                for (m = 0; m < MAX_TEA_ARITH_TOTAL; m++)
                {
                    struCnfp.struGroupIdentify[i][j][k].struTeaColor[m].nArithEnable = 0;
                }

            }
        }
    }
}

/* 初始化设别参数 */
void GlobalFlow::getGroupIdentify()
{
    int i, j, k, m, l;
    int row, col, per;
    int nEjectDelay, nEjectDelay2,nEjectDelay3;

    nEjectDelay3 = 200;
    switch(struCnfg.nMachineType)
    {
    case MACHINE_VERTICAL_DM_RG:
        nEjectDelay    = 15;
        nEjectDelay2   = 180;
        break;
    case MACHINE_VERTICAL_DM_ZK:
        nEjectDelay    = 15;
        nEjectDelay2   = 180;
        break;
    case MACHINE_VERTICAL_ZL_CS:
    case MACHINE_VERTICAL_ZL_CV:
        nEjectDelay    = 15;
        nEjectDelay2   = 180;
        break;
    default:
        nEjectDelay    = 15;
        nEjectDelay2   = 180;
        break;
    }

    struCnfp.n_coff = 9000000;
    struCnfp.bColorRestoreEnable = 0;

    struCnfp.sStaticsGetDuration = 20;
    struCnfp.nImpuredIpcCount = 3;
    struCnfp.nEjectDurationIpcCount = 1;
    struCnfp.bEnableBalanceFeed = 0;  //默认关闭自动产量
    struCnfp.bCurCal = 0;

    struCnfp.ipcBadPointscoff = 1.0;
    for(i = 0; i < struCnfg.nLayerTotal ; i++)
    {
        for(j = 0;j < MAX_GROUP_TICK;j++){
            struCnfp.nAutoFeed[i][j].target_feeder = 50;
            struCnfp.nAutoFeed[i][j].start_feeder = 150;
            struCnfp.nAutoFeed[i][j].stop_feeder = 100;
            struCnfp.nAutoFeed[i][j].stop_duration = 20;
            struCnfp.nAutoFeed[i][j].ajust_feeder_range = 40;
            struCnfp.nAutoFeed[i][j].bQuickBalance = 0;
            struCnfp.nAutoFeed[i][j].bSingleAimBalance = 0;

            for(k = 0; k < AUTO_FEED_TARGET_PART; k++)
            {
                struCnfp.nAutoFeed[i][j].mode1FeedTargetPart[k] = 1500;
            }
        }

        for(j = 0; j < MAX_VIEW; j++)
        {
            switch(struCnfp.nViewSensorType[0][j])
            {
            case SENSOR_T1:
            case SENSOR_T1_850:
                row = 32;
                col = 32;
                per = row*col;
                break;
            case SENSOR_C1:
            case SENSOR_D2:
                row = 32;
                col = 32;
                per = row*col;
                break;
            default:
                row = 32;
                col = 32;
                per = row*col;
                break;
            }

            for(k = 0; k < MAX_GROUP_IDENTIFY; k++) {
                for(m = 0; m < 2; m++)
                {
                    struCnfp.struGroupIdentify[i][j][k].nEdgeCut[m]         = 0;
                    struCnfp.struGroupIdentify[i][j][k].nImpured[m]         = 0;
                    struCnfp.struGroupIdentify[i][j][k].nColseCal[m]        = 0;
                    struCnfp.struGroupIdentify[i][j][k].nReverseSort[m]     = 0;
                    struCnfp.struGroupIdentify[i][j][k].nCenterSize[m]      = 0;
                }
                for(m = 0; m < MAX_IMPURED_TOTAL; m++)
                {
                    struCnfp.struGroupIdentify[i][j][k].nImpuredIpc[m]         = 0;
                }
                struCnfp.struGroupIdentify[i][j][k].nEdgeMat                = 0;
                struCnfp.struGroupIdentify[i][j][k].nEjectDelay             = nEjectDelay;
                struCnfp.struGroupIdentify[i][j][k].nEjectDelayShape        = nEjectDelay2;
                struCnfp.struGroupIdentify[i][j][k].nEjectDelayModel        = nEjectDelay3;
                struCnfp.struGroupIdentify[i][j][k].nBadNum                 = 0;
                struCnfp.struGroupIdentify[i][j][k].nSensUpdateGeneral      = 1;
                struCnfp.struGroupIdentify[i][j][k].arithRejectRatio        = 0;
                struCnfp.struGroupIdentify[i][j][k].nEdgeMat                = 0;

                memset(struCnfp.struGroupIdentify[i][j][k].struGreyColor[ARITH_GREY_A].sName, 0, sizeof(struCnfp.struGroupIdentify[i][j][k].struGreyColor[ARITH_GREY_A].sName));
                memset(struCnfp.struGroupIdentify[i][j][k].struGreyColor[ARITH_GREY_B].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struGreyColor[ARITH_GREY_C].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struGreyColor[ARITH_GREY_D].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struGreyColor[ARITH_DISCOLOR_A].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struGreyColor[ARITH_DISCOLOR_B].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struGreyColor[ARITH_CROSS].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struSvm[0].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struSvm[1].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struSvm[2].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struSvm[3].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struShapeSvm[0].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struShapeSvm[1].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struShapeSvm[2].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struCircleLong[0].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struCircleLong[1].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struBigSmall[0].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struBigSmall[1].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struLongShort[0].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struLongShort[1].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struHsv[0].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struHsv[1].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struHsv[2].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struColorSat.sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struColorHue.sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struColorScale.sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struMildew.sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struOutline.sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struLine.sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struBud[0].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struBud[1].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struRound[0].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struRound[1].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struColorValue.sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struSplMildew.sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struWhitePeanut.sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struPepperHandle.sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struSuppressLarge[0].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struSuppressLarge[1].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struSuppressLarge[2].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struTeaColor[0].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struTeaColor[1].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struTeaColor[2].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struTeaColor[3].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struTeaColor[4].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struWheatSprout.sName, 0, AIRTH_NAME_LENGTH);


                /* 颜色算法， 灰度A,B，色差A,B，差分A... */
                if(g_myLan().g_languageIndex == LANG_CHS)
                {
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struGreyColor[ARITH_GREY_A].sName, "灰度A", sizeof("灰度A"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struGreyColor[ARITH_GREY_B].sName, "灰度B", sizeof("灰度B"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struGreyColor[ARITH_GREY_C].sName, "灰度C", sizeof("灰度C"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struGreyColor[ARITH_GREY_D].sName, "灰度D", sizeof("灰度D"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struGreyColor[ARITH_DISCOLOR_A].sName, "色差A", sizeof("色差A"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struGreyColor[ARITH_DISCOLOR_B].sName, "色差B", sizeof("色差B"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struGreyColor[ARITH_CROSS].sName, "色差C", sizeof("色差C"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struSvm[0].sName, "智能算法A", sizeof("智能算法A"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struSvm[1].sName, "智能算法B", sizeof("智能算法B"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struSvm[2].sName, "智能算法C", sizeof("智能算法C"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struSvm[3].sName, "智能算法D", sizeof("智能算法D"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struShapeSvm[0].sName, "形选智能算法A", sizeof("形选智能算法A"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struShapeSvm[1].sName, "形选智能算法B", sizeof("形选智能算法B"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struShapeSvm[2].sName, "形选智能算法C", sizeof("形选智能算法C"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struCircleLong[0].sName, "选粗选细", sizeof("选粗选细"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struCircleLong[1].sName, "选粗选细B", sizeof("选粗选细B"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struBigSmall[0].sName, "选大选小", sizeof("选大选小"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struBigSmall[1].sName, "选大选小B", sizeof("选大选小B"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struLongShort[0].sName, "选长选短A", sizeof("选长选短A"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struLongShort[1].sName, "选长选短B", sizeof("选长选短B"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struColorSat.sName, "饱和", sizeof("饱和"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struColorHue.sName, "颜色", sizeof("颜色"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struColorScale.sName, "比例", sizeof("比例"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struMildew.sName, "霉点", sizeof("霉点"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struOutline.sName, "轮廓", sizeof("轮廓"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struLine.sName, "纹理", sizeof("纹理"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struBud[0].sName, "选芽A", sizeof("选芽A"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struBud[1].sName, "选芽B", sizeof("选芽B"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struRound[0].sName, "选圆选非圆", sizeof("选圆选非圆"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struRound[1].sName, "选圆选非圆B", sizeof("选圆选非圆B"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struHsv[0].sName, "HSV-A", sizeof("HSV-A"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struHsv[1].sName, "HSV-B", sizeof("HSV-B"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struHsv[2].sName, "HSV-C", sizeof("HSV-C"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struColorValue.sName, "灰度", sizeof("灰度"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struSplMildew.sName,"葫芦籽霉点", sizeof("葫芦籽霉点"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struWhitePeanut.sName,"白沙果", sizeof("白沙果"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struPepperHandle.sName,"辣椒选把", sizeof("辣椒选把"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struSuppressLarge[0].sName, "压制大A", sizeof("压制大A"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struSuppressLarge[1].sName, "压制大B", sizeof("压制大B"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struSuppressLarge[2].sName, "压制大C", sizeof("压制大C"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struWheatSprout.sName,"小麦芽头", sizeof("小麦芽头"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struTeaColor[0].sName, "TEA_A", sizeof("TEA_A"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struTeaColor[1].sName, "TEA_B", sizeof("TEA_B"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struTeaColor[2].sName, "TEA_C", sizeof("TEA_C"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struTeaColor[3].sName, "TEA_D", sizeof("TEA_D"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struTeaColor[4].sName, "TEA_E", sizeof("TEA_E"));
                }
                else
                {
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struGreyColor[ARITH_GREY_A].sName, "Grey-A", sizeof("Grey-A"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struGreyColor[ARITH_GREY_B].sName, "Grey-B", sizeof("Grey-B"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struGreyColor[ARITH_GREY_C].sName, "Grey-C", sizeof("Grey-C"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struGreyColor[ARITH_GREY_D].sName, "Grey-D", sizeof("Grey-D"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struGreyColor[ARITH_DISCOLOR_A].sName, "Discolor-A", sizeof("Discolor-A"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struGreyColor[ARITH_DISCOLOR_B].sName, "Discolor-B", sizeof("Discolor-B"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struGreyColor[ARITH_CROSS].sName, "Discolor-C", sizeof("Discolor-C"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struSvm[0].sName, "AI-A", sizeof("AI-A"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struSvm[1].sName, "AI-B", sizeof("AI-B"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struSvm[2].sName, "AI-C", sizeof("AI-C"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struSvm[3].sName, "AI-D", sizeof("AI-D"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struShapeSvm[0].sName, "ShapeAI-A", sizeof("ShapeAI-A"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struShapeSvm[1].sName, "ShapeAI-B", sizeof("ShapeAI-B"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struShapeSvm[2].sName, "ShapeAI-C", sizeof("ShapeAI-C"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struCircleLong[0].sName, "CircleLong", sizeof("CircleLong"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struCircleLong[1].sName, "CircleLong-B", sizeof("CircleLong-B"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struBigSmall[0].sName, "BigSmall", sizeof("BigSmall"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struBigSmall[1].sName, "BigSmall-B", sizeof("BigSmall-B"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struLongShort[0].sName, "LongShort-A", sizeof("LongShort-A"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struLongShort[1].sName, "LongShort-B", sizeof("LongShort-B"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struColorSat.sName, "Sat", sizeof("Sat"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struColorHue.sName, "Hue", sizeof("Hue"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struColorScale.sName, "Scale", sizeof("Scale"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struMildew.sName, "Mildew", sizeof("Mildew"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struOutline.sName, "Outline", sizeof("Outline"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struLine.sName, "Line", sizeof("Line"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struBud[0].sName, "BudA", sizeof("BudA"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struBud[1].sName, "BudB", sizeof("BudB"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struRound[0].sName, "Round", sizeof("Round"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struRound[1].sName, "Round-B", sizeof("Round-B"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struHsv[0].sName, "HSV-A", sizeof("HSV-A"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struHsv[1].sName, "HSV-B", sizeof("HSV-B"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struHsv[2].sName, "HSV-C", sizeof("HSV-C"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struColorValue.sName, "Value", sizeof("Value"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struSplMildew.sName,"SplMildew", sizeof("SplMildew"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struWhitePeanut.sName,"WhitePeanut", sizeof("WhitePeanut"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struPepperHandle.sName, "Pepper-Handle", sizeof("Pepper-Handle"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struSuppressLarge[0].sName, "SuppressLargeA", sizeof("SuppressLargeA"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struSuppressLarge[1].sName, "SuppressLargeB", sizeof("SuppressLargeB"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struSuppressLarge[2].sName, "SuppressLargeC", sizeof("SuppressLargeC"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struWheatSprout.sName,"WheatSprout", sizeof("WheatSprout"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struTeaColor[0].sName, "TEA_A", sizeof("TEA_A"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struTeaColor[1].sName, "TEA_B", sizeof("TEA_B"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struTeaColor[2].sName, "TEA_C", sizeof("TEA_C"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struTeaColor[3].sName, "TEA_D", sizeof("TEA_D"));
                    memcpy(struCnfp.struGroupIdentify[i][j][k].struTeaColor[4].sName, "TEA_E", sizeof("TEA_E"));
                }

                for (m = 0; m < MAX_GREY_COLOR_ARITH_TOTAL; m++)  {	// 0: -grey A, 1: -grey B, 2: -grey C, 3: -grey D, 4: -discolor A, 5: -discolor B, 6差分A
                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].nArithEnable    = 0;
                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].nDispEnable     = 0;
                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].nVavle          = VAVLE_TYPE_1;
                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].nRow            = row;
                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].nColumn         = col;
                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].nPercent        = per;
                    if((m == ARITH_GREY_A)||(m == ARITH_GREY_B)||(m == ARITH_GREY_C)||(m == ARITH_GREY_D))
                    {
                        struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].nMain           = 2;
                        struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].nVice           = 2;
                    }
                    else
                    {
                        struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].nMain           = 0;
                        struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].nVice           = 1;
                    }
                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].nDiffColor           = 0;

                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].nSensLow        = 0;
                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].nSensUp         = 0;
                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].nMode           = 0;  //差分默认选亮
                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].lightLimit      = 0;

                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].fPropotionSensLow  = 1.0;
                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].fPropotionSensUp  = 1.0;
                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].nPropResSens  = 0.0;
                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].nPropotionParaSensLow  = 1;

                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].nLineDefectEnable = 0;
                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].nLineDefectIndex  = 0;
                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].fnLineDefectAreaLow  = 0;
                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].fnLineDefectAreaUp  = 0;
                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].fnLineDefectLengthLow  = 0;
                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].fnLineDefectLengthUp  = 0;

                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].nResType        = 0;   //可见光灰度A，灰度B，C,D 色差A，色差B，默认颜色保留使能
                    if((m==ARITH_GREY_C)||(m==ARITH_GREY_D))//灰度C和灰度D，默认比例保留开
                    {
                        struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].nResType        = 2;
                    }

                    //                    if(((struCnfg.struLayerInfo[i].nViewSensorType[j]==SENSOR_T1)&&((m==ARITH_GREY_A)||(m==ARITH_GREY_B)||(m==ARITH_DISCOLOR_A)||(m==ARITH_DISCOLOR_B))))
                    //                    {
                    //                        struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].nResType        = 0;   //可见光灰度A，灰度B，色差A，色差B，默认颜色保留使能
                    //                    }
                    //                    else if(((struCnfg.struLayerInfo[i].nViewSensorType[j]==SENSOR_T1)&&(m==ARITH_GREY_C))
                    //                           ||(struCnfg.struLayerInfo[i].nViewSensorType[j]==SENSOR_T1_850)&&(m==ARITH_GREY_A))//灰度C和近红外灰度A，默认比例保留开
                    //                    {
                    //                    else if((m==ARITH_GREY_C)||(m==ARITH_GREY_D))//灰度C和近红外灰度A，默认比例保留开
                    //                    {
                    //                        struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].nResType        = 2;
                    //                    }
                    //                    else
                    //                    {
                    //                        struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].nResType        = 0;
                    //                    }
                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].nSensUpdate     = 0;
                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].nEdgeType       = 0;
                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].nImpureType     = 0;
                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].nPropotionEnable  = 0;

                    /* 传统颜色保留 */
                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].struResGreyColor.nColor1       = 0;
                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].struResGreyColor.nSensLow1     = 0;
                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].struResGreyColor.nSensUp1      = 0;
                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].struResGreyColor.nMode1        = 0;
                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].struResGreyColor.nGreyResEnable    = 0;
                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].struResGreyColor.nMain2        = 0;
                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].struResGreyColor.nVice2        = 1;
                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].struResGreyColor.nResDiffColor = 0;
                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].struResGreyColor.nSensLow2     = 0;
                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].struResGreyColor.nSensUp2     = 0;
                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].struResGreyColor.nMode2        = 0;
                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].struResGreyColor.nDiscolorResEnable    = 0;

                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].nEdge        = 0;
                    struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].delayType    = 1;
                }

                /*  智能化-SVM */
                for (m = 0; m < MAX_SVM_ARITH_TOTAL; m++) {
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nTypeIndex           = 1; //svm编号
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nArithEnable         = 0;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nDispEnable          = 0;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nVavle               = VAVLE_TYPE_1;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nSizeType            = 0; //尺寸类型
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nRow                 = row;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nColumn              = col;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nPercent             = per;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nSens                = 100;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nModeSvm             = 0;

                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nEdgeType            = 0;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nImpureType          = 0;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].fPropotionSensLow    = 1.0;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].fPropotionSensUp     = 1.0;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nPropotionParaSensLow  = 1;

                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nLineDefectEnable = 0;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nLineDefectIndex  = 0;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].fnLineDefectAreaLow  = 0.0;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].fnLineDefectAreaUp  = 0.0;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].fnLineDefectLengthLow  = 0.0;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].fnLineDefectLengthUp  = 0.0;

                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nQuoCons[0] = 0;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nQuoCons[1] = 255;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nQuoR[0] = 0;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nQuoR[1] = 255;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nQuoG[0] = 0;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nQuoG[1] = 255;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nQuoB[0] = 0;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nQuoB[1] = 255;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nQuoN[0] = 0;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nQuoN[1] = 255;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nQuoRR[0] = 0;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nQuoRR[1] = 201;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nQuoGG[0] = 0;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nQuoGG[1] = 202;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nQuoBB[0] = 0;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nQuoBB[1] = 203;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nQuoRG[0] = 0;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nQuoRG[1] = 255;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nQuoRB[0] = 0;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nQuoRB[1] = 255;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nQuoGB[0] = 0;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nQuoGB[1] = 255;

                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nQuoRN[0] = 0;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nQuoRN[1] = 255;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nQuoGN[0] = 0;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nQuoGN[1] = 255;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nQuoBN[0] = 0;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nQuoBN[1] = 255;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nQuoNN[0] = 0;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nQuoNN[1] = 255;

                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nEdge= 0;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nModeType= 0;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].nConsExpandedValue= 1;

                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].delayType = 1;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].maxSenseValue = 100;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].impuritPos = 0;
                    struCnfp.struGroupIdentify[i][j][k].struSvm[m].brightLimit = 0;
                }

                /* 选圆选长算法 */
                for(int m = 0; m < MAX_CIRCLE_LONG_TOTAL; m++)
                {
                    struCnfp.struGroupIdentify[i][j][k].struCircleLong[m].nTypeIndex         = 0x20;
                    struCnfp.struGroupIdentify[i][j][k].struCircleLong[m].nArithEnable       = 0;
                    struCnfp.struGroupIdentify[i][j][k].struCircleLong[m].nDispEnable        = 0;
                    struCnfp.struGroupIdentify[i][j][k].struCircleLong[m].nVavle             = VAVLE_TYPE_1;
                    struCnfp.struGroupIdentify[i][j][k].struCircleLong[m].nCiloLow           = 0;
                    struCnfp.struGroupIdentify[i][j][k].struCircleLong[m].nCilosUp           = 0;
                    struCnfp.struGroupIdentify[i][j][k].struCircleLong[m].nMode              = 0;
                    struCnfp.struGroupIdentify[i][j][k].struCircleLong[m].nLimit             = 10;
                    struCnfp.struGroupIdentify[i][j][k].struCircleLong[m].delayType          = 1;
                }

                /* 长短算法 */
                for(int m = 0; m < MAX_LONG_SHORT_TOTAL; m++)
                {
                    struCnfp.struGroupIdentify[i][j][k].struLongShort[m].nTypeIndex         = 0x10;
                    struCnfp.struGroupIdentify[i][j][k].struLongShort[m].nArithEnable       = 0;
                    struCnfp.struGroupIdentify[i][j][k].struLongShort[m].nDispEnable        = 0;
                    struCnfp.struGroupIdentify[i][j][k].struLongShort[m].nVavle             = VAVLE_TYPE_1;
                    struCnfp.struGroupIdentify[i][j][k].struLongShort[m].nLengthLow         = 0;
                    struCnfp.struGroupIdentify[i][j][k].struLongShort[m].nLengthUp          = 0;
                    struCnfp.struGroupIdentify[i][j][k].struLongShort[m].nLengthMode        = 0;
                    struCnfp.struGroupIdentify[i][j][k].struLongShort[m].nLimit             = 10;
                    struCnfp.struGroupIdentify[i][j][k].struLongShort[m].delayType          = 1;
                }


                /* 大小算法 */
                for(int m = 0; m < MAX_BIG_SMALL_TOTAL; m++)
                {
                    struCnfp.struGroupIdentify[i][j][k].struBigSmall[m].nTypeIndex         = 0x00;
                    struCnfp.struGroupIdentify[i][j][k].struBigSmall[m].nArithEnable       = 0;
                    struCnfp.struGroupIdentify[i][j][k].struBigSmall[m].nDispEnable        = 0;
                    struCnfp.struGroupIdentify[i][j][k].struBigSmall[m].nVavle             = VAVLE_TYPE_1;
                    struCnfp.struGroupIdentify[i][j][k].struBigSmall[m].nMode              = 0;
                    struCnfp.struGroupIdentify[i][j][k].struBigSmall[m].nAreaLow           = 0;
                    struCnfp.struGroupIdentify[i][j][k].struBigSmall[m].nAreaUp            = 0;
                    struCnfp.struGroupIdentify[i][j][k].struBigSmall[m].nLimit             = 10;
                    struCnfp.struGroupIdentify[i][j][k].struBigSmall[m].delayType          =  1;
                }

                /* 饱和算法 */
                struCnfp.struGroupIdentify[i][j][k].struColorSat.nArithEnable       = 0;
                struCnfp.struGroupIdentify[i][j][k].struColorSat.nDispEnable        = 0;
                struCnfp.struGroupIdentify[i][j][k].struColorSat.nVavle              = 0;
                struCnfp.struGroupIdentify[i][j][k].struColorSat.nRow           = 4;
                struCnfp.struGroupIdentify[i][j][k].struColorSat.nColumn            = 4;
                struCnfp.struGroupIdentify[i][j][k].struColorSat.nPercent        = 12;
                struCnfp.struGroupIdentify[i][j][k].struColorSat.nMode              = 0;
                struCnfp.struGroupIdentify[i][j][k].struColorSat.nSensLow           = 0;
                struCnfp.struGroupIdentify[i][j][k].struColorSat.nSensUp            = 0;
                struCnfp.struGroupIdentify[i][j][k].struColorSat.lightLimit            = 0;
                /* 比例算法 */
                struCnfp.struGroupIdentify[i][j][k].struColorScale.nArithEnable     = 0;
                struCnfp.struGroupIdentify[i][j][k].struColorScale.nDispEnable      = 0;
                struCnfp.struGroupIdentify[i][j][k].struColorScale.nSenIdx          = 0;
                struCnfp.struGroupIdentify[i][j][k].struColorScale.nSen             = 0;
                struCnfp.struGroupIdentify[i][j][k].struColorScale.nLimit           = 0;

                /* 颜色算法 */
                struCnfp.struGroupIdentify[i][j][k].struColorHue.nArithEnable       = 0;
                struCnfp.struGroupIdentify[i][j][k].struColorHue.nDispEnable        = 0;
                struCnfp.struGroupIdentify[i][j][k].struColorHue.nHighHue           = 0;
                struCnfp.struGroupIdentify[i][j][k].struColorHue.nLowHue            = 0;
                struCnfp.struGroupIdentify[i][j][k].struColorHue.lightLimit         = 0;

                /* 霉点算法 */
                struCnfp.struGroupIdentify[i][j][k].struMildew.nArithEnable         = 0;
                struCnfp.struGroupIdentify[i][j][k].struMildew.nDispEnable          = 0;
                struCnfp.struGroupIdentify[i][j][k].struMildew.nSens                = 0;
                struCnfp.struGroupIdentify[i][j][k].struMildew.nLimit               = 0;
                struCnfp.struGroupIdentify[i][j][k].struMildew.nMark                = 0;
                struCnfp.struGroupIdentify[i][j][k].struMildew.nType               = 0;

                /* 轮廓算法 */
                struCnfp.struGroupIdentify[i][j][k].struOutline.nArithEnable        = 0;
                struCnfp.struGroupIdentify[i][j][k].struOutline.nDispEnable         = 0;
                struCnfp.struGroupIdentify[i][j][k].struOutline.nSens               = 0;
                struCnfp.struGroupIdentify[i][j][k].struOutline.nHighLimit          = 0;
                struCnfp.struGroupIdentify[i][j][k].struOutline.nLowLimit           = 0;

                /* 纹理算法 */
                struCnfp.struGroupIdentify[i][j][k].struLine.nArithEnable           = 0;
                struCnfp.struGroupIdentify[i][j][k].struLine.nDispEnable            = 0;
                struCnfp.struGroupIdentify[i][j][k].struLine.nColor                 = 0;
                struCnfp.struGroupIdentify[i][j][k].struLine.nSize                  = 0;
                struCnfp.struGroupIdentify[i][j][k].struLine.nSens1                 = 0;
                struCnfp.struGroupIdentify[i][j][k].struLine.nSens2                 = 0;

                /* 选芽算法 */
                for(int m = 0; m < MAX_BUD_TOTAL; m++)
                {
                    struCnfp.struGroupIdentify[i][j][k].struBud[m].nArithEnable            = 0;
                    struCnfp.struGroupIdentify[i][j][k].struBud[m].nDispEnable             = 0;
                    struCnfp.struGroupIdentify[i][j][k].struBud[m].nSize                   = 0;
                    struCnfp.struGroupIdentify[i][j][k].struBud[m].nLimit                  = 0;
                    struCnfp.struGroupIdentify[i][j][k].struBud[m].nSens                   = 0;
                    struCnfp.struGroupIdentify[i][j][k].struBud[m].nBlackSens              = 0;
                    struCnfp.struGroupIdentify[i][j][k].struBud[m].nYellowSens             = 0;
                }


                /* 选圆算法 */
                for(int m = 0; m < MAX_ROUND_TOTAL; m++)
                {
                    struCnfp.struGroupIdentify[i][j][k].struRound[m].nArithEnable          = 0;
                    struCnfp.struGroupIdentify[i][j][k].struRound[m].nDispEnable           = 0;
                    struCnfp.struGroupIdentify[i][j][k].struRound[m].nSelectMode           = 0;
                    struCnfp.struGroupIdentify[i][j][k].struRound[m].nSensUp               = 0;
                    struCnfp.struGroupIdentify[i][j][k].struRound[m].nSensLow              = 0;
                    struCnfp.struGroupIdentify[i][j][k].struRound[m].nLimit                = 0;
                }

                /* HSV算法 */
                for(int m = 0; m < MAX_HSV_TOTAL; m++)
                {
                    struCnfp.struGroupIdentify[i][j][k].struHsv[m].nArithEnable       = 0;
                    struCnfp.struGroupIdentify[i][j][k].struHsv[m].nDispEnable        = 0;
                    struCnfp.struGroupIdentify[i][j][k].struHsv[m].nVavle             = VAVLE_TYPE_1;
                    struCnfp.struGroupIdentify[i][j][k].struHsv[m].nRow                 = row;
                    struCnfp.struGroupIdentify[i][j][k].struHsv[m].nColumn              = col;
                    struCnfp.struGroupIdentify[i][j][k].struHsv[m].nPercent             = per;
                    struCnfp.struGroupIdentify[i][j][k].struHsv[m].nHsvColor           = 0;
                    struCnfp.struGroupIdentify[i][j][k].struHsv[m].nHsvMode            = 0;
                    struCnfp.struGroupIdentify[i][j][k].struHsv[m].nHsvSensLowH        = 0;
                    struCnfp.struGroupIdentify[i][j][k].struHsv[m].nHsvSensUpH         = 100;
                    struCnfp.struGroupIdentify[i][j][k].struHsv[m].nHsvSensLowS        = 50;
                    struCnfp.struGroupIdentify[i][j][k].struHsv[m].nHsvSensUpS         = 100;
                    struCnfp.struGroupIdentify[i][j][k].struHsv[m].nHsvSensLowV        = 0;
                    struCnfp.struGroupIdentify[i][j][k].struHsv[m].nHsvSensUpV         = 255;
                    struCnfp.struGroupIdentify[i][j][k].struHsv[m].delayType           =  1;

                    /* 传统颜色保留 */
                    struCnfp.struGroupIdentify[i][j][k].struHsv[m].struResGreyColor.nColor1       = 0;
                    struCnfp.struGroupIdentify[i][j][k].struHsv[m].struResGreyColor.nSensLow1     = 0;
                    struCnfp.struGroupIdentify[i][j][k].struHsv[m].struResGreyColor.nSensUp1      = 0;
                    struCnfp.struGroupIdentify[i][j][k].struHsv[m].struResGreyColor.nMode1        = 0;
                    struCnfp.struGroupIdentify[i][j][k].struHsv[m].struResGreyColor.nGreyResEnable    = 0;
                    struCnfp.struGroupIdentify[i][j][k].struHsv[m].struResGreyColor.nMain2        = 0;
                    struCnfp.struGroupIdentify[i][j][k].struHsv[m].struResGreyColor.nVice2        = 1;
                    struCnfp.struGroupIdentify[i][j][k].struHsv[m].struResGreyColor.nResDiffColor = 0;
                    struCnfp.struGroupIdentify[i][j][k].struHsv[m].struResGreyColor.nSensLow2     = 0;
                    struCnfp.struGroupIdentify[i][j][k].struHsv[m].struResGreyColor.nSensUp2     = 0;
                    struCnfp.struGroupIdentify[i][j][k].struHsv[m].struResGreyColor.nMode2        = 0;
                    struCnfp.struGroupIdentify[i][j][k].struHsv[m].struResGreyColor.nDiscolorResEnable    = 0;
                }

                /* 灰度算法 */
                struCnfp.struGroupIdentify[i][j][k].struColorValue.nArithEnable       = 0;
                struCnfp.struGroupIdentify[i][j][k].struColorValue.nDispEnable        = 0;
                struCnfp.struGroupIdentify[i][j][k].struColorValue.nRow               = 4;
                struCnfp.struGroupIdentify[i][j][k].struColorValue.nColumn            = 4;
                struCnfp.struGroupIdentify[i][j][k].struColorValue.nPercent           = 12;
                struCnfp.struGroupIdentify[i][j][k].struColorValue.nMode              = 0;
                struCnfp.struGroupIdentify[i][j][k].struColorValue.nSensLow           = 0;
                struCnfp.struGroupIdentify[i][j][k].struColorValue.nSensUp            = 0;

                /* 葫芦籽霉点算法 */
                struCnfp.struGroupIdentify[i][j][k].struSplMildew.nArithEnable         = 0;
                struCnfp.struGroupIdentify[i][j][k].struSplMildew.nDispEnable          = 0;
                struCnfp.struGroupIdentify[i][j][k].struSplMildew.nSens                = 0;

                /* 白沙果算法 */
                struCnfp.struGroupIdentify[i][j][k].struWhitePeanut.nArithEnable         = 0;
                struCnfp.struGroupIdentify[i][j][k].struWhitePeanut.nDispEnable          = 0;
                struCnfp.struGroupIdentify[i][j][k].struWhitePeanut.nSensUp              = 0;
                struCnfp.struGroupIdentify[i][j][k].struWhitePeanut.nSensLow             = 0;

                /* 辣椒选把算法 */
                struCnfp.struGroupIdentify[i][j][k].struPepperHandle.nArithEnable         = 0;
                struCnfp.struGroupIdentify[i][j][k].struPepperHandle.nDispEnable          = 0;
                struCnfp.struGroupIdentify[i][j][k].struPepperHandle.nSens                = 0;
                struCnfp.struGroupIdentify[i][j][k].struPepperHandle.nPercent             = 0;
                struCnfp.struGroupIdentify[i][j][k].struPepperHandle.nLimit               = 0;

                /* 压制大算法 */
                for(int m = 0; m < MAX_SUPPRESS_LARGE_TOTAL; m++)
                {
                    struCnfp.struGroupIdentify[i][j][k].struSuppressLarge[m].nArithEnable          = 0;
                    struCnfp.struGroupIdentify[i][j][k].struSuppressLarge[m].nDispEnable           = 0;
                    struCnfp.struGroupIdentify[i][j][k].struSuppressLarge[m].nSize1           = 1;
                    struCnfp.struGroupIdentify[i][j][k].struSuppressLarge[m].nSize2               = 1;
                    struCnfp.struGroupIdentify[i][j][k].struSuppressLarge[m].nPercent1              = 1;
                    struCnfp.struGroupIdentify[i][j][k].struSuppressLarge[m].nPercent2                = 1;
                    struCnfp.struGroupIdentify[i][j][k].struSuppressLarge[m].nSenIdx               = 0;
                    struCnfp.struGroupIdentify[i][j][k].struSuppressLarge[m].nTurns                = 0;
                }

                /* 小麦芽头算法 */
                struCnfp.struGroupIdentify[i][j][k].struWheatSprout.nArithEnable         = 0;
                struCnfp.struGroupIdentify[i][j][k].struWheatSprout.nDispEnable          = 0;
                struCnfp.struGroupIdentify[i][j][k].struWheatSprout.nLow                 = 60;
                struCnfp.struGroupIdentify[i][j][k].struWheatSprout.nPercent             = 20;
                struCnfp.struGroupIdentify[i][j][k].struWheatSprout.nSense               = 75;
                struCnfp.struGroupIdentify[i][j][k].struWheatSprout.nH0                  = 26;
                struCnfp.struGroupIdentify[i][j][k].struWheatSprout.nH1                  = 35;

                /* TEA算法 */
                for(int m = 0; m < MAX_TEA_ARITH_TOTAL; m++)
                {
                    struCnfp.struGroupIdentify[i][j][k].struTeaColor[m].nArithEnable          = 0;
                    struCnfp.struGroupIdentify[i][j][k].struTeaColor[m].nDispEnable           = 0;
                    struCnfp.struGroupIdentify[i][j][k].struTeaColor[m].tc                    = 0.0;
                    struCnfp.struGroupIdentify[i][j][k].struTeaColor[m].ec                    = 0.0;
                    struCnfp.struGroupIdentify[i][j][k].struTeaColor[m].ac                    = 0.0;
                    struCnfp.struGroupIdentify[i][j][k].struTeaColor[m].sense                 = 0;
                }
            }

            /* 各相机灵敏度偏置 */
            for(k = 0; k < struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal; k++)
            {
                for(m = 0; m < struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++) // 认为视的次数和吹的序号一致
                {
                    for(l = 0;l < 2;l++){
                        for(int x = 0; x < MAX_GREY_COLOR_ARITH_TOTAL; x++)
                        {
                            struCnfp.struUnitSens[i][j][m][l].greyColor[x] = 0;
                        }
                        for(int x = 0; x < MAX_SVM_ARITH_TOTAL; x++)
                        {
                            struCnfp.struUnitSens[i][j][m][l].svm[x]  = 0;
                        }
                        for(int x = 0; x < MAX_LONG_SHORT_TOTAL; x++)
                        {
                            struCnfp.struUnitSens[i][j][m][l].longShort[x]  = 0;
                        }
                        for(int x = 0; x < MAX_BUD_TOTAL; x++)
                        {
                            struCnfp.struUnitSens[i][j][m][l].bud[x]  = 0;
                        }
                        for(int x = 0; x < MAX_BIG_SMALL_TOTAL; x++)
                        {
                            struCnfp.struUnitSens[i][j][m][l].bigSmall[x]  = 0;
                        }
                        for(int x = 0; x < MAX_CIRCLE_LONG_TOTAL; x++)
                        {
                            struCnfp.struUnitSens[i][j][m][l].circleLong[x]  = 0;
                        }
                        for(int x = 0; x < MAX_ROUND_TOTAL; x++)
                        {
                            struCnfp.struUnitSens[i][j][m][l].round[x]  = 0;
                        }
                        for(int x = 0; x < MAX_HSV_TOTAL; x++)
                        {
                            struCnfp.struUnitSens[i][j][m][l].hsv[x]  = 0;
                        }
                        //                        struCnfp.struUnitSens[i][j][m][l].bigSmall  = 0;
                        //                        struCnfp.struUnitSens[i][j][m][l].circleLong  = 0;
                        struCnfp.struUnitSens[i][j][m][l].sat        = 0;
                        struCnfp.struUnitSens[i][j][m][l].hueH        = 0;
                        struCnfp.struUnitSens[i][j][m][l].hueL        = 0;
                        struCnfp.struUnitSens[i][j][m][l].scale      = 0;
                        struCnfp.struUnitSens[i][j][m][l].mildew     = 0;
                        struCnfp.struUnitSens[i][j][m][l].outline    = 0;
                        struCnfp.struUnitSens[i][j][m][l].line       = 0;
                        //                        struCnfp.struUnitSens[i][j][m][l].round      = 0;
                        struCnfp.struUnitSens[i][j][m][l].value      = 0;
                        struCnfp.struUnitSens[i][j][m][l].splMildew  = 0;
                        struCnfp.struUnitSens[i][j][m][l].whitePeanut  = 0;
                    }
                }
            }
        }
    }
}

/* 初始化剔除组参数 */
void GlobalFlow::getGroupTick()
{
    int i, j, k;

    int nEjectDuration,nEjectDuration2;

    for(i = 0; i < struCnfg.nLayerTotal ; i++)
    {
        for (j = 0; j < struCnfg.struLayerInfo[i].nVavleChainTotal; j++)
        {
            switch(struCnfg.nMachineType)
            {
            case MACHINE_VERTICAL_DM_RG:
                nEjectDuration = 20;
                nEjectDuration2 = 30;
                break;
            case MACHINE_VERTICAL_DM_ZK:
                nEjectDuration = 20;
                nEjectDuration2 = 30;
                break;
            case MACHINE_VERTICAL_ZL_CS:
            case MACHINE_VERTICAL_ZL_CV:
                nEjectDuration = 20;
                nEjectDuration2 = 30;
                break;
            default:
                nEjectDuration = 20;
                nEjectDuration2 = 30;
                break;
            }

            for(k = 0; k < struCnfg.struLayerInfo[i].stuVavleInfo[j].nTickGroupTotal; k++)
            {
                struCnfp.struGroupTick[i][j][k].nEjectDuration       = nEjectDuration;
                struCnfp.struGroupTick[i][j][k].nEjectDuration2       = nEjectDuration2;
                struCnfp.struGroupTick[i][j][k].nWaitTime            = 0;
                struCnfp.struGroupTick[i][j][k].nTailTime            = 0;
                struCnfp.struGroupTick[i][j][k].nFrontRearMode       = 0;
                struCnfp.struGroupTick[i][j][k].nEjwidth             = 1;
            }
        }

        /* 喷阀保护-区分次数 */
        struCnfp.struGroupTick[i][j][0].nEjectTimesLimit = 250;
        struCnfp.struGroupTick[i][j][1].nEjectTimesLimit = 300;
        struCnfp.struGroupTick[i][j][2].nEjectTimesLimit = 300;
        struCnfp.struGroupTick[i][j][2].nEjectTimesLimit = 300;
    }
}

/* 初始化算法名称 */
void  GlobalFlow::getArithmeticName()
{
    g_Runtime().sArithmeticName[ARITH_GREY_A]           = g_myLan().arith_gray_a;
    g_Runtime().sArithmeticName[ARITH_GREY_B]           = g_myLan().arith_gray_b;
    g_Runtime().sArithmeticName[ARITH_GREY_C]           = g_myLan().arith_gray_c;
    g_Runtime().sArithmeticName[ARITH_GREY_D]           = g_myLan().arith_gray_d;
    g_Runtime().sArithmeticName[ARITH_DISCOLOR_A]       = g_myLan().arith_discolor_a;
    g_Runtime().sArithmeticName[ARITH_DISCOLOR_B]       = g_myLan().arith_discolor_b;
    g_Runtime().sArithmeticName[ARITH_CROSS]            = g_myLan().arith_cross;
    g_Runtime().sArithmeticName[ARITH_SVM_A]            = "SVMA";
    g_Runtime().sArithmeticName[ARITH_SVM_B]            = "SVMB";
    g_Runtime().sArithmeticName[ARITH_SVM_C]            = "SVMC";
    g_Runtime().sArithmeticName[ARITH_SVM_D]            = "SVMD";
    g_Runtime().sArithmeticName[ARITH_SHAPE_SVM_A]      = g_myLan().shape+g_myLan().arith_svm_a;
    g_Runtime().sArithmeticName[ARITH_SHAPE_SVM_B]      = g_myLan().shape+g_myLan().arith_svm_b;
    g_Runtime().sArithmeticName[ARITH_SHAPE_SVM_C]      = g_myLan().shape+g_myLan().arith_svm_c;
    g_Runtime().sArithmeticName[ARITH_BIG_SMALL]        = g_myLan().arith_big_small;
    g_Runtime().sArithmeticName[ARITH_BIG_SMALL_B]      = g_myLan().arith_big_small;
    g_Runtime().sArithmeticName[ARITH_LONG_SHORT]       = g_myLan().arith_long_short;
    g_Runtime().sArithmeticName[ARITH_LONG_SHORT_B]     = g_myLan().arith_long_short_b;
    g_Runtime().sArithmeticName[ARITH_CIRCLE_LONG]      = g_myLan().arith_circle_long;
    g_Runtime().sArithmeticName[ARITH_CIRCLE_LONG_B]      = g_myLan().arith_circle_long;
    g_Runtime().sArithmeticName[ARITH_SAT]              = g_myLan().arith_sat;
    g_Runtime().sArithmeticName[ARITH_HUE]              = g_myLan().arith_hue;
    g_Runtime().sArithmeticName[ARITH_SCALE]            = g_myLan().arith_scale;
    g_Runtime().sArithmeticName[ARITH_MILDEW]           = g_myLan().arith_mildew;
    g_Runtime().sArithmeticName[ARITH_OUTLINE]          = g_myLan().arith_outline;
    g_Runtime().sArithmeticName[ARITH_LINE]             = g_myLan().arith_line;
    g_Runtime().sArithmeticName[ARITH_BUD]              = g_myLan().arith_bud;
    g_Runtime().sArithmeticName[ARITH_BUD_B]            = g_myLan().arith_bud_b;
    g_Runtime().sArithmeticName[ARITH_ROUND]            = g_myLan().arith_round;
    g_Runtime().sArithmeticName[ARITH_ROUND_B]            = g_myLan().arith_round;
    g_Runtime().sArithmeticName[ARITH_VALUE]            = g_myLan().arith_value;
    g_Runtime().sArithmeticName[ARITH_SPL_MILDEW]       = g_myLan().arith_spl_mildew;
    g_Runtime().sArithmeticName[ARITH_W_PEANUT]         = g_myLan().arith_white_peanut;
    g_Runtime().sArithmeticName[ARITH_PEPPER_HANDLE]    = g_myLan().arith_pepper_handle;
    g_Runtime().sArithmeticName[ARITH_SUPPRESS_LARGE_A]    = g_myLan().arith_suppress_large+"A";
    g_Runtime().sArithmeticName[ARITH_SUPPRESS_LARGE_B]    = g_myLan().arith_suppress_large+"B";
    g_Runtime().sArithmeticName[ARITH_SUPPRESS_LARGE_C]    = g_myLan().arith_suppress_large+"C";
    g_Runtime().sArithmeticName[ARITH_WHEAT_SPROUT]     = g_myLan().arith_wheat_sprout;
    g_Runtime().sArithmeticName[TEA_ARITH_COLOR]       = "TEA_A";
    g_Runtime().sArithmeticName[TEA_ARITH_COLOR_2]     = "TEA_B";
    g_Runtime().sArithmeticName[TEA_ARITH_COLOR_3]     = "TEA_C";
    g_Runtime().sArithmeticName[TEA_ARITH_COLOR_4]     = "TEA_D";
    g_Runtime().sArithmeticName[TEA_ARITH_COLOR_5]     = "TEA_E";

}

/* 获取色选方案参数 */
void GlobalFlow::getProfile()
{
    initProfile();
    if(struCnfg.nProfile >=0){
        QString fileName = QString("/sdcard/cnf/%1").arg(QString().fromLatin1(struCnfg.stuProfileIndex[struCnfg.nProfile].sProfileName));
        if (struCnfs.value != SCHEME_FILE_ERROR) {
            if (QFileInfo(fileName).exists() && QFileInfo(fileName).size() > 0)  {
                if (!getProfileSetting(fileName)) {
                    qDebug("getProfile(): can not get profile parameters, using default");
                    goto end;
                }
                g_Runtime().copyFileToPath(fileName, fileName+"_2",true);
            }
        }
        else {
            if (QFileInfo(fileName+"_2").exists() && QFileInfo(fileName+"_2").size() > 0) {
                if (!getProfileSetting(fileName+"_2")) {
                    qDebug("getProfile(): can not get profile parameters, using default");
                    goto end;
                }
                g_Runtime().copyFileToPath(fileName+"_2", fileName,true);
            }
        }
        memcpy(&_t_struCnfp, &struCnfp, sizeof(struCnfProfile));
    }else{
        logger()->info("getProfile(): can not get profile parameters, using default");
    }

    getFixedParamsByMachine();

end:
    if(struCnfg.camParaInCnf){
        memcpy(struCnfg.struLayerInfo[0].stuCameraInfo,struCnfp.stuCameraInfo,sizeof(struCnfp.stuCameraInfo));
    }
    return;
}

void GlobalFlow::getFixedParamsByMachine()
{
    if((struCnfg.nChuteTotal == 1)||(struCnfg.nMachineSpliceType == MACHINE_SPLICE_TYPE_BIG))
    {
        struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeedSetType = 0;
    }
    if(struCnfg.nEnableDispArithSetByTimes == 0)
    {
        struCnfp.nMatCopySetMode = MAT_COPY_ALL_ALONE;  //恢复成全部独立设置
    }
    /* 用合板机器，一二次相同*/
    //    if((struCnfg.struLayerInfo[struGsh.nLayer].nSplice == 1)&&(struCnfg.nChuteTotal == 1))
    //    {
    //        struCnfp.nMatCopySetMode = MAT_COPY_SECOND_ALIKE_FIRST;
    //    }
    //    else
    //    {
    //        struCnfp.nMatCopySetMode = MAT_COPY_ALL_ALONE;  //恢复成全部独立设置
    //    }
}

bool GlobalFlow::saveProfile()
{
    if(struCnfg.nProfileTotal == 0){
        QString smaterialName = g_myLan().ids_new;
        QByteArray sMaterialArr = smaterialName.toUtf8();

        memset(struCnfg.stuProfileIndex[struCnfg.nProfileTotal].sMaterialName, 0,
                sizeof(struCnfg.stuProfileIndex[struCnfg.nProfileTotal].sMaterialName));
        memcpy(struCnfg.stuProfileIndex[struCnfg.nProfileTotal].sMaterialName,
                sMaterialArr.data(), sMaterialArr.length());
        QDateTime time = QDateTime::currentDateTime();
        QString tmpTime = time.toString("yyyyMMddhhmmsszzz");
        QByteArray array = tmpTime.toUtf8();
        const char *strName = array.data();

        memset(struCnfg.stuProfileIndex[struCnfg.nProfileTotal].sProfileName, 0, sizeof(struCnfg.stuProfileIndex[struCnfg.nProfileTotal].sProfileName));
        memcpy(struCnfg.stuProfileIndex[struCnfg.nProfileTotal].sProfileName, strName, array.length());
        struCnfg.nProfileTotal= 1;
    }
    if(struCnfg.nProfile == -1)
        struCnfg.nProfile = 0;
    QByteArray filename(struCnfg.stuProfileIndex[struCnfg.nProfile].sProfileName,strlen(struCnfg.stuProfileIndex[struCnfg.nProfile].sProfileName));
    QString fileNameStr =QString::fromUtf8(filename);
    memcpy(struCnfp.stuCameraInfo,struCnfg.struLayerInfo[0].stuCameraInfo,sizeof(struCnfp.stuCameraInfo));

    if(saveProfile(fileNameStr,struCnfp)){
        memcpy(&_t_struCnfp,&struCnfp,sizeof(struCnfp));
        return true;
    }
    return false;
}

/* 保存色选方案参数 */
bool GlobalFlow::saveProfile(const QString& fileName, const struCnfProfile &cnfProfile)
{
    int i;
    QString  fileDir = CNF_MODE_DIR;
    QDir dir(fileDir);
    if(!dir.exists())
    {
        bool ok = dir.mkdir(fileDir);//只创建一级子目录，即必须保证上级目录存在
    }

    QString filePath = QString("%1%2").arg(CNF_MODE_DIR).arg(fileName);
    saveParamsFileStaus(SCHEME_FILE_ERROR);
    for (i = 0; i < 3; i++) {
        if (saveProfileSetting(filePath,cnfProfile)) {
            saveParamsFileStaus(ALL_PARAMS_FILE_OK);
            break;
        }
    }

    if (i == 3)
        return false;

    if (QFileInfo(filePath).exists() && QFileInfo(filePath).size() > 0) {
        bool backfile = g_Runtime().copyFileToPath(filePath, filePath+"_2",true);
        return backfile;
    }

    return true;
}

bool GlobalFlow::getGlobalSetting(const QString &fileName)
{
    Json::Value root;
    if(!CMyJson::LoadJsonFile(fileName.toUtf8().data(),root)){
        return false;
    }
    return JsonDataConvert::JsonToCnfGlobal(root,struCnfg);
}

bool GlobalFlow::saveGlobalSetting(const QString &fileName)
{
    Json::Value root;
    JsonDataConvert::CnfGlobalToJson(struCnfg, root);

    logger()->info("save json");
    if(!CMyJson::SaveJsonFile(fileName.toUtf8().data(), root))
    {
        //保存json文件失败，返回false
        return false;
    }
    memcpy(&_t_struCnfg,&struCnfg,sizeof(struCnfg));
    return true;
}

bool GlobalFlow::getProfileSetting(const QString &fileName)
{
    Json::Value profile;
    if(!CMyJson::LoadJsonFile(fileName.toUtf8().data(),profile)){
        return false;
    }
    return JsonDataConvert::JsonTostruCnfProfile(profile,struCnfp);
}

bool GlobalFlow::saveProfileSetting(const QString &fileName,const struCnfProfile &cnfProfile)
{
    Json::Value profile;
    if(!JsonDataConvert::StruCnfProfileToJson(cnfProfile, profile))
        return false;

    logger()->info("save jsonprofile");
    if(!CMyJson::SaveJsonFile(fileName.toUtf8().data(), profile))
    {
        //保存json文件失败，返回false

        return false;
    }
    return true;
}

void GlobalFlow::createProfile()
{
    initProfile();
    if(g_Runtime().m_expModeName.compare("common") == 0)
    {
        memcpy(&_t_struCnfp, &struCnfp, sizeof(struCnfProfile));
        return;
    }
    QString fileName = QString("/sdcard/cnf/prefab/%1").arg(g_Runtime().m_expModeName);
    if (struCnfs.value != SCHEME_FILE_ERROR) {
        if (QFileInfo(fileName).exists() && QFileInfo(fileName).size() > 0)  {
            if (!getProfileSetting(fileName)) {
                memcpy(&_t_struCnfp, &struCnfp, sizeof(struCnfProfile));
                qDebug("getProfile(): can not get profile parameters, using default");
                return;
            }
        }
        memcpy(&_t_struCnfp, &struCnfp, sizeof(struCnfProfile));
    }
}

/* 根据相机编号获取所在组的编号 */
int  GlobalFlow::getIdentifyGroupIndex(int nLayer, int nView, int nUnit){
    for(int i = 0;i < struCnfg.struLayerInfo[nLayer].stuViewInfo[nView].nIdentifyGroupTotal;i++){
        for(int j = 0;j < struCnfg.struLayerInfo[nLayer].stuViewInfo[nView].stuIdenGupInfo[i].nGroupUnitTotal;j++){
            if(nUnit == struCnfg.struLayerInfo[nLayer].stuViewInfo[nView].stuIdenGupInfo[i].nUnitGupId[j]){
                return i;
            }
        }
    }
    return -1;
}

/* 获取识别组每个组组名 */
void GlobalFlow::getIdentifyGroupName()
{
    int i, j;
    QString sLevelName[3];

    switch(struCnfg.nMachineType) {
    case MACHINE_VERTICAL_DM_RG:
    case MACHINE_VERTICAL_ZL_CS:
    case MACHINE_VERTICAL_ZL_CV:
        sLevelName[0] = "";
        break;
    case MACHINE_VERTICAL_DM_ZK:
        sLevelName[0] = "";
        break;
    default:
        break;
    }
}

void GlobalFlow::getLocalIpcParams()
{
    int i, j, k, m;

    for(i = 0; i < MAX_LAYER; i++)
    {
        for(j = 0; j < MAX_VIEW; j++)
        {
            struCnfp.nIpcDelayBufRowCount[i][j] = 400;
        }
    }

    for(i = 0; i < MAX_VIEW; i++)
    {

        for(j =0; j < MAX_GROUP_IDENTIFY;j++)
            struCnfp.ipcModelClassCount[i][j] =5;// MAX_IPC_MODEL_CLASS;

        struCnfp.nIpcRoiHeight[i] = 0;
        struCnfp.nIpcRoiWidth[i]  = 0;
        struCnfp.nIpcRoiRepeat[i] = 0;
        struCnfp.nIpcRoiRepeat2[i] = 45;
        struCnfp.nIpcRoiRepeatf[i] = 0;
        struCnfp.imageEnahnce[i] = 0;
    }

    //    for(i = 0; i < MAX_LAYER; i++)
    //    {
    //        for(j = 0; j < MAX_VIEW; j++)
    //        {
    //            struCnfp.struCameraIpcClassParams[i][j].lengthLimit = 0;

    //            for(m = 0; m < MAX_IPC_MODEL_CLASS; m++)
    //            {
    //                struCnfp.struCameraIpcClassParams[i][j].classEnable[m] = 0;
    //                struCnfp.struCameraIpcClassParams[i][j].classSens[m] = 50;
    //                struCnfp.struCameraIpcClassParams[i][j].classThreshold[m] = 30;
    //                struCnfp.struCameraIpcClassParams[i][j].classLabelBox[m] = 10;
    //                struCnfp.struCameraIpcClassParams[i][j].classTickMode[m] = 1;
    //                struCnfp.struCameraIpcClassParams[i][j].classFixMode[m] = 0;
    //            }

    //        }
    //    }
    for(j = 0; j < MAX_VIEW; j++)
    {
        for(k = 0; k < MAX_GROUP_IDENTIFY; k++)
        {
            struCnfp.struCameraIpcClassGroupParams[j][k].lengthLimit = 0;

            for(m = 0; m < MAX_IPC_MODEL_CLASS; m++)
            {
                struCnfp.struCameraIpcClassGroupParams[j][k].classEnable[m] = 0;
                struCnfp.struCameraIpcClassGroupParams[j][k].classSens[m] = 50;
                struCnfp.struCameraIpcClassGroupParams[j][k].classThreshold[m] = 30;
                struCnfp.struCameraIpcClassGroupParams[j][k].classLabelBox[m] = 2;
                struCnfp.struCameraIpcClassGroupParams[j][k].classLabelBox2[m] = 10;
                struCnfp.struCameraIpcClassGroupParams[j][k].classTickMode[m] = 0;
                struCnfp.struCameraIpcClassGroupParams[j][k].classFixMode[m] = 0;
                struCnfp.struCameraIpcClassGroupParams[j][k].classFixMode2[m] = 0;
                struCnfp.struCameraIpcClassGroupParams[j][k].classMode[m] = 0;
                struCnfp.struCameraIpcClassGroupParams[j][k].classDelayMode[m] = 0;
                struCnfp.struCameraIpcClassGroupParams[j][k].classHWRatio[m] = 50;
                struCnfp.struCameraIpcClassGroupParams[j][k].classPosAdjust[m] = 0;
                struCnfp.struCameraIpcClassGroupParams[j][k].classBD[m] = 0;
                struCnfp.struCameraIpcClassGroupParams[j][k].classBX[m] = 0;
                struCnfp.struCameraIpcClassGroupParams[j][k].classANDEnable[m] = 0;
            }
        }
    }

    struCnfp.nHasSaveModel = 0;
    struCnfp.nIpcBadMatAlarmCount = 1;
    struCnfp.nIpcHasMatPixelPercert = 5;
    struCnfp.nDualLayerModelSame = 1;
    struCnfp.nIpcCameraClassSameMode = 0;
    struCnfp.bCaliGainEnable = 0;
    struCnfp.bCaliBiasEnable = 0;
    struCnfp.nIpcStatisticsBigclass = 0;
    for(int i = 0; i< MAX_IPC_MODEL_CLASS ; i++)
    {
        struCnfp.nIpcStatisticsclassId[i] = 0;
        struCnfp.nIpcStatisticsbigclassCoeff[i] = 100;
        struCnfp.bstaticClassAlarm[i] = 0;
    }
    memset(struCnfp.sCurModelName, 0, MAX_VIEW*MAX_IPC_MODEL_NAME_LENGTH*MAX_GROUP_IDENTIFY);
    memset(struCnfp.sCurModelDate, 0, MAX_VIEW*MAX_IPC_MODEL_DATE_LENGTH*MAX_GROUP_IDENTIFY);
}

/* 获取剔除组每个组组名 */
void GlobalFlow::getTickGroupName()
{
    int i, j;
    QString sLevelName[3];

    switch(struCnfg.nMachineType) {
    case MACHINE_VERTICAL_DM_RG:
    case MACHINE_VERTICAL_ZL_CS:
    case MACHINE_VERTICAL_ZL_CV:
        sLevelName[0] = "";
        break;
    case MACHINE_VERTICAL_DM_ZK:
        sLevelName[0] = "";
        break;
    default:
        break;
    }
}

void GlobalFlow::getLampName()
{
    g_Runtime().sLamp[0] = g_myLan().lamp_rear_bg_red;
    g_Runtime().sLamp[1] = g_myLan().lamp_rear_bg_green;
    g_Runtime().sLamp[2] = g_myLan().lamp_rear_bg_blue;
    g_Runtime().sLamp[3] = g_myLan().lamp_front_bg_red;
    g_Runtime().sLamp[4] = g_myLan().lamp_front_bg_green;
    g_Runtime().sLamp[5] = g_myLan().lamp_front_bg_blue;
}

void GlobalFlow::initChannelPixelCompensate()
{
    int i;

    for (i = 0; i < struCnfg.struLayerInfo[struGsh.nLayer].nEjectorsPerChute; i++) {
        struGsh.fPixelCompensate[i] = 0.0;
    }
}

/* 初始化全局不需要保存的参数*/
void GlobalFlow::initGsh()
{
    int i, j, k;

    memset(&struGsh, 0, sizeof(struShare));

    struGsh.nPageSm = 0;
    struGsh.nCounter = 0;
    struGsh.nRunCounter = 0;
    struGsh.nStaticCounter = 0;
    struGsh.bLostFpgaBoard = 0;

    struGsh.bUpdateState        = 0;
    struGsh.bSortStart           = 0;
    struGsh.bStatEject          = 0;
    for (i = 0; i < MAX_LAYER; i++) {
        struGsh.bStatBelt[i]           = 0;
    }
    struGsh.bKeepAliveType      = 0;

    struGsh.bFlagAutowipe       = 1;
    struGsh.bFlagManuwipe       = 0;
    struGsh.bFlagWiping       = 0;
    struGsh.bFlagRowing         = 1;
    struGsh.bFlagMachinetest    = 0;

    struGsh.bFlagPowerOff = 0;
    struGsh.bFlagMqttConnect    = 0;
    struGsh.bFlagGettingStatistic = 0;

    struGsh.bCaliGainEnable         = 1;
    struGsh.bCaliBiasEnable         = 1;
    struGsh.bCaliDotReviseEnable         = 0;

    struGsh.bAlarmEjector       = 0;
    struGsh.bAlarmLamp          = 0;
    struGsh.bAlarmBlower        = 0;
    struGsh.bAlarmStatus        = 0;
    struGsh.curAlarmCount       = 0;
    struGsh.bAlarmValid         = 0;

    struGsh.nLayer              = 0;
    struGsh.nView               = 0;
    struGsh.nVavle               = 0;
    struGsh.nUnit               = 0;

    struGsh.bFlagFirstPwdValid  = 0;
    struGsh.bFlagSecondPwdValid = 0;

    struGsh.nMatGroup = 0;

    struGsh.bAlarmLastFeedoff = 0;
    struGsh.bAlarmColorVoiceLast = 0;

    QSettings setting(CFG_APPSet,QSettings::IniFormat);
    struGsh.serialNum = setting.value("portName","2").toInt();
    struGsh.serialNum_2 = setting.value("portName_2","1").toInt();

    for(i = 0; i < MAX_LAYER; i++)
    {
        for(j = 0; j < MAX_VIEW; j++)
        {
            for(k = 0; k < MAX_VIEW_UNIT; k++)
            {
                struGsh.bFlagBrightCali[i][j][k][0] = -1;
                struGsh.bFlagBrightCali[i][j][k][1] = -1;
                struGsh.bFlagDarkCali[i][j][k][0] = -1;
                struGsh.bFlagDarkCali[i][j][k][1] = -1;
            }
        }
    }
    for(i = 0; i < MAX_LAYER; i++)
    {
        for(j = 0; j < CTRL_ALARM_MAX_FEED_SENSOR; j++)
        {
            struGsh.struMachineStatus[i].bAlarmFeedLevel[j] = 0;        //默认值
        }

        for(j = 0; j < CTRL_ALARM_MAX_TEMP_SENSOR; j++)
        {
            struGsh.struMachineStatus[i].nTempSensorValue[j] = 0.0;
        }

        struGsh.struMachineStatus[i].nPressureValue = 0;
        //        struGsh.struMachineStatus[i].bAlarmPressureLow = 0;

        for(j = 0; j < CTRL_ALARM_MAX_FEED_CURRENT; j++)
        {
            struGsh.struMachineStatus[i].bFeederCurrentOver[j] = 0;
        }
        struGsh.struMachineStatus[i].nMachineUseAir = 0;
    }
    struGsh.b_getFlow =false;

    struGsh.nImageCapMode = 0;
    struGsh.nSensCoefficient = 0;

    struGsh.delayPerRow = 1;  //每行损耗1/10 ms

    getIdentifyGroupName();             // 获取识别组界面显示名称
    getTickGroupName();                 // 获取剔除组界面显示名称
    getLampName();                      // 获取灯控名称

    initChannelPixelCompensate();       // 获取镜头畸变补偿系数

    for (i = 0; i < IMAGE_MAX; i++) {
        struGsh.nIntelSampColor[i] = 1;
        struGsh.nIntelSampMode[i]  = 0;
        struGsh.nIntelSampSens[i]  = 255;
    }

    struGsh.nIntelSampBurst = 0;
    struGsh.nIntelSampAi    = 0;
    struGsh.nIntelSampInf   = 0;
    struGsh.nAuthenticationLevel = AUTHENTICATION_LEVEL_OPERATOR;

    struGsh.nBacklightStat = 1;
    setTsBackLight(1);
    struGsh.nBacklightCounter = 0;

    struGsh.nPixelBegin = 40;
    struGsh.nPixelEnd   = 2000;

    struGsh.focusAveBeginPixel   = 50;
    struGsh.focusAveEndPixel   = 100;
    struGsh.focusAveBeginPixel2   = 50;
    struGsh.focusAveEndPixel2   = 100;

    struGsh.nImageIndex = 0;
    struGsh.ageTestEnable = 0;
    struGsh.ageTestFre = 0;
    for(i = 0; i < MAX_FEEDER; i++)
    {
        struGsh.ageTestChuteEnable[i] = 0;
    }

    /* 智能清灰相关 */
    struGsh.nCurWiperLeftOrRight = struCnfg.struIntelWipe.wiperDefaultPosi;
    struGsh.nCurWiperInChute = 0;
    struGsh.nIntelWipeTotalWaitTime = 0;
    for(i = 0; i < MAX_LAYER; i++)
    {
        for(j = 0; j < MAX_VIEW; j++)
        {
            for(k = 0; k < MAX_VIEW_UNIT; k++)
            {
                struGsh.struCameraStatus[i][j][k].nWiperPosition = 0;
                struGsh.struCameraStatus[i][j][k].nWiperGetWidth = 0;
                struGsh.struCameraStatus[i][j][k].nCameraSignalAve = 0;
                struGsh.struCameraStatus[i][j][k].nbgSignalAve = 0;
                struGsh.struCameraStatus[i][j][k].nTempSensorValue = 0;
            }
        }
    }

    struGsh.nImageCapTimeout = 25;             //图像采集超时
    struGsh.nImageCapDelayTime = 800;
    struGsh.nImageCapTrigValue = 0;                //图像触发阈值
    struGsh.bCorrectImage = 0;

    //    struGsh.nImageCapTrigPixelBegin = struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nChannelBegin[struGsh.nUnit];
    //    struGsh.nImageCapTrigPixelEnd   = struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nChannelEnd[struGsh.nUnit];

    struGsh.nImageCapTrigPixelBegin = struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nChannelBegin[struGsh.nUnit];
    struGsh.nImageCapTrigPixelEnd   = 250;

    struGsh.nVavleTestIsRun = 0;
    struGsh.nVersionCaptureIsRun = 0;
    struGsh.nImageCaptureIsRun = 0;
    struGsh.ntrainComplete = false;
    struGsh.nSvmExpandValue= 1;
    struGsh.nSvmSimulateSens = 100;
    struGsh.nSvmRow = 8;
    struGsh.nSvmPercent = 15;
    struGsh.nSvmTypeIndex = 0;
    struGsh.nSvmBrightLimit = 0;
    struGsh.nRemoteImageCaptureIsRun = 0;

    struGsh.nRemoteSeriesBlowCount = 0;
    /*--------------------------------------------------------------*/

    struGsh.m_bAutoStudy[0] = false;
    struGsh.m_bAutoStudy[1] = false;
    struGsh.m_bManulStudy[0] = true;
    struGsh.m_bManulStudy[1] = false;
    struGsh.nIrCurRedPointIndex = 1;
    struGsh.nIrCurGreenPointIndex = 1;

    for(int i = 0;i<4;i++){
        struGsh.svmThreshold[i][0] = 0;
        struGsh.svmThreshold[i][1] = 255;
        struGsh.svmThreshold[i][2] = 0;
        struGsh.svmThreshold[i][3] = 255;
    }
    for(int i =0;i<6;i++){
        struGsh.svmDisThreshold[i][0] = 0.0;
        struGsh.svmDisThreshold[i][1] = 100.0;
        struGsh.svmDisThreshold[i][2] = 0.0;
        struGsh.svmDisThreshold[i][3] = 100.0;
    }
    struGsh.svmSample[0] = 0;
    struGsh.svmSample[1] = 0;
    struGsh.m_bGoodBadDiff = 1;
    struGsh.nSignalDispType = 0;
    struGsh.nSignalDispColor = 0;

    struGsh.debugMode = 0;
    struGsh.debugModeIpc = 0;
    struGsh.bRunMode = RUNMODE_USER;

#ifdef Q_OS_WIN
    struGsh.debugMode = 1;
    struGsh.debugModeIpc = 1;
    struGsh.bRunMode = RUNMODE_FACTORY;
#else
    QSettings dbsetting(DEBUG_MODE_Set,QSettings::IniFormat);
    int mode = dbsetting.value("debugmode",0).toInt();
    if(mode == 1){
        struGsh.debugMode = 1;
        struGsh.debugModeIpc = 1;
        struGsh.bRunMode = RUNMODE_FACTORY;
    }
#endif
    if(CUSTOM == CUSTOM_YANSI)
        struGsh.bRunMode = RUNMODE_ENGINEER;

    struGsh.allArithParamsResetCnts = 0;
    struGsh.bNetContinuousCapture = 0;

    struGsh.nValveBlowCaptureIsRun = 0;
    struGsh.bConveyorOnOff = false;
    struGsh.bColorRestoreEnable = 0;
    struGsh.isInIpcClassPage = 0;
    struGsh.isInModelchangePage = 0;
    struGsh.imgCaptureCnt = 1;
    struGsh.cornLastIndex = 0;
    struGsh.nIpcLoadCnt = 0;

    for(int i = 0; i < MAX_VIEW;i++){
        for(int j = 0; j < MAX_VIEW_UNIT;j++){
            for(int k = 0; k < 100;k++){
                struGsh.cornStatics[i][j][k] = 0;
            }
        }
    }

    /* 启动模式暂时不用，强制为1 20220906*/
    for(int i = 0; i < struCnfg.nLayerTotal; i++)
    {

        for(j = 0; j < struCnfg.struLayerInfo[i].nVavleChainTotal; j++)
        {
            struCnfg.nVavleBoardMode[i][j] = 1;    // 每吹的模式默认都是1
        }
    }
    struGsh.isClearMode = 0;
    struGsh.autoFeedEnabled = false;
    struGsh.isRadarStateAlarm = false;
    switch(struCnfg.stuFeedAutoCtrl.autoFeedMode)
    {
    case AUTO_FEED_MODE_0:
        struGsh.curBalanceStateByLevel = STAT_LEVEL_BALANCE;
        break;
    case AUTO_FEED_MODE_1:
        struGsh.curBalanceStateByLevel = STAT_LEVEL_BALANCE_MODE1;
        break;
    case AUTO_FEED_MODE_2:
        struGsh.curBalanceStateByLevel = STAT_LEVEL_BALANCE_MODE2;
        break;
    case AUTO_FEED_MODE_3:
        struGsh.curBalanceStateByLevel = STAT_LEVEL_BALANCE_MODE3;
        break;
    }

    for(int i=0;i<2;i++)
    {
        struGsh.nIrCurPointIndex[i] = 1;
    }
    for(i = 0; i <MAX_LAYER; i++)
    {
        for(j = 0; j < MAX_VIEW; j++)
        {
            for(int m = 0; m < MAX_VIEW_UNIT; m++)
            {
                struGsh.struVer.sUnitbefore[i][j][m] = -1;
            }
        }
    }

    getMachineConfig();

}

void GlobalFlow::getSetting()
{
    QSettings setting(CFG_APPSet,QSettings::IniFormat);
    struGsh.poweroff_status =  setting.value("poweroffstatus","0").toInt();
    struGsh.backCnf_status =  setting.value("backCnf_status","0").toInt();
    setting.beginGroup("eth1");
    ipForInterface =  setting.value("ipStr","").toString();
    QString maskStr1 = setting.value("maskStr","").toString();
    QString gatewayStr1 = setting.value("gatewayStr","162.254.129.1").toString();
    setting.endGroup();

#ifdef Q_OS_UNIX
    if(!ipForInterface.isEmpty()){
        setting.beginGroup("eth0");
        ipForFastNetwork =  setting.value("ipStr","").toString();
        QString maskStr0 = setting.value("maskStr","").toString();
        QString gatewayStr0 = setting.value("gatewayStr","192.168.1.1").toString();
        setting.endGroup();

        QProcess process;
        if(!ipForFastNetwork.isEmpty()){
            process.execute("ifconfig eth0 down");
            process.execute("ifconfig eth0 "+ipForFastNetwork+" netmask "+maskStr0 +" up");

            QString str1= "route add default gw "+gatewayStr0;
            const char* cmd1 = QString(str1).toLocal8Bit().constData();
            system(cmd1);

            QString str2 = "echo nameserver "+ gatewayStr0 +" >/etc/resolv.conf";
            const char* cmd2 = QString(str2).toLocal8Bit().constData();
            system(cmd2);

        }
        process.execute("ifconfig eth1 down");
        process.execute("ifconfig eth1 "+ipForInterface+" netmask "+maskStr1 +" up");

    }
    else{
        setting.beginGroup("eth0");
        QString str0 =  setting.value("ipStr","").toString();
        QString maskStr0 = setting.value("maskStr","").toString();
        QString gatewayStr0 = setting.value("gatewayStr","162.254.129.1").toString();
        setting.endGroup();

        if(str0 == "")
        {
            str0 = "192.168.1.100";
        }
        if(maskStr0 == "")
        {
            maskStr0 = "255.255.255.0";
        }
        if(gatewayStr0 == "")
        {
            gatewayStr0 = "192.168.1.1";
        }

        QProcess process;
        process.execute("ifconfig eth0 down");
        process.execute("ifconfig eth0 "+str0+" netmask "+maskStr0 +" up");
    }
    //    imgHeight = setting.value("imgheight",IMAGE_HEIGHT_NOT_DP).toInt();
#endif

    QDir dir(CNF_MODE_DIR);
    dir.setFilter(QDir::Files);
    QFileInfoList list = dir.entryInfoList();
    (list.size() == 0) ? startWithoutCnf = 1 : startWithoutCnf = 0;


}

/***********************************************************************************************************************
 *
                                       下面是全局流程定义函数
 *
 ***********************************************************************************************************************/
/* 重置机型配置 */
void GlobalFlow::resetMachineConfigRG()
{
    int i, j;
    int board;
    char data[SEND_PACKET_DATA_SIZE];

    /* 控制板链数 */
    memset(data, 0, SEND_PACKET_DATA_SIZE);
    data[0] = struCnfg.nCtrlBoardTotal;
    data[6] = struCnfg.nValveSite;
    mySerial.pushCom1CmdData(CMD_ALL_MACHINE_CONFIG, 0, BOARD_TYPE_CONTROL, BOARD_TYPE_ALL_IN_CHAIN, data, 3);

    /* 灯控板链数 */
    memset(data, 0, SEND_PACKET_DATA_SIZE);
    data[0] = struCnfg.stuLayerLampCfg.nLampBoardTotal;
    data[6] = struCnfg.nValveSite;
    mySerial.pushCom1CmdData(CMD_ALL_MACHINE_CONFIG, 0, BOARD_TYPE_LAMP, BOARD_TYPE_ALL_IN_CHAIN, data, 3);

    /* 接口板接收每层的通道数、每个通道的喷嘴数 */
    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        memset(data, 0, SEND_PACKET_DATA_SIZE);
        data[0] = struCnfg.nChuteTotal;
        data[13] = struCnfg.nValveDefend;

        for(j = 0; j < MAX_VIEW_CHIAN; j++)
        {
            if(struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]  )
                data[j+1] = 1|(struCnfp.nSensCmdSpeed[i][j]<<1);

            if(struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j+MAX_VIEW_CHIAN])
            {
                data[j+1] = 1|(struCnfp.nSensCmdSpeed[i][j+MAX_VIEW_CHIAN]<<1);
            }
        }
        data[5] = struCnfg.struLayerInfo[i].nEjectorsPerChute;                 // 每视单喷阀板对应的喷嘴数
        data[6] = struCnfg.nValveSite;                       // 每视单喷阀板对应的喷嘴数
        if(struCnfg.struLayerInfo[i].nSplice == 1 )
        {
            data[7] = struCnfg.struLayerInfo[i].nSpliceChuteAddr;                                  //
            data[8] = struCnfg.struLayerInfo[i].nSpliceType;                                    //
            //data[8] =0;
        }

        /* 接口板链数 */
        mySerial.pushCom1CmdData(CMD_ALL_MACHINE_CONFIG, 0, BOARD_TYPE_INTERFACE, BOARD_TYPE_ALL_IN_CHAIN, data, 3);

        /* 喷阀板链数 */
        for(j = 0; j < struCnfg.struLayerInfo[i].nVavleChainTotal; j++)
        {
            data[0] = struCnfg.struLayerInfo[i].nVavleBoardTotal[j];
            board = struCnfg.struLayerInfo[i].nVavleBoardType[j];
            data[10] = struCnfg.nValveType;
            mySerial.pushCom1CmdData(CMD_ALL_MACHINE_CONFIG, 0, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        }

        /* 相机板链数 */
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
            data[0]  = struCnfg.struLayerInfo[i].nViewUnitTotal[j];
            data[5] = (struCnfg.struLayerInfo[i].stuViewInfo[j].nEjectorsPerCamera > 255) ? 255 : struCnfg.struLayerInfo[i].stuViewInfo[j].nEjectorsPerCamera ;   // 每视单喷阀板对应的喷嘴数
            data[9] = board;
            //选择交叉相机功能时，主视和辅视相机数目不同
            //            if(1 == struCnfg.bCrossCam)
            //            {
            //                if(1 == struCnfg.bCrossMode1)//主视比辅视多1个相机
            //                {
            //                    if(board == BOARD_CAMERA_1_FRONT_MAIN || board == BOARD_CAMERA_1_REAR_MAIN ||
            //                            board == BOARD_CAMERA_2_FRONT_MAIN || board == BOARD_CAMERA_2_REAR_MAIN)
            //                    {
            //                        data[0] = struCnfg.nChuteTotal;
            //                    }
            //                    else
            //                    {
            //                        data[0] = struCnfg.nChuteTotal-1;
            //                    }
            //                }
            //                else if(1 == struCnfg.bCrossMode2)//辅视比主视多1个相机
            //                {
            //                    if(board == BOARD_CAMERA_1_FRONT_VICE || board == BOARD_CAMERA_1_REAR_VICE
            //                            || board == BOARD_CAMERA_2_FRONT_MAIN || board == BOARD_CAMERA_2_REAR_MAIN ) //2021/8/20 lds机型四视交叉
            //                            || board == BOARD_CAMERA_2_FRONT_VICE || board == BOARD_CAMERA_2_REAR_VICE ) //2021/8/20 lds机型四视交叉
            //                    {
            //                        data[0] = struCnfg.nChuteTotal+1;
            //                    }
            //                    else
            //                    {
            //                        data[0] = struCnfg.nChuteTotal;
            //                    }
            //                }
            //                else
            //                {
            //                    data[0]  = struCnfg.struLayerInfo[i].nViewUnitTotal[j];
            //                }
            //            }
            mySerial.pushCom1CmdData(CMD_ALL_MACHINE_CONFIG, 0, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        }


    }
}

/* 配置交叉相机 */
void GlobalFlow::resetCrossCam()
{
    int i, j;
    int board;
    char data[SEND_PACKET_DATA_SIZE];
#if 0 //2021/8/21 只需要发给接口板，相机板只需知道有几个相机
    // 相机板
    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            memset(data, 0, SEND_PACKET_DATA_SIZE);
            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
            //            data[0] = struCnfg.bCrossCam;    // 是否交叉
            //            if(struCnfg.bCrossMode1)
            //            {
            //                data[1] = 0;
            //            }
            //            else if(struCnfg.bCrossMode2)
            //            {
            //                data[1] = 1;
            //            }
            //2021/8/20 周星协议
            if (!struCnfg.bCrossCam)  data[0] = 0;
            if (struCnfg.bCrossMode1) data[0] = 1;
            if (struCnfg.bCrossMode2) data[0] = 2;

            mySerial.pushCom1CmdData(CMD_CAMERA_PIXEL_CORSS, 0, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        }
    }
#endif
    memset(data, 0, SEND_PACKET_DATA_SIZE);
    //    if (!struCnfg.bCrossCam)  data[0] = 0;
    //    if (struCnfg.bCrossMode1) data[0] = 1;
    data[0] = struCnfg.bCrossMode1;
    data[2] = struCnfg.nCorssEjector;

    mySerial.pushCom1CmdData(CMD_CAMERA_PIXEL_CORSS, 0, BOARD_TYPE_INTERFACE, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
}

/* 重置FPGA运行模式 */
void GlobalFlow::resetFpgaRunModeRG()
{
    int i, j,m,n;
    int board;
    char data[SEND_PACKET_DATA_SIZE];
    memset(data, 0, SEND_PACKET_DATA_SIZE);
    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(!struCnfg.struLayerInfo[i].nViewIsUsed[j])
                continue;
            memset(data, 0, SEND_PACKET_DATA_SIZE);
            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
            for(m  = 0; m < struCnfg.struLayerInfo[i].nViewUnitTotal[j]; m++)
            {
                n = struCnfg.struLayerInfo[i].nViewUnitTotal[j] - m -1;
                data[0] = struCnfp.nCameraBoardUserMode[i][j][n];
                mySerial.pushCom1CmdData(CMD_ALL_FPGA_USER_MODE, 0, board, n, data, 3);
                qDebug()<<"reset FPGAmode "<<j<<n<< struCnfp.nCameraBoardUserMode[i][j][n];
                myFlow.msleep(100);
            }
        }
    }
    //    memset(data, 0, SEND_PACKET_DATA_SIZE);
    //    data[0] = struCnfg.nInterfaceBoardMode;
    //    mySerial.pushCom1CmdData(CMD_ALL_FPGA_USER_MODE, 0, BOARD_TYPE_INTERFACE, BOARD_TYPE_ALL_IN_CHAIN, data, 3);

}

#if 1
void GlobalFlow::onOff()
{
    char data[SEND_PACKET_DATA_SIZE];
    memset(data, 0, SEND_PACKET_DATA_SIZE);
    struGsh.bFlagAutowipe = 0;      // 禁止清灰

    //    if (struGsh.bSortStart == 1 && struGsh.bStatEject == 0) {
    if (struGsh.bSortStart == 1) {
        struGsh.bSortStart = 0;
        struGsh.bStatEject = 0;
        struCnfg.ntotalFeedTime += struGsh.nStaticCounter;
        resetFeederRG();
        sleep(struCnfg.nOnoffDelay);

        //if (!myNetWork->udpthread->isrunning & struGsh.bFlagMqttConnect)
        //                {
        //                            myNetWork->setUdpRecvType(CAPTURE_BLOW_COUNTS);
        //                            myNetWork->udpStart();
        //                            sleep(1);
        //                            myNetWork->udpStop();
        //                }

        resetSortOnOff(0, 0);
        vavleOnOff();
        configIpcSortStart(struGsh.bSortStart);
        sleep(1);
        if(struCnfg.autoTargetImage == 1 && myFastNetThread->isrunning == true)
        {
            g_infoWidget().setLabelText(g_myLan().Stop_AI_image_capture);
            g_infoWidget().delayShow();
            myFastNetThread->udpSendTransStartStop(myFastNetThread->Stop,FAST_TARGET_CAPTURE_IMAGE,BOARD_TYPE_ALL_CAMERA,BOARD_TYPE_ALL_IN_CHAIN);
            myFastNetThread->isrunning = false;
        }
    }
    else {
        if(!interfaceEncryptAns)
        {
            dccryptCodeTimeGet();
        }
        // 红外部分
        //开启振动器前发送明场校正命令
        if(struCnfg.nRealTimeCorrection )
        {
            myFlow.resetSortOnOff(0, 0);
            myFlow.cameraBrightCalibrateRG(true, struGsh.nLayer, struGsh.nView, struGsh.nUnit, 0);
            myFlow.sleep(2);
        }
        struGsh.nRunCounter = 0;
        struGsh.bSortStart = 1;
        struGsh.bStatEject = 1;
        configIpcSortStart(struGsh.bSortStart);
        sleep(1);
        struGsh.nStaticCounter = 0; //每次开色选，开始新一轮统计
        vavleOnOff();
        if (struGsh.bFlagMachinetest == 2) {    // 整机老化自检状态
            //to add
        }
        else {
            resetSortOnOff(1, 0);
        }
        sleep(struCnfg.nOnoffDelay_2);
        // 根据柔性供料，添加初始料位判定
        /*
        autoFeedRadarLevelJudge();
        for(int i = 0; i < struCnfg.nLayerTotal; i++)
        {
            for(int j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
            {
                if (!struCnfg.struLayerInfo[i].nViewIsUsed[j]) continue;
                if (struCnfg.struLayerInfo[i].nViewSensorType[j] != SENSOR_T1) continue; //只有T1支持产量含杂查询
                int board = struCnfg.struLayerInfo[i].nViewBoardType[j];
                mySerial.pushCom1CmdData(CMD_CAMERA_STATICS_CLEAR, 0, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
            }
        }
*/
        resetFeederRG();
        if(struCnfg.autoTargetImage == 1)
        {
            if(struCnfg.nFreeSpace <= 1000.0){
                g_infoWidget().setLabelText(g_myLan().msg_memory_space_inadequate);
                g_infoWidget().delayShow();
                myFlow.sleep(2);
                g_infoWidget().hide();
            }
            else
            {
                myFastNetThread->setUdpRecvType(FAST_TARGET_CAPTURE_IMAGE,struGsh.nLayer,struGsh.nView,struGsh.nUnit);
                myFastNetThread->isrunning = true;
                myFastNetThread->start();
            }
        }
        struGsh.bAlarmIpcStatus[0] = 0; //关闭色选IPC报警停
        struGsh.bAlarmIpcStatus[1] = 0; //关闭色选IPC报警停

    }
    struGsh.bFlagAutowipe = 1;      // 允许清灰
}
#endif

/* 喷阀开关 接口板，相机板  */
void GlobalFlow::vavleOnOff()
{
    int i, j;
    int board;
    char data[SEND_PACKET_DATA_SIZE];

    memset(data, 0, SEND_PACKET_DATA_SIZE);
    data[0] = struGsh.bStatEject;

    mySerial.pushCom1CmdData(CMD_ALL_EJECTOR_ENABLE, 0, BOARD_TYPE_INTERFACE, BOARD_TYPE_ALL_IN_CHAIN, data, 3);

    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
            mySerial.pushCom1CmdData(CMD_ALL_EJECTOR_ENABLE, 0, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        }

        for(j = 0; j < struCnfg.struLayerInfo[i].nVavleChainTotal; j++)
        {
            board = struCnfg.struLayerInfo[i].nVavleBoardType[j];
            mySerial.pushCom1CmdData(CMD_ALL_EJECTOR_ENABLE, 0, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        }
    }
}

/* 发送相机参数 */
void GlobalFlow::resetCamera()
{
    /* 发送整机模拟增益 */
    resetCameraAnaGainRG(true, 0, 0, 0);

    /* 发送整机相机偏置 */
    resetCameraBiasRG(true, 0, 0, 0);

    /* 发送整机相机横向分辨率 */
    resetCameraPixelHoriRG();

    /* 发送整机传感器维度 */
    resetCameraPixelDimRG();

    /* 发送整机传感器配置 */
    resetSensorConfigRG();

    /* 发送整机像元划分 */
    resetCameraPixelRG(true, 0, 0, 0);

    /* 发送亚像素校正信息 */
    resetCameraPixelCalibrateRG(true, 0, 0,0);

    /* 发送相机交叉信息*/
    resetCrossCam();

    /* 发送相机压缩方式*/
    resetCameraImageConpressMode();

    /* 发送相机丢行方式*/
    resetCameraDropLine();

    /*图像颜色分类选择 */
    resetCameraColorChoose();

    if(struCnfg.nMachineType == MACHINE_LD_ZL_GP || struCnfg.nMachineType == MACHINE_LD2_ZL_GP)
    {
        resetCameraEnhance();
    }
    /*resize模式*/
    //    resetCameraResizeMode(); //24/1/2 改为模型加载成功后发送
}

/* 发送模拟增益 */
void GlobalFlow::resetCameraAnaGainRG(bool bIsAll, int nLayerId, int nViewId, int nUnit)
{
    int i, j, k;
    int nInt = 0, board, nUnitAddr;
    char data[SEND_PACKET_DATA_SIZE];
    int nCurCal = struCnfp.bCurCal;

    if(bIsAll == true)
    {
        for(i = 0; i < struCnfg.nLayerTotal; i++)  //层
        {
            for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)  // 视
            {
                if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
                board = struCnfg.struLayerInfo[i].nViewBoardType[j];
                for(k = 0; k < struCnfg.struLayerInfo[i].nViewUnitTotal[j]; k++) // 相机单元
                {
                    nUnitAddr = struCnfg.struLayerInfo[i].stuViewInfo[j].nViewUnitId[k];
                    switch(struCnfp.nSensorDim[i][j])
                    {
                    case 1:
                        memset(data, 0, SEND_PACKET_DATA_SIZE);
                        data[0] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nAnaGainRed[nUnitAddr][nCurCal];
                        mySerial.pushCom1CmdData(CMD_CAMERA_ANA_GAIN, nInt, board, nUnitAddr, data, 3);
                        break;
                    case 2:
                        memset(data, 0, SEND_PACKET_DATA_SIZE);
                        data[0] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nAnaGainRed[nUnitAddr][nCurCal];
                        data[1] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nAnaGainGreen[nUnitAddr][nCurCal];
                        mySerial.pushCom1CmdData(CMD_CAMERA_ANA_GAIN, nInt, board, nUnitAddr, data, 3);
                        break;
                    case 3:
                        memset(data, 0, SEND_PACKET_DATA_SIZE);
                        if(struCnfp.nViewSensorType[0][j] == SENSOR_D2)
                        {

                            data[0] = 31 - struCnfg.struLayerInfo[i].stuCameraInfo[j].nAnaGainRed[nUnitAddr][nCurCal];
                            data[1] = 31 - struCnfg.struLayerInfo[i].stuCameraInfo[j].nAnaGainGreen[nUnitAddr][nCurCal];
                            data[2] = 31 - struCnfg.struLayerInfo[i].stuCameraInfo[j].nAnaGainBlue[nUnitAddr][nCurCal];
                            data[3] = 31;
                            data[0] = data[0] < 0 ? 0: data[0];
                            data[1] = data[1] < 0 ? 0: data[1];
                            data[2] = data[2] < 0 ? 0: data[2];
                        }
                        else{
                            data[0] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nAnaGainRed[nUnitAddr][nCurCal];
                            data[1] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nAnaGainGreen[nUnitAddr][nCurCal];
                            data[2] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nAnaGainBlue[nUnitAddr][nCurCal];
                        }
                        mySerial.pushCom1CmdData(CMD_CAMERA_ANA_GAIN, nInt, board, nUnitAddr, data, 3);
                        break;
                    case 4:
                        memset(data, 0, SEND_PACKET_DATA_SIZE);
                        if(struCnfp.nViewSensorType[0][j] == SENSOR_D2)
                        {
                            data[0] = 31-struCnfg.struLayerInfo[i].stuCameraInfo[j].nAnaGainRed[nUnitAddr][nCurCal];
                            data[1] = 31-struCnfg.struLayerInfo[i].stuCameraInfo[j].nAnaGainGreen[nUnitAddr][nCurCal];
                            data[2] = 31-struCnfg.struLayerInfo[i].stuCameraInfo[j].nAnaGainBlue[nUnitAddr][nCurCal];
                            data[3] = 31-struCnfg.struLayerInfo[i].stuCameraInfo[j].nAnaGainNir[nUnitAddr][nCurCal];
                        }else{
                            data[0] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nAnaGainRed[nUnitAddr][nCurCal];
                            data[1] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nAnaGainGreen[nUnitAddr][nCurCal];
                            data[2] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nAnaGainBlue[nUnitAddr][nCurCal];
                            data[3] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nAnaGainNir[nUnitAddr][nCurCal];
                        }
                        mySerial.pushCom1CmdData(CMD_CAMERA_ANA_GAIN, nInt, board, nUnitAddr, data, 3);
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }
    else
    {
        memset(data, 0, SEND_PACKET_DATA_SIZE);

        nInt = nLayerId;
        board = struCnfg.struLayerInfo[nInt].nViewBoardType[nViewId];
        nUnitAddr = struCnfg.struLayerInfo[nInt].stuViewInfo[nViewId].nViewUnitId[nUnit];
        switch(struCnfp.nSensorDim[nInt][nViewId])
        {
        case 1:
            memset(data, 0, SEND_PACKET_DATA_SIZE);
            data[0] = struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nAnaGainRed[nUnitAddr][nCurCal];
            mySerial.pushCom1CmdData(CMD_CAMERA_ANA_GAIN, nInt, board, nUnitAddr, data, 3);
            break;
        case 2:
            memset(data, 0, SEND_PACKET_DATA_SIZE);
            data[0] = struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nAnaGainRed[nUnitAddr][nCurCal];
            data[1] = struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nAnaGainGreen[nUnitAddr][nCurCal];
            mySerial.pushCom1CmdData(CMD_CAMERA_ANA_GAIN, nInt, board, nUnitAddr, data, 3);
            break;
        case 3:
            memset(data, 0, SEND_PACKET_DATA_SIZE);
            if(struCnfp.nViewSensorType[0][j] == SENSOR_D2)
            {
                data[0] = 31 - struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nAnaGainRed[nUnitAddr][nCurCal];
                data[1] = 31 - struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nAnaGainGreen[nUnitAddr][nCurCal];
                data[2] = 31 - struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nAnaGainBlue[nUnitAddr][nCurCal];
                data[3] = 31;
                data[0] = data[0] < 0 ? 0: data[0];
                data[1] = data[1] < 0 ? 0: data[1];
                data[2] = data[2] < 0 ? 0: data[2];
            }
            else{
                data[0] = struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nAnaGainRed[nUnitAddr][nCurCal];
                data[1] = struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nAnaGainGreen[nUnitAddr][nCurCal];
                data[2] = struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nAnaGainBlue[nUnitAddr][nCurCal];
            }
            mySerial.pushCom1CmdData(CMD_CAMERA_ANA_GAIN, nInt, board, nUnitAddr, data, 3);
            break;
        case 4:
            memset(data, 0, SEND_PACKET_DATA_SIZE);
            if(struCnfp.nViewSensorType[0][nViewId] == SENSOR_D2)
            {
                data[0] = 31-struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nAnaGainRed[nUnitAddr][nCurCal];
                data[1] = 31-struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nAnaGainGreen[nUnitAddr][nCurCal];
                data[2] = 31-struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nAnaGainBlue[nUnitAddr][nCurCal];
                data[3] = 31-struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nAnaGainNir[nUnitAddr][nCurCal];
            }else{
                data[0] = struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nAnaGainRed[nUnitAddr][nCurCal];
                data[1] = struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nAnaGainGreen[nUnitAddr][nCurCal];
                data[2] = struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nAnaGainBlue[nUnitAddr][nCurCal];
                data[3] = struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nAnaGainNir[nUnitAddr][nCurCal];
            }
            mySerial.pushCom1CmdData(CMD_CAMERA_ANA_GAIN, nInt, board, nUnitAddr, data, 3);
            break;
        default:
            break;
        }

    }
}

/* 发送相机偏置 */
void GlobalFlow::resetCameraBiasRG(bool bIsAll, int nLayerId, int nViewId, int nUnit)
{
    int i, j, k;
    int nInt=0, board, nUnitAddr;
    char data[SEND_PACKET_DATA_SIZE];

    if(bIsAll == true)
    {
        for(i = 0; i < struCnfg.nLayerTotal; i++)  //层
        {
            for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)  // 视
            {
                if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
                board = struCnfg.struLayerInfo[i].nViewBoardType[j];
                for(k = 0; k < struCnfg.struLayerInfo[i].nViewUnitTotal[j]; k++) // 相机单元
                {
                    nUnitAddr = struCnfg.struLayerInfo[i].stuViewInfo[j].nViewUnitId[k];
                    switch(struCnfp.nSensorDim[i][j])
                    {
                    case 1:
                        memset(data, 0, SEND_PACKET_DATA_SIZE);
                        data[0] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nBiasingRed[nUnitAddr];
                        mySerial.pushCom1CmdData(CMD_CAMERA_BIASING, nInt, board, nUnitAddr, data, 3);
                        break;
                    case 2:
                        memset(data, 0, SEND_PACKET_DATA_SIZE);
                        data[0] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nBiasingRed[nUnitAddr];
                        data[1] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nBiasingGreen[nUnitAddr];
                        mySerial.pushCom1CmdData(CMD_CAMERA_BIASING, nInt, board, nUnitAddr, data, 3);
                        break;
                    case 3:
                        memset(data, 0, SEND_PACKET_DATA_SIZE);
                        data[0] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nBiasingRed[nUnitAddr];
                        data[1] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nBiasingGreen[nUnitAddr];
                        data[2] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nBiasingBlue[nUnitAddr];
                        mySerial.pushCom1CmdData(CMD_CAMERA_BIASING, nInt, board, nUnitAddr, data, 3);
                        break;
                    case 4:
                        memset(data, 0, SEND_PACKET_DATA_SIZE);
                        data[0] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nBiasingRed[nUnitAddr];
                        data[1] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nBiasingGreen[nUnitAddr];
                        data[2] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nBiasingBlue[nUnitAddr];
                        data[3] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nBiasingNir[nUnitAddr];
                        mySerial.pushCom1CmdData(CMD_CAMERA_BIASING, nInt, board, nUnitAddr, data, 3);
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }
    else
    {
        memset(data, 0, SEND_PACKET_DATA_SIZE);

        nInt = nLayerId;
        board = struCnfg.struLayerInfo[nInt].nViewBoardType[nViewId];
        nUnitAddr = struCnfg.struLayerInfo[nInt].stuViewInfo[nViewId].nViewUnitId[nUnit];

        switch(struCnfp.nSensorDim[nInt][nViewId])
        {
        case 1:
            data[0] = struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nBiasingRed[nUnitAddr];
            mySerial.pushCom1CmdData(CMD_CAMERA_BIASING, nInt, board, nUnitAddr, data, 3);
            break;
        case 2:
            data[0] = struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nBiasingRed[nUnitAddr];
            data[1] = struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nBiasingGreen[nUnitAddr];
            mySerial.pushCom1CmdData(CMD_CAMERA_BIASING, nInt, board, nUnitAddr, data, 3);
            break;
        case 3:
            data[0] = struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nBiasingRed[nUnitAddr];
            data[1] = struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nBiasingGreen[nUnitAddr];
            data[2] = struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nBiasingBlue[nUnitAddr];
            mySerial.pushCom1CmdData(CMD_CAMERA_BIASING, nInt, board, nUnitAddr, data, 3);
            break;
        case 4:
            data[0] = struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nBiasingRed[nUnitAddr];
            data[1] = struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nBiasingGreen[nUnitAddr];
            data[2] = struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nBiasingBlue[nUnitAddr];
            data[3] = struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nBiasingNir[nUnitAddr];
            mySerial.pushCom1CmdData(CMD_CAMERA_BIASING, nInt, board, nUnitAddr, data, 3);
            break;
        default:
            break;
        }
    }
}


/* 相机信号预处理，校正使能、偏置使能、纠错使能 */
void GlobalFlow::cameraWaveCaliEnable()
{
    /* 明场校正使能 */
    cameraBrightCalibrateEnableRG();

    /*暗场校正使能 */
    cameraDarkCalibrateEnableRG();
#if 0   //new
    int i, j, k;
    int nInt=0, board, nUnitAddr;
    char data[SEND_PACKET_DATA_SIZE];

    /* 整机使能相同 */
    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            memset(data, 0, SEND_PACKET_DATA_SIZE);
            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
            data[0] = struGsh.bCaliGainEnable;
            data[1] = struGsh.bCaliBiasEnable;
            data[2] = struGsh.bCaliDotReviseEnable;

            mySerial.pushCom1CmdData(CMD_CAMERA_WAVE_PROCESSING, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        }
    }
#endif
}

void GlobalFlow::resetSortNamesByLan()
{
    int i,j,k;
    for(i = 0; i < struCnfg.nLayerTotal ; i++)
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {

            for(k = 0; k < struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal; k++) {

                memset(struCnfp.struGroupIdentify[i][j][k].struGreyColor[ARITH_GREY_A].sName, 0, sizeof(struCnfp.struGroupIdentify[i][j][k].struGreyColor[ARITH_GREY_A].sName));
                memset(struCnfp.struGroupIdentify[i][j][k].struGreyColor[ARITH_GREY_B].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struGreyColor[ARITH_GREY_C].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struGreyColor[ARITH_GREY_D].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struGreyColor[ARITH_DISCOLOR_A].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struGreyColor[ARITH_DISCOLOR_B].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struGreyColor[ARITH_CROSS].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struSvm[0].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struSvm[1].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struSvm[2].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struSvm[3].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struShapeSvm[0].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struShapeSvm[1].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struShapeSvm[2].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struCircleLong[0].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struCircleLong[1].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struBigSmall[0].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struBigSmall[1].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struLongShort[0].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struLongShort[1].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struHsv[0].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struHsv[1].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struHsv[2].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struColorSat.sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struColorHue.sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struColorScale.sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struMildew.sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struOutline.sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struLine.sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struBud[0].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struBud[1].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struRound[0].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struRound[1].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struColorValue.sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struSplMildew.sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struWhitePeanut.sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struPepperHandle.sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struSuppressLarge[0].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struSuppressLarge[1].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struSuppressLarge[2].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struWheatSprout.sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struTeaColor[0].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struTeaColor[1].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struTeaColor[2].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struTeaColor[3].sName, 0, AIRTH_NAME_LENGTH);
                memset(struCnfp.struGroupIdentify[i][j][k].struTeaColor[4].sName, 0, AIRTH_NAME_LENGTH);
                /* 颜色算法， 灰度A,B，色差A,B，差分A... */
                QByteArray array = g_myLan().arith_gray_a.toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struGreyColor[0].sName,array.data(), array.length());
                array = g_myLan().arith_gray_b.toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struGreyColor[1].sName,array.data(), array.length());
                array = g_myLan().arith_gray_c.toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struGreyColor[2].sName,array.data(), array.length());
                array = g_myLan().arith_gray_d.toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struGreyColor[3].sName,array.data(), array.length());
                array = g_myLan().arith_discolor_a.toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struGreyColor[4].sName,array.data(), array.length());
                array = g_myLan().arith_discolor_b.toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struGreyColor[5].sName,array.data(), array.length());
                array = g_myLan().arith_cross.toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struGreyColor[6].sName,array.data(), array.length());
                array = QString("SVMA").toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struSvm[0].sName,array.data(), array.length());
                array = QString("SVMB").toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struSvm[1].sName,array.data(), array.length());
                array = QString("SVMC").toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struSvm[2].sName,array.data(), array.length());
                array = QString("SVMD").toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struSvm[3].sName,array.data(), array.length());
                array = QString(g_myLan().shape+g_myLan().arith_svm_a).toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struShapeSvm[0].sName,array.data(), array.length());
                array = QString(g_myLan().shape+g_myLan().arith_svm_b).toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struShapeSvm[1].sName,array.data(), array.length());
                array = QString(g_myLan().shape+g_myLan().arith_svm_c).toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struShapeSvm[2].sName,array.data(), array.length());
                array = g_myLan().arith_circle_long.toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struCircleLong[0].sName,array.data(), array.length());
                array = g_myLan().arith_circle_long_b.toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struCircleLong[1].sName,array.data(), array.length());
                array = g_myLan().arith_big_small.toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struBigSmall[0].sName,array.data(), array.length());
                array = g_myLan().arith_big_small_b.toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struBigSmall[1].sName,array.data(), array.length());
                array = g_myLan().arith_long_short.toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struLongShort[0].sName,array.data(), array.length());
                array = g_myLan().arith_long_short_b.toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struLongShort[1].sName,array.data(), array.length());
                array = QString("HSV-A").toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struHsv[0].sName,array.data(), array.length());
                array = QString("HSV-B").toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struHsv[1].sName,array.data(), array.length());
                array = QString("HSV-C").toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struHsv[2].sName,array.data(), array.length());
                array = g_myLan().arith_sat.toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struColorSat.sName,array.data(), array.length());
                array = g_myLan().arith_hue.toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struColorHue.sName,array.data(), array.length());
                array = g_myLan().arith_scale.toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struColorScale.sName,array.data(), array.length());
                array = g_myLan().arith_mildew.toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struMildew.sName,array.data(), array.length());
                array = g_myLan().arith_outline.toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struOutline.sName,array.data(), array.length());
                array = g_myLan().arith_line.toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struLine.sName,array.data(), array.length());
                array = g_myLan().arith_bud.toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struBud[0].sName,array.data(), array.length());
                array = g_myLan().arith_bud_b.toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struBud[1].sName,array.data(), array.length());
                array = g_myLan().arith_value.toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struColorValue.sName,array.data(), array.length());
                array = g_myLan().arith_round.toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struRound[0].sName,array.data(), array.length());
                array = g_myLan().arith_round_b.toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struRound[1].sName,array.data(), array.length());
                array = g_myLan().arith_spl_mildew.toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struSplMildew.sName,array.data(), array.length());
                array = g_myLan().arith_white_peanut.toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struWhitePeanut.sName,array.data(), array.length());
                array = g_myLan().arith_pepper_handle.toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struPepperHandle.sName,array.data(), array.length());
                array = QString(g_myLan().arith_suppress_large+"A").toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struSuppressLarge[0].sName,array.data(), array.length());
                array = QString(g_myLan().arith_suppress_large+"B").toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struSuppressLarge[1].sName,array.data(), array.length());
                array = QString(g_myLan().arith_suppress_large+"C").toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struSuppressLarge[2].sName,array.data(), array.length());
                array = g_myLan().arith_wheat_sprout.toUtf8();
                memcpy(struCnfp.struGroupIdentify[i][j][k].struWheatSprout.sName,array.data(), array.length());
            }
        }
    }
}

void GlobalFlow::resetSensorNamesByLan()
{
    for(int i = 0; i < CTRL_ALARM_MAX_FEED_SENSOR; i++)
    {
        memset(struCnfg.struAlarmConfig.nFeederLevelAlarmName[i], 0, 128);
        if(g_myLan().g_languageIndex == LANG_CHS)
        {
            sprintf(struCnfg.struAlarmConfig.nFeederLevelAlarmName[i], "料位 %d", i+1);
        }
        else
        {
            sprintf(struCnfg.struAlarmConfig.nFeederLevelAlarmName[i], "Feed Level %d", i+1);
        }
    }

    for(int i = 0; i < CTRL_ALARM_MAX_TEMP_SENSOR; i++)
    {
        memset(struCnfg.struAlarmConfig.nTemptureAlarmName[i], 0, 128);
        if(g_myLan().g_languageIndex == LANG_CHS)
        {
            sprintf(struCnfg.struAlarmConfig.nTemptureAlarmName[i], "温度 %d", i+1);
        }
        else
        {
            sprintf(struCnfg.struAlarmConfig.nTemptureAlarmName[i], "Temp %d", i+1);
        }
    }

}

/* 灵敏度校正使能 */
void GlobalFlow::resetSensUpdateEnable()
{
    int i, j, k, m;
    int nInt=0, board, nUnitAddr;
    char data[SEND_PACKET_DATA_SIZE];

    /* 整机使能相同 */
    //    for(i = 0; i < struCnfg.nLayerTotal; i++)
    //    {
    //        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
    //        {
    //            memset(data, 0, SEND_PACKET_DATA_SIZE);
    //            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
    //            data[0] = struCnfp.struGroupIdentify[i][j][k].nSensUpdateGeneral;

    //            mySerial.pushCom1CmdData(CMD_CAMERA_SENS_UPDATE, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
    //        }
    //    }
    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
            for(k = 0; k < struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal; k++)
            {
                for(m = 0; m < struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++) // 认为视的次数和吹的序号一致
                {
                    memset(data, 0, SEND_PACKET_DATA_SIZE);
                    nUnitAddr = getIdentifyGroupAddr(i, j, k, m)%64;
                    data[0] =  struCnfp.struGroupIdentify[i][j][k].nSensUpdateGeneral;
                    mySerial.pushCom1CmdData(CMD_CAMERA_SENS_UPDATE, nInt, board, nUnitAddr%64, data, 3);

                }
            }
        }
    }
}

void GlobalFlow::updateCurViewImgHight()
{
    switch(struCnfp.nSensorPixel[struGsh.nLayer][struGsh.nView])
    {
    case SENSOR_PIXELS_1024:
    case SENSOR_PIXELS_1024_TD:
        imgHeight = 416;
        break;
    case SENSOR_PIXELS_2048:
    case SENSOR_PIXELS_2048_TD:
    case SENSOR_PIXELS_2048_XTD:
    case SENSOR_PIXELS_2048_GTD:
        imgHeight = 832;
        break;
    default:
        imgHeight = 416;
        break;
    }
}

void  GlobalFlow::updateCameraTimeDivisionMode(int nLayer,int nView)
{
    switch(struCnfp.nSensorPixel[struGsh.nLayer][struGsh.nView])
    {
    case SENSOR_PIXELS_1024_TD:
    case SENSOR_PIXELS_2048_TD:
    case SENSOR_PIXELS_2048_XTD:
        if(struGsh.nCamTdMode == 0)
        {
            struGsh.nCamTdMode = 1;
            struGsh.nTdMode = TD_1024_A;
        }

        break;
    case SENSOR_PIXELS_2048_GTD:
        if(struGsh.nCamTdMode == 0){
            struGsh.nCamTdMode = TD_2048_A;
            struGsh.nTdMode = TD_1024_A;
        }
        break;
    default:
        struGsh.nCamTdMode = 0;
        struGsh.nTdMode = 0;
        break;
    }
    //   qDebug()<<"struGsh.nCamTdMode"<<struGsh.nCamTdMode<<"struGsh.nTdMode "<<struGsh.nTdMode ;
}

/* 明场校正使能 */
void GlobalFlow::cameraBrightCalibrateEnableRG()
{
    int i, j, k;
    int nInt=0, board, nUnitAddr;
    char data[SEND_PACKET_DATA_SIZE];

    /* 整机使能相同 */
    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
            memset(data, 0, SEND_PACKET_DATA_SIZE);
            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
            data[0] = struCnfp.bCaliGainEnable;
            data[1] = struCnfp.bCurCal;
            mySerial.pushCom1CmdData(CMD_CAMERA_CALIBRATE_BRIGHT_ENABLE, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        }
    }
}

/* 暗场校正使能 */
void GlobalFlow::cameraDarkCalibrateEnableRG()
{
    int i, j;
    int nInt=0, board;
    char data[SEND_PACKET_DATA_SIZE];

    /* 整机使能相同 */
    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
            memset(data, 0, SEND_PACKET_DATA_SIZE);
            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
            data[0] = struCnfp.bCaliBiasEnable;

            mySerial.pushCom1CmdData(CMD_CAMERA_CALIBRATE_DARK_ENABLE, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        }
    }
}

/* 设置明场校正
flag = 0, 明场校正获取， flag = 1 明场校正保存
*/
void GlobalFlow::cameraBrightCalibrateRG(bool bIsAll, int nLayerId, int nViewId, int nUnit, int flag)
{
    int i, j, k;
    int nInt=0, board, nUnitAddr;
    char data[SEND_PACKET_DATA_SIZE];
    int nCurCal = struCnfp.bCurCal;

    if(bIsAll == true)
    {
        for(i = 0; i < struCnfg.nLayerTotal; i++)  //层
        {
            for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)  // 视
            {
                if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
                if(struCnfg.nRealTimeCorrection && !struCnfp.nRealTimeCorrection[struGsh.nLayer][j]) continue;
                board = struCnfg.struLayerInfo[i].nViewBoardType[j];

                for(k = 0; k < struCnfg.struLayerInfo[i].nViewUnitTotal[j]; k++) // 相机单元
                {
                    nUnitAddr = struCnfg.struLayerInfo[i].stuViewInfo[j].nViewUnitId[k];

                    memset(data, 0, SEND_PACKET_DATA_SIZE);
                    if(flag == 0)   //明场矫正获取
                    {
                        switch(struCnfp.nSensorDim[i][j])
                        {
                        case 1:
                            data[0] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibrateRed[nUnitAddr][nCurCal];
                            break;
                        case 2:
                            data[0] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibrateRed[nUnitAddr][nCurCal];
                            data[1] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibrateGreen[nUnitAddr][nCurCal];
                            break;
                        case 3:
                            data[0] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibrateRed[nUnitAddr][nCurCal];
                            data[1] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibrateGreen[nUnitAddr][nCurCal];
                            data[2] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibrateBlue[nUnitAddr][nCurCal];
                            data[4] = struGsh.nTdMode%256;
                            break;
                        case 4:
                            data[0] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibrateRed[nUnitAddr][nCurCal];
                            data[1] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibrateGreen[nUnitAddr][nCurCal];
                            data[2] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibrateBlue[nUnitAddr][nCurCal];
                            data[3] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibrateNir[nUnitAddr][nCurCal];
                            break;
                        default:
                            break;
                        }
                        data[5] = data[0]&0xF0;
                        data[6] = data[1]&0xF0;
                        data[7] = data[2]&0xF0;
                        data[8] = data[3]&0xF0;
                        data[9] = nCurCal;
                        mySerial.pushCom1CmdData(CMD_CAMERA_CALIBRATE_BRIGHT_CAPTURE, nInt, board, nUnitAddr, data, 3);
                    }
                    else{   //明场矫正保存
                        data[0] = struGsh.nTdMode%256;
                        data[1] = nCurCal;
                        QDateTime  dateTime = QDateTime::currentDateTime();
                        data[2]= dateTime.date().year()%100;
                        data[3]= dateTime.date().month();
                        data[4]= dateTime.date().day();
                        data[5]= dateTime.time().hour();
                        data[6]= dateTime.time().minute();
                        data[7]= dateTime.time().second();
                        data[8]= struCnfp.nTimeDivisionScheme[i][j];
                        data[9]= struCnfp.nSensorRowFre[i][j]/256;
                        data[10] = struCnfp.nSensorRowFre[i][j]%256;
                        data[11] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibrateRed[nUnitAddr][nCurCal];
                        mySerial.pushCom1CmdData(CMD_CAMERA_CALIBRATE_BRIGHT_SAVE, nInt, board, nUnitAddr, data, 3);
                    }
                }
            }
        }
    }
    else
    {
        board = struCnfg.struLayerInfo[nInt].nViewBoardType[nViewId];
        nUnitAddr = struCnfg.struLayerInfo[nInt].stuViewInfo[nViewId].nViewUnitId[nUnit];

        memset(data, 0, SEND_PACKET_DATA_SIZE);

        if(flag == 0)   //明场矫正获取
        {
            switch(struCnfp.nSensorDim[nInt][nViewId])
            {
            case 1:
                data[0] = struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nCalibrateRed[nUnitAddr][nCurCal];
                break;
            case 2:
                data[0] = struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nCalibrateRed[nUnitAddr][nCurCal];
                data[1] = struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nCalibrateGreen[nUnitAddr][nCurCal];
                break;
            case 3:
                data[0] = struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nCalibrateRed[nUnitAddr][nCurCal];
                data[1] = struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nCalibrateGreen[nUnitAddr][nCurCal];
                data[2] = struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nCalibrateBlue[nUnitAddr][nCurCal];
                //  qDebug()<<"struGsh.nTdMode:"<<struGsh.nTdMode;
                data[4] = struGsh.nTdMode%256;
                break;
            case 4:
                data[0] = struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nCalibrateRed[nUnitAddr][nCurCal];
                data[1] = struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nCalibrateGreen[nUnitAddr][nCurCal];
                data[2] = struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nCalibrateBlue[nUnitAddr][nCurCal];
                data[3] = struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nCalibrateNir[nUnitAddr][nCurCal];
                break;
            default:
                break;
            }
            data[5] = data[0]&0xF0;
            data[6] = data[1]&0xF0;
            data[7] = data[2]&0xF0;
            data[8] = data[3]&0xF0;
            data[9] = nCurCal;
            mySerial.pushCom1CmdData(CMD_CAMERA_CALIBRATE_BRIGHT_CAPTURE, nInt, board, nUnitAddr, data, 3);
        }
        else
        {
            data[0] = struGsh.nTdMode%256;
            data[1] = nCurCal;
            QDateTime  dateTime = QDateTime::currentDateTime();
            data[2]= dateTime.date().year()%100;
            data[3]= dateTime.date().month();
            data[4]= dateTime.date().day();
            data[5]= dateTime.time().hour();
            data[6]= dateTime.time().minute();
            data[7] = dateTime.time().second();
            data[8]  = struCnfp.nTimeDivisionScheme[nInt][nViewId];
            data[9]  = struCnfp.nSensorRowFre[nInt][nViewId]/256;
            data[10] = struCnfp.nSensorRowFre[nInt][nViewId]%256;
            data[11] = struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nCalibrateRed[nUnitAddr][nCurCal];
            mySerial.pushCom1CmdData(CMD_CAMERA_CALIBRATE_BRIGHT_SAVE, nInt, board, nUnitAddr, data, 3);
        }
    }
}

/* 设置暗场校正
flag = 0, 暗场校正获取， flag = 1 暗场校正保存
*/
void GlobalFlow::cameraDarkCalibrateRG(bool bIsAll, int nLayerId, int nViewId, int nUnit, int flag)
{
    int i, j, k;
    int nInt=0, board, nUnitAddr;
    char data[SEND_PACKET_DATA_SIZE];

    if(bIsAll == true)
    {
        for(i = 0; i < struCnfg.nLayerTotal; i++)  //层
        {
            for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)  // 视
            {
                if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
                board = struCnfg.struLayerInfo[i].nViewBoardType[j];
                for(k = 0; k < struCnfg.struLayerInfo[i].nViewUnitTotal[j]; k++) // 相机单元
                {
                    nUnitAddr = struCnfg.struLayerInfo[i].stuViewInfo[j].nViewUnitId[k];
                    memset(data, 0, SEND_PACKET_DATA_SIZE);
                    if(flag == 0)
                    {
                        mySerial.pushCom1CmdData(CMD_CAMERA_CALIBRATE_DARK_CAPTURE, nInt, board, nUnitAddr, data, 3);
                    }
                    else
                    {
                        mySerial.pushCom1CmdData(CMD_CAMERA_CALIBRATE_DARK_SAVE, nInt, board, nUnitAddr, data, 3);
                    }
                }
            }
        }
    }
    else
    {
        memset(data, 0, SEND_PACKET_DATA_SIZE);

        board = struCnfg.struLayerInfo[nInt].nViewBoardType[nViewId];
        nUnitAddr = struCnfg.struLayerInfo[nInt].stuViewInfo[nViewId].nViewUnitId[nUnit];
        if(flag == 0)
        {
            mySerial.pushCom1CmdData(CMD_CAMERA_CALIBRATE_DARK_CAPTURE, nInt, board, nUnitAddr, data, 3);
        }
        else
        {
            mySerial.pushCom1CmdData(CMD_CAMERA_CALIBRATE_DARK_SAVE, nInt, board, nUnitAddr, data, 3);
        }
    }
}




/* 发送像元划分 */
void GlobalFlow::resetCameraPixelRG(bool bIsAll, int nLayerId, int nViewId, int nUnit)
{
    int i, j, k, m;
    int tmpIndex = 0, tmpEjector = 0;
    int nInt=0, board, nUnitAddr;
    char data[SEND_PACKET_DATA_SIZE];
    int curPixels = 2048, quo=1;
    if(bIsAll == true)
    {
        for(i = 0; i < struCnfg.nLayerTotal; i++)  //层
        {
            for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)  // 视
            {
                if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
                curPixels = myFlow.getSensorPixels(i, j);
                quo = (curPixels > 1024)?2:1;

                board = struCnfg.struLayerInfo[i].nViewBoardType[j];
                for(k = 0; k < struCnfg.struLayerInfo[i].nViewUnitTotal[j]; k++) // 相机单元
                {
                    nUnitAddr = struCnfg.struLayerInfo[nInt].stuViewInfo[nViewId].nViewUnitId[k];

                    memset(data, 0, SEND_PACKET_DATA_SIZE);

                    data[0] = (struCnfg.struLayerInfo[i].stuCameraInfo[j].nChannelBegin[nUnitAddr]/quo)/256;
                    data[1] = (struCnfg.struLayerInfo[i].stuCameraInfo[j].nChannelBegin[nUnitAddr]/quo)%256;
                    data[2] = (struCnfg.struLayerInfo[i].stuCameraInfo[j].nChannelEnd[nUnitAddr]/quo)/256;
                    data[3] = (struCnfg.struLayerInfo[i].stuCameraInfo[j].nChannelEnd[nUnitAddr]/quo)%256;
                    data[4] = 0;
                    data[5] = 0;
                    data[6] = (struCnfg.struLayerInfo[i].stuCameraInfo[j].nEjectBegin[nUnitAddr]-1)%256;
                    data[7] = (struCnfg.struLayerInfo[i].stuCameraInfo[j].nEjectEnd[nUnitAddr]-1)%256;
                    data[8] = (struCnfg.struLayerInfo[i].stuCameraInfo[j].nChannelBegin2[nUnitAddr]/quo)/256;
                    data[9] = (struCnfg.struLayerInfo[i].stuCameraInfo[j].nChannelBegin2[nUnitAddr]/quo)%256;
                    data[10] = (struCnfg.struLayerInfo[i].stuCameraInfo[j].nChannelEnd2[nUnitAddr]/quo)/256;
                    data[11] = (struCnfg.struLayerInfo[i].stuCameraInfo[j].nChannelEnd2[nUnitAddr]/quo)%256;
                    data[12] = (((struCnfg.struLayerInfo[i].stuCameraInfo[j].nEjectBegin[nUnitAddr]-1)>>8)<<4)|
                            (((struCnfg.struLayerInfo[i].stuCameraInfo[j].nEjectEnd[nUnitAddr]-1)>>8)&0x0f);
                    data[13] = 0;
                    data[14] = 0;
                    data[15] = 0;
                    mySerial.pushCom1CmdData(CMD_CAMERA_PIXEL_SCALE, nInt, board, nUnitAddr, data, 3);
                }
            }
        }
    }
    else
    {
        curPixels = myFlow.getSensorPixels(nInt, nViewId);
        quo = (curPixels > 1024)?2:1;

        memset(data, 0, SEND_PACKET_DATA_SIZE);

        board = struCnfg.struLayerInfo[nInt].nViewBoardType[nViewId];
        nUnitAddr = struCnfg.struLayerInfo[nInt].stuViewInfo[nViewId].nViewUnitId[nUnit];

        data[0] = (struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nChannelBegin[nUnitAddr]/quo)/256;
        data[1] = (struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nChannelBegin[nUnitAddr]/quo)%256;
        data[2] = (struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nChannelEnd[nUnitAddr]/quo)/256;
        data[3] = (struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nChannelEnd[nUnitAddr]/quo)%256;
        data[4] = 0;
        data[5] = 0;
        data[6] = (struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nEjectBegin[nUnitAddr]-1)%256;
        data[7] = (struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nEjectEnd[nUnitAddr]-1)%256;
        data[8] = ( struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nChannelBegin2[nUnitAddr]/quo )/256;
        data[9] = ( struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nChannelBegin2[nUnitAddr]/quo )%256;
        data[10] = ( struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nChannelEnd2[nUnitAddr]/quo )/256;
        data[11] = ( struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nChannelEnd2[nUnitAddr]/quo )%256;
        data[12] = (((struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nEjectBegin[nUnitAddr]-1)>>8)<<4)|
                (((struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nEjectEnd[nUnitAddr]-1)>>8)&0x0f);
        data[13] = 0;
        data[14] = 0;
        data[15] = 0;

        mySerial.pushCom1CmdData(CMD_CAMERA_PIXEL_SCALE, nInt, board, nUnitAddr, data, 3);

    }
}

/* 根据相机的起始截止像元，计算每个喷嘴的像元 */
void GlobalFlow::getCameraEjectorPixel(int nLayerId, int nViewId, int nUnit)
{
    int m;
    int beginCh, endCh;
    double ave;

    beginCh   = struCnfg.struLayerInfo[nLayerId].stuCameraInfo[nViewId].nChannelBegin[nUnit];
    endCh     = struCnfg.struLayerInfo[nLayerId].stuCameraInfo[nViewId].nChannelEnd[nUnit];

    ave = (endCh-beginCh+1)/(double)struCnfg.struLayerInfo[nLayerId].stuViewInfo[nViewId].nEjectorsPerCamera;

    for(m = 0; m < struCnfg.struLayerInfo[nLayerId].stuViewInfo[nViewId].nEjectorsPerCamera; m++)
    {
        struCnfg.struLayerInfo[nLayerId].stuCameraInfo[nViewId].nChannelEjectorBegin[nUnit][m] = (int)(beginCh+m*ave+0.5);    //起始像素为0，表示像素划分无效
        if(m > 0)
        {
            struCnfg.struLayerInfo[nLayerId].stuCameraInfo[nViewId].nChannelEjectorEnd[nUnit][m-1] = struCnfg.struLayerInfo[nLayerId].stuCameraInfo[nViewId].nChannelEjectorBegin[nUnit][m]-1;
        }
    }

    struCnfg.struLayerInfo[nLayerId].stuCameraInfo[nViewId].nChannelEjectorEnd[nUnit][struCnfg.struLayerInfo[nLayerId].nEjectorsPerChute-1] = endCh;
}

/* 传感器维度 */
void GlobalFlow::resetCameraPixelDimRG()
{
    int i, j, k;
    int nInt=0, board;
    char data[SEND_PACKET_DATA_SIZE];

    for(i = 0; i < struCnfg.nLayerTotal; i++)  //层
    {
        memset(data, 0, SEND_PACKET_DATA_SIZE);
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)  // 视
        {
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
            data[j] = struCnfp.nSensorDim[i][j];
            if(CUSTOM == CUSTOM_YANSI)
            {
                data[j%4] = struCnfp.nSensorDim[i][j];
            }
        }
        mySerial.pushCom1CmdData(CMD_CAMERA_SENSOR_DIM, 0, BOARD_TYPE_INTERFACE, BOARD_TYPE_ALL_IN_CHAIN, data, 3);

        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)  // 视
        {
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
            mySerial.pushCom1CmdData(CMD_CAMERA_SENSOR_DIM, 0, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        }
    }
}

/* 横向分别率 */
void GlobalFlow::resetCameraPixelHoriRG()
{
    int i, j, k;
    int nInt=0, board, nUnitAddr;
    char data[SEND_PACKET_DATA_SIZE];

    for(i = 0; i < struCnfg.nLayerTotal; i++)  //层
    {
        memset(data, 0, SEND_PACKET_DATA_SIZE);
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)  // 视
        {
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
            data[j] = struCnfp.nSensorPixel[i][j];
            if(struCnfp.nSensorPixel[i][j] == SENSOR_PIXELS_2048_XTD || struCnfp.nSensorPixel[i][j] == SENSOR_PIXELS_2048_GTD)
                data[j] = SENSOR_PIXELS_2048;
        }
        mySerial.pushCom1CmdData(CMD_CAMERA_HORI_DPI, 0, BOARD_TYPE_INTERFACE, BOARD_TYPE_ALL_IN_CHAIN, data, 3);

        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)  // 视
        {
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
            mySerial.pushCom1CmdData(CMD_CAMERA_HORI_DPI, 0, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        }
    }
}

/* 发送相机板 传感器配置 */
void GlobalFlow::resetSensorConfigRG()
{
    int i, j, k;
    int nInt=0, board, nUnitAddr,freqValue;
    char data[SEND_PACKET_DATA_SIZE];

    for(i = 0; i < struCnfg.nLayerTotal; i++)  //层
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)  // 视
        {
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
            memset(data, 0, SEND_PACKET_DATA_SIZE);

            board = struCnfg.struLayerInfo[i].nViewBoardType[j];

            if(struCnfp.nSensorRowFre[i][j] > 0){

                if(struCnfp.nViewSensorType[0][j] == SENSOR_T1)
                {
                    freqValue = (int)(10.46*28*30*100/struCnfp.nSensorRowFre[i][j]/(struCnfp.nSensorPixel[i][j]+1));
                    data[0] = freqValue/256;
                    data[1] = freqValue%256;
                }
                if(struCnfp.nViewSensorType[0][j] == SENSOR_D2)
                {
                    //                    freqValue = (int)(10.46*31.5*100/struCnfg.struLayerInfo[i].stuViewInfo[j].nSensorRowFre*50*1.25*struCnfg.struLayerInfo[i].stuViewInfo[j].nSensorDimD2/4/(struCnfg.struLayerInfo[i].stuViewInfo[j].nSensorPixel+1));
                    //2023/5/10 肖军不支持1024的d2帧频计算公式
                    freqValue = (int)(10.46*31.5*100/struCnfp.nSensorRowFre[i][j]*50*1.25*struCnfp.nSensorDimD2[i][j]/4/2);
                    data[0] = freqValue/256;
                    data[1] = freqValue%256;
                }
                if(struCnfp.nViewSensorType[0][j] == SENSOR_G2)
                {
                    freqValue = (int)(10.46*14*80*100/struCnfp.nSensorRowFre[i][j]);
                    data[0] = freqValue/256;
                    data[1] = freqValue%256;
                }
                if(struCnfp.nViewSensorType[0][j] == SENSOR_G3)
                {
                    freqValue = (int)(10.46*14*80*100/struCnfp.nSensorRowFre[i][j]);
                    data[0] = freqValue/256;
                    data[1] = freqValue%256;
                }
                if((struCnfp.nViewSensorType[0][j] == SENSOR_C1) || (struCnfp.nViewSensorType[0][j] == SENSOR_C1_2))
                {
                    if(struCnfp.nSensorPixel[i][j] == SENSOR_PIXELS_512)
                    {
                        freqValue = ((25.39*25)/(struCnfp.nSensorRowFre[i][j]/100))*80;
                        data[0] = freqValue/256;
                        data[1] = freqValue%256;
                    }
                    else
                    {
                        freqValue = ((25.39*12.5)/(struCnfp.nSensorRowFre[i][j]/100))*120;
                        data[0] = freqValue/256;
                        data[1] = freqValue%256;
                    }
                }
            }
            data[2] =   struCnfp.nSensorDataType[i][j];
            data[3] = 1;
            data[4] = 1;
            data[5] =  struCnfp.nMirror[i][j];
            data[6] =  struCnfp.nSenseBrightSelection[i][j];
            data[14] = struCnfp.nTimeDivisionScheme[i][j];

            mySerial.pushCom1CmdData(CMD_CAMERA_SENSOR_CONFIG, 0, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        }
    }
}

/* 图像压缩模式 */
void GlobalFlow::resetCameraImageConpressMode()
{
    int i, j, k;
    int nInt = 0, board, nUnitAddr;
    char data[SEND_PACKET_DATA_SIZE];

    for(i = 0; i < struCnfg.nLayerTotal; i++)  //层
    {
        memset(data, 0, SEND_PACKET_DATA_SIZE);

        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)  // 视
        {
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
            board = struCnfg.struLayerInfo[i].nViewBoardType[j];

            for(k = 0; k < struCnfg.struLayerInfo[i].nViewUnitTotal[j]; k++) // 相机单元
            {
                nUnitAddr = struCnfg.struLayerInfo[i].stuViewInfo[j].nViewUnitId[k];

                memset(data, 0, SEND_PACKET_DATA_SIZE);

                data[0] = struCnfp.nCameraCompressMode[i][j];

                mySerial.pushCom1CmdData(CMD_CAMERA_IMAGE_COMPRESS_MODE, nInt, board, nUnitAddr, data, 3);
            }
        }
    }
}

/* 图像丢行模式 */
void GlobalFlow::resetCameraDropLine()
{
    int i, j, k;
    int nInt = 0, board, nUnitAddr;
    char data[SEND_PACKET_DATA_SIZE];

    for(i = 0; i < struCnfg.nLayerTotal; i++)  //层
    {
        memset(data, 0, SEND_PACKET_DATA_SIZE);

        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)  // 视
        {
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
            board = struCnfg.struLayerInfo[i].nViewBoardType[j];

            for(k = 0; k < struCnfg.struLayerInfo[i].nViewUnitTotal[j]; k++) // 相机单元
            {
                nUnitAddr = struCnfg.struLayerInfo[i].stuViewInfo[j].nViewUnitId[k];

                memset(data, 0, SEND_PACKET_DATA_SIZE);

                data[0] = struCnfp.nSensDropLine[i][j];

                mySerial.pushCom1CmdData(CMD_CAMERA_IMAGE_DROP_LINE, nInt, board, nUnitAddr, data, 3);
            }
        }
    }
}
/*图像颜色分类选择 */
void GlobalFlow::resetCameraColorChoose()
{
    int i, j, k;
    int nInt = 0, board, nUnitAddr;
    char data[SEND_PACKET_DATA_SIZE];


    for(i = 0; i < struCnfg.nLayerTotal; i++)  //层
    {
        memset(data, 0, SEND_PACKET_DATA_SIZE);

        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)  // 视
        {
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
            board = struCnfg.struLayerInfo[i].nViewBoardType[j];

            if(struCnfp.nViewSensorType[0][j] == SENSOR_D2)
            {
                memset(struCnfp.nSensorColorChoose[i][j], 0, sizeof(struCnfp.nSensorColorChoose[i][j]));
                switch (struCnfp.nSensorD2Mode[i][j])
                {
                case 1://rgb
                    struCnfp.nSensorColorChoose[i][j][3] = 1;
                    break;
                case 2://rgn
                    struCnfp.nSensorColorChoose[i][j][0] = 1;
                    break;
                case 3://rbn
                    struCnfp.nSensorColorChoose[i][j][1] = 1;
                    break;
                case 4://gbn
                    struCnfp.nSensorColorChoose[i][j][2] = 1;
                    break;
                default:
                    break;
                }
                for(k = 0; k < struCnfg.struLayerInfo[i].nViewUnitTotal[j]; k++) // 相机单元
                {
                    nUnitAddr = struCnfg.struLayerInfo[i].stuViewInfo[j].nViewUnitId[k];
                    memset(data, 0, SEND_PACKET_DATA_SIZE);
                    data[0] = struCnfp.nSensorColorChoose[i][j][0]
                            | struCnfp.nSensorColorChoose[i][j][1]<<1
                                                                    | struCnfp.nSensorColorChoose[i][j][2]<<2
                                                                                                            | struCnfp.nSensorColorChoose[i][j][3]<<3;
                    mySerial.pushCom1CmdData(CMD_CAMERA_COLOR_CHOOSE, nInt, board, nUnitAddr, data, 3);
                }
            }
        }
    }
}

/* resize模式 */
void GlobalFlow::resetCameraResizeMode()
{
    int i, j, k;
    int nInt = 0, board, nUnitAddr;
    char data[SEND_PACKET_DATA_SIZE];

    for(i = 0; i < struCnfg.nLayerTotal; i++)  //层
    {
        memset(data, 0, SEND_PACKET_DATA_SIZE);

        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)  // 视
        {
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
            if(struCnfp.nIpcRoiWidth[j] == 768)
                struCnfp.nSensResizeMode[i][j] = 1;
            else
                struCnfp.nSensResizeMode[i][j] = 0;
            data[0] = struCnfp.nSensResizeMode[i][j];
            if(data[0] > 0)
            {
                data[0] -= 1;
                data[1]  = 1;
            }
            else
                data[1] = 0;

            mySerial.pushCom1CmdData(CMD_RESIZE_MODE, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        }
    }
}
/* 亚像素矫正 */
void GlobalFlow::resetCameraPixelCalibrateRG(bool bIsAll, int nLayerId, int nViewId, int nUnit)
{
    int i, j, k;
    int nInt=0, board, nUnitAddr;
    char data[SEND_PACKET_DATA_SIZE];

    if(bIsAll == true)
    {

        for(i = 0; i < struCnfg.nLayerTotal; i++)  //层
        {
            for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)  // 视
            {
                if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;

                board = struCnfg.struLayerInfo[i].nViewBoardType[j];
                for(k = 0; k < struCnfg.struLayerInfo[i].nViewUnitTotal[j]; k++) // 相机单元
                {
                    nUnitAddr = struCnfg.struLayerInfo[nInt].stuViewInfo[j].nViewUnitId[k];

                    memset(data, 15, SEND_PACKET_DATA_SIZE);

                    switch( struCnfp.nSensorDim[struGsh.nLayer][j])
                    {
                    case 1:
                        data[0] = 15+struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibratePixelRed;
                        data[1] = 15+struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibratePixelRedVert[k];
                        mySerial.pushCom1CmdData(CMD_CAMERA_PIXEL_SHIFT, 0, board, nUnitAddr, data, 3);
                        break;
                    case 2:
                        data[0] = 15+struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibratePixelRed;
                        data[1] = 15+struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibratePixelRedVert[k];
                        data[2] = 15+struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibratePixelGreen;
                        data[3] = 15+struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibratePixelGreenVert[k];
                        mySerial.pushCom1CmdData(CMD_CAMERA_PIXEL_SHIFT, 0, board, nUnitAddr, data, 3);
                        break;
                    case 3:
                        data[0] = 15+struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibratePixelRed;
                        data[1] = 15+struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibratePixelRedVert[k];
                        data[2] = 15+struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibratePixelGreen;
                        data[3] = 15+struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibratePixelGreenVert[k];
                        data[4] = 15+struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibratePixelBlue;
                        data[5] = 15+struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibratePixelBlueVert[k];
                        mySerial.pushCom1CmdData(CMD_CAMERA_PIXEL_SHIFT, 0, board, nUnitAddr, data, 3);
                        break;
                    case 4:
                        data[0] = 15+struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibratePixelRed;
                        data[1] = 15+struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibratePixelRedVert[k];
                        data[2] = 15+struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibratePixelGreen;
                        data[3] = 15+struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibratePixelGreenVert[k];
                        data[4] = 15+struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibratePixelBlue;
                        data[5] = 15+struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibratePixelBlueVert[k];
                        data[6] = 15+struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibratePixelNir;
                        data[7] = 15+struCnfg.struLayerInfo[i].stuCameraInfo[j].nCalibratePixelNirVert[k];
                        mySerial.pushCom1CmdData(CMD_CAMERA_PIXEL_SHIFT, 0, board, nUnitAddr, data, 3);
                        break;
                    default:
                        break;
                    }
                }
            }



        }
    }
    else
    {
        board = struCnfg.struLayerInfo[nInt].nViewBoardType[nViewId];
        nUnitAddr = struCnfg.struLayerInfo[nInt].stuViewInfo[nViewId].nViewUnitId[nUnit];

        memset(data, 15, SEND_PACKET_DATA_SIZE);
        switch( struCnfp.nSensorDim[struGsh.nLayer][nViewId])
        {
        case 1:
            data[0] = 15+struCnfg.struLayerInfo[i].stuCameraInfo[nViewId].nCalibratePixelRed;
            data[1] = 15+struCnfg.struLayerInfo[i].stuCameraInfo[nViewId].nCalibratePixelRedVert[nUnit];
            mySerial.pushCom1CmdData(CMD_CAMERA_PIXEL_SHIFT, 0, board, nUnitAddr, data, 3);
            break;
        case 2:
            data[0] = 15+struCnfg.struLayerInfo[i].stuCameraInfo[nViewId].nCalibratePixelRed;
            data[1] = 15+struCnfg.struLayerInfo[i].stuCameraInfo[nViewId].nCalibratePixelRedVert[nUnit];
            data[2] = 15+struCnfg.struLayerInfo[i].stuCameraInfo[nViewId].nCalibratePixelGreen;
            data[3] = 15+struCnfg.struLayerInfo[i].stuCameraInfo[nViewId].nCalibratePixelGreenVert[nUnit];
            mySerial.pushCom1CmdData(CMD_CAMERA_PIXEL_SHIFT, 0, board, nUnitAddr, data, 3);
            break;
        case 3:
            data[0] = 15+struCnfg.struLayerInfo[i].stuCameraInfo[nViewId].nCalibratePixelRed;
            data[1] = 15+struCnfg.struLayerInfo[i].stuCameraInfo[nViewId].nCalibratePixelRedVert[nUnit];
            data[2] = 15+struCnfg.struLayerInfo[i].stuCameraInfo[nViewId].nCalibratePixelGreen;
            data[3] = 15+struCnfg.struLayerInfo[i].stuCameraInfo[nViewId].nCalibratePixelGreenVert[nUnit];
            data[4] = 15+struCnfg.struLayerInfo[i].stuCameraInfo[nViewId].nCalibratePixelBlue;
            data[5] = 15+struCnfg.struLayerInfo[i].stuCameraInfo[nViewId].nCalibratePixelBlueVert[nUnit];
            mySerial.pushCom1CmdData(CMD_CAMERA_PIXEL_SHIFT, 0, board, nUnitAddr, data, 3);
            break;
        case 4:
            data[0] = 15+struCnfg.struLayerInfo[i].stuCameraInfo[nViewId].nCalibratePixelRed;
            data[1] = 15+struCnfg.struLayerInfo[i].stuCameraInfo[nViewId].nCalibratePixelRedVert[nUnit];
            data[2] = 15+struCnfg.struLayerInfo[i].stuCameraInfo[nViewId].nCalibratePixelGreen;
            data[3] = 15+struCnfg.struLayerInfo[i].stuCameraInfo[nViewId].nCalibratePixelGreenVert[nUnit];
            data[4] = 15+struCnfg.struLayerInfo[i].stuCameraInfo[nViewId].nCalibratePixelBlue;
            data[5] = 15+struCnfg.struLayerInfo[i].stuCameraInfo[nViewId].nCalibratePixelBlueVert[nUnit];
            data[6] = 15+struCnfg.struLayerInfo[i].stuCameraInfo[nViewId].nCalibratePixelNir;
            data[7] = 15+struCnfg.struLayerInfo[i].stuCameraInfo[nViewId].nCalibratePixelNirVert[nUnit];
            mySerial.pushCom1CmdData(CMD_CAMERA_PIXEL_SHIFT, 0, board, nUnitAddr, data, 3);
            break;
        default:
            break;
        }

    }
}

/* 红外TDI使能 */
void GlobalFlow::resetCameraNirTdiEnable()
{
    int i, j, k;
    int nInt=0, board, nUnitAddr;
    char data[SEND_PACKET_DATA_SIZE];

    for(i = 0; i < struCnfg.nLayerTotal; i++)  //层
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)  // 视
        {
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
            if(struCnfp.nViewSensorType[0][j] == SENSOR_C1)
            {
                board = struCnfg.struLayerInfo[i].nViewBoardType[j];

                memset(data, 15, SEND_PACKET_DATA_SIZE);

                data[0] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nNirTdiEnable;
                data[1] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nNirUpdownEnable;

                mySerial.pushCom1CmdData(CMD_CAMERA_TDI_ENABLE, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
            }
        }
    }
}

/* 红外坏点纠错 */
void GlobalFlow::resetCameraNirDotCorrectiveRG(int nLayerId, int nViewId, int nUnit)
{
    int i, j,ud;;
    int tmpIndex = 0;
    int nInt=0, board, nUnitAddr;
    char data[SEND_PACKET_DATA_SIZE];

    board = struCnfg.struLayerInfo[nInt].nViewBoardType[nViewId];
    nUnitAddr = struCnfg.struLayerInfo[nInt].stuViewInfo[nViewId].nViewUnitId[nUnit];

    /* 红色坏点纠错 */
    for(ud = 0;ud <2;ud++)
    {
        for(i = 0; i < 5; i++)
        {
            memset(data, 0, SEND_PACKET_DATA_SIZE);
            data[0] = ud;
            data[1] = i;
            for(j = 0; j < 14; j++)
            {
                if((i == 4)&&(j == 8))  //第5个包只要8个字节
                {
                    break;
                }

                tmpIndex = (i < 4) ? i*112+(14-j)*8 : i*112+(8-j)*8;

                data[2+j] = (struGsh.nNirDotCorrectiveRed[nViewId][nUnitAddr][ud][tmpIndex-1]<<7)|(struGsh.nNirDotCorrectiveRed[nViewId][nUnitAddr][ud][tmpIndex-2]<<6)|(struGsh.nNirDotCorrectiveRed[nViewId][nUnitAddr][ud][tmpIndex-3]<<5)
                        |(struGsh.nNirDotCorrectiveRed[nViewId][nUnitAddr][ud][tmpIndex-4]<<4)|(struGsh.nNirDotCorrectiveRed[nViewId][nUnitAddr][ud][tmpIndex-5]<<3)|(struGsh.nNirDotCorrectiveRed[nViewId][nUnitAddr][ud][tmpIndex-6]<<2)
                        |(struGsh.nNirDotCorrectiveRed[nViewId][nUnitAddr][ud][tmpIndex-7]<<1)|(struGsh.nNirDotCorrectiveRed[nViewId][nUnitAddr][ud][tmpIndex-8]);
            }
            mySerial.pushCom1CmdData(CMD_CAMERA_DOT_CORRECTION, nInt, board, nUnitAddr, data, 3);
        }
    }

    /* 绿色坏点纠错 */
    for(ud = 0;ud <2;ud++)
    {
        for(i = 0; i < 5; i++)
        {
            memset(data, 0, SEND_PACKET_DATA_SIZE);
            data[0] = ud+2;
            data[1] = i;
            for(j = 0; j < 14; j++)
            {
                if((i == 4)&&(j == 8))  //第5个包只要8个字节
                {
                    break;
                }
                tmpIndex = (i < 4) ? i*112+(14-j)*8 : i*112+(8-j)*8;
                data[2+j] = (struGsh.nNirDotCorrectiveGreen[nViewId][nUnitAddr][ud][tmpIndex-1]<<7)|(struGsh.nNirDotCorrectiveGreen[nViewId][nUnitAddr][ud][tmpIndex-2]<<6)|(struGsh.nNirDotCorrectiveGreen[nViewId][nUnitAddr][ud][tmpIndex-3]<<5)
                        |(struGsh.nNirDotCorrectiveGreen[nViewId][nUnitAddr][ud][tmpIndex-4]<<4)|(struGsh.nNirDotCorrectiveGreen[nViewId][nUnitAddr][ud][tmpIndex-5]<<3)|(struGsh.nNirDotCorrectiveGreen[nViewId][nUnitAddr][ud][tmpIndex-6]<<2)
                        |(struGsh.nNirDotCorrectiveGreen[nViewId][nUnitAddr][ud][tmpIndex-7]<<1)|(struGsh.nNirDotCorrectiveGreen[nViewId][nUnitAddr][ud][tmpIndex-8]);
            }
            mySerial.pushCom1CmdData(CMD_CAMERA_DOT_CORRECTION, nInt, board, nUnitAddr, data, 3);
        }
    }

    /* 蓝色坏点纠错 */
    for(ud = 0;ud <2;ud++)
    {
        for(i = 0; i < 5; i++)
        {
            memset(data, 0, SEND_PACKET_DATA_SIZE);
            data[0] = ud+4;
            data[1] = i;
            for(j = 0; j < 14; j++)
            {
                if((i == 4)&&(j == 8))  //第5个包只要8个字节
                {
                    break;
                }
                tmpIndex = (i < 4) ? i*112+(14-j)*8 : i*112+(8-j)*8;
                data[2+j] = (struGsh.nNirDotCorrectiveBlue[nViewId][nUnitAddr][ud][tmpIndex-1]<<7)|(struGsh.nNirDotCorrectiveBlue[nViewId][nUnitAddr][ud][tmpIndex-2]<<6)|(struGsh.nNirDotCorrectiveBlue[nViewId][nUnitAddr][ud][tmpIndex-3]<<5)
                        |(struGsh.nNirDotCorrectiveBlue[nViewId][nUnitAddr][ud][tmpIndex-4]<<4)|(struGsh.nNirDotCorrectiveBlue[nViewId][nUnitAddr][ud][tmpIndex-5]<<3)|(struGsh.nNirDotCorrectiveBlue[nViewId][nUnitAddr][ud][tmpIndex-6]<<2)
                        |(struGsh.nNirDotCorrectiveBlue[nViewId][nUnitAddr][ud][tmpIndex-7]<<1)|(struGsh.nNirDotCorrectiveBlue[nViewId][nUnitAddr][ud][tmpIndex-8]);
            }
            mySerial.pushCom1CmdData(CMD_CAMERA_DOT_CORRECTION, nInt, board, nUnitAddr, data, 3);
        }
    }

    /* N色坏点纠错 */
    for(ud = 0;ud <2;ud++)
    {
        for(i = 0; i < 5; i++)
        {
            memset(data, 0, SEND_PACKET_DATA_SIZE);
            data[0] = ud+6;
            data[1] = i;
            for(j = 0; j < 14; j++)
            {
                if((i == 4)&&(j == 8))  //第5个包只要8个字节
                {
                    break;
                }
                tmpIndex = (i < 4) ? i*112+(14-j)*8 : i*112+(8-j)*8;
                data[2+j] = (struGsh.nNirDotCorrectiveN[nViewId][nUnitAddr][ud][tmpIndex-1]<<7)|(struGsh.nNirDotCorrectiveN[nViewId][nUnitAddr][ud][tmpIndex-2]<<6)|(struGsh.nNirDotCorrectiveN[nViewId][nUnitAddr][ud][tmpIndex-3]<<5)
                        |(struGsh.nNirDotCorrectiveN[nViewId][nUnitAddr][ud][tmpIndex-4]<<4)|(struGsh.nNirDotCorrectiveN[nViewId][nUnitAddr][ud][tmpIndex-5]<<3)|(struGsh.nNirDotCorrectiveN[nViewId][nUnitAddr][ud][tmpIndex-6]<<2)
                        |(struGsh.nNirDotCorrectiveN[nViewId][nUnitAddr][ud][tmpIndex-7]<<1)|(struGsh.nNirDotCorrectiveN[nViewId][nUnitAddr][ud][tmpIndex-8]);
            }
            mySerial.pushCom1CmdData(CMD_CAMERA_DOT_CORRECTION, nInt, board, nUnitAddr, data, 3);
        }
    }
}

/* 红外坏点纠错 */
void GlobalFlow::resetCameraWaveType(int nLayerId, int nViewId, int nUnit)
{
    int i, j, k;
    int nInt=0, board, nUnitAddr;
    char data[SEND_PACKET_DATA_SIZE];

    memset(data, 0, SEND_PACKET_DATA_SIZE);

    board = struCnfg.struLayerInfo[nInt].nViewBoardType[nViewId];
    nUnitAddr = struCnfg.struLayerInfo[nInt].stuViewInfo[nViewId].nViewUnitId[nUnit];

    data[0] = struGsh.nSignalDispType;

    mySerial.pushCom1CmdData(CMD_CAMERA_WAVE_DISPLAY_TYPE, nInt, board, nUnitAddr, data, 3);
}


/* 发送色选识别参数 */
void GlobalFlow::resetSortParams()
{
    /* 重置物料算法参数 */
    resetAllArithParams();

    /* 重置背景颜色设置 */
    resetBackgroundColorRG(true, 0, 0);

    /* 设置喷阀时间 */
    resetEjectTime();

    //设置控碎
    resetArithRejectRatio();

    /* 设置传染、切边缘、正反选... */
    //    resetImageDealSize();

    /* 重置剔除设置 */
    resetTick();

    /* 重置算法使能 */
    //   resetArithmeticEnable();
}

/* 计算获取识别组 相机单元地址， 含是否分割 */
int GlobalFlow::getIdentifyGroupAddr(int nLayer, int nViewChain, int nGroup, int nUnit)
{
    int nUnitAddr = 0;

    switch (struCnfg.struLayerInfo[nLayer].stuViewInfo[nViewChain].stuIdenGupInfo[nGroup].nUnitGupSplice[nUnit] ) {
    case 0:
        nUnitAddr = struCnfg.struLayerInfo[nLayer].stuViewInfo[nViewChain].stuIdenGupInfo[nGroup].nUnitGupId[nUnit];
        break;
    case 1:
        nUnitAddr = 1 << 6 | struCnfg.struLayerInfo[nLayer].stuViewInfo[nViewChain].stuIdenGupInfo[nGroup].nUnitGupId[nUnit];
        break;
    case 2:
        nUnitAddr = 1 << 7 | struCnfg.struLayerInfo[nLayer].stuViewInfo[nViewChain].stuIdenGupInfo[nGroup].nUnitGupId[nUnit];
        break;
    default:
        break;
    }
    return nUnitAddr;
}

void GlobalFlow::setSvmParas(int layer, int view, int group, int svmIndex)
{
    int cntNum = 100000;

    struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nModeType = struCnfg.n_svmType;

    if(struGsh.m_svmcoefs[9] > 0){
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoCons[0] = 0;
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoCons[1] = struGsh.m_svmcoefs[9]*cntNum;
    }else{
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoCons[0] = 1;
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoCons[1] = -1*struGsh.m_svmcoefs[9]*cntNum;
    }

    if(struGsh.m_svmcoefs[0] > 0){
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoR[0] = 0;
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoR[1] = struGsh.m_svmcoefs[0]*cntNum;
    }else{
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoR[0] = 1;
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoR[1] = -1*struGsh.m_svmcoefs[0]*cntNum;
    }

    if(struGsh.m_svmcoefs[1] > 0){
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoG[0] = 0;
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoG[1] = struGsh.m_svmcoefs[1]*cntNum;
    }else{
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoG[0] = 1;
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoG[1] = -1*struGsh.m_svmcoefs[1]*cntNum;
    }

    if(struGsh.m_svmcoefs[2] > 0){
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoB[0] = 0;
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoB[1] = struGsh.m_svmcoefs[2]*cntNum;
    }else{
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoB[0] = 1;
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoB[1] = -1*struGsh.m_svmcoefs[2]*cntNum;
    }

    if(struGsh.m_svmcoefs[3] > 0){
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoRG[0] = 0;
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoRG[1] = struGsh.m_svmcoefs[3]*cntNum;
    }else{
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoRG[0] = 1;
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoRG[1] = -1*struGsh.m_svmcoefs[3]*cntNum;
    }

    if(struGsh.m_svmcoefs[4] > 0){
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoGB[0] = 0;
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoGB[1] = struGsh.m_svmcoefs[4]*cntNum;
    }else{
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoGB[0] = 1;
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoGB[1] = -1*struGsh.m_svmcoefs[4]*cntNum;
    }

    if(struGsh.m_svmcoefs[5] > 0){
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoRB[0] = 0;
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoRB[1] = struGsh.m_svmcoefs[5]*cntNum;
    }else{
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoRB[0] = 1;
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoRB[1] = -1*struGsh.m_svmcoefs[5]*cntNum;
    }

    if(struGsh.m_svmcoefs[6] > 0){
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoRR[0] = 0;
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoRR[1] = (struCnfg.n_svmType == 0) ? 0 :struGsh.m_svmcoefs[6]*cntNum;
    }else{
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoRR[0] = 1;
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoRR[1] = (struCnfg.n_svmType == 0) ? 0 :(-1*struGsh.m_svmcoefs[6]*cntNum);
    }

    if(struGsh.m_svmcoefs[7] > 0){
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoGG[0] = 0;
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoGG[1] = (struCnfg.n_svmType == 0) ? 0 :struGsh.m_svmcoefs[7]*cntNum;
    }else
    {
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoGG[0] = 1;
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoGG[1] = (struCnfg.n_svmType == 0) ? 0 :(-1*struGsh.m_svmcoefs[7]*cntNum);
    }

    if(struGsh.m_svmcoefs[8] > 0){
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoBB[0] = 0;
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoBB[1] = (struCnfg.n_svmType == 0) ? 0 :struGsh.m_svmcoefs[8]*cntNum;
    }else{
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoBB[0] = 1;
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoBB[1] = (struCnfg.n_svmType == 0) ? 0 :(-1*struGsh.m_svmcoefs[8]*cntNum);
    }

    struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nSens = struGsh.nSvmSimulateSens;
    struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nRow= struGsh.nSvmRow;
    struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nPercent= struGsh.nSvmPercent;
    struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nConsExpandedValue = struGsh.nSvmExpandValue;
}

void GlobalFlow::setShapeSvmParas(int layer, int view, int group, int shapeSvmIndex)
{
    int cntNum = 100000;
#if 1
    if(struGsh.m_Shapecoef[9] > 0){
        struCnfp.struGroupIdentify[layer][view][group].struShapeSvm[shapeSvmIndex].nQuoCons[0] = 0;
        struCnfp.struGroupIdentify[layer][view][group].struShapeSvm[shapeSvmIndex].nQuoCons[1] = struGsh.m_Shapecoef[9]*cntNum;
    }else{
        struCnfp.struGroupIdentify[layer][view][group].struShapeSvm[shapeSvmIndex].nQuoCons[0] = 1;
        struCnfp.struGroupIdentify[layer][view][group].struShapeSvm[shapeSvmIndex].nQuoCons[1] = -1*struGsh.m_Shapecoef[9]*cntNum;
    }

    if(struGsh.m_Shapecoef[0] > 0){
        struCnfp.struGroupIdentify[layer][view][group].struShapeSvm[shapeSvmIndex].nQuoA[0] = 0;
        struCnfp.struGroupIdentify[layer][view][group].struShapeSvm[shapeSvmIndex].nQuoA[1] = struGsh.m_Shapecoef[0]*cntNum;
    }else{
        struCnfp.struGroupIdentify[layer][view][group].struShapeSvm[shapeSvmIndex].nQuoA[0] = 1;
        struCnfp.struGroupIdentify[layer][view][group].struShapeSvm[shapeSvmIndex].nQuoA[1] = -1*struGsh.m_Shapecoef[0]*cntNum;
    }

    if(struGsh.m_Shapecoef[1] > 0){
        struCnfp.struGroupIdentify[layer][view][group].struShapeSvm[shapeSvmIndex].nQuoB[0] = 0;
        struCnfp.struGroupIdentify[layer][view][group].struShapeSvm[shapeSvmIndex].nQuoB[1] = struGsh.m_Shapecoef[1]*cntNum;
    }else{
        struCnfp.struGroupIdentify[layer][view][group].struShapeSvm[shapeSvmIndex].nQuoB[0] = 1;
        struCnfp.struGroupIdentify[layer][view][group].struShapeSvm[shapeSvmIndex].nQuoB[1] = -1*struGsh.m_Shapecoef[1]*cntNum;
    }

    if(struGsh.m_Shapecoef[2] > 0){
        struCnfp.struGroupIdentify[layer][view][group].struShapeSvm[shapeSvmIndex].nQuoC[0] = 0;
        struCnfp.struGroupIdentify[layer][view][group].struShapeSvm[shapeSvmIndex].nQuoC[1] = struGsh.m_Shapecoef[2]*cntNum;
    }else{
        struCnfp.struGroupIdentify[layer][view][group].struShapeSvm[shapeSvmIndex].nQuoC[0] = 1;
        struCnfp.struGroupIdentify[layer][view][group].struShapeSvm[shapeSvmIndex].nQuoC[1] = -1*struGsh.m_Shapecoef[2]*cntNum;
    }

    if(struGsh.m_Shapecoef[3] > 0){
        struCnfp.struGroupIdentify[layer][view][group].struShapeSvm[shapeSvmIndex].nQuoD[0] = 0;
        struCnfp.struGroupIdentify[layer][view][group].struShapeSvm[shapeSvmIndex].nQuoD[1] = struGsh.m_Shapecoef[3]*cntNum;
    }else{
        struCnfp.struGroupIdentify[layer][view][group].struShapeSvm[shapeSvmIndex].nQuoD[0] = 1;
        struCnfp.struGroupIdentify[layer][view][group].struShapeSvm[shapeSvmIndex].nQuoD[1] = -1*struGsh.m_Shapecoef[3]*cntNum;
    }

    if(struGsh.m_Shapecoef[4] > 0){
        struCnfp.struGroupIdentify[layer][view][group].struShapeSvm[shapeSvmIndex].nQuoE[0] = 0;
        struCnfp.struGroupIdentify[layer][view][group].struShapeSvm[shapeSvmIndex].nQuoE[1] = struGsh.m_Shapecoef[4]*cntNum;
    }else{
        struCnfp.struGroupIdentify[layer][view][group].struShapeSvm[shapeSvmIndex].nQuoE[0] = 1;
        struCnfp.struGroupIdentify[layer][view][group].struShapeSvm[shapeSvmIndex].nQuoE[1] = -1*struGsh.m_Shapecoef[4]*cntNum;
    }

    if(struGsh.m_Shapecoef[5] > 0){
        struCnfp.struGroupIdentify[layer][view][group].struShapeSvm[shapeSvmIndex].nQuoF[0] = 0;
        struCnfp.struGroupIdentify[layer][view][group].struShapeSvm[shapeSvmIndex].nQuoF[1] = struGsh.m_Shapecoef[5]*cntNum;
    }else{
        struCnfp.struGroupIdentify[layer][view][group].struShapeSvm[shapeSvmIndex].nQuoF[0] = 1;
        struCnfp.struGroupIdentify[layer][view][group].struShapeSvm[shapeSvmIndex].nQuoF[1] = -1*struGsh.m_Shapecoef[5]*cntNum;
    }

#endif

#if 0 // 没有降维
    if(struGsh.m_svmcoefs[9] > 0){
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoCons[0] = 0;
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoCons[1] = struGsh.m_svmcoefs[9]*cntNum;
    }else{
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoCons[0] = 1;
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoCons[1] = -1*struGsh.m_svmcoefs[9]*cntNum;
    }

    if(struGsh.m_svmcoefs[0] > 0){
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoR[0] = 0;
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoR[1] = struGsh.m_svmcoefs[0]*cntNum;
    }else{
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoR[0] = 1;
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoR[1] = -1*struGsh.m_svmcoefs[0]*cntNum;
    }

    if(struGsh.m_svmcoefs[1] > 0){
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoG[0] = 0;
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoG[1] = struGsh.m_svmcoefs[1]*cntNum;
    }else{
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoG[0] = 1;
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoG[1] = -1*struGsh.m_svmcoefs[1]*cntNum;
    }

    if(struGsh.m_svmcoefs[2] > 0){
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoB[0] = 0;
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoB[1] = struGsh.m_svmcoefs[2]*cntNum;
    }else{
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoB[0] = 1;
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoB[1] = -1*struGsh.m_svmcoefs[2]*cntNum;
    }

    if(struGsh.m_svmcoefs[3] > 0){
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoRR[0] = 0;
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoRR[1] = struGsh.m_svmcoefs[3]*cntNum;
    }else{
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoRR[0] = 1;
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoRR[1] = -1*struGsh.m_svmcoefs[3]*cntNum;
    }

    if(struGsh.m_svmcoefs[4] > 0){
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoGG[0] = 0;
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoGG[1] = struGsh.m_svmcoefs[4]*cntNum;
    }else{
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoGG[0] = 1;
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoGG[1] = -1*struGsh.m_svmcoefs[4]*cntNum;
    }

    if(struGsh.m_svmcoefs[5] > 0){
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoBB[0] = 0;
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoBB[1] = struGsh.m_svmcoefs[5]*cntNum;
    }else{
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoBB[0] = 1;
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoBB[1] = -1*struGsh.m_svmcoefs[5]*cntNum;
    }

    if(struGsh.m_svmcoefs[6] > 0){
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoRG[0] = 0;
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoRG[1] = struGsh.m_svmcoefs[6]*cntNum;
    }else{
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoRG[0] = 1;
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoRG[1] = -1*struGsh.m_svmcoefs[6]*cntNum;
    }

    if(struGsh.m_svmcoefs[7] > 0){
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoGB[0] = 0;
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoGB[1] = struGsh.m_svmcoefs[7]*cntNum;
    }else{
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoGB[0] = 1;
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoGB[1] = -1*struGsh.m_svmcoefs[7]*cntNum;
    }

    if(struGsh.m_svmcoefs[8] > 0){
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoRB[0] = 0;
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoRB[1] = struGsh.m_svmcoefs[8]*cntNum;
    }else{
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoRB[0] = 1;
        struCnfp.struGroupIdentify[layer][view][group].struSvm[svmIndex].nQuoRB[1] = -1*struGsh.m_svmcoefs[8]*cntNum;
    }
#endif
}

/* 计算获取剔除组喷阀板发送地址 */
int GlobalFlow::getTickGroupAddr(int nLayer, int nVavleChain, int nGroup, int nVavleBoard)
{
    int nVavleAddr = 0;

    switch (struCnfg.struLayerInfo[nLayer].stuVavleInfo[nVavleChain].stuTickGupInfo[nGroup].nEjBoardGupSplice[nVavleBoard] ) {
    case 0:
    case 1:
        nVavleAddr = struCnfg.struLayerInfo[nLayer].stuVavleInfo[nVavleChain].stuTickGupInfo[nGroup].nEjBoardGupId[nVavleBoard];
        break;
    case 2:
        nVavleAddr = 1 << 7 | struCnfg.struLayerInfo[nLayer].stuVavleInfo[nVavleChain].stuTickGupInfo[nGroup].nEjBoardGupId[nVavleBoard];
        break;
    default:
        break;
    }
    /*
    switch (struCnfg.struLayerInfo[nLayer].stuVavleInfo[nVavleChain].stuTickGupInfo[nGroup].nEjBoardGupSplice[nVavleBoard] ) {
    case 0:
        nVavleAddr = struCnfg.struLayerInfo[nLayer].stuVavleInfo[nVavleChain].stuTickGupInfo[nGroup].nEjBoardGupId[nVavleBoard];
        break;
    case 1:
        nVavleAddr = 1 << 6 | struCnfg.struLayerInfo[nLayer].stuVavleInfo[nVavleChain].stuTickGupInfo[nGroup].nEjBoardGupId[nVavleBoard];
        break;
    case 2:
        nVavleAddr = 1 << 7 | struCnfg.struLayerInfo[nLayer].stuVavleInfo[nVavleChain].stuTickGupInfo[nGroup].nEjBoardGupId[nVavleBoard];
        break;
    default:
        break;
    }
*/
    return nVavleAddr;
}

int GlobalFlow::getTickGroupVavleNum(int nLayer, int nVavleChain, int nGroup)
{
    int vavleNum = 0;
    stru_vavle_group_info vavleinfo;
    memcpy(&vavleinfo,&struCnfg.struLayerInfo[nLayer].stuVavleInfo[nVavleChain].stuTickGupInfo[nGroup],sizeof(stru_vavle_group_info));

    bool splice = false;
    int gupsplice = 0;
    for(int i = 0;i < vavleinfo.nEjBoardGupTotal;i++){
        if(vavleinfo.nEjBoardGupSplice[i] != 0){
            splice = true;
            gupsplice = vavleinfo.nEjBoardGupSplice[i];
            break;
        }
    }
    if(splice){
        switch (struCnfg.struLayerInfo[nLayer].nSpliceType) {
        case 1://1:3
            if(gupsplice == 1){
                vavleNum = struCnfg.struLayerInfo[nLayer].nEjectorsPerChute*vavleinfo.nEjBoardGupTotal-struCnfg.struLayerInfo[nLayer].nEjectorsPerChute/4*3;
            }else{
                vavleNum = struCnfg.struLayerInfo[nLayer].nEjectorsPerChute*vavleinfo.nEjBoardGupTotal-struCnfg.struLayerInfo[nLayer].nEjectorsPerChute/4*1;
            }
            break;
        case 2://1:1
            vavleNum = struCnfg.struLayerInfo[nLayer].nEjectorsPerChute*vavleinfo.nEjBoardGupTotal-struCnfg.struLayerInfo[nLayer].nEjectorsPerChute/2;
            break;
        case 3://3:1
            if(gupsplice == 1){
                vavleNum = struCnfg.struLayerInfo[nLayer].nEjectorsPerChute*vavleinfo.nEjBoardGupTotal-struCnfg.struLayerInfo[nLayer].nEjectorsPerChute/4*1;
            }else{
                vavleNum = struCnfg.struLayerInfo[nLayer].nEjectorsPerChute*vavleinfo.nEjBoardGupTotal-struCnfg.struLayerInfo[nLayer].nEjectorsPerChute/4*3;
            }
            break;
        default:
            break;
        }


    }else{
        vavleNum = struCnfg.struLayerInfo[nLayer].nEjectorsPerChute*vavleinfo.nEjBoardGupTotal;
    }
#if 0
    switch(vavleinfo.nEjBoardGupSplice[vavleinfo.nEjBoardGupTotal-1]){
    case 0:
        vavleNum = struCnfg.struLayerInfo[nLayer].nEjectorsPerChute*vavleinfo.nEjBoardGupTotal;
        break;
    case 1:
    case 2:
        vavleNum = struCnfg.struLayerInfo[nLayer].nEjectorsPerChute*vavleinfo.nEjBoardGupTotal-struCnfg.struLayerInfo[nLayer].nEjectorsPerChute/2;
        break;
    default:
        break;
    }
#endif
    return vavleNum;
}

/* 获取某视某链上的具体相机分辨率 */
int GlobalFlow::getSensorPixels(int nLayer, int nView)
{
    int curPixels = 0;

    switch(struCnfp.nSensorPixel[nLayer][nView])
    {
    case SENSOR_PIXELS_256:
        curPixels = 256;
        break;
    case SENSOR_PIXELS_512:
        curPixels = 512;
        break;
    case SENSOR_PIXELS_1024:
    case SENSOR_PIXELS_1024_TD:
        curPixels = 1024;
        break;
    case SENSOR_PIXELS_2048:
    case SENSOR_PIXELS_2048_GTD:
        curPixels = 2048;
        break;
    case SENSOR_PIXELS_2048_TD:
    case SENSOR_PIXELS_2048_XTD:
        if(struGsh.nCamTdMode == 2)
            curPixels = 1024;
        else
            curPixels = 2048;
        break;
    default:
        curPixels = 2048;
        break;
    }

    return curPixels;
}

/* 重置所有算法参数 */
void GlobalFlow::resetAllArithParams()
{
    int i, j, k;

    for(i = 0; i < struCnfg.nLayerTotal; i++)  //层
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)  // 视
        {
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
            for(k = 0; k <struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal; k++)
            {
                resetMaterial(i, j, k, 1, 0, 1);
            }
        }
    }
    if (++struGsh.allArithParamsResetCnts ){
        for(i = 0; i < struCnfg.nLayerTotal; i++)  //层
        {
            for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)  // 视
            {
                if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
                for(k = 0; k <struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal; k++)
                {
                    resetMaterial(i, j, k, 1, 0, 1);
                }
            }
        }
    }

}

/* 重置背景颜色设置 */
void GlobalFlow::resetBackgroundColorRG(bool isAll, int nLayer, int nView)
{
    int i=0,j = 0, k =0;
    int nInt=0, board, nUnitAddr = BOARD_TYPE_ALL_IN_CHAIN;  // 0xff表示当前 视的所有相机板

    char data[SEND_PACKET_DATA_SIZE];

    int s_mode,e_mode;

    if(isAll)   //是否整机发送
    {
        for(i = 0; i < struCnfg.nLayerTotal; i++)  //层
        {
            for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)  // 视
            {
                if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;

                if(struCnfp.nSensorPixel[i][j] == SENSOR_PIXELS_1024_TD
                        || struCnfp.nSensorPixel[i][j] == SENSOR_PIXELS_2048_TD
                        || struCnfp.nSensorPixel[i][j] == SENSOR_PIXELS_2048_XTD)
                {
                    s_mode = 1;
                    e_mode = 2;
                }
                else
                {
                    s_mode = 0;
                    e_mode = 0;
                }
                board = struCnfg.struLayerInfo[i].nViewBoardType[j];

                memset(data, 0, SEND_PACKET_DATA_SIZE);

                for(k = s_mode;k <= e_mode;k++)
                {
                    data[5] = k;
                    switch(struCnfp.nBackgroundColor[i][j][k])
                    {
                    case BACKGROUND_COLOR_RED:
                    case BACKGROUND_COLOR_GREEN:
                    case BACKGROUND_COLOR_BLUE:
                        data[0] = struCnfp.nBackgroundColor[i][j][k];
                        data[1] = struCnfp.nBackgroundColorValueLow[i][j][k];
                        data[2] = (int)(struCnfp.nBackgroundColorProLow[i][j][k]*2.55+0.5);
                        break;
                    case BACKGROUND_COLOR_GREY:
                        data[0] = struCnfp.nBackgroundColor[i][j][k]+1;     //灰度和黄背景颜色需加1
                        data[1] = struCnfp.nBackgroundColorValueLow[i][j][k];
                        data[2] = struCnfp.nBackgroundColorValueUp[i][j][k];
                        break;
                    case BACKGROUND_COLOR_YELLOW:
                        data[0] = struCnfp.nBackgroundColor[i][j][k]+1;     //灰度和黄背景颜色需加1
                        data[1] = struCnfp.nBackgroundColorValueLow[i][j][k];
                        data[2] = struCnfp.nBackgroundColorValueUp[i][j][k];
                        data[3] = struCnfp.nBackgroundColorValueVLow[i][j][k];
                        data[4] = struCnfp.nBackgroundColorValueVUp[i][j][k];
                        break;
                    case BACKGROUND_COLOR_BLACK_WHITE:
                        data[0] = 4;     //黑白与红外颠倒数值索引
                        data[1] = struCnfp.nBackgroundColorValueLow[i][j][k];
                        data[2] = struCnfp.nBackgroundColorValueUp[i][j][k];
                        data[3] = struCnfp.nBackgroundColorSatUp[i][j][k];
                        break;
                    case BACKGROUND_COLOR_NIR:              //黑白与红外颠倒数值索引
                        data[0] = 3;
                        data[1] = struCnfp.nBackgroundColorValueLow[i][j][k];
                        data[2] = struCnfp.nBackgroundColorValueUp[i][j][k];
                        break;
                    default:
                        break;
                    }
                    mySerial.pushCom1CmdData(CMD_CAMERA_BACKGROUND_SET, nInt, board, nUnitAddr, data, 3);
                }
            }
        }
    }
    else
    {
        if(struCnfp.nSensorPixel[0][nView] == SENSOR_PIXELS_1024_TD
                || struCnfp.nSensorPixel[0][nView] == SENSOR_PIXELS_2048_TD
                || struCnfp.nSensorPixel[0][nView] == SENSOR_PIXELS_2048_XTD)
        {
            s_mode = 1;
            e_mode = 2;
        }
        else
        {
            s_mode = 0;
            e_mode = 0;
        }

        board = struCnfg.struLayerInfo[nLayer].nViewBoardType[nView];
        memset(data, 0, SEND_PACKET_DATA_SIZE);
        for(k = s_mode;k <= e_mode;k++)
        {
            data[5] = k;
            switch(struCnfp.nBackgroundColor[i][j][k])
            {
            case BACKGROUND_COLOR_RED:
            case BACKGROUND_COLOR_GREEN:
            case BACKGROUND_COLOR_BLUE:
                data[0] = struCnfp.nBackgroundColor[nLayer][nView][k];
                data[1] = struCnfp.nBackgroundColorValueLow[nLayer][nView][k];
                data[2] = (int)(struCnfp.nBackgroundColorProLow[nLayer][nView][k]*2.55+0.5);
                break;
            case BACKGROUND_COLOR_GREY:
                data[0] = struCnfp.nBackgroundColor[nLayer][nView][k]+1;
                data[1] = struCnfp.nBackgroundColorValueLow[nLayer][nView][k];
                data[2] = struCnfp.nBackgroundColorValueUp[nLayer][nView][k];
                break;
            case BACKGROUND_COLOR_YELLOW:
                data[0] = struCnfp.nBackgroundColor[nLayer][nView][k]+1;
                data[1] = struCnfp.nBackgroundColorValueLow[nLayer][nView][k];
                data[2] = struCnfp.nBackgroundColorValueUp[nLayer][nView][k];
                data[3] = struCnfp.nBackgroundColorValueVLow[nLayer][nView][k];
                data[4] = struCnfp.nBackgroundColorValueVUp[nLayer][nView][k];
                break;
            case BACKGROUND_COLOR_BLACK_WHITE:
                data[0] = 4;
                data[1] = struCnfp.nBackgroundColorValueLow[nLayer][nView][k];
                data[2] = struCnfp.nBackgroundColorValueUp[nLayer][nView][k];
                data[3] = struCnfp.nBackgroundColorSatUp[nLayer][nView][k];
                break;
            case BACKGROUND_COLOR_NIR:
                data[0] = 3;
                data[1] = struCnfp.nBackgroundColorValueLow[nLayer][nView][k];
                data[2] = struCnfp.nBackgroundColorValueUp[nLayer][nView][k];
                break;
            default:
                break;
            }
            mySerial.pushCom1CmdData(CMD_CAMERA_BACKGROUND_SET, nInt, board, nUnitAddr, data, 3);
        }
    }
}

void GlobalFlow::resetTick()
{
    resetCameraBadNumRG();                         //坏点个数

    resetCameraReverseSortRG();                    //正反选模式

    resetCameraImpureRG();                         //传染尺寸

    resetCameraCenterSize();                       //中心化尺寸

    resetCameraEdgeCutRG();                        //边缘切除

    resetEjectFrontRearLogicRG();                  // 前后视相与相或
}

/* 发送接口板延迟时间, 吹气时间，等待时间, 吹气方式 */
void GlobalFlow::resetEjectTime()
{
    int i, j, k, m, n;

    //延迟时间
    resetEjectDelayTimeRG();

    //吹气时间
    resetEjectDurationTimeRG();

    //等待时间
    resetEjectWaitTimeRG();

    //尾气时间
    resetEjectTailTimeRG();

    //吹气方式
    resetEjectVavleModeRG();
}

/* 发送相机板延迟时间*/
int GlobalFlow::resetEjectDelayTimeRG()
{
    int i, j, k, m, n;
    int nDelayTime[MAX_LAYER][MAX_VIEW][MAX_GROUP_IDENTIFY];
    int nDelayTime2[MAX_LAYER][MAX_VIEW][MAX_GROUP_IDENTIFY];
    int nDelayTime3[MAX_LAYER][MAX_VIEW][MAX_GROUP_IDENTIFY];
    int nDelayTime4[MAX_LAYER][MAX_VIEW][MAX_GROUP_IDENTIFY];
    int nModelDelayTime[MAX_LAYER][MAX_VIEW][MAX_GROUP_IDENTIFY];
    int nMinDelay = 511;
    int nInt=0, board, nUnitAddr;
    char data[SEND_PACKET_DATA_SIZE];
    char dataH[SEND_PACKET_DATA_SIZE];
    char dataM[SEND_PACKET_DATA_SIZE];

    char s_data[SEND_PACKET_DATA_SIZE];
    char s_dataH[SEND_PACKET_DATA_SIZE];


    int tmpVavleId = 0;
    int bigDuration = 0;
    int spliceInCurChain = 0;
    /*  1. 计算延迟时间是否大于0 */
    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
            if(struCnfg.nMachineAttr == MACHINE_TYPE_BELT_DOUBLE)
            {
                tmpVavleId = j/4;
            }
            else
            {
                tmpVavleId = 0;
            }
            for(k = 0; k < struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal; k++)
            {
                /* half duration for centre position*/

                //dp mode          //取较大值
                bigDuration = (struCnfp.struGroupTick[i][tmpVavleId][k].nEjectDuration >= struCnfp.struGroupTick[i][tmpVavleId][k].nEjectDuration2)
                        ? struCnfp.struGroupTick[i][tmpVavleId][k].nEjectDuration : struCnfp.struGroupTick[i][tmpVavleId][k].nEjectDuration2;
                if(struCnfp.struGroupTick[i][tmpVavleId][k].nEjwidth == 1)   //重吹
                {
                    if (struCnfg.nDPMachine)
                        nDelayTime[i][j][k] = struCnfp.struGroupIdentify[i][j][k].nEjectDelay-struCnfp.struGroupTick[i][tmpVavleId][k].nEjectDuration2/2;
                    else
                        nDelayTime[i][j][k] = struCnfp.struGroupIdentify[i][j][k].nEjectDelay-struCnfp.struGroupTick[i][tmpVavleId][k].nEjectDuration/2;
                }
                else
                {
                    nDelayTime[i][j][k] = struCnfp.struGroupIdentify[i][j][k].nEjectDelay-struCnfp.struGroupTick[i][tmpVavleId][k].nEjectDuration*1.5;
                }
                /* delay time must bigger than 0 */
                nDelayTime2[i][j][k] = struCnfp.struGroupIdentify[i][j][k].nEjectDelayShape;
                nDelayTime3[i][j][k] = struCnfp.struGroupIdentify[i][j][k].nEjectDelay3;
                nDelayTime4[i][j][k] = struCnfp.struGroupIdentify[i][j][k].nEjectDelay4;
                nModelDelayTime[i][j][k] = struCnfp.struGroupIdentify[i][j][k].nEjectDelayModel;

                nDelayTime[i][j][k] = (nDelayTime[i][j][k] > 0) ? nDelayTime[i][j][k] : 0;
                nDelayTime2[i][j][k] = (nDelayTime2[i][j][k] > 0) ? nDelayTime2[i][j][k] : 0;

                /* get min delay */
                nMinDelay = (nMinDelay > nDelayTime[i][j][k]) ? nDelayTime[i][j][k] : nMinDelay;
                nMinDelay = (nMinDelay > nDelayTime2[i][j][k]) ? nDelayTime2[i][j][k] : nMinDelay;
            }
        }
    }
    if(nMinDelay < 0)
    {
        return 0;   //延迟时间小等于0,表示错误，不能进行此修改，界面已更改，但不发送
    }

    /* 2. 发送相机板 延迟时间 高字节*/
    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        /* 发送色选延迟时间 */
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
            memset(dataH, 0, SEND_PACKET_DATA_SIZE);
            memset(s_dataH, 0, SEND_PACKET_DATA_SIZE);
            dataH[0] = j%4;  //所属相机视链, 0:前主， 1：后主， 2：前辅， 3：后辅
            s_dataH[0] = dataH[0];
            for(k = 0; k < struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal; k++)
            {
                for(m = 0; m < struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++) // 认为视的次数和吹的序号一致
                {
                    nUnitAddr = getIdentifyGroupAddr(i, j, k, m)%64;
                    if(struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nUnitGupSplice[m] == 0 ){
                        dataH[1+nUnitAddr]   = (nDelayTime[i][j][k])/256;
                        s_dataH[1+nUnitAddr] = dataH[1+nUnitAddr];
                    }
                    else if(struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nUnitGupSplice[m] == 1 ){
                        spliceInCurChain = 1;
                        dataH[1+nUnitAddr]   = (nDelayTime[i][j][k])/256;
                    }
                    else if(struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nUnitGupSplice[m] == 2){
                        spliceInCurChain = 1;
                        s_dataH[1+nUnitAddr] = (nDelayTime[i][j][k])/256;
                    }

                }
            }

            if(spliceInCurChain){
                mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME_H, nInt, board, BOARD_TYPE_ALL_IN_CHAIN_SPLIE_LEFT, dataH, 3);
                mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME_H, nInt, board, BOARD_TYPE_ALL_IN_CHAIN_SPLIE_RIGHT, s_dataH, 3);
            }
            else{
                mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME_H, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, dataH, 3);
            }


        }
        //        {
        //            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
        //            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
        //            memset(dataH, 0, SEND_PACKET_DATA_SIZE);
        //            dataH[0] = j%4;  //所属相机视链, 0:前主， 1：后主， 2：前辅， 3：后辅
        //            for(k = 0; k < struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal; k++)
        //            {
        //                for(m = 0; m < struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++) // 认为视的次数和吹的序号一致
        //                {
        //                    nUnitAddr = getIdentifyGroupAddr(i, j, k, m)%64;
        //                    dataH[1+nUnitAddr%64]  = (nDelayTime[i][j][k])/256;
        //                }
        //            }
        //           // mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME_H, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, dataH, 3);
        //        }
        mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME_H, nInt, BOARD_TYPE_INTERFACE, BOARD_TYPE_ALL_IN_CHAIN, dataH, 3);
        /* 发送形选延迟时间 */
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
            memset(dataH, 0, SEND_PACKET_DATA_SIZE);
            memset(s_dataH, 0, SEND_PACKET_DATA_SIZE);
            dataH[0] = j%4+8;  //所属相机视链, 0:前主， 1：后主， 2：前辅， 3：后辅
            s_dataH[0] = dataH[0];
            for(k = 0; k < struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal; k++)
            {
                for(m = 0; m < struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++) // 认为视的次数和吹的序号一致
                {
                    nUnitAddr = getIdentifyGroupAddr(i, j, k, m)%64;
                    if(struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nUnitGupSplice[m] == 0 ){
                        dataH[1+nUnitAddr]   = (nDelayTime2[i][j][k])/256;
                        s_dataH[1+nUnitAddr] = dataH[1+nUnitAddr];
                    }
                    else if(struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nUnitGupSplice[m] == 1 ){
                        spliceInCurChain = 1;
                        dataH[1+nUnitAddr]   = (nDelayTime2[i][j][k])/256;
                    }
                    else if(struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nUnitGupSplice[m] == 2){
                        spliceInCurChain = 1;
                        s_dataH[1+nUnitAddr] = (nDelayTime2[i][j][k])/256;
                    }
                }
            }
            if(spliceInCurChain){
                mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME_H, nInt, board, BOARD_TYPE_ALL_IN_CHAIN_SPLIE_LEFT, dataH, 3);
                mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME_H, nInt, board, BOARD_TYPE_ALL_IN_CHAIN_SPLIE_RIGHT, s_dataH, 3);
            }
            else
            {
                mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME_H, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, dataH, 3);
            }

            //mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME_H, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, dataH, 3);
        }
        //        {
        //            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
        //            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
        //            memset(dataH, 0, SEND_PACKET_DATA_SIZE);
        //            dataH[0] = j%4+8;  //所属相机视链, 0:前主， 1：后主， 2：前辅， 3：后辅
        //            for(k = 0; k < struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal; k++)
        //            {
        //                for(m = 0; m < struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++) // 认为视的次数和吹的序号一致
        //                {
        //                    nUnitAddr = getIdentifyGroupAddr(i, j, k, m)%64;
        //                    dataH[1+nUnitAddr]  = (nDelayTime2[i][j][k])/256;
        //                }
        //            }
        //            mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME_H, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, dataH, 3);
        //        }
        mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME_H, nInt, BOARD_TYPE_INTERFACE, BOARD_TYPE_ALL_IN_CHAIN, dataH, 3);

        /* 发送延迟时间3 */
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]
                    || struCnfp.nSensorPixel[0][j] != SENSOR_PIXELS_2048_GTD) continue;
            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
            memset(dataH, 0, SEND_PACKET_DATA_SIZE);
            memset(s_dataH, 0, SEND_PACKET_DATA_SIZE);
            dataH[0] = j%4+16;  //所属相机视链, 0:前主， 1：后主， 2：前辅， 3：后辅
            s_dataH[0] = dataH[0];
            for(k = 0; k < struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal; k++)
            {
                for(m = 0; m < struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++) // 认为视的次数和吹的序号一致
                {
                    nUnitAddr = getIdentifyGroupAddr(i, j, k, m)%64;
                    if(struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nUnitGupSplice[m] == 0 ){
                        dataH[1+nUnitAddr]   = (nDelayTime3[i][j][k])/256;
                        s_dataH[1+nUnitAddr] = dataH[1+nUnitAddr];
                    }
                    else if(struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nUnitGupSplice[m] == 1 ){
                        spliceInCurChain = 1;
                        dataH[1+nUnitAddr]   = (nDelayTime3[i][j][k])/256;
                    }
                    else if(struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nUnitGupSplice[m] == 2){
                        spliceInCurChain = 1;
                        s_dataH[1+nUnitAddr] = (nDelayTime3[i][j][k])/256;
                    }
                }
            }
            if(spliceInCurChain){
                mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME_H, nInt, board, BOARD_TYPE_ALL_IN_CHAIN_SPLIE_LEFT, dataH, 3);
                mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME_H, nInt, board, BOARD_TYPE_ALL_IN_CHAIN_SPLIE_RIGHT, s_dataH, 3);
            }
            else
            {
                mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME_H, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, dataH, 3);
            }

            //mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME_H, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, dataH, 3);
        }
        //        {
        //            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
        //            if(struCnfp.nSensorPixel[0][struGsh.nView] != SENSOR_PIXELS_2048_GTD) continue;
        //            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
        //            memset(dataH, 0, SEND_PACKET_DATA_SIZE);
        //            dataH[0] = j%4+16;  //所属相机视链, 0:前主， 1：后主， 2：前辅， 3：后辅
        //            for(k = 0; k < struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal; k++)
        //            {
        //                for(m = 0; m < struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++) // 认为视的次数和吹的序号一致
        //                {
        //                    nUnitAddr = getIdentifyGroupAddr(i, j, k, m)%64;
        //                    dataH[1+nUnitAddr]  = (nDelayTime3[i][j][k])/256;
        //                }
        //            }
        //            mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME_H, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, dataH, 3);
        //        }
        /* 发送延迟4 */
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]
                    || struCnfp.nSensorPixel[0][j] != SENSOR_PIXELS_2048_GTD) continue;
            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
            memset(dataH, 0, SEND_PACKET_DATA_SIZE);
            memset(s_dataH, 0, SEND_PACKET_DATA_SIZE);
            dataH[0] = j%4+24;  //所属相机视链, 0:前主， 1：后主， 2：前辅， 3：后辅
            s_dataH[0] = dataH[0];
            for(k = 0; k < struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal; k++)
            {
                for(m = 0; m < struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++) // 认为视的次数和吹的序号一致
                {
                    nUnitAddr = getIdentifyGroupAddr(i, j, k, m)%64;
                    if(struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nUnitGupSplice[m] == 0 ){
                        dataH[1+nUnitAddr]   = (nDelayTime4[i][j][k])/256;
                        s_dataH[1+nUnitAddr] = dataH[1+nUnitAddr];
                    }
                    else if(struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nUnitGupSplice[m] == 1 ){
                        spliceInCurChain = 1;
                        dataH[1+nUnitAddr]   = (nDelayTime4[i][j][k])/256;
                    }
                    else if(struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nUnitGupSplice[m] == 2){
                        spliceInCurChain = 1;
                        s_dataH[1+nUnitAddr] = (nDelayTime4[i][j][k])/256;
                    }
                }
            }
            if(spliceInCurChain){
                mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME_H, nInt, board, BOARD_TYPE_ALL_IN_CHAIN_SPLIE_LEFT, dataH, 3);
                mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME_H, nInt, board, BOARD_TYPE_ALL_IN_CHAIN_SPLIE_RIGHT, s_dataH, 3);
            }
            else
            {
                mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME_H, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, dataH, 3);
            }

            //mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME_H, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, dataH, 3);
        }
        //        {
        //            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
        //            if(struCnfp.nSensorPixel[0][struGsh.nView] != SENSOR_PIXELS_2048_GTD) continue;
        //            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
        //            memset(dataH, 0, SEND_PACKET_DATA_SIZE);
        //            dataH[0] = j%4+24;  //所属相机视链, 0:前主， 1：后主， 2：前辅， 3：后辅
        //            for(k = 0; k < struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal; k++)
        //            {
        //                for(m = 0; m < struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++) // 认为视的次数和吹的序号一致
        //                {
        //                    nUnitAddr = getIdentifyGroupAddr(i, j, k, m)%64;
        //                    dataH[1+nUnitAddr]  = (nDelayTime4[i][j][k])/256;
        //                }
        //            }
        //            mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME_H, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, dataH, 3);
        //        }
        //        mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME_H, nInt, BOARD_TYPE_INTERFACE, BOARD_TYPE_ALL_IN_CHAIN, dataH, 3);


        if (struCnfg.nDPMachine)
        {
            /* 发送模型延迟时间 */
            for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
            {
                if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
                board = struCnfg.struLayerInfo[i].nViewBoardType[j];
                memset(dataM, 0, SEND_PACKET_DATA_SIZE);
                for(k = 0; k < struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal; k++)
                {
                    for(m = 0; m < struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++) // 认为视的次数和吹的序号一致
                    {
                        nUnitAddr = getIdentifyGroupAddr(i, j, k, m);
                        dataM[0] = nModelDelayTime[0][j][k]/255;
                        dataM[1] = nModelDelayTime[0][j][k]%255;
                        mySerial.pushCom1CmdData(CMD_INT_VAVLE_MODEL_DELAY_TIME, nInt, board, nUnitAddr, dataM, 3);
                    }
                }
            }
        }
    }

    /* 2. 发送相机板 延迟时间 低字节*/
    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        /* 发送色选延迟时间 */
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
            memset(data, 0, SEND_PACKET_DATA_SIZE);
            memset(s_data, 0, SEND_PACKET_DATA_SIZE);
            data[0] = j%4;  //所属相机视链, 0:前主， 1：后主， 2：前辅， 3：后辅
            s_data[0] = data[0];
            for(k = 0; k < struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal; k++)
            {
                for(m = 0; m < struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++) // 认为视的次数和吹的序号一致
                {
                    nUnitAddr = getIdentifyGroupAddr(i, j, k, m)%64;
                    if(struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nUnitGupSplice[m] == 0 ){
                        data[1+nUnitAddr]   = (nDelayTime[i][j][k])%256;
                        s_data[1+nUnitAddr] = data[1+nUnitAddr];
                    }
                    else if(struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nUnitGupSplice[m] == 1 ){
                        spliceInCurChain = 1;
                        data[1+nUnitAddr]   = (nDelayTime[i][j][k])%256;
                    }
                    else if(struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nUnitGupSplice[m] == 2){
                        spliceInCurChain = 1;
                        s_data[1+nUnitAddr] = (nDelayTime[i][j][k])%256;
                    }
                }
            }
            if(spliceInCurChain){
                mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME, nInt, board, BOARD_TYPE_ALL_IN_CHAIN_SPLIE_LEFT, data, 3);
                mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME, nInt, board, BOARD_TYPE_ALL_IN_CHAIN_SPLIE_RIGHT, s_data, 3);
            }
            else{
                mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
            }
            //mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        }
        //        {
        //            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
        //            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
        //            memset(data, 0, SEND_PACKET_DATA_SIZE);
        //            data[0] = j%4;  //所属相机视链, 0:前主， 1：后主， 2：前辅， 3：后辅
        //            for(k = 0; k < struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal; k++)
        //            {
        //                for(m = 0; m < struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++) // 认为视的次数和吹的序号一致
        //                {
        //                    nUnitAddr = getIdentifyGroupAddr(i, j, k, m)%64;
        //                    data[1+nUnitAddr]   = (nDelayTime[i][j][k])%256;
        //                }
        //            }
        //            mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        //        }
        mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME, nInt, BOARD_TYPE_INTERFACE, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        /* 发送形选延迟时间 */
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
            memset(data, 0, SEND_PACKET_DATA_SIZE);
            memset(s_data, 0, SEND_PACKET_DATA_SIZE);
            data[0] = j%4+8;  //所属相机视链, 0:前主， 1：后主， 2：前辅， 3：后辅
            s_data[0] = data[0];
            for(k = 0; k < struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal; k++)
            {
                for(m = 0; m < struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++) // 认为视的次数和吹的序号一致
                {
                    nUnitAddr = getIdentifyGroupAddr(i, j, k, m)%64;
                    if(struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nUnitGupSplice[m] == 0 ){
                        data[1+nUnitAddr]   = (nDelayTime2[i][j][k])%256;
                        s_data[1+nUnitAddr] = data[1+nUnitAddr];
                    }
                    else if(struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nUnitGupSplice[m] == 1 ){
                        spliceInCurChain = 1;
                        data[1+nUnitAddr]   = (nDelayTime2[i][j][k])%256;
                    }
                    else if(struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nUnitGupSplice[m] == 2){
                        spliceInCurChain = 1;
                        s_data[1+nUnitAddr] = (nDelayTime2[i][j][k])%256;
                    }
                }
            }
            if(spliceInCurChain){
                mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME, nInt, board, BOARD_TYPE_ALL_IN_CHAIN_SPLIE_LEFT, data, 3);
                mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME, nInt, board, BOARD_TYPE_ALL_IN_CHAIN_SPLIE_RIGHT, s_data, 3);
            }
            else{
                mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
            }

            // mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        }
        //        {
        //            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
        //            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
        //            memset(data, 0, SEND_PACKET_DATA_SIZE);
        //            data[0] = j%4+8;  //所属相机视链, 0:前主， 1：后主， 2：前辅， 3：后辅
        //            for(k = 0; k < struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal; k++)
        //            {
        //                for(m = 0; m < struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++) // 认为视的次数和吹的序号一致
        //                {
        //                    nUnitAddr = getIdentifyGroupAddr(i, j, k, m)%64;
        //                    data[1+nUnitAddr]   = (nDelayTime2[i][j][k])%256;
        //                }
        //            }
        //            mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        //        }
        mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME, nInt, BOARD_TYPE_INTERFACE, BOARD_TYPE_ALL_IN_CHAIN, data, 3);

        /* 发送延迟3 */
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]
                    || struCnfp.nSensorPixel[0][j] != SENSOR_PIXELS_2048_GTD) continue;
            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
            memset(data, 0, SEND_PACKET_DATA_SIZE);
            memset(s_data, 0, SEND_PACKET_DATA_SIZE);
            data[0] = j%4+16;  //所属相机视链, 0:前主， 1：后主， 2：前辅， 3：后辅
            s_data[0] = data[0];
            for(k = 0; k < struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal; k++)
            {
                for(m = 0; m < struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++) // 认为视的次数和吹的序号一致
                {
                    nUnitAddr = getIdentifyGroupAddr(i, j, k, m)%64;
                    if(struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nUnitGupSplice[m] == 0 ){
                        data[1+nUnitAddr]   = (nDelayTime3[i][j][k])%256;
                        s_data[1+nUnitAddr] = data[1+nUnitAddr];
                    }
                    else if(struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nUnitGupSplice[m] == 1 ){
                        spliceInCurChain = 1;
                        data[1+nUnitAddr]   = (nDelayTime3[i][j][k])%256;
                    }
                    else if(struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nUnitGupSplice[m] == 2){
                        spliceInCurChain = 1;
                        s_data[1+nUnitAddr] = (nDelayTime3[i][j][k])%256;
                    }
                }
            }
            if(spliceInCurChain){
                mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME, nInt, board, BOARD_TYPE_ALL_IN_CHAIN_SPLIE_LEFT, data, 3);
                mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME, nInt, board, BOARD_TYPE_ALL_IN_CHAIN_SPLIE_RIGHT, s_data, 3);
            }
            else{
                mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
            }

            // mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        }
        //        {
        //            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
        //            if(struCnfp.nSensorPixel[0][struGsh.nView] != SENSOR_PIXELS_2048_GTD) continue;
        //            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
        //            memset(data, 0, SEND_PACKET_DATA_SIZE);
        //            data[0] = j%4+16;  //所属相机视链, 0:前主， 1：后主， 2：前辅， 3：后辅
        //            for(k = 0; k < struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal; k++)
        //            {
        //                for(m = 0; m < struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++) // 认为视的次数和吹的序号一致
        //                {
        //                    nUnitAddr = getIdentifyGroupAddr(i, j, k, m)%64;
        //                    data[1+nUnitAddr]   = (nDelayTime3[i][j][k])%256;
        //                }
        //            }
        //            mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        //        }
        //  mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME, nInt, BOARD_TYPE_INTERFACE, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        /* 发送延迟4 */
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]
                    || struCnfp.nSensorPixel[0][j] != SENSOR_PIXELS_2048_GTD) continue;
            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
            memset(data, 0, SEND_PACKET_DATA_SIZE);
            memset(s_data, 0, SEND_PACKET_DATA_SIZE);
            data[0] = j%4+24;  //所属相机视链, 0:前主， 1：后主， 2：前辅， 3：后辅
            s_data[0] = data[0];
            for(k = 0; k < struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal; k++)
            {
                for(m = 0; m < struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++) // 认为视的次数和吹的序号一致
                {
                    nUnitAddr = getIdentifyGroupAddr(i, j, k, m)%64;
                    if(struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nUnitGupSplice[m] == 0 ){
                        data[1+nUnitAddr]   = (nDelayTime4[i][j][k])%256;
                        s_data[1+nUnitAddr] = data[1+nUnitAddr];
                    }
                    else if(struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nUnitGupSplice[m] == 1 ){
                        spliceInCurChain = 1;
                        data[1+nUnitAddr]   = (nDelayTime4[i][j][k])%256;
                    }
                    else if(struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nUnitGupSplice[m] == 2){
                        spliceInCurChain = 1;
                        s_data[1+nUnitAddr] = (nDelayTime4[i][j][k])%256;
                    }
                }
            }
            if(spliceInCurChain){
                mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME, nInt, board, BOARD_TYPE_ALL_IN_CHAIN_SPLIE_LEFT, data, 3);
                mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME, nInt, board, BOARD_TYPE_ALL_IN_CHAIN_SPLIE_RIGHT, s_data, 3);
            }
            else{
                mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
            }

            // mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        }
        //        {
        //            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
        //            if(struCnfp.nSensorPixel[0][struGsh.nView] != SENSOR_PIXELS_2048_GTD) continue;
        //            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
        //            memset(data, 0, SEND_PACKET_DATA_SIZE);
        //            data[0] = j%4+24;  //所属相机视链, 0:前主， 1：后主， 2：前辅， 3：后辅
        //            for(k = 0; k < struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal; k++)
        //            {
        //                for(m = 0; m < struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++) // 认为视的次数和吹的序号一致
        //                {
        //                    nUnitAddr = getIdentifyGroupAddr(i, j, k, m)%64;
        //                    data[1+nUnitAddr]   = (nDelayTime4[i][j][k])%256;
        //                }
        //            }
        //            mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        //        }
        //  mySerial.pushCom1CmdData(CMD_INT_VAVLE_DELAY_TIME, nInt, BOARD_TYPE_INTERFACE, BOARD_TYPE_ALL_IN_CHAIN, data, 3);

    }

    return nMinDelay;
}

/* 发送接口板吹气方式 */
void GlobalFlow::resetEjectVavleModeRG()
{
    int i, j, k, m, n;
    int nInt=0, board, nUnitAddr;
    char data[SEND_PACKET_DATA_SIZE];

    /* 发送接口板 吹气方式 */
    for(i = 0; i < struCnfg.nLayerTotal; i++)  //层
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nVavleChainTotal; j++)  // 视
        {
            board = (struCnfg.nMachineAttr == MACHINE_TYPE_BELT_DOUBLE) ? struCnfg.struLayerInfo[i].nVavleBoardType[j] : BOARD_TYPE_INTERFACE;
            memset(data, 0, SEND_PACKET_DATA_SIZE);

            for(k = 0; k < struCnfg.struLayerInfo[i].stuVavleInfo[j].nTickGroupTotal; k++) // 相机 组单元
            {
                for(m = 0; m < struCnfg.struLayerInfo[i].stuVavleInfo[j].stuTickGupInfo[k].nEjBoardGupTotal; m++)
                {
                    if(struCnfg.struLayerInfo[i].nSplice == 0) //带带分割的机器
                    {
                        nUnitAddr = getTickGroupAddr(i, j, k, m);
                        data[nUnitAddr] = struCnfp.struGroupTick[i][j][k].nEjwidth;
                    }
                    else  //带带分割的机器
                    {
                        if(struCnfg.struLayerInfo[i].stuVavleInfo[j].stuTickGupInfo[k].nEjBoardGupSplice[m] == 0)
                        {
                            nUnitAddr = getTickGroupAddr(i, j, k, m);
                            data[nUnitAddr] = struCnfp.struGroupTick[i][j][k].nEjwidth;
                        }
                        else if(struCnfg.struLayerInfo[i].stuVavleInfo[j].stuTickGupInfo[k].nEjBoardGupSplice[m] == 1)
                        {
                            nUnitAddr = getTickGroupAddr(i, j, k, m)%64;
                            data[nUnitAddr] = struCnfp.struGroupTick[i][j][k].nEjwidth;
                            data[15] = struCnfp.struGroupTick[i][j][k+1].nEjwidth;
                        }
                    }
                }
            }

            mySerial.pushCom1CmdData(CMD_ALL_EJECT_MODE, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        }
    }
}

/* 发送接口板喷阀数据与或关系 */
void GlobalFlow::resetEjectFrontRearLogicRG()
{
    int i, j, k, m, n;
    int nInt=0, board, nUnitAddr;
    char data[SEND_PACKET_DATA_SIZE];

    /* 发送接口板 与或关系 */
    for(i = 0; i < struCnfg.nLayerTotal; i++)  //层
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nVavleChainTotal; j++)  // 吹
        {
            board = (struCnfg.nMachineAttr == MACHINE_TYPE_BELT_DOUBLE) ? struCnfg.struLayerInfo[i].nVavleBoardType[j] : BOARD_TYPE_INTERFACE;
            memset(data, 0, SEND_PACKET_DATA_SIZE);

            for(k = 0; k < struCnfg.struLayerInfo[i].stuVavleInfo[j].nTickGroupTotal; k++) // 喷阀 组单元
            {
                for(m = 0; m < struCnfg.struLayerInfo[i].stuVavleInfo[j].stuTickGupInfo[k].nEjBoardGupTotal; m++)
                {
                    nUnitAddr = getTickGroupAddr(i, j, k, m);
                    if(nUnitAddr/64 == 0)
                    {
                        data[nUnitAddr] = struCnfp.struGroupTick[i][j][k].nFrontRearMode;
                    }
                }
            }
            mySerial.pushCom1CmdData(CMD_INT_VIEW_LOGIC_RELATION, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        }
    }
}

/* 发送接口板喷阀吹气时间 */
void GlobalFlow::resetEjectDurationTimeRG()
{
    int i, j, k, m, n;
    int nInt=0, board, nUnitAddr;
    char data[SEND_PACKET_DATA_SIZE];

    /* 发送接口板 喷阀吹气时间 */
    for(i = 0; i < struCnfg.nLayerTotal; i++)  //层
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nVavleChainTotal; j++)  // 喷阀链
        {
            board = (struCnfg.nMachineAttr == MACHINE_TYPE_BELT_DOUBLE) ? struCnfg.struLayerInfo[i].nVavleBoardType[j] : BOARD_TYPE_INTERFACE;
            memset(data, 0, SEND_PACKET_DATA_SIZE);

            for(k = 0; k < struCnfg.struLayerInfo[i].stuVavleInfo[j].nTickGroupTotal; k++) // 相机 组单元
            {
                for(m = 0; m < struCnfg.struLayerInfo[i].stuVavleInfo[j].stuTickGupInfo[k].nEjBoardGupTotal; m++)
                {
                    if(struCnfg.struLayerInfo[i].nSplice == 0) //不带分割的机器
                    {
                        nUnitAddr = getTickGroupAddr(i, j, k, m);
                        data[nUnitAddr] = struCnfp.struGroupTick[i][j][k].nEjectDuration;
                    }
                    else  //带带分割的机器
                    {
                        if(struCnfg.struLayerInfo[i].stuVavleInfo[j].stuTickGupInfo[k].nEjBoardGupSplice[m] == 0)
                        {
                            nUnitAddr = getTickGroupAddr(i, j, k, m);
                            data[nUnitAddr] = struCnfp.struGroupTick[i][j][k].nEjectDuration;
                        }
                        else if(struCnfg.struLayerInfo[i].stuVavleInfo[j].stuTickGupInfo[k].nEjBoardGupSplice[m] == 1)
                        {
                            nUnitAddr = getTickGroupAddr(i, j, k, m)%64;
                            data[nUnitAddr] = struCnfp.struGroupTick[i][j][k].nEjectDuration;
                            data[15] = struCnfp.struGroupTick[i][j][k+1].nEjectDuration;
                        }
                    }
                }
            }
            mySerial.pushCom1CmdData(CMD_INT_VAVLE_TIME, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        }
    }

    if(struCnfg.nDPMachine)
    {
        /* 发送接口板 喷阀吹气时间,新增加 */
        for(int t = 0; t < struCnfp.nEjectDurationIpcCount; t++)
        {
            for(i = 0; i < struCnfg.nLayerTotal; i++)  //层
            {
                for(j = 0; j < struCnfg.struLayerInfo[i].nVavleChainTotal; j++)  // 喷阀链
                {
                    board = (struCnfg.nMachineAttr == MACHINE_TYPE_BELT_DOUBLE) ? struCnfg.struLayerInfo[i].nVavleBoardType[j] : BOARD_TYPE_INTERFACE;
                    memset(data, 0, SEND_PACKET_DATA_SIZE);

                    data[0] = t;

                    for(k = 0; k < struCnfg.struLayerInfo[i].stuVavleInfo[j].nTickGroupTotal; k++) // 相机 组单元
                    {
                        for(m = 0; m < struCnfg.struLayerInfo[i].stuVavleInfo[j].stuTickGupInfo[k].nEjBoardGupTotal; m++)
                        {
                            if(struCnfg.struLayerInfo[i].nSplice == 0) //不带分割的机器
                            {
                                nUnitAddr = getTickGroupAddr(i, j, k, m);
                                data[nUnitAddr+1] = struCnfp.struGroupTick[i][j][k].nEjectDuration2;
                            }
                            else  //带带分割的机器
                            {
                                if(struCnfg.struLayerInfo[i].stuVavleInfo[j].stuTickGupInfo[k].nEjBoardGupSplice[m] == 0)
                                {
                                    nUnitAddr = getTickGroupAddr(i, j, k, m);
                                    data[nUnitAddr+1] = struCnfp.struGroupTick[i][j][k].nEjectDuration;
                                }
                                else if(struCnfg.struLayerInfo[i].stuVavleInfo[j].stuTickGupInfo[k].nEjBoardGupSplice[m] == 1)
                                {
                                    nUnitAddr = getTickGroupAddr(i, j, k, m)%64;
                                    data[nUnitAddr+1] = struCnfp.struGroupTick[i][j][k].nEjectDuration2;
                                    data[15] = struCnfp.struGroupTick[i][j][k+1].nEjectDuration2;
                                }
                            }
                        }
                    }
                    mySerial.pushCom1CmdData(CMD_CAMERA_EJECT_DURATION_IPC, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
                }
            }
        }
    }
}

/* 发送接口板喷阀等待时间 */
void GlobalFlow::resetEjectWaitTimeRG()
{
    int i, j, k, m, n;
    int nInt=0, board, nUnitAddr;
    char data[SEND_PACKET_DATA_SIZE];

    /* 发送接口板 等待时间 */
    for(i = 0; i < struCnfg.nLayerTotal; i++)  //层
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nVavleChainTotal; j++)  // 喷阀链
        {
            board = (struCnfg.nMachineAttr == MACHINE_TYPE_BELT_DOUBLE) ? struCnfg.struLayerInfo[i].nVavleBoardType[j] : BOARD_TYPE_INTERFACE;
            memset(data, 0, SEND_PACKET_DATA_SIZE);

            for(k = 0; k < struCnfg.struLayerInfo[i].stuVavleInfo[j].nTickGroupTotal; k++) // 相机 组单元
            {
                for(m = 0; m < struCnfg.struLayerInfo[i].stuVavleInfo[j].stuTickGupInfo[k].nEjBoardGupTotal; m++)
                {
                    if(struCnfg.struLayerInfo[i].nSplice == 0) //不带分割的机器
                    {
                        nUnitAddr = getTickGroupAddr(i, j, k, m);
                        data[nUnitAddr] = struCnfp.struGroupTick[i][j][k].nWaitTime;
                    }
                    else  //带带分割的机器
                    {
                        if(struCnfg.struLayerInfo[i].stuVavleInfo[j].stuTickGupInfo[k].nEjBoardGupSplice[m] == 0)
                        {
                            nUnitAddr = getTickGroupAddr(i, j, k, m);
                            data[nUnitAddr] = struCnfp.struGroupTick[i][j][k].nWaitTime;
                        }
                        else if(struCnfg.struLayerInfo[i].stuVavleInfo[j].stuTickGupInfo[k].nEjBoardGupSplice[m] == 1)
                        {
                            nUnitAddr = getTickGroupAddr(i, j, k, m)%64;
                            data[nUnitAddr] = struCnfp.struGroupTick[i][j][k].nWaitTime;
                            data[15] = struCnfp.struGroupTick[i][j][k+1].nWaitTime;
                        }
                    }
                }
            }

            mySerial.pushCom1CmdData(CMD_INT_VAVLE_WAIT_TIME, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        }
    }
}

/* 发送接口板喷阀尾气时间 */
void GlobalFlow::resetEjectTailTimeRG()
{
    int i, j, k, m, n;
    int nInt=0, board, nUnitAddr;
    char data[SEND_PACKET_DATA_SIZE];

    /* 发送接口板 尾气时间 */
    for(i = 0; i < struCnfg.nLayerTotal; i++)  //层
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nVavleChainTotal; j++)  // 喷阀链
        {
            board = (struCnfg.nMachineAttr == MACHINE_TYPE_BELT_DOUBLE) ? struCnfg.struLayerInfo[i].nVavleBoardType[j] : BOARD_TYPE_INTERFACE;
            memset(data, 0, SEND_PACKET_DATA_SIZE);

            for(k = 0; k < struCnfg.struLayerInfo[i].stuVavleInfo[j].nTickGroupTotal; k++) // 相机 组单元
            {
                for(m = 0; m < struCnfg.struLayerInfo[i].stuVavleInfo[j].stuTickGupInfo[k].nEjBoardGupTotal; m++)
                {
                    if(struCnfg.struLayerInfo[i].nSplice == 0) //不带分割的机器
                    {
                        nUnitAddr = getTickGroupAddr(i, j, k, m);
                        data[nUnitAddr] = struCnfp.struGroupTick[i][j][k].nTailTime;
                    }
                    else  //带带分割的机器
                    {
                        if(struCnfg.struLayerInfo[i].stuVavleInfo[j].stuTickGupInfo[k].nEjBoardGupSplice[m] == 0)
                        {
                            nUnitAddr = getTickGroupAddr(i, j, k, m);
                            data[nUnitAddr] = struCnfp.struGroupTick[i][j][k].nTailTime;
                        }
                        else if(struCnfg.struLayerInfo[i].stuVavleInfo[j].stuTickGupInfo[k].nEjBoardGupSplice[m] == 1)
                        {
                            nUnitAddr = getTickGroupAddr(i, j, k, m)%64;
                            data[nUnitAddr] = struCnfp.struGroupTick[i][j][k].nTailTime;
                            data[15] = struCnfp.struGroupTick[i][j][k+1].nTailTime;
                        }
                    }
                }
            }

            mySerial.pushCom1CmdData(CMD_INT_VAVLE_TAIL_TIME, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        }
    }
}

void GlobalFlow::resetVavleDefendTime()
{
    int i, j, k;
    int nInt=0, board;
    char data[SEND_PACKET_DATA_SIZE];

    memset(data, 0, SEND_PACKET_DATA_SIZE);

    data[0] = (struCnfg.nValveDefendTime >> 24)   & 0xff;
    data[1] = (struCnfg.nValveDefendTime >> 16)   & 0xff;
    data[2] = (struCnfg.nValveDefendTime >> 8)    & 0xff;
    data[3] = (struCnfg.nValveDefendTime >> 0)    & 0xff;
    data[4] = (struCnfg.nValveUndefendTime >> 24) & 0xff;
    data[5] = (struCnfg.nValveUndefendTime >> 16) & 0xff;
    data[6] = (struCnfg.nValveUndefendTime >> 8)  & 0xff;
    data[7] = (struCnfg.nValveUndefendTime >> 0)  & 0xff;

    mySerial.pushCom1CmdData(CMD_INT_VAVLE_DEFEND_TIME, nInt, BOARD_TYPE_INTERFACE, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
}

/* 发送相机板正反选模式 */
void GlobalFlow::resetCameraReverseSortRG()
{
    int i, j, k, m;
    int nInt=0, board, nUnitAddr = 0;
    char data[SEND_PACKET_DATA_SIZE];

    //   int sendView = 0;   //全部当做前主

    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
            memset(data, 0, SEND_PACKET_DATA_SIZE);
            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
            for(k = 0; k < struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal; k++)
            {
                for(m = 0; m < struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++) // 认为视的次数和吹的序号一致
                {
                    nUnitAddr = getIdentifyGroupAddr(i, j, k, m)%64;
                    data[nUnitAddr] =  struCnfp.struGroupIdentify[i][j][k].nReverseSort[0];
                }
            }
            mySerial.pushCom1CmdData(CMD_ALL_SORT_REVERSE, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        }
        //        mySerial.pushCom1CmdData(CMD_ALL_SORT_REVERSE, nInt, BOARD_TYPE_INTERFACE, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
    }
}

/* 发送相机板坏点个数 */
void GlobalFlow::resetCameraBadNumRG()
{
    int i, j, k, m;
    int nInt=0, board, nUnitAddr = 0;
    char data[SEND_PACKET_DATA_SIZE];

    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
            memset(data, 0, SEND_PACKET_DATA_SIZE);
            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
            for(k = 0; k < struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal; k++)
            {
                for(m = 0; m < struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++) // 认为视的次数和吹的序号一致
                {
                    nUnitAddr = getIdentifyGroupAddr(i, j, k, m)%64;
                    data[nUnitAddr] =  struCnfp.struGroupIdentify[i][j][k].nBadNum;
                }
            }
            mySerial.pushCom1CmdData(CMD_CAMERA_BAD_POINT, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        }
    }
}

/* 发送相机板色选传染尺寸*/
void GlobalFlow::resetCameraImpureRG()
{
    int i, j, k, m;
    int nInt=0, board, nUnitAddr = 0;
    char data[SEND_PACKET_DATA_SIZE];

    // 色选传染算法
    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
            memset(data, 0, SEND_PACKET_DATA_SIZE);
            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
            for(k = 0; k < struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal; k++)
            {
                for(m = 0; m < struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++) // 认为视的次数和吹的序号一致
                {
                    nUnitAddr = getIdentifyGroupAddr(i, j, k, m)%64;
                    if(nUnitAddr/64 == 0)
                    {
                        data[nUnitAddr] =  struCnfp.struGroupIdentify[i][j][k].nImpured[0];
                    }
                }
            }
            mySerial.pushCom1CmdData(CMD_CAMERA_INPURED, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        }
    }

    // 形选传染算法
    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
            memset(data, 0, SEND_PACKET_DATA_SIZE);
            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
            for(k = 0; k < struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal; k++)
            {
                for(m = 0; m < struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++) // 认为视的次数和吹的序号一致
                {
                    nUnitAddr = getIdentifyGroupAddr(i, j, k, m)%64;
                    if(nUnitAddr/64 == 0)
                    {
                        data[nUnitAddr] =  struCnfp.struGroupIdentify[i][j][k].nImpured[1];
                    }
                }
            }
            mySerial.pushCom1CmdData(CMD_CAMERA_SHAPE_INPURED, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        }
    }
    if(struCnfg.nDPMachine)
    {
        // IPC传染算法
        struCnfp.nImpuredIpcCount = 3;
        for(int t = 0; t < struCnfp.nImpuredIpcCount; t++)
        {
            for(i = 0; i < struCnfg.nLayerTotal; i++)
            {
                for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
                {
                    if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
                    memset(data, 0, SEND_PACKET_DATA_SIZE);
                    data[0] = t;
                    board = struCnfg.struLayerInfo[i].nViewBoardType[j];
                    for(k = 0; k < struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal; k++)
                    {
                        for(m = 0; m < struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++) // 认为视的次数和吹的序号一致
                        {
                            nUnitAddr = getIdentifyGroupAddr(i, j, k, m)%64;
                            if(nUnitAddr/64 == 0)
                            {
                                data[nUnitAddr+1] =  struCnfp.struGroupIdentify[i][j][k].nImpuredIpc[t];
                            }
                        }
                    }
                    mySerial.pushCom1CmdData(CMD_CAMERA_IMPURED_IPC, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
                }
            }
        }
    }
}

/* 发送相机板边缘切除*/
void GlobalFlow::resetCameraEdgeCutRG()
{
    int i, j, k, m;
    int nInt=0, board, nUnitAddr = 0;
    char data[SEND_PACKET_DATA_SIZE];

    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
            memset(data, 0, SEND_PACKET_DATA_SIZE);
            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
            for(k = 0; k < struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal; k++)
            {
                for(m = 0; m < struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++) // 认为视的次数和吹的序号一致
                {
                    nUnitAddr = getIdentifyGroupAddr(i, j, k, m)%64;
                    data[nUnitAddr] =  struCnfp.struGroupIdentify[i][j][k].nEdgeCut[0];
                }
            }
            mySerial.pushCom1CmdData(CMD_CAMERA_EDGE_CUT, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        }
    }
}

/* 发送相机板边缘切除*/
void GlobalFlow::resetCameraCenterSize()
{
    int i, j, k, m;
    int nInt=0, board, nUnitAddr = 0;
    char data[SEND_PACKET_DATA_SIZE];

    /* 中心化尺寸 1*/
    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
            memset(data, 0, SEND_PACKET_DATA_SIZE);
            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
            for(k = 0; k < struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal; k++)
            {
                for(m = 0; m < struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++) // 认为视的次数和吹的序号一致
                {
                    nUnitAddr = getIdentifyGroupAddr(i, j, k, m)%64;
                    data[nUnitAddr] =  struCnfp.struGroupIdentify[i][j][k].nCenterSize[0];
                }
            }
            mySerial.pushCom1CmdData(CMD_CAMERA_CENTER_SIZE_1, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        }
    }

    /* 中心化尺寸 2*/
    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
            memset(data, 0, SEND_PACKET_DATA_SIZE);
            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
            for(k = 0; k < struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal; k++)
            {
                for(m = 0; m < struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++) // 认为视的次数和吹的序号一致
                {
                    nUnitAddr = getIdentifyGroupAddr(i, j, k, m)%64;
                    data[nUnitAddr] =  struCnfp.struGroupIdentify[i][j][k].nCenterSize[1];
                }
            }
            mySerial.pushCom1CmdData(CMD_CAMERA_CENTER_SIZE_2, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        }
    }
}

void GlobalFlow::resetArithRejectRatio()
{
    int i,j,k,m;
    int nInt=0, board, nUnitAddr, value;
    char data[SEND_PACKET_DATA_SIZE];
    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
            memset(data, 0, SEND_PACKET_DATA_SIZE);
            for(k = 0; k < struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal; k++)
            {
                for(m = 0; m < struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++) // 认为视的次数和吹的序号一致
                {
                    nUnitAddr = getIdentifyGroupAddr(i, j, k, m)%64;
                    if(nUnitAddr/64 == 0)
                    {
                        value = struCnfp.struGroupIdentify[i][j][k].arithRejectRatio;
                        data[0] = value/256;
                        data[1] = value%256;
                        mySerial.pushCom1CmdData(CMD_INT_ARITH_REJECT_RATIO, nInt, board, nUnitAddr, data, 3);
                    }
                }
            }
        }
    }
}

void GlobalFlow::switchUserMirror(uchar mode,uchar boardType, uchar boardNum, uchar controlKind)
{
    int nInt=0;
    char data[SEND_PACKET_DATA_SIZE];
    memset(data, 0, SEND_PACKET_DATA_SIZE);
    data[0] = mode & 0x0f;

    if(controlKind == BOARD_TYPE_CONTROL_ZD)
    {
        data[1] = 1;
    }
    else if (controlKind == BOARD_TYPE_CONTROL_DL)
    {
        data[1] = 2;
    }
    else
    {
        data[1] = ((mode & 0xf0) >>4) == 2 ? 1 : 0;
    }

    if(boardNum == 0)
        mySerial.pushCom1CmdData(CMD_ALL_FPGA_USER_MODE, nInt, boardType, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
    else
        mySerial.pushCom1CmdData(CMD_ALL_FPGA_USER_MODE, nInt, boardType, boardNum-1, data, 3);
}

void GlobalFlow::readVersion(uchar boardType, uchar boardNum)
{
    /* 每次重新初始化各变量状态 */
    struGsh.struVer.nInterfaceRes = -1;
    for(int i = 0; i <5 ; i++)
    {
        struGsh.struVer.sInterface[i] = 0;
    }
    for(int i = 0; i <MAX_LAYER; i++)
    {
        for(int j = 0; j < MAX_VIEW; j++)
        {
            if(struCnfg.struLayerInfo[i].nViewIsUsed[j] == 1)
            {
                for(int m = 0; m < struCnfg.struLayerInfo[i].nViewUnitTotal[j]; m++)
                {
                    for(int n = 0; n <5; n++)
                    {
                        if(n < 2)
                            struGsh.struVer.sUnitSync[i][j][m][n] = 0;
                        struGsh.struVer.sUnit[i][j][m][n] = 0;
                        struGsh.struVer.sUnitIpc[i][j][m][n] = 0;
                    }
                    struGsh.struVer.nUnitRes[i][j][m] = -1;
                    struCnfg.struVer.nUnitRes[i][j][m] = -1;
                }
            }
        }
    }

    char data[SEND_PACKET_DATA_SIZE];
    memset(data, 0, SEND_PACKET_DATA_SIZE);
    if(boardNum == 0)
        mySerial.pushCom1CmdData(CMD_ALL_VERSION_REQUIRE, 0, boardType, BOARD_TYPE_ALL_IN_CHAIN, data, 1);
    else
        mySerial.pushCom1CmdData(CMD_ALL_VERSION_REQUIRE, 0, boardType, boardNum-1, data, 1);
}

void GlobalFlow::tellUpgrade(uchar boardType, uchar boardNum, uchar mirrorIndex)
{
    int nInt=0;
    char data[SEND_PACKET_DATA_SIZE];
    memset(data, 0, SEND_PACKET_DATA_SIZE);
    data[0] = (mirrorIndex & 0x0f) + 1;
    data[1] = 0;
    data[2] = ((mirrorIndex & 0xf0) >>4) == 2 ? 1 : 0; //相机驱动板置1
    if(boardNum == 0)
        mySerial.pushCom1CmdData(CMD_ALL_FPGA_UPGRADE_MODE, nInt, boardType, BOARD_TYPE_ALL_IN_CHAIN, data, 1);
    else
        mySerial.pushCom1CmdData(CMD_ALL_FPGA_UPGRADE_MODE, nInt, boardType, boardNum-1, data, 1);
}

bool GlobalFlow::isNetWorkOk(int addr)
{
    QString ip = "162.254.129.10";
    ip = (addr == 1)? "162.254.129.10" :"ntp.ntsc.ac.cn";

    QProcess m_cmd;
    QString cmd = "ping -w 3 " + ip;

    m_cmd.start(cmd.toLocal8Bit().constData());

    if(m_cmd.waitForFinished())
    {
        QString str = m_cmd.readAll();

#ifdef Q_OS_UNIX
        if(!str.contains("ttl"))
#else
        if(!str.contains("TTL"))
#endif
        {
            m_cmd.kill();
            return 0;
        }
        else
            return 1;
    }
}


void GlobalFlow::resetSortOnOff(int onOff, int boardType)
{
    int i, j;
    int board;
    char data[SEND_PACKET_DATA_SIZE];

    memset(data, 0, SEND_PACKET_DATA_SIZE);
    data[0] = onOff;

    if(onOff == 1)
    {
        struGsh.nSortStartCount = 0;
    }

    if(boardType == 0)
    {
        mySerial.pushCom1CmdData(CMD_ALL_SORT_START, 0, BOARD_TYPE_ALL_BOARD, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        mySerial.pushCom1CmdData(CMD_ALL_SORT_START, 0, BOARD_TYPE_INTERFACE, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        mySerial.pushCom1CmdData(CMD_ALL_SORT_START, 0, BOARD_TYPE_CONTROL, BOARD_TYPE_ALL_IN_CHAIN, data, 3);

        for(i = 0; i < struCnfg.nLayerTotal; i++)
        {
            for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
            {
                if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
                board = struCnfg.struLayerInfo[i].nViewBoardType[j];
                mySerial.pushCom1CmdData(CMD_ALL_SORT_START, 0, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
            }

            for(j = 0; j < struCnfg.struLayerInfo[i].nVavleChainTotal; j++)
            {
                if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
                board = struCnfg.struLayerInfo[i].nVavleBoardType[j];
                mySerial.pushCom1CmdData(CMD_ALL_SORT_START, 0, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
            }
        }
    }
    else
    {
        mySerial.pushCom1CmdData(CMD_ALL_SORT_START, 0, BOARD_TYPE_INTERFACE, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
    }
}

/* 发送控制板参数 */
void GlobalFlow::resetControl()
{
    /* 整机控制 */
    resetSystemControlRG();

    /* 初始化履带电机伺服*/
    resetConveyor();

    /* 设置供料量 */
    resetFeederRG();

    /* 设置清灰 */
    resetWiperRG(0x55);

    /* 灯光控制 */
    resetLampRG();

    /* 报警设置,报警时，是否关闭振动器 */
    resetControlAlarmRG();

    /* 发送加速度传感器参数 */
    if(struCnfg.accChecked)
        resetAccelerationParam();
}


/* 柔性供料料位判定 1*/
void GlobalFlow::autoFeedRadarLevelJudge()
{
    if(struCnfp.bEnableBalanceFeed == 1)
    {
        if(struCnfg.stuFeedAutoCtrl.isHasRadar == 1)    //雷达传感器
        {
            for(int i = 0; i < MAX_GROUP_TICK; i++)
            {
                myAutoFeed.clearBalStateCount(i);
                myAutoFeed.mode1ClearBalStateCount(i);
                //myAutoFeed.mode2ClearBalStateCount(i);
            }
            myAutoFeed.mode2ClearBalStateCount();

            if(struCnfg.stuFeedAutoCtrl.autoFeedMode == AUTO_FEED_MODE_0)
            {
                myFlow.getCurRadarLevel(AUTO_FEED_MODE_0);
            }
            else if(struCnfg.stuFeedAutoCtrl.autoFeedMode == AUTO_FEED_MODE_1)
            {
                myAutoFeed.getCurRadarLevelMode1WhileTurnOn();
            }
            else if(struCnfg.stuFeedAutoCtrl.autoFeedMode == AUTO_FEED_MODE_2)
            {
                myAutoFeed.getCurRadarLevelMode2WhileTurnOn();
            }
            else if(struCnfg.stuFeedAutoCtrl.autoFeedMode == AUTO_FEED_MODE_3)
            {
                myAutoFeed.getCurRadarLevelMode3WhileTurnOn();
            }
        }
        else if(struCnfg.stuFeedAutoCtrl.isHasRadar == 2) //料位开关
        {
            if(struCnfg.stuFeedAutoCtrl.autoFeedMode == AUTO_FEED_MODE_1)
            {
                for(int i = 0; i < MAX_GROUP_TICK; i++)
                {
                    myAutoFeed.modeSwitchClearBalStateCount(i);
                }
                myAutoFeed.getCurFeedLevelModeSwitchWhileTurnOn();
            }
        }

        myAutoFeed.enbleFreshFeedPage(true);

        //        qDebug("000 struFeedAutoConfigMode1.curBalanceStateByLevel is %d", myAutoFeed.struFeedAutoConfigMode1.curBalanceStateByLevel);
        //        qDebug("000 struFeedAutoConfigMode1.mode1BalancePart is %d",myAutoFeed.struFeedAutoConfigMode1.mode1BalancePart);
        //        qDebug("000 current target  is %d",struCnfp.nAutoFeed[0][0].mode1FeedTargetPart[myAutoFeed.struFeedAutoConfigMode1.mode1BalancePart]);
    }
}

/* 柔性供料料位判定 2*/
void GlobalFlow::getCurRadarLevel(int mode)
{
    int view = 0;
    int px = struGsh.struMachineStatus[struGsh.nLayer].nFeedLvelValue[0];
    int f1, f2, f3;
    int p1, p2, p3;

    if(mode == AUTO_FEED_MODE_0)
    {
        p1 = struCnfg.stuFeedAutoCtrl.levelEmptyPosi;
        p2 = struCnfg.stuFeedAutoCtrl.levelMorePosi;
        p3 = struCnfg.stuFeedAutoCtrl.levelFullPosi;

        f1 = struCnfg.stuFeedAutoCtrl.feedEmpytLowLimitValue;
        f2 = struCnfg.stuFeedAutoCtrl.feedMoreStartValue;;
        f3 = 99;

        if(px < struCnfg.stuFeedAutoCtrl.levelFullPosi)    //料满
        {
            struGsh.curBalanceStateByLevel = STAT_LEVEL_FULL;
        }
        else if(px < struCnfg.stuFeedAutoCtrl.levelMorePosi)    // 料多
        {
            struGsh.curBalanceStateByLevel = STAT_LEVEL_MORE;
        }
        else if(px < struCnfg.stuFeedAutoCtrl.levelEmptyPosi)   // 料匀
        {
            struGsh.curBalanceStateByLevel = STAT_LEVEL_BALANCE;
        }
        else                                                    // 料空
        {
            struGsh.curBalanceStateByLevel = STAT_LEVEL_EMPTY;
        }

        struGsh.lastBalanceStateByLevel = struGsh.curBalanceStateByLevel;

        switch(struGsh.curBalanceStateByLevel)
        {
        case STAT_LEVEL_EMPTY:
            struGsh.feedByRadar = f1;
            break;
        case STAT_LEVEL_BALANCE:
            struGsh.feedByRadar = (f2*(p1-px)+f1*(px-p2))/(p1-p2);
            break;
        case STAT_LEVEL_MORE:
            struGsh.feedByRadar = (f2*(p1-px)+f1*(px-p2))/(p1-p2);
            break;
        case STAT_LEVEL_FULL:
            struGsh.feedByRadar = struCnfg.stuFeedAutoCtrl.feedMoreStartValue;
            break;
        }

        for(int i = 0;i < struCnfg.nLayerTotal;i++){
            for(int j = 0;j < struCnfg.struLayerInfo[i].stuViewInfo[view].nIdentifyGroupTotal;j++)
            {
                for(int k = 0;k < struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nGroupUnitTotal;k++)
                {
                    int unitIndex = struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nUnitGupId[k]%128;

                    struGsh.feederRealSend[unitIndex] = struGsh.feedByRadar+struCnfg.stuFeedAutoCtrl.nFeedValueBias[unitIndex]-100;

                    struGsh.feederRealSend[unitIndex] = (struGsh.feederRealSend[unitIndex] <= 100) ? struGsh.feederRealSend[unitIndex] : 100;
                    struGsh.feederRealSend[unitIndex] = (struGsh.feederRealSend[unitIndex] > 0) ? struGsh.feederRealSend[unitIndex] : 0;
                }
            }
        }
    }
}

void GlobalFlow::resetFeederClear()
{
    int i, j;
    char data[SEND_PACKET_DATA_SIZE];
    int totalFeed;
    int tmpMainFeedValue[MAX_FEEDER];

    if((struCnfp.bEnableBalanceFeed == 1)&&(struCnfg.stuFeedAutoCtrl.isHasRadar == 1)&&(struCnfg.stuFeedAutoCtrl.autoFeedMode == AUTO_FEED_MODE_0))
    {
        for(i = 0; i < MAX_FEEDER; i++)
        {
            tmpMainFeedValue[i] = struGsh.feederRealSend[i];
        }
    }
    else
    {
        for(i = 0; i < MAX_FEEDER; i++)
        {
            tmpMainFeedValue[i] = struCnfp.struGroupCtrl[0].nFeederValueMain[i];
        }
    }

    if((struCnfg.nMachineSpliceType == MACHINE_SPLICE_TYPE_BIG)&&(struCnfg.nChuteTotal > 1 || (struCnfg.nChuteTotal == 1 && !struCnfg.nAllInOne)))
    {
        totalFeed = struCnfg.nChuteTotal+1;
    }
    else
    {
        totalFeed = struCnfg.nChuteTotal;
    }

    memset(data, 0, SEND_PACKET_DATA_SIZE);

    switch(struCnfg.nMachineType)
    {
    case MACHINE_VERTICAL_DM_RG:
    case MACHINE_VERTICAL_DM_ZK:
    case MACHINE_VERTICAL_ZL_CS:
    case MACHINE_VERTICAL_ZL_CV:
        if(struGsh.bSortStart == 1)
        {
            struGsh.feedMsecond = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000;

            for(i = 0; i < totalFeed; i++)
            {
                data[i] = (int)(struCnfg.stuFeedAutoCtrl.feedClearModeValue*2.55);
            }
        }
        else{
            if(struGsh.feedMsecond != 0){
                int tempFeedMsecond = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000-struGsh.feedMsecond;
                struCnfg.ntotalFeedTime += tempFeedMsecond;
                struGsh.feedMsecond = 0;
            }
        }
        //        memcpy(struGsh.chuteFeedMainValue,struCnfp.struGroupCtrl[0].nFeederValueMain,sizeof(struCnfp.struGroupCtrl[0].nFeederValueMain));
        //memcpy(struCnfp.struGroupCtrl[0].nFeederValueMain,tmpMainFeedValue,sizeof(tmpMainFeedValue));
        mySerial.pushCom1CmdData(CMD_CTRL_FEED_SET, 0, struCnfg.nCtrlBoardType, 0, data, 3); //关闭下料时， data全部为0
        break;
    default:
        if(struGsh.bSortStart == 1)
        {
            struGsh.feedMsecond = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000;

            if(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeedSetType == 2)    //清仓模式
            {
                for(i = 0; i < totalFeed; i++)
                {
                    data[i] = (int)(struCnfg.stuFeedAutoCtrl.feedClearModeValue*2.55);
                }
            }
        }else{
            if(struGsh.feedMsecond != 0){
                int tempFeedMsecond = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000-struGsh.feedMsecond;
                struCnfg.ntotalFeedTime += tempFeedMsecond;
                struGsh.feedMsecond = 0;
            }
        }

        //memcpy(struCnfp.struGroupCtrl[0].nFeederValueMain,tmpMainFeedValue,sizeof(tmpMainFeedValue));
        mySerial.pushCom1CmdData(CMD_CTRL_FEED_SET, 0, struCnfg.nCtrlBoardType, 0, data, 3); //关闭下料时， data全部为0
        resetConveyorSpeed();
        break;
    }

}

/* 发送控制振动量 */
/**************************************************************************************************************************
 *
 *255*（12.5/50）= 63.75， 63.75≈63
  255*（43.1/50）= 219.81，219.81≈219
 用0~100映射到63~219
 *
 *
 * ************************************************************************************************************************/
void GlobalFlow::resetFeederRG()
{
    int i, j;
    char data[SEND_PACKET_DATA_SIZE];
    int totalFeed;

    if((struCnfg.nMachineSpliceType == MACHINE_SPLICE_TYPE_BIG)&&(struCnfg.nChuteTotal > 1 || (struCnfg.nChuteTotal == 1 && !struCnfg.nAllInOne)))
    {
        totalFeed = struCnfg.nChuteTotal+1;
    }
    else
    {
        totalFeed = struCnfg.nChuteTotal;
    }

    memset(data, 0, SEND_PACKET_DATA_SIZE);

    switch(struCnfg.nMachineType)
    {
    case MACHINE_VERTICAL_DM_RG:
    case MACHINE_VERTICAL_DM_ZK:
    case MACHINE_VERTICAL_ZL_CS:
    case MACHINE_VERTICAL_ZL_CV:
        if(struGsh.bSortStart == 1)
        {
            if(struCnfg.nFeedControlType1 == 0)
            {
                for(i = 0; i < totalFeed; i++)
                {
                    data[i] = (int)(struCnfp.struGroupCtrl[0].nFeederValueMain[i]*2.55);
                }
                /* 辅振动器 */
                if(struCnfg.nFeedViceTotal == 1)
                {
                    data[i] = (int)(struCnfp.struGroupCtrl[0].nFeederValueVice[0]*2.55);
                }
            }
            else
            {
                for(i = 0; i < totalFeed; i++)
                {
                    // data[i] = 63+(int)(struCnfp.struGroupCtrl[0].nFeederValueMain[i]*1.56);
                    data[i] = (struCnfp.struGroupCtrl[0].nFeederValueMain[i] == 0) ? 0 : feedconst+(int)(struCnfp.struGroupCtrl[0].nFeederValueMain[i]*1.55);
                }
                /* 辅振动器 */
                if(struCnfg.nFeedViceTotal == 1)
                {
                    // data[i] = 63+(int)(struCnfp.struGroupCtrl[0].nFeederValueVice[0]*1.56);
                    data[i] = (struCnfp.struGroupCtrl[0].nFeederValueVice[0] == 0) ? 0 : feedconst+(int)(struCnfp.struGroupCtrl[0].nFeederValueVice[0]*1.55);
                }
            }
        }

        if (struCnfg.bigVibIsUse){
            data[struCnfg.nBigVibPos-1] = (int)(struCnfp.struGroupCtrl[0].nFeederTea*2.55);
        }
        memcpy(struGsh.chuteFeedMainValue,struCnfp.struGroupCtrl[0].nFeederValueMain,sizeof(struCnfp.struGroupCtrl[0].nFeederValueMain));
        mySerial.pushCom1CmdData(CMD_CTRL_FEED_SET, 0, struCnfg.nCtrlBoardType, 0, data, 3); //关闭下料时， data全部为0
        break;
    default:
        if(struGsh.bSortStart == 1)
        {
            if(struCnfg.nFeedControlType1 == 0)
            {
                for(i = 0; i < totalFeed; i++)
                {
                    data[i] = (int)(struCnfp.struGroupCtrl[0].nFeederValueMain[i]*2.55);
                }
                /* 辅振动器 */
                if(struCnfg.nFeedViceTotal == 1)
                {
                    data[i] = (int)(struCnfp.struGroupCtrl[0].nFeederValueVice[0]*2.55);
                }
            }
            else
            {
                for(i = 0; i < totalFeed; i++)
                {
                    data[i] = (struCnfp.struGroupCtrl[0].nFeederValueMain[i] == 0) ? 0 : feedconst+(int)(struCnfp.struGroupCtrl[0].nFeederValueMain[i]*1.55);
                }
                /* 辅振动器 */
                if(struCnfg.nFeedViceTotal == 1)
                {
                    data[i] = (struCnfp.struGroupCtrl[0].nFeederValueVice[0] == 0) ? 0 : feedconst+(int)(struCnfp.struGroupCtrl[0].nFeederValueVice[0]*1.55);
                }
            }
        }

        if (struCnfg.bigVibIsUse){
            data[struCnfg.nBigVibPos-1] = (int)(struCnfp.struGroupCtrl[0].nFeederTea*2.55);
        }
        memcpy(struGsh.chuteFeedMainValue,struCnfp.struGroupCtrl[0].nFeederValueMain,sizeof(struCnfp.struGroupCtrl[0].nFeederValueMain));
        mySerial.pushCom1CmdData(CMD_CTRL_FEED_SET, 0, struCnfg.nCtrlBoardType, 0, data, 3); //关闭下料时， data全部为0
        resetConveyorSpeed();
        break;
    }

#ifdef Q_OS_UNIX
//    myMqttMsgParaseThread->n_uploadRealTimeParaFlag = 1;   //下料开与关
#endif
}

void GlobalFlow::resetConveyor()
{
    MotorCtrl_Init();
}

void GlobalFlow::resetConveyorSpeed()
{
    if(struCnfg.nLVMachine){
        //        if(struGsh.bConveyorOnOff)
        //        {
        //            MotorCtrl_Speed(struCnfp.struGroupCtrl[0].nConveyorSpeed);
        //        }
        //        else
        //        {
        //            MotorCtrl_Speed(0);
        //        }
        MotorCtrl_Speed((struGsh.bConveyorOnOff==1)?struCnfp.struGroupCtrl[0].nConveyorSpeed:0);
    }
    else{
        MotorCtrl_Speed((struGsh.bSortStart==1)?struCnfp.struGroupCtrl[0].nConveyorSpeed:0);
    }

}

void GlobalFlow::resetAutoFeeder()
{
    int i, j;
    char data[SEND_PACKET_DATA_SIZE];
    int totalFeed;

    if((struCnfg.nMachineSpliceType == MACHINE_SPLICE_TYPE_BIG)&&(struCnfg.nChuteTotal > 1 || (struCnfg.nChuteTotal == 1 && !struCnfg.nAllInOne)))
    {
        totalFeed = struCnfg.nChuteTotal+1;
    }
    else
    {
        totalFeed = struCnfg.nChuteTotal;
    }

    memset(data, 0, SEND_PACKET_DATA_SIZE);


    if(struGsh.bSortStart == 1)
    {
        struGsh.feedMsecond = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000;
        //        for(i = 0; i < struCnfg.nChuteTotal; i++)
        //        {
        //            data[i] = (int)(struGsh.chuteFeedMainValue[i]*2.55);
        //        }
        if(struCnfg.nFeedControlType1 == 0)
        {
            for(i = 0; i < totalFeed; i++)
            {
                data[i] = (int)(struGsh.chuteFeedMainValue[i]*2.55);
            }
        }
        else
        {
            for(i = 0; i < totalFeed; i++)
            {
                data[i] = (struGsh.chuteFeedMainValue[i]== 0) ? 0 : 63+(int)(struGsh.chuteFeedMainValue[i]*1.56);
            }
        }
    }else{
        if(struGsh.feedMsecond != 0){
            int tempFeedMsecond = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000-struGsh.feedMsecond;
            struCnfg.ntotalFeedTime += tempFeedMsecond;
            struGsh.feedMsecond = 0;
        }
    }
    mySerial.pushCom1CmdData(CMD_CTRL_FEED_SET, 0, struCnfg.nCtrlBoardType, 0, data, 3); //关闭下料时， data全部为0

}

/* 整机控制控制 */
void GlobalFlow::resetSystemControlRG()
{
    int i, j;
    int board;
    char data[SEND_PACKET_DATA_SIZE];

    memset(data, 0, SEND_PACKET_DATA_SIZE);
    //    data[0] = struCnfp.struGroupCtrl[struGsh.nLayer].nWipeUnion;
    data[0] = 0;    //强制全部定时清灰
    data[1] = struCnfg.nSpray;
    data[2] = struCnfp.struGroupCtrl[struGsh.nLayer].nWipeTimes;
    data[3] = struCnfg.nVibType;
    data[4] = 0;
    data[5] = struCnfg.nVibFreq;
    data[6] = struCnfg.struAlarmConfig.nAirPressureAlarmType;
    data[7] = struCnfg.bigVibIsUse?struCnfg.nBigVibPos:0;
    data[8] = struCnfg.nVibFreq_2;

    mySerial.pushCom1CmdData(CMD_CTRL_MISC_CONTROL, 0, BOARD_TYPE_CONTROL, 0, data, 3);   //默认第一个控制板清灰
}

/* 清灰控制，控制板，相机板 */
void GlobalFlow::resetWiperRG(int flag)
{
    int i, j;
    int board;
    char data[SEND_PACKET_DATA_SIZE];

    memset(data, 0, SEND_PACKET_DATA_SIZE);
    data[0] = flag;
    data[1] = struCnfp.struGroupCtrl[struGsh.nLayer].nWipeDuration-3;
    data[2] = struCnfp.struGroupCtrl[struGsh.nLayer].nWipeInterval;
    data[3] = struCnfg.nWipeEnable;

    mySerial.pushCom1CmdData(CMD_ALL_WIPE_SET, 0, BOARD_TYPE_CONTROL, BOARD_TYPE_ALL_IN_CHAIN, data, 3);

    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        /* 发送给相机清灰控制 */
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
            mySerial.pushCom1CmdData(CMD_ALL_WIPE_SET, 0, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        }
    }
}

/* 灯光控制 */
void GlobalFlow::resetLampRG()
{
    int i, j, m;
    char data[SEND_PACKET_DATA_SIZE];
    /*
    float lampQuo = 0.0;
    switch(struCnfg.nLampBoardQuoType)
    {
    case LAMP_BOARD_QUO_TYPE_M:
        lampQuo = (struCnfg.nChuteTotal+1)/11.0;
        break;
    case LAMP_BOARD_QUO_TYPE_L1:
        lampQuo = 2/8.5;
        break;
    case LAMP_BOARD_QUO_TYPE_L2:
        lampQuo = 3/8.5;
        break;
    case LAMP_BOARD_QUO_TYPE_L3:
        lampQuo = 4/8.5;
        break;
    default:
        lampQuo = (struCnfg.nChuteTotal+1)/11.0;
        break;
    }
*/
    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        memset(data, 0, SEND_PACKET_DATA_SIZE);
        if(struCnfg.nAllInOne){
            j = MAX_LAMP_BOARD_PER_INTERFACE-2;
            for(m = 0; m < MAX_LAMP_PER_BOARD; m++)
            {
                data[m] = (int)(struCnfp.nLampValue[i][j][m]*2.55*struCnfg.stuLayerLampCfg.fLampBoardQuo[j][m]);
            }
            mySerial.pushCom1CmdData(CMD_LAMP_BRIGHT, 0, BOARD_TYPE_LAMP, 0, data, 3);
        }
        for(j = 0; j < struCnfg.stuLayerLampCfg.nLampBoardTotal; j++)
        {
            for(m = 0; m < MAX_LAMP_PER_BOARD; m++)
            {
                data[m] = (int)(struCnfp.nLampValue[i][j][m]*2.55*struCnfg.stuLayerLampCfg.fLampBoardQuo[j][m]);
            }
            if(struCnfg.nAllInOne){
                mySerial.pushCom1CmdData(CMD_LAMP_BRIGHT, 0, BOARD_TYPE_LAMP, j+1, data, 3);
            }else{
                mySerial.pushCom1CmdData(CMD_LAMP_BRIGHT, 0, BOARD_TYPE_LAMP, j, data, 3);
            }
        }
    }

    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        memset(data, 0, SEND_PACKET_DATA_SIZE);
        if(struCnfg.nAllInOne){
            j = MAX_LAMP_BOARD_PER_INTERFACE-2;
            for(m = 0; m < MAX_LAMP_PER_BOARD; m++)
            {
                data[m] = struCnfp.nLampEnable[i][j][m];
            }
            mySerial.pushCom1CmdData(CMD_LAMP_ON_OFF, 0, BOARD_TYPE_LAMP, 0, data, 3);
        }
        for(j = 0; j < struCnfg.stuLayerLampCfg.nLampBoardTotal; j++)
        {
            for(m = 0; m < MAX_LAMP_PER_BOARD; m++)
            {
                data[m] = struCnfp.nLampEnable[i][j][m];
            }
            if(struCnfg.nAllInOne){
                mySerial.pushCom1CmdData(CMD_LAMP_ON_OFF, 0, BOARD_TYPE_LAMP, j+1, data, 3);
            }else{
                mySerial.pushCom1CmdData(CMD_LAMP_ON_OFF, 0, BOARD_TYPE_LAMP, j, data, 3);
            }
        }
    }

    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        memset(data, 0, SEND_PACKET_DATA_SIZE);
        if(struCnfg.nAllInOne){
            j = MAX_LAMP_BOARD_PER_INTERFACE-2;
            for(m = 0; m < MAX_LAMP_PER_BOARD; m++)
            {
                data[m] = struCnfp.nLampSeq[i][j][m];
            }
            mySerial.pushCom1CmdData(CMD_INT_LIGHT_ORDER, 0, BOARD_TYPE_LAMP, 0, data, 3);
        }
        for(j = 0; j < struCnfg.stuLayerLampCfg.nLampBoardTotal; j++)
        {
            for(m = 0; m < MAX_LAMP_PER_BOARD; m++)
            {
                data[m] = struCnfp.nLampSeq[i][j][m];
            }
            if(struCnfg.nAllInOne){
                mySerial.pushCom1CmdData(CMD_INT_LIGHT_ORDER, 0, BOARD_TYPE_LAMP, j+1, data, 3);
            }else{
                mySerial.pushCom1CmdData(CMD_INT_LIGHT_ORDER, 0, BOARD_TYPE_LAMP, j, data, 3);
            }
        }
    }
}

void GlobalFlow::resetLampPlcOff()
{
    logger()->info("resetLampPlcOff");
    int i, j, m;
    char data[SEND_PACKET_DATA_SIZE];
    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        memset(data, 0, SEND_PACKET_DATA_SIZE);
        if(struCnfg.nAllInOne){
            j = MAX_LAMP_BOARD_PER_INTERFACE-2;
            for(m = 0; m < MAX_LAMP_PER_BOARD; m++)
            {
                data[m] = (int)(struCnfp.nLampValue[i][j][m]*2.55*struCnfg.stuLayerLampCfg.fLampBoardQuo[j][m]);
            }
            mySerial.pushCom1CmdData(CMD_LAMP_BRIGHT, 0, BOARD_TYPE_LAMP, 0, data, 3);
        }
        for(j = 0; j < struCnfg.stuLayerLampCfg.nLampBoardTotal; j++)
        {
            for(m = 0; m < MAX_LAMP_PER_BOARD; m++)
            {
                data[m] = (int)(struCnfp.nLampValue[i][j][m]*2.55*struCnfg.stuLayerLampCfg.fLampBoardQuo[j][m]);
            }
            if(struCnfg.nAllInOne){
                mySerial.pushCom1CmdData(CMD_LAMP_BRIGHT, 0, BOARD_TYPE_LAMP, j+1, data, 3);
            }else{
                mySerial.pushCom1CmdData(CMD_LAMP_BRIGHT, 0, BOARD_TYPE_LAMP, j, data, 3);
            }
        }
    }
    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        memset(data, 0, SEND_PACKET_DATA_SIZE);
        if(struCnfg.nAllInOne){
            j = MAX_LAMP_BOARD_PER_INTERFACE-2;
            for(m = 0; m < MAX_LAMP_PER_BOARD; m++)
            {
                data[m] = 0;
            }
            mySerial.pushCom1CmdData(CMD_LAMP_ON_OFF, 0, BOARD_TYPE_LAMP, 0, data, 3);
        }
        for(j = 0; j < struCnfg.stuLayerLampCfg.nLampBoardTotal; j++)
        {
            for(m = 0; m < MAX_LAMP_PER_BOARD; m++)
            {
                data[m] = 0;
            }
            if(struCnfg.nAllInOne){
                mySerial.pushCom1CmdData(CMD_LAMP_ON_OFF, 0, BOARD_TYPE_LAMP, j+1, data, 3);
            }else{
                mySerial.pushCom1CmdData(CMD_LAMP_ON_OFF, 0, BOARD_TYPE_LAMP, j, data, 3);
            }
        }
    }
}
/* 电机控制 -透传命令*/
void GlobalFlow::resetMotor()
{
    char data[SEND_PACKET_DATA_SIZE];

    memset(data, 0, SEND_PACKET_DATA_SIZE);
    data[0] = 0;
    data[1] = 10;

    for(int i = 0; i < 10; i++)
    {
        data[i+2] = i;
    }

    mySerial.pushCom1CmdData(CMD_CTRL_TRANSPARENT, 0, BOARD_TYPE_CONTROL, 0, data, 3);   //默认第一个控制板清灰
}

/* 报警设置+声光报警*/
void GlobalFlow::resetControlAlarmRG()
{
    char data[SEND_PACKET_DATA_SIZE];

    memset(data, 0, SEND_PACKET_DATA_SIZE);
    //    data[0] = (struCnfg.nControlAlarmSet && struGsh.nAlarmSetData0);
    data[0] = 0;

    // 不用了
    //    if((struCnfg.nAlarmValid[ALARM_IPC] == 1)&&((struGsh.bAlarmIpcStatus[0]|struGsh.bAlarmIpcStatus[1]) == 1))
    //    {
    //        struGsh.struMachineStatus[struGsh.nLayer].nColorVoiceAlarmValue = 1;
    //        data[0] = 0;
    //    }
    //    else
    //    {
    //        struGsh.struMachineStatus[struGsh.nLayer].nColorVoiceAlarmValue = 0;
    //    }

    if(struGsh.nAlarmStautsCurrent[ALARM_CLASS] || struGsh.nAlarmStautsCurrent[ALARM_IPC])
    {
        struGsh.struMachineStatus[struGsh.nLayer].nColorVoiceAlarmValue = 1;
    }
    else
    {
        struGsh.struMachineStatus[struGsh.nLayer].nColorVoiceAlarmValue = 0;
    }

    switch(struCnfg.struAlarmConfig.nLightVoiceAlarmType)
    {
    case 0: //不使能
        break;
    case 1: //使能灯光+声音
        if(struGsh.nAlarmSetData0 || struGsh.struMachineStatus[struGsh.nLayer].nColorVoiceAlarmValue == 1)
        {
            data[1] = 0x09;
        }
        break;
    case 2: //仅灯光
        if(struGsh.nAlarmSetData0 || struGsh.struMachineStatus[struGsh.nLayer].nColorVoiceAlarmValue == 1)
        {
            data[1] = 0x01;
        }
        break;
    case 3: //仅声音
        if(struGsh.nAlarmSetData0 || struGsh.struMachineStatus[struGsh.nLayer].nColorVoiceAlarmValue == 1)
        {
            data[1] = 0x08;
        }
        break;
    default:
        break;
    }
    data[2] = struGsh.bConveyorOnOff<<1;

    for(int i  =0 ; i < struCnfg.bLogoLightNum; i++)
    {
        if(struGsh.bAlarmStatus == 1)
        {
            data[3+i*2] = 3;
        }
        else if(struGsh.bSortStart == 1)
        {
            data[3+i*2] = 0;
        }
        else
        {
            data[3+i*2] = struCnfg.bLogoLightMode[i];
        }

        data[4+i*2] = struCnfg.bLogoLightLength[i];
    }

    mySerial.pushCom1CmdData(CMD_CTRL_ALARM_SET, 0, BOARD_TYPE_CONTROL, BOARD_TYPE_ALL_IN_CHAIN, data, 3);   //默认第一个控制板清灰
}

/* 接口板，老化测试时的自检频率 */
void GlobalFlow::resetAgeTestFre(int onOff)
{
    int i, j, k;
    int nInt=0, board;
    char data[SEND_PACKET_DATA_SIZE];

    /* 发送接口板 自检频率*/
    for(i = 0; i < struCnfg.nLayerTotal; i++)  //层
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nVavleChainTotal; j++)  // 视
        {
            board = struCnfg.struLayerInfo[i].nVavleBoardType[j];
            memset(data, 0, SEND_PACKET_DATA_SIZE);

            data[0] = struGsh.ageTestEnable;
            data[1] = struGsh.ageTestFre;

            if(onOff == 1)
            {
                if(struGsh.ageTestAll){
                    for(k = 0; k < struCnfg.struLayerInfo[i].nVavleBoardTotal[j]; k++)
                    {
                        data[2+k] = 1;
                    }
                }else{
                    data[2+struGsh.ageTestChute-1] = 1;
                }
            }
            mySerial.pushCom1CmdData(CMD_INT_VAVLE_AGING_TEST, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        }
        mySerial.pushCom1CmdData(CMD_INT_VAVLE_AGING_TEST, nInt, BOARD_TYPE_INTERFACE, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
    }
}

QString GlobalFlow::getImageShowName(QString imagePath)
{
    QString path = imagePath.split(".").at(0);
    QStringList list = path.split("/");
    QStringList namelist = list.last().split("_");
    if(namelist.count() < 3)
        return QString("");
    QString viewName="*";
    QString type="";

    QString fsType = namelist.last();
    if(!fsType.contains("A") && !fsType.contains("B"))
        fsType.clear();
    if(struCnfg.nMachineType == MACHINE_LD2_ZL_GP){
        type = (namelist[1].toInt() > 3) ? g_myLan().upper_layer : g_myLan().lower_layer;
    }
    switch (namelist[1].toInt()) {
    case 0:
    case 4:
        viewName = QString("%1%2%3").arg(type).arg(g_myLan().ids_fm).arg(namelist[2].toInt()+1);
        break;
    case 1:
    case 5:
        viewName = QString("%1%2%3").arg(type).arg(g_myLan().ids_bm).arg(namelist[2].toInt()+1);
        break;
    case 2:
    case 6:
        viewName = QString("%1%2%3").arg(type).arg(g_myLan().ids_fs).arg(namelist[2].toInt()+1);
        break;
    case 3:
    case 7:
        viewName = QString("%1%2%3").arg(type).arg(g_myLan().ids_bs).arg(namelist[2].toInt()+1);
        break;
    default:
        break;
    }
    if(fsType.contains("A")) struGsh.nImgTdMode = 1;
    if(fsType.contains("B")) struGsh.nImgTdMode = 2;
    if(fsType.isEmpty()) struGsh.nImgTdMode = 0;
    return (viewName+fsType);
}

QString GlobalFlow::getUnitShowName(int view, int unit)
{
    QString viewName;
    QString type="";
    if(struCnfg.nMachineType == MACHINE_LD2_ZL_GP){
        type = (view > 3) ? g_myLan().upper_layer : g_myLan().lower_layer;
    }
    switch (view) {
    case 0:
    case 4:
        viewName = QString("%1%2 %3").arg(type).arg(g_myLan().ids_fm).arg(unit+1);
        break;
    case 1:
    case 5:
        viewName = QString("%1%2 %3").arg(type).arg(g_myLan().ids_bm).arg(unit+1);
        break;
    case 2:
    case 6:
        viewName = QString("%1%2 %3").arg(type).arg(g_myLan().ids_fs).arg(unit+1);
        break;
    case 3:
    case 7:
        viewName = QString("%1%2 %3").arg(type).arg(g_myLan().ids_bs).arg(unit+1);
        break;
    default:
        break;
    }
    return viewName;
}

/* 清灰操作 */
void GlobalFlow::startWipe()
{
    int nStatFeed = 0;
    int nStatSort = 0;
    int flagBacklight = 0;
    int ret = 0, needSleep;

    if(struGsh.nBacklightStat == 0) {
        flagBacklight = 1;
        myFlow.setTsBackLight(flagBacklight);
    }
    /* 上位机定时清灰 */
    //    struGsh.nomalWipeCount ++;
    struGsh.nCurWiperLeftOrRight += (struCnfp.struGroupCtrl[0].nWipeTimes == 0) ? 0 : 1;

    if(struGsh.bSortStart == 1) {
        if(!struCnfp.struGroupCtrl[0].nWipeFeedOnOff){
            nStatFeed = 1;
            struGsh.bSortStart = 0;
            myFlow.resetFeederRG();
            myFlow.sleep(struCnfg.nOnoffDelay);
        }
        if(!struCnfp.struGroupCtrl[0].nWipeSortOnOff && struGsh.bStatEject){
            nStatSort = 1;
            struGsh.bStatEject = 0;
            myFlow.vavleOnOff();
        }
    }

    resetWiperRG(0xaa);

    myFlow.sleep(struCnfp.struGroupCtrl[0].nWipeDuration);

    if(struCnfg.nRealTimeCorrection)
    {
        if((nStatFeed == 1) || (nStatSort == 1))
        {
            myFlow.resetSortOnOff(0, 0);
            myFlow.cameraBrightCalibrateRG(true, struGsh.nLayer, struGsh.nView, struGsh.nUnit, 0);
            myFlow.sleep(3);
            myFlow.resetSortOnOff(1, 0);
        }
        else
        {
            myFlow.cameraBrightCalibrateRG(true, struGsh.nLayer, struGsh.nView, struGsh.nUnit, 0);
            myFlow.sleep(3);
        }
    }

    if (nStatSort == 1){
        if(!struCnfp.struGroupCtrl[0].nWipeSortOnOff){
            nStatSort = 0;
            struGsh.bStatEject = 1;
            myFlow.vavleOnOff();
        }
    }
    if (nStatFeed == 1) {
        if(!struCnfp.struGroupCtrl[0].nWipeFeedOnOff){
            //            myFlow.resetSortParams();
            nStatFeed = 0;
            struGsh.bSortStart = 1;
            if(!struGsh.nAlarmSetData0)
                myFlow.resetFeederRG();
        }
    }

    if( flagBacklight == 1) {
        flagBacklight = 0;
        myFlow.setTsBackLight(flagBacklight);
    }

    //    struGsh.bFlagWiping = 0;
}

int GlobalFlow::dealIntelWipe()
{
    int useView = 0, error = 0;   //默认只用前主
    int intelWipeStartChute = 0, tmpWipeInChute;
    int waitTime = 100, maxWaitTime;
    int board, nUnitAddr;
    int tmpId, tmpCount;
    char data[SEND_PACKET_DATA_SIZE];

    struct timezone tz;
    struct timeval tvafter,tvpre;

    int useMsecTime = 0;

    memset(data, 0, SEND_PACKET_DATA_SIZE);

    struGsh.nCurWiperInChute = 255;
    struGsh.nIntelWipeTotalWaitTime = 0;

    for(int i = 0; i < struCnfg.nChuteTotal; i++)
    {
        memset(data, 0, SEND_PACKET_DATA_SIZE);

        board = struCnfg.struLayerInfo[struGsh.nLayer].nViewBoardType[useView];

        if(board == BOARD_CAMERA_1_FRONT_MAIN)
        {
            tmpId = (struGsh.nCurWiperLeftOrRight%2 == 0) ? i : struCnfg.nChuteTotal-1-i;

            if(i == 0)
            {
                maxWaitTime = 50;  //第一次最多等待5秒
                tmpWipeInChute = tmpId;

                /* 关闭下料 */
                resetFeederIntelWipe(struGsh.nCurWiperLeftOrRight, 0, tmpWipeInChute);
                myFlow.msleep(300);
                getWipingCommunicationStatus();

                /* 开始清灰 */
                resetWiperRG(0xaa);
            }
            else
            {
                maxWaitTime = struCnfg.struIntelWipe.wiperDurationPerChute; //区分64喷嘴和80喷嘴
            }

            /* 开始查询清灰刷位置 */
            nUnitAddr = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[useView].nViewUnitId[tmpId];

            for(tmpCount = 0; tmpCount < maxWaitTime; tmpCount++) //等待1秒钟
            {
                mySerial.pushCom1CmdData(CMD_CAMERA_MISC_CAPTURE, 0, board, nUnitAddr, data, 1);
                myFlow.msleep(waitTime);
                struGsh.nIntelWipeTotalWaitTime += waitTime;

                //                qDebug("111 tmpCount is %d, tmpId is %d, tmpWipeInChute is %d, struGsh.nCurWiperInChute is %d", tmpCount, tmpId, tmpWipeInChute, struGsh.nCurWiperInChute);

                if(tmpWipeInChute == struGsh.nCurWiperInChute)
                {
                    if(i==0)
                    {
                        gettimeofday (&tvpre , &tz);
                    }
                    else
                    {
                        gettimeofday (&tvafter , &tz);
                        useMsecTime=(tvafter.tv_sec-tvpre.tv_sec)*1000+(tvafter.tv_usec-tvpre.tv_usec)/1000;

                        //                        qDebug("111 send use time:%d\n", (tvafter.tv_sec-tvpre.tv_sec)*1000+(tvafter.tv_usec-tvpre.tv_usec)/1000);
                        gettimeofday (&tvpre , &tz);

                    }

                    /* 更新关闭下料的滑槽 */
                    tmpWipeInChute = (struGsh.nCurWiperLeftOrRight%2 == 0) ? tmpWipeInChute+1 : tmpWipeInChute-1;
                    //                    qDebug("111 change tmpWipeInChute is %d", tmpWipeInChute);
                    /* 关闭下料 */
                    if((tmpWipeInChute >= 0)&&(tmpWipeInChute < struCnfg.nChuteTotal))
                    {
                        resetFeederIntelWipe(struGsh.nCurWiperLeftOrRight, 0, tmpWipeInChute);
                    }
                    break;
                }
            }
            if(tmpCount == maxWaitTime) //只要有一个通道超时,立即停止智能清灰
            {
                return 1;
            }
        }
    }
    /* 强制打开所有色选和振动器 */
    struGsh.bSortStart = 1;
    myFlow.resetFeederRG();

    /* 清灰刷定位到机器另一边*/
    struGsh.nCurWiperLeftOrRight += 1;

    /* 处理清灰刷需要返回 */
    if(struCnfp.struGroupCtrl[0].nWipeTimes == 0)
    {
        myFlow.sleep(1);
        struGsh.nCurWiperInChute = 255;

        for(int i = 0; i < struCnfg.nChuteTotal; i++)
        {
            memset(data, 0, SEND_PACKET_DATA_SIZE);

            board = struCnfg.struLayerInfo[struGsh.nLayer].nViewBoardType[useView];

            if(board == BOARD_CAMERA_1_FRONT_MAIN)
            {
                tmpId = (struGsh.nCurWiperLeftOrRight%2 == 0) ? i : struCnfg.nChuteTotal-1-i;

                if(i == 0)
                {
                    maxWaitTime = struCnfp.struGroupCtrl[0].nWipeDuration*10/2;  //第一次最多等待清灰周期一半的时间
                    tmpWipeInChute = tmpId;

                    /* 关闭下料 */
                    resetFeederIntelWipe(struGsh.nCurWiperLeftOrRight, 1, tmpWipeInChute);
                }
                else
                {
                    maxWaitTime = struCnfg.struIntelWipe.wiperDurationPerChute; //区分64喷嘴和80喷嘴
                }

                /* 开始查询清灰刷位置 */
                nUnitAddr = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[useView].nViewUnitId[tmpId];

                for(tmpCount = 0; tmpCount < maxWaitTime; tmpCount++) //等待1秒钟
                {
                    mySerial.pushCom1CmdData(CMD_CAMERA_MISC_CAPTURE, 0, board, nUnitAddr, data, 1);
                    myFlow.msleep(waitTime);
                    struGsh.nIntelWipeTotalWaitTime += waitTime;

                    qDebug("222 tmpCount is %d, tmpId is %d, tmpWipeInChute is %d, struGsh.nCurWiperInChute is %d", tmpCount, tmpId, tmpWipeInChute, struGsh.nCurWiperInChute);

                    if(tmpWipeInChute == struGsh.nCurWiperInChute)
                    {
                        if(i==0)
                        {
                            gettimeofday (&tvpre , &tz);
                        }
                        else
                        {
                            gettimeofday (&tvafter , &tz);
                            useMsecTime=(tvafter.tv_sec-tvpre.tv_sec)*1000+(tvafter.tv_usec-tvpre.tv_usec)/1000;

                            //                           qDebug("222 send use time:%d\n", (tvafter.tv_sec-tvpre.tv_sec)*1000+(tvafter.tv_usec-tvpre.tv_usec)/1000);
                            gettimeofday (&tvpre , &tz);

                        }

                        /* 更新关闭下料的滑槽 */
                        tmpWipeInChute = (struGsh.nCurWiperLeftOrRight%2 == 0) ? tmpWipeInChute+1 : tmpWipeInChute-1;

                        /* 关闭下料 */
                        if((tmpWipeInChute >= 0)&&(tmpWipeInChute < struCnfg.nChuteTotal))
                        {
                            //                            qDebug("222 change tmpWipeInChute is %d", tmpWipeInChute);
                            resetFeederIntelWipe(struGsh.nCurWiperLeftOrRight, 1, tmpWipeInChute);
                        }
                        break;
                    }
                }
                if(tmpCount == maxWaitTime) //只要有一个通道超时,立即停止智能清灰
                {
                    return 2;
                }
            }
        }

        /* 强制打开所有色选和振动器 */
        struGsh.bSortStart = 1;
        myFlow.resetFeederRG();

        struGsh.nCurWiperLeftOrRight += 1;
    }

    return 0;
}

/* 发送智能控制控制振动量 */
void GlobalFlow::resetFeederIntelWipe(int direction, int isReturn, int curChute)
{
    int i, j;
    int begin, end;
    char data[SEND_PACKET_DATA_SIZE];

    memset(data, 0, SEND_PACKET_DATA_SIZE);

    /* 清灰刷停靠在机器左边 */
    if(direction%2 == 0)
    {
        struGsh.nCurWiperLeftOrRight = 0;   //清零
        for(i = 0; i < struCnfg.nChuteTotal; i++)
        {
            data[i] = (int)(struCnfp.struGroupCtrl[0].nFeederValueMain[i]*2.55);
        }

        begin = curChute;
        end = curChute+struCnfg.struIntelWipe.feederOnffCount;
        if(end > struCnfg.nChuteTotal)
        {
            end = struCnfg.nChuteTotal;
        }
        for(i = begin; i < end; i++)
        {
            data[i] = 0;
        }
        mySerial.pushCom1CmdData(CMD_CTRL_FEED_SET, 0, struCnfg.nCtrlBoardType, 0, data, 3); //关闭下料时， data全部为0
    }
    else    /* 清灰刷停靠在机器右边 */
    {
        struGsh.nCurWiperLeftOrRight = 1;   //清零

        for(i = 0; i < struCnfg.nChuteTotal; i++)
        {
            data[i] = (int)(struCnfp.struGroupCtrl[0].nFeederValueMain[i]*2.55);
        }

        begin = curChute;
        end = curChute-struCnfg.struIntelWipe.feederOnffCount;
        if(end < 0)
        {
            end = 0;
            data[0] = 0;    //第一个振动器停止
        }

        for(i = begin; i > end; i--)
        {
            data[i] = 0;
        }

        mySerial.pushCom1CmdData(CMD_CTRL_FEED_SET, 0, struCnfg.nCtrlBoardType, 0, data, 3); //关闭下料时， data全部为0
    }
}

/* 清灰的时候，查询当前相机板的通信状态 */
void GlobalFlow::getWipingCommunicationStatus()
{
    int nInt=0, board, nUnitAddr;
    char data[SEND_PACKET_DATA_SIZE];
    int normalWipe = 0;

    if((struCnfp.struGroupCtrl[0].nWipeUnion != 2)  //智能清灰模式
            ||(struCnfg.nChuteTotal < 6)            //5通道及以下机器
            //            ||(struGsh.bSortStart == 0)              //非色选状态
            ||(struGsh.bAlarmStatus == 1))          //气压报警时
    {
        normalWipe = 1;
    }

    for(int i = 0; i < struCnfg.nLayerTotal; i++)  //层
    {
        for(int j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)  // 视
        {
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
            if(struCnfp.nViewSensorType[0][j] == SENSOR_T1)
            {
                board = struCnfg.struLayerInfo[i].nViewBoardType[j];

                if (normalWipe)
                {
                    if (board != BOARD_CAMERA_1_FRONT_MAIN) // 正常清灰模式下，只在第一次清灰时查询，且只查询前主相机
                        continue;
                }

                for(int k = 0; k < struCnfg.struLayerInfo[i].nViewUnitTotal[j]; k++) // 相机单元
                {
                    nUnitAddr = struCnfg.struLayerInfo[nInt].stuViewInfo[j].nViewUnitId[k];

                    if(nUnitAddr > 128) //克服通道分割
                    {
                        continue;
                    }

                    memset(data, 0, SEND_PACKET_DATA_SIZE);

                    struGsh.struCameraStatus[i][j][nUnitAddr].nFirstRequireCmd = 1;

                    mySerial.pushCom1CmdData(CMD_CAMERA_MISC_CAPTURE, nInt, board, nUnitAddr, data, 1);
                    myFlow.msleep(10);
                }
            }
        }
    }
}

/* 加密码 */
void GlobalFlow::writeFpgaDccryptCode()
{
    int i, nUnitAddr = 0;
    char data[SEND_PACKET_DATA_SIZE];

    memset(data, 0, SEND_PACKET_DATA_SIZE);
    data[0] = 1;
    for(i = 0; i < 15; i++)
    {
        data[i+1] = paramDelayCode.delay_code_eeprom[i];
    }
    mySerial.pushCom1CmdData(CMD_INT_ENCRYPT_CODE_WRITE, 0, BOARD_TYPE_INTERFACE, nUnitAddr, data, 3);  //只写第一个接口板

    sleep(1);

    memset(data, 0, SEND_PACKET_DATA_SIZE);
    data[0] = 2;
    for(i = 0; i < DCCRYPT_THIS_CODE_SIZE-15; i++)
    {
        data[i+1] = paramDelayCode.delay_code_eeprom[15+i];
    }
    mySerial.pushCom1CmdData(CMD_INT_ENCRYPT_CODE_WRITE, 0, BOARD_TYPE_INTERFACE, nUnitAddr, data, 3);  //只写第一个接口板
}

/* 加密时间 */
void GlobalFlow::writeFpgaDccryptTime()
{
    char data[SEND_PACKET_DATA_SIZE];
    int nUnitAddr = 0;
    unsigned int nCrcItt=0;
    int year;

    memset(data, 0, SEND_PACKET_DATA_SIZE);

    QString dateText = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");

    year = dateText.left(2).toInt(0,10)*100+dateText.mid(2,2).toInt(0,10);
    data[0] = year/256;
    data[1] = year%256;
    data[2] = dateText.mid(4,2).toInt(0,10);
    data[3] = dateText.mid(6,2).toInt(0,10);
    data[4] = dateText.mid(8,2).toInt(0,10);
    data[5] = dateText.mid(10,2).toInt(0,10);
    data[6] = dateText.right(2).toInt(0,10);

    nCrcItt = mySerial.crcFpga(data, 7);
    data[7] = nCrcItt/256;
    data[8] = nCrcItt%256;

    mySerial.pushCom1CmdData(CMD_INT_ENCRYPT_TIME_WRITE, 0, BOARD_TYPE_INTERFACE, nUnitAddr, data, 3);  //只写第一个接口板
}

/* 判断FPGA和上位机的时间先后 */
void GlobalFlow::writeDccryptBiggerTime()
{
    char strsettime[64];
    char strcurtime[16];
    char strgettime[16];

    time_t curtime;
    struct tm *stru_curtime;

    time(&curtime);
    stru_curtime = localtime(&curtime);
    sprintf(strcurtime, "%04d%02d%02d%02d%02d%02d", 1900+stru_curtime->tm_year, 1+stru_curtime->tm_mon, stru_curtime->tm_mday,
            stru_curtime->tm_hour, stru_curtime->tm_min, stru_curtime->tm_sec);

    int year = struGsh.struDccryptData.nDccryptGetTime[0]*256+struGsh.struDccryptData.nDccryptGetTime[1];
    int month = struGsh.struDccryptData.nDccryptGetTime[2];
    int date = struGsh.struDccryptData.nDccryptGetTime[3];
    int hour = struGsh.struDccryptData.nDccryptGetTime[4];
    int min = struGsh.struDccryptData.nDccryptGetTime[5];
    int sec = struGsh.struDccryptData.nDccryptGetTime[6];

    sprintf(strgettime, "%04d%02d%02d%02d%02d%02d", year, month, date, hour, min, sec);

    //  qDebug("strcurtime is %s, strgettime is %s", strcurtime, strgettime);
    if(strcmp(strgettime, strcurtime) > 0)   //下位机的时间更大,更新上位机时间
    {
        sprintf(strsettime, "date -s \"%04d-%02d-%02d %02d:%02d:%02d\"", year, month, date, hour, min, sec);
        system(strsettime);
        myFlow.sleep(1);
        popen("hwclock -w","r");

//        writeSysTimeToE2prom();
//        writeSysTimeToEmmc();
    }
    else
    {
        writeFpgaDccryptTime();     //上位机的时间更大,更新下位机时间
    }
}

/* FPGA加密状态判断 */
int GlobalFlow::dccryptCodeTimeGet()
{
    int i, nUnitAddr = 0;
    int waitTime = 50;
    int tmpCurDay, tmp30DayStartDay, remainDay;
    char data[SEND_PACKET_DATA_SIZE];

    memset(data, 0, SEND_PACKET_DATA_SIZE);
    struGsh.struDccryptData.bDccryptGetError = ENCRYPT_FPGA_NO_PACKET;
    interfaceEncryptAns = 0;

    if(paramDelayCode.enable != 0xAA)   //上位机未加密
    {
        for(i = 0; i < 1 ; i++)
        {
            mySerial.pushCom1CmdData(CMD_INT_ENCRYPT_PARAMS_REQUIRE, 0, BOARD_TYPE_INTERFACE, nUnitAddr, data, 1);
            for(int tt = 0; tt < 30; tt++)
            {
                msleep(waitTime);
                if(struGsh.struDccryptData.bDccryptGetError != ENCRYPT_FPGA_NO_PACKET)
                {
                    //                    qDebug("struGsh.struDccryptData.bDccryptGetError is %d", struGsh.struDccryptData.bDccryptGetError);
                    if(struGsh.struDccryptData.bDccryptGetError == ENCRYPT_FPGA_BURNING)
                    {
                        return 0;
                    }
                    if(struGsh.struDccryptData.bDccryptGetError != ENCRYPT_FPGA_CODE_FORMAT_ERROR)
                    {
                        if(paramDelayCode30Day.fpagCodeErrCount != 0)
                        {
                            paramDelayCode30Day.fpagCodeErrCount = 0;
                            paramDelayCode30Day.fpgaCodeErrCountWrite();
                        }
                    }

                    if(struGsh.struDccryptData.bDccryptGetError == ENCRYPT_FPGA_NO_ENCRYPT
                            ||(struGsh.struDccryptData.bDccryptGetError == ENCRYPT_FPGA_HAS_DCCRYPT))   //接口板未加密,上位机要判断是否到30天的加密期限
                    {
                        if(paramDelayCode30Day.product30DayUseStartEnable == 0xAA)
                        {
                            if(paramDelayCode30Day.product30DayUseAccHour > DCCRYPT_30DAY_ACC_HOUR)
                            {
                                paramDelayCode30Day.product30DayUseOver = 0xAA;

                                logger()->info("code 4 shutdown");
                                myFlow.sleep(2);
                                paramDelayCode30Day.write30DayInfoToEeprom();    //保存30天临时加密信息
#ifndef Q_OS_WIN
                                myFlow.sleep(1);
                                struGsh.bFlagPowerOff = 1;
                                system("shutdown -h now");
#endif
                                return 1;
                            }
                        }
                        break;
                    }
                    else if(struGsh.struDccryptData.bDccryptGetError == ENCRYPT_FPGA_CODE_FORMAT_ERROR) //密码格式错误，连续3次，立即加密
                    {
                        if(paramDelayCode30Day.fpagCodeErrCount++ > 3)
                        {
                            paramDelayCode30Day.fpagCodeErrCount = 4;
                        }
                        paramDelayCode30Day.fpgaCodeErrCountWrite();
                        myFlow.sleep(3);

                        if(paramDelayCode30Day.fpagCodeErrCount > 3)
                        {
                            paramDelayCode30Day.product30DayUseStartEnable = 0xAA;
                            paramDelayCode30Day.product30DayUseOver = 0xAA;
                            paramDelayCode30Day.code_size_30day = 12;

                            memcpy(paramDelayCode30Day.product30DayUseStartDay, QDateTime::currentDateTime().toString("yyyyMMdd").toLatin1().data(), 8);
                            QString tmpTime = QDateTime::currentDateTime().toString("yyyyMMddhhmmss").right(12);
                            memcpy(paramDelayCode30Day.product30DayUseOnlyIdEeprom, tmpTime.toLatin1().data(), 12);

                            logger()->info("code 4 shutdown");
                            myFlow.sleep(2);
                            paramDelayCode30Day.write30DayInfoToEeprom();    //保存30天临时加密信息
#ifndef Q_OS_WIN
                            myFlow.sleep(1);
                            struGsh.bFlagPowerOff = 1;
                            system("shutdown -h now");
#endif
                            return 1;
                        }
                        break;
                    }
                    else if(struGsh.struDccryptData.bDccryptGetError == ENCRYPT_FPGA_CODE_FORMAT_OK) //上位机未加密机器，需要写入ARM加密码，并自动关机重启
                    {
                        writeDccryptBiggerTime();

                        logger()->info("code 4 shutdown");
                        myFlow.sleep(2);
                        paramDelayCode30Day.clear30DayInfoEeprom();

                        paramDelayCode.writeDelaycode2eeprom();
#ifndef Q_OS_WIN
                        myFlow.sleep(1);
                        struGsh.bFlagPowerOff = 1;
                        system("shutdown -h now");
#endif
                        return 1;
                    }
                }
            }
        }
    }
    else    //上位机已加密
    {
        for(i = 0; i < 1 ; i++)
        {
            mySerial.pushCom1CmdData(CMD_INT_ENCRYPT_PARAMS_REQUIRE, 0, BOARD_TYPE_INTERFACE, nUnitAddr, data, 1);
            for(int tt = 0; tt < 30; tt++)
            {
                msleep(waitTime);
                if(struGsh.struDccryptData.bDccryptGetError != ENCRYPT_FPGA_NO_PACKET)
                {
                    if((struGsh.struDccryptData.bDccryptGetError == ENCRYPT_FPGA_NO_ENCRYPT)                //接口板未加密
                            ||(struGsh.struDccryptData.bDccryptGetError == ENCRYPT_FPGA_CODE_FORMAT_ERROR)  //接口板密码格式错误
                            ||(struGsh.struDccryptData.bDccryptGetError == ENCRYPT_FPGA_HAS_DCCRYPT))       //接口板全解
                    {
                        writeFpgaDccryptCode();
                        writeFpgaDccryptTime();
                        return 0;
                    }
                    else if(struGsh.struDccryptData.bDccryptGetError == ENCRYPT_FPGA_CODE_MISMATCH_LCD)//加密码不匹配，发送FPGA加密码,使用使用期限更短的
                    {
                        writeFpgaDccryptCode(); //使用上位机密码
                        writeDccryptBiggerTime();

                        return 0;
                    }
                    else if(struGsh.struDccryptData.bDccryptGetError == ENCRYPT_FPGA_CODE_MATCH) //上下位机加密码一致，需要修正时间
                    {
                        writeDccryptBiggerTime();

                        return 0;
                    }
                    else if(struGsh.struDccryptData.bDccryptGetError == ENCRYPT_FPGA_CODE_MISMATCH_FPGA) //FPGA加密优先级更高，需要写入ARM加密码，并自动关机重启
                    {
                        writeDccryptBiggerTime();

                        logger()->info("code 4 shutdown");
                        myFlow.sleep(2);
                        paramDelayCode.writeDelaycode2eeprom();
#ifndef Q_OS_WIN
                        myFlow.sleep(1);
                        struGsh.bFlagPowerOff = 1;
                        system("shutdown -h now");
#endif
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

void GlobalFlow::savedelaycode(QString str)
{
    QSettings setting(CFG_APPSet,QSettings::IniFormat);
    setting.setValue("delaycode",str);
    setting.sync();
}

QString GlobalFlow::getFpgaVersionProduct(int value)
{
    QString productType="0";

    if(value == 0)
        productType = "M";
    else if (value < 16)
        productType = "M"+QString::number(value);
    else if (value == 16)
        productType = "Z";
    else if (value < 48)
        productType ="Z"+QString::number(value-15);
    else if (value == 48)
        productType = "C";
    else if (value < 64)
        productType ="C"+QString::number(value-47);
    else if (value == 64)
        productType = "X";
    else if (value < 80)
        productType ="X"+QString::number(value-63);
    else if (value == 80)
        productType = "KS";
    else if (value < 96)
        productType ="KS"+QString::number(value-79);
    else if (value == 96)
        productType = "MT";
    else if (value < 112)
        productType ="MT"+QString::number(value-95);
    else if (value == 112)
        productType = "GF";
    else if (value < 128)
        productType ="GF"+QString::number(value-111);
    else if (value < 146)
        productType ="M"+QString::number(value-111);//128-M17
    else if (value < 178)
        productType ="Z"+QString::number(value-113);//146-Z33
    else if (value < 194)
        productType ="C"+QString::number(value-161);//178-C17
    else if (value < 210)
        productType ="X"+QString::number(value-177);//194-X17
    else if (value < 226)
        productType ="KS"+QString::number(value-193);//210-KS17
    else if (value < 234)
        productType ="MT"+QString::number(value-209);//226-MT17
    else if (value < 256)
        productType ="GF"+QString::number(value-217);//234-GF17


    return productType;
}

/* 通信自检 */
int GlobalFlow::resetCommunication()
{
    int i, j, k, m, n, board, nUnitAddr;
    char data[SEND_PACKET_DATA_SIZE];
    char data_autoBoard[SEND_PACKET_RECTRL_BOARD_SIZE];

    int gap = 4, waitTime = 50;
    int intResult;   // -1 未查询到， 0： 查询到，但FPGA模式错误， 1：结果正确
    // int cameraResult[MAX_LAYER][MAX_VIEW][MAX_VIEW_UNIT];
    int vavleResult[MAX_LAYER][MAX_VAVLE][MAX_VAVLE_BOARD ];
    int ctrlResult[MAX_CTRL_BOARD_PER_INTERFACE];
    int lampResult[MAX_LAMP_BOARD_PER_INTERFACE];

    int reExamine = 0, err = 0;

    struGsh.nVersionCaptureIsRun = 1;
    struGsh.bInterModeSwitchErr = 0;
    struGsh.bFpgaModeAutoSwitchErr = 0;
    struGsh.bControlModeSwitchErr = 0;
    struGsh.bLampAutoSwitchErr = 0;
    struGsh.bValveAutoSwitchErr = 0;
    struGsh.nomalWipeCount = 0;
    /* 每次自检重新初始化各变量状态 */
    for(i = 0; i <MAX_LAYER; i++)
    {
        /* 初始化相机板 */
        for(j = 0; j < MAX_VIEW; j++)
        {
            for(m = 0; m < struCnfg.struLayerInfo[i].nViewUnitTotal[j]; m++)
            {
                for(n = 0; n <5; n++)
                {
                    struGsh.struVer.sUnit[i][j][m][n] = 0;
                    struGsh.struVer.sUnitIpc[i][j][m][n] = 0;
                }
            }
        }

        /* 初始化喷阀板 */
        for(j = 0; j < struCnfg.struLayerInfo[i].nVavleChainTotal; j++)
        {
            for(m = 0; m < struCnfg.struLayerInfo[i].nVavleBoardTotal[j]; m++)
            {
                for(n = 0; n <5; n++)
                {
                    struGsh.struVer.sVavle[i][j][m][n] = 0;
                }
            }
        }
    }

    for(i = 0; i <MAX_LAYER; i++)
    {
        for(j = 0; j <MAX_VIEW; j++)
        {
            for(m = 0; m < MAX_VIEW_UNIT; m++)
            {
                struGsh.struVer.nUnitRes[i][j][m] = -1;
                struCnfg.struVer.nUnitRes[i][j][m] = -1;
                struGsh.struVer.nUnitFalutCodeRes[i][j][m] = 0;
            }
        }
        for(j = 0; j <MAX_VAVLE; j++)
        {
            for(m = 0; m < MAX_VAVLE_BOARD; m++)
            {
                struGsh.struVer.nVavleRes[i][j][m] = -1;
            }
        }
    }

    for (i = 0; i < MAX_LAYER; i++)
    {

        for (j = 0; j < MAX_VIEW; j++)
        {
            for(m = 0; m < MAX_VIEW_UNIT; m++)
            {
                struGsh.cameraResult[i][j][m] = -1;
            }
            for(m = 0; m < MAX_VAVLE_BOARD; m++)
            {
                vavleResult[i][j][m] = -1;
            }
        }
    }

    /* 初始化接口 */
    struGsh.struVer.nInterfaceRes = -1;
    for(i = 0; i <5 ; i++)
    {
        struGsh.struVer.sInterface[i] = 0;
    }
    intResult = -1;

    /* 初始化灯光板 */
    for(i = 0; i < struCnfg.stuLayerLampCfg.nLampBoardTotal; i++)
    {
        struGsh.struVer.nLampRes[i] = -1;
        for(m = 0; m < 5; m++)
        {
            struGsh.struVer.sLampBoard[i][m] = 0;
        }
    }
    for (i = 0; i < MAX_LAMP_BOARD_PER_INTERFACE; i++)
    {
        lampResult[i] = -1;
    }

    /* 初始化控制板 */
    for(i = 0; i < struCnfg.nCtrlBoardTotal; i++)
    {
        struGsh.struVer.nCtrlRes[i] = -1;
        for(m = 0; m < 5; m++)
        {
            struGsh.struVer.sCtrlBoard[i][m] = 0;
        }
    }
    for (i = 0;  i < MAX_CTRL_BOARD_PER_INTERFACE; i++)
    {
        ctrlResult[i] = -1;
    }

    /* 初始化智能控制板 */
    struGsh.struVer.nAutoCtrlRes = -1;
    for(m = 0; m < 5; m++)
    {
        struGsh.struVer.sAutoCtrlBoard[m] = 0;
    }
    int autoCtrlResult = -1;

    for(i = 0; i < MAX_ACCELERATION_BOARD; i++)
    {
        struGsh.struVer.nAccelerationRes[i] = -1;
        for(m = 0; m < 5; m++)
        {
            struGsh.struVer.sAccelerationBoard[i][m] = 0;
        }
    }

    memset(data, 0, SEND_PACKET_DATA_SIZE);
    memset(data_autoBoard, 0, SEND_PACKET_RECTRL_BOARD_SIZE);
update:
    reExamine = 0;

    g_Runtime().getFreeSpace();
    resetMemoryDetection();

    /* 接口板版本 */
    for(i = 0; i < 3 ; i++)
    {
        mySerial.pushCom1CmdData(CMD_ALL_VERSION_REQUIRE, 0, BOARD_TYPE_INTERFACE, BOARD_TYPE_ALL_IN_CHAIN, data, 1);
        for(int tt = 0; tt < 10; tt++)
        {
            msleep(waitTime);
            if(struGsh.struVer.nInterfaceRes == 1)
            {
                break;
            }
        }
        if(struGsh.struVer.nInterfaceRes == 1)  // 接口板查询到
        {
            if ((struGsh.struVer.sInterface[2]) != struCnfg.nInterfaceBoardMode)
            {
                intResult = 0;
                err = 1;
                struGsh.bInterModeSwitchErr = 1;
            }
            else
            {
                if((paramDelayCode30Day.product30DayUseStartEnable == 0xFF)&&(struGsh.struDccryptData.bDccryptGetError != ENCRYPT_FPGA_BURNING))
                {
                    paramDelayCode30Day.product30DayUseStartEnable = 0xAA;
                    paramDelayCode30Day.product30DayUseOver = 0xFF;
                    paramDelayCode30Day.code_size_30day = 12;

                    memcpy(paramDelayCode30Day.product30DayUseStartDay, QDateTime::currentDateTime().toString("yyyyMMdd").toLatin1().data(), 8);
                    QString tmpTime = QDateTime::currentDateTime().toString("yyyyMMddhhmmss").right(12);
                    memcpy(paramDelayCode30Day.product30DayUseOnlyIdEeprom, tmpTime.toLatin1().data(), 12);

                    paramDelayCode30Day.write30DayInfoToEeprom();

                    paramDelayCode30Day.fpagCodeErrCount = 0;
                    paramDelayCode30Day.fpgaCodeErrCountWrite();

                    paramDelayCode30Day.product30DayUseAccHour = 0;
                    paramDelayCode30Day.product30DayUseAccHourWrite();

                    sleep(1);
                }
                intResult = 1;
            }
            break;
        }
        else
        {
            if(i == 2)
            {
                err = 1;    //三次没有查到，认为出错
                logger()->info("interface link down");

                g_MainManager().SendCurWidgetMsg(MSG_VERSION);
                struGsh.nVersionCaptureIsRun = 0;
                return err;
            }
        }
    }

    /* 相机板版本 */
    for(i = 0; i < struCnfg.nLayerTotal ; i++)
    {
        for(j = 0; j < MAX_VIEW; j++)
        {
            if(struCnfg.struLayerInfo[i].nViewIsUsed[j]){
                board = struCnfg.struLayerInfo[i].nViewBoardType[j];
                for(m = 0; m < struCnfg.struLayerInfo[i].nViewUnitTotal[j]; m++)
                {
                    nUnitAddr = struCnfg.struLayerInfo[i].stuViewInfo[j].nViewUnitId[m];
                    for(k = 0; k < 3; k++)
                    {
                        mySerial.pushCom1CmdData(CMD_ALL_VERSION_REQUIRE, 0, board, nUnitAddr, data, 1);

                        for(int tt = 0; tt < 10; tt++)
                        {
                            msleep(waitTime);
                            if(struGsh.struVer.nUnitRes[i][j][m] == 1)
                            {
                                break;
                            }
                        }
                        if(struGsh.struVer.nUnitRes[i][j][m] == 1)  // 当前视链查询到版本
                        {
                            if (struGsh.struVer.sUnit[i][j][m][2] == FACTORY_MODE)
                            {
                                struGsh.cameraResult[i][j][m] = 0;
                                // err = 1;
                                struGsh.bFpgaModeAutoSwitchErr = 1;
                                break;
                            }
                            else if(struGsh.struVer.sUnit[i][j][m][2] != struCnfp.nCameraBoardUserMode[i][j][m])
                            {
                                struGsh.cameraResult[i][j][m] = 1;
                                //err = 1;
                                struGsh.bFpgaModeAutoSwitchErr = 1;
                                break;
                            }
                            else
                            {
                                struGsh.cameraResult[i][j][m] = 2;
                            }

                            if ((struCnfg.nDPMachine == 1)&&(struCnfp.nIsCameraConpress == 0) &&( struCnfp.nViewSensorType[0][j] != SENSOR_G2)
                                    &&( struCnfp.nViewSensorType[0][j] != SENSOR_G3))//深度学习机器(低配除外)还需要check驱动板
                            {
                                //                                if (struCnfg.struIpcCameraParams[i][j][m].ipcType != IPC_CAMERA_TYPE_NULL)
                                //                                {
                                //                                    if (struGsh.struVer.sUnitIpc[i][j][m][2] == FACTORY_MODE)
                                //                                    {
                                //                                        struGsh.cameraResult[i][j][m] = 0;
                                //                                        //err = 1;
                                //                                        struGsh.bFpgaModeAutoSwitchErr = 1;
                                //                                    }
                                //                                    else if(struGsh.struVer.sUnitIpc[i][j][m][2] != struCnfp.nCameraBoardUserMode[i][j][m])
                                //                                    {
                                //                                        struGsh.cameraResult[i][j][m] = 1;
                                //                                        //err = 1;
                                //                                        struGsh.bFpgaModeAutoSwitchErr = 1;
                                //                                    }
                                //                                    else
                                //                                    {
                                //                                        struGsh.cameraResult[i][j][m] = 2;
                                //                                    }
                                //                                }
                            }
                            break;
                        }
                        else
                        {
                            if(k == 2)
                            {
                                err = 1;    //三次没有查到，认为出错
                                struGsh.cameraResult[i][j][m] = -1;
                                //                                resetMemoryDetection();
                                //                                g_MainManager().SendCurWidgetMsg(MSG_VERSION);
                                //                                struGsh.nVersionCaptureIsRun = 0;
                                //                                return err;
                            }
                        }
                    }
                }
            }
        }
    }

    /* 喷阀板版本 */
    for(i = 0; i < struCnfg.nLayerTotal ; i++)
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nVavleChainTotal; j++)
        {
            board = struCnfg.struLayerInfo[i].nVavleBoardType[j];
            for(m = 0; m < struCnfg.struLayerInfo[i].nVavleBoardTotal[j]; m++)
            {
                nUnitAddr = struCnfg.struLayerInfo[i].stuVavleInfo[j].nVavleBoardId[m];
                for(k = 0; k < 3; k++)
                {
                    mySerial.pushCom1CmdData(CMD_ALL_VERSION_REQUIRE, 0, board, nUnitAddr, data, 1);
                    for(int tt = 0; tt < 10; tt++)
                    {
                        msleep(waitTime);
                        if(struGsh.struVer.nVavleRes[i][j][m] == 1)
                        {
                            break;
                        }
                    }
                    if(struGsh.struVer.nVavleRes[i][j][m] == 1)  // 当前视链查询到版本
                    {
                        if ((struGsh.struVer.sVavle[i][j][m][2]) != struCnfg.nVavleBoardMode[i][j])
                        {
                            vavleResult[i][j][m] = 0;
                            err = 1;
                            struGsh.bValveAutoSwitchErr = 1;
                        }
                        else
                        {
                            vavleResult[i][j][m] = 1;
                        }

                        break;
                    }
                    else
                    {
                        if(k == 2)
                        {
                            err = 1;    //三次没有查到，认为出错
                        }
                    }
                }
            }
        }
    }

    /* 远程控制板版本 */

    if(struCnfg.bautoplcctrlEnable)
    {
        board = struCnfg.nCtrlBoardType;

        for(k = 0; k < 3; k++)
        {
            mySerial.pushCom2CmdData(CMD_CTRL_BOARD_VERSION_REQUEST, 0, board, 0x0e, data_autoBoard, 1);
            for(int tt = 0; tt < 10; tt++)
            {
                msleep(waitTime);
                if(struGsh.struVer.nAutoCtrlRes == 1)
                {
                    break;
                }
            }

            if(struGsh.struVer.nAutoCtrlRes == 1)  // 当前视链查询到版本
            {
                if ((struGsh.struVer.sAutoCtrlBoard[2]) != struCnfg.nCtrlBoardMode)
                {
                    autoCtrlResult = 0;
                    err = 1;
                }
                else
                {
                    autoCtrlResult = 1;
                }

                break;
            }
            else
            {
                if(k == 2)
                {
                    err = 1;    //三次没有查到，认为出错
                }
            }
        }
    }
    /* 灯光板版本 */
    for(m = 0; m < struCnfg.stuLayerLampCfg.nLampBoardTotal; m++)
    {
        board = struCnfg.nLampBoardType;
        int i = m;
        if(struCnfg.nAllInOne){
            i = m+1;
        }
        for(k = 0; k < 3; k++)
        {
            mySerial.pushCom1CmdData(CMD_ALL_VERSION_REQUIRE, 0, board, i, data, 1);
            for(int tt = 0; tt < 10; tt++)
            {
                msleep(waitTime);
                if(struGsh.struVer.nLampRes[i] == 1)
                {
                    break;
                }
            }
            if(struGsh.struVer.nLampRes[i] == 1)  // 当前视链查询到版本
            {
                if ((struGsh.struVer.sLampBoard[i][2]) != struCnfg.nLampBoardMode)
                {
                    lampResult[i] = 0;
                    err = 1;
                    struGsh.bLampAutoSwitchErr = 1;
                }
                else
                {
                    lampResult[i] = 1;
                }
                break;
            }
            else
            {
                if(k == 2)
                {
                    err = 1;    //三次没有查到，认为出错
                }
            }
        }
    }

    if(!struCnfg.nAllInOne)
    {
        /* 控制板版本 */
        for(i = 0; i < struCnfg.nCtrlBoardTotal; i++)
        {
            board = struCnfg.nCtrlBoardType;

            for(k = 0; k < 3; k++)
            {
                mySerial.pushCom1CmdData(CMD_ALL_VERSION_REQUIRE, 0, board, i, data, 1);
                for(int tt = 0; tt < 10; tt++)
                {
                    msleep(waitTime);
                    if(struGsh.struVer.nCtrlRes[i] == 1)
                    {
                        break;
                    }
                }
                if(struGsh.struVer.nCtrlRes[i] == 1)  // 当前视链查询到版本
                {
                    if ((struGsh.struVer.sCtrlBoard[i][2]) != struCnfg.nCtrlBoardMode)
                    {
                        ctrlResult[i] = 0;
                        err = 1;
                        struGsh.bControlModeSwitchErr = 1;
                    }
                    else
                    {
                        ctrlResult[i] = 1;
                    }

                    break;
                }
                else
                {
                    if(k == 2)
                    {
                        err = 1;    //三次没有查到，认为出错
                    }
                }
            }
        }
        mySerial.pushCom1CmdData(CMD_CTRL_VI_DETECT_BOARD_VERSION_QUERY, 0, board, 0, data, 1);
        msleep(100);
    }

    resetMemoryDetection();
    g_MainManager().SendCurWidgetMsg(MSG_VERSION);

    struGsh.nVersionCaptureIsRun = 0;
    return err;
}

void GlobalFlow::resetMemoryDetection()
{
#if 0
    QFile myfile;
    if(myfile.exists("/dev/mmcblk1")) //sd存在
    {
        if (myFlow.saveMemoryDetectedFileStaus("/media/mmcblk0p1"))
            struGsh.struMemoryStatus.nSDStatus[0] = 1;
        else
            struGsh.struMemoryStatus.nSDStatus[0] = 0;

        if(myfile.exists("/sdcard/3-app.bin")) //从sd卡启动
        {
            struGsh.struMemoryStatus.nCurRunLevel = 0;

            if(myFlow.saveMemoryDetectedFileStaus("/app"))
                struGsh.struMemoryStatus.nSDStatus[1] = 1;
            else
                struGsh.struMemoryStatus.nSDStatus[1] = 0;
        }
        else    //从emmc启动
        {
            struGsh.struMemoryStatus.nCurRunLevel = 1;

            if (myFlow.saveMemoryDetectedFileStaus("/media/mmcblk1p1"))
                struGsh.struMemoryStatus.nEMMCStatus[0] = 1;
            else
                struGsh.struMemoryStatus.nEMMCStatus[0] = 0;

            if(myFlow.saveMemoryDetectedFileStaus("/app"))
                struGsh.struMemoryStatus.nEMMCStatus[1] = 1;
            else
                struGsh.struMemoryStatus.nEMMCStatus[1] = 0;

            if(myFlow.saveMemoryDetectedFileStaus("/sdcard"))
                struGsh.struMemoryStatus.nEMMCStatus[2] = 1;
            else
                struGsh.struMemoryStatus.nEMMCStatus[2] = 0;

        }
    }
    else //sd卡不存在
    {
        struGsh.struMemoryStatus.nCurRunLevel = 1;
        struGsh.struMemoryStatus.nSDStatus[0] = 0;
        struGsh.struMemoryStatus.nSDStatus[1] = 0;

        if (myFlow.saveMemoryDetectedFileStaus("/media/mmcblk0p1"))
            struGsh.struMemoryStatus.nEMMCStatus[0] = 1;
        else
            struGsh.struMemoryStatus.nEMMCStatus[0] = 0;

        if(myFlow.saveMemoryDetectedFileStaus("/app"))
            struGsh.struMemoryStatus.nEMMCStatus[1] = 1;
        else
            struGsh.struMemoryStatus.nEMMCStatus[1] = 0;

        if(myFlow.saveMemoryDetectedFileStaus("/sdcard"))
            struGsh.struMemoryStatus.nEMMCStatus[2] = 1;
        else
            struGsh.struMemoryStatus.nEMMCStatus[2] = 0;

    }
#endif

#ifdef Q_OS_UNIX
    memset(&myfs.struMemoryStatus,0,sizeof(myfs.struMemoryStatus));
    QFile myfile;
    if(myfile.exists("/dev/mmcblk1")) //sd存在
    {
        myfs.struMemoryStatus.nCurRunLevel[1] = 1;
        QString cnfArea = myfs.getMountDevice(0,"/sdcard").trimmed();

        if(cnfArea == "/dev/mmcblk0p1")//卡启动
        {
            myfs.struMemoryStatus.nCurRunLevel[0] = 1;
            myfs.struMemoryStatus.nSDMountStatus[0] = myfs.getMountStaus("/media/mmcblk0p1");
            myfs.struMemoryStatus.nSDUsedStatus[0] = myfs.getUsedStaus("/media/mmcblk0p1");
            myfs.struMemoryStatus.nSDMountStatus[1] = myfs.getMountStaus("/app");
            myfs.struMemoryStatus.nSDUsedStatus[1] = myfs.getUsedStaus("/app");

            if(myfile.exists("/dev/mmcblk1boot0")&& myfile.exists("/dev/mmcblk1boot1"))//emmc hwok
            {
                myfs.struMemoryStatus.nCurRunLevel[2] = 1;
                myfs.struMemoryStatus.nEMMCMountStatus[0] = myfs.getMountStaus("/media/mmcblk1p1");
                myfs.struMemoryStatus.nEMMCUsedStatus[0] = myfs.getUsedStaus("/media/mmcblk1p1");
                myfs.struMemoryStatus.nEMMCMountStatus[2] = myfs.getMountStaus("/media/mmcblk1p2");
                myfs.struMemoryStatus.nEMMCUsedStatus[2] = myfs.getUsedStaus("/media/mmcblk1p2");
            }
        }
        if(cnfArea == "/dev/mmcblk1p2")//emmc启动
        {
            myfs.struMemoryStatus.nCurRunLevel[0] = 2;
            myfs.struMemoryStatus.nCurRunLevel[2] = 1;
            myfs.struMemoryStatus.nSDMountStatus[0] = myfs.getMountStaus("/media/mmcblk0p1");
            myfs.struMemoryStatus.nSDUsedStatus[0] = myfs.getUsedStaus("/media/mmcblk0p1");
            myfs.struMemoryStatus.nEMMCMountStatus[0] = myfs.getMountStaus("/media/mmcblk1p1");
            myfs.struMemoryStatus.nEMMCUsedStatus[0] = myfs.getUsedStaus("/media/mmcblk1p1");
            myfs.struMemoryStatus.nEMMCMountStatus[1] = myfs.getMountStaus("/app");
            myfs.struMemoryStatus.nEMMCUsedStatus[1] = myfs.getUsedStaus("/app");
            myfs.struMemoryStatus.nEMMCMountStatus[2] = myfs.getMountStaus("/media/mmcblk1p2");
            myfs.struMemoryStatus.nEMMCUsedStatus[2] = myfs.getUsedStaus("/media/mmcblk1p2");
        }
    }
    else //sd卡不存在
    {
        myfs.struMemoryStatus.nCurRunLevel[1] = 0;
        myfs.struMemoryStatus.nCurRunLevel[0] = 2;
        myfs.struMemoryStatus.nCurRunLevel[2] = 1;
        myfs.struMemoryStatus.nEMMCMountStatus[0] = myfs.getMountStaus("/media/mmcblk0p1");
        myfs.struMemoryStatus.nEMMCUsedStatus[0] = myfs.getUsedStaus("/media/mmcblk0p1");
        myfs.struMemoryStatus.nEMMCMountStatus[1] = myfs.getMountStaus("/app");
        myfs.struMemoryStatus.nEMMCUsedStatus[1] = myfs.getUsedStaus("/app");
        myfs.struMemoryStatus.nEMMCMountStatus[2] = myfs.getMountStaus("/media/mmcblk0p2");
        myfs.struMemoryStatus.nEMMCUsedStatus[2] = myfs.getUsedStaus("/media/mmcblk0p2");
    }
    QString runstat,sdStat,emmcStat,svnstr;
    runstat = QString("%1%2%3").arg(myfs.struMemoryStatus.nCurRunLevel[0])
            .arg(myfs.struMemoryStatus.nCurRunLevel[1]).arg(myfs.struMemoryStatus.nCurRunLevel[2]);
    sdStat = QString("%1%2'%3%4").arg(myfs.struMemoryStatus.nSDMountStatus[0])
            .arg(myfs.struMemoryStatus.nSDMountStatus[1])
            .arg(myfs.struMemoryStatus.nSDUsedStatus[0])
            .arg(myfs.struMemoryStatus.nSDUsedStatus[1]);
    emmcStat = QString("%1%2%3'%4%5%6").arg(myfs.struMemoryStatus.nEMMCMountStatus[0])
            .arg(myfs.struMemoryStatus.nEMMCMountStatus[1])
            .arg(myfs.struMemoryStatus.nEMMCMountStatus[2])
            .arg(myfs.struMemoryStatus.nEMMCUsedStatus[0])
            .arg(myfs.struMemoryStatus.nEMMCUsedStatus[1])
            .arg(myfs.struMemoryStatus.nEMMCUsedStatus[2]);
    svnstr = QString("%1-%2-%3").arg(runstat).arg(sdStat).arg(emmcStat);
    logger()->info("fscheck:%1",svnstr);
#endif

}

void GlobalFlow::sendAll()
{
    g_infoWidget().setLabelText(g_myLan().msg_applying);
    g_infoWidget().delayShow();

    int access = struGsh.bRunMode;
    int tAlarmFeedStaus[CTRL_ALARM_MAX_FEED_SENSOR];
    int tAlarmFeedLevel[CTRL_ALARM_MAX_FEED_SENSOR];
    memcpy(tAlarmFeedStaus,struGsh.bAlarmFeedStaus,sizeof(tAlarmFeedStaus));
    memcpy(tAlarmFeedLevel,struGsh.struMachineStatus[0].bAlarmFeedLevel,sizeof(tAlarmFeedLevel));

    stru_version m_struVer;                   // 版本信息
    memcpy(&m_struVer,&struGsh.struVer,sizeof(stru_version));

    logger()->info("000");
    initGsh();                  // 获取全局共享参数，初始化串口等设备
    struGsh.bRunMode = access;
    memcpy(struGsh.bAlarmFeedStaus,tAlarmFeedStaus,sizeof(tAlarmFeedStaus));
    memcpy(struGsh.struMachineStatus[0].bAlarmFeedLevel,tAlarmFeedLevel,sizeof(tAlarmFeedLevel));
    memcpy(&struGsh.struVer,&m_struVer,sizeof(stru_version));

    struGsh.bUpdateState = 1;

    logger()->info("111");
    resetMachineConfigRG();       //初始化机型配置

    logger()->info("222");
    /* 重置所有参数 */
    initSendAllParams();

    logger()->info("333");
    sleep(8);
    struGsh.bUpdateState = 0;
    g_infoWidget().hide();

    logger()->info("444");
}

#if 0
/* 根据剔除方向设置字符串的显示 */
void GlobalFlow::setStringByDirect()
{
    switch(struCnfg.nTickDirect) {
    case 0:     // 1->2->3
        struGsh.first = g_myLan().ids_one;
        struGsh.second = g_myLan().ids_two;
        struGsh.third = g_myLan().ids_three;
        break;
    case 1:     // 2->1->3
        struGsh.first = g_myLan().ids_two;
        struGsh.second = g_myLan().ids_one;
        struGsh.third = g_myLan().ids_three;
        break;
    case 2:     // 3->2->1
        struGsh.first = g_myLan().ids_three;
        struGsh.second = g_myLan().ids_two;
        struGsh.third = g_myLan().ids_one;
        break;
    default:
        break;
    }
}
#endif

void GlobalFlow::resetNetWaveStart()
{
    /* 通用平台不需要重新发送读波形 */
    //    if((struGsh.nPageSm == SM_BACKGROUND_SET_WIDGET)
    //            ||(struGsh.nPageSm == SM_CAMERA_SIGNAL_WIDGET)
    //            ||struGsh.nPageSm==SM_DIVIDE_PIXEL_WIDGET)
    //    {
    //        myFlow.myNetWork->serialSendTransStartStop(1);
    //    }
}

/* 判断原始图像像素点是否为背景点 */
bool GlobalFlow::imagePixelIsBackground(uchar r, uchar g, uchar b, uchar n, int nLevel, int nViewId,int tdmode)
{
    int ave = 0;
    float h_new=0.0;
    switch(struCnfp.nBackgroundColor[nLevel][nViewId][tdmode]){
    case BACKGROUND_COLOR_RED: //红
        //        if((r>=struCnfp.nBackgroundColorValueLow[nLevel][nViewId])&&(r<=struCnfp.nBackgroundColorValueUp[nLevel][nViewId])
        //                &&(100.0*r/(r+g+b)>=struCnfp.nBackgroundColorProLow[nLevel][nViewId])&&(100.0*r/(r+g+b)<struCnfp.nBackgroundColorProUp[nLevel][nViewId]))
        if((r>=struCnfp.nBackgroundColorValueLow[nLevel][nViewId][tdmode])
                &&(100.0*r/(r+g+b)>=struCnfp.nBackgroundColorProLow[nLevel][nViewId][tdmode]))
            return true;
        break;
    case BACKGROUND_COLOR_GREEN: //绿
        //        if((g>=struCnfp.nBackgroundColorValueLow[nLevel][nViewId])&&(g<=struCnfp.nBackgroundColorValueUp[nLevel][nViewId])
        //                &&(100.0*g/(r+g+b)>=struCnfp.nBackgroundColorProLow[nLevel][nViewId])&&(100.0*g/(r+g+b)<struCnfp.nBackgroundColorProUp[nLevel][nViewId]))
        if((g>=struCnfp.nBackgroundColorValueLow[nLevel][nViewId][tdmode])
                &&(100.0*g/(r+g+b)>=struCnfp.nBackgroundColorProLow[nLevel][nViewId][tdmode]))
            return true;
        break;
    case BACKGROUND_COLOR_BLUE: //蓝
        //        if((b>=struCnfp.nBackgroundColorValueLow[nLevel][nViewId])&&(b<=struCnfp.nBackgroundColorValueUp[nLevel][nViewId])
        //                &&(100.0*b/(r+g+b)>=struCnfp.nBackgroundColorProLow[nLevel][nViewId])&&(100.0*b/(r+g+b)<struCnfp.nBackgroundColorProUp[nLevel][nViewId]))
        if((b>=struCnfp.nBackgroundColorValueLow[nLevel][nViewId][tdmode])
                &&(100.0*b/(r+g+b)>=struCnfp.nBackgroundColorProLow[nLevel][nViewId][tdmode]))
            return true;
        break;
    case BACKGROUND_COLOR_NIR: //红外N
        if((n>=struCnfp.nBackgroundColorValueLow[nLevel][nViewId][tdmode])&&(n<struCnfp.nBackgroundColorValueUp[nLevel][nViewId][tdmode]))
            return true;
        break;
    case BACKGROUND_COLOR_BLACK_WHITE: //黑白
        if((r>=struCnfp.nBackgroundColorValueLow[nLevel][nViewId][tdmode])&&(r<=struCnfp.nBackgroundColorValueUp[nLevel][nViewId][tdmode])
                && (g>=struCnfp.nBackgroundColorValueLow[nLevel][nViewId][tdmode])&&(g<=struCnfp.nBackgroundColorValueUp[nLevel][nViewId][tdmode])
                && (b>=struCnfp.nBackgroundColorValueLow[nLevel][nViewId][tdmode])&&(b<=struCnfp.nBackgroundColorValueUp[nLevel][nViewId][tdmode]))
            return true;
        //        if((r+g+b>=struCnfp.nBackgroundColorValueLow[nLevel][nViewId]*2.55)&&(r+g+b<struCnfp.nBackgroundColorValueUp[nLevel][nViewId]*2.55))
        //            return true;
        break;
    case BACKGROUND_COLOR_GREY: //灰度
        ave = (r+g+b)/3;
        if((ave>=struCnfp.nBackgroundColorValueLow[nLevel][nViewId][tdmode])&&(ave<struCnfp.nBackgroundColorValueUp[nLevel][nViewId][tdmode]))
            return true;
        break;
    case BACKGROUND_COLOR_YELLOW: //黄  r>上限， g>上限, b<下限
        //        if((r>=struCnfp.nBackgroundColorValueUp[nLevel][nViewId])
        //                && (g>=struCnfp.nBackgroundColorValueUp[nLevel][nViewId])
        //                && (b<struCnfp.nBackgroundColorValueLow[nLevel][nViewId]))
        //            return true;
        getHsv(r, g, b, h, s, v);
        h_new = h/10.0;
        if((h_new>=struCnfp.nBackgroundColorValueLow[nLevel][nViewId][tdmode])&&(h_new<=struCnfp.nBackgroundColorValueUp[nLevel][nViewId][tdmode])&&
                (v>=struCnfp.nBackgroundColorValueVLow[nLevel][nViewId][tdmode])&&(v<=struCnfp.nBackgroundColorValueVUp[nLevel][nViewId][tdmode]))
        {
            return true;
        }
    default:
        break;
    }
    return false;
}


bool GlobalFlow::imagePixelIsBad(uchar r, uchar g, uchar b, uchar n, int nLevel, int nViewId, int nGroup,int nUnit, int nArith)
{
    int mode, main, vice, index;
    int nMin, nMax;
    int nValue;

    int resGreyMode, resGreyColor, resGreyMin, resGreyMax, resGreyValue;
    int resColorMode, resColorMain, resColorVice, resColorMin, resColorMax, resColorValue;
    //    bool resGreyGood = false, resColorGood = false;
    static int nr,ng,nb,nrg,ngb,nrb,nrr,ngg,nbb,ncon;

    int nColorArithIsBad = 0, nResArithIsBad=0;   //用于灰度算法下黑白颜色好坏判断
    int tmpDiffColor,dim;

    switch (nArith) {
    case ARITH_GREY_A:
    case ARITH_GREY_B:
    case ARITH_GREY_C:
    case ARITH_GREY_D:
        nColorArithIsBad = 0;
        index = nArith-ARITH_GREY_A;
        main = struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struGreyColor[index].nMain;
        mode = struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struGreyColor[index].nMode;

        if(mode == 0)
        {
            nMin = 0;
            nMax = struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struGreyColor[index].nSensLow+struCnfp.struUnitSens[nLevel][nViewId][nUnit][0].greyColor[nArith];//暂不支持相机分割
        }
        else
        {
            nMin = ARITH_GREY_MAX_SENS-(struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struGreyColor[index].nSensUp+struCnfp.struUnitSens[nLevel][nViewId][nUnit][0].greyColor[nArith]);
            nMax = ARITH_GREY_MAX_SENS;
        }
        switch(main)
        {
        case BACKGROUND_COLOR_RED:
            nValue = r;
            break;
        case BACKGROUND_COLOR_GREEN:
            nValue = g;
            break;
        case BACKGROUND_COLOR_BLUE:
            nValue = b;
            break;
        case BACKGROUND_COLOR_NIR:
            nValue = n;
            break;
        case BACKGROUND_COLOR_BLACK_WHITE:
            if((r >= nMin)&&(r <= nMax)&&(g >= nMin)&&(g <= nMax)&&(b >= nMin)&&(b <= nMax)&&(nMin != nMax))
            {
                nColorArithIsBad = 1;
            }
            break;
        case BACKGROUND_COLOR_GREY:
            nValue = (r+g+b)/3;
            break;
        }
        break;
    case ARITH_DISCOLOR_A:
    case ARITH_DISCOLOR_B:
    case ARITH_CROSS:
        index = nArith-ARITH_GREY_A;
        /******亮度下限判断******/
        dim = struCnfp.nSensorDim[nLevel][nViewId];
        switch(dim){
        case 1:
            if(r < struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struGreyColor[index].lightLimit)
                return false;
            break;
        case 2:
            if((r+g) < struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struGreyColor[index].lightLimit*dim)
                return false;
            break;
        case 3:
            if((r+g+b) < struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struGreyColor[index].lightLimit*dim)
                return false;
            break;
        case 4:
            if((r+g+b+n) < struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struGreyColor[index].lightLimit*dim)
                return false;
            break;
        default:
            break;
        }
        mode = struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struGreyColor[index].nMode;

        tmpDiffColor = struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struGreyColor[index].nDiffColor;

        if(mode == 0)
        {
            nMin = 0;
            nMax = struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struGreyColor[index].nSensLow+struCnfp.struUnitSens[nLevel][nViewId][nUnit][0].greyColor[nArith];
        }
        else
        {
            nMin = ARITH_COLOR_MAX_SENS-(struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struGreyColor[index].nSensUp+struCnfp.struUnitSens[nLevel][nViewId][nUnit][0].greyColor[nArith]);
            nMax = ARITH_COLOR_MAX_SENS;
        }

        switch(tmpDiffColor)
        {
        case 0:
            if (r == 0) nValue = 0;
            else nValue = ARITH_COLOR_MAX_SENS*r/(r+g);
            break;
        case 1:
            if (r == 0) nValue = 0;
            else nValue = ARITH_COLOR_MAX_SENS*r/(r+b);
            break;
        case 2:
            if (g == 0) nValue = 0;
            else nValue = ARITH_COLOR_MAX_SENS*g/(g+b);
            break;
        case 3:
            if (r == 0) nValue = 0;
            else nValue = ARITH_COLOR_MAX_SENS*r/(r+n);
            break;
        case 4:
            if (g == 0) nValue = 0;
            else nValue = ARITH_COLOR_MAX_SENS*g/(g+n);
            break;
        case 5:
            if (b == 0) nValue = 0;
            else nValue = ARITH_COLOR_MAX_SENS*b/(b+n);
            break;
        }
        break;
    case ARITH_SVM_A:
    case ARITH_SVM_B:
    case ARITH_SVM_C:
    case ARITH_SVM_D:
    {
        int svmIndex = nArith-ARITH_SVM_A;
        nr = struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struSvm[svmIndex].nQuoR[0] == 0?struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struSvm[svmIndex].nQuoR[1]:struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struSvm[svmIndex].nQuoR[1]*(-1);
        ng = struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struSvm[svmIndex].nQuoG[0] == 0?struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struSvm[svmIndex].nQuoG[1]:struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struSvm[svmIndex].nQuoG[1]*(-1);
        nb = struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struSvm[svmIndex].nQuoB[0] == 0?struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struSvm[svmIndex].nQuoB[1]:struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struSvm[svmIndex].nQuoB[1]*(-1);
        nrg = struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struSvm[svmIndex].nQuoRG[0] == 0?struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struSvm[svmIndex].nQuoRG[1]:struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struSvm[svmIndex].nQuoRG[1]*(-1);
        ngb = struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struSvm[svmIndex].nQuoGB[0] == 0?struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struSvm[svmIndex].nQuoGB[1]:struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struSvm[svmIndex].nQuoGB[1]*(-1);
        nrb = struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struSvm[svmIndex].nQuoRB[0] == 0?struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struSvm[svmIndex].nQuoRB[1]:struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struSvm[svmIndex].nQuoRB[1]*(-1);
        nrr = struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struSvm[svmIndex].nQuoRR[0] == 0?struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struSvm[svmIndex].nQuoRR[1]:struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struSvm[svmIndex].nQuoRR[1]*(-1);
        ngg = struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struSvm[svmIndex].nQuoGG[0] == 0?struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struSvm[svmIndex].nQuoGG[1]:struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struSvm[svmIndex].nQuoGG[1]*(-1);
        nbb = struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struSvm[svmIndex].nQuoBB[0] == 0?struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struSvm[svmIndex].nQuoBB[1]:struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struSvm[svmIndex].nQuoBB[1]*(-1);
        ncon = struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struSvm[svmIndex].nQuoCons[0] == 0?struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struSvm[svmIndex].nQuoCons[1]:struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struSvm[svmIndex].nQuoCons[1]*(-1);

        int nSignCons = struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struSvm[svmIndex].nQuoCons[0];
        int tmpSvmCons = ncon;
        int unitSens = struCnfp.struUnitSens[nLevel][nViewId][nGroup][0].svm[svmIndex];
        int svmsense = struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struSvm[svmIndex].nSens+unitSens;
        int nSvmCons;
        if(struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struSvm[svmIndex].nModeSvm == 1) { //反选时，变换符号位标志
            nSignCons = (nSignCons == 1) ? 0:1;
        }
        if(nSignCons == 1) {//综合正反选后，常数项为负数
            /* 灵敏度越大，常数项绝对值越大，选的越多 */
            if (svmsense >= 100) {
                nSvmCons = tmpSvmCons*(1+(svmsense-100)/100.0);
            }
            else {   // 灵敏度越小，常数项绝对值越小，选的越小
                nSvmCons = tmpSvmCons*(1-(100-svmsense)/100.0);
            }
        }
        else {
            /* 灵敏度越大，常数项越小，选的越多 */
            if (svmsense >= 100) {
                nSvmCons = tmpSvmCons*(1-(svmsense-100)/100.0);
            }
            else {   // 灵敏度越小，常数项越大，选的越少
                nSvmCons = tmpSvmCons*(1+(100-svmsense)/100.0);
            }
        }

        //智能模式３，需要对常数项放大
        if (struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struSvm[svmIndex].nModeType ==2){
            nSvmCons = struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struSvm[svmIndex].nConsExpandedValue*tmpSvmCons*((svmsense-100)/100.0);
        }

        //r,g,b任何一个小于亮度下限就不吹
        if(r < struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struSvm[svmIndex].brightLimit
                ||g < struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struSvm[svmIndex].brightLimit
                ||b < struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struSvm[svmIndex].brightLimit)
            return false;

        if(struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struSvm[svmIndex].nModeSvm == 0){//pos
            if((nr*r+ng*g+nb*b+nrg*r*g+ngb*g*b+nrb*r*b+nrr*r*r+ngg*g*g+nbb*b*b+nSvmCons) > 0){
                return false;
            }else{
                return true;
            }
        }else{
            if((nr*r+ng*g+nb*b+nrg*r*g+ngb*g*b+nrb*r*b+nrr*r*r+ngg*g*g+nbb*b*b+nSvmCons) < 0){
                return false;
            }else{
                return true;
            }
        }
    }
        break;
    case ARITH_HSV:
    case ARITH_HSV_B:
    case ARITH_HSV_C:
    {
        getHsv(r,g,b,h,s,v);
        int hsvIndex = nArith-ARITH_HSV;
        /* step 1. 灰度保留判断 */
        nResArithIsBad = 0;
        resGreyColor = struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struHsv[hsvIndex].struResGreyColor.nColor1;
        resGreyMin = struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struHsv[hsvIndex].struResGreyColor.nSensLow1;
        resGreyMax = struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struHsv[hsvIndex].struResGreyColor.nSensUp1;
        switch(resGreyColor)
        {
        case BACKGROUND_COLOR_RED:
            resGreyValue = r;
            break;
        case BACKGROUND_COLOR_GREEN:
            resGreyValue = g;
            break;
        case BACKGROUND_COLOR_BLUE:
            resGreyValue = b;
            break;
        case BACKGROUND_COLOR_NIR:
            resGreyValue = n;
            break;
        case BACKGROUND_COLOR_BLACK_WHITE:
            if((r > resGreyMin)&&(r < resGreyMax)&&(g > resGreyMin)&&(g < resGreyMax)&&(b > resGreyMin)&&(b < resGreyMax))
            {
                nResArithIsBad = 1;
            }
            break;
        case BACKGROUND_COLOR_GREY:
            resGreyValue = (r+g+b)/3;
            break;
        }

        /* step 2. 色差保留判断 */
        int tmpDiffColor =struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struHsv[hsvIndex].struResGreyColor.nResDiffColor;
        resColorMin = struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struHsv[hsvIndex].struResGreyColor.nSensLow2;
        resColorMax = struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struHsv[hsvIndex].struResGreyColor.nSensUp2;
        switch(tmpDiffColor)
        {
        case 0:
            if (r == 0) resColorValue = 0;
            else resColorValue = ARITH_COLOR_MAX_SENS*r/(r+g);
            break;
        case 1:
            if (r == 0) resColorValue = 0;
            else resColorValue= ARITH_COLOR_MAX_SENS*r/(r+b);
            break;
        case 2:
            if (g == 0) resColorValue = 0;
            else resColorValue= ARITH_COLOR_MAX_SENS*g/(g+b);
            break;
        case 3:
            if (r == 0) resColorValue = 0;
            else resColorValue= ARITH_COLOR_MAX_SENS*r/(r+n);
            break;
        case 4:
            if (g == 0) resColorValue = 0;
            else resColorValue= ARITH_COLOR_MAX_SENS*g/(g+n);
            break;
        case 5:
            if (b == 0) resColorValue = 0;
            else resColorValue= ARITH_COLOR_MAX_SENS*b/(b+n);
            break;
        }

        if(struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struHsv[hsvIndex].nHsvSensLowH < struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struHsv[hsvIndex].nHsvSensUpH)
        {
            if(h >= struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struHsv[hsvIndex].nHsvSensLowH && h <= struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struHsv[hsvIndex].nHsvSensUpH  \
                    &&s >= struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struHsv[hsvIndex].nHsvSensLowS &&s <= struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struHsv[hsvIndex].nHsvSensUpS \
                    &&v >= struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struHsv[hsvIndex].nHsvSensLowV &&v <= struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struHsv[hsvIndex].nHsvSensUpV)
            {
                if(((resGreyValue>resGreyMin)&&(resGreyValue<=resGreyMax)||(nResArithIsBad == 1))
                        &&(resColorValue > resColorMin)&&(resColorValue <= resColorMax))
                {
                    return false;
                }else
                {
                    return true;
                }
            }
            else
                return false;
        }else
        {
            if((h >= struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struHsv[hsvIndex].nHsvSensLowH || h <= struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struHsv[hsvIndex].nHsvSensUpH)  \
                    &&s >= struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struHsv[hsvIndex].nHsvSensLowS &&s <= struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struHsv[hsvIndex].nHsvSensUpS \
                    &&v >= struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struHsv[hsvIndex].nHsvSensLowV &&v <= struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struHsv[hsvIndex].nHsvSensUpV)
            {
                if(((resGreyValue>resGreyMin)&&(resGreyValue<=resGreyMax)||(nResArithIsBad == 1))
                        &&(resColorValue > resColorMin)&&(resColorValue <= resColorMax))
                {
                    return false;
                }else
                {
                    return true;
                }
            }
            else
                return false;
        }
        break;
    }
    case ARITH_SAT:
        // 2021/8/19 (s<=Up &&s>Low)&&(r+g+b>=light_L*2)
        getHsv(r, g, b, h, s, v);

        if (struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struColorSat.nMode == 1) {
            nMin = ARITH_SAT_MAX_SENSE-(struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struColorSat.nSensUp+struCnfp.struUnitSens[nLevel][nViewId][nUnit][0].sat);
            nMin = (nMin <= 0) ? 0 : nMin;
            nMax = ARITH_SAT_MAX_SENSE;
        }
        else {
            nMin = 0;
            nMax = struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struColorSat.nSensLow+struCnfp.struUnitSens[nLevel][nViewId][nUnit][0].sat;
            nMax = (nMax > ARITH_SAT_MAX_SENSE) ? ARITH_SAT_MAX_SENSE : nMax;
        }

        if(s <= nMax && s > nMin && (r+g+b >= 2* struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struColorSat.lightLimit ))
            return true;
        else
            return false;

        break;
    default:
        break;
    }

    if((nColorArithIsBad == 1)||((nValue >= nMin)&&(nValue <= nMax)))
    {
        /* step 1. 灰度保留判断 */
        nResArithIsBad = 0;

        resGreyColor = struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struGreyColor[index].struResGreyColor.nColor1;
        resGreyMin = struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struGreyColor[index].struResGreyColor.nSensLow1;
        resGreyMax = struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struGreyColor[index].struResGreyColor.nSensUp1;

        switch(resGreyColor)
        {
        case BACKGROUND_COLOR_RED:
            resGreyValue = r;
            break;
        case BACKGROUND_COLOR_GREEN:
            resGreyValue = g;
            break;
        case BACKGROUND_COLOR_BLUE:
            resGreyValue = b;
            break;
        case BACKGROUND_COLOR_NIR:
            resGreyValue = n;
            break;
        case BACKGROUND_COLOR_BLACK_WHITE:
            if((r >= resGreyMin)&&(r <= resGreyMax)&&(g >= resGreyMin)&&(g <= resGreyMax)&&(b >= resGreyMin)&&(b <= resGreyMax)&&(resGreyMin != resGreyMax))
            {
                nResArithIsBad = 1;
            }
            break;
        case BACKGROUND_COLOR_GREY:
            resGreyValue = (r+g+b)/3;
            break;
        }

        /* step 2. 色差保留判断 */
        resColorMode = struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struGreyColor[index].struResGreyColor.nMode2;

        int tmpDiffColor =struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struGreyColor[index].struResGreyColor.nResDiffColor;

        resColorMin = struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struGreyColor[index].struResGreyColor.nSensLow2;
        resColorMax = struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struGreyColor[index].struResGreyColor.nSensUp2;
        switch(tmpDiffColor)
        {
        case 0:
            if (r == 0) resColorValue = 0;
            else resColorValue = ARITH_COLOR_MAX_SENS*r/(r+g);
            break;
        case 1:
            if (r == 0) resColorValue = 0;
            else resColorValue= ARITH_COLOR_MAX_SENS*r/(r+b);
            break;
        case 2:
            if (g == 0) resColorValue = 0;
            else resColorValue= ARITH_COLOR_MAX_SENS*g/(g+b);
            break;
        case 3:
            if (r == 0) resColorValue = 0;
            else resColorValue= ARITH_COLOR_MAX_SENS*r/(r+n);
            break;
        case 4:
            if (g == 0) resColorValue = 0;
            else resColorValue= ARITH_COLOR_MAX_SENS*g/(g+n);
            break;
        case 5:
            if (b == 0) resColorValue = 0;
            else resColorValue= ARITH_COLOR_MAX_SENS*b/(b+n);
            break;
        }
        if(((resGreyValue>=resGreyMin)&&(resGreyValue<=resGreyMax)&&(resGreyMin !=resGreyMax)||(nResArithIsBad == 1))
                &&(resColorValue >= resColorMin)&&(resColorValue <= resColorMax)&&(resColorMin !=resColorMax))
        {
            return false;
        }

        return true;
    }
    return false;
}
#if 0
if((nColorArithIsBad == 1)||((nValue > nMin)&&(nValue <= nMax)))
{
    nResArithIsBad = 0;
    if(struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struGreyColor[index].nResType == 1)  //传统灰度色差保留
    {
        if(struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struGreyColor[index].struResGreyColor.nGreyResEnable == 1)   //灰度保留使能
        {
            resGreyMode = struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struGreyColor[index].struResGreyColor.nMode1;
            resGreyColor = struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struGreyColor[index].struResGreyColor.nColor1;

            if(resGreyMode == 0)
            {
                resGreyMin = 0;
                resGreyMax = struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struGreyColor[index].struResGreyColor.nSensLow1;
            }
            else
            {
                resGreyMin = ARITH_GREY_MAX_SENS-struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struGreyColor[index].struResGreyColor.nSensUp1;
                resGreyMax = ARITH_GREY_MAX_SENS;
            }

            switch(resGreyColor)
            {
            case BACKGROUND_COLOR_RED:
                resGreyValue = r;
                break;
            case BACKGROUND_COLOR_GREEN:
                resGreyValue = g;
                break;
            case BACKGROUND_COLOR_BLUE:
                resGreyValue = b;
                break;
            case BACKGROUND_COLOR_NIR:
                resGreyValue = n;
                break;
            case BACKGROUND_COLOR_BLACK_WHITE:
                if((r > resGreyMin)&&(g <= resGreyMax)&&(g > resGreyMin)&&(g <= resGreyMax)&&(b > resGreyMin)&&(b <= resGreyMax))
                {
                    nResArithIsBad = 1;
                }
                break;
            case BACKGROUND_COLOR_GREY:
                resGreyValue = (r+g+b)/3;
                break;
            }
            if((resGreyValue > resGreyMin)&&(resGreyValue <= resGreyMax)||(nResArithIsBad == 1))
            {
                return false;
            }
        }

        if(struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struGreyColor[index].struResGreyColor.nDiscolorResEnable == 1)   //色差保留使能
        {
            resColorMode = struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struGreyColor[index].struResGreyColor.nMode2;

            int tmpDiffColor = getDiffArithColor(struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struGreyColor[index].struResGreyColor.nResDiffColor, true);
            resColorMain = tmpDiffColor>>4;
            resColorVice = tmpDiffColor%16;

            //                resColorMain = struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struGreyColor[index].struResGreyColor.nMain2;
            //                resColorVice = struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struGreyColor[index].struResGreyColor.nVice2;

            if(resColorMode == 0)
            {
                resColorMin = 0;
                resColorMax = struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struGreyColor[index].struResGreyColor.nSensLow2;
            }
            else
            {
                resColorMin = ARITH_COLOR_MAX_SENS-struCnfp.struGroupIdentify[nLevel][nViewId][nGroup].struGreyColor[index].struResGreyColor.nSensUp2;
                resColorMax = ARITH_COLOR_MAX_SENS;
            }

            switch(resColorMain)
            {
            case BACKGROUND_COLOR_RED:
                if(resColorVice == BACKGROUND_COLOR_GREEN)
                {
                    resColorValue = ARITH_COLOR_MAX_SENS*r/(r+g);
                }
                else if(resColorVice == BACKGROUND_COLOR_BLUE)
                {
                    resColorValue = ARITH_COLOR_MAX_SENS*r/(r+b);
                }
                else if(resColorVice == BACKGROUND_COLOR_NIR)
                {
                    resColorValue = ARITH_COLOR_MAX_SENS*r/(r+n);
                }
                break;
            case BACKGROUND_COLOR_GREEN:
                if(resColorVice == BACKGROUND_COLOR_BLUE)
                {
                    resColorValue = ARITH_COLOR_MAX_SENS*g/(g+b);
                }
                else if(resColorVice == BACKGROUND_COLOR_NIR)
                {
                    resColorValue = ARITH_COLOR_MAX_SENS*g/(g+n);
                }
                break;
            case BACKGROUND_COLOR_BLUE:
                resColorValue = ARITH_COLOR_MAX_SENS*b/(b+n);
                break;
            case BACKGROUND_COLOR_NIR:
                break;
            }

            if((resColorValue > resColorMin)&&(resColorValue <= resColorMax))
            {
                return false;
            }
        }
    }

    return true;
}
return false;
}
#endif

bool GlobalFlow::imagePixelIsBadBySvm(uchar r, uchar g, uchar b, uchar n, int value)
{
    static int nr,ng,nb,nrg,ngb,nrb,nrr,ngg,nbb,ncon;
    nr = struGsh.m_svmcoefs[0]*100000;
    ng = struGsh.m_svmcoefs[1]*100000;
    nb = struGsh.m_svmcoefs[2]*100000;
    nrg = struGsh.m_svmcoefs[3]*100000;
    ngb = struGsh.m_svmcoefs[4]*100000;
    nrb = struGsh.m_svmcoefs[5]*100000;
    nrr = struGsh.m_svmcoefs[6]*100000;
    ngg = struGsh.m_svmcoefs[7]*100000;
    nbb =struGsh.m_svmcoefs[8]*100000;
    ncon = struGsh.m_svmcoefs[9]*100000;

    int tmpSvmCons = ncon;
    int svmsense = value;
    int nSvmCons;
    if(ncon < 0) {//综合正反选后，常数项为负数
        /* 灵敏度越大，常数项绝对值越大，选的越多 */
        if (svmsense >= 100) {
            nSvmCons = tmpSvmCons*(1+(svmsense-100)/100.0);
        }
        else {   // 灵敏度越小，常数项绝对值越小，选的越小
            nSvmCons = tmpSvmCons*(1-(100-svmsense)/100.0);
        }
    }else {
        /* 灵敏度越大，常数项越小，选的越多 */
        if (svmsense >= 100) {
            nSvmCons = tmpSvmCons*(1-(svmsense-100)/100.0);
        }
        else {   // 灵敏度越小，常数项越大，选的越少
            nSvmCons = tmpSvmCons*(1+(100-svmsense)/100.0);
        }
    }
    if((nr*r+ng*g+nb*b+nrg*r*g+ngb*g*b+nrb*r*b+nrr*r*r+ngg*g*g+nbb*b*b+nSvmCons*(1+(float)(svmsense-100)/100.0)) > 0){
        return false;
    }else{
        return true;
    }
}

bool GlobalFlow::imagePixelTotalIsGood(uchar r, uchar g, uchar b, uchar n, int nLevel, int nViewId, int nGroup)//接口已不用
{
    for(int arithIndex = 0;arithIndex<= ARITH_CROSS;arithIndex++){
        //        for(int view = 0;view < MAX_VIEW;view++){
        if(struCnfp.nGroupArithEnable[nLevel][nViewId][nGroup][arithIndex]){
            //                if(imagePixelIsBad(r,g,b,n,nLevel,nViewId,nGroup,arithIndex))
            //                    return true;
        }
        //        }
    }
}
bool GlobalFlow::imagePixelIsOutThreshold(uchar r, uchar g, uchar b, uchar n,int type)
{
    if(r+g ==0 || r+b == 0 || g+b == 0) return true;

    float rg = (float)r/(r+g)*100.0;
    float rb = (float)r/(r+b)*100.0;
    float gb = (float)g/(g+b)*100.0;
    if (type == 0)// 好料
    {
        if((r < struGsh.svmThreshold[0][0]) || (r > struGsh.svmThreshold[0][1]) || (g < struGsh.svmThreshold[1][0]) \
                || (g > struGsh.svmThreshold[1][1]) || (b < struGsh.svmThreshold[2][0]) || (b > struGsh.svmThreshold[2][1])){
            return true;
        }

        if( (rg < struGsh.svmDisThreshold[0][0]) || (rg > struGsh.svmDisThreshold[0][1]) || (rb < struGsh.svmDisThreshold[1][0]) \
                || (rb > struGsh.svmDisThreshold[1][1]) || (gb < struGsh.svmDisThreshold[2][0]) || (gb > struGsh.svmDisThreshold[2][1])){
            return true;
        }
    }
    else if(type == 1)
    {
        if((r < struGsh.svmThreshold[0][2]) || (r > struGsh.svmThreshold[0][3]) || (g < struGsh.svmThreshold[1][2]) \
                || (g > struGsh.svmThreshold[1][3]) || (b < struGsh.svmThreshold[2][2]) || (b > struGsh.svmThreshold[2][3])){
            return true;
        }

        if( (rg < struGsh.svmDisThreshold[0][2]) || (rg > struGsh.svmDisThreshold[0][3]) || (rb < struGsh.svmDisThreshold[1][2]) \
                || (rb > struGsh.svmDisThreshold[1][3]) || (gb < struGsh.svmDisThreshold[2][2]) || (gb > struGsh.svmDisThreshold[2][3])){
            return true;
        }
    }

    return false;
}

/* 长短算法获取物料值 */
int GlobalFlow::getShapeLongShort(int left_x, int left_y, int top_x, int top_y, int right_x, int right_y, int btm_x, int btm_y)
{
    int leftTop_x = left_x, leftTop_y = top_y;
    int rightBtm_x = right_x, rightBtm_y = btm_y;

    int shapeSens = (int)sqrt((rightBtm_x-leftTop_x)*(rightBtm_x-leftTop_x)+(rightBtm_y-leftTop_y)*(rightBtm_y-leftTop_y));

    if(shapeSens > ARITH_LENGTH_MAX_VALUE)
    {
        shapeSens = ARITH_LENGTH_MAX_VALUE;
    }

    return shapeSens;
}

/* 选圆选长算法获取值 */
int GlobalFlow::getShapeCircleLong(int left_x, int left_y, int top_x, int top_y, int right_x, int right_y, int btm_x, int btm_y)
{
    int leftTop_x = left_x, leftTop_y = top_y;
    int rightBtm_x = right_x, rightBtm_y = btm_y;

    int shapeSens = (int)sqrt((rightBtm_x-leftTop_x)*(rightBtm_x-leftTop_x)+(rightBtm_y-leftTop_y)*(rightBtm_y-leftTop_y));

    int area = (int)3.14*(shapeSens/2)*(shapeSens/2);

    return area;
}

/*存储图像 2维存成3维，4维的缩略图需单独处理*/
int GlobalFlow::saveBmp(uchar *pdata, QString bmp_file, int width, int height, int dim ,bool needCopy)
{
    if(struCnfg.nFreeSpace < 100.0)
        return 0;

    if(dim == 2) dim = 3;

    BITMAPFILEHEADER bmfh; //bmp文件头
    BITMAPINFOHEADER bmih;
    unsigned short wWidth, wHeight;//图像的分辨率，需要预先设置
    wWidth = width;
    wHeight = height;

    const char* filename = QString(bmp_file).toLocal8Bit().constData();
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

    //写文件
    write(fd, &bmfh, sizeof(BITMAPFILEHEADER));
    write(fd, &bmih, sizeof(BITMAPINFOHEADER));
    if(dim == 1){
        RGBQUAD rgbquad[256];
        for(int i = 0; i < 256; i++)
        {
            rgbquad[i].rgbBlue = i;
            rgbquad[i].rgbGreen = i;
            rgbquad[i].rgbRed = i;
            rgbquad[i].rgbReserved = 0;
        }
        write(fd,&rgbquad, sizeof(RGBQUAD)*256);
        for(int i = 0; i < wHeight; i++)
        {
            write(fd,pdata+ (wHeight- 1 - i) * wWidth,wWidth);
        }
    }else{
        write(fd, pdata, imageBuffSize);
    }
    //完成
#ifdef Q_OS_UNIX
    fsync(fd);
#endif
    close(fd);

    if(needCopy && !struGsh.nRemoteImageCaptureIsRun)
    {
        if(dim == 4){
            for(int i = 0;i < height;i++){
                for(int j =0;j< width;j++){
                    pdata[i*width*4 +j*4+3] = 255;   //缩略图不带有透明度
                }
            }
            QImage img(pdata, width, height, QImage::Format_ARGB32);
            img = img.scaled(180,120);
            img.save(bmp_file+"_copy","PNG",50);
        }else{

            QImage img(bmp_file);
            img = img.scaled(180,120);
            img.save(bmp_file+"_copy","PNG",50);
        }
    }
    struCnfg.nFreeSpace -=(bmfh.bfSize/1024.0/1024.0);
    return 1;
}

/* 开机初始化参数及全局线程 */
void GlobalFlow::initAll()
{
    getSetting();               // 获取配置文件参数内容
    getParamsFileStaus();       // 获取参数文件状态
    if( struGsh.poweroff_status  == 0 && struGsh.backCnf_status == 1 ) // 异常断电或者没有参数文件 且有备份文件
    {
        checkCompressParams();        //导出备份文件
    }
    //    getEngineer();              // 获取全局工程参数文件
    getGlobal();                // 获取全局需要保存的参数
    //    setLang();
    //    getCamera();                // 获取相机参数
    getProfile();               // 获取方案参数
    //    getPrefixedName();  	    // 重新获取预制方案父节点和子节点名称
    initGsh();                  // 获取全局共享参数，初始化串口等设备
    /*根据配置文件获取多套灯管系数值*/


    /**/
    QSettings setting(CFG_APPSet,QSettings::IniFormat);
    struGsh.poweroff_status = 0;
    setting.setValue("poweroffstatus",struGsh.poweroff_status); // 开机状态重置为0
    setting.sync();
    initIpcAttr();
#ifdef Q_OS_UNIX
    struGsh.serialPortOpenState = mySerial.comOpen(struGsh.serialNum);
    struGsh.serialPortOpenState2 = mySerial.comOpen(struGsh.serialNum_2);
#endif
    updateCurViewImgHight();
    //    resetFpgaRunModeRG();         //初始化用户模式
    resetMachineConfigRG();       //初始化机型配置
    //    resetSensorConfigRG();
    /* 开启全局串口发送线程 */
    // 串口发送线程
    mySerialSend = new mySerialSendThread;
    mySerialSend->start();
    // 串口接收线程
    mySerialRecv = new mySerialRecvThread;
    mySerialRecv->start();
    mySerial2Send = new mySerial2SendThread;
    mySerial2Send->start();
    mySerial2Recv = new mySerial2RecvThread;
    mySerial2Recv->start();
    plcautoctrl = new PlcAutoCtrlManager;
    // 更新时钟、自动清灰、报警查询命令线程
    myUpdateStatusThread = new updateStatusThread;
    myUpdateStatusThread->start();
    g_Udp.initSocket();
    // 网络读信号机波形线程
    myNetWork = new NetWork;
    myNetWork->udpBind(ipForInterface,"8081");

    // 2022/3/22 关闭狂采图功能
    myNetCpture = new MyNetCptureThread;
    //    myNetCpture->start();
    myFastNetThread = new MyFastNetWorkThread;
    //获取最新Token，开启MQTT线程

    // [zcy 2026.06.15] mqtt config
    // 为了适配 MQTT 的第三方支持和各项文件依赖所进行的改动
#ifdef Q_OS_UNIX
    myHttpFileClient = new MyHttpFileClient;
    myMqttThread = new mqttThread;
    myMqttMsgParaseThread = new mqttMsgParaseThread;
    if (ipForInterface == "162.254.129.100"){
        myMqttThread->start();
        myMqttMsgParaseThread->start();
    }
#endif
//    getThread->start();
}

/* 开机发送参数 */
void GlobalFlow::initSendAllParams()
{
    qDebug()<<"view"<<struGsh.nView;
    //#ifdef Q_OS_UNIX
#if 1
    /* 发送相机参数 */
    resetCamera();

    /* 发送相机板IP */
    if(struCnfg.nDPMachine)
        ipcResetCameraIpaddr(true, 0, 0, 0);

    /* 发送色选参数 */
    resetSortParams();

    /* 发送控制板参数 */
    resetControl();

    /* 发送整机相机校正使能 */
    cameraWaveCaliEnable();

    /* 发送灵敏度校正使能 */
    resetSensUpdateEnable();

    /* 关闭下位机色选 */
    resetSortOnOff(0, 0);	//turn off sort
    configIpcSortStart(0);

    /* 关闭下位机喷阀 */
    vavleOnOff();

    /* 喷阀保护时间　*/
    resetVavleDefendTime();

    //延迟缓冲时间
    if(struCnfg.nDPMachine)
        setIpcDelayBufRowCount();

    // 颜色还原及使能
    for(int i = 0; i < 3; i++)
    {
        setCameraColorRestore(i);
    }
    colorRestoreEnable();
#endif
}

/*****************************
 * 以下模型相关
 *
 * ****************************************/
void GlobalFlow::getGlobalIpcConfig()
{
    int i, j, k;

    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
           // if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
            for(k = 0; k < MAX_VIEW_UNIT; k++)
            {
                struCnfg.struIpcCameraParams[i][j][k].ipcType       = IPC_CAMERA_TYPE_IPC1;
                struCnfg.struIpcCameraParams[i][j][k].cameraId      = k;
                struCnfg.struIpcCameraParams[i][j][k].nIpAddr[0]    = 192;
                struCnfg.struIpcCameraParams[i][j][k].nIpAddr[1]    = 168;
                struCnfg.struIpcCameraParams[i][j][k].nIpAddr[2]    = 1;
                struCnfg.struIpcCameraParams[i][j][k].nIpAddr[3]    = MAX_VIEW_UNIT*j+k+1;
                struCnfg.struIpcCameraParams[i][j][k].nHoriPixel      = 1024;
                struCnfg.struIpcCameraParams[i][j][k].nVertPixel      = 0;
                struCnfg.struIpcCameraParams[i][j][k].nHoriOffset      = 1;
                struCnfg.struIpcCameraParams[i][j][k].nHoriWidth      = 100;
                struCnfg.struIpcCameraParams[i][j][k].nHoriWidth_2      = 0;
                struCnfg.struIpcCameraParams[i][j][k].nVertOffset      = 100;
                struCnfg.struIpcCameraParams[i][j][k].nVertWidth      = 100;
                struCnfg.struIpcCameraParams[i][j][k].nSaveImgEnable  = 0;
                if(k == 0)
                    struCnfg.struIpcCameraParams[i][j][k].nSaveImgEnable  = 1;

            }
        }
    }

    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
            struCnfg.struLayerInfo[i].stuCameraInfo[j].nColorRestoreRed[0] = 23372;
            struCnfg.struLayerInfo[i].stuCameraInfo[j].nColorRestoreRed[1] = -4725;
            struCnfg.struLayerInfo[i].stuCameraInfo[j].nColorRestoreRed[2] = -172;

            struCnfg.struLayerInfo[i].stuCameraInfo[j].nColorRestoreGreen[0] = -2604;
            struCnfg.struLayerInfo[i].stuCameraInfo[j].nColorRestoreGreen[1] = 24671;
            struCnfg.struLayerInfo[i].stuCameraInfo[j].nColorRestoreGreen[2] = -2521;

            struCnfg.struLayerInfo[i].stuCameraInfo[j].nColorRestoreBlue[0] = -533;
            struCnfg.struLayerInfo[i].stuCameraInfo[j].nColorRestoreBlue[1] = 3856;
            struCnfg.struLayerInfo[i].stuCameraInfo[j].nColorRestoreBlue[2] = 13685;
        }
    }
}

/* 初始化模型参数*/
void GlobalFlow::initIpcAttr()
{
    int i, j, k;

    memset(&struIpcShare, 0, sizeof(struIpcShareInfo));

    for(i = 0; i < MAX_VIEW; i++)
    {
        for( j = 0;j < MAX_VIEW_UNIT;j++)
        {
            struIpcShare.struIpcInfo[i][j].aliveNumLast = 0;
            struIpcShare.struIpcInfo[i][j].aliveRetryCnt = 100;
            struIpcShare.struIpcInfo[i][j].aliveNum = 0;
            struIpcShare.struIpcInfo[i][j].restartStat = 0;
            memset(struIpcShare.struIpcInfo[i][j].version, 0, sizeof(struIpcShare.struIpcInfo[i][j].version));
            memset(struIpcShare.struIpcInfo[i][j].versionLib, 0, sizeof(struIpcShare.struIpcInfo[i][j].versionLib));

            struIpcShare.struIpcInfo[i][j].sortStat = 0;
            struIpcShare.struIpcInfo[i][j].modelStat = 0;
            struIpcShare.struIpcInfo[i][j].modelUploadStat = 0;
            struIpcShare.struIpcInfo[i][j].recvCamTotalLines = 0;
            struIpcShare.struIpcInfo[i][j].loseCamTotalLines = 0;
            struIpcShare.struIpcInfo[i][j].freeSize = 0;
            struIpcShare.struIpcInfo[i][j].npuUsed = 0;
            struIpcShare.struIpcInfo[i][j].chipTemp = 0;
            struIpcShare.struIpcInfo[i][j].lineFreq = 0;

            struIpcShare.struIpcStatus[i][j].ipcStatus = IPC_REQ_RETURN_NULL;
            struIpcShare.struIpcStatus[i][j].modelLoadStatus = 0;
            struIpcShare.struIpcStatus[i][j].cameraCount = struCnfg.nIpcCamera;

            struIpcShare.struIpcStatus[i][j].cameraStatus = IPC_REQ_RETURN_NULL;

            struIpcShare.struIpcModelAble[i][j].modelCount = 0;
            for(k = 0; k < MAX_IPC_MODEL_NUM; k++)
            {
                memset(struIpcShare.struIpcModelAble[i][j].modelInfo[k].modelname, 0, MAX_IPC_MODEL_NAME_LENGTH);
                memset(struIpcShare.struIpcModelAble[i][j].modelInfo[k].modelDate, 0, MAX_IPC_MODEL_DATE_LENGTH);
            }

            struIpcShare.struIpcModelClass[i][j].classCount = 0;
            for(k = 0; k < MAX_IPC_MODEL_CLASS; k++)
            {
                memset(struIpcShare.struIpcModelClass[i][j].className[k], 0, MAX_IPC_CLASS_NAME_LENGTH);
            }

            struIpcShare.struIpcReqValue[i][j].configArithValue = IPC_REQ_RETURN_NULL;
            struIpcShare.struIpcReqValue[i][j].modelLoadValue = IPC_REQ_RETURN_NULL;
            memset(struIpcShare.struIpcReqValue[i][j].modelLoadName, 0, MAX_IPC_MODEL_NAME_LENGTH);
            struIpcShare.struIpcReqValue[i][j].sortStartRes = IPC_REQ_RETURN_NULL;
            struIpcShare.struIpcReqValue[i][j].sortStopRes = IPC_REQ_RETURN_NULL;
            struIpcShare.struIpcReqValue[i][j].shutDownRes = IPC_REQ_RETURN_NULL;

            struIpcShare.struIpcReqStauts[i][j].ipcInfoReq = IPC_REQ_RETURN_NULL;
            struIpcShare.struIpcReqStauts[i][j].ipcStautsReq = IPC_REQ_RETURN_NULL;
            struIpcShare.struIpcReqStauts[i][j].modelEnableReq = IPC_REQ_RETURN_NULL;
            struIpcShare.struIpcReqStauts[i][j].modelInfoReq = IPC_REQ_RETURN_NULL;
            struIpcShare.struIpcReqStauts[i][j].configCameraReq = IPC_REQ_RETURN_NULL;
            struIpcShare.struIpcReqStauts[i][j].modelLoadReq = IPC_REQ_RETURN_NULL;
            struIpcShare.struIpcReqStauts[i][j].sortStartReq = IPC_REQ_RETURN_NULL;
            struIpcShare.struIpcReqStauts[i][j].sortStopReq = IPC_REQ_RETURN_NULL;
            struIpcShare.struIpcReqStauts[i][j].shutDownReq = IPC_REQ_RETURN_NULL;
            struIpcShare.struIpcReqStauts[i][j].ipcAICfgReq = IPC_REQ_RETURN_NULL;
            struIpcShare.struIpcReqStauts[i][j].modelTargetValueReq = IPC_REQ_RETURN_NULL;
            struIpcShare.struIpcClassParamsReqStauts[i][j] = IPC_REQ_RETURN_NULL;

            //    struIpcShare.ipcHasConnect = 0;

            struIpcShare.ipcHeartStatus[i][j] = 0;
            struIpcShare.ipcLoadStatus[i][j] = 0;
            struIpcShare.isIpcRestart[i][j] = 0;
            struIpcShare.sCurModelTargetValue[i][j] = 0;

            for(k = 0; k < MAX_IPC_MODEL_CLASS; k++)
            {
                struIpcShare.sCurModelClassBadPointsCountPerCam[i][j][k] = 0;
                struIpcShare.sCurModelClassBadPointsCount[i][j][k] = 0;
                struIpcShare.sCurModelLastClassBadPointsCount[k] = 0;
            }
        }
    }

    //初始化当前模型
    QByteArray modelName(struCnfp.sCurModelName[struGsh.nView][struGsh.nMatGroup]);
    g_Runtime().modelFullName = QString(modelName);
    g_Runtime().modelFullNameList.clear();
    for(int i =0;i<struIpcShare.struIpcModelAble[struGsh.nView][struGsh.nUnit].modelCount;i++)
    {
        QByteArray modelName(struIpcShare.struIpcModelAble[struGsh.nView][struGsh.nUnit].modelInfo[i].modelname);
        g_Runtime().modelFullNameList.append(modelName);
    }
}
// 01 查询心跳
void GlobalFlow::getIpcInfo(bool isAll, int view, int unit)
{
    //    struIpcShare.allIpcIsCalling = 1;
    for(int i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(int j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(struCnfg.struLayerInfo[i].nViewIsUsed[j] == 1)
            {
                for(int k = 0; k <struCnfg.struLayerInfo[i].nViewUnitTotal[j]; k++)
                {
                    if(!isAll && (view != j || unit != k)) continue;
                    if(struCnfg.struIpcCameraParams[i][j][k].ipcType != IPC_CAMERA_TYPE_NULL
                            && struCnfg.struVer.nUnitRes[i][j][k] == 1)
                    {
                        struIpcShare.struIpcReqStauts[j][k].ipcInfoReq = IPC_REQ_RETURN_NULL;
                        g_Udp.pushUdpCmdData(CMD_UDP_IPC_REQ_INFO, true, j, k, 0, NULL);
                        for(int m = 0;m < struCnfg.nWaitTimeout/10;m++)
                        {
                            if(struGsh.bUpdateState) return;
                            msleep(10);
                            if(struIpcShare.struIpcReqStauts[j][k].ipcInfoReq == 1)
                            {
                                break;
                            }
                        }
                        if(!struCnfg.nXsystemBoard[j])
                        {
                            struIpcShare.struIpcReqStauts[j][k].keepAlive = IPC_REQ_RETURN_NULL;
                            g_Udp.pushUdpCmdData(CMD_CAMERA_AI_MISC_STATUS, true, j, k, 0, NULL);
                            for(int m = 0;m < struCnfg.nWaitTimeout/10;m++)
                            {
                                if(struGsh.bUpdateState) return;
                                msleep(10);
                                if(struIpcShare.struIpcReqStauts[j][k].keepAlive == 1)
                                {
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    //    struIpcShare.allIpcIsCalling = 0;
}

// 02 查询模型列表
void GlobalFlow::getIpcModelEnable(bool isAll, int view, int unit)
{
    int i, j, k;
    for(i = 0; i < MAX_VIEW; i++)
    {
        for( j = 0;j < MAX_VIEW_UNIT;j++)
        {
            struIpcShare.struIpcModelAble[i][j].modelCount = 0;
            for(k = 0; k < MAX_IPC_MODEL_NUM; k++)
            {
                memset(struIpcShare.struIpcModelAble[i][j].modelInfo[k].modelname, 0, MAX_IPC_MODEL_NAME_LENGTH);
                memset(struIpcShare.struIpcModelAble[i][j].modelInfo[k].modelDate, 0, MAX_IPC_MODEL_DATE_LENGTH);
            }
        }
    }

    for(int i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(int j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(struCnfg.struLayerInfo[i].nViewIsUsed[j] == 1)
            {

                for(int k = 0; k <struCnfg.struLayerInfo[i].nViewUnitTotal[j]; k++)
                {
                    if(!isAll && (view != j || unit != k)) continue;
                    if(struCnfg.struIpcCameraParams[i][j][k].ipcType != IPC_CAMERA_TYPE_NULL)
                    {
                        struIpcShare.struIpcReqStauts[j][k].modelEnableReq = IPC_REQ_RETURN_NULL;
                        g_Udp.pushUdpCmdData(CMD_UDP_IPC_REQ_MODEL_ABLE, true, j, k, 0, NULL);
                        //                        msleep(100);
                        for(int m = 0;m < 10;m++)
                        {
                            myFlow.msleep(100);
                            if(struIpcShare.struIpcReqStauts[j][k].modelEnableReq == 1)
                            {
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}

// 03 查询模型信息
void GlobalFlow::getIpcModelInfo(bool isAll, int view, int unit)
{
    unsigned char data[MAX_IPC_MODEL_NAME_LENGTH];

    memset(data, 0, MAX_IPC_MODEL_NAME_LENGTH);
    // memcpy(data, struCnfp.sCurModelName[view], sizeof(struCnfp.sCurModelName[view]));

    //找到 通道unit 所在的group

    for(int i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(int j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(struCnfg.struLayerInfo[i].nViewIsUsed[j] == 1)
            {
                for(int k = 0; k <struCnfg.struLayerInfo[i].nViewUnitTotal[j]; k++)
                {
                    if(!isAll && (view != j || unit != k)) continue;
                    struIpcShare.struIpcModelClass[j][k].classCount = 0;
                    for(int m = 0; m < MAX_IPC_MODEL_CLASS; m++)
                    {
                        memset(struIpcShare.struIpcModelClass[j][k].className[m], 0, MAX_IPC_CLASS_NAME_LENGTH);
                    }
                    if(struCnfg.struIpcCameraParams[i][j][k].ipcType != IPC_CAMERA_TYPE_NULL)
                    {
                        memset(data, 0, MAX_IPC_MODEL_NAME_LENGTH);

                        for(int group = 0; group < struCnfg.struLayerInfo[0].stuViewInfo[j].nIdentifyGroupTotal; group++)
                        {
                            if(myFlow.getUnitGroup(j,k,group))
                            {
                                memcpy(data, struCnfp.sCurModelName[j][group], sizeof(struCnfp.sCurModelName[j][group]));
                            }
                        }

                        struIpcShare.struIpcReqStauts[j][k].modelInfoReq = IPC_REQ_RETURN_NULL ;
                        g_Udp.pushUdpCmdData(CMD_UDP_IPC_REQ_MODEL_INFO, true, j,k, MAX_IPC_MODEL_NAME_LENGTH, data);
                        for(int m = 0;m < 100;m++)
                        {
                            myFlow.msleep(10);
                            if(struIpcShare.struIpcReqStauts[j][k].modelInfoReq == 1)
                            {
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}

// 04 加载模型
void GlobalFlow::loadIpcModel(int view, int unit, int modelIndex)
{
    int i, j,nUnitAddr;

    unsigned char data[MAX_IPC_MODEL_LOAD_LENGTH];
    memset(data, 0, MAX_IPC_MODEL_LOAD_LENGTH);

    struIpcShare.struIpcReqValue[view][unit].modelLoadValue = IPC_REQ_RETURN_NULL;

    if(struCnfp.nDualLayerModelSame == 1) // 全部相同
    {
        memset(data, 0, MAX_IPC_MODEL_LOAD_LENGTH);
        memcpy(data, struCnfp.sCurModelName[view][0], sizeof(struCnfp.sCurModelName[view][0]));
        g_Udp.pushUdpCmdData(CMD_UDP_IPC_REQ_MODEL_LOAD, true, 0x09,0xff, MAX_IPC_MODEL_LOAD_LENGTH, data);
        data[64]= struGsh.nIpcLoadCnt;
        logger()->info("%1 load model",QString::fromUtf8(struCnfp.sCurModelName[view][0]));
    }
    else if(struCnfp.nDualLayerModelSame == 3) //单视相同
    {
        memset(data, 0, MAX_IPC_MODEL_LOAD_LENGTH);
        memcpy(data, struCnfp.sCurModelName[0][0], sizeof(struCnfp.sCurModelName[0][0]));
        g_Udp.pushUdpCmdData(CMD_UDP_IPC_REQ_MODEL_LOAD, true, LCD_VERI_MAIN_VIEW_FRONT,0xff, MAX_IPC_MODEL_LOAD_LENGTH, data);
        data[64]= struGsh.nIpcLoadCnt;
        if(struCnfg.nXsystemBoard[LCD_VERI_MAIN_VIEW_FRONT] == 0)
            logger()->info("layer_1 view %1 %2 load rk model",LCD_VERI_MAIN_VIEW_FRONT,QString::fromUtf8(struCnfp.sCurModelName[0][0]));
        else
            logger()->info("layer_1 view %1 %2 load x model",LCD_VERI_MAIN_VIEW_FRONT,QString::fromUtf8(struCnfp.sCurModelName[0][0]));

        memset(data, 0, MAX_IPC_MODEL_LOAD_LENGTH);
        memcpy(data, struCnfp.sCurModelName[1][0], sizeof(struCnfp.sCurModelName[1][0]));
        g_Udp.pushUdpCmdData(CMD_UDP_IPC_REQ_MODEL_LOAD, true, LCD_VERI_MAIN_VIEW_REAR,0xff, MAX_IPC_MODEL_LOAD_LENGTH, data);
        data[64]= struGsh.nIpcLoadCnt;
        if(struCnfg.nXsystemBoard[LCD_VERI_MAIN_VIEW_REAR] == 0)
            logger()->info("layer_1 view %1 %2 load rk model",LCD_VERI_MAIN_VIEW_REAR,QString::fromUtf8(struCnfp.sCurModelName[1][0]));
        else
            logger()->info("layer_1 view %1 %2 load x model",LCD_VERI_MAIN_VIEW_REAR,QString::fromUtf8(struCnfp.sCurModelName[1][0]));

        if(struCnfg.struLayerInfo[0].nViewIsUsed[2] == 1){
            memset(data, 0, MAX_IPC_MODEL_LOAD_LENGTH);
            memcpy(data, struCnfp.sCurModelName[2][0], sizeof(struCnfp.sCurModelName[2][0]));
            g_Udp.pushUdpCmdData(CMD_UDP_IPC_REQ_MODEL_LOAD, true, LCD_VERI_VICE_VIEW_FRONT,0xff, MAX_IPC_MODEL_LOAD_LENGTH, data);
            data[64]= struGsh.nIpcLoadCnt;
            if(struCnfg.nXsystemBoard[LCD_VERI_VICE_VIEW_FRONT] == 0)
                logger()->info("layer_1 view %1 %2 load rk model",LCD_VERI_VICE_VIEW_FRONT,QString::fromUtf8(struCnfp.sCurModelName[2][0]));
            else
                logger()->info("layer_1 view %1 %2 load x model",LCD_VERI_VICE_VIEW_FRONT,QString::fromUtf8(struCnfp.sCurModelName[2][0]));

            memset(data, 0, MAX_IPC_MODEL_LOAD_LENGTH);
            memcpy(data, struCnfp.sCurModelName[3][0], sizeof(struCnfp.sCurModelName[3][0]));

            g_Udp.pushUdpCmdData(CMD_UDP_IPC_REQ_MODEL_LOAD, true, LCD_VERI_VICE_VIEW_REAR,0xff, MAX_IPC_MODEL_LOAD_LENGTH, data);
            data[64]= struGsh.nIpcLoadCnt;
            if(struCnfg.nXsystemBoard[LCD_VERI_VICE_VIEW_REAR] == 0)
                logger()->info("layer_1 view %1 %2 load rk model",LCD_VERI_VICE_VIEW_REAR,QString::fromUtf8(struCnfp.sCurModelName[3][0]));
            else
                logger()->info("layer_1 view %1 %2 load x model",LCD_VERI_VICE_VIEW_REAR,QString::fromUtf8(struCnfp.sCurModelName[3][0]));
        }
        if(struCnfg.nMachineType == MACHINE_LD2_ZL_GP){
            memset(data, 0, MAX_IPC_MODEL_LOAD_LENGTH);
            memcpy(data, struCnfp.sCurModelName[4][0], sizeof(struCnfp.sCurModelName[4][0]));

            g_Udp.pushUdpCmdData(CMD_UDP_IPC_REQ_MODEL_LOAD, true, LCD_VERI_MAIN_VIEW_FRONT_2,0xff, MAX_IPC_MODEL_LOAD_LENGTH, data);
            data[64]= struGsh.nIpcLoadCnt;
            if(struCnfg.nXsystemBoard[LCD_VERI_MAIN_VIEW_FRONT_2] == 0)
                logger()->info("layer_2 view %1 %2 load rk model",LCD_VERI_MAIN_VIEW_FRONT_2,QString::fromUtf8(struCnfp.sCurModelName[4][0]));
            else
                logger()->info("layer_2 view %1 %2 load x model",LCD_VERI_MAIN_VIEW_FRONT_2,QString::fromUtf8(struCnfp.sCurModelName[4][0]));

            memset(data, 0, MAX_IPC_MODEL_LOAD_LENGTH);
            memcpy(data, struCnfp.sCurModelName[5][0], sizeof(struCnfp.sCurModelName[5][0]));

            g_Udp.pushUdpCmdData(CMD_UDP_IPC_REQ_MODEL_LOAD, true, LCD_VERI_MAIN_VIEW_REAR_2,0xff, MAX_IPC_MODEL_LOAD_LENGTH, data);
            data[64]= struGsh.nIpcLoadCnt;
            if(struCnfg.nXsystemBoard[LCD_VERI_MAIN_VIEW_REAR_2] == 0)
                logger()->info("layer_2 view %1 %2 load rk model",LCD_VERI_MAIN_VIEW_REAR_2,QString::fromUtf8(struCnfp.sCurModelName[5][0]));
            else
                logger()->info("layer_2 view %1 %2 load x model",LCD_VERI_MAIN_VIEW_REAR_2,QString::fromUtf8(struCnfp.sCurModelName[5][0]));

            if(struCnfg.struLayerInfo[0].nViewIsUsed[6] == 1)
            {
                memset(data, 0, MAX_IPC_MODEL_LOAD_LENGTH);
                memcpy(data, struCnfp.sCurModelName[6][0], sizeof(struCnfp.sCurModelName[6][0]));

                g_Udp.pushUdpCmdData(CMD_UDP_IPC_REQ_MODEL_LOAD, true, LCD_VERI_VICE_VIEW_FRONT_2,0xff, MAX_IPC_MODEL_LOAD_LENGTH, data);
                data[64]= struGsh.nIpcLoadCnt;
                if(struCnfg.nXsystemBoard[LCD_VERI_VICE_VIEW_FRONT_2] == 0)
                    logger()->info("layer_2 view %1 %2 load rk model",LCD_VERI_VICE_VIEW_FRONT_2,QString::fromUtf8(struCnfp.sCurModelName[6][0]));
                else
                    logger()->info("layer_2 view %1 %2 load x model",LCD_VERI_VICE_VIEW_FRONT_2,QString::fromUtf8(struCnfp.sCurModelName[6][0]));

                memset(data, 0, MAX_IPC_MODEL_LOAD_LENGTH);
                memcpy(data, struCnfp.sCurModelName[7][0], sizeof(struCnfp.sCurModelName[7][0]));

                g_Udp.pushUdpCmdData(CMD_UDP_IPC_REQ_MODEL_LOAD, true, LCD_VERI_VICE_VIEW_REAR_2,0xff, MAX_IPC_MODEL_LOAD_LENGTH, data);
                data[64]= struGsh.nIpcLoadCnt;
                if(struCnfg.nXsystemBoard[LCD_VERI_VICE_VIEW_REAR_2] == 0)
                    logger()->info("layer_2 view %1 %2 load rk model",LCD_VERI_VICE_VIEW_REAR_2,QString::fromUtf8(struCnfp.sCurModelName[7][0]));
                else
                    logger()->info("layer_2 view %1 %2 load x model",LCD_VERI_VICE_VIEW_REAR_2,QString::fromUtf8(struCnfp.sCurModelName[7][0]));
            }
        }
    }
    else if(struCnfp.nDualLayerModelSame == 2 || struCnfp.nDualLayerModelSame == 4)
    {
        // 前主
        for(int k = 0; k < struCnfg.struLayerInfo[0].stuViewInfo[0].nIdentifyGroupTotal; k++)
        {
            memset(data, 0, MAX_IPC_MODEL_LOAD_LENGTH);
            memcpy(data, struCnfp.sCurModelName[0][k], sizeof(struCnfp.sCurModelName[0][k]));
            for(int m = 0; m < struCnfg.struLayerInfo[0].stuViewInfo[0].stuIdenGupInfo[k].nGroupUnitTotal; m++)
            {
                nUnitAddr = getIdentifyGroupAddr(0, 0, k, m);
                if(nUnitAddr > 128)
                    continue;
                else
                    nUnitAddr= getIdentifyGroupAddr(0, 0, k, m)%64;
                g_Udp.pushUdpCmdData(CMD_UDP_IPC_REQ_MODEL_LOAD, true, LCD_VERI_MAIN_VIEW_FRONT,nUnitAddr, MAX_IPC_MODEL_LOAD_LENGTH, data);

                if(struCnfg.nXsystemBoard[LCD_VERI_MAIN_VIEW_FRONT] == 0)
                    logger()->info("layer_1 view %1 unit %2 %3 load rk model",LCD_VERI_MAIN_VIEW_FRONT,nUnitAddr,QString::fromUtf8(struCnfp.sCurModelName[0][k]));
                else
                    logger()->info("layer_1 view %1 unit %2 %3 load x model",LCD_VERI_MAIN_VIEW_FRONT,nUnitAddr,QString::fromUtf8(struCnfp.sCurModelName[0][k]));
            }
            data[64]= struGsh.nIpcLoadCnt;
        }

        // 后主
        for(int k = 0; k < struCnfg.struLayerInfo[0].stuViewInfo[1].nIdentifyGroupTotal; k++)
        {
            memset(data, 0, MAX_IPC_MODEL_LOAD_LENGTH);
            memcpy(data, struCnfp.sCurModelName[1][k], sizeof(struCnfp.sCurModelName[1][k]));

            for(int m = 0; m < struCnfg.struLayerInfo[0].stuViewInfo[1].stuIdenGupInfo[k].nGroupUnitTotal; m++)
            {
                nUnitAddr = getIdentifyGroupAddr(0, 1, k, m);
                if(nUnitAddr > 128)
                    continue;
                else
                    nUnitAddr= getIdentifyGroupAddr(0, 1, k, m)%64;
                g_Udp.pushUdpCmdData(CMD_UDP_IPC_REQ_MODEL_LOAD, true, LCD_VERI_MAIN_VIEW_REAR,nUnitAddr, MAX_IPC_MODEL_LOAD_LENGTH, data);

                if(struCnfg.nXsystemBoard[LCD_VERI_MAIN_VIEW_REAR] == 0)
                    logger()->info("layer_1 view %1 unit %2 %3 load rk model",LCD_VERI_MAIN_VIEW_REAR,nUnitAddr,QString::fromUtf8(struCnfp.sCurModelName[1][k]));
                else
                    logger()->info("layer_1 view %1 unit %2 %3 load x model",LCD_VERI_MAIN_VIEW_REAR,nUnitAddr,QString::fromUtf8(struCnfp.sCurModelName[1][k]));
            }

            data[64]= struGsh.nIpcLoadCnt;
        }

        if(struCnfg.struLayerInfo[0].nViewIsUsed[2] == 1){

            // 前辅
            for(int k = 0; k < struCnfg.struLayerInfo[0].stuViewInfo[2].nIdentifyGroupTotal; k++)
            {
                memset(data, 0, MAX_IPC_MODEL_LOAD_LENGTH);
                memcpy(data, struCnfp.sCurModelName[2][k], sizeof(struCnfp.sCurModelName[2][k]));
                for(int m = 0; m < struCnfg.struLayerInfo[0].stuViewInfo[2].stuIdenGupInfo[k].nGroupUnitTotal; m++)
                {
                    nUnitAddr = getIdentifyGroupAddr(0, 2, k, m);
                    if(nUnitAddr > 128)
                        continue;
                    else
                        nUnitAddr= getIdentifyGroupAddr(0, 2, k, m)%64;
                    g_Udp.pushUdpCmdData(CMD_UDP_IPC_REQ_MODEL_LOAD, true, LCD_VERI_VICE_VIEW_FRONT,nUnitAddr, MAX_IPC_MODEL_LOAD_LENGTH, data);

                    if(struCnfg.nXsystemBoard[LCD_VERI_VICE_VIEW_FRONT] == 0)
                        logger()->info("layer_1 view %1 unit %2 %3 load rk model",LCD_VERI_VICE_VIEW_FRONT,nUnitAddr,QString::fromUtf8(struCnfp.sCurModelName[2][k]));
                    else
                        logger()->info("layer_1 view %1 unit %2 %3 load x model",LCD_VERI_VICE_VIEW_FRONT,nUnitAddr,QString::fromUtf8(struCnfp.sCurModelName[2][k]));
                }
                data[64]= struGsh.nIpcLoadCnt;
            }

            // 后辅
            for(int k = 0; k < struCnfg.struLayerInfo[0].stuViewInfo[3].nIdentifyGroupTotal; k++)
            {
                memset(data, 0, MAX_IPC_MODEL_LOAD_LENGTH);
                memcpy(data, struCnfp.sCurModelName[3][k], sizeof(struCnfp.sCurModelName[3][k]));
                for(int m = 0; m < struCnfg.struLayerInfo[0].stuViewInfo[3].stuIdenGupInfo[k].nGroupUnitTotal; m++)
                {
                    nUnitAddr = getIdentifyGroupAddr(0, 3, k, m);
                    if(nUnitAddr > 128)
                        continue;
                    else
                        nUnitAddr= getIdentifyGroupAddr(0, 3, k, m)%64;
                    g_Udp.pushUdpCmdData(CMD_UDP_IPC_REQ_MODEL_LOAD, true, LCD_VERI_VICE_VIEW_REAR,nUnitAddr, MAX_IPC_MODEL_LOAD_LENGTH, data);

                    if(struCnfg.nXsystemBoard[LCD_VERI_VICE_VIEW_REAR] == 0)
                        logger()->info("layer_1 view %1 unit %2 %3 load rk model",LCD_VERI_VICE_VIEW_REAR,nUnitAddr,QString::fromUtf8(struCnfp.sCurModelName[3][k]));
                    else
                        logger()->info("layer_1 view %1 unit %2 %3 load x model",LCD_VERI_VICE_VIEW_REAR,nUnitAddr,QString::fromUtf8(struCnfp.sCurModelName[3][k]));
                }
                data[64]= struGsh.nIpcLoadCnt;
            }
        }



        if(struCnfg.nMachineType == MACHINE_LD2_ZL_GP){

            // 前主
            for(int k = 0; k < struCnfg.struLayerInfo[0].stuViewInfo[4].nIdentifyGroupTotal; k++)
            {
                memset(data, 0, MAX_IPC_MODEL_LOAD_LENGTH);
                memcpy(data, struCnfp.sCurModelName[4][k], sizeof(struCnfp.sCurModelName[4][k]));
                for(int m = 0; m < struCnfg.struLayerInfo[0].stuViewInfo[4].stuIdenGupInfo[k].nGroupUnitTotal; m++)
                {
                    nUnitAddr = getIdentifyGroupAddr(0, 4, k, m);
                    if(nUnitAddr > 128)
                        continue;
                    else
                        nUnitAddr= getIdentifyGroupAddr(0, 4, k, m)%64;
                    g_Udp.pushUdpCmdData(CMD_UDP_IPC_REQ_MODEL_LOAD, true, LCD_VERI_MAIN_VIEW_FRONT_2,nUnitAddr, MAX_IPC_MODEL_LOAD_LENGTH, data);

                    if(struCnfg.nXsystemBoard[LCD_VERI_MAIN_VIEW_FRONT_2] == 0)
                        logger()->info("layer_2 view %1 unit %2 %3 load rk model",LCD_VERI_MAIN_VIEW_FRONT_2,nUnitAddr,QString::fromUtf8(struCnfp.sCurModelName[4][k]));
                    else
                        logger()->info("layer_2 view %1 unit %2 %3 load x model",LCD_VERI_MAIN_VIEW_FRONT_2,nUnitAddr,QString::fromUtf8(struCnfp.sCurModelName[4][k]));
                }
                data[64]= struGsh.nIpcLoadCnt;
            }

            // 后主
            for(int k = 0; k < struCnfg.struLayerInfo[0].stuViewInfo[5].nIdentifyGroupTotal; k++)
            {
                memset(data, 0, MAX_IPC_MODEL_LOAD_LENGTH);
                memcpy(data, struCnfp.sCurModelName[5][k], sizeof(struCnfp.sCurModelName[5][k]));
                for(int m = 0; m < struCnfg.struLayerInfo[0].stuViewInfo[5].stuIdenGupInfo[k].nGroupUnitTotal; m++)
                {
                    nUnitAddr = getIdentifyGroupAddr(0, 5, k, m);
                    if(nUnitAddr > 128)
                        continue;
                    else
                        nUnitAddr= getIdentifyGroupAddr(0, 5, k, m)%64;
                    g_Udp.pushUdpCmdData(CMD_UDP_IPC_REQ_MODEL_LOAD, true, LCD_VERI_MAIN_VIEW_REAR_2,nUnitAddr, MAX_IPC_MODEL_LOAD_LENGTH, data);

                    if(struCnfg.nXsystemBoard[LCD_VERI_MAIN_VIEW_REAR_2] == 0)
                        logger()->info("layer_2 view %1 unit %2 %3 load rk model",LCD_VERI_MAIN_VIEW_REAR_2,nUnitAddr,QString::fromUtf8(struCnfp.sCurModelName[5][k]));
                    else
                        logger()->info("layer_2 view %1 unit %2 %3 load x model",LCD_VERI_MAIN_VIEW_REAR_2,nUnitAddr,QString::fromUtf8(struCnfp.sCurModelName[5][k]));
                }
                data[64]= struGsh.nIpcLoadCnt;
            }

            if(struCnfg.struLayerInfo[0].nViewIsUsed[6] == 1){

                // 前辅
                for(int k = 0; k < struCnfg.struLayerInfo[0].stuViewInfo[6].nIdentifyGroupTotal; k++)
                {
                    memset(data, 0, MAX_IPC_MODEL_LOAD_LENGTH);
                    memcpy(data, struCnfp.sCurModelName[6][k], sizeof(struCnfp.sCurModelName[6][k]));
                    for(int m = 0; m < struCnfg.struLayerInfo[0].stuViewInfo[6].stuIdenGupInfo[k].nGroupUnitTotal; m++)
                    {
                        nUnitAddr = getIdentifyGroupAddr(0, 6, k, m);
                        if(nUnitAddr > 128)
                            continue;
                        else
                            nUnitAddr= getIdentifyGroupAddr(0, 6, k, m)%64;
                        g_Udp.pushUdpCmdData(CMD_UDP_IPC_REQ_MODEL_LOAD, true, LCD_VERI_VICE_VIEW_FRONT_2,nUnitAddr, MAX_IPC_MODEL_LOAD_LENGTH, data);

                        if(struCnfg.nXsystemBoard[LCD_VERI_VICE_VIEW_FRONT_2] == 0)
                            logger()->info("layer_2 view %1 unit %2 %3 load rk model",LCD_VERI_VICE_VIEW_FRONT_2,nUnitAddr,QString::fromUtf8(struCnfp.sCurModelName[6][k]));
                        else
                            logger()->info("layer_2 view %1 unit %2 %3 load x model",LCD_VERI_VICE_VIEW_FRONT_2,nUnitAddr,QString::fromUtf8(struCnfp.sCurModelName[6][k]));
                    }
                    data[64]= struGsh.nIpcLoadCnt;
                }

                // 后辅
                for(int k = 0; k < struCnfg.struLayerInfo[0].stuViewInfo[7].nIdentifyGroupTotal; k++)
                {
                    memset(data, 0, MAX_IPC_MODEL_LOAD_LENGTH);
                    memcpy(data, struCnfp.sCurModelName[7][k], sizeof(struCnfp.sCurModelName[7][k]));
                    for(int m = 0; m < struCnfg.struLayerInfo[0].stuViewInfo[6].stuIdenGupInfo[k].nGroupUnitTotal; m++)
                    {
                        nUnitAddr = getIdentifyGroupAddr(0, 7, k, m);
                        if(nUnitAddr > 128)
                            continue;
                        else
                            nUnitAddr= getIdentifyGroupAddr(0, 7, k, m)%64;
                        g_Udp.pushUdpCmdData(CMD_UDP_IPC_REQ_MODEL_LOAD, true, LCD_VERI_VICE_VIEW_REAR_2,nUnitAddr, MAX_IPC_MODEL_LOAD_LENGTH, data);

                        if(struCnfg.nXsystemBoard[LCD_VERI_VICE_VIEW_REAR_2] == 0)
                            logger()->info("layer_2 view %1 unit %2 %3 load rk model",LCD_VERI_VICE_VIEW_REAR_2,nUnitAddr,QString::fromUtf8(struCnfp.sCurModelName[7][k]));
                        else
                            logger()->info("layer_2 view %1 unit %2 %3 load x model",LCD_VERI_VICE_VIEW_REAR_2,nUnitAddr,QString::fromUtf8(struCnfp.sCurModelName[7][k]));
                    }
                    data[64]= struGsh.nIpcLoadCnt;
                }
            }
        }
    }

    myFlow.msleep(10);
    for(int j = 0; j < struCnfg.struLayerInfo[0].nViewChainTotal; j++)
    {
        for(int k = 0; k < struCnfg.struLayerInfo[0].nViewUnitTotal[j]; k++)
        {
            struIpcShare.struIpcInfo[j][k].modelStat  = 1;
        }
    }
}

// 05 配置相机参数 (发给相机板)
void GlobalFlow::configIpcCamera()
{
    int i, j, k, m,curPixels,quo,begin,end1,end2;
    unsigned char data[16];
    memset(data, 0, 16);

    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(struCnfg.struLayerInfo[i].nViewIsUsed[j] == 1)
            {
                for(k = 0; k < struCnfg.struLayerInfo[i].nViewUnitTotal[j]; k++)
                {
                    if(struCnfg.struIpcCameraParams[i][j][k].ipcType != IPC_CAMERA_TYPE_NULL)
                    {
                        memset(data, 0, 16);
                        curPixels = getSensorPixels(i, j);
                        quo = (curPixels > 1024)?2:1;

                        if(struCnfp.nSensResizeMode[i][j] == 0){//1024 不压缩
                            begin = qRound(struCnfg.struLayerInfo[i].stuCameraInfo[j].nChannelBegin[k]/(float)quo);
                            end1  = qRound(struCnfg.struLayerInfo[i].stuCameraInfo[j].nChannelEnd[k]/(float)quo);
                            end2  = qRound(struCnfg.struLayerInfo[i].stuCameraInfo[j].nChannelEnd2[k]/(float)quo);
                            if(end2 >=1024)
                                end2 = 1023;
                        }else{
                            begin = qRound((struCnfg.struLayerInfo[i].stuCameraInfo[j].nChannelBegin[k])*3.0/4/quo);
                            end1  = qRound((struCnfg.struLayerInfo[i].stuCameraInfo[j].nChannelEnd[k])*3.0/4/quo);
                            if(end1 >=768)
                                end1 = 767;
                        }

                        struCnfg.struIpcCameraParams[i][j][k].nHoriOffset =begin;
                        struCnfg.struIpcCameraParams[i][j][k].nHoriWidth
                                = end1-begin+1;
                        if(end2 > end1){
                            struCnfg.struIpcCameraParams[i][j][k].nHoriWidth_2
                                    = end2-end1+1;
                        }
                        else{
                            struCnfg.struIpcCameraParams[i][j][k].nHoriWidth_2 = 0;
                        }

                        data[0] = struCnfg.struIpcCameraParams[i][j][k].nHoriOffset/256;
                        data[1] = struCnfg.struIpcCameraParams[i][j][k].nHoriOffset%256;
                        data[2] = struCnfg.struIpcCameraParams[i][j][k].nHoriWidth/256;
                        data[3] = struCnfg.struIpcCameraParams[i][j][k].nHoriWidth%256;
                        data[4] = struCnfg.struIpcCameraParams[i][j][k].nHoriWidth_2/256;
                        data[5] = struCnfg.struIpcCameraParams[i][j][k].nHoriWidth_2%256;

                        g_Udp.pushUdpCmdData(CMD_UDP_IPC_REQ_CAMERA_PARAMS, true, j, k, 16, data);
                    }
                }
            }
        }
    }
}

// 06 配置算法信息，灵敏度&阈值
void GlobalFlow::configIpcClassParams(bool isall, int view,int unit)
{
    int i, j, k, t, m, n;
    int nUnitAddr = 0;
    int tmpIp = 1;
    int err = 0;
    int limit1 = 0;
    int limit2 = 600;
    int tempLim = 0;
    float ratio = 1.0;
    int tdmode;
    unsigned char data[512];

    memset(data, 0, 512);
    struGsh.bUpdateState = 1;

    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(struCnfg.struLayerInfo[i].nViewIsUsed[j] == 1)
            {
                if(struCnfp.nSensorPixel[i][j] == SENSOR_PIXELS_1024_TD
                        || struCnfp.nSensorPixel[i][j] == SENSOR_PIXELS_2048_TD
                        || struCnfp.nSensorPixel[i][j] == SENSOR_PIXELS_2048_XTD)
                {
                    tdmode = 1;//可见
                }
                else
                    tdmode = 0;
                for(k = 0; k <struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal; k++)
                {
                    for(t = 0; t < struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; t++) // 认为视的次数和吹的序号一致
                    {
                        nUnitAddr = (getIdentifyGroupAddr(i, j, k, t))%64;

                        if(!isall && (view != j || unit != nUnitAddr)) continue;
                        if(struCnfg.struIpcCameraParams[i][j][nUnitAddr].ipcType == IPC_CAMERA_TYPE_NULL) continue;

                        data[4] = 255;  //22/5/2 添加红外和黑白背景，及饱和度
                        switch(struCnfp.nBackgroundColor[i][j][tdmode])
                        {
                        case BACKGROUND_COLOR_RED:
                        case BACKGROUND_COLOR_GREEN:
                        case BACKGROUND_COLOR_BLUE:
                            data[1] = struCnfp.nBackgroundColor[i][j][tdmode];
                            data[2] = struCnfp.nBackgroundColorProLow[i][j][tdmode];
                            data[3] = struCnfp.nBackgroundColorValueLow[i][j][tdmode];
                            break;
                        case BACKGROUND_COLOR_BLACK_WHITE:
                            data[1] = 4;             //黑白与红外颠倒数值索引
                            data[2] = struCnfp.nBackgroundColorValueUp[i][j][tdmode];
                            data[3] = struCnfp.nBackgroundColorValueLow[i][j][tdmode];
                            data[4] = struCnfp.nBackgroundColorSatUp[i][j][tdmode];
                            break;
                        case BACKGROUND_COLOR_NIR:  //黑白与红外颠倒数值索引
                            data[1] = 3;
                            data[2] = struCnfp.nBackgroundColorValueUp[i][j][tdmode];
                            data[3] = struCnfp.nBackgroundColorValueLow[i][j][tdmode];
                            break;
                        default:
                            data[1] = struCnfp.nBackgroundColor[i][j][tdmode];
                            data[2] = struCnfp.nBackgroundColorProLow[i][j][tdmode];
                            data[3] = struCnfp.nBackgroundColorValueLow[i][j][tdmode];
                            break;
                        }

                        struIpcShare.struIpcClassParamsReqStauts[j][nUnitAddr] = IPC_REQ_RETURN_NULL;

                        n = k;
                        data[0]  = 0;
                        qDebug()<<t<<struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nUnitGupSplice[t];
                        if(struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nUnitGupSplice[t] == 1)
                        {
                            if(j%2 == 0) //22/4/26 前主小端发送二次的参数，但地址仍然是小端
                            {
                                data[0] |= 1<<6;
                                n = k+1;
                            }
                            else
                            {
                                data[0] |= 1<<6;
                                n = k;
                            }
                        }
                        else if(struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nUnitGupSplice[t] == 2)
                        {
                            if(j%2 == 0)
                            {
                                data[0] |= 1<<7;
                                n = k-1;
                            }
                            else
                            {
                                data[0] |= 1<<7;
                                n = k;
                            }
                        }
                        //22/5/2 长度限制改为3个字节
                        data[5] = (struCnfp.struCameraIpcClassGroupParams[j][n].lengthLimit >>16) & 0xff;
                        data[6] = (struCnfp.struCameraIpcClassGroupParams[j][n].lengthLimit >>8)  & 0xff;
                        data[7] = (struCnfp.struCameraIpcClassGroupParams[j][n].lengthLimit >>0)  & 0xff;

                        data[12] = struCnfp.ipcModelClassCount[j][n];

                        for(m = 0; m < struCnfp.ipcModelClassCount[j][n]; m++)
                        {
                            if(m > 21) break;
                            //                            qDebug()<<"view"<<j<<"group"<<n<<"unit"<<nUnitAddr<<"model"<<m<<"checkd"<<struCnfp.struCameraIpcClassGroupParams[j][n].classEnable[m];
                            if(struCnfp.struCameraIpcClassGroupParams[j][n].classEnable[m] == 1)
                            {
                                data[13+m*19] = struCnfp.struCameraIpcClassGroupParams[j][n].classSens[m];
                            }
                            else
                            {
                                data[13+m*19] = 0;
                            }

                            data[13+m*19+1] = struCnfp.struCameraIpcClassGroupParams[j][n].classThreshold[m];//阈值
                            data[13+m*19+2] = struCnfp.struCameraIpcClassGroupParams[j][n].classLabelBox[m]/256;//剔除框1
                            data[13+m*19+3] = struCnfp.struCameraIpcClassGroupParams[j][n].classLabelBox[m]%256;
                            data[13+m*19+4] = (struCnfp.struCameraIpcClassGroupParams[j][n].classFixMode[m]<<1)//剔除模式
                                    |(struCnfp.struCameraIpcClassGroupParams[j][n].classDelayMode[m]<<2)
                                    |(struCnfp.struCameraIpcClassGroupParams[j][n].classTickMode[m]);
                            data[13+m*19+5] = struCnfp.struCameraIpcClassGroupParams[j][n].classMode[m];//剔除算法
                            data[13+m*19+6] = struCnfp.struCameraIpcClassGroupParams[j][n].classLabelBox2[m]/256;//剔除框2
                            data[13+m*19+7] = struCnfp.struCameraIpcClassGroupParams[j][n].classLabelBox2[m]%256;
                            data[13+m*19+8] = struCnfp.struCameraIpcClassGroupParams[j][n].classHWRatio[m];//高宽比阈值
                            data[13+m*19+9] = struCnfp.struCameraIpcClassGroupParams[j][n].classPosAdjust[m];//坐标调整
                            data[13+m*19+10] = struCnfp.struCameraIpcClassGroupParams[j][n].classBD[m];//BD
                            data[13+m*19+11] = struCnfp.struCameraIpcClassGroupParams[j][n].classBX[m]/256;//BX
                            data[13+m*19+12] = struCnfp.struCameraIpcClassGroupParams[j][n].classBX[m]%256;

                            data[13+m*19+13] = struCnfp.struCameraIpcClassGroupParams[j][n].selRerverse[m];
                            switch(struCnfp.struCameraIpcClassGroupParams[j][n].classMode[m])
                            {
                            case 0:
                                ratio = 0;
                                break;
                            case 1:
                                ratio = 15;
                                break;
                            case 2:
                                ratio = 225.0/128;
                                break;
                            case 3:
                                ratio = 32;
                                break;
                            case 4:
                                ratio = 15;
                                break;
                            default:
                                ratio = 15;
                                break;
                            }
                            limit1 = (int)(struCnfp.struCameraIpcClassGroupParams[j][n].selLimit1[m]*ratio);
                            limit2 = (int)(struCnfp.struCameraIpcClassGroupParams[j][n].selLimit2[m]*ratio);
                            if(limit1 > limit2){
                                tempLim = limit1;
                                limit1 = limit2;
                                limit2 = tempLim;
                            }
                            data[13+m*19+14] = limit1/256;
                            data[13+m*19+15] = limit1%256;
                            data[13+m*19+16] = limit2/256;
                            data[13+m*19+17] = limit2%256;
                        }
                        g_Udp.pushUdpCmdData(CMD_UDP_IPC_REQ_ARITH_PARAMS, true, j, nUnitAddr, struCnfp.ipcModelClassCount[j][n]*19+13, data);
                        if(struGsh.bSortStart)
                            myFlow.msleep(2);
                    }
                    //                    mydebug(data,16);
                }
            }
        }
    }
    struGsh.bUpdateState = 0;
}

// 07 配置AI
void GlobalFlow::configIpcAIPara(int view,int unit)
{
    unsigned char data[16];
    //    memset(data, 0, 16);
    //    data[0] = struCnfp.nIpcRoiRepeat/255;
    //    data[1] = struCnfp.nIpcRoiRepeat%255;
    //    g_Udp.pushUdpCmdData(CMD_UDP_IPC_CFG_CONFIG_FILE, true, 0x09,0xff, 16, data);

    //    struCnfp.nIpcRoiRepeat[struGsh.nView] = ((int)struCnfp.nIpcRoiRepeatf[struGsh.nView]) == 0?(struCnfp.nIpcRoiHeight[struGsh.nView]/4):(struCnfp.nIpcRoiRepeatf[struGsh.nView]*struCnfp.nIpcRoiHeight[struGsh.nView]);

    for(int i = 0;i < MAX_VIEW; i++){
        struCnfp.nIpcRoiRepeat[i] = (struCnfp.nIpcRoiRepeatf[i] == 0)?(struCnfp.nIpcRoiHeight[i]/4):(struCnfp.nIpcRoiRepeatf[i]*struCnfp.nIpcRoiHeight[i]);
    }

    for(int i = 0;i < struCnfg.struLayerInfo[0].nViewChainTotal;i++){
        if(struCnfg.struLayerInfo[0].nViewIsUsed[i] == 1) {
            memset(data, 0, 16);
            data[0] = struCnfp.nIpcRoiRepeat[i]/256;
            data[1] = struCnfp.nIpcRoiRepeat[i]%256;
            data[2] = struCnfp.nIpcRoiRepeat2[i];
            g_Udp.pushUdpCmdData(CMD_UDP_IPC_CFG_CONFIG_FILE, true, i,0xff, 16, data);
            //            qDebug()<<i<<struCnfp.nIpcRoiRepeat[i];
        }
    }
}

// 08 检测开关
void GlobalFlow::configIpcSortStart(int stat)
{
    if(!struCnfg.nUseIpcEnable) return;

    unsigned char data[1];
    data[0] = stat;
    int i, j, k;

    g_Udp.pushUdpCmdData(CMD_UDP_IPC_REQ_START_SORT, true, 0x09, 0xff, 1, data);
}

// 09 关机/重启/清除标识(stat: 0/1/0xff)
void GlobalFlow::shutdownIpc(bool bIsAll,int view,int unit, int stat)
{
    unsigned char data[1];
    data[0] = stat;
    int i, j, k;

    if (bIsAll == true )
    {
        g_Udp.pushUdpCmdData(CMD_UDP_IPC_REQ_SHUTDOWN, true, 0x09, 0xff, 1, data);
    }
    return;

    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(struCnfg.struLayerInfo[i].nViewIsUsed[j] == 1)
            {
                if (bIsAll == true && stat == 1) //重启进入离线状态，心跳加速
                {
                    for(k = 0; k <struCnfg.struLayerInfo[i].nViewUnitTotal[j]; k++)
                    {
                        struIpcShare.struIpcInfo[j][k].restartStat = 1;
                        //                        struIpcShare.struIpcInfo[j][k].aliveRetryCnt = 100;
                        struIpcShare.struIpcInfo[j][k].modelStat = 0;
                        struIpcShare.struIpcInfo[j][k].modelUploadStat = 0;
                    }
                }
                else
                {
                    for(k = 0; k <struCnfg.struLayerInfo[i].nViewUnitTotal[j]; k++)
                    {
                        if (view != j || unit !=k) continue;
                        g_Udp.pushUdpCmdData(CMD_UDP_IPC_REQ_SHUTDOWN, true, j, k, 1, data);
                        if(stat == 1) //重启进入离线状态，心跳加速
                        {
                            struIpcShare.struIpcInfo[j][k].restartStat = 1;
                            //                            struIpcShare.struIpcInfo[j][k].aliveRetryCnt = 100;
                            struIpcShare.struIpcInfo[j][k].modelStat = 0;
                            struIpcShare.struIpcInfo[j][k].modelUploadStat = 0;
                        }
                    }
                }
            }
        }
    }
    if(stat == 1){
        //        struGsh.bUpdateState = 1;
        sleep(3);
        //        struGsh.bUpdateState = 0;
    }
}

// 0A 查询含杂
void GlobalFlow::getIpcCamBadPointsCount()
{
    unsigned char data[16];
    memset(data, 0, 16);
    data[0] = 1; // 标志
    int i, j, k;
    memset(struGsh.camBadPoiontRet,0,sizeof(struGsh.camBadPoiontRet));
    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(struCnfg.struLayerInfo[i].nViewIsUsed[j] == 1)
            {
                for(k = 0; k <struCnfg.struLayerInfo[i].nViewUnitTotal[j]; k++)
                {
                    if(struCnfg.struIpcCameraParams[i][j][k].ipcType != IPC_CAMERA_TYPE_NULL)
                    {
                        g_Udp.pushUdpCmdData(CMD_UDP_IPC_REQ_BAD_MATERIAL_COUNTS_2, true, j, k, 16, data);
                        qDebug()<<"push CMD_UDP_IPC_REQ_BAD_MATERIAL_COUNTS_2";
                        msleep(20);
                    }
                }
            }
        }
    }

}
void GlobalFlow::setIpcCamOverRatio(int isall,int layer,int view,  int unit)
{

    unsigned char data[16];
    int i, j, k;
    memset(data, 0, 16);

    // struCnfg.areaThres = 0;
    data[0] = struCnfg.overRatio; // 交并比设置
    data[1] = struCnfg.areaThres/256; // 面积阈值
    data[2] = struCnfg.areaThres%256; // 面积阈值
    if(isall)
    {
        for(i = 0; i < struCnfg.nLayerTotal; i++)
        {
            for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
            {
                if(struCnfg.struLayerInfo[layer].nViewIsUsed[j] == 1)
                {
                    for(k = 0; k <struCnfg.struLayerInfo[layer].nViewUnitTotal[j]; k++)
                    {
                        if(struCnfg.struIpcCameraParams[layer][j][k].ipcType != IPC_CAMERA_TYPE_NULL)
                        {
                            data[3] = struCnfg.overRatioTime/256; // 交并比等待时间
                            data[4] = struCnfg.overRatioTime%256; // 交并比等待时间
                            g_Udp.pushUdpCmdData(CMD_UDP_IPC_REQ_BAD_MATERIAL_PARAMS, true, j, k, 16, data);
                            msleep(10);
                        }
                    }
                }
            }
        }
    }
    else
    {
        struCnfg.overRatioTime = 1000.0 * (2*struIpcShare.struIpcInfo[view][unit].roiHeight) /struIpcShare.struIpcInfo[view][unit].lineFreq;
        if(struCnfg.overRatioTime >500)
            struCnfg.overRatioTime = 500;
        data[3] = struCnfg.overRatioTime/256; // 交并比等待时间
        data[4] = struCnfg.overRatioTime%256; // 交并比等待时间
        g_Udp.pushUdpCmdData(CMD_UDP_IPC_REQ_BAD_MATERIAL_PARAMS, true, view, unit, 16, data);
    }
}

// 0B 清零含杂
void GlobalFlow::clearIpcCamBadPointsCount()
{
    int i, j, k;

    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(struCnfg.struLayerInfo[i].nViewIsUsed[j] == 1)
            {
                g_Udp.pushUdpCmdData(CMD_UDP_IPC_CLEAR_BAD_MATERIAL_COUNTS, true, j, 0xff, 0, NULL);
            }
        }
    }
}
// 0F 模型配置
void GlobalFlow::modelConfigure(int view, int unit)
{
    //    int i, j, k;
    //    unsigned char data[16];
    //    memset(data, 0, 16);

    //    data[0] = struCnfp.nIpcRoiHeight[view]/256;
    //    data[1] = struCnfp.nIpcRoiHeight[view]%256;
    //    data[2] = struCnfp.nIpcRoiWidth[view]/256;
    //    data[3] = struCnfp.nIpcRoiWidth[view]%256;

    //    g_Udp.pushUdpCmdData(CMD_UDP_IPC_MODEL_CONFIG, true, 0x09, 0xff, 4, data);
}

// 10 删除模型
void GlobalFlow::deleteIpcmodel(bool bIsAll, int view, char*modelName)
{
    int i, j, k;
    unsigned char data[MAX_IPC_MODEL_NAME_LENGTH];
    memset(data, 0, MAX_IPC_MODEL_NAME_LENGTH);

    memcpy(data, modelName, strlen(modelName));
    memcpy(&data[strlen(modelName)],".*",strlen(".*"));

    unsigned char cmd[128] = "rm -f /data/ACS/model/";
    strcat((char*)cmd,(char*)data);
    printf("%s\n",cmd);

    for(int i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(int j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(struCnfg.struLayerInfo[i].nViewIsUsed[j] == 1)
            {
                if(struCnfg.nXsystemBoard[j] == 0)  //RK
                {
                    for(int k = 0; k <struCnfg.struLayerInfo[i].nViewUnitTotal[j]; k++)
                    {
                        if(struCnfg.struIpcCameraParams[i][j][k].ipcType != IPC_CAMERA_TYPE_NULL)
                        {
                            struIpcShare.struIpcReqStauts[j][k].finshShellCmdReq = IPC_REQ_RETURN_NULL;
                            g_Udp.pushUdpCmdData(CMD_SCREEN_SHELL_CMD, true, j, k, sizeof(cmd), cmd );
                            for(int m =0 ; m <struCnfg.nWaitTimeout/10+1; m++){
                                msleep(10);
                                if(struIpcShare.struIpcReqStauts[j][k].finshShellCmdReq ==1)
                                    break;
                            }
                        }
                    }
                }
            }
        }
    }
}

// 10 删除X系统模型
void GlobalFlow::deleteXsysmodel(bool bIsAll, int view, char*modelName)
{
    int i, j, k;
    unsigned char data[MAX_IPC_MODEL_NAME_LENGTH];
    memset(data, 0, MAX_IPC_MODEL_NAME_LENGTH);

    qDebug()<<"strlen(modelName)"<<strlen(modelName)<<sizeof(modelName);

    memcpy(data, modelName, strlen(modelName));
    //    memcpy(&data[strlen(modelName)],".*",strlen(".*"));

    //memcpy(data, modelName, sizeof(modelName));


    for(int i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(int j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(struCnfg.struLayerInfo[i].nViewIsUsed[j] == 1)
            {
                if(struCnfg.nXsystemBoard[j] == 1)  //X System
                {
                    for(int k = 0; k <struCnfg.struLayerInfo[i].nViewUnitTotal[j]; k++)
                    {
                        if(struCnfg.struIpcCameraParams[i][j][k].ipcType != IPC_CAMERA_TYPE_NULL)
                        {
                            qDebug()<<("ccc")<<j<<k;
                            struIpcShare.struIpcReqStauts[j][k].finshShellCmdReq = IPC_REQ_RETURN_NULL;
                            //                            g_Udp.pushUdpCmdData(CMD_SCREEN_SHELL_CMD, true, j, k, sizeof(cmd), cmd );
                            g_Udp.pushUdpCmdData(CMD_UDP_IPC_MODEL_DELETE, true, j, k, MAX_IPC_MODEL_NAME_LENGTH, data);
                            msleep(50);
                            for(int m =0 ; m <struCnfg.nWaitTimeout/10+1; m++){
                                msleep(10);
                                if(struIpcShare.struIpcReqStauts[j][k].finshShellCmdReq ==1)
                                    break;
                            }

                        }
                    }
                }
            }
        }
    }
    //    qDebug()<<("????????");
}

/* 颜色还原使能 */
void GlobalFlow::colorRestoreEnable()
{
    int i, j, k;
    int nInt=0, board, nUnitAddr;
    char data[SEND_PACKET_DATA_SIZE];

    /* 整机使能相同 */
    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
            memset(data, 0, SEND_PACKET_DATA_SIZE);
            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
            data[0] = struCnfp.bColorRestoreEnable;
            mySerial.pushCom1CmdData(CMD_COLOR_RESTORE_ENABLE, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        }
    }
}

/* 发送IP地址 */
void GlobalFlow::ipcResetCameraIpaddr(bool bIsAll, int nLayerId, int nViewId, int nUnit)
{
    int i, j, k;
    int nInt=0, board, nUnitAddr;
    char data[SEND_PACKET_DATA_SIZE];

    memset(data, 0, SEND_PACKET_DATA_SIZE);

    if(bIsAll == true)
    {
        for(i = 0; i < struCnfg.nLayerTotal; i++)  //层
        {
            for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)  // 视
            {
                if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
                board = struCnfg.struLayerInfo[i].nViewBoardType[j];
                for(k = 0; k < struCnfg.struLayerInfo[i].nViewUnitTotal[j]; k++) // 相机单元
                {
                    nUnitAddr = struCnfg.struLayerInfo[nInt].stuViewInfo[nViewId].nViewUnitId[k];

                    if(struCnfg.struIpcCameraParams[i][j][nUnitAddr].ipcType != IPC_CAMERA_TYPE_NULL)
                    {
                        memset(data, 0, SEND_PACKET_DATA_SIZE);

                        data[0] = struCnfg.struIpcCameraParams[i][j][nUnitAddr].nIpAddr[0];
                        data[1] = struCnfg.struIpcCameraParams[i][j][nUnitAddr].nIpAddr[1];
                        data[2] = struCnfg.struIpcCameraParams[i][j][nUnitAddr].nIpAddr[2];
                        data[3] = struCnfg.struIpcCameraParams[i][j][nUnitAddr].nIpAddr[3];

                        mySerial.pushCom1CmdData(CMD_IPC_CAMERA_IP_SET, nInt, board, nUnitAddr, data, 3);
                    }
                }
            }
        }
    }
    else
    {
        board = struCnfg.struLayerInfo[nInt].nViewBoardType[nViewId];
        nUnitAddr = struCnfg.struLayerInfo[nInt].stuViewInfo[nViewId].nViewUnitId[nUnit];

        data[0] = struCnfg.struIpcCameraParams[nLayerId][nViewId][nUnit].nIpAddr[0];
        data[1] = struCnfg.struIpcCameraParams[nLayerId][nViewId][nUnit].nIpAddr[1];
        data[2] = struCnfg.struIpcCameraParams[nLayerId][nViewId][nUnit].nIpAddr[2];
        data[3] = struCnfg.struIpcCameraParams[nLayerId][nViewId][nUnit].nIpAddr[3];

        mySerial.pushCom1CmdData(CMD_IPC_CAMERA_IP_SET, nInt, board, nUnitAddr, data, 3);
    }
}

/* 延迟时间缓存行数 */
void GlobalFlow::setIpcDelayBufRowCount()
{
    int i, j;
    int nInt=0, board;
    char data[SEND_PACKET_DATA_SIZE];

    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
            memset(data, 0, SEND_PACKET_DATA_SIZE);
            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
            data[0] = struCnfp.nIpcDelayBufRowCount[i][j]/256;
            data[1] = struCnfp.nIpcDelayBufRowCount[i][j]%256;

            mySerial.pushCom1CmdData(CMD_DELAY_BUF_ROW_COUNT, 0, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        }
    }
}

/* 颜色还原 */
void GlobalFlow::setCameraColorRestore(int color)
{
    int i, j, k;
    int nInt=0, board;
    char data[SEND_PACKET_DATA_SIZE];


    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
            memset(data, 0, SEND_PACKET_DATA_SIZE);
            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
            data[0] = color;
            switch(color)
            {
            case 0:
                for(k = 0; k < 3; k++)
                {
                    if(struCnfg.struLayerInfo[i].stuCameraInfo[j].nColorRestoreRed[k] >= 0)
                    {
                        data[k*2+1] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nColorRestoreRed[k]/256;
                        data[k*2+2] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nColorRestoreRed[k]%256;
                    }
                    else
                    {
                        data[k*2+1] = (-1*struCnfg.struLayerInfo[i].stuCameraInfo[j].nColorRestoreRed[k])/256|0x80;
                        data[k*2+2] = (-1*struCnfg.struLayerInfo[i].stuCameraInfo[j].nColorRestoreRed[k])%256;
                    }
                }
                break;
            case 1:
                for(k = 0; k < 3; k++)
                {
                    if(struCnfg.struLayerInfo[i].stuCameraInfo[j].nColorRestoreGreen[k] >= 0)
                    {
                        data[k*2+1] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nColorRestoreGreen[k]/256;
                        data[k*2+2] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nColorRestoreGreen[k]%256;
                    }
                    else
                    {
                        data[k*2+1] = (-1*struCnfg.struLayerInfo[i].stuCameraInfo[j].nColorRestoreGreen[k])/256|0x80;
                        data[k*2+2] = (-1*struCnfg.struLayerInfo[i].stuCameraInfo[j].nColorRestoreGreen[k])%256;
                    }
                }
                break;
            case 2:
                for(k = 0; k < 3; k++)
                {
                    if(struCnfg.struLayerInfo[i].stuCameraInfo[j].nColorRestoreBlue[k] >= 0)
                    {
                        data[k*2+1] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nColorRestoreBlue[k]/256;
                        data[k*2+2] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nColorRestoreBlue[k]%256;
                    }
                    else
                    {
                        data[k*2+1] = (-1*struCnfg.struLayerInfo[i].stuCameraInfo[j].nColorRestoreBlue[k])/256|0x80;
                        data[k*2+2] = (-1*struCnfg.struLayerInfo[i].stuCameraInfo[j].nColorRestoreBlue[k])%256;
                    }
                }
                break;
            }

            mySerial.pushCom1CmdData(CMD_IPC_CAMERA_COLOR_RESTORE, 0, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        }
    }
}

void GlobalFlow::resetIpcParams(int view,int unit)
{
    int ipcRoiCnt = 0;
    int m_unit = 0;

    if(struIpcShare.allIpcModelLoadOk == 1) return;
    if(view == 0xff && unit == 0xff)
    {
        //前主第一个可正常使用的rk
        for(m_unit = 0;m_unit<struCnfg.struLayerInfo[0].nViewUnitTotal[0];m_unit++)
        {
            if(struCnfg.struIpcCameraParams[0][0][m_unit].ipcType != IPC_CAMERA_TYPE_NULL)
                break;
        }
        view = 0,unit = m_unit;
        if(struCnfg.nUseIpcEnable == 1 && struCnfp.nHasSaveModel == 1 && struCnfg.nDPMachine == 1)
        {
            logger()->info("load model start");
            struIpcShare.allIpcModelLoadOk = 1;
            struGsh.bKeepAliveType = 1;
            configIpcSortStart(struGsh.bSortStart);
            loadIpcModel(view,unit, 0);
            //模型加载完，查询到类别信息后，可以建表，并对ipc含杂清理
            //            if(struIpcShare.struIpcReqStauts[view][unit].modelInfoReq == 1 && struCnfg.nAutoStaticsEnable == 1 )
            //            {
            //                QByteArray modelName(struCnfp.sCurModelName[view][0]);
            //                g_Runtime().modelFullName = QString(modelName);
            //                g_Runtime().modelFullNameList.clear();
            //                for(int i  =0;i<struIpcShare.struIpcModelAble[view][unit].modelCount;i++)
            //                {
            //                    QByteArray modelName(struIpcShare.struIpcModelAble[view][unit].modelInfo[i].modelname);
            //                    g_Runtime().modelFullNameList.append(modelName);
            //                }
            //                MySqlite mysql;
            //                mysql.clearTable(g_Runtime().modelFullName);
            //                mysql.creatTable(g_Runtime().modelFullName);
            //                mysql.getCurItemInfo(g_Runtime().modelFullName);
            //加载完新模型需要清0
            //                myFlow.clearIpcCamBadPointsCount();
            //            }
            ipcRoiCnt = 2;
            for(int i  = 0; i < 200; i++)
            {
                msleep(100);
                for(int j = 0; j < struCnfg.struLayerInfo[0].nViewChainTotal; j++)
                {
                    if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
                    for(int k = 0; k < struCnfg.struLayerInfo[0].nViewUnitTotal[j]; k++)
                    {
                        if(struCnfg.struIpcCameraParams[0][j][k].ipcType != IPC_CAMERA_TYPE_NULL)
                        {
                            ipcRoiCnt = struIpcShare.struIpcInfo[j][k].modelStat ;
                            //                            logger()->info("----------view%1-unit%2-loadStat%3",j,k, struIpcShare.struIpcInfo[j][k].modelStat);
                            //0-未加载模型, 1-正在加载, 2-加载成功,3-加载失败－文件不存在,4-加载失败，模型文件错误，5-加载失败－色选状态不匹配
                            switch (struIpcShare.struIpcInfo[j][k].modelStat ) {
                            case 0:
                            case 1:
                            case 3:
                            case 4:
                            case 5:
                            case 6:
                                goto end;
                                break;
                            default:
                                break;
                            }
                        }
                    }
                }
end:
                if(ipcRoiCnt == 2)
                    break;
#ifdef Q_OS_UNIX
                //myMqttMsgParaseThread->n_uploadPartsStatusFlag = (struIpcShare.allIpcModelLoadOk  == 2 ? 1: 0);
#endif
            }
        }
    }
    struIpcShare.allIpcModelLoadOk = ipcRoiCnt;
    struGsh.bKeepAliveType = 0;

    if(struIpcShare.allIpcModelLoadOk == 2){

        myFlow.resetCameraResizeMode();
        myFlow.getIpcModelEnable(true,0,0);
        myFlow.msleep(100);
        myFlow.getIpcModelInfo(true,0,0);
        myFlow.msleep(100);
        myFlow.configIpcCamera();//发送像元划分参数
        myFlow.msleep(100);
        myFlow.configIpcClassParams(true,0,0);
        myFlow.msleep(100);
        myFlow.IpcClassResultANDOperat(true,0,0);
        myFlow.configIpcAIPara(0,0);
        myFlow.setIpcCamOverRatio(true,0,0,0);
#ifdef Q_OS_UNIX
//        myMqttMsgParaseThread->n_uploadModelParamsFlag  = 1;
#endif

        if(struCnfg.nAutoStaticsEnable == 1)
        {
            QByteArray modelName(struCnfp.sCurModelName[view][0]);
            g_Runtime().modelFullName = QString(modelName);
            logger()->info("create table %1",g_Runtime().modelFullName);
            g_Runtime().modelFullNametest = QString(modelName) + "_Count";
            g_Runtime().modelFullName_Area = QString(modelName) + "_Area";
            g_Runtime().modelFullName_Frame = QString(modelName) + "_Frame";
            g_Runtime().modelFullNameList.clear();
            MySqlite mysql;
            for(int i = 0; i < struCnfg.struLayerInfo[0].nViewChainTotal; i++)
            {
                if(struCnfg.struLayerInfo[0].nViewIsUsed[i] == 1)
                {
                    for(int j = 0; j <struCnfg.struLayerInfo[0].nViewUnitTotal[i]; j++)
                    {
                        if(struCnfg.struIpcCameraParams[0][i][j].ipcType != IPC_CAMERA_TYPE_NULL)
                        {

                            mysql.creatstaticTable(g_Runtime().modelFullNametest + QString("_%1_%2").arg(i).arg(j),i, j);
                            mysql.creatstaticTable(g_Runtime().modelFullName_Area + QString("_%1_%2").arg(i).arg(j),i, j);
                            mysql.creatstaticTable(g_Runtime().modelFullName_Frame + QString("_%1_%2").arg(i).arg(j),i, j);
                        }
                    }
                }
            }
        }

    }
    else
    {
        for(int j = 0; j < struCnfg.struLayerInfo[0].nViewChainTotal; j++)
        {
            QString loadres = "view"+QString::number(j)+" load status: ";
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
            for(int k = 0; k < struCnfg.struLayerInfo[0].nViewUnitTotal[j]; k++)
            {
                loadres.append(QString::number(struIpcShare.struIpcInfo[j][k].modelStat)+" ");
            }
            logger()->info("%1",loadres);
        }
    }
    logger()->info("load model end:%1",ipcRoiCnt);
    printf("load---------------------%d\n",struIpcShare.allIpcModelLoadOk);
    return;
}

#if 0
// 通过ipc camid 获取对应视、通道信息
int GlobalFlow::getIpcCameraViewInfo(int &view, int &unit,int ipcIndex, int ipcCamId)
{
    int boardType[MAX_VIEW]={2,4,3,5,0x13,0x14,0x2f,0x30};

    if (ipcIndex < 0 || ipcIndex >struCnfg.nIpcCount)
        return false;
    for(int i = 0; i < MAX_VIEW;i++){
        if (struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[i]){
            for(int j = 0; j < struCnfg.nChuteTotal;j++){
                if (struCnfg.struIpcCameraParams[struGsh.nLayer][i][j].nIpAddr[3] == ipcCamId+1 &&
                        struCnfg.struIpcCameraParams[struGsh.nLayer][i][j].ipcType == ipcIndex+1 ){
                    view = boardType[i];
                    unit = j+1;
                    return true;
                }
            }
        }
    }
    return false;
}
#endif

// 通过ipc camid 获取对应视、通道信息
int GlobalFlow::getIpcCameraViewUnitInfo(int &view, int &unit, int&ipId, QString Ip)
{
    QStringList ipList = Ip.split(".");
    if(ipList.length() == 4){
        ipId = ipList.last().toInt() -1;
    }
    else
        return false;
    if(ipId >= 95)
        return false;
    for(int i = 0; i < MAX_VIEW;i++){
        if (struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[i]){
            for(int j = 0; j < struCnfg.struLayerInfo[0].nViewUnitTotal[i]; j++){
                if (struCnfg.struIpcCameraParams[struGsh.nLayer][i][j].nIpAddr[3] == ipId+1 &&
                        struCnfg.struIpcCameraParams[struGsh.nLayer][i][j].ipcType != IPC_CAMERA_TYPE_NULL ){
                    view = i;
                    unit = j;
                    return true;
                }
            }
        }
    }
    return false;
}
// 通过视、通道信息获取对应ipc索引
int GlobalFlow::getIpcIndexByViewInfo(int view, int unit)
{
    if (view > 5 || unit < 1) return -1;
    QMap<int,int> map;
    map.insert(2,0);
    map.insert(3,1);
    map.insert(4,2);
    map.insert(5,3);

    for(int i = 0; i < struCnfg.nIpcCount; i++)
    {
        if (struCnfg.struIpcCameraParams[struGsh.nLayer][map.value(view)][unit-1].ipcType == i+1 ){
            return i;
        }
    }
}

void GlobalFlow::ipcRunTestStart(bool status)
{
    //    unsigned char data[16];
    //    memset(data, 0, 16);
    //    data[0] = status;

    //    for(int i = 0; i < struCnfg.nIpcCount; i++)
    //    {
    //        struIpcShare.struIpcReqStauts[i].ipcRunTestReq = IPC_REQ_RETURN_NULL;
    //        g_Udp.pushUdpCmdData(CMD_UDP_IPC_RUN_TEST, true, struCnfg.nIpcIpAddrBase+i, 0, 1, data);
    //   }
}

void GlobalFlow::ipcSaveImg(bool status)
{
#if 0

#endif
}
int GlobalFlow::getUpdateFileSize(QString fileName)
{
    if(fileName.contains("SIZE"))
    {
        QStringList vlist = fileName.split("SIZE");
        QString fileSizeType = vlist.at(1);
        QStringList vTmpLst = fileSizeType.split("_");
        QString fileType = vTmpLst.at(0);
        if(fileType == "0")
        {
            return 3550368;
        }
        else if(fileType == "1")
        {
            return 678656;
        }
        else if(fileType == "2")
        {
            return 6291456;
        }
        else if(fileType == "3")
        {
            return  12581888;
        }
    }
    else
    {
        return 3550368;
    }
}
//设置rk信息
void GlobalFlow::setRKInfo(bool bIsAll,int view,int unit,int type)
{
    int i, j, k, t, m, n;
    int nUnitAddr = 0;
    int tmpIp = 1;
    int err = 0;
    unsigned char data[256];

    QString cmd,cmd2;
    memset(data, 0, 256);

    if(bIsAll){
        switch (type) {
        case 0:  //date
            cmd = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            cmd2 = "date -s \""+cmd+"\"";
            g_Udp.pushUdpCmdData(CMD_SCREEN_GET_FILE, true, 0xff, 0xff, cmd2.length(),(unsigned char *) cmd2.toLocal8Bit().data());
            break;
        default:
            break;
        }
    }
    else{
        switch (type) {
        case 0:  //date
            cmd = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            cmd2 = "date -s \""+cmd+"\"";
            g_Udp.pushUdpCmdData(CMD_SCREEN_SHELL_CMD, true, view, unit, cmd2.length(),(unsigned char *) cmd2.toLocal8Bit().data());
            //            cmd2 = "killall zkLedShine";
            //            g_Udp.pushUdpCmdData(CMD_SCREEN_SHELL_CMD, true, view, unit, cmd2.length(),(unsigned char *) cmd2.toLocal8Bit().data());
            break;
        default:
            break;
        }
    }
}
// 0x5007 相机图像信息数据
void GlobalFlow::getCamImgInfo(bool isall, int view,int unit)
{
    if(isall){
        g_Udp.pushUdpCmdData(CMD_UDP_HT_GET_CAM_IMAGE_INFO, true, 0x09, 0xff, 0, NULL);
        return;
    }
    struIpcShare.struIpcReqStauts[view][unit].camImgInfoReq = IPC_REQ_RETURN_NULL;
    g_Udp.pushUdpCmdData(CMD_UDP_HT_GET_CAM_IMAGE_INFO, true, view, unit, 0, NULL);
}

void GlobalFlow::transformCameraOffset(int nLayerId, int nViewId, int nUnit)
{
    stru_view_camera_Info &cameraInfo = struCnfg.struLayerInfo[nLayerId].stuCameraInfo[nViewId];
    //R值
    cameraInfo.nNirRedSendOffsetX[nUnit] = -(cameraInfo.nNirGreenOffsetX[nUnit]/2);
    cameraInfo.nNirRedSendOffsetY[nUnit] = -(cameraInfo.nNirGreenOffsetY[nUnit]/2);
    //G值
    cameraInfo.nNirGreenSendOffsetX[nUnit] = cameraInfo.nNirGreenOffsetX[nUnit]/2;
    cameraInfo.nNirGreenSendOffsetY[nUnit] = cameraInfo.nNirGreenOffsetY[nUnit]/2;
    //B值
    cameraInfo.nNirBlueSendOffsetX[nUnit] = cameraInfo.nNirBlueOffsetX[nUnit]-(-cameraInfo.nNirRedSendOffsetX[nUnit]);
    cameraInfo.nNirBlueSendOffsetY[nUnit] = cameraInfo.nNirBlueOffsetY[nUnit]-(-cameraInfo.nNirRedSendOffsetY[nUnit]);
    //N值
    cameraInfo.nNirNSendOffsetX[nUnit] = cameraInfo.nNirNOffsetX[nUnit]-(-cameraInfo.nNirRedSendOffsetX[nUnit]);
    cameraInfo.nNirNSendOffsetY[nUnit] = cameraInfo.nNirNOffsetY[nUnit]-(-cameraInfo.nNirRedSendOffsetY[nUnit]);
}


void GlobalFlow::resetIrCameraOffset(bool bIsAll, int nLayerId, int nViewId, int nUnit)
{
    int i, j, k;
    int nInt=0, board, nUnitAddr;
    char data[SEND_PACKET_DATA_SIZE];



    //注：界面显示的偏移范围是-15至15，实际发送的是0-30
    if(bIsAll == true)
    {
        for(i = 0; i < struCnfg.nLayerTotal; i++)  //层
        {
            for(j = 0; j < MAX_VIEW; j++)  // 视
            {
                if(struCnfg.struLayerInfo[i].nViewIsUsed[j])
                {
                    board = struCnfg.struLayerInfo[i].nViewBoardType[j];
                    for(k = 0; k < struCnfg.struLayerInfo[i].nViewUnitTotal[j]; k++) // 相机单元
                    {
                        nUnitAddr = struCnfg.struLayerInfo[i].stuViewInfo[j].nViewUnitId[k];
                        transformCameraOffset(i,j,nUnitAddr);
                        switch(struCnfp.nSensorDim[i][j])
                        {
                        case 2:
                            memset(data, 0, SEND_PACKET_DATA_SIZE);

                            data[0] = 48 + struCnfg.struLayerInfo[i].stuCameraInfo[j].nNirGreenSendOffsetX[nUnitAddr];
                            data[1] = 48 + struCnfg.struLayerInfo[i].stuCameraInfo[j].nNirGreenSendOffsetY[nUnitAddr];
                            data[4] = 48 + struCnfg.struLayerInfo[i].stuCameraInfo[j].nNirRedSendOffsetX[nUnitAddr];
                            data[5] = 48 + struCnfg.struLayerInfo[i].stuCameraInfo[j].nNirRedSendOffsetY[nUnitAddr];
                            mySerial.pushCom1CmdData(CMD_CAMERA_IMG_SHIFT, nInt, board, nUnitAddr, data, 3);
                            break;
                        case 3:
                            memset(data, 0, SEND_PACKET_DATA_SIZE);
                            data[0] = 48 + struCnfg.struLayerInfo[i].stuCameraInfo[j].nNirGreenSendOffsetX[nUnitAddr];
                            data[1] = 48 + struCnfg.struLayerInfo[i].stuCameraInfo[j].nNirGreenSendOffsetY[nUnitAddr];
                            data[2] = 48 + struCnfg.struLayerInfo[i].stuCameraInfo[j].nNirBlueSendOffsetX[nUnitAddr];
                            data[3] = 48 + struCnfg.struLayerInfo[i].stuCameraInfo[j].nNirBlueSendOffsetY[nUnitAddr];
                            data[4] = 48 + struCnfg.struLayerInfo[i].stuCameraInfo[j].nNirRedSendOffsetX[nUnitAddr];
                            data[5] = 48 + struCnfg.struLayerInfo[i].stuCameraInfo[j].nNirRedSendOffsetY[nUnitAddr];
                            mySerial.pushCom1CmdData(CMD_CAMERA_IMG_SHIFT, nInt, board, nUnitAddr, data, 3);
                            break;
                        case 4:
                            memset(data, 0, SEND_PACKET_DATA_SIZE);
                            data[0] = 48 + struCnfg.struLayerInfo[i].stuCameraInfo[j].nNirGreenSendOffsetX[nUnitAddr];
                            data[1] = 48 + struCnfg.struLayerInfo[i].stuCameraInfo[j].nNirGreenSendOffsetY[nUnitAddr];
                            data[2] = 48 + struCnfg.struLayerInfo[i].stuCameraInfo[j].nNirBlueSendOffsetX[nUnitAddr];
                            data[3] = 48 + struCnfg.struLayerInfo[i].stuCameraInfo[j].nNirBlueSendOffsetY[nUnitAddr];
                            data[4] = 48 + struCnfg.struLayerInfo[i].stuCameraInfo[j].nNirRedSendOffsetX[nUnitAddr];
                            data[5] = 48 + struCnfg.struLayerInfo[i].stuCameraInfo[j].nNirRedSendOffsetY[nUnitAddr];
                            data[6] = 48 + struCnfg.struLayerInfo[i].stuCameraInfo[j].nNirNSendOffsetX[nUnitAddr];
                            data[7] = 48 + struCnfg.struLayerInfo[i].stuCameraInfo[j].nNirNSendOffsetY[nUnitAddr];
                            mySerial.pushCom1CmdData(CMD_CAMERA_IMG_SHIFT, nInt, board, nUnitAddr, data, 3);
                        default:
                            break;
                        }
                    }
                }
            }
        }
    }
    else
    {
        memset(data, 0, SEND_PACKET_DATA_SIZE);

        nInt = nLayerId;
        board = struCnfg.struLayerInfo[nInt].nViewBoardType[nViewId];
        nUnitAddr = struCnfg.struLayerInfo[nInt].stuViewInfo[nViewId].nViewUnitId[nUnit];
        transformCameraOffset(nInt,nViewId,nUnit);

        switch(struCnfp.nSensorDim[nInt][nViewId])
        {
        case 2:
            data[0] = 48 + struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nNirGreenSendOffsetX[nUnitAddr];
            data[1] = 48 + struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nNirGreenSendOffsetY[nUnitAddr];
            data[4] = 48 + struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nNirRedSendOffsetX[nUnitAddr];
            data[5] = 48 + struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nNirRedSendOffsetY[nUnitAddr];
            mySerial.pushCom1CmdData(CMD_CAMERA_IMG_SHIFT, nInt, board, nUnitAddr, data, 3);
            break;
        case 3:
            data[0] = 48 + struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nNirGreenSendOffsetX[nUnitAddr];
            data[1] = 48 + struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nNirGreenSendOffsetY[nUnitAddr];
            data[2] = 48 + struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nNirBlueSendOffsetX[nUnitAddr];
            data[3] = 48 + struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nNirBlueSendOffsetY[nUnitAddr];
            data[4] = 48 + struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nNirRedSendOffsetX[nUnitAddr];
            data[5] = 48 + struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nNirRedSendOffsetY[nUnitAddr];
            mySerial.pushCom1CmdData(CMD_CAMERA_IMG_SHIFT, nInt, board, nUnitAddr, data, 3);
            break;
        case 4:
            data[0] = 48 + struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nNirGreenSendOffsetX[nUnitAddr];
            data[1] = 48 + struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nNirGreenSendOffsetY[nUnitAddr];
            data[2] = 48 + struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nNirBlueSendOffsetX[nUnitAddr];
            data[3] = 48 + struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nNirBlueSendOffsetY[nUnitAddr];
            data[4] = 48 + struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nNirRedSendOffsetX[nUnitAddr];
            data[5] = 48 + struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nNirRedSendOffsetY[nUnitAddr];
            data[6] = 48 + struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nNirNSendOffsetX[nUnitAddr];
            data[7] = 48 + struCnfg.struLayerInfo[nInt].stuCameraInfo[nViewId].nNirNSendOffsetY[nUnitAddr];
            mySerial.pushCom1CmdData(CMD_CAMERA_IMG_SHIFT, nInt, board, nUnitAddr, data, 3);
            break;
        default:
            break;
        }
    }
}

int GlobalFlow::getUnitGroup(int view, int unit, int group)
{
    for(int k = 0; k < struCnfg.struLayerInfo[0].stuViewInfo[view].stuIdenGupInfo[group].nGroupUnitTotal; k++)
    {
        if(struCnfg.struLayerInfo[0].stuViewInfo[view].stuIdenGupInfo[group].nUnitGupId[k] == unit)
        {
            return 1;
//            int nUnitAddr = getIdentifyGroupAddr(0, view, group, k);
//            if(nUnitAddr > 128)
//            {
//                continue;
//            }
//            else
//            {
//                return 1;
//            }
        }
    }
    return 0;

}

void GlobalFlow::saveScreenshot()
{

    QSettings setting(CFG_APPSet,QSettings::IniFormat);
    QString connectName = setting.value("connectName","").toString();


    QDir dir("/sdcard/" +connectName );

    if(!dir.exists())
    {
        dir.mkdir("/sdcard/"+connectName);
    }


    QDesktopWidget *desktop = QApplication::desktop();

    int width = desktop->width();
    int height = desktop->height();

    QPixmap screenshot = QPixmap::grabWindow(desktop->winId(), 0, 0, width, height);
    QString fileName = QString("/sdcard/") +connectName + "/" + QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + ".png";
    if (!fileName.isEmpty()) {
        screenshot.save(fileName,"PNG");
    }
}
void GlobalFlow::sendStatus(int power)
{
    if(struCnfg.bUpdateStatus == 1)  //心跳锁
    {
        return;
    }
    char data[SEND_PACKET_RECTRL_BOARD_SIZE];
    memset(data, 0, SEND_PACKET_RECTRL_BOARD_SIZE);

    data[0]=(struCnfg.nPlcCtrl & power);
    data[1]+=1;  //开机状态
    data[1]+=struGsh.bSortStart<<1;  //下料状态
    data[2]+=(struGsh.nAlarmStautsCurrent[ALARM_PRESSURE]);         //bit0 气压报警
    data[2]+=struCnfg.nBoardStatusAlarm<<1;                         //bit1 电路板版本报警
    data[2]+=(struGsh.nAlarmStautsCurrent[ALARM_IPC])<<2;           //bit2 深度学习IPC报警
    data[2]+=(struGsh.nAlarmStautsCurrent[ALARM_FEED_LEVEL])<<3;    //bit3 料位报警
    data[2]+=struGsh.nAlarmStautsCurrent[ALARM_ACCELERATION]<<5;                                              //bit5 加速度传感器报警
    for(int k = 0; k < 2; k++)
    {
        for(int i=0; i< MAX_FEEDER; i++)
        {
            data[3+MAX_FEEDER*k+i] = struCnfp.struGroupCtrl[k].nFeederValueMain[i];
        }
    }
    mySerial.pushCom2CmdData(CMD_UPPER_COMPUTER_STATUS_RET_PKG, 0, BOARD_TYPE_CONTROL, 0x0e, data, 3);
}

// 模型结果与运算
void GlobalFlow::IpcClassResultANDOperat(bool isall, int view,int unit)
{
    int i, j, k, t, m, n;
    int nUnitAddr = 0;

    unsigned char data[512];

    memset(data, 0, 512);
    struGsh.bUpdateState = 1;

    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(struCnfg.struLayerInfo[i].nViewIsUsed[j] == 1)
            {

                for(k = 0; k <struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal; k++)
                {
                    for(t = 0; t < struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; t++) // 认为视的次数和吹的序号一致
                    {
                        nUnitAddr = (getIdentifyGroupAddr(i, j, k, t))%64;

                        if(!isall && (view != j || unit != nUnitAddr)) continue;
                        if(struCnfg.struIpcCameraParams[i][j][nUnitAddr].ipcType == IPC_CAMERA_TYPE_NULL) continue;


                        data[0] = struCnfp.ipcModelClassCount[j][k];

                        for(m = 0; m < struCnfp.ipcModelClassCount[j][k]; m++)
                        {
                            if(m > 21) break;
                            // qDebug()<<"view"<<j<<"group"<<k<<"unit"<<nUnitAddr<<"model"<<m<<"checkd"<<struCnfp.struCameraIpcClassGroupParams[j][k].classANDEnable[m];
                            if(struCnfp.struCameraIpcClassGroupParams[j][k].classANDEnable[m] == 1)
                            {
                                data[1+m] = struCnfp.struCameraIpcClassGroupParams[j][k].classANDEnable[m];
                            }
                            else
                            {
                                data[1+m] = 0;
                            }
                        }
                        g_Udp.pushUdpCmdData(CMD_UDP_IPC_RESULT_ANDOPERATE, true, j, nUnitAddr, struCnfp.ipcModelClassCount[j][k]+1, data);
                        if(struGsh.bSortStart)
                            myFlow.msleep(2);
                    }
                    //                    mydebug(data,16);
                }
            }
        }
    }
    struGsh.bUpdateState = 0;
}

bool GlobalFlow::boardSwitchFactory(int boardIndex,int chainIndex, int chuteIndex)
{
    switch (boardIndex) {
    case board_interface:
        if(struGsh.struVer.nInterfaceRes == 1 && struGsh.struVer.sInterface[2] == FACTORY_MODE)
            return true;
        break;
    case board_camera:
        if(struGsh.struVer.nUnitRes[0][chainIndex-1][chuteIndex] == 1 && struGsh.struVer.sUnit[0][chainIndex-1][chuteIndex][0] != 0 && struGsh.struVer.sUnit[0][chainIndex-1][chuteIndex][2] == FACTORY_MODE)
            return true;
        break;
    case board_ipc_camera:
        if(struGsh.struVer.nUnitRes[0][chainIndex-1][chuteIndex] == 1 && struGsh.struVer.sUnitIpc[0][chainIndex-1][chuteIndex][0] != 0 && struGsh.struVer.sUnitIpc[0][chainIndex-1][chuteIndex][2] == FACTORY_MODE)
            return true;
        break;
    case board_valve:
        if(struGsh.struVer.nVavleRes[0][chainIndex-1][chuteIndex] == 1 && struGsh.struVer.sVavle[0][chainIndex-1][chuteIndex][0] != 0 && struGsh.struVer.sVavle[0][chainIndex-1][chuteIndex][2] == FACTORY_MODE)
            return true;
        break;
    case board_control:
        if(struGsh.struVer.nCtrlRes[chuteIndex] == 1 && struGsh.struVer.sCtrlBoard[chuteIndex][0] != 0 && struGsh.struVer.sCtrlBoard[chuteIndex][2] == FACTORY_MODE)
            return true;
        break;
    case board_lamp:
        if(struGsh.struVer.nLampRes[chuteIndex] == 1 && struGsh.struVer.sLampBoard[chuteIndex][0] != 0 && struGsh.struVer.sLampBoard[chuteIndex][2] == FACTORY_MODE)
            return true;
        break;
    case board_control_zd:
        if(struGsh.struVer.nAccelerationRes[chuteIndex] == 1 && struGsh.struVer.sAccelerationBoard[chuteIndex][0] != 0 && struGsh.struVer.sAccelerationBoard[chuteIndex][2] == FACTORY_MODE)
            return true;
        break;
    case board_control_dl:
        if(struGsh.struVer.nHalogenRes[chuteIndex] == 1 && struGsh.struVer.sAccelerationBoard[chuteIndex][0] != 0 && struGsh.struVer.sAccelerationBoard[chuteIndex][2] == FACTORY_MODE)
            return true;
        break;
    default:
        return false;
        break;
    }
    return false;
}
int GlobalFlow::getBoardAddr(int boardIndex, int chainIndex)
{
    int m_boardAddr;

    QStringList chainList;
    chainList.clear();
    for(int i = 0;i< struCnfg.struLayerInfo[0].nViewChainTotal;i++){
        if(struCnfg.struLayerInfo[0].nViewIsUsed[i])
        {
            chainList<<QString::number(i);

        }
    }
    int m_view = 0;
    switch(boardIndex){
    case board_interface:
        m_boardAddr = BOARD_TYPE_INTERFACE;
        break;
    case board_camera:
    case board_ipc_camera:
        if(chainIndex == 0)
            //return struCnfg.struLayerInfo[struGsh.nLayer].nViewBoardType[chainIndex];
            return BOARD_TYPE_ALL_CAMERA;
        else{
            for(int i=0;i<chainList.size();i++)
            {
                if((chainIndex -1)== chainList.at(i).toInt())
                {
                    m_view = chainIndex - 1;
                }
            }
            qDebug()<<"m_view = "<<m_view;
            return struCnfg.struLayerInfo[struGsh.nLayer].nViewBoardType[m_view];
        }
        break;
    case board_valve:
        return struCnfg.struLayerInfo[struGsh.nLayer].nVavleBoardType[0];
        break;
    case board_control:
        return struCnfg.nCtrlBoardType;
        break;
    case board_lamp:
        return struCnfg.nLampBoardType;
        break;
    case board_control_zd:
        return struCnfg.nCtrlBoardType;
        break;
    case board_control_dl:
        return struCnfg.nCtrlBoardType;
        break;
    }
    return m_boardAddr;
}
bool GlobalFlow::boardUpdateSuccess(int boardIndex, int chainIndex, int chuteIndex, uchar versionToUpdate)
{
    switch (boardIndex) {
    case board_interface:
        if(struGsh.struVer.nInterfaceRes == 1 && struGsh.struVer.sInterface[2] == USER_MODE )
            return true;
        break;
    case board_camera:
        if(struGsh.struVer.nUnitRes[0][chainIndex-1][chuteIndex] == 1 && struGsh.struVer.sUnit[0][chainIndex-1][chuteIndex][2] ==(struCnfp.nCameraBoardUserMode[0][chainIndex-1][chuteIndex] ))
            return true;
        break;
    case board_ipc_camera:
        if(struGsh.struVer.nUnitRes[0][chainIndex-1][chuteIndex] == 1 && struGsh.struVer.sUnitIpc[0][chainIndex-1][chuteIndex][2] == USER_MODE)
            return true;
        break;
    case board_valve:
        if(struGsh.struVer.nVavleRes[0][chainIndex-1][chuteIndex] == 1 && struGsh.struVer.sVavle[0][chainIndex-1][chuteIndex][2] == USER_MODE)
            return true;
        break;
    case board_control:
        if(struGsh.struVer.nCtrlRes[chuteIndex] == 1 && struGsh.struVer.sCtrlBoard[chuteIndex][2] == USER_MODE)
            return true;
        break;
    case board_lamp:
        if(struGsh.struVer.nLampRes[chuteIndex] == 1 && struGsh.struVer.sLampBoard[chuteIndex][2] == USER_MODE)
            return true;
        break;
    case board_control_zd:
        if(struGsh.struVer.nAccelerationRes[chuteIndex] == 1 && struGsh.struVer.sAccelerationBoard[chuteIndex][2] == USER_MODE)
            return true;
        break;
    case board_control_dl:
        if(struGsh.struVer.nHalogenRes[chuteIndex] == 1 && struGsh.struVer.sHalogenBoard[chuteIndex][2] == USER_MODE)
            return true;
        break;
    default:
        return false;
        break;
    }
    return false;
}


void GlobalFlow::SwitchUserMode(int chain ,int unit)
{
    myFlow.switchUserMirror(board_camera<<4|FACTORY_MODE,myFlow.getBoardAddr(board_camera,chain),unit,0);
    myFlow.sleep(6);
    if(chain  == 0) // 整机
    {
        int k;
        for(int i = 0; i < MAX_VIEW ; i++)
        {
            if(struCnfg.struLayerInfo[0].nViewIsUsed[i] == 0)
                continue;

            for (int m =0;m<struCnfg.struLayerInfo[0].nViewUnitTotal[i];m++)
            {
                myFlow.readVersion(myFlow.getBoardAddr(board_camera,i+1), m+1);
                for(k = 0; k < 20; k++)
                {
                    myFlow.msleep(500);
                    if(myFlow.boardSwitchFactory(board_camera,i+1,m))
                    {
                        break;
                    }
                }
                if(k == 20)
                {
                    return;
                }
            }
        }
    }
    else
    {
        if(unit == 0) // 全通道
        {
            int k;
            for (int m =0;m<struCnfg.struLayerInfo[0].nViewUnitTotal[chain-1];m++)
            {
                myFlow.readVersion(myFlow.getBoardAddr(board_camera,chain), m+1);
                for(k = 0; k < 20; k++)
                {
                    myFlow.msleep(500);
                    if(myFlow.boardSwitchFactory(board_camera,chain,m))
                    {
                        break;
                    }
                }
                if(k == 10)
                {
                    return;
                }
            }
        }
        else
        {
            myFlow.readVersion(myFlow.getBoardAddr(board_camera,chain), unit);
            int k;
            for(k = 0; k < 10; k++)
            {
                myFlow.msleep(500);
                if(myFlow.boardSwitchFactory(board_camera,chain,unit-1))
                {
                    break;
                }
            }
            if(k == 10)
            {
                return;
            }
        }
    }

    myFlow.resetFpgaRunModeRG();
    myFlow.sleep(20);
    myFlow.resetMachineConfigRG();
    myFlow.resetSensorConfigRG();
}



void GlobalFlow::SwitchAllViewUserMode()
{
    struGsh.bUpdateState = 1;

    int error = 0;
    int changemode = 0;

    int m_viewUserMode[MAX_VIEW];
    int m_viewRes[MAX_VIEW];
    for(int i = 0; i < MAX_VIEW; i++)
    {
        m_viewRes[i] = struGsh.struVer.nUnitRes[0][i][0];
        m_viewUserMode[i] = struGsh.struVer.sUnit[0][i][0][2];
    }

    for(int i = 0; i < MAX_VIEW; i++)
    {
        if(!struCnfg.struLayerInfo[0].nViewIsUsed[i])
            continue;

        if(m_viewRes[i] == -1) // 未检测到版本
            continue;

        if(m_viewUserMode[i] == struCnfp.nCameraBoardUserMode[0][i][0])
        {
            continue;
        }
        else
        {
            logger()->info("view %1 change usermode to %2",i,struCnfp.nCameraBoardUserMode[0][i][0]);
            changemode = 1;
            g_infoWidget().setLabelText(g_myLan().msg_mode_changing);
            g_infoWidget().delayShow();
            myFlow.SwitchUserMode(i+1, 0);
            myFlow.msleep(3000);
        }
    }

    if(changemode == 0)
    {
        struGsh.bUpdateState = 0;
        return;
    }

    myFlow.resetCommunication();

    for(int i = 0; i < MAX_VIEW; i++)
    {
        if(!struCnfg.struLayerInfo[0].nViewIsUsed[i])
            continue;

        for (int m =0; m<struCnfg.struLayerInfo[0].nViewUnitTotal[i];m++)
        {
            int k;
            for( k = 0; k < 20; k++)
            {
                myFlow.msleep(500);
                if(myFlow.boardUpdateSuccess(board_camera,i+1,m,0))
                {
                    break;
                }
            }
            if(k == 20)
            {
                error = 1;
            }
        }
    }

    if(error == 1)
    {
        g_infoWidget().setLabelText(g_myLan().ids_setUserModeFailed);
        g_infoWidget().showSecs();
    }

    struGsh.bUpdateState = 0;
    return;
}

int GlobalFlow::checkIpcModel()
{

    for(int i = 0 ; i < MAX_VIEW ; i++)
    {
        if(struCnfg.struLayerInfo[0].nViewIsUsed[i] != 1)
        {
            continue;
        }
        for(int k = 0; k < struCnfg.struLayerInfo[0].stuViewInfo[i].nIdentifyGroupTotal; k++)
        {

            for(int m = 0; m < struCnfg.struLayerInfo[0].stuViewInfo[i].stuIdenGupInfo[k].nGroupUnitTotal; m++)
            {
                if(QString::fromUtf8(struCnfp.sCurModelName[i][k]) == "")
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

bool GlobalFlow::getModelTargetValue(bool isAll,int view,int unit)
{
    unsigned char data[MAX_IPC_MODEL_NAME_LENGTH];
    memset(data, 0, MAX_IPC_MODEL_NAME_LENGTH);

    int m = 0;
//    qDebug()<<QString::fromUtf8(struCnfp.sCurModelName[view][0]);
    for(int i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(int j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(struCnfg.struLayerInfo[i].nViewIsUsed[j] == 1)
            {
                int k = 0;
                //for(int k = 0; k <struCnfg.struLayerInfo[i].nViewUnitTotal[j]; k++)
                {
                  //  if(!isAll && (view != j || unit != k)) continue;
                    if(struCnfg.struIpcCameraParams[i][j][k].ipcType != IPC_CAMERA_TYPE_NULL)
                    {
                        struIpcShare.struIpcReqStauts[j][k].modelTargetValueReq = IPC_REQ_RETURN_NULL;
                        memset(data, 0, MAX_IPC_MODEL_NAME_LENGTH);
                        memcpy(data, struCnfp.sCurModelName[j][k], sizeof(struCnfp.sCurModelName[j][k]));

                        g_Udp.pushUdpCmdData(CMD_UDP_IPC_GET_TARGER_VALUE, true, j, k, MAX_IPC_MODEL_NAME_LENGTH, data);
                        //                        msleep(100);
                        for(m = 0;m < 10;m++)
                        {
                            myFlow.msleep(100);
                            if(struIpcShare.struIpcReqStauts[j][k].modelTargetValueReq == 1)
                            {
                                break;
                            }
                        }

                        if(m == 10)
                            return false;
                    }
                }
            }
        }
    }
    return true ;
}

/* 发送加速度传感器参数 */
void GlobalFlow::resetAccelerationParam()
{
    int i;
    char data[SEND_PACKET_DATA_SIZE];
    char data2[SEND_PACKET_DATA_SIZE];

    memset(data, 0, SEND_PACKET_DATA_SIZE);
    memset(data2, 0, SEND_PACKET_DATA_SIZE);

    for(i = 0; i < MAX_ACCELERATION; i++)
    {
        data[i] = struCnfg.accelerationEnable[i];
    }
    data[14] = struCnfg.accelerationNum;
    data[15] = struCnfg.accelerationTolerance;
    mySerial.pushCom1CmdData(CMD_ACCELERATION_ENABLE, 0, BOARD_TYPE_CONTROL, 0, data, 1);

    myFlow.msleep(10);
    for(i = 0; i < MAX_ACCELERATION; i++)
    {
        data2[i] = struCnfg.accelerationEnable2[i];
    }
    data2[14] = struCnfg.accelerationNum2;
    data2[15] = struCnfg.accelerationTolerance2;
    mySerial.pushCom1CmdData(CMD_ACCELERATION_ENABLE, 0, BOARD_TYPE_CONTROL, 1, data2, 1);
}


void  GlobalFlow::getCompressParams()
{

    QString tarCmd;
    //    if(g_Runtime().dirExist(CNF_BAK_DIR))
    //    {
    //        tarCmd = "rm -rf "+ QString::fromLatin1(CNF_BAK_DIR) +"/*";
    //        g_Runtime().mySystem(tarCmd);
    //        system("sync");
    //    }
    g_Runtime().dirExist(CNF_BAK_DIR);
    QString namelist ;
    for(int i =0 ;i <struCnfg.nProfileTotal; i++)
    {
        QByteArray filename(struCnfg.stuProfileIndex[i].sProfileName,strlen(struCnfg.stuProfileIndex[i].sProfileName));
        QString fileNameStr =QString::fromUtf8(filename);
        namelist = " " + QString::fromLatin1(CNF_MODE_DIR) + fileNameStr;
        tarCmd = "cp  -r " + namelist + " " +QString::fromLatin1(CNF_BAK_DIR);
        g_Runtime().mySystem(tarCmd);
        namelist = " " + QString::fromLatin1(CNF_MODE_DIR) + fileNameStr + "_2";
        tarCmd = "cp  -r " + namelist + " " +QString::fromLatin1(CNF_BAK_DIR);
        g_Runtime().mySystem(tarCmd);
    }
    tarCmd = "cp  -r " + QString::fromLatin1(CNF_GLOBAL) + " " +QString::fromLatin1(CNF_BAK_DIR);
    g_Runtime().mySystem(tarCmd);
    tarCmd = "cp  -r " + QString::fromLatin1(CNF_GLOBAL_B) + " " +QString::fromLatin1(CNF_BAK_DIR);
    g_Runtime().mySystem(tarCmd);
    return;
}

void  GlobalFlow::checkCompressParams()
{

    QDir newdir(CNF_BAK_DIR);
    QString tarCmd;
    if(newdir.exists())
    {
        logger()->info("backup cnffile");
        tarCmd = "cp  -r " + QString::fromLatin1(CNF_BAK_DIR) + "*  " +QString::fromLatin1(CNF_MODE_DIR);
        g_Runtime().mySystem(tarCmd);
        system("sync");
    }
    else
    {
    }
    return;
}
void GlobalFlow::resetCameraEnhance()
{
    int i, j, k;
    int nInt = 0, board, nUnitAddr;
    char data[SEND_PACKET_DATA_SIZE];
    for(i = 0; i < struCnfg.nLayerTotal; i++)  //层
    {
        memset(data, 0, SEND_PACKET_DATA_SIZE);
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)  // 视
        {
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
            data[0] = struCnfp.imageEnahnce[j];
            mySerial.pushCom1CmdData(CMD_CAMERA_IMAGE_ENHANCE, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        }
    }
}
/* 恢复FPGA运行模式 */
void GlobalFlow::resetFpgaBeforeRunModeRG()
{
    int i, j , m , n;
    int board;
    char data[SEND_PACKET_DATA_SIZE];
    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(!struCnfg.struLayerInfo[i].nViewIsUsed[j])
                continue;
            memset(data, 0, SEND_PACKET_DATA_SIZE);
            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
            for( m = 0; m < struCnfg.struLayerInfo[i].nViewUnitTotal[j]; m++)
            {
                n = struCnfg.struLayerInfo[i].nViewUnitTotal[j] - m -1;
                if(struGsh.struVer.sUnitbefore[i][j][n] != -1)
                {
                    data[0] = struGsh.struVer.sUnitbefore[i][j][n];
                    mySerial.pushCom1CmdData(CMD_ALL_FPGA_USER_MODE, 0, board, n, data, 3);
                    myFlow.msleep(100);
                    logger()->info("view:%1 unit:%2 usermode:%3  Restore ",i,n,struGsh.struVer.sUnitbefore[0][i][n]);
                }
            }
        }
    }
}
/* 重置FPGA运行模式 */
void GlobalFlow::resetInterfaceRunModeRG()
{
    char data[SEND_PACKET_DATA_SIZE];
    memset(data, 0, SEND_PACKET_DATA_SIZE);
    data[0] = struCnfg.nInterfaceBoardMode;
    mySerial.pushCom1CmdData(CMD_ALL_FPGA_USER_MODE, 0, BOARD_TYPE_INTERFACE, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
}
void GlobalFlow::resetValveRunModeRG()
{
    int i, j;
    int board;
    char data[SEND_PACKET_DATA_SIZE];
    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(int j = 0; j < struCnfg.struLayerInfo[i].nVavleChainTotal; j++)
        {
            memset(data, 0, SEND_PACKET_DATA_SIZE);
            board = struCnfg.struLayerInfo[i].nVavleBoardType[j];
            data[0] = struCnfg.nVavleBoardMode[i][j];    // 每视的模式默认都是1
            mySerial.pushCom1CmdData(CMD_ALL_FPGA_USER_MODE, 0, board, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
        }
    }
}
void GlobalFlow::resetControlRunModeRG()
{
    int i, j;
    int board;
    char data[SEND_PACKET_DATA_SIZE];
    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        if(!struCnfg.nAllInOne)
        {
            for(int j = 0;j < struCnfg.nCtrlBoardTotal;j++){
                memset(data, 0, SEND_PACKET_DATA_SIZE);
                data[0] = struCnfg.nCtrlBoardMode;
                mySerial.pushCom1CmdData(CMD_ALL_FPGA_USER_MODE, 0, BOARD_TYPE_CONTROL, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
            }
        }
    }
}

void GlobalFlow::resetLampRunModeRG()
{
    int i, j;
    int board;
    char data[SEND_PACKET_DATA_SIZE];
    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        memset(data, 0, SEND_PACKET_DATA_SIZE);
        data[0] = struCnfg.nLampBoardMode;
        mySerial.pushCom1CmdData(CMD_ALL_FPGA_USER_MODE, 0, BOARD_TYPE_LAMP, BOARD_TYPE_ALL_IN_CHAIN, data, 3);
    }
}

void GlobalFlow::getFpgaCameraArith(int boardIndex, int chainIndex, int chuteIndex)
{
    memset(struGsh.fpgaAirthAdd , 0 ,sizeof(struGsh.fpgaAirthAdd));
    unsigned char data[SEND_PACKET_DATA_SIZE];
    memset(data, 0, SEND_PACKET_DATA_SIZE);


    if(chainIndex == 0)
        g_Udp.pushUdpCmdData(CMD_UDP_REQ_CAMERA_ARITH, true, BOARD_TYPE_ALL_CAMERA,0xff, 0, data);
    else if(chuteIndex == 0)
    {
        g_Udp.pushUdpCmdData(CMD_UDP_REQ_CAMERA_ARITH, true, chainIndex-1,0xff, 0, data);
    }
    else
        g_Udp.pushUdpCmdData(CMD_UDP_REQ_CAMERA_ARITH, true, chainIndex-1,chuteIndex-1, 0, data);


    myFlow.sleep(1);

    return ;


    //    memset(struGsh.fpgaAirthAdd , 0 ,sizeof(struGsh.fpgaAirthAdd));
    //    unsigned char data[SEND_PACKET_DATA_SIZE];
    //    memset(data, 0, SEND_PACKET_DATA_SIZE);

    //    for(int i = 0; i < struCnfg.nLayerTotal; i++)
    //    {
    //        for(int j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
    //        {
    //            if(struCnfg.struLayerInfo[i].nViewIsUsed[j] == 1)
    //            {
    //                for(int k = 0; k <struCnfg.struLayerInfo[i].nViewUnitTotal[j]; k++)
    //                {
    //                    if(!isAll && (view != j || unit != k)) continue;
    //                    //if(struCnfg.struIpcCameraParams[i][j][k].ipcType != IPC_CAMERA_TYPE_NULL)
    //                    {
    //                        g_Udp.pushUdpCmdData(CMD_UDP_REQ_CAMERA_ARITH, true, j,k, 0, data);
    //                        myFlow.msleep(100);

    //                    }
    //                }
    //            }
    //        }
    //    }

}

void GlobalFlow::setFpgaRegisterStatus(int boardIndex, int chainIndex, int chuteIndex)
{
    unsigned char data[SEND_PACKET_DATA_SIZE];
    memset(data, 0, SEND_PACKET_DATA_SIZE);
    int datalen = 0;

    for(int i = 0; i < struCnfg.fpgaParamsNum; i++)
    {
        data[i] = struGsh.fpgaParmsforDebug[i];
        datalen++;
    }

    if(chainIndex == 0)
        g_Udp.pushUdpCmdData(CMD_UDP_SET_FPGA_STATUS, true, BOARD_TYPE_ALL_CAMERA,0xff, datalen, data);
    else if(chuteIndex == 0)
    {
        g_Udp.pushUdpCmdData(CMD_UDP_SET_FPGA_STATUS, true, chainIndex-1,0xff, datalen, data);
    }
    else
        g_Udp.pushUdpCmdData(CMD_UDP_SET_FPGA_STATUS, true, chainIndex-1,chuteIndex-1, datalen, data);

    myFlow.sleep(1);
}

void GlobalFlow::getFpgaRegisterStatus(int boardIndex, int chainIndex, int chuteIndex)
{
    unsigned char data[SEND_PACKET_DATA_SIZE];
    memset(data, 0, SEND_PACKET_DATA_SIZE);


    if(chainIndex == 0)
        g_Udp.pushUdpCmdData(CMD_UDP_GET_FPGA_PARAMS, true, BOARD_TYPE_ALL_CAMERA,0xff, 0, data);
    else if(chuteIndex == 0)
    {
        g_Udp.pushUdpCmdData(CMD_UDP_GET_FPGA_PARAMS, true, chainIndex-1,0xff, 0, data);
    }
    else
        g_Udp.pushUdpCmdData(CMD_UDP_GET_FPGA_PARAMS, true, chainIndex-1,chuteIndex-1, 0, data);


    myFlow.sleep(1);

}


/* 获取色选方案参数 */
void GlobalFlow::getMachineConfig()
{
    QSettings setting(CFG_MachineSet,QSettings::IniFormat);
    setting.beginGroup("machineset");

    memset(struGsh.strumachineset.machinetype, 0, sizeof(struGsh.strumachineset.machinetype));

    QString str = setting.value("machinetype","").toString();
    if(str == "")
    {
        logger()->info("No machine type, return");
        return;
    }
    QByteArray ba = str.toLocal8Bit();
    strncpy(struGsh.strumachineset.machinetype , ba.data() ,sizeof(ba)-1);

    for(int i = 0; i < MAX_CALIBRATE_SETS; i++)
    {
        int tmp = setting.value(QString("mode%1").arg(i), 0).toInt();
        if(tmp == 0)
            continue;

        struGsh.strumachineset.targetlightValue[i] =  tmp;
        struGsh.strumachineset.targetCount++;
    }
    setting.endGroup();

    // 将目标值带入多套系数中
    for(int i = 0 ; i < MAX_VIEW;i++)
    {
        for(int j = 0; j < MAX_VIEW_UNIT; j++)
        {
            for(int k = 0 ;k < struGsh.strumachineset.targetCount; k++)
            {
                struCnfg.struLayerInfo[0].stuCameraInfo[i].nCalibrateRed[j][k] = struGsh.strumachineset.targetlightValue[k];
                struCnfg.struLayerInfo[0].stuCameraInfo[i].nCalibrateGreen[j][k] = struGsh.strumachineset.targetlightValue[k];
                struCnfg.struLayerInfo[0].stuCameraInfo[i].nCalibrateBlue[j][k] = struGsh.strumachineset.targetlightValue[k];
            }
        }
    }

    return;
}




void GlobalFlow::cameraChangeCalibrate()
{

    for(int i = 0; i < MAX_LAMP_BOARD_PER_INTERFACE;i++){
        for(int j = 0; j < MAX_LAMP_PER_BOARD;j++){
            struCnfp.nLampValue[0][i][j] = struCnfg.stuLayerLampCfg.nLampValue[i][j][struCnfp.bCurCal] ;
            struCnfp.nLampEnable[0][i][j] = struCnfg.stuLayerLampCfg.nLampEnable[i][j][struCnfp.bCurCal] ;
            struCnfp.nLampSeq[0][i][j] = struCnfg.stuLayerLampCfg.nLampSeq[i][j][struCnfp.bCurCal] ;
        }
    }
    for(int i = 0; i < MAX_VIEW; i++){
        struCnfp.nSenseBrightSelection[struGsh.nLayer][i] = struCnfg.nSenseBrightSelection_calb[i][struCnfp.bCurCal];
        struCnfp.nSensorRowFre[struGsh.nLayer][i] = struCnfg.nSensorRowFre_calb[i][struCnfp.bCurCal];
        struCnfp.nTimeDivisionScheme[struGsh.nLayer][i] = struCnfg.bCaliTDmode_calb[i][struCnfp.bCurCal];
    }
    struCnfp.bCaliGainEnable = struCnfg.bCaliGainEnable_calb[struCnfp.bCurCal];
    struCnfp.bCaliBiasEnable = struCnfg.bCaliBiasEnable_calb[struCnfp.bCurCal];

}


void GlobalFlow::cameraSaveCalibrate()
{
    // 保存多套系数
    for(int i = 0; i < MAX_LAMP_BOARD_PER_INTERFACE;i++){
        for(int j = 0; j < MAX_LAMP_PER_BOARD;j++){
            struCnfg.stuLayerLampCfg.nLampValue[i][j][struCnfp.bCurCal] = struCnfp.nLampValue[0][i][j];
            struCnfg.stuLayerLampCfg.nLampEnable[i][j][struCnfp.bCurCal] = struCnfp.nLampEnable[0][i][j];
            struCnfg.stuLayerLampCfg.nLampSeq[i][j][struCnfp.bCurCal] = struCnfp.nLampSeq[0][i][j];
        }
    }
    for(int i = 0; i < MAX_VIEW; i++){
        struCnfg.nSenseBrightSelection_calb[i][struCnfp.bCurCal] = struCnfp.nSenseBrightSelection[struGsh.nLayer][i];
        struCnfg.nSensorRowFre_calb[i][struCnfp.bCurCal] = struCnfp.nSensorRowFre[struGsh.nLayer][i];
        struCnfg.bCaliTDmode_calb[i][struCnfp.bCurCal] = struCnfp.nTimeDivisionScheme[struGsh.nLayer][i];
    }
    struCnfg.bCaliGainEnable_calb[struCnfp.bCurCal] = struCnfp.bCaliGainEnable;
    struCnfg.bCaliBiasEnable_calb[struCnfp.bCurCal] = struCnfp.bCaliBiasEnable;
}


void GlobalFlow::getDefaultLampConfig()
{
    int i,j;
    switch (struCnfg.nMachineType) {
    case MACHINE_VERTICAL_DM_RG:
    case MACHINE_VERTICAL_DM_ZK:
    case MACHINE_VERTICAL_ZL_CS:
    case MACHINE_VERTICAL_ZL_CV:
    case MACHINE_LD_ZL_GP:

        /* 区分12通道，1通道、其他通道 */
       if(struCnfg.nChuteTotal <= 2 )
        {
            struCnfg.stuLayerLampCfg.fLampBoardQuo[0][0] = 2.000;   //前面4个白物料灯
            struCnfg.stuLayerLampCfg.fLampBoardQuo[0][1] = 0.800;   //后面2个白物料灯
            struCnfg.stuLayerLampCfg.fLampBoardQuo[0][2] = 0.800;   //后面2个蓝物料灯
            struCnfg.stuLayerLampCfg.fLampBoardQuo[0][3] = 0.800;   //2个背景灯
            struCnfg.stuLayerLampCfg.fLampBoardQuo[0][4] = 0.800;   //不用
            struCnfg.stuLayerLampCfg.fLampBoardQuo[0][5] = 0.800;   //不用

        }
        else if(struCnfg.nChuteTotal <= 10 )
        {
            for(i = 0; i < struCnfg.stuLayerLampCfg.nLampBoardTotal; i++)
            {
                for(j = 0; j < MAX_LAMP_PER_BOARD; j++)
                {
                    struCnfg.stuLayerLampCfg.fLampBoardQuo[i][j] = (struCnfg.nChuteTotal+1)/11.0;
                }
            }
        }
       else
       {
           for(i = 0; i < struCnfg.stuLayerLampCfg.nLampBoardTotal; i++)
           {
               for(j = 0; j < MAX_LAMP_PER_BOARD; j++)
               {
                   struCnfg.stuLayerLampCfg.fLampBoardQuo[i][j] = 1;
               }
           }
       }
        break;
    case MACHINE_LD2_ZL_GP:

        if(struCnfg.nChuteTotal <= 10 )
        {
            for(i = 0; i < struCnfg.stuLayerLampCfg.nLampBoardTotal; i++)
            {
                for(j = 0; j < MAX_LAMP_PER_BOARD; j++)
                {
                    struCnfg.stuLayerLampCfg.fLampBoardQuo[i][j] = (struCnfg.nChuteTotal+1)/11.0;
                }
            }
        }
        else
        {
            for(i = 0; i < struCnfg.stuLayerLampCfg.nLampBoardTotal; i++)
            {
                for(j = 0; j < MAX_LAMP_PER_BOARD; j++)
                {
                    struCnfg.stuLayerLampCfg.fLampBoardQuo[i][j] = 1;
                }
            }
        }

        break;
    default:
        break;
    }
}
