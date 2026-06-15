#include "pixeladjust.h"
#include "ui_pixeladjust.h"
#include "global.h"
PixelAdjust::PixelAdjust(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::PixelAdjust)
{
    ui->setupUi(this);
    m_pViewBtn[0] = ui->pushButton;
    m_pViewBtn[1] = ui->pushButton_2;
    m_pViewBtn[2] = ui->pushButton_3;
    m_pViewBtn[3] = ui->pushButton_4;
    m_pViewBtn[4] = ui->pushButton_5;
    m_pViewBtn[5] = ui->pushButton_6;
    m_pViewBtn[6] = ui->pushButton_7;
    m_pViewBtn[7] = ui->pushButton_8;
    m_pLayerBtn[0] = ui->mainBtn;
    m_pLayerBtn[1] = ui->viceBtn;

    m_edit[0] = ui->lineEdit_R;
    m_edit[1] = ui->lineEdit_G;
    m_edit[2] = ui->lineEdit_B;
    m_edit[3] = ui->lineEdit_N;

    m_label[0] = ui->label_R;
    m_label[1] = ui->label_G;
    m_label[2] = ui->label_B;
    m_label[3] = ui->label_N;

    m_pushButton[0] = ui->btnR;
    m_pushButton[1] = ui->btnG;
    m_pushButton[2] = ui->btnB;
    m_pushButton[3] = ui->btnN;
    m_pChuteWidget = ui->chuteWidget;
    setSlotConnect();
}

PixelAdjust::~PixelAdjust()
{
    delete ui;
}

void PixelAdjust::showPage(bool bshow)
{
    basewidget::showPage(bshow);
    if(bshow){
        updateSetValue();
        updateEditVisible();
    }
}

void PixelAdjust::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_DATA_CHANGED){

        if(msg2 == chute_change){
            basewidget::receiveMsg(msg1,msg2,msg3);
        }
        else
        {
           myFlow.resetCameraPixelCalibrateRG(false, 0,struGsh.nView,struGsh.nUnit);
        }


    }
}
void PixelAdjust::OnChuteChange()
{
    updateSetValue();
    updateEditVisible();
}

void PixelAdjust::OnViewChange(int view)
{
    updateSetValue();
    updateEditVisible();
}

void PixelAdjust::OnLayerChange()
{
    updateSetValue();
    updateEditVisible();
}

void PixelAdjust::retranslateUiWidget()
{
    ui->label_R->setText(g_myLan().ids_red);
    ui->label_G->setText(g_myLan().ids_green);
    ui->label_B->setText(g_myLan().ids_blue);
    ui->label_N->setText(g_myLan().grey);

    ui->btnR->setText(g_myLan().ids_run);
    ui->btnG->setText(g_myLan().ids_run);
    ui->btnB->setText(g_myLan().ids_run);
    ui->btnN->setText(g_myLan().ids_run);

    ui->radioButton->setText(g_myLan().show_ori_signal);
    ui->radioButton_2->setText(g_myLan().show_bright_signal);
    ui->radioButton_3->setText(g_myLan().show_dark_signal);
}

void PixelAdjust::updateSetValue()
{
    stru_view_camera_Info &cameraInfo = struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView];
    ui->lineEdit_R->setMinMax(this,-15,15,0,&cameraInfo.nCalibratePixelRedVert[struGsh.nUnit]);
    ui->lineEdit_G->setMinMax(this,-15,15,1,&cameraInfo.nCalibratePixelGreenVert[struGsh.nUnit]);
    ui->lineEdit_B->setMinMax(this,-15,15,2,&cameraInfo.nCalibratePixelBlueVert[struGsh.nUnit]);
    ui->lineEdit_N->setMinMax(this,-15,15,3,&cameraInfo.nCalibratePixelNirVert[struGsh.nUnit]);


    ui->lineEdit_R->setText(QString::number(cameraInfo.nCalibratePixelRedVert[struGsh.nUnit]));
    ui->lineEdit_G->setText(QString::number(cameraInfo.nCalibratePixelGreenVert[struGsh.nUnit]));
    ui->lineEdit_B->setText(QString::number(cameraInfo.nCalibratePixelBlueVert[struGsh.nUnit]));
    ui->lineEdit_N->setText(QString::number(cameraInfo.nCalibratePixelNirVert[struGsh.nUnit]));

}

void PixelAdjust::updateEditVisible()
{
    int dim = struCnfp.nSensorDim[0][struGsh.nView];

    for(int i = 0;i < 4;i++){
        m_edit[i]->setVisible(i < dim);
        m_label[i]->setVisible(i < dim);
        m_pushButton[i]->setVisible(false);
    }
}

/* 显示正常波形 */
void PixelAdjust::on_radioButton_clicked()
{
    if(struGsh.nSignalDispType != 0)
    {
        struGsh.nSignalDispType = 0;
        myFlow.resetCameraWaveType(struGsh.nLayer,struGsh.nView,struGsh.nUnit);
    }
}

/* 显示明场校正数据 */
void PixelAdjust::on_radioButton_2_clicked()
{
    if(struGsh.nSignalDispType != 1)
    {
        struGsh.nSignalDispType = 1;
        myFlow.resetCameraWaveType(struGsh.nLayer,struGsh.nView,struGsh.nUnit);
    }
}

/* 显示暗场校正数据 */
void PixelAdjust::on_radioButton_3_clicked()
{
    if(struGsh.nSignalDispType != 2)
    {
        struGsh.nSignalDispType = 2;
        myFlow.resetCameraWaveType(struGsh.nLayer,struGsh.nView,struGsh.nUnit);
    }
}
