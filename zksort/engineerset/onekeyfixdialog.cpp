#include "onekeyfixdialog.h"
#include "ui_onekeyfixdialog.h"

OneKeyDialog::OneKeyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OneKeyDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);

    ui->lineEdit_name->setType(textType);
    ui->lineEdit_phone->setType(textType);
    ui->lineEdit_QCnum->setType(textType);
    ui->lineEdit_comname->setType(textType);


    ui->lineEdit_place_1->setType(textType);
    ui->lineEdit_place_2->setType(textType);
    ui->lineEdit_place_3->setType(textType);
    ui->lineEdit_place_4->setType(textType);
    ui->lineEdit_type2->setType(textType);
    ui->lineEdit_machineID->setType(textType);

    updateCustomInfo();
    InfoEnableChange(false);

    ui->label->setText(g_myLan().Confirm_information);
    ui->cancelBtn->setText(g_myLan().ids_cancel);
    ui->okBtn->setText(g_myLan().ids_ok);
    ui->label->hide();
}

OneKeyDialog::~OneKeyDialog()
{
    delete ui;
}

void OneKeyDialog::receiveMsg(int msg1, int msg2, int msg3)
{


}

void OneKeyDialog::InfoEnableChange(bool enable)
{

    ui->checkBox->setChecked(enable);
    ui->lineEdit_name->setEnabled(enable);
    ui->lineEdit_phone->setEnabled(enable);
    ui->lineEdit_QCnum->setEnabled(enable);
    ui->lineEdit_comname->setEnabled(enable);
    ui->lineEdit_place_1->setEnabled(enable);
    ui->lineEdit_place_2->setEnabled(enable);
    ui->lineEdit_place_3->setEnabled(enable);
    ui->lineEdit_place_4->setEnabled(enable);
    ui->lineEdit_type2->setEnabled(enable);
    ui->lineEdit_machineID->setEnabled(enable);
}

void OneKeyDialog::updateCustomInfo()
{

    QSettings setting(CFG_APPSet,QSettings::IniFormat);
    QString temp = setting.value("name","").toString();
    ui->lineEdit_name->setText(temp);

    temp = setting.value("customPhone","").toString();
    ui->lineEdit_phone->setText(temp);


    temp = setting.value("customerName","").toString();
    ui->lineEdit_comname->setText(temp);

    temp = setting.value("Province","").toString();
    ui->lineEdit_place_1->setText(temp);

    temp = setting.value("City","").toString();
    ui->lineEdit_place_2->setText(temp);

    temp = setting.value("County","").toString();
    ui->lineEdit_place_3->setText(temp);

    temp = setting.value("DetialPlace","").toString();
    ui->lineEdit_place_4->setText(temp);

    temp = setting.value("customQCnum","").toString();
    ui->lineEdit_QCnum->setText(temp);

    temp = setting.value("connectName","").toString();
    ui->lineEdit_machineID->setText(temp);

    temp = setting.value("extModel","").toString();
    ui->lineEdit_type2->setText(temp);


    QMap<int,QString> m_machineTypeMap;
    m_machineTypeMap.clear();
    m_machineTypeMap.insert(MACHINE_VERTICAL_ZL_CV,"ZL_LS");
    m_machineTypeMap.insert(MACHINE_LD_ZL_GP,"ZL_LD");
    m_machineTypeMap.insert(MACHINE_LD2_ZL_GP,"ZL_LDS");
    ui->label_type->setText(g_myLan().ids_machineModel + m_machineTypeMap.value(struCnfg.nMachineType));

}

void OneKeyDialog::on_cancelBtn_clicked()
{
    reject();
}

