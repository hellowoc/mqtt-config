#ifndef QUALITYCHART
#define QUALITYCHART


#include <QWidget>

class QualityChart: public QWidget
{
    Q_OBJECT

public:
    QualityChart(QWidget* parent = NULL);
    ~QualityChart();


    QList<double> m_numList;
    double maxvalue;
     QStringList namelist; // 横坐标名称

    int numtotal;
    QList<double> impuritylist;  // 杂质顺序


    //原点位置
    int x_start ;
    int y_start ;

    //坐标轴长度
    int y_length ;
    int x_length ;

    // 坐标轴宽度
    int axis_width ;
    // 坐标箭头长度
    int arrow_length ;

    // 坐标轴刻度长度
    int scale_length;



public slots:

    void updateData();


protected:
    void paintEvent(QPaintEvent* event);

};

#endif // QUALITYCHART

