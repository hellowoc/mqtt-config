#include "runtime.h"
#include <QApplication>
#include <QTranslator>
#include <QFontDatabase>
#include <QSettings>
#include "data/jsondataconvert.h"
#include "globalflow.h"
#include <QDir>
#include <QDirIterator>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QtNetwork/QHostInfo>
Runtime::Runtime(QObject *parent) : QObject(parent)
{
    g_selfCheckState = true;

    g_level = 0;
    g_level2 = 3;
    g_usedFamilyId = -1;
    for(unsigned int id = 0; id < sizeof(m_fontFamilyId)/sizeof(int); id++)
        m_fontFamilyId[id] = -1;

    QSettings setting(CFG_APPSet,QSettings::IniFormat);

    /* 读取界面语言信息 */
    g_layerTotal =  setting.value("layertotal",1).toInt();
    g_skinIndex = setting.value("skinIndex",1).toInt();
    g_vendorIndex = setting.value("vendorIndex",1).toInt();
    m_sampleMap[0].clear();
    m_sampleMap[1].clear();
    vendorNameList<<"none"<<"amd"<<"ucs"<<"sw"<<"lauffer"<<"iran"<<"selgron"<<"tanis";
    nMachineArithTotal = 0;
    appPositionX = 0;
    appPositionY = 0;



}

const QString Runtime::getVerdorName()
{
    if(!vendorNameList.isEmpty())
        return vendorNameList[g_vendorIndex];
    else
        return QString("zk");
}

void Runtime::getCurImageLayerAndView(int &layer, int &view ,int &nUnit, int index)
{
    if(!nimageNameVec.isEmpty()){
        if(index < nimageNameVec.count()){
            QString imagePath = nimageNameVec[index];

            QStringList list = imagePath.split("/");
            QStringList namelist = list.last().split("_");
            if(namelist.count() < 3){
                layer = 0;
                view = 0;
                nUnit = 0;
                logger()->warn("imagepath is not standard:%1",list.last());
            }else{
#ifdef Q_OS_WIN
                //                layer = 0;
                //                view = 0;
                //                nUnit = 0;
                layer =  namelist[0].toInt();
                view = namelist[1].toInt();
                nUnit = namelist[2].toInt();
#else
                layer =  namelist[0].toInt();
                view = namelist[1].toInt();
                nUnit = namelist[2].toInt();
#endif
            }
        }
    }
}

/* 转换文件大小为B、K、M */
QString Runtime::getFileSize(QString filePath)
{
    QFileInfo file(filePath);

    int integer = 0;  //整数位
    int decimal = 0;  //小数位，保留2位
    char unit ='B';
    qint64 standardSize = file.size();
    qint64 curSize = file.size();

    if(standardSize > 1024) {
        curSize = standardSize * 100;
        curSize /= 1024;
        integer = curSize / 100;
        decimal = curSize % 100;
        standardSize /= 1024;
        unit = 'K';
        if(standardSize > 1024) {
            curSize = standardSize * 100;
            curSize /= 1024;
            integer = curSize / 100;
            decimal = curSize % 100;
            standardSize /= 1024;
            unit = 'M';
            if(standardSize > 1024) {
                curSize = standardSize * 100;
                curSize /= 1024;
                integer = curSize / 100;
                decimal = curSize % 100;
                unit = 'G';
            }
        }
    }

    QString dec = "0";
    if (0 <= decimal && decimal <= 9) {
        dec = dec + QString::number(decimal);
    }

    if (10 <= decimal && decimal <= 99) {
        dec = QString::number(decimal);
    }

    return QString::number(integer) + "." + dec + unit;
}

