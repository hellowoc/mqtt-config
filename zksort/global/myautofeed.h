#ifndef MYAUTOFEED_H
#define MYAUTOFEED_H

#include "constant.h"
#include "global.h"



typedef struct stu_feed_auto_config_normal {
    int curCountBalStateChg[6];
    int balanceStateChange;

//    int isInBalancing;
    int feedEmptyCount;   //料空持续时间，到达一定时间后，再将二三四次的流量设置到0最低
    int lastBalanceState;
    int levelMoreToNormalCount;    //料多->料均衡的持续时间
    int feedFullCount;    //料满转正常的持续时间
    int flowOfChute[MAX_FEEDER];//
    bool moreToBal2ndEnable; //从料多切换到均衡，满足超时，二次允许开始均衡的流量判断

    bool autoFeedEnabled;             //使能柔性供料
    bool isRadarStateAlarm;           //当前是否雷达料位报警
    int feedByRadar;   //根据雷达料位，对应的动态料位
    int curBalanceStateByLevel;           // 根据当前雷达料位，获取的实时均衡模式
    int lastBalanceStateByLevel;          // 上次的均衡模式
    int feederRealSend[MAX_FEEDER];  //实际发送的流量值
    int feederLastRealSend[MAX_FEEDER];  //上次实际发送的流量值

    bool hasAutoFeedTargetCal;    //是否进行过均衡目标计算
    int groupFlowTarget[MAX_GROUP_IDENTIFY];
}stru_feed_auto_config_normal;

typedef struct stu_feed_auto_config_mode1 {
    int curCountBalStateChg[6];
    int curCountBalChgClock[6];

    int balanceStateChange;

//    int isInBalancing;
    int feedEmptyCount;   //料空持续时间，到达一定时间后，再将二三四次的流量设置到0最低
    int lastBalanceState;
    int levelMoreToNormalCount;    //料多->料均衡的持续时间
    int feedFullCount;    //料满转正常的持续时间
    int flowOfChute[MAX_FEEDER];//
    bool moreToBal2ndEnable; //从料多切换到均衡，满足超时，二次允许开始均衡的流量判断

    bool autoFeedEnabled;             //使能柔性供料
    bool isRadarStateAlarm;           //当前是否雷达料位报警
    int feedByRadar;   //根据雷达料位，对应的动态料位
    int curBalanceStateByLevel;           // 根据当前雷达料位，获取的实时均衡模式
    int lastBalanceStateByLevel;          // 上次的均衡模式
    int feederRealSend[MAX_FEEDER];  //实际发送的流量值
    int feederLastRealSend[MAX_FEEDER];  //上次实际发送的流量值

    bool hasAutoFeedTargetCal;    //是否进行过均衡目标计算
    int groupFlowTarget[MAX_GROUP_IDENTIFY];

    int mode1FeedStateChangeToBal;   // 1-从料满->均衡， 2- 从料空->均衡， 0-不变//模式1， 是否是从料满/料空切换到均衡，给二三四一个延迟
    int mode1LevelFullToBalCount;
    int mode1BalancePart;       //模式1， 均衡所在阶段
    int mode1FeedBalPartScale[AUTO_FEED_TARGET_PART];   //模式1，均衡阶段，分段的各阶段门限
    int mode1FeedTargetPart[AUTO_FEED_TARGET_PART];     //模式1，均衡阶段，分段的各阶段目标值
    int mode1CountBalPartChg[AUTO_FEED_TARGET_PART];                        //模式1，均衡阶段，分段的各阶段状态跳变计数器
}stru_feed_auto_config_mode1;

typedef struct stu_feed_auto_config_mode2 {
    int curCountBalStateChg[6];
    int curCountBalChgClock[6];
    int balanceStateChange;

//    int isInBalancing;
    int feedEmptyCount;   //料空持续时间，到达一定时间后，再将二三四次的流量设置到0最低
    int lastBalanceState;
    int levelMoreToNormalCount;    //料多->料均衡的持续时间
    int feedFullCount;    //料满转正常的持续时间
    int flowOfChute[MAX_FEEDER];//
    bool moreToBal2ndEnable; //从料多切换到均衡，满足超时，二次允许开始均衡的流量判断

    bool autoFeedEnabled;             //使能柔性供料
    bool isRadarStateAlarm;           //当前是否雷达料位报警
    int feedByRadar;   //根据雷达料位，对应的动态料位
    int curBalanceStateByLevel;           // 根据当前雷达料位，获取的实时均衡模式
    int lastBalanceStateByLevel;          // 上次的均衡模式
    int feederRealSend[MAX_FEEDER];  //实际发送的流量值
    int feederLastRealSend[MAX_FEEDER];  //上次实际发送的流量值

    bool hasAutoFeedTargetCal;    //是否进行过均衡目标计算
    int groupFlowTarget[MAX_GROUP_IDENTIFY];
    int mode2FeedStateChangeToBal;   // 模式1， 是否是从料满/料空切换到均衡，给二三四一个延迟
    int mode2LevelFullToBalCount;
}stru_feed_auto_config_mode2;

