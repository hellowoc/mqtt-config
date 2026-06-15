#include "ipcclassparams.h"
#include "ui_ipcclassparams.h"
#include "globalflow.h"
#include <QScrollBar>

ipcclassparams::ipcclassparams(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::ipcclassparams)
{
    ui->setupUi(this);
    m_pGroupWidget = ui->groupWidget;
    m_pViewBtn[0] = ui->pushButton;
    m_pViewBtn[1] = ui->pushButton_1;
    m_pViewBtn[2] = ui->pushButton_2;
    m_pViewBtn[3] = ui->pushButton_3;
    m_pViewBtn[4]= ui->pushButton_4;
    m_pViewBtn[5]= ui->pushButton_5;
    m_pViewBtn[6]= ui->pushButton_6;
    m_pViewBtn[7]= ui->pushButton_7;

    setSlotConnect();

    m_pClassEnable[0]= ui->checkBox;
    m_pClassEnable[1]= ui->checkBox_2;
    m_pClassEnable[2]= ui->checkBox_3;
    m_pClassEnable[3]= ui->checkBox_4;
    m_pClassEnable[4]= ui->checkBox_5;
    m_pClassEnable[5]= ui->checkBox_6;
    m_pClassEnable[6]= ui->checkBox_7;
    m_pClassEnable[7]= ui->checkBox_8;
    m_pClassEnable[8]= ui->checkBox_9;
    m_pClassEnable[9]= ui->checkBox_10;
    m_pClassEnable[10]= ui->checkBox_11;
    m_pClassEnable[11]= ui->checkBox_12;
    m_pClassEnable[12]= ui->checkBox_13;
    m_pClassEnable[13]= ui->checkBox_14;
    m_pClassEnable[14]= ui->checkBox_15;
    m_pClassEnable[15]= ui->checkBox_16;
    m_pClassEnable[16]= ui->checkBox_17;
    m_pClassEnable[17]= ui->checkBox_18;
    m_pClassEnable[18]= ui->checkBox_19;
    m_pClassEnable[19]= ui->checkBox_20;
    m_pClassEnable[20]= ui->checkBox_21;
    m_pClassEnable[21]= ui->checkBox_22;
    m_pClassEnable[22]= ui->checkBox_23;
    m_pClassEnable[23]= ui->checkBox_24;
    m_pClassEnable[24]= ui->checkBox_25;
    m_pClassEnable[25]= ui->checkBox_26;
    m_pClassEnable[26]= ui->checkBox_27;
    m_pClassEnable[27]= ui->checkBox_28;
    m_pClassEnable[28]= ui->checkBox_29;
    m_pClassEnable[29]= ui->checkBox_30;
    m_pClassEnable[30]= ui->checkBox_31;
    m_pClassEnable[31]= ui->checkBox_32;
    m_pClassEnable[32]= ui->checkBox_33;
    m_pClassEnable[33]= ui->checkBox_34;
    m_pClassEnable[34]= ui->checkBox_35;
    m_pClassEnable[35]= ui->checkBox_36;
    m_pClassEnable[36]= ui->checkBox_37;
    m_pClassEnable[37]= ui->checkBox_38;
    m_pClassEnable[38]= ui->checkBox_39;
    m_pClassEnable[39]= ui->checkBox_40;
    m_pClassEnable[40]= ui->checkBox_41;
    m_pClassEnable[41]= ui->checkBox_42;
    m_pClassEnable[42]= ui->checkBox_43;
    m_pClassEnable[43]= ui->checkBox_44;
    m_pClassEnable[44]= ui->checkBox_45;
    m_pClassEnable[45]= ui->checkBox_46;
    m_pClassEnable[46]= ui->checkBox_47;
    m_pClassEnable[47]= ui->checkBox_48;
    m_pClassEnable[48]= ui->checkBox_49;
    m_pClassEnable[49]= ui->checkBox_50;
    m_pClassEnable[50]= ui->checkBox_51;
    m_pClassEnable[51]= ui->checkBox_52;

    m_nameClassEdit[0]= ui->class1NameEdit;
    m_nameClassEdit[1]= ui->class1NameEdit_2;
    m_nameClassEdit[2]= ui->class1NameEdit_3;
    m_nameClassEdit[3]= ui->class1NameEdit_4;
    m_nameClassEdit[4]= ui->class1NameEdit_5;
    m_nameClassEdit[5]= ui->class1NameEdit_6;
    m_nameClassEdit[6]= ui->class1NameEdit_7;
    m_nameClassEdit[7]= ui->class1NameEdit_8;
    m_nameClassEdit[8]= ui->class1NameEdit_9;
    m_nameClassEdit[9]= ui->class1NameEdit_10;
    m_nameClassEdit[10]= ui->class1NameEdit_11;
    m_nameClassEdit[11]= ui->class1NameEdit_12;
    m_nameClassEdit[12]= ui->class1NameEdit_13;
    m_nameClassEdit[13]= ui->class1NameEdit_14;
    m_nameClassEdit[14]= ui->class1NameEdit_15;
    m_nameClassEdit[15]= ui->class1NameEdit_16;
    m_nameClassEdit[16]= ui->class1NameEdit_17;
    m_nameClassEdit[17]= ui->class1NameEdit_18;
    m_nameClassEdit[18]= ui->class1NameEdit_19;
    m_nameClassEdit[19]= ui->class1NameEdit_20;
    m_nameClassEdit[20]= ui->class1NameEdit_21;
    m_nameClassEdit[21]= ui->class1NameEdit_22;
    m_nameClassEdit[22]= ui->class1NameEdit_23;
    m_nameClassEdit[23]= ui->class1NameEdit_24;
    m_nameClassEdit[24]= ui->class1NameEdit_25;
    m_nameClassEdit[25]= ui->class1NameEdit_26;
    m_nameClassEdit[26]= ui->class1NameEdit_27;
    m_nameClassEdit[27]= ui->class1NameEdit_28;
    m_nameClassEdit[28]= ui->class1NameEdit_29;
    m_nameClassEdit[29]= ui->class1NameEdit_30;
    m_nameClassEdit[30]= ui->class1NameEdit_31;
    m_nameClassEdit[31]= ui->class1NameEdit_32;
    m_nameClassEdit[32]= ui->class1NameEdit_33;
    m_nameClassEdit[33]= ui->class1NameEdit_34;
    m_nameClassEdit[34]= ui->class1NameEdit_35;
    m_nameClassEdit[35]= ui->class1NameEdit_36;
    m_nameClassEdit[36]= ui->class1NameEdit_37;
    m_nameClassEdit[37]= ui->class1NameEdit_38;
    m_nameClassEdit[38]= ui->class1NameEdit_39;
    m_nameClassEdit[39]= ui->class1NameEdit_40;
    m_nameClassEdit[40]= ui->class1NameEdit_41;
    m_nameClassEdit[41]= ui->class1NameEdit_42;
    m_nameClassEdit[42]= ui->class1NameEdit_43;
    m_nameClassEdit[43]= ui->class1NameEdit_44;
    m_nameClassEdit[44]= ui->class1NameEdit_45;
    m_nameClassEdit[45]= ui->class1NameEdit_46;
    m_nameClassEdit[46]= ui->class1NameEdit_47;
    m_nameClassEdit[47]= ui->class1NameEdit_48;
    m_nameClassEdit[48]= ui->class1NameEdit_49;
    m_nameClassEdit[49]= ui->class1NameEdit_50;
    m_nameClassEdit[50]= ui->class1NameEdit_51;
    m_nameClassEdit[51]= ui->class1NameEdit_52;

    m_pClassSensEdit[0]= ui->class1SensWidget;
    m_pClassSensEdit[1]= ui->class1SensWidget_2;
    m_pClassSensEdit[2]= ui->class1SensWidget_3;
    m_pClassSensEdit[3]= ui->class1SensWidget_4;
    m_pClassSensEdit[4]= ui->class1SensWidget_5;
    m_pClassSensEdit[5]= ui->class1SensWidget_6;
    m_pClassSensEdit[6]= ui->class1SensWidget_7;
    m_pClassSensEdit[7]= ui->class1SensWidget_8;
    m_pClassSensEdit[8]= ui->class1SensWidget_9;
    m_pClassSensEdit[9]= ui->class1SensWidget_10;
    m_pClassSensEdit[10]= ui->class1SensWidget_11;
    m_pClassSensEdit[11]= ui->class1SensWidget_12;
    m_pClassSensEdit[12]= ui->class1SensWidget_13;
    m_pClassSensEdit[13]= ui->class1SensWidget_14;
    m_pClassSensEdit[14]= ui->class1SensWidget_15;
    m_pClassSensEdit[15]= ui->class1SensWidget_16;
    m_pClassSensEdit[16]= ui->class1SensWidget_17;
    m_pClassSensEdit[17]= ui->class1SensWidget_18;
    m_pClassSensEdit[18]= ui->class1SensWidget_19;
    m_pClassSensEdit[19]= ui->class1SensWidget_20;
    m_pClassSensEdit[20]= ui->class1SensWidget_21;
    m_pClassSensEdit[21]= ui->class1SensWidget_22;
    m_pClassSensEdit[22]= ui->class1SensWidget_23;
    m_pClassSensEdit[23]= ui->class1SensWidget_24;
    m_pClassSensEdit[24]= ui->class1SensWidget_25;
    m_pClassSensEdit[25]= ui->class1SensWidget_26;
    m_pClassSensEdit[26]= ui->class1SensWidget_27;
    m_pClassSensEdit[27]= ui->class1SensWidget_28;
    m_pClassSensEdit[28]= ui->class1SensWidget_29;
    m_pClassSensEdit[29]= ui->class1SensWidget_30;
    m_pClassSensEdit[30]= ui->class1SensWidget_31;
    m_pClassSensEdit[31]= ui->class1SensWidget_32;
    m_pClassSensEdit[32]= ui->class1SensWidget_33;
    m_pClassSensEdit[33]= ui->class1SensWidget_34;
    m_pClassSensEdit[34]= ui->class1SensWidget_35;
    m_pClassSensEdit[35]= ui->class1SensWidget_36;
    m_pClassSensEdit[36]= ui->class1SensWidget_37;
    m_pClassSensEdit[37]= ui->class1SensWidget_38;
    m_pClassSensEdit[38]= ui->class1SensWidget_39;
    m_pClassSensEdit[39]= ui->class1SensWidget_40;
    m_pClassSensEdit[40]= ui->class1SensWidget_41;
    m_pClassSensEdit[41]= ui->class1SensWidget_42;
    m_pClassSensEdit[42]= ui->class1SensWidget_43;
    m_pClassSensEdit[43]= ui->class1SensWidget_44;
    m_pClassSensEdit[44]= ui->class1SensWidget_45;
    m_pClassSensEdit[45]= ui->class1SensWidget_46;
    m_pClassSensEdit[46]= ui->class1SensWidget_47;
    m_pClassSensEdit[47]= ui->class1SensWidget_48;
    m_pClassSensEdit[48]= ui->class1SensWidget_49;
    m_pClassSensEdit[49]= ui->class1SensWidget_50;
    m_pClassSensEdit[50]= ui->class1SensWidget_51;
    m_pClassSensEdit[51]= ui->class1SensWidget_52;

    m_pClassThresholdEdit[0]= ui->class1ThreadWidget;
    m_pClassThresholdEdit[1]= ui->class1ThreadWidget_2;
    m_pClassThresholdEdit[2]= ui->class1ThreadWidget_3;
    m_pClassThresholdEdit[3]= ui->class1ThreadWidget_4;
    m_pClassThresholdEdit[4]= ui->class1ThreadWidget_5;
    m_pClassThresholdEdit[5]= ui->class1ThreadWidget_6;
    m_pClassThresholdEdit[6]= ui->class1ThreadWidget_7;
    m_pClassThresholdEdit[7]= ui->class1ThreadWidget_8;
    m_pClassThresholdEdit[8]= ui->class1ThreadWidget_9;
    m_pClassThresholdEdit[9]= ui->class1ThreadWidget_10;
    m_pClassThresholdEdit[10]= ui->class1ThreadWidget_11;
    m_pClassThresholdEdit[11]= ui->class1ThreadWidget_12;
    m_pClassThresholdEdit[12]= ui->class1ThreadWidget_13;
    m_pClassThresholdEdit[13]= ui->class1ThreadWidget_14;
    m_pClassThresholdEdit[14]= ui->class1ThreadWidget_15;
    m_pClassThresholdEdit[15]= ui->class1ThreadWidget_16;
    m_pClassThresholdEdit[16]= ui->class1ThreadWidget_17;
    m_pClassThresholdEdit[17]= ui->class1ThreadWidget_18;
    m_pClassThresholdEdit[18]= ui->class1ThreadWidget_19;
    m_pClassThresholdEdit[19]= ui->class1ThreadWidget_20;
    m_pClassThresholdEdit[20]= ui->class1ThreadWidget_21;
    m_pClassThresholdEdit[21]= ui->class1ThreadWidget_22;
    m_pClassThresholdEdit[22]= ui->class1ThreadWidget_23;
    m_pClassThresholdEdit[23]= ui->class1ThreadWidget_24;
    m_pClassThresholdEdit[24]= ui->class1ThreadWidget_25;
    m_pClassThresholdEdit[25]= ui->class1ThreadWidget_26;
    m_pClassThresholdEdit[26]= ui->class1ThreadWidget_27;
    m_pClassThresholdEdit[27]= ui->class1ThreadWidget_28;
    m_pClassThresholdEdit[28]= ui->class1ThreadWidget_29;
    m_pClassThresholdEdit[29]= ui->class1ThreadWidget_30;
    m_pClassThresholdEdit[30]= ui->class1ThreadWidget_31;
    m_pClassThresholdEdit[31]= ui->class1ThreadWidget_32;
    m_pClassThresholdEdit[32]= ui->class1ThreadWidget_33;
    m_pClassThresholdEdit[33]= ui->class1ThreadWidget_34;
    m_pClassThresholdEdit[34]= ui->class1ThreadWidget_35;
    m_pClassThresholdEdit[35]= ui->class1ThreadWidget_36;
    m_pClassThresholdEdit[36]= ui->class1ThreadWidget_37;
    m_pClassThresholdEdit[37]= ui->class1ThreadWidget_38;
    m_pClassThresholdEdit[38]= ui->class1ThreadWidget_39;
    m_pClassThresholdEdit[39]= ui->class1ThreadWidget_40;
    m_pClassThresholdEdit[40]= ui->class1ThreadWidget_41;
    m_pClassThresholdEdit[41]= ui->class1ThreadWidget_42;
    m_pClassThresholdEdit[42]= ui->class1ThreadWidget_43;
    m_pClassThresholdEdit[43]= ui->class1ThreadWidget_44;
    m_pClassThresholdEdit[44]= ui->class1ThreadWidget_45;
    m_pClassThresholdEdit[45]= ui->class1ThreadWidget_46;
    m_pClassThresholdEdit[46]= ui->class1ThreadWidget_47;
    m_pClassThresholdEdit[47]= ui->class1ThreadWidget_48;
    m_pClassThresholdEdit[48]= ui->class1ThreadWidget_49;
    m_pClassThresholdEdit[49]= ui->class1ThreadWidget_50;
    m_pClassThresholdEdit[50]= ui->class1ThreadWidget_51;
    m_pClassThresholdEdit[51]= ui->class1ThreadWidget_52;

    //高级参数
    m_pBoxClassLable[0]= ui->labelBox1;
    m_pBoxClassLable[1]= ui->labelBox1_2;
    m_pBoxClassLable[2]= ui->labelBox1_3;
    m_pBoxClassLable[3]= ui->labelBox1_4;
    m_pBoxClassLable[4]= ui->labelBox1_5;
    m_pBoxClassLable[5]= ui->labelBox1_6;
    m_pBoxClassLable[6]= ui->labelBox1_7;
    m_pBoxClassLable[7]= ui->labelBox1_8;
    m_pBoxClassLable[8]= ui->labelBox1_9;
    m_pBoxClassLable[9]= ui->labelBox1_10;
    m_pBoxClassLable[10]= ui->labelBox1_11;
    m_pBoxClassLable[11]= ui->labelBox1_12;
    m_pBoxClassLable[12]= ui->labelBox1_13;
    m_pBoxClassLable[13]= ui->labelBox1_14;
    m_pBoxClassLable[14]= ui->labelBox1_15;
    m_pBoxClassLable[15]= ui->labelBox1_16;
    m_pBoxClassLable[16]= ui->labelBox1_17;
    m_pBoxClassLable[17]= ui->labelBox1_18;
    m_pBoxClassLable[18]= ui->labelBox1_19;
    m_pBoxClassLable[19]= ui->labelBox1_20;
    m_pBoxClassLable[20]= ui->labelBox1_21;
    m_pBoxClassLable[21]= ui->labelBox1_22;
    m_pBoxClassLable[22]= ui->labelBox1_23;
    m_pBoxClassLable[23]= ui->labelBox1_24;
    m_pBoxClassLable[24]= ui->labelBox1_25;
    m_pBoxClassLable[25]= ui->labelBox1_26;
    m_pBoxClassLable[26]= ui->labelBox1_27;
    m_pBoxClassLable[27]= ui->labelBox1_28;
    m_pBoxClassLable[28]= ui->labelBox1_29;
    m_pBoxClassLable[29]= ui->labelBox1_30;
    m_pBoxClassLable[30]= ui->labelBox1_31;
    m_pBoxClassLable[31]= ui->labelBox1_32;
    m_pBoxClassLable[32]= ui->labelBox1_33;
    m_pBoxClassLable[33]= ui->labelBox1_34;
    m_pBoxClassLable[34]= ui->labelBox1_35;
    m_pBoxClassLable[35]= ui->labelBox1_36;
    m_pBoxClassLable[36]= ui->labelBox1_37;
    m_pBoxClassLable[37]= ui->labelBox1_38;
    m_pBoxClassLable[38]= ui->labelBox1_39;
    m_pBoxClassLable[39]= ui->labelBox1_40;
    m_pBoxClassLable[40]= ui->labelBox1_41;
    m_pBoxClassLable[41]= ui->labelBox1_42;
    m_pBoxClassLable[42]= ui->labelBox1_43;
    m_pBoxClassLable[43]= ui->labelBox1_44;
    m_pBoxClassLable[44]= ui->labelBox1_45;
    m_pBoxClassLable[45]= ui->labelBox1_46;
    m_pBoxClassLable[46]= ui->labelBox1_47;
    m_pBoxClassLable[47]= ui->labelBox1_48;
    m_pBoxClassLable[48]= ui->labelBox1_49;
    m_pBoxClassLable[49]= ui->labelBox1_50;
    m_pBoxClassLable[50]= ui->labelBox1_51;
    m_pBoxClassLable[51]= ui->labelBox1_52;

    m_pBoxClassLable2[0]= ui->labelBox2;
    m_pBoxClassLable2[1]= ui->labelBox2_2;
    m_pBoxClassLable2[2]= ui->labelBox2_3;
    m_pBoxClassLable2[3]= ui->labelBox2_4;
    m_pBoxClassLable2[4]= ui->labelBox2_5;
    m_pBoxClassLable2[5]= ui->labelBox2_6;
    m_pBoxClassLable2[6]= ui->labelBox2_7;
    m_pBoxClassLable2[7]= ui->labelBox2_8;
    m_pBoxClassLable2[8]= ui->labelBox2_9;
    m_pBoxClassLable2[9]= ui->labelBox2_10;
    m_pBoxClassLable2[10]= ui->labelBox2_11;
    m_pBoxClassLable2[11]= ui->labelBox2_12;
    m_pBoxClassLable2[12]= ui->labelBox2_13;
    m_pBoxClassLable2[13]= ui->labelBox2_14;
    m_pBoxClassLable2[14]= ui->labelBox2_15;
    m_pBoxClassLable2[15]= ui->labelBox2_16;
    m_pBoxClassLable2[16]= ui->labelBox2_17;
    m_pBoxClassLable2[17]= ui->labelBox2_18;
    m_pBoxClassLable2[18]= ui->labelBox2_19;
    m_pBoxClassLable2[19]= ui->labelBox2_20;
    m_pBoxClassLable2[20]= ui->labelBox2_21;
    m_pBoxClassLable2[21]= ui->labelBox2_22;
    m_pBoxClassLable2[22]= ui->labelBox2_23;
    m_pBoxClassLable2[23]= ui->labelBox2_24;
    m_pBoxClassLable2[24]= ui->labelBox2_25;
    m_pBoxClassLable2[25]= ui->labelBox2_26;
    m_pBoxClassLable2[26]= ui->labelBox2_27;
    m_pBoxClassLable2[27]= ui->labelBox2_28;
    m_pBoxClassLable2[28]= ui->labelBox2_29;
    m_pBoxClassLable2[29]= ui->labelBox2_30;
    m_pBoxClassLable2[30]= ui->labelBox2_31;
    m_pBoxClassLable2[31]= ui->labelBox2_32;
    m_pBoxClassLable2[32]= ui->labelBox2_33;
    m_pBoxClassLable2[33]= ui->labelBox2_34;
    m_pBoxClassLable2[34]= ui->labelBox2_35;
    m_pBoxClassLable2[35]= ui->labelBox2_36;
    m_pBoxClassLable2[36]= ui->labelBox2_37;
    m_pBoxClassLable2[37]= ui->labelBox2_38;
    m_pBoxClassLable2[38]= ui->labelBox2_39;
    m_pBoxClassLable2[39]= ui->labelBox2_40;
    m_pBoxClassLable2[40]= ui->labelBox2_41;
    m_pBoxClassLable2[41]= ui->labelBox2_42;
    m_pBoxClassLable2[42]= ui->labelBox2_43;
    m_pBoxClassLable2[43]= ui->labelBox2_44;
    m_pBoxClassLable2[44]= ui->labelBox2_45;
    m_pBoxClassLable2[45]= ui->labelBox2_46;
    m_pBoxClassLable2[46]= ui->labelBox2_47;
    m_pBoxClassLable2[47]= ui->labelBox2_48;
    m_pBoxClassLable2[48]= ui->labelBox2_49;
    m_pBoxClassLable2[49]= ui->labelBox2_50;
    m_pBoxClassLable2[50]= ui->labelBox2_51;
    m_pBoxClassLable2[51]= ui->labelBox2_52;

    m_pBoxClassEdit[0]= ui->lineEditBox1;
    m_pBoxClassEdit[1]= ui->lineEditBox1_2;
    m_pBoxClassEdit[2]= ui->lineEditBox1_3;
    m_pBoxClassEdit[3]= ui->lineEditBox1_4;
    m_pBoxClassEdit[4]= ui->lineEditBox1_5;
    m_pBoxClassEdit[5]= ui->lineEditBox1_6;
    m_pBoxClassEdit[6]= ui->lineEditBox1_7;
    m_pBoxClassEdit[7]= ui->lineEditBox1_8;
    m_pBoxClassEdit[8]= ui->lineEditBox1_9;
    m_pBoxClassEdit[9]= ui->lineEditBox1_10;
    m_pBoxClassEdit[10]= ui->lineEditBox1_11;
    m_pBoxClassEdit[11]= ui->lineEditBox1_12;
    m_pBoxClassEdit[12]= ui->lineEditBox1_13;
    m_pBoxClassEdit[13]= ui->lineEditBox1_14;
    m_pBoxClassEdit[14]= ui->lineEditBox1_15;
    m_pBoxClassEdit[15]= ui->lineEditBox1_16;
    m_pBoxClassEdit[16]= ui->lineEditBox1_17;
    m_pBoxClassEdit[17]= ui->lineEditBox1_18;
    m_pBoxClassEdit[18]= ui->lineEditBox1_19;
    m_pBoxClassEdit[19]= ui->lineEditBox1_20;
    m_pBoxClassEdit[20]= ui->lineEditBox1_21;
    m_pBoxClassEdit[21]= ui->lineEditBox1_22;
    m_pBoxClassEdit[22]= ui->lineEditBox1_23;
    m_pBoxClassEdit[23]= ui->lineEditBox1_24;
    m_pBoxClassEdit[24]= ui->lineEditBox1_25;
    m_pBoxClassEdit[25]= ui->lineEditBox1_26;
    m_pBoxClassEdit[26]= ui->lineEditBox1_27;
    m_pBoxClassEdit[27]= ui->lineEditBox1_28;
    m_pBoxClassEdit[28]= ui->lineEditBox1_29;
    m_pBoxClassEdit[29]= ui->lineEditBox1_30;
    m_pBoxClassEdit[30]= ui->lineEditBox1_31;
    m_pBoxClassEdit[31]= ui->lineEditBox1_32;
    m_pBoxClassEdit[32]= ui->lineEditBox1_33;
    m_pBoxClassEdit[33]= ui->lineEditBox1_34;
    m_pBoxClassEdit[34]= ui->lineEditBox1_35;
    m_pBoxClassEdit[35]= ui->lineEditBox1_36;
    m_pBoxClassEdit[36]= ui->lineEditBox1_37;
    m_pBoxClassEdit[37]= ui->lineEditBox1_38;
    m_pBoxClassEdit[38]= ui->lineEditBox1_39;
    m_pBoxClassEdit[39]= ui->lineEditBox1_40;
    m_pBoxClassEdit[40]= ui->lineEditBox1_41;
    m_pBoxClassEdit[41]= ui->lineEditBox1_42;
    m_pBoxClassEdit[42]= ui->lineEditBox1_43;
    m_pBoxClassEdit[43]= ui->lineEditBox1_44;
    m_pBoxClassEdit[44]= ui->lineEditBox1_45;
    m_pBoxClassEdit[45]= ui->lineEditBox1_46;
    m_pBoxClassEdit[46]= ui->lineEditBox1_47;
    m_pBoxClassEdit[47]= ui->lineEditBox1_48;
    m_pBoxClassEdit[48]= ui->lineEditBox1_49;
    m_pBoxClassEdit[49]= ui->lineEditBox1_50;
    m_pBoxClassEdit[50]= ui->lineEditBox1_51;
    m_pBoxClassEdit[51]= ui->lineEditBox1_52;

    m_pBoxClassEdit2[0]= ui->lineEditBox2;
    m_pBoxClassEdit2[1]= ui->lineEditBox2_2;
    m_pBoxClassEdit2[2]= ui->lineEditBox2_3;
    m_pBoxClassEdit2[3]= ui->lineEditBox2_4;
    m_pBoxClassEdit2[4]= ui->lineEditBox2_5;
    m_pBoxClassEdit2[5]= ui->lineEditBox2_6;
    m_pBoxClassEdit2[6]= ui->lineEditBox2_7;
    m_pBoxClassEdit2[7]= ui->lineEditBox2_8;
    m_pBoxClassEdit2[8]= ui->lineEditBox2_9;
    m_pBoxClassEdit2[9]= ui->lineEditBox2_10;
    m_pBoxClassEdit2[10]= ui->lineEditBox2_11;
    m_pBoxClassEdit2[11]= ui->lineEditBox2_12;
    m_pBoxClassEdit2[12]= ui->lineEditBox2_13;
    m_pBoxClassEdit2[13]= ui->lineEditBox2_14;
    m_pBoxClassEdit2[14]= ui->lineEditBox2_15;
    m_pBoxClassEdit2[15]= ui->lineEditBox2_16;
    m_pBoxClassEdit2[16]= ui->lineEditBox2_17;
    m_pBoxClassEdit2[17]= ui->lineEditBox2_18;
    m_pBoxClassEdit2[18]= ui->lineEditBox2_19;
    m_pBoxClassEdit2[19]= ui->lineEditBox2_20;
    m_pBoxClassEdit2[20]= ui->lineEditBox2_21;
    m_pBoxClassEdit2[21]= ui->lineEditBox2_22;
    m_pBoxClassEdit2[22]= ui->lineEditBox2_23;
    m_pBoxClassEdit2[23]= ui->lineEditBox2_24;
    m_pBoxClassEdit2[24]= ui->lineEditBox2_25;
    m_pBoxClassEdit2[25]= ui->lineEditBox2_26;
    m_pBoxClassEdit2[26]= ui->lineEditBox2_27;
    m_pBoxClassEdit2[27]= ui->lineEditBox2_28;
    m_pBoxClassEdit2[28]= ui->lineEditBox2_29;
    m_pBoxClassEdit2[29]= ui->lineEditBox2_30;
    m_pBoxClassEdit2[30]= ui->lineEditBox2_31;
    m_pBoxClassEdit2[31]= ui->lineEditBox2_32;
    m_pBoxClassEdit2[32]= ui->lineEditBox2_33;
    m_pBoxClassEdit2[33]= ui->lineEditBox2_34;
    m_pBoxClassEdit2[34]= ui->lineEditBox2_35;
    m_pBoxClassEdit2[35]= ui->lineEditBox2_36;
    m_pBoxClassEdit2[36]= ui->lineEditBox2_37;
    m_pBoxClassEdit2[37]= ui->lineEditBox2_38;
    m_pBoxClassEdit2[38]= ui->lineEditBox2_39;
    m_pBoxClassEdit2[39]= ui->lineEditBox2_40;
    m_pBoxClassEdit2[40]= ui->lineEditBox2_41;
    m_pBoxClassEdit2[41]= ui->lineEditBox2_42;
    m_pBoxClassEdit2[42]= ui->lineEditBox2_43;
    m_pBoxClassEdit2[43]= ui->lineEditBox2_44;
    m_pBoxClassEdit2[44]= ui->lineEditBox2_45;
    m_pBoxClassEdit2[45]= ui->lineEditBox2_46;
    m_pBoxClassEdit2[46]= ui->lineEditBox2_47;
    m_pBoxClassEdit2[47]= ui->lineEditBox2_48;
    m_pBoxClassEdit2[48]= ui->lineEditBox2_49;
    m_pBoxClassEdit2[49]= ui->lineEditBox2_50;
    m_pBoxClassEdit2[50]= ui->lineEditBox2_51;
    m_pBoxClassEdit2[51]= ui->lineEditBox2_52;

    m_pTickModeClass[0]= ui->tickModeCheckBox1;
    m_pTickModeClass[1]= ui->tickModeCheckBox1_2;
    m_pTickModeClass[2]= ui->tickModeCheckBox1_3;
    m_pTickModeClass[3]= ui->tickModeCheckBox1_4;
    m_pTickModeClass[4]= ui->tickModeCheckBox1_5;
    m_pTickModeClass[5]= ui->tickModeCheckBox1_6;
    m_pTickModeClass[6]= ui->tickModeCheckBox1_7;
    m_pTickModeClass[7]= ui->tickModeCheckBox1_8;
    m_pTickModeClass[8]= ui->tickModeCheckBox1_9;
    m_pTickModeClass[9]= ui->tickModeCheckBox1_10;
    m_pTickModeClass[10]= ui->tickModeCheckBox1_11;
    m_pTickModeClass[11]= ui->tickModeCheckBox1_12;
    m_pTickModeClass[12]= ui->tickModeCheckBox1_13;
    m_pTickModeClass[13]= ui->tickModeCheckBox1_14;
    m_pTickModeClass[14]= ui->tickModeCheckBox1_15;
    m_pTickModeClass[15]= ui->tickModeCheckBox1_16;
    m_pTickModeClass[16]= ui->tickModeCheckBox1_17;
    m_pTickModeClass[17]= ui->tickModeCheckBox1_18;
    m_pTickModeClass[18]= ui->tickModeCheckBox1_19;
    m_pTickModeClass[19]= ui->tickModeCheckBox1_20;
    m_pTickModeClass[20]= ui->tickModeCheckBox1_21;
    m_pTickModeClass[21]= ui->tickModeCheckBox1_22;
    m_pTickModeClass[22]= ui->tickModeCheckBox1_23;
    m_pTickModeClass[23]= ui->tickModeCheckBox1_24;
    m_pTickModeClass[24]= ui->tickModeCheckBox1_25;
    m_pTickModeClass[25]= ui->tickModeCheckBox1_26;
    m_pTickModeClass[26]= ui->tickModeCheckBox1_27;
    m_pTickModeClass[27]= ui->tickModeCheckBox1_28;
    m_pTickModeClass[28]= ui->tickModeCheckBox1_29;
    m_pTickModeClass[29]= ui->tickModeCheckBox1_30;
    m_pTickModeClass[30]= ui->tickModeCheckBox1_31;
    m_pTickModeClass[31]= ui->tickModeCheckBox1_32;
    m_pTickModeClass[32]= ui->tickModeCheckBox1_33;
    m_pTickModeClass[33]= ui->tickModeCheckBox1_34;
    m_pTickModeClass[34]= ui->tickModeCheckBox1_35;
    m_pTickModeClass[35]= ui->tickModeCheckBox1_36;
    m_pTickModeClass[36]= ui->tickModeCheckBox1_37;
    m_pTickModeClass[37]= ui->tickModeCheckBox1_38;
    m_pTickModeClass[38]= ui->tickModeCheckBox1_39;
    m_pTickModeClass[39]= ui->tickModeCheckBox1_40;
    m_pTickModeClass[40]= ui->tickModeCheckBox1_41;
    m_pTickModeClass[41]= ui->tickModeCheckBox1_42;
    m_pTickModeClass[42]= ui->tickModeCheckBox1_43;
    m_pTickModeClass[43]= ui->tickModeCheckBox1_44;
    m_pTickModeClass[44]= ui->tickModeCheckBox1_45;
    m_pTickModeClass[45]= ui->tickModeCheckBox1_46;
    m_pTickModeClass[46]= ui->tickModeCheckBox1_47;
    m_pTickModeClass[47]= ui->tickModeCheckBox1_48;
    m_pTickModeClass[48]= ui->tickModeCheckBox1_49;
    m_pTickModeClass[49]= ui->tickModeCheckBox1_50;
    m_pTickModeClass[50]= ui->tickModeCheckBox1_51;
    m_pTickModeClass[51]= ui->tickModeCheckBox1_52;

    m_pFixedClass[0]= ui->fixedModecheckBox1;
    m_pFixedClass[1]= ui->fixedModecheckBox1_2;
    m_pFixedClass[2]= ui->fixedModecheckBox1_3;
    m_pFixedClass[3]= ui->fixedModecheckBox1_4;
    m_pFixedClass[4]= ui->fixedModecheckBox1_5;
    m_pFixedClass[5]= ui->fixedModecheckBox1_6;
    m_pFixedClass[6]= ui->fixedModecheckBox1_7;
    m_pFixedClass[7]= ui->fixedModecheckBox1_8;
    m_pFixedClass[8]= ui->fixedModecheckBox1_9;
    m_pFixedClass[9]= ui->fixedModecheckBox1_10;
    m_pFixedClass[10]= ui->fixedModecheckBox1_11;
    m_pFixedClass[11]= ui->fixedModecheckBox1_12;
    m_pFixedClass[12]= ui->fixedModecheckBox1_13;
    m_pFixedClass[13]= ui->fixedModecheckBox1_14;
    m_pFixedClass[14]= ui->fixedModecheckBox1_15;
    m_pFixedClass[15]= ui->fixedModecheckBox1_16;
    m_pFixedClass[16]= ui->fixedModecheckBox1_17;
    m_pFixedClass[17]= ui->fixedModecheckBox1_18;
    m_pFixedClass[18]= ui->fixedModecheckBox1_19;
    m_pFixedClass[19]= ui->fixedModecheckBox1_20;
    m_pFixedClass[20]= ui->fixedModecheckBox1_21;
    m_pFixedClass[21]= ui->fixedModecheckBox1_22;
    m_pFixedClass[22]= ui->fixedModecheckBox1_23;
    m_pFixedClass[23]= ui->fixedModecheckBox1_24;
    m_pFixedClass[24]= ui->fixedModecheckBox1_25;
    m_pFixedClass[25]= ui->fixedModecheckBox1_26;
    m_pFixedClass[26]= ui->fixedModecheckBox1_27;
    m_pFixedClass[27]= ui->fixedModecheckBox1_28;
    m_pFixedClass[28]= ui->fixedModecheckBox1_29;
    m_pFixedClass[29]= ui->fixedModecheckBox1_30;
    m_pFixedClass[30]= ui->fixedModecheckBox1_31;
    m_pFixedClass[31]= ui->fixedModecheckBox1_32;
    m_pFixedClass[32]= ui->fixedModecheckBox1_33;
    m_pFixedClass[33]= ui->fixedModecheckBox1_34;
    m_pFixedClass[34]= ui->fixedModecheckBox1_35;
    m_pFixedClass[35]= ui->fixedModecheckBox1_36;
    m_pFixedClass[36]= ui->fixedModecheckBox1_37;
    m_pFixedClass[37]= ui->fixedModecheckBox1_38;
    m_pFixedClass[38]= ui->fixedModecheckBox1_39;
    m_pFixedClass[39]= ui->fixedModecheckBox1_40;
    m_pFixedClass[40]= ui->fixedModecheckBox1_41;
    m_pFixedClass[41]= ui->fixedModecheckBox1_42;
    m_pFixedClass[42]= ui->fixedModecheckBox1_43;
    m_pFixedClass[43]= ui->fixedModecheckBox1_44;
    m_pFixedClass[44]= ui->fixedModecheckBox1_45;
    m_pFixedClass[45]= ui->fixedModecheckBox1_46;
    m_pFixedClass[46]= ui->fixedModecheckBox1_47;
    m_pFixedClass[47]= ui->fixedModecheckBox1_48;
    m_pFixedClass[48]= ui->fixedModecheckBox1_49;
    m_pFixedClass[49]= ui->fixedModecheckBox1_50;
    m_pFixedClass[50]= ui->fixedModecheckBox1_51;
    m_pFixedClass[51]= ui->fixedModecheckBox1_52;

    m_pDelayModeClass[0]= ui->delayModecheckBox1;
    m_pDelayModeClass[1]= ui->delayModecheckBox1_2;
    m_pDelayModeClass[2]= ui->delayModecheckBox1_3;
    m_pDelayModeClass[3]= ui->delayModecheckBox1_4;
    m_pDelayModeClass[4]= ui->delayModecheckBox1_5;
    m_pDelayModeClass[5]= ui->delayModecheckBox1_6;
    m_pDelayModeClass[6]= ui->delayModecheckBox1_7;
    m_pDelayModeClass[7]= ui->delayModecheckBox1_8;
    m_pDelayModeClass[8]= ui->delayModecheckBox1_9;
    m_pDelayModeClass[9]= ui->delayModecheckBox1_10;
    m_pDelayModeClass[10]= ui->delayModecheckBox1_11;
    m_pDelayModeClass[11]= ui->delayModecheckBox1_12;
    m_pDelayModeClass[12]= ui->delayModecheckBox1_13;
    m_pDelayModeClass[13]= ui->delayModecheckBox1_14;
    m_pDelayModeClass[14]= ui->delayModecheckBox1_15;
    m_pDelayModeClass[15]= ui->delayModecheckBox1_16;
    m_pDelayModeClass[16]= ui->delayModecheckBox1_17;
    m_pDelayModeClass[17]= ui->delayModecheckBox1_18;
    m_pDelayModeClass[18]= ui->delayModecheckBox1_19;
    m_pDelayModeClass[19]= ui->delayModecheckBox1_20;
    m_pDelayModeClass[20]= ui->delayModecheckBox1_21;
    m_pDelayModeClass[21]= ui->delayModecheckBox1_22;
    m_pDelayModeClass[22]= ui->delayModecheckBox1_23;
    m_pDelayModeClass[23]= ui->delayModecheckBox1_24;
    m_pDelayModeClass[24]= ui->delayModecheckBox1_25;
    m_pDelayModeClass[25]= ui->delayModecheckBox1_26;
    m_pDelayModeClass[26]= ui->delayModecheckBox1_27;
    m_pDelayModeClass[27]= ui->delayModecheckBox1_28;
    m_pDelayModeClass[28]= ui->delayModecheckBox1_29;
    m_pDelayModeClass[29]= ui->delayModecheckBox1_30;
    m_pDelayModeClass[30]= ui->delayModecheckBox1_31;
    m_pDelayModeClass[31]= ui->delayModecheckBox1_32;
    m_pDelayModeClass[32]= ui->delayModecheckBox1_33;
    m_pDelayModeClass[33]= ui->delayModecheckBox1_34;
    m_pDelayModeClass[34]= ui->delayModecheckBox1_35;
    m_pDelayModeClass[35]= ui->delayModecheckBox1_36;
    m_pDelayModeClass[36]= ui->delayModecheckBox1_37;
    m_pDelayModeClass[37]= ui->delayModecheckBox1_38;
    m_pDelayModeClass[38]= ui->delayModecheckBox1_39;
    m_pDelayModeClass[39]= ui->delayModecheckBox1_40;
    m_pDelayModeClass[40]= ui->delayModecheckBox1_41;
    m_pDelayModeClass[41]= ui->delayModecheckBox1_42;
    m_pDelayModeClass[42]= ui->delayModecheckBox1_43;
    m_pDelayModeClass[43]= ui->delayModecheckBox1_44;
    m_pDelayModeClass[44]= ui->delayModecheckBox1_45;
    m_pDelayModeClass[45]= ui->delayModecheckBox1_46;
    m_pDelayModeClass[46]= ui->delayModecheckBox1_47;
    m_pDelayModeClass[47]= ui->delayModecheckBox1_48;
    m_pDelayModeClass[48]= ui->delayModecheckBox1_49;
    m_pDelayModeClass[49]= ui->delayModecheckBox1_50;
    m_pDelayModeClass[50]= ui->delayModecheckBox1_51;
    m_pDelayModeClass[51]= ui->delayModecheckBox1_52;

    m_pModeLabel[0]= ui->labelMode;
    m_pModeLabel[1]= ui->labelMode_2;
    m_pModeLabel[2]= ui->labelMode_3;
    m_pModeLabel[3]= ui->labelMode_4;
    m_pModeLabel[4]= ui->labelMode_5;
    m_pModeLabel[5]= ui->labelMode_6;
    m_pModeLabel[6]= ui->labelMode_7;
    m_pModeLabel[7]= ui->labelMode_8;
    m_pModeLabel[8]= ui->labelMode_9;
    m_pModeLabel[9]= ui->labelMode_10;
    m_pModeLabel[10]= ui->labelMode_11;
    m_pModeLabel[11]= ui->labelMode_12;
    m_pModeLabel[12]= ui->labelMode_13;
    m_pModeLabel[13]= ui->labelMode_14;
    m_pModeLabel[14]= ui->labelMode_15;
    m_pModeLabel[15]= ui->labelMode_16;
    m_pModeLabel[16]= ui->labelMode_17;
    m_pModeLabel[17]= ui->labelMode_18;
    m_pModeLabel[18]= ui->labelMode_19;
    m_pModeLabel[19]= ui->labelMode_20;
    m_pModeLabel[20]= ui->labelMode_21;
    m_pModeLabel[21]= ui->labelMode_22;
    m_pModeLabel[22]= ui->labelMode_23;
    m_pModeLabel[23]= ui->labelMode_24;
    m_pModeLabel[24]= ui->labelMode_25;
    m_pModeLabel[25]= ui->labelMode_26;
    m_pModeLabel[26]= ui->labelMode_27;
    m_pModeLabel[27]= ui->labelMode_28;
    m_pModeLabel[28]= ui->labelMode_29;
    m_pModeLabel[29]= ui->labelMode_30;
    m_pModeLabel[30]= ui->labelMode_31;
    m_pModeLabel[31]= ui->labelMode_32;
    m_pModeLabel[32]= ui->labelMode_33;
    m_pModeLabel[33]= ui->labelMode_34;
    m_pModeLabel[34]= ui->labelMode_35;
    m_pModeLabel[35]= ui->labelMode_36;
    m_pModeLabel[36]= ui->labelMode_37;
    m_pModeLabel[37]= ui->labelMode_38;
    m_pModeLabel[38]= ui->labelMode_39;
    m_pModeLabel[39]= ui->labelMode_40;
    m_pModeLabel[40]= ui->labelMode_41;
    m_pModeLabel[41]= ui->labelMode_42;
    m_pModeLabel[42]= ui->labelMode_43;
    m_pModeLabel[43]= ui->labelMode_44;
    m_pModeLabel[44]= ui->labelMode_45;
    m_pModeLabel[45]= ui->labelMode_46;
    m_pModeLabel[46]= ui->labelMode_47;
    m_pModeLabel[47]= ui->labelMode_48;
    m_pModeLabel[48]= ui->labelMode_49;
    m_pModeLabel[49]= ui->labelMode_50;
    m_pModeLabel[50]= ui->labelMode_51;
    m_pModeLabel[51]= ui->labelMode_52;

    m_pEjctorArithCombox[0]= ui->comboBoxMode;
    m_pEjctorArithCombox[1]= ui->comboBoxMode_2;
    m_pEjctorArithCombox[2]= ui->comboBoxMode_3;
    m_pEjctorArithCombox[3]= ui->comboBoxMode_4;
    m_pEjctorArithCombox[4]= ui->comboBoxMode_5;
    m_pEjctorArithCombox[5]= ui->comboBoxMode_6;
    m_pEjctorArithCombox[6]= ui->comboBoxMode_7;
    m_pEjctorArithCombox[7]= ui->comboBoxMode_8;
    m_pEjctorArithCombox[8]= ui->comboBoxMode_9;
    m_pEjctorArithCombox[9]= ui->comboBoxMode_10;
    m_pEjctorArithCombox[10]= ui->comboBoxMode_11;
    m_pEjctorArithCombox[11]= ui->comboBoxMode_12;
    m_pEjctorArithCombox[12]= ui->comboBoxMode_13;
    m_pEjctorArithCombox[13]= ui->comboBoxMode_14;
    m_pEjctorArithCombox[14]= ui->comboBoxMode_15;
    m_pEjctorArithCombox[15]= ui->comboBoxMode_16;
    m_pEjctorArithCombox[16]= ui->comboBoxMode_17;
    m_pEjctorArithCombox[17]= ui->comboBoxMode_18;
    m_pEjctorArithCombox[18]= ui->comboBoxMode_19;
    m_pEjctorArithCombox[19]= ui->comboBoxMode_20;
    m_pEjctorArithCombox[20]= ui->comboBoxMode_21;
    m_pEjctorArithCombox[21]= ui->comboBoxMode_22;
    m_pEjctorArithCombox[22]= ui->comboBoxMode_23;
    m_pEjctorArithCombox[23]= ui->comboBoxMode_24;
    m_pEjctorArithCombox[24]= ui->comboBoxMode_25;
    m_pEjctorArithCombox[25]= ui->comboBoxMode_26;
    m_pEjctorArithCombox[26]= ui->comboBoxMode_27;
    m_pEjctorArithCombox[27]= ui->comboBoxMode_28;
    m_pEjctorArithCombox[28]= ui->comboBoxMode_29;
    m_pEjctorArithCombox[29]= ui->comboBoxMode_30;
    m_pEjctorArithCombox[30]= ui->comboBoxMode_31;
    m_pEjctorArithCombox[31]= ui->comboBoxMode_32;
    m_pEjctorArithCombox[32]= ui->comboBoxMode_33;
    m_pEjctorArithCombox[33]= ui->comboBoxMode_34;
    m_pEjctorArithCombox[34]= ui->comboBoxMode_35;
    m_pEjctorArithCombox[35]= ui->comboBoxMode_36;
    m_pEjctorArithCombox[36]= ui->comboBoxMode_37;
    m_pEjctorArithCombox[37]= ui->comboBoxMode_38;
    m_pEjctorArithCombox[38]= ui->comboBoxMode_39;
    m_pEjctorArithCombox[39]= ui->comboBoxMode_40;
    m_pEjctorArithCombox[40]= ui->comboBoxMode_41;
    m_pEjctorArithCombox[41]= ui->comboBoxMode_42;
    m_pEjctorArithCombox[42]= ui->comboBoxMode_43;
    m_pEjctorArithCombox[43]= ui->comboBoxMode_44;
    m_pEjctorArithCombox[44]= ui->comboBoxMode_45;
    m_pEjctorArithCombox[45]= ui->comboBoxMode_46;
    m_pEjctorArithCombox[46]= ui->comboBoxMode_47;
    m_pEjctorArithCombox[47]= ui->comboBoxMode_48;
    m_pEjctorArithCombox[48]= ui->comboBoxMode_49;
    m_pEjctorArithCombox[49]= ui->comboBoxMode_50;
    m_pEjctorArithCombox[50]= ui->comboBoxMode_51;
    m_pEjctorArithCombox[51]= ui->comboBoxMode_52;

    m_pHWRatioLabel[0]= ui->labelHWRatio1;
    m_pHWRatioLabel[1]= ui->labelHWRatio1_2;
    m_pHWRatioLabel[2]= ui->labelHWRatio1_3;
    m_pHWRatioLabel[3]= ui->labelHWRatio1_4;
    m_pHWRatioLabel[4]= ui->labelHWRatio1_5;
    m_pHWRatioLabel[5]= ui->labelHWRatio1_6;
    m_pHWRatioLabel[6]= ui->labelHWRatio1_7;
    m_pHWRatioLabel[7]= ui->labelHWRatio1_8;
    m_pHWRatioLabel[8]= ui->labelHWRatio1_9;
    m_pHWRatioLabel[9]= ui->labelHWRatio1_10;
    m_pHWRatioLabel[10]= ui->labelHWRatio1_11;
    m_pHWRatioLabel[11]= ui->labelHWRatio1_12;
    m_pHWRatioLabel[12]= ui->labelHWRatio1_13;
    m_pHWRatioLabel[13]= ui->labelHWRatio1_14;
    m_pHWRatioLabel[14]= ui->labelHWRatio1_15;
    m_pHWRatioLabel[15]= ui->labelHWRatio1_16;
    m_pHWRatioLabel[16]= ui->labelHWRatio1_17;
    m_pHWRatioLabel[17]= ui->labelHWRatio1_18;
    m_pHWRatioLabel[18]= ui->labelHWRatio1_19;
    m_pHWRatioLabel[19]= ui->labelHWRatio1_20;
    m_pHWRatioLabel[20]= ui->labelHWRatio1_21;
    m_pHWRatioLabel[21]= ui->labelHWRatio1_22;
    m_pHWRatioLabel[22]= ui->labelHWRatio1_23;
    m_pHWRatioLabel[23]= ui->labelHWRatio1_24;
    m_pHWRatioLabel[24]= ui->labelHWRatio1_25;
    m_pHWRatioLabel[25]= ui->labelHWRatio1_26;
    m_pHWRatioLabel[26]= ui->labelHWRatio1_27;
    m_pHWRatioLabel[27]= ui->labelHWRatio1_28;
    m_pHWRatioLabel[28]= ui->labelHWRatio1_29;
    m_pHWRatioLabel[29]= ui->labelHWRatio1_30;
    m_pHWRatioLabel[30]= ui->labelHWRatio1_31;
    m_pHWRatioLabel[31]= ui->labelHWRatio1_32;
    m_pHWRatioLabel[32]= ui->labelHWRatio1_33;
    m_pHWRatioLabel[33]= ui->labelHWRatio1_34;
    m_pHWRatioLabel[34]= ui->labelHWRatio1_35;
    m_pHWRatioLabel[35]= ui->labelHWRatio1_36;
    m_pHWRatioLabel[36]= ui->labelHWRatio1_37;
    m_pHWRatioLabel[37]= ui->labelHWRatio1_38;
    m_pHWRatioLabel[38]= ui->labelHWRatio1_39;
    m_pHWRatioLabel[39]= ui->labelHWRatio1_40;
    m_pHWRatioLabel[40]= ui->labelHWRatio1_41;
    m_pHWRatioLabel[41]= ui->labelHWRatio1_42;
    m_pHWRatioLabel[42]= ui->labelHWRatio1_43;
    m_pHWRatioLabel[43]= ui->labelHWRatio1_44;
    m_pHWRatioLabel[44]= ui->labelHWRatio1_45;
    m_pHWRatioLabel[45]= ui->labelHWRatio1_46;
    m_pHWRatioLabel[46]= ui->labelHWRatio1_47;
    m_pHWRatioLabel[47]= ui->labelHWRatio1_48;
    m_pHWRatioLabel[48]= ui->labelHWRatio1_49;
    m_pHWRatioLabel[49]= ui->labelHWRatio1_50;
    m_pHWRatioLabel[50]= ui->labelHWRatio1_51;
    m_pHWRatioLabel[51]= ui->labelHWRatio1_52;

    m_pHWRatioEdit[0]= ui->lineEditHWRatio1;
    m_pHWRatioEdit[1]= ui->lineEditHWRatio1_2;
    m_pHWRatioEdit[2]= ui->lineEditHWRatio1_3;
    m_pHWRatioEdit[3]= ui->lineEditHWRatio1_4;
    m_pHWRatioEdit[4]= ui->lineEditHWRatio1_5;
    m_pHWRatioEdit[5]= ui->lineEditHWRatio1_6;
    m_pHWRatioEdit[6]= ui->lineEditHWRatio1_7;
    m_pHWRatioEdit[7]= ui->lineEditHWRatio1_8;
    m_pHWRatioEdit[8]= ui->lineEditHWRatio1_9;
    m_pHWRatioEdit[9]= ui->lineEditHWRatio1_10;
    m_pHWRatioEdit[10]= ui->lineEditHWRatio1_11;
    m_pHWRatioEdit[11]= ui->lineEditHWRatio1_12;
    m_pHWRatioEdit[12]= ui->lineEditHWRatio1_13;
    m_pHWRatioEdit[13]= ui->lineEditHWRatio1_14;
    m_pHWRatioEdit[14]= ui->lineEditHWRatio1_15;
    m_pHWRatioEdit[15]= ui->lineEditHWRatio1_16;
    m_pHWRatioEdit[16]= ui->lineEditHWRatio1_17;
    m_pHWRatioEdit[17]= ui->lineEditHWRatio1_18;
    m_pHWRatioEdit[18]= ui->lineEditHWRatio1_19;
    m_pHWRatioEdit[19]= ui->lineEditHWRatio1_20;
    m_pHWRatioEdit[20]= ui->lineEditHWRatio1_21;
    m_pHWRatioEdit[21]= ui->lineEditHWRatio1_22;
    m_pHWRatioEdit[22]= ui->lineEditHWRatio1_23;
    m_pHWRatioEdit[23]= ui->lineEditHWRatio1_24;
    m_pHWRatioEdit[24]= ui->lineEditHWRatio1_25;
    m_pHWRatioEdit[25]= ui->lineEditHWRatio1_26;
    m_pHWRatioEdit[26]= ui->lineEditHWRatio1_27;
    m_pHWRatioEdit[27]= ui->lineEditHWRatio1_28;
    m_pHWRatioEdit[28]= ui->lineEditHWRatio1_29;
    m_pHWRatioEdit[29]= ui->lineEditHWRatio1_30;
    m_pHWRatioEdit[30]= ui->lineEditHWRatio1_31;
    m_pHWRatioEdit[31]= ui->lineEditHWRatio1_32;
    m_pHWRatioEdit[32]= ui->lineEditHWRatio1_33;
    m_pHWRatioEdit[33]= ui->lineEditHWRatio1_34;
    m_pHWRatioEdit[34]= ui->lineEditHWRatio1_35;
    m_pHWRatioEdit[35]= ui->lineEditHWRatio1_36;
    m_pHWRatioEdit[36]= ui->lineEditHWRatio1_37;
    m_pHWRatioEdit[37]= ui->lineEditHWRatio1_38;
    m_pHWRatioEdit[38]= ui->lineEditHWRatio1_39;
    m_pHWRatioEdit[39]= ui->lineEditHWRatio1_40;
    m_pHWRatioEdit[40]= ui->lineEditHWRatio1_41;
    m_pHWRatioEdit[41]= ui->lineEditHWRatio1_42;
    m_pHWRatioEdit[42]= ui->lineEditHWRatio1_43;
    m_pHWRatioEdit[43]= ui->lineEditHWRatio1_44;
    m_pHWRatioEdit[44]= ui->lineEditHWRatio1_45;
    m_pHWRatioEdit[45]= ui->lineEditHWRatio1_46;
    m_pHWRatioEdit[46]= ui->lineEditHWRatio1_47;
    m_pHWRatioEdit[47]= ui->lineEditHWRatio1_48;
    m_pHWRatioEdit[48]= ui->lineEditHWRatio1_49;
    m_pHWRatioEdit[49]= ui->lineEditHWRatio1_50;
    m_pHWRatioEdit[50]= ui->lineEditHWRatio1_51;
    m_pHWRatioEdit[51]= ui->lineEditHWRatio1_52;

    m_pPosAdjustLabel[0]= ui->labelPosAdjust1;
    m_pPosAdjustLabel[1]= ui->labelPosAdjust1_2;
    m_pPosAdjustLabel[2]= ui->labelPosAdjust1_3;
    m_pPosAdjustLabel[3]= ui->labelPosAdjust1_4;
    m_pPosAdjustLabel[4]= ui->labelPosAdjust1_5;
    m_pPosAdjustLabel[5]= ui->labelPosAdjust1_6;
    m_pPosAdjustLabel[6]= ui->labelPosAdjust1_7;
    m_pPosAdjustLabel[7]= ui->labelPosAdjust1_8;
    m_pPosAdjustLabel[8]= ui->labelPosAdjust1_9;
    m_pPosAdjustLabel[9]= ui->labelPosAdjust1_10;
    m_pPosAdjustLabel[10]= ui->labelPosAdjust1_11;
    m_pPosAdjustLabel[11]= ui->labelPosAdjust1_12;
    m_pPosAdjustLabel[12]= ui->labelPosAdjust1_13;
    m_pPosAdjustLabel[13]= ui->labelPosAdjust1_14;
    m_pPosAdjustLabel[14]= ui->labelPosAdjust1_15;
    m_pPosAdjustLabel[15]= ui->labelPosAdjust1_16;
    m_pPosAdjustLabel[16]= ui->labelPosAdjust1_17;
    m_pPosAdjustLabel[17]= ui->labelPosAdjust1_18;
    m_pPosAdjustLabel[18]= ui->labelPosAdjust1_19;
    m_pPosAdjustLabel[19]= ui->labelPosAdjust1_20;
    m_pPosAdjustLabel[20]= ui->labelPosAdjust1_21;
    m_pPosAdjustLabel[21]= ui->labelPosAdjust1_22;
    m_pPosAdjustLabel[22]= ui->labelPosAdjust1_23;
    m_pPosAdjustLabel[23]= ui->labelPosAdjust1_24;
    m_pPosAdjustLabel[24]= ui->labelPosAdjust1_25;
    m_pPosAdjustLabel[25]= ui->labelPosAdjust1_26;
    m_pPosAdjustLabel[26]= ui->labelPosAdjust1_27;
    m_pPosAdjustLabel[27]= ui->labelPosAdjust1_28;
    m_pPosAdjustLabel[28]= ui->labelPosAdjust1_29;
    m_pPosAdjustLabel[29]= ui->labelPosAdjust1_30;
    m_pPosAdjustLabel[30]= ui->labelPosAdjust1_31;
    m_pPosAdjustLabel[31]= ui->labelPosAdjust1_32;
    m_pPosAdjustLabel[32]= ui->labelPosAdjust1_33;
    m_pPosAdjustLabel[33]= ui->labelPosAdjust1_34;
    m_pPosAdjustLabel[34]= ui->labelPosAdjust1_35;
    m_pPosAdjustLabel[35]= ui->labelPosAdjust1_36;
    m_pPosAdjustLabel[36]= ui->labelPosAdjust1_37;
    m_pPosAdjustLabel[37]= ui->labelPosAdjust1_38;
    m_pPosAdjustLabel[38]= ui->labelPosAdjust1_39;
    m_pPosAdjustLabel[39]= ui->labelPosAdjust1_40;
    m_pPosAdjustLabel[40]= ui->labelPosAdjust1_41;
    m_pPosAdjustLabel[41]= ui->labelPosAdjust1_42;
    m_pPosAdjustLabel[42]= ui->labelPosAdjust1_43;
    m_pPosAdjustLabel[43]= ui->labelPosAdjust1_44;
    m_pPosAdjustLabel[44]= ui->labelPosAdjust1_45;
    m_pPosAdjustLabel[45]= ui->labelPosAdjust1_46;
    m_pPosAdjustLabel[46]= ui->labelPosAdjust1_47;
    m_pPosAdjustLabel[47]= ui->labelPosAdjust1_48;
    m_pPosAdjustLabel[48]= ui->labelPosAdjust1_49;
    m_pPosAdjustLabel[49]= ui->labelPosAdjust1_50;
    m_pPosAdjustLabel[50]= ui->labelPosAdjust1_51;
    m_pPosAdjustLabel[51]= ui->labelPosAdjust1_52;

    m_pPosAdjustEdit[0]= ui->lineEditPosAdjust1;
    m_pPosAdjustEdit[1]= ui->lineEditPosAdjust1_2;
    m_pPosAdjustEdit[2]= ui->lineEditPosAdjust1_3;
    m_pPosAdjustEdit[3]= ui->lineEditPosAdjust1_4;
    m_pPosAdjustEdit[4]= ui->lineEditPosAdjust1_5;
    m_pPosAdjustEdit[5]= ui->lineEditPosAdjust1_6;
    m_pPosAdjustEdit[6]= ui->lineEditPosAdjust1_7;
    m_pPosAdjustEdit[7]= ui->lineEditPosAdjust1_8;
    m_pPosAdjustEdit[8]= ui->lineEditPosAdjust1_9;
    m_pPosAdjustEdit[9]= ui->lineEditPosAdjust1_10;
    m_pPosAdjustEdit[10]= ui->lineEditPosAdjust1_11;
    m_pPosAdjustEdit[11]= ui->lineEditPosAdjust1_12;
    m_pPosAdjustEdit[12]= ui->lineEditPosAdjust1_13;
    m_pPosAdjustEdit[13]= ui->lineEditPosAdjust1_14;
    m_pPosAdjustEdit[14]= ui->lineEditPosAdjust1_15;
    m_pPosAdjustEdit[15]= ui->lineEditPosAdjust1_16;
    m_pPosAdjustEdit[16]= ui->lineEditPosAdjust1_17;
    m_pPosAdjustEdit[17]= ui->lineEditPosAdjust1_18;
    m_pPosAdjustEdit[18]= ui->lineEditPosAdjust1_19;
    m_pPosAdjustEdit[19]= ui->lineEditPosAdjust1_20;
    m_pPosAdjustEdit[20]= ui->lineEditPosAdjust1_21;
    m_pPosAdjustEdit[21]= ui->lineEditPosAdjust1_22;
    m_pPosAdjustEdit[22]= ui->lineEditPosAdjust1_23;
    m_pPosAdjustEdit[23]= ui->lineEditPosAdjust1_24;
    m_pPosAdjustEdit[24]= ui->lineEditPosAdjust1_25;
    m_pPosAdjustEdit[25]= ui->lineEditPosAdjust1_26;
    m_pPosAdjustEdit[26]= ui->lineEditPosAdjust1_27;
    m_pPosAdjustEdit[27]= ui->lineEditPosAdjust1_28;
    m_pPosAdjustEdit[28]= ui->lineEditPosAdjust1_29;
    m_pPosAdjustEdit[29]= ui->lineEditPosAdjust1_30;
    m_pPosAdjustEdit[30]= ui->lineEditPosAdjust1_31;
    m_pPosAdjustEdit[31]= ui->lineEditPosAdjust1_32;
    m_pPosAdjustEdit[32]= ui->lineEditPosAdjust1_33;
    m_pPosAdjustEdit[33]= ui->lineEditPosAdjust1_34;
    m_pPosAdjustEdit[34]= ui->lineEditPosAdjust1_35;
    m_pPosAdjustEdit[35]= ui->lineEditPosAdjust1_36;
    m_pPosAdjustEdit[36]= ui->lineEditPosAdjust1_37;
    m_pPosAdjustEdit[37]= ui->lineEditPosAdjust1_38;
    m_pPosAdjustEdit[38]= ui->lineEditPosAdjust1_39;
    m_pPosAdjustEdit[39]= ui->lineEditPosAdjust1_40;
    m_pPosAdjustEdit[40]= ui->lineEditPosAdjust1_41;
    m_pPosAdjustEdit[41]= ui->lineEditPosAdjust1_42;
    m_pPosAdjustEdit[42]= ui->lineEditPosAdjust1_43;
    m_pPosAdjustEdit[43]= ui->lineEditPosAdjust1_44;
    m_pPosAdjustEdit[44]= ui->lineEditPosAdjust1_45;
    m_pPosAdjustEdit[45]= ui->lineEditPosAdjust1_46;
    m_pPosAdjustEdit[46]= ui->lineEditPosAdjust1_47;
    m_pPosAdjustEdit[47]= ui->lineEditPosAdjust1_48;
    m_pPosAdjustEdit[48]= ui->lineEditPosAdjust1_49;
    m_pPosAdjustEdit[49]= ui->lineEditPosAdjust1_50;
    m_pPosAdjustEdit[50]= ui->lineEditPosAdjust1_51;
    m_pPosAdjustEdit[51]= ui->lineEditPosAdjust1_52;

    m_pBDLabel[0]= ui->labelBD;
    m_pBDLabel[1]= ui->labelBD_2;
    m_pBDLabel[2]= ui->labelBD_3;
    m_pBDLabel[3]= ui->labelBD_4;
    m_pBDLabel[4]= ui->labelBD_5;
    m_pBDLabel[5]= ui->labelBD_6;
    m_pBDLabel[6]= ui->labelBD_7;
    m_pBDLabel[7]= ui->labelBD_8;
    m_pBDLabel[8]= ui->labelBD_9;
    m_pBDLabel[9]= ui->labelBD_10;
    m_pBDLabel[10]= ui->labelBD_11;
    m_pBDLabel[11]= ui->labelBD_12;
    m_pBDLabel[12]= ui->labelBD_13;
    m_pBDLabel[13]= ui->labelBD_14;
    m_pBDLabel[14]= ui->labelBD_15;
    m_pBDLabel[15]= ui->labelBD_16;
    m_pBDLabel[16]= ui->labelBD_17;
    m_pBDLabel[17]= ui->labelBD_18;
    m_pBDLabel[18]= ui->labelBD_19;
    m_pBDLabel[19]= ui->labelBD_20;
    m_pBDLabel[20]= ui->labelBD_21;
    m_pBDLabel[21]= ui->labelBD_22;
    m_pBDLabel[22]= ui->labelBD_23;
    m_pBDLabel[23]= ui->labelBD_24;
    m_pBDLabel[24]= ui->labelBD_25;
    m_pBDLabel[25]= ui->labelBD_26;
    m_pBDLabel[26]= ui->labelBD_27;
    m_pBDLabel[27]= ui->labelBD_28;
    m_pBDLabel[28]= ui->labelBD_29;
    m_pBDLabel[29]= ui->labelBD_30;
    m_pBDLabel[30]= ui->labelBD_31;
    m_pBDLabel[31]= ui->labelBD_32;
    m_pBDLabel[32]= ui->labelBD_33;
    m_pBDLabel[33]= ui->labelBD_34;
    m_pBDLabel[34]= ui->labelBD_35;
    m_pBDLabel[35]= ui->labelBD_36;
    m_pBDLabel[36]= ui->labelBD_37;
    m_pBDLabel[37]= ui->labelBD_38;
    m_pBDLabel[38]= ui->labelBD_39;
    m_pBDLabel[39]= ui->labelBD_40;
    m_pBDLabel[40]= ui->labelBD_41;
    m_pBDLabel[41]= ui->labelBD_42;
    m_pBDLabel[42]= ui->labelBD_43;
    m_pBDLabel[43]= ui->labelBD_44;
    m_pBDLabel[44]= ui->labelBD_45;
    m_pBDLabel[45]= ui->labelBD_46;
    m_pBDLabel[46]= ui->labelBD_47;
    m_pBDLabel[47]= ui->labelBD_48;
    m_pBDLabel[48]= ui->labelBD_49;
    m_pBDLabel[49]= ui->labelBD_50;
    m_pBDLabel[50]= ui->labelBD_51;
    m_pBDLabel[51]= ui->labelBD_52;

    m_pBDEdit[0]= ui->lineEditBD;
    m_pBDEdit[1]= ui->lineEditBD_2;
    m_pBDEdit[2]= ui->lineEditBD_3;
    m_pBDEdit[3]= ui->lineEditBD_4;
    m_pBDEdit[4]= ui->lineEditBD_5;
    m_pBDEdit[5]= ui->lineEditBD_6;
    m_pBDEdit[6]= ui->lineEditBD_7;
    m_pBDEdit[7]= ui->lineEditBD_8;
    m_pBDEdit[8]= ui->lineEditBD_9;
    m_pBDEdit[9]= ui->lineEditBD_10;
    m_pBDEdit[10]= ui->lineEditBD_11;
    m_pBDEdit[11]= ui->lineEditBD_12;
    m_pBDEdit[12]= ui->lineEditBD_13;
    m_pBDEdit[13]= ui->lineEditBD_14;
    m_pBDEdit[14]= ui->lineEditBD_15;
    m_pBDEdit[15]= ui->lineEditBD_16;
    m_pBDEdit[16]= ui->lineEditBD_17;
    m_pBDEdit[17]= ui->lineEditBD_18;
    m_pBDEdit[18]= ui->lineEditBD_19;
    m_pBDEdit[19]= ui->lineEditBD_20;
    m_pBDEdit[20]= ui->lineEditBD_21;
    m_pBDEdit[21]= ui->lineEditBD_22;
    m_pBDEdit[22]= ui->lineEditBD_23;
    m_pBDEdit[23]= ui->lineEditBD_24;
    m_pBDEdit[24]= ui->lineEditBD_25;
    m_pBDEdit[25]= ui->lineEditBD_26;
    m_pBDEdit[26]= ui->lineEditBD_27;
    m_pBDEdit[27]= ui->lineEditBD_28;
    m_pBDEdit[28]= ui->lineEditBD_29;
    m_pBDEdit[29]= ui->lineEditBD_30;
    m_pBDEdit[30]= ui->lineEditBD_31;
    m_pBDEdit[31]= ui->lineEditBD_32;
    m_pBDEdit[32]= ui->lineEditBD_33;
    m_pBDEdit[33]= ui->lineEditBD_34;
    m_pBDEdit[34]= ui->lineEditBD_35;
    m_pBDEdit[35]= ui->lineEditBD_36;
    m_pBDEdit[36]= ui->lineEditBD_37;
    m_pBDEdit[37]= ui->lineEditBD_38;
    m_pBDEdit[38]= ui->lineEditBD_39;
    m_pBDEdit[39]= ui->lineEditBD_40;
    m_pBDEdit[40]= ui->lineEditBD_41;
    m_pBDEdit[41]= ui->lineEditBD_42;
    m_pBDEdit[42]= ui->lineEditBD_43;
    m_pBDEdit[43]= ui->lineEditBD_44;
    m_pBDEdit[44]= ui->lineEditBD_45;
    m_pBDEdit[45]= ui->lineEditBD_46;
    m_pBDEdit[46]= ui->lineEditBD_47;
    m_pBDEdit[47]= ui->lineEditBD_48;
    m_pBDEdit[48]= ui->lineEditBD_49;
    m_pBDEdit[49]= ui->lineEditBD_50;
    m_pBDEdit[50]= ui->lineEditBD_51;
    m_pBDEdit[51]= ui->lineEditBD_52;

    m_pBXLabel[0]= ui->labelBX;
    m_pBXLabel[1]= ui->labelBX_2;
    m_pBXLabel[2]= ui->labelBX_3;
    m_pBXLabel[3]= ui->labelBX_4;
    m_pBXLabel[4]= ui->labelBX_5;
    m_pBXLabel[5]= ui->labelBX_6;
    m_pBXLabel[6]= ui->labelBX_7;
    m_pBXLabel[7]= ui->labelBX_8;
    m_pBXLabel[8]= ui->labelBX_9;
    m_pBXLabel[9]= ui->labelBX_10;
    m_pBXLabel[10]= ui->labelBX_11;
    m_pBXLabel[11]= ui->labelBX_12;
    m_pBXLabel[12]= ui->labelBX_13;
    m_pBXLabel[13]= ui->labelBX_14;
    m_pBXLabel[14]= ui->labelBX_15;
    m_pBXLabel[15]= ui->labelBX_16;
    m_pBXLabel[16]= ui->labelBX_17;
    m_pBXLabel[17]= ui->labelBX_18;
    m_pBXLabel[18]= ui->labelBX_19;
    m_pBXLabel[19]= ui->labelBX_20;
    m_pBXLabel[20]= ui->labelBX_21;
    m_pBXLabel[21]= ui->labelBX_22;
    m_pBXLabel[22]= ui->labelBX_23;
    m_pBXLabel[23]= ui->labelBX_24;
    m_pBXLabel[24]= ui->labelBX_25;
    m_pBXLabel[25]= ui->labelBX_26;
    m_pBXLabel[26]= ui->labelBX_27;
    m_pBXLabel[27]= ui->labelBX_28;
    m_pBXLabel[28]= ui->labelBX_29;
    m_pBXLabel[29]= ui->labelBX_30;
    m_pBXLabel[30]= ui->labelBX_31;
    m_pBXLabel[31]= ui->labelBX_32;
    m_pBXLabel[32]= ui->labelBX_33;
    m_pBXLabel[33]= ui->labelBX_34;
    m_pBXLabel[34]= ui->labelBX_35;
    m_pBXLabel[35]= ui->labelBX_36;
    m_pBXLabel[36]= ui->labelBX_37;
    m_pBXLabel[37]= ui->labelBX_38;
    m_pBXLabel[38]= ui->labelBX_39;
    m_pBXLabel[39]= ui->labelBX_40;
    m_pBXLabel[40]= ui->labelBX_41;
    m_pBXLabel[41]= ui->labelBX_42;
    m_pBXLabel[42]= ui->labelBX_43;
    m_pBXLabel[43]= ui->labelBX_44;
    m_pBXLabel[44]= ui->labelBX_45;
    m_pBXLabel[45]= ui->labelBX_46;
    m_pBXLabel[46]= ui->labelBX_47;
    m_pBXLabel[47]= ui->labelBX_48;
    m_pBXLabel[48]= ui->labelBX_49;
    m_pBXLabel[49]= ui->labelBX_50;
    m_pBXLabel[50]= ui->labelBX_51;
    m_pBXLabel[51]= ui->labelBX_52;

    m_pBXEdit[0]= ui->lineEditBX;
    m_pBXEdit[1]= ui->lineEditBX_2;
    m_pBXEdit[2]= ui->lineEditBX_3;
    m_pBXEdit[3]= ui->lineEditBX_4;
    m_pBXEdit[4]= ui->lineEditBX_5;
    m_pBXEdit[5]= ui->lineEditBX_6;
    m_pBXEdit[6]= ui->lineEditBX_7;
    m_pBXEdit[7]= ui->lineEditBX_8;
    m_pBXEdit[8]= ui->lineEditBX_9;
    m_pBXEdit[9]= ui->lineEditBX_10;
    m_pBXEdit[10]= ui->lineEditBX_11;
    m_pBXEdit[11]= ui->lineEditBX_12;
    m_pBXEdit[12]= ui->lineEditBX_13;
    m_pBXEdit[13]= ui->lineEditBX_14;
    m_pBXEdit[14]= ui->lineEditBX_15;
    m_pBXEdit[15]= ui->lineEditBX_16;
    m_pBXEdit[16]= ui->lineEditBX_17;
    m_pBXEdit[17]= ui->lineEditBX_18;
    m_pBXEdit[18]= ui->lineEditBX_19;
    m_pBXEdit[19]= ui->lineEditBX_20;
    m_pBXEdit[20]= ui->lineEditBX_21;
    m_pBXEdit[21]= ui->lineEditBX_22;
    m_pBXEdit[22]= ui->lineEditBX_23;
    m_pBXEdit[23]= ui->lineEditBX_24;
    m_pBXEdit[24]= ui->lineEditBX_25;
    m_pBXEdit[25]= ui->lineEditBX_26;
    m_pBXEdit[26]= ui->lineEditBX_27;
    m_pBXEdit[27]= ui->lineEditBX_28;
    m_pBXEdit[28]= ui->lineEditBX_29;
    m_pBXEdit[29]= ui->lineEditBX_30;
    m_pBXEdit[30]= ui->lineEditBX_31;
    m_pBXEdit[31]= ui->lineEditBX_32;
    m_pBXEdit[32]= ui->lineEditBX_33;
    m_pBXEdit[33]= ui->lineEditBX_34;
    m_pBXEdit[34]= ui->lineEditBX_35;
    m_pBXEdit[35]= ui->lineEditBX_36;
    m_pBXEdit[36]= ui->lineEditBX_37;
    m_pBXEdit[37]= ui->lineEditBX_38;
    m_pBXEdit[38]= ui->lineEditBX_39;
    m_pBXEdit[39]= ui->lineEditBX_40;
    m_pBXEdit[40]= ui->lineEditBX_41;
    m_pBXEdit[41]= ui->lineEditBX_42;
    m_pBXEdit[42]= ui->lineEditBX_43;
    m_pBXEdit[43]= ui->lineEditBX_44;
    m_pBXEdit[44]= ui->lineEditBX_45;
    m_pBXEdit[45]= ui->lineEditBX_46;
    m_pBXEdit[46]= ui->lineEditBX_47;
    m_pBXEdit[47]= ui->lineEditBX_48;
    m_pBXEdit[48]= ui->lineEditBX_49;
    m_pBXEdit[49]= ui->lineEditBX_50;
    m_pBXEdit[50]= ui->lineEditBX_51;
    m_pBXEdit[51]= ui->lineEditBX_52;

    m_pSelReverse[0]= ui->SelReverseCheckBox1;
    m_pSelReverse[1]= ui->SelReverseCheckBox2;
    m_pSelReverse[2]= ui->SelReverseCheckBox3;
    m_pSelReverse[3]= ui->SelReverseCheckBox4;
    m_pSelReverse[4]= ui->SelReverseCheckBox5;
    m_pSelReverse[5]= ui->SelReverseCheckBox6;
    m_pSelReverse[6]= ui->SelReverseCheckBox7;
    m_pSelReverse[7]= ui->SelReverseCheckBox8;
    m_pSelReverse[8]= ui->SelReverseCheckBox9;
    m_pSelReverse[9]= ui->SelReverseCheckBox10;
    m_pSelReverse[10]= ui->SelReverseCheckBox11;
    m_pSelReverse[11]= ui->SelReverseCheckBox12;
    m_pSelReverse[12]= ui->SelReverseCheckBox13;
    m_pSelReverse[13]= ui->SelReverseCheckBox14;
    m_pSelReverse[14]= ui->SelReverseCheckBox15;
    m_pSelReverse[15]= ui->SelReverseCheckBox16;
    m_pSelReverse[16]= ui->SelReverseCheckBox17;
    m_pSelReverse[17]= ui->SelReverseCheckBox18;
    m_pSelReverse[18]= ui->SelReverseCheckBox19;
    m_pSelReverse[19]= ui->SelReverseCheckBox20;
    m_pSelReverse[20]= ui->SelReverseCheckBox21;
    m_pSelReverse[21]= ui->SelReverseCheckBox22;
    m_pSelReverse[22]= ui->SelReverseCheckBox23;
    m_pSelReverse[23]= ui->SelReverseCheckBox24;
    m_pSelReverse[24]= ui->SelReverseCheckBox25;
    m_pSelReverse[25]= ui->SelReverseCheckBox26;
    m_pSelReverse[26]= ui->SelReverseCheckBox27;
    m_pSelReverse[27]= ui->SelReverseCheckBox28;
    m_pSelReverse[28]= ui->SelReverseCheckBox29;
    m_pSelReverse[29]= ui->SelReverseCheckBox30;
    m_pSelReverse[30]= ui->SelReverseCheckBox31;
    m_pSelReverse[31]= ui->SelReverseCheckBox32;
    m_pSelReverse[32]= ui->SelReverseCheckBox33;
    m_pSelReverse[33]= ui->SelReverseCheckBox34;
    m_pSelReverse[34]= ui->SelReverseCheckBox35;
    m_pSelReverse[35]= ui->SelReverseCheckBox36;
    m_pSelReverse[36]= ui->SelReverseCheckBox37;
    m_pSelReverse[37]= ui->SelReverseCheckBox38;
    m_pSelReverse[38]= ui->SelReverseCheckBox39;
    m_pSelReverse[39]= ui->SelReverseCheckBox40;
    m_pSelReverse[40]= ui->SelReverseCheckBox41;
    m_pSelReverse[41]= ui->SelReverseCheckBox42;
    m_pSelReverse[42]= ui->SelReverseCheckBox43;
    m_pSelReverse[43]= ui->SelReverseCheckBox44;
    m_pSelReverse[44]= ui->SelReverseCheckBox45;
    m_pSelReverse[45]= ui->SelReverseCheckBox46;
    m_pSelReverse[46]= ui->SelReverseCheckBox47;
    m_pSelReverse[47]= ui->SelReverseCheckBox48;
    m_pSelReverse[48]= ui->SelReverseCheckBox49;
    m_pSelReverse[49]= ui->SelReverseCheckBox50;
    m_pSelReverse[50]= ui->SelReverseCheckBox51;
    m_pSelReverse[51]= ui->SelReverseCheckBox52;

    m_pSelLimit1[0] = ui->limit1Edit1;
    m_pSelLimit1[1] = ui->limit1Edit2;
    m_pSelLimit1[2] = ui->limit1Edit3;
    m_pSelLimit1[3] = ui->limit1Edit4;
    m_pSelLimit1[4] = ui->limit1Edit5;
    m_pSelLimit1[5] = ui->limit1Edit6;
    m_pSelLimit1[6] = ui->limit1Edit7;
    m_pSelLimit1[7] = ui->limit1Edit8;
    m_pSelLimit1[8] = ui->limit1Edit9;
    m_pSelLimit1[9] = ui->limit1Edit10;
    m_pSelLimit1[10] = ui->limit1Edit11;
    m_pSelLimit1[11] = ui->limit1Edit12;
    m_pSelLimit1[12] = ui->limit1Edit13;
    m_pSelLimit1[13] = ui->limit1Edit14;
    m_pSelLimit1[14] = ui->limit1Edit15;
    m_pSelLimit1[15] = ui->limit1Edit16;
    m_pSelLimit1[16] = ui->limit1Edit17;
    m_pSelLimit1[17] = ui->limit1Edit18;
    m_pSelLimit1[18] = ui->limit1Edit19;
    m_pSelLimit1[19] = ui->limit1Edit20;
    m_pSelLimit1[20] = ui->limit1Edit21;
    m_pSelLimit1[21] = ui->limit1Edit22;
    m_pSelLimit1[22] = ui->limit1Edit23;
    m_pSelLimit1[23] = ui->limit1Edit24;
    m_pSelLimit1[24] = ui->limit1Edit25;
    m_pSelLimit1[25] = ui->limit1Edit26;
    m_pSelLimit1[26] = ui->limit1Edit27;
    m_pSelLimit1[27] = ui->limit1Edit28;
    m_pSelLimit1[28] = ui->limit1Edit29;
    m_pSelLimit1[29] = ui->limit1Edit30;
    m_pSelLimit1[30] = ui->limit1Edit31;
    m_pSelLimit1[31] = ui->limit1Edit32;
    m_pSelLimit1[32] = ui->limit1Edit33;
    m_pSelLimit1[33] = ui->limit1Edit34;
    m_pSelLimit1[34] = ui->limit1Edit35;
    m_pSelLimit1[35] = ui->limit1Edit36;
    m_pSelLimit1[36] = ui->limit1Edit37;
    m_pSelLimit1[37] = ui->limit1Edit38;
    m_pSelLimit1[38] = ui->limit1Edit39;
    m_pSelLimit1[39] = ui->limit1Edit40;
    m_pSelLimit1[40] = ui->limit1Edit41;
    m_pSelLimit1[41] = ui->limit1Edit42;
    m_pSelLimit1[42] = ui->limit1Edit43;
    m_pSelLimit1[43] = ui->limit1Edit44;
    m_pSelLimit1[44] = ui->limit1Edit45;
    m_pSelLimit1[45] = ui->limit1Edit46;
    m_pSelLimit1[46] = ui->limit1Edit47;
    m_pSelLimit1[47] = ui->limit1Edit48;
    m_pSelLimit1[48] = ui->limit1Edit49;
    m_pSelLimit1[49] = ui->limit1Edit50;
    m_pSelLimit1[50] = ui->limit1Edit51;
    m_pSelLimit1[51] = ui->limit1Edit52;

    m_pSelLimit2[0] = ui->limit2Edit1;
    m_pSelLimit2[1] = ui->limit2Edit2;
    m_pSelLimit2[2] = ui->limit2Edit3;
    m_pSelLimit2[3] = ui->limit2Edit4;
    m_pSelLimit2[4] = ui->limit2Edit5;
    m_pSelLimit2[5] = ui->limit2Edit6;
    m_pSelLimit2[6] = ui->limit2Edit7;
    m_pSelLimit2[7] = ui->limit2Edit8;
    m_pSelLimit2[8] = ui->limit2Edit9;
    m_pSelLimit2[9] = ui->limit2Edit10;
    m_pSelLimit2[10] = ui->limit2Edit11;
    m_pSelLimit2[11] = ui->limit2Edit12;
    m_pSelLimit2[12] = ui->limit2Edit13;
    m_pSelLimit2[13] = ui->limit2Edit14;
    m_pSelLimit2[14] = ui->limit2Edit15;
    m_pSelLimit2[15] = ui->limit2Edit16;
    m_pSelLimit2[16] = ui->limit2Edit17;
    m_pSelLimit2[17] = ui->limit2Edit18;
    m_pSelLimit2[18] = ui->limit2Edit19;
    m_pSelLimit2[19] = ui->limit2Edit20;
    m_pSelLimit2[20] = ui->limit2Edit21;
    m_pSelLimit2[21] = ui->limit2Edit22;
    m_pSelLimit2[22] = ui->limit2Edit23;
    m_pSelLimit2[23] = ui->limit2Edit24;
    m_pSelLimit2[24] = ui->limit2Edit25;
    m_pSelLimit2[25] = ui->limit2Edit26;
    m_pSelLimit2[26] = ui->limit2Edit27;
    m_pSelLimit2[27] = ui->limit2Edit28;
    m_pSelLimit2[28] = ui->limit2Edit29;
    m_pSelLimit2[29] = ui->limit2Edit30;
    m_pSelLimit2[30] = ui->limit2Edit31;
    m_pSelLimit2[31] = ui->limit2Edit32;
    m_pSelLimit2[32] = ui->limit2Edit33;
    m_pSelLimit2[33] = ui->limit2Edit34;
    m_pSelLimit2[34] = ui->limit2Edit35;
    m_pSelLimit2[35] = ui->limit2Edit36;
    m_pSelLimit2[36] = ui->limit2Edit37;
    m_pSelLimit2[37] = ui->limit2Edit38;
    m_pSelLimit2[38] = ui->limit2Edit39;
    m_pSelLimit2[39] = ui->limit2Edit40;
    m_pSelLimit2[40] = ui->limit2Edit41;
    m_pSelLimit2[41] = ui->limit2Edit42;
    m_pSelLimit2[42] = ui->limit2Edit43;
    m_pSelLimit2[43] = ui->limit2Edit44;
    m_pSelLimit2[44] = ui->limit2Edit45;
    m_pSelLimit2[45] = ui->limit2Edit46;
    m_pSelLimit2[46] = ui->limit2Edit47;
    m_pSelLimit2[47] = ui->limit2Edit48;
    m_pSelLimit2[48] = ui->limit2Edit49;
    m_pSelLimit2[49] = ui->limit2Edit50;
    m_pSelLimit2[50] = ui->limit2Edit51;
    m_pSelLimit2[51] = ui->limit2Edit52;

    m_pClassANDEnable[0]= ui->checkBox_53;
    m_pClassANDEnable[1]= ui->checkBox_54;
    m_pClassANDEnable[2]= ui->checkBox_55;
    m_pClassANDEnable[3]= ui->checkBox_56;
    m_pClassANDEnable[4]= ui->checkBox_57;
    m_pClassANDEnable[5]= ui->checkBox_58;
    m_pClassANDEnable[6]= ui->checkBox_59;
    m_pClassANDEnable[7]= ui->checkBox_60;
    m_pClassANDEnable[8]= ui->checkBox_61;
    m_pClassANDEnable[9]= ui->checkBox_62;
    m_pClassANDEnable[10]= ui->checkBox_63;
    m_pClassANDEnable[11]= ui->checkBox_64;
    m_pClassANDEnable[12]= ui->checkBox_65;
    m_pClassANDEnable[13]= ui->checkBox_66;
    m_pClassANDEnable[14]= ui->checkBox_67;
    m_pClassANDEnable[15]= ui->checkBox_68;
    m_pClassANDEnable[16]= ui->checkBox_69;
    m_pClassANDEnable[17]= ui->checkBox_70;
    m_pClassANDEnable[18]= ui->checkBox_71;
    m_pClassANDEnable[19]= ui->checkBox_72;
    m_pClassANDEnable[20]= ui->checkBox_73;
    m_pClassANDEnable[21]= ui->checkBox_74;
    m_pClassANDEnable[22]= ui->checkBox_75;
    m_pClassANDEnable[23]= ui->checkBox_76;
    m_pClassANDEnable[24]= ui->checkBox_77;
    m_pClassANDEnable[25]= ui->checkBox_78;
    m_pClassANDEnable[26]= ui->checkBox_79;
    m_pClassANDEnable[27]= ui->checkBox_80;
    m_pClassANDEnable[28]= ui->checkBox_81;
    m_pClassANDEnable[29]= ui->checkBox_82;
    m_pClassANDEnable[30]= ui->checkBox_83;
    m_pClassANDEnable[31]= ui->checkBox_84;
    m_pClassANDEnable[32]= ui->checkBox_85;
    m_pClassANDEnable[33]= ui->checkBox_86;
    m_pClassANDEnable[34]= ui->checkBox_87;
    m_pClassANDEnable[35]= ui->checkBox_88;
    m_pClassANDEnable[36]= ui->checkBox_89;
    m_pClassANDEnable[37]= ui->checkBox_90;
    m_pClassANDEnable[38]= ui->checkBox_91;
    m_pClassANDEnable[39]= ui->checkBox_92;
    m_pClassANDEnable[40]= ui->checkBox_93;
    m_pClassANDEnable[41]= ui->checkBox_94;
    m_pClassANDEnable[42]= ui->checkBox_95;
    m_pClassANDEnable[43]= ui->checkBox_96;
    m_pClassANDEnable[44]= ui->checkBox_97;
    m_pClassANDEnable[45]= ui->checkBox_98;
    m_pClassANDEnable[46]= ui->checkBox_99;
    m_pClassANDEnable[47]= ui->checkBox_100;
    m_pClassANDEnable[48]= ui->checkBox_101;
    m_pClassANDEnable[49]= ui->checkBox_102;
    m_pClassANDEnable[50]= ui->checkBox_103;
    m_pClassANDEnable[51]= ui->checkBox_104;
    m_curLayer = 0;

    m_curIpcIndex = 0;
    m_totalPage = 1;
    m_curPage = 0;

    getCurrentViewIpcIndex();

    /* 提速显示用 */
    for(int i = CONST_MAX_MODEL_PER_PAGE; i < CONST_MAX_MODEL_PER_PAGE_SHOW; i++){
        m_pClassSensEdit[i]->setVisible(false);
        m_pClassEnable[i]->setVisible(false);
        m_pClassThresholdEdit[i]->setVisible(false);
        m_nameClassEdit[i]->setVisible(false);
        m_pBoxClassLable[i]->setVisible(false);
        m_pBoxClassLable2[i]->setVisible(false);
        m_pBoxClassEdit[i]->setVisible(false);
        m_pBoxClassEdit2[i]->setVisible(false);
        m_pTickModeClass[i]->setVisible(false);
        m_pDelayModeClass[i]->setVisible(false);
        m_pFixedClass[i]->setVisible(false);
        m_pModeLabel[i]->setVisible(false);
        m_pEjctorArithCombox[i]->setVisible(false);
        m_pHWRatioLabel[i]->setVisible(false);
        m_pHWRatioEdit[i]->setVisible(false);
        m_pPosAdjustLabel[i]->setVisible(false);
        m_pPosAdjustEdit[i]->setVisible(false);
        m_pBDLabel[i]->setVisible(false);
        m_pBDEdit[i]->setVisible(false);
        m_pBXLabel[i]->setVisible(false);
        m_pBXEdit[i]->setVisible(false);
        m_pSelReverse[i]->setVisible(false);
        m_pSelLimit1[i]->setVisible(false);
        m_pSelLimit2[i]->setVisible(false);
        m_pClassANDEnable[i]->setVisible(false);
    }
    m_classEnableSigMapper = new QSignalMapper(this);
    m_pSelReverseEnableSigMapper = new QSignalMapper(this);
    m_classANDEnableSigMapper = new QSignalMapper(this);
    for(int i = 0; i < CONST_MAX_MODEL_PER_PAGE; i++){
        m_classEnableSigMapper->setMapping(m_pClassEnable[i], i);
        m_pClassEnable[i]->setObjectName("ipcClassEnale");
        connect(m_pClassEnable[i],SIGNAL(clicked()),m_classEnableSigMapper,SLOT(map()));

        m_pSelReverseEnableSigMapper->setMapping(m_pSelReverse[i],i);
        m_pSelReverse[i]->setObjectName("xSelReverse");
        connect(m_pSelReverse[i],SIGNAL(clicked()),m_pSelReverseEnableSigMapper,SLOT(map()));
        m_classANDEnableSigMapper->setMapping(m_pClassANDEnable[i], i);
        m_pClassANDEnable[i]->setObjectName("ipcClassEnale");
        connect(m_pClassANDEnable[i],SIGNAL(clicked()),m_classANDEnableSigMapper,SLOT(map()));
    }
    connect(m_classEnableSigMapper,SIGNAL(mapped(int)),this,SLOT(sClassEnableSlots(int)));
    connect(m_pSelReverseEnableSigMapper,SIGNAL(mapped(int)),this,SLOT(sSelReverseSlots(int)));
    connect(m_classANDEnableSigMapper,SIGNAL(mapped(int)),this,SLOT(sClassANDEnableSlots(int)));

    m_classNameEditSigMapper = new QSignalMapper(this);
    for(int i = 0; i < CONST_MAX_MODEL_PER_PAGE; i++){
        m_nameClassEdit[i]->setType(textType);
        m_classNameEditSigMapper->setMapping(m_nameClassEdit[i], i);
        m_nameClassEdit[i]->setObjectName("ipcClassName");
        connect(m_nameClassEdit[i],SIGNAL(valueSetSignal()),m_classNameEditSigMapper,SLOT(map()));
    }
    connect(m_classNameEditSigMapper,SIGNAL(mapped(int)),this,SLOT(sClass1NameChanged(int)));


    m_classTickEnableSigMapper = new QSignalMapper(this);
    for(int i = 0; i < CONST_MAX_MODEL_PER_PAGE; i++){
        m_classTickEnableSigMapper->setMapping(m_pTickModeClass[i], i);
        m_pTickModeClass[i]->setObjectName("ipcClassTickModeEnable");
        connect(m_pTickModeClass[i],SIGNAL(clicked()),m_classTickEnableSigMapper,SLOT(map()));
    }
    connect(m_classTickEnableSigMapper,SIGNAL(mapped(int)),this,SLOT(sClassTickEnableChanged(int)));


    m_classDelayEnableSigMapper = new QSignalMapper(this);
    for(int i = 0; i < CONST_MAX_MODEL_PER_PAGE; i++){
        m_classDelayEnableSigMapper->setMapping(m_pDelayModeClass[i], i);
        m_pDelayModeClass[i]->setObjectName("ipcClassDelayModeEnable");
        connect(m_pDelayModeClass[i],SIGNAL(clicked()),m_classDelayEnableSigMapper,SLOT(map()));
    }
    connect(m_classDelayEnableSigMapper,SIGNAL(mapped(int)),this,SLOT(sClassDelayEnableChanged(int)));


    m_classFixedEnableSigMapper = new QSignalMapper(this);
    for(int i = 0; i < CONST_MAX_MODEL_PER_PAGE; i++){
        m_classFixedEnableSigMapper->setMapping(m_pFixedClass[i], i);
        m_pFixedClass[i]->setObjectName("ipcClassFixedEnable");
        connect(m_pFixedClass[i],SIGNAL(clicked()),m_classFixedEnableSigMapper,SLOT(map()));
    }
    connect(m_classFixedEnableSigMapper,SIGNAL(mapped(int)),this,SLOT(sClassFixedChanged(int)));

    QStringList modeType;
    modeType<<"0"<<"1"<<"2"<<"3"<<"4";
//    modeType<<g_myLan().All_eject<<g_myLan().ids_length<<g_myLan().ids_area<<
//             g_myLan().length_width<<g_myLan().thickness;
    m_classModeSigMapper = new QSignalMapper(this);
    for(int i = 0; i < CONST_MAX_MODEL_PER_PAGE; i++){
        m_pEjctorArithCombox[i]->addItems(modeType);
        m_pEjctorArithCombox[i]->setObjectName("ipcClassMode");
        m_classModeSigMapper->setMapping(m_pEjctorArithCombox[i], i);
        connect(m_pEjctorArithCombox[i],SIGNAL(activated(int)),m_classModeSigMapper,SLOT(map()));
    }
    connect(m_classModeSigMapper,SIGNAL(mapped(int)),this,SLOT(sClassModeChanged(int)));

    shottimer = new QTimer;
    connect(shottimer,SIGNAL(timeout()),this, SLOT(getscreenshot()));


    retranslateUiWidget();
}

