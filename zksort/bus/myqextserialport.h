#ifndef MYQEXTSERIALPORT_H
#define MYQEXTSERIALPORT_H

//#define _TTY_POSIX_     1       // qextserialport使用
#include <QThread>
#define USE_NEW_PLATFORM_PROTOCAL       0   //是否用新通讯协议

#include "qextserialport.h"          //
#include "myqueue.h"
#include "constant.h"
#include "global.h"

#if (USE_NEW_PLATFORM_PROTOCAL == 1)
/***************************************************************************
 *
 * 新平台通信协议
 *
 * **************************************************************************/
/* 命令编码 0x01~0x1f */
/* 1. 系统命令 0x0001~0x001f*/
#define CMD_INT_ENCRYPT_PARAMS          0x0001             // 读写加密参数
#define CMD_INT_ENCRYPT_TIME            0x0002             // 写加密时间
#define CMD_ALL_MACHINE_CONFIG          0x0003             // 机型配置
#define CMD_ALL_FPGA_MODE               0x0004             // FPGA用户模式
#define CMD_ALL_FPGA_UPGRADE_MODE       0x0005             // FPGA用升级数据所属模式
#define CMD_ALL_VERSION_CAPTURE         0x0006             // 版本查询
#define CMD_ALL_SORT_ON_OFF             0x0007             // 色选开关
#define CMD_ALL_WIPE_SET                0x0008             // 通知清灰，相机板+控制板命令
#define CMD_HARDWARE_INTERFACE_CONFIG   0x0009             // 接口板命令，硬件接口配置，
#define CMD_CAMERA_STATUS_CAPTURE       0x000A             // 相机板的实时状态查询

/* 2. 识别色选参数 0x20~0x5f*/
#define CMD_CAMERA_ANA_GAIN             0x0020             // 模拟增益
#define CMD_CAMERA_BIASING              0x0021             // 偏置信息
#define CMD_CAMERA_PIXEL_SCALE          0x0022             // 像元划分
#define CMD_CAMERA_WAVE_PROCESSING      0x0023             // 波形显示预处理, 是否启动校正...
#define CMD_CAMERA_IMG_SHIFT            0x0024             // 图像分量偏移
#define CMD_CAMERA_IMG_PROCESSING       0x0025             // 图像信息预处理， 传染、切边缘、闭运算...
#define CMD_CAMERA_BACKGROUND_SET       0x0026             // 背景条件
#define CMD_CAMERA_SORT_ARITH_PARAMS    0x0027             // 算法处理相关设置， 最大尺寸、正反选
#define CMD_CAMERA_VAVLE_DELAY_TIME     0x0028             // 喷阀数据预处理， 坏点个数、喷阀开关、延迟时间...
#define CMD_CAMERA_GRAY_COLOR           0x0029             // 灰度&色差算法&差分
#define CMD_CAMERA_ARITH_COLOR_RESERVE  0x002A             // 颜色算法保留 --灰度&色差&差分
#define CMD_CAMERA_PROPOTION_RESERVE    0x002B             // 比例颜色保留算法 --此命令已取消
#define CMD_CAMERA_ARITH_PROPOTION      0x002C             // 比例算法
#define CMD_CAMERA_LINE_DEFECT          0x002D             // 线性病斑
#define CMD_CAMERA_SVM_SIZE             0x002E             // SVM算法--尺寸信息
#define CMD_CAMERA_SVM_QUO              0x002F             // SVM算法--系数
#define CMD_CAMERA_ARITH_SHAPE          0x0030             // 形状算法， 大小、长短、圆长
#define CMD_CAMERA_ARITH_ENABLE         0x0031             // 算法使能

/* 3. 相机操作 0x60~0x9f*/
#define CMD_CAMERA_CALIBRATE_BRIGHT     0x0060             //明场校正
#define CMD_CAMERA_CALIBRATE_DARK       0x0061             //暗场校正
#define CMD_CAMERA_WAVE_CAPTURE         0x0062             //图像获取
#define CMD_CAMERA_STATICS_CAPTURE      0x0063             //实时获取统计信息

