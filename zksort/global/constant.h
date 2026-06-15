#ifndef CONSTANT_H
#define CONSTANT_H
#include "custom.h"
/* ----------------------------------------------------------*/
/* 界面id */
#define SM_USER_WIDGET              1
#define SM_ENGINEER_WIDGET          2
#define SM_SYSSTATE_WIDGET          3
#define SM_FACTORY_SET_WIDGET       4
#define SM_LANGUAGE_WIDGET          5
#define SM_FEED_SET_WIDGET          6
#define SM_MODE_MANAGE_WIDGET       7
#define SM_MODE_SET_WIDGET          8
#define SM_SENSE_SET_WIDGET         9
#define SM_BACKGROUND_SET_WIDGET    10
#define SM_IAMGE_WIDGET             11
#define SM_MACHINE_SET_WIDGET       12
#define SM_CAMERA_SET_WIDGET        13
#define SM_GRAYSENSE_SET_WIDGET     14
#define SM_IMAGE_MANAGE_WIDGET      15
#define SM_BIG_SMALL_SENSE_SET_WIDGET 16
#define SM_CIRCLE_LONG_SENSE_SET_WIDGET 17
#define SM_LONG_SHORT_SENSE_SET_WIDGET 18
#define SM_SVM_SENSE_SET_WIDGET 19
#define SM_MAIN_PAGE_WIDGET 20
#define SM_IDENTIFY_WIDGET 21
#define SM_EJECT_WIDGET 22
#define SM_LIGHE_SET_WIDGET 23
#define SM_WIPE_SET_WIDGET  24
#define SM_VAVLE_TEST_WIDGET  25
#define SM_CAMERA_SIGNAL_WIDGET 26
#define SM_DIVIDE_PIXEL_WIDGET 27
#define SM_DIVIDE_PIXEL_SHOWALL_WIDGET 28
#define SM_IMAGE_CAPTURE_WIDGET 29
#define SM_IMAGE_ANALYSIS_WIDGET 30
#define SM_MACHINE_FUNC_WIDGET 31
#define SM_MACHINE_DCCRYPT_WIDGET 32
#define SM_SELF_CHECK_WIDGET 33
#define SM_EJECT_TIME_WIDGET 34
#define SM_SIMULATE_WIDGET 35
#define SM_FPGA_UPGRADE     36
#define SM_PIXEL_AJUST_WIDGET 37
#define SM_LAMP_BOARD_SET_WIDGET 38
#define SM_MAIN_PAGE_WIDGET_NEW 39
#define SM_COLOR_SAT_SENSE_SET_WIDGET 40
#define SM_COLOR_HUE_SENSE_SET_WIDGET 41
#define SM_COLOR_SCALE_SENSE_SET_WIDGET 42
#define SM_MILDEW_SENSE_SET_WIDGET 43
#define SM_OUTLINE_SENSE_SET_WIDGET 44
#define SM_LINE_SENSE_SET_WIDGET 45
#define SM_BUD_SENSE_SET_WIDGET 46
#define SM_ROUND_SENSE_SET_WIDGET 47
#define SM_COLOR_VALUE_SENSE_SET_WIDGET 48
#define SM_IR_BAD_POINT_WIDGET 49
#define SM_NIR_DOT_CORRECTIVE_LIST_WIDGET 50
#define SM_DISCOLOR_SENSE_SET_WIDGET     51
#define SM_AUTO_FEEDER_WIDGET         52
#define SM_AI_WIPE_SET_WIDGET         53
#define SM_CREATE_MODE_WIDGET         54
#define SM_FEEDER_SENSOR_SET_WIDGET   55
#define SM_TEMP_SENSOR_SET_WIDGET     56
#define SM_UPDATE_FPGA_WDIGET         57
#define SM_HSV_SENSE_SET_WIDGET       58
#define SM_SPL_MILDEW_SENSE_SET_WIDGET 59
#define SM_WHITE_PEANUT_SENSE_SET_WIDGET 60
#define SM_DOUBLE_LIGHE_SET_WIDGET 61
#define SM_SHAPE_SVM_SENSE_SET_WIDGET  62
#define SM_PEPPER_HANDLE_SENSE_SET_WIDGET  63
#define SM_CAMERA_COLOR_RESTORE_WIDGET     64
#define SM_IPC_SET_WIDGET  65
#define SM_IPC_CAMERA_SET_WIDGET  66
#define SM_IPC_CLASS_PARAMS_WIDGET  67
#define SM_IPC_MODEL_CHANGE_WIDGET       68
#define SM_IPC_MODEL_CLASS_INFO_WIDGET       69
#define SM_IPC_CLASS_ADV_PARAMS_WIDGET       70
#define SM_IPC_TEST_NET_WIDGET           71
#define SM_IPC_UPGRADE_MODEL_WIDGET           72
#define SM_IMAGE_DATA_CHART_WIDGET 73
#define SM_IMAEG_DATA_RGB_CHART_WIDGET 74
#define SM_IMAEG_DATA_DISCOLOR_CHART_WIDGET 75
#define SM_IPC_ALARM_CONTROL_WIDGET 76
#define SM_IPC_EJECT_WIDGET         77
#define SM_IMAGE_INFO_RGB_WIDGET    78
#define SM_UPDATE_ACS_WIDGET        79
#define SM_SUPPRESS_LARGE_SENSE_SET_WIDGET  80
#define SM_WHEAT_SPROUT_SENSE_SET_WIDGET    81
#define SM_IPC_AI_SET_WIDGET        82
#define SM_SVM_SAMPLE_WIDGET        83
#define SM_IMAGE_FAST_CAPTURE_WIDGET     84
#define SM_ONE_KEY_UPDATE_FPGA_WIDGET   85
#define SM_SELECT_UPDATE_FILE_WIDGET    86
#define SM_TEA_COLOR_SENSE_SET_WIDGET 87
#define SM_IR_BAD_POINT_WIDGET_I      88
#define SM_SVM_IMAGE_WIDGET           89
#define SM_ACCELERATION_SENSOR_SET_WIDGET 90
#define SM_VI_DETECT_BOARD_WIDGET   91
#define SM_IPC_REMOTE_DOWNLOAD_MODEL_WIDGET   92
#define SM_REMOTE_CONTROL_WIDGET 93
#define SM_PLC_CTRL_WIDGET          94
#define SM_FPGA_SET_WIDGET                95
#define SM_IPC_OUTPUT_WIDGET              96
#define SM_IPC_CLASSSET_WIDGET            97
/* ----------------------------------------------------------*/
/* 界面消息id */
#define MSG_DATA_CHANGED 0
#define WAVE_MSG  1
#define IMAGE_MSG 2
#define MSG_VERSION  3 //版本查询
#define MSG_LEVEL_CHANGE 4//等级切换
#define MSG_OFF  5//报警关闭下料
#define MSG_SYS_CHANGE       7  //分选系统开关切换
#define MSG_VID_CHANGE       8  //电流电源检测板数据更新
/* ----------------------------------------------------------*/
/* 界面语言列表 */
#define LANG_CHS         1      /*简体中文*/
#define LANG_ENG         2      /*英文*/
#define LANG_RUS         3      /*俄文*/
#define LANG_VIET        4      /*越南文*/
#define LANG_THAILAND    5      /*泰文*/
#define LANG_SPAN        6      /*西班牙文*/
#define LANG_CHT         7      /*繁体中文*/
#define LANG_KOREAN      8      /*韩语*/
#define LANG_TURKEY      9      /*土耳其*/
#define LANG_CAMBODIA    10      /*柬埔寨*/
#define LANG_POLAND      11     /*波兰*/
#define LANG_BURMA       12       /*缅甸*/
#define LANG_PERSIA      13       /*波斯*/
#define LANG_UKRAINE     14       /*乌克兰*/
#define LANG_ITALY       15       /*意大利*/
#define LANG_GREEK       16       /*希腊*/
#define LANG_UIGHUR      17       /*维吾尔*/
#define LANG_ARABIC      18       /*阿拉伯*/
#define LANG_INDONESIA   19       /*印尼语*/
#define LANG_BULGARIAN   20       /*保加利亚*/
#define LANG_FRENCH      21       /*法语*/
#define LANG_SWEDISH     22       /*瑞典*/
#define LANG_JAPAN       23       /*日本*/
#define LANG_PORTUGUESE  24       /*葡萄牙语*/
#define LANG_HINDI       25       /*印地语*/
#define LANG_GERMAN      26       /*德语*/


