#ifndef BARCHART_H
#define BARCHART_H

#include <QWidget>
#include <QTime>
#include <QSqlQuery>
#include <QMap>
#include "data/mysqlite.h"

#define LENGTH 500
#define INTERVAL 300 //存储间隔为300s
#define MAX_ARITH_COUNT 16
#define MAX_DATA_COUNT  365
enum BarPage
{
    //算法
    grey_a,
    grey_b,
    discolor_a,
    discolor_b,
    cross,
    svm_a,
    svm_b,
    big_small,
    long_short,
    circle_long,

    NCurveData
};

enum dataType
{
    hsv_h = 0,hsv_s,hsv_v,rgb_r,rgb_g,rgb_b,discolor_rg,discolor_rb,discolor_gb,matRGB_r,matRGB_g,matRGB_b
};

class BarChart: public QWidget
{
    Q_OBJECT

public:
    explicit BarChart(QWidget *parent = 0);
    void setDrawType(StatisticType type,QueryType type1,ViewType typ2);
    void setDrawType(dataType type ,int dataIndex);
    void getNewdata(StatisticType type);
    ~BarChart();

    void setXByData(int value);                              // 根据数据设置横轴坐标
    void setYByData(int value);                              // 根据数据设置纵轴坐标
private:
    void paintAxis(QPainter &painter);                  // 绘制整体的坐标轴
    void paintAxisX(QPainter &painter);                 // 绘制坐标轴x
    void paintAxisY(QPainter &painter);                 // 绘制坐标轴y
    void paintSingleLine();

    void setMinValueX(int value);
    void setMaxValueX(int value);
    void setMinValueY(int value);
    void setMaxValueY(int value);
    void setBorder(int left, int right, int up, int down);      // 设置坐标轴的边界
    int getLeftBorder(void);                            // 获取左边界
    int getRightBorder(void);                           // 获取右边界
    int getUpBorder(void);                              // 获取上边界
    int getDownBorder(void);                            // 获取下边界
    void setTickAttr(void);                             // 设置标记属性
    void setTickNum(void);                              // 设置标记代表的值
    void setTickValue(void);                            // 设置标记的数量

    void drawWanted(QPainter &painter);                 // 绘制想要的波形

    void setBorderShow(bool bFlag);                     // 设置边界是否显示
    bool getBorderShow(void);                           // 获取边界是否显示

    void drawBackgroundValue(QPainter &painter);        // 绘制灰度背景的背景阈值

    void updateSeekTime();                              // 更新查询时间

protected:
    virtual void paintEvent(QPaintEvent *);

signals:

public slots:

private:
    StatisticType m_nDrawStatisticType;
    QueryType m_nDrawQueryType;
    ViewType m_nDrawViewType;

    /* 绘制坐标轴类的参数 */
    QFont font;
    int minValueX;
    int maxValueX;
    int minValueY;
    int maxValueY;
    quint8 leftBorder;
    quint8 rightBorder;
    quint8 upBorder;
    quint8 downBorder;
    int minorTickValueX;
    int minorTickValueY;
    int majorTickValueX;
    int majorTickValueY;
    int majorXNum;
    int majorYNum;
    int minorXNum;
    int minorYNum;

    qreal valueXPerPixel;
    qreal valueYPerPixel;

    /* 左边界与右边界位置 */
    int m_nLeft;
    int m_nRight;
    bool m_bDrawBoarder;
    dataType m_type;
    int m_dataIndex;
    QString beginTime,endTime;      //起始查询时间
};



#endif // BARCHART_H
