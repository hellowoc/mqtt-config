#ifndef SELECTUPDATEFILEWIDGET_H
#define SELECTUPDATEFILEWIDGET_H

#include "basewidget.h"
#include <QListWidget>
namespace Ui {
class SelectUpdateFileWidget;
}

class SelectUpdateFileWidget : public basewidget
{
    Q_OBJECT

public:
    explicit SelectUpdateFileWidget(QWidget *parent = 0);
    ~SelectUpdateFileWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);

protected:
    void retranslateUiWidget();

private slots:
    void on_loadFileBtn_clicked();

    void on_applyBtn_clicked();

    void on_startUpdateBtn_clicked();

    void on_tarUpdateFileBtn_clicked();

private:
    Ui::SelectUpdateFileWidget *ui;
    QListWidget *m_plistWidget;
    const QString specificFolder = "update_packet";


    void listCompressedFiles(const QString &directoryPath);
    void updateCurUpdateFile();
    bool checkFilesSize(const QString& path); //判断文件大小
    void tarUpdateFile();
};

#endif // SELECTUPDATEFILEWIDGET_H
