#ifndef GLOBALPARAMS_H
#define GLOBALPARAMS_H

#include "constant.h"
#include <QMutex>
#include <QString>
#include <QVector>
#define FloatToPercentQString(x) QString("%1").arg(QString::number(x,'f',1)) //float转成1位小数的百分数
#define IntToQString(x) QString::number(x)

//柔性供料－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
typedef struct stu_feed_auto_ctrl{
    int isHasRadar;            //是否有雷达, 0-无雷达、料位开关， 1-雷达， 2-料位开关
    int riceContainerDepth;    //客户集料斗深度
    int levelEmptyPosi;        //料空模式起始位置，默认riceContainerDepth-100，表示100mm，大于 集料斗深度-100， 进入料空模式
    int levelMorePosi;         //料多模式起始位置，默认300，表示300mm，低于此值，进入料多模式
    int levelFullPosi;         //料满模式起始位置，默认250，表示250mm，低于此值，进入料满模式
    int levelLowPosi;           //料少，比料空多 进入料空模式，此时保持在均衡现象的流量
    int feedBalanceClearUpValue;            //清仓模式的流量
    int feedLevelChangeWaitTime;            // 根据料位，不同状态直接切换等待时间
    int autoFeedMode;                   //柔性供料模式，
    int feedBalAdjustStep;              // 步长， 均衡->料满，或均衡->料空，调整的步长
    int feedBalAdjustDuration;          // 步长周期，均衡->料满，或均衡->料空，每步调整的周期
    int posiChgThres;                   //雷达精度，料位变化超过posiChgThres,振动器流量需要,需要判断posiChgWait
    int posiChgWait;                   //调节等待，posiChgWait,超过posiChgWait，需要调整振动器流量
    int balUpFeedflow;                  //匀料产量上限
    int balLowFeedflow;                  //匀料产量下限
    int feedEmpytLowLimitValue;         //料空流量，通道最低值，不能低于此值
    int feedPreFullConstValue;          // 预满流量， 料位预满时的固定产量，所有通道一致
    int feedClearModeValue;             // 清仓流量
    //以下参数不用
    int feedMoreStartValue;     //不用， 料满模式 起始流量，默认90
    int feedFullToNormalContinueTime;       // 不用， 料满转成正常的持续时间单位Min
    int nFeedValueBias[MAX_FEEDER];         // 不用， 主给料器的振动量，偏置，基准100
    int feedLevelPreFullWaitTime;            // 不用，料位预满->实满的维持时间，单位：秒
    int mode1FeedTargetPartCount;       //不用， 模式1，均衡目标分段区间数，
    int balUpLowFlow;                   // 不用, 均衡时，满足产量低于balUpLowFlow,且振动器值大于balUpFeedWhileLowFlow，则振动器流量不再上调，防止空载
    int balUpFeedWhileLowFlow;          // 不用
    int adjustPosuUnit;                   //不用， 调整单元系数
}stru_feed_auto_ctrl;


/*************************************************************************************
*
工控机信息
*
**************************************************************************************/
typedef struct stru_ipc_camera_info {
    char cameraName[16];
    char cameraVersion[2];
    unsigned char cameraType;
    int cameraHoriPixel;
    int cameraVeriPixel;
}stu_ipc_camera_info;

typedef struct stru_ipc_info {
    int aliveNumLast;  //判断离线用
    int aliveRetryCnt; //判断离线用
    int aliveNum;
    int restartStat;    //启动状态，0-默认，１-rk上电或复位，2-等待模型加载
    char version[32];
    char versionLib[32];
    int chuteSplice;
    int camDim;
    int roiWidth;
    int roiHeight;
    int roiRepeat;
    int sortStat;//色选状态，0-停止检测, 1-开始检测
    int modelIndex;//模型加载序号
    int modelStat;//0-未加载模型, 1-正在加载, 2-加载成功,3-加载失败－文件不存在,4-加载失败，模型文件错误，5-加载失败－色选状态不匹配,6,模型平台错误
    int modelUploadStat; //0-等待上传，1-准备上传模型1，2-正在上传模型中，3-上传模型成功，4-上传模型失败
    int recvCamTotalLines;
    int loseCamTotalLines;
    int freeSize;
    int npuUsed;
    int cpuUsed;
    int memUsed;
    int chipTemp;
    int lineFreq;
    int picCnt;
    char ugDate[6];
    char ugVer[2];
    //    stu_ipc_camera_info struIpcCameraInfo;
}stu_ipc_info;

/* 工控机+相机状态 */
typedef struct stru_ipc_status {
    unsigned char ipcStatus;    //255,未检测
    unsigned char cameraCount;
    unsigned char cameraStatus;
    unsigned char modelLoadStatus; //模型加载状态 0,默认，1,开始加载，2,加载完参数发送完成
    unsigned int cameraBadMaterialContinueLast;     //上一时间的累计,工控机重启才会清零
    unsigned int cameraBadMaterialContinueCurrent;  //当前时间最新累计,工控机重启才会清零
    unsigned int cameraBadMaterial;                 //本次查询值，30秒一次
    unsigned int cameraBadMaterialTotal;            //本次统计值，30*统计次数
}stu_ipc_status;

/* 工控机可用模型 */
typedef struct stru_ipc_model_able_info {
    char modelname[64];
    char modelDate[20];
}stu_ipc_model_able_info;

typedef struct stru_ipc_model_able {
    unsigned char modelCount;
    stu_ipc_model_able_info modelInfo[MAX_IPC_MODEL_NUM];
}stu_ipc_model_able;

/* 模型信息，数量，类别名称 */
typedef struct stru_ipc_model_class {
    unsigned char classCount;
    char className[MAX_IPC_MODEL_CLASS][MAX_IPC_CLASS_NAME_LENGTH];
}stu_ipc_model_class;

/* 工控机配置文件内容 */
typedef struct stru_ipc_config_file_info {
    int cameraCount;
    int batchSize;
    int roiHeight;
    int roiRepeat;
    int nms;
    int resultProto;
    char ipCamera[16];
    char ipToCamera[16];
    char ipToPanel[16];
}stu_ipc_config_file_info;

/* 模型回包状态， 1-有回包，0-无回包， -1-未请求*/
typedef struct stru_ipc_status_req_value_class {
    char configArithValue;   //配置相算法, 0-成功，255-失败（格式错），254-失败（功能错）
    char modelLoadValue;      //加载模型,0-成功，1-失败
    char modelLoadName[MAX_IPC_MODEL_NAME_LENGTH];  // 加载返回的模型名称
    char sortStartRes;      //开始检测,0-成功，255-失败（格式错），254-失败（功能错）
    char sortStopRes;       //停止检测,0-成功，255-失败（格式错），254-失败（功能错）
    char shutDownRes;       // 关机,0-成功，255-失败（格式错），254-失败（功能错）
    char modelAddHeadRes;   // 添加模型-头 0成功，255失败
    char modelAddDataRes;   // 添加模型-数据 0成功，1仅模型文件失败，2仅配置文件失败，3模型文件和配置文件均失败
    char struIpcSavePicReqStauts;
    char AICfgRes; //配置AI，0-成功，1-无效参数
}stu_ipc_status_req_value_class;

/* 模型回包状态， 1-有回包，0-无回包， -1-未请求*/
typedef struct stru_ipc_status_req_status_class {
    char ipcInfoReq;        //心跳
    char ipcStautsReq;      //工控机状态
    char modelEnableReq;    //查询可用模型
    char modelInfoReq;      //模型信息
    char configCameraReq;   //配置相机参数
    char modelLoadReq;      //加载模型
    char sortStartReq;      //开始检测
    char sortStopReq;       //停止检测
    char shutDownReq;       // 关机
    char ipcAICfgReq;       //配置AI
    char modelInfoReq2;      //模型信息
    char camImgInfoReq;      //相机板图像信息

    char modelAddHeadReq;        // 添加模型-头
    char modelAddDataReq;        // 添加模型-数据
    char testNetHardwareInfoReq;    //硬件信息
    char testNetConfigFileReq;      //配置文件
    char badMaterialCount;          //坏料个数
    char ipcNeedRestartReq;         //工控机重启

    char badPointsCountClear;      //相机含杂清理
    char ipcRunTestReq;            //连续运行测试
    char keepAlive;
    char getFileReq;
    char finshShellCmdReq;
    char modelTargetValueReq;
}stu_ipc_status_req_status_class;

typedef struct stru_ipc_test_net_info {
    unsigned int testScreenRecieved;  // 测试收到的工控机收到的包
    unsigned int screenIpcSeqNum;  // 上位机与工控机之间的序号
    unsigned int screenIpcTotal; //上位机与IPC之间测试包总数
    unsigned int screenIpcLost;  //上位机与IPC之间丢失包总数
    unsigned int testTotalPacket;//IPC与相机板之间测试包总数
    unsigned int testLostPacket; //IPC与相机板之间丢失包数
    unsigned int cpuUsed;   //CPU使用率
    unsigned int memoryUsed;   //内存使用率
    unsigned int gpu0Used;   //GPU-0使用率
    unsigned int gpu1Used;   //GPU-1使用率
    unsigned int cpuTemp;     //CPU温度
    unsigned int gpuTemp;     //GPU温度
    unsigned int netTransSpeed[MAX_CAMERA_PER_IPC];//网卡传输速度（Mbps）
    stu_ipc_config_file_info struIpcConfigFileInfo;
}stu_ipc_test_net_info;

typedef struct stru_ipc_run_test_info {
    char         timeStamp[32];
    unsigned int cpuUsed;   //CPU使用率
    unsigned int memoryUsed;   //内存使用率
    unsigned int gpu0Used;   //GPU-0使用率
    unsigned int gpu1Used;   //GPU-1使用率
    unsigned int cpuTemp;     //CPU温度
    unsigned int gpuTemp;     //GPU温度
    unsigned int netTransSpeed;//网卡传输速度（Mbps）
    unsigned int camBadPoints; //相机坏点数
}stu_ipc_run_test_info;

/* ---------------------------------------------------- */
/* IPC共享参数 */
typedef struct struIpcShareInfo {
    stu_ipc_info struIpcInfo[MAX_VIEW][MAX_VIEW_UNIT];
    stu_ipc_status struIpcStatus[MAX_VIEW][MAX_VIEW_UNIT];
    stu_ipc_model_able struIpcModelAble[MAX_VIEW][MAX_VIEW_UNIT];
    stu_ipc_model_class struIpcModelClass[MAX_VIEW][MAX_VIEW_UNIT];
    char configCameraStatus[MAX_VIEW][MAX_VIEW_UNIT];
    stu_ipc_status_req_value_class struIpcReqValue[MAX_VIEW][MAX_VIEW_UNIT];  //回包值
    stu_ipc_status_req_status_class struIpcReqStauts[MAX_VIEW][MAX_VIEW_UNIT]; //是否有回包，以及状态
    int struIpcClassParamsReqStauts[MAX_VIEW][MAX_VIEW_UNIT];
    stu_ipc_test_net_info struIpcTestNetInfo[MAX_VIEW][MAX_VIEW_UNIT];
    int ipcHeartStatus[MAX_VIEW][MAX_VIEW_UNIT];   //工控机心跳状态
    int ipcLoadStatus[MAX_VIEW][MAX_VIEW_UNIT];   //工控机加载 , 0-未加载， 1-加载中，2-加载成功， 3-加载错误
    int nTestNetworkIsRun[MAX_VIEW][MAX_VIEW_UNIT];                   //IPC是否正在测试网络, 1:运行， 0：不在
    int nIpcRunTestIsRun[MAX_VIEW][MAX_VIEW_UNIT];                    //IPC是否正在连续运行测试, 1:运行， 0：不在
    int isIpcRestart[MAX_VIEW][MAX_VIEW_UNIT];                        //工控机是否自动重启, 1-发生重启， 0-无
    int badTotalPerPeriod[MAX_VIEW][MAX_VIEW_UNIT];                   // 当前工控机坏点总数
    int staticsCount[MAX_VIEW][MAX_VIEW_UNIT];                        //当前查询坏点的次数
    int sCurModelLastClassBadPointsCount[MAX_IPC_MODEL_CLASS]; //数据库最新 实际没用到次数

    int sCurModelClassBadPointsCountPerCam[MAX_VIEW][MAX_VIEW_UNIT][MAX_IPC_MODEL_CLASS];
    int sCurModelClassBadPointsAreaPerCam[MAX_VIEW][MAX_VIEW_UNIT][MAX_IPC_MODEL_CLASS];
    int sCurModelClassBadPointsFrameAreaPerCam[MAX_VIEW][MAX_VIEW_UNIT][MAX_IPC_MODEL_CLASS];

    int sCurModelClassBadPointsCount[MAX_VIEW][MAX_VIEW_UNIT][MAX_IPC_MODEL_CLASS];         //即将入库的物料个数
    int sCurModelClassBadPointsArea[MAX_VIEW][MAX_VIEW_UNIT][MAX_IPC_MODEL_CLASS];          //即将入库的物料面积
    int sCurModelClassBadPointsFrameArea[MAX_VIEW][MAX_VIEW_UNIT][MAX_IPC_MODEL_CLASS];     //即将入库的框面积

    int allIpcConnecOk;     //所有工控机连接正常（连接状态刷新用）0-断线，1-在线
    int allIpcModelLoadOk;  //所有工控机模型加载正常（色选用）　　0-未加载，1-加载中，2-加载成功，3-加载错
    int allIpcIsCalling;    //所有工控机正在心跳（避免单个超时２s导致死锁）
    int sCurModelTargetValue[MAX_VIEW][MAX_VIEW_UNIT];
}struIpcShareInfo;

/* **********************************************************************************
 *
 *                          物料算法参数相关结构体
 *
 * *********************************************************************************/
/* ---------------------------------------------------- */
/* 传统保留算法:灰度杂质A、B，色差杂质A、B，差分A、B) */
typedef struct stru_reserved_grey_color {
    int nColor1;            // 灰度颜色
    int nSensLow1;          // 灰度灵敏度下限
    int nSensUp1;          // 灰度灵敏度上现
    int nMode1;             // 灰度模式1: -- 选亮, 0: -- 选暗
    int nGreyResEnable;     //灰度保留使能标志， 1：使能， 0：不使能
    int nMain2;
    int nVice2;         // 色差颜色 0-: r/(r+g), 1-: r/(r+b), 2-: g/(g+b)
    int nResDiffColor;          // 色差颜色 0-红绿 1-红蓝 2-绿蓝 3-红/红外 4-绿/红外 5-蓝红外
    int nSensLow2;          // 色差灵敏度下限
    int nSensUp2;          // 色差灵敏度上现
    int nMode2;             // 色差模式1: -- 选亮, 0: -- 选暗
    int nDiscolorResEnable;     //灰度保留使能标志， 1：使能， 0：不使能
}stu_res_grey_color;

