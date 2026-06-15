#include "aisetdialog.h"
#include "ui_aisetdialog.h"

AiSetDialog::AiSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AiSetDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);

    lineEdit[0][0] = ui->lineEdit_red;
    lineEdit[0][1] = ui->lineEdit_red_2;
    lineEdit[0][2] = ui->lineEdit_red_3;
    lineEdit[0][3] = ui->lineEdit_red_4;
    lineEdit[1][0] = ui->lineEdit_green;
    lineEdit[1][1] = ui->lineEdit_green_2;
    lineEdit[1][2] = ui->lineEdit_green_3;
    lineEdit[1][3] = ui->lineEdit_green_4;
    lineEdit[2][0] = ui->lineEdit_blue;
    lineEdit[2][1] = ui->lineEdit_blue_2;
    lineEdit[2][2] = ui->lineEdit_blue_3;
    lineEdit[2][3] = ui->lineEdit_blue_4;
    lineEdit2[0][0] = ui->lineEdit_rg;
    lineEdit2[0][1] = ui->lineEdit_rg_2;
    lineEdit2[0][2] = ui->lineEdit_rg_3;
    lineEdit2[0][3] = ui->lineEdit_rg_4;
    lineEdit2[1][0] = ui->lineEdit_rb;
    lineEdit2[1][1] = ui->lineEdit_rb_2;
    lineEdit2[1][2] = ui->lineEdit_rb_3;
    lineEdit2[1][3] = ui->lineEdit_rb_4;
    lineEdit2[2][0] = ui->lineEdit_gb;
    lineEdit2[2][1] = ui->lineEdit_gb_2;
    lineEdit2[2][2] = ui->lineEdit_gb_3;
    lineEdit2[2][3] = ui->lineEdit_gb_4;

    lineEdit[3][0] = ui->lineEdit_scale;
    lineEdit[3][1] = ui->lineEdit_scale_2;
    m_lineEdit[0] = ui->lineEdit_hzt;
    m_lineEdit[1] = ui->lineEdit_vtc;

    memset(data, 0,sizeof(data));
    memset(data2, 0,sizeof(data));
    for(int i = 0;i<4;i++){
        if (i< 3){
            data[i][0] = struGsh.svmThreshold[i][0];
            data[i][1] = struGsh.svmThreshold[i][1];
            data[i][2] = struGsh.svmThreshold[i][2];
            data[i][3] = struGsh.svmThreshold[i][3];

            lineEdit[i][0]->setText(QString::number(data[i][0]));
            lineEdit[i][0]->setMinMax(this,0,255,0,&data[i][0]);
            lineEdit[i][1]->setText(QString::number(data[i][1]));
            lineEdit[i][1]->setMinMax(this,0,255,0,&data[i][1]);
            lineEdit[i][2]->setText(QString::number(data[i][2]));
            lineEdit[i][2]->setMinMax(this,0,255,0,&data[i][2]);
            lineEdit[i][3]->setText(QString::number(data[i][3]));
            lineEdit[i][3]->setMinMax(this,0,255,0,&data[i][3]);
        }
        else{
            lineEdit[i][0]->setText(QString::number(data[i][0]));
            lineEdit[i][0]->setMinMax(this,0,65535,0,&data[i][0]);

            lineEdit[i][1]->setText(QString::number(data[i][1]));
            lineEdit[i][1]->setMinMax(this,0,65535,0,&data[i][1]);
        }
    }
    for(int i = 0;i<3;i++){
        data2[i][0] = struGsh.svmDisThreshold[i][0];
        data2[i][1] = struGsh.svmDisThreshold[i][1];
        data2[i][2] = struGsh.svmDisThreshold[i][2];
        data2[i][3] = struGsh.svmDisThreshold[i][3];

        lineEdit2[i][0]->setText(QString::number(data2[i][0]));
        lineEdit2[i][0]->setFloatMinMax(this,0,100.0,0,&data2[i][0],false);
        lineEdit2[i][1]->setText(QString::number(data2[i][1]));
        lineEdit2[i][1]->setFloatMinMax(this,0,100.0,0,&data2[i][1],false);
        lineEdit2[i][2]->setText(QString::number(data2[i][2]));
        lineEdit2[i][2]->setFloatMinMax(this,0,100.0,0,&data2[i][2],false);
        lineEdit2[i][3]->setText(QString::number(data2[i][3]));
        lineEdit2[i][3]->setFloatMinMax(this,0,100.0,0,&data2[i][3],false);
    }

    for(int i = 0;i<2;i++){
        m_data[i] = struGsh.svmSample[i];
        m_lineEdit[i]->setText(QString::number(m_data[i]));
        m_lineEdit[i]->setMinMax(this,0,3,0,&m_data[i]);
    }

    ui->label_red->setText(g_myLan().ids_red);
    ui->label_green->setText(g_myLan().ids_green);
    ui->label_blue->setText(g_myLan().ids_blue);
    ui->label_rb->setText(g_myLan().red_blue);
    ui->label_bg->setText(g_myLan().green_blue);
    ui->label_rg->setText(g_myLan().red_green);
    ui->okBtn->setText(g_myLan().ids_ok);
    ui->label_scale->setText(g_myLan().shape);
    ui->cancelBtn->setText(g_myLan().ids_cancel);

    ui->label_vtc->setText(g_myLan().verticalSpacing);
    ui->label_hzt->setText(g_myLan().horizontalSpacing);
    ui->checkBox->setText(g_myLan().good_bad_differ);
    ui->checkBox_2->setText(g_myLan().edge_cut);
    ui->checkBox_3->setText(g_myLan().rm_redundancy);

    ui->checkBox->setChecked(struGsh.m_bGoodBadDiff);
    ui->checkBox_2->setChecked(struGsh.m_bEdge);
    ui->checkBox_3->setChecked(struGsh.m_bRMRedtmdancy);
    ui->groupBox->setTitle(g_myLan().ai_good);
    ui->groupBox_2->setTitle(g_myLan().ai_bad);

    ui->checkBox_svm1->setText(g_myLan().svm_type1);
    ui->checkBox_svm2->setText(g_myLan().svm_type2);
    ui->checkBox_svm3->setText(g_myLan().svm_type3);
    ui->checkBox_svm1->setChecked(struCnfg.n_svmType == color_normal);
    ui->checkBox_svm2->setChecked(struCnfg.n_svmType == color_advanced);
    ui->checkBox_svm3->setChecked(struCnfg.n_svmType == color_similarity);


    svmTypeList.append(ui->checkBox_svm1);
    svmTypeList.append(ui->checkBox_svm2);
    svmTypeList.append(ui->checkBox_svm3);
    svmTypeSignalMap = new QSignalMapper(this);
    for(int i =0; i < 3; i++){
        svmTypeSignalMap->setMapping(svmTypeList.at(i),i);
        connect(svmTypeList.at(i),SIGNAL(clicked(bool)),svmTypeSignalMap,SLOT(map()));
    }
    connect(svmTypeSignalMap,SIGNAL(mapped(int)),this,SLOT(updateSvmTypeChecked(int)));
}

