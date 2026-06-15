#ifndef MYNOSCROLLCOMBOBOX_H
#define MYNOSCROLLCOMBOBOX_H

#include "uikits_shared.h"
#include <QComboBox>
#include <QWheelEvent>

class UIKITS_SHARED NoScrollComboBox : public QComboBox
{
    Q_OBJECT
public:
    using QComboBox::QComboBox; // 继承父类构造函数

protected:
    void wheelEvent(QWheelEvent *event) override;
};

#endif // MYNOSCROLLCOMBOBOX_H
