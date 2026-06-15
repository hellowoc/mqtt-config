#include "updateacswidget.h"
#include "ui_updateacswidget.h"

UpdateAcsWidget::UpdateAcsWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::UpdateAcsWidget)
{
    ui->setupUi(this);
    m_curIpcIndex = 0;
    iscut = false;
    acsUpdateThread = new AcsUpdateThread();
    acsUpdateThread->m_parentWidget = this;
    connect(acsUpdateThread,SIGNAL(sAcsUpdateFinished()),this,SLOT(updateAll()));
    ui->checkBox->hide();
    ui->label_2->setText("192.168.200.200");
    ui->listWidget->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->listWidget_2->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->acsSelectAllBtn->setCheckable(true);
    ui->armSelectAllBtn->setCheckable(true);
}

UpdateAcsWidget::~UpdateAcsWidget()
{
    delete ui;
}
void UpdateAcsWidget::showPage(bool bshow)
{
    if(bshow)
    {
        updateIpcCount();
        ui->listWidget->path = "/media";
        ui->listWidget_2->path = "/sdcard";
        updateAll();
    }
}

void UpdateAcsWidget::retranslateUiWidget()
{
}

void UpdateAcsWidget::updateIpcCount()
{
    QStringList ipcTypeList;
    ui->ipcSelectCombox->clear();
    ui->ipcSelectCombox->hide();
    for(int i = 0; i < struCnfg.nIpcCount; i++)
    {
        QString ipcType = QString("IPC%1").arg(i+1);
        ipcTypeList.append(ipcType);
    }
    if(struCnfg.nIpcCount > 1)
    {
        ui->ipcSelectCombox->show();
        ui->ipcSelectCombox->addItems(ipcTypeList);
        m_curIpcIndex = (m_curIpcIndex < ipcTypeList.length()) ? m_curIpcIndex : 0;
        ui->ipcSelectCombox->setCurrentIndex(m_curIpcIndex);
    }
}
bool UpdateAcsWidget::checkIpcMounted(int ipcindex)
{
    int ipcId = ipcindex + struCnfg.nIpcIpAddrBase;
    if(struCnfg.nUseBMModule){
//        ipcId = ipcindex*2 + struCnfg.nIpcIpAddrBase;
        ipcId = ipcindex + 1;
    }

//    ipcId = 26; //测试用
    localDir = QString("/sdcard/%1").arg(ipcId);
    g_Runtime().dirExist(localDir+"/ACS");
    g_Runtime().dirExist(localDir+"/data");
    ui->listWidget_2->path = localDir;
    m_curIpcIpAddr = QString("192.168.200.%1").arg(ipcId);
    ui->label_2->setText(m_curIpcIpAddr);

    g_Runtime().mySystem("source /app/mynfs.sh c "+QString::number(ipcId));
    return g_Runtime().checkIpcMounted(m_curIpcIpAddr);
}
bool UpdateAcsWidget::unMountIpc(int ipcindex)
{
    int ipcId = ipcindex + struCnfg.nIpcIpAddrBase;
    if(struCnfg.nUseBMModule){
        ipcId = ipcindex + 1;
    }
    g_Runtime().mySystem("source /app/mynfs.sh d "+QString::number(ipcId));
    return g_Runtime().checkIpcMounted(m_curIpcIpAddr);
}

void UpdateAcsWidget::changeValue(mylistwidget *src, mylistwidget *dst)
{
    QList<QListWidgetItem*> selecteditemList = src->selectedItems();
    srcPathList.clear();
    distPathList.clear();
    int selectCnt = 0;
    qDebug()<<selecteditemList.length()<<"/"<<src->count();
    for(int i = 0 ; i< selecteditemList.length();i++)
    {
        if(selecteditemList.at(i))
        {
            selectCnt++;
            QString selecteditemName = selecteditemList.at(i)->text().split("  ").first();
            QString m_srcPath = src->path+"/"+selecteditemName;
            srcPathList.append(m_srcPath);
            QString m_distPath = dst->path+"/"+selecteditemName;
            distPathList.append(m_distPath);
        }
    }
    if(selectCnt > 0)
    {
        g_infoWidget().setLabelText(g_myLan().ids_copy+"...");
        g_infoWidget().show();
        acsUpdateThread->start();
    }

//      if (selecteditem){
//          QString selecteditemName = selecteditem->text().split("  ").first();
//          srcPath = src->path+"/"+selecteditemName;
//          distPath = dst->path+"/"+selecteditemName;
//          g_infoWidget().setLabelText(g_myLan().ids_copy+"...");
//          g_infoWidget().show();
//          acsUpdateThread->start();
//      }
}

