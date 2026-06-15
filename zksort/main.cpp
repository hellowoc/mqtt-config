#include "zksort.h"
#include <QApplication>
#include <QDesktopWidget>
#include "logger.h"
#include "ttcclayout.h"
#include "dailyrollingfileappender.h"
#include "loggerrepository.h"
#include "logmanager.h"
#include "propertyconfigurator.h"
#include "helpers/properties.h"
#include <QTextCodec>
#include <QFileInfo>
#include <QThread>
#include <QResource>
#include <QFontDatabase>
#include "global.h"
#include "data/jsondataconvert.h"
#include <QSplashScreen>

#ifdef breakpad
#include "client/linux/handler/exception_handler.h"
static bool dumpCallback(const google_breakpad::MinidumpDescriptor& descriptor, void* context, bool succeeded)
{
    qDebug()<<"Dump path: "<<descriptor.path()<<" succeeded:"<<succeeded;
    return succeeded;
}
#endif
void setDumpPath(const QString& dirName)
{
#ifdef Q_OS_LINUX
            QDir dir;
            bool bExists = dir.exists("./"+dirName);//判断根目录下是否存在crash目录
            if(!bExists)
            {
               bool ok=false;
               ok = dir.mkdir("./"+dirName);
               qDebug()<<"create dir "<<dirName<<" success";

            }else
            {
                qDebug()<<dirName<<" exist";
            }
#endif
}