ipcclassparams::~ipcclassparams()
{
    delete ui;
}

void ipcclassparams::getCurrentViewIpcIndex()
{
}

void ipcclassparams::sClassEnableSlots(int index)
{
    struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classEnable[index]
            = !struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classEnable[index];
}

void ipcclassparams::sSelReverseSlots(int index)
{
    struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].selRerverse[index]
            = !struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].selRerverse[index];
}
void ipcclassparams::sClassANDEnableSlots(int index)
{
    struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classANDEnable[index]
            = !struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classANDEnable[index];
    if(struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classANDEnable[index])
    {
        for(int i =0 ; i < CONST_MAX_MODEL_PER_PAGE_SHOW; i++)
        {
            if(i == index)
                continue;
            struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classANDEnable[i] = 0;
            m_pClassANDEnable[i]->setChecked(false);
        }
    }
}

void ipcclassparams::sClass1NameChanged(int index)
{
    memset(struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].className[index],0,
            sizeof(struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].className[index]));

    QString name = m_nameClassEdit[index]->text();
    QByteArray namearr = name.toUtf8();
    const char *strName = namearr.data();
    memcpy(struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].className[index],strName,strlen(strName));
}

void ipcclassparams::sClassLabelBoxChanged(int index)
{
    struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classEnable[index]
            = !struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classEnable[index];
}

