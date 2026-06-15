#include "imagemanagewidget.h"
#include "ui_imagemanagewidget.h"
#include "imagewidget.h"
#include "setsvmusedialog.h"
#include "aisetdialog.h"

/* 定义当前图片对应的物料状态 */
/*! \enum mater class property */
enum{
    CLASS_NULL = -1,                         /*!<< class: no property   */
    CLASS_BAD  = 0,                          /*!<< class: bad  property */
    CLASS_GOOD = 1                          /*!<< class: good property */
};

ImageManageWidget::ImageManageWidget(QWidget *parent) :
        basewidget(parent),
        ui(new Ui::ImageManageWidget)
{
    ui->setupUi(this);
    m_studythread.m_pParentWidget = this;
    m_imgExportThread.m_pParentWidget = this;
    connect(&m_studythread,SIGNAL(finished()),this,SLOT(studyFinished()));
    m_imgListBtn[0] = ui->pushButton_01;
    m_imgListBtn[1] = ui->pushButton_02;
    m_imgListBtn[2] = ui->pushButton_03;
    m_imgListBtn[3] = ui->pushButton_04;
    m_imgListBtn[4] = ui->pushButton_05;
    m_imgListBtn[5] = ui->pushButton_06;
    m_imgListBtn[6] = ui->pushButton_07;
    m_imgListBtn[7] = ui->pushButton_08;
    m_imgListBtn[8] = ui->pushButton_09;
    m_imgListBtn[9] = ui->pushButton_10;
    m_imgListBtn[10] = ui->pushButton_11;
    m_imgListBtn[11] = ui->pushButton_12;
    m_imgPropertyBtn[0] = ui->PropertyBtn1;
    m_imgPropertyBtn[1] = ui->PropertyBtn2;
    m_imgPropertyBtn[2] = ui->PropertyBtn3;
    m_imgPropertyBtn[3] = ui->PropertyBtn4;
    m_imgPropertyBtn[4] = ui->PropertyBtn5;
    m_imgPropertyBtn[5] = ui->PropertyBtn6;
    m_imgPropertyBtn[6] = ui->PropertyBtn7;
    m_imgPropertyBtn[7] = ui->PropertyBtn8;
    m_imgPropertyBtn[8] = ui->PropertyBtn9;
    m_imgPropertyBtn[9] = ui->PropertyBtn10;
    m_imgPropertyBtn[10] = ui->PropertyBtn11;
    m_imgPropertyBtn[11] = ui->PropertyBtn12;
    m_imgListLabel[0] = ui->imageNamelbl1;
    m_imgListLabel[1] = ui->imageNamelbl2;
    m_imgListLabel[2] = ui->imageNamelbl3;
    m_imgListLabel[3] = ui->imageNamelbl4;
    m_imgListLabel[4] = ui->imageNamelbl5;
    m_imgListLabel[5] = ui->imageNamelbl6;
    m_imgListLabel[6] = ui->imageNamelbl7;
    m_imgListLabel[7] = ui->imageNamelbl8;
    m_imgListLabel[8] = ui->imageNamelbl9;
    m_imgListLabel[9] = ui->imageNamelbl10;
    m_imgListLabel[10] = ui->imageNamelbl11;
    m_imgListLabel[11] = ui->imageNamelbl12;

    m_imgCheckBox[0] = ui->checkBox;
    m_imgCheckBox[1] = ui->checkBox_2;
    m_imgCheckBox[2] = ui->checkBox_3;
    m_imgCheckBox[3] = ui->checkBox_4;
    m_imgCheckBox[4] = ui->checkBox_5;
    m_imgCheckBox[5] = ui->checkBox_6;
    m_imgCheckBox[6] = ui->checkBox_7;
    m_imgCheckBox[7] = ui->checkBox_8;
    m_imgCheckBox[8] = ui->checkBox_9;
    m_imgCheckBox[9] = ui->checkBox_10;
    m_imgCheckBox[10] = ui->checkBox_11;
    m_imgCheckBox[11] = ui->checkBox_12;

    m_aiRadionBtn[0] = ui->manuRadioBtn;
    m_aiRadionBtn[1] = ui->manuRadioBtn_2;
    m_aiRadionBtn[2] = ui->autoRadioBtn;
    m_aiRadionBtn[3] = ui->autoRadioBtn_2;

    m_pImageBtnsignalMapper = new QSignalMapper(this);
    m_pImagePropertyBtnMapper = new QSignalMapper(this);
    m_pImageCheckBtnMapper = new QSignalMapper(this);
    m_pAIRadioBtnMapper = new QSignalMapper(this);
    for(int i = 0;i < MAX_IMG_NUM;i++){
        m_imgListBtn[i]->setObjectName("imglistbtn");
        m_imgPropertyBtn[i]->setObjectName("imgpropertybtn");
        m_imgListLabel[i]->setObjectName("imglistlabel");

        m_pImageBtnsignalMapper->setMapping(m_imgListBtn[i],i);
        connect(m_imgListBtn[i],SIGNAL(clicked()),m_pImageBtnsignalMapper,SLOT(map()));

        m_pImagePropertyBtnMapper->setMapping(m_imgPropertyBtn[i],i);
        connect(m_imgPropertyBtn[i],SIGNAL(clicked(bool)),m_pImagePropertyBtnMapper,SLOT(map()));


        m_pImageCheckBtnMapper->setMapping(m_imgCheckBox[i],i);
        connect(m_imgCheckBox[i],SIGNAL(clicked(bool)),m_pImageCheckBtnMapper,SLOT(map()));

    }
    for(int i = 0; i < MAX_AI_CHOICE; i++ ){
        m_pAIRadioBtnMapper->setMapping(m_aiRadionBtn[i],i);
        connect(m_aiRadionBtn[i],SIGNAL(clicked(bool)),m_pAIRadioBtnMapper,SLOT(map()));
    }
    connect(m_pImageBtnsignalMapper,SIGNAL(mapped(int)),this,SLOT(onImageListBtnClicked(int)));
    connect(m_pImagePropertyBtnMapper,SIGNAL(mapped(int)),this,SLOT(onImageProperBtnClicked(int)));
    connect(m_pImageCheckBtnMapper,SIGNAL(mapped(int)),this,SLOT(onImageCheckBtnClicked(int)));
    connect(m_pAIRadioBtnMapper,SIGNAL(mapped(int)),this,SLOT(onAiRadioBtnClicked(int)));

    m_nIndexOfPage = 0;
    //    struGsh.m_bAutoStudy[0] = 0;
    //    ui->autoRadioBtn->setChecked(struGsh.m_bAutoStudy[0]);
    struGsh.m_bManulStudy[0] = 1;
    ui->manuRadioBtn->setChecked(struGsh.m_bManulStudy[0]);
    ui->exportAllBtn->setEnabled(m_checkIndexList.count() != 0);
    ui->delBtn->setEnabled(m_checkIndexList.count() != 0);
    ui->saveAllBakBtn->setVisible(false);

    //暂不支持形选智能化
    ui->manuRadioBtn_2->hide();
    ui->autoRadioBtn_2->hide();
    ui->exportBtn->hide();
    ui->importBtn->hide();
}

