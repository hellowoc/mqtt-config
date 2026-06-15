#ifndef IPCCAMERASETWIDGET_H
#define IPCCAMERASETWIDGET_H

#include <basewidget.h>
#include "mylineedit.h"
#include <QLabel>
#include <QComboBox>

namespace Ui {
class IpcCameraSetWidget;
}

class IpcCameraSetWidget : public basewidget
{
    Q_OBJECT

    enum{
        CONST_MAX_CAMERA_PER_VIEW = 12
    };

public:
    explicit IpcCameraSetWidget(QWidget *parent = 0);
    ~IpcCameraSetWidget();
    void showPage(bool bshow);
    void receiveMsg(int msg1, int msg2, int msg3);

    void OnViewChange(int view);
    void OnChuteChange();
    void OnLayerChange();

protected:
    virtual void retranslateUiWidget();
private slots:
    void ipcBtnClicked(int);
    void ipcTypeGroupClicked(int cameraIndex);
    void ipcIpEditClicked(int cameraIndex);
    void on_comboBox_camera_same_activated(int index);
    void ipcSavePicEnabled(int cameraIndex);
    void on_ipcSavePicBtn_clicked();

private:
    Ui::IpcCameraSetWidget *ui;
    QLabel *m_pIpcCamTitle[MAX_VIEW_UNIT];
    myLineEdit *m_pIpcCamPara[MAX_VIEW_UNIT][5];

    QComboBox *m_pIpcTypeCmx[MAX_VIEW_UNIT];
    myLineEdit *m_pIpcIpAddr[MAX_VIEW_UNIT];
    QCheckBox *m_pIpcSavePicEnable[MAX_VIEW_UNIT];

    QButtonGroup *m_pIpcBtnGroup;
    QSignalMapper *m_pIpcTypeGroupSigMapper;
    QSignalMapper *m_pIpcIpEditSigMapper;
    QSignalMapper *m_pIpcSavePicSigMapper;
    int m_curIpcIndex;

    void updateIpcBtn();
    void updateCamState();

    void updateView();
    void updateIpcCameraId();
    int getIpcBasicCamId(int ipcIndex);
};

#endif // IPCCAMERASETWIDGET_H
