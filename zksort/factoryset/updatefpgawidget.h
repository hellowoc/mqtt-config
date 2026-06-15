#ifndef UPDATEFPGAWIDGET_H
#define UPDATEFPGAWIDGET_H

#include "basewidget.h"
#include "logger.h"
#define time_switchMode	6000	//等待切换到工厂模式的时间（ms）
enum TimerEnum
{
    timer_ask_interface_version = 1,//查询接口板版本信息
    timer_ask_other_version = 2,//查询其它板版本信息
    timer_tell_upgrade = 3,//通知升级
    timer_send_file = 4,//发送升级文件
    timer_ask_interface_version_after_upgrade = 5,//查询接口板版本信息（升级后）
    timer_ask_other_version_after_upgrade = 6,//查询其它板版本信息（升级后）
    timer_wait_30secs = 7,//等待30秒
    timer_ask_interface_version_after_others_upgrade = 8,//查询接口板版本信息（在其它板子升级后）
    timer_interface_wait_toFactory,//接口板等待切换工厂模式
    timer_others_wait_toFactory,//其它板子等待切换工厂模式
    timer_interface_wait_toUser,//升级后接口板切换回原有模式
};


namespace Ui {
class UpdateFpgaWidget;
}

class UpdateFpgaWidget : public basewidget
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

public:
    explicit UpdateFpgaWidget(QWidget *parent = 0);
    ~UpdateFpgaWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
    unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT,unsigned short CRC_Temp);
    bool StringToHexGroup( char *OutHexBuffer, char *InStrBuffer, unsigned int strLength);
protected:
    void retranslateUiWidget();
    void timerEvent(QTimerEvent *event);
private slots:
    void on_comboBox_board_activated(int index);

    void on_comboBox_chute_activated(int index);

    void on_comboBox_chain_activated(int index);

    void on_comboBox_mode_activated(int index);

    void on_startUpgradeBtn_clicked();

    void on_checkFileBtn_clicked();

//    void on_comboBox_board_currentIndexChanged(int index);

//    void on_comboBox_chain_currentIndexChanged(int index);

//    void on_comboBox_chute_currentIndexChanged(int index);

    void on_OneKeyUpdateBtn_clicked();

    void on_BackBurnBtn_clicked();

    void on_pushButton_clicked();
private:
    Ui::UpdateFpgaWidget *ui;
    void updateAll();
    void updateElecBoardCombobox();
    void updateChuteCombobox();
    void updateModeCombobox();
    void updateChainCombobox();
    void appendUpdateResult(QString str,bool ok = true);
//    int timer_ask_interface_version;//查询接口板版本信息
//    int timer_ask_other_version;//查询其它板版本信息
//    int timer_tell_upgrade;//通知升级
//    int timer_send_file;//发送升级文件
//    int timer_ask_interface_version_after_upgrade;//查询接口板版本信息（升级后）
//    int timer_ask_other_version_after_upgrade;//查询其它板版本信息（升级后）
//    int timer_wait_30secs;//等待30秒
//    int timer_ask_interface_version_after_others_upgrade;//查询接口板版本信息（在其它板子升级后）
//    int timer_interface_wait_toFactory;//接口板等待切换工厂模式
//    int timer_others_wait_toFactory;//其它板子等待切换工厂模式
//    int timer_interface_wait_toUser;//升级后接口板切换回原有模式


    bool boardUpdateSuccess(int boardIndex,int chainIndex,int chuteIndex,uchar versionToUpdate);

    bool boardSwitchFactory(int boardIndex,int chainIndex,int chuteIndex);
    int getBoardAddr(int boardIndex,int chainIndex);
    void getUpdateFileInfo();

    bool checkUpdateFile();     //检查升级文件是否匹配电路板
    QVector<QStringList> getBackBurnFileName();   // 读取文件路径
    void sendUpdateFile(QString fileName);        // 发送升级文件
    QVector<QString> flattenQVector(const QVector<QStringList> &nestedVector); //将文件名读到ｑｖｃｅｔｏｒ中
    bool queryBoardVersion(QString fileName);  // 查询反烧工厂程序版本
    QString extractVersion(const QString &fileName);     // 解析出版本号
    bool interfaceNeedChgToUser; // 仅在升级失败后将接口板切回用户模式
    uchar m_BoardAddr[10];
//    uchar m_CameraBoardAddr[MAX_VIEW];
//    uchar m_vavleBoardAddr[MAX_VAVLE];
    int   m_nBoardFactoryOk[MAX_VIEW][MAX_VIEW_UNIT]; //切换工厂模式成功
    int   m_nBoardUpdateOk[MAX_VIEW][MAX_VIEW_UNIT];  //升级后切换用户模式成功
    int m_nBoardTypeIndex;
    int m_nChainIndex;
    int m_nChuteIndex;
    int m_nMirrorIndex;
    int m_view;
    QStringList filelist;
    int m_boardAddr;
    int m_versionToUpgrade[5];
    QString fileName;
    int m_count;
    QStringList viewList;
    QStringList chainList;
    QMap<int,QString> viewMap;
    int id;
    int isBackBurn;
    int m_needFileSize,m_needDelay;
};

#endif // UPDATEFPGAWIDGET_H