void UpdateAcsWidget::goIntoFolder(mylistwidget *src)
{
    QListWidgetItem *selecteditem = src->currentItem();
    if (selecteditem){
        QString selecteditemName = selecteditem->text().split("  ").first();
        srcPath = src->path+"/"+selecteditemName;

        QFileInfo fileinfo(srcPath);
        if(fileinfo.isDir())
        {
            src->path = fileinfo.absoluteFilePath();
            src->updateList();
        }
    }
}
void UpdateAcsWidget::goBackFolder(mylistwidget *src)
{
    QStringList list = src->path.split("/");
    if(list.size() > 0){
        QString path = list.last();
        if(path == "media" || path == "sdcard")
        {
            src->updateList();
            return;
        }
    }
    QDir dir(src->path);
    if(dir.cdUp() && (src->path.right(1)!="/"))
    {
        src->path= dir.absolutePath();
    }
    src->updateList();
}

void UpdateAcsWidget::updateAll()
{
    g_infoWidget().hide();
    for(int i = 0 ;i < srcPathList.size();i++)
    {
        QString str = srcPathList.at(i).right(10);
        if(str =="device.ini")
        {
            myMessageBox msgBox(MSG_QUES, g_myLan().ids_query+" device.ini?");
            int ret = msgBox.exec();
            if (ret == QDialog::Accepted)
            {
                QFile myfile(srcPathList.at(i));
                myfile.open(QIODevice::ReadOnly|QIODevice::Text);
                QTextStream out(&myfile);
                QString str = out.readAll();
                myLongMessageBox msgBox(MSG_ABOUT, str);
                msgBox.exec();
            }
        }
    }
    ui->listWidget->updateList();
    ui->listWidget_2->updateList();
    srcPathList.clear();
    distPathList.clear();
}

void UpdateAcsWidget::on_checkBox_clicked(bool checked)
{
    ui->listWidget->iscut = checked;
    ui->listWidget_2->iscut = checked;
}

void UpdateAcsWidget::on_refreshBtn_clicked()
{
    updateAll();
}

void UpdateAcsWidget::on_copyL2RBtn_clicked()
{
    changeValue(ui->listWidget,ui->listWidget_2);
}

void UpdateAcsWidget::on_copyR2LBtn_clicked()
{
    changeValue(ui->listWidget_2,ui->listWidget);
}

void UpdateAcsWidget::on_ipcSelectCombox_currentIndexChanged(int index)
{
    m_curIpcIndex = index;
}

void UpdateAcsWidget::on_connectBtn_clicked()
{
    ui->checkBox->setEnabled(false);
    checkIpcMounted(m_curIpcIndex);
    ui->checkBox->setEnabled(true);
    ui->listWidget_2->updateList();
}




AcsUpdateThread::AcsUpdateThread(QThread *parent):QThread(parent)
{
}

AcsUpdateThread::~AcsUpdateThread()
{
}
void AcsUpdateThread::run()
{
    int cnt =  m_parentWidget->srcPathList.length();
    int resCnt = 0;
    for(int i = 0; i< cnt;i++)
    {
        g_infoWidget().setLabelText(g_myLan().ids_copy+"..."+QString("%1/%2").arg(i+1).arg(cnt));
        g_Runtime().copyFileToPath(m_parentWidget->srcPathList.at(i),m_parentWidget->distPathList.at(i),true);
    }

    system("sync");
    for(int i = 0;i < cnt;i++)
    {
        g_infoWidget().setLabelText(g_myLan().ipc_verificating+"..."+QString("%1/%2").arg(i+1).arg(cnt));
        QFileInfo fileinfo(m_parentWidget->srcPathList.at(i));
        if(!fileinfo.isDir())
        {
            QString mdSrc = g_Runtime().getFileMd5(m_parentWidget->srcPathList.at(i));
            QString mdDist = g_Runtime().getFileMd5(m_parentWidget->distPathList.at(i));
            if(mdSrc == mdDist){
               resCnt++;
            }
        }
    }
    if(cnt == resCnt){
        g_infoWidget().setLabelText(g_myLan().ids_succeeded);
    }
    else{
        g_infoWidget().setLabelText(g_myLan().ids_sendFileFailed);
    }
    sleep(2);
    emit sAcsUpdateFinished();
}