void OneKeyDialog::on_okBtn_clicked()
{

#ifdef Q_OS_UNIX


    QString temp ;

    if(ui->lineEdit_name->text() == "" || ui->lineEdit_phone->text() == "" ||  ui->lineEdit_comname->text() == "" ||   ui->lineEdit_place_1->text() == "" ||
            ui->lineEdit_place_2->text() == "" ||   ui->lineEdit_place_3->text() == "" ||   ui->lineEdit_place_4->text() == "" ||   ui->lineEdit_QCnum->text() == "" ||   ui->lineEdit_machineID->text() == "" ||
            ui->lineEdit_type2->text() == "" ||   ui->label_type->text() == "")
    {
        g_infoWidget().setLabelText(g_myLan().information_error);
        g_infoWidget().showSecs();
        return;
    }


    temp = ui->lineEdit_phone->text();
    if (temp.length() == 11 && temp.startsWith('1') && temp.toStdString().find_first_not_of("0123456789") == std::string::npos)
    {

    } else {
        g_infoWidget().setLabelText(g_myLan().information_error);
        g_infoWidget().showSecs();
        return;
    }

    temp = ui->lineEdit_QCnum->text();
    if (temp.length() == 8 && temp.startsWith("QC")) {
        QChar c3 = temp[2];  // 第3个字符
        QChar c4 = temp[3];  // 第4个字符
        QChar c5 = temp[4];  // 第5个字符（大写字母）
        QChar c6 = temp[5];  // 第6个字符
        QChar c7 = temp[6];  // 第7个字符
        QChar c8 = temp[7];  // 第8个字符
        // 检查每个位置的字符是否符合要求
        bool isValid = c3.isDigit() &&            // 第3个字符是数字
                c4.isDigit() &&            // 第4个字符是数字
                c5.isUpper() &&            // 第5个字符是大写字母
                c6.isDigit() &&            // 第6个字符是数字
                c7.isDigit() &&             // 第7个字符是数字
                c8.isDigit();             // 第8个字符是数字
        if (isValid)
        {

        } else {
            g_infoWidget().setLabelText("QC"+g_myLan().information_error);
            g_infoWidget().showSecs();
            return;
        }
    } else {
        g_infoWidget().setLabelText("QC"+g_myLan().information_error);
        g_infoWidget().showSecs();
        return;
    }

    temp = ui->lineEdit_type2->text();

    for (QString::const_iterator it = temp.begin(); it != temp.end(); ++it) {
        QChar c = *it;
        if (c.isUpper() || c.isDigit()) {

        }
        else
        {
            g_infoWidget().setLabelText(g_myLan().information_error);
            g_infoWidget().showSecs();
            return;
        }
    }

//    myMqttMsgParaseThread->n_uploadOneKeyFixFlag = 1;
    g_infoWidget().setLabelText(g_myLan().ids_succeeded);
    g_infoWidget().showSecs();
#endif

    accept();
}


void OneKeyDialog::on_lineEdit_name_textChanged(const QString &arg1)
{
    if (arg1 != "")
    {
        QSettings setting(CFG_APPSet,QSettings::IniFormat);
        setting.setValue("name",arg1);
        setting.sync();

    }
}

void OneKeyDialog::on_lineEdit_QCnum_textChanged(const QString &arg1)
{
    if (arg1 != "")
    {
        QSettings setting(CFG_APPSet,QSettings::IniFormat);
        setting.setValue("customQCnum",arg1);
        setting.sync();

    }
}

void OneKeyDialog::on_lineEdit_phone_textChanged(const QString &arg1)
{
    if (arg1 != "")
    {
        QSettings setting(CFG_APPSet,QSettings::IniFormat);
        setting.setValue("customPhone",arg1);
        setting.sync();

    }
}


void OneKeyDialog::on_checkBox_clicked(bool checked)
{
    InfoEnableChange(checked);
}

void OneKeyDialog::on_lineEdit_comname_textChanged(const QString &arg1)
{
    if (arg1 != "")
    {
        QSettings setting(CFG_APPSet,QSettings::IniFormat);
        setting.setValue("customerName",arg1);
        setting.sync();

    }
}

void OneKeyDialog::on_lineEdit_place_1_textChanged(const QString &arg1)
{
    if (arg1 != "")
    {
        QSettings setting(CFG_APPSet,QSettings::IniFormat);
        setting.setValue("Province",arg1);
        setting.sync();

    }
}

void OneKeyDialog::on_lineEdit_place_2_textChanged(const QString &arg1)
{
    if (arg1 != "")
    {
        QSettings setting(CFG_APPSet,QSettings::IniFormat);
        setting.setValue("City",arg1);
        setting.sync();

    }
}

void OneKeyDialog::on_lineEdit_place_3_textChanged(const QString &arg1)
{
    if (arg1 != "")
    {
        QSettings setting(CFG_APPSet,QSettings::IniFormat);
        setting.setValue("County",arg1);
        setting.sync();

    }
}

void OneKeyDialog::on_lineEdit_place_4_textChanged(const QString &arg1)
{
    if (arg1 != "")
    {
        QSettings setting(CFG_APPSet,QSettings::IniFormat);
        setting.setValue("DetialPlace",arg1);
        setting.sync();

    }
}

void OneKeyDialog::on_lineEdit_type2_textChanged(const QString &arg1)
{
    if (arg1 != "")
    {
        QSettings setting(CFG_APPSet,QSettings::IniFormat);
        setting.setValue("extModel",arg1);
        setting.sync();

    }
}

void OneKeyDialog::on_lineEdit_machineID_textChanged(const QString &arg1)
{
    if (arg1 != "")
    {
        QSettings setting(CFG_APPSet,QSettings::IniFormat);
        setting.setValue("connectName",arg1);
        setting.sync();

    }
}