ImageManageWidget::~ImageManageWidget()
{
    delete ui;
}

void ImageManageWidget::retranslateUiWidget()
{
    ui->exportAllBtn->setText(g_myLan().ids_exportImage);
    ui->delBtn->setText(g_myLan().ids_deleteImage);
    ui->autoRadioBtn->setText(g_myLan().ai_auto_cal);
    ui->manuRadioBtn->setText(g_myLan().ai_manul);
    ui->autoRadioBtn_2->setText(g_myLan().shape+"1");
    ui->manuRadioBtn_2->setText(g_myLan().shape+"2");
    ui->studyBtn->setText(g_myLan().ai_calculate);
    ui->saveAllBakBtn->setText(g_myLan().backup_all);
    ui->applyBtn->setText(g_myLan().ids_apply);
    ui->groupBox_2->setTitle(g_myLan().ai);
    ui->sampleSetBtn->setText(g_myLan().sample_set);
    ui->allInCBox->setText(g_myLan().ids_select_all);
    ui->exportBtn->setText(g_myLan().ids_export);
    ui->importBtn->setText(g_myLan().ids_import);
    ui->thispageallInCBox->setText(g_myLan().ids_selected);
}

void ImageManageWidget::showPage(bool bshow)
{
    if(bshow){
        initImgProperty();
        updateImgList();
        updateBtnStatus();
        ui->exportAllBtn->setEnabled(m_checkIndexList.count() != 0);
        ui->delBtn->setEnabled(m_checkIndexList.count() != 0);
        ui->applyBtn->setVisible(struGsh.ntrainComplete);
        ui->applyBtn->setVisible(true);
        ui->saveAllBakBtn->setVisible(false);
    }else{

    }
}

