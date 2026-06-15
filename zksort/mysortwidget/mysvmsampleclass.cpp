#include "mysvmsampleclass.h"
#include "ui_mysvmsampleclass.h"

MySvmSampleClass::MySvmSampleClass(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::MySvmSampleClass)
{
    ui->setupUi(this);

}

MySvmSampleClass::~MySvmSampleClass()
{
    delete ui;
}

void MySvmSampleClass::showPage(bool bshow)
{
    if(bshow){
        on_svmrefreshBtn_clicked();
    }
}

void MySvmSampleClass::receiveMsg(int msg1, int msg2, int msg3)
{

}

void MySvmSampleClass::retranslateUiWidget()
{
    ui->svmdeleteBtn->setText(g_myLan().ids_delete);
    ui->svmrefreshBtn->setText(g_myLan().update);
    ui->svmimportBtn->setText(g_myLan().ids_import);
    ui->svmsaveBtn->setText(g_myLan().ids_saveSet);
    ui->svmrenameBtn->setText(g_myLan().ids_rename);
}

void MySvmSampleClass::appendUpdateResult(QString str,bool ok)
{
    QString date = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString log =date+"    "+str;
    QListWidgetItem *item= new QListWidgetItem(log,ui->FileListWidget);

    if(!ok)
        item->setTextColor(QColor(255,0,0));
    ui->FileListWidget->scrollToBottom();
}



void MySvmSampleClass::on_svmrefreshBtn_clicked()
{
    QDir myDir("/sdcard/cnf/svm/");

    g_Runtime().dirExist(myDir.path());
    ui->FileListWidget->clear();

    QFileInfoList list;
    list = myDir.entryInfoList(myDir.filter() | QDir::NoDotAndDotDot);

    int m_nFileNum = list.size();
    if (m_nFileNum < 1) {
        ui->svmimportBtn->setEnabled(false);
    } else {
        ui->svmimportBtn->setEnabled(true);
    }

    QString filename;
    float filesize;
    QDateTime filedate;
    QString str;
    QStringList filelist;

    for(int i=0;i< list.size();i++)
    {
        filename = list.at(i).fileName();
        filesize = list.at(i).size();
        filedate = list.at(i).lastModified();
        if (filesize > 1024*1024)
        {
            filesize = filesize /1024 /1024;
            str= filename + "        "+filedate.toString("yyyy/MM/dd hh:mm:ss")+ "        "+QString::number(filesize,'f',1)+"M";
        }
        else
        {
            filesize = filesize/1024;
            str = filename + "        "+filedate.toString("yyyy/MM/dd hh:mm:ss")+ "        "+QString::number(filesize,'f',1)+"K";
        }

        filelist.append(str);
    }

    ui->FileListWidget->insertItems(0, filelist);
    ui->FileListWidget->setCurrentRow(0);

}

void MySvmSampleClass::on_svmimportBtn_clicked()
{
    if(!ui->FileListWidget->currentItem()){
        g_infoWidget().setLabelText(g_myLan().ids_notSelectFile);
        return;
    }
    QString listinfo = ui->FileListWidget->currentItem()->text();
    QStringList list = listinfo.split(" ");

    QString fileName = QString("/sdcard/cnf/svm/%1").arg(list.at(0));

    myMessageBox msgBox(MSG_QUES, g_myLan().ids_toImportLibrary);
    if(msgBox.exec() == QDialog::Accepted){ //用户操作，同意进行
        g_Runtime().m_sampleMap[0].clear();
        g_Runtime().m_sampleMap[1].clear();
        struGsh.nGoodSampCount = 0;
        struGsh.nBadSampCount = 0;
        readSamplesNums(QString(fileName).toLocal8Bit().constData());
//        myFlow.getSvmParas(0,struGsh.nSelectSvmViewIndex,0,struGsh.nSelectSvmIndex);
        if(struGsh.nGoodSampCount > 0 && struGsh.nBadSampCount >0){
//            markAndInitImage();
            g_infoWidget().setLabelText(g_myLan().Sample_import_success);
            g_infoWidget().delayShow();
            myFlow.sleep(2);
            g_infoWidget().hide();
//            updatePixColor();
//            updateBtnState();
//            updateImageSize();
//            struGsh.ntrainComplete = true;

        }else{
            g_infoWidget().setLabelText(g_myLan().Sample_import_failed);
            g_infoWidget().delayShow();
            myFlow.sleep(2);
            g_infoWidget().hide();
        }
//        updateSvmSimulateEditValue();
    }
    return;
}


