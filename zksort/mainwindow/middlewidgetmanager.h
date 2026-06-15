#ifndef MIDDLEWIDGETMANAGER_H
#define MIDDLEWIDGETMANAGER_H
#include <QObject>
#include <QStackedWidget>
#include <QMap>
#include "logger.h"
#include "global/constant.h"

class basewidget;
class ZKSort;
class MiddleWidgetManager : public QObject
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
public:
    explicit MiddleWidgetManager(QObject *parent = 0);
    void returnBackPage();
    void setStackedWidget(QStackedWidget *stack,ZKSort *mainWidget);
    void RefreshCurWidget();
    void ShowWidget(uint pageId, bool bBack = false);
    void CreateWidgets();
    void SetLanguage();
    void SendCurWidgetMsg(int msg1,int msg2=0,int msg3=0);
    void SendWidgetMsg(uint pageId,int msg1,int msg2=0,int msg3=0);
    basewidget *GetCurWidget();
    QString  GetWidgetNameByIndex(int index);
    basewidget* GetWidgetByID(uint pageId);
    void SetLanguageTitle(QString str);
    void SetBottomEnable(bool enable);
    void RefreshBottomStatus();
private:
    QStackedWidget *m_pStackedWidget;
    ZKSort *m_pMainWidget;
    QMap<int,int> m_WidgetMap;// pageid   page在stackedwidget中的索引
    basewidget *m_pCurWidget;
    int m_CurIndex;

    int CreatePageByID(uint pageId);

public slots:
    void RecvMsgSlot(int msg1, int msg2, int msg3);
    void RecvMsgToPageSlot(QString pageName, int msg1, int msg2, int msg3);

};

#endif // MIDDLEWIDGETMANAGER_H