/* 获取文件md5值 */
QString Runtime::getFileMd5(QString filePath)
{
    QFile localFile(filePath);
    if (!localFile.open(QFile::ReadOnly))
    {
        qDebug() << "file open error.";
        return 0;
    }

    QCryptographicHash ch(QCryptographicHash::Md5);

    quint64 totalBytes = 0;
    quint64 bytesWritten = 0;
    quint64 bytesToWrite = 0;
    quint64 loadSize = 1024 * 4;
    QByteArray buf;

    totalBytes = localFile.size();
    bytesToWrite = totalBytes;

    while (1)
    {
        if(bytesToWrite > 0)
        {
            buf = localFile.read(qMin(bytesToWrite, loadSize));
            if(buf.size() <= 0)
            {
                qDebug()<<"get md5 err";
                return 0;
            }
            ch.addData(buf);
            bytesWritten += buf.length();
            bytesToWrite -= buf.length();
            buf.resize(0);
        }
        else
        {
            break;
        }

        if(bytesWritten == totalBytes)
        {
            break;
        }
    }

    localFile.close();
    QByteArray md5 = ch.result();
    QString str = QString(md5.toHex());

    return QString(md5.toHex().toUpper());
}

void Runtime::saveSetting()
{
    QSettings setting(CFG_APPSet,QSettings::IniFormat);

    setting.setValue("vendorIndex",g_vendorIndex);
}
bool Runtime::save()
{
    logger()->info("save json begin");
    g_Runtime().mySystem("sysctl -w vm.drop_caches=3 > /dev/zero");
    struGsh.backCnf_status = 0;
    QSettings setting(CFG_APPSet,QSettings::IniFormat);
    setting.setValue("backCnf_status",struGsh.backCnf_status);
    setting.sync();
    myFlow.getCompressParams();
    struGsh.backCnf_status = 1;
    setting.setValue("backCnf_status",struGsh.backCnf_status);
    setting.sync();
    // 多灯光系数保存背景灯光使能
    for(int i = 0; i < MAX_LAMP_BOARD_PER_INTERFACE;i++){
        for(int j = 0; j < MAX_LAMP_PER_BOARD;j++){
              if(struCnfg.stuLayerLampCfg.nLampPortType[i][j] == 1){
                    struCnfg.stuLayerLampCfg.nLampEnable[i][j][struCnfp.bCurCal] = struCnfp.nLampEnable[0][i][j];
              }
        }
    }
    struCnfg.bCaliGainEnable_calb[struCnfp.bCurCal] = struCnfp.bCaliGainEnable;

    if(myFlow.saveProfile()&& myFlow.saveGlobal())
    {
#ifdef Q_OS_UNIX
        QFileInfo fileInfo("/dev/mmcblk0p2");
        if(!fileInfo.exists())//SD卡已插入
        {
            QString  fileDir = "/media/mmcblk0p1/cnf";
            QDir dir(fileDir);
            if(!dir.exists())
            {
                bool ok = dir.mkdir(fileDir);
                if (!ok) return false;
            }
            system("cp /sdcard/cnf/* -fr /media/mmcblk0p1/cnf/");
        }
        system("sync");
#endif
        struGsh.backCnf_status = 0;
        QSettings setting(CFG_APPSet,QSettings::IniFormat);
        setting.setValue("backCnf_status",struGsh.backCnf_status);
        setting.sync();
        // 再备份文件
        myFlow.getCompressParams();
        struGsh.backCnf_status = 1;
        setting.setValue("backCnf_status",struGsh.backCnf_status);
        setting.sync();
        logger()->info("save json end ok");
        return true;
    }
    else{
        logger()->info("save json end fail");
        return false;
    }
}


void Runtime::updateTmpImgList()
{
    QFileInfoList fileInfo;
    QString fileName;
    QString dirPath;
#ifdef Q_OS_UNIX

    if(g_Runtime().checkUsbExist() && struCnfg.autoTargetImage == 1)
    {
        system("sync");
        QString fileName = QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz");
        QString path = g_Runtime().getUsbPath();
        if (path != "")
        {
           dirPath = path+"IMG/"+fileName.left(8) + "/";
        }
        else
        {
           dirPath = QString("/sdcard/bmp/");
        }
    }
    else
        dirPath = QString("/sdcard/bmp/");
#else
    dirPath = QString(qApp->applicationDirPath()+"/bmp/");
#endif
    QDir dir(dirPath);
    QStringList filter;
    filter<<"*.png"<<"*.bmp";
    dir.setNameFilters(filter);
    fileInfo = dir.entryInfoList(filter,QDir::Files,QDir::Time);
    nimageNameVec.clear();

    for(int i=0; i<fileInfo.size(); i++){
        fileName = fileInfo.at(i).fileName();
        if((fileName.contains(".png")|| fileName.contains(".bmp"))&&!fileName.contains("_copy"))
            nimageNameVec.push_back(dirPath+fileName);
    }
}