void ImageManageWidget::updateImgList()
{
    bool isallSelect = true;
    for(int j = 12 *m_nIndexOfPage ;j < 12*(m_nIndexOfPage+1);j++){
        int i = j%12;
        if(j>=g_Runtime().nimageNameVec.count()){
            m_imgListBtn[i]->setIcon(QPixmap());
            m_imgListBtn[i]->setEnabled(false);
            m_imgPropertyBtn[i]->setEnabled(false);
            m_imgCheckBox[i]->setEnabled(false);
            m_imgCheckBox[i]->setChecked(false);
            m_imgListLabel[i]->setText("");
            continue;
        }

        m_imgCheckBox[i]->setChecked(false);
        if(m_checkIndexList.contains(j))
            m_imgCheckBox[i]->setChecked(true);
        bool bIsImg = true;
        QPixmap  image;
#ifdef Q_OS_UNIX
        QString imagePath = g_Runtime().nimageNameVec[j]+"_copy";
        bIsImg = image.load(imagePath,"png",Qt::AutoColor);
        if(!bIsImg){
            bIsImg = image.load(g_Runtime().nimageNameVec[j],"bmp",Qt::AutoColor);
        }
#else
        QString imagePath = g_Runtime().nimageNameVec[j];
        bIsImg = image.load(imagePath);
#endif
        if (bIsImg) {
            m_imgListBtn[i]->setIconSize(image.size());
            m_imgListBtn[i]->setIcon(image);
            m_imgListBtn[i]->setEnabled(true);
            m_imgPropertyBtn[i]->setEnabled(true);
            m_imgCheckBox[i]->setEnabled(true);

            m_imgListLabel[i]->setText(myFlow.getImageShowName(imagePath));
            updateImagePropertyBtnState(i);
        }
        else {
            m_imgListBtn[i]->setIcon(QPixmap());
            m_imgListBtn[i]->setEnabled(false);
            m_imgPropertyBtn[i]->setEnabled(false);
            m_imgCheckBox[i]->setEnabled(false);
        }

        if( bIsImg &&!m_imgCheckBox[i]->isChecked())
            isallSelect = false;
    }

    /* 更新页码显示 */
    int tmp = g_Runtime().nimageNameVec.count()%12 ;
    if (tmp == 0)
        m_nTotalPage = g_Runtime().nimageNameVec.count()/12;
    else
        m_nTotalPage = g_Runtime().nimageNameVec.count()/12 + 1;
    QString str = QString("%1/%2").arg(m_nIndexOfPage+1).arg(m_nTotalPage);
    ui->label_page->setText(str);
    ui->thispageallInCBox->setChecked(isallSelect);
}

void ImageManageWidget::updateImgName(int indexOfImg, bool bFlagSet)
{

}

void ImageManageWidget::updateBtnStatus()
{
    QStringList grouplist;
    for(int i = 0;i < struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].nIdentifyGroupTotal;i++){
        if(i == 0)
            grouplist<<g_myLan().ids_one;
        if(i == 1)
            grouplist<<g_myLan().ids_two;
        if(i == 2)
            grouplist<<g_myLan().ids_three;
        if(i == 3)
            grouplist<<g_myLan().ids_four;
        if(i == 4)
            grouplist<<g_myLan().ids_five;
    }
    ui->autoRadioBtn->setChecked(struGsh.m_bAutoStudy[0]);
    ui->autoRadioBtn_2->setChecked(struGsh.m_bAutoStudy[1]);
    ui->manuRadioBtn->setChecked(struGsh.m_bManulStudy[0]);
    ui->manuRadioBtn_2->setChecked(struGsh.m_bManulStudy[1]);
    for(int i = 0;i < MAX_PER_LIST;i++){
        m_imgPropertyBtn[i]->setVisible(struGsh.m_bAutoStudy[0]|struGsh.m_bAutoStudy[1]);
        updateImagePropertyBtnState(m_nIndexOfPage*MAX_PER_LIST+ i);
    }
}

void ImageManageWidget::initImgProperty()
{
    m_autoStatOfImg.clear();
    for(int i = 0;i < g_Runtime().nimageNameVec.count();i++){
        m_autoStatOfImg.insert(i,CLASS_NULL);
    }
}

void ImageManageWidget::onImageListBtnClicked(int index)
{
    int i = (m_nIndexOfPage) * MAX_PER_LIST + index;
    struGsh.nImageIndex = i;
    g_MainManager().ShowWidget(SM_IMAGE_ANALYSIS_WIDGET);
}

void ImageManageWidget::onImageProperBtnClicked(int index)
{
    int i = (m_nIndexOfPage) *MAX_PER_LIST + index;
    imageListChangeSlot(i);
}

void ImageManageWidget::onImageCheckBtnClicked(int index)
{
    int i = (m_nIndexOfPage) *MAX_PER_LIST + index;
    if(m_imgCheckBox[index]->isChecked())
        m_checkIndexList.append(i);
    else
        m_checkIndexList.removeOne(i);
    ui->exportAllBtn->setEnabled(m_checkIndexList.count() != 0);
    ui->delBtn->setEnabled(m_checkIndexList.count() != 0);
}

void ImageManageWidget::onAiRadioBtnClicked(int index)
{
    struGsh.m_bManulStudy[0] = (index == 0)? 1 : 0;
    struGsh.m_bManulStudy[1] = (index == 1)? 1 : 0;
    struGsh.m_bAutoStudy[0] = (index == 2)? 1 : 0;
    struGsh.m_bAutoStudy[1] = (index == 3)? 1 : 0;
    updateBtnStatus();
}

/* 优化取样时判断像素点是否为边缘点 */
bool ImageManageWidget::pixelIsEdgeImage(QImage *image, int x, int y,int layer,int view)
{
    if(x==0 || y==0 || x==IMAGE_WIDTH-1 || y==myFlow.imgHeight-1)
        return false;

    QColor rgb;
    for(int m=-1; m<=1; m++){
        for(int n=-1; n<=1; n++){
            if(m!=0 || n!=0){
                rgb = image->pixel(x+n,y+m);
                if(myFlow.imagePixelIsBackground(rgb.red(), rgb.green(), rgb.blue(),0,layer,view,struGsh.nImgTdMode))
                    return true;
            }
        }
    }
    return false;
}


