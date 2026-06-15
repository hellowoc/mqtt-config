#ifndef G_CONFIG_H
#define G_CONFIG_H

#include <QtCore>
#include <QtGui>

#define BACKLIGHT_TIMEOUT   300		// 背光间隔：300 Sec

#define ICON_WIDTH          32
#define ICON_HEIGHT         32

#if(LCD_WIDTH == 1024)
    #define BTN_HEIGHT          50
    #define BTN_WIDTH           160
    #define SMALL_BTN_WIDTH     50
    #define DEFAULT_BTN_WIDTH   120
    #define BIG_BTN_WIDTH       250
    #define LCD_TITLE_HEIGHT    50
    #define LCD_CONTENT_HEIGTH  668
    #define LCD_STATUS_HEIGHT   60
    #define CURVE_WIDGET_HEIGHT 420
    #define CURVE_WIDGET_WIDTH	850
    #define TAB_HEIGHT          45
    #define PAGE_BTN_WIDTH      350
    #define PAGE_BTN_HEIGHT      60
#else
    #define BTN_HEIGHT          40
    #define BTN_WIDTH           135
    #define SMALL_BTN_WIDTH     40
    #define DEFAULT_BTN_WIDTH   100
    #define BIG_BTN_WIDTH       200
    #define LCD_TITLE_HEIGHT    45
    #define LCD_CONTENT_HEIGTH  390
    #define LCD_STATUS_HEIGHT   45
    #define CURVE_WIDGET_HEIGHT 310
    #define CURVE_WIDGET_WIDTH	500
    #define TAB_HEIGHT          40
    #define PAGE_BTN_WIDTH      250
    #define PAGE_BTN_HEIGHT      45
#endif

enum{
    DEFAULT_BTN_SIZE,
    BIG_BTN_SIZE,
    SMALL_BTN_SIZE
};

enum{
    DEFAULT_FONT_SIZE,
    FONT_SIZE_16,
    FONT_SIZE_12
};

class g_Config : public CSingletonBase<g_Config>
{
public:
    g_Config();
    void setBtnSize(int btnFlag,int w,int h);
    void setFont(int fontFlag, int size);
    QFont getFont(int fontFlag = DEFAULT_FONT_SIZE);
    QSize getBtnSize(int btnFlag);

private:
    QSize normalBtnSize;
    QSize bigBtnSize;
    QSize smallBtnSize;
    QFont normalFontSize;
    QFont font16;
    QFont font12;
    friend class CSingletonBase<g_Config>;
};

#endif // G_CONFIG_H