void ipcclassparams::sClassTickEnableChanged(int index)
{
    struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classTickMode[index]
            = !struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classTickMode[index];
}

void ipcclassparams::sClassDelayEnableChanged(int index)
{
    struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classDelayMode[index]
            = !struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classDelayMode[index];
}

void ipcclassparams::sClassFixedChanged(int index)
{
    struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classFixMode[index]
            = !struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classFixMode[index];
}

void ipcclassparams::sClassModeChanged(int index)
{
    struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classMode[index]
            = m_pEjctorArithCombox[index]->currentIndex();
    updateView();
}

void ipcclassparams::showPage(bool bshow)
{
    basewidget::showPage(bshow);
    if(bshow){
        struGsh.isInIpcClassPage = 1;
        struGsh.nLayer = 0;
        struGsh.nView = 0;
        struGsh.nMatGroup = 0;

        ui->syncModeNameBtn->setVisible(struGsh.bRunMode > RUNMODE_USER);
        ui->labelRoiR->setVisible(struGsh.bRunMode > RUNMODE_USER && struCnfg.nXsystemBoard[struGsh.nView] == 0);
        ui->labelRoiR2->setVisible(struGsh.bRunMode > RUNMODE_USER && struCnfg.nXsystemBoard[struGsh.nView] == 0);
        ui->lineEditRoiR->setVisible(struGsh.bRunMode > RUNMODE_USER && struCnfg.nXsystemBoard[struGsh.nView] == 0);
        ui->lineEditRoiR2->setVisible(struGsh.bRunMode > RUNMODE_USER && struCnfg.nXsystemBoard[struGsh.nView] == 0);

        updateViewButtonVisible();
        updateViewBtnState();
        updateGroupWidgetState();

        getCurrentViewIpcIndex();

        for(int i = 0; i < MAX_VIEW; i++)
        {
            //struCnfp.ipcModelClassCount[i] = 16;//struIpcShare.struIpcModelClass[i][0].classCount;
        }
        updateView();

        basewidget::showPage(bshow);


        if( struGsh.bScreenShotStatus ==1)
        {
            shottimer->start(1000);

        }
    }
    else
    {
        struGsh.nLayer = 0;
        struGsh.nView = 0;
        struGsh.nMatGroup = 0;
        struGsh.isInIpcClassPage = 0;
    }
}