/* 4.1 控制板配置命令 0xA0~0xCf*/
#define CMD_CTRL_FEED_SET               0x00A0             //振动量开关及大小
#define CMD_CTRL_ALARM_SET              0x00A1             //报警设置， 1--报警时要停止下料， 0--不停止
#define CMD_CTRL_TRANSPARENT            0x00A2             //透传命令

/* 4.2 控制板查询命令 0xD0~0xDf*/
#define CMD_CTRL_ALARM_CAPTURE          0x00D0             //报警查询
#define CMD_CTRL_AIR_CAPTURE            0x00D1             //耗气总量查询

/* 5 恒流源板命令 0xE0~0xFF*/
#define CMD_LAMP_BRIGHT                 0x00E0             // 光源亮度或开关

/* 6.1 接口板配置命令 0x0101~0x011F*/
#define CMD_INT_VAVLE_TIME              0x0101             // 喷阀吹气时间
#define CMD_INT_EJECTOR_VAVLE_TEST      0x0102             // 喷阀自检， 通道1~通道10
#define CMD_INT_VAVLE_AGING_TEST        0x0103             // 喷阀老化测试
#define CMD_VAVLE_ON_OFF				0x0104             // 喷阀on off

/* 6.2 接口板查询命令 0x0120~0x012F*/
#define CMD_INT_EJECT_TIMES_CAPTURE     0x0120             // 吹气次数查询
#else
/****************************************************************************
 *
 * 通用平台通信协议
 *
 * **************************************************************************/
/*-----------------------------------------------------------------------*/

/* 命令编码 0x01~0x1f */
/* 1. 系统命令 0x0001~0x001f*/
#define CMD_INT_ENCRYPT_PARAMS_REQUIRE          0x0000             // 读加密参数
#define CMD_INT_ENCRYPT_CODE_WRITE              0x0001             // 写加密密码
#define CMD_INT_ENCRYPT_TIME_WRITE              0x0002             // 写加密时间
#define CMD_ALL_MACHINE_CONFIG                  0x0003             // 机型配置,接，相机
#define CMD_ALL_FPGA_USER_MODE                  0x0004             // FPGA用户模式切换
#define CMD_ALL_FPGA_UPGRADE_MODE               0x0005             // FPGA用升级数据所属模式
#define CMD_ALL_VERSION_REQUIRE                 0x0006             // 版本查询

#define CMD_HARDWARE_INTERFACE_CONFIG           0x0009             // 接口板命令，硬件接口配置，
#define CMD_CAMERA_STATUS_CAPTURE               0x000A             // 相机板的实时状态查询

/* 2. 相机参数 0x10~0x1f*/
#define CMD_CAMERA_ANA_GAIN                     0x0010             // 模拟增益
#define CMD_CAMERA_BIASING                      0x0011             // 偏置信息
#define CMD_CAMERA_HORI_DPI                     0x0012             // 传感器横向分辨率
#define CMD_CAMERA_PIXEL_SCALE                  0x0013             // 像元划分
#define CMD_CAMERA_DOT_SAVE                     0x0014             // 坏点保存
#define CMD_CAMERA_SENSOR_CONFIG                0x0015             // 传感器配置
#define CMD_CAMERA_CALIBRATE_BRIGHT_ENABLE      0x0016             // 明场校正使能
#define CMD_CAMERA_CALIBRATE_BRIGHT_CAPTURE     0x0017             // 明场校正获取
#define CMD_CAMERA_CALIBRATE_BRIGHT_SAVE        0x0018             // 明场校正保存
#define CMD_CAMERA_CALIBRATE_DARK_ENABLE        0x0019             // 暗场校正使能
#define CMD_CAMERA_CALIBRATE_DARK_CAPTURE       0x001A             // 暗场校正
#define CMD_CAMERA_CALIBRATE_DARK_SAVE          0x001B             // 暗场保存
#define CMD_CAMERA_MATERIAL_INFALTE             0x001C             // 物料膨胀, 双红外材质机，暂时不用
#define CMD_CAMERA_IMG_SHIFT                    0x001D             // 图像分量偏移, 双红外材质机，暂时不用
#define CMD_CAMERA_SENSOR_DIM                   0x001E             // 传感器维度
#define CMD_CAMERA_PIXEL_SHIFT                  0x001F             // 亚像素拼接

