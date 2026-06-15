#ifndef RUNTIME_H
#define RUNTIME_H

#include <QObject>
#include <QVector>
#include <QStringList>
#include "logger.h"
#include "mylanguage.h"
#include "data/myjson.h"
#include <QTcpServer>
#include <QTcpSocket>

#define color_btn background-color:rgb(51,153,255,255)

typedef struct stru_Arith_Index_Name {
    int nIndex;                           // 算法索引
    QString sName;                        // 算法名称
}struArithIndexName;

typedef struct SampleObj{
    int r;int g;int b;int n;
    bool operator ==(const SampleObj &obj){
        return (r == obj.r)&&(g==obj.g)&& (b==obj.b)&& (n==obj.n);
    }
    bool operator <(const SampleObj &obj)const{
        if(r != obj.r)
            return r < obj.r;
        if(g != obj.g)
            return g < obj.g;
        if(b != obj.b)
            return b < obj.b;
        return n < obj.n;
    }
}SampleObj;
typedef struct ScaleSampleObj{
    int area, edge, width, height, centroid;
    bool operator ==(const ScaleSampleObj &obj){
        return (area == obj.area)&&(edge==obj.edge)&& (width==obj.width)&& (height==obj.height)&& (centroid==obj.centroid);
    }
    bool operator <(const ScaleSampleObj &obj)const{
        if(area != obj.area)
            return area < obj.area;
        if(edge != obj.edge)
            return edge < obj.edge;
        if(width != obj.width)
            return width < obj.width;
        if(height != obj.height)
            return height < obj.height;
        if(centroid != obj.centroid)
            return centroid < obj.centroid;
    }
}ScaleSampleObj;



class Runtime : public QObject
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
public:
    explicit Runtime(QObject *parent = 0);

    QString sIdentifyGroupName[MAX_LAYER][MAX_VIEW][MAX_GROUP_IDENTIFY];  // 识别组界面显示名称, 一次前后视、二次前后视、三次前后视， 上下视...
    QString sTickGroupName[MAX_LAYER][MAX_VIEW][MAX_GROUP_TICK];          // 剔除组界面显示名称, 一次、二次、三次， 上下层...
    QString sArithmeticName[ARITHMETIC_TOTAL];                  		  // 物料识别算法名称
    QString sLamp[MAX_LAMP_PER_BOARD];                                    // 灯光设置名称

    QString sVendor;
    QString sProduct;
    QString sMachine;
    QString sVersionAPP;
    QString sVersionSVN;
    const QString getVerdorName();

    int g_level;//权限 默认为客户
    int g_level2;//工程师显示 等级 1 2 3
    int g_skinIndex;    //皮肤主题
    int g_layerTotal;   //读配置文件的总层数
    int g_vendorIndex;  //品牌索引 默认为0 中科
    int g_selfCheckState;
    int nMachineArithTotal;
    QVector<struArithIndexName> arithIndexName;                       // 整机识别算法列表
    QStringList vendorNameList;


    QStringList nimageNameVec;
    void getCurImageLayerAndView(int& layer,int& view,int &nUnit,int index);
    int  appPositionX;
    int  appPositionY;//程序的左上角在屏幕上的绝对位置

    int     m_fontFamilyId[10];
    int     g_usedFamilyId;

    QString getFileSize(QString filePath);
    QString getFileMd5(QString filePath);
    void saveSetting();
    bool save();
    void updateTmpImgList();
    QString GetIpAddress(QString netName);
    void initLan();
    bool copyFileToPath(QString sourceDir, QString toDir, bool coverFileIfExist);
    bool copyDirToPath(const QString& srcPath, const QString& dstPath, bool coverFileIfExist);
    bool delFile(QString filePath);
    bool checkUsbExist();
    bool dirExist(QString dir);
    QStringList getUsbSpace(QString dev);
    QString getUsbPath();
    int getStartUpMode();  //获取当前系统启动模式 -1-异常 0-sd卡启动，1-emmc启动
    bool checkIpcMounted(QString ipcIpAddr);
    QString fillIn(QString str,int maxLen, QChar c);
    int mySystem(QString str);
    QString mySystemStr(QString str);
    void getFreeSpace();
    QMap<SampleObj,int> m_sampleMap[2];//
    QString m_expModeName;
    QMap<int,int> m_imgDataMap[2][9];

    void updateCornStastic();
    void ConnectToControlScreen();


    QString modelFullName;
    QString modelFullNametest; // 类别个数统计表格
    QString modelFullName_Area; // 类别面积统计表格
    QString modelFullName_Frame; // 类别框面积统计表格

    QString piciname;
    QStringList modelFullNameList;
    QString updateZipFileName;


};

#endif // RUNTIME_H
