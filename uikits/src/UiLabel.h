#ifndef UiLabel_H
#define UiLabel_H
#include "uikits_shared.h"
#include <QLabel>

class UIKITS_SHARED UiLabel : public QLabel
{
    Q_OBJECT
public:
    explicit UiLabel(QWidget *parent = 0);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);

public:
    bool getBSupporttips() const;
    void setBSupporttips(bool value);

    QString getContentTips() const;
    void setContentTips(const QString &value);
    void setOffset(int offsetX,int offsetY);

signals:

public slots:
private:
    bool bSupporttips;
    QString contentTips;
    int m_offsetX,m_offsetY;

};

#endif // UiLabel_H
