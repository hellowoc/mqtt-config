#include "mydelaycode.h"
#include <time.h>
#include <QApplication>
#include <QtGui>
#include <QProcess>
#include "mysetdatetime.h"
#ifdef Q_OS_UNIX
#include <unistd.h>
#endif

bool timeDccryptProcessIsRun=false;
myDelayCode paramDelayCode;
myDelayCode paramDelayCode30Day;

myDelayCode::myDelayCode(QObject *parent) :
    QObject(parent)
{
    delayCodeClear();
}

void myDelayCode::delayCodeClear()
{
    memset(delay_code_eeprom, 0, DCCRYPT_MAX_STR);
    memset(limit_date_cur, 0, DCCRYPT_MAX_LIMIT_DATE);
    memset(product_only_id_cur, 0, DCCRYPT_MAX_ONLY_ID);

    memset(delay_code, 0, DCCRYPT_MAX_STR);
    memset(limit_date, 0, DCCRYPT_MAX_LIMIT_DATE);
    memset(product_only_id, 0, DCCRYPT_MAX_ONLY_ID);

    memset(paramDelayCode30Day.delay_code_eeprom, 0, DCCRYPT_MAX_STR);
    memset(paramDelayCode30Day.limit_date_cur, 0, DCCRYPT_MAX_LIMIT_DATE);
    memset(paramDelayCode30Day.product_only_id_cur, 0, DCCRYPT_MAX_ONLY_ID);

    memset(paramDelayCode30Day.delay_code, 0, DCCRYPT_MAX_STR);
    memset(paramDelayCode30Day.limit_date, 0, DCCRYPT_MAX_LIMIT_DATE);
    memset(paramDelayCode30Day.product_only_id, 0, DCCRYPT_MAX_ONLY_ID);

    memset(paramDelayCode30Day.product30DayUseStartDay, 0, 8);
    memset(paramDelayCode30Day.product30DayUseOnlyCurId, 0, DCCRYPT_MAX_ONLY_30DAY_ID);
    memset(paramDelayCode30Day.product30DayUseOnlyIdEeprom, 0, DCCRYPT_MAX_ONLY_30DAY_ID);

    enable  = 0;
    code_size = 0;

    wYear = 0;
    wMonth = 0;
    wDay = 0;

    pYear = &wYear;
    pMonth = &wMonth;
    pDay = &wDay;

    uiLanguage = 0xFF;
}