/* ---------------------------------------------------- */
/* 灰度杂质A、B，色差杂质A、B 差分A、B*/
typedef struct stru_color_arith {
    char sName[AIRTH_NAME_LENGTH];         // 杂质名称
    int nArithEnable;       // 算法使能
    int nDispEnable;          // 显示使能
    int nVavle;              // 对应的喷阀, 0:主喷阀， 1：辅喷阀
    int nRow;                // 行数, 面积 = 行数*列数
    int nColumn;             // 列数, 列数 = 行数
    int nPercent;            // 纯度
    int nMain;               // 灰度差分颜色或色差主色,0-: red, 1-: green, 2-: blue, 3-:black/white 4-灰度 5-红外
    int nVice;               // 色差辅色 0-: red, 1-: green, 2-: blue, 3-:Nir
    int nDiffColor;          // 色差颜色 0-红绿 1-红蓝 2-绿蓝 3-红/红外 4-绿/红外 5-蓝红外
    int nSensLow;            // 灵敏度下限, 灰度 0~255， 色差 0~1023
    int nSensUp;            // 灵敏度上限
    int lightLimit;             // 亮度下限
    int nMode;               // 亮暗模式：  0: --选暗, 1: -- 选亮
    int nPropotionEnable;      // 比例算使能， 0：不使能， 1:使能, 色差A对应比例算法1，灰度A对应比例算法2，智能A对应比例算法3 注意比例算法与行列尺寸是互斥的,颜色算法和形选都有
    float fPropotionSensLow;    //比例算法下阈， 仅灰度A、色差A有效
    float fPropotionSensUp;     //比例算法上阈， 仅灰度A、色差A有效
    int nPropotionParaSensLow;  //比例算法参数下限

    int nLineDefectEnable;      // 线性病斑使能， 0：不使能， 1:使能
    int nLineDefectIndex;       // 线性病斑参数序号， 1：参数1，对应色差B  2：参数2， 对应灰度B  3：参数3，对应智能B
    int fnLineDefectAreaLow;  // 面积参数下阈， 仅灰度B、色差B有效
    int fnLineDefectAreaUp;   // 面积参数上阈， 仅灰度B、色差B有效
    int fnLineDefectLengthLow;  // 长度参数下阈， 仅灰度B、色差B有效
    int fnLineDefectLengthUp;   // 长度参数上阈， 仅灰度B、色差B有效

    int nSensUpdate;         // 灵敏度校正使能 0: 不使能， 1:使能，尺寸，选亮选暗在一起,只有某些颜色算法有，通用平台不用
    int nEdgeType;           // 切边缘种类， 0：尺寸1， 1：尺寸2，默认必须选一个
    int nImpureType;         // 传染算法种类， 0：尺寸1， 1：尺寸2，默认必须选一个
    int nResType;            // 保留算法种类, 0:不使能保留， 1： 传统灰度色差保留， 2：颜色保留(比例保留)
    float nPropResSens;         // 颜色保留(比例保留)阈值 当nResType==2时有效
    int     nEdge;               //算法切边圈数
    stu_res_grey_color struResGreyColor;  // 传统灰度色差参数 nResType == 1
    int delayType;               //延迟类型选择，0：延迟1，   1：延迟2
}stu_color_arith;

/* ---------------------------------------------------- */
/* 智能算法 多分类*/
typedef struct stru_svm {
    char sName[128];            // 杂质名称
    int nArithEnable;           // 算法使能
    int nDispEnable;          // 显示使能
    int nTypeIndex;             // 算法种类和序号, 高4位：0：普通SVM， 1：差分SVM， 低4位SVM序号
    int nVavle;                 // 对应的喷阀
    int nSizeType;              // 区域类型， 0：行列尺寸， 1：面积长度尺寸
    int nRow;                   // 行数, 面积 = 行数*列数
    int nColumn;                // 列数, 列数 = 行数*1
    int nPercent;               // 纯度
    //    int nAreaLow;               //面积下阈
    //    int nAreaUp;                //面积上阈
    //    int nLengthLow;             //长度下阈
    //    int nLengthUp;              //长度上阈
    int nSens;                  // 灵敏度,用于调整常数项, 默认100%, 范围在 10%~200%
    int nModeSvm;               // 0： 正选， 1： 反选
    int nEdgeType;           // 切边缘种类， 0：尺寸1， 1：尺寸2，默认必须选一个
    int nImpureType;         // 传染算法种类， 0：尺寸1， 1：尺寸2，默认必须选一个
    int nPropotionEnable;         // 比例算法种类， 0：不使能， 1：使能，仅对智能A有效, 注意比例算法与行列尺寸是互斥的,颜色算法和形选都有
    float fPropotionSensLow;
    float fPropotionSensUp;
    int nPropotionParaSensLow;  //比例算法参数下限

    int nLineDefectEnable;      // 线性病斑使能， 0：不使能， 1:使能
    int nLineDefectIndex;       // 线性病斑参数序号， 1：参数1，对应色差B  2：参数2， 对应灰度B  3：参数3，对应智能B
    int fnLineDefectAreaLow;  // 面积参数下阈， 仅智能B有效
    int fnLineDefectAreaUp;   // 面积参数上阈， 仅智能B有效   0~100%表示 0~65535
    int fnLineDefectLengthLow;  // 长度参数下阈， 仅智能B有效
    int fnLineDefectLengthUp;   // 长度参数上阈， 仅智能B有效 0~100%表示 0~1023

    int nQuoCons[2];            // 常数项， cons[0]-0： 正， 1：负，下面所有系数同理
    int nQuoR[2];               //r
    int nQuoG[2];               //g
    int nQuoB[2];               //b
    int nQuoN[2];               //n
    int nQuoRG[2];              //rg
    int nQuoRB[2];              //rb
    int nQuoGB[2];              //gb
    int nQuoRR[2];              //rr
    int nQuoGG[2];              //gg
    int nQuoBB[2];              //bb
    int nQuoRN[2];              //rn
    int nQuoGN[2];              //gn
    int nQuoBN[2];              //bn
    int nQuoNN[2];              //nn

    int nEdge;                  //算法切边
    int nModeType;
    int nConsExpandedValue;   //常数相放大系数（模型３专用）
    int delayType;               //延迟类型选择，0：延迟1，   1：延迟2
    int maxSenseValue;  //灵敏度区间
    int impuritPos;     //含杂最小档位
    int brightLimit;    //亮度下限
}stu_svm;

/* ---------------------------------------------------- */

/* 形选智能算法 */
typedef struct stru_shape_svm {
    char sName[128];            // 杂质名称
    int nArithEnable;           // 算法使能
    int nDispEnable;            // 显示使能
    int nAreaLow;               //面积下阈
    int nAreaUp;                //面积上阈
    int nSens;                  // 灵敏度,用于调整常数项, 默认100%, 范围在 10%~200%
    int nModeSvm;               // 0： 正选， 1： 反选

    int nQuoCons[2];            // 常数项， cons[0]-0： 正， 1：负，下面所有系数同理
    int nQuoA[2];
    int nQuoB[2];
    int nQuoC[2];
    int nQuoD[2];
    int nQuoE[2];
    int nQuoF[2];

    int nEdge;                  //算法切边
}stu_shape_svm;

/* 选圆选长算法*/
typedef struct stru_circle_long {
    char sName[128];        // 杂质名称
    int nArithEnable;       // 算法使能
    int nDispEnable;          // 显示使能
    int nTypeIndex;          // 算法种类和序号，默认值 0x20, 0010 0000
    int nVavle;             // 对应的喷阀
    int nCiloLow;           // 灵敏度下限   范围：0~1023, 选长的灵敏度
    int nCilosUp;            // 灵敏度上限   0-1023, 选圆的灵敏度
    int nMode;              // 选圆/选长模式切换， 1： 选圆，  0： 选长
    int nLimit;             // 参数下限, 范围：0~65535
    int delayType;               //延迟类型选择，0：延迟1，   1：延迟2
}stu_circle_long;

/* ---------------------------------------------------- */
/* 长短算法*/
typedef struct stru_long_short {
    char sName[128];        // 长短杂质名称
    int nArithEnable;       // 算法使能
    int nDispEnable;          // 显示使能
    int nTypeIndex;          // 算法种类和序号，默认值 0x10, 0001 0000
    int nVavle;             // 对应的喷阀
    int nLengthLow;         // 长短灵敏度下阈              //选短时用
    int nLengthUp;         // 长短灵敏度上阈   0-1023      //选长时用
    int nLengthMode;        // 选长/选短模式切换， 0： 选短， 1： 选长，
    int nLimit;             // 参数下限 范围：0~65535
    int delayType;               //延迟类型选择，0：延迟1，   1：延迟2
}stu_long_short;

/* ---------------------------------------------------- */
/* 大小算法 */
typedef struct stru_big_small {
    char sName[128];        // 杂质名称
    int nArithEnable;       // 算法使能
    int nDispEnable;          // 显示使能
    int nTypeIndex;          // 算法种类和序号，默认值 0x00, 0000 0000
    int nVavle;             // 对应的喷阀
    int nMode;              // 大小模式切换, 0: 选小, 1: 选大
    int nAreaLow;             // 灵敏度
    int nAreaUp;             // 灵敏度 0-65535
    int nLimit;             // 参数下限 范围：0~65535
    int delayType;               //延迟类型选择，0：延迟1，   1：延迟2
}stu_big_small;

/* ---------------------------------------------------- */
/* 杂粮色选算法饱和 */
typedef struct stru_color_sat {
    char sName[128];        // 杂质名称
    int nArithEnable;       // 算法使能
    int nDispEnable;          // 显示使能
    int nVavle;             // 对应的喷阀
    int nRow;                // 行数, 面积 = 行数*列数
    int nColumn;             // 列数, 列数 = 行数
    int nPercent;            // 纯度
    int nMode;               // 亮暗模式：  0: --选暗, 1: -- 选亮
    int nSensLow;            // 灵敏度下限
    int nSensUp;            // 灵敏度上限
    int lightLimit;             // 亮度下限

}stru_color_sat;

/* ---------------------------------------------------- */
/* 杂粮色选算法比例 */
typedef struct stru_color_scale {
    char sName[128];        // 杂质名称
    int nArithEnable;       // 算法使能
    int nDispEnable;          // 显示使能
    int nVavle;             // 对应的喷阀
    int nSenIdx;            // 对应的算法序号
    int nSen;               // 灵敏度
    int nLimit;             // 限值

}stru_color_scale;

/* ---------------------------------------------------- */
/* 杂粮色选算法颜色 */
typedef struct stru_color_hue {
    char sName[128];        // 杂质名称
    int nArithEnable;       // 算法使能
    int nDispEnable;          // 显示使能
    int nRow;                // 行数, 面积 = 行数*列数
    int nColumn;             // 列数, 列数 = 行数
    int nPercent;            // 纯度
    int nHighHue;            // 正向色度
    int nLowHue;             // 负向色度
    int lightLimit;             // 亮度下限

}stru_color_hue;

/* ---------------------------------------------------- */
/* 杂粮色选算法灰度 */
typedef struct stru_color_value {
    char sName[128];        // 杂质名称
    int nArithEnable;       // 算法使能
    int nDispEnable;          // 显示使能
    int nRow;                // 行数, 面积 = 行数*列数
    int nColumn;             // 列数, 列数 = 行数
    int nPercent;            // 纯度
    int nMode;               // 亮暗模式：  0: --选暗, 1: -- 选亮
    int nSensUp;            // 灵敏度上限
    int nSensLow;            // 灵敏度下限

}stru_color_value;

/* ---------------------------------------------------- */
/* 杂粮色选算法霉点 */
typedef struct stru_mildew {
    char sName[128];        // 杂质名称
    int nArithEnable;       // 算法使能
    int nDispEnable;          // 显示使能
    int nSens;              // 灵敏度
    int nPercent;            // 纯度
    int nLimit;              // 下限
    int nMark;               // 霉点标记：  0: --选亮, 1: -- 选暗
    int nType;               // 霉点类型：  0: --深霉, 1: -- 浅霉, 2:-- 全霉
}stru_mildew;

/* ---------------------------------------------------- */
/* 杂粮色选算法葫芦籽霉点 */
typedef struct stru_spl_mildew {
    char sName[128];        // 杂质名称
    int nArithEnable;       // 算法使能
    int nDispEnable;          // 显示使能
    int nSens;              // 灵敏度
}stru_spl_mildew;

/* ---------------------------------------------------- */
/* 杂粮色选算法白沙果算法 */
typedef struct stru_white_peanut {
    char sName[128];        // 杂质名称
    int nArithEnable;       // 算法使能
    int nDispEnable;          // 显示使能
    int nSensUp;              // 灵敏度上限
    int nSensLow;             // 灵敏度下限
}stru_white_peanut;

/* ---------------------------------------------------- */
/* 杂粮形选算法轮廓 */
typedef struct stru_outline {
    char sName[128];        // 杂质名称
    int nArithEnable;       // 算法使能
    int nDispEnable;          // 显示使能
    int nSens;              // 灵敏度
    int nPercent;            // 纯度
    int nHighLimit;          // 上限
    int nLowLimit;           // 下限
}stru_outline;

/* ---------------------------------------------------- */
/* 杂粮形选算法纹理 */
typedef struct stru_line {
    char sName[128];            // 杂质名称
    int nArithEnable;           // 算法使能
    int nDispEnable;            // 显示使能
    int nColor;                 // 纹理颜色：0 红色 1 绿色 2 蓝色
    int nSize;                  // 尺寸
    int nRow;                   // 行数, 面积 = 行数*列数
    int nColumn;                // 列数, 列数 = 行数
    int nPercent;               // 纯度
    int nSens1;                 // 纹理精度1
    int nSens2;                 // 纹理精度2
}stru_line;

/* ---------------------------------------------------- */
/* 杂粮形选算法选芽 */
typedef struct stru_bud {
    char sName[128];            // 杂质名称
    int nArithEnable;           // 算法使能
    int nDispEnable;            // 显示使能
    int nSize;                  // 尺寸
    int nSens;                  // 选芽精度
    int nLimit;                 // 选芽下限
    int nYellowSens;            // 黄芽精度
    int nBlackSens;             // 黑芽精度2
}stru_bud;

/* ---------------------------------------------------- */
/* 杂粮形选算法选圆非圆 */
typedef struct stru_round {
    char sName[128];            // 杂质名称
    int nArithEnable;           // 算法使能
    int nDispEnable;            // 显示使能
    int nSelectMode;            // 0 选圆 1 选非圆
    int nSensUp;                  // 圆度阈值
    int nSensLow;
    int nLimit;                 // 圆度下限
}stru_round;


/* ---------------------------------------------------- */
/* ---------------------------------------------------- */
/* HSV算法-h */
typedef struct stru_hsv_h {
    char sName[128];        // 杂质名称
    int nArithEnable;       // 算法使能
    int nDispEnable;          // 显示使能
    int nVavle;             // 对应的喷阀
    int nRow;                   // 行数, 面积 = 行数*列数
    int nColumn;                // 列数, 列数 = 行数*1
    int nPercent;               // 纯度
    int nRadioMode;          //横纵比，0表示1:1，1表示2
    int nHsvColor;              //0-使用H作为灵敏度， 1-使用S作为灵敏度， 2-使用V作为灵敏度
    int nHsvMode;               //0-选暗， 1-选亮
    int nHsvSensLowH;               // H灵敏度下限,  0~359，
    int nHsvSensUpH;                // H灵敏度上限，0~359，
    int nHsvSensLowS;               // S灵敏度下限,  0~255
    int nHsvSensUpS;                // S灵敏度上限，0~255
    int nHsvSensLowV;               // V灵敏度下限,  0~255
    int nHsvSensUpV;                // V灵敏度上限，0~255
    int nUserArithNum;          //操作工界面算法编号
    int delayType;               //延迟类型选择，0：延迟1，   1：延迟2
    stu_res_grey_color struResGreyColor;  // 传统灰度色差参数 nResType == 1
}stu_hsv;

/* TEA color*/
typedef struct stru_tea_color{
    char sName[128];        // 杂质名称
    int nArithEnable;       // 算法使能
    int nDispEnable;          // 显示使能
    float tc;//0-100
    float ec; //0-255
    float ac;//0-255
    float sense;//0-50 0.1
    int nRow;                // 行数, 面积 = 行数*列数
    int nColumn;             // 列数, 列数 = 行数
    int nPercent;            // 纯度
    int nMode;// 0 light 1:dark
    int nEdge;              // 切边缘圈数0-7
    int nInhibit;           //  压制选择0-1
    int nTeaIndex;//当前tea常数在预设列表中索引
    int nTeaMode;//是否使用预设列表中的值
}stu_tea_color;

/* ---------------------------------------------------- */
/* 杂粮辣椒选把算法 */
typedef struct stru_pepper_handle {
    char sName[128];            // 杂质名称
    int nArithEnable;           // 算法使能
    int nDispEnable;            // 显示使能
    int nPercent;               // 斑点比例
    int nSens;                  // 选把精度
    int nLimit;                 // 面积下限
}stru_pepper_handle;

