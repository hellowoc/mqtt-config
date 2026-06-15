#ifndef IPSETWIDGET_H
#define IPSETWIDGET_H

#include "basewidget.h"

namespace Ui {
class IPSetWidget;
}

static QString virtualAddr= "";

/* ping线程:运行时每5s发送一次ping命令,判断服务器是否有效 */
class pingThread : public QThread
{
    Q_OBJECT
public:
    explicit pingThread();
    bool serverIsAlive(void);

protected:
    void run(void);

signals:
    void pingFailSig();

public slots:
    void stopPing(void);

private:
    bool m_bIsRunning;
    QProcess *m_cmd;
};


class IPSetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit IPSetWidget(QWidget *parent = 0);
    ~IPSetWidget();
    void showPage(bool bshow);
    void retranslateUiWidget();
    bool getLocalVirtualAddr(void);                 // 获取本地虚拟网络地址
    bool connetToServer(void);                      // 判断屏连接服务器是否成功
private slots:
    void on_okBtn_clicked();
    void on_checkBox_clicked(bool checked);

    void on_btn_nettest_clicked();

    void on_btn_connect_clicked();

    void on_btn_disconnect_clicked();

    void on_btn_request_cert_clicked();

    void on_btn_import_cert_clicked();

    void on_changeBtn_clicked();

private:
    Ui::IPSetWidget *ui;
    bool m_bModify;
    void initUi();
    void updateUi();
    pingThread *m_thread;
    QString ipStr1;
};



#endif // IPSETWIDGET_H
