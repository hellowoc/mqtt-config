#ifndef HISTOGRAMWIDGET_H
#define HISTOGRAMWIDGET_H

#include <QWidget>
#include <QMap>
#include <QString>

class HistogramWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HistogramWidget(QWidget *parent = 0);

    // 设置两个数据集
    void setData(const QMap<QString, double>& data1, const QMap<QString, double>& data2);
    QMap<QString, double> m_data1;
    QMap<QString, double> m_data2;
protected:
    void paintEvent(QPaintEvent *event) override;

private:

    QStringList m_keys;  // 排序后的所有键
    int m_maxValue;      // 所有数据的最大值
};

#endif // HISTOGRAMWIDGET_H
