#include "histogramwidget.h"
#include <QPainter>
#include <QSet>
#include <QFontMetrics>
#include <algorithm>

HistogramWidget::HistogramWidget(QWidget *parent)
    : QWidget(parent), m_maxValue(0)
{
    setMinimumSize(400, 300);
}

void HistogramWidget::setData(const QMap<QString, double>& data1, const QMap<QString, double>& data2)
{
    m_data1 = data1;
    m_data2 = data2;

    // 合并所有键，保持排序顺序（QMap的keys()已经排序）
    QStringList keys1 = data1.keys();
    QStringList keys2 = data2.keys();
    m_keys.clear();
    QStringList::const_iterator it1 = keys1.begin();
    QStringList::const_iterator it2 = keys2.begin();
    while (it1 != keys1.end() && it2 != keys2.end()) {
        if (*it1 == *it2) {
            m_keys.append(*it1);
            ++it1; ++it2;
        } else if (*it1 < *it2) {
            m_keys.append(*it1);
            ++it1;
        } else {
            m_keys.append(*it2);
            ++it2;
        }
    }
    while (it1 != keys1.end()) {
        m_keys.append(*it1);
        ++it1;
    }
    while (it2 != keys2.end()) {
        m_keys.append(*it2);
        ++it2;
    }
    // 计算所有数据的最大值（用于归一化）
    m_maxValue = 0;
    foreach (const QString& key, m_keys) {
        int val1 = m_data1.value(key, 0);
        int val2 = m_data2.value(key, 0);
        if (val1 > m_maxValue) m_maxValue = val1;
        if (val2 > m_maxValue) m_maxValue = val2;
    }

    if (m_maxValue == 0) m_maxValue = 1; // 避免除零
    m_maxValue += 15;
    update(); // 请求重绘
}

void HistogramWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    if (m_keys.isEmpty()) return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int w = width();
    int h = height();

    // 边距（为坐标轴和标签留出空间）
    const int leftMargin   = 0;
    const int rightMargin  = 20;
    const int topMargin    = 20;
    const int bottomMargin = 50;

    int chartWidth  = w - leftMargin - rightMargin;
    int chartHeight = h - topMargin - bottomMargin;
    if (chartWidth <= 0 || chartHeight <= 0) return;

    // 绘制白色背景
//    painter.fillRect(rect(), Qt::white);

    // 绘制坐标轴
    painter.setPen(Qt::black);
    //painter.drawLine(leftMargin, topMargin, leftMargin, h - bottomMargin);               // 纵轴
    painter.drawLine(leftMargin, h - bottomMargin, w - rightMargin, h - bottomMargin);  // 横轴

    // 绘制纵轴刻度
    int numTicks = 5;
    for (int i = 0; i <= numTicks; ++i) {
        int y = h - bottomMargin - i * chartHeight / numTicks;
        int value = i * m_maxValue / numTicks;
       // painter.drawLine(leftMargin - 5, y, leftMargin, y);
       // painter.drawText(leftMargin - 40, y - 5, QString::number(value));
    }

    // 判断数据集存在情况
    bool hasData1 = !m_data1.isEmpty();
    bool hasData2 = !m_data2.isEmpty();
    if (!hasData1 && !hasData2) return;  // 没有数据

    // 每个键占据的宽度
    int n = m_keys.size();
    double barSlotWidth = static_cast<double>(chartWidth) / n;

    // 柱子宽度和偏移量（根据数据集数量动态调整）
    double barWidth;
    double leftBarX;   // 数据集1的柱子左偏移
    double rightBarX;  // 数据集2的柱子左偏移（仅当两个数据集都存在时使用）

    if (hasData1 && hasData2) {
        // 两个数据集：并排显示，柱子较窄
        barWidth = barSlotWidth * 0.3;
        leftBarX = barSlotWidth * 0.15;
        rightBarX = barSlotWidth * 0.55;
    } else if (hasData1) {
        // 只有数据集1：柱子居中加宽
        barWidth = barSlotWidth * 0.5;
        leftBarX = (barSlotWidth - barWidth) / 2;
    } else { // 只有数据集2
        barWidth = barSlotWidth * 0.5;
        leftBarX = (barSlotWidth - barWidth) / 2; // 仍用leftBarX，但绘制时用数据集2的颜色
    }

    // 准备字体度量，用于计算文本高度
    QFontMetrics fm(painter.font());
    int textHeight = fm.height(); // 单行文本高度

    for (int i = 0; i < n; ++i) {
        const QString& key = m_keys[i];
        double x0 = leftMargin + i * barSlotWidth; // 当前槽的左边缘

        // 绘制数据集1的柱子（蓝色）
        if (hasData1 && m_data1.contains(key)) {
            int val1 = m_data1.value(key, 0);
            if (val1 > 0) {
                double barHeight = static_cast<double>(val1) / m_maxValue * chartHeight;
                QRectF barRect(x0 + leftBarX, h - bottomMargin - barHeight, barWidth, barHeight);
                painter.fillRect(barRect, Qt::blue);

                // 在柱子顶端绘制数值
                double textY = h - bottomMargin - barHeight - textHeight;
                if (textY < topMargin) {
                    textY = h - bottomMargin - barHeight; // 放在柱子内部顶端
                }
                painter.setPen(Qt::black);
                painter.drawText(QRectF(x0 + leftBarX, textY, barWidth, textHeight),
                                 Qt::AlignCenter, QString::number(val1));
            }
        }

        // 绘制数据集2的柱子（红色），注意当只有数据集2时，使用leftBarX位置居中
        if (hasData2 && m_data2.contains(key)) {
            int val2 = m_data2.value(key, 0);
            if (val2 > 0) {
                double barHeight = static_cast<double>(val2) / m_maxValue * chartHeight;
                QRectF barRect(x0 + (hasData1 && hasData2 ? rightBarX : leftBarX),
                               h - bottomMargin - barHeight, barWidth, barHeight);
                painter.fillRect(barRect, Qt::red);

                double textY = h - bottomMargin - barHeight - textHeight;
                if (textY < topMargin) {
                    textY = h - bottomMargin - barHeight;
                }
                painter.setPen(Qt::black);
                painter.drawText(QRectF(x0 + (hasData1 && hasData2 ? rightBarX : leftBarX),
                                        textY, barWidth, textHeight),
                                 Qt::AlignCenter, QString::number(val2));
            }
        }

        // 绘制横轴标签（键名），如果太长则截断
        QString label = key;
        if (label.length() > 8)
            label = label.left(6) + "...";
        painter.setPen(Qt::black);
        painter.drawText(QRectF(x0, h - bottomMargin + 5, barSlotWidth, 20),
                         Qt::AlignCenter, label);
    }

    // 绘制图例
    int legendY = topMargin + 20;
    if (hasData1) {
        painter.setPen(Qt::blue);
        painter.drawText(leftMargin + 10, legendY, "");
        legendY += 20;
    }
    if (hasData2) {
        painter.setPen(Qt::red);
        painter.drawText(leftMargin + 10, legendY, "");
    }
}
