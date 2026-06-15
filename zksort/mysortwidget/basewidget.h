#ifndef basewidget_H
#define basewidget_H

#include "baseui.h"
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include "UiGroupWidget.h"
#include <QSignalMapper>
#include <QButtonGroup>
#include <QComboBox>
#include "global.h"
enum data_change{
    chute_change = 100,
    calimode_change = 101
};

class SetEditValueWidget;
//抽象接口类
class AbstractInterface
{
public:
    virtual void receiveMsg(int msg1,int msg2 = 0,int msg3=0) = 0;
};
class ItemCell
{
public:
    int     itemId;
    QString itemText;
    QString itemObjName;
    ItemCell(int _id,QString _text,QString _objName)
    {
        itemId = _id;
        itemText = _text;
        itemObjName = _objName;
    }
    ItemCell()
    {
        itemId = -1;
        itemText = "";
        itemObjName = "";
    }
};
class SingleSenseData{
public:
    int m_senseIndex;//0-5  灰度 色差  6-7 svm 8 大小 9 长短 10 圆
    int m_value;
    int m_max;
    int m_min;
    int m_bUsed;
    int m_view;
    QString m_senseName;

    SingleSenseData(int index,int value,QString name,int max,int min,int view,int used= 0){
        m_senseIndex = index;
        m_value = value;
        m_senseName =name;
        m_max = max;
        m_min = min;
        m_bUsed = used;
        m_view = view;
    }

};

class basewidget : public BaseUi, public AbstractInterface
{
    Q_OBJECT
public:
    explicit basewidget(QWidget *parent = 0);
    virtual ~basewidget();
    uint    m_CurrentPageId;      //页面Id
    uint    m_PreviousPageId;      //上一个页面的Id

public:
    QPushButton *m_pViewBtn[MAX_VIEW];
    QPushButton *m_pLayerBtn[2];
    SetEditValueWidget *m_pChuteWidget;

    QLabel *m_pGroupLbl;
    int m_chuteShowNum;
    void setSlotConnect();
    void onViewClicked(int view);
    void onLayerClicked(int layer);
    void updateViewButtonVisible();
    void updateViewBtnState();
    void updateChuteNum();
    void updateLayerBtnState();
    void setAndSendParams();//部分设置参数界面使用
    void updateGroupWidget();
    void updateGroupWidgetState();
    void updateGroupLabelShow();
    UiGroupWidget *m_pGroupWidget;
private:
    QList<GroupItem> groupItems;
public slots:
    void viewClickedSlots();
    void groupBtnClick(int);
    void layerClickedSlots();
signals:
public:
    virtual void showPage(bool bshow = true);
    void returnBack();
    virtual void receiveMsg(int msg1,int msg2 = 0,int msg3 = 0);
    virtual void OnViewChange(int view);
    virtual void OnGroupChange(int groupIndex);
    virtual void OnChuteChange();
    virtual void OnLayerChange();
};

#endif // basewidget_H
