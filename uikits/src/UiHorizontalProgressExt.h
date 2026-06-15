#ifndef UiHorizontalProgressEXT_H
#define UiHorizontalProgressEXT_H

#include "uikits_shared.h"
#include "UiAbstractProgressExt.h"
#include "UiHorizontalProgress.h"
#include <QHBoxLayout>
class UIKITS_SHARED UiHorizontalProgressExt : public UiAbstractProgressExt
{
    Q_OBJECT
public:
    explicit UiHorizontalProgressExt(QWidget *parent = 0);
    virtual void onLayout();
signals:

public slots:

private:
    QHBoxLayout *layout;
};

#endif // UiHorizontalProgressEXT_H