void ImageManageWidget::autoStudy()
{
    m_studyResult = tr_unknown_error;
    memset(struGsh.m_svmcoefs,0,sizeof(struGsh.m_svmcoefs));
    memset(struGsh.m_Shapecoef,0,sizeof(struGsh.m_Shapecoef));
    g_infoWidget().setLabelText(g_myLan().ai_analysis);
    g_infoWidget().delayShow();
    m_studythread.start();
}

void ImageManageWidget::updateImagePropertyBtnState(int index)
{
    if(m_autoStatOfImg.contains(index)){
        switch (m_autoStatOfImg.value(index)) {
        case CLASS_NULL:
            m_imgPropertyBtn[index%MAX_PER_LIST]->setText(g_myLan().ai_basic);
            break;
        case CLASS_BAD:
            m_imgPropertyBtn[index%MAX_PER_LIST]->setText(g_myLan().ai_bad);
            break;
        case CLASS_GOOD:
            m_imgPropertyBtn[index%MAX_PER_LIST]->setText(g_myLan().ai_good);
            break;
        default:
            break;
        }
    }else{
        m_imgPropertyBtn[index%MAX_PER_LIST]->setText(g_myLan().ai_basic);
    }
}

/*!
 * \brief AIMainWidget::imageListChangeSlt
 * \param index
 */
void ImageManageWidget::imageListChangeSlot(int index)
{
    if(!m_autoStatOfImg.contains(index))
       m_autoStatOfImg[index] = CLASS_NULL;
    switch (m_autoStatOfImg.value(index)) {
    case CLASS_NULL:
        m_autoStatOfImg[index] = CLASS_BAD;
        break;
    case CLASS_BAD:
        m_autoStatOfImg[index] = CLASS_GOOD;
        break;
    case CLASS_GOOD:
        m_autoStatOfImg[index] = CLASS_NULL;
        break;
    default:
        break;
    }
    updateImagePropertyBtnState(index);
}
void ImageManageWidget::on_delBtn_clicked(bool checked)
{
    myMessageBox msgBox(MSG_QUES, g_myLan().ids_toDeleteImage);
    int ret = msgBox.exec();
    if (ret != QDialog::Accepted){ //用户操作，同意进行
        return;
    }

    QStringList tmpDelList;
    for(int i = 0;i < m_checkIndexList.count();i++){
        g_Runtime().delFile(g_Runtime().nimageNameVec[m_checkIndexList[i]]);
        g_Runtime().delFile(g_Runtime().nimageNameVec[m_checkIndexList[i]]+"_copy");
        tmpDelList.append(g_Runtime().nimageNameVec[m_checkIndexList[i]]);
    }
    for(int i= 0;i < tmpDelList.count();i++){
        g_Runtime().nimageNameVec.removeOne(tmpDelList[i]);
    }
    m_checkIndexList.clear();
//    updateImgList();
    m_nIndexOfPage = 0;
    ui->allInCBox->setChecked(false);
    ui->thispageallInCBox->setChecked(false);
    g_Runtime().getFreeSpace();
    if(g_Runtime().nimageNameVec.count() == 0) returnBack();

    showPage(true);
}
void ImageManageWidget::on_saveAllBakBtn_clicked()
{
#ifdef Q_OS_UNIX
    if(!g_Runtime().checkUsbExist()){
        g_infoWidget().setLabelText(g_myLan().msg_usb_insert);
        g_infoWidget().showSecs();
        return;
    }
    QString path = g_Runtime().getUsbPath();
    if (path == ""){
        g_infoWidget().setLabelText(g_myLan().msg_usb_not_recognized);
        g_infoWidget().showSecs();
        return;
    }
    g_infoWidget().setLabelText(g_myLan().backup_all);
    g_infoWidget().delayShow();
    g_Runtime().mySystem("cp /sdcard/bmp -fr "+path);
    QDir dir(path+"bmp");
    QDir dir1("/sdcard/bmp");
    if (dir.exists())
    {
        QStringList list = dir.entryList();
        QStringList list1 = dir1.entryList();

        if (list.length() == list1.length())
        {
            g_infoWidget().setLabelText(g_myLan().ids_backup+g_myLan().ids_succeeded);
            g_infoWidget().delayShow();
            myFlow.sleep(2);
            g_infoWidget().hide();
        }
        else
        {
            g_infoWidget().setLabelText(g_myLan().ids_backup+g_myLan().ai_image_save_error);
            g_infoWidget().delayShow();
            myFlow.sleep(2);
            g_infoWidget().hide();
        }
    }
    else
    {
        g_infoWidget().setLabelText(g_myLan().ai_image_save_error);
        g_infoWidget().delayShow();
        myFlow.sleep(2);
        g_infoWidget().hide();
    }
#endif
}