//设备操作
#define CMD_ALL_SORT_START                      0x0020             // 色选开关,接，相机
#define CMD_CAMERA_WAVE_CAPTURE                 0x0021             // 图像获取， 0：1行， 1：400行， 2：5000行

#define CMD_CTRL_MISC_CONTROL                   0x0022             // 整机控制
#define CMD_ALL_WIPE_SET                        0x0023             // 通知清灰，主相机板+控制板命令
#define CMD_CAMERA_MISC_CAPTURE                 0x0024             // 查询相机状态和信号 0~1清灰刷中心位置， 2~3 清灰刷宽度
#define CMD_CAMERA_STATICS_CAPTURE              0x0025             // 实时获取统计信息
#define CMD_CAMERA_STATICS_CLEAR                0x0026             // 清零实时获取统计信息


#define CMD_CTRL_FEED_SET                       0x0027             // 供料量
#define CMD_LAMP_BRIGHT                         0x0028             // 光源亮度或开关
#define CMD_LAMP_ON_OFF                         0x0029             // 光源开关

#define CMD_INT_VAVLE_TIME                      0x002A             // 喷阀吹气时间,延迟时间见0x38
#define CMD_INT_EJECTOR_VAVLE_TEST              0x002B             // 喷阀自检， 通道1~通道10

#define CMD_CAMERA_INPURED                      0x002C             // 传染算法，范围0~31
#define CMD_CAMERA_BAD_POINT                    0x002D             // 坏点个数，范围0~15
#define CMD_ALL_EJECTOR_ENABLE                  0x002E             // 喷阀使能, 整机状态一致
#define CMD_ALL_SORT_REVERSE                    0x002F             // 正反选， 0正选，1反选，按次设置
#define CMD_ALL_EJECT_MODE                      0x0030             // 吹气方式，0经济选，1精选，按次设置
#define CMD_CAMERA_WIPE_RESULT                  0x0031             // 查询清灰结果， 暂时不用
#define CMD_CTRL_AIR_CAPTURE                    0x0032             // 耗气总量查询, 作废
#define CMD_INT_VAVLE_OPEN_TIMES                0x0033             // 喷阀打开次数,单位：百万次
#define CMD_CAMERA_THROUGHPUT_CAPtrue           0x0034             // 产量查询
#define CMD_CTRL_ALARM_CAPTURE                  0x0035             // 报警查询
#define CMD_CTRL_ALARM_SET                      0x0036             // 报警设置， 1--报警时要停止下料， 0--不停止
#define CMD_CTRL_TRANSPARENT                    0x0037             // 透传命令,控制履带
#define CMD_INT_VAVLE_DELAY_TIME                0x0038             // 喷阀延迟时间,吹气时间见0x2A, 按相机发送
#define CMD_INT_VIEW_LOGIC_RELATION             0x0039             // 前后视逻辑关系， 0：相或， 1：相与
#define CMD_INT_VAVLE_DELAY_TIME_H              0x003A             // 喷阀延迟时间高字节


//识别算法
#define CMD_CAMERA_ARITH_DISCOLOR_1             0x0040             // 色差算法1，浅黄算法
#define CMD_CAMERA_ARITH_DISCOLOR_2             0x0041             // 色差算法2, 深黄算法
#define CMD_CAMERA_ARITH_GREY_1                 0x0042             // 灰度算法1, 浅暗算法
#define CMD_CAMERA_ARITH_GREY_2                 0x0043             // 灰度算法2, 深暗算法
#define CMD_CAMERA_ARITH_GREY_3                 0x0044             // 灰度算法3, 选玻璃算法
#define CMD_CAMERA_ARITH_GREY_4                 0x0045             // 灰度算法4, 腹白算法，红外850
#define CMD_CAMERA_ARITH_GREY_5                 0x0046             // 灰度算法5, 恶杂算法，红外1450