void ipcclassparams::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_DATA_CHANGED){
        if (msg2 <= MAX_IPC_MODEL_CLASS*2){
            if(msg2 < MAX_IPC_MODEL_CLASS)
            {
                struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classSens[msg2] = m_sensValue[msg2];
            }
            else
            {
                struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classThreshold[msg2-MAX_IPC_MODEL_CLASS] = m_ThresholdValue[msg2-MAX_IPC_MODEL_CLASS];
            }
        }
        else if(msg2 == 200)
        {
            if(0 > struCnfp.ipcModelClassCount[struGsh.nView][struGsh.nMatGroup])
            {
                for(int i = 0; i < MAX_VIEW; i++)
                {
                    for(int j = 0; j < MAX_GROUP_IDENTIFY ; j++)
                        struCnfp.ipcModelClassCount[i][j] = 0;
                }
            }
            if (MAX_IPC_MODEL_CLASS < struCnfp.ipcModelClassCount[struGsh.nView][struGsh.nMatGroup] )
            {
                for(int i = 0; i < MAX_VIEW; i++)
                {
                    for(int j = 0; j < MAX_GROUP_IDENTIFY ; j++)
                        struCnfp.ipcModelClassCount[i][j] = MAX_IPC_MODEL_CLASS;
                }
            }

            updateView();
        }
        else if(msg2 == 201)
        {
            if(struCnfp.nIpcRoiHeight[struGsh.nView] > 0){
                struCnfp.nIpcRoiRepeatf[struGsh.nView] =(float)struCnfp.nIpcRoiRepeat[struGsh.nView]/(float)struCnfp.nIpcRoiHeight[struGsh.nView];
                if(struCnfp.nDualLayerModelSame==1){
                    for(int i = 0; i< MAX_VIEW;i++){
                        struCnfp.nIpcRoiRepeatf[i] = struCnfp.nIpcRoiRepeatf[struGsh.nView];
                        struCnfp.nIpcRoiRepeat2[i] = struCnfp.nIpcRoiRepeat2[struGsh.nView];
                    }
                }
//                myFlow.configIpcAIPara(0,0);
            }
        }
    }
}

