#include "modelselectwidget.h"
#include "ui_modelselectwidget.h"
#include "mylistwidgetitem.h"
#include <QDateTime>
#include "global.h"
#include "renameprofiledialog.h"
#include "messagedialog.h"
#include "myinputmethod.h"
#include "bus/mqttsrv.h"

ModelSelectWidget::ModelSelectWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::ModelSelectWidget)
{
    ui->setupUi(this);
    m_plistWidget = ui->listWidget;
    m_plistWidget->setIconSize(QSize(40,40));
    //    m_pViewBtn[0] = ui->pushButton;
    //    m_pViewBtn[1] = ui->pushButton_2;
    //    m_pViewBtn[2] = ui->pushButton_3;
    //    m_pViewBtn[3] = ui->pushButton_4;
    //    m_pViewBtn[4] = ui->pushButton_5;
    //    m_pViewBtn[5] = ui->pushButton_6;
    //    m_pViewBtn[6] = ui->pushButton_7;
    //    m_pViewBtn[7] = ui->pushButton_8;

    m_pChuteWidget = ui->chuteWidget;
    m_pChuteWidget->hide();
    ui->mainBtn->hide();
    ui->viceBtn->hide();
    //    m_pLayerBtn[0] = ui->mainBtn;
    //    m_pLayerBtn[1] = ui->viceBtn;
    setSlotConnect();
    retranslateUiWidget();

    biglabel[0] = ui->label;
    biglabel[1] = ui->label_2;
    biglabel[2] = ui->label_3;
    biglabel[3] = ui->label_4;
    biglabel[4] = ui->label_5;

    labelname[0] = ui->comboBox;
    labelname[1] = ui->comboBox_2;
    labelname[2] = ui->comboBox_3;
    labelname[3] = ui->comboBox_4;
    labelname[4] = ui->comboBox_5;

    biglabel[0]->setVisible(false);
    biglabel[4]->setVisible(false);
    labelname[0]->setVisible(false);
    labelname[4]->setVisible(false);

    ui->lineEdit->setType(textType);
    struGsh.currentpageid = 1;

    ui->extendBtn->setCheckable(true);//
    timer = new QTimer;
    timer->setSingleShot(true);

    m_classModeSigMapper = new QSignalMapper(this);
    for(int i = 0; i < MAX_MODE_BIGLABLE_NUM; i++){

        m_classModeSigMapper->setMapping(labelname[i], i);
        connect(labelname[i],SIGNAL(activated(int)),m_classModeSigMapper,SLOT(map()));
    }
    connect(m_classModeSigMapper,SIGNAL(mapped(int)),this,SLOT(sClassModeChanged(int)));

#ifdef Q_OS_UNIX
//    connect(myMqttMsgParaseThread,SIGNAL(getModelistResult_sig()),this,SLOT(getModelistResult()));
   // connect(myMqttMsgParaseThread,SIGNAL(uploadEvaluateResult_sig()),this,SLOT(uploadEvaluateResult()));
#endif
    connect(timer, SIGNAL(timeout()) , this, SLOT(getModelistResult()));
}

ModelSelectWidget::~ModelSelectWidget()
{
    delete ui;
}


void ModelSelectWidget::showPage(bool bshow)
{
    basewidget::showPage(bshow);
    if(bshow){

        struGsh.selectDownloadModel = 1;
         ui->extendBtn->setChecked(false);

        QStringList templist ;
        for(int i =0;  i < MAX_MODE_BIGLABLE_NUM ; i++)
        {

            labelname[i]->clear();
            if(i >= struCnfg.stumodeLableList.labelnum)
                break;

            biglabel[i]->setText(QString("%1").arg(struCnfg.stumodeLableList.labelname[i]));
            templist.clear();
            templist.append("");
            for(int j = 0; j < MAX_MODELABLE_NUM ; j++)
            {
                if(j >= struCnfg.stumodeLableList.labelinfo[i].labelnum)
                    break;
                templist.append(QString("%1").arg(struCnfg.stumodeLableList.labelinfo[i].labelname[j]));
            }
            labelname[i]->addItems(templist);

        }

        updatelabelname();

        on_extendBtn_clicked(false);
        m_plistWidget->setCurrentRow(struGsh.stumodelData.checkindex);


    }
    else
    {

          struGsh.selectDownloadModel = 0;

    }
}
void ModelSelectWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_DATA_CHANGED){
        getmodellist();
    }
}

void ModelSelectWidget::OnViewChange(int view)
{
    //updateListWidget();
}

void ModelSelectWidget::OnChuteChange()
{
    //updateListWidget();
}

void ModelSelectWidget::OnLayerChange()
{
    // updateListWidget();
}

void ModelSelectWidget::retranslateUiWidget()
{


    ui->recheckBtn->setText(g_myLan().ids_query);

    ui->downloadBtn->setText(g_myLan().download);
    ui->introducBtn->setText(g_myLan().introduction);
    ui->pushButton_3->setText(g_myLan().Thumb_up);

    ui->label_7->setText(g_myLan().current_page);
}


void ModelSelectWidget::sClassModeChanged(int index)
{
    struCnfg.stumodeLableSelect.labelindex[index] = labelname[index]->currentIndex(); // 从1开始
}

void ModelSelectWidget::getModelistResult()
{


    timer->stop();
    g_infoWidget().hide();


    if(struGsh.updateModelistresult == 0)
    {
        g_infoWidget().setLabelText( g_myLan().ids_requestFailed);
        g_infoWidget().delayShow();

        myFlow.sleep(2);
        g_infoWidget().hide();
    }

    updateListWidget();
    updateBtn();


}


