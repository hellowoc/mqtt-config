#ifndef PIECHART_H
#define PIECHART_H

#include <QMap>
#include <QWidget>
#include <QTimer>
class Piechart : public QWidget
{
    Q_OBJECT

public:
    Piechart(QWidget* parent = NULL);
    ~Piechart();

protected:
    void paintEvent(QPaintEvent* event) final;

    QTimer *time1;

public slots:

    void updatewidget();

public:
    QMap<QString, int> m_answerMap;
    QStringList namelist;   // 名称列
    QList<double> m_numList; // 具体各类型含杂数据


    QList<float> resultList; // 结果百分比数据
    QList<int>  numindex; // 饼状图含杂下标，
    QList<int> zhifangindex;  // 值方图含杂下标，
    QList<QColor> colorList;
    int impuritynum  = 0; // 含杂种类个数
    QList<QString> m_answerStrList; // 饼状图显示的杂质名称
    int m_nPeopleCount = 0;
    qreal m_pi = 3.1415;
    int flag = 0;
    double resultsum;

};
#endif