/* ---------------------------------------------------- */
/* 杂粮压制大算法 */
typedef struct stru_suppress_large {
    char sName[128];            // 杂质名称
    int nArithEnable;           // 算法使能
    int nDispEnable;            // 显示使能
    int nSize1;                 // 尺寸1
    int nSize2;                 // 尺寸2
    int nPercent1;              // 尺寸1纯度
    int nPercent2;              // 尺寸2纯度
    int nSenIdx;                // 颜色选择索引
    int nTurns;
}stru_suppress_large;

/* ---------------------------------------------------- */
/* 小麦芽头算法 */
typedef struct stru_wheat_sprout {
    char sName[128];            // 杂质名称
    int nArithEnable;           // 算法使能
    int nDispEnable;            // 显示使能
    int nLow;                   // 下限
    int nPercent;               // 纯度
    int nSense;                 // 精度
    int nH0;                    // H0
    int nH1;                    // H1
}stru_wheat_sprout;


/* ---------------------------------------------------- */
/* 比例算法 */
//typedef struct stru_propotion {
//    int nSensLow1;          // 比例1下阈
//    int nSensUp1;           // 比例1上阈
//    int nSensLow2;          // 比例2下阈
//    int nSensUp2;           // 比例2上阈
//}stu_propotion;
/* **********************************************************************************
 *                         参数文件保存状态
 *
 * value = 0	所有文件都保存正确
 * value = 1	工程模式文件保存错误
 * value = 2	全局参数文件保存错误
 * value = 3	方案参数文件保存错误
 *
 * *********************************************************************************/
typedef struct struCnfFileStatus {
    int value;
}struCnfFileStatus;


/* **********************************************************************************
 *
 *                          工程模式参数相关结构体
 *
 * *********************************************************************************/

/* 子预制方案文件信息 */
typedef struct stru_son_prefixedprifile_info {
    int nIsAble;                    // 子预制方案使能状态
    char sMaterialName[128];        // 子预制方案物料名称
    char sProfileName[128];         // 子预制方案文件名称
}stru_son_prefixedprifile_info;

/* 父预制方案文件信息*/
typedef struct stru_parent_prefixedprofile_info{
    int nIsAble;                   // 父预制方案使能状态
    int nSonProfileCount;          // 当前父预制方案中子预制方案数量
    char sMaterialName[128];       // 父预制方案物料名称
    struct stru_son_prefixedprifile_info  struSonProfile[MAX_SON_PREFIXED_PROFILE_COUNT];      // 子预制方案信息
}stru_parent_prefixedprofile_info;

/* 工程文件参数  */
typedef struct struCnfEngineer {
    int nMachine;                               // 机型：CF系、单层履带机、双层履带机
    int nProduct;                               // 产品类型通用产品、定制产品
    int nArithmeticTotal;                       // 当前机型物料算法总数
    int nArithmeticSeq[ARITHMETIC_TOTAL];       // 物料算法使能状态
    int nParentPrefixedProfileTotal;            // 根预制方案数量
    stru_parent_prefixedprofile_info struParentPrefixedProfile[MAX_PARENT_PREFIXED_PROFILE_COUNT];   // 根预制方案信息
    int nLangList[64];                          // 语言列表，[0] - 当前设置语言总数, [other] - 语言编号
    char sTitle[64];                            // 定制主界面标题
}struCnfEngineer;

/* **********************************************************************************
 *
 *                          全局参数相关结构体
 *
 * *********************************************************************************/
/* 方案文件名称和方案命名索引 */
typedef struct stru_Profile_Index {
    char sProfileName[128];     // 方案保存文件名称
    char sMaterialName[128];    // 界面显示方案名称
    int  nProfileLock;          // 方案是否加锁， 1：锁住， 0：不锁
}stru_Profile_Index;

/* 视内相机参数 */
typedef struct stru_view_camera_Info {
    int nChannelBegin[MAX_VIEW_UNIT];                                // 象元起始
    int nChannelEnd[MAX_VIEW_UNIT];                                  // 象元终止
    int nChannelBegin2[MAX_VIEW_UNIT];                                // 象元起始
    int nChannelEnd2[MAX_VIEW_UNIT];                                  // 象元终止
    int nEjectBegin[MAX_VIEW_UNIT];                                 // 吹嘴起始
    int nEjectEnd[MAX_VIEW_UNIT];                                   // 吹嘴终止
    int nEjectBegin2[MAX_VIEW_UNIT];                                // 吹嘴起始
    int nEjectEnd2[MAX_VIEW_UNIT];                                  // 吹嘴终止
    int nChannelEjectorBegin[MAX_VIEW_UNIT][MAX_EJECTOR];            // 喷嘴象元起始
    int nChannelEjectorEnd[MAX_VIEW_UNIT][MAX_EJECTOR];              // 喷嘴象元终止
    int nBiasingRed[MAX_VIEW_UNIT];                                  // 红色偏置
    int nBiasingGreen[MAX_VIEW_UNIT];                                // 绿色偏置
    int nBiasingBlue[MAX_VIEW_UNIT];                                 // 蓝色偏置
    int nBiasingNir[MAX_VIEW_UNIT];                                  // N色偏置
    int nAnaGainRed[MAX_VIEW_UNIT][MAX_CALIBRATE_SETS];                                  // 红色模拟增益      //增益后期有可能 跟随方案变化
    int nAnaGainGreen[MAX_VIEW_UNIT][MAX_CALIBRATE_SETS];                                // 绿色模拟增益
    int nAnaGainBlue[MAX_VIEW_UNIT][MAX_CALIBRATE_SETS];                                 // 蓝色模拟增益
    int nAnaGainNir[MAX_VIEW_UNIT][MAX_CALIBRATE_SETS];                                  // N色模拟增益
    int anaGainLockStatus[MAX_VIEW_UNIT];                                 //增益锁定,仅D2使用
    int anaGainLockColor[MAX_VIEW_UNIT];                            //增益锁定的颜色,仅D2使用
    int nGainTargetRed[MAX_VIEW_UNIT];                               // 红色增益目标值      //增益后期有可能 跟随方案变化
    int nGainTargetGreen[MAX_VIEW_UNIT];                             // 绿色增益目标值
    int nGainTargetBlue[MAX_VIEW_UNIT];                              // 蓝色增益目标值
    int nGainTargetNir[MAX_VIEW_UNIT];                               // N色增益目标值
    int nDigiGainRed[MAX_VIEW_UNIT];                                 // 红色数字增益
    int nDigiGainGreen[MAX_VIEW_UNIT];                               // 绿色数字增益
    int nDigiGainBlue[MAX_VIEW_UNIT];                                // 蓝色数字增益
    int nDigiGainNir[MAX_VIEW_UNIT];                                 // N色数字增益

    int nCalibrateRed[MAX_VIEW_UNIT][MAX_CALIBRATE_SETS];                          // 明场校正 红色目标值
    int nCalibrateGreen[MAX_VIEW_UNIT][MAX_CALIBRATE_SETS];                        // 明场校正 绿色目标值
    int nCalibrateBlue[MAX_VIEW_UNIT][MAX_CALIBRATE_SETS];                         // 明场校正 蓝色目标值
    int nCalibrateNir[MAX_VIEW_UNIT][MAX_CALIBRATE_SETS];                          // 明场校正 红外目标值

    int nCalibratePixelRed;                      //水平方向亚像素校正,红色
    int nCalibratePixelRedVert[MAX_VIEW_UNIT];                //垂直方向亚像素校正,红色
    int nCalibratePixelGreen;                    //水平方向亚像素校正，绿色
    int nCalibratePixelGreenVert[MAX_VIEW_UNIT];              //垂直方向亚像素校正,绿色
    int nCalibratePixelBlue;                     //水平方向亚亚像素校正，蓝色
    int nCalibratePixelBlueVert[MAX_VIEW_UNIT];               //垂直方向亚像素校正,蓝色
    int nCalibratePixelNir;                      //水平方向亚亚像素校正，N
    int nCalibratePixelNirVert[MAX_VIEW_UNIT];                //垂直方向亚像素校正,N


    int nNirTdiEnable;                          //红外TDI使能, 0-不使能， 1-使能
    int nNirUpdownEnable;                       //红外up/down信号使能, 0-不使能， 1-使能
    int nColorRestoreRed[3];                    // 颜色还原 红色， R11, R12, R13
    int nColorRestoreGreen[3];                    // 颜色还原 红色， R11, R12, R13
    int nColorRestoreBlue[3];                    // 颜色还原 红色， R11, R12, R13

    // 红外部分
    int nNirGreenOffsetX[MAX_VIEW_UNIT];        //红外相机绿色X方向的偏移矫正
    int nNirGreenOffsetY[MAX_VIEW_UNIT];        //红外相机绿色Y方向的偏移矫正
    int nNirBlueOffsetX[MAX_VIEW_UNIT];         //红外相机蓝色X方向的偏移矫正
    int nNirBlueOffsetY[MAX_VIEW_UNIT];         //红外相机蓝色Y方向的偏移矫正
    int nNirRedOffsetX[MAX_VIEW_UNIT];          //红外相机红色x方向偏移矫正
    int nNirRedOffsetY[MAX_VIEW_UNIT];          //红外相机红色Y方向偏移矫正
    int nNirNOffsetX[MAX_VIEW_UNIT];          //红外相机N色x方向偏移矫正
    int nNirNOffsetY[MAX_VIEW_UNIT];          //红外相机N色Y方向偏移矫正

    int nNirGreenSendOffsetX[MAX_VIEW_UNIT];        //红外相机绿色X方向的偏移矫正
    int nNirGreenSendOffsetY[MAX_VIEW_UNIT];        //红外相机绿色Y方向的偏移矫正
    int nNirBlueSendOffsetX[MAX_VIEW_UNIT];         //红外相机蓝色X方向的偏移矫正
    int nNirBlueSendOffsetY[MAX_VIEW_UNIT];         //红外相机蓝色Y方向的偏移矫正
    int nNirRedSendOffsetX[MAX_VIEW_UNIT];          //红外相机红色x方向偏移矫正
    int nNirRedSendOffsetY[MAX_VIEW_UNIT];          //红外相机红色Y方向偏移矫正
    int nNirNSendOffsetX[MAX_VIEW_UNIT];          //红外相机N色x方向偏移矫正
    int nNirNSendOffsetY[MAX_VIEW_UNIT];          //红外相机N色Y方向偏移矫正
}stru_view_camera_Info;

/* 视->组信息 */
typedef struct stru_iden_group_info{
    int nLayerIndex;                     // 组所在的层索引
    int nViewIndex;                      // 组所在的视索引
    int nNameIndex;                      // 组名称索引
    int nGroupUnitTotal;                 // 组内相机板数量
    int nUnitGupId[MAX_VIEW_UNIT];          // 组内相机编号
    int nUnitGupSplice[MAX_VIEW_UNIT];      // 相机是否分割，0-:不分割, 1-:分割，前一半, 2-:分割，后一半
}stru_iden_group_info;

/* 视(相机) 信息 前主视、前辅视、后主视...*/
typedef struct stru_view_info{
    int nLayerIndex;                        // 视所在的层索引
    int nNameIndex;                         // 视名称索引 主视、辅视...
    int nViewUnitId[MAX_VIEW_UNIT];         // 视内相机编号，给相机参数寻址使用，如增益、偏置等
    //    int nFpgaMode[MAX_VIEW_UNIT];           // 视内相机板 FPGA用户模式， 0:-工厂模式， 1:-用户模式1, 2:-用户模式2,3:-用户模式3, F:-保持当前模式不变

    int nEjectorsPerCamera;                 // 视每个相机对应的喷嘴数量 保留不用，   用nEjectorsPerChute

    //int nSensorType;                      // 传感器型号---SENSOR_K:- Keda  SENSOR_T_2566:- Toshiba
    //int nSensorDim;                         // 传感器维度， 1-1维， 2-2维， 3-3维， 4-4维
    //int nSensBitWidth;                      // 相机板位宽， 默认8 表示颜色范围 0~255
    //int nSensorPixel;                       // 传感器分辨率， 8:-256，9:-512, 10:-1024, 11:-2048
    // int nSensorRowFre;                      // 传感器行频
    //int nSensorDimD2;                       // D2传感器维度， 1-1维， 2-2维， 3-3维， 4-4维(sb操作，配合帧频调节公式使用)
    // int nSensorColorChoose[4];              // 传感器颜色分类选择 0-3:bgrn
    int nSensorD2Mode;                      // D2颜色选择模式 0:初始值, 1:rgb, 2:nrg, 3:nrb, 4:ngb
    int rowColQuo;                          // 传感器类型决定的，行与列直接的系数关系,便于计算行列尺寸使用
    //int nImageSplice;                       // 图像使用，2048拼接成1024图像
    //  int nSensorDataType;                    // 传感器数据合并方式 立式机:0,履带机:1
    //int nMirror;                            // 是否使用镜像
    //int nSenseBrightSelection;              // 传感器亮度选择，０:0-11位，１:1-12位（rgb数据取各分量１5位中的...）
    //int nSensDropLine;                      // 传感器丢行（0:不丢，1-100:表示多少行丢１行）
    //int nSensCmdSpeed;                      // 传感器命令转发速率
    int nSensResizeMode;                    // resize模式　0:不使用，1:模式１-768,2:模式2-预留
    // int nRealTimeCorrection;                // 红外传感器实时校正

    int nBrightRefRed;                      // 明场校正：红色参考
    int nBrightRefGreen;                    // 明场校正：绿色参考
    int nBrightRefBlue;                     // 明场校正：蓝色参考
    int nBrightRefNir;                      // 明场校正：N色参考

    int nIdentifyGroupTotal;                  // 视内识别组总数
    int nViewArithTotal;                      // 视内算法总数
    int nViewArithEnable[ARITHMETIC_TOTAL];   // 视内支持的算法（依据相机传感器类型代码中写死），即该视相机支持的所有算法
    int nViewArithVisiable[ARITHMETIC_TOTAL]; // 提供给识别界面显示的算法
    struct stru_iden_group_info stuIdenGupInfo[MAX_GROUP_IDENTIFY];
    int bViewSplice[MAX_VIEW_UNIT];         //  相机是否被分割
}stru_view_info;

/* 吹->剔除组信息 */
typedef struct stru_vavle_group_info{
    int nLayerIndex;                        // 组所在的层索引
    int nVavleIndex;                        // 组所在的吹种类索引
    int nNameIndex;                         // 组名称索引    一次、 一次上吹、一次下吹...
    int nEjBoardGupTotal;                   // 组内喷阀板数量
    int nEjBoardGupId[MAX_VAVLE_BOARD];     // 组内喷阀板地址编号
    int nEjBoardGupSplice[MAX_VAVLE_BOARD]; // 喷阀板是否分割，0-:不分割, 1-:分割，前一半, 2-:分割，后一半
    int nUnitGupTotal;                      // 组内相机单元数量
    int nUnitGupId[MAX_VAVLE_BOARD];        // 组内相机单元地址编号
}stru_vavle_group_info;

/* 吹(喷阀) 信息 主喷阀、辅喷阀*/
typedef struct stru_vavle_info{
    int nLayerIndex;                        // 吹所在的层索引
    int nNameIndex;                         // 吹名称索引 主喷阀、辅喷阀...
    int nVavleBoardId[MAX_VAVLE_BOARD];     // 吹内喷阀板编号，给剔除参数寻址使用
    int nViewTotal;                         // 吹内视数，
    int nVavleViewBoardType[MAX_VIEW];      // 吹内对应的视的类型， 比如上层上视， 上层下视...
    //    int nEjectorsPerVavleBoard;             // 吹内每个阀板对应的喷嘴数量
    int nTickGroupTotal;                    // 吹内剔除组总数
    struct stru_vavle_group_info stuTickGupInfo[MAX_GROUP_TICK];
}stru_vavle_info;

