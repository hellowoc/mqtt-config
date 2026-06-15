#ifndef THUIPROGRESSEXT_H
#define THUIPROGRESSEXT_H
#include "uikits_shared.h"
#include <QVBoxLayout>
#include "UiAbstractProgressExt.h"
#include "UiVerticalProgress.h"

class UIKITS_SHARED UiVerticalProgressExt : public UiAbstractProgressExt
{
    Q_OBJECT
public:
     explicit UiVerticalProgressExt(QWidget *parent = 0);
     virtual void onLayout();
private:
    QVBoxLayout *layout;
};

#endif // THUIPROGRESSEXT_H