AiSetDialog::~AiSetDialog()
{
    delete ui;
}

void AiSetDialog::receiveMsg(int msg1, int msg2, int msg3)
{


}

void AiSetDialog::on_cancelBtn_clicked()
{
    reject();
}

void AiSetDialog::on_okBtn_clicked()
{
    for(int i = 0;i<4;i++){
        struGsh.svmThreshold[i][0] = data[i][0];
        struGsh.svmThreshold[i][1] = data[i][1];
        struGsh.svmThreshold[i][2] = data[i][2];
        struGsh.svmThreshold[i][3] = data[i][3];
    }
    for(int i = 0;i<3;i++){
        struGsh.svmDisThreshold[i][0] = data2[i][0];
        struGsh.svmDisThreshold[i][1] = data2[i][1];
        struGsh.svmDisThreshold[i][2] = data2[i][2];
        struGsh.svmDisThreshold[i][3] = data2[i][3];
    }

    for(int i = 0;i<2;i++){
        struGsh.svmSample[i] = m_data[i];
    }
    struGsh.m_bGoodBadDiff = ui->checkBox->isChecked();
    struGsh.m_bEdge = ui->checkBox_2->isChecked();
    struGsh.m_bRMRedtmdancy = ui->checkBox_3->isChecked();

    accept();
}

void AiSetDialog::updateSvmTypeChecked(int index)
{
    if (index >=3) struCnfg.n_svmType = 0;
    for(int i = 0; i< 3; i++){
        svmTypeList.at(i)->setChecked(i == index);
    }
    struCnfg.n_svmType = svm_type(index);
}
