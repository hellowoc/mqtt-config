#include "setsvmusedialog.h"
#include "ui_setsvmusedialog.h"
#include "constant.h"
#include "global.h"
SetSvmUseDialog::SetSvmUseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetSvmUseDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);

    m_pTimesLbl[0] = ui->timesLbl1;
    m_pTimesLbl[1] = ui->timesLbl2;
    m_pTimesLbl[2] = ui->timesLbl3;
    m_pTimesLbl[3] = ui->timesLbl4;
    m_pTimesLbl[4] = ui->timesLbl5;

    m_pViewLbl[0] = ui->viewLbl1;
    m_pViewLbl[1] = ui->viewLbl2;
    m_pViewLbl[2] = ui->viewLbl3;
    m_pViewLbl[3] = ui->viewLbl4;
    m_pViewLbl[4] = ui->viewLbl1_2;
    m_pViewLbl[5] = ui->viewLbl2_2;
    m_pViewLbl[6] = ui->viewLbl3_2;
    m_pViewLbl[7] = ui->viewLbl4_2;

    m_pUseCheckbox[0][0] = ui->checkBoxfm1;
    m_pUseCheckbox[0][1] = ui->checkBoxfm2;
    m_pUseCheckbox[0][2] = ui->checkBoxfm3;
    m_pUseCheckbox[0][3] = ui->checkBoxfm4;
    m_pUseCheckbox[0][4] = ui->checkBoxfm5;
    m_pUseCheckbox[1][0] = ui->checkBoxrm1;
    m_pUseCheckbox[1][1] = ui->checkBoxrm2;
    m_pUseCheckbox[1][2] = ui->checkBoxrm3;
    m_pUseCheckbox[1][3] = ui->checkBoxrm4;
    m_pUseCheckbox[1][4] = ui->checkBoxrm5;
    m_pUseCheckbox[2][0] = ui->checkBoxfv1;
    m_pUseCheckbox[2][1] = ui->checkBoxfv2;
    m_pUseCheckbox[2][2] = ui->checkBoxfv3;
    m_pUseCheckbox[2][3] = ui->checkBoxfv4;
    m_pUseCheckbox[2][4] = ui->checkBoxfv5;
    m_pUseCheckbox[3][0] = ui->checkBoxrv1;
    m_pUseCheckbox[3][1] = ui->checkBoxrv2;
    m_pUseCheckbox[3][2] = ui->checkBoxrv3;
    m_pUseCheckbox[3][3] = ui->checkBoxrv4;
    m_pUseCheckbox[3][4] = ui->checkBoxrv5;

    m_pUseCheckbox[4][0] = ui->checkBoxfm1_2;
    m_pUseCheckbox[4][1] = ui->checkBoxfm2_2;
    m_pUseCheckbox[4][2] = ui->checkBoxfm3_2;
    m_pUseCheckbox[4][3] = ui->checkBoxfm4_2;
    m_pUseCheckbox[4][4] = ui->checkBoxfm5_2;
    m_pUseCheckbox[5][0] = ui->checkBoxrm1_2;
    m_pUseCheckbox[5][1] = ui->checkBoxrm2_2;
    m_pUseCheckbox[5][2] = ui->checkBoxrm3_2;
    m_pUseCheckbox[5][3] = ui->checkBoxrm4_2;
    m_pUseCheckbox[5][4] = ui->checkBoxrm5_2;
    m_pUseCheckbox[6][0] = ui->checkBoxfv1_2;
    m_pUseCheckbox[6][1] = ui->checkBoxfv2_2;
    m_pUseCheckbox[6][2] = ui->checkBoxfv3_2;
    m_pUseCheckbox[6][3] = ui->checkBoxfv4_2;
    m_pUseCheckbox[6][4] = ui->checkBoxfv5_2;
    m_pUseCheckbox[7][0] = ui->checkBoxrv1_2;
    m_pUseCheckbox[7][1] = ui->checkBoxrv2_2;
    m_pUseCheckbox[7][2] = ui->checkBoxrv3_2;
    m_pUseCheckbox[7][3] = ui->checkBoxrv4_2;
    m_pUseCheckbox[7][4] = ui->checkBoxrv5_2;

    m_pTimesLbl[0]->setText(g_myLan().ids_one);
    m_pTimesLbl[1]->setText(g_myLan().ids_two);
    m_pTimesLbl[2]->setText(g_myLan().ids_three);
    m_pTimesLbl[3]->setText(g_myLan().ids_four);
    m_pTimesLbl[4]->setText(g_myLan().ids_five);

    m_pViewLbl[0]->setText(g_myLan().ids_fm);
    m_pViewLbl[1]->setText(g_myLan().ids_bm);
    m_pViewLbl[2]->setText(g_myLan().ids_fs);
    m_pViewLbl[3]->setText(g_myLan().ids_bs);

    ui->checkBox_svm1->setText("SVMA");
    ui->checkBox_svm2->setText("SVMB");
    ui->checkBox_svm3->setText("SVMC");
    ui->checkBox_svm4->setText("SVMD");
    ui->checkBox_shapesvm1->setText(g_myLan().shape+g_myLan().arith_svm_a);
    ui->checkBox_shapesvm2->setText(g_myLan().shape+g_myLan().arith_svm_b);
    ui->checkBox_shapesvm3->setText(g_myLan().shape+g_myLan().arith_svm_c);

    ui->cancelBtn->setText(g_myLan().ids_cancel);
    ui->okBtn->setText(g_myLan().ids_ok);

    for(int view = 0;view < MAX_VIEW;view++){
        m_pViewLbl[view]->hide();
        for(int times = 0;times < MAX_GROUP_IDENTIFY;times++){
            m_pTimesLbl[times]->hide();
            m_pUseCheckbox[view][times]->hide();
        }
    }

    for(int view = 0;view < MAX_VIEW;view++){
        if(struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[view]){
            m_pViewLbl[view]->show();
            for(int times = 0;times < struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[view].nIdentifyGroupTotal;times++){
                m_pUseCheckbox[view][times]->show();
                m_pTimesLbl[times]->show();
            }
        }
    }

    ui->checkBox_shapesvm1->setVisible(struGsh.m_bAutoStudy[1]||struGsh.m_bManulStudy[1]);
    ui->checkBox_shapesvm2->setVisible(struGsh.m_bAutoStudy[1]||struGsh.m_bManulStudy[1]);
    ui->checkBox_shapesvm3->setVisible(struGsh.m_bAutoStudy[1]||struGsh.m_bManulStudy[1]);
    ui->checkBox_svm1->setVisible(struGsh.m_bAutoStudy[0]||struGsh.m_bManulStudy[0]);
    ui->checkBox_svm2->setVisible(struGsh.m_bAutoStudy[0]||struGsh.m_bManulStudy[0]);
    ui->checkBox_svm3->setVisible(struGsh.m_bAutoStudy[0]||struGsh.m_bManulStudy[0]);
    ui->checkBox_svm4->setVisible(struGsh.m_bAutoStudy[0]||struGsh.m_bManulStudy[0]);

    if(struCnfg.nMachineType == MACHINE_LD2_ZL_GP){
        m_pViewLbl[0]->setText(g_myLan().lower_layer+g_myLan().ids_fm);
        m_pViewLbl[1]->setText(g_myLan().lower_layer+g_myLan().ids_bm);
        m_pViewLbl[2]->setText(g_myLan().lower_layer+g_myLan().ids_fs);
        m_pViewLbl[3]->setText(g_myLan().lower_layer+g_myLan().ids_bs);
        m_pViewLbl[4]->setText(g_myLan().upper_layer+g_myLan().ids_fm);
        m_pViewLbl[5]->setText(g_myLan().upper_layer+g_myLan().ids_bm);
        m_pViewLbl[6]->setText(g_myLan().upper_layer+g_myLan().ids_fs);
        m_pViewLbl[7]->setText(g_myLan().upper_layer+g_myLan().ids_bs);
    }
}

