#ifndef SYSOUTPUTWIDGET_H
#define SYSOUTPUTWIDGET_H

#include "basewidget.h"
#include "mysortwidget/mylineedit.h"



namespace Ui {
class SysOutputWidget;
}


class SysOutputWidget : public basewidget
{
    Q_OBJECT

public:

    explicit SysOutputWidget(QWidget *parent = 0);

    ~SysOutputWidget();

    int year;
    int month;
    int day;
    int currenshowtype; // 当前界面显示类型 0 按照当天时间  1 直方图 2 饼状图
    int currentview;
    myLineEdit *m_dateTimeWidget[3];
    QString     seektime;      // 查询时间
    QString     selectime;
    QString     currenttable;             // 当前操作的模型名称
    QStringList picinameList;             // 当前的批次集合名称
    QStringList viewnameList;             //
    QMap<QString,int> viewMap;

    void showPage(bool bshow);

    void retranslateUiWidget();

    void receiveMsg(int msg1, int msg2, int msg3);

    void updatepicinfolist();

    void updateshowpage();

    void getseektime();

    void getcurrentpicinameandinfo();

public slots:
    void updateshowdata();  // 更新数据界面显示
private slots:

    void on_comboBox_activated(int index);

    void on_search_clicked();

    void on_classSet_clicked();

    void on_comboBoxtype_activated(int index);

    void on_exportdataBtn_clicked();

    void uploadMulScreendata();  // 上传数据到质控屏

    void on_comboBox_2_activated(int index);

    void on_comboBox_view_activated(int index);

    void on_deletedataBtn_clicked();

    void on_comboBox_view_activated(const QString &arg1);

private:
    Ui::SysOutputWidget *ui;

};



#endif // SYSVERSIONINFOWIDGET_H
