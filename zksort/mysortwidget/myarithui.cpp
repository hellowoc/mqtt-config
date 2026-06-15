#include "myarithui.h"
#include "ui_myarithui.h"
#include "constant.h"
#include "globalflow.h"
MyArithUi::MyArithUi(int arithType, QWidget *parent):
    QWidget(parent),
    ui(new Ui::MyArithUi),
   m_arithType(arithType)
{
     ui->setupUi(this);
     ui->checkBox->setCheckable(true);
}

MyArithUi::~MyArithUi()
{
    delete ui;
}

void MyArithUi::showWidget()
{
    switch (m_arithType) {
    case ARITH_GREY_A:
        ui->checkBox->setText(g_myLan().arith_gray_a);
        break;
    case ARITH_GREY_B:
        ui->checkBox->setText(g_myLan().arith_gray_b);
        break;
    case ARITH_GREY_C:
        ui->checkBox->setText(g_myLan().arith_gray_c);
        break;
    case ARITH_GREY_D:
        ui->checkBox->setText(g_myLan().arith_gray_d);
        break;
    case ARITH_DISCOLOR_A:
        ui->checkBox->setText(g_myLan().arith_discolor_a);
        break;
    case ARITH_DISCOLOR_B:
        ui->checkBox->setText(g_myLan().arith_discolor_b);
        break;
    case ARITH_CROSS:
        ui->checkBox->setText(g_myLan().arith_cross);
        break;
    case ARITH_SVM_A:
        ui->checkBox->setText("SVMA");
        break;
    case ARITH_SVM_B:
        ui->checkBox->setText("SVMB");
        break;
    case ARITH_SVM_C:
        ui->checkBox->setText("SVMC");
        break;
    case ARITH_SVM_D:
        ui->checkBox->setText("SVMD");
        break;
    case ARITH_SHAPE_SVM_A:
        ui->checkBox->setText(g_myLan().shape+g_myLan().arith_svm_a);
        break;
    case ARITH_SHAPE_SVM_B:
        ui->checkBox->setText(g_myLan().shape+g_myLan().arith_svm_b);
        break;
    case ARITH_SHAPE_SVM_C:
        ui->checkBox->setText(g_myLan().shape+g_myLan().arith_svm_c);
        break;
    case ARITH_BIG_SMALL:
        ui->checkBox->setText(g_myLan().arith_big_small);
        break;
    case ARITH_BIG_SMALL_B:
        ui->checkBox->setText(g_myLan().arith_big_small_b);
        break;
    case ARITH_LONG_SHORT:
        ui->checkBox->setText(g_myLan().arith_long_short);
        break;
    case ARITH_LONG_SHORT_B:
        ui->checkBox->setText(g_myLan().arith_long_short_b);
        break;
    case ARITH_CIRCLE_LONG:
        ui->checkBox->setText(g_myLan().arith_circle_long);
        break;
    case ARITH_CIRCLE_LONG_B:
        ui->checkBox->setText(g_myLan().arith_circle_long_b);
        break;
    case ARITH_HUE:
        ui->checkBox->setText(g_myLan().arith_hue);
        break;
    case ARITH_SAT:
        ui->checkBox->setText(g_myLan().arith_sat);
        break;
    case ARITH_SCALE:
        ui->checkBox->setText(g_myLan().arith_scale);
        break;
    case ARITH_MILDEW:
        ui->checkBox->setText(g_myLan().arith_mildew);
        break;
    case ARITH_OUTLINE:
        ui->checkBox->setText(g_myLan().arith_outline);
        break;
    case ARITH_LINE:
        ui->checkBox->setText(g_myLan().arith_line);
        break;
    case ARITH_BUD:
        ui->checkBox->setText(g_myLan().arith_bud);
        break;
    case ARITH_BUD_B:
        ui->checkBox->setText(g_myLan().arith_bud_b);
        break;
    case ARITH_ROUND:
        ui->checkBox->setText(g_myLan().arith_round);
        break;
    case ARITH_ROUND_B:
        ui->checkBox->setText(g_myLan().arith_round_b);
        break;
    case ARITH_HSV:
        ui->checkBox->setText("HSVA");
        break;
    case ARITH_HSV_B:
        ui->checkBox->setText("HSVB");
        break;
    case ARITH_HSV_C:
        ui->checkBox->setText("HSVC");
        break;
    case ARITH_VALUE:
        ui->checkBox->setText(g_myLan().arith_value);
        break;
    case ARITH_SPL_MILDEW:
        ui->checkBox->setText(g_myLan().arith_spl_mildew);
        break;
    case ARITH_W_PEANUT:
        ui->checkBox->setText(g_myLan().arith_white_peanut);
        break;
    case ARITH_PEPPER_HANDLE:
        ui->checkBox->setText(g_myLan().arith_pepper_handle);
        break;
    case ARITH_SUPPRESS_LARGE_A:
        ui->checkBox->setText(g_myLan().arith_suppress_large+"A");
        break;
    case ARITH_SUPPRESS_LARGE_B:
        ui->checkBox->setText(g_myLan().arith_suppress_large+"B");
        break;
    case ARITH_SUPPRESS_LARGE_C:
        ui->checkBox->setText(g_myLan().arith_suppress_large+"C");
        break;
    case ARITH_WHEAT_SPROUT:
        ui->checkBox->setText(g_myLan().arith_wheat_sprout);
        break;
    case TEA_ARITH_COLOR:
        ui->checkBox->setText("TEA_A");
        break;
    case TEA_ARITH_COLOR_2:
        ui->checkBox->setText("TEA_B");
        break;
    case TEA_ARITH_COLOR_3:
        ui->checkBox->setText("TEA_C");
        break;
    case TEA_ARITH_COLOR_4:
        ui->checkBox->setText("TEA_D");
        break;
    case TEA_ARITH_COLOR_5:
        ui->checkBox->setText("TEA_E");
        break;
    default:
        break;
    }
    updateWidget();
}