void ipcclassparams::OnViewChange(int view)
{
    updateView();
}

void ipcclassparams::OnChuteChange()
{
    updateView();
}

void ipcclassparams::OnGroupChange(int groupIndex)
{
    updateView();
}

void ipcclassparams::OnLayerChange()
{
    updateView();
}

void ipcclassparams::retranslateUiWidget()
{
    ui->m_applyBtn->setText(g_myLan().ids_apply);
    ui->labelLengthLimit->setText(g_myLan().ids_lowlimit);
    for(int i = 0;i < CONST_MAX_MODEL_PER_PAGE; i++){
        m_pBoxClassLable[i]->setText(g_myLan().ids_lowlimit);          //剔除框１
        m_pBoxClassLable2[i]->setText(g_myLan().ids_lowlimit+"2");     //剔除框２
        m_pTickModeClass[i]->setText(g_myLan().ipc_heavyBlow);         //重吹/轻吹
        m_pFixedClass[i]->setText(g_myLan().maximum);                  //最值/普通模式
        m_pDelayModeClass[i]->setText(g_myLan().keyPoint);             //关键点/中心点
        m_pModeLabel[i]->setText(g_myLan().ids_algorithm);             //剔除算法:0-3
        m_pHWRatioLabel[i]->setText("H/W");                            //高宽比阈值
        m_pPosAdjustLabel[i]->setText(g_myLan().pos);                  //坐标调整
        m_pSelReverse[i]->setText(g_myLan().reverse_sort);
        m_pClassANDEnable[i]->setText(g_myLan().ids_And);
    }
    ui->syncModeNameBtn->setText(g_myLan().ipc_syncName);
}

