#include <QString>
#include <QSettings>
#include "mylanguage.h"
#include <QTranslator>
#include <QFontDatabase>
#include <QApplication>
#include <QStringList>
#include <QDebug>
#include <QFile>
MyLanguage::MyLanguage(QObject *parent): QObject(parent)
{
    QSettings setting(CFG_APPSet,QSettings::IniFormat);

    /* 读取界面语言信息 */
    g_languageIndex =  setting.value("languageindex",1).toInt();
}
void MyLanguage::ChangeLanguage()
{
    QString qmFileName;
    static QTranslator *translator;
    if(translator != NULL){
        qApp->removeTranslator(translator);
        delete translator;
        translator = NULL;
    }

    translator = new QTranslator;

    logger()->info("%1",QString::number(g_languageIndex));
    switch(g_languageIndex)
    {
    case LANG_CHS:
        qmFileName = QString("./lang/Chinese.ini");
        break;
    case LANG_ENG:
        qmFileName = QString("./lang/English.ini");
        break;
    case LANG_RUS:
        qmFileName = QString("./lang/Russian.ini");
        break;
    case LANG_VIET:
        qmFileName = QString("./lang/Vietnam.ini");
        break;
    case LANG_THAILAND:
        qmFileName = QString("./lang/Thai.ini");
        break;
    case LANG_SPAN:
        qmFileName = QString("./lang/Spanish.ini");
        break;
    case LANG_CHT:
        qmFileName = QString("./lang/Chinese_tw.ini");
        break;
    case LANG_KOREAN:
        qmFileName = QString("./lang/Korean.ini");
        break;
    case LANG_TURKEY:
        qmFileName = QString("./lang/Turkish.ini");
        break;
    case LANG_CAMBODIA:
        qmFileName = QString("./lang/Cambodia.ini");
        break;
    case LANG_POLAND:
        qmFileName = QString("./lang/Poland.ini");
        break;
    case LANG_BURMA:
        qmFileName = QString("./lang/Burma.ini");
        break;
    case LANG_PERSIA:
        qmFileName = QString("./lang/Iranian.ini");
        break;
    case LANG_UKRAINE:
        qmFileName = QString("./lang/Ukraine.ini");
        break;
    case LANG_ITALY:
        qmFileName = QString("./lang/Italy.ini");
        break;
    case LANG_GREEK:
        qmFileName = QString("./lang/Greek.ini");
        break;
    case LANG_UIGHUR:
        qmFileName = QString("./lang/Uighur.ini");
        break;
    case LANG_ARABIC:
        qmFileName = QString("./lang/Arabic.ini");
        break;
    case LANG_INDONESIA:
        qmFileName = QString("./lang/Indonesia.ini");
        break;
    case LANG_BULGARIAN:
        qmFileName = QString("./lang/Bulgarian.ini");
        break;
    case LANG_FRENCH:
        qmFileName = QString("./lang/French.ini");
        break;
    case LANG_SWEDISH:
        qmFileName = QString("./lang/Swedish.ini");
        break;
    case LANG_JAPAN:
        qmFileName = QString("./lang/Japanese.ini");
        break;
    case LANG_PORTUGUESE:
        qmFileName = QString("./lang/Portuguese.ini");
        break;
    case LANG_HINDI:
        qmFileName = QString("./lang/Hindi.ini");
        break;
    case LANG_GERMAN:
        qmFileName = QString("./lang/German.ini");
        break;
    default:
        qmFileName = QString("./lang/English.ini");
        break;
    }
#ifdef Q_OS_UNIX
//        QFontDatabase database;
//        foreach(const QString &family, database.families())
//        {
//            qDebug() << family;
//        }
//    以上这段代码就可以枚举出系统支持的所有字体的名称；字体目录放在qt根目录下打lib/fonts中

    QFont font = qApp->font();
    font.cleanup();
    /* 泰文时需修改字体 */
    if(g_languageIndex == LANG_THAILAND){
        font.setFamily("Leelawadee");
    }else if(g_languageIndex == LANG_BURMA ||g_languageIndex == LANG_ARABIC || g_languageIndex == LANG_PERSIA || g_languageIndex == LANG_VIET){/* 缅甸语时需修改字体 */
        font.setFamily("Tahoma");
    }else if(g_languageIndex == LANG_HINDI){
        font.setFamily("Amita");
    }
    else{
        font.setFamily("WenQuanYi Micro Hei");
    }
    qApp->setFont(font);
#endif
    QSettings setting(CFG_APPSet,QSettings::IniFormat);
    setting.setValue("languageindex",g_languageIndex);
    setting.sync();

    loadLan(qmFileName);
    initLangTs();
    QEvent ev(QEvent::LanguageChange);
    QCoreApplication::sendEvent(qApp, &ev);
}

void MyLanguage::loadLan(QString filePath)
{
    m_LanMap.clear();
    QFile lanFile(filePath);

    if(!lanFile.open(QIODevice::ReadOnly |QIODevice::Text))
    {
#ifdef IGNORE_DEBUG
#else
        qDebug()<<lanFile.errorString();
#endif
        return;
    }

    QTextStream in;
    in.setDevice(&lanFile);
    QString lineStr;
    QStringList lineStrList;
    while(!in.atEnd())
    {
        lineStr = in.readLine().replace("\\n","");
        if(lineStr.contains("=")){
            lineStrList = lineStr.split("=");
            if(lineStrList.count() == 2){
                m_LanMap.insert(lineStrList[0].simplified(),lineStrList[1].simplified());
            }
        }
    }
}

