#ifndef MYDELAYCODE_H
#define MYDELAYCODE_H

#include <QtGui>
#include <QDialog>
#include "time_encrypt.h"
#include "globalparams.h"
#include "messagedialog.h"
#include "myinputmethod.h"
#ifdef Q_OS_UNIX
    #include "myeeprom.h"
#endif

#define DCCRYPT_INVALID_YEAR    2088
#define DCCRYPT_TEMPORARY_DAYS  30
#define DCCRYPT_30DAY_ACC_HOUR  100 //100小时

#define DCCRYPT_MAX_STR		64
#define DCCRYPT_MAX_DC      32
#define DCCRYPT_MIN_DC      6

#define DCCRYPT_LEN_PS     13
#define DCCRYPT_LEN_LIMIT  8

#define DCCRYPT_MAX_ONLY_ID         20
#define DCCRYPT_MAX_LIMIT_DATE		16
#define DCCRYPT_MAX_YEAR_LENGTH     6
#define DCCRYPT_MAX_DAY_LENGTH      4
#define DCCRYPT_THIS_CODE_SIZE      27

#define CNF_FILE_CURTIME      "/sdcard/cnf/cnf.curtime"
#define CNF_FILE_DELAYCODE    "/sdcard/cnf/cnf.delaycode"

#define BASE_ADDR_DELAY                 0x00000000
#define BASE_30DAY_USE_ADDR_DELAY       0x00000100  //从第256个字节开始，
#define BASE_30DAY_FPGA_CODE_ERR_ADDR	0x00000200  //从第512个字节开始，

#define BASE_UI_LANGUAGE_SET_ADDR       0x00000300  //从第768个字节开始，

#define DCCRYPT_MAX_ONLY_30DAY_ID	16  //，

enum GET_FPGA_CRYPT_STATUS
{
    ENCRYPT_FPGA_NO_PACKET = 0,         //未获取到加密回包
    ENCRYPT_FPGA_CODE_FORMAT_ERROR = 1, //获取的密码格式错误
    ENCRYPT_FPGA_CODE_MISMATCH_LCD  = 2,    //获取的加密码不匹配,使用上位机
    ENCRYPT_FPGA_CODE_MISMATCH_FPGA  = 3,    //获取的加密码不匹配,使用上位机
    ENCRYPT_FPGA_CODE_MATCH  = 4,       //已加密机器，获取的加密码匹配
    ENCRYPT_FPGA_CODE_FORMAT_OK  = 5,   //未加密机器，获取的密码正确，需要写入ARM板
    ENCRYPT_FPGA_HAS_DCCRYPT  = 6,      //获取的加密信息，已全解
    ENCRYPT_FPGA_NO_ENCRYPT  = 7,        //读取到的全为0xFF,表示未加密的接口板
    ENCRYPT_FPGA_BURNING  = 8,       //
};

class myDelayCode : public QObject
{
public:
    explicit myDelayCode(QObject *parent = 0);
    char curDate[64];
    char delay_code_eeprom[DCCRYPT_MAX_STR];    // eeprom中存的加密code
    char limit_date_cur[DCCRYPT_MAX_LIMIT_DATE];        //从eeprom中的code解析出的到期时间20200409
    char product_only_id_cur[DCCRYPT_MAX_ONLY_ID];   //从eeprom中的code解析出的onlyID

    char delay_code[DCCRYPT_MAX_STR];       // 输入的加密码
    char limit_date[DCCRYPT_MAX_LIMIT_DATE];            // 从输入的加密码，解析的到期时间
    char product_only_id[DCCRYPT_MAX_ONLY_ID];       // 从输入的加密码，解析的onlyID

    char enable;   //enable == 0xAA,表示加密使能
    char code_size;  //密码长度，0xFF表示无效

    // 30天加密参数
    char product30DayUseStartEnable;                 // 机器组装开始
    char product30DayUseOver;                    // 临时使用的30天加密到期
    char product30DayUseStartDay[8];                  // 机器临时使用日期 格式yyyymmdd

