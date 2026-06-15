#include "mynoscrollcombobox.h"
void NoScrollComboBox::wheelEvent(QWheelEvent *event)
{
    event->ignore(); // 忽略滚轮事件
}