//柔性供料－－－－－－－－－－－－－－－－－－－－－－－－－－
/* 均衡供料模式 */
#define AUTO_FEED_MODE_0        0       //柔性，无人值守
#define AUTO_FEED_MODE_1        1       //柔性+均衡，均衡目标分段
#define AUTO_FEED_MODE_2        2       //柔性+均衡，均衡目标连续
#define AUTO_FEED_MODE_3        3       //柔性+不均衡，料位跟随变化，区域稳定

#define STAT_LEVEL_EMPTY        0       //料空
#define STAT_LEVEL_BALANCE      1       //料匀
#define STAT_LEVEL_MORE         2       //料多
#define STAT_LEVEL_FULL         3       //料满

// mode 1
#define STAT_LEVEL_EMPTY_MODE1          0       //料空
#define STAT_LEVEL_BALANCE_MODE1        1       //料匀
#define STAT_LEVEL_PRE_FULL_MODE1       2       //料预满
#define STAT_LEVEL_FULL_MODE1           3       //料实满

#define MODE1_BALANCE_STAT_1    0
#define MODE1_BALANCE_STAT_2    1
#define MODE1_BALANCE_STAT_3    2
#define MODE1_BALANCE_STAT_4    3

// mode 2
#define STAT_LEVEL_EMPTY_MODE2          0       //料空
#define STAT_LEVEL_BALANCE_MODE2        1       //料匀
#define STAT_LEVEL_MORE_MODE2           2       //料多
#define STAT_LEVEL_FULL_MODE2           3       //料满

#define AUTO_FEED_TARGET_PART           4       //模式1，分段均衡目标值，最大分段数量

// mode switch
#define STAT_LEVEL_EMPTY_MODE_SWITCH    0       //料空
#define STAT_LEVEL_BALANCE_MODE_SWITCH       1       //料匀
#define STAT_LEVEL_PRE_FULL_MODE_SWITCH      2       //料预满
#define STAT_LEVEL_FULL_MODE_SWITCH          3       //料满

// mode 3
#define STAT_LEVEL_EMPTY_MODE3          0       //料空
#define STAT_LEVEL_BALANCE_MODE3        1       //料匀
#define STAT_LEVEL_FULL_MODE3           2       //料满


/* ----------------------------------------------------------*/
/* 机型列表 */
/* 通用机型列表 */
#define PRODUCT_VENDOR_ZK       0       // 品牌ZK
#define PRODUCT_VENDOR_LV       1       // 品牌LV

#define PRODUCT_DEFAULT_VENDOR       PRODUCT_VENDOR_ZK       // 默认品牌

#define MACHINE_TYPE_VERTICAL       0       // 立式机
#define MACHINE_TYPE_BELT           1       // 履带机
#define MACHINE_TYPE_BELT_DOUBLE    2       // 双层履带机

#define MACHINE_VERTICAL_DM_RG     0       // 立式大米通用色选机， RG系列
#define MACHINE_VERTICAL_DM_ZK     1        // 立式大米通用色选机， ZK系列
#define MACHINE_VERTICAL_ZL_CS     2       // 立式杂粮色选机 CS
#define MACHINE_VERTICAL_ZL_CV     3       // 立式杂粮色选机 CV
#define MACHINE_LD_ZL_GP           4       // 单层履带式杂粮通用色选机
#define MACHINE_LD2_ZL_GP          5       // 双层履带式杂粮通用色选机

/* 是否带分割的种类 */
#define MACHINE_SPLICE_TYPE_NULL        0   //未分割机器，
#define MACHINE_SPLICE_TYPE_M80         1   //M80合板带分割，
#define MACHINE_SPLICE_TYPE_BIG         2   //正常带分割的大机器

/* 杂粮定制机型*/
#define MACHINE_SERIAL_GENERAL              1      //品牌：通用机型

/* ---------------------------------------------------- */
/* 机型设置相关-极值 */

#define UNIT_NULL               -1      // 空

#define MAX_LAYER               1          // 最大层数 或最大接口板数量
#define MAX_VIEW                8          // 单层最多视数（立式机常规最大4视，双层履带机最大8视）
#define MAX_VIEW_CHIAN          4          // 单层最大视链数（固定的）
#define MAX_VIEW_UNIT           16         // 单视最大彩色相机板数量

#define MAX_VIEW_UNIT_USE       12         // 程序界面单视支持最大相机板数量

#define MAX_VAVLE               6          // 单层最大阀链路数,含主、辅喷阀
#define MAX_VAVLE_BOARD         24         // 单类型最大喷阀板数量
#define MAX_VAVLE_TYPE          2          // 单个相机对应的最大阀的种类，目前只有喷阀1和喷阀2

#define MAX_GROUP_IDENTIFY      5          // 最大识别组数量
#define MAX_GROUP_TICK          5          // 最大剔除组数量
#define MAX_GROUP_YANSI         10    //烟叶机最大分组数
//#define MAX_CTRL_CHAIN                  2    // 最大控制板链数
#define MAX_CTRL_BOARD_PER_INTERFACE    2    // 每个接口板的最多控制板数量
#define MAX_FEEDER                      16         //单个控制板最多外接振动器数量

