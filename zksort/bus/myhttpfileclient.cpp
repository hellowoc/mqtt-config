#include "myhttpfileclient.h"
#include <QStringList>
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
#include <QSettings>
#include "constant.h"
#include <QDomAttr>
#include <QDomElement>

#define MYHTTP 0
MyHttpFileClient *myHttpFileClient;

MyHttpFileClient::MyHttpFileClient()
{
    g_token = "";
    g_token_expires = 0;
    QSettings setting(CFG_APPSet,QSettings::IniFormat);
    g_connectName = setting.value("connectName","").toString();
    fileUrl = "";
}

bool MyHttpFileClient::downLoadFile(QString url, QString path)
{
    QNetworkAccessManager manage;
    QNetworkRequest request;
    QEventLoop eventLoop;
    bool ret = false;

    request.setUrl(QUrl(url));
    request.setRawHeader("User-Agent", "MyOwnBrowser 1.0");

    QObject::connect(&manage, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    QNetworkReply *reply = manage.get(request);

    QStringList list = path.split("/");
    QString filename = list.at(list.length() -1);

    QFile myfile(filename);
    myfile.open(QIODevice::WriteOnly|QIODevice::Truncate);

    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

    if( reply->error() == QNetworkReply::NoError)
    {
        QByteArray ba = reply->readAll();
//        qDebug()<<QString(ba);
        myfile.write(ba);
        myfile.close();
        qDebug()<<"file download ok!";
        ret = true;
    }
    else
    {
        int res = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
        /* statusCode是HTTP服务器的相应码 */
//        qDebug()<< "status code: " << res;
        ret = false;
    }

    reply->deleteLater();
    return ret;
}

bool MyHttpFileClient::canWebsiteVisit(QString url)
{
    QNetworkAccessManager manage;
    QNetworkRequest request;
    QEventLoop eventLoop;
    bool ret = false;

    QTimer timer;
    timer.setSingleShot(true);

    QObject::connect(&timer, SIGNAL(timeout()), &eventLoop, SLOT(quit()));
    QObject::connect(&manage, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    timer.start(5000);
    request.setUrl(QUrl(url));

    QNetworkReply *reply = manage.get(request);

    qDebug()<<"visit "<<url ;
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

    if (timer.isActive())
    {
        timer.stop();
        int res = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();

        if( res == HTTP_STATUS_CREATED || res == HTTP_STATUS_OK)
        {
            ret = true;
        }
        else
        {
            ret = false;
        }
//        qDebug()<< "status code: " << res;

        reply->deleteLater();
        return ret;
    }
    else {
        qDebug()<< "http request timeout";

        QObject::disconnect(&manage, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
        reply->deleteLater();
    }

}

void MyHttpFileClient::requestToken(QString url)
{
    QByteArray array;
    array.append("grant_type=client_credentials");
    array.append("&scope=server");
    array.append("&code=xiewj-test");
    array.append("&model=RZ+8-64X");
    array.append("&version=1.1.0");

    QNetworkAccessManager manage;
    QNetworkRequest request;
    QEventLoop eventLoop;

    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));
    request.setRawHeader("Authorization","Basic YXBpOmFwaQ==");
    request.setRawHeader("client_id","7BIp1rTdiM+ylTb6gAlvbg==");
    request.setRawHeader("client_secret","7BIp1rTdiM+ylTb6gAlvbg==");
    request.setRawHeader("TENANT_ID","1");
#if MYHTTP
    QSslConfiguration config = request.sslConfiguration();
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    #if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
        config.setProtocol(QSsl::TlsV1_0);
    #else
        config.setProtocol(QSsl::TlsV1);
    #endif
        request.setSslConfiguration(config);
#endif
    QObject::connect(&manage, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    QNetworkReply *reply = manage.post(request,array);

    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

    if( reply->error() != QNetworkReply::NoError)
    {
//        qDebug()<<reply->errorString();
    }
    else
    {
        QByteArray ba = reply->readAll();
        string rcv = QString(ba).toStdString();

        Json::Reader reader;
        Json::Value  root;
        //{"access_token":"771ee5f4-1fc7-46c1-9583-36bddb551ceb","token_type":"bearer","expires_in":36134,"scope":"server"}
        if (reader.parse(rcv, root))
        {
           Json::Value root_code= root["access_token"];
           g_token = root_code.asString();
           root_code = root["token_type"];
           string g_token_type =root_code.asString();
           root_code = root["expires_in"];
           g_token_expires =root_code.asInt();

           qDebug()<<"g_token"<<QString().fromStdString(g_token)
                   <<"g_token_type"<<QString().fromStdString(g_token_type)
                   <<"g_token_expires"<<g_token_expires;
        }

    }
    reply->deleteLater();

 }

void MyHttpFileClient::requestTokenUpdate(QString url)
{
    QByteArray array;

//    QString username = "18093I040-005";
    QString username = "mobile=DEVICE@"+g_connectName;
    array.append(username);

//    array.append("mobile=DEVICE@18093I040-005");
//    QByteArray array("{\"mobile\":\"DEVICE@18093I040-005\"}");

    QNetworkAccessManager manage;
    QNetworkRequest request;
    QEventLoop eventLoop;
    QTimer timer;

    QSettings setting(CFG_APPSet,QSettings::IniFormat);

    QString testip = setting.value("testIp","cloud.chinaamd.cn").toString();

    request.setUrl(QUrl(HTTPS+testip+url));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));

//    request.setRawHeader("Authorization","Basic U29ydGVyUmVtb3RIOmZsdXR0ZXJBcHA=");
    request.setRawHeader("Authorization","Basic YXBpOmFwaQ==");
    request.setRawHeader("TENANT_ID","1");

// pc test need no ssl certificate
#if MYHTTP
    QSslConfiguration config = request.sslConfiguration();
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    #if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
        config.setProtocol(QSsl::TlsV1_0);
    #else
        config.setProtocol(QSsl::TlsV1);
    #endif
        request.setSslConfiguration(config);
#endif

    timer.setSingleShot(true);
    QObject::connect(&timer, SIGNAL(timeout()), &eventLoop, SLOT(quit()));
    QObject::connect(&manage, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    QNetworkReply *reply = manage.post(request,array);
    timer.start(10000);
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
    if(timer.isActive()) {
        timer.stop();
    } else {
        logger()->info("requestToken timeout");
        QObject::disconnect(&manage, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
        reply->abort();
        reply->close();
        reply->deleteLater();
        return ;
    }

    int res = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();

//    qDebug()<<"http status "<<res;


    if( reply->error() != QNetworkReply::NoError)
    {
//        qDebug()<<reply->errorString();
        logger()->info("requesttoken :%1-%2",res,reply->errorString());
    }
    else
    {
        QByteArray ba = reply->readAll();
        string rcv = QString(ba).toStdString();

        Json::Reader reader;
        Json::Value  root;
        //{"access_token":"771ee5f4-1fc7-46c1-9583-36bddb551ceb","token_type":"bearer","expires_in":36134,"scope":"server"}
        if (reader.parse(rcv, root))
        {
           Json::Value root_code= root["access_token"];
           g_token = root_code.asString();
           root_code = root["token_type"];
           string g_token_type =root_code.asString();
           root_code = root["expires_in"];
           g_token_expires =root_code.asInt();

           qDebug()<<"g_token"<<QString().fromStdString(g_token)
                   <<"g_token_type"<<QString().fromStdString(g_token_type)
                   <<"g_token_expires"<<g_token_expires;
        }

    }
    reply->deleteLater();

 }

//upLoadFile("https://www.chinaamd.cn:8900/api/operatelog/file/upload/RG/xiewj-test","/sdcard/logs/log.txt");
//upLoadFile("https://www.chinaamd.cn:8900/api/operatelog/file/upload/RG/xiewj-test/20200201","./2020-02-01.log");
//upLoadFile("https://www.chinaamd.cn:8900/api/runparam/file/upload/RG/xiewj-test","/sdcard/cnf/");

bool MyHttpFileClient::upLoadFile(QString url, QString path)
{
//    g_token = "507e65d1-d1b3-416c-87e9-337d724b4e1a";
    bool ret = 0;
    QEventLoop eventLoop;
    QFileInfo info(path);
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/octet-stream"));
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file\"; filename=\""+info.fileName()+"\""));
    QFile *file = new QFile(path);
    file->open(QIODevice::ReadOnly);
    filePart.setBodyDevice(file);
    file->setParent(multiPart);
    multiPart->append(filePart);

    QNetworkRequest request;
    QNetworkAccessManager manager;

    QSettings setting(CFG_APPSet,QSettings::IniFormat);
    QString testip = setting.value("testIp","cloud.chinaamd.cn").toString();

    request.setUrl(QUrl(HTTPS+testip+url));
    QString str = QString("Bearer")+QString().fromStdString(g_token);
    QByteArray auth = QString(str).toLocal8Bit();

//    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));
//    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    request.setRawHeader("Authorization",auth);
    request.setRawHeader("TENANT_ID","1");
#if MYHTTP
    QSslConfiguration config = request.sslConfiguration();
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    #if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
        config.setProtocol(QSsl::TlsV1_0);
    #else
        config.setProtocol(QSsl::TlsV1);
    #endif
        request.setSslConfiguration(config);
#endif
    QObject::connect(&manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    QNetworkReply *reply = manager.post(request, multiPart);
    multiPart->setParent(reply);

    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

    int res = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();

    if( reply->error() != QNetworkReply::NoError)
    {
        qDebug()<<"error "<< res <<reply->errorString();
        logger()->info("upLoadFile: %1",reply->errorString());
        ret = 0;
    }
    else
    {
        QByteArray ba = reply->readAll();
        qDebug()<<QString(ba);
        ret = 1;
    }
    reply->deleteLater();
    return ret;

}

bool MyHttpFileClient::upLoadData(QString url,string data)
{
//    g_token = "507e65d1-d1b3-416c-87e9-337d724b4e1a";
    bool ret = 0;
    QEventLoop eventLoop;

    QNetworkRequest request;
    QNetworkAccessManager manager;
    QTimer timer;

    QSettings setting(CFG_APPSet,QSettings::IniFormat);
    QString testip = setting.value("testIp","cloud.chinaamd.cn").toString();

    request.setUrl(QUrl(HTTPS+testip+url));
    QString str = QString("Bearer")+QString().fromStdString(g_token);
    QByteArray auth = QString(str).toLocal8Bit();

    QByteArray updata = QString().fromStdString(data).toLocal8Bit();

//    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    request.setRawHeader("Authorization",auth);
    request.setRawHeader("TENANT_ID","1");
#if MYHTTP
    QSslConfiguration config = request.sslConfiguration();
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
    config.setProtocol(QSsl::TlsV1_0);
#else
    config.setProtocol(QSsl::TlsV1);
#endif
    request.setSslConfiguration(config);
#endif

    timer.setSingleShot(true);

    QObject::connect(&timer, SIGNAL(timeout()), &eventLoop, SLOT(quit()));
    QObject::connect(&manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    QNetworkReply *reply = manager.post(request, updata);

    timer.start(10000);   // 10 secs. timeout

    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
    if(timer.isActive()) {
        timer.stop();
    } else {
        logger()->info("upLoadData timeout");
        QObject::disconnect(&manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
        reply->abort();
        reply->close();
        reply->deleteLater();
        return 0;
    }
    int res = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();

    if( reply->error() != QNetworkReply::NoError)
    {
        qDebug()<<"error "<< res <<reply->errorString();
        logger()->info("upLoadData :%1-%2",res,reply->errorString());
        ret = 0;
    }
    else
    {
        QByteArray ba = reply->readAll();
        qDebug()<<request.url().toString();
        qDebug()<<QString(ba)<<"\n";
        ret = 1;
    }
    reply->deleteLater();
    return ret;
}

bool MyHttpFileClient::requestVPNCertificate(QString url)
{
    //    QByteArray array("{\"username\":\"xwj\"}");
    int ret = 0;
    QString tmpName = g_connectName.replace("-","_");
    QString username = "{\"username\":\""+tmpName+"\"}";

    qDebug()<<"username"<<username;
    char* name = QString(username).toLocal8Bit().data();
    QByteArray array(name);

    QNetworkAccessManager manage;
    QNetworkRequest request;
    QEventLoop eventLoop;
    QTimer timer;

    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    //    request.setRawHeader("Content-Type","application/json");

    // pc test need no ssl certificate
#if MYHTTP
    QSslConfiguration config = request.sslConfiguration();
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
    config.setProtocol(QSsl::TlsV1_0);
#else
    config.setProtocol(QSsl::TlsV1);
#endif
    request.setSslConfiguration(config);
#endif

    timer.setSingleShot(true);

    QObject::connect(&timer, SIGNAL(timeout()), &eventLoop, SLOT(quit()));
    QObject::connect(&manage, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    QNetworkReply *reply = manage.post(request,array);

    timer.start(10000);   // 10 secs. timeout
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

    if(timer.isActive()) {
        timer.stop();
    } else {
        logger()->info("requestvpncert timeout");
        QObject::disconnect(&manage, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
        reply->abort();
        reply->close();
        reply->deleteLater();
        return 0;
    }
    int res = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();

    qDebug()<<"http status "<<res;

    if( reply->error() != QNetworkReply::NoError)
    {
        qDebug()<<reply->errorString();
        logger()->info("requestvpncert :%1-%2",res,reply->errorString());
        ret = 0;
    }
    else
    {
        QByteArray ba = reply->readAll();
        string rcv = QString(ba).toStdString();

        Json::Reader reader;
        Json::Value  root;
        //{"client_config":"# this ovpn file is automatically generated by [OVPM]...."}
        if (reader.parse(rcv, root))
        {
            Json::Value root_cert= root["client_config"];
            string s_cert =root_cert.asString();

            QString str = QString().fromStdString(s_cert).replace("/\n/g", "\\\\n");
            //           CMyJson::SaveJsonFile("/app/openvpn/zkcert.ovpn",root_cert);

            QFile myfile(OPENVPN_CERT_PATH);
            if (!myfile.open(QIODevice::WriteOnly | QIODevice::Text))
                return 0;

            myfile.write(str.toLocal8Bit(),str.length());
            system("sync");
            myfile.close();
            ret = 1;
        }

    }
    reply->deleteLater();
    return ret;

}
/* 通过中科服务器获取通往阿里云的证书及路径 */
bool MyHttpFileClient::requestOssUploadSign(QString url,string data)
{
    bool ret = 0;
    QEventLoop eventLoop;

    QNetworkRequest request;
    QNetworkAccessManager manager;

    QSettings setting(CFG_APPSet,QSettings::IniFormat);
    QString testip = setting.value("testIp","cloud.chinaamd.cn").toString();

    request.setUrl(QUrl(HTTPS+testip+url));
    QString str = QString("Bearer")+QString().fromStdString(g_token);
    QByteArray auth = QString(str).toLocal8Bit();

    QByteArray updata = QString().fromStdString(data).toLocal8Bit();

    //    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    request.setRawHeader("Authorization",auth);
    request.setRawHeader("TENANT_ID","1");
#if MYHTTP
    QSslConfiguration config = request.sslConfiguration();
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
    config.setProtocol(QSsl::TlsV1_0);
#else
    config.setProtocol(QSsl::TlsV1);
#endif
    request.setSslConfiguration(config);
#endif
    QObject::connect(&manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    QNetworkReply *reply = manager.post(request, updata);

    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

    int res = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();

    if( reply->error() != QNetworkReply::NoError)
    {
        qDebug()<<"error "<< res <<reply->errorString();
        logger()->info("requestosssign :%1-%2",res,reply->errorString());
        ret = 0;
    }
    else
    {
        QByteArray ba = reply->readAll();
        string rcv = QString(ba).toStdString();
        qDebug()<<"osssign : "<<QString(ba);
        Json::Reader reader;
        Json::Value  root;
        //"{"accessId":"LTAI4GCQZwRnNJt4cbizMKRx","policy":"eyJleHBpcmF0aW9uIjoiMjAyMC0xMS0yOFQwNjoxMToxNi42MTBaIiwiY29uZGl0aW9ucyI6W1siY29udGVudC1sZW5ndGgtcmFuZ2UiLDAsMTA0ODU3NjAwMF0sWyJzdGFydHMtd2l0aCIsIiRrZXkiLCJ1cGxvYWQiXV19","signature":"z7/529YbfclTmFLiUuSNBLaaC+I=","dir":"upload/DEVICE_IMG/MATERIAL/","host":"oss-cn-qingdao.aliyuncs.com","expire":"1606543876","bucketName":"sorter-ai"}
        if (reader.parse(rcv, root))
        {
//           Json::Value root_code= root["accessId"];
//           accessId = root_code.asString();
//           root_code = root["policy"];
//           policy =root_code.asString();
//           root_code = root["signature"];
//           signature =root_code.asString();
//           root_code = root["dir"];
//           dir = root_code.asString();
//           root_code = root["host"];
//           host =root_code.asString();
//           root_code = root["expire"];
//           expire =root_code.asString();
//           root_code = root["bucketName"];
//           bucketName =root_code.asString();

            bucketName          = root["data"]["bucketName"].asString();
            endpoint            = root["data"]["endpoint"].asString();
            x_amz_date          = root["data"]["x-amz-date"].asString();
            x_amz_signature     = root["data"]["x-amz-signature"].asString();
            x_amz_algorithm     = root["data"]["x-amz-algorithm"].asString();
            x_amz_credential    = root["data"]["x-amz-credential"].asString();
            key                 = root["data"]["key"].asString();
            policy              = root["data"]["policy"].asString();
            content_type        = root["data"]["Content-Type"].asString();
//            qDebug()<<"\n--------------------------------------------------------------------";
//            qDebug()<<"bucketName = "<<QString::fromStdString(bucketName);
//            qDebug()<<"endpoint = "<<QString::fromStdString(endpoint);
//            qDebug()<<"x_amz_date = "<<QString::fromStdString(x_amz_date);
//            qDebug()<<"x_amz_signature = "<<QString::fromStdString(x_amz_signature);
//            qDebug()<<"x_amz_algorithm = "<<QString::fromStdString(x_amz_algorithm);
//            qDebug()<<"x_amz_credential = "<<QString::fromStdString(x_amz_credential);
//            qDebug()<<"key = "<<QString::fromStdString(key);
//            qDebug()<<"policy = "<<QString::fromStdString(policy);
//            qDebug()<<"content_type = "<<QString::fromStdString(content_type);
//            qDebug()<<"--------------------------------------------------------------------\n";

        }
        ret = 1;
    }
    reply->deleteLater();
    return ret;
}

/* 上传到阿里云 */
bool MyHttpFileClient::upLoadOssFile(QString fileName)
{
    bool ret = 0;
    QEventLoop eventLoop;
    QTimer timer;
    QFileInfo info(fileName);
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    qDebug()<<info.fileName();
    QHttpPart textPart_key;
    textPart_key.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"key\""));
    textPart_key.setBody(QString::fromStdString(key).toLatin1());

    QHttpPart textPart_policy;
    textPart_policy.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"policy\""));
    textPart_policy.setBody(QString::fromStdString(policy).toLatin1());

    QHttpPart textPart_success_action_status;
    textPart_success_action_status.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"success_action_status\""));
    textPart_success_action_status.setBody("201");

    QHttpPart textPart_date;
    textPart_date.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"x-amz-date\""));
    textPart_date.setBody(QString::fromStdString(x_amz_date).toLatin1());

    QHttpPart textPart_signature;
    textPart_signature.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"x-amz-signature\""));
    textPart_signature.setBody(QString::fromStdString(x_amz_signature).toLatin1());

    QHttpPart textPart_algorithm;
    textPart_algorithm.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"x-amz-algorithm\""));
    textPart_algorithm.setBody(QString::fromStdString(x_amz_algorithm).toLatin1());

    QHttpPart textPart_credential;
    textPart_credential.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"x-amz-credential\""));
    textPart_credential.setBody(QString::fromStdString(x_amz_credential).toLatin1());

    QHttpPart textPart_contenttype;
    textPart_contenttype.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"Content-Type\""));
    textPart_contenttype.setBody(QString::fromStdString(content_type).toLatin1());

    QHttpPart filePart;
    //    filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/octet-stream"));
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file\"; filename=\""+info.fileName()+"\""));
    QFile *file = new QFile(fileName);
    file->open(QIODevice::ReadOnly);
    filePart.setBodyDevice(file);
    file->setParent(multiPart);

    multiPart->append(textPart_key);
    multiPart->append(textPart_policy);
    multiPart->append(textPart_success_action_status);
    multiPart->append(textPart_date);
    multiPart->append(textPart_signature);
    multiPart->append(textPart_algorithm);
    multiPart->append(textPart_credential);
    multiPart->append(textPart_contenttype);
    multiPart->append(filePart);

    QNetworkRequest request;
    QNetworkAccessManager manager;

    // 阿里云地址需要通过中科服务端提前获取拼接
    request.setUrl(QUrl(QString::fromStdString(endpoint) + "/" +QString::fromStdString(bucketName)));
