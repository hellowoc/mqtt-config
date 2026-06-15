#include "UiLabel.h"
#include "UiToast.h"
UiLabel::UiLabel(QWidget *parent) :
    QLabel(parent),
    m_offsetX(0),
    m_offsetY(0)
{
    contentTips = "";
}
void UiLabel::mousePressEvent(QMouseEvent *event){

    if(bSupporttips){

        if(contentTips.isEmpty()){
            contentTips = "no tips";
        }
        UiToast *toast=new UiToast((QWidget *)parent(),contentTips);
        toast->showToast(this,m_offsetX,m_offsetY);
    }
}

void UiLabel::mouseReleaseEvent(QMouseEvent *event){

}

bool UiLabel::getBSupporttips() const
{
    return bSupporttips;
}

void UiLabel::setBSupporttips(bool value)
{
    bSupporttips = value;
}
QString UiLabel::getContentTips() const
{
    return contentTips;
}

void UiLabel::setContentTips(const QString &value)
{
    contentTips = value;
}

void UiLabel::setOffset(int offsetX, int offsetY)
{
    m_offsetX = offsetX;
    m_offsetY = offsetY;
}


