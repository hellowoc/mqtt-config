#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>
#include "globalconfig.h"
enum    purviewType{
    userOpertor,
    userEngineer,
    userFactory
};
#define DEF         0
#define RED         1
#define GREEN       2

class myPushButton : public QPushButton
{
    //Q_OBJECT
public:
    explicit myPushButton(QString text, QString iconName, QRect rect, bool enable, QWidget *parent = 0);
    explicit myPushButton(QString text, QString iconName, QRect rect, QWidget *parent = 0);
    explicit myPushButton(QString iconName, QSize size, QWidget *parent = 0);
    explicit myPushButton(QString text, QString iconName, QRect rect, bool fixedSize, bool enable, QWidget *parent = 0); // 固定大小
    explicit myPushButton(purviewType userType,QString text, QString iconName, QRect rect, bool enable, QWidget *parent = 0);
    void setRedColor(int color);

signals:
    
public slots:

private:
    purviewType userType;
    g_Config *config;
    QString m_nName;
    QString m_sText;
};

#endif // MYPUSHBUTTON_H
