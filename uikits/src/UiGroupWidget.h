#ifndef UiGroupWidget_H
#define UiGroupWidget_H
#include "uikits_shared.h"
#include <QWidget>
#include <QPushButton>
#include <QList>
#include <QString>
#include <QHBoxLayout>
typedef struct GroupItem
{
    QString itemName;
    int     tag;
    int     subTag;

}GroupItem;
class UIKITS_SHARED UiGroupWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UiGroupWidget(QWidget *parent = 0);


    void setGroupContents(QList<GroupItem> contents);
    void setButtonSize(int w,int h);
    void setButtonStyle(const QString &style);

    int GetSelected_pos() const;
    void setSelected_pos(int selected_pos);
    void setPosEnable(int pos,bool enable);
signals:
    void clickedButton(int pos);
public slots:
    void buttonClick();
protected:
    void caculateWidgetSize();
    void initButtonStyle();
private:
    QList<GroupItem> m_contents;
    QList<QPushButton *> m_buttons;
    int m_btn_width;
    int m_btn_height;
    int m_selected_pos;
    QString m_style;
    QString m_style_selected;
    QHBoxLayout *layout;
};

#endif // UiGroupWidget_H
