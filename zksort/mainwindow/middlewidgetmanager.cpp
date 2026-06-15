#include "middlewidgetmanager.h"
#include "userwidget.h"
#include "engineerwidget.h"
#include "systeminfo/sysstatewidget.h"
#include "systeminfo/vavletest.h"
#include "factoryset/factorysetwidget.h"
#include "factoryset/languagesetwidget.h"
#include "zksort.h"
#include "mainpage/feedsetwidget.h"
#include "mainpage/modemanagewidget.h"
#include "mainpage/modelchangewidget.h"
#include "mainpage/sensesetwidget.h"
#include "engineerset/imagewidget.h"
#include "engineerset/upgrademodelwidget.h"
#include "mainpage/wipesetwidget.h"
#include "engineerset/backgroundsetwidget.h"
#include "factoryset/machinesetwidget.h"
#include "factoryset/camerasetwidget.h"
#include "arithprofilewidget/graysensesetwidget.h"
#include "engineerset/imagemanagewidget.h"
#include "arithprofilewidget/bigsmallsensesetwidget.h"
#include "arithprofilewidget/circlelongsensesetwidget.h"
#include "arithprofilewidget/longshortsensesetwidget.h"
#include "arithprofilewidget/svmsensesetwidget.h"
#include "arithprofilewidget/shapesvmsensesetwidget.h"
#include "arithprofilewidget/colorsatsensesetwidget.h"
#include "arithprofilewidget/colorhuesensesetwidget.h"
#include "arithprofilewidget/colorscalesensesetwidget.h"
#include "arithprofilewidget/mildewsensessetwidget.h"
#include "arithprofilewidget/outlinesensesetwidget.h"
#include "arithprofilewidget/linesensesetwidget.h"
#include "arithprofilewidget/budsensesetwidget.h"
#include "arithprofilewidget/colorvaluesensesetwidget.h"
#include "arithprofilewidget/roundsensesetwidget.h"
#include "arithprofilewidget/splmildewsensesetwidget.h"
#include "arithprofilewidget/whitepeanutsensesetwidget.h"
#include "mainpagewidget.h"
#include "identifywidget.h"
#include "ejectwidget.h"
#include "lightsetwidget.h"
#include "doublelightsetwidget.h"
#include "engineerset/backgroundsetwidget.h"
#include "factoryset/camerasignalwidget.h"
#include "factoryset/dividepixelwidget.h"
#include "factoryset/pixeldivisionshowwidget.h"

#include "engineerset/imagecapturewidget.h"
#include "engineerset/imageanalysiswidget.h"
#include "factoryset/machinefunction.h"
#include "mainpage/dccryptwidget.h"
#include "mainpage/ejectdelay.h"
#include "mainpage/selfcheckwidget.h"
#include "mainpage/simulatewidget.h"
#include "factoryset/pixeladjust.h"
#include "factoryset/lampboardcfg.h"
#include "mainpage/newmainpage.h"
#include "factoryset/nirdotcorrectivelistwidget.h"
#include "arithprofilewidget/discolorsensesetwidget.h"

#include "mainpage/feederaotuajust.h"
#include "mainpage/aiwipesetwidget.h"
#include "mainpage/createnewmodewidget.h"
#include "mainpage/ipcclassparams.h"
#include "factoryset/tempsensorsetwidget.h"
#include "factoryset/feedersensorsetwidget.h"
#include "factoryset/updatefpgawidget.h"
#include "arithprofilewidget/hsvsensesetwidget.h"

#include "factoryset/ipcsetwidget.h"
#include "factoryset/ipccamerasetwidget.h"
#include "factoryset/ipcalarmcontrol.h"
#include "arithprofilewidget/pepperhandlesensewidget.h"
#include "factoryset/cameracolorrestore.h"
#include "engineerset/imagedatachartwidget.h"
#include "engineerset/imagedatachartrgbwidget.h"
#include "engineerset/imagedatachartdiscolorwidget.h"
#include "engineerset/ipcejectwidget.h"
#include "engineerset/imageinforgbwidget.h"
#include "factoryset/updateacswidget.h"
#include "arithprofilewidget/suppresslargesensesetwidget.h"
#include "arithprofilewidget/wheatsproutsensesetwidget.h"
#include "factoryset/ipcaisetwidget.h"
#include "mysortwidget/mysvmsampleclass.h"
#include "engineerset/imagefastcapturewidget.h"
#include "factoryset/onekeyupdatefpgawidget.h"
#include "factoryset/selectupdatefilewidget.h"
#include "arithprofilewidget/teacolorsensesetwidget.h"
#include "plastic/irbadpointsetwidget.h"
#include "engineerset/svmimagewidget.h"
#include "plastic/accelerationsensorsetwidget.h"
#include "systeminfo/videtectboardinfowidget.h"
#include "mainpage/modelselectwidget.h"
#include "mainpage/remoteControl.h"
#include "autocontrol/plcctrlwidget.h"
#include "factoryset/fpgamodesetwidget.h"