#define CMD_CAMERA_ARITH_SVM_1                  0x0047             // SVM算法1
#define CMD_CAMERA_ARITH_SVM_2                  0x0048             // SVM算法2
#define CMD_CAMERA_ARITH_SCALE                  0x0049             // 大小算法
#define CMD_CAMERA_ARITH_MAX_SIZE               0x004A             // 最大行列尺寸，废弃不用

#define CMD_CAMERA_BACKGROUND_SET               0x004B             // 背景条件
#define CMD_CAMERA_EDGE_CUT                     0x004C             // 边缘切除
#define CMD_INT_VAVLE_AGING_TEST                0x004E             // 喷阀老化测试

#define CMD_CAMERA_ARITH_CIRCLE_LONG            0x0050             // 选圆选长算法,0-选长， 1-选圆
#define CMD_CAMERA_ARITH_LONG_SHORT             0x0051             // 选长选短算法,0-选短， 1-选长,
#define CMD_CAMERA_SENS_UPDATE                  0x0052             // 灵敏度校正, 米机默认使能
#define CMD_CAMERA_ARITH_HSV                    0x0053            // HSV调试(三维一体)
#define CMD_CAMERA_ARITH_MULTY_SVM              0x005E            // 多维SVM

#define CMD_CAMERA_SENS_SAT                     0x0060             // 饱和算法
#define CMD_CAMERA_SENS_HUE                     0x0069             // 颜色算法
#define CMD_CAMERA_SENS_SCALE                   0x0062             // 比例算法
#define CMD_CAMERA_SENS_MILDEW                  0x0063             // 霉点算法
#define CMD_CAMERA_SENS_OUTLINE                 0x0064             // 轮廓算法
#define CMD_CAMERA_SENS_LINE                    0x0065             // 纹理算法
#define CMD_CAMERA_SENS_BUD                     0x0066             // 选芽算法
#define CMD_CAMERA_SENS_ROUND                   0x0067             // 选圆选非圆算法
#define CMD_CAMERA_SENS_SPL_MILDEW              0x006B             // 葫芦籽霉点算法
//#define CMD_CAMERA_SENS_W_PEANUT                0x006C             // 白沙果算法
#define CMD_CAMERA_SENS_W_PEANUT                0x0070             // 白沙果算法
#define CMD_CAMERA_ARITH_DISCOLOR_3             0x0071             // 色差算法3
//#define CMD_CAMERA_ARITH_LONG_SHORT_B           0x0071             // 长短B
//#define CMD_CAMERA_SENS_BUD_B                   0x0072             // 选芽算法B
#define CMD_CAMERA_ARITH_SVM_3                  0x0072             // SVM算法3
#define CMD_CAMERA_ARITH_SCALE_GREY_1           0x0073             // 灰度算法1比例
#define CMD_CAMERA_ARITH_SCALE_DISCOLOR_1       0x0074             // 色差算法1比例
#define CMD_CAMERA_ARITH_SCALE_SVM_2            0x0075             // 智能算法2比例
#define CMD_CAMERA_ARITH_EDGE_CUT               0x0076             // 算法切边
#define CMD_CAMERA_ARITH_SHAPE_SVM_1            0x0077             // 形选智能
#define CMD_CAMERA_ARITH_SCALE_2                0x0078             // 大小算法2
#define CMD_CAMERA_ARITH_CIRCLE_LONG_2          0x0079             // 圆长算法2
#define CMD_CAMERA_ARITH_LONG_SHORT_2           0x007A             // 长短算法2
#define CMD_CAMERA_SENS_ROUND_2                 0x007B             // 圆非圆算法2
#define CMD_CAMERA_ARITH_PEPPER_HANDLE          0x007C             // 辣椒选把算法