/* 灯光板 信息 */
typedef struct stru_layer_lamp_config {               //灯光板配置信息
    int nLampBoardTotal;                                                        // 灯光板总数
    float nLampValueQuo;                                                        //灯板系数
    int nLampPortEnable[MAX_LAMP_BOARD_PER_INTERFACE][MAX_LAMP_PER_BOARD];      // 每个灯光板上的输出口的使能状态，0-不使能，1-使能，
    int nLampPortType[MAX_LAMP_BOARD_PER_INTERFACE][MAX_LAMP_PER_BOARD];        // 每个灯光板上的输出口的输出类型，0-物料灯  1-背景灯，
    int nLampLayerAddr[MAX_LAMP_BOARD_PER_INTERFACE][MAX_LAMP_PER_BOARD];       // 每个灯光板上的输出口所在的层地址， 0：第一层， 1-第二层...
    int nLampViewAddr[MAX_LAMP_BOARD_PER_INTERFACE][MAX_LAMP_PER_BOARD];        // 每个灯光板上的输出口所在的视地址， 0：前分选室/上分选室， 1-后分选室/下分选室
    int nLampBoardLimit[MAX_LAMP_BOARD_PER_INTERFACE][MAX_LAMP_PER_BOARD];      // 每个灯光板上的输出口的灯光阈值上限， 默认100
    float fLampBoardQuo[MAX_LAMP_BOARD_PER_INTERFACE][MAX_LAMP_PER_BOARD];        // 每个灯光板上的输出口的灯光系数值， 默认1.000,三位小数
    char sLampName[MAX_LAMP_BOARD_PER_INTERFACE][MAX_LAMP_PER_BOARD][128];           // 每个灯光板上的输出口的灯光名称
    int nLampValue[MAX_LAMP_BOARD_PER_INTERFACE][MAX_LAMP_PER_BOARD][MAX_CALIBRATE_SETS];            // 每个灯板的每路灯控的亮度值, 兼容亮灭模式和亮度模式, bit7：-0关灯， -1开灯， bit6~bit0 亮度，范围0~100
    int nLampEnable[MAX_LAMP_BOARD_PER_INTERFACE][MAX_LAMP_PER_BOARD][MAX_CALIBRATE_SETS];           // 每个灯板的每路灯控的使能， 1-使能， 0-关闭
    int nLampSeq[MAX_LAMP_BOARD_PER_INTERFACE][MAX_LAMP_PER_BOARD][MAX_CALIBRATE_SETS];              // 亮灯顺序 0 常灭  1111 常亮 0001 顺序1  0010 顺序2

}stru_layer_lamp_cfg;
/* 层内部信息*/
typedef struct stru_layer_info {
    int nLayerIndex;                                                        // 视所在的层索引
    int nViewChainTotal;                                                    // 层内部视链数
    int nViewIsUsed[MAX_VIEW];                                              // 层内某视是否存在
    int nViewId[MAX_VIEW];                                                  // 层内每视的编号, 默认 0： 前主， 1：后主，2： 前辅， 3：后辅
    int nViewBoardType[MAX_VIEW];                                           // 层内视电路板型号 前主、前辅、后主、后辅...
    //int nViewSensorType[MAX_VIEW];                                          // 层内视传感器类型 nViewId[i] = 0, 0-:2564， 1-:1450. 2-: 850 ...
    int nViewUnitTotal[MAX_VIEW];                                           // 层内各视相机单元数量

    int nVavleChainTotal;                                                   // 层内喷阀链总数
    int nVavleBoardType[MAX_VAVLE];                                         // 层内喷阀电路板型号 链路0， 1， ...
    int nVavleType[MAX_VAVLE_TYPE];                                         // 层内各吹 喷阀类型  0：喷阀种类1, 1:喷阀种类2 ..., 目前只有喷阀1
    int nVavleBoardTotal[MAX_VAVLE];                                        // 层内各吹喷阀板数量
    //    int nLampChainTotal;                                                    // 层内灯控链总数
    //    int nCtrlBoardTotal;                                                    // 层内主控板总数

    int nEjectorsPerChute;                                                  // 每个通道对应的喷嘴数量，暂时不考虑双喷阀的情况
    int nEjectorsPosition;                                                  // 喷阀板位置， 0：米机，后侧， 1：茶叶机， 前侧
    //    int nLampBoardType;                                                     //恒流源板型号

    /* 视(相机)参数 */
    struct stru_view_info stuViewInfo[MAX_VIEW];                            // 层内视信息

    /* 吹(喷阀)参数 */
    struct stru_vavle_info stuVavleInfo[MAX_VAVLE];                         // 层内吹信息

    /* 灯光板参数 */
    //    struct stru_layer_lamp_info stuLampInfo;                                // 层内灯光参数, 每个接口板只有一个灯光链

    /* 相机参数 */
    struct stru_view_camera_Info stuCameraInfo[MAX_VIEW];                   // 层内相机参数
    int nSplice;                                                            //是否分割通道
    int nOnlyDispFirst;                                                     //是否只显示一次， 1-是， 0-否
    int nSpliceChuteAddr;                                                   //分割的通道地址
    int nSpliceType;                                                        //分割类型 1  1：3       2  1:1      3  3：1
}stru_layer_info;

/* 统计信息配置信息*/
typedef struct stru_statics_config {
    int nStaticsTotal;                          //当前视用到的统计信息总数

    int nIndex[MAX_STATICS_TYPE];               //每视的回包给的每个统计信息的的位置， 种类+识别算法编号（产量和温度没有算法编号）
    //种类 0-产量 1-温度 2-灰度 3-色差 4-差分 5-svm 6-形选, 编号：以色差为例， 色差A-编号0， 色差B-编号1。 所以若值是0x20代表色差A， 0x31代表色差B          //
    int nBytes[MAX_STATICS_TYPE];               // 每种统计信息的长度
}stru_statics_config;

typedef struct stu_feed_auto_data{
    int  bEnable;//使能
    int  target_feeder;//目标流量
    int  start_feeder;//匀料门限
    int  stop_feeder;//停运门限
    int  stop_duration;//停运时长
    int  ajust_feeder_range;//流量调整范围
    int  bQuickBalance;     //使能快速均衡，慢速系数200，快速系数100
    int  bSingleAimBalance;     //使能独立通道均衡目标值

    //柔性供料－－－－－－－－－－－－－－－－－－－－
    int mode2_target_upper;//均衡目标流量上限，模式2
    int mode2_target_lower;//均衡目标流量下限，模式2
    int mode1FeedTargetPart[AUTO_FEED_TARGET_PART];
}stu_feed_auto_data;

/* 配件使用寿命设置 */
typedef struct stu_use_limit_cfg{
    int  valve_start_use;     //喷阀开始使用时间
    int  valve_life;          //喷阀寿命
    int  filter_start_use;    //喷阀开始使用时间
    int  filter_life;         //过滤器寿命
    int  wipe_start_use;      //开始使用时间
    int  wipe_life;           //清灰刷寿命
    int  lamp_start_use;      //开始使用时间
    int  lamp_life;           //灯寿命
}stu_use_limit_cfg;


/* 智能清灰相关配置*/
typedef struct stu_intel_wipe_config{
    int feederOnffCount;        // 智能清灰，单次需要关闭的振动器数量, 默认3
    int wiperDurationPerChute;  // 清灰刷在每个振动器经过的时间，单位:ms， 64喷嘴默认60， 80喷嘴默认80
    int wiperWidth;             // 清灰刷宽度，默认200
    int	wipeTimeout;            // 清灰超时时间，暂时没用
    int	wiperDefaultPosi;       // 清灰刷初始默认位置， 0-左， 1-右
    int wiperConfirmThread;     // 清灰刷确定比例阈值 默认 60
}stu_intel_wipe_cfg;

/* 报警相关配置*/
typedef struct stu_alarm_config{
    int nAirPressureAlarmType;              // 气压报警器， 0-压力开关， 1-数显表
    int nLightVoiceAlarmType;                           //0-不使能， 1-使能灯光+声音， 2-仅灯光， 3-仅声音
    char nFeederLevelAlarmName[CTRL_ALARM_MAX_FEED_SENSOR][128];
    int nFeederLevelAlarmEnable[CTRL_ALARM_MAX_FEED_SENSOR];   //8个料位传感器使能
    int nFeederLevelAlarmPosi[CTRL_ALARM_MAX_FEED_SENSOR];   //8个料位传感器位置，0-下， 1-上, 在下方，1说明非空料，正常，0说明料空，正常。在上方，1说明料满，异常，0-说明非满料，正常
    int nFeederLevelAlarmDealMode;                      // 此变量暂时不用，料位报警处理方式，默认：只报警， 0-不处理， 1-报警， 2-报警+关闭下料
    char nTemptureAlarmName[CTRL_ALARM_MAX_TEMP_SENSOR][128];
    int nTemptureAlarmEnable[CTRL_ALARM_MAX_TEMP_SENSOR];      //5个温度传感器使能
    int nTemptureAlarmLimit[CTRL_ALARM_MAX_TEMP_SENSOR];       //温度传感器温度报警阈值
    int nTemptureAlarmDealMode;                      // 此变量暂时不用，料位报警处理方式， 默认：只报警，0-不处理， 1-报警， 2-报警+关闭下料
}stu_alarm_cfg;

/* 模型相关相机设置 */
typedef struct stru_ipc_camera_params{
    int ipcType;                        // 0-此相机非IPC， 1-IPC1， 2-IPC2...
    int cameraId;                       // 相机编号
    int nIpAddr[4];                     //4段IP
    int nHoriPixel;                     //水平像素数
    int nVertPixel;                     //垂直像素数
    int nHoriOffset;                     // 水平偏移
    int nHoriWidth;                     // 水平宽度
    int nHoriWidth_2;                     // 水平宽度2
    int nVertOffset;                     // 垂直偏移
    int nVertWidth;                     // 垂直宽度
    int nSaveImgEnable;                  //ipc采图使能
}stu_ipc_camera_params;



/* **********************************************************************************
 *
 *                          色选方案参数相关结构体
 *
 * *********************************************************************************/
/* ---------------------------------------------------- */
/* 识别组参数*/
typedef struct stu_profile_group_identify{
    int nEdgeCut[2];                    // 边缘切除1&2 0-:disable，其他-：切除的圈数 0~3, 只用nEdgeCut[0]
    int nImpured[2];                    // 传染算法1&2使能,  0-:默认值， 其他-：传染行数,所有算法默认都是1 上限32, 1:色选传染， 2:形选传染
    int nImpuredIpc[MAX_IMPURED_TOTAL];                    // 新增加传染算法种类IPC使用,  0-:默认值， 其他-：传染行数,所有算法默认都是1 上限32, 1:色选传染， 2:形选传染
    int nColseCal[2];                   // 闭运算算法1&2使能, 0-:默认值，其他-：尺寸,与切边缘在一起    不用
    int nEdgeMat;                       // 切物料圈数,  0-:disable，其他-：尺寸,所有形选通用一个     不用
    int nCenterSize[2];                   // 中心化尺寸
    int nEjectDelay;                    // 色选延迟时间,  相机板命令
    int nEjectDelayShape;               // 形选延迟时间,  相机板命令
    int nEjectDelay3;                    // 色选延迟时间3,  相机板命令
    int nEjectDelay4;                    // 色选延迟时间4,  相机板命令
    int nEjectDelayModel;               // 模型延迟时间,  相机板命令

    int arithRejectRatio;               //深度学习算法剔除比例，相机板命令
    int nBadNum;                        // 坏点个数,  相机板命令, 范围0~15
    int nSensUpdateGeneral;             // 灵敏度校正，通用使用
    //    int nPropotionSens1[2];             // 比例算法1 灵敏度， 0： 上阈， 1：下阈，对应灰度算法A
    //    int nPropotionSens2[2];             // 比例算法2 灵敏度， 0： 上阈， 1：下阈，对应色差算法A
    //    int nPropotionSens3[2];             // 比例算法3 灵敏度， 0： 上阈， 1：下阈，对应智能算法A
    int nReverseSort[2];                // 正反选1&2设置 0: 正选, 1, 反选; 只用nReverseSort[0]
    stu_color_arith struGreyColor[MAX_GREY_COLOR_ARITH_TOTAL];    // 0: 灰度 A,  1: 灰度 B,  2：灰度C， 3：灰度D, 4: 色差 A,  5: 色差 B, 6:差分
    stu_svm	struSvm[4];                 // SVM 1、2、3
    stu_shape_svm struShapeSvm[3];      //形选智能化
    stu_big_small struBigSmall[2];         // 大小参数
    stu_circle_long struCircleLong[2];     // 选圆选长算法
    stu_long_short  struLongShort[2];      // struShapeSvm
    stu_hsv struHsv[3];                    // HSV算法
    stru_color_sat      struColorSat;       // 饱和算法
    stru_color_scale    struColorScale;     // 比例算法
    stru_color_hue      struColorHue;       // 颜色算法
    stru_color_value    struColorValue;     // 新增灰度算法
    stru_mildew         struMildew;         // 霉点算法
    stru_outline        struOutline;        // 轮廓算法
    stru_line           struLine;           // 纹理算法
    stru_bud            struBud[2];            // 选芽算法
    stru_round          struRound[2];          // 选圆、非圆算法
    stru_spl_mildew     struSplMildew;      // 葫芦籽霉点
    stru_white_peanut   struWhitePeanut;    // 白沙果算法
    stru_pepper_handle  struPepperHandle;   // 辣椒选把
    stru_suppress_large struSuppressLarge[3];  // 压制大
    stru_wheat_sprout   struWheatSprout;  //小麦芽头
    stu_tea_color       struTeaColor[MAX_TEA_ARITH_TOTAL]; //TEA算法
}stu_group_identify;

/* 识别组参数灵敏度*/
typedef struct stu_profile_group_identify_sens{
    int greyColor[MAX_GREY_COLOR_ARITH_TOTAL];    // 0: 灰度 A,  1: 灰度 B,  2: 色差 A,  3: 色差 B, 4:差分
    int	svm[3];                 // SVM 1和2
    int shapeSvm[3];
    int bigSmall[2];         // 大小参数
    int circleLong[2];     // 选圆选长算法
    int longShort[2];      // 选长选短
    int sat;
    int scale;
    int hueH;
    int hueL;
    int mildew;
    int outline;
    int line;
    int bud[2];
    int round[2];
    int value;
    int splMildew;
    int whitePeanut;
    int pepperHandle;
    int hsv[3];
    int suppressLarge[3];
}stu_group_identify_sens;

/* ---------------------------------------------------- */
/* 剔除组参数 */
typedef struct stu_profile_group_tick{
    int nEjectDuration;             // 吹气时间， 接口板命令
    int nEjectDuration2;            // 吹气时间， 接口板命令
    int nFrontRearMode;             // 各视算法关系 0-: or, 1-:and, bit: 3 2 1 0 分别代表后辅、前辅、后主、前主， 接口,目前默认只有前主&后主 相与相或，其他暂时不考虑, 即0x03代表相与， 0x00代表相或
    int nEjwidth;                   // 吹气模式, 0-: 经济吹 1-:精选吹， 接口板命令
    int nWaitTime;                  // 等待时间
    int nTailTime;                  // 尾气时间
    int nEjectTimesLimit;           // 喷阀吹气次数限制， 暂时未用
}stu_group_tick;

/* -----------------外围控制参数----------------------------------- */
/* 控制板参数 */
typedef struct stu_profile_group_control{
    //    int nFeederMainTotal;                   //当前主振动器数量
    //    int nFeederViceTotal;                   //当前辅振动器数量
    int nFeederValueMain[MAX_FEEDER];       // 主给料量大小，振动器为0，表示关闭下料，
    int nFeederValueVice[MAX_FEEDER];       // 辅给料量大小  //不用
    int nFeederValueMainPlc[MAX_FEEDER];    // Plc主给料量大小，振动器为0，表示关闭下料，
    int nWipeDuration;                      // 清灰时间
    int nWipeInterval;                      // 清灰间隔
    int nWipeUnion;                         // 0：上位机定时清灰  1：上下位机联合清灰  2：下位机自动清灰, 3:智能清灰
    int nWipeTimes;                         // 单次清灰，清灰刷是否需要返回， 0：清灰刷有返回， 1：清灰刷不返回
    int nWipeWindDuration;                  // 气帘清灰时间  履带机专用
    int nWipeWindInterval;                  // 气帘清灰间隔,  履带机专用
    int nWipeWaterDuration;                 // 喷水时间,  履带机专用
    int nConveyorIsRun;                     //履带开关
    int nConveyorSpeed;                     // 履带速度
    int nBackgroundFront;                   // 前背景板角度  //不用
    int nBackgroundRear;                    // 后背景板角度  //不用
    int nFeedSetType;                       // 0按料槽  1按次  默认初始值按料槽
    int nFeederTea;
    int nWipeSortOnOff;                     //清灰色选开关 0关 1开
    int nWipeFeedOnOff;                     //清灰下料开关 0关 1开
}stu_group_ctrl;