#include "systeminfo/sysoutputwidget.h"
#include "systeminfo/ipcclasssetwidget.h"
MiddleWidgetManager::MiddleWidgetManager(QObject *parent) : QObject(parent)
{
    m_CurIndex = -1;
    m_pCurWidget = NULL; 
    m_pStackedWidget = NULL;
    m_pMainWidget = NULL;
}

void MiddleWidgetManager::returnBackPage()
{
    if(m_pCurWidget)
        m_pCurWidget->returnBack();
}

void MiddleWidgetManager::setStackedWidget(QStackedWidget *stack, ZKSort *mainWidget)
{
    m_pStackedWidget = stack;
    m_pMainWidget = mainWidget;
}

void MiddleWidgetManager::RefreshCurWidget()
{
    if(m_pCurWidget){
        m_pCurWidget->showPage();
        m_pMainWidget->PageChanged(m_pCurWidget->m_CurrentPageId);
    }
}

void MiddleWidgetManager::ShowWidget(uint pageId, bool bBack)
{
    int index;
    bool bFirstCreate = false;
    //需要显示的页面不存在，则通过页面ID创建
    if(!m_WidgetMap.contains(pageId)){
        index=CreatePageByID(pageId);
        bFirstCreate = true;
        if(index == -1){
            logger()->warn("create page fail,page name is not exist");
            return;
        }
    }

    index = m_WidgetMap[pageId];
    if(index == m_CurIndex)
        return;
       //如果和当前页相同，返回

    basewidget* widget = (basewidget *) m_pStackedWidget->widget(index);
    if(NULL == widget)
        return;
    if(bFirstCreate){
        widget->retranslateUiWidget();
    }
    widget->m_CurrentPageId = pageId;
    if(m_pCurWidget != NULL)
    {
        if(false == bBack)
        {
            widget->m_PreviousPageId = m_pCurWidget->m_CurrentPageId;
        }
        m_pCurWidget->showPage(false);
    }

    //保存当前信息，通知显示了，子页面作相应的工作
    m_CurIndex = index;
    m_pCurWidget = widget;

    m_pStackedWidget->setCurrentIndex(m_CurIndex);

    m_pCurWidget->showPage();

    struGsh.nPageSm = m_pCurWidget->m_CurrentPageId;

    m_pMainWidget->PageChanged(pageId);
}

void MiddleWidgetManager::CreateWidgets()
{
    if(struCnfg.nUseIpcEnable && struCnfg.nDPMachine)
        CreatePageByID(SM_IPC_CLASS_PARAMS_WIDGET);
}

void MiddleWidgetManager::SendCurWidgetMsg(int msg1, int msg2, int msg3)
{
    if(m_pCurWidget)
        m_pCurWidget->receiveMsg(msg1,msg2,msg3);
}

void MiddleWidgetManager::SendWidgetMsg(uint pageId, int msg1, int msg2, int msg3)
{
    basewidget *widget = (basewidget*)m_pStackedWidget->widget(m_WidgetMap[pageId]);
    if(widget){
        widget->receiveMsg(msg1,msg2,msg3);
    }
}


basewidget *MiddleWidgetManager::GetCurWidget()
{
    Q_ASSERT(m_pCurWidget == NULL);
    return m_pCurWidget;
}

basewidget *MiddleWidgetManager::GetWidgetByID(uint pageId)
{
    if(m_WidgetMap.contains(pageId))
        return (basewidget*)m_pStackedWidget->widget(m_WidgetMap[pageId]);
    return NULL;
}

void MiddleWidgetManager::SetBottomEnable(bool enable)
{
    m_pMainWidget->setBottomEnable(enable);
}
void MiddleWidgetManager::RefreshBottomStatus()
{
    m_pMainWidget->refreshBottomStatus();
}