void ImageManageWidget::on_studyBtn_clicked()
{
    autoStudy();
}


//void ImageManageWidget::on_autoRadioBtn_clicked(bool checked)
//{
//    struGsh.m_bAutoStudy = checked;
//    updateBtnStatus();
//    //todo隐藏图片界面的好坏按钮
//}

//void ImageManageWidget::on_manuRadioBtn_clicked(bool checked)
//{
//    struGsh.m_bAutoStudy = !checked;
//    updateBtnStatus();
//    //todo显示图片界面的好坏按钮
//}

void ImageManageWidget::on_exportAllBtn_clicked()
{
    m_imgExportThread.start();
}

void ImageManageWidget::on_applyBtn_clicked()
{
    SetSvmUseDialog dlg;
    dlg.exec();
}

void ImageManageWidget::on_btn_nextPage_clicked()
{
    if (m_nIndexOfPage < (m_nTotalPage-1)) {
        m_nIndexOfPage++;
        updateImgList();
    }
    updateBtnStatus();
}

void ImageManageWidget::on_btn_prePage_clicked()
{
    if (m_nIndexOfPage >= 1) {
        m_nIndexOfPage--;
        updateImgList();
    }
    updateBtnStatus();
}

void ImageManageWidget::studyFinished()
{
    g_infoWidget().hide();
    myMessageBox msg(MSG_INFO,g_myLan().ai_analysis);
    struGsh.ntrainComplete = (m_studyResult == tr_succeeded);
    switch(m_studyResult){
    case tr_succeeded:
        msg.setText(g_myLan().ai_success);
        if(struGsh.m_bManulStudy[1]||struGsh.m_bAutoStudy[1]){
            printf("svmshapecoefs:sense:%f a:%f b:%f c:%f d:%f e:%f f:%f g:%f h:%f i:%f\n",struGsh.m_Shapecoef[9],struGsh.m_Shapecoef[0],struGsh.m_Shapecoef[1]\
                            ,struGsh.m_Shapecoef[2],struGsh.m_Shapecoef[3],struGsh.m_Shapecoef[4],struGsh.m_Shapecoef[5],struGsh.m_Shapecoef[6],struGsh.m_Shapecoef[7],struGsh.m_Shapecoef[8]);

        }
        else{
            printf("svmcoefs:sense:%f r:%f g:%f b:%f rg;%f gb:%f rb:%f rr:%f gg:%f bb:%f\n",struGsh.m_svmcoefs[9],struGsh.m_svmcoefs[0],struGsh.m_svmcoefs[1]\
                        ,struGsh.m_svmcoefs[2],struGsh.m_svmcoefs[3],struGsh.m_svmcoefs[4],struGsh.m_svmcoefs[5],struGsh.m_svmcoefs[6],struGsh.m_svmcoefs[7],struGsh.m_svmcoefs[8]);
        }
        break;
    case tr_unsupported_type:
        msg.setText(g_myLan().ai_unsupported);
        break;
    case tr_no_good_data:
        msg.setText(g_myLan().ai_err_no_good);
        break;
    case tr_no_bad_data:
        msg.setText(g_myLan().ai_err_no_bad);
        break;
    case tr_null_data_ptr:
        msg.setText(g_myLan().ai_err_no_set);
        break;
    case tr_failed_write_file:
        msg.setText(g_myLan().ai_err_fail_write);
        break;
    case tr_no_config_file:
        msg.setText(g_myLan().ai_err_no_config_file);
        break;
    case tr_unknown_error:
        msg.setText(g_myLan().ai_unknow_err);
        break;
    default:
        break;

    }
    ui->applyBtn->setVisible(struGsh.ntrainComplete);
    msg.exec();
}