typedef struct stu_feed_auto_config_mode_switch {
    int curCountBalStateChg[6];
    int curCountBalChgClock[6];
//    int isInBalancing;

    int levelFullToNormalCount;    //料满->料均衡的持续时间
    int flowOfChute[MAX_FEEDER];//

    bool autoFeedEnabled;             //使能柔性供料
    bool isFeedLevelSwitchAlarm;           //当前是否料位开关报警
    int curBalanceStateByLevel;           // 根据当前雷达料位，获取的实时均衡模式
    int lastBalanceStateByLevel;          // 上次的均衡模式
    int feederRealSend[MAX_FEEDER];  //实际发送的流量值
    int feederLastRealSend[MAX_FEEDER];  //上次实际发送的流量值

    bool hasAutoFeedTargetCal;    //是否进行过均衡目标计算
    int groupFlowTarget[MAX_GROUP_IDENTIFY];
    int modeSwitchFeedStateChangeToBal;   // 模式1， 是否是从料满/料空切换到均衡，给二三四一个延迟
    int modeSwitchLevelFullToBalCount;
}stru_feed_auto_config_mode_switch;

typedef struct stu_feed_auto_config_mode3 {
    int curReadIndex;
    int tmpPosiSave[2];
    int curCountBalStateChg[6];
    int curCountBalChgClock[6];
    int curBalanceStateByLevel;           // 根据当前雷达料位，获取的实时均衡模式，料空、匀料、料满
    int feedEmptyCount;   //料空持续时间，到达一定时间后，再将二三四次的流量设置到0最低
    int feedFullCount;    //料满转正常的持续时间
    int lastRadarPosi;  //上次料位
    int posiRiseCount;  // 料位增加计数
    int posiDescendCount;  // 料位增加计数
    int levelMoreCount;     //料多计数
    int balanceStateChange;
    int mode3FeedStatefullToBal;   // 料满->均衡
    int mode3FeedStateBalToEmpty;   // 均衡->料空
    int mode3LevelFullToBalCount;

    int mode3BalToEmptyAdjustCount;
    int mode3BalLowPosiCount;
}stru_feed_auto_config_mode3;

class myAutoFeedSetClass
{
//    enum{
//        MODE1_BALANCE_STAT_1=0,
//        MODE1_BALANCE_STAT_2=1,
//        MODE1_BALANCE_STAT_3=2,
//        MODE1_BALANCE_STAT_4=3
//    };
public:
    myAutoFeedSetClass();

    void enbleFreshFeedPage(bool isEnable); //刚开始打开下料时，是否复位刷新值界面
    bool getFreshFeedPage(); //刚开始打开下料时，判断是否刷新流量值界面


    /* mode 0 */
    void getCurFeedByLevel(int isAll, int index);
    void clearBalStateCount(int group);
    bool checkCurRicePoSi();
    bool autoFlowAdjust();

    /* mode 1*/
    void getCurRadarLevelMode1WhileTurnOn();
    bool mode1CheckCurRicePoSi();
    void mode1GetCurBalancePartPoSi(int group, int level);
    void mode1ClearBalStateCount(int group);
    void mode1ClearBalPartCount(int group);
    int mode1GetCurFeedByLevel(int isAll, int index);
    void mode1StateChangeToResetFeed(int index, bool isChangeToBal, bool needDelay);
    bool mode1AutoFlowAdjust();

    /* mode 2*/
    void getCurRadarLevelMode2WhileTurnOn();
    bool mode2CheckCurRicePoSi();
    void mode2GetCurBalanceTargetByPoSi(int level);
    void mode2ClearBalStateCount();
    //void mode2ClearBalStateCount(int group);
    int mode2GetCurFeedByLevel(int isAll, int index);
    void mode2StateChangeToResetFeed(int index, bool isChangeToBal, bool needDelay);
    bool mode2AutoFlowAdjust();

    /* mode sensor switch 料位开关*/
    void getCurFeedLevelModeSwitchWhileTurnOn();
    bool modeSwitchCheckCurRicePoSi();
//    void mode1GetCurBalancePartPoSi(int level);
    void modeSwitchClearBalStateCount(int group);
    void modeSwitchStateChangeToResetFeed(int index, bool isChangeToBal, bool needDelay);
//    void mode1ClearBalPartCount();
    int modeSwitchGetCurFeedByLevel(int isAll, int index);

    bool modeSwitchAutoFlowAdjust();

    /* mode 3 */
    void getCurRadarLevelMode3WhileTurnOn();
    bool mode3CheckCurRicePoSi();
    void mode3SetCurFeedValueByPoSi(int level);
    void mode3ClearBalStateCount();
    //void mode2ClearBalStateCount(int group);
    int mode3GetCurFeedByLevel(int isAll, int index);
    void mode3StateChangeToResetFeed(int index, bool isChangeToBal, bool needDelay);

    stru_feed_auto_config_normal struFeedAutoConfigNormal[MAX_GROUP_TICK];
    stru_feed_auto_config_mode1 struFeedAutoConfigMode1[MAX_GROUP_TICK];
    stru_feed_auto_config_mode2 struFeedAutoConfigMode2;
    //stru_feed_auto_config_mode2 struFeedAutoConfigMode2[MAX_GROUP_TICK];
    stru_feed_auto_config_mode_switch struFeedAutoConfigModeSwitch[MAX_GROUP_TICK];
    stru_feed_auto_config_mode3 struFeedAutoConfigMode3;

private:
//	int curCountBalStateChg[6];
//    int balanceStateChange;
    bool isInBalancing;
    int countPerMin;
    int countLimitQuick;           //滤波
    int countLimitSlow;            //滤波

    bool needFreshFeedPage;

};

extern myAutoFeedSetClass myAutoFeed;

#endif