int myDelayCode::delayCodeCheck()
{
    char curYear[64];
    char curMonth[64];
    char curDay[64];
    char curHour[64];
    char curMin[64];
    char curSec[64];
    time_t curtime,savedtime,savedtime2,tmptime = 0;
    struct tm *stru_curtime;
    FILE *fp;
    int counter;
    int ret = 0;
    /*加密使能*/
#ifdef Q_OS_UNIX
        for(int i = 1; i < 3;)
        {
            if (myeeprom_read(BASE_ADDR_DELAY+i*DCCRYPT_MAX_STR, &enable, 1)) {//enable是第一个字节
                if (enable == 0xFF)
                {
                     i++;
                     continue;
                }
                else {  //eeprom写过数据
                    break;
                }
            }
            else{
                i++;
                if (i >= 3)
                {
                    return 0; //表示没有eeprom或坏了
                }
            }
	    }

//        qDebug("enable is %d", enable);
	    /*加密长度*/
        if (enable == 0xAA)
        {  //加密使能
            for(int i = 1; i< 3;)
            {
                if(myeeprom_read(BASE_ADDR_DELAY+i*DCCRYPT_MAX_STR+1, &code_size, 1))
                {
                    if(code_size == 0xFF) {
                        i++;
                        continue;
                    }
                    else {
                        break;
                    }
                }
                else
                {
                    i++;
                    if (i == 3) //eeprom数据被破坏
                    {
                        return 0;
                    }
                }
            }
	    }
        else
        {
            return 0;//无加密
	    }
//        qDebug("code_size is %d", code_size);
	    /*读取加密码*/
        if (code_size != 0xFF)
        {
            for(int i = 1; i < 3;)
            {
                if(myeeprom_read(BASE_ADDR_DELAY+i*DCCRYPT_MAX_STR+2, delay_code_eeprom, code_size))
                {
                    if (delay_code_eeprom[0]== 0xFF&&delay_code_eeprom[1]== 0xFF&&delay_code_eeprom[2]== 0xFF&&delay_code_eeprom[3] == 0xFF) //表示数据无效
                    {
                        i++;
                        if (i == 3)
                        {
                            return 0;
                        }
                        continue;
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    i++;
                    if (i == 3)
                    {
                        return 0;
                    }
                }
            }
	    }
        else
        {
		    return 0;
	    }
#endif
#ifdef Q_OS_WIN32
    return 0;   //windows环境不判断
#endif
    /*启动定时保存参数功能*/
    QProcess *process = new QProcess;
    QStringList strList;
    strList<<"";
    process->startDetached("./zktimer",strList);

    timeDccryptProcessIsRun = true;

    /*检测加密方式，累计计时加密及系统时间加密，判断加密是否到期*/
//    qDebug("delay_code_eeprom is %s", delay_code_eeprom);


    ret = paramDelayCode.delayDecryptCode(paramDelayCode.delay_code_eeprom, wYear, wMonth, wDay, product_only_id_cur);
    sprintf(paramDelayCode.limit_date_cur, "%04d%02d%02d", wYear, wMonth, wDay);

//    printf("limit_date_cur is %d \n", atoi(limit_date_cur));
//    printf("product_only_id_cur is %s\n", product_only_id_cur);

    time(&curtime);
    stru_curtime = localtime(&curtime);

//    getSysTimeFromE2prom(&savedtime);
//    getSysTimeFromEmmc(&savedtime2);
    tmptime = qMax(savedtime,savedtime2);

    if (curtime < tmptime){   //当前时间 < 保存的时间， RTC错误,用保存的时间替代当前时间
        stru_curtime = localtime(&tmptime);
        strftime(curYear ,64,"%Y",stru_curtime);
        strftime(curMonth,64,"%m",stru_curtime);
        strftime(curDay  ,64,"%d",stru_curtime);
        strftime(curHour ,64,"%H",stru_curtime);
        strftime(curMin  ,64,"%M",stru_curtime);
        strftime(curSec  ,64,"%S",stru_curtime);

        char time[64];
        sprintf(time, "date -s \"%04d-%02d-%02d %02d:%02d:%02d\"", atoi(curYear), atoi(curMonth),
        atoi(curDay),atoi(curHour),atoi(curMin),atoi(curSec));
        system(time);
        popen("hwclock -w","r");
    }
    strftime(curDate,64,"%Y%m%d",stru_curtime);

    if (atoi(curDate) < atoi(limit_date_cur)) {  //加密未到期
        int yy1 = 1900+stru_curtime->tm_year;
        int mm1 = 1+stru_curtime->tm_mon;
        int dd1 = stru_curtime->tm_mday;
        int yy2 = int(wYear);
        int mm2 = int(wMonth);
        int dd2 = int(wDay);

        struGsh.struDccryptData.mCryptDayLast = myFlow.dayDistance(yy1, mm1, dd1, yy2, mm2, dd2);
        return 0;
    }

    return 1;
}

/* 判断加密码是否使用过 */
int myDelayCode::delaycodeMatch(const char *str)
{
    FILE *fp;
    char line[DCCRYPT_MAX_STR] = "";

    if (!(fp = fopen(CNF_FILE_DELAYCODE, "r"))) {
        fprintf(stderr, "unable to open %s\n", CNF_FILE_DELAYCODE);
        return 0;
    }

    while (!feof(fp)) {
        fscanf(fp,"%s", line);
//        printf("%s\n",line);
//        printf("%s\n",str);
        if (strcmp(line, str) == 0) {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);

    return 0;
}

/* 保存加密密码,防止重复使用 */
int myDelayCode::delaycodeSave(char *str)
{
    FILE *fp;

    if (!(fp = fopen(CNF_FILE_DELAYCODE, "a"))) {
        fprintf(stderr, "unable to open %s\n", CNF_FILE_DELAYCODE);
        return 0;
    }

    fprintf(fp, "%s\n", str);
    fclose(fp);

    return 1;
}

int myDelayCode::delayDecryptCode(char* code, unsigned short wYear, unsigned short wMonth, unsigned short wDay, char* onlyID)
{
    int ret = decrypt_error_code_format;

#ifdef Q_OS_UNIX
    ret = DecryptCode((const char*)code, pYear, pMonth, pDay, onlyID);
#endif

    return ret;
}

bool myDelayCode::delayWhetherCodesMatch(char* code1, char* code2)
{
    bool ret=false;

#ifdef Q_OS_UNIX
    ret = WhetherCodesMatch((const char*)code1, (const char*)code2);
#endif

    return ret;
}

void myDelayCode::writeDelaycode2eeprom()
{


#ifdef Q_OS_UNIX
    writeSysTimeToE2prom();
    writeSysTimeToEmmc();
       myFlow.sleep(3);
        /* to eeprom */
       for(int i = 1; i < 3;i++) {
            myeeprom_write(BASE_ADDR_DELAY+i*DCCRYPT_MAX_STR, &paramDelayCode.enable, 1);
            myeeprom_read(BASE_ADDR_DELAY+i*DCCRYPT_MAX_STR, &paramDelayCode.enable, 1);
            usleep(5000);
//            printf("enable is %d\n",paramDelayCode.enable);
            paramDelayCode.code_size = strlen(paramDelayCode.delay_code_eeprom);
            myeeprom_write(BASE_ADDR_DELAY+i*DCCRYPT_MAX_STR+1, &paramDelayCode.code_size, 1);
            myeeprom_read(BASE_ADDR_DELAY+i*DCCRYPT_MAX_STR+1, &paramDelayCode.code_size, 1);
            usleep(5000);
//            printf("code_size is %d\n",paramDelayCode.code_size);
            myeeprom_write(BASE_ADDR_DELAY+i*DCCRYPT_MAX_STR+2, paramDelayCode.delay_code_eeprom, strlen(paramDelayCode.delay_code_eeprom));
            myeeprom_read(BASE_ADDR_DELAY+i*DCCRYPT_MAX_STR+2, paramDelayCode.delay_code_eeprom, paramDelayCode.code_size);
 //           printf("code is %s\n",paramDelayCode.delay_code_eeprom);
            usleep(5000);
        }
#endif
}

int myDelayCode::get30DayInfofromEeprom()
{
    char curYear[64];
    char curMonth[64];
    char curDay[64];
    char curHour[64];
    char curMin[64];
    char curSec[64];
    time_t curtime,savedtime,savedtime2,tmptime = 0;
    struct tm *stru_curtime;
    FILE *fp;
    int counter;
    int ret = 0;
    /*加密使能*/
#ifdef Q_OS_UNIX
        for(int i = 1; i < 3;)
        {
            if (myeeprom_read(BASE_30DAY_USE_ADDR_DELAY+i*DCCRYPT_MAX_STR, &paramDelayCode30Day.product30DayUseStartEnable, 1)) {//product30DayUseStartEnable是第一个字节
                if (paramDelayCode30Day.product30DayUseStartEnable == 0xFF)
                {
                     i++;
                     continue;
                }
                else {  //eeprom写过数据
                    break;
                }
            }
            else{
                i++;
                if (i >= 3)
                {
                    return 0; //表示没有eeprom或坏了
                }
            }
        }

 //       qDebug("product30DayUseStartEnable is %d", paramDelayCode30Day.product30DayUseStartEnable);
        /* 30天加密是否到期 */
        for(int i = 1; i < 3;)
        {
            if (myeeprom_read(BASE_30DAY_USE_ADDR_DELAY+i*DCCRYPT_MAX_STR+1, &paramDelayCode30Day.product30DayUseOver, 1)) {//product30DayUseOver
                if (paramDelayCode30Day.product30DayUseOver == 0xFF)
                {
                     i++;
                     continue;
                }
                else {  //eeprom写过数据
                    break;
                }
            }
            else{
                i++;
                if (i >= 3)
                {
                    return 0; //表示没有eeprom或坏了
                }
            }
        }
 //       qDebug("product30DayUseOver is %d", paramDelayCode30Day.product30DayUseOver);
        /*30天加密起始日期*/
        if (paramDelayCode30Day.product30DayUseStartEnable == 0xAA)
        {
            for(int i = 1; i < 3;)
            {
                if (myeeprom_read(BASE_30DAY_USE_ADDR_DELAY+i*DCCRYPT_MAX_STR+2, paramDelayCode30Day.product30DayUseStartDay, 8)) {//product30DayUseOver
//                    qDebug("start day is %d-%d-%d-%d-%d-%-%d-%d", product30DayUseStartDay[0], product30DayUseStartDay[1], product30DayUseStartDay[2], product30DayUseStartDay[3]
//                            ,product30DayUseStartDay[4], product30DayUseStartDay[5], product30DayUseStartDay[6], product30DayUseStartDay[7]);
                    if (paramDelayCode30Day.product30DayUseStartDay[0]== 0xFF&&paramDelayCode30Day.product30DayUseStartDay[1]== 0xFF
                            &&paramDelayCode30Day.product30DayUseStartDay[2]== 0xFF&&paramDelayCode30Day.product30DayUseStartDay[3] == 0xFF) //表示数据无效
                    {
                         i++;
                         continue;
                    }
                    else {  //eeprom写过数据
                        break;
                    }
                }
                else{
                    i++;
                    if (i >= 3)
                    {
                        return 0; //表示没有eeprom或坏了
                    }
                }
            }
            for(int i = 1; i< 3;)
            {
                if(myeeprom_read(BASE_30DAY_USE_ADDR_DELAY+i*DCCRYPT_MAX_STR+10, &paramDelayCode30Day.code_size_30day, 1))
                {
                    if(paramDelayCode30Day.code_size_30day == 0xFF) {
                        i++;
                        continue;
                    }
                    else {
                        break;
                    }
                }
                else
                {
                    i++;
                    if (i == 3) //eeprom数据被破坏
                    {
                        return 0;
                    }
                }
            }
        }
        else
        {
            return 0;//无加密
        }
//        qDebug("111 product30DayUseStartDay is %s", paramDelayCode30Day.product30DayUseStartDay);
//        qDebug("222 code_size_30day is %d", paramDelayCode30Day.code_size_30day);
        /*读取机器编号*/
        if (paramDelayCode30Day.code_size_30day != 0xFF)
        {
            for(int i = 1; i < 3;)
            {
                if(myeeprom_read(BASE_30DAY_USE_ADDR_DELAY+i*DCCRYPT_MAX_STR+11, paramDelayCode30Day.product30DayUseOnlyIdEeprom, paramDelayCode30Day.code_size_30day))
                {
                    if (paramDelayCode30Day.product30DayUseOnlyIdEeprom[0]== 0xFF&&paramDelayCode30Day.product30DayUseOnlyIdEeprom[1]== 0xFF
                            &&paramDelayCode30Day.product30DayUseOnlyIdEeprom[2]== 0xFF&&paramDelayCode30Day.product30DayUseOnlyIdEeprom[3] == 0xFF) //表示数据无效
                    {
                        i++;
                        if (i == 3)
                        {
                            return 0;
                        }
                        continue;
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    i++;
                    if (i == 3)
                    {
                        return 0;
                    }
                }
            }
        }
        else
        {
            return 0;
        }
//        qDebug("product30DayUseOnlyIdEeprom is %s",paramDelayCode30Day.product30DayUseOnlyIdEeprom);
#endif
#ifdef Q_OS_WIN32
    return 0;   //windows环境不判断
#endif
    if(timeDccryptProcessIsRun == false)
    {
        /*启动定时保存参数功能*/
        QProcess *process = new QProcess;
        QStringList strList;
        strList<<"";
        process->startDetached("./zktimer",strList);

        timeDccryptProcessIsRun = true;
    }
    /*检测加密方式，累计计时加密及系统时间加密，判断加密是否到期*/
//    qDebug("delay_code_eeprom is %s", delay_code_eeprom);


//    ret = paramDelayCode.delayDecryptCode(paramDelayCode.delay_code_eeprom, wYear, wMonth, wDay, product_only_id_cur);
//    sprintf(paramDelayCode.limit_date_cur, "%04d%02d%02d", wYear, wMonth, wDay);

//    printf("limit_date_cur is %d \n", atoi(limit_date_cur));
//    printf("product_only_id_cur is %s\n", product_only_id_cur);

    time(&curtime);
    stru_curtime = localtime(&curtime);

//    getSysTimeFromE2prom(&savedtime);
//    getSysTimeFromEmmc(&savedtime2);
    tmptime = qMax(savedtime,savedtime2);

    if (curtime < tmptime){   //当前时间 < 保存的时间， RTC错误,用保存的时间替代当前时间
        stru_curtime = localtime(&tmptime);
        strftime(curYear ,64,"%Y",stru_curtime);
        strftime(curMonth,64,"%m",stru_curtime);
        strftime(curDay  ,64,"%d",stru_curtime);
        strftime(curHour ,64,"%H",stru_curtime);
        strftime(curMin  ,64,"%M",stru_curtime);
        strftime(curSec  ,64,"%S",stru_curtime);

        char time[64];
        sprintf(time, "date -s \"%04d-%02d-%02d %02d:%02d:%02d\"", atoi(curYear), atoi(curMonth),
        atoi(curDay),atoi(curHour),atoi(curMin),atoi(curSec));
        system(time);
        popen("hwclock -w","r");
    }
/*
    strftime(curDate,64,"%Y%m%d",stru_curtime);

    if (atoi(curDate) < atoi(limit_date_cur)) {  //加密未到期
        int yy1 = 1900+stru_curtime->tm_year;
        int mm1 = 1+stru_curtime->tm_mon;
        int dd1 = stru_curtime->tm_mday;
        int yy2 = int(wYear);
        int mm2 = int(wMonth);
        int dd2 = int(wDay);

        struGsh.struDccryptData.mCryptDayLast = myFlow.dayDistance(yy1, mm1, dd1, yy2, mm2, dd2);
        return 0;
    }
*/
    return 1;
}

bool myDelayCode::write30DayInfoToEeprom()
{
//    writeSysTimeToE2prom();
//    writeSysTimeToEmmc();

#ifdef Q_OS_UNIX
       myFlow.sleep(3);
        /* to eeprom */
       for(int i = 1; i < 3;i++) {
           /* 30天临时加密开启 */
            myeeprom_write(BASE_30DAY_USE_ADDR_DELAY+i*DCCRYPT_MAX_STR, &paramDelayCode30Day.product30DayUseStartEnable, 1);
            myeeprom_read(BASE_30DAY_USE_ADDR_DELAY+i*DCCRYPT_MAX_STR, &paramDelayCode30Day.product30DayUseStartEnable, 1);
            usleep(5000);
//            printf("product30DayUseStartEnable is %d\n",paramDelayCode30Day.product30DayUseStartEnable);

            /* 30天加密到期 */
            myeeprom_write(BASE_30DAY_USE_ADDR_DELAY+i*DCCRYPT_MAX_STR+1, &paramDelayCode30Day.product30DayUseOver, 1);
            myeeprom_read(BASE_30DAY_USE_ADDR_DELAY+i*DCCRYPT_MAX_STR+1, &paramDelayCode30Day.product30DayUseOver, 1);
            usleep(5000);
 //           printf("product30DayUseOver is %d\n",paramDelayCode30Day.product30DayUseOver);

            /* 30天使用计时日期 */
            myeeprom_write(BASE_30DAY_USE_ADDR_DELAY+i*DCCRYPT_MAX_STR+2, paramDelayCode30Day.product30DayUseStartDay, 8);
            myeeprom_read(BASE_30DAY_USE_ADDR_DELAY+i*DCCRYPT_MAX_STR+2, paramDelayCode30Day.product30DayUseStartDay, 8);
            usleep(5000);
   //         printf("product30DayUseStartDay is %s\n",paramDelayCode30Day.product30DayUseStartDay);

            /* 30天使用的机器编号长度，默认12个字节 */
            myeeprom_write(BASE_30DAY_USE_ADDR_DELAY+i*DCCRYPT_MAX_STR+10, &paramDelayCode30Day.code_size_30day, 1);
            myeeprom_read(BASE_30DAY_USE_ADDR_DELAY+i*DCCRYPT_MAX_STR+10, &paramDelayCode30Day.code_size_30day, 1);
            usleep(5000);
//            printf("code_size_30day is %d\n",paramDelayCode30Day.code_size_30day);

            /* 30天使用的机器编号 */
            myeeprom_write(BASE_30DAY_USE_ADDR_DELAY+i*DCCRYPT_MAX_STR+11, paramDelayCode30Day.product30DayUseOnlyIdEeprom, 12);
            myeeprom_read(BASE_30DAY_USE_ADDR_DELAY+i*DCCRYPT_MAX_STR+11, paramDelayCode30Day.product30DayUseOnlyIdEeprom, 12);
            usleep(5000);
//            printf("product30DayUseOnlyIdEeprom is %s\n",paramDelayCode30Day.product30DayUseOnlyIdEeprom);
            usleep(5000);
       }
       if(timeDccryptProcessIsRun == false)
       {
            /*启动定时保存参数功能*/
            QProcess *process = new QProcess;
            QStringList strList;
            strList<<"";
            process->startDetached("./zktimer",strList);

            timeDccryptProcessIsRun = true;
       }
       myFlow.sleep(1);

#endif

       return true;
}

void myDelayCode::clear30DayInfoEeprom()
{
    paramDelayCode30Day.product30DayUseStartEnable = 0xFA;  //表面已经全解过的屏，不需要再重新进行30天
    paramDelayCode30Day.product30DayUseOver = 0xFF;
    paramDelayCode30Day.code_size_30day = 0xFF;
    memset(paramDelayCode30Day.product30DayUseStartDay, 0xFF, 8);
    memset(paramDelayCode30Day.product30DayUseOnlyIdEeprom, 0xFF, DCCRYPT_MAX_ONLY_30DAY_ID);

    paramDelayCode30Day.write30DayInfoToEeprom();    //清除30天临时加密信息

    paramDelayCode30Day.fpagCodeErrCount = 0;
    paramDelayCode30Day.fpgaCodeErrCountWrite();

    paramDelayCode30Day.product30DayUseAccHour = 0;
    paramDelayCode30Day.product30DayUseAccHourWrite();
}

void myDelayCode::fpgaCodeErrCountGet()
{

#ifdef Q_OS_UNIX

    myeeprom_read(BASE_30DAY_FPGA_CODE_ERR_ADDR+1, &paramDelayCode30Day.fpagCodeErrCount, 1);

    if(paramDelayCode30Day.fpagCodeErrCount == 0xFF)
    {
        paramDelayCode30Day.fpagCodeErrCount = 0;
    }

#endif

    return;
}


void myDelayCode::fpgaCodeErrCountWrite()
{
#ifdef Q_OS_UNIX
    myeeprom_write(BASE_30DAY_FPGA_CODE_ERR_ADDR+1, &paramDelayCode30Day.uiLanguage, 1);
    usleep(5000);
#endif

    return;
}

void myDelayCode::getUiLanguage()
{

#ifdef Q_OS_UNIX
    myeeprom_read(BASE_UI_LANGUAGE_SET_ADDR+1, &paramDelayCode.uiLanguage, 1);
#endif

    return;
}


void myDelayCode::setUiLanguage()
{
#ifdef Q_OS_UNIX
    myeeprom_write(BASE_UI_LANGUAGE_SET_ADDR+1, &paramDelayCode.uiLanguage, 1);
    usleep(5000);
#endif

    return;
}

void myDelayCode::product30DayUseAccHourGet()
{
#ifdef Q_OS_UNIX
    myeeprom_read(BASE_30DAY_FPGA_CODE_ERR_ADDR+2, &paramDelayCode30Day.product30DayUseAccHour, 1);

    if(paramDelayCode30Day.product30DayUseAccHour == 0xFF)
    {
        paramDelayCode30Day.product30DayUseAccHour = 0;
    }
#endif

    return;
}

void myDelayCode::product30DayUseAccHourWrite()
{
#ifdef Q_OS_UNIX
    myeeprom_write(BASE_30DAY_FPGA_CODE_ERR_ADDR+2, &paramDelayCode30Day.product30DayUseAccHour, 1);
    usleep(5000);
#endif

    return;
}

/**************解密代码********/
myDccrypt::myDccrypt(QWidget *parent) :
    QDialog(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
//    setGeometry(QRect(0, 5+1, LCD_WIDTH, struCnfg.screenH-52));

    setGeometry(QRect(0, 60+1, LCD_WIDTH, struCnfg.screenH-122));
    setStyleSheet("background-color: rgb(190,190,190);");

    /*自运行程序*/
    timer =new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeOutSlt()));//timeoutslot()为自定义槽
    timer->start(100);

    /*槽函数*/
    connect(this,SIGNAL(inputDelayCodeSig())  ,this,SLOT(inputDelayCodeSlt()));
    connect(this,SIGNAL(explainDelayCodeSig()),this,SLOT(explainDelayCodeSlt()));
}

void myDccrypt::timeOutSlt()
{
    timer->stop();
    emit inputDelayCodeSig();
}

void myDccrypt::inputDelayCodeSlt()
{
    str.sprintf("%s%s", paramDelayCode.product_only_id_cur, paramDelayCode.limit_date_cur+2);

    myInputMethod inputPanel;
    inputPanel.setType(KB_DC);
    inputPanel.setTitleAndInput(g_myLan().input, str);
    inputPanel.setInputBtnText(g_myLan().ids_ok,g_myLan().ids_cancel,g_myLan().ids_delete,g_myLan().ids_space);
    if (inputPanel.exec() == QDialog::Accepted) {
        str = inputPanel.getText();
        qDebug()<<str;
        emit explainDelayCodeSig();
    }
    else {
        emit inputDelayCodeSig();
    }
}

void myDccrypt::explainDelayCodeSlt()
{
    int counter = 0;
    time_t curtime;
    FILE *fp;
    int ret = 0;

    const char *delay_char = str.toLatin1();
    QString delaycode = str;
    memcpy(paramDelayCode.delay_code, delay_char, DCCRYPT_MAX_STR);
    memset(paramDelayCode.limit_date, 0, DCCRYPT_MAX_LIMIT_DATE);
    memset(paramDelayCode.product_only_id, 0, DCCRYPT_MAX_ONLY_ID);

    paramDelayCode.wYear = 0;
    paramDelayCode.wMonth = 0;
    paramDelayCode.wDay = 0;

    MessageDialog msgBox;
    MessageDialog msgBox1("", g_myLan().dc_illegal, 0);

    ret = paramDelayCode.delayDecryptCode(paramDelayCode.delay_code, paramDelayCode.wYear, paramDelayCode.wMonth, paramDelayCode.wDay, paramDelayCode.product_only_id);
    if(ret == decrypt_succeed)
    {
        if(paramDelayCode.delayWhetherCodesMatch(paramDelayCode.delay_code_eeprom, paramDelayCode.delay_code))    //机器码匹配
        {
            sprintf(paramDelayCode.limit_date, "%04d%02d%02d", paramDelayCode.wYear, paramDelayCode.wMonth, paramDelayCode.wDay);

            str = QString("%1: %2\n%3: %4%5%6%7-%8%9-%10%11").arg(g_myLan().dc_productsn).arg(paramDelayCode.product_only_id).arg(g_myLan().dc_limit).arg(paramDelayCode.limit_date[0]).arg(paramDelayCode.limit_date[1]).arg(paramDelayCode.limit_date[2]).arg(paramDelayCode.limit_date[3]).arg(paramDelayCode.limit_date[4]).arg(paramDelayCode.limit_date[5]).arg(paramDelayCode.limit_date[6]).arg(paramDelayCode.limit_date[7]);

            msgBox.setText(str);
            ret = msgBox.exec();

            if (ret == QDialog::Accepted)
            {
                /*加密时间为无效时间*/
                if (atoi(paramDelayCode.curDate) >= atoi(paramDelayCode.limit_date)) {
                        msgBox1.setText(g_myLan().dc_illegal);
                    msgBox1.exec();
                    emit inputDelayCodeSig();
                    return;
                }
                /*彻底解密*/
                if (paramDelayCode.wYear == DCCRYPT_INVALID_YEAR) {
                    memset(paramDelayCode.limit_date_cur, 0xFF, DCCRYPT_MAX_LIMIT_DATE);
                    memset(paramDelayCode.product_only_id_cur, 0xFF, DCCRYPT_MAX_ONLY_ID);
                    memset(paramDelayCode.delay_code_eeprom, 0xFF, sizeof(paramDelayCode.delay_code_eeprom));
                    paramDelayCode.enable = 0xFF;
                    myFlow.savedelaycode(delaycode); // 保留全解码
                }
                else {
                    strcpy(paramDelayCode.delay_code_eeprom, paramDelayCode.delay_code);
                    paramDelayCode.enable = 0xAA;
                }



#ifdef Q_OS_UNIX
                /* 加密成功，重新保存当前加密的系统时间 */
                writeSysTimeToE2prom();
                writeSysTimeToEmmc();
                g_infoWidget().setLabelText(g_myLan().msg_applying);
                g_infoWidget().delayShow();

                myFlow.sleep(3);
                /* to eeprom */
                for(int i = 1; i < 3;i++) {
                    myeeprom_write(BASE_ADDR_DELAY+i*DCCRYPT_MAX_STR, &paramDelayCode.enable, 1);
                    myeeprom_read(BASE_ADDR_DELAY+i*DCCRYPT_MAX_STR, &paramDelayCode.enable, 1);
//                    printf("enable is %d\n",paramDelayCode.enable);
                    usleep(5000);
                    paramDelayCode.code_size = strlen(paramDelayCode.delay_code_eeprom);
                    myeeprom_write(BASE_ADDR_DELAY+i*DCCRYPT_MAX_STR+1, &paramDelayCode.code_size, 1);
                    myeeprom_read(BASE_ADDR_DELAY+i*DCCRYPT_MAX_STR+1, &paramDelayCode.code_size, 1);
//                    printf("code_size is %d\n",paramDelayCode.code_size);
                    usleep(5000);
                    myeeprom_write(BASE_ADDR_DELAY+i*DCCRYPT_MAX_STR+2, paramDelayCode.delay_code_eeprom, strlen(paramDelayCode.delay_code_eeprom));
                    myeeprom_read(BASE_ADDR_DELAY+i*DCCRYPT_MAX_STR+2, paramDelayCode.delay_code_eeprom, paramDelayCode.code_size);
//                    printf("code is %s\n",paramDelayCode.delay_code_eeprom);
                    usleep(5000);
                }

                myFlow.writeFpgaDccryptTime();
                myFlow.writeFpgaDccryptCode();

                paramDelayCode30Day.clear30DayInfoEeprom();
                myFlow.sleep(2);

                paramDelayCode30Day.fpagCodeErrCount = 0;
                paramDelayCode30Day.fpgaCodeErrCountWrite();
                myFlow.sleep(2);

                g_infoWidget().hide();
#endif
                accept();
            }
            else {
                emit inputDelayCodeSig();
                return;
            }
        }
        else
        {
            msgBox1.setText(g_myLan().dc_productsn_mismatch);
            msgBox1.exec();
            emit inputDelayCodeSig();
            return;
        }
    }
    else
    {
        msgBox1.setText(g_myLan().dc_productsn_mismatch);
        msgBox1.exec();
        emit inputDelayCodeSig();
        return;
    }
}

/*加密部分*/
myEncrypt::myEncrypt(QWidget *parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
//    setGeometry(QRect(0, 5+1, LCD_WIDTH, LCD_HEIGHT-52));
    setGeometry(QRect(0, 61, LCD_WIDTH, struCnfg.screenH-122));
    setStyleSheet("background-color: rgb(190,190,190);");

    bFlagSetNewTime = true;
    /*自运行程序*/
    timer =new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeOutSlt()));//timeoutslot()为自定义槽
    timer->start(100);

    /*槽函数*/
    connect(this,SIGNAL(inputDelayCodeSig())  ,this,SLOT(inputDelayCodeSlt()));
    connect(this,SIGNAL(explainDelayCodeSig()),this,SLOT(explainDelayCodeSlt()));
    connect(this,SIGNAL(setCurrentTimeSig())  ,this,SLOT(setCurrentTimeSlt()));
}