QString Runtime::GetIpAddress(QString netName)
{
    //netName  lo  eth0  eth1   wlan
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    QString ipAddress="";
    foreach (QNetworkInterface nf, list)
    {
        if(netName != nf.name())
        {
            continue;
        }

        foreach(QNetworkAddressEntry entry , nf.addressEntries())
        {

            QHostAddress address =  entry.ip();// entry.broadcast();
            if(address.protocol() == QAbstractSocket::IPv4Protocol)
            {
                ipAddress = address.toString();
                break;
            }

        }

    }
    if(!ipAddress.isEmpty())
        qDebug()<<ipAddress;
    return ipAddress;
}

void Runtime::initLan()
{
}

bool Runtime::copyDirToPath(const QString& srcPath, const QString& dstPath, bool coverFileIfExist)
{
    QDir srcDir(srcPath);
    QDir dstDir(dstPath);
    if (!dstDir.exists()) { //目的文件目录不存在则创建文件目录
        if (!dstDir.mkdir(dstDir.absolutePath()))
            return false;
    }
    QFileInfoList fileInfoList = srcDir.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList) {
        if (fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;

        if (fileInfo.isDir()) {    // 当为目录时，递归的进行copy
            if (!copyDirToPath(fileInfo.filePath(),dstDir.filePath(fileInfo.fileName()),coverFileIfExist))
                return false;
        }
        else {            //当允许覆盖操作时，将旧文件进行删除操作
            if (coverFileIfExist && dstDir.exists(fileInfo.fileName())) {
                dstDir.remove(fileInfo.fileName());
            }
            // 进行文件copy
            if (!QFile::copy(fileInfo.filePath(), dstDir.filePath(fileInfo.fileName()))) {
                return false;
            }
        }
    }
    return true;
}

bool Runtime::copyFileToPath(QString sourceDir, QString toDir, bool coverFileIfExist)
{
    //toDir.replace("\\","/");
    if(sourceDir == toDir)
        return true;

    if(!QFile::exists(sourceDir))
        return false;

    QDir createfile;
    bool exist = createfile.exists(toDir);
    if(exist)
    {
        if(coverFileIfExist)
        {
            createfile.remove(toDir);
        }
    }

    if(!QFile::copy(sourceDir, toDir))
    {
        return false;
    }
    return true;
}

bool Runtime::delFile(QString filePath)
{
    QFile file(filePath);
    return file.remove();
}

bool Runtime::checkUsbExist()
{
    QDir myDir("/proc/scsi/usb-storage");
    if (!myDir.exists()) {
        return false;
    }
    return true;
}

bool Runtime::dirExist(QString dir)
{
    QDir newdir(dir);
    if(!newdir.exists()){
        newdir.mkpath(dir);
        return false;
    }
    return true;
}

QString Runtime::getUsbPath()
{
    QStringList options;
    options << "-c" << "cat /proc/mounts |grep /dev/sd |sed -n '$p' |awk '{printf $2}'";
    QProcess process;
    process.start("/bin/bash", options);
    if(process.waitForFinished()){
        QString str = process.readAll();
        if (str.contains("/media/sd"))
            return str+"/";
    }

    int i = 0;
    //u盘盘符变化
    QString path = "";
    for(i = 0;i < 5; i++)
    {
        if (i == 0) path = "/media/sda/";
        else path = "/media/sda" + QString::number(i)+"/";
        QFileInfo fileinfo(path);
        if (fileinfo.exists())
            return path;
    }

    for(i = 0;i < 5; i++)
    {
        if (i == 0) path = "/media/sdb/";
        else path = "/media/sdb" + QString::number(i)+"/";
        QFileInfo fileinfo(path);
        if (fileinfo.exists())
            return path;
    }

    path = "";
    return path;

}