#define CMD_CAMERA_EJECT_DURATION_IPC           0x007d             // 新多种类吹气时间
#define CMD_CAMERA_IMPURED_IPC                  0x007e             // 新多种类传染
#define CMD_INT_VAVLE_MODEL_DELAY_TIME          0x007f             // 模型延迟时间

#define CMD_CAMERA_TDI_ENABLE                   0x0080             // TDI使能, C1相机
#define CMD_CAMERA_SHAPE_INPURED                0x0081             // 形选传染算法
#define CMD_CAMERA_DOT_CORRECTION               0x0082             // 坏点纠错
#define CMD_CAMERA_TEMP_CAPTURE                 0x0083             // 相机板温度

#define CMD_CAMERA_IMAGE_CAPTURE_SYNC           0x0086             // 同步获取图像， 0-获取图像， 1-上传图像，用于前后视同步采集，分时上传 (230131 弃用)
#define CMD_CAMERA_PIXEL_CORSS                  0x0087             // 是否交叉像元，
#define CMD_CAMERA_CENTER_SIZE_1                0x0088             // 中心化尺寸1
#define CMD_CAMERA_CENTER_SIZE_2                0x0089             // 中心化尺寸2
#define CMD_INT_VAVLE_WAIT_TIME                 0x008A             // 等待时间，范围：0~15，单位：0.1ms
#define CMD_INT_VAVLE_TAIL_TIME                 0x008B             // 等待时间，范围：0~15，单位：0.1ms
#define CMD_CAMERA_WAVE_DISPLAY_TYPE            0x008C             // 波形数据显示类型， 0-正常显示信号， 1—明场校正数据，2-暗场校正数据，3-坏点纠错数据
#define CMD_INT_BLOW_COUNTS_CAPTURE             0x008D             // 获取单个喷阀吹气次数
#define CMD_CAMERA_ARITH_DELAY_TYPE             0x008F             // 延迟类型选择

#define CMD_INT_VAVLE_DEFEND_TIME               0x00C2             //　喷阀保护及解除保护时间
#define CMD_INT_LIGHT_ORDER                     0x00C3            //　亮灯顺序
#define CMD_CAMERA_ARITH_SVM_4                  0x010C              // SVM算法4

#define CMD_CAMERA_COLOR_CHOOSE                 0x0301             // 图像颜色分类选择,byte0[0-3]:BGRN
#define CMD_HSV_ARITH_RESERVE_PARAM             0x0302             // HSV算法保留参数
#define CMD_CAMERA_IMAGE_COMPRESS_MODE          0x0303             // 图像压缩模式， 0-模式1，抽样，1-模式2，均值，2-模式3，最大，3-模式4，最小
#define CMD_INT_ARITH_REJECT_RATIO              0x0307             // DP算法剔除比例（控碎率）
#define CMD_ARITH_SUPPRESS_LARGE                0x0312             // 压制大
#define CMD_ARITH_WHEAT_SPROUT                  0x0313             // 小麦芽头
#define CMD_CLEAR_HISTOGRAM                     0x0318             // 清零直方图
#define CMD_RESIZE_MODE                         0x0319             // resize模式使能
#define CMD_CAMERA_TEA_COLOR                    0x0320             // tea
#define CMD_CAMERA_ARITH_HSV_2                  0x00B0             // HSV算法2
#define CMD_CAMERA_ARITH_HSV_3                  0x00B1             // HSV算法3

#define CMD_DELAY_BUF_ROW_COUNT                 0xEEEB             // 相机板缓冲图像行数，用于延迟时间偏置
#define CMD_COLOR_RESTORE_ENABLE                0xEEEC             // 相机板颜色还原使能
#define CMD_IPC_CAMERA_COLOR_RESTORE            0xEEED             // 相机板颜色还原数值
#define CMD_IPC_CAMERA_IP_SET                   0xEEEE             // 设置相机板IP地址
#define CMD_CAMERA_IMAGE_DROP_LINE              0xEEEF             // 图像丢行模式　0:不丢，1-100:ｎ行丢１行

