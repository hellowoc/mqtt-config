#ifndef MYWAVEWIDGET_H
#define MYWAVEWIDGET_H
/***********************
 *
 * 坐标轴窗口绘制类
 *
 ****************************************************/
#include <QWidget>
enum WavePage{
    Divide_PIXEL,
    Gain_Page,
    BackGround_Page,
    IR_BadPoints,
    IPC_EJECT
};
class MyWaveWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyWaveWidget(QWidget *parent = 0);
    void setDrawType(WavePage type,bool showSrc = 1);
    ~MyWaveWidget();
    void chgAxisYValue();
    void resetAxisYValue();
private:
    void paintAxis(QPainter &painter);                  // 绘制整体的坐标轴
    void paintAxisX(QPainter &painter);                 // 绘制坐标轴x
    void paintAxisY(QPainter &painter);                 // 绘制坐标轴y
    void paintSingleLine();
    void paitDivideAxisX(QPainter &painter);

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

    void setXByData(void);                              // 根据数据设置横轴坐标
    void setYByData(void);                              // 根据数据设置纵轴坐标
    void drawWanted(QPainter &painter);                 // 绘制想要的波形
    void drawColorWave(QPainter &painter);              // 绘制彩色波形
    void drawGrayWave(QPainter &painter);               // 绘制灰度波形
    void drawSingleWave(QPainter &painter,int flag);    // 根据标志位显示单一的波形
    void getNewdata();                                  // 获取下一帧数据
                            // 设置绘制的类型
    void drawEjectTimes(QPainter &painter);             // 绘制吹气次数
    void drawSingleTimes(QPainter &painter, int flag);  // 绘制单视的吹气次数波形
    void setBorderShow(bool bFlag);                     // 设置边界是否显示
    bool getBorderShow(void);                           // 获取边界是否显示
    void setMultiWaveFlag(bool bFlag);                  // 设置是否绘制重叠波形标志位
    void setPixmap(QPixmap pixmap);                     // 设置需要绘制的图像

    void drawDividePix(QPainter &painter);

    void drawIpcEject(QPainter &painter);
    void drawBackgroundValue(QPainter &painter);        // 绘制灰度背景的背景阈值

    void drawIrBadPoint(QPainter &painter);

    int getModifiedChannalXPixel(int pixel);
protected:
    virtual void paintEvent(QPaintEvent *);

signals:

public slots:

private:
    WavePage m_nDrawType;
//    g_Config *config;
    bool m_bDrawMultiWave;
    QPixmap m_pixmap;          // 图像用于保存波形页面的显示
    bool sourceShow;

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
    uchar *redData;
    uchar *greenData;
    uchar *blueData;
    qreal valueXPerPixel;
    qreal valueYPerPixel;

    /* 左边界与右边界位置 */
    int m_nLeft;
    int m_nRight;
    bool m_bDrawBoarder;
    int m_drawSignalType;
};

#endif // MYWAVEWIDGET_H