int MiddleWidgetManager::CreatePageByID(uint pageId)
{
    int widgetIndex = 0;
    switch (pageId) {
    case SM_USER_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new UserWidget);
        break;
    case SM_ENGINEER_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new EngineerWidget);
        break;
    case SM_SYSSTATE_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new SysStateWidget);
        break;
    case SM_VAVLE_TEST_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new vavletest);
        break;
    case SM_FACTORY_SET_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new FactorySetWidget);
        break;
    case SM_LANGUAGE_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new LanguageSetWidget);
        break;
    case SM_FEED_SET_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new FeedSetWidget);
        break;
    case SM_MODE_MANAGE_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new ModeManageWidget);
        break;
    case SM_SENSE_SET_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new SenseSetWidget);
        break;
    case SM_BACKGROUND_SET_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new BackGroundSetWidget);
        break;
    case SM_IAMGE_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new ImageWidget);
        break;
    case SM_MACHINE_SET_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new MachineSetWidget);
        break;
    case SM_CAMERA_SET_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new CameraSetWidget);
        break;
    case SM_GRAYSENSE_SET_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new GraySenseSetWidget);
        break;
    case SM_IMAGE_MANAGE_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new ImageManageWidget);
        break;
    case SM_BIG_SMALL_SENSE_SET_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new BigSmallSenseSetWidget);
        break;
    case SM_CIRCLE_LONG_SENSE_SET_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new CircleLongSenseSetWidget);
        break;
    case SM_LONG_SHORT_SENSE_SET_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new LongShortSenseSetWidget);
        break;
    case SM_SVM_SENSE_SET_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new SvmSenseSetWidget);
        break;
    case SM_MAIN_PAGE_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new MainPageWidget);
        break;
    case SM_IDENTIFY_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new IdentifyWidget);
        break;
    case SM_EJECT_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new EjectWidget);
        break;
    case SM_LIGHE_SET_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new LightSetWidget);
        break;
    case SM_DOUBLE_LIGHE_SET_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new DoubleLightSetWidget);
        break;
    case SM_WIPE_SET_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new wipeSetWidget);
        break;
    case SM_CAMERA_SIGNAL_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new CameraSignalWidget);
        break;
    case SM_DIVIDE_PIXEL_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new DividePixelWidget);
        break;
    case SM_DIVIDE_PIXEL_SHOWALL_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new PixelDivisionShowWidget);
        break;
    case SM_IMAGE_CAPTURE_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new ImageCaptureWidget);
        break;
    case SM_IMAGE_ANALYSIS_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new ImageAnalysisWidget);
        break;
    case SM_MACHINE_FUNC_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new MachineFunction);
        break;
    case SM_MACHINE_DCCRYPT_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new DccryptWidget);
        break;
    case SM_SELF_CHECK_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new SelfCheckWidget);
        break;
    case SM_EJECT_TIME_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new ejectDelay);
        break;
    case SM_SIMULATE_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new SimulateWidget);
        break;
    case SM_PIXEL_AJUST_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new PixelAdjust);
        break;
    case SM_LAMP_BOARD_SET_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new LampBoardCfg);
        break;
    case SM_MAIN_PAGE_WIDGET_NEW:
        widgetIndex = m_pStackedWidget->addWidget(new NewMainPage);
        break;
    case SM_COLOR_SAT_SENSE_SET_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new ColorSatSenseSetWidget);
        break;
    case SM_COLOR_HUE_SENSE_SET_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new ColorHueSenseSetWidget);
        break;
    case SM_COLOR_SCALE_SENSE_SET_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new ColorScaleSenseSetWidget);
        break;
    case SM_MILDEW_SENSE_SET_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new MildewSensesSetWidget);
        break;
    case SM_OUTLINE_SENSE_SET_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new OutlineSenseSetWidget);
        break;
    case SM_LINE_SENSE_SET_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new LineSenseSetWidget);
        break;
    case SM_BUD_SENSE_SET_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new BudSenseSetWidget);
        break;
    case SM_COLOR_VALUE_SENSE_SET_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new ColorValueSenseSetWidget);
        break;
    case SM_NIR_DOT_CORRECTIVE_LIST_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new NirDotCorrectiveListWidget);
        break;
    case SM_DISCOLOR_SENSE_SET_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new DiscolorSenseSetWidget);
        break;
    case SM_AUTO_FEEDER_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new FeederAotuAjust);
        break;
    case SM_AI_WIPE_SET_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new AiWipeSetWidget);
        break;
    case SM_CREATE_MODE_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new CreateNewModeWidget);
        break;
    case SM_FEEDER_SENSOR_SET_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new FeederSensorSetWidget);
        break;
    case SM_TEMP_SENSOR_SET_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new TempSensorSetWidget);
        break;
    case SM_UPDATE_FPGA_WDIGET:
        widgetIndex = m_pStackedWidget->addWidget(new UpdateFpgaWidget);
        break;
    case SM_HSV_SENSE_SET_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new HsvSenseSetWidget);
        break;
    case SM_ROUND_SENSE_SET_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new RoundSenseSetWidget);
        break;
    case SM_SPL_MILDEW_SENSE_SET_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new splMildewSenseSetWidget);
        break;
    case SM_WHITE_PEANUT_SENSE_SET_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new WhitePeanutSenseSetWidget);
        break;
    case SM_SHAPE_SVM_SENSE_SET_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new ShapeSvmSenseSetWidget);
        break;
    case SM_PEPPER_HANDLE_SENSE_SET_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new PepperHandleSenseWidget);
        break;
    case SM_IPC_SET_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new IpcSetWidget);
        break;
    case SM_IPC_CAMERA_SET_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new IpcCameraSetWidget);
        break;
    case SM_CAMERA_COLOR_RESTORE_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new cameraColorRestoreWidget);
        break;
    case SM_IPC_CLASS_PARAMS_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new ipcclassparams);
        break;
    case SM_IPC_MODEL_CHANGE_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new ModelChangeWidget);
        break;
    case SM_IMAGE_DATA_CHART_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new ImageDataChartWidget);
        break;
    case SM_IMAEG_DATA_RGB_CHART_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new ImageDataChartRgbWidget);
        break;
    case SM_IMAEG_DATA_DISCOLOR_CHART_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new ImageDataChartDiscolorWidget);
        break;
    case SM_IPC_UPGRADE_MODEL_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new upgradeModelWidget);
        break;
    case SM_IPC_ALARM_CONTROL_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new ipcAlarmControlWidget);
        break;
    case SM_IPC_EJECT_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new IpcEjectWidget);
        break;
    case SM_IMAGE_INFO_RGB_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new ImageInfoRgbWidget);
        break;        
    case SM_UPDATE_ACS_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new UpdateAcsWidget);
        break;
    case SM_SUPPRESS_LARGE_SENSE_SET_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new SuppressLargeSenseSetWidget);
        break;
    case SM_WHEAT_SPROUT_SENSE_SET_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new WheatSproutSenseSetWidget);
        break;
    case SM_IPC_AI_SET_WIDGET:
        widgetIndex =  m_pStackedWidget->addWidget(new IpcAISetWidget);
        break;
    case SM_SVM_SAMPLE_WIDGET:
        widgetIndex =  m_pStackedWidget->addWidget(new MySvmSampleClass);
        break;
    case SM_IMAGE_FAST_CAPTURE_WIDGET:
        widgetIndex =  m_pStackedWidget->addWidget(new ImageFastCaptureWidget);
        break;
    case SM_ONE_KEY_UPDATE_FPGA_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new OneKeyUpdateFpgaWidget);
        break;
    case SM_SELECT_UPDATE_FILE_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new SelectUpdateFileWidget);
        break;
    case SM_TEA_COLOR_SENSE_SET_WIDGET:
        widgetIndex =  m_pStackedWidget->addWidget(new TeaColorSenseSetWidget);
        break;
    case SM_IR_BAD_POINT_WIDGET_I:
        widgetIndex = m_pStackedWidget->addWidget(new IrBadPointSetWidget);
        break;
    case SM_SVM_IMAGE_WIDGET:
        widgetIndex =  m_pStackedWidget->addWidget(new SvmImageWidget);
        break;
    case SM_ACCELERATION_SENSOR_SET_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new AccelerationSensorSetWidget);
        break;    
	case SM_VI_DETECT_BOARD_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new VIDetectBoardInfoWidget);
        break;
    case SM_IPC_REMOTE_DOWNLOAD_MODEL_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new ModelSelectWidget);
        break;
    case SM_REMOTE_CONTROL_WIDGET:
        #ifdef Q_OS_UNIX
        widgetIndex = m_pStackedWidget->addWidget(new RemoteControlWidget);
        #endif
        break;
    case SM_PLC_CTRL_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new PlcCtrlWidget);
        break;
    case SM_FPGA_SET_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new FpgaModeSetWidget);
        break;
    case SM_IPC_OUTPUT_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new SysOutputWidget);
        break;
    case SM_IPC_CLASSSET_WIDGET:
        widgetIndex = m_pStackedWidget->addWidget(new IpcClassSetWidget);
        break;
    default:
        break;
    }
    m_WidgetMap[pageId] = widgetIndex;
    return widgetIndex;
}

void MiddleWidgetManager::RecvMsgSlot(int msg1, int msg2, int msg3)
{
    SendCurWidgetMsg(msg1,msg2,msg3);
}

void MiddleWidgetManager::RecvMsgToPageSlot(QString pageName, int msg1, int msg2, int msg3)
{

}