/* 模型 类别参数，灵敏度，阈值...  */
typedef struct stru_ipc_class_sens {
    int classCount; //不用 20211013
    int lengthLimit;
    int classEnable[MAX_IPC_MODEL_CLASS];
    int classSens[MAX_IPC_MODEL_CLASS];
    int classThreshold[MAX_IPC_MODEL_CLASS];
    char className[MAX_IPC_MODEL_CLASS][MAX_IPC_CLASS_NAME_LENGTH];
    int classLabelBox[MAX_IPC_MODEL_CLASS];  // 纵向标记框大小
    int classLabelBox2[MAX_IPC_MODEL_CLASS]; // 横向标记框大小
    int classTickMode[MAX_IPC_MODEL_CLASS]; // 1重吹，0轻吹
    int classFixMode[MAX_IPC_MODEL_CLASS];  // 纵向: 1表示最值模式，0表示普通模式
    int classFixMode2[MAX_IPC_MODEL_CLASS]; // 横向: 1表示最值模式，0表示普通模式 //2024/1/29弃
    int classMode[MAX_IPC_MODEL_CLASS];     // 算法0（全吹）,算法1（长短）,算法2（面积）,算法3（长/宽）,算法4（粗细）

    int classDelayMode[MAX_IPC_MODEL_CLASS];     // 0模式1，1模式2
    int classHWRatio[MAX_IPC_MODEL_CLASS];       // 横杆判别高宽比阈值(0-100)，默认50
    int classPosAdjust[MAX_IPC_MODEL_CLASS];     // 坐标调整(有符号数，范围-128~+127)，默认为0
    int classBD[MAX_IPC_MODEL_CLASS];     // 边界保留-离边界距离
    int classBX[MAX_IPC_MODEL_CLASS];     // 边界保留-框的大小
    int classANDEnable[MAX_IPC_MODEL_CLASS];     // 相与 1使能
    int selRerverse[MAX_IPC_MODEL_CLASS];     //X系统是否反选
    float selLimit1[MAX_IPC_MODEL_CLASS];     //X系统选择范围下限
    float selLimit2[MAX_IPC_MODEL_CLASS];     //X系统选择范围上限
}stu_ipc_class_sens;

/* ---------------------------------------------------- */
/* 色选方案参数 */
/* 保存在 cnf.profile* */
typedef struct struCnfProfile {
    //    int nAlarmValid[MAX_ALARM];                     // 报警设置:0-:disable alarm, 1-:enable alarm
    int nMatCopySetMode;                           // 物料参数复制设置方式: 0-全部独立, 1: 全部相同, 2-view 相同， 前主=后主， 前辅=后辅, 3-同一视内， 一二三四五次相同
    //    int nBackgroundColor[MAX_LAYER][MAX_VIEW];             // 背景颜色, 0:- Red, 1:- green, 2:-blue, 3:-black, 4:- white 5：N black, 6: N white 5:-yellow, 6:-cyan(青), 7:-purple(紫)
    int nBackgroundColor[MAX_LAYER][MAX_VIEW][3];             // 背景颜色, 0:- Red, 1:- green, 2:-blue, 3:-grey, 4: -N,在grey和N的条件下，只有阈值上下限，不显示比例。在red,green以及blue情况下，不显示阈值和比例的上限,上限默认255或100%
    int nBackgroundColorValueLow[MAX_LAYER][MAX_VIEW][3];        // 背景阈值 下阈
    int nBackgroundColorValueUp[MAX_LAYER][MAX_VIEW][3];        // 背景阈值，上阈
    int nBackgroundColorValueVLow[MAX_LAYER][MAX_VIEW][3];        // 背景V阈值 下阈
    int nBackgroundColorValueVUp[MAX_LAYER][MAX_VIEW][3];        // 背景V阈值，上阈
    int nBackgroundColorProLow[MAX_LAYER][MAX_VIEW][3];          // 颜色比例，下阈
    int nBackgroundColorProUp[MAX_LAYER][MAX_VIEW][3];           // 颜色比例，上阈
    int nBackgroundColorSatUp[MAX_LAYER][MAX_VIEW][3];           // 饱和度，上阈(整瓶机专用)
    int nLampValue[MAX_LAYER][MAX_LAMP_BOARD_PER_INTERFACE][MAX_LAMP_PER_BOARD];               // 每个灯板的每路灯控的亮度值, 兼容亮灭模式和亮度模式, bit7：-0关灯， -1开灯， bit6~bit0 亮度，范围0~100
    int nLampEnable[MAX_LAYER][MAX_LAMP_BOARD_PER_INTERFACE][MAX_LAMP_PER_BOARD];               // 每个灯板的每路灯控的使能， 1-使能， 0-关闭
    int nLampSeq[MAX_LAYER][MAX_LAMP_BOARD_PER_INTERFACE][MAX_LAMP_PER_BOARD];                  // 亮灯顺序    0001 顺序1  0010 顺序2
    int nGroupArithEnable[MAX_LAYER][MAX_VIEW][MAX_GROUP_IDENTIFY][ARITHMETIC_TOTAL];          // 组内算法使能,用于上位机仿真使用
    int nGroupArithDisplayEnable[MAX_LAYER][MAX_VIEW][MAX_GROUP_IDENTIFY][ARITHMETIC_TOTAL];   // 组内算法显示使能，方案设置中，判断灵敏度界面是否显示
    int nGroupArithEdgeEnable[MAX_LAYER][MAX_VIEW][MAX_GROUP_IDENTIFY][ARITHMETIC_TOTAL];      // 组内算法切边使能圈数

    /* 在大米色选机中，增益值跟随方案变化 */
    int nProfileAnaGainRed[MAX_LAYER][MAX_VIEW][MAX_VIEW_UNIT];                                  // 红色模拟增益      //增益后期有可能 跟随方案变化
    int nProfileAnaGainGreen[MAX_LAYER][MAX_VIEW][MAX_VIEW_UNIT];                                // 绿色模拟增益
    int nProfileAnaGainBlue[MAX_LAYER][MAX_VIEW][MAX_VIEW_UNIT];                                 // 蓝色模拟增益
    int nProfileAnaGainNir[MAX_LAYER][MAX_VIEW][MAX_VIEW_UNIT];                                  // N色模拟增益
    int nProfileGainTargetRed[MAX_LAYER][MAX_VIEW][MAX_VIEW_UNIT];                                // 红色增益目标值      //增益后期有可能 跟随方案变化
    int nProfileGainTargetGreen[MAX_LAYER][MAX_VIEW][MAX_VIEW_UNIT];                              // 绿色增益目标值
    int nProfileGainTargetBlue[MAX_LAYER][MAX_VIEW][MAX_VIEW_UNIT];                               // 蓝色增益目标值
    int nProfileGainTargetNir[MAX_LAYER][MAX_VIEW][MAX_VIEW_UNIT];                                // N色增益目标值
    int nProfileDigiGainRed[MAX_LAYER][MAX_VIEW][MAX_VIEW_UNIT];                                 // 红色数字增益
    int nProfileDigiGainGreen[MAX_LAYER][MAX_VIEW][MAX_VIEW_UNIT];                               // 绿色数字增益
    int nProfileDigiGainBlue[MAX_LAYER][MAX_VIEW][MAX_VIEW_UNIT];                                // 蓝色数字增益
    int nProfileDigiGainNir[MAX_LAYER][MAX_VIEW][MAX_VIEW_UNIT];                                 // N色数字增益

    //    int nArithSens[MAX_LAYER][MAX_VIEW][MAX_VIEW_UNIT][MAX_ARITH_TOTAL];               //每个相机的独立灵敏度，不分上下阈

    stu_group_identify struGroupIdentify[MAX_LAYER][MAX_VIEW][MAX_GROUP_IDENTIFY];          // 识别组, 0-: 一次前视, 1-: 一次后视, 2-: 二次前视, 3-: 二次后视, 4-: 三次前视, 5-:三次后视
    stu_group_identify_sens struUnitSens[MAX_LAYER][MAX_VIEW][MAX_VIEW_UNIT][2];             // 单相机色选精度偏置,区分正负，考虑到相机分割，需要左右两个灵敏度
    stu_group_tick struGroupTick[MAX_LAYER][MAX_VAVLE_BOARD][MAX_GROUP_TICK];               // 剔除组， 0： 一次， 1： 二次， 2： 三次
    stu_group_ctrl struGroupCtrl[MAX_CTRL_BOARD_PER_INTERFACE];                                                // 外围控制参数

    int n_coff;                                                 // 像素点与质量的关系系数,1kg包含的像素点个数
    int bColorRestoreEnable;                                    // 颜色还原使能
    int sStaticsGetDuration;                                    // 统计信息间隔时间，流量调整专用，只查询前主
    stu_feed_auto_data nAutoFeed[MAX_LAYER][MAX_GROUP_TICK];    // 流量调整
    int nHsvArithEnable[2];       // 算法使能
    int nHsvRow[2];                   // 行数, 面积 = 行数*列数
    int nHsvColumn[2];                // 列数, 列数 = 行数*1
    int nHsvPercent[2];               // 纯度
    int nHsvSensLowH[2];            // 灵敏度下限, 灰度 0~255， 色差 0~1023
    int nHsvSensUpH[2];                // 灵敏度上限
    int nHsvSensLowS[2];            // 灵敏度下限, 灰度 0~255， 色差 0~1023
    int nHsvSensUpS[2];                // 灵敏度上限
    int nHsvSensLowV[2];            // 灵敏度下限, 灰度 0~255， 色差 0~1023
    int nHsvSensUpV[2];                // 灵敏度上限

    //    stu_ipc_class_sens struCameraIpcClassParams[MAX_LAYER][MAX_VIEW][MAX_VIEW_UNIT];
    int nIpcDelayBufRowCount[MAX_LAYER][MAX_VIEW];        // 不同相机缓冲行数
    int nImpuredIpcCount;               // 当前IPC传染种类
    int nEjectDurationIpcCount;               // 当前IPC吹气种类
    char sCurModelName[MAX_VIEW][MAX_GROUP_IDENTIFY][MAX_IPC_MODEL_NAME_LENGTH];         // 当前模型名称
    char sCurModelDate[MAX_VIEW][MAX_GROUP_IDENTIFY][MAX_IPC_MODEL_DATE_LENGTH];         // 当前模型日期
    int nHasSaveModel;              // 确定是否保存过模型
    int nIpcBadMatAlarmCount;       // 工控机检测到的坏点个数，低于此值，则报警
    int nIpcHasMatPixelPercert;       // 触发工控机坏点个数时，判断当前机器有物料像素点所占比例下限,需要乘以相机数量
    //    stu_ipc_class_sens struIpcClassParams[MAX_IPC_NUM];
    int ipcModelClassCount[MAX_VIEW][MAX_GROUP_IDENTIFY];    //模型类别数量
    stu_ipc_class_sens struCameraIpcClassParams[MAX_VIEW];
    stu_ipc_class_sens struCameraIpcClassGroupParams[MAX_VIEW][MAX_GROUP_IDENTIFY];

    float ipcBadPointscoff;         // ipc含杂统计标定系数

    /* 传感器部分信息跟随方案走　*/
    char sD2ImgModeName[4][16];         // d2图像模式
    int nSensorD2Mode[MAX_LAYER][MAX_VIEW];    // D2颜色选择模式 0:初始值, 1:rgb, 2:nrg, 3:nrb, 4:ngb
    int nSenseBrightSelection[MAX_LAYER][MAX_VIEW]; // 传感器亮度选择，０:0-11位，１:1-12位（rgb数据取各分量１5位中的...）
    int nSensorRowFre[MAX_LAYER][MAX_VIEW];        // 传感器行频
    int nSensResizeMode[MAX_LAYER][MAX_VIEW];      //　resize模式　0:不使用，1:模式１-768,2:模式2-预留
    int nIpcRoiHeight[MAX_VIEW];
    int nIpcRoiWidth[MAX_VIEW];
    int nIpcRoiRepeat[MAX_VIEW];
    float nIpcRoiRepeatf[MAX_VIEW];//roirepeat 比例
    int nIpcRoiRepeat2[MAX_VIEW];   //交叠比参数
    int imageEnahnce[MAX_VIEW] ; //图像增强使能
    int nDualLayerModelSame;                        // 1 模型全部相同 2模型前后相同 可分次 3 模型单视相同   4模型全部独立
    int nIpcCameraClassSameMode; //模型精度单独调节 0：单独 1：前后同 2：整机相同

    stru_view_camera_Info stuCameraInfo[MAX_VIEW];                   // 层内相机参数
    int bCaliGainEnable;
    int bCaliBiasEnable;

    int nTimeDivisionScheme[MAX_LAYER][MAX_VIEW];                 // 分时方案 0 不分时 1 紫外 2 反分时 3 透分时
    int nCameraBoardMode[MAX_LAYER][MAX_VIEW];          // 彩色相机板启动模式， 每个视的启动模式相同
    int nCameraBoardUserMode[MAX_LAYER][MAX_VIEW][MAX_VIEW_UNIT];                  // 彩色相机板启动模式，
    int nViewSensorType[MAX_LAYER][MAX_VIEW];                     // 层内视传感器类型 nViewId[i] = 0, 0-:2564， 1-:1450. 2-: 850 ...
    int nSensorPixel[MAX_LAYER][MAX_VIEW];                        // 传感器分辨率， 8:-256，9:-512, 10:-1024, 11:-2048
    int nSensorDim[MAX_LAYER][MAX_VIEW];                          // 传感器维度， 1-1维， 2-2维， 3-3维， 4-4维
    int nSensorDimD2[MAX_LAYER][MAX_VIEW];                        // D2传感器维度， 1-1维， 2-2维， 3-3维， 4-4维(sb操作，配合帧频调节公式使用)
    int nSensorColorChoose[MAX_LAYER][MAX_VIEW][4];               // 传感器颜色分类选择 0-3:bgrn
    int nSensBitWidth[MAX_LAYER][MAX_VIEW];                       // 相机板位宽， 默认8 表示颜色范围 0~255
    int nSensorDataType[MAX_LAYER][MAX_VIEW];                     // 传感器数据合并方式 立式机:0,履带机:1
    int nImageSplice[MAX_LAYER][MAX_VIEW];                        // 图像使用，2048拼接成1024图像
    int nMirror[MAX_LAYER][MAX_VIEW];                             // 是否使用镜像
    int nViewChainAddr[MAX_LAYER][MAX_VIEW];                      // 相机地址
    int nSensDropLine[MAX_LAYER][MAX_VIEW];                       // 传感器丢行（0:不丢，1-100:表示多少行丢１行）
    int nSensCmdSpeed[MAX_LAYER][MAX_VIEW];                       // 传感器命令转发速率
    int nRealTimeCorrection[MAX_LAYER][MAX_VIEW];                 // 红外传感器实时校正
    int nIsCameraConpress;                                        //是否图像压缩
    int nCameraCompressMode[MAX_LAYER][MAX_VIEW];                 // 彩色相机板压缩模式，


    //柔性供料－－－－－－－－－－－－－－－－－－－－－－－
    int bEnableBalanceFeed;
    int bCurCal;                                                // 当前灯光方案

    int nIpcStatisticsclassId[MAX_IPC_MODEL_CLASS];             // 模型类型对应的统计类型的id  从1开始  0表示该模型类别不做统计
    int nIpcStatisticsbigclassCoeff[MAX_IPC_MODEL_CLASS];       // 统计类型重量系数 每500个像素点对应的重量 单位g
    char sCurBigstatisname[MAX_ONEPAGE_IPCMODEL_CLASS][MAX_IPC_MODEL_NAME_LENGTH];         // 当前统计类型名称
    int nIpcStatisticsBigclass;                                 // 统计类别个数
    int bstaticClassAlarm[MAX_IPC_MODEL_CLASS];          // 指定检测物料告警

}struCnfProfile;

