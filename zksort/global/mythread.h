#ifndef MYTHREAD_H
#define MYTHREAD_H

#include "myqextserialport.h"
#include "myhttpfileclient.h"
#include "myautofeed.h"
#include "data/mysqlite.h"

//系统状态更新线程，即在系统运行记时，更新系统时间，自动清灰处理
class updateStatusThread : public QThread
{
    Q_OBJECT

    Q_CLASSINFO("author","Wang")
    Q_CLASSINFO("company","UPC")
    Q_CLASSINFO("version","3.0.1")

public:
   updateStatusThread();
   ~updateStatusThread(){};

signals:
   void sWipeStartSig();
   void sLostFpgaPowerOffSig();
   void sUpdateSystemTimeSig();
   void sDccryptWarningSig();
   void sDccryptCodeWriteSig();
   void s30DayDccryptTimeWriteSig();
   void sAlarmTurnOffSig();
   void sAlarmTurnOnSig();

   void sAlarmColorVoiceOnSig();
   void sAlarmColorVoiceOffSig();

   void sStartGetStaticsSig(int groupIndex);

   void setSignal();
   void sUpdateRealtimeThroughout();

   void sGetIpcHeart();
   void sGetIpcHeartSingle(int ipcIndex);
   void sGetIpcBadMaterial();
   void sInfoStartValve();
   void sGetIpcRunTestInfo();
   void gUpdateIpcRunTestInfo();

   void sInfoIpcStartFilterCottonClean(); //工控机散热风机过滤棉清理提醒
   void sUpdateIpcInfo();
   void sUpdateCornChuteState();
   void sCheckCurFeederLevelSig();
   void sRefreshBalFeedSig();

public slots:
   void systemClockRun();
   void getStaticsSlt(int groupIndex);
   void setSlots();
   void getIpcHeartSlt();
   void getIpcHeartSingleSlt(int groupIndex);
   void getIpcBadMaterialSlt();
   void getChannelFeedIsOn(quint8);
   void getIpcRunTestInfo();
   void checkCurFeederLevelSlt();

protected:
    void run(void);

private:
    char cmdData[SEND_PACKET_DATA_SIZE];
    void checkAlarmStatus(void);     // 检查报警状态
    void getIpcHeart();
    int m_alarminfo[MAX_ALARM],m_alarm_IPC;
    int backlightOffCount;

    int curIpcIndex,lastIpcIndex;
    int m_alarmFeedLevel[CTRL_ALARM_MAX_FEED_SENSOR]; //用于检测料位传感器信号变化
    int alarmcount;
};

extern updateStatusThread *myUpdateStatusThread;

#endif
