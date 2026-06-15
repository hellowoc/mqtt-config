#ifndef ONEKEYUPDATEFPGAWIDGET_H
#define ONEKEYUPDATEFPGAWIDGET_H

#include "basewidget.h"
#include "logger.h"
#include <QDir>
#include <QStringList>

#define time_switchMode	6000	//等待切换到工厂模式的时间（ms）

namespace Ui {
class OneKeyUpdateFpgaWidget;
}

class VersionInfo{
public:
    QString strName;
    int state;
    QString value;
    QString strMode;
    QString strSvn;
    VersionInfo(QString str1,int stateint,QString str3,QString str4,QString str5){
        strName = str1;
        state = stateint;
        value = str3;
        strMode = str4;
        strSvn = str5;
    }
};

enum UpdateTimerEnum
{
    timer1_ask_interface_version = 1,//查询接口板版本信息
    timer1_ask_other_version = 2,//查询其它板版本信息
    timer1_tell_upgrade = 3,//通知升级
    timer1_send_file = 4,//发送升级文件
    timer1_ask_interface_version_after_upgrade = 5,//查询接口板版本信息（升级后）
    timer1_ask_other_version_after_upgrade = 6,//查询其它板版本信息（升级后）
    timer1_wait_30secs = 7,//等待30秒
    timer1_ask_interface_version_after_others_upgrade = 8,//查询接口板版本信息（在其它板子升级后）
    timer1_interface_wait_toFactory,//接口板等待切换工厂模式
    timer1_others_wait_toFactory,//其它板子等待切换工厂模式
    timer1_interface_wait_toUser,//升级后接口板切换回原有模式
    timer1_ask_front_master_version,    //查询前主相机板
    timer1_ask_back_master_version,    //查询后主相机板
    timer1_ask_front_slave_version,    //查询前辅相机板
    timer1_ask_back_slave_version,    //查询后辅相机板
    timer1_ask_valve_version,          //查询阀板
    timer1_ask_control_version,          //查询控制板
    timer1_ask_lamp_version,          //查询灯板
    timer1_ask_zdjc_version,          //查询震动检测板
    timer1_ask_dljc_version,          //查询电流板
    timer1_goto_next_board,             // 跳转到下一个类别板子升级
    timer1_stop_update,                 // 终止升级
    timer1_final_interface,              // 最后升级接口板
};

enum PCBType
{
    PCB_interface = 0,
    PCB_front_master,
    PCB_back_master,
    PCB_front_slave,
    PCB_back_slave,
    PCB_valve,
    PCB_control,
    PCB_lamp,
    PCB_control_zd,
    PCB_control_dl
};

class OneKeyUpdateFpgaWidget : public basewidget
{
    Q_OBJECT

public:
    explicit OneKeyUpdateFpgaWidget(QWidget *parent = 0);
    ~OneKeyUpdateFpgaWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);

    void updateCurVersion();
    void updateFurVersion();
    void getZipFilePath();
    QString getFurVersion(QString fileName,int boardType);
    bool StringToHexGroup( char *OutHexBuffer, char *InStrBuffer, unsigned int strLength);
    int getUpdateFileSize();

protected:
    void retranslateUiWidget();
    void timerEvent(QTimerEvent *event);

private:
    Ui::OneKeyUpdateFpgaWidget *ui;
    QSignalMapper *pUpdateBoardEnableSigMapper;
    QCheckBox *pUpdateBoardEnable[MAX_BOARD_TYPE];
    QLabel *pcurVersion[MAX_BOARD_TYPE];
    QLabel *pupdateVersion[MAX_BOARD_TYPE];
    QList<VersionInfo> m_verinfoVec;
    QList<QString> m_furVersionVec;
    QList<QString> m_updateVersionVec;
    QString updateFileDir;
    const QString specificFolder = "update_packet";
    int m_count,finalInterface;
    int id,mCurBoardType;
    int mUpdateBoardEnable[10];     //　板级升级使能
    uchar m_boardAddr;
    int   m_nBoardFactoryOk[MAX_BOARD_TYPE][MAX_VIEW_UNIT]; //切换工厂模式成功
    int   m_nBoardUpdateOk[MAX_BOARD_TYPE][MAX_VIEW_UNIT];  //升级后切换用户模式成功
    int m_versionToUpgrade[MAX_BOARD_TYPE][5];

    void appendUpdateResult(QString str,int state);
    bool boardSwitchFactory(int boardIndex,int chainIndex,int chuteIndex);
    bool boardUpdateSuccess(int boardIndex,int chainIndex,int chuteIndex,uchar versionToUpdate);

public slots:
    void updateBoardEnable(int index);
private slots:
    void on_tarUpdateFileBtn_clicked();
    void on_checkFileBtn_clicked();
    void on_startBtn_clicked();
    void on_stopBtn_clicked();
};

#endif // ONEKEYUPDATEFPGAWIDGET_H