void myEncrypt::timeOutSlt()
{
    timer->stop();
    emit inputDelayCodeSig();
}

void myEncrypt::inputDelayCodeSlt()
{
    /* get current limit and product_only_id */
   if (strlen(paramDelayCode.delay_code_eeprom) > 0) {

        memset(paramDelayCode.delay_code, 0, DCCRYPT_MAX_STR);
        memset(paramDelayCode.limit_date, 0, DCCRYPT_MAX_LIMIT_DATE);
        memset(paramDelayCode.product_only_id, 0, DCCRYPT_MAX_ONLY_ID);

        paramDelayCode.wYear = 0;
        paramDelayCode.wMonth = 0;
        paramDelayCode.wDay = 0;

        paramDelayCode.delayDecryptCode(paramDelayCode.delay_code_eeprom, paramDelayCode.wYear, paramDelayCode.wMonth, paramDelayCode.wDay, paramDelayCode.product_only_id_cur);
        sprintf(paramDelayCode.limit_date_cur, "%04d%02d%02d", paramDelayCode.wYear, paramDelayCode.wMonth, paramDelayCode.wDay);
        bFlagSetNewTime = false;
    }

   str.sprintf("%s%s", paramDelayCode.product_only_id_cur, paramDelayCode.limit_date_cur+2);

   myInputMethod inputPanel;
   inputPanel.setType(KB_DC);
   inputPanel.setTitleAndInput(g_myLan().ids_apply, str);
   inputPanel.setInputBtnText(g_myLan().ids_ok,g_myLan().ids_cancel,g_myLan().ids_delete,g_myLan().ids_space);
   if (inputPanel.exec() == QDialog::Accepted) {
       str = inputPanel.getText();
       qDebug()<<str;
       emit explainDelayCodeSig();
   }
   else {
       reject();
//       emit inputDelayCodeSig();
   }
}