void SvmThread::run()
{
    struGsh.bUpdateState = 1; //停止除时钟外所有工作
#ifdef Q_OS_UNIX
    if(!g_Runtime().dirExist("/sdcard/bin")){
        system("cp -fr /app/svm* /sdcard/bin/");
        system("sync");
    }
#endif

    if(struGsh.m_bAutoStudy[0]||struGsh.m_bManulStudy[0]){
        if(struGsh.m_bAutoStudy[0]){
            QMap<SampleObj,int> m_sampleMap;
            m_sampleMap.clear();
            QImage image;
            QColor rgb;

            memset(struGsh.nGoodSamp,0,sizeof(struGsh.nGoodSamp));
            memset(struGsh.nBadSamp,0,sizeof(struGsh.nBadSamp));
            int goodNum = 0;
            int badNum= 0;
            int r,g,b,image_layer,image_view,image_unit;
            QString imagePath;
            int imageCount = g_Runtime().nimageNameVec.count()<MAX_IMG_NUM? g_Runtime().nimageNameVec.count():MAX_IMG_NUM;
            QStringList sortImagePath;
            QStringList sortImageStat;
            sortImagePath.clear();
            sortImageStat.clear();

            //好坏样本图片分开，先处理好物料
            for (int k = 0; k < imageCount;k++) {
                imagePath = g_Runtime().nimageNameVec[k];
                int tempMark = m_pParentWidget->m_autoStatOfImg.value(k);
                if(tempMark == CLASS_NULL)
                    continue;
                if (tempMark == CLASS_GOOD){
                    sortImagePath.insert(sortImagePath.begin(),imagePath);
                    sortImageStat.insert(sortImageStat.begin(),"1");
                }
                if (tempMark == CLASS_BAD){
                    sortImagePath.insert(sortImagePath.end(),imagePath);
                    sortImageStat.insert(sortImageStat.end(),"0");
                }
            }

            for (int k = 0; k < sortImagePath.length();k++) {
                imagePath = sortImagePath.at(k);
                image.load(imagePath);
                QStringList list = imagePath.split("/");
                QStringList namelist = list.last().split("_");
                image_layer  = namelist[0].toInt();
                image_view = namelist[1].toInt();
                image_unit = namelist[2].toInt();
                int tempMark = sortImageStat.at(k).toInt();

                for(int i=0; i<image.height(); i++){
                    if(i % (struGsh.svmSample[1]+1 ) != 0)
                        continue;
                    for(int j=0; j<image.width(); j++){
                        if (j % (struGsh.svmSample[0]+1) != 0)
                            continue;
                        int sensornSplice = struCnfp.nImageSplice[image_layer][image_view];
                        sensornSplice = (sensornSplice == 0)? 1 : 2;
                        if(struGsh.nImgTdMode == 2 && struCnfp.nSensorPixel[0][struGsh.nView] == SENSOR_PIXELS_2048_XTD
                                && struCnfp.nViewSensorType[0][struGsh.nView] == SENSOR_G2)
                        {
                            sensornSplice = 2;
                        }
                        if(j < struCnfg.struLayerInfo[image_layer].stuCameraInfo[image_view].nChannelBegin[image_unit]/sensornSplice \
                                || j > struCnfg.struLayerInfo[image_layer].stuCameraInfo[image_view].nChannelEnd[image_unit]/sensornSplice)
                            continue;
                        rgb = image.pixel(j, i);
                        r = rgb.red();
                        g = rgb.green();
                        b = rgb.blue();
                        if (myFlow.imagePixelIsBackground(r,g,b,0,image_layer,image_view,struGsh.nImgTdMode)){
                            continue;
                        }
                        if (myFlow.imagePixelIsOutThreshold(r,g,b,0,tempMark)){
                            continue;
                        }
                        if (struGsh.m_bEdge && m_pParentWidget->pixelIsEdgeImage(&image,j, i,image_layer,image_view)){
                            continue;
                        }

                        SampleObj obj = {r,g,b};

                        if(struGsh.m_bRMRedtmdancy){ // 去除冗余 + 好坏相异
                            for(int k1 = -1; k1 <= 1; k1++){
                                for(int k2 = -1; k2 <= 1; k2++){
                                    for(int k3 = -1; k3 <= 1; k3++){
                                        SampleObj objTmp1 = {r+k1, g+k2, b+k3};
                                        if(m_sampleMap.contains(objTmp1)){
                                            continue;
                                        }
                                    }
                                }
                            }
                        }
                        m_sampleMap.insert(obj,tempMark);
                    }
                }
            }

            QMap<SampleObj,int>::const_iterator var;
            int i = 0;
            for (var = m_sampleMap.constBegin(); var !=m_sampleMap.constEnd(); ++var,i++) {
                r = var.key().r;
                g = var.key().g;
                b = var.key().b;
                if(var.value()){
                    if(goodNum < MAX_SAMPLE_NUM-1){
                        struGsh.nGoodSamp[goodNum*3] = var.key().r;
                        struGsh.nGoodSamp[goodNum*3+1] = var.key().g;
                        struGsh.nGoodSamp[goodNum*3+2] = var.key().b;
                        goodNum++;
                    }
                }else{
                    if(badNum < MAX_SAMPLE_NUM-1){
                        struGsh.nBadSamp[badNum*3] = var.key().r;
                        struGsh.nBadSamp[badNum*3+1] = var.key().g;
                        struGsh.nBadSamp[badNum*3+2] = var.key().b;
                        badNum++;
                    }
                }
            }

            qDebug()<<"good sample:"<<goodNum;
            qDebug()<<"bad sample:"<<badNum;
            qDebug()<<"svm type:"<<struCnfg.n_svmType;
#ifdef Q_OS_UNIX
//            m_pParentWidget->m_studyResult = train(struGsh.nGoodSamp,goodNum,struGsh.nBadSamp,badNum,(svm_type)struCnfg.n_svmType,"/sdcard/bin",struGsh.m_svmcoefs);
#else
            m_pParentWidget->m_studyResult = train(struGsh.nGoodSamp,goodNum,struGsh.nBadSamp,badNum,(svm_type)struCnfg.n_svmType,qApp->applicationDirPath().toUtf8().data(),struGsh.m_svmcoefs);
#endif
        }else if(struGsh.m_bManulStudy[0]){
#ifdef Q_OS_UNIX
//            m_pParentWidget->m_studyResult = train(struGsh.nGoodSamp,struGsh.nGoodSampCount,struGsh.nBadSamp,struGsh.nBadSampCount,(svm_type)struCnfg.n_svmType,"/sdcard/bin",struGsh.m_svmcoefs);
#else
            m_pParentWidget->m_studyResult = train(struGsh.nGoodSamp,struGsh.nGoodSampCount,struGsh.nBadSamp,struGsh.nBadSampCount,(svm_type)struCnfg.n_svmType,qApp->applicationDirPath().toUtf8().data(),struGsh.m_svmcoefs);
#endif
        }
    }
    else if(struGsh.m_bManulStudy[1] || struGsh.m_bAutoStudy[1] ){
#ifdef Q_OS_UNIX
//        m_pParentWidget->m_studyResult = train(struGsh.nShapeGoodSamp,struGsh.nShapeGoodSampCount/5,struGsh.nShapeBadSamp,struGsh.nShapeBadSampCount/5,shape_normal,"/sdcard/bin",struGsh.m_Shapecoef);
#endif
        memset(struGsh.nShapeGoodSamp,0,sizeof(struGsh.nShapeGoodSamp));
        memset(struGsh.nShapeBadSamp,0,sizeof(struGsh.nShapeBadSamp));
        struGsh.nShapeGoodSampCount = 0;
        struGsh.nShapeBadSampCount = 0;
    }

    struGsh.bUpdateState = 0;

}


