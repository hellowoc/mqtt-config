#include "selectupdatefilewidget.h"
#include "ui_selectupdatefilewidget.h"

SelectUpdateFileWidget::SelectUpdateFileWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::SelectUpdateFileWidget)
{
    ui->setupUi(this);
    m_plistWidget = ui->listWidget;
    m_plistWidget->setIconSize(QSize(40,40));
    ui->tarUpdateFileBtn->hide();
    ui->startUpdateBtn->hide();
}

SelectUpdateFileWidget::~SelectUpdateFileWidget()
{
    delete ui;
}

void SelectUpdateFileWidget::showPage(bool bshow)
{
    if(bshow)
    {
        m_plistWidget->clear();
        on_loadFileBtn_clicked();
    }
}

void SelectUpdateFileWidget::receiveMsg(int msg1, int msg2, int msg3)
{

}

void SelectUpdateFileWidget::retranslateUiWidget()
{

}

void SelectUpdateFileWidget::on_loadFileBtn_clicked()
{
    QString dirPath = g_Runtime().getUsbPath() + specificFolder;
    if(!g_Runtime().checkUsbExist()){
        g_infoWidget().setLabelText(g_myLan().msg_usb_insert);
        g_infoWidget().showSecs();
        return;
    }
    listCompressedFiles(dirPath);
}

void SelectUpdateFileWidget::listCompressedFiles(const QString &directoryPath) {
    QDir dir(directoryPath);
    QStringList filters;
    filters << "*.zip";
    QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files);

    m_plistWidget->clear();
    foreach(QFileInfo fileInfo, fileInfoList) {
        m_plistWidget->addItem(fileInfo.fileName());
    }
}

void SelectUpdateFileWidget::on_applyBtn_clicked()
{
    QListWidgetItem *currentItem = m_plistWidget->currentItem();
    if (currentItem) {
        g_Runtime().updateZipFileName  = currentItem->text();
    } else {
        g_infoWidget().setLabelText("Please select a file to extract!");
        g_infoWidget().showSecs();
        return;
    }
    updateCurUpdateFile();

    // 判断路径是否存在
    QString absoluteFileName;
    QString rootDir;
    rootDir = g_Runtime().getUsbPath() + specificFolder;
    QDir absoluteFileDir(rootDir);
    absoluteFileName = absoluteFileDir.absoluteFilePath(g_Runtime().updateZipFileName);
    absoluteFileName.chop(absoluteFileName.length() - absoluteFileName.lastIndexOf('.'));
    tarUpdateFile();
    myFlow.sleep(1);
    g_infoWidget().setLabelText(g_myLan().ids_checkUpdateFile);
    g_infoWidget().showSecs();
    g_MainManager().ShowWidget(SM_ONE_KEY_UPDATE_FPGA_WIDGET);


}

void SelectUpdateFileWidget::updateCurUpdateFile()
{
    ui->label_2->setText(g_Runtime().updateZipFileName);
}

void SelectUpdateFileWidget::on_startUpdateBtn_clicked()
{
    if(g_Runtime().updateZipFileName == "")
    {
        g_infoWidget().setLabelText(g_myLan().ids_openUpgradeFile);
        g_infoWidget().showSecs();
        return;
    }
    g_MainManager().ShowWidget(SM_ONE_KEY_UPDATE_FPGA_WIDGET);
}

void SelectUpdateFileWidget::tarUpdateFile()
{
    QString absoluteFileName;
    QString rootDir;
    rootDir = g_Runtime().getUsbPath() + specificFolder;
    QDir absoluteFileDir(rootDir);
    absoluteFileName = absoluteFileDir.absoluteFilePath(g_Runtime().updateZipFileName);

    QProcess process;

    // 定义解压命令.
    QString program = "unzip";
    QStringList arguments;

    arguments << "-q"; // -q 表示静默解压
    arguments << "-o"; // -o 表示覆盖已经存在的文件
    arguments << absoluteFileName; // your zip file path
    arguments << "-d"; // -d 表示解压到指定目录
    arguments << rootDir; // 解压到目标文件夹

    // 启动进程并执行命令
    g_infoWidget().setLabelText(g_myLan().ids_apply);
    g_infoWidget().delayShow();
    process.start(program, arguments);

    arguments << "sync";
    process.start(program, arguments);

    absoluteFileName.chop(absoluteFileName.length() - absoluteFileName.lastIndexOf('.'));
    QString updateFileDir = absoluteFileName+"/";

    myFlow.sleep(3);

    if (!process.waitForFinished(-1)) {
        qDebug() << "Unzip operation failed: " << process.errorString();
    } else {
        bool result = checkFilesSize(updateFileDir);
        if(result)
        {
            g_infoWidget().setLabelText(g_myLan().Unzip_success);
            g_infoWidget().showSecs();
        }
        else
        {
            g_infoWidget().setLabelText(g_myLan().Unzip_failed);
            g_infoWidget().showSecs();
        }
    }
}

void SelectUpdateFileWidget::on_tarUpdateFileBtn_clicked()
{


}

bool SelectUpdateFileWidget::checkFilesSize(const QString& path) {
    QDir dir(path);

    // 获取所有文件信息
    QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    foreach (const QFileInfo& fileInfo, fileInfoList) {
        qDebug()<<"fileInfo.size()"<<fileInfo.size();
        if (fileInfo.size() == 0) {
            return false;
        }
    }
    return true;
}