void MyArithUi::updateWidget()
{
    bool bstate = false;
    switch (m_arithType) {
    case ARITH_GREY_A:
    case ARITH_GREY_B:
    case ARITH_GREY_C:
    case ARITH_GREY_D:
    case ARITH_DISCOLOR_A:
    case ARITH_DISCOLOR_B:
    case ARITH_CROSS:
        if(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[m_arithType].nDispEnable)
        {
            bstate = true;
        }else{
            bstate = false;
        }
        break;
    case ARITH_SVM_A:
    case ARITH_SVM_B:
    case ARITH_SVM_C:
    case ARITH_SVM_D:
        if(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[m_arithType-ARITH_SVM_A].nDispEnable)
        {
            bstate = true;
        }else{
            bstate = false;
        }
        break;
    case ARITH_SHAPE_SVM_A:
    case ARITH_SHAPE_SVM_B:
    case ARITH_SHAPE_SVM_C:
        if(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struShapeSvm[m_arithType-ARITH_SHAPE_SVM_A].nDispEnable)
        {
            bstate = true;
        }else{
            bstate = false;
        }
        break;
    case ARITH_BIG_SMALL:
    case ARITH_BIG_SMALL_B:
        if(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struBigSmall[m_arithType-ARITH_BIG_SMALL].nDispEnable)
        {
            bstate = true;
        }else{
            bstate = false;
        }
        break;
    case ARITH_LONG_SHORT:
    case ARITH_LONG_SHORT_B:
        if(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struLongShort[m_arithType-ARITH_LONG_SHORT].nDispEnable)
        {
            bstate = true;
        }else{
            bstate = false;
        }
        break;
    case ARITH_CIRCLE_LONG:
    case ARITH_CIRCLE_LONG_B:
        if(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struCircleLong[m_arithType-ARITH_CIRCLE_LONG].nDispEnable)
        {
            bstate = true;
        }else{
            bstate = false;
        }
        break;
    case ARITH_SAT:
        if(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struColorSat.nDispEnable)
        {
            bstate = true;
        }else{
            bstate = false;
        }
        break;
    case ARITH_SCALE:
        if(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struColorScale.nDispEnable)
        {
            bstate = true;
        }else{
            bstate = false;
        }
        break;
    case ARITH_HUE:
        if(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struColorHue.nDispEnable)
        {
            bstate = true;
        }else{
            bstate = false;
        }
        break;
    case ARITH_HSV:
    case ARITH_HSV_B:
    case ARITH_HSV_C:
        if(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv[m_arithType-ARITH_HSV].nDispEnable)
        {
            bstate = true;
        }else{
            bstate = false;
        }
        break;
    case ARITH_MILDEW:
        if(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struMildew.nDispEnable)
        {
            bstate = true;
        }else{
            bstate = false;
        }
        break;
    case ARITH_OUTLINE:
        if(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struOutline.nDispEnable)
        {
            bstate = true;
        }else{
            bstate = false;
        }
        break;
    case ARITH_LINE:
        if(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struLine.nDispEnable)
        {
            bstate = true;
        }else{
            bstate = false;
        }
        break;
    case ARITH_BUD:
    case ARITH_BUD_B:
        if(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struBud[m_arithType-ARITH_BUD].nDispEnable)
        {
            bstate = true;
        }else{
            bstate = false;
        }
        break;
    case ARITH_ROUND:
    case ARITH_ROUND_B:
        if(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struRound[m_arithType-ARITH_ROUND].nDispEnable)
        {
            bstate = true;
        }else{
            bstate = false;
        }
        break;
    case ARITH_VALUE:
        if(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struColorValue.nDispEnable)
        {
            bstate = true;
        }else{
            bstate = false;
        }
        break;
    case ARITH_SPL_MILDEW:
        if(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSplMildew.nDispEnable)
        {
            bstate = true;
        }else{
            bstate = false;
        }
        break;
    case ARITH_W_PEANUT:
        if(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struWhitePeanut.nDispEnable)
        {
            bstate = true;
        }else{
            bstate = false;
        }
        break;
    case ARITH_PEPPER_HANDLE:
        if(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struPepperHandle.nDispEnable)
        {
            bstate = true;
        }else{
            bstate = false;
        }
        break;
    case ARITH_SUPPRESS_LARGE_A:
    case ARITH_SUPPRESS_LARGE_B:
    case ARITH_SUPPRESS_LARGE_C:
        if(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSuppressLarge[m_arithType-ARITH_SUPPRESS_LARGE_A].nDispEnable)
        {
            bstate = true;
        }else{
            bstate = false;
        }
        break;
    case ARITH_WHEAT_SPROUT:
        if(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struWheatSprout.nDispEnable)
        {
            bstate = true;
        }else{
            bstate = false;
        }
        break;
    case TEA_ARITH_COLOR:
    case TEA_ARITH_COLOR_2:
    case TEA_ARITH_COLOR_3:
    case TEA_ARITH_COLOR_4:
    case TEA_ARITH_COLOR_5:
        if(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struTeaColor[m_arithType-TEA_ARITH_COLOR].nDispEnable)
        {
            bstate = true;
        }else{
            bstate = false;
        }
        break;
    default:
        break;
    }
    ui->checkBox->setChecked(bstate);
}

