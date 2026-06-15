#ifndef MQTTSRV_H
#define MQTTSRV_H

#include "mosquitto.h"
#include "mosquittopp.h"
#include <QDebug>
#include "data/myjson.h"
#include "globalparams.h"
#include "globalflow.h"
#include "global.h"
#include "constant.h"
#include "logger.h"

#include <QWaitCondition>
#include <QMutex>

#define MQTT_TEST 1
#define HTTP_TEST 1

enum ErrorCode
{
    SYSTEM_ERROR            =-1, //系统错误
    SUCCESS                 = 0, //成功
    NONE                    = 1, //缺省
    UNKNOWN_CMD             = 2, //未知命令
    CMD_PARA_ERROR          = 3, //参数错误
    GLOBAL_VIEW_ERROR       = 4, //全局精度视角错误
    GLOBAL_PARA_ERROR       = 5, //全局精度参数错误
    MEM_SETING              = 6, //切换方案
    NONE_EXECUTE            = 7, //无需修改
    FEED_ING                = 8, //正在供料
    NOT_MAIN_WND            = 9, //不在主界面，不执行
    CLEAN_ING               = 10,//正在清灰
    VALVE_TESTING           = 11,//远程测阀
    ALARM_PRESSUER          = 12,//气压报警
    RECEIVE                 = 13,//指令已收到
    NOT_EXIST               = 14,//文件不存在
    VERSIONSAME             = 15,//文件相同，不升级
    USERREJECT              = 16,//用户拒绝，不升级
    FILEERROR               = 17,//文件异常，不升级
    DOWNLOADERROR           = 18,//升级文件下载失败
    UPDATEFAIL              = 19,//升级失败
    M_TIMEOUT               = 20,//超时
    IMG_NOT_CORRECT         = 21,//采图不完整
    NETWORK_ERR             = 22,//网络异常
    UPLOAD_ERR              = 23,//上传失败
    OSS_REQUESRT_ERR        = 24,//oss权鉴申请失败
    OSS_UPLOAD_ERR          = 25,//oss上传失败

    MODEL_FILE_MISS         = 27,//模型文件缺少错误
    MODEL_FILE_MISMATCH     = 28,//模型文件不匹配错误
    IPC_CONNECT_FAIL        = 29,//工控机连接失败错误
    FILE_UPGRADING          = 30,//升级中
    MODEL_FILE_UPGARDE_FAIL = 31,//模型文件升级失败
    CFG_FILE_UPGRADE_FAIL   = 32,//配置文件升级失败
    OTHER_ERR               = 33,//其他错误
    DOWNLOAD_SUCCESS        = 34,//升级文件下载成功
    LOCKED_ERROR     = 35,            // 当前不在锁屏状态
    OCCUPY_ERROR     = 36,            // 锁屏占用报错
    REMOTE_CANCEL    = 37,            // 远程取消确认
    NOT_USER_WINDOW  = 38,             // 不在用户权限下 ， 不执行指令
    CAPTURE_ERROR    = 39,             // 全部失败
    PARTIAL_SUCCESS  = 40,             //部分成功
    MACHINE_MISMATCH        = 41,     //机型不匹配
    CMD_ISRUNNING        = 42,     //其他指令正在执行
    ErrorCode_END,
};

enum ReportFlag
{
    Normal_Send,                 //主动上传
    Request_Send,                //请求上传
    Normal_Resend,               //主动上传数据补发
    Request_Resend,              //请求上传数据补发
};

// [zcy 2026.06.15] mqtt config
// 为了适配 MQTT 的第三方支持和各项文件依赖所进行的改动
#ifdef Q_OS_UNIX
class mqttsrv:public mosqpp::mosquittopp
{
public:
    mqttsrv():mosquittopp()
    {

    }
    mqttsrv(const char *id,bool clean_session=false):mosquittopp(id)
    {
        m_connect_name = id;
    }
    void on_connect(int rc)
    {
        qDebug()<<"on_connect";
    }
    void on_disconnect()
    {
        qDebug()<<"on_disconnect";
    }
    void on_publish(int mid)
    {
        qDebug()<<"on_publish";
    }
    void on_subscribe(int mid, int qos_count, const int *granted_qos);	//订阅回调函数
    void on_message(const struct mosquitto_message *message);			//订阅主题接收到消息

    string m_connect_name;
public:
    string strRcv;

    QList<string> recvMsgList;          //收到mqtt指令队列
    QList<string> sendMsgList;          //待发送mqtt消息队列
    QList<int>    sendMsgListCnt;       //待发送次数
    QMutex m_paraseLock;
};


/* mqtt通信线程，接收服务端控制指令，并反馈执行状态码 */
class mqttThread : public QThread
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

    public:
        explicit mqttThread(QThread *parent = 0);

    void doParseMessage();
    void reconnect();

    mqttsrv *test;
    int networkConnectIsOk;
    QString topic;
    string m_connect_name;
    QString m_connect_IP;

protected:
    void run();

};