void ModelSelectWidget::uploadEvaluateResult()
{


    timer->stop();
    g_infoWidget().hide();


    if(struGsh.updateModelistresult == 0)
    {
        g_infoWidget().setLabelText(g_myLan().ids_requestFailed);
        g_infoWidget().delayShow();

        myFlow.sleep(2);
        g_infoWidget().hide();
    }

    updateListWidget();
    updateBtn();


}
void ModelSelectWidget::updatelabelname()
{

    for(int i =0; i < MAX_MODE_BIGLABLE_NUM; i++)
    {
        biglabel[i]->setText(QString("%1").arg(struCnfg.stumodeLableList.labelname[i]));
        labelname[i]->setCurrentIndex(struCnfg.stumodeLableSelect.labelindex[i]);
    }
    ui->lineEdit->setText(QString("%1").arg(struCnfg.stumodeLableSelect.keyword));


}



void ModelSelectWidget::updateListWidget()
{

    int count = m_plistWidget->count();
    for(int i = 0;i< count;i++){
        delete m_plistWidget->takeItem(0);
    }
    m_plistWidget->clear();
    m_curModelIndex = 0;

    for(int i = 0; i < struGsh.stumodelData.currentmodelnum; i++){
        QByteArray arrayName(struGsh.stumodelData.modelname[i]);

        QListWidgetItem *schemeListItem = new QListWidgetItem(QString::fromUtf8(arrayName));
        m_plistWidget->addItem(schemeListItem);
    }

}

void ModelSelectWidget::getmodellist()
{


    memset(&struGsh.stumodelData, 0 , sizeof(struGsh.stumodelData));
    struGsh.updateModelistresult = 0;
    timer->start(20000);

#ifdef Q_OS_UNIX
//    myMqttMsgParaseThread->n_getModelListFlag = 1;
#endif

    g_infoWidget().setLabelText(g_myLan().msg_applying);
    g_infoWidget().delayShow();


}


void ModelSelectWidget::updateBtn()
{

    int tmp = struGsh.stumodelData.totalmodelnum%MAX_MODE_NUM_LIST ;
    if (tmp == 0)
        m_nTotalPage = struGsh.stumodelData.totalmodelnum/MAX_MODE_NUM_LIST;
    else
        m_nTotalPage = struGsh.stumodelData.totalmodelnum/MAX_MODE_NUM_LIST + 1;

    ui->lineEdit_2->setMinMax(this,1,m_nTotalPage,0,&struGsh.currentpageid);
    ui->lineEdit_2->setText(QString::number(struGsh.currentpageid));


    qDebug()<<struGsh.currentpageid;
    QString str = QString("%1/%2").arg(struGsh.currentpageid).arg(m_nTotalPage);
    ui->label_page->setText(str);


}

void ModelSelectWidget::on_recheckBtn_clicked()
{
    struGsh.currentpageid = 1;
    getmodellist();

//    updateListWidget();
//    updateBtn();

}

void ModelSelectWidget::on_downloadBtn_clicked()
{


    struGsh.stumodelData.checkindex = ui->listWidget->currentRow();
    qDebug()<<QString("%1").arg(struGsh.stumodelData.modelpath[struGsh.stumodelData.checkindex]);
#ifdef Q_OS_UNIX

//    myMqttMsgParaseThread->n_downLoadFlag = 1;
#endif


}

void ModelSelectWidget::on_btn_prePage_clicked()
{
    if (struGsh.currentpageid >= 2) {
        struGsh.currentpageid--;

    }
getmodellist();

//    updateListWidget();
//    updateBtn();

}

void ModelSelectWidget::on_btn_nextPage_clicked()
{
    if (struGsh.currentpageid < (m_nTotalPage)) {
        struGsh.currentpageid++;

    }
   getmodellist();
//    updateListWidget();
//    updateBtn();

}

void ModelSelectWidget::on_lineEdit_textChanged(const QString &arg1)
{

    memset(&struCnfg.stumodeLableSelect.keyword,0,128);

    qDebug()<<arg1;
    QByteArray namearr = arg1.toUtf8();
    const char *strName1 = namearr.data();
    memcpy(struCnfg.stumodeLableSelect.keyword,strName1,strlen(strName1));

}



void ModelSelectWidget::on_introducBtn_clicked()
{
    struGsh.stumodelData.checkindex = ui->listWidget->currentRow();
    qDebug()<<QString("%1").arg(struGsh.stumodelData.modelinstruct[struGsh.stumodelData.checkindex]);
    QString instructinfo = QString("%1").arg(struGsh.stumodelData.modelinstruct[struGsh.stumodelData.checkindex]);

    if(instructinfo == "")
        instructinfo = g_myLan().No_introduction;
    g_infoWidget().setLabelText(instructinfo);
    g_infoWidget().showSecs(5);

}

void ModelSelectWidget::on_pushButton_3_clicked()
{

    struGsh.stumodelData.checkindex = ui->listWidget->currentRow();

#ifdef Q_OS_UNIX
//    myMqttMsgParaseThread->n_uploadModelEvaluate = 1;
#endif

    g_infoWidget().setLabelText(g_myLan().ids_succeeded);
    g_infoWidget().showSecs(2);
}

void ModelSelectWidget::on_extendBtn_clicked(bool checked)
{
    for(int i = 0 ; i < MAX_MODE_BIGLABLE_NUM; i++)
    {
        biglabel[i]->setVisible(checked);
        labelname[i]->setVisible(checked);
    }


    biglabel[0]->setVisible(false);
    biglabel[4]->setVisible(false);
    labelname[0]->setVisible(false);
    labelname[4]->setVisible(false);
}
