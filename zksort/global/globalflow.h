#ifndef GLOBALFLOW_H
#define GLOBALFLOW_H

#include <QString>
#include <QObject>
#include <QTime>
#include <QCoreApplication>
#include <QVector>
#include "logger.h"
#include "bus/myqextserialport.h"
#include "bus/myqueue.h"
#include "bus/mynetwork.h"
#include "bus/myfastnetwork.h"
#include "bus/myudpthread.h"
#include "mydelaycode.h"
#include "mythread.h"
#include "globalparams.h"
#include "autocontrol/plcautoctrlmanager.h"
#include "systeminfo/tcpserverobj.h"

#define mydebug(arr,len) do{int i = 0;for(i = 0;i<len;i++) printf("%02x ",arr[i]);printf("\n");fflush(0);}while(0)

class NetWork;
class MyNetWorkThread;
class FastNetWork;
class MyFastNetWorkThread;

class GlobalFlow : public QObject           // 必须从QObject继承，否则不能用g_myLan().")方式显示文字
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
    public:
        //    GlobalFlow();
        explicit GlobalFlow(QObject *parent = 0);
    /* ---------------------------------------------------- */
    /* misc 函数 */
    void sleep(int secs);                                       // 设置秒级的延迟
    void msleep(int mSecs);                                     // 设置毫秒级的延迟

    //    int dataSortCompare(const void *a, const void *b);
    void dataSort(unsigned char *buf, int number, int byteSize, int riseDown);
    int dayDistance(int yearStart, int monthStart, int dayStart, int yearEnd, int monthEnd, int dayEnd);
    float calTemptureSens(int type, int value);
    float calPressureValue(int value);

    void getHsv(int Red, int Green, int Blue, int& H, int& S, int& V);

    void onOff();                                               // 开关下料
    void updateOnoff(int onOff);                                // 开启/关闭校正，进入相机设置界面关闭校正，退出时打开校正
    void startWipe(void);                                       // 清灰操作
    int dealIntelWipe();                                        // 处理智能清灰 返回0：智能清灰正常， 返回1：智能清灰异常
    void resetFeederIntelWipe(int direction, int isReturn, int curChute);                        // direction = 0, 正向顺序， direction=1,反序
    void initChannelPixelCompensate();                          // 镜头畸变的系数
    void getWipingCommunicationStatus();                        // 清灰时获取通道信号报警

    //    void setLang();                                             // 设置界面语言

    void initAll();                                             // 整机开机初始化工作
    void initGsh();                                             // 初始化全局共享参数
    void initLan();                                             // 初始化词根
    //    void initProfile(struCnfProfile& profile);                // 不用
    //    void initGroupIdentify(struCnfProfile& profile);            // 初始化识别组模块参数
    //    void initGroupTick(struCnfProfile& profile);                // 初始化剔除组模块参数

    /* ---------------------------------------------------- */
    /* 读取/保存参数函数 */
    void writeFpgaDccryptCode();
    void writeFpgaDccryptTime();
    void writeDccryptBiggerTime();
    int dccryptCodeTimeGet();
    void savedelaycode(QString);
    void getSetting();
    int  getParamsFileStaus();
    int  saveParamsFileStaus(int value);
    int  saveMemoryDetectedFileStaus(QString path);

    // fileType:0 engineer file, profileSeq: cnfg.profile, fileSeq:1 profile， 2： profile_2， value:0 profile save error

    void initProfile();
    void saveSetting();
    bool saveEngineer();                  // 保存工程模式参数
    void getEngineer();                   // 获取工程模式参数
    void getGlobal();                     // 获取全局参数
    bool saveGlobal();                    // 保存全局参数
    void getCamera();                     // 获取相机参数
    bool saveCamera();                    // 保存相机参数
    void getProfile();                    // 获取方案参数
    bool saveProfile();                   // 保存当前方案参数
    bool saveProfile(const QString& fileName, const struCnfProfile &cnfProfile); // 保存方案参数
    void getMachineConfig();  //获取配置文件参数
    void getCameraParams();
    void getVertRiceGeneralLayerInfo();     // 获取立式大米通用机型每层内视、组、相机单元信息
    void getBeltLd2GeneralLayerInfo();     // 获取双层履带机通用机型每层内视、组、相机单元信息

    bool getEngineerSetting(const QString & fileName);
    bool saveEngineerSetting(const QString & fileName);
    bool getGlobalSetting(const QString & fileName);
    bool saveGlobalSetting(const QString & fileName);
    bool getCameraSetting(const QString & fileName);
    bool saveCameraSetting(const QString & fileName);
    bool getProfileSetting(const QString & fileName);
    bool saveProfileSetting(const QString & fileName,const struCnfProfile &cnfProfile);

    void getFixedParamsByMachine();   //根据机型，恢复部分参数，比如M80默认一二次相同，防止已保存的方案参数干涉
    void createProfile();           // 创建方案
    void deleteProfile();           // 删除方案
    void getCompressParams();
    void checkCompressParams();
    int getSensorPixels(int nLayer, int nView);  //获取某视链上的具体相机分辨率
    void getGlobalLampConfig();                     //获取恒流源板灯光配置信息
    void getDefaultLampConfig();
    void getCameraViewDefaultArithEnable(int nLayer);                     //获取相机板默认算法使能
    QString getArithNameByIndex(int index);

    int getDiffArithColor(int diffColor, bool isSim);
    int changeColorBlackWhite2Nir(int color);
    /* ---------------------------------------------------- */
    void resetMachineConfigRG();                              // 重置机型配置
    void resetAllParams();                                   // 整机开机发送所有参数
    void resetFpgaRunModeRG();                                 // FPGA运行模式重置
    int  resetCommunication();                               // 开机通讯自检
    void resetFpgaBeforeRunModeRG(); // 相机版运行模式重置回原来模式
    void resetValveRunModeRG(); // 阀版FPGA运行模式重置
    void resetInterfaceRunModeRG(); // 接口版版FPGA运行模式重置
    void resetControlRunModeRG(); // 控制版FPGA运行模式重置
    void resetLampRunModeRG(); // 灯版FPGA运行模式重置
    QString getFpgaVersionProduct(int value);                            // 获取下位机FPGA程序产品种类， 大米/杂粮/茶叶/矿石...
    void resetMemoryDetection();                             // 开机存储介质检测
    void resetCrossCam();                                    // 配置是否为交叉相机

    void sendAll();
    void initSendAllParams();                               // 开机初始化发送所有参数
    /* 发送参数给FPGA的流程 */

    void resetCamera();                    // 重置相机参数
    void resetCameraAnaGainRG(bool bIsAll, int nLayerId, int nViewId, int nUnit);             // 重置相机模拟增益
    void resetCameraBiasRG(bool bIsAll, int nLayerId, int nViewId, int nUnit);                // 重置相机偏置
    void resetCameraPixelRG(bool bIsAll, int nLayerId, int nViewId, int nUnit);               // 重置相机像元划分
    void resetCameraPixelHoriRG();                                                            // 重置相机横向分辨率
    void resetCameraPixelDimRG();                                                             // 重置相机传感器维度
    void resetCameraImageConpressMode();                                                      // 重置相机图像压缩模式
    void resetSensorConfigRG();                                                               // 重置传感器帧频灯配置
    void resetCameraDropLine();                                                               // 重置相机丢行模式
    void resetCameraColorChoose();                                                            // 重置相机颜色分类选择
    void resetCameraResizeMode();                                                             // 重置相机resize模式
    void resetCameraEnhance() ;// 图像增强
    void SwitchUserMode(int view,int chute); // 切换双用户模式
    void SwitchAllViewUserMode();
    void getCameraEjectorPixel(int nLayerId, int nViewId, int nUnit);           //根据相机的起始截止像元，计算每个喷嘴的像元

    void resetCameraWaveType(int nLayerId, int nViewId, int nUnit);

    void updateCurViewImgHight();
    void updateCameraTimeDivisionMode(int nLayId,int nViewId);                                 //获取传感器分时模式，0:不分时,1:分时
    void cameraBrightCalibrateEnableRG();                                                     //明场矫正使能
    void cameraBrightCalibrateRG(bool bIsAll, int nLayerId, int nViewId, int nUnit, int flag);     //明场校正， flag = 0, 获取， flag = 1, 保存
    void cameraDarkCalibrateEnableRG();                                                     //暗场矫正使能
    void cameraDarkCalibrateRG(bool bIsAll, int nLayerId, int nViewId, int nUnit, int flag);       //暗场校正， flag = 0, 获取， flag = 1, 保存
    void cameraSaveCalibrate();
    void cameraChangeCalibrate();
    void resetCameraPixelCalibrateRG(bool bIsAll, int nLayerId, int nViewId, int nUnit);    // 亚像素校正重置
    void resetCameraNirDotCorrectiveRG(int nLayerId, int nViewId, int nUnit);                      // 红外坏点纠错
    void resetCameraNirTdiEnable();                      // 红外TDI使能
    void cameraWaveCaliEnable();                            // 相机信号校正增益使能、偏置使能、纠错使能
    void resetSensorNamesByLan();
    void resetSortNamesByLan();
    void resetSortParams();               // 重置色选参数
    void resetBackgroundColorRG(bool isAll, int nLayer, int nView);          // 重置背景颜色设置, iaAll = ture, 整机重新设置背景； nLayer view:视序号
    void resetAllArithParams();              // 重置所有算法参数
    void resetAllArithEnableClear(); // 清除所有算法使能
    void resetMaterial(int nLayerId, int nViewId, int nGroupId, bool bIsAllArith, int nArithSeq, int bIsReserved);   // 重置物料参数
    void resetSensUpdateEnable();           //灵敏度校正使能
    /* ---------------------------------------------------- */
    /* 识别相关函数 */
    int  getIdentifyGroupIndex(int nLayer, int nView, int nUnit);
    void getIdentifyGroupName();                            // 获取各个识别组模块名称
    //    void resetArithmeticEnable();                           // 重置算法使能
    void getArithmeticName();                               // 获取算法名称
    void resetFrontRearRelation();                          // 重置前后视算法关系
    void resetImageDealSize();                              // 重置图像处理尺寸， 传染尺寸、切边缘尺寸、正反选等
    void resetMatSizeReverseSort();                         // 重置最大行列尺寸和正选/反选功能

    void getPrefixedName();                                 // 获取预制方案名称
    int getIdentifyGroupAddr(int nLayer, int nViewChain, int nGroup, int nUnit);   // 获取识别组发送模块地址

    void setSvmParas(int layer,int view,int group,int svmIndex);
    void setShapeSvmParas(int layer,int view,int group,int ShapeSvmIndex);
    /* ---------------------------------------------------- */
    /*剔除相关函数 */
    void getTickGroupName();                            // 获取各个剔除组模块名称
    void resetTick();                                   // 重置剔除参数
    //    void getEelayImpure();                              // 获取传染算法延迟损耗
    //    void getDelayEdgeCut();                             // 获取边缘切除延迟损耗
    //    void getDelayCloseCal();                            // 获取闭运算延迟损耗
    //    void getDelayMatCut();                              // 获取切物料延迟损耗
    //    void getDelayMaterialSize();                        // 获取物料算法尺寸延迟损耗
    void resetEjectTime();                              // 重置喷阀时间
    int resetEjectDelayTime();                          // 发送相机板延迟时间、坏点个数、喷阀开关
    void resetEjectDurationTime();                      // 发送发送接口板吹气时间,吹气方式、喷阀数据与或关系
    void resetEjectVavleMode();                         // 发送发送接口板吹气方式系
    void resetEjectFrontRearLogic();                    // 发送发送接口板喷阀数据与或关系

    int  getTickGroupAddr(int nLayer, int nVavleChain, int nGroup, int nVavleBoard);   // 获取剔除组发送 喷阀板地址
    int  getTickGroupVavleNum(int nLayer,int nVavleChain,int nGroup);
    void resetSortOnOff(int onOff, int boardType);      //boardType:0所有电路板， 1：只控制接口板
    void vavleOnOff();              // 喷阀板关闭喷阀
    /* ---------------------------------------------------- */
    /*物料算法相关函数*/
    //    void materialParamsSet(int nCmd, int nLevelId, int nGroupId, int nIndex);
    void materialIdentifyParamsSend(int nCmd, int nLayerId, int nViewId, int nGroupId, int nIndex);                          // 物料识别算法下发
    void materialResetGroupAssemble(int nLayerId, int nViewId, int nGroupId, bool bIsAllArith, int nArithSeq, int bIsReserved);  // 物料识别参数发送组地址确定，暂时不用
    void materialParamsCopy(int nLayerIdSrc, int nViewIdSrc, int nGroupIdSrc, int nLayerIdDst, int nViewDst, int nGroupDst, bool bIsAllArith, int nArithSeq, int bIsReserved);// 物料参数复制
    void materialParamsCopySendAssemble(int nLevelId, int nViewId, int nGroupId, bool bIsAllArith, int nArithSeq, int bIsReserved, bool bNeedSend);        // 物料参数组地址确定

    void materialDispEnableCopy(int nLayerIdSrc, int nViewIdSrc, int nGroupIdSrc, int nLayerIdDst, int nViewDst, int nGroupDst, int nArithSeq);
    void materialArithEnableCopy(int nLayerId, int nViewId, int nGroupId, int nArithSeq, bool bIsDisp);
    /* ---------------------------------------------------- */
    /* 控制板&灯光板相关函数 */
    void getControlGroupName();         // 获取各个控制板组模块名称
    void resetControl();                // 重置控制板所有参数
    void resetSystemControlRG();          // 整机控制

    void autoFeedRadarLevelJudge();
    void getCurRadarLevel(int mode);            // 获取当前料位
    void resetFeederClear();
    void resetFeederRG();                 // 重置供料量
    void resetConveyor();                 //设置履带
    void resetConveyorSpeed();            //调节履带速度
    void resetAutoFeeder();               //调整临时供料量
    void resetWiperRG(int flag);          // 清灰控制
    void resetMotor();                  // 电机透传指令
    void resetLampRG();
    void resetLampPlcOff() ;          //PLC关闭灯光
    void resetControlAlarmRG();           // 1：报警时要关闭振动器，0：报警不要关闭振动器
    void setTsBackLight(int flag);      // 设置触摸屏背光
    void sendTsBackLightOn();
    void resetAccelerationParam();      // 发送加速度传感器参数

    /* 共享参数相关函数 */
    void setStringByDirect(void);       // 根据剔除方向设置字符串的显示
    void setHasInfByParams(void);       // 根据全局参数设置识别组是否有辅助相机标志位
    void clearDelayBias(void);          // 清空延迟偏置

    void resetAgeTestFre(int onOff);             // 重置整机老化频率

    /* rgb彩色相机专用函数 */
    void sendAllParamsRgb(void);                 // 彩色机型开机发送所有参数

    /* 下发色选参数函数 */
    int initCommunicationRS(void);              // 机型通信自检
    void sendDiscolorAndAreaParams(void);       // 发送病点色差与坏料面积参数
    void resetEjectTimeRS(void);                // 重置喷阀时间
    void sendCameraChannelParams(void);         // 发送所有相机板象元参数
    void sendAllParamsRS();                 // 机型开机及开关下料后发送所有参数
    void sendInterfaceParams(void);             // 发送接口板参数
    void sendCameraParams(void);                // 发送前置板参数
    void sendCtrlParams(void);                  // 发送控制板参数
    void sendSortParams(void);                  // 发送色选参数
    void sendChannelParams(int unit);		// 发送相机象元参数
    void sendAnaGainParams(int unit);           // 发送相机模拟增益参数
    void sendDigGainParams(int unit);           // 发送相机数字增益参数
    void sendGainTargetParams(int unit);        // 发送相机增益目标参数
    void sendInfChannelParams(int unit);	// 发送红外相机象元参数
    void sendInfAnaGainParams(int unit);        // 发送红外相机模拟增益参数
    void sendInfDigGainParams(int unit);        // 发送红外相机数字增益参数
    void sendInfGainTargetParams(int unit);     // 发送红外相机增益目标参数
    QString getNameByMode(int nMode);           // 根据模式索引号获取模式名称
    QString getImageShowName(QString imagePath);
    QString getUnitShowName(int view,int unit);
    void resetNetWaveStart();                   //波形界面时，当发生开关下料、开关喷阀、自动清灰、统计信息、报警信息等，需要重新给接口板发送清灰开始命令
    bool imagePixelIsBackground(uchar r, uchar g, uchar b, uchar n, int nLevel, int nViewId,int tdmode = 0);    //判断是否背景，true：是，
    bool imagePixelIsBad(uchar r, uchar g, uchar b, uchar n, int nLevel, int nViewId, int nGroup,int nUnit, int nArith);  //
    bool imagePixelIsBadBySvm(uchar r ,uchar g,uchar b,uchar n,int value);
    bool imagePixelTotalIsGood(uchar r, uchar g, uchar b, uchar n, int nLevel, int nViewId, int nGroup);
    bool imagePixelIsOutThreshold(uchar r, uchar g, uchar b, uchar n,int type);
    int getShapeLongShort(int left_x, int left_y, int top_x, int top_y, int right_x, int right_y, int btm_x, int btm_y);
    int getShapeCircleLong(int left_x, int left_y, int top_x, int top_y, int right_x, int right_y, int btm_x, int btm_y);


    /* RG大米机函数 >>>>*/
    int resetEjectDelayTimeRG();                          // 发送相机板延迟时间
    void resetEjectDurationTimeRG();                      // 发送发送接口板吹气时间
    void resetEjectWaitTimeRG();                          // 发送发送接口板等待时间
    void resetEjectTailTimeRG();                          // 发送发送接口板尾气时间
    void resetEjectVavleModeRG();                         // 发送发送接口板吹气方式系
    void resetEjectFrontRearLogicRG();                    // 发送发送接口板喷阀数据与或关系
    void resetVavleDefendTime();                          // 发送接口板喷阀保护时间

    void resetCameraBadNumRG();                         //坏点个数
    void resetCameraReverseSortRG();                    //正反选模式
    void resetCameraImpureRG();                         //传染尺寸
    void resetCameraEdgeCutRG();                        //边缘切除
    void resetCameraCenterSize();                       //中心化尺寸
    void resetArithRejectRatio();                       //dp控碎精度

    /* 模型相关 */
    void setIpcDelayBufRowCount();                      // 延迟时间缓存行数
    void setCameraColorRestore(int color);              // 重置颜色还原
    void getGlobalIpcConfig();                          // struCnfg初始化的模型
    void initIpcAttr();                                 // 初始化模型
    void getIpcInfo(bool isAll, int view,int unit);                                  // 获取工控机信息,开机发送
    void getIpcStatus(int view,int unit);                                // 获取工控机状态，定时发送，10秒
    void getIpcModelEnable(bool isAll,int view,int unit);                           // 获取工控机可用模型信息
    void getIpcModelInfo(bool isAll,int view,int unit);                             // 获取工控机模型具体信息
    void configIpcCamera();                             // 配置相机参数
    void configIpcClassParams(bool isall, int view,int unit);                        // 配置类别参数
    void loadIpcModel(int view,int unit, int modelIndex);        // 加载模型
    void configIpcSortStart(int stat);                          // 开始检测
    void configIpcSortStop(int stat);                           // 停止检测
    void shutdownIpc(bool bIsAll,int view,int unit,int stat);        // 关机
    void getIpcHardwareInfo(int ipcIndex);                          //获取工控机硬件和配置文件信息
    int getIpcCameraViewInfo(int &view,int &unit, int ipcIndex,int ipcCamId);   //通过ipc camid 获取对应视、通道信息
    int getIpcCameraViewUnitInfo(int &view,int &unit, int&ipId,QString Ip);   //通过ipc camid 获取对应视、通道信息
    int getIpcIndexByViewInfo(int view,int unit);                               //通过视、通道信息获取ipc索引
    void getIpcCamBadPointsCount();                                 // 获取工控机各相机含杂
    void setIpcCamOverRatio(int isall,int layer,int view,  int unit);                                 // 统计参数设置 模型加载后下发
    void clearIpcCamBadPointsCount();                               // 清除工控机各相机含杂
    void modelConfigure(int view, int unit);                        // 模型配置
    void deleteIpcmodel(bool bIsAll, int view, char *modelName);                     // rk模型删除
    void deleteXsysmodel(bool bIsAll, int view, char *modelName);                     // Xsys模型删除
    void ipcRunTestStart(bool status);                              // 开始/停止工控机连续运行测试
    void ipcSaveImg(bool status);                                   // 开始/停止工控机保存图片
    void configIpcAIPara(int view,int unit);
    void setRKInfo(bool bIsAll,int view,int unit,int type);         //设置rk信息
    void ipcResetCameraIpaddr(bool bIsAll, int nLayerId, int nViewId, int nUnit);
    void colorRestoreEnable();                          //颜色还原使能
    void resetIpcParams(int view,int unit);                 //重置发送IPC相关参数
    int getUpdateFileSize(QString fileName);
    void IpcClassResultANDOperat(bool isall, int view,int unit);
    int checkIpcModel();
    bool getModelTargetValue(bool isall,int view,int unit);
    /*下位机升级*/

    void switchUserMirror(uchar mode, uchar boardType, uchar boardNum, uchar controlKind);
    void readVersion(uchar boardType,uchar boardNum);
    void tellUpgrade(uchar boardType,uchar boardNum,uchar mirrorIndex);
    bool isNetWorkOk(int addr = 1);                   //网口通信是否正常
    NetWork *myNetWork;
    QString ipForInterface;
    QString portForInterfaceBoard;
    QString ipForFastNetwork;
    QString portForFastNetwork;
    int imgHeight;
    int startWithoutCnf;
    int interfaceEncryptAns;

    void MotorCtrl_Speed(quint16 speed);
    void MotorCtrl_Init();

    void getCamImgInfo(bool isall, int view,int unit);
    int saveBmp(uchar *pdata, QString bmp_file, int width, int height, int dim ,bool needCopy);

    void transformCameraOffset(int nLayerId, int nViewId, int nUnit);
    void resetIrCameraOffset(bool bIsAll, int nLayerId, int nViewId, int nUnit);              // 重置红外相机的绿色和蓝色偏移

    void saveScreenshot();
    int  getUnitGroup(int view, int unit, int group); // 相机通道所在的分次
    void sendStatus(int);
    bool boardSwitchFactory(int boardIndex,int chainIndex, int chuteIndex);
    int  getBoardAddr(int boardIndex, int chainIndex);
    bool boardUpdateSuccess(int boardIndex, int chainIndex, int chuteIndex, uchar versionToUpdate);

    void getFpgaCameraArith(int boardIndex, int chainIndex, int chuteIndex);     //获取当前相机支持算法种类
    void setFpgaRegisterStatus(int boardIndex, int chainIndex, int chuteIndex);  // 设置FPGA调试参数
    void getFpgaRegisterStatus(int boardIndex, int chainIndex, int chuteIndex);  // 获取寄存器状态

