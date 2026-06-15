#ifndef HSVCOLORCIRCLE_H
#define HSVCOLORCIRCLE_H
#define S_V_MAX 255
#include <QWidget>
class HsvColorCircle : public QWidget
{
    Q_OBJECT

public:
    HsvColorCircle(QWidget* parent = NULL);
    ~HsvColorCircle();

    enum SelectedMode
    {
        Normal,     // 未选中状态
        Circle,     // 圆环选中状态
        Rect        // 矩形区域选中状态
    };

    // 设置/获取当前颜色
    QColor getCurrentColor(void);
    void setCurrentColor(const QColor& color);
    void setHsv(int h1,int s1,int v1,int h2,int s2,int v2);
    void getHsv(int &h1,int &s1,int &v1,int &h2,int &s2,int &v2);
protected:
    void paintEvent(QPaintEvent* event) final;

    void customPaint(QPainter* painter);
    void resizeEvent(QResizeEvent* event) override;
    QSize sizeHint() const override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    int m_nCircleWidth = 30;
    int m_nHValue = 0;
    int m_nHValueLow;
    int m_nHValueUpp;
    int m_nSValueLow;
    int m_nSValueUpp;
    int m_nVValueLow;
    int m_nVValueUpp;
    QVector<QColor> m_colorVec;
    SelectedMode m_selectMode = Normal;
    QColor m_currentColor;

    // 绘制中心矩形
    void drawCenterRect(const QRect& rect, QPainter* painter);
    // 获取两点之间的距离
    qreal distance(QPoint p1, QPoint p2);
    // 根据点的位置或者角度
    int getHValue(QPoint pos);
    // 更新当前颜色值显示
    void updateCurrentColor(QPoint pos);

    bool m_bupper;
    bool m_bMove;
signals:
    void colorChanged(void);
};
#endif
