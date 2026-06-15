#ifndef FPGA_H
#define FPGA_H

#include <QWidget>
#include <QtGui>
#include <QtCore>

//#include "common/globalconfig.h"
//#include "common/mypushbutton.h"
//#include "common/mymessagebox.h"
//#include "common/mymutex.h"
#include "myqextserialport.h"
#include "constant.h"
#include "mySortWidget/mywid.h"

/* 通用机型下远程升级部分宏定义 */
#define PACKET_COUNT        8192        // 发送2M rpd文件需要读取包的次数
#define PACKET_COUNT_SMALL  2048        // 发送512K rpd文件需要读取包的次数
#define FILE_PACKET_SIZE    256         // 文件包的大小
#define PACKET_SIZE         24          // 命令包的大小

/* 玉米机型下远程升级部分宏定义 */
#define FILE_PACKET_SIZE_MAIZE      8192    // 玉米机型使用高速串口,每个包大小为8192字节
#define PACKET_COUNT_MAIZE          256     // 玉米机型使用高速串口,共256个包
#define PACKET_COUNT_SMALL_MAIZE    64      // 玉米机型下红外升级文件供64个包

#define SNED_FILE_BY_USB    1       // 表征当前发送升级文件的形式
#define UPDATE_FILE_PATH    "/udisk/"   // 升级文件目录

#ifdef Q_OS_UNIX
#include "common/myusb/myusb.h"
#endif

/*!
 * \brief 彩色机型下位机远程升级界面类
 */
class fpga : public QDialog
{
    Q_OBJECT
    
public:
    fpga(QWidget *parent = 0);
    ~fpga();

    void initUpdateParams(void);        // 初始化下位机升级相关参数

    void createMountUdiskPage(void);    // 创建挂载U盘界面
    void createFileListPage(void);      // 创建文件列表界面
    void createNoticePage(void);        // 创建提示界面
    void createBoardUpdatePage(void);   // 创建板级升级界面
    void createWholeUpdatePage(void);   // 创建整机升级界面
    void setUpdateDisplay(void);        // 根据当前状态设置板级升级界面的显示
    void setUpdateWholeDisplay(void);   // 根据当前状态设置整机升级界面的显示

    int intBoardUpdate(int step);       // 接口板升级流程
    int colorBoardUpdate(int step);     // 彩色相机板升级流程
    int infBoardUpdate(int step);       // 红外相机板升级流程
    int ctrlBoardUpdate(int step);      // 控制板升级流程
    void wholeUpdate(void);             // 整机升级
    void wholeUpdatIntferMaize(void);

signals:
    void sBackToFactorySetPage();

private slots:
    void onStackedWidgetIndexChangedSlt(int index);
    void goToFirstPageSlt();

    /* 挂载页面的槽函数 */
    void onMountUdiskBackBtnClicked();
    void onMountUdiskGoBtnClicked();

    /* 文件列表界面的槽函数 */
    void onFileListChangedSlt(int index);
    void onFileListNextBtnClicked();
    void onFileListLastBtnClicked();
    void onTransModeBtnClicked();

    /* 提示页面的槽函数 */
    void onNoticeLastBtnClicked();
    void onNoticeNextBtnClicked();

    /* 板级升级页面的槽函数 */
    void onBoardUpdateBackBtnClicked();
    void onBoardUpdateGoBtnClicked();
    void onBoardUpdateWholeBtnClicked();
    void onBoardUpdateRetryBtnClicked();
    void onUserModeChangedSlt(int index);
    void onLayerChangedSlt(int index);
    void onlayerAddChangedSlt(int index);
    void onBoardNumPlusBtnClicked();
    void onBoardNumMinusBtnClicked();

    /* 整机升级页面的槽函数 */
    void onWholeUpdateBackBtnClicked();
    void onWholeUpdateReturnBtnClicked();

private:
    /* 远程升级页面索引号 */
    enum{
        MOUNT_PAGE,         // 挂载页面
        FILE_LIST_PAGE,     // 文件列表界面
        NOTICE_PAGE,        // 提示页面
        UPDATE_PAGE,        // 板级升级页面
        WHOLE_UPDATE_PAGE   // 整机升级页面
    };

    /* 板类型索引号 */
    typedef enum {
        INT_BOARD,          // 接口板
        COLOR_BOARD,        // 彩色相机板
        CTRL_BOARD,         // 控制板
        VAVLE_BOARD,        // 喷阀板
        LAMP_BOARD,         // 灯板
    }boardMode;

    enum {
        MES_NUM = 7,
        MAX_FILE_NUM = 18,
        MAX_BOARD_PER_LEVEL = 8
    };

    /* 升级状态索引号 */
    typedef enum {
        UPDATE_NO_START,    // 升级未开始
        UPDATE_DOING,       // 升级进行中
        UPDATE_OK,          // 升级成功
        UPDATE_FAIL,        // 升级失败
        UPDATE_NO_EXIST     // 升级板不存在
    }updateStatus;

    void USBSendFile(int cmd, unsigned char module, int num);   // USB发送文件

