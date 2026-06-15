#ifndef MYHTTPFILECLIENT_H
#define MYHTTPFILECLIENT_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QEventLoop>
#include <QNetworkConfiguration>
#include "data/myjson.h"
#include <QHttpMultiPart>
#include <QFileInfo>
#include <QSslConfiguration>
#include <QSslSocket>
#include "logger.h"
#include "globalflow.h"
#include "mqttsrv.h"

//
// HTTP Response Status Codes:
//

#define HTTP_STATUS_CONTINUE            100 // OK to continue with request
#define HTTP_STATUS_SWITCH_PROTOCOLS    101 // server has switched protocols in upgrade header

#define HTTP_STATUS_OK                  200 // request completed
#define HTTP_STATUS_CREATED             201 // object created, reason = new URI
#define HTTP_STATUS_ACCEPTED            202 // async completion (TBS)
#define HTTP_STATUS_PARTIAL             203 // partial completion
#define HTTP_STATUS_NO_CONTENT          204 // no info to return
#define HTTP_STATUS_RESET_CONTENT       205 // request completed, but clear form
#define HTTP_STATUS_PARTIAL_CONTENT     206 // partial GET furfilled

#define HTTP_STATUS_AMBIGUOUS           300 // server couldn't decide what to return
#define HTTP_STATUS_MOVED               301 // object permanently moved
#define HTTP_STATUS_REDIRECT            302 // object temporarily moved
#define HTTP_STATUS_REDIRECT_METHOD     303 // redirection w/ new access method
#define HTTP_STATUS_NOT_MODIFIED        304 // if-modified-since was not modified
#define HTTP_STATUS_USE_PROXY           305 // redirection to proxy, location header specifies proxy to use
#define HTTP_STATUS_REDIRECT_KEEP_VERB  307 // HTTP/1.1: keep same verb

#define HTTP_STATUS_BAD_REQUEST         400 // invalid syntax
#define HTTP_STATUS_DENIED              401 // access denied
#define HTTP_STATUS_PAYMENT_REQ         402 // payment required
#define HTTP_STATUS_FORBIDDEN           403 // request forbidden
#define HTTP_STATUS_NOT_FOUND           404 // object not found
#define HTTP_STATUS_BAD_METHOD          405 // method is not allowed
#define HTTP_STATUS_NONE_ACCEPTABLE     406 // no response acceptable to client found
#define HTTP_STATUS_PROXY_AUTH_REQ      407 // proxy authentication required
#define HTTP_STATUS_REQUEST_TIMEOUT     408 // server timed out waiting for request
#define HTTP_STATUS_CONFLICT            409 // user should resubmit with more info
#define HTTP_STATUS_GONE                410 // the resource is no longer available
#define HTTP_STATUS_LENGTH_REQUIRED     411 // the server refused to accept request w/o a length
#define HTTP_STATUS_PRECOND_FAILED      412 // precondition given in request failed
#define HTTP_STATUS_REQUEST_TOO_LARGE   413 // request entity was too large
#define HTTP_STATUS_URI_TOO_LONG        414 // request URI too long
#define HTTP_STATUS_UNSUPPORTED_MEDIA   415 // unsupported media type
#define HTTP_STATUS_RETRY_WITH          449 // retry after doing the appropriate action.

#define HTTP_STATUS_SERVER_ERROR        500 // internal server error
#define HTTP_STATUS_NOT_SUPPORTED       501 // required not supported
#define HTTP_STATUS_BAD_GATEWAY         502 // error response received from gateway
#define HTTP_STATUS_SERVICE_UNAVAIL     503 // temporarily overloaded
#define HTTP_STATUS_GATEWAY_TIMEOUT     504 // timed out waiting for gateway
#define HTTP_STATUS_VERSION_NOT_SUP     505 // HTTP version not supported

#define HTTP_STATUS_FIRST               HTTP_STATUS_CONTINUE
#define HTTP_STATUS_LAST                HTTP_STATUS_VERSION_NOT_SUP

#define TMPFILE     "/sdcard/tmp.zip"
#define MODEL_FILE_PATH "/sdcard/udisk/"
typedef struct UpdateUnit
{
    string m_uuid;
    string m_cmd;
    QString url;
    QString md5;
    QString ver;
    int force;
}UpdateUnit;
Q_DECLARE_METATYPE(UpdateUnit);


enum UpdateFileType {
    ZKSORT,
    IPCMODEL
};

class MyHttpFileClient:public QObject
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
public:
    MyHttpFileClient();

    bool downLoadFile(QString url, QString path );

    bool canWebsiteVisit(QString url);

    void requestToken(QString url);

    //注意，后面可能要根据每个usrid去获取或更需token
    void requestTokenUpdate(QString url);

    bool upLoadFile(QString url,QString path );

    bool upLoadData(QString url,string data);

    bool requestVPNCertificate(QString url);

    bool requestOssUploadSign(QString url,string data);

    bool upLoadOssFile(QString fileName);

    bool requestModelLableList(QString);

    bool requestModelList(QString urlpath,string data);



public:
    string  g_token;
    int     g_token_expires;
    QString g_connectName;
    QString fileUrl;

private:
    //oss
    string accessId;
    string signature;
    string dir;
    string host;
    string expire;

    string bucketName;
    string endpoint;
    string x_amz_date;
    string x_amz_signature;
    string x_amz_algorithm;
    string x_amz_credential;
    string key;
    string policy;
    string content_type;

};


class httpDownloadManager: public QObject
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

public:
    explicit httpDownloadManager(QObject *parent = 0);
    ~httpDownloadManager();
    void setDownloadInfo(QVariant m_unit);
    QString getDownloadUrl();
    void setBreakPointSupport(bool isBreakPointOk);
    void startDownload();
    void stopDownload();
    void stopWork();
    void closeDownload();
    void removeFile(QString filename);
    void reset();

public slots:
    void downloadFinished();
    void downloadReadyRead();
    void downloadProgress(qint64,qint64);
    void downloadError(QNetworkReply::NetworkError);
signals:
    void sigDownloadFinished(int stat);
    void sigDownloadProcess(qint64,qint64);
    void sigDownloadError();
private:
    QNetworkAccessManager* m_Manager;
    QNetworkRequest m_Request;
    QNetworkReply *m_Reply;
    UpdateUnit unit;
    QFile myfile;
    bool runflag;
    int m_bytesCurrentReceived;
    int m_bytesReceived;
    int m_bytesTotal;
    bool m_isBreakPointOk;
    bool m_isStop;
    bool m_isError;
};

extern MyHttpFileClient *myHttpFileClient;
//extern httpDownloadManager *myHttpDownloadManager;
#endif // MYHTTPFILECLIENT_H