    char code_size_30day;  //30天机器编号长度，0xFF表示无效
    char product30DayUseOnlyCurId[DCCRYPT_MAX_ONLY_30DAY_ID];
    char product30DayUseOnlyIdEeprom[DCCRYPT_MAX_ONLY_30DAY_ID];        //30天到期自动生成或接口板连续读到的加密码格式连续出错3次，onlyID,


    unsigned short wYear;
    unsigned short wMonth;
    unsigned short wDay;

    unsigned short* pYear;
    unsigned short* pMonth;
    unsigned short* pDay;

    char fpagCodeErrCount;
    char product30DayUseAccHour;
    char uiLanguage;        // 0xAA国外, 其余只显示中文
	   
    int delayCodeCheck();
    int delaycodeMatch(const char *str);		// delay code match
    int delaycodeSave(char *str);		// delay code save
    void delayCodeClear();

    void fpgaCodeErrCountGet();
    void fpgaCodeErrCountWrite();

    void product30DayUseAccHourGet();
    void product30DayUseAccHourWrite();

    /*输入加密码，输出到期时间与唯一码*/
    int delayDecryptCode(char* code, unsigned short wYear, unsigned short wMonth, unsigned short wDay, char* onlyID);

    /*两个加密码是否匹配，唯一码相同的匹配，返回true*/
    bool delayWhetherCodesMatch(char* code1, char* code2);
    void writeDelaycode2eeprom();

     /* */
    int get30DayInfofromEeprom();  //1. 30天使用标志:1Byte，0xAA表示使能，其它：不使能；
                                    //2. 开始日期：4Bytes，YYYYMMDD
                                    //3. 唯一ID:12 Bytes
                                    //4. 预留7Bytes
    bool write30DayInfoToEeprom();  //1. 30天使用标志:1Byte，0xAA表示使能，其它：不使能；
                                    //2. 开始日期：4Bytes，YYYYMMDD
                                    //3. 唯一ID:12 Bytes
                                    //4. 预留7Bytes

    void clear30DayInfoEeprom();

    void getUiLanguage();
    void setUiLanguage();
};
extern myDelayCode paramDelayCode;
extern myDelayCode paramDelayCode30Day; //30天临时加密
extern bool timeDccryptProcessIsRun;

/*解密*/
class myDccrypt : public QDialog
{
    Q_OBJECT
public:
    explicit myDccrypt(QWidget *parent = 0);
    QTimer *timer;
    QString str;    //界面上输入的密码

signals:
    void inputDelayCodeSig();
    void explainDelayCodeSig();
    void setCurrentTimeSig();
    
public slots:
    void timeOutSlt();
    void inputDelayCodeSlt();
    void explainDelayCodeSlt();
};
/*加密*/
class myEncrypt : public QDialog
{
    Q_OBJECT
public:
    explicit myEncrypt(QWidget *parent = 0);
    bool bFlagSetNewTime;
    QTimer *timer;
    QString str;

signals:
    void inputDelayCodeSig();
    void explainDelayCodeSig();
    void setCurrentTimeSig();

public slots:
    void timeOutSlt();
    void inputDelayCodeSlt();
    void explainDelayCodeSlt();
    void setCurrentTimeSlt();
};

/***********下面是临时加密解密30天***********************************/
/*解密30天*/
class myDccrypt30Day : public QDialog
{
    Q_OBJECT
public:
    explicit myDccrypt30Day(QWidget *parent = 0);
    bool bFlagSetNewTime;
    QTimer *timer30Day;
    QString strNormal;    //界面上输入的密码
    QString str30Day;    //界面上输入的密码,30天

    char product30DayUseOnlyId[DCCRYPT_MAX_ONLY_30DAY_ID];
signals:
    void inputDelayCode30DaySig();
    void explainDelayCode30DaySig();
    void inputDelayCode30DayToNormalSig();
    void explainDelayCode30DayToNoarmalEncryptSig();
    void setCurrentTime30DaySig();

public slots:
    void timeOut30DaySlt();
    void inputDelayCode30DaySlt();
    void explainDelayCode30DaySlt();
    void inputDelayCode30DayToNormalSlt();
    void explainDelayCodeToNoarmalEncryptSlt();
};

#endif // MYDELAYCODE_H
