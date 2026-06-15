#ifndef IMAGECAPTUREWIDGET_H
#define IMAGECAPTUREWIDGET_H

#include "basewidget.h"

#define MAX_CAP_TYPE 3 //单张,连续，连续同步

namespace Ui {
class ImageCaptureWidget;
}

class ImageCaptureWidget : public basewidget
{
    Q_OBJECT

public:
    explicit ImageCaptureWidget(QWidget *parent = 0);
    ~ImageCaptureWidget();
    void showPage(bool bshow);
    void receiveMsg(int msg1, int msg2, int msg3);
protected:
    void retranslateUiWidget();

private slots:
    void on_captureBtn_clicked();
    void on_analysisBtn_clicked();
    void updateImage(int correctWaveLines,QString filePath);

    void updateContinuousImage(QString filePath);

    void on_pushButton_6_clicked();

    void on_syncCaptureBtn_clicked();

    void on_sequentialCapture_clicked(bool checked);

    void on_pushButton_l_clicked();

    void on_pushButton_r_clicked();

    void on_sequentialSyncCapture_clicked(bool checked);

    void on_checkBox1_clicked(bool checked);

private:
    Ui::ImageCaptureWidget *ui;
    int m_time,m_threshold;
    QString m_curFilePath;
    bool m_autoFeedEnable;
    void updateBtnState();
    int m_curCapType;
    QPushButton *m_pCapBtn[MAX_CAP_TYPE];
    void updateCapBtnShow();
};

#endif // IMAGECAPTUREWIDGET_H