int MySvmSampleClass::readSamplesNums(const char *file)
{
    QString lineStr;
    QStringList lineList;
    FILE *fp = fopen(file, "r");
    if(fp == NULL)
    {
        printf("打开文件%s失败！\n", file );
        return 0;
    }

    int cnt = 0, ind = 0;
    char line[20];
    while(fgets(line, 20, fp))
    {
        cnt ++;
        if( strlen( line ) == 1 )	// line中只有一个\n
        {

            fclose( fp );
            fp = NULL;
            return 0;
        }

        lineStr = QString::fromLocal8Bit(line,20);
        lineList = lineStr.split(" ");

        if(lineList.size() > 5){
            SvmSampleObj obj = {lineList.at(0).toInt(0,10),lineList.at(1).toInt(0,10),lineList.at(2).toInt(0,10),lineList.at(3).toInt(0,10),lineList.at(4).toInt(0,10)};

            SampleObj obj_o={lineList.at(1).toInt(0,10),lineList.at(2).toInt(0,10),lineList.at(3).toInt(0,10),lineList.at(4).toInt(0,10)};
            SamplesType mode = (SamplesType)lineList.at(0).toInt(0,10);
            g_Runtime().m_sampleMap[mode].insert(obj_o,mode);
            if(mode == Bad){
                if(m_PreviousPageId == SM_IAMGE_WIDGET)
                {
                    struGsh.nBadSamp[struGsh.nBadSampCount*3] = obj.r;
                    struGsh.nBadSamp[struGsh.nBadSampCount*3+1] = obj.g;
                    struGsh.nBadSamp[struGsh.nBadSampCount*3+2] = obj.b;
                }
                else
                {
                    struGsh.nBadSamp[struGsh.nBadSampCount*4] = obj.r;
                    struGsh.nBadSamp[struGsh.nBadSampCount*4+1] = obj.g;
                    struGsh.nBadSamp[struGsh.nBadSampCount*4+2] = obj.b;
                    struGsh.nBadSamp[struGsh.nBadSampCount*4+3] = obj.n;
                }
                struGsh.nBadSampCount++;
            }else{
                if(m_PreviousPageId == SM_IAMGE_WIDGET)
                {
                    struGsh.nBadSamp[struGsh.nGoodSampCount*3] = obj.r;
                    struGsh.nBadSamp[struGsh.nGoodSampCount*3+1] = obj.g;
                    struGsh.nBadSamp[struGsh.nGoodSampCount*3+2] = obj.b;
                }
                else
                {
                    struGsh.nGoodSamp[struGsh.nGoodSampCount*4] = obj.r;
                    struGsh.nGoodSamp[struGsh.nGoodSampCount*4+1] = obj.g;
                    struGsh.nGoodSamp[struGsh.nGoodSampCount*4+2] = obj.b;
                    struGsh.nGoodSamp[struGsh.nGoodSampCount*4+3] = obj.n;
                }
                struGsh.nGoodSampCount++;
            }
        }else{
            fclose( fp );
            fp = NULL;
            return 0;
        }
        ind++;
    }

    fclose( fp );
    qDebug()<< struGsh.nGoodSampCount<< struGsh.nBadSampCount;
    fp = NULL;
    return ind;
}