SetSvmUseDialog::~SetSvmUseDialog()
{
    delete ui;
}
void SetSvmUseDialog::on_cancelBtn_clicked()
{
    reject();
}

void SetSvmUseDialog::on_okBtn_clicked()
{
    for(int view = 0;view < MAX_VIEW;view++){
        if(struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[view]){
            for(int times = 0;times < struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[view].nIdentifyGroupTotal;times++){
                if(m_pUseCheckbox[view][times]->isVisible() && m_pUseCheckbox[view][times]->isChecked()){
                    if(ui->checkBox_svm1->isChecked()){
                        myFlow.setSvmParas(0,view,times,0);
                        myFlow.resetMaterial(struGsh.nLayer,view,times,false,ARITH_SVM_A,0);
                    }
                    if(ui->checkBox_svm2->isChecked()){
                        myFlow.setSvmParas(0,view,times,1);
                        myFlow.resetMaterial(struGsh.nLayer,view,times,false,ARITH_SVM_B,0);
                    }
                    if(ui->checkBox_svm3->isChecked()){
                        myFlow.setSvmParas(0,view,times,2);
                        myFlow.resetMaterial(struGsh.nLayer,view,times,false,ARITH_SVM_C,0);
                    }
                    if(ui->checkBox_svm4->isChecked()){
                        myFlow.setSvmParas(0,view,times,3);
                        myFlow.resetMaterial(struGsh.nLayer,view,times,false,ARITH_SVM_D,0);
                    }
                    if(ui->checkBox_shapesvm1->isChecked()){
                        myFlow.setShapeSvmParas(0,view,times,0);
                        myFlow.resetMaterial(struGsh.nLayer,view,times,false,ARITH_SHAPE_SVM_A,0);
                    }
                    if(ui->checkBox_shapesvm2->isChecked()){
                        myFlow.setShapeSvmParas(0,view,times,1);
                        myFlow.resetMaterial(struGsh.nLayer,view,times,false,ARITH_SHAPE_SVM_B,0);
                    }
                    if(ui->checkBox_shapesvm3->isChecked()){
                        myFlow.setShapeSvmParas(0,view,times,2);
                        myFlow.resetMaterial(struGsh.nLayer,view,times,false,ARITH_SHAPE_SVM_C,0);
                    }
                }
            }
        }
    }

    accept();

}