void ImageManageWidget::on_sampleSetBtn_clicked()
{
    AiSetDialog aiset;
    int ret = aiset.exec();
}

void ImageManageWidget::on_allInCBox_clicked(bool checked)
{
    m_checkIndexList.clear();
    if(checked){
        for(int i =0; i <g_Runtime().nimageNameVec.count();i++){
            m_checkIndexList.append(i);
        }
        for(int j = 0;j < MAX_PER_LIST;j++){
            if((m_nIndexOfPage * MAX_PER_LIST + j)< g_Runtime().nimageNameVec.count())
                m_imgCheckBox[j]->setChecked(true);
        }
    }
    else{
        m_checkIndexList.clear();
        for(int j = 0;j < MAX_PER_LIST;j++){
            if((m_nIndexOfPage * MAX_PER_LIST + j)< g_Runtime().nimageNameVec.count())
                m_imgCheckBox[j]->setChecked(false);
        }
    }

    ui->exportAllBtn->setEnabled(m_checkIndexList.count() != 0);
    ui->delBtn->setEnabled(m_checkIndexList.count() != 0);
    ui->thispageallInCBox->setChecked(checked);
}

void ImageManageWidget::on_exportBtn_clicked()
{
    //        QSettings setting(CFG_APPSet,QSettings::IniFormat);
#ifdef Q_OS_UNIX
    QSettings setting(g_Runtime().getUsbPath()+"svm.ini",QSettings::IniFormat);
#else
    QSettings setting("/sdcard/cfg/svm.ini",QSettings::IniFormat);
#endif
    g_infoWidget().setLabelText(g_myLan().msg_exporting_params);
    g_infoWidget().delayShow();
    setting.setValue("R",struGsh.m_svmcoefs[0]);
    setting.setValue("G",struGsh.m_svmcoefs[1]);
    setting.setValue("B",struGsh.m_svmcoefs[2]);
    setting.setValue("RG",struGsh.m_svmcoefs[3]);
    setting.setValue("GB",struGsh.m_svmcoefs[4]);
    setting.setValue("RB",struGsh.m_svmcoefs[5]);
    setting.setValue("RR",struGsh.m_svmcoefs[6]);
    setting.setValue("GG",struGsh.m_svmcoefs[7]);
    setting.setValue("BB",struGsh.m_svmcoefs[8]);
    setting.setValue("CON",struGsh.m_svmcoefs[9]);

    myFlow.sleep(2);
    g_infoWidget().hide();
}