void ipcclassparams::getscreenshot()
{
    shottimer->stop();
    {

        QScrollBar *bar = ui->scrollArea->verticalScrollBar();

        for(int i=0;i<MAX_VIEW;i++){
            if(struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[i])
            {
                QMetaObject::invokeMethod(m_pViewBtn[i],"clicked", Qt::QueuedConnection);
                 updateView();

                 struGsh.nMatGroup = 0;
                 for(int j=0; j<struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[i].nIdentifyGroupTotal;j++)
                 {
                     groupBtnClick(j);
                     updateGroupWidgetState();

                     int  currentvalue = 0;

                     while(currentvalue <= bar->maximum())
                     {
                         bar->setValue(currentvalue);
                         myFlow.sleep(1);
                         myFlow.saveScreenshot();
                         currentvalue += 380;

                     }
                     bar->setValue(bar->maximum());
                     myFlow.sleep(1);
                     myFlow.saveScreenshot();
                 }
            }
        }
        myFlow.sleep(1);
        struGsh.bScreenShotStatus = 0;
    }
}

void ipcclassparams::updateView()
{
    ui->lineEditRoiR->setText(QString::number(struCnfp.nIpcRoiRepeat[struGsh.nView]));
    ui->lineEditRoiR->setMinMax(this,0,struCnfp.nIpcRoiHeight[struGsh.nView] /2,201,&struCnfp.nIpcRoiRepeat[struGsh.nView] );
    ui->lineEditRoiR2->setText(QString::number(struCnfp.nIpcRoiRepeat2[struGsh.nView] ));
    ui->lineEditRoiR2->setMinMax(this,0,100,201,&struCnfp.nIpcRoiRepeat2[struGsh.nView] );

    ui->classCountWidget->setParams(this,0, MAX_IPC_MODEL_CLASS,200,&struCnfp.ipcModelClassCount[struGsh.nView][struGsh.nMatGroup]);
    ui->classCountWidget->setValue(struCnfp.ipcModelClassCount[struGsh.nView][struGsh.nMatGroup]);
    ui->classCountWidget->setTitle("");
    ui->classCountWidget->setVisible(struGsh.debugMode);

    ui->lineEditLengthLimit->setText(QString::number(struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].lengthLimit));
    ui->lineEditLengthLimit->setMinMax(this, 0, 10000000, 201,&struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].lengthLimit);  

    for(int i = 0;i < CONST_MAX_MODEL_PER_PAGE;i++){

        if(i < (struCnfp.ipcModelClassCount[struGsh.nView][struGsh.nMatGroup])){
            m_pClassEnable[i]->setVisible(true);
            m_pClassEnable[i]->setChecked(struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classEnable[i]);
            m_sensValue[i] = struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classSens[i];
            m_pClassSensEdit[i]->setVisible(true);
            m_pClassSensEdit[i]->setParams(this,0,100,i,&m_sensValue[i]);
            m_pClassSensEdit[i]->setTitle(QString::number(i+1));
            m_pClassSensEdit[i]->setValue(m_sensValue[i]);
            m_pClassSensEdit[i]->setFixedWidth(250);

            m_ThresholdValue[i] = struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classThreshold[i];
            m_pClassThresholdEdit[i]->setVisible(struGsh.bRunMode > RUNMODE_ENGINEER);
            m_pClassThresholdEdit[i]->setParams(this,0,100, MAX_IPC_MODEL_CLASS+i,&m_ThresholdValue[i]);
            m_pClassThresholdEdit[i]->setTitle("");
            m_pClassThresholdEdit[i]->setValue(m_ThresholdValue[i]);

            m_nameClassEdit[i]->setVisible(true);
            m_nameClassEdit[i]->setFixedWidth(200);
            m_nameClassEdit[i]->setText(QString("%1").arg(struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].className[i]));

            //高级参数
            m_pBoxClassLable[i]->setVisible(struGsh.bRunMode > RUNMODE_USER);
            m_pBoxClassEdit[i]->setVisible(struGsh.bRunMode > RUNMODE_USER);
            m_pBoxClassEdit[i]->setText(QString::number(struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classLabelBox[i]));
            if (struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classMode[i] == 1)
                tmpValue = 24;
            else
                tmpValue = 65535;
            m_pBoxClassEdit[i]->setMinMax(this, 0, tmpValue, 202,&struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classLabelBox[i]);

            m_pBoxClassLable2[i]->setVisible(struGsh.bRunMode > RUNMODE_USER );
            m_pBoxClassEdit2[i]->setVisible(struGsh.bRunMode > RUNMODE_USER );
            m_pBoxClassEdit2[i]->setText(QString::number(struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classLabelBox2[i]));
            m_pBoxClassEdit2[i]->setMinMax(this, 0, tmpValue, 202,&struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classLabelBox2[i]);

            m_pTickModeClass[i]->setVisible(struGsh.bRunMode > RUNMODE_USER);
            m_pTickModeClass[i]->setChecked(struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classTickMode[i]);
            m_pFixedClass[i]->setVisible(struGsh.bRunMode > RUNMODE_USER);
            m_pFixedClass[i]->setChecked(struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classFixMode[i]);
            m_pDelayModeClass[i]->setVisible(struGsh.bRunMode > RUNMODE_USER );
            m_pDelayModeClass[i]->setChecked(struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classDelayMode[i]);

            m_pModeLabel[i]->setVisible(struGsh.bRunMode > RUNMODE_ENGINEER);
            m_pEjctorArithCombox[i]->setVisible(struGsh.bRunMode > RUNMODE_ENGINEER);
            m_pEjctorArithCombox[i]->setCurrentIndex(struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classMode[i]);

            m_pHWRatioLabel[i]->setVisible(struGsh.bRunMode > RUNMODE_USER );
            m_pHWRatioEdit[i]->setVisible(struGsh.bRunMode > RUNMODE_USER );
            m_pHWRatioEdit[i]->setText(QString::number(struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classHWRatio[i]));
            m_pHWRatioEdit[i]->setMinMax(this, 0, 100, 203,&struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classHWRatio[i]);

            m_pPosAdjustLabel[i]->setVisible(struGsh.bRunMode > RUNMODE_USER );
            m_pPosAdjustEdit[i]->setVisible(struGsh.bRunMode > RUNMODE_USER );
            m_pPosAdjustEdit[i]->setText(QString::number(struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classPosAdjust[i]));
            m_pPosAdjustEdit[i]->setMinMax(this, -128, 127, 204,&struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classPosAdjust[i]);

            m_pBDLabel[i]->setVisible(struGsh.bRunMode > RUNMODE_USER );
            m_pBDEdit[i]->setVisible(struGsh.bRunMode > RUNMODE_USER );
            m_pBDEdit[i]->setText(QString::number(struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classBD[i]));
            if(struCnfg.nXsystemBoard[struGsh.nView])
                tmpValue = 100;
            else
                tmpValue = 10;
            m_pBDEdit[i]->setMinMax(this, 0, tmpValue, 204,&struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classBD[i]);

            m_pBXLabel[i]->setVisible(struGsh.bRunMode > RUNMODE_USER );
            m_pBXEdit[i]->setVisible(struGsh.bRunMode > RUNMODE_USER );
            m_pBXEdit[i]->setText(QString::number(struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classBX[i]));
            m_pBXEdit[i]->setMinMax(this, 0, 2048, 204,&struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classBX[i]);

            m_pSelReverse[i]->setVisible(struGsh.bRunMode > RUNMODE_USER && struCnfg.nXsystemBoard[struGsh.nView] == 1);
            m_pSelReverse[i]->setChecked(struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].selRerverse[i]);
            switch(struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classMode[i]){
            case 0:
                tmpValue  = 0;
                tmpValue2 = 40.0;
                break;
            case 1:
                tmpValue  = 0;
                tmpValue2 = 40.0;
                break;
            case 2:
                tmpValue  = 0;
                tmpValue2 = 100.0;
                break;
            case 3:
                tmpValue  = 1;
                tmpValue2 = 10.0;
                break;
            case 4:
                tmpValue  = 0.1;
                tmpValue2 = 40.0;
                break;
            }
            if(struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].selLimit1[i] <tmpValue )
                struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].selLimit1[i]= tmpValue;
            if(struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].selLimit1[i] > tmpValue2)
                struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].selLimit1[i] = tmpValue2;

            if(struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].selLimit2[i] <tmpValue )
                struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].selLimit2[i]= tmpValue;
            if(struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].selLimit2[i] > tmpValue2)
                struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].selLimit2[i] = tmpValue2;
            m_pSelLimit1[i]->setVisible(struGsh.bRunMode > RUNMODE_USER && struCnfg.nXsystemBoard[struGsh.nView] == 1);
            m_pSelLimit1[i]->setText(QString::number(struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].selLimit1[i]));
            m_pSelLimit1[i]->setFloatMinMax(this,tmpValue,tmpValue2,204,&struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].selLimit1[i],false);
            m_pSelLimit2[i]->setVisible(struGsh.bRunMode > RUNMODE_USER && struCnfg.nXsystemBoard[struGsh.nView] == 1);
            m_pSelLimit2[i]->setText(QString::number(struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].selLimit2[i]));
            m_pSelLimit2[i]->setFloatMinMax(this,tmpValue,tmpValue2,204,&struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].selLimit2[i],false);
            m_pClassANDEnable[i]->setVisible(struGsh.bRunMode > RUNMODE_USER);
            m_pClassANDEnable[i]->setChecked(struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classANDEnable[i]);
        }
        else{
            m_pClassSensEdit[i]->setVisible(false);
            m_pClassEnable[i]->setVisible(false);
            m_pClassThresholdEdit[i]->setVisible(false);
            m_nameClassEdit[i]->setVisible(false);
            m_pBoxClassLable[i]->setVisible(false);
            m_pBoxClassLable2[i]->setVisible(false);
            m_pBoxClassEdit[i]->setVisible(false);
            m_pBoxClassEdit2[i]->setVisible(false);
            m_pTickModeClass[i]->setVisible(false);
            m_pDelayModeClass[i]->setVisible(false);
            m_pFixedClass[i]->setVisible(false);
            m_pModeLabel[i]->setVisible(false);
            m_pEjctorArithCombox[i]->setVisible(false);
            m_pHWRatioLabel[i]->setVisible(false);
            m_pPosAdjustLabel[i]->setVisible(false);
            m_pHWRatioEdit[i]->setVisible(false);
            m_pPosAdjustEdit[i]->setVisible(false);
            m_pBDLabel[i]->setVisible(false);
            m_pBDEdit[i]->setVisible(false);
            m_pBXLabel[i]->setVisible(false);
            m_pBXEdit[i]->setVisible(false);

            m_pSelReverse[i]->setVisible(false);
            m_pSelLimit1[i]->setVisible(false);
            m_pSelLimit2[i]->setVisible(false);
            m_pClassANDEnable[i]->setVisible(false);
        }   
    }

