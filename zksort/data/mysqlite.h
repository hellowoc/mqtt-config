#ifndef MYSQLITE_H
#define MYSQLITE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStringList>
#include <QThread>
#include <QMap>
#include "globalflow.h"

#define STATISTIC_DATABASE "/sdcard/ts/statisticDataLog.db"

enum StatisticType{
    Throughout,
    ArithEjectTimes
};
enum QueryType{
    RealTime,
    LastDay,
    LastWeek,
    Last3Month

};
enum ViewType{
    MainFront,
    MainRear,
    ViceFront,
    ViceRear
};

//统计信息查询线程
class queryStatisticThread : public QThread
{
    Q_OBJECT

public:
    queryStatisticThread();
    ~queryStatisticThread();

    void clearDataMap();        //清空数据结构，避免错误数据绘制
    void setQueryInfo(StatisticType sType,QueryType qType,ViewType vType= MainFront);
    void updateCurrentAriths();                         // 更新当前算法
    void updateCurrentChutes();                         // 更新当前通道
    void getArithName(int);                          // 获取算法语言名称

signals:
    void statisticQueryFinsishedSig(int);

protected:
    void run(void);

public:
    StatisticType m_nStatisticType;
    QueryType m_nQueryType;
    ViewType m_nViewType;

    QList<int> m_dataValueList; //产量查询专用
    QList<QString> m_dataStrList;
    QList<QString> m_dataArithStrList;

//    QList<int> m_nameList;
    QList<QString> m_nameStrList;        //通道号名称
    QList<QString> m_arithNameStrList;   //界面显示算法名称
    QList<QString> m_arithDBNameStrList; //数据库算法字段名称

//    QVector<bool> isQueryedOk;          //上次查询是否成功
    bool isQuerying;
    bool isNeedInserting;
    bool isNeedRepainting;

    int m_dataCount;//通道计数
    int m_arith_dataCount;// 算法计数
    int index ;

    int m_realTimeTotalDataInfo[4];      //为单个含杂或总产量查询时，回传界面显示用 //2020/11/6 修改分别为总产量，总吹气次数
};


class MySqlite : public QObject
{
    Q_OBJECT
public:
    explicit MySqlite(QObject *parent = 0);
    ~MySqlite();
    bool openDatabase();
    void closeDatabase();

    bool creatTable(QString tableName="");
    bool updateTable(QString tableName="");
    bool updateTable(QString tableName,int view, int unit,int type);
        bool updateTable_2(QVector<QString>  tableNamelist,int type);
    bool clearTable(QString tableName="");
    bool deleteTable(QString tableName="");
    bool creatstaticTable(QString tablename,int view,int unit);
    bool getCurItemInfo(QString tableName);
    bool deleteExpiredata(QString tableName, QString value);// 删除过期的统计数据 待删除
    bool clearStatisTable(QString tableName,QString time);
    bool clearStatisTable(QStringList tabellist,QString time);

    //按单视单方案查
    double calculateSingleColumnAverage(QString t_begin,QString t_end,QString coloumnName);
    //按通道单方案查
    double calculateSinglePathAverage(QString t_begin,QString t_end,int index);

    bool isTableExist(QString table);
    bool isItemExist(QString table,QString item);

    QSqlDatabase db;
    QStringList viewName; //视

    //数据更新处理
public:
    void updateStagedThroughout();
    void clearStagedThroughout();

signals:
private:
    QString headName;     //表头
    QString value;        //:表头
};

class getImpurityThread : public QThread
{
    Q_OBJECT
public:
    getImpurityThread() {
        currentview = 0;
        currentpiciname = "ALL";
    }
    QString     seektime;      // 查询时间
    QString     seektimeinTable; // 查询时间+hh
    QString     currentpiciname;
    QString     selectime;
    QString     currenttable;             // 当前操作的模型名称
    int         currentview;              // 当前统计的视
    QStringList picinameList;             // 当前的批次集合名称
    QStringList classNameStrList;         // 模型类型名称
    QStringList bigclassNameStrList;      // 统计类型名称
    QStringList StaticClassNameList;   // 含杂显示的类别名称
    QVector<double> m_onetimeValueVec;     // 一个小时内所有模型类型数据
    QVector<double> m_alltimeValueVec;     // 24小时的所有物料数据集合
    QVector<double> m_totalValueVec;       // 所有模型类型在一天的结果集合
    QVector<double> m_statictotalValueVec;   // 所有统计类型在一天的结果集合
    QVector<double> m_onetimeAreaVec;   // 1小时内模型类型的面积集合 用于计算重量
    QVector<double> m_totalAreaVec;      //所有物料面积在一天的结果集合 用于计算重量
QVector<double> m_statictotalAreaVec; // 所有统计类型在一天的面积集合 用于计算重量
    void getbigclassname();
    void getsmallclassnamelist(int view);
    void getbigclassstaticlist();
    void adddatatovector(int view);
    void getpicinfolistformtable(QString tablename);
    void updatestaticdata(int view);
    void updatesearchdata(int view, int unit);
    bool getdatafromtable(QString tableName,int view, int unit); // 从表中获取数据
    bool getAreadatafromtable(QString tableName,int view, int unit);
protected:
    void run();
signals:
    void sGetClassPointsFinished();
};
extern   getImpurityThread *getThread;
extern queryStatisticThread *myQueryStatisticThread;

#endif // MYSQLITE_H