class mqttMsgParaseThread : public QThread
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

    public:
        enum UpdateFileType{
                 UPDATE_NULL,
                 UPDATE_ZKSORT,
                 UPDATE_IPCMODEL
};
    explicit mqttMsgParaseThread(QThread *parent = 0);

    void doParseMessage();

    int onCmdFeed(string cmd_value);
    int onOpenSwitch(string cmd_value);
    int onOpenSwitchs(string cmd_value);
    int onSaveCfg(void);
    int onParaUpload(QString cmd_value);
    void onShutDown(void);
    int onGlobalParaSet(string cmd_value);
    int onMemSet(string cmd_value,string uuid);
    void onCmdReturn(string uuid,string cmd,int errorcode);
    int onVPNPushCert(string cmd_value);
    int onVPNConnect(string cmd_value);
    int onCaptureImage(string cmd_value,string tmp);
    int onLogfileUpload(string cmd_value);
    int onCnffileUpload(void);
    int onWipeSpeedUpload(void);
    int onBlowCountsUpload(void);
    int onVavleAudioUpload(void);
    int onCaptureAllImage(void);
    int onCaptureAllWave(void);
    void uploadPara();
    void upLoadSchemeInfo();
    void onUpdateZKSort(string cmd_value);//添加上位机远程升级接口 2021/3/19
    void onShellCmd(string cmd_value);

    // 添加远程服务接口 2020/8/27
    void uploadRealTimePara();
    void uploadMachinePara();
    int uploadPartsStatusInfo();
    void uploadParaAlarm();
    int uploadStatistics();
    void uploadRemoteAccess(int);

    int uploadLogFile(string date ="");

    //添加本地缓存接口 2020/9/24
    void cacheLocalRemoteInfo(string url,string val);
    void uploadLocalRemoteInfo();

    //添加远程采图接口 2020/11/27
    int uploadCaptureImage();

    //添加远程波形采集接口 2020/12/8
    int uploadCaptureWave(int Layer = 0,int View =0,int Unit=0);

    //添加喷阀吹气次数上传接口 2020/12/24
    int uploadBlowCounts();

    void doStateChange(QString tmp);
    void uploadMemsetReturn();//上传切换成功
    //远程测阀模块
    int onAllEjtTest(QString cmd_value);
    int onSglEjtTest(QString cmd_value);
    int onReSetEjt(QString cmd_value);
    int onEjtTestMode(QString cmd_value);

    //深度学习机器新增接口 2022/2/15
    int uploadIpcPartsStatusInfo(void);
    void onIpcInfoUpload(string cmd_value);

    int getModeList();
    int uploadModelEvaluate();
    int onRemotecontrol(int cmd_value,int time, QString phonenum);
    void uploadRemoteControl();
    int onModelParamsUpload();
    void  uploadOneKeyFixInfo();
    int onSeriesBlowCounts(int total_time,int cmd_fre);
    int onModelParamsSet(string);
    int offlineDetection(string); // 下线检测
    void onRemoteOneKeyTest(string); //  一键调试
    void uploadOnekeytest();


    bool isrunning;
    int m_uploadStat;   //0,不在上传，1,正常上传

    int n_uploadPartsStatusFlag;
    int n_uploadAlarmFlag;
    int n_uploadWipeSpeedFlag;
    int n_uploadImageFlag;
    int n_uploadWaveFlag;
    int n_uploadBlowCountsFlag;
    int n_uploadStatisticsFlag;
    int n_uploadRealTimeParaFlag; // 1:上传实时参数 0:完成上传
    int n_uploadSchemeInfoFlag;
    int n_uploadValveAudioFlag;
    int n_uploadIpcNetTestInfoFlag;
    int n_uploadLogFileFlag; // 1:上传当日
    int n_getModelListFlag;
    int n_uploadModelEvaluate;  // 上传评价
    int n_downLoadFlag;         // 下载模型
    int n_uploadRemoteControlFlag;
    int n_uploadModelParamsFlag;

    int n_uploadOneKeyFixFlag; //

    string msg;

    QString n_imagePath;
    string m_tmp;
    string m_value;
    string m_uuid;
    string remotelock_uuid;

    QString userphonenum;  //当前小程序用户手机号

    int reportFlag; //上报数据标记，0:主动上报,1:手动上报,2:主动上报补发，3：手动上报补发
    int sendFailedCnt; //连续上传三次失败，需进行联网判断

    int m_nextWeek;
    int m_updateFileType;

    Json::Value resultinfo; //下线检测结果
    Json::Value onekeytest; // 一键测试结果
signals:
    void cmd_OpenSwitch(bool);
    void cmd_Feed(QStringList);
    void cmd_updatezksort_ask(QVariant);
    void cmd_updateIpcModel_ask(QVariant);
    void cmd_uploadIpcInfo_ask();
    void getModelistResult_sig();
    void StartRemoteControl(QString);
    void EndRemoteControl();
    void SeriesBlowCount(int,int);


protected:
    void run();

private slots:
    void imgaeCaptureOver(int,QString);
    void waveCaptureOver();
    void blowCountsCaptureOver();
    void DealStartRemoteControl(QString phonenum);
        void DealSeriesBlowCount(int,int);

private:
    string m_version;
};


extern mqttThread *myMqttThread;
extern mqttMsgParaseThread *myMqttMsgParaseThread;
#endif

#endif // GLOBALPARAMS_H
