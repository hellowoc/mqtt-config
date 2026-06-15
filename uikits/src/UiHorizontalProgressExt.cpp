#include "UiHorizontalProgressExt.h"

UiHorizontalProgressExt::UiHorizontalProgressExt(QWidget *parent) :
    UiAbstractProgressExt(parent)
{


    layout = new QHBoxLayout(this);
    layout->setMargin(0);

    progress = new UiHorizontalProgress();

    onLayout();
    slotsConnect();
}
void UiHorizontalProgressExt::onLayout(){
    int btn_width = DEFAULT_BUTTON_WIDTH;

    if(btn_width>height()){
        btn_width = height();
    }
    button_add->setFixedSize(btn_width,btn_width);
    button_sub->setFixedSize(btn_width,btn_width);

    label_ValueText->setFixedSize(80,45);
    int progress_width = DEFALUE_PROGRESS_WIDTH;

    if(progress_width>btn_width){
         progress_width= btn_width*F_RATIO;
    }
    progress->setFixedHeight(DEFALUE_PROGRESS_WIDTH);

    layout->addWidget(label_ValueText,0,Qt::AlignVCenter);
    layout->addWidget(button_sub,0, Qt::AlignVCenter);
    layout->addSpacing(VIEW_DIVIDE);
    layout->addWidget(progress,0, Qt::AlignVCenter);
    layout->addSpacing(VIEW_DIVIDE);
    layout->addWidget(button_add,0, Qt::AlignVCenter);
}