    bool stepSwitchToFactory(boardMode module); // 切换下位机至工厂模式
    bool stepSendFile(void);                    // 向下位机发送升级文件
    bool stepSendFileByHighUART(void);          // 通过高速串口发送升级文件
    bool stepSendFileByUSB(void);               // 通过USB向下位机发送升级文件
    bool stepBurnFile(void);                    // 下位机烧写升级文件
    bool stepSwitchToUser(boardMode module);    // 切换下位机至用户模式

    bool ctrlSwitchToFactory(void);             // 切换控制板至工厂模式
    bool ctrlEraseFlash(void);                  // 擦除控制板的用户控件
    bool ctrlSendFile(void);                    // 向控制板发送文件
    bool ctrlSendFileImprove(void);             // 向控制板发送文件（改进版）
    bool ctrlSwitchToUser(void);                // 切换控制板至用户模式


    void setStatusLabelShow(int mesIndex, updateStatus status);     //根据某一步升级状态显示不同内容
    void setWholeStatusShow(int level, int mesIndex, updateStatus status);  //根据单板的升级状态显示不同内容

//    g_Config *config;
    QStackedWidget *stackedWidget;
    int margin;
    QPalette pe;

    /* 下位机升级相关参数 */
    quint8 defIntMode;              //默认的接口板用户模式
    quint8 defCameraMode[MAX_VIEW];            //默认的相机板用户模式
    quint8 defCtrlMode;            //默认的控制板用户模式
    quint8 defLampMode;            //默认的灯光板用户模式


    boardMode curBoardMode;         //当前升级板子的类型
    quint8 intUserMode;             //当前接口板的用户模式
    quint8 cameraUserMode[MAX_VIEW];           //当前彩色相机板的用户模式
    quint8 vavleUserMode[MAX_VAVLE];           //当前喷阀板的用户模式
    quint8 lampUserMode;           //当前彩色相机板的用户模式
    quint8 ctrlUserMode;            //当前控制板的用户模式

    bool bCameraFileIsOk[MAX_VIEW]; //表示相机升级文件是否已下载成功
    bool bVavleFileIsOk[MAX_VAVLE]; //表示喷阀板升级文件是否已下载成功
    bool bCtrlFileIsOk;             //表示控制板升级文件是否已下载成功
    bool bLampFileIsOk;             //表示灯光板升级文件是否已下载成功

    quint8 intAddr;                 //接口板地址
    quint8 cameraAddr[MAX_VIEW];    //相机板地址
    quint8 vavleAddr[MAX_VAVLE];    //喷阀板地址
    quint8 lampAddr;                //灯光板地址
    quint8 ctrlAddr;                //控制板地址

    bool bIntIsFactory;             //表示当前接口板是否在工厂模式
    bool bCameraIsFactory[MAX_VIEW];           //表示当前相机板是否在工厂模式
    bool bVavleIsFactory[MAX_VAVLE];           //表示当前喷阀板是否在工厂模式
    bool bCtrlIsFactory;           //表示控制板是否在工厂模式
    bool bLampIsFactory;           //表示灯光板是否在工厂模式
    char packetBuf[PACKET_SIZE];    //保存串口读取到的数据
    uchar fileBuf[PACKET_COUNT][FILE_PACKET_SIZE];  //保存升级文件中的数据
    quint8 curUpdateStep;           //记录当前升级的步骤，0表示升级未开始或已成功
    quint8 curLevel;                //记录当前升级的层数
    quint8 curView;                 //记录当前升级的视编号
    bool bTransByUSB;               //表示是否使用USB下传升级文件

    /* 挂载界面的控件 */
    QWidget *mountUdiskWidget;
    QLabel *mountUdiskLabel;
    QLabel *warningLabel;
    myPushButton *mountUdiskBackBtn;
    myPushButton *mountUdiskGoBtn;

    /* 升级文件列表界面的控件 */
    QWidget *fileListWidget;
    QListWidget *fileList;
    myPushButton *lastBtn;
    myPushButton *nextBtn;
    QStringList updateFileList;
    QListWidgetItem *fileItem[MAX_FILE_NUM];
    myPushButton *transModeBtn;

    /* 提示页面的控件 */
    QWidget *noticeWidget;
    QLabel *noticeLabel;
    myPushButton *noticeLastBtn;
    myPushButton *noticeNextBtn;

    /* 板级升级界面的控件 */
    QWidget *boardUpdateWidget;
    myMutex *userModeGroup;
    myMutex *layerGroup;
    myMutex *layerMaizeAddGroup;
    QGroupBox *boardNumGroup;
    QGroupBox *messagesGroup;
    myPushButton *boardNumPlusBtn;
    myPushButton *boardNumMinusBtn;
    myPushButton *boardUpdateBackBtn;
    myPushButton *boardUpdateGoBtn;
    myPushButton *boardUpdateWholeBtn;
    myPushButton *boardUpdateRetryBtn;
    QLabel *updateMesLabels[MES_NUM];
    QLabel *updateStatusLabels[MES_NUM];
    QLCDNumber *boardUpdateLcdNum;

    /* 整机升级界面的控件 */
    QWidget *updateWholeWidget;
    myPushButton *updateWholeBackBtn;
    myPushButton *updateWholeReturnBtn;
    QLabel *boardNumLabels[MAX_VIEW][MAX_VIEW_UNIT];
    QLabel *statusLabels[MAX_VIEW][MAX_VIEW_UNIT];
};

#endif // FPGA_H