/*************************************************************************************
 *
 *      以下是不需要保存的全局共享参数结构体定义
 *
 ************************************************************************************/

/* 动态延迟损耗 */
typedef struct stu_dynamic_delay{
    int nEdgeCutDelay;      //边缘切除
    int nEdgeMatDelay;      //切物料
    int nImpureDelay;       //传染
    int nCloseCalDelay;     //闭运算
    int nMaxMatSize;
    int nMatSizeDelay;
}stu_dynamic_delay;

/* 版本信息 */
typedef struct stru_version_data {
    int sInterface[5];                            // 接口板版本，0：大版本号, 1： 小版本号, 2:定制版本|模式代号,定制版本高4位，模式代号低4位, 下同 3:电路板SVN版本  4-定制版本
    int sUnit[MAX_LAYER][MAX_VIEW][MAX_VIEW_UNIT][5];        // 相机板版本，0：大版本号, 1： 小版本号, 2:定制版本|模式代号3:电路板SVN版本
    int sUnitbefore[MAX_LAYER][MAX_VIEW][MAX_VIEW_UNIT];    //  开机时用户模式不匹配的相机
    int sUnitIpc[MAX_LAYER][MAX_VIEW][MAX_VIEW_UNIT][5];        // IPC相机板版本，0：大版本号, 1： 小版本号, 2:定制版本|模式代号3:电路板SVN版本
    int sUnitSync[MAX_LAYER][MAX_VIEW][MAX_VIEW_UNIT][2];        // 相机同步信号转接板版本，0：大版本号, 1： 小版本号
    int sVavle[MAX_LAYER][MAX_VAVLE][MAX_VAVLE_BOARD][5];        // 喷阀板版本，0：大版本号, 1： 小版本号, 2:定制版本|模式代号3:电路板SVN版本
    int sCtrlBoard[MAX_CTRL_BOARD_PER_INTERFACE][5];     // 控制板版本，0：大版本号, 1： 小版本号, 2:定制版本|模式代号3:电路板SVN版本
    int sLampBoard[MAX_LAMP_BOARD_PER_INTERFACE][5];     // 灯光板版本，0：大版本号, 1： 小版本号, 2:定制版本|模式代号3:电路板SVN版本
    int sAutoCtrlBoard[5];     // 智能控制板版本，0：大版本号, 1： 小版本号, 2:定制版本|模式代号3:电路板SVN版本
    int sAccelerationBoard[MAX_ACCELERATION_BOARD][5];        // 加速度板版本，0：大版本号, 1： 小版本号, 2:定制版本|模式代号3:电路板SVN版本
    int sHalogenBoard[MAX_HALOGEN_BOARD][5];                // 电流板版本，0：大版本号, 1： 小版本号, 2:定制版本|模式代号3:电路板SVN版本

    int nInterfaceRes;                      //接口板版本查询结果， -1：未查询， 0：查询失败， 1：查询成功
    int nUnitRes[MAX_LAYER][MAX_VIEW][MAX_VIEW_UNIT];
    int nVavleRes[MAX_LAYER][MAX_VAVLE][MAX_VAVLE_BOARD];
    //    int nCtrlRes[MAX_CTRL_BOARD_PER_INTERFACE];
    int nCtrlRes[MAX_CTRL_BOARD_PER_INTERFACE];
    int nLampRes[MAX_LAMP_BOARD_PER_INTERFACE];                                       // 保留
    int nUnitFalutCodeRes[MAX_LAYER][MAX_VIEW][MAX_VIEW_UNIT];

    int nAccelerationRes[MAX_ACCELERATION_BOARD];
    int nHalogenRes[MAX_HALOGEN_BOARD];
    int nAutoCtrlRes;                     //远程控制板版本查询结果， -1：未查询， 0：查询失败， 1：查询成功

    int sVIDetectBoard[MAX_VI_BOARD][5];
    int nCtrlVIRes;
    int sUnitCali[MAX_LAYER][MAX_VIEW][MAX_VIEW_UNIT][MAX_CALIBRATE_SETS][6];
    int sUnitCaliFSType[MAX_LAYER][MAX_VIEW][MAX_VIEW_UNIT][MAX_CALIBRATE_SETS];
    int sUnitCaliRowFre[MAX_LAYER][MAX_VIEW][MAX_VIEW_UNIT][MAX_CALIBRATE_SETS];
    int sUnitCaliTarget[MAX_LAYER][MAX_VIEW][MAX_VIEW_UNIT][MAX_CALIBRATE_SETS];
}stru_version;

/* 统计信息，算法含杂指吹气次数 */
typedef struct stu_statics_data {
    int nStaticsPeriod;     //本次统计的时间，2字节，单位0.1秒
    int nThroughout;        //产量, 4字节， 高字节在先
    int nTotalArithEjectTimes;  //算法总的吹气次数, 3字节
    int nGreyBadData[3];        //灰度算法含杂, 3*3=9字节,灰度A,B,C
    int nDiscoloryBadData[3];   //色差算法含杂, 3*3=9字节,色差A,B,C
    int nSvmData[3];         //SVM智能算法，3*3=9字节
    int nShapeData;          // 形选算法，3字节
    //    int tempera;             //温度, 4字节， 高字节在先
    //    int nBigSmallData;       // 大小算法 3字节
    //    int nLongShortData;      // 长度算法 3字节
    //    int nCircleLongData;     // 选圆选长 3字节
}stru_statics;

/* 实时报警信息值 */
typedef struct stu_machine_status_data {
    int bAlarmFeedLevel[CTRL_ALARM_MAX_FEED_SENSOR];         // 料位报警， 1-报警， 0-正常
    float nTempSensorValue[CTRL_ALARM_MAX_TEMP_SENSOR];               // 5个温度传感器温度, temp=(2000*(1/21+ad_data/81920*Vref)/(20/21-ad_data/(81920*Vref)))-100)/0.38
    float nPressureValue;                     // 气压报警值
    int bFeederCurrentOver[CTRL_ALARM_MAX_FEED_CURRENT];             // 振动器电流过流, 1 - 过流, 0 - normal
    int nMachineUseAir;                     //机器耗气量
    int nColorVoiceAlarmValue;          // 声光报警器, 0-正常，显示绿色(bit2)  1-报警，显示红色(bit0)，跟进设置，判断是否需要声音
    int nAccelerationStatus[MAX_ACCELERATION]; // 加速度传感器报警
    int nAccelerationStatus2[MAX_ACCELERATION]; // 加速度传感器报警
    int nFeedLvelValue[MAX_GROUP_TICK];                 // 料位雷达监测到的距离,单位mm, 有效范围：200~4095(100m~2047mm),低于200表示距离低于100mm或高于2047mm,超出测量范围
    int nFeedLvelValueReal[MAX_GROUP_TICK];             // 当前实际料位
    int nFeedLevelSensorSwitchStat;           // 料位开关传感器获取到的当前料位状态， 0-料空， 1-料匀， 2--料满
}stru_machine_status;

typedef struct stu_camera_status_data {
    int nWiperPosition;                     //清灰刷中心位置信息
    int nWiperGetWidth;                        //清灰刷宽度
    int nCameraSignalAve;                   // 64行信号的rgb平均值
    int nbgSignalAve;                       // 8192个背景
    int nTempSensorValue;                   //相机传感器温度
    int nFirstRequireCmd;                   // 只有清灰时的第一条命令的回包，才是有效值
    int comunicationCode1;                  // 相机通信状态1
    int comunicationCode2;                  // 相机通信状态2
    int nWipeSpeed;                         // 清灰速度(bf v4.04_b)
    int nWipeSpeed_backward;                // 反向清灰时间
    int nWipeSpeed_forward;                 // 正向清灰时间
    int nWipeSpeed_pre;                     // 前级清灰时间
    int nWipeSpeed_total;                   // 刷子总清灰时间
}stru_camera_status;

//typedef struct stu_board_communication_status {
//    int nFirstRequireCmd;                       // 只有清灰时的第一条命令的回包，才是有效值
//    int nCameraRecvLineErr;                     // 命令接收线路连接异常
//    int nCameraRecvTimeout;                     // 命令包超时或校验错误
//    int nCameraSendLineErr;                     // 命令回报线路连接异常
//    int nCameraSendTimeout;                     // 命令回包超时或校验错误
//    int nVavleSendLineErr;                      // 相机板喷阀回包线路连接异常
//    int nVavleSendTimeout;                      // 相机板喷阀回包超时或校验错误
//    int nVavleNoEjectErr;                       // 色选过程中喷阀1分钟无吹气动作
//}stru_board_communication_status;


typedef struct stu_dccrypt_data {
    int mCryptDayLast;                           //加密剩余天数
    int bDccryptGetError;                        //获取下位机加密状态
    char nDccryptGetTime[16];               // 获取下位机的加密时间
}stru_dccrypt_data;


typedef struct stu_model_data{

    int totalmodelnum ; // 所有模型个数
    int currentmodelnum ; // 当前模型个数
    int checkindex; //
    char modelname[MAX_MODE_NUM_LIST][128];  // 模型名称名称
    char modelpath[MAX_MODE_NUM_LIST][256*4];  // 模型下载地址
    char modelinstruct[MAX_MODE_NUM_LIST][256*4]; // 模型简介
}stru_model_data;

typedef struct stu_modelabel_info{

    int  labelnum ; // 标签个数
    char labelname[MAX_MODELABLE_NUM][128]; // 搜模型用的标签名称
    char labeltype[MAX_MODELABLE_NUM][128];       // 搜模型用的标签类型
    char labelID[MAX_MODELABLE_NUM][128];        // 搜模型用的标签类型
}stru_modelabel_info;


typedef struct stu_modelabel_list{

    int labelnum ; // 标签大类个数
    char labelname[MAX_MODE_BIGLABLE_NUM][128]; // 大类名称
    stru_modelabel_info labelinfo[MAX_MODE_BIGLABLE_NUM];
}stru_modelabel_list;

// 界面勾选的标签信息
typedef struct stu_modelabel_select{

    int selectnum ; // 标签个数
    char keyword[128];  // 搜索关键字
    char labelname[MAX_MODE_BIGLABLE_NUM][128];  // 标签名称
    char labeltype[MAX_MODE_BIGLABLE_NUM][128];  // 标签类型
    int labelindex[MAX_MODE_BIGLABLE_NUM];       // 标签列号
}stru_modelabel_select;


//机型参数 包括分次信息 和 灯光校正值
typedef struct stu_machine_set{

    char machinetype[20] ; //
    int targetCount;
    int targetlightValue[MAX_CALIBRATE_SETS];

    /**/
    int groupnum[MAX_VIEW];
    int groupinfo_start[MAX_VIEW][MAX_GROUP_IDENTIFY];
    int groupinfo_end[MAX_VIEW][MAX_GROUP_IDENTIFY];
}stru_machine_set;