void myEncrypt::explainDelayCodeSlt()
{
    int counter = 0;
    time_t curtime;
    FILE *fp;
    int ret = 0;

    memset(paramDelayCode.delay_code, 0, DCCRYPT_MAX_STR);
    memset(paramDelayCode.limit_date, 0, DCCRYPT_MAX_LIMIT_DATE);
    memset(paramDelayCode.product_only_id, 0, DCCRYPT_MAX_ONLY_ID);

    paramDelayCode.wYear = 0;
    paramDelayCode.wMonth = 0;
    paramDelayCode.wDay = 0;

    const char *delay_char = str.toLatin1();
    QString delaycode = str;
    memcpy(paramDelayCode.delay_code, delay_char, DCCRYPT_MAX_STR);     //需要cp这么多长度吗？sunsh

    MessageDialog msgBox("", "", 0);
    MessageDialog msgBox1("", g_myLan().dc_illegal_used, 0);

    ret = paramDelayCode.delayDecryptCode(paramDelayCode.delay_code, paramDelayCode.wYear, paramDelayCode.wMonth, paramDelayCode.wDay, paramDelayCode.product_only_id);

    if(ret == decrypt_succeed)
    {
        if(strlen(paramDelayCode.delay_code_eeprom) == 0)
        {
            bFlagSetNewTime = true;
        }
        else
        {
            bFlagSetNewTime = false;
            if(!paramDelayCode.delayWhetherCodesMatch(paramDelayCode.delay_code_eeprom, paramDelayCode.delay_code))    //机器码匹配
            {
                bFlagSetNewTime = true;
                msgBox1.setText(g_myLan().dc_productsn_mismatch);
                msgBox1.exec();
                emit inputDelayCodeSig();
                return;
            }
        }
        sprintf(paramDelayCode.limit_date, "%04d%02d%02d", paramDelayCode.wYear, paramDelayCode.wMonth, paramDelayCode.wDay);
        str = QString("%1: %2\n%3: %4%5%6%7-%8%9-%10%11").arg(g_myLan().dc_productsn).arg(paramDelayCode.product_only_id).arg(g_myLan().dc_limit).arg(paramDelayCode.limit_date[0]).arg(paramDelayCode.limit_date[1]).arg(paramDelayCode.limit_date[2]).arg(paramDelayCode.limit_date[3]).arg(paramDelayCode.limit_date[4]).arg(paramDelayCode.limit_date[5]).arg(paramDelayCode.limit_date[6]).arg(paramDelayCode.limit_date[7]);

         msgBox.setText(str);
         ret = msgBox.exec();

         if (ret == QDialog::Accepted)
         {
#if 0
             /*加密码是否已经使用过*/
             if(paramDelayCode.delaycodeMatch(paramDelayCode.delay_code)){
                 msgBox1.setText(g_myLan().dc_illegal_used);
                 msgBox1.exec();
                 emit inputDelayCodeSig();
                 return;
             }
             else {/*保存加密码*/
                 if(!paramDelayCode.delaycodeSave(paramDelayCode.delay_code)) {
                     msgBox1.setText(g_myLan().dc_save_erro);
                     msgBox1.exec();
                     emit inputDelayCodeSig();
                     return;
                 }
             }
#endif
             /*彻底解密*/
             if(paramDelayCode.wYear == DCCRYPT_INVALID_YEAR)
             {
                 memset(paramDelayCode.limit_date_cur, 0xFF, DCCRYPT_MAX_LIMIT_DATE);
                 memset(paramDelayCode.product_only_id_cur, 0xFF, DCCRYPT_MAX_ONLY_ID);
                 memset(paramDelayCode.delay_code_eeprom, 0xFF, sizeof(paramDelayCode.delay_code_eeprom));
                 paramDelayCode.enable = 0xFF;
                 myFlow.savedelaycode(delaycode); // 保留全解码

             }
             else {
                 strcpy(paramDelayCode.delay_code_eeprom, paramDelayCode.delay_code);
                 paramDelayCode.enable = 0xAA;
             }

//             writeSysTimeToE2prom();
//             writeSysTimeToEmmc();

 #ifdef Q_OS_UNIX
             g_infoWidget().setLabelText(g_myLan().msg_applying);
             g_infoWidget().delayShow();

            myFlow.sleep(3);
             /* to eeprom */
            for(int i = 1; i < 3;i++) {
                 myeeprom_write(BASE_ADDR_DELAY+i*DCCRYPT_MAX_STR, &paramDelayCode.enable, 1);
                 myeeprom_read(BASE_ADDR_DELAY+i*DCCRYPT_MAX_STR, &paramDelayCode.enable, 1);
                 usleep(5000);
 //                printf("enable is %d\n",paramDelayCode.enable);
                 paramDelayCode.code_size = strlen(paramDelayCode.delay_code_eeprom);
                 myeeprom_write(BASE_ADDR_DELAY+i*DCCRYPT_MAX_STR+1, &paramDelayCode.code_size, 1);
                 myeeprom_read(BASE_ADDR_DELAY+i*DCCRYPT_MAX_STR+1, &paramDelayCode.code_size, 1);
                 usleep(5000);
//                 printf("code_size is %d\n",paramDelayCode.code_size);
                 myeeprom_write(BASE_ADDR_DELAY+i*DCCRYPT_MAX_STR+2, paramDelayCode.delay_code_eeprom, strlen(paramDelayCode.delay_code_eeprom));
                 myeeprom_read(BASE_ADDR_DELAY+i*DCCRYPT_MAX_STR+2, paramDelayCode.delay_code_eeprom, paramDelayCode.code_size);
//                 printf("code is %s\n",paramDelayCode.delay_code_eeprom);
                 usleep(5000);
             }

            myFlow.writeFpgaDccryptTime();
            myFlow.writeFpgaDccryptCode();

            paramDelayCode30Day.clear30DayInfoEeprom();
            myFlow.sleep(2);

            paramDelayCode30Day.fpagCodeErrCount = 0;
            paramDelayCode30Day.fpgaCodeErrCountWrite();
            myFlow.sleep(2);

            g_infoWidget().hide();
 #endif
             if (bFlagSetNewTime) {//启动设置时间
                 emit setCurrentTimeSig();
                 return;
             }
             else {
                 reject();
             }
         }
         else {
             reject();
         }
    }
    else
    {
        msgBox1.setText(g_myLan().dc_productsn_mismatch);
        msgBox1.exec();
        emit inputDelayCodeSig();
        return;
    }
}