void MyArithUi::on_checkBox_clicked(bool checked)
{
    int nEnable = (int)checked;

    switch (m_arithType) {
    case ARITH_GREY_A:
    case ARITH_GREY_B:
    case ARITH_GREY_C:
    case ARITH_GREY_D:
    case ARITH_DISCOLOR_A:
    case ARITH_DISCOLOR_B:
    case ARITH_CROSS:
        struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[m_arithType].nDispEnable = nEnable;
        break;
    case ARITH_SVM_A:
    case ARITH_SVM_B:
    case ARITH_SVM_C:
    case ARITH_SVM_D:
        struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[m_arithType-ARITH_SVM_A].nDispEnable= nEnable;
        break;
    case ARITH_SHAPE_SVM_A:
    case ARITH_SHAPE_SVM_B:
    case ARITH_SHAPE_SVM_C:
        struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struShapeSvm[m_arithType-ARITH_SHAPE_SVM_A].nDispEnable= nEnable;
        break;
    case ARITH_BIG_SMALL:
    case ARITH_BIG_SMALL_B:
        struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struBigSmall[m_arithType-ARITH_BIG_SMALL].nDispEnable = nEnable;
        break;
    case ARITH_LONG_SHORT:
    case ARITH_LONG_SHORT_B:
        struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struLongShort[m_arithType-ARITH_LONG_SHORT].nDispEnable = nEnable;
        break;
    case ARITH_CIRCLE_LONG:
    case ARITH_CIRCLE_LONG_B:
        struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struCircleLong[m_arithType-ARITH_CIRCLE_LONG].nDispEnable = nEnable;
        break;
    case ARITH_SAT:
        struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struColorSat.nDispEnable = nEnable;
        break;
    case ARITH_HUE:
        struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struColorHue.nDispEnable = nEnable;
        break;
    case ARITH_SCALE:
        struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struColorScale.nDispEnable = nEnable;
        break;
    case ARITH_HSV:
    case ARITH_HSV_B:
    case ARITH_HSV_C:
        struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv[m_arithType-ARITH_HSV].nDispEnable = nEnable;
        break;
    case ARITH_MILDEW:
        struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struMildew.nDispEnable = nEnable;
        break;
    case ARITH_OUTLINE:
        struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struOutline.nDispEnable = nEnable;
        break;
    case ARITH_LINE:
        struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struLine.nDispEnable = nEnable;
        break;
    case ARITH_BUD:
    case ARITH_BUD_B:
        struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struBud[m_arithType-ARITH_BUD].nDispEnable = nEnable;
        break;
    case ARITH_ROUND:
    case ARITH_ROUND_B:
        struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struRound[m_arithType-ARITH_ROUND].nDispEnable = nEnable;
        break;
    case ARITH_VALUE:
        struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struColorValue.nDispEnable = nEnable;
        break;
    case ARITH_SPL_MILDEW:
        struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSplMildew.nDispEnable = nEnable;
        break;
    case ARITH_W_PEANUT:
        struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struWhitePeanut.nDispEnable = nEnable;
        break;
    case ARITH_PEPPER_HANDLE:
        struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struPepperHandle.nDispEnable = nEnable;
        break;
    case ARITH_SUPPRESS_LARGE_A:
    case ARITH_SUPPRESS_LARGE_B:
    case ARITH_SUPPRESS_LARGE_C:
        struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSuppressLarge[m_arithType-ARITH_SUPPRESS_LARGE_A].nDispEnable = nEnable;
        break;
    case ARITH_WHEAT_SPROUT:
        struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struWheatSprout.nDispEnable = nEnable;
        break;
    case TEA_ARITH_COLOR:
    case TEA_ARITH_COLOR_2:
    case TEA_ARITH_COLOR_3:
    case TEA_ARITH_COLOR_4:
    case TEA_ARITH_COLOR_5:
        struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struTeaColor[m_arithType-TEA_ARITH_COLOR].nDispEnable = nEnable;
        break;
    default:
        break;
    }

    if(nEnable == 0)
    {
        struCnfp.nGroupArithEnable[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup][m_arithType]= nEnable;

        switch (m_arithType) {
        case ARITH_GREY_A:
        case ARITH_GREY_B:
        case ARITH_GREY_C:
        case ARITH_GREY_D:
        case ARITH_DISCOLOR_A:
        case ARITH_DISCOLOR_B:
        case ARITH_CROSS:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[m_arithType].nArithEnable = nEnable;
            break;
        case ARITH_SVM_A:
        case ARITH_SVM_B:
        case ARITH_SVM_C:
        case ARITH_SVM_D:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[m_arithType-ARITH_SVM_A].nArithEnable = nEnable;
            break;
        case ARITH_SHAPE_SVM_A:
        case ARITH_SHAPE_SVM_B:
        case ARITH_SHAPE_SVM_C:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struShapeSvm[m_arithType-ARITH_SHAPE_SVM_A].nArithEnable = nEnable;
            break;
        case ARITH_BIG_SMALL:
        case ARITH_BIG_SMALL_B:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struBigSmall[m_arithType-ARITH_BIG_SMALL].nArithEnable = nEnable;
            break;
        case ARITH_LONG_SHORT:
        case ARITH_LONG_SHORT_B:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struLongShort[m_arithType-ARITH_LONG_SHORT].nArithEnable = nEnable;
            break;
        case ARITH_CIRCLE_LONG:
        case ARITH_CIRCLE_LONG_B:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struCircleLong[m_arithType-ARITH_CIRCLE_LONG].nArithEnable = nEnable;
            break;
        case ARITH_SAT:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struColorSat.nArithEnable = nEnable;
            break;
        case ARITH_HUE:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struColorHue.nArithEnable = nEnable;
            break;
        case ARITH_SCALE:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struColorScale.nArithEnable = nEnable;
            break;
        case ARITH_HSV:
        case ARITH_HSV_B:
        case ARITH_HSV_C:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv[m_arithType-ARITH_HSV].nArithEnable = nEnable;
            break;
        case ARITH_MILDEW:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struMildew.nArithEnable = nEnable;
            break;
        case ARITH_OUTLINE:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struOutline.nArithEnable = nEnable;
            break;
        case ARITH_LINE:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struLine.nArithEnable = nEnable;
            break;
        case ARITH_BUD:
        case ARITH_BUD_B:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struBud[m_arithType-ARITH_BUD].nArithEnable = nEnable;
            break;
        case ARITH_ROUND:
        case ARITH_ROUND_B:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struRound[m_arithType-ARITH_ROUND].nArithEnable = nEnable;
            break;
        case ARITH_VALUE:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struColorValue.nArithEnable = nEnable;
            break;
        case ARITH_SPL_MILDEW:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSplMildew.nArithEnable = nEnable;
            break;
        case ARITH_W_PEANUT:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struWhitePeanut.nArithEnable = nEnable;
            break;
        case ARITH_PEPPER_HANDLE:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struPepperHandle.nArithEnable = nEnable;
            break;
        case ARITH_SUPPRESS_LARGE_A:
        case ARITH_SUPPRESS_LARGE_B:
        case ARITH_SUPPRESS_LARGE_C:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSuppressLarge[m_arithType-ARITH_SUPPRESS_LARGE_A].nArithEnable = nEnable;
            break;
        case ARITH_WHEAT_SPROUT:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struWheatSprout.nArithEnable = nEnable;
            break;
        case TEA_ARITH_COLOR:
        case TEA_ARITH_COLOR_2:
        case TEA_ARITH_COLOR_3:
        case TEA_ARITH_COLOR_4:
        case TEA_ARITH_COLOR_5:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struTeaColor[m_arithType-TEA_ARITH_COLOR].nArithEnable = nEnable;
            break;
        default:
            break;
        }
    }
    myFlow.materialArithEnableCopy(struGsh.nLayer, struGsh.nView, struGsh.nMatGroup, m_arithType, true); //根据参数设置方式，添加参数复制
    if(nEnable == 0)
    {
        myFlow.materialParamsCopySendAssemble(struGsh.nLayer, struGsh.nView, struGsh.nMatGroup, false, m_arithType, 0, true);
    }

     if(struCnfg.nMachineSpliceType == MACHINE_SPLICE_TYPE_BIG)
     {
        copySpliceDisplayEnable(nEnable, m_arithType);
     }
}