/*加速度传感器部分*/
#define CMD_ACCELERATION_CAPTURE                  0x0310              // 加速传感器查询
#define CMD_ACCELERATION_ENABLE                   0x0309              // 加速度传感器使能

#define CMD_INT_BLOW_COUNTS_CAPTURE_2             0x0321             //获取喷阀吹气次数2，大于12通道，吹嘴大于96时吹气次数

#define CMD_CTRL_VI_DETECT_BOARD_VERSION_QUERY    0x0323              // 电压电流检测板版本查询
#define CMD_CTRL_NEW_ALARM_CAPTURE                0x0324              // 新报警查询
#define CMD_CAMERA_GET_CALIINFO                   0x0325              // 校正信息查询
#define CMD_CAMERA_IMAGE_ENHANCE                  0x0327              // 图像增强
#endif
/* 定义单个接口板外接前置板的最大数量 */
//const int MAX_UNIT_PER_INT = 16;

class MyQextSerialPort
{
public:
    MyQextSerialPort();

    int comOpen(int nPortno);
    int comClose(int nPortno);

    int com1Write(int nCmd, char intAddr, char boardType,  char boardAddr, char *data, int nCount);
    int com1Read(char *sBuf, int nLen);

    int com2Write(int nCmd, char sModuleAddr, char arg, int nValue, int nCount);

    int com2Read(char *sBuf, int nLen);

    unsigned int crcFpga(char *sData, int nLen);       // CRC check
    void crcFpga2(char *sData, int nLen, uint &ret);
    int com1Flush();
    int com2Flush();		//

    /*************************************************************************************************************
     * 新加的消息队列收发
     * ************************************************************************************************************/
    int isCmdNeedResp(int nCmd);
    int sendCom1Cmd(char *data);
    int sendCom2Cmd(char *data);
    void pushCom1CmdData(int nCmd, char intAddr, char boardType,  char boardAddr, char *data, int nCount);  //经过消息队列，再调用串口发送函数
    void pushCom2CmdData(int nCmd, char intAddr, char boardType,  char boardAddr, char *data, int nCount);  //经过消息队列，再调用串口发送函数
    void com1WriteDirect(int nCmd, char intAddr, char boardType,  char boardAddr, char *data, int nCount);  //直接调用的串口发送函数，喷阀自检命令

private:
    QextSerialPort *myPort1, *myPort2;
    QMutex m_sendLock;

    int testSendTimes;
    //    int sendTimes = 0;
};

#ifdef Q_OS_UNIX
/* 以下是ZkSerialPort */
struct stru_port_info{
    int  is_busy;
    char name[32];
    int  handle;
};
class ZkSerialPort
{
    enum
    {
        MAX_PORTS = 4,
        BAUDRATE_TTYS1=115200,
        BAUDRATE_TTYS2=115200,
    };
public:
    ZkSerialPort();

    int comOpen(int nPortno);
    int comClose(int nPortno);

    int com1Write(int nCmd, char intAddr, char boardType,  char boardAddr, char *data, int nCount);
    int com1Read(char *sBuf, int nLen);

    int com2Write(int nCmd, char sModuleAddr, char arg, int nValue, int nCount);

    int com2Read(char *sBuf, int nLen);

    unsigned int crcFpga(char *sData, int nLen);       // CRC check
    void crcFpga2(char *sData, int nLen, uint &ret);
    int com1Flush();		//
    int com2Flush();		//

    /*************************************************************************************************************
     * 新加的消息队列收发
     * ************************************************************************************************************/
    int isCmdNeedResp(int nCmd);
    int sendCom1Cmd(char *data);
    int sendCom2Cmd(char *data);
    void pushCom1CmdData(int nCmd, char intAddr, char boardType,  char boardAddr, char *data, int nCount);  //经过消息队列，再调用串口发送函数
    void pushCom2CmdData(int nCmd, char intAddr, char boardType,  char boardAddr, char *data, int nCount);  //经过消息队列，再调用串口发送函数