/* ---------------------------------------------------- */
/* 全局共享参数 */
typedef struct struShare {
    int nMachine;                           // 机型：LD2或者CF
    int nPageSm;                            // 界面状态机索引
    int nCounter;                           // 本次运行时间
    int nRunCounter;                        // 本次选料时间
    int nSortStartCount;                    // 本次打开下料色选实际
    int nStaticCounter;                     // 数据库写入时间
    int mCryptDayLast;                      // 加密剩余天数
    int feedMsecond;                        //  打开下料时间
    int nSelfExamineFre;                    // 整机自检频率
    float fPixelCompensate[MAX_EJECTOR];    // 象元划分校正系数
    stru_version struVer;                   // 版本信息
    stru_dccrypt_data struDccryptData;      // 加密信息
    int bDccryptGetError;                   // 获取下位机加密状态  0：初始值，1：错误，2:正确
    char nDccryptGetTime[16];               // 获取下位机的加密时间
    //    char nDccryptGetCode[32];               // 获取下位机的加密密码
    int bLostFpgaBoard;                     // 上下位机掉电保护,此时上位机具有UPS, 0-默认值，不处理 1-使能掉电保护检测， 2-发现异常掉电,保存参数，关机

    int bUpdateState;                       //远程升级状态
    int bSortStart;                          // 运行色选状态(开启色选时，才允许设置流量大小)    : 1 - on, 0 - off
    int bStatEject;                         // 喷阀状态   : 1 - on, 0 - off
    int bStatBelt[MAX_LAYER];               // 输送带状态     : 1 - on, 0 - off
    int bKeepAliveType;                     // 心跳间隔种类 ：0-10s, 1-5s

    int bFlagAutowipe;                      // 自动清灰使能 : 1 - auto wipe, 0 - not
    int bFlagGettingStatistic;              // 查询统计信息标志
    int bFlagManuwipe;                      // 手动清灰标志
    int bFlagWiping;                        // 正在清灰标志
    int bFlagRowing;                        // 波形请求使能   : 1 - get row data, 0 - not
    int bFlagPowerOff;                      // 系统关机标志 （用于防止背光关闭后被误触发）
    int bFlagMqttConnect;                   // 小程序连接标志 ： 1 - 在线， 0 - 离线
    int bFlagMachinetest;                   // 整机自检状态    : 1 - 整机自检, 0 - 正常运行
    int bFlagFirstPwdValid;                 // 厂家设置一级密码状态 :1 - valid, 0 - need recheck
    int bFlagSecondPwdValid;                // 厂家设置二级密码状态 :1 - valid, 0 - need recheck
    int bRunMode;                           // 当前运行模式
    int bFlagPasswdAble;                    // 高级参数密码  1: need passwd to advanced, 0:disable
    int bCaliGainEnable;                    // 校正增益使能， 1-使用校正增益， 0：不使用
    int bCaliBiasEnable;                    // 校正偏置使能， 1-使用校正偏置， 0：不使用
    int bCaliDotReviseEnable;               // 坏点纠错使能， 1-使用坏点纠错后的数据， 0：不使用

    int nAlarmStautsCurrent[MAX_ALARM];    // 当前实时报警状态, : 1 - alarm, 0 - normal
    //    stru_board_communication_status struBoardCommstatus[MAX_LAYER][MAX_VIEW][MAX_VIEW_UNIT];
    int bAlarmEjector;                      // 喷阀报警状态  : 1 - alarm, 0 - normal
    int bAlarmLamp;                         // 灯报警状态  : 1 - alarm, 0 - normal
    int bAlarmBlower;                        // 风机警状态  : 1 - alarm, 0 - normal
    int bAlarmStatus;                       // 报警状态1 : alarm, 0 - not
    int bAlarmIpcStatus[MAX_IPC_NUM];                    // 工控机报警

    int curAlarmCount;                      //  开机报警次数
    int bAlarmLastFeedoff;                  // 上次报警关闭下料，报警正常时需要恢复下料
    int bAlarmColorVoiceLast;
    int bAlarmValid;                        // 报警设置0 : go on while alarm, 1: turn off while alrm , 2: skip alarm
    int bAlarmNeedWipe;                     // 吹气次数等导致的报警，需要自动清灰

    stru_machine_status struMachineStatus[MAX_LAYER];

    int bFlagBrightCali[MAX_LAYER][MAX_VIEW][MAX_VIEW_UNIT][2];                 //0:明场校正获取成功， 1：明场校正保存成功
    int bFlagDarkCali[MAX_LAYER][MAX_VIEW][MAX_VIEW_UNIT][2];                   //0:暗场校正获取成功， 1：暗场校正保存成功

    int nSignalDispType;                                                            //波形数据显示类型， 0-正常显示信号， 1—明场校正数据，2-暗场校正数据，3-坏点纠错数据
    int nSignalDispColor;                                                           // 信号显示颜色， 0-显示全部， 1-显示红色， 2-显示绿色， 3-显示蓝色 4-显示红外
    int nNirDotCorrectiveRed[MAX_VIEW][MAX_VIEW_UNIT][2][1024];                                // 红外坏点纠错，红色
    int nNirDotCorrectiveGreen[MAX_VIEW][MAX_VIEW_UNIT][2][1024];                              // 红外坏点纠错，红色
    int nNirDotCorrectiveBlue[MAX_VIEW][MAX_VIEW_UNIT][2][1024];                              // 红外坏点纠错，蓝色(上下两个传感器)
    int nNirDotCorrectiveN[MAX_VIEW][MAX_VIEW_UNIT][2][1024];                              // 红外坏点纠错，N色(上下两个传感器)
    int nNirDotCorrectived;                                                                //是否显示纠错后信息， 0-显示原始信号，1-显示纠错后信号

    int nLayer;                             // 当前层编号
    int nView;                              // 当前视编号
    int nVavle;                             // 当前吹编号
    int nUnit;                              // 当前处理单元编号
    int nCamNo;                             // 当前镜头号
    int nMatGroup;                          // 当前物料参数所在组0-: first front, 1-: first rear, ... 4-: 3rd front, 5-: 3rd rear
    int ctrlboardIndex;                     //当前控制板索引
    int nArithIndex;                         //当前精细设置灰度算法索引
    uchar sRgbRow[WAVE_FRAME_SINGLE_LEN];         // 一帧波形数据信息,512+头(2)+序号(1)+地址(2)+CRC(2)+尾(1)=520
    //    uchar sRgbRow[520*2];         // 一帧波形数据信息,512+头(2)+序号(1)+地址(2)+CRC(2)+尾(1)=520
    uchar sRowRed[SENSOR_MAX_PIXELS];                    // 红值信号
    uchar sRowGreen[SENSOR_MAX_PIXELS];                  // 绿值信号
    uchar sRowBlue[SENSOR_MAX_PIXELS];                   // 蓝值信号
    uchar sRowInf[SENSOR_MAX_PIXELS];                    // N值信号
    uchar sRowHue[SENSOR_MAX_PIXELS];                   // HSV的H值信号
    uchar sRowVaule[SENSOR_MAX_PIXELS];                 // HSV的V值信号
    int sRowTotal[SENSOR_MAX_PIXELS];
    uchar sRowAveForFocusData[SENSOR_MAX_PIXELS];            //
    uchar sRowFocusHighData[SENSOR_MAX_PIXELS];

    int focusAveBeginPixel;
    int focusAveEndPixel;
    int focusAveBeginPixel2;
    int focusAveEndPixel2;

    float focusAveVavle;
    float focusAveVavle2;

    float fRowAveRed;                           // 红值均值
    float fRowAveGreen;                         // 绿值均值
    float fRowAveBlue;                          // 蓝值均值
    float fRowAveInf;                           // N值均值

    int nImageCapMode;                                   // 图像/信号波形触发方式， 0：指定相机， 1：指定通道的所有相机

    char sVersionBuf[READ_PACKET_LENGTH_64];       // 版本查询
    char sStaticsBuf[READ_PACKET_LENGTH_1024];       // 统计信息
    char sAlarmStatusBuf[READ_PACKET_LENGTH_64];   //报警&实时状态,控制板命令

    stru_statics struStatics[MAX_LAYER][MAX_VIEW][MAX_VIEW_UNIT];    //具体到每个相机单元    //产量、温度、含杂(吹气频率)
    float stagedThroughout[MAX_LAYER][MAX_VIEW][MAX_VIEW_UNIT];     //具体到每个相机单元    //阶段时间产量（kg）
    float fixedThroughout;                                          //标定开始后累计产量，用户标定产量K值时使用
    int stagedTotalArithEjectTimes[MAX_LAYER][MAX_VIEW][MAX_VIEW_UNIT];   //具体到每个相机单元    //阶段时间吹气次数
    int chanelArithEjectTimes[MAX_LAYER][MAX_VIEW][MAX_VIEW_UNIT];        //具体到每个相机单元    // 2020/11/4 每个通道总吹气次数,接口板查询上传

    int nCurWiperLeftOrRight;               //当前清灰刷是从左边还是右边开始
    int nCurWiperInChute;                   //当前清灰刷所在通道
    int nIntelWipeTotalWaitTime;            //智能清灰，单次累计等待时间
    stru_camera_status struCameraStatus[MAX_LAYER][MAX_VIEW][MAX_VIEW_UNIT];    //具体到每个相机单元状态信息    清灰刷位置，传感器温度
    int nIamgeLine;                         // 波形请求行数
    int nSensCoefficient;                   // 传感器帧频计算系数

    int nIntelSampColor[IMAGE_MAX];         // 智能模式取样颜色：1: red, 2: green, 4: blue
    int nIntelSampMode[IMAGE_MAX];          // 取样模式: 0,选暗, 1:选亮
    int nIntelSampSens[IMAGE_MAX];          // 取样灵敏度
    int nIntelSampBurst;                    // 触发阈值
    int nIntelSampAi;                       // 普通模式
    int nIntelSampInf;                      // 红外模式

    int nSvmExpandValue;//放大系数
    int nSvmSimulateSens;//svm仿真精度
    int nSvmRow;
    int nSvmPercent;
    int nSvmCut;
    int nSvmTypeIndex;//hsv or rgb
    int nSvmBrightLimit;

    int m_isNewTrain; //使用张涛svm
    bool ntrainComplete;                     //svm分析完成且成功
    double m_svmcoefs[COEF_COUNT];
    int nGoodSamp[MAX_SAMPLE_NUM*4];        //智能 好物料像素点颜色rgb分量
    int nBadSamp[MAX_SAMPLE_NUM*4];         //智能 坏物料像素点颜色rgb分量
    int nGoodSampCount;                     //智能 好物料像素点个数
    int nBadSampCount;                      //智能 坏物料像素点个数
    quint8  nAvgH[2];
    quint8  nAvgS[2];
    quint8  nAvgV[2];
    quint8  nAvgR[2];
    quint8  nAvgG[2];
    quint8  nAvgB[2];
    float  nAvgRG[2];
    float  nAvgRB[2];
    float  nAvgGB[2];
    quint8  nMatAvgR;
    quint8  nMatAvgG;
    quint8  nMatAvgB;
    double m_Shapecoef[COEF_COUNT];         //形选智能
    int nShapeGoodSamp[MAX_SAMPLE_NUM*5];
    int nShapeBadSamp[MAX_SAMPLE_NUM*5];
    int nShapeSingleGoodSampCount;
    int nShapeSingleBadSampCount;
    int nShapeGoodSampCount;
    int nShapeBadSampCount;
    //    int nEjectImpureDelay;                  // 传染算法损耗时间
    struct stu_dynamic_delay struDynamicDelay[MAX_LAYER][MAX_VIEW][MAX_GROUP_IDENTIFY];   // 剔除损耗时间
    int delayPerRow;                      // 根据不同传感器计算每行的延迟损耗
    int heightPerPixel;                     // 根据不同传感器和帧频，获取每个像素点的高度
    int heightWidthPerPixel;                // 根据不同传感器和帧频，获取行列数比值关系  1: 列数=行数， 2：列数=行数*2
    int maxRowNumber;                       // 根据不同传感器和帧频，可设置的最大行数不同, T2:16, T3:32, D:32

    int nEjectEdgeDelay[MAX_LAYER][MAX_GROUP_TICK];                   // 边缘切除损耗时间
    int nMaxMaterialSize[MAX_LAYER][MAX_GROUP_TICK];                  //最大物料算法尺寸
    int nEjectMaterialSizeDelay[MAX_LAYER][MAX_GROUP_TICK];           // 物料算法尺寸延迟时间s

    int nBacklightStat;                     // 当前背光的状态 0:off, 1:on
    int nBacklightCounter;                  // 背光开启的时间

    int nAuthenticationLevel;               // 访问权限 0 - 操作工, 1 - 工程师, 2 - 厂家
    int nPixelBegin;                        // 计算像元划分起始
    int nPixelEnd;                          // 计算像元划分截止
    int ageTestEnable;                      // 整机老化开关， 1：使能， 0：禁用
    int ageTestFre;                         // 整机老化频率, 0: 50Hz, 1:100Hz, 2:200Hz, 3:400Hz
    int ageTestChute;
    int ageTestAll;
    int ageTestChuteEnable[MAX_FEEDER];     // 整机各滑槽老化开关， 1：使能， 0：禁用
    int chuteFeederBeforeStop[MAX_FEEDER]; //关闭前的流量值
    int chuteStopStateByDuration[MAX_FEEDER];// 1:表示由于低于门限停止  默认为0
    int chuteStopDuration[MAX_FEEDER];      // 停运持续时间
    int chuteAjustState[MAX_FEEDER];        //是否达到调节的最低门限
    int chuteFeedMainValue[MAX_FEEDER];     //初始给料量，流量自动调整用
    bool b_getFlow;                         //获取像素点标定
    int testSerialPacketSend[4];           // 测试串口 包的序号, 发送
    int testSerialPacketRead[4];           // 测试串口 包的序号， 回包
    int isTestErr;
    int nImageIndex;                       //分析图像索引
    int nImageCapTimeout;                  //图像采集超时
    int nImageCapDelayTime;                //图像采集传输延迟
    int nImageCapTrigValue;                //图像触发阈值
    int nImageCapTrigPixelBegin;           //图像触发像元起始
    int nImageCapTrigPixelEnd;             //图像触发像元终止
    int nVavleTestIsRun;                   //喷阀自检线程是否正在run,即正在发送参数 1:运行， 0：停止, 停止查询报警
    int nVersionCaptureIsRun;              //正在进行版本查询,即正在发送参数 1:运行， 0：停止，停止查询报警
    int nImageCaptureIsRun;                //正在进行图形采集,即正在发送参数 1:运行， 0：停止，停止查询报警
    int nRemoteImageCaptureIsRun;          //正在远程采图、波形，此时正常采图会失效直到远程采图结束
    int nRemoteCaptureAllWave;          //远程全通道采波形
    int nRemoteSeriesBlowCount;
    int nIpcAlarmImageCaptureIsRun;        //IPC报警采图中，配合远程采图使用
    int serialPortOpenState;                //串口打开状态
    int serialPortOpenState2;                //串口打开状态
    int serialNum;                          //当前打开串口号
    int serialNum_2;                          //当前打开串口号
    bool m_bAutoStudy[2];                   //智能化一键智能分析相关参数
    bool m_bManulStudy[2];                  //智能化手动智能分析相关参数

    int nIrCurRedPointIndex;                //红外相机当前配置的红色点
    int nIrCurGreenPointIndex;
    int svmThreshold[4][4];                 //智能化中红绿蓝红外标记的阈值 索引0下限 索引1上限
    float svmDisThreshold[6][4];            //智能化中比例标记的阈值，r/r+g,r/r+b,g/g+b
    int svmSample[2];
    bool m_bGoodBadDiff;
    bool m_bEdge;
    bool m_bRMRedtmdancy;

    bool bIsUpgrading;
    bool debugMode;                         //调试打印信息开关
    bool debugModeIpc;                      //IPC调试打印信息开关
    int bFpgaModeAutoSwitchErr;            //下位机用户模式切换失败 1 处于工厂 2 与当前屏的模式不匹配
    int bInterModeSwitchErr;            //接口版用户模式切换失败 1 处于工厂
    int bControlModeSwitchErr;            //控制版用户模式切换失败 1 处于工厂
    int bLampAutoSwitchErr;            //灯版用户模式切换失败 1 处于工厂
    int bValveAutoSwitchErr;            //阀版用户模式切换失败 1 处于工厂
    bool bContinuous;
    bool bFilterBackground;               //过滤背景
    bool bCorrectImage;                   //纠偏图像
    int nomalWipeCount;                     //仅用于清灰时间查询

    int  allArithParamsResetCnts;           //仅用于首次获取各算法中边缘切除
    int  bNetContinuousCapture;             //0初始状态，1,采到正确图片，-1,异常


    bool nValveBlowCaptureIsRun;      //喷阀吹气次数显示界面
    bool bConveyorOnOff;                    //履带开关
    int bColorRestoreEnable;                //颜色还原使能
    bool isInIpcClassPage;
    bool isInModelchangePage;
    int bAlarmFeedStaus[CTRL_ALARM_MAX_FEED_SENSOR]; //8个料位传感器信号变化状态
    int imgCaptureCnt;                //多张采图张数
    int nIpcInfoUploadAccess;         //是否可以进行工控机信息查询及上传 0：初始化值，1:允许，2:用户拒绝
    int nChuteFeedIsOn[MAX_VIEW][MAX_VIEW_UNIT];//通道是否在正常下料（dp机型诊断用）
    //    uchar oriBuf[WAVE_FRAME_DATA_LEN*MAX_WAVE_DEVIDE*MAX_WAVE_DIM*IMAGE_HEIGHT]; //特殊采图用
    int nIpcRestartFlag;              //远程上报信息用
    //    int nIpcOfflineCounts[MAX_IPC_NUM];

    int nAlarmSetData0;               //报警设置byte0字节
    //物料统计使用
    int colorRGBTotal[3][256]; //RGB颜色统计，0：R，1：G，2:B
    int ipcSavePicFlag;          //IPC保存图片
    int intefaceAlive;   //接口板保活用
    int cornStatics[MAX_VIEW][MAX_VIEW_UNIT][102];    //具体到每个相机单元
    int cornLastIndex;
    // int nCamTdMode;     //0:默认，不分时，１:分时Ａ，２：分时Ｂ
    int nCamTdMode;     //0:默认，不分时，１:1024TD_Ａ,2:1024TD_Ｂ, 10:2048TD_A,11:2048TD_B
    int nTdMode;     //0:默认，不分时，１:1024TD_Ａ,2:1024TD_Ｂ　用于图像波形的数据结构

    int nIpcLoadCnt; //手动加载模型次数
    float nFreeSpace; //单位:MB
    int nImgTdMode;  //0:默认，不分时，１:分时Ａ，２：分时Ｂ 图像使用
    int nCamMaxImgCnt[MAX_VIEW][MAX_VIEW_UNIT];//相机板最大存储图片数量
    int nCamCurImgCnt[MAX_VIEW][MAX_VIEW_UNIT];//相机板当前存储图片数量

    int accelerationAmplitude[MAX_ACCELERATION];      //加速度振幅值
    int accelerationDutyCycle[MAX_ACCELERATION];      //加速度占空比
    int accelerationChangedDutyCycle[MAX_ACCELERATION]; //调节后的加速度占空比
    int accelerationAmplitude2[MAX_ACCELERATION];      //加速度振幅值
    int accelerationDutyCycle2[MAX_ACCELERATION];      //加速度占空比
    int accelerationChangedDutyCycle2[MAX_ACCELERATION]; //调节后的加速度占空比


    //柔性供料－－－－－－－－－－－－－－－－－－－－－－－－
    bool isClearMode;                   //是否清仓模式
    bool autoFeedEnabled;             //使能柔性供料
    bool isRadarStateAlarm;           //当前是否雷达料位报警
    int feedByRadar;   //根据雷达料位，对应的动态料位
    int curBalanceStateByLevel;           // 根据当前雷达料位，获取的实时均衡模式
    int lastBalanceStateByLevel;          // 上次的均衡模式
    int feederRealSend[MAX_FEEDER];  //实际发送的流量值
    int feederLastRealSend[MAX_FEEDER];  //上次实际发送的流量值
    int nViewforstatic;                              // 当前统计产量视编号
    int chuteFeedAutoLastValue[MAX_FEEDER];     //均衡流量上一次值，均衡模式整用

    // 红外部分
    int nIrCurPointIndex[2];                //红外相机当前配置的点(每个红外相机都会有上下两组数据)
    bool mIrOffsetEnable;                     //红外偏移校正使能

    int  updateModelistresult;
    int  selectDownloadModel;        // 1 当前屏主动下载模型
    int  bScreenShotId;               // 一键截屏界面编号
    int  bScreenShotStatus;         // 一键截屏状态  0关闭 1开启
    stru_model_data  stumodelData;  // 远程获取模型列表信息
    int  currentpageid;             // 远程模型列表界面页
    int  remoControlmode;          // 远程小程序操作屏幕
    int  remoteControlstatus;
    int  remoControltime;

    int sBlowTimes[16*128];  //当前通道吹气次数
    bool valveProtectValue[MAX_VIEW_UNIT_USE] ;   // 当前各通道喷阀保护状态
    int offlineDetection; // 下线检测
    int remoteOnekeytest; // 一键检验
    int poweroff_status;  // 0 异常关闭程序  1  正常关闭程序
    int backCnf_status;  // 0 参数备份文件异常  1  参数备份正常
    int RkFrameNum[MAX_VIEW][MAX_VIEW_UNIT] ; // RK 框个数
    int StaticsFrameNum[MAX_VIEW][MAX_VIEW_UNIT]; // 统计框个数
    double  totalArea[20]; // 当前内所有统计类别的面积
    int bChuteOverRatio[MAX_VIEW][MAX_VIEW_UNIT];//通道是否下发交并比
    int cameraResult[MAX_LAYER][MAX_VIEW][MAX_VIEW_UNIT]; // 查询结果  -1 未查到版本 0 在工厂 1 用户模式不匹配 2 正常
    int IpcStatisdataisrun; //
    int updateIpcStatisdata; // 1 更新统计数据显示
    int deleteIpcStatisdata; // 删除统计数据
    int camBadPoiontRet[MAX_VIEW][MAX_VIEW_UNIT];
    //int bAlarmStaticStatus[MAX_IPC_MODEL_CLASS];                    // 指定类别被识别 告警
    //目标采图模式
//    int trigArea;       //触发面积
//    int cutHeight;      //截断高度
//    int imgDim;         //图像维度

    int fpgaArithCount[MAX_VIEW];
    int  fpgaParmsforDebug[16];   // FPGA调试参数
    char fpgaDebugData[10][4+1472]; //0 view 1 unit  2-3 本包长度
    char fpgaAirthAdd[MAX_VIEW][100];

    stru_machine_set strumachineset;

}struShare;