//#define MAX_LAMP_CHAIN                  2
#define MAX_LAMP_BOARD_PER_INTERFACE    8          // 单个接口板灯链上的最多灯光板
#define MAX_LAMP_PER_BOARD              8          // 单个灯光板，能连接的最大灯数
#define MAX_ACCELERATION_BOARD          2          // 最大加速度传感器检测板
#define MAX_HALOGEN_BOARD               6          // 最大电流检测板

#define LAMP_BOARD_POSI_NULL            0          // 没有灯光控制板
#define LAMP_BOARD_POSI_FRONT           1          // 灯光板在前/上分选室
#define LAMP_BOARD_POSI_REAR            2          // 灯光板在后/下分选室
#define MAX_ACCELERATION                12          // 最大加速度传感器个数
#define MAX_VI_BOARD                    32          // 最大电压电流检测板个数
#define MAX_VI_BOARD_INFO               4           // 电压电流检测板参数个数
#define MAX_EJECTOR                 96         //单喷阀 单通道对应的最大喷嘴数量
//#define MAX_LAMP        6               // 灯最大数量
#define MAX_CALIBRATE_SETS              16 //最大校正参数套数

/*  电路板型号定义 */
// 1. 通用类型
#define BOARD_TYPE_ALL_IN_CHAIN         0xff        // 当前链上所有电路板
#define BOARD_TYPE_ALL_BOARD            0x00        // 所有型号的电路板
#define BOARD_TYPE_ALL_CAMERA           0x09        // 所有相机板
#define BOARD_TYPE_ALL_VAVLE            0x0a        // 除接口板外的所有电路板
//#define BOARD_TYPE_ALL_INTERFACE        0x00        // 当前链上所有电路板    ，ARM板上位机自定义
#define BOARD_TYPE_ALL_BUT_INTERFACE    10

// 2. 接口板
#define BOARD_TYPE_INTERFACE            0x01        // 接口板

#define BOARD_TYPE_ALL_IN_CHAIN_SPLIE_LEFT     0x40-1
#define BOARD_TYPE_ALL_IN_CHAIN_SPLIE_RIGHT    0x80-1

#define USER_MODE                       1      //用户模式
#define FACTORY_MODE                    0      //工厂模式

// 3. 相机板
#define BOARD_CAMERA_1_FRONT_MAIN     0x02    //单层机 前主相机板
#define BOARD_CAMERA_1_FRONT_VICE     0x03    //单层机 前辅相机板
#define BOARD_CAMERA_1_REAR_MAIN      0x04    //单层机 后主相机板
#define BOARD_CAMERA_1_REAR_VICE      0x05    //单层机 后辅相机板

//4. 喷阀板
#define BOARD_VAVLE_LAYER_1           0x06    //单层机喷阀驱动板

//5. 外围控制板
#define BOARD_TYPE_CONTROL            0x07    //外围控制板型号

//6. 恒流源板
#define BOARD_TYPE_LAMP               0x08    //恒流源板型号
//5. 外围控制板
#define BOARD_TYPE_CONTROL_ZD         0x0B    //外围控制板型号

//5. 外围控制板
#define BOARD_TYPE_CONTROL_DL         0x0C    //外围控制板型号

//6. 恒流源板
#define BOARD_TYPE_LAMP               0x08    //恒流源板型号
#define MAX_BOARD_TYPE              10          // 最大板子类别

#define BOARD_VAVLE_LAYER_2      0x1a    //双层机第二层喷阀驱动板
#define BOARD_VAVLE_LAYER_3      0x1b    //三层机第三层喷阀驱动板
#define BOARD_VAVLE_LAYER_4      0x1c    //四层机第四层喷阀驱动板
#define BOARD_VAVLE_LAYER_5      0x1d    //五层机第五层喷阀驱动板
#define BOARD_VAVLE_LAYER_6      0x1e    //六层机第六层喷阀驱动板
#define BOARD_VAVLE_LAYER_7      0x1f    //七层机第七层喷阀驱动板


#define BOARD_CAMERA_2_FRONT_MAIN 0x13
#define BOARD_CAMERA_2_REAR_MAIN  0x14    //双层机 第二层后主相机板
#define BOARD_CAMERA_3_FRONT_MAIN 0x15
#define BOARD_CAMERA_4_FRONT_MAIN 0x16
#define BOARD_CAMERA_5_FRONT_MAIN 0x17
#define BOARD_CAMERA_6_FRONT_MAIN 0x18
#define BOARD_CAMERA_7_FRONT_MAIN 0x19

#define BOARD_CAMERA_2_FRONT_VICE 0x2f   //双层机 第二层前辅相机板
#define BOARD_CAMERA_2_REAR_VICE  0x30


/*  上位机软件内部定义 */
/* 1.立式机  */
#define LCD_VERI_MAIN_VIEW_FRONT              0        //前视/前主
#define LCD_VERI_MAIN_VIEW_REAR               1        //后视/后主
#define LCD_VERI_VICE_VIEW_FRONT              2        //前辅
#define LCD_VERI_VICE_VIEW_REAR               3        //后辅
#define LCD_VERI_MAIN_VIEW_FRONT_2              4        //L2前视/前主
#define LCD_VERI_MAIN_VIEW_REAR_2               5        //L2后视/后主
#define LCD_VERI_VICE_VIEW_FRONT_2              6        //L2前辅
#define LCD_VERI_VICE_VIEW_REAR_2               7        //L2后辅

/* 2.单层履带机  */
#define LCD_LD_MAIN_VIEW_UP                0        //单层上视/上主
#define LCD_LD_MAIN_VIEW_LOW               1        //下视/下主
#define LCD_LD_VICE_VIEW_UP                2        //上辅
#define LCD_LD_VICE_VIEW_LOW               3        //下辅

/* 3.双层履带机  */
#define LCD_LD2_UPLAYER_VIEW_UP               0        //上层上视
#define LCD_LD2_UPLAYER_VIEW_LOW              1        //上层下视
#define LCD_LD2_LOWLAYER_VIEW_UP              2        //下层上视
#define LCD_LD2_LOWLAYER_VIEW_LOW             3        //下层下视

/* 4. 背靠背大米机 暂时不用 */
#define LCD_1_MAIN_VIEW_FRONT              0        //1机，前视/前主
#define LCD_1_MAIN_VIEW_REAR               1        //1机，后视/后主
#define LCD_1_VICE_VIEW_FRONT              2        //1机，前辅
#define LCD_1_VICE_VIEW_REAR               3        //1机，后辅
#define LCD_2_MAIN_VIEW_FRONT              4        //2机，前视/前主
#define LCD_2_MAIN_VIEW_REAR               5        //2机，后视/后主
#define LCD_2_VICE_VIEW_FRONT              6        //2机，前辅
#define LCD_2_VICE_VIEW_REAR               7        //2机，后辅

