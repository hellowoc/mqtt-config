#include "mypushbutton.h"

myPushButton::myPushButton(QString text, QString iconName, QRect rect, bool enable, QWidget *parent) :
    QPushButton(parent)
{
    config = g_Config::getInstance();
    setText(text);
    setFont(config->getFont(DEFAULT_FONT_SIZE));
    setIcon(QIcon(iconName));
    setIconSize(QSize(32,32));
    setGeometry(rect);
    setEnabled(enable);
    m_nName = iconName;
    m_sText = text;

    if(iconName == "" || text == ""){
        setStyleSheet("QPushButton {"
                          "text-align:center;"
                          "background-color: palegoldenrod;"
                          "border-width: 2px;"
                          "border-style: solid;"
                          "border-radius: 10;"
                          "padding: 3px;}"

                      "QPushButton:pressed {"
                          "padding-left: 5px;"
                         " padding-top: 5px;"
                          "background-color: #d0d67c;}");
    }
    else{
        setStyleSheet("QPushButton {"
                          "text-align:left;"
                          "background-color: palegoldenrod;"
                          "border-width: 2px;"
                          "border-style: solid;"
                          "border-radius: 10;"
                          "padding: 3px;}"

                      "QPushButton:pressed {"
                          "padding-left: 5px;"
                         " padding-top: 5px;"
                          "background-color: #d0d67c;}");
    }

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setSizePolicy(sizePolicy);
}

myPushButton::myPushButton(QString text, QString iconName, QRect rect, QWidget *parent) :
    QPushButton(parent)
{
    config = g_Config::getInstance();
    setText(text);
    setFont(config->getFont(DEFAULT_FONT_SIZE));
    setIcon(QIcon(iconName));
    setIconSize(QSize(32,32));
    setGeometry(rect);
    m_nName = iconName;
    m_sText = text;

    if(iconName == "" || text == ""){
        setStyleSheet("QPushButton {"
                          "text-align:center;"
                          "background-color: palegoldenrod;"
                          "border-width: 2px;"
                          "border-style: solid;"
                          "border-radius: 10;"
                          "padding: 3px;}"

                      "QPushButton:pressed {"
                          "padding-left: 5px;"
                         " padding-top: 5px;"
                          "background-color: #d0d67c;}");
    }
    else{
        setStyleSheet("QPushButton {"
                          "text-align:left;"
                          "background-color: palegoldenrod;"
                          "border-width: 2px;"
                          "border-style: solid;"
                          "border-radius: 10;"
                          "padding: 3px;}"

                      "QPushButton:pressed {"
                          "padding-left: 5px;"
                         " padding-top: 5px;"
                          "background-color: #d0d67c;}");
    }
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setSizePolicy(sizePolicy);
}

myPushButton::myPushButton(QString iconName, QSize size, QWidget *parent) :
    QPushButton(parent)
{
    setIcon(QIcon(iconName));
    setFixedSize(size);
    setIconSize(QSize(32,32));
    m_nName = iconName;
    m_sText = "";

    setStyleSheet("QPushButton {"
                      "text-align:center;"
                      "background-color: palegoldenrod;"
                      "border-width: 2px;"
                      "border-style: solid;"
                      "border-radius: 10;"
                      "padding: 3px;}"

                  "QPushButton:pressed {"
                      "padding-left: 5px;"
                     " padding-top: 5px;"
                      "background-color: #d0d67c;}");

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setSizePolicy(sizePolicy);
}

myPushButton::myPushButton(purviewType userType, QString text, QString iconName, QRect rect, bool enable, QWidget *parent)
{
    config = g_Config::getInstance();
    setText(text);
    setFont(config->getFont(DEFAULT_FONT_SIZE));
    setIcon(QIcon(iconName));
    setIconSize(QSize(32,32));
    setGeometry(rect);
    setEnabled(enable);
    m_nName = iconName;
    m_sText = text;

    if(iconName == "" || text == ""){
        switch(userType) {
        case userOpertor:
            setStyleSheet("QPushButton {"
                              "text-align:center;"
                              "background-color: palegoldenrod;"
                              "border-width: 2px;"
                              "border-style: solid;"
                              "border-radius: 10;"
                              "padding: 3px;}"

                          "QPushButton:pressed {"
                              "padding-left: 5px;"
                              "padding-top: 5px;"
                              "background-color: #d0d67c;}");
            break;
        case userEngineer:
            setStyleSheet("QPushButton {"
                              "text-align:center;"
                              "background-color: palegoldenrod;"
                              "border-width: 2px;"
                              "border-style: solid;"
                              "border-radius: 10;"
                              "padding: 3px;}"

                          "QPushButton:pressed {"
                              "padding-left: 5px;"
                              "padding-top: 5px;"
                              "background-color: #d0d67c;}");
            break;
        case userFactory:
            setStyleSheet("QPushButton {"
                              "text-align:center;"
                              "background-color: palegoldenrod;"
                              "border-width: 2px;"
                              "border-style: solid;"
                              "border-radius: 10;"
                              "padding: 3px;}"

                          "QPushButton:pressed {"
                              "padding-left: 5px;"
                              "padding-top: 5px;"
                              "background-color: #d0d67c;}");
            break;
        }
    }
    else{
        switch(userType) {
        case userOpertor:
            setStyleSheet("QPushButton {"
                              "text-align:left;"
                              "background-color: palegreen;"
                              "border-width: 2px;"
                              "border-style: solid;"
                              "border-radius: 10;"
                              "padding: 3px;}"

                          "QPushButton:pressed {"
                              "padding-left: 5px;"
                              "padding-top: 5px;"
                              "background-color: #d0d67c;}");
            break;
        case userEngineer:
            setStyleSheet("QPushButton {"
                              "text-align:left;"
                              "background-color: palegoldenrod;"
                              "border-width: 2px;"
                              "border-style: solid;"
                              "border-radius: 10;"
                              "padding: 3px;}"

                          "QPushButton:pressed {"
                              "padding-left: 5px;"
                              "padding-top: 5px;"
                              "background-color: #d0d67c;}");
            break;
        case userFactory:
            setStyleSheet("QPushButton {"
                              "text-align:left;"
                              "background-color: palevioletred;"
                              "border-width: 2px;"
                              "border-style: solid;"
                              "border-radius: 10;"
                              "padding: 3px;}"

                          "QPushButton:pressed {"
                              "padding-left: 5px;"
                              "padding-top: 5px;"
                              "background-color: #d0d67c;}");
            break;
        }
    }

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setSizePolicy(sizePolicy);
}