void ImageManageWidget::on_importBtn_clicked()
{    
    g_infoWidget().setLabelText(g_myLan().msg_importing_params);
    g_infoWidget().delayShow();
//    QSettings setting(CFG_APPSet,QSettings::IniFormat);
#ifdef Q_OS_UNIX
    QSettings setting(g_Runtime().getUsbPath()+"svm.ini",QSettings::IniFormat);
#else
    QSettings setting("/sdcard/cfg/svm.ini",QSettings::IniFormat);
#endif
    struGsh.m_svmcoefs[0] = setting.value("R",struGsh.m_svmcoefs[0]).toDouble();
    struGsh.m_svmcoefs[1] = setting.value("G",struGsh.m_svmcoefs[1]).toDouble();
    struGsh.m_svmcoefs[2] = setting.value("B",struGsh.m_svmcoefs[2]).toDouble();
    struGsh.m_svmcoefs[3] = setting.value("RG",struGsh.m_svmcoefs[3]).toDouble();
    struGsh.m_svmcoefs[4] = setting.value("GB",struGsh.m_svmcoefs[4]).toDouble();
    struGsh.m_svmcoefs[5] =  setting.value("RB",struGsh.m_svmcoefs[5]).toDouble();
    struGsh.m_svmcoefs[6] =  setting.value("RR",struGsh.m_svmcoefs[6]).toDouble();
    struGsh.m_svmcoefs[7] =  setting.value("GG",struGsh.m_svmcoefs[7]).toDouble();
    struGsh.m_svmcoefs[8] =  setting.value("BB",struGsh.m_svmcoefs[8]).toDouble();
    struGsh.m_svmcoefs[9] =  setting.value("CON",struGsh.m_svmcoefs[9]).toDouble();

    myFlow.sleep(2);
    g_infoWidget().hide();
}

void ImageManageWidget::on_thispageallInCBox_clicked(bool checked)
{
    if(checked){
        for(int i = 12 *m_nIndexOfPage ;i< 12*(m_nIndexOfPage+1);i++){
            if(i<g_Runtime().nimageNameVec.count())
                m_checkIndexList.append(i);
        }
        for(int j = 0;j < MAX_PER_LIST;j++){
            if((m_nIndexOfPage * MAX_PER_LIST + j)< g_Runtime().nimageNameVec.count())
                m_imgCheckBox[j]->setChecked(true);
        }
    }
    else{
        for(int i = 12 *m_nIndexOfPage ;i< 12*(m_nIndexOfPage+1);i++){
            if(i<g_Runtime().nimageNameVec.count())
                m_checkIndexList.removeOne(i);
        }
        for(int j = 0;j < MAX_PER_LIST;j++){
            if((m_nIndexOfPage * MAX_PER_LIST + j)< g_Runtime().nimageNameVec.count())
                m_imgCheckBox[j]->setChecked(false);
        }
    }
    ui->exportAllBtn->setEnabled(m_checkIndexList.count() != 0);
    ui->delBtn->setEnabled(m_checkIndexList.count() != 0);
}


void ExportThread::run()
{
    if(!g_Runtime().checkUsbExist()){
        g_infoWidget().setLabelText(g_myLan().msg_usb_insert);
        g_infoWidget().showSecs();
        return;
    }
    QString path = g_Runtime().getUsbPath();
    if (path == ""){
        g_infoWidget().setLabelText(g_myLan().msg_usb_not_recognized);
        g_infoWidget().showSecs();
        return;
    }

#ifdef Q_OS_UNIX
    char *cmd = "ls /sys/class/scsi_device/";
    char buf[128];
    FILE *fp;

    if ((fp = popen( cmd , "r")) == NULL)
    {
        g_infoWidget().setLabelText("Error");
        g_infoWidget().delayShow();
        myFlow.sleep(2);
        g_infoWidget().hide();
        return;
    }
    memset(buf, 0 ,sizeof(buf));
    fread(buf, 1, 7, fp);

    // 未查出U盘信息，判空
    if(buf != NULL){
        if(strlen(buf) == 0)
        {
            g_infoWidget().setLabelText(g_myLan().cfm_insert_udisk);
            g_infoWidget().delayShow();
            myFlow.sleep(2);
            g_infoWidget().hide();
            return;
        }
        else
        {
            g_infoWidget().setLabelText(g_myLan().ids_exportImage);
            g_infoWidget().delayShow();
        //            QString cmdstr = QString("cp -fr /sdcard/bmp/*.png ")+ path;
        //            char *cmd1 = QString(cmdstr).toLatin1().data();
        //            system(cmd1);

            for(int i = 0;i < m_pParentWidget->m_checkIndexList.count();i++){
                g_infoWidget().setLabelText(g_myLan().ids_exportImage+QString("%1/%2").arg(i+1).arg(m_pParentWidget->m_checkIndexList.count()));
                QFileInfo file(g_Runtime().nimageNameVec[m_pParentWidget->m_checkIndexList[i]]);
                g_Runtime().copyFileToPath(g_Runtime().nimageNameVec[m_pParentWidget->m_checkIndexList[i]],path+file.fileName(),true);
                g_Runtime().copyFileToPath(g_Runtime().nimageNameVec[m_pParentWidget->m_checkIndexList[i]]+"_copy",path+file.fileName()+"_copy",true);
            }
            g_infoWidget().setLabelText(g_myLan().ipc_verificating);

            system("sync");
            myFlow.sleep(1);

            g_infoWidget().hide();
        }
    }
    else
    {
        return;
    }
    g_infoWidget().setLabelText(g_myLan().ids_exportImageFinished);
    g_infoWidget().delayShow();

    myFlow.sleep(2);

    g_infoWidget().hide();
#endif
}