//    if(struCnfg.nXsystemBoard[struGsh.nView] == 1) //X系统隐藏部分参数
//    {
//        ui->labelRoiR->setVisible(false);
//        ui->labelRoiR2->setVisible(false);
//        ui->lineEditRoiR->setVisible(false);
//        ui->lineEditRoiR2->setVisible(false);
//    }
//    else
//    {
//        ui->labelRoiR->setVisible(true);
//        ui->labelRoiR2->setVisible(true);
//        ui->lineEditRoiR->setVisible(true);
//        ui->lineEditRoiR2->setVisible(true);
//    }

//    for(int i = 0; i < struCnfp.ipcModelClassCount[struGsh.nView]; i++){
//        if(struCnfg.nXsystemBoard[struGsh.nView] == 1) //X系统隐藏部分参数
//        {
//            m_pDelayModeClass[i]->setVisible(false);
//            m_pBoxClassLable2[i]->setVisible(false);
//            m_pBoxClassEdit2[i]->setVisible(false);
//            m_pHWRatioLabel[i]->setVisible(false);
//            m_pHWRatioEdit[i]->setVisible(false);
//            m_pPosAdjustLabel[i]->setVisible(false);
//            m_pPosAdjustEdit[i]->setVisible(false);
//            m_pBDLabel[i]->setVisible(false);
//            m_pBDEdit[i]->setVisible(false);
//            m_pBXLabel[i]->setVisible(false);
//            m_pBXEdit[i]->setVisible(false);
//        }
//        else
//        {
//            m_pDelayModeClass[i]->setVisible(true);
//            m_pBoxClassLable2[i]->setVisible(true);
//            m_pBoxClassEdit2[i]->setVisible(true);
//            m_pHWRatioLabel[i]->setVisible(true);
//            m_pHWRatioEdit[i]->setVisible(true);
//            m_pPosAdjustLabel[i]->setVisible(true);
//            m_pPosAdjustEdit[i]->setVisible(true);
//            m_pBDLabel[i]->setVisible(true);
//            m_pBDEdit[i]->setVisible(true);
//            m_pBXLabel[i]->setVisible(true);
//            m_pBXEdit[i]->setVisible(true);
//        }
//    }
}

