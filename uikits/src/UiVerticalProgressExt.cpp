#include "UiVerticalProgressExt.h"
#include <QDebug>

UiVerticalProgressExt::UiVerticalProgressExt(QWidget *parent) :
    UiAbstractProgressExt(parent)
{

    layout = new QVBoxLayout(this);
    layout->setMargin(0);
    progress = new UiVerticalProgress();
    onLayout();
    slotsConnect();
}
void UiVerticalProgressExt::onLayout(){

    label_ValueText->setFixedSize(80,45);
    progress->setFixedWidth(DEFALUE_PROGRESS_WIDTH);
    layout->addWidget(button_add,0,Qt::AlignHCenter);
    layout->addSpacing(VIEW_DIVIDE);
    layout->addWidget(progress,0, Qt::AlignHCenter);
    layout->addSpacing(VIEW_DIVIDE);
    layout->addWidget(button_sub,0,Qt::AlignHCenter);
    layout->addWidget(label_ValueText,0,Qt::AlignHCenter);
}