    void com1WriteDirect(int nCmd, char intAddr, char boardType,  char boardAddr, char *data, int nCount);  //直接调用的串口发送函数，喷阀自检命令
    void com2WriteDirect(int nCmd, char intAddr, char boardType,  char boardAddr, char *data, int nCount);  //直

private:
    long zkGetBaudrate(long baudrate);
    int zkSerialOpenConfig(int portno, char *deviceName, long baudrate, int parity, int dataBits, int stopBits, int iqSize, int oqSize);

    int zkSerialOpen(int portno, char *deviceName, long baudrate);
    int zkSerialClose(int portno);
    int zkSerialRead(int portno, char *buf, int maxCount, int timeout);
    int zkSerialWrite(int portno, char *buf, int maxCount);
    void zkSerialFlushRead(int portno);

    struct stru_port_info ports[MAX_PORTS];

    //    QextSerialPort *myPort1, *myPort2;
    QMutex m_sendLock;
    QMutex m_sendLock_2;

    int testSendTimes;
    //    int sendTimes = 0;
};
#endif

/******************************************************************************************************

                        发送串口线程

******************************************************************************************************/
class mySerialSendThread : public QThread
{
    //    Q_OBJECT;

public:
    mySerialSendThread();


protected:
    void run(void);
private:

};



class mySerial2SendThread : public QThread
{
    //    Q_OBJECT;

public:
    mySerial2SendThread();


protected:
    void run(void);
private:

};


/******************************************************************************************************

                        接收串口线程

******************************************************************************************************/
class mySerialRecvThread : public QThread
{
    Q_OBJECT
    enum
    {
        Recv_Packet_Type_64 = 0,
        Recv_Packet_Type_1024 = 1
    };
public:
    mySerialRecvThread();
    int getRecvPacketLength(uchar cmd_h,uchar cmd_l);
    int getRecvPacketTimeOut(uchar cmd_h,uchar cmd_l);



protected:
    void run(void);
private:
    void recvDataAnalysis(char *sBuf, int nType); //串口回包解析   nType: 0-general包， 1-统计信息等长包
    unsigned short wYear, wMonth, wDay;
signals:
    void signalGetFlow();
    void signalupdateVIdata();
};


class mySerial2RecvThread : public QThread
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
    enum
    {
        Recv_Packet_Type_64 = 0,
        Recv_Packet_Type_1024 = 1
    };
public:
    mySerial2RecvThread();


protected:
    void run(void);
private:
    void recvDataAnalysis(char *sBuf, int nType); //串口回包解析   nType: 0-general包， 1-统计信息等长包
    unsigned short wYear, wMonth, wDay;
signals:
    void signalGetFlow();
    void signalCtrlBoardOutage();
    void signalCtrlBoardIpPortQuery();
    void signalCtrlBoardFeedOnOffQuery(int);
    void signalCtrlBoardStatusQuery(int);
    void signalCtrlBoardMachineInfoQuery();
    void signalCtrlBoardFeedMainValueQuery();
    void signalCtrlBoardFeedViceValueQuery();
};


static int lastResCmdId = 0;        // 上一条需要回包的命令ID
static bool hasRecvLastCmd = false;     // 是否收到上一条需要回包的命令回包

static int lastResCmdId2 = 0;        // 上一条需要回包的命令ID
static bool hasRecvLastCmd2 = false;     // 是否收到上一条需要回包的命令回包


#ifdef Q_OS_UNIX
extern ZkSerialPort mySerial;
#else
extern MyQextSerialPort mySerial;
#endif

extern mySerialRecvThread *mySerialRecv;
extern mySerialSendThread *mySerialSend;
extern mySerial2RecvThread *mySerial2Recv;
extern mySerial2SendThread *mySerial2Send;
#endif // MYQEXTSERIALPORT_H
