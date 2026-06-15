#ifndef IMAGEFASTCAPTUREWIDGET_H
#define IMAGEFASTCAPTUREWIDGET_H

#include "basewidget.h"

#define MAX_CAP_TYPE 3 //单张，同步,目标

namespace Ui {
class ImageFastCaptureWidget;
}

class ImageFastCaptureWidget : public basewidget
{
    Q_OBJECT

public:
    explicit ImageFastCaptureWidget(QWidget *parent = 0);
    ~ImageFastCaptureWidget();
    void showPage(bool bshow);
    void receiveMsg(int msg1, int msg2, int msg3);
    void setlrBtnEnabled(bool stat);
protected:
    void retranslateUiWidget();

private slots:
    void on_captureBtn_clicked(bool checked);

    void on_syncCaptureBtn_clicked(bool checked);

    void updateImage(QStringList ,QStringList );

    void on_pushButton_l_clicked();

    void on_pushButton_r_clicked();

    void on_checkBox1_clicked(bool checked);

    void doImageRecvFull(int,int);

    void doFRImageRecvOver();

//    void doImageRecvTimeout();

    void on_analysisBtn_clicked();

    void on_targetCaptureBtn_clicked(bool checked);

    void on_checkBox_clicked(bool checked);

private:
    Ui::ImageFastCaptureWidget *ui;
    int m_time,m_threshold;
    QString m_curFilePath;
    bool m_autoFeedEnable;
    void updateBtnState();
    int m_curCapType;
    QPushButton *m_pCapBtn[MAX_CAP_TYPE];
    void updateCapBtnShow();
};

#endif // IMAGEFASTCAPTUREWIDGET_H