void myEncrypt::setCurrentTimeSlt()
{
    time_t curtime;
    FILE *fp;

    MySetDateTime setTime;
    int ret = setTime.exec();
    if (ret == QDialog::Accepted) {
//        writeSysTimeToE2prom();
//        writeSysTimeToEmmc();
        myFlow.sleep(1);
        accept();
    }
    else
    {
        reject();
    }
}

#if 1
/*
**
*
**********30天解密代码 *****
*
*
*/
myDccrypt30Day::myDccrypt30Day(QWidget *parent) :
    QDialog(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
//    setGeometry(QRect(0, 5+1, LCD_WIDTH, LCD_HEIGHT-52));

    setGeometry(QRect(0, 60+1, LCD_WIDTH, struCnfg.screenH-122));
    setStyleSheet("background-color: rgb(190,190,190);");

    /*自运行程序*/
    timer30Day =new QTimer(this);
    connect(timer30Day,SIGNAL(timeout()),this,SLOT(timeOut30DaySlt()));//timeoutslot()为自定义槽
    timer30Day->start(100);

    /*槽函数*/
    connect(this,SIGNAL(inputDelayCode30DaySig())  ,this,SLOT(inputDelayCode30DaySlt()));
    connect(this,SIGNAL(explainDelayCode30DaySig()),this,SLOT(explainDelayCode30DaySlt()));

    connect(this,SIGNAL(inputDelayCode30DayToNormalSig()),this,SLOT(inputDelayCode30DayToNormalSlt()));
    connect(this,SIGNAL(explainDelayCode30DayToNoarmalEncryptSig()),this,SLOT(explainDelayCodeToNoarmalEncryptSlt()));
}