myPushButton::myPushButton(QString text, QString iconName, QRect rect, bool fixedSize, bool enable, QWidget *parent)
{
    config = g_Config::getInstance();
    setText(text);
    setFont(config->getFont(DEFAULT_FONT_SIZE));
    setIcon(QIcon(iconName));
    setIconSize(QSize(32,32));
    setEnabled(enable);
    setGeometry(rect);
    m_nName = iconName;
    m_sText = text;

    if(iconName == ""){
        setStyleSheet("QPushButton {"
                                "text-align:center; "
                                "background-color: palegoldenrod;"
                                "border-width: 2px;"
                                "border-style: solid;"
                                "border-radius: 10;"
                                "padding: 3px;}"

                            "QPushButton:pressed {"
                                "padding-left: 5px;"
                                "padding-top: 5px;"
                                "background-color: #d0d67c;}"
                            );
    }
    else {
        setStyleSheet("QPushButton {"
                                "text-align:left; "
                                "background-color: palegoldenrod;"
                                "border-width: 2px;"
                                "border-style: solid;"
                                "border-radius: 10;"
                                "padding: 3px;}"

                            "QPushButton:pressed {"
                                "padding-left: 5px;"
                                "padding-top: 5px;"
                                "background-color: #d0d67c;}"
                            );
    }

    if(fixedSize) {
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        setSizePolicy(sizePolicy);
    }
}

void myPushButton::setRedColor(int color)
{
    if((m_nName != "") && (m_sText != "")){
        switch(color)
        {
        case RED:
            setStyleSheet("QPushButton {"
                              "text-align:left;"
                              "background-color: red;"
                              "border-width: 2px;"
                              "border-style: solid;"
                              "border-radius: 10;"
                              "padding: 3px;}"

                          "QPushButton:pressed {"
                              "padding-left: 5px;"
                              "padding-top: 5px;"
                              "background-color: #d0d67c;}");
            break;
        case GREEN:
            setStyleSheet("QPushButton {"
                              "text-align:left;"
                              "background-color: chartreuse;"
                              "border-width: 2px;"
                              "border-style: solid;"
                              "border-radius: 10;"
                              "padding: 3px;}"

                          "QPushButton:pressed {"
                              "padding-left: 5px;"
                              "padding-top: 5px;"
                              "background-color: #d0d67c;}");
            break;
        case DEF:
            setStyleSheet("QPushButton {"
                              "text-align:left;"
                              "background-color: palegoldenrod;"
                              "border-width: 2px;"
                              "border-style: solid;"
                              "border-radius: 10;"
                              "padding: 3px;}"

                          "QPushButton:pressed {"
                              "padding-left: 5px;"
                              "padding-top: 5px;"
                              "background-color: #d0d67c;}");
            break;
        }
    }
    else{
        switch(color)
        {
        case RED:
            setStyleSheet("QPushButton {"
                              "text-align:center;"
                              "background-color: chartreuse;"
                              "border-width: 2px;"
                              "border-style: solid;"
                              "border-radius: 10;"
                              "padding: 3px;}"

                          "QPushButton:pressed {"
                              "padding-left: 5px;"
                             " padding-top: 5px;"
                              "background-color: #d0d67c;}");
            break;
        case GREEN:
            setStyleSheet("QPushButton {"
                              "text-align:center;"
                              "background-color: chartreuse;"
                              "border-width: 2px;"
                              "border-style: solid;"
                              "border-radius: 10;"
                              "padding: 3px;}"

                          "QPushButton:pressed {"
                              "padding-left: 5px;"
                             " padding-top: 5px;"
                              "background-color: #d0d67c;}");
            break;
        case DEF:
            setStyleSheet("QPushButton {"
                              "text-align:center;"
                              "background-color: palegoldenrod;"
                              "border-width: 2px;"
                              "border-style: solid;"
                              "border-radius: 10;"
                              "padding: 3px;}"

                          "QPushButton:pressed {"
                              "padding-left: 5px;"
                             " padding-top: 5px;"
                              "background-color: #d0d67c;}");
            break;
        }
    }
}