int main(int argc, char *argv[])
{
#ifdef breakpad
    setDumpPath("/crash");
    google_breakpad::MinidumpDescriptor descriptor("./crash");
    google_breakpad::ExceptionHandler eh(descriptor, NULL, dumpCallback, NULL, true, -1);
#endif
    zkApplication a(argc, argv);
    QThread::currentThread()->setObjectName("Mainthread");
    //定义编码格式
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    //设置和对本地文件系统读写时候的编码格式
    QTextCodec::setCodecForLocale(codec);
    //设置传给tr函数时的默认字符串编码
//    QTextCodec::setCodecForTr(codec);
//    //用在字符常量或者QByteArray构造QString对象时使用的一种编码方式
//    QTextCodec::setCodecForCStrings(codec);

    QString usbPath;
    if ((usbPath = g_Runtime().getUsbPath()) != ""){
        QFileInfo config(usbPath+"zksort_del_cnf.txt");
        if(config.exists()){
            g_Runtime().delFile(config.absoluteFilePath());
            g_Runtime().mySystem("rm /sdcard/cnf -rf");
        }
        QFileInfo config2(usbPath+"zksort_upgrade.txt");
        if(config2.exists()){
            g_Runtime().delFile(config2.absoluteFilePath());
            g_Runtime().mySystem("touch /media/mmcblk0p1/uprade");
        }
        QFileInfo config3(usbPath+"zksort_upgradep1.txt");
        if(config3.exists()){
            g_Runtime().delFile(config3.absoluteFilePath());
            g_Runtime().mySystem("touch /media/mmcblk0p1/upradep1");
        }
        QFileInfo configShell(usbPath+"cmd.sh");
        if(configShell.exists()){
            g_Runtime().mySystem("cp "+usbPath+"cmd.sh -fr /sdcard/.cmd.sh");
            g_Runtime().mySystem("sync");
            g_Runtime().mySystem("chmod a+x /sdcard/.cmd.sh");
            g_Runtime().mySystem("mv "+usbPath+"cmd.sh "+usbPath+".cmd.sh");
            QProcess::startDetached("sh", QStringList() << "/sdcard/.cmd.sh");
            myFlow.sleep(5);
            g_Runtime().mySystem("sync");
        }
    }

    //启动动画
    QFileInfo bootImg("/sdcard/logo/boot.bmp");
    QSplashScreen *splash = NULL;
    if(bootImg.exists()){
        QPixmap pixmap("/sdcard/logo/boot.bmp");
        splash = new QSplashScreen;
        splash->setPixmap(pixmap);
//          QMovie *Movie = new QMovie(qApp->applicationDirPath()+"./test.gif");
//          QLabel  *Label = new QLabel(&splash);
//          Label->setMovie(Movie);
//          Movie->start();
        splash->show();
    }

    //日志文件管理
    #ifdef Q_OS_WIN
        QString path = qApp->applicationDirPath()+"/logs/";
    #else
        QString path = "/sdcard/logs/";
    #endif

    QDir dir(path);
    QStringList filter;
    filter<<"log.txt.*";
    dir.setNameFilters(filter);
    //按时间排序
    QList<QFileInfo> fileInfo = QList<QFileInfo>(dir.entryInfoList(filter,QDir::AllEntries,QDir::Name));

    if(fileInfo.size() >= 30){
        QFile::remove(fileInfo.at(0).absoluteFilePath());
    }

    QString dateString = QDateTime::currentDateTime().toString("yyyy-MM-dd");
//    QString name = dateString+ ".txt";
    QString name = "log.txt";
    #ifdef Q_OS_WIN
        QFileInfo logFileInfo(qApp->applicationDirPath()+"/logs/"+name);
    #else
        QFileInfo logFileInfo("/sdcard/logs/"+name);
    #endif

    if (logFileInfo.exists() && logFileInfo.size() > 0)
    {
        QStringList sections ;

        QFile file(logFileInfo.absoluteFilePath());
        if(file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            while (!file.atEnd())
            {
                QByteArray line = file.readLine();
                QString str(line);
    //            str.remove("\n");// 去掉日志换行显示
                if(str == "header" || str == "footer") continue;
                sections =  str.split(QRegExp(" "));
                if(sections.at(0).contains("-"))
                {
                    break;
                }
            }
        }
        file.close();
        if (sections.at(0) != "")
        {
            if (sections.at(0) != "")
            {
                //跨天开机需将临时文件log.txt移为当日
                if (sections.at(0) !=  dateString)
                {
                    g_Runtime().copyFileToPath(logFileInfo.absoluteFilePath(),logFileInfo.absoluteFilePath()+"."+sections.at(0),true);
                    g_Runtime().delFile(logFileInfo.absoluteFilePath());
                }
            }
        }
    }

    //产品UI日志
    Log4Qt::Properties qtlogProper;
    qtlogProper.setProperty("log4j.rootLogger", "DEBUG, A1");

    qtlogProper.setProperty("log4j.appender.A1", "org.apache.log4j.DailyRollingFileAppender");
    qtlogProper.setProperty("log4j.appender.A1.appendFile","true");
    qtlogProper.setProperty("log4j.appender.A1.immediateFlush","true");
    qtlogProper.setProperty("log4j.appender.A1.datePattern","'.'yyyy-MM-dd");
//    qtlogProper.setProperty("log4j.appender.A1.file", qApp->applicationDirPath()+QString("/logs/log.txt"));
//    qtlogProper.setProperty("log4j.appender.A1.file", ("/sdcard/logs/log.txt"));
     qtlogProper.setProperty("log4j.appender.A1.file", logFileInfo.absoluteFilePath());

//        qtlogProper.setProperty("log4j.appender.A1.layout", "org.apache.log4j.TTCCLayout");
//        qtlogProper.setProperty("log4j.appender.A1.layout.DateFormat", "ISO8601");
//        qtlogProper.setProperty("log4j.appender.A1.layout.categoryPrefixing","true");
//        qtlogProper.setProperty("log4j.appender.A1.layout.contextPrinting","true");
//        qtlogProper.setProperty("log4j.appender.A1.layout.threadPrinting","true");
    qtlogProper.setProperty("log4j.appender.A1.layout", "org.apache.log4j.PatternLayout");
    qtlogProper.setProperty("log4j.appender.A1.layout.ConversionPattern", "%d [%t] %p %c%x - %m%n");
    qtlogProper.setProperty("log4j.appender.A1.layout.header", "程序启动");
    qtlogProper.setProperty("log4j.appender.A1.layout.footer", "程序结束");

    Log4Qt::PropertyConfigurator::configure(qtlogProper);
//    Log4Qt::LogManager::setHandleQtMessages(true);
    Log4Qt::Logger* logger = Log4Qt::Logger::rootLogger();

    //LOG_INFO("==================================================================");
    QDesktopWidget* desktop = QApplication::desktop();
//    int screenW = desktop->width();
//    int screenH = desktop->height();
    int screenW = 1024;
    int screenH = 768;

    if(screenW > 1024)
        screenW = 1024;
    if(screenH > 768)
        screenH = 768;
    QResource::registerResource(QApplication::applicationDirPath()+"/resource.rcc");
    myFlow.initAll();
    struCnfg.screenH = screenH;

    paramDelayCode.getUiLanguage();
    g_myLan().ChangeLanguage();

    g_infoWidget().move((QApplication::desktop()->width()-g_infoWidget().width())/2, (screenH-g_infoWidget().height())/2);
    ZKSort w;
    w.setFixedSize(screenW, screenH);
    g_Runtime().appPositionX = (desktop->width() - w.width())/2;
    g_Runtime().appPositionY = (screenH - w.height())/2;
    w.move(0,0);
    w.show();

    int execResult = a.exec();
    if (splash){
        splash->finish(&w);
        delete splash;
        splash = NULL;
    }
    logger->removeAllAppenders();
    logger->loggerRepository()->shutdown();
    return execResult;
}