#define FIRST_FRONT             0       // 一次前视
#define FIRST_REAR              1       // 一次后视
#define SECOND_FRONT            2       // 二次前视
#define SECOND_REAR             3       // 二次后视
#define THIRD_FRONT             4       // 三次前视
#define THIRD_REAR              5       // 三次后视
#define FOURTH_FRONT            6       // 四次前视
#define FOURTH_REAR             7       // 四次后视
#define FIVETH_FRONT            8       // 五次前视
#define FIVETH_REAR             9       // 五次后视
#define SIXTH_FRONT             10      // 六次前视
#define SIXTH_REAR              11      // 六次后视

#define FIRST_MAIN_FRONT        20       // 一次前主
#define SECOND_MAIN_FRONT       21       // 二次前主
#define THIRD_MAIN_FRONT        22       // 三次前主
#define FOURTH_MAIN_FRONT       23       // 四次前主
#define FIVETH_MAIN_FRONT       24       // 五次前主
#define SIXTH_MAIN_FRONT        25       // 六次前主

#define FIRST_VICE_FRONT        30       // 一次后主
#define SECOND_VICE_FRONT       31       // 二次后主
#define THIRD_VICE_FRONT        32       // 三次后主
#define FOURTH_VICE_FRONT       33       // 四次后主
#define FIVETH_VICE_FRONT       34       // 五次后主
#define SIXTH_VICE_FRONT        35       // 六次后主

#define FIRST_MAIN_REAR         40       // 一次前辅
#define SECOND_MAIN_REAR        41       // 二次前辅
#define THIRD_MAIN_REAR         42       // 三次前辅
#define FOURTH_MAIN_REAR        43       // 四次前辅
#define FIVETH_MAIN_REAR        44       // 五次前辅
#define SIXTH_MAIN_REAR         45       // 六次前辅


#define FIRST_VICE_REAR         50       // 一次后辅
#define SECOND_VICE_REAR        51       // 二次后辅
#define THIRD_VICE_REAR         52       // 三次后辅
#define FOURTH_VICE_REAR        53       // 四次后辅
#define FIVETH_VICE_REAR        54       // 五次后辅
#define SIXTH_VICE_REAR         55       // 六次后辅


//#define FIRST_SORT              60       // 一次
//#define SECOND_SORT             61       // 二次
//#define THIRD_SORT              62       // 三次
//#define FOURTH_SORT             63       // 四次
//#define FIFTH_SORT              64       // 五次
//#define SIXTH_SORT              65       // 六次

// 背靠背大米机
#define B2B_MAIN_FRONT_VIEW          60      // 1机前主
#define B2B_MAIN_REAR_VIEW           61      // 1机后主
#define B2B_VICE_FRONT_VIEW          62      // 1机前辅
#define B2B_VICE_REAR_VIEW           33      // 1机后辅
#define B2B_MAIN_FRONT_VIEW          60      // 2机前主
#define B2B_MAIN_REAR_VIEW           61      // 2机后主
#define B2B_VICE_FRONT_VIEW          62      // 2机前辅
#define B2B_VICE_REAR_VIEW           33      // 2机后辅

// 立式机
#define FRONT_VIEW              70        //前视
#define REAR_VIEW               71        //后视
#define MAIN_FRONT              72        //前主
#define MAIN_REAR               73        //后主
#define VICE_FRONT              74        //前辅
#define VICE_REAR               75        //后辅
#define MAIN_FRONT_2            76        //L2前主
#define MAIN_REAR_2             77        //L2后主
#define VICE_FRONT_2            78        //L2前辅
#define VICE_REAR_2             79        //L2后辅

//履带机
#define UP_LAYER                80      // 上层
#define LOW_LAYER               81      // 下层
#define UP_VIEW                 82      // 上视
#define LOW_VIEW                83      // 下视

#define UP_MAIN_VIEW            90      // 上层主视
#define LOW_MAIN_VIEW           91      // 下层主视
#define UP_VICE_VIEW            92      // 上层辅视
#define LOW_VICE_VIEW           93      // 下层辅视



//#define MAIN_VAVLE_TYPE1        100     // 主喷阀吹
//#define MAIN_VAVLE_TYPE2        101     // 辅喷阀吹

#define MAIN_LAYER_VAVLE        100     // 喷阀板
#define FIRST_LAYER_VAVLE       101     // 上喷阀板
#define SECOND_LAYER_VAVLE      102     // 下喷阀板

#define FIRST_SORT              110       // 一次
#define SECOND_SORT             111       // 二次
#define THIRD_SORT              112       // 三次
#define FOURTH_SORT             113       // 四次
#define FIFTH_SORT              114       // 五次
#define SIXTH_SORT              115       // 六次


#define FIRST_MAIN_VAVLE              120       // 一次主阀
#define SECOND_MAIN_VAVLE             121       // 二次主阀
#define THIRD_MAIN_VAVLE              122       // 三次主阀
#define FOURTH_MAIN_VAVLE             123       // 四次主阀
#define FIFTH_MAIN_VAVLE              124       // 五次主阀
#define SIXTH_MAIN_VAVLE              125       // 六次主阀

#define FIRST_REAR_VAVLE              130       // 一次辅阀
#define SECOND_REAR_VAVLE             131       // 二次辅阀
#define THIRD_REAR_VAVLE              132       // 三次辅阀
#define FOURTH_REAR_VAVLE             133       // 四次辅阀
#define FIFTH_REAR_VAVLE              134       // 五次辅阀
#define SIXTH_REAR_VAVLE              135       // 六次辅阀


#define CONTROL_BOARD1          0       // 控制板1
#define CONTROL_BOARD2          1       // 控制板2

#define INTERFACE_BOARD1        0       // 接口板1
#define INTERFACE_BOARD2        1       // 接口板2

/* ---------------------------------------------------- */
/* 方案算法相关 */
#define MAX_PROFILE             150             // 最大方案数量
#define PROFILE_PRE_FIXED       30              // 预制方案数量
#define ARITHMETIC_TOTAL        46              // 最大算法总数
#define MAT_MAX                 4               // 智能模式中，最大物料数量
#define IMAGE_MAX               18              // 智能模式中，最大的物料图片数

#define IMG_LEN                 1024*3		//智能模式中，保存图片的长度
#define CAP_WID                 1024		//图片宽度(像素点)
#define CAP_HEI			500		//图片高度(像素点)
#define CAP_INF			512		//红外图片宽度(像素点)


