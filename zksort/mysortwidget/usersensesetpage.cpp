#include "usersensesetpage.h"
#include "ui_usersensesetpage.h"
#include "userwidget.h"

UserSenseSetPage::UserSenseSetPage(int pageNo, QWidget *container, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserSenseSetPage)
{
    ui->setupUi(this);

    m_pageNo = pageNo;
    m_Attach = qobject_cast<UserWidget*> (parent);

    m_label[0] = ui->label1;
    m_label[1] = ui->label2;
    m_label[2] = ui->label3;

    m_Histogram[0] = ui->sense1;
    m_Histogram[1] = ui->sense2;
    m_Histogram[2] = ui->sense3;


    for(int i = 0; i < MAX_NUM_HISTOGRAM; i++)
    {
        connect(m_Histogram[i], SIGNAL(onValueChangeExt(int)), this, SLOT(onSenseValueChange(int)));
    }


}


UserSenseSetPage::~UserSenseSetPage()
{
    delete ui;
}



void UserSenseSetPage::InitUiPage()
{
    m_count = m_Attach->m_singleSenseDataVec.size() - m_pageNo*MAX_NUM_HISTOGRAM;    //当前页显示的算法数据
    if(m_count > MAX_NUM_HISTOGRAM)
        m_count = MAX_NUM_HISTOGRAM;

    for(int i = 0; i < MAX_NUM_HISTOGRAM; i++)
    {
        if(i < m_count)
        {
            m_Histogram[i]->show();
            m_label[i]->show();

            SingleSenseData senseUnit = m_Attach->m_singleSenseDataVec[MAX_NUM_HISTOGRAM*m_pageNo + i];
            m_Histogram[i]->setMaxAndMinValue(senseUnit.m_max, senseUnit.m_min);
            m_Histogram[i]->setCurrentValue(senseUnit.m_value);
            m_label[i]->setText(senseUnit.m_senseName);
        }
        else
        {
            m_Histogram[i]->hide();
            m_label[i]->hide();
        }
    }
}

void UserSenseSetPage::onSenseValueChange(int value)
{
    static int view = 0;
    static int group = struGsh.nMatGroup;
    int arithIndex;
    UiVerticalProgressExt* pHor = qobject_cast<UiVerticalProgressExt*>(sender());
    for(int i = 0; i < MAX_NUM_HISTOGRAM; i++)
    {
        if(pHor == m_Histogram[i])
        {
            SingleSenseData sense = m_Attach->m_singleSenseDataVec[MAX_NUM_HISTOGRAM*m_pageNo + i];
            view = sense.m_view;
            arithIndex = sense.m_senseIndex;
            m_Attach->m_singleSenseDataVec[MAX_NUM_HISTOGRAM*m_pageNo + i].m_value = value;

            switch(sense.m_senseIndex)
            {
            case ARITH_GREY_A:
            case ARITH_GREY_B:
            case ARITH_GREY_C:
            case ARITH_GREY_D:
            case ARITH_DISCOLOR_A:
            case ARITH_DISCOLOR_B:
            case ARITH_CROSS:
                if(struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struGreyColor[arithIndex].nMode == 0)
                    struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struGreyColor[arithIndex].nSensLow = value;
                else
                    struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struGreyColor[arithIndex].nSensUp = value;
                break;
            case ARITH_SVM_A:
            case ARITH_SVM_B:
            case ARITH_SVM_C:
            case ARITH_SVM_D:
                struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struSvm[arithIndex-ARITH_SVM_A].nSens = value;
                break;
            case ARITH_SHAPE_SVM_A:
            case ARITH_SHAPE_SVM_B:
            case ARITH_SHAPE_SVM_C:
                struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struShapeSvm[arithIndex-ARITH_SHAPE_SVM_A].nSens = value;
                break;
            case ARITH_BIG_SMALL:
            case ARITH_BIG_SMALL_B:
                if(struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struBigSmall[arithIndex-ARITH_BIG_SMALL].nMode ==0){
                    struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struBigSmall[arithIndex-ARITH_BIG_SMALL].nAreaLow = value;
                }else{
                    struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struBigSmall[arithIndex-ARITH_BIG_SMALL].nAreaUp = value;
                }
                break;
            case ARITH_LONG_SHORT:
            case ARITH_LONG_SHORT_B:
                if(struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struLongShort[arithIndex-ARITH_LONG_SHORT].nLengthMode == 0){
                    struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struLongShort[arithIndex-ARITH_LONG_SHORT].nLengthLow = value;
                }else{
                    struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struLongShort[arithIndex-ARITH_LONG_SHORT].nLengthUp = value;
                }
                break;
            case ARITH_CIRCLE_LONG:
            case ARITH_CIRCLE_LONG_B:
                if(struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struCircleLong[arithIndex-ARITH_CIRCLE_LONG].nMode == 0){
                    struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struCircleLong[arithIndex-ARITH_CIRCLE_LONG].nCiloLow = value;
                }else{
                    struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struCircleLong[arithIndex-ARITH_CIRCLE_LONG].nCilosUp = value;
                }
                break;
            case ARITH_SAT:
                if(struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struColorSat.nMode == 0){
                    struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struColorSat.nSensLow = value;
                }else{
                    struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struColorSat.nSensUp = value;
                }
                break;
            case ARITH_HUE:
                struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struColorHue.nHighHue = value;
                break;
            case ARITH_SCALE:
                struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struColorScale.nSen = value;
                break;
            case ARITH_MILDEW:
                struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struMildew.nSens    = value;
                break;
            case ARITH_OUTLINE:
                struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struOutline.nSens   = value;
                break;
            case ARITH_LINE:
                struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struLine.nSens1     = value;
                break;
            case ARITH_BUD:
            case ARITH_BUD_B:
                struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struBud[arithIndex-ARITH_BUD].nSens   = value;
                break;
            case ARITH_ROUND:
            case ARITH_ROUND_B:
                if(struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struRound[arithIndex-ARITH_ROUND].nSelectMode == 0){
                    struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struRound[arithIndex-ARITH_ROUND].nSensUp = value;
                }else{
                    struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struRound[arithIndex-ARITH_ROUND].nSensLow = value;
                }
                break;
            case ARITH_VALUE:
                if(struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struColorValue.nMode == 0){
                    struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struColorValue.nSensLow = value;
                }else{
                    struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struColorValue.nSensUp = value;
                }
                break;
            case ARITH_SPL_MILDEW:
                struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struSplMildew.nSens    = value;
                break;
            case ARITH_W_PEANUT:
                struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struWhitePeanut.nSensLow    = value;
                break;
            case ARITH_PEPPER_HANDLE:
                struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struPepperHandle.nSens    = value;
                break;
            case ARITH_SUPPRESS_LARGE_A:
            case ARITH_SUPPRESS_LARGE_B:
            case ARITH_SUPPRESS_LARGE_C:
//                struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struSuppressLarge[arithIndex-ARITH_ROUND].nSensLow = value;
                break;
            case ARITH_WHEAT_SPROUT:
                struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struWheatSprout.nSense    = value;
                break;
            case TEA_ARITH_COLOR:
            case TEA_ARITH_COLOR_2:
            case TEA_ARITH_COLOR_3:
            case TEA_ARITH_COLOR_4:
            case TEA_ARITH_COLOR_5:
                struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struTeaColor[arithIndex-TEA_ARITH_COLOR].sense = value;
                break;
            default:
                break;
            }
        }
    }
}