void MySvmSampleClass::on_svmsaveBtn_clicked()
{
    if(struGsh.nGoodSampCount == 0 && struGsh.nBadSampCount == 0){
        g_infoWidget().setLabelText(g_myLan().No_sample_selected);
        g_infoWidget().showSecs();
        return;
    }
    int x = 1,y = 1,tr = 0,tg = 0,tb = 0,tn = 0;
    QString name = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    myInputMethod inputPanel;
    inputPanel.setTitleAndInput(g_myLan().input,name);
    inputPanel.setInputBtnText(g_myLan().ids_ok,g_myLan().ids_cancel,g_myLan().ids_delete,g_myLan().ids_space);
    if (inputPanel.exec() == QDialog::Accepted) {
        name = "/sdcard/cnf/svm/"+ inputPanel.getText();
        QFile m_sampleFile(name);
        g_Runtime().delFile(name);
        qDebug()<<name;

        if(m_sampleFile.open(QIODevice::ReadWrite|QIODevice::Append|QIODevice::Text))
        {
            QTextStream out(&m_sampleFile);
            QString tempString("");
            QMap<SampleObj,int>::const_iterator var;
            for ( var = g_Runtime().m_sampleMap[Good].constBegin(); var !=g_Runtime().m_sampleMap[Good].constEnd(); ++var) {
                if(struGsh.nGoodSampCount >= MAX_SAMPLE_NUM-1){
                    g_infoWidget().setLabelText(g_myLan().good_is_sufficient);
                    g_infoWidget().showSecs();
                    break;
                }

                tr = var.key().r;
                tg = var.key().g;
                tb = var.key().b;
                tn = var.key().n;

                tempString+=QString::number(Good);
                tempString+=QString(" ");

                tempString+=QString::number(tr);
                tempString+=QString(" ");

                tempString+=QString::number(tg);
                tempString+=QString(" ");

                tempString+=QString::number(tb);
                tempString+=QString(" ");

                if(m_PreviousPageId == SM_IMAGE_ANALYSIS_WIDGET)
                {
                    struGsh.nGoodSamp[struGsh.nGoodSampCount*3] = tr;
                    struGsh.nGoodSamp[struGsh.nGoodSampCount*3+1] = tg;
                    struGsh.nGoodSamp[struGsh.nGoodSampCount*3+2] = tb;
                }
                else
                {
                    struGsh.nGoodSamp[struGsh.nGoodSampCount*4] = tr;
                    struGsh.nGoodSamp[struGsh.nGoodSampCount*4+1] = tg;
                    struGsh.nGoodSamp[struGsh.nGoodSampCount*4+2] = tb;
                    struGsh.nGoodSamp[struGsh.nGoodSampCount*4+3] = tn;
                    tempString+=QString::number(tn);
                    tempString+=QString(" ");
                }
                out<<tempString<<"\n";
                tempString.clear();
            }
            for (var = g_Runtime().m_sampleMap[Bad].constBegin(); var !=g_Runtime().m_sampleMap[Bad].constEnd(); ++var) {
                if(struGsh.nBadSampCount >= MAX_SAMPLE_NUM-1){
                    g_infoWidget().setLabelText(g_myLan().bad_is_sufficient);
                    g_infoWidget().showSecs();
                    break;
                }
                tr = var.key().r;
                tg = var.key().g;
                tb = var.key().b;
                tn = var.key().n;

                tempString+=QString::number(Bad);
                tempString+=QString(" ");

                tempString+=QString::number(tr);
                tempString+=QString(" ");

                tempString+=QString::number(tg);
                tempString+=QString(" ");

                tempString+=QString::number(tb);
                tempString+=QString(" ");

                if(m_PreviousPageId == SM_IMAGE_ANALYSIS_WIDGET)
                {
                    struGsh.nBadSamp[struGsh.nBadSampCount*3] = tr;
                    struGsh.nBadSamp[struGsh.nBadSampCount*3+1] = tg;
                    struGsh.nBadSamp[struGsh.nBadSampCount*3+2] = tb;
                }
                else
                {
                    struGsh.nBadSamp[struGsh.nBadSampCount*4] = tr;
                    struGsh.nBadSamp[struGsh.nBadSampCount*4+1] = tg;
                    struGsh.nBadSamp[struGsh.nBadSampCount*4+2] = tb;
                    struGsh.nBadSamp[struGsh.nBadSampCount*4+3] = tn;
                    tempString+=QString::number(tn);
                    tempString+=QString(" ");
                }
                out<<tempString<<"\n";
                tempString.clear();
            }
        }
        m_sampleFile.close();
    }
    on_svmrefreshBtn_clicked();
}

void MySvmSampleClass::on_svmdeleteBtn_clicked()
{
    if(!ui->FileListWidget->currentItem()){
        g_infoWidget().setLabelText(g_myLan().ids_notSelectFile);
        g_infoWidget().showSecs();
        return;
    }
    QString listinfo = ui->FileListWidget->currentItem()->text();
    QStringList list = listinfo.split(" ");

    QString fileName = QString("/sdcard/cnf/svm/%1").arg(list.at(0));
    qDebug("\t fileName = %s", fileName.toUtf8().data());

    myMessageBox msgBox(MSG_QUES, g_myLan().ids_toDeleteModel+"?");
    if(msgBox.exec() == QDialog::Accepted) //用户操作，同意进行
        g_Runtime().delFile(fileName);
    on_svmrefreshBtn_clicked();
}

void MySvmSampleClass::on_svmrenameBtn_clicked()
{
    if(!ui->FileListWidget->currentItem()){
        g_infoWidget().setLabelText(g_myLan().ids_notSelectFile);
        g_infoWidget().showSecs();
        return;
    }
    QString listinfo = ui->FileListWidget->currentItem()->text();
    QStringList list = listinfo.split(" ");
    QString name = list.at(0);
    myInputMethod inputPanel;
    inputPanel.setTitleAndInput(g_myLan().input,name);
    inputPanel.setInputBtnText(g_myLan().ids_ok,g_myLan().ids_cancel,g_myLan().ids_delete,g_myLan().ids_space);
    if (inputPanel.exec() == QDialog::Accepted) {
        QString newname = inputPanel.getText();
        QFile::rename("/sdcard/cnf/svm/"+name,"/sdcard/cnf/svm/"+newname);
        on_svmrefreshBtn_clicked();
    }

}