#define AUTHENTICATION_LEVEL_OPERATOR 0
#define AUTHENTICATION_LEVEL_ENGINEER 1
#define AUTHENTICATION_LEVEL_FACTORY  2

#define MAX_PARENT_PREFIXED_PROFILE_COUNT        32
#define MAX_SON_PREFIXED_PROFILE_COUNT           32

/* 灯光名称索引 起始地址  */
/* 前分选室：物料灯1...物料灯N, 背景灯1...背景灯N */
/* 后分选室：物料灯1...物料灯N, 背景灯1...背景灯N */
#define LAMP_LAYER_INDEX_SET    100
#define LAMP_FRONT_MAT_BEGIN    0x00
#define LAMP_FRONT_BG_BEGIN     0x10
#define LAMP_REAR_MAT_BEGIN     0x20
#define LAMP_REAR_BG_BEGIN      0x30

/* 延迟付款密码长度 */
#define MAX_DELAYCODE             64             // 延迟付款密码最大长度

/* 统计信息种类 */
#define MAX_STATICS_TYPE    18          //统计信息 识别算法总数+产量+温度

#define STATICS_TYPE_FEED               0           // 产量
#define STATICS_TYPE_GREY_A             10           // 灰度杂质 A
#define STATICS_TYPE_GREY_B             11           // 灰度杂质 B
#define STATICS_TYPE_DISCOLOR_A         20           // 色差杂质 A
#define STATICS_TYPE_DISCOLOR_B         21           // 色差杂质 B
#define STATICS_TYPE_CROSS              30           // 差分统计
#define STATICS_TYPE_SVM_A              40           // SVM A
#define STATICS_TYPE_SVM_B              41           // SVM B
#define STATICS_TYPE_BIG_SMALL          50           // 大小算法
#define STATICS_TYPE_LONG_SHORT         51           // 长短算法
#define STATICS_TYPE_CIRCLE_LONG        52           // 圆长算法


/* 物料算法索引 */
#define MAX_ARITH_TOTAL             46          // 最多算法数量
#define MAX_GREY_COLOR_ARITH_TOTAL  7           // 最多支持的灰度，色差，差分算法总数
#define MAX_SVM_ARITH_TOTAL         4           // 最多支持的SVM算法总数
#define MAX_SHAPE_SVM_ARITH_TOTAL   3           // 最多支持的形选SVM算法总数
#define MAX_LONG_SHORT_TOTAL        2           // 最多支持的长短算法总数
#define MAX_BIG_SMALL_TOTAL         2           // 最多支持的大小算法总数
#define MAX_CIRCLE_LONG_TOTAL       2           // 最多支持的圆长算法总数
#define MAX_ROUND_TOTAL             2           // 最多支持的圆/非圆算法总数
#define MAX_BUD_TOTAL               2           // 最多支持的选芽算法总数
#define MAX_IMPURED_TOTAL           16          // 最多增加支持的传染种类
#define MAX_SUPPRESS_LARGE_TOTAL    3           // 最多支持的压制大算法总数
#define MAX_HSV_TOTAL               3           // 最多支持的HSV算法总数
#define MAX_TEA_ARITH_TOTAL         5           // 最多支持的TEA算法总数

#define AIRTH_NAME_LENGTH 120   //算法名称长度

#define ARITH_GREY_A            0           // 灰度杂质 A
#define ARITH_GREY_B            1           // 灰度杂质 B
#define ARITH_GREY_C            2           // 灰度杂质 C
#define ARITH_GREY_D            3           // 灰度杂质 D
#define ARITH_DISCOLOR_A        4           // 色差杂质 A
#define ARITH_DISCOLOR_B        5           // 色差杂质 B
#define ARITH_CROSS             6           // 差分算法  色差C借用差分算法
#define ARITH_SVM_A             7           // 智能算法 A
#define ARITH_SVM_B             8           // 智能算法 B
#define ARITH_SVM_C             9           // 智能算法 C
#define ARITH_SVM_D             10          // 智能算法 D
#define ARITH_SHAPE_SVM_A       11         // 形选智能算法 A
#define ARITH_SHAPE_SVM_B       12         // 形选智能算法 B
#define ARITH_SHAPE_SVM_C       13         // 形选智能算法 C
#define ARITH_BIG_SMALL         14         // 形状算法, 大小算法
#define ARITH_BIG_SMALL_B       15         // 形状算法, 大小算法2
#define ARITH_LONG_SHORT        16         // 形状算法, 选长/选短
#define ARITH_LONG_SHORT_B      17         // 形状算法, 选长短B
#define ARITH_SAT               18         // 饱和算法
#define ARITH_HUE               19         // 颜色算法
#define ARITH_SCALE             20         // 比例算法
#define ARITH_MILDEW            21         // 霉点算法
#define ARITH_OUTLINE           22         // 轮廓算法
#define ARITH_LINE              23         // 细纹
#define ARITH_BUD               24         // 选芽
#define ARITH_BUD_B             25         // 选芽B
#define ARITH_HSV               26         // HSV算法
#define ARITH_HSV_B             27         // HSV算法B
#define ARITH_HSV_C             28         // HSV算法C
#define ARITH_VALUE             29        // 灰度算法
#define ARITH_SPL_MILDEW        30        // 葫芦籽霉点
#define ARITH_CIRCLE_LONG       31        // 圆长
#define ARITH_CIRCLE_LONG_B     32        // 圆长2
#define ARITH_ROUND             33        // 圆非圆
#define ARITH_ROUND_B           34        // 圆/非圆2
#define ARITH_W_PEANUT          35        // 白沙果算法
#define ARITH_PEPPER_HANDLE     36        // 辣椒选把算法
#define ARITH_SUPPRESS_LARGE_A  37        // 压制大 A
#define ARITH_SUPPRESS_LARGE_B  38        // 压制大 B
#define ARITH_SUPPRESS_LARGE_C  39         // 压制大 C
#define ARITH_WHEAT_SPROUT      40         // 小麦芽头
#define TEA_ARITH_COLOR         41           //tea color
#define TEA_ARITH_COLOR_2       42           //tea color
#define TEA_ARITH_COLOR_3       43           //tea color
#define TEA_ARITH_COLOR_4       44          //tea color
#define TEA_ARITH_COLOR_5       45           //tea color



//#define PARAMS_ALL_SEPARATE             0   // 全部独立
//#define PARAMS_ALL_SAME                 1   // 全部相同
//#define PARAMS_VIEW_SAME                2   // view 相同， 前主视=后主视， 前辅视=后辅视
//#define PARAMS_TIMES_SAME               3   // 一二（三）次相同