void MyArithUi::copySpliceDisplayEnable(int enable, int arithIndex)
{
    if((struGsh.nMatGroup < struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].nIdentifyGroupTotal-1)
            &&(struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].stuIdenGupInfo[struGsh.nMatGroup+1].nGroupUnitTotal == 1)       //只有一个相机板,且被分割
            &&(struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].stuIdenGupInfo[struGsh.nMatGroup+1].nUnitGupSplice[0] == 2)) //1表示左半边
    {
        switch (arithIndex) {
        case ARITH_GREY_A:
        case ARITH_GREY_B:
        case ARITH_GREY_C:
        case ARITH_GREY_D:
        case ARITH_DISCOLOR_A:
        case ARITH_DISCOLOR_B:
        case ARITH_CROSS:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struGreyColor[arithIndex].nDispEnable = enable;
            break;
        case ARITH_SVM_A:
        case ARITH_SVM_B:
        case ARITH_SVM_C:
        case ARITH_SVM_D:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struSvm[arithIndex-ARITH_SVM_A].nDispEnable= enable;
            break;
        case ARITH_SHAPE_SVM_A:
        case ARITH_SHAPE_SVM_B:
        case ARITH_SHAPE_SVM_C:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struShapeSvm[arithIndex-ARITH_SHAPE_SVM_A].nDispEnable= enable;
            break;
        case ARITH_BIG_SMALL:
        case ARITH_BIG_SMALL_B:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struBigSmall[arithIndex-ARITH_BIG_SMALL].nDispEnable = enable;
            break;
        case ARITH_LONG_SHORT:
        case ARITH_LONG_SHORT_B:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struLongShort[arithIndex-ARITH_LONG_SHORT].nDispEnable = enable;
            break;
        case ARITH_CIRCLE_LONG:
        case ARITH_CIRCLE_LONG_B:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struCircleLong[arithIndex-ARITH_CIRCLE_LONG].nDispEnable = enable;
            break;
        case ARITH_HSV:
        case ARITH_HSV_B:
        case ARITH_HSV_C:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struHsv[m_arithType-ARITH_HSV].nDispEnable = enable;
            break;
        case ARITH_SAT:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struColorSat.nDispEnable = enable;
            break;
        case ARITH_HUE:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struColorHue.nDispEnable = enable;
            break;
        case ARITH_SCALE:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struColorScale.nDispEnable = enable;
            break;
        case ARITH_MILDEW:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struMildew.nDispEnable = enable;
            break;
        case ARITH_OUTLINE:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struOutline.nDispEnable = enable;
            break;
        case ARITH_LINE:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struLine.nDispEnable = enable;
            break;
        case ARITH_BUD:
        case ARITH_BUD_B:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struBud[arithIndex-ARITH_BUD].nDispEnable = enable;
            break;
        case ARITH_ROUND:
        case ARITH_ROUND_B:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struRound[arithIndex-ARITH_ROUND].nDispEnable = enable;
            break;
        case ARITH_VALUE:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struColorValue.nDispEnable = enable;
            break;
        case ARITH_SPL_MILDEW:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struSplMildew.nDispEnable = enable;
            break;
        case ARITH_W_PEANUT:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struWhitePeanut.nDispEnable = enable;
            break;
        case ARITH_PEPPER_HANDLE:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struPepperHandle.nDispEnable = enable;
            break;
        case ARITH_SUPPRESS_LARGE_A:
        case ARITH_SUPPRESS_LARGE_B:
        case ARITH_SUPPRESS_LARGE_C:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struSuppressLarge[m_arithType-ARITH_SUPPRESS_LARGE_A].nDispEnable = enable;
            break;
        case ARITH_WHEAT_SPROUT:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struWheatSprout.nDispEnable = enable;
            break;
        case TEA_ARITH_COLOR:
        case TEA_ARITH_COLOR_2:
        case TEA_ARITH_COLOR_3:
        case TEA_ARITH_COLOR_4:
        case TEA_ARITH_COLOR_5:
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struTeaColor[m_arithType-TEA_ARITH_COLOR].nDispEnable = enable;
            break;
        default:
            break;
        }

        if(enable == 0)
        {
            struCnfp.nGroupArithEnable[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1][arithIndex]= enable;

            switch (arithIndex) {
            case ARITH_GREY_A:
            case ARITH_GREY_B:
            case ARITH_GREY_C:
            case ARITH_GREY_D:
            case ARITH_DISCOLOR_A:
            case ARITH_DISCOLOR_B:
            case ARITH_CROSS:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struGreyColor[arithIndex].nArithEnable = enable;
                break;
            case ARITH_SVM_A:
            case ARITH_SVM_B:
            case ARITH_SVM_C:
            case ARITH_SVM_D:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struSvm[arithIndex-ARITH_SVM_A].nArithEnable = enable;
                break;
            case ARITH_SHAPE_SVM_A:
            case ARITH_SHAPE_SVM_B:
            case ARITH_SHAPE_SVM_C:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struShapeSvm[arithIndex-ARITH_SHAPE_SVM_A].nArithEnable = enable;
                break;
            case ARITH_BIG_SMALL:
            case ARITH_BIG_SMALL_B:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struBigSmall[arithIndex-ARITH_BIG_SMALL].nArithEnable = enable;
                break;
            case ARITH_LONG_SHORT:
            case ARITH_LONG_SHORT_B:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struLongShort[arithIndex-ARITH_LONG_SHORT].nArithEnable = enable;
                break;
            case ARITH_CIRCLE_LONG:
            case ARITH_CIRCLE_LONG_B:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struCircleLong[arithIndex-ARITH_CIRCLE_LONG].nArithEnable = enable;
                break;
            case ARITH_HSV:
            case ARITH_HSV_B:
            case ARITH_HSV_C:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struHsv[m_arithType-ARITH_HSV].nArithEnable = enable;
                break;
            case ARITH_SAT:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struColorSat.nArithEnable = enable;
                break;
            case ARITH_HUE:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struColorHue.nArithEnable = enable;
                break;
            case ARITH_SCALE:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struColorScale.nArithEnable = enable;
                break;
            case ARITH_MILDEW:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struMildew.nArithEnable = enable;
                break;
            case ARITH_OUTLINE:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struOutline.nArithEnable = enable;
                break;
            case ARITH_LINE:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struLine.nArithEnable = enable;
                break;
            case ARITH_BUD:
            case ARITH_BUD_B:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struBud[arithIndex-ARITH_BUD].nArithEnable = enable;
                break;
            case ARITH_ROUND:
            case ARITH_ROUND_B:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struRound[arithIndex-ARITH_ROUND].nArithEnable = enable;
                break;
            case ARITH_VALUE:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struColorValue.nArithEnable = enable;
                break;
            case ARITH_SPL_MILDEW:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struSplMildew.nArithEnable = enable;
                break;
            case ARITH_W_PEANUT:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struWhitePeanut.nArithEnable = enable;
                break;
            case ARITH_PEPPER_HANDLE:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struPepperHandle.nArithEnable = enable;
                break;
            case ARITH_SUPPRESS_LARGE_A:
            case ARITH_SUPPRESS_LARGE_B:
            case ARITH_SUPPRESS_LARGE_C:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struSuppressLarge[m_arithType-ARITH_SUPPRESS_LARGE_A].nArithEnable = enable;
                break;
            case ARITH_WHEAT_SPROUT:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struWheatSprout.nArithEnable = enable;
                break;
            case TEA_ARITH_COLOR:
            case TEA_ARITH_COLOR_2:
            case TEA_ARITH_COLOR_3:
            case TEA_ARITH_COLOR_4:
            case TEA_ARITH_COLOR_5:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup+1].struTeaColor[m_arithType-TEA_ARITH_COLOR].nArithEnable = enable;
                break;
            default:
                break;
            }

            myFlow.materialParamsCopySendAssemble(struGsh.nLayer, struGsh.nView, struGsh.nMatGroup+1, false, arithIndex, 0, true);
        }
    }
}


