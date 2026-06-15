#include "globalconfig.h"

g_Config::g_Config()
{
    setBtnSize(BIG_BTN_SIZE, BIG_BTN_WIDTH, BTN_HEIGHT);
    setBtnSize(DEFAULT_BTN_SIZE, DEFAULT_BTN_WIDTH, BTN_HEIGHT);

    if(LCD_WIDTH == 1024)
        setBtnSize(SMALL_BTN_SIZE, SMALL_BTN_WIDTH, BTN_HEIGHT);
    else
        setBtnSize(SMALL_BTN_SIZE, SMALL_BTN_WIDTH, BTN_HEIGHT);

    if(LCD_WIDTH == 1024)
        setFont(DEFAULT_FONT_SIZE, 24);
    else
        setFont(DEFAULT_FONT_SIZE, 16);
    setFont(FONT_SIZE_12, 12);
    setFont(FONT_SIZE_16, 16);
}

/* 根据按键的标志设置按键的尺寸 */
void g_Config::setBtnSize(int btnFlag, int w, int h)
{
    switch(btnFlag){
    case BIG_BTN_SIZE:
        bigBtnSize.setWidth(w);
        bigBtnSize.setHeight(h);
        break;
    case DEFAULT_BTN_SIZE:
        normalBtnSize.setWidth(w);
        normalBtnSize.setHeight(h);
        break;
    case SMALL_BTN_SIZE:
        smallBtnSize.setWidth(w);
        smallBtnSize.setHeight(h);
        break;
    default:
        break;
    }
}

/* 根据字体的标志设置字体 */
void g_Config::setFont(int fontFlag, int size)
{
    switch(fontFlag){
    case DEFAULT_FONT_SIZE:
        normalFontSize.setPixelSize(size);
        break;
    case FONT_SIZE_12:
        font12.setPixelSize(12);
        break;
    case FONT_SIZE_16:
        font16.setPixelSize(16);
        break;
    default:
        break;
    }
}

/* 根据字体的标志获取字体 */
QFont g_Config::getFont(int fontFlag)
{
    switch(fontFlag){
    case DEFAULT_FONT_SIZE:
        return normalFontSize;
    case FONT_SIZE_12:
        return font12;
    case FONT_SIZE_16:
        return font16;
    default:
        return QFont();
    }
}

/* 根据按键标识获取按键的尺寸 */
QSize g_Config::getBtnSize(int btnFlag)
{
    switch(btnFlag){
    case BIG_BTN_SIZE:
        return bigBtnSize;
    case DEFAULT_BTN_SIZE:
        return normalBtnSize;
    case SMALL_BTN_SIZE:
        return smallBtnSize;
    default:
        return QSize(0,0);
    }
}