#define MAT_COPY_ALL_ALONE              0   //全部独立
#define MAT_COPY_ALL_SAME               1   //全部相同
#define MAT_COPY_REAR_ALIKE_FRONT       2   //前主=后主， 前辅=后辅
#define MAT_COPY_SECOND_ALIKE_FIRST     3   //同一视内，二、三、四、五次的值与一次值相等

/* 报警相关处理 */
#define MAX_ALARM       16              // 最大报警数量, 0-气压, 1-风机, 2-灯, 3-皮带状态， 4-...
#define MAX_ALARM_COLOR_VOICE   4       // 灯光报警最大数量
#define CTRL_ALARM_MAX_FEED_SENSOR      8       // 料位传感器    //8个料满标志
#define CTRL_ALARM_MAX_TEMP_SENSOR      5      // 温度传感器, 0-控制板自带， 1~4 1-4个外围传感器
#define CTRL_ALARM_MAX_FEED_CURRENT     12      // 振动器电流传感器

#define ALARM_PRESSURE          0       // 气压报警
#define ALARM_FEED_LEVEL        1       // 料位报警
#define ALARM_TEMP_HIGH         2       // 温度报警
#define ALARM_LAMP              3       // 灯报警
#define ALARM_FEED              4       // 供料报警
#define ALARM_WIPE              5       // 清灰报警
#define ALARM_IPC               6       // IPC报警
#define ALARM_ACCELERATION      7       // 加速度传感器报警 (不用 先预留)
#define ALARM_VI                8       // 电流电压检测板报警
#define ALARM_CLASS             9       // 物料识别告警



/* 传感器型号 */
#define SENSOR_T1       1                   // 2564传感器, 可见光相机, 3维, 2048 10k行频,通用机型
#define SENSOR_T1_850   2                   // 2564传感器, 红外850相机, 3维, 2048 10k行频,通用机型
#define SENSOR_C1       3                   // C1 1450红外传感器, 2维， 512， 5k行频，RGI
#define SENSOR_D1       4                   // D1可见光传感器, 4维, 2048, 12.5k行频，用于履带机
#define SENSOR_D2       5                   // D2可见光传感器, 4维, 2048, 6.6k行频,D1的升级版，RG+
#define SENSOR_H1       6                   // H1红外光传感器, 1维, 512, 2.5k行频,米机、履带机红外
#define SENSOR_G2       7                   // G2传感器
#define SENSOR_C1_2     8                   // 二维红外传感器
#define SENSOR_G3       9                   // G3传感器

#define SENSOR_PIXELS_1024      0      // 1024传感器
#define SENSOR_PIXELS_2048      1      // 2048传感器
#define SENSOR_PIXELS_256       2       // 256传感器
#define SENSOR_PIXELS_512       3       // 512传感器
#define SENSOR_PIXELS_1152      4       // 512传感器
#define SENSOR_PIXELS_4096      5       // 512传感器
#define SENSOR_PIXELS_1664      6       // 512传感器
#define SENSOR_PIXELS_3328      7       // 512传感器
#define SENSOR_PIXELS_1024_TD   8       // 2048传感器 前1024是分时A的数据，后1024是分时B的数据
#define SENSOR_PIXELS_2048_TD    10      // 5120传感器 前2048是分时A的数据，后1024是分时B的数据
#define SENSOR_PIXELS_2048_XTD   11     // 2048传感器 奇行是分时A的数据，偶行是分时B的数据(2048*832-->2048*832+1024*416)
#define SENSOR_PIXELS_2048_GTD   12     // 2048传感器 奇行是分时A的数据，偶行是分时B的数据(2048*832-->2048*832+2048×832)

#define SENSOR_MAX_PIXELS       2048    // 传感器最大分辨率
#define SENSOR_MAX_DIM          4       //最大传感器维度


#define TD_1024_A  1
#define TD_1024_B  2
#define TD_2048_A  3
#define TD_2048_B  4


#define SENSOR_FOCUS_GRADE_DADA_LEN     0.3

/* 喷阀板型号 */
#define VAVLE_TYPE_1   1                // 喷阀 1
#define VAVLE_TYPE_2   2                // 喷阀 2


#define VAVLE_EJECTORS_80         80      //每个喷阀对应80个喷嘴
#define VAVLE_EJECTORS_64         64      //每个喷阀对应64个喷嘴
#define VAVLE_EJECTORS_60         60      //每个喷阀对应60个喷嘴
#define VAVLE_EJECTORS_96         96      //每个喷阀对应60个喷嘴
#define VAVLE_EJECTORS_128         128      //每个喷阀对应60个喷嘴
/* 背景条件设置 */
/* 相机板2.23版本及之前 */
//#define BACKGROUND_COLOR_RED              0
//#define BACKGROUND_COLOR_GREEN            1
//#define BACKGROUND_COLOR_BLUE             2
//#define BACKGROUND_COLOR_BLACK_WHITE      3
//#define BACKGROUND_COLOR_NIR              4
//#define BACKGROUND_COLOR_GREY             5
/* 相机板2.23版本及之后
注意：发送时BACKGROUND_COLOR_BLACK_WHITE和BACKGROUND_COLOR_NIR位置颠倒一下
*/
#define BACKGROUND_COLOR_RED                0
#define BACKGROUND_COLOR_GREEN              1
#define BACKGROUND_COLOR_BLUE               2
#define BACKGROUND_COLOR_BLACK_WHITE        3
#define BACKGROUND_COLOR_GREY               4
#define BACKGROUND_COLOR_YELLOW             5
#define BACKGROUND_COLOR_NIR                6

/* 恒流源板系数计算类型 */
#define LAMP_BOARD_QUO_TYPE_M        0
#define LAMP_BOARD_QUO_TYPE_L1       1
#define LAMP_BOARD_QUO_TYPE_L2       2
#define LAMP_BOARD_QUO_TYPE_L3       3

/* 串口相关设置 */
#define FIRST_SERIAL_NUM         2    //主串口序号， 通用
#define SECOND_SERIAL_NUM        1   //从口序号，备用，一般用

#define TIMEOUT             200
#define FRAME_LEN           256
#define SEND_PACKET_SIZE         25
#define SEND_PACKET_DATA_SIZE    16
#define SEND_PACKET_RECTRL_BOARD_SIZE   73

#define WAVE_FRAME_DATA_LEN         512     // 波形显示和图像采集，每包的数据长度
#define WAVE_FRAME_SINGLE_LEN       520     // 波形显示和图像采集，每包的长度
#define READ_PACKET_LENGTH_64       73      //版本查询每包的长度 64+9
#define READ_PACKET_LENGTH_1024     73       //目前回包都是73字节 1035      //版本查询每包的长度 1024+11

//#define DATA_LENGTH  512         //波形单包数据长度
#define MAX_WAVE_DEVIDE     4     //最大每行单色分成的包数      //2048=4*512
#define MAX_WAVE_DIM        4          // RGB_COUNT    3           //RGB 3色

