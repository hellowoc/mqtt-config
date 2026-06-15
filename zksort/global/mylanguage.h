#ifndef MYLANGUAGE_H
#define MYLANGUAGE_H

#include <QObject>
#include "logger.h"
#include "constant.h"
#include "globalparams.h"
/* 必须从QObject继承，否则不能用g_myLan().")方式显示文字 */
class MyLanguage : public QObject
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
    public:
        explicit MyLanguage(QObject *parent = 0);
    //[home]
    QString	ids_conveyor;//传送带
    QString	ids_pogramIsRunning;//应用程序已经在运行
    QString	ids_connectSqlFailed;//连接数据库失败
    QString	ids_select;//精度
    QString	ids_feed;//产量
    QString	ids_cleaning;//清灰
    QString	ids_valve;//气阀
    QString	ids_model;//模式
    QString	ids_set;//设置
    QString	ids_setting;//设置
    QString	ids_run;//运行
    QString	ids_stop;//停止
    QString	ids_shutdown;//关机
    QString	ids_cannotRun;//停止工作中……
    QString	ids_toRun;//要开始运行吗？
    QString	ids_toShutdown;//确定关机吗？
    QString	ids_Detect_do_not_close;//请停止检测后在关闭程序！
    QString	ids_toSaveParameters;//要保存参数吗？
    QString	ids_output;//加工量
    QString	ids_capactiy;//产量
    QString	ids_broken;//含碎
    QString	ids_rejectedBroken;//含杂/碎
    QString	ids_yise ;// 异色
    QString	ids_qianhuan ;//浅黄
    QString	ids_ebai ;//恶白
    QString	ids_eza ;// 恶杂
    QString	ids_MachineExpired;//设备运行期限已到期，检测已停止。\n请立即关机，并联系厂家。
    QString	ids_WaitAFewSeconds;//履带状态没稳定，请稍等数秒。

    QString	ids_ok;//确定
    QString	ids_cancel;//取消
    QString	ids_back;//返回
    QString	ids_reset;//重置
    QString ids_space;//空格
    //	[selfcheck]
    QString	ids_deviceManager;//设备\n管理
    QString	ids_shutdownForCheck;//关机\n检查
    QString	ids_close;//关闭
    QString	ids_board;//电路板
    QString	ids_checkingRet;//检测结果
    QString	ids_versionInfo;//版本信息
    QString	ids_errorcode ;// 状态码
    QString	ids_initialize;//初始化……
    QString	ids_selfcheck;//开机自检……
    QString	ids_commPassed;//通信成功
    QString	ids_contactManufacturer;//系统被恶意篡改，请联系厂家
    QString	ids_expire;//设备到期，请输入解密码
    QString	ids_ToShutdownForCheck;//要关机检查吗？

    //	[version]
    QString	ids_userSoftware;//色选机控制软件
    QString	ids_interfaceBoard;//接口板
    QString	ids_controlBoard;//控制板
    QString	ids_ccsBoard;//恒流源板
    QString	ids_accelerationBoard;//振动检测板
    QString	ids_halogenBoard;//振动检测板
    QString	ids_frontMaster;//前主相机
    QString	ids_frontSlave;//前辅相机
    QString	ids_backMaster;//后主相机
    QString	ids_backSlave;//后辅相机
    QString	ids_valveBoard;//喷阀板
    QString	ids_queryingVersion;//版本查询中……
    QString	ids_query;//查询
    QString	ids_AboutToExpire1;//设备还有
    QString	ids_AboutToExpire2;//天到期
    QString	ids_BuyCode;//请及时购买运行期限码

    //	[select]
    QString	ids_fm;//前主
    QString	ids_bm;//后主
    QString	ids_fs;//前辅
    QString	ids_bs;//后辅
    QString	ids_more;//更多
    QString	ids_learn;//学习
    QString	ids_one;//一次
    QString	ids_two;//二次
    QString	ids_three;//三次
    QString	ids_four;//四次
    QString	ids_five;//五次

    QString	ids_selectAlgToDisplay;//勾选你要显示的算法
    QString	ids_byTimes;//分次设置
    QString	ids_allSame;//全部相同
    QString	ids_masterSame;//前后相同
    QString	ids_enableFM;//使能前主
    QString	ids_enableBM;//使能后主
    QString	ids_selectMore;//精度-更多
    QString	ids_AdjustSens;//校正精度

    QString	ids_channel;//通道
    QString ids_camNum;//镜头号
    QString	ids_sensitivity;//灵敏度
    QString	ids_advance;//高级
    QString	ids_reserve;//保留
    QString	ids_selectBright;//选亮
    QString	ids_selectDark;//选暗
    QString	ids_selectBig;//选大
    QString	ids_selectSmall;//选小
    QString	ids_selectTransparent;//选透
    QString	ids_selectWhite;//选白
    QString	ids_size;//尺寸
    QString	ids_purity;//斑点
    QString	ids_brightness;//亮度
    QString	ids_masterColor;//主色
    QString	ids_slaveColor;//辅色
    QString	ids_red;//红色
    QString	ids_green;//绿色
    QString	ids_blue;//蓝色
    QString	ids_black;//黑色
    QString	ids_white;//白色
    QString	ids_grayMColor;//灰度

    QString	ids_black_white;// 黑白
    QString	ids_n;// N色

    QString	ids_sensitivity2;//范围
    QString	ids_diffMColor;//色差
    QString	ids_diffSColor;//
    QString	ids_noise;//噪声
    QString	ids_selectLong;//选长
    QString	ids_selectShort;//选短
    QString	ids_selectCircular;//选圆
    QString ids_selectNotRound;//选非圆
    QString ids_selectThick;//选粗
    QString ids_selectThin;//选细


    //	[learn]
    QString	ids_imgProcess;//图像处理
    QString	ids_noProcess;//无
    QString	ids_colorProcess;//色选
    QString	ids_svmProcess;//学习
    QString	ids_setSample;//样本设置
    QString	ids_notSample;//非样本
    QString	ids_okAll;//全好
    QString	ids_okPlus;//选中好
    QString	ids_okMinus;//反选好
    QString	ids_ngAll;//全坏
    QString	ids_ngPlus;//选中坏
    QString	ids_ngMinus;//反选坏
    QString	ids_open;//打开图像
    QString	ids_resetAll;//重置样本
    QString	ids_resetLast;//撤销
    QString	ids_cannotLearnForNoSample;//未选取任何样本，无法学习
    QString	ids_cannotLearnForNoFeature;//未选取任何特征，无法学习
    QString	ids_tooFewSamples;//样本数量少于2个，不满足学习条件
    QString	ids_NoOkOrNgPixels;//没有好样本点或者坏样本点，无法学习
    QString	ids_NoOkOrNgSamples;//缺少好样本或者坏样本图片，无法学习
    QString	ids_learnFailed;//学习失败
    QString	ids_learnFinished;//学习完成，请注意保存学习结果
    QString	ids_onlyOpenImgAsFollows;//只能打开如下尺寸的图片：512*400，1024*400或2048*400
    QString	ids_learningType;//学习模型
    QString	ids_Type;//模型
    QString	ids_applyGoal;//应用目标
    QString	ids_master;//前后主
    QString	ids_allSmart;//智能1与2
    QString	ids_sampleDetails;//样本详情
    QString	ids_frontCamera;//前视
    QString	ids_backCamera;//后视
    QString	ids_saveRet;//保存结果
    QString	ids_selectbothFandB;//主视参数相同时，前视，后视须同时选中
    QString	ids_applyToSmart;//要将本次学习结果应用到当前设置目标吗？
    QString	ids_noSample;//没有选择样本
    QString	ids_NoCfgFile;//缺少配置文件
    QString	ids_color;//颜色
    QString	ids_erode2;//腐蚀
    QString	ids_oversize;//智能尺寸越界
    QString	ids_sampleColor;//样本色
    QString	ids_purple;//紫色
    QString	ids_cyan;//青色

    //	[feed]
    QString	ids_byChannelSet;//通道设置
    QString	ids_autoFeed;//自动产量
    QString	ids_CalibrateMass;//标定产量
    QString	ids_CalibrateInKG;//质量(kg)
    QString	ids_ToCalibrateMass;//确定标定产量吗？
    QString	ids_InputNonzeroValue;//请输入非零的标定产量值
    QString	ids_ClearTodaysData;//这将会清零今日统计值
    QString	ids_CalibrateFinished;//产量标定完成，标定系数为
    QString	ids_Conveyor;//输送带
    QString	ids_lowerFeed;//下层产量

    QString	ids_moreFeed;//产量-更多
    QString	ids_stopFeedWhenNoMat;//单通道无料停运
    QString	ids_QueryInterval;//间隔(s)
    QString	ids_StopCycles;//停止(T)
    QString	ids_MatFew;//料少
    QString	ids_MatMuch;//料多
    QString	ids_AdjustFeedSen;//差异性(%)
    QString	ids_AdjustStep;//步长

    QString	ids_SetYeild ;// 期望产量
    QString	ids_StopFeedThre ;// 停运门限
    QString	ids_EvenFeedRateThre ;// 匀料门限
    QString	ids_FeedSetRange ;// 调节范围
    QString	ids_FeedSetStep ;// 调节步长

    //[clean]
    QString	ids_cleaningTime;//时间（秒）
    QString	ids_cleaningInterval;//间隔（分）
    QString	ids_stop_channel_num_while_smart_clean ;// 停止通道
    QString	ids_cleaningMode;//清灰模式
    QString	ids_timingMode;//定时清灰
    QString	ids_teamMode;//联合清灰
    QString	ids_timingMode_smart ;//智能清灰
    QString	ids_cleaningEnable;//清灰使能
    QString	ids_isCleaning;//正在清灰

    //	[valve]
    QString	ids_blowTime;//吹气时间(ms)
    QString	ids_valveTest;//气阀测试
    QString	ids_index;//编号
    QString	ids_singleTest;//气阀单检
    QString	ids_cycleTest;//气阀循检
    QString	ids_stopTest;//停止检测
    QString	ids_towhichboard ;// 板级

    QString	ids_valveMore;//气阀-更多
    QString	ids_delayTime;//延时(ms)
    QString	ids_delayTime2 ;//延时2(ms)
    QString	ids_expand;//色选拓展
    QString	ids_shapeExpand;//形选拓展
    QString	ids_erode;//腐蚀尺寸
    QString	ids_reverseSelect;//反选
    QString	ids_lightSelect;//轻选
    QString	ids_heavySelect;//重选
    QString	ids_valveEnable;//气阀使能
    QString	ids_agingTest;//老化测试
    QString	ids_dilateSens;//膨胀阈值
    QString	ids_dilateSize;//膨胀尺寸
    QString	ids_centerSize;//中心
    QString	ids_fvAnd;//前后与
    QString	ids_waittime;//等待(ms)
    QString	ids_tailtime;//尾气(ms)
    QString	ids_sameForCameras;//以下设置不区分相机
    QString	ids_invalidTailTime;//吹气时间与等待时间之和不能大于5.6ms

    QString	ids_valveAging;//气阀老化
    QString	ids_agingFreq;//老化频率
    QString	ids_checkChannel;//勾选老化通道
    QString	ids_aging;//老化
    QString	ids_oneChannelAtLeast;//至少要选择一个通道


    QString	ids_time;//时间
    QString	ids_new;//新建
    QString	ids_copy;//复制
    QString	ids_delete;//删除
    QString	ids_rename;//重命名
    QString	ids_restore;//恢复
    QString	ids_apply;//应用
    QString	ids_selectOneModel;//请选择一个模式
    QString	ids_maxModelNumber;//模式库已满！建议先删除不需要的模式
    QString	ids_inputModelName;//请输入模式名
    QString	ids_copyModel;//复制模式
    QString	ids_modelNameNull;//模式名称不能为空
    QString	ids_sameName;//新模式与已有模式重名
    QString	ids_newModelError;//新建模式时发生错误
    QString	ids_selectOneTemplate;//请选择一个模式作为模板
    QString	ids_keepOneModelAtLeast;//至少需要保留一个模式
    QString	ids_cannotDeleteModelInUse;//不允许删除正在使用的模式
    QString	ids_cannotDeleteModelInLocked;//该模式处于锁定状态，无法删除。解锁请点击“更多”。
    QString	ids_toDeleteModel;//模式删除后无法恢复，确定要删除选中的模式吗？
    QString	ids_cannotRenameModelInLocked;//该模式处于锁定状态，无法重命名。解锁请点击“更多”。
    QString	ids_inputNewName;//请输入新的模式名
    QString	ids_renameFailed;//重命名失败
    QString	ids_renameModelError;//重命名模式时发生错误
    QString	ids_cannotSwitchModel;//设备运行过程中，禁止切换模式
    QString	ids_paramDisplay;//参数显示
    QString	ids_paramName;//参数

    QString	ids_displayEnable;//显示使能

    QString	ids_locked;//锁定
    QString	ids_lock;//锁定/解锁
    QString	ids_backup;//备份
    QString	ids_export;//导出库
    QString	ids_import;//导入库
    QString	ids_noMobileDevices;//没有寻找到移动存储设备！
    QString	ids_exportFinished;//用户模式库与厂家模式库导出完成
    QString	ids_yes;//是
    QString	ids_no;//否
    QString	ids_noLibrary;//移动设备里没有用户模式库或厂家模式库
    QString	ids_toImportLibrary;//确定导入模式库？
    QString	ids_libraryWillBeOverwritten;//本地的用户模式库与厂家模式库将会被覆盖，无法恢复！
    QString	ids_importFinished;//导入完成，重启系统后生效
    QString	ids_modelManagement;//模式管理
    QString	ids_enterPassword;//输入密码
    QString	ids_wrongPassword;//密码错误
    QString	ids_sameNameWhenBackup;//备份模式名称与厂家模式库里已有模式重名
    QString	ids_backupModelError;//备份模式时发生错误
    QString	ids_backupFinished;//完成模式备份

    QString	ids_selectModelToRestore;//选择要恢复的模式
    QString	ids_factoryModel;//厂家模式
    QString	ids_toRestoreModel;//确定恢复到用户模式库？
    QString	ids_sameNameWhenRestore;//恢复的模式名称与用户模式库里已有模式重名！
    QString	ids_restoreModelError;//恢复模式时发生错误！
    QString	ids_restoreFinished;//完成模式恢复
    QString	ids_default_library;//默认库
    QString	ids_select_all;//全选
    QString	ids_select_none;//全部不选
    QString	ids_selected;//选中
    QString	ids_toOverrideModel;//你要覆盖同名模式[%s]吗？
    QString	ids_readInfo;//正在读取模式信息

    //	[background]
    QString	ids_background;//背景
    QString	ids_image;//图像
    QString	ids_system;//系统

    QString	ids_redBkg;//红背景
    QString	ids_greenBkg;//绿背景
    QString	ids_blueBkg;//蓝背景
    QString	ids_NBkg ;// N背景
    QString	ids_blackBkg;//黑背景
    QString	ids_whiteBkg;//白背景
    QString	ids_black_whiteBkg ;// 黑白背景
    QString	ids_grayBkg ;// 灰背景
    QString	ids_yellowBkg ;// 黄背景
    QString	ids_irBkg;//I背景
    QString	ids_threshold;//阈值
    QString	ids_ratio;//比值
    QString	ids_originalSignal;//原始信号
    QString	ids_correctedSignal;//校正信号
    QString	ids_pixel;//像素
    QString	ids_gray;//灰度
    QString	ids_frontBkgBoard;//前背景板
    QString	ids_backBkgBoard;//后背景板
    QString	ids_uplimit;//上限
    QString	ids_lowlimit ;// 下限

    //	[image]
    QString	ids_getImage;//获取\n图像
    QString	ids_openImage;//打开\n图像
    QString	ids_deleteImage;//删除\n图像
    QString ids_importImage; //图片导入
    QString	ids_exportImage;//导出\n图像
    QString	ids_videoMode;//视频模式
    QString	ids_saveSet;//保存设置
    QString	ids_notSave;//不保存
    QString	ids_savePath1;//路径1
    QString	ids_savePath2;//路径2
    QString	ids_toDeleteImage;//确定要删除图像？删除后无法恢复！
    QString	ids_deleteImageFinished;//图像删除完成
    QString	ids_exportImageFinished;//图像导出完成
    QString	ids_getImage2;//同步\n拍照

    //	[system]
    QString	ids_remote;// 远程\n连接
    QString	ids_calibration;//屏幕\n校准
    QString	ids_remoteFailed;//远程助手启动失败!
    QString	ids_user;//账户设置
    QString	ids_operator;//普通用户
    QString	ids_engineer;//工程师
    QString	ids_skinStyle;//皮肤风格
    QString	ids_blueSkin;//活力蓝
    QString	ids_blackSkin;//魅力黑

    QString	ids_machineModel;// 机型
    QString	ids_mirror;//镜像
    QString	ids_lightSource;//光源
    QString	ids_correct;//校正
    QString	ids_devidePixel;//像元划分
    QString	ids_language;//语言
    QString	ids_upgrade;//升级
    QString	ids_encrypt;//编码

    //	[model]
    QString	ids_channelNumber;//通道数量
    QString	ids_valveModule;//喷阀模组
    QString	ids_resolution;//分辨率
    QString	ids_batchChannel;//通道分次
    QString	ids_machineModel2;//机型
    QString	ids_rice;//大米
    QString	ids_grains;//杂粮
    QString	ids_configFIR;//配前辅
    QString	ids_configBIR;//配后辅
    QString	ids_algorithm;//算法
    QString	ids_named;//命名
    QString	ids_switchAlg;//算法\n开关
    QString	ids_difference;//色差
    QString	ids_smart;//智能
    QString	ids_whiteBelly;//透白
    QString	ids_impurity;//恶杂
    QString	ids_masterGlass;//玻璃
    QString	ids_fsGlass;//近红玻璃
    QString	ids_bsGlass;//红外玻璃
    QString	ids_area;//面积
    QString	ids_length;//长度
    QString	ids_circular;//圆度
    QString	ids_selectOneAlg;//请选择一个算法
    QString	ids_selectOneGlass;//只能选择一种玻璃算法
    QString	ids_inputNewAlgName;//输入新的算法名称
    QString	ids_cannotEnableForNoFIR;//未配置前辅相机，该算法使能无法开启
    QString	ids_cannotEnableForNoBIR;//未配置后辅相机，该算法使能无法开启
    QString	ids_wrongResolution;//分辨率设置错误！只能设置为1024或者2048
    QString	ids_enable;//使能
    QString	ids_disable;//禁用
    QString	ids_algCfg;//算法配置
    QString	ids_init;//初始化
    QString	ids_toInit;//要初始化配置信息吗？
    QString	ids_resetName;//重置\n命名
    QString	ids_toResetName;//要重置所有模式的算法名称吗？

    QString	ids_saveAfterSet;//设定后需要保存，重启软件才能生效

    //	[light]
    QString	ids_frontLight;//前灯
    QString	ids_backLight;//后灯
    QString	ids_materialLight1;//物料灯上
    QString	ids_materialLight2;//物料灯下
    QString	ids_infraredLight1;//红外灯上
    QString	ids_infraredLight2;//红外灯下
    QString	ids_bkgLight;//背景灯
    QString	ids_infraredBkgLight;//红外背景
    QString	ids_turnOn;//打开
    QString	ids_light;//灯
    QString	ids_materialLight;//物料灯

    //	[correct]
    QString	ids_target;//目标
    QString	ids_gain;//增益
    QString	ids_offset;//偏置
    QString	ids_getCorrection;//明场\n校正
    QString	ids_saveCorrection;//保存\n明场
    QString	ids_toGetCorrection;//确定要明场校正？
    QString	ids_toSaveCorrection;//确定要保存明场校正？
    QString	ids_onekeyBright;//一键\n明场
    QString	ids_toOnekeyBright;//确定要一键获取并保存所有明场校正？
    QString	ids_getDark;//暗场\n校正
    QString	ids_saveDark;//保存\n暗场
    QString	ids_toGetDark;//确定要暗场校正？
    QString	ids_toSaveDark;//确定要保存暗场校正？
    QString	ids_onekeyDark;//一键\n暗场
    QString	ids_toOnekeyDark;//确定要一键获取并保存所有暗场校正？
    QString	ids_correctNG;//纠错
    QString	ids_toCorrectNG;//确定要纠错当前相机的坏点吗?
    QString	ids_position;//位置
    QString	ids_up;//上
    QString	ids_down;//下
    QString	ids_enableTDI;//TDI模式
    QString	ids_clearR;//清零R
    QString	ids_clearG;//清零G
    QString	ids_clearB;//清零B
    QString	ids_closeTdi;//需要关闭tdi模式，并切换到原始信号
    QString	ids_sureToClearNG;//确定要清零R坏点吗？
    QString	ids_sureToClearNG2;//确定要清零G坏点吗？
    QString	ids_sureToClearNG3;//确定要清零B坏点吗？
    QString	ids_sensor;//传感器
    QString	ids_sensorError;//传感器异常
    QString	ids_tooManyNG;//坏点数超过4个


    //	[dividePixel]
    QString	ids_startPixel;//起始像元
    QString	ids_endPixel;//结束像元
    QString	ids_pixelList;//像元\n列表
    QString	ids_pixelList2;//像元列表
    QString	ids_camera;//相机
    QString	ids_offset_x;//G偏移X
    QString	ids_offset_y;//G偏移Y
    QString	ids_offset_x_B;//B偏移X
    QString	ids_offset_y_B;//B偏移Y
    QString	ids_enableOffset;//偏置使能
    QString	ids_enableGainSetD2 ;// 增益锁定

    //	[language]
    QString	ids_chinese;//中文
    QString	ids_english;//English
    QString	ids_turkish;//Türkçe
    QString	ids_spanish;//español
    QString	ids_russian;//русск
    QString	ids_thai;//ภาษาไทย
    QString	ids_vietnamese;//Việt nam
    QString	ids_iranian;//فارسی
    QString	ids_portuguese;//Português - Brasil
    QString	ids_arabic;//العربية
    QString	ids_korean;//한국어
    QString	ids_indonesian ;// Indonesia

    QString	ids_selectOneLanguage;//选择一种语言
    QString	ids_noLanguageLib;//没有对应的语言库

    //	[upgrade]
    QString	ids_openUpgradeFile;//请打开升级文件
    QString	ids_openFile;//打开
    QString	ids_startUpgrade;//开始\n升级
    QString	ids_yourFileIs;//你的升级文件是：
    QString	ids_wrongFileName;//升级文件命名不符合规范！
    QString	ids_notSelectFile;//未选择升级文件！
    QString	ids_startUpgrading;//开始升级……
    QString	ids_openFileFailed;//打开文件失败
    QString	ids_sendFileFailed;//发送文件失败
    QString	ids_restoreInterfaceFailed;//恢复接口板用户模式失败
    QString	ids_restoreInterfaceSucceeded;//恢复接口板用户模式成功
    QString	ids_allChannel;//所有通道
    QString ids_allChain;//所有层
    QString	ids_upgradeSucceeded;//升级成功
    QString	ids_upgradeFailed;//升级失败
    QString ids_upgradeResult;//升级结果
    QString	ids_ipc;//上位机
    QString	ids_filesList;//文件名
    QString	ids_selectOtherFiles;//选择你需要的其它配置
    QString	ids_upgradeLog;//日志描述
    QString	ids_tellToUpgrade;//通知下位机准备升级
    QString	ids_sendData;//发送数据
    QString	ids_setFactoryFailed;//工厂模式设置失败
    QString	ids_setFactorySucceeded;//工厂模式设置成功
    QString	ids_upgrading;//正在升级……
    QString ids_chain; //链路
    QString ids_checkUpdateFile;//检测升级文件
    QString ids_startUpdate;//开始升级

    //	[encrypt]
    QString	ids_runTime;//运行期限
    QString	ids_invalidCode;//无效密码
    QString	ids_tooLongCode;//输入的密码过长！
    QString	ids_tooShortCode;//输入的密码过短！
    QString	ids_wrongCodeFormat;//密码格式错误！
    QString	ids_unmatchedCode;//该密码与机器编号不匹配！
    QString	ids_overdueCode;//这是一个时间过期密码！
    QString	ids_decryptMachine;//你要完全解密本设备？
    QString	ids_machineNumber;//你要加（解）密的设备编号是
    QString	ids_dueTime;//到期时间是
    QString	ids_dueProduction;//到期产量是
    QString	ids_restartMachine;//操作成功，请重启设备
    QString	ids_setCode;//设置加（解）密码
    QString	ids_productID;//机器编码：
    QString	ids_setID;//设置机器编码
    QString	ids_errorCodeRules;//错误的加密规则
    QString	ids_confirmTime;//你需要确认系统时间
    QString	ids_nowTime;//当前时间是：

    //    [info]
    QString	ids_status;//系统状态
    QString	ids_version;//版本查询
    QString	ids_report;//生产报表
    QString	ids_alarm;//报警设置
    QString	ids_maintenance;//维护保养
    QString	ids_remote_assist ;// 远程协助
    QString	ids_get_ovpn ;// 获取证书
    QString	ids_about;//关于本机

    //   [status]
    QString	ids_thisRunTime;//本次运行
    QString	ids_totalRunTime;//累计运行
    QString	ids_hours;//小时
    QString	ids_cloud;//云平台
    QString	ids_feedState;//下料状态
    QString	ids_feedAlarm;//供料报警
    QString	ids_cleaningAlarm;//清灰报警
    QString	ids_airpressureAlarm;//气压报警
    QString	ids_materialLevelAlarm;//料位报警
    QString	ids_statisticsInfo;//统计信息
    QString	ids_off;//关闭
    QString	ids_normal;//正常
    QString	ids_abnormal;//异常
    QString	ids_rejected;//含杂
    QString	ids_rejectedRatio;//含杂率
    QString	ids_cleaningFailed;//清灰失败
    QString	ids_glass;//玻璃
    QString	ids_powerDownProtection;//掉电保护
    QString	ids_TemperatureAlarm ;// 温度报警
    QString	ids_sensorTemperature;//传感器温度
    QString	ids_askTemperature;//查询\n温度

    //    [report]
    QString	ids_queryCycle;//查询周期
    QString	ids_startDate;//开始时间
    QString	ids_endDate;//结束时间
    QString	ids_queryMode;//查询方式
    QString	ids_byMachine;//按整机
    QString	ids_byChannel;//按通道
    QString	ids_date;//日期
    QString	ids_exportReport;//导出
    QString	ids_today;//今天
    QString	ids_oneWeek;//一周内
    QString	ids_oneMonth;//一月内
    QString	ids_threeMonth ;// 三月内
    QString	ids_custom;//自定义
    QString	ids_reportExportFailed;//报表导出失败
    QString	ids_reportExportDone;//报表导出完成
    QString	ids_displayOnHomepage;//主页显示统计信息

    //   [alarm]
    QString	ids_autoCleaning;//自动清灰
    QString	ids_stopFeed;//停振动器

    QString	ids_parts;//部件名称
    QString	ids_indicators;//性能指标
    QString	ids_runtimeValue;//运行值
    QString	ids_criticalValue;//临界值
    QString	ids_assessment;//评估警示
    QString	ids_sootCylinder;//清灰气缸
    QString	ids_runningTimes;//运行次数
    QString	ids_filtration;//过滤系统
    QString	ids_gasConsumption;//耗气总量
    QString	ids_switchTimes;//开关次数
    QString	ids_healthy;//健康
    QString	ids_subHealthy;//亚健康
    QString	ids_needChecked;//需体检

    //    [about]
    QString	ids_versionValue;//版本
    QString	ids_compiled;//编译
    QString	ids_allRightsReserved;//保留所有权利

    QString	ids_inputNumber;//当前数值：
    QString	ids_channelError;//各次通道数量之和与机型不匹配！
    QString	ids_alarmInfo;//报警信息
    QString	ids_lowAirpressure;//气压过低，请立即处理！
    QString	ids_highAirpressure;//气压过高，请立即处理！
    QString	ids_illegalDelay;//延时参数非法
    QString	ids_illegalDelayDetails;//延时过小，或尺寸或吹气参数过大
    QString	ids_material_empty ;// 物料空
    QString	ids_material_full ;// 物料满
    QString	ids_temperature_alarm ;// 温度异常

    //    [sort]
    QString	ids_Sort;//排序
    QString	ids_AddSortSample;//增加样本
    QString	ids_GetSortSample;//获取样本
    QString	ids_ResetSortSample;//重置样本
    QString	ids_SampleSize;//样本尺寸
    QString	ids_ToResetSortSample;//确定要重置样本吗？

    //    [cloud]
    QString	ids_expiredToken;//token过期，重新获取
    QString	ids_succeeded;//成功
    QString	ids_requestOkButReturnException;//请求成功，但云平台返回异常
    QString	ids_illegalURL;//非法URL
    QString	ids_requestFailed;//请求失败
    QString	ids_requestException;//请求遇到异常
    QString	ids_performErrorCode;//执行错误码
    QString	ids_perform;//并成功执行，
    QString	ids_postErrorCode;//上报错误码
    QString	ids_post;//并上报
    QString	ids_returnErrorCode;//返回错误码
    QString	ids_postRunLog;//运行日志发起上报：
    QString	ids_postModeInfo;//模式信息发起上报：
    QString	ids_postStatusInfo;//设备状态信息发起上报：
    QString	ids_postAlarmInfo;//报警信息发起上报：
    QString	ids_postMaterialInfo;//物料信息发起上报：
    QString	ids_receivedCapacity;//收到产量设置命令，
    QString	ids_receivedShutdown;//收到关机命令，
    QString	ids_receivedRunStop;//收到运行/停止命令，
    QString	ids_receivedSwitchMode;//收到模式切换命令，
    QString	ids_receivedSetAccuracy;//收到精度设置命令，
    QString	ids_receivedSave;//收到保存参数命令，
    QString	ids_receivedCmd;//收到命令：
    QString	ids_topicMatchingFailed;//订阅主题匹配失败
    QString	ids_mqttConnectionSucceeded;//mqtt连接成功
    QString	ids_loopSucceeded;//loop成功
    QString	ids_subscribeSucceeded;//订阅成功
    QString	ids_reconnectLoop;//loop失败，准备重连接。
    QString	ids_mqttReConnectionSucceeded;//mqtt重连接成功
    QString	ids_mqttReConnectionFailed;//mqtt重连接失败，
    QString	ids_subscribeFailed;//订阅失败，
    QString	ids_loopFailed;//loop失败，
    QString	ids_mqttConnectionFailed;//mqtt连接失败，
    QString	ids_show_yeild_per_channel ;// 实时产量
    QString	ids_Detect_do_not_test_eject ;// 检测中不能测阀!
    QString	ids_agree ;// 同意并继续
    QString	ids_disagree;//不同意并退出

    QString	ids_postRealTimeParamsInfo ;// 实时温度信息上报：
    QString	ids_postMachineParamsInfo ;// 整机参数信息上报：
    QString	ids_postPartStatusInfo ;// 部件状态加预警上报:
    QString	ids_postParamAlarmInfo ;// 参数异常警报上报:
    QString	ids_postStatisticsParamInfo ;// 产量统计数据上报：
    QString	ids_postRemoteProtocol ;// 远程连接同意上报:
    QString	ids_valveSystem ;// 喷阀系统
    QString	ids_lampSystem ;// 灯光系统
    QString	ids_running_time ;// 运行时间
    QString	ids_usefultime_valve ;// 喷阀寿命
    QString	ids_usefultime_filter ;// 过滤寿命
    QString	ids_usefultime_clean ;//清灰寿命
    QString	ids_usefultime_lamp ;// 灯光寿命
    QString	ids_operate_log ;//操作日志


    /************************ NEW **********************/
    QString input;                  //输入
    QString clear;                  //清空
    QString on;                     //开
    QString off;                    //关
    QString list;//文件名
    QString page_home;              //主界面
    QString save_parameters;        //保存参数
    QString set_purview;            //设置权限
    QString enter_system;           //进入系统
    QString last_page;              //上一页
    QString next_page;              //下一页
    QString state;                  //状态
    QString cfm_restore_default;    //恢复默认

    QString dc_set;                 //加密设置
    QString dc_input;               //请输入
    QString dc_productsn;           //产品编号
    QString dc_productsn_mismatch;  //产品编号错误
    QString dc_save_erro;           //保存错误
    QString dc_illegal;             //密码非法
    QString dc_limit;               //加密日期
    QString dc_illegal_used;        //密码重复使用

    QString vice_feeder;            //辅振动器
    QString feederCtrlType1;         //振动方式1
    QString main_view;
    QString vice_view;
    QString front_rear_vice;
    QString front;                  //前
    QString rear;                   //后
    QString front_rear_view;        //前后视
    QString upper;                  //上
    QString mid;                    //中
    QString lower;                  //下
    QString upper_view;             //上视
    QString lower_view;             //下视
    QString upper_layer;            //上层
    QString lower_layer;            //下层
    QString mid_layer;              //中层
    QString layer;                  //层
    QString times;                  //次
    QString splice;                 //分割
    QString ejector_num;            //喷嘴数量



    QString basic_set;              //基本设置
    QString standard;               //基准值
    QString impurity_name;         //杂质名称
    QString identify_params;        //识别参数
    QString arith_gray_a;           //灰度算法A
    QString arith_gray_b ;          //灰度算法B
    QString arith_gray_c ;          //灰度算法C
    QString arith_gray_d ;          //灰度算法D
    QString arith_discolor_a;       //色差算法A
    QString arith_discolor_b;       //色差算法B

    QString arith_cross;            //差分算法
    QString arith_svm_a;            //智能A
    QString arith_svm_b;            //智能B
    QString arith_svm_c;            //智能C
    QString arith_svm_d;            //智能C
    QString arith_big_small;        //选大选小
    QString arith_long_short;       //选长选短
    QString arith_long_short_b;     //选长短B
    QString arith_circle_long;      //选圆选长
    QString arith_hsv;              //颜色分选
    QString arith_sat;              //饱和
    QString arith_hue;              //颜色
    QString arith_scale;            //比例
    QString arith_mildew;           //霉点
    QString arith_outline;          //轮廓
    QString arith_line;             //纹理
    QString arith_bud;              //选芽
    QString arith_bud_b;            //选芽B
    QString arith_round;            //选圆、非圆
    QString arith_value;            //灰度
    QString arith_spl_mildew;       //葫芦籽霉点
    QString arith_white_peanut;     //白沙果算法
    QString arith_big_small_b;      //大小B
    QString arith_circle_long_b;    //圆长B
    QString arith_round_b;          //圆/非圆B
    QString arith_pepper_handle;    //辣椒选把

    QString related_mode;           //关联方式
    QString mode;                   //模式
    QString purity;                 //纯度
    QString colorful;               //彩色
    QString origin;                 //原图
    QString grey ;                  // 灰背景
    QString yellow ;                // 黄背景
    QString red_green;              //红绿色差
    QString red_blue;               //红蓝色差
    QString green_blue;             //绿蓝色差
    QString red_inf;                //红红外
    QString green_inf;              //绿红外
    QString blue_inf;               //蓝红外
    QString hsv_h;                  //h
    QString hsv_s;                  //s
    QString hsv_v;                  //v
    QString scale_index;            //比例索引
    QString yellow_sense;           //黄芽精度
    QString black_sense;            //黑芽精度
    QString sense_up;               //灵敏度上限

    QString common;                 //通用
    QString yellow_white;           //黄白
    QString yellow_alpha;           //黄透
    QString belly_white;            //腹白

    QString shape;                  //形选
    QString params_limit;           //参数下限
    QString second_alike_first;     //一二次相同
    QString all_seperate;           //全部独立
    QString all_alike;              //全部相同
    QString arith_propotion;        //比例算法
    QString percent;                //纯度
    QString light_limit;            //亮度下限
    QString arith_line_defect;      //线斑算法


    QString lamp_front_bg_red;      //前背景灯红
    QString lamp_front_bg_green;    //前背景灯绿
    QString lamp_front_bg_blue;     //前背景灯蓝
    QString lamp_rear_bg_red;       //后背景灯红
    QString lamp_rear_bg_green;     //后背景灯绿
    QString lamp_rear_bg_blue;      //后背景灯蓝
    QString lamp_upper_bg_red;      //上背景灯红
    QString lamp_upper_bg_green;    //上背景灯绿
    QString lamp_upper_bg_blue;     //上背景灯蓝
    QString lamp_low_bg_red;        //下背景灯红
    QString lamp_low_bg_green;      //下背景灯绿
    QString lamp_low_bg_blue;       //下背景灯蓝
    QString lamp_coef;              //系数


    QString wipe_set;               //清灰设置
    QString wipe_duration;          //清灰周期(S)
    QString wipe_enable;            //清灰使能
    QString wipe_single;            //单次清灰
    QString wipe_manual;            //手动清灰
    QString wipe_wind;              //清灰气帘
    QString wipe_wind_duration;     //清灰气帘周期
    QString wipe_wind_interval;     //清灰气帘间隔
    QString wipe_ai_set;            //智能清灰设置
    QString wipe_ai_off_chute;      //关闭滑槽数
    QString wipe_ai_speed;          //速度
    QString wipe_ai_width;          //宽度
    QString wipe_ai_propotion;      //比例
    QString wipe_ai_position;       //位置



    QString vavle;                  //喷阀
    QString ejector_test_fast;      //快速自建
    QString ejector_test_signal;    //信号自检
    QString ejector_keep_loop;      //单喷阀
    QString lamp_control;           //设置灯光
    QString tick_params;            //剔除参数
    QString infect_arithmetic;      //传染
    QString infect_arithmetic_1;    //传染一
    QString infect_arithmetic_2;    //传染二
    QString edge_cut;               //切除边缘
    QString edge_cut_1;             //切除边缘一
    QString edge_cut_2;             //切除边缘二
    QString bad_point;              //坏点个数
    QString logic_and;              //相与
    QString logical_relation;       //相与相或
    QString normal_sort;            //正选
    QString reverse_sort;           //反选
    QString sort_mode;              //色选模式
    QString close_calculate;        //闭运算
    QString svm_type1;
    QString svm_type2;
    QString svm_type3;


    // system status
    QString system_stat;            //系统状态
    QString camera_board;           //相机板
    QString recheck;                //重新检测
    QString software_version;       //版本
    QString version_factory;        //状态码 //内控版本
    QString version_custom;         //客户定制
    QString main_vavle;             //
    QString vavle_type1;
    QString vavle_type2;
    QString normal;                 //正常
    QString abnormal;               //异常
    QString eject_frequency;        //吹气频率
    QString system_time;            //系统时间
    QString year;                   //年
    QString month;                  //月
    QString day;                    //日
    QString hour;                   //时
    QString minute;                 //分
    QString sec;                    //秒
    QString update;                 //更新
    QString information;            //信息
    QString yield;
    QString total_impurities;
    QString single_impurity;
    QString eject_stat;             //喷阀状态
    QString alarm_turn_off;         //关振动器
    QString last_year_query;        //近一年查询
    QString realtime_query;         //实时查询
    QString total_feed_time;        //累计下料时间
    QString cur_pressure_alarm_times;//本次开机气压报警次数
    QString total_pressure_alarm_times;//累计气压报警次数
    QString pressure_value;         //气压
    QString build_time;             //编译时间
    QString clearTotalRunTime;      //累计运行时间清零

    // net
    QString remote_control;         //远程控制
    QString network_config;         //设置网络
    QString network_1;              //网络一
    QString network_2;              //网二
    QString ip_address;             //IP地址
    QString subnet_mask;            //子网掩码
    QString default_gateway;        //默认网关
    QString dns_server;             //DNS服务器
    QString mac_address;            //MAC地址
    QString route_address;          //路由器地址

    // remote control
    QString server_ip;              //服务器地址
    QString server_port;            //服务器端口
    QString local_virtual_ip;       //本地虚拟地址
    QString username;               //用户名
    QString password;               //密码
    QString connect;                //链接
    QString disconnect;             //断开链接
    QString network_test;           //网络测试
    QString msg_connecting;         //链接中。。。
    QString msg_connect_fail;       //链接失败
    QString msg_network_testing;    //网络连接检测中
    QString msg_network_success;    //网络连接成功
    QString msg_network_fail;       //网络连接失败
    QString msg_cert_request;         //申请证书
    QString msg_cert_not_exist;       //证书不存在
    QString msg_cert_request_success; //证书申请成功
    QString msg_cert_import;          //导入证书
    QString msg_cert_import_success;  //证书导入成功

    //	[模式]
    QString scheme_parameter;       //方案参数

    QString scheme_backup;          //备份方案
    QString scheme_cur;
    QString cfm_scheme_create;      //确认创建方案
    QString cfm_scheme_copy;        //确认拷贝方案
    QString cfm_scheme_rename;      //确认重命名方案
    QString cfm_restore_from_local; //确认从本地恢复方案
    QString cfm_backup_to_local;    //确认备份方案到本地
    QString msg_backup_to_local_success; //方案本地备份成功
    QString msg_backup_to_local_error;   //方案本地备份失败
    QString msg_restore_to_local_error;   //方案本地恢复失败
    QString cfm_restore_from_usb;       //确认从U盘导入方案
    QString msg_restore_from_usb_error; //U盘恢复方案错误
    QString cfm_backup_to_usb;          //确认导出方案到U盘
    QString cfm_select_current_scheme;  //确认选择当前方案
    QString cfm_save_current_scheme; //当前方案已更改，确认保存
    QString mode_discolor;              //异色粒正选
    QString mode_discolor_reverse;      //异色粒反选
    QString mode_milky;                 //乳白粒正选
    QString mode_milky_reverse;         //乳白粒反选
    QString mode_yellow_milky;          //黄白同选
    QString mode_yellow_trans;          //黄透同选
    QString mode_yellow_yellow_reverse; //黄+黄反选
    QString mode_yellow_white_reverse;  //黄+白反选
    QString mode_user_defined;
    QString cfm_reset_current_scheme_materialname; //确认还原当前方案算法名称


    // factory set
    QString factory_set;            //工厂模式
    QString engin_set;              //工程师设置
    QString advance;                //高级
    QString language;               //语言
    QString product;                //设置机型
    QString upgrade_screen;         //上位机升级
    QString upgrade_fpga;           //下位机升级
    QString mode_1;                 //模式1
    QString mode_2;                 //模式2
    QString mode_3;                 //模式3
    QString cfm_ts_calibrate;       //确认校准触摸屏
    QString cfm_upgrade_screen;     //确认升级升级上位机
    QString cfm_upgrade_fpga;       //确认升级下位机
    //factory
    QString camera_set;                 //设置相机
    QString signal_set;                 //相机信号
    QString system_misc_set;            //系统设置
    QString msg_times_set_err;          //分组错误
    // camera
    QString sensor_type;                //传感器类型
    QString sensor_dip;                 //分辨率
    QString sensor_dim;                 //维度
    QString sensor_bit_width;           //位宽
    QString sensor_fre;                 //帧频
    QString camera_chain_addr;          //地址
    QString camera_arith_config;        //算法配置
    QString camera_arith_list;          //算法列表
    QString camera_signal_bias_enable;  //偏置使能
    QString camera_signal_cali_enable;  //校正使能
    QString msg_bright_cali_capture;    //明场校正获取中...
    QString msg_bright_cali_save;       //明场校正保存...
    QString msg_dark_cali_capture;      //暗场校正获取中...
    QString msg_dark_cali_save;         //暗场校正保存...
    QString pixel_join;                 //亚像素调整
    QString show_ori_signal;            //显示信号
    QString show_bright_signal;//显示明场信号
    QString show_dark_signal;//显示暗场信号
    QString pixelBad;//坏点纠错
    QString ana_gain_reset;//增益重置
    QString cfm_ana_gain_reset;//确定增益重置
    QString get_all_bad;//获取所有坏点信息
    QString send_all;//发送所有

    QString ai_trigger_threshold;   //触发阈值
    QString threshold;              //阈值
    QString sample_set;             //样本设置
    QString bad_number;             //坏点数
    QString good_number;            //好点数
    QString clean_sample;           //清空样本
    QString cfm_clean_sample;       //确认清楚
    QString ai_image_save_error;    //保存错误
    QString ai_image_capturing;     //图像采集中...
    QString ai_image_capture_error; //图像采集错误
    QString ai_analysis;            //图像分析
    QString ai_analysis_set;        //图像分析设置
    QString msg_capture_image_first;//请先采集图像
    // ai
    QString ai_simulate;            //仿真
    QString ai_simulating;          //仿真中
    QString ai_basic;               //基本
    QString ai_more;                //更多
    QString ai_reselect_last;       //撤销
    QString ai_material;            //物料
    QString ai_calculate;           //智能计算
    QString ai_add_sample;          //添加样本
    QString ai_sample_color;        //取样颜色
    QString ai_sample_sens;         //取样灵敏度
    QString ai_sample_mode;         //取样模式

    QString ai_good;                //好
    QString ai_bad;                 //坏
    QString msg_computing ;         //正在计算中...
    QString msg_usb_insert;         //请插入U盘
    QString msg_usb_not_recognized; //U盘不识别
    QString set_logo;               //设置logo
    QString msg_usb_logofile_not_exist;         //logo文件不存在
    QString msg_usb_logofile_replace_success;   //文件替换成功
    QString msg_compute_error;      //计算错误
    QString ai;//智能化
    QString backup_all;//备份所有
    QString ai_auto_cal;            //一键智能
    QString ai_manul;
    QString ai_err_no_bad;          //请设置坏物料
    QString ai_err_no_good;         //请设置好物料
    QString ai_err_no_set;          //请设置物料好坏

    QString ai_success;
    QString ai_err_fail_write;
    QString ai_err_no_config_file;
    QString ai_unknow_err;
    QString ai_unsupported;
    QString cfm_rename_image;
    QString cfm_ai_mark_sample;
    QString rgb;
    QString rgn;
    QString rbn;
    QString gbn;
    QString movefix;
    QString show_bad_all;
    // [pixel scale]
    QString pixel_restore;              //像元重置

    QString nir_bad_set;                //红外相机坏点设置
    QString nir_bad;                         //坏点
    QString nir_bad_point;                  //坏点坐标
    QString nir_tdi;                        //TDI设置
    QString nir_up_down;                    //TDI上下
    QString nir_correct;                    //纠错后
    QString nir_show_all;                   //全显示

    // [message]
    QString msg_cfm_save_para;          //参数已更改，确认保存？
    QString msg_applying;               //应用中...
    QString msg_turning_on;             //正在打开下料
    QString msg_turning_off;            //正在关闭下料
    QString msg_turn_off_first;         //请先关闭下料
    QString msg_shutdown;               //正在关机中...
    QString msg_wiping;                 //正在清灰中...
    QString msg_communicating;          //正在通信自检中...
    QString msg_require_version;        //正在版本检测中...
    QString msg_system_init;            //正在系统初始化...
    QString msg_mode_changing;          //正在切换模式中...
    QString msg_output_testing;         //正在产量测试中...
    QString msg_change_init_mode;       //正在切换启动模式...
    QString msg_saving_params;          //正在保存参数中...
    QString msg_importing_params;       //正在导入参数中...
    QString msg_exporting_params;       //正在导出参数中...
    QString msg_params_save_error;      //保存参数出错
    QString msg_delay_time_limit;       //延迟时间设置出错
    QString msg_tip_reboot;             //参数导出完成，设备即将重启

    // confirm
    QString cfm_turn_on;                //确认打开下料
    QString cfm_import_parameter;       //确认导入参数
    QString cfm_export_parameter;       //确认导出参数

    QString setByChute;                 //按料槽
    QString setByGroup;                 //按次

    // feeder
    QString feeder;                 //流量
    QString stop_duration;          //停运时长
    QString adjust_interval;        //调节间隔
    QString search_interval;        //查询间隔
    QString searching;              //正在获取信息

    // fpga upgrade
    QString last;
    QString next;
    QString msg_insert_data_wire;
    QString user_mode;
    QString board_num;
    QString upgrade;
    QString upgrade_retry;
    QString upgrade_whole;
    QString upgrade_return;
    QString upgrade_file_list;
    QString upgrade_board;
    QString upgrade_fail;
    QString upgrade_ok;
    QString msg_select_user_mode;
    QString cfm_upgrade_whole;
    QString cfm_insert_udisk;
    //<<<<<<<<<<

    //sysstate
    QString valve_test;
    QString version_info;
    QString machine_info;

    QString flow_statis;
    QString vendor;
    QString operating_mode;         //工作模式
    QString set_delay_time_by_time;  //按次设置延迟时间
    QString arith_params_set_alike;  //参数是否设置相同
    QString arith_big_small_limit;   //大小算法上限
    QString edge_cut_max_limit;   //边缘切除圈数上限
    QString impured_max_limit;   //传染行数上限
    QString eject_duration_max_limit;   //吹气时间上限
    QString ui_style;                //界面风格
    QString flow_set;
    QString realtime_statistical_query_cycle; //实时统计查询周期
    QString one_click_recovery;//一键还原当前方案算法名称
    QString alarm_method;//报警方式
    QString lighting;//灯光
    QString sound;//声音
    QString level_sensor;//料位传感器
    QString top;  //料位传感器是否放置在顶部
    QString temperature_sensor;     //温度传感器
    QString pressure_alarm_type;    //压力报警装置
    QString pressure_alarm_switch;  //压力开关
    QString pressure_alarm_digi;  //数显表

    QString sensorNameRestore;        //传感器名称还原
    QString port;                     //串口
    QString recovery_set;             //还原设置
    QString recover_all;              //恢复全部
    QString recover_part;             //部分恢复
    QString recover_cancel;           //取消恢复
    QString cfm_recover_all;          //确认全部恢复
    QString cfm_recover_part;         //确认部分恢复
    QString recover_all_cfg_success;  //全部恢复配置成功
    QString recover_part_cfg_success; //部分恢复配置成功
    QString days_avaiable;
    QString rice_mode;
    QString material_mode;
    QString synchronization;
    QString independent;
    QString continuous;
    QString continuous_sync;        //连续同步
    QString horizontalSpacing;      //横向间隔
    QString verticalSpacing;        //纵向间隔
    QString good_bad_differ;        //好坏相异
    QString rm_redundancy;          //去除冗余
    QString index;
    QString nir_bad_list;           //坏点列表
    QString noModify_nonProfessionals;   //非专业人士请勿设置系统参数

    QString accept_remote_upgrade; //接受远程升级
    QString start;          //开始
    QString pause;          //暂停
    QString downloading;    //正在下载
    QString download_pause; //暂停下载
    QString download_stop;  //关闭下载
    QString download_ok;    //升级包下载完成
    QString download_fail;  //升级包下载出错

    QString wipe_continue_eject;  //清灰不停喷阀
    QString wipe_continue_feed;   //清灰不停振动器
    QString crossCAM;		  //交叉相机
    QString valveboard_pos;	  //喷阀板位置
    QString big_feeder_pos;	  //振动筛位置
    QString big_feeder;		  //使用振动筛
    QString big_feeder_freq;  //振动筛频率
    QString feeder_freq;	  //振动器频率
    QString teaFeeder;		  //喂料器
    QString type;           //类型
    QString heavy;          //深(霉点)
    QString light;          //浅
    QString all;            //全

    QString restore;        //还原
    QString pls_reboot_normaly;    //请正常断电重启系统后再操作

    QString spray; //喷淋
    QString wheelRun;//履带正转
    QString shortDrive; //短驱动
    QString longDrive;  //长驱动

    QString ipc_colorRestore;   //IPC颜色还原
    QString ipc_model;
    QString ipc_classCount;
    QString ipc_blowTime2;
    QString ipc_lightBlow;
    QString ipc_heavyBlow;
    QString ipc_flag;
    QString ipc_modelChange;
    QString ipc_toalCount;
    QString ipc_h_move;
    QString ipc_h_width;
    QString ipc_v_move;
    QString ipc_v_width;
    QString ipc_info;
    QString ipc_state;
    QString ipc_h_resolution;
    QString ipc_v_resolution;
    QString ipc;
    QString ipc_num;
    QString ipc_typeName;

    QString ids_allInOne;//多合一板
    QString languageChange;//切换语言
    QString checkLanChange;
    QString checkIPCOnOff;
    QString waitingModel;
    QString modelLoadingErr;
    QString gasConsumption;
    QString ipcState_idle;
    QString ipcState_running;
    QString ipcState_loading;
    QString ipcState_error;

    QString imgCapTureCount; //图像采集张数
    QString msg_memory_space_inadequate; //存储空间不足
    QString cfm_upload_ipcinfo;          //确认接受Ipc信息上传
    QString msg_reset_machine_info;      //请重新设置机器信息
    QString ids_valveDefend;             //喷阀保护
    QString ids_valveDefendTime;         //喷阀保护时间
    QString ids_valveUndefendTime;       //保护解除时间
    QString alarm;                       //报警
    QString bad_low_limit;               //坏点下限
    QString mat_low_limit;               //有料下限
    QString check;                       //检测
    QString checking;                    //测试中
    QString total_frame;                 //总帧
    QString compression_ratio;           //压缩率
    QString speed;                       //速率
    QString cfm_start_net_test;          //确认开始网络测试
    QString stop_testing;                //停止测试中
    QString cfm_start_ipc_status_test;   //确认开始测试工控机状态
    QString bad_spot_statistical_time;   //坏点统计时间
    QString alarm_and_resatrt_ipc;       //报警时重启工控机
    QString alarm_and_feed_off;          //报警时关闭下料
    QString add_model;                   //添加模型
    QString change_model;                //切换模型
    QString apply_model;                 //应用当前模型
    QString choose_model;                //选择当前模型
    QString check_model;                 //检测可用模型
    QString not_satrt;                   //未开始
    QString model_file_miss;             //模型文件缺少
    QString model_file_mismatch;         //模型文件不匹配
    QString connect_ipc_fail;            //无法链接工控机
    QString model_file_upgrading;        //模型文件升级中，请稍候...
    QString upgrade_timeout;             //升级超时
    QString upgrade_success;             //升级成功
    QString model_file_upgrade_fail;     //模型文件升级失败
    QString config_file_upgrade_fail;    //配置文件升级失败
    QString other_error;                 //其他错误
    QString msg_ipc_poweroff;            //工控机未开启

    QString ids_ScreenSet;//自动熄屏
    QString ids_nMirror;//是否镜像
    QString ids_saturation;     //饱和度
    QString maximum;            //最值
    QString cur_model;          //当前模型
    QString ipc_reboot;         //重启工控机
    QString cfm_ipc_reboot;     //确认重启工控机
    QString msg_ipc_clean_filter_cotton;  //请及时清理工控机散热风机过滤棉
    QString brightSelect;       //亮度选择
    QString sample_analysis;    //样本分析
    QString cfm_sync_class_name; //确认要同步模型类别名称
    QString classType;           //类别
    QString consecRunTest;       //连续运行测试
    QString compression;         //压缩
    QString dropLine;            //丢行
    QString cfm_ld_state;        //确认履带控制状态

    QString cfm_bright_cali_capture; //确认明场校正获取
    QString cfm_dark_cali_capture;   //确认暗场校正获取
    QString cfm_bright_cali_save;    //确认明场校正保存
    QString cfm_dark_cali_save;      //确认暗场校正保存

    QString ipc_syncName;            //同步名称
    QString ipc_verificating;        //校验中...
    QString filter_background;    //过滤背景
    QString colorSelect;          //颜色选择
    QString ids_arithRejectRatio; //控碎
    QString arith_suppress_large; //压制大
    QString turns;                //圈数
    QString lowCfg_sync;          //压缩同步
    QString arith_wheat_sprout;   //小麦芽头
    QString freeSpace;            //剩余空间
    QString ids_delay_select;     //延时选择
    QString resizeMode;           //保密模式
    QString dualLayerModelSame;   //上下层模型相同
    QString max_latency;          //最大延时
    QString keyPoint;             //关键点
    QString pos;                  //坐标
    QString HSCapture;              //快速采图
    QString delay;                  //传输延迟
    QString realtimeCorrect;        //实时校正
    QString stopCapturing;          //停止采图中
    QString stopFRCapturing;        //停止前后采图中
    QString ejectstart; //起始吹嘴
    QString ejectend;  //截止吹嘴
    QString ids_acceleration;   // 加速度传感器
    QString ids_acceleration_feedback1; //占空比1；
    QString ids_acceleration_feedback2; // 占空比2；
    QString ids_acceleration_tolerance; // 容差值
    QString ids_acceleration_feeder;    // 下料器选择
    QString ids_acceleration_oblique;   // 斜料口
    QString ids_acceleration_straight;  // 直料口
    QString ids_intelligent_control;    // 智能控制
    QString ids_vib_assist_control;     // 振动器辅助控制


    QString cfm_screen_shot; // 确认截屏
    QString screen_shot_finish;  // 截屏完成
    QString Please_cfm_machine_number;
    QString Hot_mode;          //热点模式
    QString WIFI_mode;  //wifi 模式
    QString WIFI_name;          //热点名称
    QString current_mode;       //当前模式
    QString change_mode;       //切换模式
    QString connected_machine; // 已链接设备
    QString update_lib; // 更新库
    QString export_screenshot_file; // 导出截屏文件
    QString download; // 下载
    QString introduction; // 简介
    QString Thumb_up; // 点赞
    QString No_introduction; //没有简介
    QString current_page; //当前页
    QString  ids_kernelTime;//内核版本
    QString  ids_remote_download;//远程下载

    QString ids_autoControlBoard;
    QString cfm_open_plc_ctrl;
    QString cfm_close_plc_ctrl;
    QString plc_ctrling;
    QString auto_ctrl_board_offline;
    QString auto_ctrl_board_ip;
    QString auto_ctrl_board_port;
    QString gateway;
    QString baudrate;
    QString ids_querying;
    QString plc_ctrl;
    QString DelayPowerCut;//延时断电
    QString AutoCtrlEnable;
    QString PowerCut;
    QString DefaultPlcEnable;
    QString plc_ctrl_set;
    QString model_same_all;
    QString model_same_view;
    QString model_same_group;
    QString Begin_IP;
    QString End_IP;
    QString InstructTimeout;
    QString UpgradeFileTimeout;
    QString ForwardingInterval;
    QString PacketTimeout;
    QString OneKey_Update;

    QString Intelligent_Set;
    QString TCP_Set;
    QString Server_connection;
    QString Screenshot;
    QString together_export_image;
    QString usePreSetCnf;
    QString cfmClearAllCurrentCnf;
    QString	cfmChgCorrection;//确定要切换明场校正？
    QString	ids_And;//相与
    QString	NoChange;//
    QString ChangeUserMode;
    QString	ids_setUserModeFailed;//
    QString	ids_setUserModeSucceeded;//

    QString	All_eject;// 全吹
    QString length_width; // 长宽
    QString	thickness;// 粗细
    QString Other_Group_error; // 其他次加载错误
    QString staticSet;
    QString allDayData;
    QString histogram;
    QString PieChart;
    QString unitton;
    QString batchName;
    QString batchInfo;
    QString classGroupNum;
    QString image_enhance;   //图像增强
    QString staticClassname;
    QString staticClassdata;

    QString Buffer_lines; // 缓冲行数
    QString Confirm_information; // 确认信息
    QString information_error ; //信息错误
    QString BG_points_cannot_be_marked ;// 背景点不能被标记
    QString Already_marked_as_good; // 已经被标记成好料
    QString Already_marked_as_bad; // 已经被标记成坏料
    QString Infection_intensity ; // 传染强度
    QString Bad_material_enough;// 坏料已足够
    QString Save_sample_failed;// 保存样本失败
    QString Good_material_enough;// 好料已足够
    QString Marked_material_not_saved;// 当前标记物料没有被保存
    QString Completed; // 已完成
    QString No_file_detected; // 未检测到文件
    QString Reapply_machine_model; // 重新应用机型

    QString Unzip_success ;// 解压成功
    QString Unzip_failed ;// 解压失败
    QString Bottom; // 底
    QString Low ; // 低
    QString Medium;//  中
    QString High;// 高
    QString Pass;// 跳过
    QString Abort_upgrade;// 终止升级
    QString ids_midPixel;// 中间像元
    QString MaterialLevel;// 料位
    QString Temperature; // 温度
    QString Reblow_time_not_less_blow_time; //重吹不小于吹气时间
    QString Reblow_time_setting_error; //重吹时间设置出错
    QString Adjust_ratio; // 调整比例
    QString Box_maximum_value;// 框最大值
    QString Switch; // 切换
    QString Model_Nomatch_calibration;   // 模型与机型校正值不匹配
    QString Stop_AI_image_capture;       // 停止AI采图
    QString Remote_request_lockControl;  // 远程请求锁屏控制
    QString Remote_lockControl ;         // 远程正在操作
    QString Test_Version; // 测试版程序
    QString Download_rate;// 下载速度
    QString Remaining_time;// 剩余时间
    QString File_size;// 文件大小
    QString Sample_import_success;  // 样本导入成功
    QString Sample_import_failed ;  // 样本导入失败

    QString No_sample_selected ;    // 未选取任何样本，无需保存
    QString good_is_sufficient;     // 好料已经足够
    QString bad_is_sufficient;      // 好料已经足够
    QString data_not_entered;       // 有数据未输入
    QString Material_count;         // 物料个数
    QString Material_area;          // 物料面积
    QString Frame_area;             // 框面积
    QString Invalid_scheme;         // 无效方案
    QString Must; // 必须
    QString cfm_perform; // 是否确认执行
public:
    void initLangTs();
    int  g_languageIndex;//语言索引
    void ChangeLanguage();
private:
    void loadLan(QString filePath);
    QMap<QString,QString> m_LanMap;
};

#endif // MYLANGUAGE_H