void ipcclassparams::on_m_applyBtn_clicked()
{
    if(!struGsh.debugMode &&(struIpcShare.allIpcModelLoadOk!= 2 || struIpcShare.allIpcConnecOk == 0))
    {
        g_infoWidget().setLabelText(g_myLan().msg_ipc_poweroff);
        g_infoWidget().delayShow();
        myFlow.sleep(2);
        g_infoWidget().hide();
        return;
    }
    for(int i = 0;i < struCnfp.ipcModelClassCount[struGsh.nView][struGsh.nMatGroup]; i++){
        if(struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classMode[i] == 1){
            if(struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classLabelBox[i] > 24){
                g_infoWidget().setLabelText(g_myLan().ids_model + "2" + g_myLan().Box_maximum_value + "24");
                g_infoWidget().showSecs();
                return;
            }
        }
    }

    g_infoWidget().setLabelText(g_myLan().msg_applying);
    g_infoWidget().delayShow();

    switch(struCnfp.nIpcCameraClassSameMode)
    {
    case 0:
        break;
    case 1: //前后相同
        for(int i = 0; i < struCnfg.struLayerInfo[0].nViewChainTotal; i++)
        {
            for(int j = 0; j < struCnfg.struLayerInfo[0].stuViewInfo[i].nIdentifyGroupTotal; j++)
            {
                for(int m = 0; m < MAX_IPC_MODEL_CLASS; m++)
                {
                    struCnfp.struCameraIpcClassGroupParams[i][j].classEnable[m] = struCnfp.struCameraIpcClassGroupParams[i/2*2][j].classEnable[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classSens[m] = struCnfp.struCameraIpcClassGroupParams[i/2*2][j].classSens[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classThreshold[m] = struCnfp.struCameraIpcClassGroupParams[i/2*2][j].classThreshold[m];

                    struCnfp.struCameraIpcClassGroupParams[i][j].classLabelBox[m] = struCnfp.struCameraIpcClassGroupParams[i/2*2][j].classLabelBox[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classLabelBox2[m] = struCnfp.struCameraIpcClassGroupParams[i/2*2][j].classLabelBox2[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classTickMode[m] = struCnfp.struCameraIpcClassGroupParams[i/2*2][j].classTickMode[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classFixMode[m] = struCnfp.struCameraIpcClassGroupParams[i/2*2][j].classFixMode[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classMode[m] = struCnfp.struCameraIpcClassGroupParams[i/2*2][j].classMode[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classDelayMode[m] = struCnfp.struCameraIpcClassGroupParams[i/2*2][j].classDelayMode[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classHWRatio[m] = struCnfp.struCameraIpcClassGroupParams[i/2*2][j].classHWRatio[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classPosAdjust[m] = struCnfp.struCameraIpcClassGroupParams[i/2*2][j].classPosAdjust[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classBD[m] = struCnfp.struCameraIpcClassGroupParams[i/2*2][j].classBD[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classBX[m] = struCnfp.struCameraIpcClassGroupParams[i/2*2][j].classBX[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].selRerverse[m] = struCnfp.struCameraIpcClassGroupParams[i/2*2][j].selRerverse[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].selLimit1[m] = struCnfp.struCameraIpcClassGroupParams[i/2*2][j].selLimit1[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].selLimit2[m] = struCnfp.struCameraIpcClassGroupParams[i/2*2][j].selLimit2[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classANDEnable[m] = struCnfp.struCameraIpcClassGroupParams[i/2*2][j].classANDEnable[m];
                    memcpy(struCnfp.struCameraIpcClassGroupParams[i][j].className[m],struCnfp.struCameraIpcClassGroupParams[i/2*2][j].className[m],sizeof(struCnfp.struCameraIpcClassGroupParams[0][0].className[m]));
                    //qDebug()<<"view"<<i<<"group"<<j<<"model"<<m<<"checkd"<<struCnfp.struCameraIpcClassGroupParams[i][j].classEnable[m]<<struCnfp.struCameraIpcClassGroupParams[i/2*2][j].classEnable[m];
                }
            }
        }
        break;
    case 2: //整机相同
        for(int i = 0; i < struCnfg.struLayerInfo[0].nViewChainTotal; i++)
        {
            for(int j = 0; j < struCnfg.struLayerInfo[0].stuViewInfo[i].nIdentifyGroupTotal; j++)
            {
                for(int m = 0; m < MAX_IPC_MODEL_CLASS; m++)
                {
                    struCnfp.struCameraIpcClassGroupParams[i][j].classEnable[m] = struCnfp.struCameraIpcClassGroupParams[0][0].classEnable[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classSens[m] = struCnfp.struCameraIpcClassGroupParams[0][0].classSens[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classThreshold[m] = struCnfp.struCameraIpcClassGroupParams[0][0].classThreshold[m];

                    struCnfp.struCameraIpcClassGroupParams[i][j].classLabelBox[m] = struCnfp.struCameraIpcClassGroupParams[0][0].classLabelBox[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classLabelBox2[m] = struCnfp.struCameraIpcClassGroupParams[0][0].classLabelBox2[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classTickMode[m] = struCnfp.struCameraIpcClassGroupParams[0][0].classTickMode[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classFixMode[m] = struCnfp.struCameraIpcClassGroupParams[0][0].classFixMode[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classMode[m] = struCnfp.struCameraIpcClassGroupParams[0][0].classMode[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classDelayMode[m] = struCnfp.struCameraIpcClassGroupParams[0][0].classDelayMode[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classHWRatio[m] = struCnfp.struCameraIpcClassGroupParams[0][0].classHWRatio[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classPosAdjust[m] = struCnfp.struCameraIpcClassGroupParams[0][0].classPosAdjust[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classBD[m] = struCnfp.struCameraIpcClassGroupParams[0][0].classBD[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classBX[m] = struCnfp.struCameraIpcClassGroupParams[0][0].classBX[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].selRerverse[m] = struCnfp.struCameraIpcClassGroupParams[0][0].selRerverse[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].selLimit1[m] = struCnfp.struCameraIpcClassGroupParams[0][0].selLimit1[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].selLimit2[m] = struCnfp.struCameraIpcClassGroupParams[0][0].selLimit2[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classANDEnable[m] = struCnfp.struCameraIpcClassGroupParams[0][0].classANDEnable[m];
                    memcpy(struCnfp.struCameraIpcClassGroupParams[i][j].className[m],struCnfp.struCameraIpcClassGroupParams[0][0].className[m],sizeof(struCnfp.struCameraIpcClassGroupParams[0][0].className[m]));
                }
            }
        }
        break;
    }
    myFlow.configIpcAIPara(0,0);
    myFlow.configIpcClassParams(true,0,0);
    myFlow.IpcClassResultANDOperat(true,0,0);
    myFlow.setIpcCamOverRatio(true,0,0,0);
    g_infoWidget().hide();
}

void ipcclassparams::on_syncModeNameBtn_clicked()
{
    myMessageBox msgBox(MSG_QUES, g_myLan().cfm_sync_class_name+"?");
    int ret = msgBox.exec();
    if (ret == QDialog::Accepted){ //用户操作，同意进行

        int nUnitAddr = myFlow.getIdentifyGroupAddr(0, struGsh.nView, struGsh.nMatGroup, 0) %64;

        memset(struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].className,0,
                sizeof(struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].className));

        memcpy(struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].className,
                struIpcShare.struIpcModelClass[struGsh.nView][nUnitAddr].className,
                sizeof(struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].className));
        updateView();
    }
}