/* 全局参数 */
/* 保存在 cnf.global */
typedef struct struCnfGlobal {
    /* 界面风格相关 */
    int nLang;                                                  // 语言
    int nCounter;                                               // 系统累计运行秒数
    int nProfile;                                               // 当前应用方案编号
    int nProfileTotal;                                          // 当前方案总数
    struct stru_Profile_Index stuProfileIndex[MAX_PROFILE];     // 方案文件名和对应显示的方案命名

    /* 各下位机启动模式 */
    int nInterfaceBoardMode;                            // 接口板启动模式
    //int nCameraBoardMode[MAX_LAYER][MAX_VIEW];          // 彩色相机板启动模式， 每个视的启动模式相同
    //int nCameraCompressMode[MAX_LAYER][MAX_VIEW];          // 彩色相机板压缩模式，
    int nVavleBoardMode[MAX_LAYER][MAX_VAVLE];          // 喷阀板启动模式， 每个吹的启动模式相同
    int nCtrlBoardMode;                                  // 控制板启动模式， 单接口板每个控制板的启动模式相同
    int nLampBoardMode;                                 // 控制板启动模式， 单接口板每个灯光板的启动模式相同

    /* 各下位机链在接口板的地址*/
    // int nViewChainAddr[MAX_LAYER][MAX_VIEW];


    /* 机型设置相关 */
    int nMachineAttr;                                           //机型属性， 0-立式机， 1-履带式
    int nMachineType;                                           // 当前机型
    int nLampBoardQuoType;                                      // 用于计算恒流源板灯光系数的机器类型，0：M大米机， 1：L1, 2:L2，3：L3
    int nMachineSerial;                                         // 机型定制
    int nMachineSpliceType;                                     //机器分割种类， 0-未分割机器， 1-M80合板带分割， 2-正常带分割的大机器
    int nInterfaceBoardTotal;                                   // 接口板总数
    int nLayerTotal;                                             // 当前层总数
    struct stru_layer_info struLayerInfo[MAX_LAYER];            // 各个层内部信息,即各接口板外设信息
    int nChuteTotal;                                            // 当前机器滑槽数

    int nCtrlBoardTotal;                                        // 控制板总数
    int nCtrlBoardType;                                         //主控板型号
    int nFeedMainTotal;                                         // 主振动器总数   //暂时不用
    int nFeedViceTotal;                                         // 辅振动器总数   //暂时不用
    //    int nLampChainTotal;                                      // 层内灯控链总数
    int nLampBoardType;                                         //恒流源板型号

    /* misc 控制 */
    int nWipeEnable;                                            // 当前机器是否有清灰
    int nControlAlarmSet;                                       // 控制板报警设置, 1:报警时关闭下料 0：报警时，不关下料
    int nVavleTimeSetMode;                                      //喷阀时间设置方式，0：按次设置， 1：按次、视设置, 默认0
    int nEnableDispArithSetByTimes;                             // 使能显示按次、视设置识别算法
    int nFeedControlType1;                                      // 振动器控制方式1, 0-旧振动控制方式， 12.5%~43.1， 1-新控制方式， 0~50%
    int nHasUps;                                                // 是否配置UPS
    int nSpray;                                                 //喷淋功能
    int nVibType;                                               //振动器方式
    int nWheelRunType;                                          //履带转动方向 int nWheelRunType;                                          //履带转动方向
    int nVibFreq;                                               //振动器频率 0：50hz    1：51hz
    int bigVibIsUse;                                             //振动筛
    int nBigVibPos;                                              //振动筛位置 0-12 默认0不使用
    int nVibFreq_2;                                               //振动筛/喂料器频率 0：50hz  1:51hz 2:30hz

    int bCrossCam;                         // 交叉相机
    int bCrossMode1;                       // 交叉相机模式一
    int bCrossMode2;                       // 交叉相机模式二
    int nValveSite;                                            // 喷阀前后置
    int nValveType;                                            // 喷阀类型 0，LT2，启动时间0.8ms；1，LT3，启动时间1.2ms
    int nValveRatio;                                           // 0,1:1 ; 1,1:2
    int nCorssEjector;

    /* 报警设置相关 */
    stu_alarm_cfg struAlarmConfig;
    int nAlarmValid[MAX_ALARM];                     // 报警设置:0-:disable alarm, 1-:enable alarm

    /* 智能清灰相关配置 */
    stu_intel_wipe_cfg struIntelWipe;                                                           //智能清灰配置

    /* 灯光控制 */
    stru_layer_lamp_cfg stuLayerLampCfg;
    /* 统计信息配置 */
    stru_statics_config nStaticsConfig[MAX_LAYER][MAX_VIEW];    //统计信息配置        //暂时不用
    int ntotalFeedTime;                                         //累计下料时间
    int ntotalAlarmCount;                                       //累计气压报警次数
    int nthisFeedTime ;                                         // 本次下料时间
    int nBigSmallArithMaxValue;                                 // 大小算法的设置上限
    int nEdgeCutMaxValue;                                       //边缘切除最大圈数
    int nImpuredMaxValue;                                       //传染最大行数
    float nEjectDurationMaxValue;                               //吹气时间最大值
    int ntotalWipeCount;                                        //累计清灰次数
    int totalblowcount[12*80];                                  // 各通道累计吹气次数

    /* 部件使用寿命配置 */
    stu_use_limit_cfg stuUseLimitCfg;
    /* 加密 */
    char sDelayCode[MAX_DELAYCODE];                             //加密密码
    int nTickDirect;                                            // 剔除的方向：0-123,1-213,2-321
    int nChuteSpliceMode;                                        // 不用，整机滑槽分割方式，0-不分割， 1-40:40， 2-20:60， 3-:60:20
    int nChuteSpliceAddr;                                        // 不用，滑槽分割地址， 1-通道1， n-通道n

    int sStaticsGetGroupEnable[MAX_LAYER][MAX_VIEW][MAX_GROUP_IDENTIFY];          // 允许获取含杂统计信息的层视组， 1-使能含杂统计， 0-不使能
    //    int sStaticsGetDuration;                                    // 统计信息间隔时间，流量调整专用，只查询前主
    int nAutoStaticsEnable;                                     //是否允许定时统计产量信息， 1-允许， 0-不允许
    int sTotalStaticsGetDuration;                               // 统计信息间隔时间，更新数据库专用，全部查询（单位是s）
    int nCmdSendTimeout;                                        // 指令下发超时
    int nUGSendTimeout;                                         // 升级文件下发超时
    int nUgSendInterval;                                        // 升级文件转发间隔
    int nWaitTimeout;                                           // 回包超时
    int nAutoStaticsWorking;                                    //演示还是处于正常工作
    float n_coff_user;                                          //实际统计产量/客户标定产量
    int nOnoffDelay;                                            //关闭下料时振动器关闭延时时间
    int nOnoffDelay_2;                                            //关闭下料时振动器开启延时时间
    int n_svmType;                                                //智能化参数类型，
    int nLVMachine;                                             //兼容lv杂粮程序
    int nPlcControl;                                            //plc控制使能，需配合料位报警使用
    int nAutoImageCaptureEnable;                                // 自动采集保存图像
    int bIpcCrossCam;                                           // 上层数量/下层数量不一致
    int nIpcCount;                                              //工控机数量
    int nIpcCamera;                                             //模型相机板数量
    int nIpcModelCount;                                         //模型数量
    int nUseIpcEnable;                                          // 使用工控机
    int nDPMachine;                                             //深度学习机型
    int nUseBMModule;                                           // 工控机型号bm1684
    int nUseLowcfgSync;                                         // se低配带同步信号
    int nIpcIpAddrBase;
    int nIpcCameraClassSameMode;                                //0-整机类别参数独立， 1-前后视类别参数相同
    int nIpcBadMatStaticsCount;                                 // 坏点统计次数，30秒一次
    int nIpcAlarmDealMode;                                      // IPC异常处理方式,0-不处理，1-要自动重启,
    int nIpcAlarmTurnOff;                                      // IPC异常处理方式,0-不处理，1-要关闭下料
    stu_ipc_camera_params struIpcCameraParams[MAX_LAYER][MAX_VIEW][MAX_VIEW_UNIT];
    stu_ipc_run_test_info struIpcRunTestInfo[MAX_VIEW][MAX_VIEW_UNIT][100];
    stu_ipc_config_file_info stuIpcConfigFileInfo[MAX_VIEW][MAX_VIEW_UNIT];
    int ipc_run_test_count;
    int nIpcRunTestIsRun;
    int nIpcRunTestCycle;

    //    stu_ipc_model_able struIpcModelInfo[MAX_IPC_NUM];
    //    stu_ipc_model_class struIpcModelClassInfo[MAX_IPC_NUM][MAX_IPC_MODEL_NUM];
    int nAllInOne;                                             //是否多合一板
    int nValveDefend;                                          //喷阀保护
    int nValveDefendTime;                                          //喷阀保护时间
    int nValveUndefendTime;                                         //喷阀保护解除时间

    int nIpcStartPixel[MAX_VIEW][MAX_VIEW_UNIT];
    int nIpcEndPixel[MAX_VIEW][MAX_VIEW_UNIT];
    int nScreenSet;                                            //是否自动熄屏
    //int nIsCameraConpress;                                     //是否图像压缩
    stru_version struVer;                   // 版本信息
    int camParaInCnf;
    float nFreeSpace; //单位:MB
    int nXsystemBoard[MAX_VIEW]; //1:表示是X系统 0:表示rk

    //柔性供料－－－－－－－－－－－－－－－－－－－－－－－－－－－
    stru_feed_auto_ctrl stuFeedAutoCtrl;                       //均衡流量控制设定
    int screenH;
    int nRealTimeCorrection; //需要实时校正
    int accelerationEnable[MAX_ACCELERATION];           // 加速度传感器使能
    int accelerationEnable2[MAX_ACCELERATION];          // 加速度传感器使能2
    int accelerationTolerance;                          // 加速度传感器容差控制
    int accelerationTolerance2;                         // 加速度传感器容差控制2
    int accelerationNum;                                // 加速度传感器个数 下料器选择 0斜料口 1直料口
    int accelerationNum2;                               // 加速度传感器个数2 下料器选择 0斜料口 1直料口

    int bFlagFirstrun;                      // 开机运行状态 : 1 - first run, 0 - not
    int viBoardTotalCount;//电流电压检测板总数
    int viBoardAlarmEnable;
    int viBoardInfo[MAX_VI_BOARD][4];//byte0:地址,1:类型,2:电压,3-电流

    //料位报警智能开关
    int bUseAutoFeeder;
    int alarmCmdType;//区分0x35和0324
    int accChecked;
    int imgSaturationFactor;
    int imgSaturationEnable;

    stru_modelabel_list stumodeLableList;
    stru_modelabel_select  stumodeLableSelect;
    int autoCtrlBoardAlive;                      //智能控制板连接状态保活
    int remoteCtrlIsRunning;
    int bautoplcctrlEnable;
    int nDelayedShutdownTime;       //延时关机时间
    int nAutoBoardIpAddr[4];                     //4段IP 默认192.168.1.100
    int nAutoBoardPort;                          //4段IP 默认502
    int nGateway[4];                             //网关 4段IP 默认192.168.1.1
    int nSubnetMask[4];                          //子网掩码 4段IP 默认255.255.255.0
    int nModbusIP;                                //Modbus 地址
    int nBaudRate;                                //波特率
    int nPlcCtrl;                               //使能PLC
    int nDefaultPlcCtrl;                        //默认使能PLC
    int nPowerCut;                              //关机是否切断电源
    int bUpdateStatus;                           //心跳锁
    int nBoardStatusAlarm;//
    int blampctrlEnable;
    int nSenseBrightSelection_calb[MAX_VIEW][MAX_CALIBRATE_SETS]; // 传感器亮度选择，０:0-11位，１:1-12位（rgb数据取各分量１5位中的...）
    int nSensorRowFre_calb[MAX_VIEW][MAX_CALIBRATE_SETS];        // 传感器行频
    int bCaliGainEnable_calb[MAX_CALIBRATE_SETS];
    int bCaliBiasEnable_calb[MAX_CALIBRATE_SETS];
    int bCaliTDmode_calb[MAX_VIEW][MAX_CALIBRATE_SETS]; // 分时方案
    long totalBlowCount[16*128]; // 各通道累计吹气次数

    int overRatio;                                      // 交并比
    int areaThres;                                      // 面积阈值
    int overRatioTime;                                  // 交并比等待时间
    int bGetMaterialType;                               // 获取统计数据类型  0 个数  1  物料面积 2 框面积
    int bMainpageShow;                                  // 主界面显示统计数据和含杂图片

    int bLogoLightNum;      // logo 灯个数
    int bLogoLightMode[MAX_LAMP_PER_BOARD];     // logo 灯模式 0 蓝 1 绿 2 橙 3 红 4 默认 5 橙色
    int bLogoLightLength[MAX_LAMP_PER_BOARD];   // logo 灯长度
    int fpgaParamsNum; //下位机参数设置个数;
    // 目标采图设置
    int trigArea ;
    int cutHeight;
    int imgDim;
    int autoTargetImage ;


}struCnfGlobal;

/*************************************************************************************
 *
 *      全局变量外部声明
 *
 ************************************************************************************/
extern struct struShare struGsh;            // 全局共享参数
extern struct struIpcShareInfo struIpcShare;    //模型参数

extern struct struCnfGlobal struCnfg;       // 全局保存参数
extern struct struCnfGlobal _t_struCnfg;    // 全局保存参数备份
extern struct struCnfProfile struCnfp;      // 全局方案参数
extern struct struCnfProfile _t_struCnfp;   // 全局方案参数
extern struct struCnfEngineer struCnfe;     // 全局工程文件参数
extern struct struCnfEngineer _t_struCnfe;  // 全局工程文件参数
extern struct struCnfFileStatus	struCnfs;   // 参数状态文件

#endif // GLOBALPARAMS_H