//    request.setUrl(QUrl(HTTPS+QString::fromStdString(bucketName)+"."+QString::fromStdString(host)));

#if MYHTTP
    QSslConfiguration config = request.sslConfiguration();
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
    config.setProtocol(QSsl::TlsV1_0);
#else
    config.setProtocol(QSsl::TlsV1);
#endif
    request.setSslConfiguration(config);
#endif

    //这个头尤其重要！！！
//    request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data;boundary=" + multiPart->boundary());

    timer.setSingleShot(true);

    QObject::connect(&timer, SIGNAL(timeout()), &eventLoop, SLOT(quit()));
    QObject::connect(&manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    QNetworkReply *reply = manager.post(request, multiPart);
    multiPart->setParent(reply);
    timer.start(60000);   // 10 secs. timeout

    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
    if(timer.isActive()) {
        timer.stop();
    } else {
        QObject::disconnect(&manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
        logger()->info("upLoadOssFile timeout");
        reply->abort();
        reply->close();
        reply->deleteLater();
        return 0;
    }

    int res = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();

    if( reply->error() != QNetworkReply::NoError)
    {
        logger()->info("upLoadOssFile:%1-%2",res,reply->errorString());
        qDebug()<<"error "<< res <<reply->errorString();
        ret = 0;
    }
    else
    {
        QByteArray ba = reply->readAll();
        //阿里云回包为xml格式，先存储后读取解析
        QFile data("/sdcard/ts/res.xml");
        data.open(QIODevice::WriteOnly|QIODevice::Text);
        QTextStream ofile(&data);
        ofile<<QString(ba);
        data.close();

        QDomDocument doc;
        data.open(QIODevice::ReadOnly|QIODevice::Text);
        doc.setContent(&data);
        data.close();

//        <PostResponse>
//            <Bucket>sorter-ai</Bucket>
//            <Location>
//                https://sorter-ai.oss-cn-qingdao.aliyuncs.com//upload/DEVICE_IMG/MATERIAL/xiewj-test-2/test.png
//            </Location>
//            <Key>upload/DEVICE_IMG/MATERIAL/xiewj-test-2/test.png</Key>
//            <ETag>CD379AC1F9DC310470E820EE068D9260</ETag>
//        </PostResponse>
        QDomElement docElem = doc.documentElement();
        QDomNode n = docElem.firstChild();
        while(!n.isNull()){
            QDomElement nElem = n.toElement();
            if(nElem.nodeName() =="Location"){
                fileUrl = nElem.text();
                break;
            }
            n = n.nextSibling();
        }
        qDebug()<<fileUrl;
        if (fileUrl.contains("http"))
            ret = 1;
        else
            ret = 0;
    }
    reply->deleteLater();
    return ret;
}



/* 获取模型标签列表 */
bool MyHttpFileClient::requestModelLableList(QString urlpath)
{

    QTimer timer;
    // 创建网络请求
    QNetworkRequest request;
    QNetworkAccessManager manager;
    QEventLoop eventLoop;
    QString str = QString("Bearer")+QString().fromStdString(g_token);
    QByteArray auth = QString(str).toLocal8Bit();

    QSettings setting(CFG_APPSet,QSettings::IniFormat);
    QString testip = setting.value("testIp","cloud.chinaamd.cn").toString();

    request.setUrl(QUrl(HTTPS+testip+urlpath));
    request.setRawHeader("Authorization",auth);
    request.setRawHeader("TENANT_ID","1");

#if MYHTTP
    QSslConfiguration config = request.sslConfiguration();
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
    config.setProtocol(QSsl::TlsV1_0);
#else
    config.setProtocol(QSsl::TlsV1);
#endif
    request.setSslConfiguration(config);
#endif

    timer.setSingleShot(true);

    QObject::connect(&timer, SIGNAL(timeout()), &eventLoop, SLOT(quit()));
    QObject::connect(&manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    // 发送 GET 请求
    QNetworkReply *reply = manager.get(request);

    timer.start(10000);   // 10 secs. timeout

    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
    if(timer.isActive()) {
        timer.stop();
    } else {
        QObject::disconnect(&manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

        reply->abort();
        reply->close();
        reply->deleteLater();
        return 0;
    }

    if( reply->error() != QNetworkReply::NoError)
    {
        qDebug()<<"error" <<reply->errorString();

    }
    else
    {
        QByteArray ba = reply->readAll();

        // qDebug()<<QString(ba);

        string rcv = QString(ba).toStdString();
        Json::Reader reader;
        Json::Value  root;
        if (reader.parse(rcv, root))
        {

            if (root["code"].isInt() && root["code"].asInt() == 0) {
                Json::Value datas = root["data"];


                struCnfg.stumodeLableList.labelnum = datas.size();

                for (unsigned int i = 0; i < datas.size(); i++)
                {
                    Json::Value row = datas[i];
                    if (row["value"].isString())
                    {
                       // qDebug()<<QString::fromStdString(row["value"].asString());

                        QString biglabelname = QString::fromStdString(row["value"].asString());
                        QByteArray namearr = biglabelname.toUtf8();
                        const char *strName = namearr.data();
                        memcpy(struCnfg.stumodeLableList.labelname[i],strName,strlen(strName));

                        Json::Value labels =  row["labels"];
                        struCnfg.stumodeLableList.labelinfo[i].labelnum = labels.size();


                        for (unsigned int j = 0; j < labels.size(); j++)
                        {
                            Json::Value label = labels[j];

                            QString labelname = QString::fromStdString(label["value"].asString());
                            QByteArray namearr = labelname.toUtf8();
                            const char *strName1 = namearr.data();
                            memcpy(struCnfg.stumodeLableList.labelinfo[i].labelname[j],strName1,strlen(strName1));

                            labelname = QString::fromStdString(label["labelType"].asString());
                            namearr = labelname.toUtf8();
                            const char *strName2 = namearr.data();
                            memcpy(struCnfg.stumodeLableList.labelinfo[i].labeltype[j],strName2,strlen(strName2));

                            labelname = QString::fromStdString(label["id"].asString());
                            namearr = labelname.toUtf8();
                            const char *strName3 = namearr.data();
                            memcpy(struCnfg.stumodeLableList.labelinfo[i].labelID[j],strName3,strlen(strName3));


                           // qDebug()<<QString::fromStdString(label["value"].asString());
                          //  qDebug()<<QString::fromStdString(label["labelType"].asString() );

                        }


                    }

                }
            }
        } else {
            qDebug() << "Failed to parse JSON data";

        }
    }


    //  解析数据


    reply->deleteLater();
    return 0;
}


// 获取模型列表
bool MyHttpFileClient::requestModelList(QString urlpath,string data)
{

    QTimer timer;
    // 创建网络请求
    QNetworkRequest request;
    QNetworkAccessManager manager;
    QEventLoop eventLoop;
    QString str = QString("Bearer")+QString().fromStdString(g_token);
    QByteArray auth = QString(str).toLocal8Bit();
    QByteArray array = QString::fromStdString(data).toLocal8Bit();

    QSettings setting(CFG_APPSet,QSettings::IniFormat);
    QString testip = setting.value("testIp","cloud.chinaamd.cn").toString();

    request.setUrl(QUrl(HTTPS+testip+urlpath));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    request.setRawHeader("Authorization",auth);
    request.setRawHeader("TENANT_ID","1");


#if MYHTTP
    QSslConfiguration config = request.sslConfiguration();
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
    config.setProtocol(QSsl::TlsV1_0);
#else
    config.setProtocol(QSsl::TlsV1);
#endif
    request.setSslConfiguration(config);
#endif

    timer.setSingleShot(true);

    QObject::connect(&timer, SIGNAL(timeout()), &eventLoop, SLOT(quit()));
    QObject::connect(&manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    // 发送 GET 请求
    // QNetworkReply *reply = manager.get(request);

    QNetworkReply *reply = manager.post(request,array);

    timer.start(10000);   // 10 secs. timeout

    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
    if(timer.isActive()) {
        timer.stop();
    } else {
        QObject::disconnect(&manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

        reply->abort();
        reply->close();
        reply->deleteLater();

        qDebug()<<"request timeout " ;
        return false;
    }

    if( reply->error() != QNetworkReply::NoError)
    {
        qDebug()<<"error" <<reply->errorString();
        reply->deleteLater();
        return false;

    }
    else
    {
        QByteArray ba = reply->readAll();

       // qDebug()<<QString(ba);

        string rcv = QString(ba).toStdString();
        Json::Reader reader;
        Json::Value  root;
        if (reader.parse(rcv, root))
        {

            if (root["code"].isInt() && root["code"].asInt() == 0) {
                Json::Value datas = root["data"];


                struGsh.stumodelData.totalmodelnum = datas["total"].asInt();

             //   qDebug()<<struGsh.stumodelData.totalmodelnum;
                Json::Value records = datas["records"];
                struGsh.stumodelData.currentmodelnum =  records.size();
                for (unsigned int i = 0; i < records.size(); i++)
                {
                    Json::Value row = records[i];
                    if (row["modelName"].isString())
                    {

                        QString name = QString::fromStdString(row["modelName"].asString());
                        QByteArray namearr = name.toUtf8();
                        const char *strName = namearr.data();
                        memcpy(struGsh.stumodelData.modelname[i],strName,strlen(strName));

                        QString path = QString::fromStdString(row["modelPath"].asString());
                        path = "database" + path.split("database").at(1);
                        path = path.replace('\\','/');
                        namearr = path.toUtf8();
                        const char *strName1 = namearr.data();
                        memcpy(struGsh.stumodelData.modelpath[i],strName1,strlen(strName1));

                        QString instruct = QString::fromStdString(row["summary"].asString());
                       // qDebug()<<instruct;
                        namearr = instruct.toUtf8();
                        const char *strName2 = namearr.data();
                        memcpy(struGsh.stumodelData.modelinstruct[i],strName2,strlen(strName2));

                    }

                }
            }
        }

        struGsh.updateModelistresult = 1;

        reply->deleteLater();
        return true;
    }

    reply->deleteLater();
    return true;



}
/*********************************** HttpDownload Manager ************************************/
httpDownloadManager::httpDownloadManager(QObject *parent):QObject(parent)
{
    m_Manager = new QNetworkAccessManager(this);
    m_bytesCurrentReceived = 0;
    m_bytesReceived = 0;
    m_bytesTotal = 0;

    m_isBreakPointOk = true;
    m_isStop = true;
    m_isError = false;

    m_Reply = 0;
    runflag = false;
}

httpDownloadManager::~httpDownloadManager()
{

}
void httpDownloadManager::setDownloadInfo(QVariant m_unit)
{
    unit = QVariant(m_unit).value<UpdateUnit>();
}
QString httpDownloadManager::getDownloadUrl()
{
    return unit.url;
}
void httpDownloadManager::startDownload()
{
    if (m_isStop){
        m_isStop = false;
        // 非断点续传的直接删
        if (m_bytesCurrentReceived <= 0)
        {
            removeFile(TMPFILE);
            removeFile(MODEL_FILE_PATH+QString("%1").arg(struGsh.stumodelData.modelpath[struGsh.stumodelData.checkindex]));

        }

        if( struGsh.selectDownloadModel == 1)
                {
                    myfile.setFileName(MODEL_FILE_PATH+QString("%1").arg(struGsh.stumodelData.modelname[struGsh.stumodelData.checkindex]));
                    myfile.open(QIODevice::WriteOnly|QIODevice::Append);
                    m_Request.setUrl(unit.url);
                    qDebug()<<"download url"<<unit.url;
                    QString str = QString("Bearer")+QString().fromStdString(myHttpFileClient->g_token);
                    QByteArray auth = QString(str).toLocal8Bit();
                    m_Request.setRawHeader("Authorization",auth);
                    m_Request.setRawHeader("TENANT_ID","1");
        #if MYHTTP
                    QSslConfiguration config = m_Request.sslConfiguration();
                    config.setPeerVerifyMode(QSslSocket::VerifyNone);
        #if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
                    config.setProtocol(QSsl::TlsV1_0);
        #else
                    config.setProtocol(QSsl::TlsV1);
        #endif
                    m_Request.setSslConfiguration(config);
        #endif
                }
        else
        {
            myfile.setFileName(TMPFILE);
            myfile.open(QIODevice::WriteOnly|QIODevice::Append);

            m_Request.setUrl(QUrl::fromEncoded(unit.url.toLatin1()));
        }


    //    m_Request.setRawHeader("User-Agent", "MyOwnBrowser 1.0");
        //支持断点续传
        if (m_isBreakPointOk){
            QString strRange = QString("bytes=%1-").arg(m_bytesCurrentReceived);
            m_Request.setRawHeader("Range", strRange.toLatin1());
        }

        m_Reply = m_Manager->get(m_Request);
        connect(m_Reply, SIGNAL(finished()),this, SLOT(downloadFinished()));
        connect(m_Reply, SIGNAL(readyRead()), this, SLOT(downloadReadyRead()));
        connect(m_Reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(downloadProgress(qint64,qint64)));
        connect(m_Reply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(downloadError(QNetworkReply::NetworkError)));
    }

}
void httpDownloadManager::downloadFinished()
{
    myfile.flush();
    myfile.close();

    m_isStop = true;
    QVariant statusCode = m_Reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    int res = NOT_EXIST;
    if (m_Reply->error() == QNetworkReply::NoError)    {
        QFileInfo fileInfo(TMPFILE);
        if (fileInfo.exists() && g_Runtime().getFileMd5(TMPFILE) == QString(unit.md5).toLatin1().toUpper()){
            res = SUCCESS;
        }
        if( struGsh.selectDownloadModel == 1)
                res = SUCCESS;
    }
    else{
        res = FILEERROR;
        qDebug() << "error3_"<<m_Reply->errorString()<<statusCode.toFloat();
    }

    emit sigDownloadFinished(res);
    m_Reply->deleteLater();
    m_Reply = 0;

}
void httpDownloadManager::downloadReadyRead()
{
    if (!m_isStop){
        //将读到的信息写入文件
        myfile.write(m_Reply->readAll());
    }
}
void httpDownloadManager::downloadProgress(qint64 bytesReceived,qint64 bytesTotal)
{
    if (!m_isStop){

        m_bytesReceived = bytesReceived;
        m_bytesTotal = bytesTotal;
        // 更新下载进度;(加上 m_bytesCurrentReceived 是为了断点续传时之前下载的字节)
        emit sigDownloadProcess(m_bytesReceived + m_bytesCurrentReceived, m_bytesTotal + m_bytesCurrentReceived);
    }
}
void httpDownloadManager::downloadError(QNetworkReply::NetworkError)
{
    m_isError = true;
    QString strError = m_Reply->errorString();
    qDebug() << "error2__________" + strError<<m_Reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toFloat();

    stopDownload();
    emit sigDownloadError();
    m_isError = false;
}
void httpDownloadManager::stopDownload()
{
//    testcode
//    if (m_Reply !=0){
//        QString strAcceptRange = QString(m_Reply->rawHeader("Accept-Ranges"));
//        if (strAcceptRange != "bytes")
//        { qDebug() << "服务端不支持断点续传!"; }
//        else
//        { qDebug() << "支持~"; }
//    }

    if (!m_isStop)
    {
        stopWork();
        //更新当前已经下载字节数
        QFileInfo fileinfo(TMPFILE);
        if (fileinfo.exists())
            m_bytesCurrentReceived = fileinfo.size();
    }
}
void httpDownloadManager::stopWork()
{
    m_isStop = true;
    if (m_Reply != NULL)
    {
        disconnect(m_Reply, SIGNAL(finished()),this, SLOT(downloadFinished()));
        disconnect(m_Reply, SIGNAL(readyRead()), this, SLOT(downloadReadyRead()));
        disconnect(m_Reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(downloadProgress(qint64,qint64)));
        disconnect(m_Reply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(downloadError(QNetworkReply::NetworkError)));

        if (!m_isError)
            m_Reply->abort();
        m_Reply->deleteLater();
        m_Reply = NULL;
        myfile.flush();
        myfile.close();
    }
}

// 停止下载按钮被按下，关闭下载，重置参数，并删除下载的临时文件;
void httpDownloadManager::closeDownload()
{
    stopWork();
    reset();
//    removeFile(TMPFILE);
}
// 删除临时文件
void httpDownloadManager::removeFile(QString filename)
{
    QFileInfo file(filename);
    if (file.exists())
    {
        QFile::remove(filename);
    }
}
// 重置参数
void httpDownloadManager::reset()
{
    m_bytesCurrentReceived = 0;
    m_bytesReceived = 0;
    m_bytesTotal = 0;
}