void myDccrypt30Day::timeOut30DaySlt()
{
    timer30Day->stop();
    emit inputDelayCode30DaySig();
}

void myDccrypt30Day::inputDelayCode30DaySlt()
{
    paramDelayCode30Day.get30DayInfofromEeprom();
    str30Day.sprintf("%s", paramDelayCode30Day.product30DayUseOnlyIdEeprom);

//    qDebug()<<"str30Day is "<<str30Day;
    myInputMethod inputPanel;
    inputPanel.setType(KB_DC);
    inputPanel.setTitleAndInput(g_myLan().input+"-100", str30Day);
    inputPanel.setInputBtnText(g_myLan().ids_ok,g_myLan().ids_cancel,g_myLan().ids_delete,g_myLan().ids_space);
    if (inputPanel.exec() == QDialog::Accepted) {
        str30Day = inputPanel.getText();
        qDebug()<<str30Day;
        emit explainDelayCode30DaySig();
    }
    else {
        emit inputDelayCode30DaySig();
    }
}

void myDccrypt30Day::explainDelayCode30DaySlt()
{
    int counter = 0;
    time_t curtime;
    FILE *fp;
    int ret = 0;

    const char *delay_char_30Day = str30Day.toLatin1();
    memcpy(paramDelayCode30Day.delay_code, delay_char_30Day, DCCRYPT_MAX_STR);
    memset(paramDelayCode30Day.limit_date, 0, DCCRYPT_MAX_LIMIT_DATE);
    memset(paramDelayCode30Day.product30DayUseOnlyCurId, 0, DCCRYPT_MAX_ONLY_ID);

    paramDelayCode30Day.wYear = 0;
    paramDelayCode30Day.wMonth = 0;
    paramDelayCode30Day.wDay = 0;

    paramDelayCode30Day.get30DayInfofromEeprom();

    MessageDialog msgBox;
    MessageDialog msgBox1("", g_myLan().dc_illegal, 0);

    ret = paramDelayCode30Day.delayDecryptCode(paramDelayCode30Day.delay_code, paramDelayCode30Day.wYear, paramDelayCode30Day.wMonth,
                                               paramDelayCode30Day.wDay, paramDelayCode30Day.product30DayUseOnlyCurId);

    if(ret == decrypt_succeed)
    {
//        qDebug("333 product30DayUseOnlyIdEeprom is %s, product30DayUseOnlyCurId is %s", paramDelayCode30Day.product30DayUseOnlyIdEeprom,paramDelayCode30Day.product30DayUseOnlyCurId);
        if(strcmp(paramDelayCode30Day.product30DayUseOnlyIdEeprom, paramDelayCode30Day.product30DayUseOnlyCurId) == 0)   //机器码匹配，30天解密完成，进入正常加密
        {
            sprintf(paramDelayCode30Day.limit_date_cur, "%04d%02d%02d", paramDelayCode30Day.wYear, paramDelayCode30Day.wMonth, paramDelayCode30Day.wDay);
            emit inputDelayCode30DayToNormalSig();
        }
        else
        {
            msgBox1.setText(g_myLan().dc_productsn_mismatch);
            msgBox1.exec();
            emit inputDelayCode30DaySig();
            return;
        }
    }
    else
    {
//        qDebug("delayDecryptCode is decrypt_error_code_format");
        msgBox1.setText(g_myLan().dc_productsn_mismatch);
        msgBox1.exec();
        emit inputDelayCode30DaySig();
        return;
    }
}