void UpdateAcsWidget::on_armBackBtn_clicked()
{
    goBackFolder(ui->listWidget);
}

void UpdateAcsWidget::on_armGoBtn_clicked()
{
    goIntoFolder(ui->listWidget);
}

void UpdateAcsWidget::on_acsBackBtn_clicked()
{
    ui->acsSelectAllBtn->setChecked(false);
    goBackFolder(ui->listWidget_2);
}

void UpdateAcsWidget::on_acsGoBtn_clicked()
{
    ui->acsSelectAllBtn->setChecked(false);
    goIntoFolder(ui->listWidget_2);
}

void UpdateAcsWidget::on_acsDelBtn_clicked()
{
//    QListWidgetItem *selecteditem = ui->listWidget_2->currentItem();
//    if (selecteditem){
//        myMessageBox msgBox(MSG_QUES, g_myLan().ids_toDeleteModel);

//        int ret = msgBox.exec();
//        if(ret == QDialog::Accepted){
//            QString selecteditemName = selecteditem->text().split("  ").first();
//            srcPath = ui->listWidget_2->path+"/"+selecteditemName;
//            QFile::remove(srcPath);
//            system("sync");
//            ui->listWidget_2->updateList();
//        }
//    }

    QList<QListWidgetItem*> selecteditemList = ui->listWidget_2->selectedItems();
    if(selecteditemList.length() > 0)
    {
        myMessageBox msgBox(MSG_QUES, g_myLan().ids_toDeleteModel);
        int ret = msgBox.exec();
        if(ret == QDialog::Accepted)
        {
            qDebug()<<selecteditemList.length()<<"/"<<ui->listWidget_2->count();
            for(int i = 0 ; i< selecteditemList.length();i++)
            {
                if(selecteditemList.at(i))
                {
                    g_infoWidget().setLabelText(g_myLan().ids_delete+"..."+QString("%1/%2").arg(i+1).arg(selecteditemList.length()));
                    QString selecteditemName = selecteditemList.at(i)->text().split("  ").first();
                    QString m_srcPath = ui->listWidget_2->path+"/"+selecteditemName;
                    QFile::remove(m_srcPath);
                }
            }
            system("sync");
            ui->listWidget_2->updateList();
            g_infoWidget().hide();
        }
        ui->acsSelectAllBtn->setChecked(false);
    }
}

void UpdateAcsWidget::on_acsReadBtn_clicked()
{
//    if(acsSelectAll) return;
    QListWidgetItem *selecteditem = ui->listWidget_2->currentItem();
    if (selecteditem){
        QString selecteditemName = selecteditem->text().split("  ").first();
        srcPath = ui->listWidget_2->path+"/"+selecteditemName;

        QFileInfo fileinfo(srcPath);
        if(fileinfo.isDir())
        {
            return;
        }
        if(selecteditemName.contains(".cpp")
                || selecteditemName.contains(".sh")
                || selecteditemName.contains(".txt")
                || selecteditemName.contains(".log")
                || selecteditemName.contains(".idx")
                || selecteditemName.contains(".dat")
                || selecteditemName.contains(".ini"))
        {
            QFile myfile(srcPath);
            myfile.open(QIODevice::ReadOnly|QIODevice::Text);
            QTextStream out(&myfile);
            QString str = out.readAll();
            myLongMessageBox msgBox(MSG_ABOUT, str);
            msgBox.exec();
        }
    }
}

void UpdateAcsWidget::on_acsSelectAllBtn_clicked(bool clicked)
{
    int cnt = ui->listWidget_2->count();

    for(int i = 0;i<cnt; i++)
    {
        ui->listWidget_2->item(i)->setSelected(clicked);
    }
}

void UpdateAcsWidget::on_armSelectAllBtn_clicked(bool checked)
{
    int cnt = ui->listWidget->count();

    for(int i = 0;i<cnt; i++)
    {
        ui->listWidget->item(i)->setSelected(checked);
    }
}

void UpdateAcsWidget::on_disconnectBtn_clicked()
{
    ui->checkBox->setEnabled(false);
    if(!unMountIpc(m_curIpcIndex))
        ui->label_2->setText("192.168.200.200");
    ui->checkBox->setEnabled(true);
    ui->listWidget_2->updateList();
}