void MyLanguage::initLangTs()
{
    ids_conveyor = m_LanMap.value("ids_conveyor");//传送带
    ids_pogramIsRunning = m_LanMap.value("ids_pogramIsRunning");//应用程序已经在运行
    ids_connectSqlFailed = m_LanMap.value("ids_connectSqlFailed");//连接数据库失败
    ids_select = m_LanMap.value("ids_select");//精度
    ids_feed = m_LanMap.value("ids_feed");//产量
    ids_cleaning = m_LanMap.value("ids_cleaning");//清灰
    ids_valve = m_LanMap.value("ids_valve");//气阀
    ids_model = m_LanMap.value("ids_model");//模式
    ids_set = m_LanMap.value("ids_set");//设置
    ids_setting = m_LanMap.value("ids_setting");//设置
    ids_run = m_LanMap.value("ids_run");//运行
    ids_stop = m_LanMap.value("ids_stop");//停止
    ids_shutdown = m_LanMap.value("ids_shutdown");//关机
    ids_cannotRun = m_LanMap.value("ids_cannotRun");//停止工作中……
    ids_toRun = m_LanMap.value("ids_toRun");//要开始运行吗？
    ids_toShutdown = m_LanMap.value("ids_toShutdown");//确定关机吗？
    ids_Detect_do_not_close = m_LanMap.value("ids_Detect_do_not_close");//请停止检测后在关闭程序！
    ids_toSaveParameters = m_LanMap.value("ids_toSaveParameters");//要保存参数吗？
    ids_output = m_LanMap.value("ids_output");//加工量
    ids_capactiy = m_LanMap.value("ids_capactiy");//产量
    ids_broken = m_LanMap.value("ids_broken");//含碎
    ids_rejectedBroken = m_LanMap.value("ids_rejectedBroken");//含杂/碎
    ids_yise  = m_LanMap.value("ids_yise");// 异色
    ids_qianhuan  = m_LanMap.value("ids_qianhuan");//浅黄
    ids_ebai  = m_LanMap.value("ids_ebai");//恶白
    ids_eza  = m_LanMap.value("ids_eza");// 恶杂
    ids_MachineExpired = m_LanMap.value("ids_MachineExpired");//设备运行期限已到期，检测已停止。\n请立即关机，并联系厂家。
    ids_WaitAFewSeconds = m_LanMap.value("ids_WaitAFewSeconds");//请稍等...

    ids_ok = m_LanMap.value("ids_ok");//确定
    ids_cancel = m_LanMap.value("ids_cancel");//取消
    ids_back = m_LanMap.value("ids_back");//返回
    ids_reset = m_LanMap.value("ids_reset");//重置
    ids_space = m_LanMap.value("ids_space");//空格
    //	[selfcheck]
    ids_deviceManager = m_LanMap.value("ids_deviceManager");//设备\n管理
    ids_shutdownForCheck = m_LanMap.value("ids_shutdownForCheck");//关机\n检查
    ids_close = m_LanMap.value("ids_close");//关闭
    ids_board = m_LanMap.value("ids_board");//电路板
    ids_checkingRet = m_LanMap.value("ids_checkingRet");//检测结果
    ids_versionInfo = m_LanMap.value("ids_versionInfo");//版本信息
    ids_errorcode  = m_LanMap.value("ids_errorcode");// 状态码
    ids_initialize = m_LanMap.value("ids_initialize");//初始化……
    ids_selfcheck = m_LanMap.value("ids_selfcheck");//开机自检……
    ids_commPassed = m_LanMap.value("ids_commPassed");//通信成功
    ids_contactManufacturer = m_LanMap.value("ids_contactManufacturer");//系统被恶意篡改，请联系厂家
    ids_expire = m_LanMap.value("ids_expire");//设备到期，请输入解密码
    ids_ToShutdownForCheck = m_LanMap.value("ids_ToShutdownForCheck");//要关机检查吗？

    //	[version]
    ids_userSoftware = m_LanMap.value("ids_userSoftware");//色选机控制软件
    ids_interfaceBoard = m_LanMap.value("ids_interfaceBoard");//接口板
    ids_controlBoard = m_LanMap.value("ids_controlBoard");//控制板
    ids_ccsBoard = m_LanMap.value("ids_ccsBoard");//恒流源板
    ids_accelerationBoard = m_LanMap.value("ids_accelerationBoard");//振动检测板
    ids_halogenBoard = m_LanMap.value("ids_halogenBoard"); // 电流板
    ids_frontMaster = m_LanMap.value("ids_frontMaster");//前主相机
    ids_frontSlave = m_LanMap.value("ids_frontSlave");//前辅相机
    ids_backMaster = m_LanMap.value("ids_backMaster");//后主相机
    ids_backSlave = m_LanMap.value("ids_backSlave");//后辅相机
    ids_valveBoard = m_LanMap.value("ids_valveBoard");//喷阀板
    ids_queryingVersion = m_LanMap.value("ids_queryingVersion");//版本查询中……
    ids_query = m_LanMap.value("ids_query");//查询
    ids_AboutToExpire1 = m_LanMap.value("ids_AboutToExpire1");//设备还有
    ids_AboutToExpire2 = m_LanMap.value("ids_AboutToExpire2");//天到期
    ids_BuyCode = m_LanMap.value("ids_BuyCode");//请及时购买运行期限码

    //	[select]
    ids_fm = m_LanMap.value("ids_fm");//前主
    ids_bm = m_LanMap.value("ids_bm");//后主
    ids_fs = m_LanMap.value("ids_fs");//前辅
    ids_bs = m_LanMap.value("ids_bs");//后辅
    //    if(struCnfg.nMachineType == MACHINE_LD2_ZL_GP){
    //        ids_fm = m_LanMap.value("lower_layer") + m_LanMap.value("upper_view");//下层上视
    //        ids_bm = m_LanMap.value("lower_layer") + m_LanMap.value("lower_view");//下层下视
    //        ids_fs = m_LanMap.value("upper_layer") + m_LanMap.value("upper_view");//上层上视
    //        ids_bs = m_LanMap.value("upper_layer") + m_LanMap.value("lower_view");//上层下视
    //    }
    ids_more = m_LanMap.value("ids_more");//更多
    ids_learn = m_LanMap.value("ids_learn");//学习
    ids_one = m_LanMap.value("ids_one");//一次
    ids_two = m_LanMap.value("ids_two");//二次
    ids_three = m_LanMap.value("ids_three");//三次
    ids_four = m_LanMap.value("ids_four");//四次
    ids_five = m_LanMap.value("ids_five");//五次

    ids_selectAlgToDisplay = m_LanMap.value("ids_selectAlgToDisplay");//勾选你要显示的算法
    ids_byTimes = m_LanMap.value("ids_byTimes");//分次设置
    ids_allSame = m_LanMap.value("ids_allSame");//全部相同
    ids_masterSame = m_LanMap.value("ids_masterSame");//前后相同
    ids_enableFM = m_LanMap.value("ids_enableFM");//使能前主
    ids_enableBM = m_LanMap.value("ids_enableBM");//使能后主
    ids_selectMore = m_LanMap.value("ids_selectMore");//精度-更多
    ids_AdjustSens = m_LanMap.value("ids_AdjustSens");//校正精度

    ids_channel = m_LanMap.value("ids_channel");//通道
    ids_camNum  = m_LanMap.value("ids_camNum");//镜头号
    ids_sensitivity = m_LanMap.value("ids_sensitivity");//灵敏度
    ids_advance = m_LanMap.value("ids_advance");//高级
    ids_reserve = m_LanMap.value("ids_reserve");//保留
    ids_selectBright = m_LanMap.value("ids_selectBright");//选亮
    ids_selectDark = m_LanMap.value("ids_selectDark");//选暗
    ids_selectBig = m_LanMap.value("ids_selectBig");//选大
    ids_selectSmall = m_LanMap.value("ids_selectSmall");//选小
    ids_selectTransparent = m_LanMap.value("ids_selectTransparent");//选透
    ids_selectWhite = m_LanMap.value("ids_selectWhite");//选白
    ids_size = m_LanMap.value("ids_size");//尺寸
    ids_purity = m_LanMap.value("ids_purity");//斑点
    ids_brightness = m_LanMap.value("ids_brightness");//亮度
    ids_masterColor = m_LanMap.value("ids_masterColor");//主色
    ids_slaveColor = m_LanMap.value("ids_slaveColor");//辅色
    ids_red = m_LanMap.value("ids_red");//红色
    ids_green = m_LanMap.value("ids_green");//绿色
    ids_blue = m_LanMap.value("ids_blue");//蓝色
    ids_black = m_LanMap.value("ids_black");//黑色
    ids_white = m_LanMap.value("ids_white");//白色
    ids_grayMColor = m_LanMap.value("ids_grayMColor");//灰度

    ids_black_white = m_LanMap.value("ids_black_white");// 黑白
    ids_n = m_LanMap.value("ids_n");// N色

    ids_sensitivity2 = m_LanMap.value("ids_sensitivity2");//范围
    ids_diffMColor = m_LanMap.value("ids_diffMColor");//色差
    ids_diffSColor = m_LanMap.value("ids_diffSColor");//
    ids_noise = m_LanMap.value("ids_noise");//噪声
    ids_selectLong = m_LanMap.value("ids_selectLong");//选长
    ids_selectShort = m_LanMap.value("ids_selectShort");//选短
    ids_selectCircular = m_LanMap.value("ids_selectCircular");//选圆
    ids_selectNotRound = m_LanMap.value("ids_selectNotRound");//选非圆
    ids_selectThick = m_LanMap.value("ids_selectThick");//选粗
    ids_selectThin = m_LanMap.value("ids_selectThin");//选细

    // ai
    ai_simulate = m_LanMap.value("ai_simulate");            //仿真
    ai_simulating = m_LanMap.value("ai_simulating");        //仿真中
    ai_basic = m_LanMap.value("ai_basic");               //基本
    ai_more = m_LanMap.value("ai_more");                //更多
    ai_reselect_last = m_LanMap.value("ai_reselect_last");       //撤销
    ai_material = m_LanMap.value("ai_material");            //物料
    ai_calculate = m_LanMap.value("ai_calculate");           //智能计算
    ai_add_sample = m_LanMap.value("ai_add_sample");          //添加样本
    ai_sample_color = m_LanMap.value("ai_sample_color");        //取样颜色
    ai_sample_sens = m_LanMap.value("ai_sample_sens");         //取样灵敏度
    ai_sample_mode = m_LanMap.value("ai_sample_mode");         //取样模式

    ai_good = m_LanMap.value("ai_good");                //好
    ai_bad = m_LanMap.value("ai_bad");                 //坏
    msg_computing  = m_LanMap.value("msg_computing");         //正在计算中...
    msg_usb_insert = m_LanMap.value("msg_usb_insert");         //请插入U盘
    msg_usb_not_recognized = m_LanMap.value("msg_usb_not_recognized");//U盘不识别
    set_logo = m_LanMap.value("set_logo");               //设置logo
    msg_usb_logofile_not_exist = m_LanMap.value("msg_usb_logofile_not_exist");         //logo文件不存在
    msg_usb_logofile_replace_success = m_LanMap.value("msg_usb_logofile_replace_success");   //文件替换成功
    msg_compute_error = m_LanMap.value("msg_compute_error");      //计算错误
    ai = m_LanMap.value("ai");//智能化
    backup_all = m_LanMap.value("backup_all");//备份所有
    ai_auto_cal = m_LanMap.value("ai_auto_cal");            //一键智能
    ai_manul = m_LanMap.value("ai_manul");
    ai_err_no_bad = m_LanMap.value("ai_err_no_bad");          //请设置坏物料
    ai_err_no_good = m_LanMap.value("ai_err_no_good");         //请设置好物料
    ai_err_no_set = m_LanMap.value("ai_err_no_set");          //请设置物料好坏

    ai_success = m_LanMap.value("ai_success");
    ai_err_fail_write = m_LanMap.value("ai_err_fail_write");
    ai_err_no_config_file = m_LanMap.value("ai_err_no_config_file");
    ai_unknow_err = m_LanMap.value("ai_unknow_err");
    ai_unsupported = m_LanMap.value("ai_unsupported");
    cfm_rename_image = m_LanMap.value("cfm_rename_image");
    cfm_ai_mark_sample = m_LanMap.value("cfm_ai_mark_sample");
    rgb = m_LanMap.value("rgb");
    rgn = m_LanMap.value("rgn");
    rbn = m_LanMap.value("rbn");
    gbn = m_LanMap.value("gbn");
    movefix = m_LanMap.value("movefix");
    show_bad_all = m_LanMap.value("show_bad_all");
    //	[learn]
    ids_imgProcess = m_LanMap.value("ids_imgProcess");//图像处理
    ids_noProcess = m_LanMap.value("ids_noProcess");//无
    ids_colorProcess = m_LanMap.value("ids_colorProcess");//色选
    ids_svmProcess = m_LanMap.value("ids_svmProcess");//学习
    ids_setSample = m_LanMap.value("ids_setSample");//样本设置
    ids_notSample = m_LanMap.value("ids_notSample");//非样本
    ids_okAll = m_LanMap.value("ids_okAll");//全好
    ids_okPlus = m_LanMap.value("ids_okPlus");//选中好
    ids_okMinus = m_LanMap.value("ids_okMinus");//反选好
    ids_ngAll = m_LanMap.value("ids_ngAll");//全坏
    ids_ngPlus = m_LanMap.value("ids_ngPlus");//选中坏
    ids_ngMinus = m_LanMap.value("ids_ngMinus");//反选坏
    ids_open = m_LanMap.value("ids_open");//打开图像
    ids_resetAll = m_LanMap.value("ids_resetAll");//重置样本
    ids_resetLast = m_LanMap.value("ids_resetLast");//撤销
    ids_cannotLearnForNoSample = m_LanMap.value("ids_cannotLearnForNoSample");//未选取任何样本，无法学习
    ids_cannotLearnForNoFeature = m_LanMap.value("ids_cannotLearnForNoFeature");//未选取任何特征，无法学习
    ids_tooFewSamples = m_LanMap.value("ids_tooFewSamples");//样本数量少于2个，不满足学习条件
    ids_NoOkOrNgPixels = m_LanMap.value("ids_NoOkOrNgPixels");//没有好样本点或者坏样本点，无法学习
    ids_NoOkOrNgSamples = m_LanMap.value("ids_NoOkOrNgSamples");//缺少好样本或者坏样本图片，无法学习
    ids_learnFailed = m_LanMap.value("ids_learnFailed");//学习失败
    ids_learnFinished = m_LanMap.value("ids_learnFinished");//学习完成，请注意保存学习结果
    ids_onlyOpenImgAsFollows = m_LanMap.value("ids_onlyOpenImgAsFollows");//只能打开如下尺寸的图片：512*400，1024*400或2048*400
    ids_learningType = m_LanMap.value("ids_learningType");//学习模型
    ids_Type = m_LanMap.value("ids_Type");//模型
    ids_applyGoal = m_LanMap.value("ids_applyGoal");//应用目标
    ids_master = m_LanMap.value("ids_master");//前后主
    ids_allSmart = m_LanMap.value("ids_allSmart");//智能1与2
    ids_sampleDetails = m_LanMap.value("ids_sampleDetails");//样本详情
    ids_frontCamera = m_LanMap.value("ids_frontCamera");//前视
    ids_backCamera = m_LanMap.value("ids_backCamera");//后视
    ids_saveRet = m_LanMap.value("ids_saveRet");//保存结果
    ids_selectbothFandB = m_LanMap.value("ids_selectbothFandB");//主视参数相同时，前视，后视须同时选中
    ids_applyToSmart = m_LanMap.value("ids_applyToSmart");//要将本次学习结果应用到当前设置目标吗？
    ids_noSample = m_LanMap.value("ids_noSample");//没有选择样本
    ids_NoCfgFile = m_LanMap.value("ids_NoCfgFile");//缺少配置文件
    ids_color = m_LanMap.value("ids_color");//颜色
    ids_erode2 = m_LanMap.value("ids_erode2");//腐蚀
    ids_oversize = m_LanMap.value("ids_oversize");//智能尺寸越界
    ids_sampleColor = m_LanMap.value("ids_sampleColor");//样本色
    ids_purple = m_LanMap.value("ids_purple");//紫色
    ids_cyan = m_LanMap.value("ids_cyan");//青色

    //	[feed]
    ids_byChannelSet = m_LanMap.value("ids_byChannelSet");//通道设置
    ids_autoFeed = m_LanMap.value("ids_autoFeed");//自动产量
    ids_CalibrateMass = m_LanMap.value("ids_CalibrateMass");//标定产量
    ids_CalibrateInKG = m_LanMap.value("ids_CalibrateInKG");//质量(kg)
    ids_ToCalibrateMass = m_LanMap.value("ids_ToCalibrateMass");//确定标定产量吗？
    ids_InputNonzeroValue = m_LanMap.value("ids_InputNonzeroValue");//请输入非零的标定产量值
    ids_ClearTodaysData = m_LanMap.value("ids_ClearTodaysData");//这将会清零今日统计值
    ids_CalibrateFinished = m_LanMap.value("ids_CalibrateFinished");//产量标定完成，标定系数为
    ids_Conveyor = m_LanMap.value("ids_Conveyor");//输送带
    ids_lowerFeed = m_LanMap.value("ids_lowerFeed");//下层产量

    ids_moreFeed = m_LanMap.value("ids_moreFeed");//产量-更多
    ids_stopFeedWhenNoMat = m_LanMap.value("ids_stopFeedWhenNoMat");//单通道无料停运
    ids_QueryInterval = m_LanMap.value("ids_QueryInterval");//间隔(s)
    ids_StopCycles = m_LanMap.value("ids_StopCycles");//停止(T)
    ids_MatFew = m_LanMap.value("ids_MatFew");//料少
    ids_MatMuch = m_LanMap.value("ids_MatMuch");//料多
    ids_AdjustFeedSen = m_LanMap.value("ids_AdjustFeedSen");//差异性(%)
    ids_AdjustStep = m_LanMap.value("ids_AdjustStep");//步长

    ids_SetYeild  = m_LanMap.value("ids_SetYeild");// 期望产量
    ids_StopFeedThre  = m_LanMap.value("ids_StopFeedThre");// 停运门限
    ids_EvenFeedRateThre  = m_LanMap.value("ids_EvenFeedRateThre");// 匀料门限
    ids_FeedSetRange  = m_LanMap.value("ids_FeedSetRange");// 调节范围
    ids_FeedSetStep  = m_LanMap.value("ids_FeedSetStep");// 调节步长
    // feeder
    feeder = m_LanMap.value("feeder");                 //流量
    stop_duration = m_LanMap.value("stop_duration");          //停运时长
    adjust_interval = m_LanMap.value("adjust_interval");        //调节间隔
    search_interval = m_LanMap.value("search_interval");        //查询间隔
    searching = m_LanMap.value("searching");              //正在获取信息

    // wipe
    ids_cleaningTime = m_LanMap.value("ids_cleaningTime");//时间（秒）
    ids_cleaningInterval = m_LanMap.value("ids_cleaningInterval");//间隔（分）
    ids_stop_channel_num_while_smart_clean  = m_LanMap.value("ids_stop_channel_num_while_smart_clean");// 停止通道
    ids_cleaningMode = m_LanMap.value("ids_cleaningMode");//清灰模式
    ids_timingMode = m_LanMap.value("ids_timingMode");//定时清灰
    ids_teamMode = m_LanMap.value("ids_teamMode");//联合清灰
    ids_timingMode_smart  = m_LanMap.value("ids_timingMode_smart");//智能清灰
    ids_cleaningEnable = m_LanMap.value("ids_cleaningEnable");//清灰使能
    ids_isCleaning = m_LanMap.value("ids_isCleaning");//正在清灰

    //	[valve]
    ids_blowTime = m_LanMap.value("ids_blowTime");//吹气时间(ms)
    ids_valveTest = m_LanMap.value("ids_valveTest");//气阀测试
    ids_index = m_LanMap.value("ids_index");//编号
    ids_singleTest = m_LanMap.value("ids_singleTest");//气阀单检
    ids_cycleTest = m_LanMap.value("ids_cycleTest");//气阀循检
    ids_stopTest = m_LanMap.value("ids_stopTest");//停止检测
    ids_towhichboard  = m_LanMap.value("ids_towhichboard");// 板级
    ids_valveMore = m_LanMap.value("ids_valveMore");//气阀-更多
    ids_expand = m_LanMap.value("ids_expand");//色选拓展
    ids_shapeExpand = m_LanMap.value("ids_shapeExpand");//形选拓展
    ids_erode = m_LanMap.value("ids_erode");//腐蚀尺寸
    ids_reverseSelect = m_LanMap.value("ids_reverseSelect");//反选
    ids_lightSelect = m_LanMap.value("ids_lightSelect");//轻选
    ids_heavySelect = m_LanMap.value("ids_heavySelect");//重选
    ids_valveEnable = m_LanMap.value("ids_valveEnable");//气阀使能
    ids_agingTest = m_LanMap.value("ids_agingTest");//老化测试
    ids_dilateSens = m_LanMap.value("ids_dilateSens");//膨胀阈值
    ids_dilateSize = m_LanMap.value("ids_dilateSize");//膨胀尺寸
    ids_centerSize = m_LanMap.value("ids_centerSize");//中心
    ids_fvAnd = m_LanMap.value("ids_fvAnd");//前后与
    ids_waittime = m_LanMap.value("ids_waittime");              //等待时间
    ids_tailtime = m_LanMap.value("ids_tailtime");              //尾气时间
    ids_valveTest = m_LanMap.value("ids_valveTest");             //喷阀自检
    ids_index = m_LanMap.value("ids_index");        //当前喷嘴

    ids_sameForCameras = m_LanMap.value("ids_sameForCameras");//以下设置不区分相机
    ids_invalidTailTime = m_LanMap.value("ids_invalidTailTime");//吹气时间与等待时间之和不能大于5.6ms

    ids_valveAging = m_LanMap.value("ids_valveAging");//气阀老化
    ids_agingFreq = m_LanMap.value("ids_agingFreq");//老化频率
    ids_checkChannel = m_LanMap.value("ids_checkChannel");//勾选老化通道
    ids_aging = m_LanMap.value("ids_aging");//老化
    ids_oneChannelAtLeast = m_LanMap.value("ids_oneChannelAtLeast");//至少要选择一个通道

    ids_delayTime = m_LanMap.value("ids_delayTime");//延时：
    ids_delayTime2 = m_LanMap.value("ids_delayTime2"); //延时(ms)

    //	[模式]
    scheme_parameter = m_LanMap.value("scheme_parameter");       //方案参数

    scheme_backup = m_LanMap.value("scheme_backup");          //备份方案
    scheme_cur = m_LanMap.value("scheme_cur");
    cfm_scheme_create = m_LanMap.value("cfm_scheme_create");      //确认创建方案
    cfm_scheme_copy = m_LanMap.value("cfm_scheme_copy");        //确认拷贝方案
    cfm_scheme_rename = m_LanMap.value("cfm_scheme_rename");      //确认重命名方案
    cfm_restore_from_local = m_LanMap.value("cfm_restore_from_local"); //确认从本地恢复方案
    cfm_backup_to_local = m_LanMap.value("cfm_backup_to_local");    //确认备份方案到本地
    msg_backup_to_local_success = m_LanMap.value("msg_backup_to_local_success"); //方案本地备份成功
    msg_backup_to_local_error = m_LanMap.value("msg_backup_to_local_error");   //方案本地备份失败
    msg_restore_to_local_error = m_LanMap.value("msg_restore_to_local_error");   //方案本地恢复失败
    cfm_restore_from_usb = m_LanMap.value("cfm_restore_from_usb");       //确认从U盘导入方案
    msg_restore_from_usb_error = m_LanMap.value("msg_restore_from_usb_error"); //U盘恢复方案错误
    cfm_backup_to_usb = m_LanMap.value("cfm_backup_to_usb");          //确认导出方案到U盘
    cfm_select_current_scheme = m_LanMap.value("cfm_select_current_scheme");  //确认选择当前方案
    cfm_save_current_scheme = m_LanMap.value("cfm_save_current_scheme"); //当前方案已更改，确认保存
    mode_discolor = m_LanMap.value("mode_discolor");              //异色粒正选
    mode_discolor_reverse = m_LanMap.value("mode_discolor_reverse");      //异色粒反选
    mode_milky = m_LanMap.value("mode_milky");                 //乳白粒正选
    mode_milky_reverse = m_LanMap.value("mode_milky_reverse");         //乳白粒反选
    mode_yellow_milky = m_LanMap.value("mode_yellow_milky");          //黄白同选
    mode_yellow_trans = m_LanMap.value("mode_yellow_trans");          //黄透同选
    mode_yellow_yellow_reverse = m_LanMap.value("mode_yellow_yellow_reverse"); //黄+黄反选
    mode_yellow_white_reverse = m_LanMap.value("mode_yellow_white_reverse");  //黄+白反选
    cfm_reset_current_scheme_materialname = m_LanMap.value("cfm_reset_current_scheme_materialname"); //确认还原当前方案算法名称

    ids_time = m_LanMap.value("ids_time");//时间
    ids_new = m_LanMap.value("ids_new");//新建
    ids_copy = m_LanMap.value("ids_copy");//复制
    ids_delete = m_LanMap.value("ids_delete");//删除
    ids_rename = m_LanMap.value("ids_rename");//重命名
    ids_restore = m_LanMap.value("ids_restore");//恢复
    ids_apply = m_LanMap.value("ids_apply");//应用
    ids_selectOneModel = m_LanMap.value("ids_selectOneModel");//请选择一个模式
    ids_maxModelNumber = m_LanMap.value("ids_maxModelNumber");//模式库已满！建议先删除不需要的模式
    ids_inputModelName = m_LanMap.value("ids_inputModelName");//请输入模式名
    ids_copyModel = m_LanMap.value("ids_copyModel");//复制模式
    ids_modelNameNull = m_LanMap.value("ids_modelNameNull");//模式名称不能为空
    ids_sameName = m_LanMap.value("ids_sameName");//新模式与已有模式重名
    ids_newModelError = m_LanMap.value("ids_newModelError");//新建模式时发生错误
    ids_selectOneTemplate = m_LanMap.value("ids_selectOneTemplate");//请选择一个模式作为模板
    ids_keepOneModelAtLeast = m_LanMap.value("ids_keepOneModelAtLeast");//至少需要保留一个模式
    ids_cannotDeleteModelInUse = m_LanMap.value("ids_cannotDeleteModelInUse");//不允许删除正在使用的模式
    ids_cannotDeleteModelInLocked = m_LanMap.value("ids_cannotDeleteModelInLocked");//该模式处于锁定状态，无法删除。解锁请点击“更多”。
    ids_toDeleteModel = m_LanMap.value("ids_toDeleteModel");//模式删除后无法恢复，确定要删除选中的模式吗？
    ids_cannotRenameModelInLocked = m_LanMap.value("ids_cannotRenameModelInLocked");//该模式处于锁定状态，无法重命名。解锁请点击“更多”。
    ids_inputNewName = m_LanMap.value("ids_inputNewName");//请输入新的模式名
    ids_renameFailed = m_LanMap.value("ids_renameFailed");//重命名失败
    ids_renameModelError = m_LanMap.value("ids_renameModelError");//重命名模式时发生错误
    ids_cannotSwitchModel = m_LanMap.value("ids_cannotSwitchModel");//设备运行过程中，禁止切换模式
    ids_paramDisplay = m_LanMap.value("ids_paramDisplay");//参数显示
    ids_paramName = m_LanMap.value("ids_paramName");//参数

    ids_displayEnable = m_LanMap.value("ids_displayEnable");//显示使能

    ids_locked = m_LanMap.value("ids_locked");//锁定
    ids_lock = m_LanMap.value("ids_lock");//锁定/解锁
    ids_backup = m_LanMap.value("ids_backup");//备份
    ids_export = m_LanMap.value("ids_export");//导出库
    ids_import = m_LanMap.value("ids_import");//导入库
    ids_noMobileDevices = m_LanMap.value("ids_noMobileDevices");//没有寻找到移动存储设备！
    ids_exportFinished = m_LanMap.value("ids_exportFinished");//用户模式库与厂家模式库导出完成
    ids_yes = m_LanMap.value("ids_yes");//是
    ids_no = m_LanMap.value("ids_no");//否
    ids_noLibrary = m_LanMap.value("ids_noLibrary");//移动设备里没有用户模式库或厂家模式库
    ids_toImportLibrary = m_LanMap.value("ids_toImportLibrary");//确定导入模式库？
    ids_libraryWillBeOverwritten = m_LanMap.value("ids_libraryWillBeOverwritten");//本地的用户模式库与厂家模式库将会被覆盖，无法恢复！
    ids_importFinished = m_LanMap.value("ids_importFinished");//导入完成，重启系统后生效
    ids_modelManagement = m_LanMap.value("ids_modelManagement");//模式管理
    ids_enterPassword = m_LanMap.value("ids_enterPassword");//输入密码
    ids_wrongPassword = m_LanMap.value("ids_wrongPassword");//密码错误
    ids_sameNameWhenBackup = m_LanMap.value("ids_sameNameWhenBackup");//备份模式名称与厂家模式库里已有模式重名
    ids_backupModelError = m_LanMap.value("ids_backupModelError");//备份模式时发生错误
    ids_backupFinished = m_LanMap.value("ids_backupFinished");//完成模式备份

    ids_selectModelToRestore = m_LanMap.value("ids_selectModelToRestore");//选择要恢复的模式
    ids_factoryModel = m_LanMap.value("ids_factoryModel");//厂家模式
    ids_toRestoreModel = m_LanMap.value("ids_toRestoreModel");//确定恢复到用户模式库？
    ids_sameNameWhenRestore = m_LanMap.value("ids_sameNameWhenRestore");//恢复的模式名称与用户模式库里已有模式重名！
    ids_restoreModelError = m_LanMap.value("ids_restoreModelError");//恢复模式时发生错误！
    ids_restoreFinished = m_LanMap.value("ids_restoreFinished");//完成模式恢复
    ids_default_library = m_LanMap.value("ids_default_library");//默认库
    ids_select_all = m_LanMap.value("ids_select_all");//全选
    ids_select_none = m_LanMap.value("ids_select_none");//全部不选
    ids_selected = m_LanMap.value("ids_selected");//选中
    ids_toOverrideModel = m_LanMap.value("ids_toOverrideModel");//你要覆盖同名模式[%s]吗？
    ids_readInfo = m_LanMap.value("ids_readInfo");//正在读取模式信息

    //	[background]
    ids_background = m_LanMap.value("ids_background");//背景
    ids_image = m_LanMap.value("ids_image");//图像
    ids_system = m_LanMap.value("ids_system");//系统

    ids_redBkg = m_LanMap.value("ids_redBkg");//红背景
    ids_greenBkg = m_LanMap.value("ids_greenBkg");//绿背景
    ids_blueBkg = m_LanMap.value("ids_blueBkg");//蓝背景
    ids_NBkg  = m_LanMap.value("ids_NBkg");// N背景
    ids_blackBkg = m_LanMap.value("ids_blackBkg");//黑背景
    ids_whiteBkg = m_LanMap.value("ids_whiteBkg");//白背景
    ids_black_whiteBkg  = m_LanMap.value("ids_black_whiteBkg");// 黑白背景
    ids_grayBkg  = m_LanMap.value("ids_grayBkg");// 灰背景
    ids_yellowBkg  = m_LanMap.value("ids_yellowBkg");// 黄背景
    ids_irBkg = m_LanMap.value("ids_irBkg");//I背景
    ids_threshold = m_LanMap.value("ids_threshold");//阈值
    ids_ratio = m_LanMap.value("ids_ratio");//比值
    ids_originalSignal = m_LanMap.value("ids_originalSignal");//原始信号
    ids_correctedSignal = m_LanMap.value("ids_correctedSignal");//校正信号
    ids_pixel = m_LanMap.value("ids_pixel");//像素
    ids_gray = m_LanMap.value("ids_gray");//灰度
    ids_frontBkgBoard = m_LanMap.value("ids_frontBkgBoard");//前背景板
    ids_backBkgBoard = m_LanMap.value("ids_backBkgBoard");//后背景板
    ids_uplimit = m_LanMap.value("ids_uplimit");//上限
    ids_lowlimit  = m_LanMap.value("ids_lowlimit");// 下限

    //	[image]
    ids_getImage = m_LanMap.value("ids_getImage");//获取\n图像
    ids_openImage = m_LanMap.value("ids_openImage");//打开\n图像
    ids_deleteImage = m_LanMap.value("ids_deleteImage");//删除\n图像
    ids_importImage = m_LanMap.value("ids_importImage"); //图片导入
    ids_exportImage = m_LanMap.value("ids_exportImage");//导出\n图像
    ids_videoMode = m_LanMap.value("ids_videoMode");//视频模式
    ids_saveSet = m_LanMap.value("ids_saveSet");//保存设置
    ids_notSave = m_LanMap.value("ids_notSave");//不保存
    ids_savePath1 = m_LanMap.value("ids_savePath1");//路径1
    ids_savePath2 = m_LanMap.value("ids_savePath2");//路径2
    ids_toDeleteImage = m_LanMap.value("ids_toDeleteImage");//确定要删除所有图像？删除后无法恢复！
    ids_deleteImageFinished = m_LanMap.value("ids_deleteImageFinished");//图像删除完成
    ids_exportImageFinished = m_LanMap.value("ids_exportImageFinished");//图像导出完成
    ids_getImage2 = m_LanMap.value("ids_getImage2");//同步\n拍照

    ai_trigger_threshold = m_LanMap.value("ai_trigger_threshold");   //触发阈值
    threshold = m_LanMap.value("threshold");              //阈值
    sample_set = m_LanMap.value("sample_set");             //样本设置
    bad_number = m_LanMap.value("bad_number");             //坏点数
    good_number = m_LanMap.value("good_number");            //好点数
    clean_sample = m_LanMap.value("clean_sample");           //清空样本
    cfm_clean_sample = m_LanMap.value("cfm_clean_sample");       //确认清楚
    ai_image_save_error = m_LanMap.value("ai_image_save_error");    //保存错误
    ai_image_capturing = m_LanMap.value("ai_image_capturing");     //图像采集中...
    ai_image_capture_error = m_LanMap.value("ai_image_capture_error"); //图像采集错误
    ai_analysis = m_LanMap.value("ai_analysis");            //图像分析
    ai_analysis_set = m_LanMap.value("ai_analysis_set");        //图像分析设置
    msg_capture_image_first = m_LanMap.value("msg_capture_image_first");//请先采集图像

    //	[system]
    ids_remote = m_LanMap.value("ids_remote");// 远程\n连接
    ids_calibration = m_LanMap.value("ids_calibration");//屏幕\n校准
    ids_remoteFailed = m_LanMap.value("ids_remoteFailed");//远程助手启动失败!
    ids_user = m_LanMap.value("ids_user");//账户设置
    ids_operator = m_LanMap.value("ids_operator");//普通用户
    ids_engineer = m_LanMap.value("ids_engineer");//工程师
    ids_skinStyle = m_LanMap.value("ids_skinStyle");//皮肤风格
    ids_blueSkin = m_LanMap.value("ids_blueSkin");//活力蓝
    ids_blackSkin = m_LanMap.value("ids_blackSkin");//魅力黑

    ids_machineModel = m_LanMap.value("ids_machineModel");// 机型
    ids_mirror = m_LanMap.value("ids_mirror");//镜像
    ids_lightSource = m_LanMap.value("ids_lightSource");//光源
    ids_correct = m_LanMap.value("ids_correct");//校正
    ids_devidePixel = m_LanMap.value("ids_devidePixel");//像元划分
    ids_language = m_LanMap.value("ids_language");//语言
    ids_upgrade = m_LanMap.value("ids_upgrade");//升级
    ids_encrypt = m_LanMap.value("ids_encrypt");//编码

    //	[model]
    ids_channelNumber = m_LanMap.value("ids_channelNumber");//通道数量
    ids_valveModule = m_LanMap.value("ids_valveModule");//喷阀模组
    ids_resolution = m_LanMap.value("ids_resolution");//分辨率
    ids_batchChannel = m_LanMap.value("ids_batchChannel");//通道分次
    ids_machineModel2 = m_LanMap.value("ids_machineModel2");//机型
    ids_rice = m_LanMap.value("ids_rice");//大米
    ids_grains = m_LanMap.value("ids_grains");//杂粮
    ids_configFIR = m_LanMap.value("ids_configFIR");//配前辅
    ids_configBIR = m_LanMap.value("ids_configBIR");//配后辅
    ids_algorithm = m_LanMap.value("ids_algorithm");//算法
    ids_named = m_LanMap.value("ids_named");//命名
    ids_switchAlg = m_LanMap.value("ids_switchAlg");//算法\n开关
    ids_difference = m_LanMap.value("ids_difference");//色差
    ids_smart = m_LanMap.value("ids_smart");//智能
    ids_whiteBelly = m_LanMap.value("ids_whiteBelly");//透白
    ids_impurity = m_LanMap.value("ids_impurity");//恶杂
    ids_masterGlass = m_LanMap.value("ids_masterGlass");//玻璃
    ids_fsGlass = m_LanMap.value("ids_fsGlass");//近红玻璃
    ids_bsGlass = m_LanMap.value("ids_bsGlass");//红外玻璃
    ids_area = m_LanMap.value("ids_area");//面积
    ids_length = m_LanMap.value("ids_length");//长度
    ids_circular = m_LanMap.value("ids_circular");//圆度
    ids_selectOneAlg = m_LanMap.value("ids_selectOneAlg");//请选择一个算法
    ids_selectOneGlass = m_LanMap.value("ids_selectOneGlass");//只能选择一种玻璃算法
    ids_inputNewAlgName = m_LanMap.value("ids_inputNewAlgName");//输入新的算法名称
    ids_cannotEnableForNoFIR = m_LanMap.value("ids_cannotEnableForNoFIR");//未配置前辅相机，该算法使能无法开启
    ids_cannotEnableForNoBIR = m_LanMap.value("ids_cannotEnableForNoBIR");//未配置后辅相机，该算法使能无法开启
    ids_wrongResolution = m_LanMap.value("ids_wrongResolution");//分辨率设置错误！只能设置为1024或者2048
    ids_enable = m_LanMap.value("ids_enable");//使能
    ids_disable = m_LanMap.value("ids_disable");//禁用
    ids_algCfg = m_LanMap.value("ids_algCfg");//算法配置
    ids_init = m_LanMap.value("ids_init");//初始化
    ids_toInit = m_LanMap.value("ids_toInit");//要初始化配置信息吗？
    ids_resetName = m_LanMap.value("ids_resetName");//重置\n命名
    ids_toResetName = m_LanMap.value("ids_toResetName");//要重置所有模式的算法名称吗？

    ids_saveAfterSet = m_LanMap.value("ids_saveAfterSet");//设定后需要保存，重启软件才能生效

    //	[light]
    ids_frontLight = m_LanMap.value("ids_frontLight");//前灯
    ids_backLight = m_LanMap.value("ids_backLight");//后灯
    ids_materialLight1 = m_LanMap.value("ids_materialLight1");//物料灯上
    ids_materialLight2 = m_LanMap.value("ids_materialLight2");//物料灯下
    ids_infraredLight1 = m_LanMap.value("ids_infraredLight1");//红外灯上
    ids_infraredLight2 = m_LanMap.value("ids_infraredLight2");//红外灯下
    ids_bkgLight = m_LanMap.value("ids_bkgLight");//背景灯
    ids_infraredBkgLight = m_LanMap.value("ids_infraredBkgLight");//红外背景
    ids_turnOn = m_LanMap.value("ids_turnOn");//打开
    ids_light = m_LanMap.value("ids_light");//灯
    ids_materialLight = m_LanMap.value("ids_materialLight");//物料灯

    //	[correct]
    ids_target = m_LanMap.value("ids_target");//目标
    ids_gain = m_LanMap.value("ids_gain");//增益
    ids_offset = m_LanMap.value("ids_offset");//偏置
    ids_getCorrection = m_LanMap.value("ids_getCorrection");//明场\n校正
    ids_saveCorrection = m_LanMap.value("ids_saveCorrection");//保存\n明场
    ids_toGetCorrection = m_LanMap.value("ids_toGetCorrection");//确定要明场校正？
    ids_toSaveCorrection = m_LanMap.value("ids_toSaveCorrection");//确定要保存明场校正？
    ids_onekeyBright = m_LanMap.value("ids_onekeyBright");//一键\n明场
    ids_toOnekeyBright = m_LanMap.value("ids_toOnekeyBright");//确定要一键获取并保存所有明场校正？
    ids_getDark = m_LanMap.value("ids_getDark");//暗场\n校正
    ids_saveDark = m_LanMap.value("ids_saveDark");//保存\n暗场
    ids_toGetDark = m_LanMap.value("ids_toGetDark");//确定要暗场校正？
    ids_toSaveDark = m_LanMap.value("ids_toSaveDark");//确定要保存暗场校正？
    ids_onekeyDark = m_LanMap.value("ids_onekeyDark");//一键\n暗场
    ids_toOnekeyDark = m_LanMap.value("ids_toOnekeyDark");//确定要一键获取并保存所有暗场校正？
    ids_correctNG = m_LanMap.value("ids_correctNG");//纠错
    ids_toCorrectNG = m_LanMap.value("ids_toCorrectNG");//确定要纠错当前相机的坏点吗?
    ids_position = m_LanMap.value("ids_position");//位置
    ids_up = m_LanMap.value("ids_up");//上
    ids_down = m_LanMap.value("ids_down");//下
    ids_enableTDI = m_LanMap.value("ids_enableTDI");//TDI模式
    ids_clearR = m_LanMap.value("ids_clearR");//清零R
    ids_clearG = m_LanMap.value("ids_clearG");//清零G
    ids_clearB = m_LanMap.value("ids_clearB");//清零B
    ids_closeTdi = m_LanMap.value("ids_closeTdi");//需要关闭tdi模式，并切换到原始信号
    ids_sureToClearNG = m_LanMap.value("ids_sureToClearNG");//确定要清零R坏点吗？
    ids_sureToClearNG2 = m_LanMap.value("ids_sureToClearNG2");//确定要清零G坏点吗？
    ids_sureToClearNG3 = m_LanMap.value("ids_sureToClearNG3");//确定要清零B坏点吗？
    ids_sensor = m_LanMap.value("ids_sensor");//传感器
    ids_sensorError = m_LanMap.value("ids_sensorError");//传感器异常
    ids_tooManyNG = m_LanMap.value("ids_tooManyNG");//坏点数超过4个


    //	[dividePixel]
    ids_startPixel = m_LanMap.value("ids_startPixel");//起始像元
    ids_endPixel = m_LanMap.value("ids_endPixel");//结束像元
    ids_pixelList = m_LanMap.value("ids_pixelList");//像元\n列表
    ids_pixelList2 = m_LanMap.value("ids_pixelList2");//像元列表
    ids_camera = m_LanMap.value("ids_camera");//相机
    ids_offset_x = m_LanMap.value("ids_offset_x");//G偏移X
    ids_offset_y = m_LanMap.value("ids_offset_y");//G偏移Y
    ids_offset_x_B = m_LanMap.value("ids_offset_x_B");//B偏移X
    ids_offset_y_B = m_LanMap.value("ids_offset_y_B");//B偏移Y
    ids_enableOffset = m_LanMap.value("ids_enableOffset");//偏置使能
    ids_enableGainSetD2  = m_LanMap.value("ids_enableGainSetD2");// 增益锁定

    //	[language]
    ids_chinese = m_LanMap.value("ids_chinese");//中文
    ids_english = m_LanMap.value("ids_english");//English
    ids_turkish = m_LanMap.value("ids_turkish");//Türkçe
    ids_spanish = m_LanMap.value("ids_spanish");//español
    ids_russian = m_LanMap.value("ids_russian");//русск
    ids_thai = m_LanMap.value("ids_thai");//ภาษาไทย
    ids_vietnamese = m_LanMap.value("ids_vietnamese");//Việt nam
    ids_iranian = m_LanMap.value("ids_iranian");//فارسی
    ids_portuguese = m_LanMap.value("ids_portuguese");//Português - Brasil
    ids_arabic = m_LanMap.value("ids_arabic");//العربية
    ids_korean = m_LanMap.value("ids_korean");//한국어
    ids_indonesian  = m_LanMap.value("ids_indonesian");// Indonesia

    ids_selectOneLanguage = m_LanMap.value("ids_selectOneLanguage");//选择一种语言
    ids_noLanguageLib = m_LanMap.value("ids_noLanguageLib");//没有对应的语言库

    //	[upgrade]
    ids_openUpgradeFile = m_LanMap.value("ids_openUpgradeFile");//请打开升级文件
    ids_openFile = m_LanMap.value("ids_openFile");//打开
    ids_startUpgrade = m_LanMap.value("ids_startUpgrade");//开始\n升级
    ids_yourFileIs = m_LanMap.value("ids_yourFileIs");//你的升级文件是：
    ids_wrongFileName = m_LanMap.value("ids_wrongFileName");//升级文件命名不符合规范！
    ids_notSelectFile = m_LanMap.value("ids_notSelectFile");//未选择升级文件！
    ids_startUpgrading = m_LanMap.value("ids_startUpgrading");//开始升级……
    ids_openFileFailed = m_LanMap.value("ids_openFileFailed");//打开文件失败
    ids_sendFileFailed = m_LanMap.value("ids_sendFileFailed");//发送文件失败
    ids_restoreInterfaceFailed = m_LanMap.value("ids_restoreInterfaceFailed");//恢复接口板用户模式失败
    ids_restoreInterfaceSucceeded = m_LanMap.value("ids_restoreInterfaceSucceeded");//恢复接口板用户模式成功
    ids_allChannel = m_LanMap.value("ids_allChannel");//所有通道
    ids_allChain = m_LanMap.value("ids_allChain");//所有层
    ids_upgradeSucceeded = m_LanMap.value("ids_upgradeSucceeded");//升级成功
    ids_upgradeFailed = m_LanMap.value("ids_upgradeFailed");//升级失败
    ids_upgradeResult = m_LanMap.value("ids_upgradeResult");//升级结果
    ids_ipc = m_LanMap.value("ids_ipc");//上位机
    ids_filesList = m_LanMap.value("ids_filesList");//文件名
    ids_selectOtherFiles = m_LanMap.value("ids_selectOtherFiles");//选择你需要的其它配置
    ids_upgradeLog = m_LanMap.value("ids_upgradeLog");//日志描述
    ids_tellToUpgrade = m_LanMap.value("ids_tellToUpgrade");//通知下位机准备升级
    ids_sendData = m_LanMap.value("ids_sendData");//发送数据
    ids_setFactoryFailed = m_LanMap.value("ids_setFactoryFailed");//工厂模式设置失败
    ids_setFactorySucceeded = m_LanMap.value("ids_setFactorySucceeded");//工厂模式设置成功
    ids_upgrading = m_LanMap.value("ids_upgrading");//正在升级……
    ids_chain = m_LanMap.value("ids_chain");//链路
    ids_checkUpdateFile = m_LanMap.value("ids_checkUpdateFile");//检测升级文件
    ids_startUpdate = m_LanMap.value("ids_startUpdate");//开始升级

    //	[encrypt]
    ids_runTime = m_LanMap.value("ids_runTime");//运行期限
    ids_invalidCode = m_LanMap.value("ids_invalidCode");//无效密码
    ids_tooLongCode = m_LanMap.value("ids_tooLongCode");//输入的密码过长！
    ids_tooShortCode = m_LanMap.value("ids_tooShortCode");//输入的密码过短！
    ids_wrongCodeFormat = m_LanMap.value("ids_wrongCodeFormat");//密码格式错误！
    ids_unmatchedCode = m_LanMap.value("ids_unmatchedCode");//该密码与机器编号不匹配！
    ids_overdueCode = m_LanMap.value("ids_overdueCode");//这是一个时间过期密码！
    ids_decryptMachine = m_LanMap.value("ids_decryptMachine");//你要完全解密本设备？
    ids_machineNumber = m_LanMap.value("ids_machineNumber");//你要加（解）密的设备编号是
    ids_dueTime = m_LanMap.value("ids_dueTime");//到期时间是
    ids_dueProduction = m_LanMap.value("ids_dueProduction");//到期产量是
    ids_restartMachine = m_LanMap.value("ids_restartMachine");//操作成功，请重启设备
    ids_setCode = m_LanMap.value("ids_setCode");//设置加（解）密码
    ids_productID = m_LanMap.value("ids_productID");//机器编码：
    ids_setID = m_LanMap.value("ids_setID");//设置机器编码
    ids_errorCodeRules = m_LanMap.value("ids_errorCodeRules");//错误的加密规则
    ids_confirmTime = m_LanMap.value("ids_confirmTime");//你需要确认系统时间
    ids_nowTime = m_LanMap.value("ids_nowTime");//当前时间是：

    //    [info]
    ids_status = m_LanMap.value("ids_status");//系统状态
    ids_version = m_LanMap.value("ids_version");//版本查询
    ids_report = m_LanMap.value("ids_report");//生产报表
    ids_alarm = m_LanMap.value("ids_alarm");//报警设置
    ids_maintenance = m_LanMap.value("ids_maintenance");//维护保养
    ids_remote_assist  = m_LanMap.value("ids_remote_assist");// 远程协助
    ids_get_ovpn  = m_LanMap.value("ids_get_ovpn");// 获取证书
    ids_about = m_LanMap.value("ids_about");//关于本机

    //   [status]
    ids_thisRunTime = m_LanMap.value("ids_thisRunTime");//本次运行
    ids_totalRunTime = m_LanMap.value("ids_totalRunTime");//累计运行
    ids_hours = m_LanMap.value("ids_hours");//小时
    ids_cloud = m_LanMap.value("ids_cloud");//云平台
    ids_feedState = m_LanMap.value("ids_feedState");//下料状态
    ids_feedAlarm = m_LanMap.value("ids_feedAlarm");//供料报警
    ids_cleaningAlarm = m_LanMap.value("ids_cleaningAlarm");//清灰报警
    ids_airpressureAlarm = m_LanMap.value("ids_airpressureAlarm");//气压报警
    ids_materialLevelAlarm = m_LanMap.value("ids_materialLevelAlarm");//料位报警
    ids_statisticsInfo = m_LanMap.value("ids_statisticsInfo");//统计信息
    ids_off = m_LanMap.value("ids_off");//关闭
    ids_normal = m_LanMap.value("ids_normal");//正常
    ids_abnormal = m_LanMap.value("ids_abnormal");//异常
    ids_rejected = m_LanMap.value("ids_rejected");//含杂
    ids_rejectedRatio = m_LanMap.value("ids_rejectedRatio");//含杂率
    ids_cleaningFailed = m_LanMap.value("ids_cleaningFailed");//清灰失败
    ids_glass = m_LanMap.value("ids_glass");//玻璃
    ids_powerDownProtection = m_LanMap.value("ids_powerDownProtection");//掉电保护
    ids_TemperatureAlarm  = m_LanMap.value("ids_TemperatureAlarm");// 温度报警
    ids_sensorTemperature = m_LanMap.value("ids_sensorTemperature");//传感器温度
    ids_askTemperature = m_LanMap.value("ids_askTemperature");//查询\n温度

    //    [report]
    ids_queryCycle = m_LanMap.value("ids_queryCycle");//查询周期
    ids_startDate = m_LanMap.value("ids_startDate");//开始时间
    ids_endDate = m_LanMap.value("ids_endDate");//结束时间
    ids_queryMode = m_LanMap.value("ids_queryMode");//查询方式
    ids_byMachine = m_LanMap.value("ids_byMachine");//按整机
    ids_byChannel = m_LanMap.value("ids_byChannel");//按通道
    ids_date = m_LanMap.value("ids_date");//日期
    ids_exportReport = m_LanMap.value("ids_exportReport");//导出
    ids_today = m_LanMap.value("ids_today");//今天
    ids_oneWeek = m_LanMap.value("ids_oneWeek");//一周内
    ids_oneMonth = m_LanMap.value("ids_oneMonth");//一月内
    ids_threeMonth  = m_LanMap.value("ids_threeMonth");// 三月内
    ids_custom = m_LanMap.value("ids_custom");//自定义
    ids_reportExportFailed = m_LanMap.value("ids_reportExportFailed");//报表导出失败
    ids_reportExportDone = m_LanMap.value("ids_reportExportDone");//报表导出完成
    ids_displayOnHomepage = m_LanMap.value("ids_displayOnHomepage");//主页显示统计信息

    //   [alarm]
    ids_autoCleaning = m_LanMap.value("ids_autoCleaning");//自动清灰
    ids_stopFeed = m_LanMap.value("ids_stopFeed");//停振动器

    ids_parts = m_LanMap.value("ids_parts");//部件名称
    ids_indicators = m_LanMap.value("ids_indicators");//性能指标
    ids_runtimeValue = m_LanMap.value("ids_runtimeValue");//运行值
    ids_criticalValue = m_LanMap.value("ids_criticalValue");//临界值
    ids_assessment = m_LanMap.value("ids_assessment");//评估警示
    ids_sootCylinder = m_LanMap.value("ids_sootCylinder");//清灰气缸
    ids_runningTimes = m_LanMap.value("ids_runningTimes");//运行次数
    ids_filtration = m_LanMap.value("ids_filtration");//过滤系统
    ids_gasConsumption = m_LanMap.value("ids_gasConsumption");//耗气总量
    ids_switchTimes = m_LanMap.value("ids_switchTimes");//开关次数
    ids_healthy = m_LanMap.value("ids_healthy");//健康
    ids_subHealthy = m_LanMap.value("ids_subHealthy");//亚健康
    ids_needChecked = m_LanMap.value("ids_needChecked");//需体检

    //    [about]
    ids_versionValue = m_LanMap.value("ids_versionValue");//版本
    ids_compiled = m_LanMap.value("ids_compiled");//编译
    ids_allRightsReserved = m_LanMap.value("ids_allRightsReserved");//保留所有权利

    ids_inputNumber = m_LanMap.value("ids_inputNumber");//当前数值：
    ids_channelError = m_LanMap.value("ids_channelError");//各次通道数量之和与机型不匹配！
    ids_alarmInfo = m_LanMap.value("ids_alarmInfo");//报警信息
    ids_lowAirpressure = m_LanMap.value("ids_lowAirpressure");//气压过低，请立即处理！
    ids_highAirpressure = m_LanMap.value("ids_highAirpressure");//气压过高，请立即处理！
    ids_illegalDelay = m_LanMap.value("ids_illegalDelay");//延时参数非法
    ids_illegalDelayDetails = m_LanMap.value("ids_illegalDelayDetails");//延时过小，或尺寸或吹气参数过大
    ids_material_empty  = m_LanMap.value("ids_material_empty");// 物料空
    ids_material_full  = m_LanMap.value("ids_material_full");// 物料满
    ids_temperature_alarm  = m_LanMap.value("ids_temperature_alarm");// 温度异常

    //    [sort]
    ids_Sort = m_LanMap.value("ids_Sort");//排序
    ids_AddSortSample = m_LanMap.value("ids_AddSortSample");//增加样本
    ids_GetSortSample = m_LanMap.value("ids_GetSortSample");//获取样本
    ids_ResetSortSample = m_LanMap.value("ids_ResetSortSample");//重置样本
    ids_SampleSize = m_LanMap.value("ids_SampleSize");//样本尺寸
    ids_ToResetSortSample = m_LanMap.value("ids_ToResetSortSample");//确定要重置样本吗？

    //    [cloud]
    ids_expiredToken = m_LanMap.value("ids_expiredToken");//token过期，重新获取
    ids_succeeded = m_LanMap.value("ids_succeeded");//成功
    ids_requestOkButReturnException = m_LanMap.value("ids_requestOkButReturnException");//请求成功，但云平台返回异常
    ids_illegalURL = m_LanMap.value("ids_illegalURL");//非法URL
    ids_requestFailed = m_LanMap.value("ids_requestFailed");//请求失败
    ids_requestException = m_LanMap.value("ids_requestException");//请求遇到异常
    ids_performErrorCode = m_LanMap.value("ids_performErrorCode");//执行错误码
    ids_perform = m_LanMap.value("ids_perform");//并成功执行，
    ids_postErrorCode = m_LanMap.value("ids_postErrorCode");//上报错误码
    ids_post = m_LanMap.value("ids_post");//并上报
    ids_returnErrorCode = m_LanMap.value("ids_returnErrorCode");//返回错误码
    ids_postRunLog = m_LanMap.value("ids_postRunLog");//运行日志发起上报：
    ids_postModeInfo = m_LanMap.value("ids_postModeInfo");//模式信息发起上报：
    ids_postStatusInfo = m_LanMap.value("ids_postStatusInfo");//设备状态信息发起上报：
    ids_postAlarmInfo = m_LanMap.value("ids_postAlarmInfo");//报警信息发起上报：
    ids_postMaterialInfo = m_LanMap.value("ids_postMaterialInfo");//物料信息发起上报：
    ids_receivedCapacity = m_LanMap.value("ids_receivedCapacity");//收到产量设置命令，
    ids_receivedShutdown = m_LanMap.value("ids_receivedShutdown");//收到关机命令，
    ids_receivedRunStop = m_LanMap.value("ids_receivedRunStop");//收到运行/停止命令，
    ids_receivedSwitchMode = m_LanMap.value("ids_receivedSwitchMode");//收到模式切换命令，
    ids_receivedSetAccuracy = m_LanMap.value("ids_receivedSetAccuracy");//收到精度设置命令，
    ids_receivedSave = m_LanMap.value("ids_receivedSave");//收到保存参数命令，
    ids_receivedCmd = m_LanMap.value("ids_receivedCmd");//收到命令：
    ids_topicMatchingFailed = m_LanMap.value("ids_topicMatchingFailed");//订阅主题匹配失败
    ids_mqttConnectionSucceeded = m_LanMap.value("ids_mqttConnectionSucceeded");//mqtt连接成功
    ids_loopSucceeded = m_LanMap.value("ids_loopSucceeded");//loop成功
    ids_subscribeSucceeded = m_LanMap.value("ids_subscribeSucceeded");//订阅成功
    ids_reconnectLoop = m_LanMap.value("ids_reconnectLoop");//loop失败，准备重连接。
    ids_mqttReConnectionSucceeded = m_LanMap.value("ids_mqttReConnectionSucceeded");//mqtt重连接成功
    ids_mqttReConnectionFailed = m_LanMap.value("ids_mqttReConnectionFailed");//mqtt重连接失败，
    ids_subscribeFailed = m_LanMap.value("ids_subscribeFailed");//订阅失败，
    ids_loopFailed = m_LanMap.value("ids_loopFailed");//loop失败，
    ids_mqttConnectionFailed = m_LanMap.value("ids_mqttConnectionFailed");//mqtt连接失败，
    ids_show_yeild_per_channel  = m_LanMap.value("ids_show_yeild_per_channel");// 实时产量
    ids_Detect_do_not_test_eject  = m_LanMap.value("ids_Detect_do_not_test_eject");// 检测中不能测阀!
    ids_agree  = m_LanMap.value("ids_agree");// 同意并继续
    ids_disagree = m_LanMap.value("ids_disagree");//不同意并退出

    ids_postRealTimeParamsInfo  = m_LanMap.value("ids_postRealTimeParamsInfo");// 实时温度信息上报：
    ids_postMachineParamsInfo  = m_LanMap.value("ids_postMachineParamsInfo");// 整机参数信息上报：
    ids_postPartStatusInfo  = m_LanMap.value("ids_postPartStatusInfo");// 部件状态加预警上报:
    ids_postParamAlarmInfo  = m_LanMap.value("ids_postParamAlarmInfo");// 参数异常警报上报:
    ids_postStatisticsParamInfo  = m_LanMap.value("ids_postStatisticsParamInfo");// 产量统计数据上报：
    ids_postRemoteProtocol  = m_LanMap.value("ids_postRemoteProtocol");// 远程连接同意上报:
    ids_valveSystem  = m_LanMap.value("ids_valveSystem");// 喷阀系统
    ids_lampSystem  = m_LanMap.value("ids_lampSystem");// 灯光系统
    ids_running_time  = m_LanMap.value("ids_running_time");// 运行时间
    ids_usefultime_valve  = m_LanMap.value("ids_usefultime_valve");// 喷阀寿命
    ids_usefultime_filter  = m_LanMap.value("ids_usefultime_filter");// 过滤寿命
    ids_usefultime_clean  = m_LanMap.value("ids_usefultime_clean");//清灰寿命
    ids_usefultime_lamp  = m_LanMap.value("ids_usefultime_lamp");// 灯光寿命
    ids_operate_log  = m_LanMap.value("ids_operate_log");//操作日志


    /************************ NEW **********************/
    input = m_LanMap.value("input");                  //输入
    clear = m_LanMap.value("clear");                  //清空
    on = m_LanMap.value("on");                     //开
    off = m_LanMap.value("off");                    //关
    list = m_LanMap.value("list");//文件名
    page_home = m_LanMap.value("page_home");              //主界面
    save_parameters = m_LanMap.value("save_parameters");        //保存参数
    set_purview = m_LanMap.value("set_purview");            //设置权限
    enter_system = m_LanMap.value("enter_system");           //进入系统
    last_page = m_LanMap.value("last_page");              //上一页
    next_page = m_LanMap.value("next_page");              //下一页
    state = m_LanMap.value("state");                  //状态
    cfm_restore_default = m_LanMap.value("cfm_restore_default");    //恢复默认

    dc_set = m_LanMap.value("dc_set");                 //加密设置
    dc_input = m_LanMap.value("dc_input");               //请输入
    dc_productsn = m_LanMap.value("dc_productsn");           //产品编号
    dc_productsn_mismatch = m_LanMap.value("dc_productsn_mismatch");  //产品编号错误
    dc_save_erro = m_LanMap.value("dc_save_erro");           //保存错误
    dc_illegal = m_LanMap.value("dc_illegal");             //密码非法
    dc_limit = m_LanMap.value("dc_limit");               //加密日期
    dc_illegal_used = m_LanMap.value("dc_illegal_used");  //密码重复

    vice_feeder = m_LanMap.value("vice_feeder");            //辅振动器
    feederCtrlType1 = m_LanMap.value("feederCtrlType1");         //振动方式1
    main_view = m_LanMap.value("main_view");
    vice_view = m_LanMap.value("vice_view");
    front_rear_vice = m_LanMap.value("front_rear_vice");
    front = m_LanMap.value("front");                  //前
    rear = m_LanMap.value("rear");                   //后
    front_rear_view = m_LanMap.value("front_rear_view");        //前后视
    upper = m_LanMap.value("upper");                  //上
    mid = m_LanMap.value("mid");                    //中
    lower = m_LanMap.value("lower");                  //下
    upper_view = m_LanMap.value("upper_view");             //上视
    lower_view = m_LanMap.value("lower_view");             //下视
    upper_layer = m_LanMap.value("upper_layer");            //上层
    lower_layer = m_LanMap.value("lower_layer");            //下层
    mid_layer = m_LanMap.value("mid_layer");              //中层
    layer = m_LanMap.value("layer");                  //层
    times = m_LanMap.value("times");                  //次
    splice = m_LanMap.value("splice");                 //分割
    ejector_num = m_LanMap.value("ejector_num");            //喷嘴数量



    basic_set = m_LanMap.value("basic_set");              //基本设置
    standard = m_LanMap.value("standard");               //基准值
    impurity_name = m_LanMap.value("impurity_name");         //杂质名称
    identify_params = m_LanMap.value("identify_params");        //识别参数
    arith_gray_a = m_LanMap.value("arith_gray_a");           //灰度算法A
    arith_gray_b  = m_LanMap.value("arith_gray_b");          //灰度算法B
    arith_gray_c  = m_LanMap.value("arith_gray_c");          //灰度算法C
    arith_gray_d  = m_LanMap.value("arith_gray_d");          //灰度算法D
    arith_discolor_a = m_LanMap.value("arith_discolor_a");       //色差算法A
    arith_discolor_b = m_LanMap.value("arith_discolor_b");       //色差算法B

    arith_cross = m_LanMap.value("arith_cross");            //差分算法
    arith_svm_a = m_LanMap.value("arith_svm_a");            //智能A
    arith_svm_b = m_LanMap.value("arith_svm_b");            //智能B
    arith_svm_c = m_LanMap.value("arith_svm_c");            //智能C
    arith_big_small = m_LanMap.value("arith_big_small");        //选大选小
    arith_long_short = m_LanMap.value("arith_long_short");       //选长选短
    arith_long_short_b = m_LanMap.value("arith_long_short_b");
    arith_circle_long = m_LanMap.value("arith_circle_long");      //选圆选长
    arith_sat = m_LanMap.value("arith_sat");                 // 饱和算法
    arith_hue = m_LanMap.value("arith_hue");                 // 颜色算法
    arith_scale = m_LanMap.value("arith_scale");             // 比例算法
    arith_mildew = m_LanMap.value("arith_mildew");           // 霉点算法
    arith_spl_mildew = m_LanMap.value("arith_spl_mildew");   // 葫芦籽霉点
    arith_white_peanut = m_LanMap.value("arith_white_peanut");// 白沙果算法
    arith_value = m_LanMap.value("arith_value");            // 灰度算法
    arith_outline = m_LanMap.value("arith_outline");         // 轮廓算法
    arith_line = m_LanMap.value("arith_line");               // 细纹
    arith_bud = m_LanMap.value("arith_bud");                 // 选芽
    arith_bud_b = m_LanMap.value("arith_bud_b");
    arith_round = m_LanMap.value("arith_round");           // 选圆非圆
    arith_hsv = m_LanMap.value("arith_hsv");                  // HSV算法
    related_mode = m_LanMap.value("related_mode");          //关联方式
    arith_big_small_b = m_LanMap.value("arith_big_small_b");           // 选圆非圆
    arith_circle_long_b = m_LanMap.value("arith_circle_long_b");       // 选圆非圆
    arith_round_b = m_LanMap.value("arith_round_b");           // 选圆非圆
    arith_pepper_handle = m_LanMap.value("arith_pepper_handle"); //辣椒选把

    mode = m_LanMap.value("mode");                   //模式
    purity = m_LanMap.value("purity");                 //纯度
    colorful = m_LanMap.value("colorful");               //彩色
    origin = m_LanMap.value("origin");                 //原图
    grey  = m_LanMap.value("grey");                  // 灰背景
    yellow  = m_LanMap.value("yellow");                // 黄背景
    red_green = m_LanMap.value("red_green");              //红绿色差
    red_blue = m_LanMap.value("red_blue");               //红蓝色差
    green_blue = m_LanMap.value("green_blue");             //绿蓝色差
    red_inf = m_LanMap.value("red_inf");                //红红外
    green_inf = m_LanMap.value("green_inf");              //绿红外
    blue_inf = m_LanMap.value("blue_inf");               //蓝红外
    hsv_h = m_LanMap.value("hsv_h");
    hsv_s = m_LanMap.value("hsv_s");
    hsv_v = m_LanMap.value("hsv_v");
    scale_index = m_LanMap.value("scale_index");
    yellow_sense = m_LanMap.value("yellow_sense");
    black_sense = m_LanMap.value("black_sense");
    sense_up = m_LanMap.value("sense_up");

    common = m_LanMap.value("common");                 //通用
    yellow_white = m_LanMap.value("yellow_white");           //黄白
    yellow_alpha = m_LanMap.value("yellow_alpha");           //黄透
    belly_white = m_LanMap.value("belly_white");            //腹白

    shape = m_LanMap.value("shape");                  //形选
    params_limit = m_LanMap.value("params_limit");           //参数下限
    second_alike_first = m_LanMap.value("second_alike_first");     //一二次相同
    all_seperate = m_LanMap.value("all_seperate");           //全部独立
    all_alike = m_LanMap.value("all_alike");              //全部相同
    arith_propotion = m_LanMap.value("arith_propotion");        //比例算法
    percent = m_LanMap.value("percent");                //纯度
    light_limit = m_LanMap.value("light_limit");            //亮度下限
    arith_line_defect = m_LanMap.value("arith_line_defect");      //线斑算法


    lamp_front_bg_red = m_LanMap.value("lamp_front_bg_red");      //前背景灯红
    lamp_front_bg_green = m_LanMap.value("lamp_front_bg_green");    //前背景灯绿
    lamp_front_bg_blue = m_LanMap.value("lamp_front_bg_blue");     //前背景灯蓝
    lamp_rear_bg_red = m_LanMap.value("lamp_rear_bg_red");       //后背景灯红
    lamp_rear_bg_green = m_LanMap.value("lamp_rear_bg_green");     //后背景灯绿
    lamp_rear_bg_blue = m_LanMap.value("lamp_rear_bg_blue");      //后背景灯蓝
    lamp_upper_bg_red = m_LanMap.value("lamp_upper_bg_red");      //上背景灯红
    lamp_upper_bg_green = m_LanMap.value("lamp_upper_bg_green");    //上背景灯绿
    lamp_upper_bg_blue = m_LanMap.value("lamp_upper_bg_blue");     //上背景灯蓝
    lamp_low_bg_red = m_LanMap.value("lamp_low_bg_red");        //下背景灯红
    lamp_low_bg_green = m_LanMap.value("lamp_low_bg_green");      //下背景灯绿
    lamp_low_bg_blue = m_LanMap.value("lamp_low_bg_blue");       //下背景灯蓝
    lamp_coef = m_LanMap.value("lamp_coef");              //系数

    wipe_set = m_LanMap.value("wipe_set");               //清灰设置
    wipe_duration = m_LanMap.value("wipe_duration");          //清灰周期(S)
    wipe_enable = m_LanMap.value("wipe_enable");            //清灰使能
    wipe_single = m_LanMap.value("wipe_single");            //单次清灰
    wipe_manual = m_LanMap.value("wipe_manual");            //手动清灰
    wipe_wind = m_LanMap.value("wipe_wind");              //清灰气帘
    wipe_wind_duration = m_LanMap.value("wipe_wind_duration");     //清灰气帘周期
    wipe_wind_interval = m_LanMap.value("wipe_wind_interval");     //清灰气帘间隔
    wipe_ai_set = m_LanMap.value("wipe_ai_set");            //智能清灰设置
    wipe_ai_off_chute = m_LanMap.value("wipe_ai_off_chute");      //关闭滑槽数
    wipe_ai_speed = m_LanMap.value("wipe_ai_speed");          //速度
    wipe_ai_width = m_LanMap.value("wipe_ai_width");          //宽度
    wipe_ai_propotion = m_LanMap.value("wipe_ai_propotion");      //比例
    wipe_ai_position = m_LanMap.value("wipe_ai_position");       //位置


    vavle = m_LanMap.value("vavle");                  //喷阀
    ejector_test_fast = m_LanMap.value("ejector_test_fast");      //快速自建
    ejector_test_signal = m_LanMap.value("ejector_test_signal");    //信号自检
    ejector_keep_loop = m_LanMap.value("ejector_keep_loop");      //单喷阀
    lamp_control = m_LanMap.value("lamp_control");           //设置灯光
    tick_params = m_LanMap.value("tick_params");            //剔除参数
    infect_arithmetic = m_LanMap.value("infect_arithmetic");      //传染
    infect_arithmetic_1 = m_LanMap.value("infect_arithmetic_1");    //传染一
    infect_arithmetic_2 = m_LanMap.value("infect_arithmetic_2");    //传染二
    edge_cut = m_LanMap.value("edge_cut");               //切除边缘
    edge_cut_1 = m_LanMap.value("edge_cut_1");             //切除边缘一
    edge_cut_2 = m_LanMap.value("edge_cut_2");             //切除边缘二
    bad_point = m_LanMap.value("bad_point");              //坏点个数
    logic_and = m_LanMap.value("logic_and");              //相与
    logical_relation = m_LanMap.value("logical_relation");       //相与相或
    normal_sort = m_LanMap.value("normal_sort");            //正选
    reverse_sort = m_LanMap.value("reverse_sort");           //反选
    sort_mode = m_LanMap.value("sort_mode");              //色选模式
    close_calculate = m_LanMap.value("close_calculate");        //闭运算
    svm_type1 = m_LanMap.value("svm_type1");
    svm_type2 = m_LanMap.value("svm_type2");
    svm_type3 = m_LanMap.value("svm_type3");


    // system status
    system_stat = m_LanMap.value("system_stat");            //系统状态
    camera_board = m_LanMap.value("camera_board");           //相机板
    recheck = m_LanMap.value("recheck");                //重新检测
    software_version = m_LanMap.value("software_version");       //版本
    version_factory = m_LanMap.value("version_factory");        //状态码 //内控版本
    version_custom = m_LanMap.value("version_custom");         //客户定制
    main_vavle = m_LanMap.value("main_vavle");             //
    vavle_type1 = m_LanMap.value("vavle_type1");
    vavle_type2 = m_LanMap.value("vavle_type2");
    normal = m_LanMap.value("normal");                 //正常
    abnormal = m_LanMap.value("abnormal");               //异常
    eject_frequency = m_LanMap.value("eject_frequency");        //吹气频率
    system_time = m_LanMap.value("system_time");            //系统时间
    year = m_LanMap.value("year");                   //年
    month = m_LanMap.value("month");                  //月
    day = m_LanMap.value("day");                    //日
    hour = m_LanMap.value("hour");                   //时
    minute = m_LanMap.value("minute");                 //分
    sec = m_LanMap.value("sec");                    //秒
    update = m_LanMap.value("update");                 //更新
    information = m_LanMap.value("information");            //信息
    yield = m_LanMap.value("yield");
    total_impurities = m_LanMap.value("total_impurities");
    single_impurity = m_LanMap.value("single_impurity");
    eject_stat = m_LanMap.value("eject_stat");             //喷阀状态
    alarm_turn_off = m_LanMap.value("alarm_turn_off");         //关振动器
    last_year_query = m_LanMap.value("last_year_query");        //近一年查询
    realtime_query = m_LanMap.value("realtime_query");         //实时查询
    total_feed_time = m_LanMap.value("total_feed_time");        //累计下料时间
    cur_pressure_alarm_times = m_LanMap.value("cur_pressure_alarm_times");//本次开机气压报警次数
    total_pressure_alarm_times = m_LanMap.value("total_pressure_alarm_times");//累计气压报警次数
    pressure_value = m_LanMap.value("pressure_value");         //气压
    build_time = m_LanMap.value("build_time");             //编译时间
    clearTotalRunTime = m_LanMap.value("ids_clearTotalRunTime"); //累计运行时间清零

    // net
    remote_control = m_LanMap.value("remote_control");         //远程控制
    network_config = m_LanMap.value("network_config");         //设置网络
    network_1 = m_LanMap.value("network_1");              //网络一
    network_2 = m_LanMap.value("network_2");              //网二
    ip_address = m_LanMap.value("ip_address");             //IP地址
    subnet_mask = m_LanMap.value("subnet_mask");            //子网掩码
    default_gateway = m_LanMap.value("default_gateway");        //默认网关
    dns_server = m_LanMap.value("dns_server");             //DNS服务器
    mac_address = m_LanMap.value("mac_address");            //MAC地址
    route_address = m_LanMap.value("route_address");          //路由器地址

    // remote control
    server_ip = m_LanMap.value("server_ip");              //服务器地址
    server_port = m_LanMap.value("server_port");            //服务器端口
    local_virtual_ip = m_LanMap.value("local_virtual_ip");       //本地虚拟地址
    username = m_LanMap.value("username");               //用户名
    password = m_LanMap.value("password");               //密码
    connect = m_LanMap.value("connect");                //链接
    disconnect = m_LanMap.value("disconnect");             //断开链接
    network_test = m_LanMap.value("network_test");           //网络测试
    msg_connecting = m_LanMap.value("msg_connecting");         //链接中。。。
    msg_connect_fail = m_LanMap.value("msg_connect_fail");       //链接失败
    msg_network_testing = m_LanMap.value("msg_network_testing");    //网络连接检测中
    msg_network_success = m_LanMap.value("msg_network_success");    //网络连接成功
    msg_network_fail = m_LanMap.value("msg_network_fail");       //网络连接失败
    msg_cert_request = m_LanMap.value("msg_cert_request");         //申请证书
    msg_cert_not_exist = m_LanMap.value("msg_cert_not_exist");       //证书不存在
    msg_cert_request_success = m_LanMap.value("msg_cert_request_success"); //证书申请成功
    msg_cert_import = m_LanMap.value("msg_cert_import");          //导入证书
    msg_cert_import_success = m_LanMap.value("msg_cert_import_success");  //证书导入成功


    // factory set
    factory_set = m_LanMap.value("factory_set");            //工厂模式
    engin_set = m_LanMap.value("engin_set");              //工程师设置
    advance = m_LanMap.value("advance");                //高级
    language = m_LanMap.value("language");               //语言
    product = m_LanMap.value("product");                //设置机型
    upgrade_screen = m_LanMap.value("upgrade_screen");         //上位机升级
    upgrade_fpga = m_LanMap.value("upgrade_fpga");           //下位机升级
    mode_1 = m_LanMap.value("mode_1");                 //模式1
    mode_2 = m_LanMap.value("mode_2");                 //模式2
    mode_3 = m_LanMap.value("mode_3");                 //模式3
    cfm_ts_calibrate = m_LanMap.value("cfm_ts_calibrate");       //确认校准触摸屏
    cfm_upgrade_screen = m_LanMap.value("cfm_upgrade_screen");     //确认升级升级上位机
    cfm_upgrade_fpga = m_LanMap.value("cfm_upgrade_fpga");       //确认升级下位机
    //factory
    camera_set = m_LanMap.value("camera_set");                 //设置相机
    signal_set = m_LanMap.value("signal_set");                 //相机信号
    system_misc_set = m_LanMap.value("system_misc_set");            //系统设置
    msg_times_set_err = m_LanMap.value("msg_times_set_err");          //分组错误
    // camera
    sensor_type = m_LanMap.value("sensor_type");                //传感器类型
    sensor_dip = m_LanMap.value("sensor_dip");                 //分辨率
    sensor_dim = m_LanMap.value("sensor_dim");                 //维度
    sensor_bit_width = m_LanMap.value("sensor_bit_width");           //位宽
    sensor_fre = m_LanMap.value("sensor_fre");                 //帧频
    camera_chain_addr = m_LanMap.value("camera_chain_addr");          //地址
    camera_arith_config = m_LanMap.value("camera_arith_config");        //算法配置
    camera_arith_list = m_LanMap.value("camera_arith_list");          //算法列表
    camera_signal_bias_enable = m_LanMap.value("camera_signal_bias_enable");  //偏置使能
    camera_signal_cali_enable = m_LanMap.value("camera_signal_cali_enable");  //校正使能
    msg_bright_cali_capture = m_LanMap.value("msg_bright_cali_capture");    //明场校正获取中...
    msg_bright_cali_save = m_LanMap.value("msg_bright_cali_save");       //明场校正保存...
    msg_dark_cali_capture = m_LanMap.value("msg_dark_cali_capture");      //暗场校正获取中...
    msg_dark_cali_save = m_LanMap.value("msg_dark_cali_save");         //暗场校正保存...
    pixel_join = m_LanMap.value("pixel_join");                 //亚像素调整
    show_ori_signal = m_LanMap.value("show_ori_signal");            //显示信号
    show_bright_signal = m_LanMap.value("show_bright_signal");//显示明场信号
    show_dark_signal = m_LanMap.value("show_dark_signal");//显示暗场信号
    pixelBad = m_LanMap.value("pixelBad");//坏点纠错
    ana_gain_reset = m_LanMap.value("ana_gain_reset");//增益重置
    cfm_ana_gain_reset = m_LanMap.value("cfm_ana_gain_reset");//确定增益重置
    get_all_bad = m_LanMap.value("get_all_bad");//获取所有坏点信息
    send_all = m_LanMap.value("send_all");//发送所有


    // pixel scale
    pixel_restore = m_LanMap.value("pixel_restore");              //像元重置

    nir_bad_set = m_LanMap.value("nir_bad_set");                //红外相机坏点设置
    nir_bad = m_LanMap.value("nir_bad");                         //坏点
    nir_bad_point = m_LanMap.value("nir_bad_point");                  //坏点坐标
    nir_tdi = m_LanMap.value("nir_tdi");                        //TDI设置
    nir_up_down = m_LanMap.value("nir_up_down");                    //TDI上下
    nir_correct = m_LanMap.value("nir_correct");                    //纠错后
    nir_show_all = m_LanMap.value("nir_show_all");                   //全显示

    // message
    msg_cfm_save_para = m_LanMap.value("msg_cfm_save_para");          //参数已更改，确认保存？
    msg_applying = m_LanMap.value("msg_applying");               //应用中...
    msg_turning_on = m_LanMap.value("msg_turning_on");             //正在打开下料
    msg_turning_off = m_LanMap.value("msg_turning_off");            //正在关闭下料
    msg_turn_off_first = m_LanMap.value("msg_turn_off_first");         //请先关闭下料
    msg_shutdown = m_LanMap.value("msg_shutdown");               //正在关机中...
    msg_wiping = m_LanMap.value("msg_wiping");                 //正在清灰中...
    msg_communicating = m_LanMap.value("msg_communicating");          //正在通信自检中...
    msg_require_version = m_LanMap.value("msg_require_version");        //正在版本检测中...
    msg_system_init = m_LanMap.value("msg_system_init");            //正在系统初始化...
    msg_mode_changing = m_LanMap.value("msg_mode_changing");          //正在切换模式中...
    msg_output_testing = m_LanMap.value("msg_output_testing");         //正在产量测试中...
    msg_change_init_mode = m_LanMap.value("msg_change_init_mode");       //正在切换启动模式...
    msg_saving_params = m_LanMap.value("msg_saving_params");          //正在保存参数中...
    msg_importing_params = m_LanMap.value("msg_importing_params");       //正在导入参数中...
    msg_exporting_params = m_LanMap.value("msg_exporting_params");       //正在导出参数中...
    msg_params_save_error = m_LanMap.value("msg_params_save_error");      //保存参数出错
    msg_delay_time_limit = m_LanMap.value("msg_delay_time_limit");       //延迟时间设置出错
    msg_tip_reboot = m_LanMap.value("msg_tip_reboot");

    // confirm
    cfm_turn_on = m_LanMap.value("cfm_turn_on");                //确认打开下料
    cfm_import_parameter = m_LanMap.value("cfm_import_parameter");       //确认导入参数
    cfm_export_parameter = m_LanMap.value("cfm_export_parameter");       //确认导出参数

    setByChute = m_LanMap.value("setByChute");                 //按料槽
    setByGroup = m_LanMap.value("setByGroup");                 //按次
    //>>>>FPGA升级部分翻译，暂时留着，但不用
    // fpga upgrade
    last = m_LanMap.value("last");
    next = m_LanMap.value("next");
    user_mode = m_LanMap.value("user_mode");
    board_num = m_LanMap.value("board_num");
    upgrade = m_LanMap.value("upgrade");
    upgrade_retry = m_LanMap.value("upgrade_retry");
    upgrade_whole = m_LanMap.value("upgrade_whole");
    upgrade_return = m_LanMap.value("upgrade_return");
    upgrade_file_list = m_LanMap.value("upgrade_file_list");
    upgrade_board = m_LanMap.value("upgrade_board");
    upgrade_fail = m_LanMap.value("upgrade_fail");
    upgrade_ok = m_LanMap.value("upgrade_ok");
    msg_select_user_mode = m_LanMap.value("msg_select_user_mode");
    cfm_upgrade_whole = m_LanMap.value("cfm_upgrade_whole");
    cfm_insert_udisk = m_LanMap.value("cfm_insert_udisk");
    //<<<<<<<<<<

    //sysstate
    valve_test = m_LanMap.value("valve_test");
    version_info = m_LanMap.value("version_info");
    machine_info = m_LanMap.value("machine_info");

    flow_statis = m_LanMap.value("flow_statis");
    vendor = m_LanMap.value("vendor");
    operating_mode = m_LanMap.value("operating_mode");         //工作模式
    set_delay_time_by_time = m_LanMap.value("set_delay_time_by_time");  //按次设置延迟时间
    arith_params_set_alike = m_LanMap.value("arith_params_set_alike");  //参数是否设置相同
    arith_big_small_limit = m_LanMap.value("arith_big_small_limit");   //大小算法上限
    edge_cut_max_limit= m_LanMap.value("edge_cut_max_limit");   //
    impured_max_limit= m_LanMap.value("impured_max_limit");   //
    eject_duration_max_limit= m_LanMap.value("eject_duration_max_limit");   //
    ui_style = m_LanMap.value("ui_style");                //界面风格
    flow_set = m_LanMap.value("flow_set");
    realtime_statistical_query_cycle = m_LanMap.value("realtime_statistical_query_cycle"); //实时统计查询周期
    one_click_recovery = m_LanMap.value("one_click_recovery");//一键还原当前方案算法名称
    alarm_method = m_LanMap.value("alarm_method");//报警方式
    lighting = m_LanMap.value("lighting");//灯光
    sound = m_LanMap.value("sound");//声音
    level_sensor = m_LanMap.value("level_sensor");//料位传感器
    top = m_LanMap.value("top");  //料位传感器是否放置在顶部
    temperature_sensor = m_LanMap.value("temperature_sensor");     //温度传感器
    pressure_alarm_type = m_LanMap.value("pressure_alarm_type");    //压力报警装置
    pressure_alarm_switch = m_LanMap.value("pressure_alarm_switch");  //压力开关
    pressure_alarm_digi = m_LanMap.value("pressure_alarm_digi");  //数显表
    continuous = m_LanMap.value("continuous");

    sensorNameRestore = m_LanMap.value("sensorNameRestore");        //传感器名称还原
    port = m_LanMap.value("port");                     //串口
    recovery_set = m_LanMap.value("recovery_set");             //还原设置
    recover_all = m_LanMap.value("recover_all");              //恢复全部
    recover_part = m_LanMap.value("recover_part");             //部分恢复
    recover_cancel = m_LanMap.value("recover_cancel");           //取消恢复
    cfm_recover_all = m_LanMap.value("cfm_recover_all");          //确认全部恢复
    cfm_recover_part = m_LanMap.value("cfm_recover_part");         //确认部分恢复
    recover_all_cfg_success = m_LanMap.value("recover_all_cfg_success");  //全部恢复配置成功
    recover_part_cfg_success = m_LanMap.value("recover_part_cfg_success"); //部分恢复配置成功
    days_avaiable = m_LanMap.value("days_avaiable");
    valveboard_pos = m_LanMap.value("valveboard_pos");
    big_feeder_pos = m_LanMap.value("big_feeder_pos");
    big_feeder = m_LanMap.value("big_feeder");
    big_feeder_freq = m_LanMap.value("big_feeder_freq");
    feeder_freq = m_LanMap.value("feeder_freq");
    teaFeeder = m_LanMap.value("teaFeeder");
    rice_mode = m_LanMap.value("rice_mode");
    material_mode = m_LanMap.value("material_mode");
    synchronization = m_LanMap.value("synchronization");
    independent = m_LanMap.value("independent");
    continuous = m_LanMap.value("continuous");
    continuous_sync = m_LanMap.value("continuous_sync");
    horizontalSpacing = m_LanMap.value("horizontalSpacing");
    verticalSpacing = m_LanMap.value("verticalSpacing");
    good_bad_differ = m_LanMap.value("good_bad_differ");
    rm_redundancy = m_LanMap.value("rm_redundancy");
    index = m_LanMap.value("index");
    nir_bad_list = m_LanMap.value("nir_bad_list");
    noModify_nonProfessionals = m_LanMap.value("noModify_nonProfessionals");
    accept_remote_upgrade = m_LanMap.value("accept_remote_upgrade");
    start = m_LanMap.value("start");
    pause = m_LanMap.value("pause");
    downloading = m_LanMap.value("downloading");
    download_pause = m_LanMap.value("download_pause");
    download_stop = m_LanMap.value("download_stop");
    download_ok = m_LanMap.value("download_ok");
    download_fail = m_LanMap.value("download_fail");
    wipe_continue_eject = m_LanMap.value("wipe_continue_eject");
    wipe_continue_feed = m_LanMap.value("wipe_continue_feed");
    crossCAM = m_LanMap.value("crossCAM");
    valveboard_pos = m_LanMap.value("valveboard_pos");
    big_feeder_pos = m_LanMap.value("big_feeder_pos");
    big_feeder = m_LanMap.value("big_feeder");
    big_feeder_freq = m_LanMap.value("big_feeder_freq");
    feeder_freq = m_LanMap.value("feeder_freq");
    teaFeeder = m_LanMap.value("teaFeeder");
    type = m_LanMap.value("type");
    heavy = m_LanMap.value("heavy");
    light = m_LanMap.value("light");
    all = m_LanMap.value("all");
    restore = m_LanMap.value("restore");
    pls_reboot_normaly = m_LanMap.value("pls_reboot_normaly");
    spray = m_LanMap.value("spray");
    wheelRun = m_LanMap.value("wheelRun");
    shortDrive = m_LanMap.value("shortDrive");
    longDrive = m_LanMap.value("longDrive");

    ipc_colorRestore = m_LanMap.value("ipc_colorRestore");
    ipc_model = m_LanMap.value("ipc_model");
    ipc_classCount = m_LanMap.value("ipc_classCount");
    ipc_blowTime2 = m_LanMap.value("ipc_blowTime2");
    ipc_lightBlow = m_LanMap.value("ipc_lightBlow");
    ipc_heavyBlow = m_LanMap.value("ipc_heavyBlow");
    ipc_flag = m_LanMap.value("ipc_flag");
    ipc_modelChange = m_LanMap.value("ipc_modelChange");
    ipc_toalCount = m_LanMap.value("ipc_toalCount");
    ipc_h_move = m_LanMap.value("ipc_h_move");
    ipc_h_width = m_LanMap.value("ipc_h_width");
    ipc_v_move = m_LanMap.value("ipc_v_move");
    ipc_v_width = m_LanMap.value("ipc_v_width");
    ipc_info = m_LanMap.value("ipc_info");
    ipc_state = m_LanMap.value("ipc_state");
    ipc_h_resolution = m_LanMap.value("ipc_h_resolution");
    ipc_v_resolution = m_LanMap.value("ipc_v_resolution");
    ipc = m_LanMap.value("ipc");
    ipc_num = m_LanMap.value("ipc_num");
    ipc_typeName = m_LanMap.value("ipc_typeName");
    ids_allInOne = m_LanMap.value("ids_allInOne");
    languageChange = m_LanMap.value("languageChange");
    checkLanChange = m_LanMap.value("checkLanChange");
    checkIPCOnOff = m_LanMap.value("checkIPCOnOff");
    waitingModel = m_LanMap.value("waitingModel");
    modelLoadingErr = m_LanMap.value("modelLoadingErr");
    gasConsumption = m_LanMap.value("gasConsumption");
    ipcState_idle = m_LanMap.value("ipcState_idle");
    ipcState_running = m_LanMap.value("ipcState_running");
    ipcState_loading = m_LanMap.value("ipcState_loading");
    ipcState_error = m_LanMap.value("ipcState_error");

    imgCapTureCount = m_LanMap.value("imgCapTureCount");
    msg_memory_space_inadequate = m_LanMap.value("msg_memory_space_inadequate");
    cfm_upload_ipcinfo = m_LanMap.value("cfm_upload_ipcinfo");
    msg_reset_machine_info = m_LanMap.value("msg_reset_machine_info");
    ids_valveDefend = m_LanMap.value("ids_valveDefend");
    ids_valveDefendTime = m_LanMap.value("ids_valveDefendTime");
    ids_valveUndefendTime = m_LanMap.value("ids_valveUndefendTime");
    alarm = m_LanMap.value("alarm");
    bad_low_limit = m_LanMap.value("bad_low_limit");
    mat_low_limit = m_LanMap.value("mat_low_limit");
    check = m_LanMap.value("check");
    checking = m_LanMap.value("checking");
    total_frame = m_LanMap.value("total_frame");
    speed = m_LanMap.value("speed");
    compression_ratio = m_LanMap.value("compression_ratio");
    cfm_start_net_test = m_LanMap.value("cfm_start_net_test");
    stop_testing = m_LanMap.value("stop_testing");
    cfm_start_ipc_status_test = m_LanMap.value("cfm_start_ipc_status_test");
    bad_spot_statistical_time = m_LanMap.value("bad_spot_statistical_time");
    alarm_and_resatrt_ipc = m_LanMap.value("alarm_and_resatrt_ipc");
    alarm_and_feed_off = m_LanMap.value("alarm_and_feed_off");
    add_model = m_LanMap.value("add_model");
    change_model = m_LanMap.value("change_model");
    apply_model = m_LanMap.value("apply_model");
    choose_model = m_LanMap.value("choose_model");
    check_model = m_LanMap.value("check_model");
    not_satrt = m_LanMap.value("not_satrt");
    model_file_miss = m_LanMap.value("model_file_miss");
    model_file_mismatch = m_LanMap.value("model_file_mismatch");
    connect_ipc_fail = m_LanMap.value("connect_ipc_fail");
    model_file_upgrading = m_LanMap.value("model_file_upgrading");
    upgrade_timeout = m_LanMap.value("upgrade_timeout");
    upgrade_success = m_LanMap.value("upgrade_success");
    model_file_upgrade_fail = m_LanMap.value("model_file_upgrade_fail");
    config_file_upgrade_fail = m_LanMap.value("config_file_upgrade_fail");
    other_error = m_LanMap.value("other_error");
    msg_ipc_poweroff = m_LanMap.value("msg_ipc_poweroff");

    ids_ScreenSet = m_LanMap.value("ids_ScreenSet");
    ids_nMirror = m_LanMap.value("ids_nMirror");
    ids_saturation = m_LanMap.value("ids_saturation");//饱和度
    maximum = m_LanMap.value("maximum");
    cur_model = m_LanMap.value("cur_model");
    ipc_reboot = m_LanMap.value("ipc_reboot");
    cfm_ipc_reboot = m_LanMap.value("cfm_ipc_reboot");

    msg_ipc_clean_filter_cotton = m_LanMap.value("msg_ipc_clean_filter_cotton");
    brightSelect = m_LanMap.value("brightSelect");
    sample_analysis = m_LanMap.value("sample_analysis");
    cfm_sync_class_name = m_LanMap.value("cfm_sync_class_name");
    classType = m_LanMap.value("classType");
    consecRunTest = m_LanMap.value("consecRunTest");
    compression = m_LanMap.value("compression");
    dropLine = m_LanMap.value("dropLine");
    cfm_ld_state = m_LanMap.value("cfm_ld_state");
    cfm_bright_cali_capture = m_LanMap.value("cfm_bright_cali_capture");
    cfm_dark_cali_capture = m_LanMap.value("cfm_dark_cali_capture");
    cfm_bright_cali_save = m_LanMap.value("cfm_bright_cali_save");
    cfm_dark_cali_save = m_LanMap.value("cfm_dark_cali_save");
    ipc_syncName = m_LanMap.value("ipc_syncName");
    ipc_verificating = m_LanMap.value("ipc_verificating");
    filter_background = m_LanMap.value("filter_BG");
    colorSelect = m_LanMap.value("colorSelect");
    ids_arithRejectRatio = m_LanMap.value("ids_arithRejectRatio");
    arith_suppress_large = m_LanMap.value("arith_suppress_large");
    turns = m_LanMap.value("turns");
    lowCfg_sync = m_LanMap.value("lowCfg_sync");
    arith_wheat_sprout = m_LanMap.value("arith_wheat_sprout");
    freeSpace = m_LanMap.value("freeSpace");
    ids_delay_select = m_LanMap.value("ids_delay_select");
    resizeMode = m_LanMap.value("resizeMode");
    dualLayerModelSame = m_LanMap.value("dualLayerModelSame");
    max_latency = m_LanMap.value("max_latency");
    keyPoint = m_LanMap.value("keyPoint");
    pos = m_LanMap.value("pos");
    HSCapture = m_LanMap.value("HSCapture","HSCapture");
    delay = m_LanMap.value("delay","delay");
    realtimeCorrect = m_LanMap.value("realtimeCorrect","realtimeCorrect");// 实时校正
    stopCapturing = m_LanMap.value("stopCapturing","stopCapturing");
    stopFRCapturing = m_LanMap.value("stopFRCapturing","stopFRCapturing");
    ejectstart = m_LanMap.value("ejectstart","ejectstart");
    ejectend = m_LanMap.value("ejectend","ejectend");
    ids_acceleration=m_LanMap.value("ids_acceleration");
    ids_acceleration_feedback1 = m_LanMap.value("ids_acceleration_feedback1"); //占空比1；
    ids_acceleration_feedback2 = m_LanMap.value("ids_acceleration_feedback2"); // 占空比2；
    ids_acceleration_tolerance = m_LanMap.value("ids_acceleration_tolerance"); // 容差值
    ids_acceleration_feeder    = m_LanMap.value("ids_acceleration_feeder");    // 下料器选择
    ids_acceleration_oblique  = m_LanMap.value("ids_acceleration_oblique");   // 斜料口
    ids_acceleration_straight = m_LanMap.value("ids_acceleration_straight");  // 直料口
    ids_intelligent_control = m_LanMap.value("ids_intelligent_control");    // 智能控制
    ids_vib_assist_control  = m_LanMap.value("ids_vib_assist_control");     // 振动器辅助控制

    cfm_screen_shot = m_LanMap.value("cfm_screen_shot");  // 确认截屏
    screen_shot_finish = m_LanMap.value("screen_shot_finish");   // 截屏完成

    Please_cfm_machine_number = m_LanMap.value("Please_cfm_machine_number");

    Hot_mode = m_LanMap.value("Hot_mode");           //热点模式
    WIFI_mode = m_LanMap.value("WIFI_mode");   //wifi 模式
    WIFI_name = m_LanMap.value("WIFI_name");           //热点名称
    current_mode = m_LanMap.value("current_mode");        //当前模式
    change_mode = m_LanMap.value("change_mode");        //切换模式
    connected_machine = m_LanMap.value("connected_machine");  // 已链接设备
    update_lib = m_LanMap.value("update_lib");  // 更新WIFI库
    export_screenshot_file = m_LanMap.value("export_screenshot_file");  //

    download= m_LanMap.value("download"); // 下载
    introduction= m_LanMap.value("introduction"); // 简介
    Thumb_up= m_LanMap.value("Thumb_up"); // 点赞
    No_introduction= m_LanMap.value("No_introduction"); //没有简介
    current_page= m_LanMap.value("current_page"); //当前页
    ids_kernelTime= m_LanMap.value("ids_kernelTime"); //内核版本
    ids_remote_download= m_LanMap.value("ids_remote_download"); //远程下载
    ids_autoControlBoard = m_LanMap.value("ids_autoControlBoard");        //
    cfm_open_plc_ctrl = m_LanMap.value("cfm_open_plc_ctrl");  //
    cfm_close_plc_ctrl = m_LanMap.value("cfm_close_plc_ctrl");  //
    plc_ctrling = m_LanMap.value("plc_ctrling");  //
    auto_ctrl_board_offline= m_LanMap.value("auto_ctrl_board_offline"); //
    auto_ctrl_board_ip= m_LanMap.value("auto_ctrl_board_ip"); //
    auto_ctrl_board_port= m_LanMap.value("auto_ctrl_board_port"); //
    gateway= m_LanMap.value("gateway"); //
    baudrate= m_LanMap.value("baudrate"); //
    ids_querying= m_LanMap.value("ids_querying"); //
    plc_ctrl= m_LanMap.value("plc_ctrl"); //
    DelayPowerCut= m_LanMap.value("DelayPowerCut"); //
    AutoCtrlEnable= m_LanMap.value("AutoCtrlEnable"); //
    PowerCut= m_LanMap.value("PowerCut"); //
    DefaultPlcEnable= m_LanMap.value("DefaultPlcEnable"); //
    plc_ctrl_set= m_LanMap.value("plc_ctrl_set"); //
    PowerCut= m_LanMap.value("PowerCut"); //
    DefaultPlcEnable= m_LanMap.value("DefaultPlcEnable"); //
    plc_ctrl_set= m_LanMap.value("plc_ctrl_set"); //
    model_same_all= m_LanMap.value("model_same_all"); //
    model_same_view= m_LanMap.value("model_same_view"); //
    model_same_group= m_LanMap.value("model_same_group"); //

    Begin_IP= m_LanMap.value("Begin_IP"); //
    End_IP= m_LanMap.value("End_IP"); //
    InstructTimeout= m_LanMap.value("InstructTimeout"); //
    UpgradeFileTimeout= m_LanMap.value("UpgradeFileTimeout"); //
    ForwardingInterval= m_LanMap.value("ForwardingInterval"); //
    PacketTimeout= m_LanMap.value("PacketTimeout"); //
    OneKey_Update= m_LanMap.value("OneKey_Update"); //

    Intelligent_Set= m_LanMap.value("Intelligent_Set"); //
    TCP_Set= m_LanMap.value("TCP_Set"); //
    Server_connection= m_LanMap.value("Server_connection"); //
    Screenshot= m_LanMap.value("Screenshot"); //
    together_export_image= m_LanMap.value("together_export_image"); //

    usePreSetCnf = m_LanMap.value("usePreSetCnf","usePreSetCnf");
    cfmClearAllCurrentCnf = m_LanMap.value("cfmClearAllCurrentCnf","cfmClearAllCurrentCnf");
    cfmChgCorrection = m_LanMap.value("cfmChgCorrection","cfmChgCorrection");
    ids_And= m_LanMap.value("ids_And"); //
    NoChange= m_LanMap.value("NoChange"); //
    ChangeUserMode= m_LanMap.value("ChangeUserMode"); //
    ids_setUserModeFailed= m_LanMap.value("ids_setUserModeFailed"); //
    ids_setUserModeSucceeded= m_LanMap.value("ids_setUserModeSucceeded"); //

    All_eject= m_LanMap.value("All_eject"); //
    length_width= m_LanMap.value("length_width"); //
    thickness= m_LanMap.value("thickness"); //
    Other_Group_error= m_LanMap.value("Other_Group_error"); //
    staticSet=m_LanMap.value("staticSet");
    allDayData=m_LanMap.value("allDayData");
    histogram=m_LanMap.value("histogram");
    PieChart=m_LanMap.value("PieChart");
    unitton=m_LanMap.value("unitton");
    batchName=m_LanMap.value("batchName");
    batchInfo=m_LanMap.value("batchInfo");
    classGroupNum=m_LanMap.value("classGroupNum");
    image_enhance=m_LanMap.value("image_enhance");
    staticClassname=m_LanMap.value("staticClassname");
    staticClassdata=m_LanMap.value("staticClassdata");

    Buffer_lines=m_LanMap.value("Buffer_lines");
    Confirm_information=m_LanMap.value("Confirm_information");
    information_error=m_LanMap.value("information_error");
    BG_points_cannot_be_marked =m_LanMap.value("BG_points_cannot_be_marked");
    Already_marked_as_good=m_LanMap.value("Already_marked_as_good");
    Already_marked_as_bad=m_LanMap.value("Already_marked_as_bad");
    Infection_intensity=m_LanMap.value("Infection_intensity");
    Bad_material_enough=m_LanMap.value("Bad_material_enough");
    Save_sample_failed=m_LanMap.value("Save_sample_failed");
    Good_material_enough=m_LanMap.value("Good_material_enough");
    Marked_material_not_saved=m_LanMap.value("Marked_material_not_saved");
    Completed=m_LanMap.value("Completed");
    No_file_detected=m_LanMap.value("No_file_detected");
    Reapply_machine_model=m_LanMap.value("Reapply_machine_model");

    Unzip_success =m_LanMap.value("Unzip_success");
    Unzip_failed =m_LanMap.value("Unzip_failed");
    Bottom=m_LanMap.value("Bottom");
    Low=m_LanMap.value("Low");
    Medium=m_LanMap.value("Medium");
    High=m_LanMap.value("High");
    Pass=m_LanMap.value("Pass");
    Abort_upgrade=m_LanMap.value("Abort_upgrade");
    ids_midPixel=m_LanMap.value("ids_midPixel");
    MaterialLevel=m_LanMap.value("MaterialLevel");
    Temperature=m_LanMap.value("Temperature");
    Reblow_time_not_less_blow_time=m_LanMap.value("Reblow_time_not_less_blow_time");
    Reblow_time_setting_error=m_LanMap.value("Reblow_time_setting_error");
    Adjust_ratio=m_LanMap.value("Adjust_ratio");
    Box_maximum_value=m_LanMap.value("Box_maximum_value");
    Switch=m_LanMap.value("Switch");
    Model_Nomatch_calibration=m_LanMap.value("Model_Nomatch_calibration");
    Stop_AI_image_capture=m_LanMap.value("Stop_AI_image_capture");
    Remote_request_lockControl=m_LanMap.value("Remote_request_lockControl");
    Remote_lockControl=m_LanMap.value("Remote_lockControl");
    Test_Version=m_LanMap.value("Test_Version");
    Download_rate=m_LanMap.value("Download_rate");
    Remaining_time=m_LanMap.value("Remaining_time");
    File_size=m_LanMap.value("File_size");
    Sample_import_success=m_LanMap.value("Sample_import_success");
    Sample_import_failed =m_LanMap.value("Sample_import_failed");

     No_sample_selected =m_LanMap.value("No_sample_selected");
     good_is_sufficient=m_LanMap.value("good_is_sufficient");
     bad_is_sufficient=m_LanMap.value("bad_is_sufficient");
     data_not_entered=m_LanMap.value("data_not_entered");
     Material_count=m_LanMap.value("Material_count");
     Material_area=m_LanMap.value("Material_area");
     Frame_area=m_LanMap.value("Frame_area");
     Invalid_scheme=m_LanMap.value("Invalid_scheme");
     Must=m_LanMap.value("Must");
     cfm_perform=m_LanMap.value("cfm_perform");


}