void myDccrypt30Day::inputDelayCode30DayToNormalSlt()
{
    strNormal = "";
    myInputMethod inputPanel;
    inputPanel.setType(KB_DC);
    inputPanel.setTitleAndInput(g_myLan().input, strNormal);
    inputPanel.setInputBtnText(g_myLan().ids_ok,g_myLan().ids_cancel,g_myLan().ids_delete,g_myLan().ids_space);
    if (inputPanel.exec() == QDialog::Accepted) {

        strNormal = inputPanel.getText();
        qDebug()<<strNormal;
        if(str30Day == strNormal)   //加密码与出厂解密码不能一致
        {
            emit inputDelayCode30DayToNormalSig();
            return;
        }

        emit explainDelayCode30DayToNoarmalEncryptSig();
    }
    else
    {
        emit inputDelayCode30DayToNormalSig();
    }
}

void myDccrypt30Day::explainDelayCodeToNoarmalEncryptSlt()
{
    int counter = 0;
    time_t curtime;
    FILE *fp;
    int ret = 0;

    const char *delay_char = strNormal.toLatin1();

    paramDelayCode30Day.get30DayInfofromEeprom();

    memcpy(paramDelayCode30Day.delay_code, delay_char, DCCRYPT_MAX_STR);
    memset(paramDelayCode30Day.limit_date, 0, DCCRYPT_MAX_LIMIT_DATE);
    memset(paramDelayCode30Day.product30DayUseOnlyCurId, 0, DCCRYPT_MAX_ONLY_ID);

    paramDelayCode30Day.wYear = 0;
    paramDelayCode30Day.wMonth = 0;
    paramDelayCode30Day.wDay = 0;

    MessageDialog msgBox;
    MessageDialog msgBox1("", g_myLan().dc_illegal, 0);

    ret = paramDelayCode30Day.delayDecryptCode(paramDelayCode30Day.delay_code, paramDelayCode30Day.wYear, paramDelayCode30Day.wMonth, paramDelayCode30Day.wDay, paramDelayCode30Day.product30DayUseOnlyCurId);

    if(strcmp(paramDelayCode30Day.product30DayUseOnlyIdEeprom, paramDelayCode30Day.product30DayUseOnlyCurId) == 0)  //正常加密码与出厂解密码的唯一ID不能一致，但到期时间一致
    {
        emit inputDelayCode30DayToNormalSig();
        return;
    }

//    ret = decrypt_succeed;
    if(ret == decrypt_succeed)
    {
        sprintf(paramDelayCode30Day.limit_date, "%04d%02d%02d", paramDelayCode30Day.wYear, paramDelayCode30Day.wMonth, paramDelayCode30Day.wDay);

        if(atoi(paramDelayCode30Day.limit_date) != atoi(paramDelayCode30Day.limit_date_cur)) //到期时间要一致
        {
            emit inputDelayCode30DayToNormalSig();
            return;
        }
        strNormal = QString("%1: %2\n%3: %4%5%6%7-%8%9-%10%11").arg(g_myLan().dc_productsn).arg(paramDelayCode30Day.product30DayUseOnlyCurId).arg(g_myLan().dc_limit).arg(paramDelayCode30Day.limit_date[0]).arg(paramDelayCode30Day.limit_date[1]).arg(paramDelayCode30Day.limit_date[2]).arg(paramDelayCode30Day.limit_date[3]).arg(paramDelayCode30Day.limit_date[4]).arg(paramDelayCode30Day.limit_date[5]).arg(paramDelayCode30Day.limit_date[6]).arg(paramDelayCode30Day.limit_date[7]);

         msgBox.setText(strNormal);
         ret = msgBox.exec();

         if (ret == QDialog::Accepted)
         {
             /*彻底解密*/
             if(paramDelayCode30Day.wYear == DCCRYPT_INVALID_YEAR)
             {
                 memset(paramDelayCode30Day.limit_date_cur, 0xFF, DCCRYPT_MAX_LIMIT_DATE);
                 memset(paramDelayCode30Day.product30DayUseOnlyCurId, 0xFF, DCCRYPT_MAX_ONLY_ID);
                 memset(paramDelayCode30Day.delay_code_eeprom, 0xFF, sizeof(paramDelayCode30Day.delay_code_eeprom));
                 paramDelayCode30Day.enable = 0xFF;
             }
             else {
                 strcpy(paramDelayCode30Day.delay_code_eeprom, paramDelayCode30Day.delay_code);
                 paramDelayCode30Day.enable = 0xAA;
             }

//             writeSysTimeToE2prom();
//             writeSysTimeToEmmc();

 #ifdef Q_OS_UNIX
             g_infoWidget().setLabelText(g_myLan().msg_applying);
             g_infoWidget().delayShow();

            myFlow.sleep(3);
             /* to eeprom */
            for(int i = 1; i < 3;i++) {
                 myeeprom_write(BASE_ADDR_DELAY+i*DCCRYPT_MAX_STR, &paramDelayCode30Day.enable, 1);
                 myeeprom_read(BASE_ADDR_DELAY+i*DCCRYPT_MAX_STR, &paramDelayCode30Day.enable, 1);
                 myeeprom_read(BASE_ADDR_DELAY+i*DCCRYPT_MAX_STR, &paramDelayCode.enable, 1);   //为了将密码写入FPGA
                 usleep(5000);
//                 printf("enable is %d\n",paramDelayCode30Day.enable);
                 paramDelayCode30Day.code_size = strlen(paramDelayCode30Day.delay_code_eeprom);
                 myeeprom_write(BASE_ADDR_DELAY+i*DCCRYPT_MAX_STR+1, &paramDelayCode30Day.code_size, 1);
                 myeeprom_read(BASE_ADDR_DELAY+i*DCCRYPT_MAX_STR+1, &paramDelayCode30Day.code_size, 1);
                 myeeprom_read(BASE_ADDR_DELAY+i*DCCRYPT_MAX_STR+1, &paramDelayCode.code_size, 1);
                 usleep(5000);
//                 printf("code_size is %d\n",paramDelayCode30Day.code_size);
                 myeeprom_write(BASE_ADDR_DELAY+i*DCCRYPT_MAX_STR+2, paramDelayCode30Day.delay_code_eeprom, strlen(paramDelayCode30Day.delay_code_eeprom));
                 myeeprom_read(BASE_ADDR_DELAY+i*DCCRYPT_MAX_STR+2, paramDelayCode30Day.delay_code_eeprom, paramDelayCode30Day.code_size);
                 myeeprom_read(BASE_ADDR_DELAY+i*DCCRYPT_MAX_STR+2, paramDelayCode.delay_code_eeprom, paramDelayCode30Day.code_size);
//                 printf("code is %s\n",paramDelayCode30Day.delay_code_eeprom);
                 usleep(5000);
            }
            myFlow.writeFpgaDccryptTime();
            myFlow.writeFpgaDccryptCode();

            paramDelayCode30Day.clear30DayInfoEeprom();

            myFlow.sleep(2);

            paramDelayCode30Day.fpagCodeErrCount = 0;
            paramDelayCode30Day.fpgaCodeErrCountWrite();
            myFlow.sleep(2);

            g_infoWidget().hide();
 #endif
            accept();
//             if (bFlagSetNewTime) {//启动设置时间
//                 emit setCurrentTime30DaySig();
//                 return;
//             }
//             else {
//                 reject();
//             }
         }
         else {
             reject();
             emit inputDelayCode30DayToNormalSig();
             return;
         }
    }
    else
    {
        qDebug("delayDecryptCode is decrypt_error_code_format");
        msgBox1.setText(g_myLan().dc_productsn_mismatch);
        msgBox1.exec();
        emit inputDelayCode30DayToNormalSig();
        return;
    }
}
#endif