#define IMAGE_HEIGHT            832         //图像高度:行数
#define IMAGE_HEIGHT_NOT_DP     400         //图像高度:行数

#define TD_DIM             5 //分时维度
/* 部分数据的极限值  */
#define ARITH_GREY_MAX_SENS             255
#define ARITH_COLOR_MAX_SENS            1023
#define ARITH_PROPOTION_MAX_SENS        1023
#define ARITH_BIG_SMALL_MAX_VALUE       65535   //1023
#define ARITH_LENGTH_MAX_VALUE          65535
#define ARITH_LONG_CIRCLE_MAX_VALUE     255
#define ARITH_LINE_DEFECT_MAX_AREA      65535
#define ARITH_LINE_DEFECT_MAX_LENGTH    1023
#define ARITH_SAT_MAX_SENSE             255
#define ARITH_HUE_MAX_SENSE             359
#define ARITH_HUE_MAX_SENSE_NEW         3599
#define ARITH_SCALE_MAX_SENSE           1023
#define ARITH_MILDEW_MAX_SENSE          255
#define ARITH_OUTLINE_MAX_SENSE         255
#define ARITH_LINE_MAX_SENSE            255
#define ARITH_BUD_MAX_SENSE             1023
#define ARITH_ROUND_MAX_SENSE           1023
#define ARITH_VALUE_MAX_SENSE           1023
#define ARITH_SPL_MILDEW_MAX_SENSE      255
#define ARITH_WHITE_PEANUT_MAX_SENSE    440
#define ARITH_EDGE_CUT_MAX_VALUE        9       //E系统要求从10改为9
#define ARITH_PEPPER_HANDLE_MAX_SENSE   1023
#define ARITH_PEPPER_HANDLE_MAX_VALUE   255
#define ARITH_PEPPER_HANDLE_MAX_LIMIT   1023
#define ARITH_TEA_SENSE_MAX_LIMIT       100


#define MAX_PARAM_NAME_LENGTH       128         //最大算法名称字符串长度

/* misc */

/* 远程相关 */
#define OPENVPN_CERT_PATH           "/sdcard/cfg/zk.ovpn"

#define HTTP                        "http://"
#define HTTPS                       "https://"
#define URL_TOKEN_REQUEST           ":8900/auth/mobile/token/device"
#define URL_SCHEMEINFO_UPLOAD       ":8900/api/dev/schemeinfo"
#define URL_STATUSINFO_UPLOAD       ":8900/api/dev/statusinfo"
#define URL_VPN_CERT_REQUEST        "http://service.chinaamd.cn:8082/v1/remote/crt"
#define URL_LV_VPN_CERT_REQUEST     "http://112.30.123.86:8082/v1/remote/crt"
#define URL_REALTIMEINFO_UPLOAD     ":8900/api/v1/realTimeParameters"
#define URL_MACHINEINFO_UPLOAD      ":8900/api/v1/globalParameters"
#define URL_PARTSTATUSINFO_UPLOAD   ":8900/api/v1/hardwareStatus"
#define URL_PARAALARM_UPLOAD        ":8900/api/v1/parametersStatus"
#define URL_STATISTICS_UPLOAD       ":8900/api/v1/globalStatistics"
#define URL_REMOTEACCESS_UPLOAD     ":8900/api/v1/remoteProtocol"
#define URL_WIPESPEED_UPLOAD        ":8900/api/v1/wipespeed"
//#define URL_LOGFILE_UPLOAD          ":8900/api/operatelog/file/upload/"
#define URL_GETOSSUPLOADSIGN        ":8900/api/getOssUploadSign"
#define URL_IMAGECAPTURE_UPLOAD     ":8900/api/v1/image"
#define URL_LOGFILE_UPLOAD          ":8900/api/operatelog/add"
#define URL_CNFFILE_UPLOAD          ":8900/api/runparam/add"
#define URL_WAVECAPTURE_UPLOAD      ":8900/api/v1/waveform"
#define URL_BLOWCOUNTS_UPLOAD       ":8900/api/v1/blowcount"
#define URL_VALVEAUDIO_UPLOAD       ":8900/api/v1/audio"
#define URL_MODELPARAMS_UPLOAD       ":8900/api/v1/modelParams"
#define URL_ONEKEYFIX_UPLOAD          ":8900/api/v1/oneKeyFix"
#define URL_ONEKEYTEST_UPLOAD       ":8900/api/v1/remotetest"

#define URL_MODEL_TYPEANDLIST       ":8900/eim/devModel/label/typeAndList"
#define URL_MODEL_LIST              ":8900/eim/devModel/page?current="
#define URL_MODEL_DOWNLOAD          ":8900/eim/devModel/download?filePathName="
#define URL_MODEL_EVALUATE          ":8900/eim/devModel/thumb"

#define MAX_MODELABLE_NUM           20    // 最多标签个数
#define MAX_MODE_BIGLABLE_NUM       5    // 最多大类标签种类
#define MAX_MODE_NUM_LIST           15    // 界面最大显示模型个数
#define MAX_ONEPAGE_IPCMODEL_CLASS     70  //单页显示最多类别数量


/* 模型相关 */
#define MAX_IPC_NUM             16
#define MAX_CAMERA_PER_IPC      24
#define MAX_IPC_MODEL_NUM       200  //最多模型数量
#define MAX_IPC_MODEL_CLASS     52  //最多类别数量

#define MAX_IPC_MODEL_NAME_LENGTH     64  //模型名字长度
#define MAX_IPC_MODEL_DATE_LENGTH     20  //模型日期长度
#define MAX_IPC_MODEL_LOAD_LENGTH     65  //模型名字长度+加载序号

#define MAX_IPC_CLASS_NAME_LENGTH     32  //类别名字长度

#define IPC_IP_ADDR_BASE        21
#define IPC_IP_IPC2_ADDR_BASE        200
#define IPC_CAMERA_IP_ADDR_BASE 1

#define IPC_CAMERA_TYPE_NULL    0   //非模型相机
#define IPC_CAMERA_TYPE_IPC1    1   //IPC1
#define IPC_CAMERA_TYPE_IPC2    2   //IPC1
#define IPC_CAMERA_TYPE_IPC3    3   //IPC2

#define IPC_REQ_RETURN_OK       0
#define IPC_REQ_RETURN_ERR     255  //格式错
#define IPC_REQ_RETURN_FAIL    254  //功能错
#define IPC_REQ_RETURN_NULL    250  //初始值

#define IPC_WORK_STATUS_IDEL     0   //空闲中
#define IPC_WORK_STATUS_RUNNINg  1   //工作中
#define IPC_WORK_STATUS_LOADING  2   //模型加载中
#define IPC_WORK_STATUS_ERR      255   //出错