signals:
    void backgroundLightOnSig();         // 关背光后，点亮背光需要输入密码

private:
    void initInterfaceTransMode();      // 刚开机时，将接口板上传数据模式切换成串口模式，防止在USB模式下异常断电
    //    void getVertRiceGeneralLayerInfo();     // 获取立式大米通用机型每层内视、组、相机单元信息
    //    void getBeltLd2GeneralLayerInfo();     // 获取双层履带机通用机型每层内视、组、相机单元信息
    //    void getVerRiceB2BLayerInfo();          // 获取立式大米背靠背机型层内部信息


    void getGroupIdentify();            // 初始化识别组模块参数
    void getGroupTick();                // 初始化剔除组模块参数
    void getLocalIpcParams();           // 获取当前本地存储的模型参数

    void getArithmeticMacineEnable();   // 获取当前机型的各个算法使能状态
    long getFileSize(char *sFileName);  // 获取参数文件大小

    void getLampName();                 // 获取不同机型的等控名称
    QColor hsvColor;
    int h,s,v;
    void MotorCtrl(uchar objID, quint16 addr, quint16 val);
    void TransportSerial(const QVector<quint8> &dataIn, uchar boardType, uchar boardNumber);

};

extern GlobalFlow myFlow;

#endif // GLOBALFLOW_H