int Runtime::mySystem(QString str)
{
#ifdef Q_OS_UNIX
    if (str.isEmpty()) return 0;
    const char* cmd = QString(str).toLocal8Bit().constData();

    pid_t status = system(cmd);
    if (-1 == status)
    {
        return 0;
    }
    else
    {
        if (WIFEXITED(status))  //正确退出
        {
            if (0 == WEXITSTATUS(status)) //操作成功
            {
                system("sync");
                return 1;
            }
            else  //操作失败
            {
                return 0;
            }
        }
        else  //错误退出
        {
            return 0;
        }
    }
    return 0;
#endif
}

QString Runtime::mySystemStr(QString str)
{
#ifdef Q_OS_UNIX
    if (str.isEmpty()) return "";

    QStringList options;
    int value = 0;
    options << "-c" << str;
    QProcess process;
    process.start("/bin/bash", options);
    if(process.waitForFinished()){
        return process.readAll();
    }
    return "";
#endif
}

QStringList Runtime::getUsbSpace(QString dev)
{
    QProcess m_cmd;
    QStringList sdSpace;
    m_cmd.start("df " +dev);
    m_cmd.waitForFinished(3000);
    while (!m_cmd.atEnd()) {
        QString result = QLatin1String(m_cmd.readLine());
        if (result.startsWith("/dev")) {
            result.replace("\n","");
            result.replace(QRegExp("( ){1,}")," ");
            sdSpace = result.split(" ");
            break;
        }
    }
    return sdSpace;
}

int Runtime::getStartUpMode()
{
    QStringList options;
    options << "-c" << "cat /proc/mounts |grep sdcard |sed -n '$p' |awk '{printf $1}'";
    QProcess process;
    process.start("/bin/bash", options);
    if(process.waitForFinished()){
        QString str = process.readAll();
        if (str.contains("/dev/mmcblk0p1")) return 0;
        else if (str.contains("/dev/mmcblk1p2") || str.contains("/dev/mmcblk0p2")) return 1;
    }
    return -1;
}
bool Runtime::checkIpcMounted(QString ipcIpAddr)
{
    QString cmd = "cat /proc/mounts |grep "+ipcIpAddr+" |sed -n '$p' |awk '{printf $4}'";
    qDebug()<<cmd;
    QStringList options;
    options << "-c" << cmd;
    QProcess process;
    process.start("/bin/bash", options);
    if(process.waitForFinished()){
        QString str = process.readAll();
        QStringList sdAttribute = str.split(",");
        if (sdAttribute.at(0) == "rw"){
            return true;
        }
    }
    return false;
}
QString Runtime::fillIn(QString str,int maxLen, QChar c)
{
    int len = str.length();
    if(len < maxLen)
    {
        for(int i =0 ; i < maxLen - len; i++)
            str +=  c;
    }
    return str;
}

void Runtime::updateCornStastic()
{
    FILE *fp = fopen("/sdcard/ts/corndata.csv", "a+");
    if(fp == NULL){
        printf("read file failed\n");
        exit(EXIT_FAILURE);
    }

    QString date = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    fprintf(fp, "%s,%s,%s,%s,%s\n",QString(date).toLocal8Bit().data(),
            QString::number(struGsh.cornStatics[0][0][struGsh.cornLastIndex]).toLocal8Bit().data(),
            QString::number(struGsh.cornStatics[0][1][struGsh.cornLastIndex]).toLocal8Bit().data(),
            QString::number(struGsh.cornStatics[1][0][struGsh.cornLastIndex]).toLocal8Bit().data(),
            QString::number(struGsh.cornStatics[1][1][struGsh.cornLastIndex]).toLocal8Bit().data());

    fclose(fp);
}
void Runtime::getFreeSpace()
{
    QStringList options;
    int value = 0;
    QStringList spaceList = g_Runtime().getUsbSpace("/sdcard");

    if(spaceList.length() > 3){
        QString space = spaceList.at(3);
        value = QString(space).toInt();
        struCnfg.nFreeSpace = value/1024.0;
        qDebug()<<"free space:"<<struCnfg.nFreeSpace<<"M";
    }
}