/* ARM板相关 */
#define     BACKLIGHT_TIMEOUT   600     //10分钟背光
/* 参数文件保存状态 */
enum {
    ALL_PARAMS_FILE_OK = 0,             // 默认所有文件都保存正确
    ENGINEER_FILE_ERROR = 1,            // 工程参数文件保存错误
    GLOBAL_FILE_ERROR = 2,              // 全局参数文件保存错误
    CAMER_FILE_ERROR = 3,               // 相机参数文件保存错误
    SCHEME_FILE_ERROR = 4               // 方案参数文件保存错误
};


/* 一键截图界面编号 */
enum {
    None = 0,                   //
    Project = 1,                  // 方案
    SoftVersion = 2,            // 软件版本
    Sense = 3,                  // 精度
    Arith = 4,                  // 算法
    LampConfig =  5,             // 恒流源板配置
    LightConfig = 6,            // 灯光配置
    Backgroud =   7,             // 背景
    DelayTime =   8,              // 延时
    MachineSet =  9,           // 机型设置
    CameraWave = 10,             // 波形
    CameraSet =  11,             // 相机设置
    ModelSense =  12,             // 模型精度
    MaxNum

};
enum BoardType
{
    board_interface = 0,
    board_camera,
    board_ipc_camera,
    board_arm,
    board_valve,
    board_control,
    board_lamp,
    board_control_zd,
    board_control_dl
};
/*************************************************************************************
 *
 *      以下是需要保存到文件中各个全局参数的结构体定义
 *
 ************************************************************************************/
/* ---------------------------------------------------- */
/* 参数保存文件名称 */
#define CNF_MODE_DIR      "/sdcard/cnf/"                     //方案参数文件存放文件夹
#define CNF_BAK_DIR       "/sdcard/cnf_bak/"                  //方案参数文件备份存放文件夹
#define CNF_ENGINEER        "/sdcard/cnf/cnf.engineer"         // 工程模式参数文件
#define CNF_ENGINEER_B      "/sdcard/cnf/cnf.engineer_2"       // 工程模式参数文件备份
#define CNF_GLOBAL              "/sdcard/cnf/cnfglobal.json"           // 全局参数文件
#define CNF_GLOBAL_B            "/sdcard/cnf/cnfglobal_2.json"         // 全局参数文件备份
#define CNF_CAMERA              "/sdcard/cnf/cnf.camera"           // 全局相机参数文件
#define CNF_CAMERA_B            "/sdcard/cnf/cnf.camera_2"         // 全局相机参数文件备份
#define CNF_CUR_TIME            "/sdcard/cnf/cnf.cur_time"         // 延迟付款时，系统时间存放文件
#define CNF_PARAMS_FILE_STATUS      "/sdcard/cnf/cnf.params_status"           // 全局配置文件
#define CNF_PARAMS_FILE_STATUS_B    "/sdcard/cnf/cnf.params_status_2"         // 全局配置文件
#define LANG_DIR_SDCARD             "/sdcard/i18n/"
#define IMAGE_DIT_SDCARD            "/sdcard/bmp/"
#define CFG_APPSet                  "/sdcard/cfg/appsetting.ini"          // 全局配置文件
#define CFG_MachineSet                  "/sdcard/cfg/machineset.ini"          // 机型配置
#define DEBUG_MODE_Set              "/tmp/debugmode.ini"          // 全局配置文件
#define IPC_EJECT_STASTICS          "/tmp/ipceject.ini"

#ifdef Q_OS_WIN32
#define LANG_DIR                "../../MySorter/app/i18n/"   // 语言文件目录 *.qm
#else
#define LANG_DIR                "./i18n/"
#endif


#define MAX_IMG_NUM         12
#define MAX_PER_LIST        12
#define MAX_LIST_PAGE       3
#define MAX_SAMPLE_NUM      25000 //智能化物料点最大值
#define MAX_AI_CHOICE       4
const int const_max_svm = 300;
const int max_svm = 200;
const int feedconst = 100;

#define COEF_COUNT 10
#define SCREEN_VERSION  ("7.09.3." CUSTOM ".A.Z.DL")

#define RUNMODE_USER        0
#define RUNMODE_ENGINEER    1
#define RUNMODE_FACTORY     2
#define RUNMODE_ADMIN       3
//智能控制板
#define CMD_CTRL_BOARD_IP_PORT_SET                0x0001             // 远程控制板和PLC IP及PORT设置
#define CMD_CTRL_BOARD_IP_PORT_QUERY              0x0002             // 上位机查询远程控制板和PLC IP及PORT
#define CMD_CTRL_BOARD_IP_PORT_RET_PKG            0x0003             // 远程控制板和PLC IP及PORT回包
#define CMD_PLC_MODBUS_SET                        0x0004             // 远程PLC Modbus设置
#define CMD_CTRL_BOARD_VERSION_REQUEST            0x0005             // 远程控制板版本查询
#define CMD_CTRL_BOARD_VERSION_RET_PKG            0x0006             // 远程控制板版本查询回包
#define CMD_CTRL_BOARD_MACHINE_INFO_QUERY_REQUEST 0x0007             // 查询当前色选机整机信息
#define CMD_CTRL_BOARD_MACHINE_INFO_RET_PKG       0x0008             // 查询当前色选机整机信息回包
#define CMD_CTRL_BOARD_STATUS_QUERY_REQUEST       0x0009             // 远程控制板请求查询色选机状态
#define CMD_UPPER_COMPUTER_STATUS_RET_PKG         0x000A             // 色选机状态回包
#define CMD_CTRL_BOARD_OUTAGE_REQUEST             0x000B             // 远程控制板请求断电
#define CMD_UPPER_COMPUTER_OUTAGE_REQUEST         0x000C             // 上位机通知远程控制板延迟断电
#define CMD_CTRL_BOARD_FEED_ONOFF_REQUEST         0x000D             // 远程控制板开关下料请求
#define CMD_CTRL_BOARD_FEED_MAIN_VALUE_REQUEST    0x000E             // 远程控制板主供料量请求
#define CMD_CTRL_BOARD_FEED_VICE_VALUE_REQUEST    0x000F             // 远程控制板辅供料量请求

// 智控屏指令
#define POWER_STATUS_BIT            0
#define COLOR_SORTING_STATUS_BIT    1
#define AIR_PRESSURE_ALARM_BIT      0
#define CIRCUIT_BOARD_ALARM_BIT     1
#define DEEP_LEARNING_ALARM_BIT     2
#define MATERIAL_LEVEL_ALARM_BIT    3
#define LIGHT_ALARM_BIT             4
#define VIBRATOR_ALARM_BIT          5

#endif // CONSTANT_H

