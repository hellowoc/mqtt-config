#-------------------------------------------------
#
# Project created by QtCreator 2019-07-17T11:20:16
#
#-------------------------------------------------

QT       += core gui network sql xml widgets printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
#CODECFORTR = utf-8
TARGET = zksort
TEMPLATE = app
DESTDIR = ./dist
OBJECTS_DIR = ./build/obj
MOC_DIR = ./build/moc
UI_DIR = ./build/ui
QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += $$PWD \
               $$PWD/mainwindow \
               $$PWD/mainpage \
               $$PWD/mysortwidget \
               $$PWD/engineerset \
               $$PWD/global \
               $$PWD/3rdparty \
               $$PWD/bus



SOURCES += main.cpp \
    mainwindow/zksort.cpp \
    mysortwidget/basewidget.cpp \
    mainpage/userwidget.cpp \
    mainwindow/bottomwidget.cpp \
    mainwindow/topwidget.cpp \
    mainwindow/middlewidgetmanager.cpp \
    global/global.cpp \
    plastic/accelerationsensorsetwidget.cpp \
    plastic/irbadpointsetwidget.cpp \
    plastic/iroffsetwidget.cpp \
    systeminfo/sysstatewidget.cpp \
    mainpage/engineerwidget.cpp \
    global/runtime.cpp \
    factoryset/factorysetwidget.cpp \
    factoryset/languagesetwidget.cpp \
    mainpage/feedsetwidget.cpp \
    mainpage/sensesetwidget.cpp \
    mainpage/modemanagewidget.cpp \
    mysortwidget/usersensesetpage.cpp \
    mysortwidget/mywavewidget.cpp \
    mysortwidget/myswitchwidget.cpp \
    data/jsondataconvert.cpp \
    data/myjson.cpp \
    factoryset/machinesetwidget.cpp \
    global/globalflow.cpp \
    bus/myqextserialport.cpp \
    global/mylanguage.cpp \
    global/material.cpp \
    mysortwidget/mylistwidgetitem.cpp \
    systeminfo/barchart.cpp \
    systeminfo/plotcurve.cpp \
    mysortwidget/mydelaycode.cpp \
    mysortwidget/myarithui.cpp \
    mysortwidget/baseui.cpp \
    mysortwidget/basearithwidget.cpp \
    factoryset/camerasetwidget.cpp \
    bus/mynetwork.cpp \
    mysortwidget/mylightcontrol.cpp \
    bus/myqueue.cpp \
    systeminfo/sysinfowidget.cpp \
    systeminfo/sysversioninfowidget.cpp \
    systeminfo/sysinfogeneralpage.cpp \
    systeminfo/sysinfolog.cpp \
    systeminfo/alarmsetwidget.cpp \
    mysortwidget/mylineedit.cpp \
    mysortwidget/myinputdialog.cpp \
    arithprofilewidget/graysensesetwidget.cpp \
    arithprofilewidget/svmsensesetwidget.cpp \
    arithprofilewidget/circlelongsensesetwidget.cpp \
    arithprofilewidget/bigsmallsensesetwidget.cpp \
    arithprofilewidget/longshortsensesetwidget.cpp \
    mysortwidget/renameprofiledialog.cpp \
    mysortwidget/messagedialog.cpp \
    mainpage/mainpagewidget.cpp \
    mysortwidget/mytoolbutton.cpp \
    mysortwidget/mywid.cpp \
    mainpage/wipesetwidget.cpp \
    global/mythread.cpp \
    systeminfo/vavletest.cpp \
    mysortwidget/seteditvaluewidget.cpp \
    factoryset/camerasignalwidget.cpp \
    factoryset/dividepixelwidget.cpp \
    engineerset/backgroundsetwidget.cpp \
    engineerset/ejectwidget.cpp \
    engineerset/identifywidget.cpp \
    engineerset/imagemanagewidget.cpp \
    engineerset/imagewidget.cpp \
    engineerset/lightsetwidget.cpp \
    factoryset/pixeldivisionshowwidget.cpp \
    engineerset/imagecapturewidget.cpp \
    engineerset/imageanalysiswidget.cpp \
    mysortwidget/switchbtn.cpp \
    factoryset/machinefunction.cpp \
    mainpage/dccryptwidget.cpp \
    mainpage/selfcheckwidget.cpp \
    mainpage/ejectdelay.cpp \
    mainpage/simulatewidget.cpp \
    systeminfo/ipsetwidget.cpp \
    systeminfo/remotewidget.cpp \
    systeminfo/machineinfowidget.cpp \
    factoryset/fpga.cpp \
    factoryset/pixeladjust.cpp \
    mysortwidget/mysetdatetime.cpp \
    mainpage/newmainpage.cpp \
    factoryset/lampboardcfg.cpp \
    data/mysqlite.cpp \
    mysortwidget/myinputmethod.cpp \
    mainpage/feedsetbygrouppage.cpp \
    mainpage/feedsetbychutepage.cpp \
    mysortwidget/setsvmusedialog.cpp \
    bus/myhttpfileclient.cpp \
    factoryset/nirdotcorrectivelistwidget.cpp \
    arithprofilewidget/colorsatsensesetwidget.cpp \
    arithprofilewidget/colorscalesensesetwidget.cpp \
    arithprofilewidget/colorhuesensesetwidget.cpp \
    arithprofilewidget/discolorsensesetwidget.cpp \
    mainpage/feederaotuajust.cpp \
    mainpage/aiwipesetwidget.cpp \
    mainpage/createnewmodewidget.cpp \
    factoryset/feedersensorsetwidget.cpp \
    factoryset/tempsensorsetwidget.cpp \
    engineerset/aisetdialog.cpp \
    factoryset/updatefpgawidget.cpp \
    mysortwidget/hsvcolorcircle.cpp \
    arithprofilewidget/hsvsensesetwidget.cpp \
    arithprofilewidget/inhibitsensesetwidget.cpp \
    arithprofilewidget/straightsensesetwidget.cpp \
    arithprofilewidget/mildewsensessetwidget.cpp \
    arithprofilewidget/outlinesensesetwidget.cpp \
    arithprofilewidget/linesensesetwidget.cpp \
    arithprofilewidget/budsensesetwidget.cpp \
    arithprofilewidget/colorvaluesensesetwidget.cpp \
    arithprofilewidget/roundsensesetwidget.cpp \
    arithprofilewidget/splmildewsensesetwidget.cpp \
    arithprofilewidget/whitepeanutsensesetwidget.cpp \
    engineerset/doublelightsetwidget.cpp \
    arithprofilewidget/shapesvmsensesetwidget.cpp \
    arithprofilewidget/pepperhandlesensewidget.cpp \
    bus/myudpthread.cpp \
    systeminfo/ipcinfowidget.cpp \
    factoryset/ipccamerasetwidget.cpp \
    factoryset/ipcsetwidget.cpp \
    factoryset/cameracolorrestore.cpp \
    mainpage/ipcclassparams.cpp \
    mainpage/modelchangewidget.cpp \
    mysortwidget/downloaddialog.cpp \
    engineerset/imagedatachartrgbwidget.cpp \
    engineerset/imagedatachartwidget.cpp \
    engineerset/imagedatachartdiscolorwidget.cpp \
    engineerset/upgrademodelwidget.cpp \
    factoryset/ipcalarmcontrol.cpp \
    systeminfo/sysinfodebug.cpp \
    engineerset/ipcejectwidget.cpp \
    systeminfo/sysipcbadpointswidget.cpp \
    engineerset/imageinforgbwidget.cpp \
    systeminfo/sysinfonet.cpp \
    mysortwidget/mylistwidget.cpp \
    factoryset/updateacswidget.cpp \
    arithprofilewidget/suppresslargesensesetwidget.cpp \
    arithprofilewidget/wheatsproutsensesetwidget.cpp \
    factoryset/ipcaisetwidget.cpp \
    systeminfo/ipcclassinfowidget.cpp \
    mysortwidget/mysvmsampleclass.cpp \
    global/myfscheck.cpp \
    engineerset/imagefastcapturewidget.cpp \
    bus/myfastnetwork.cpp \
    mainpage/feedbanlanceaimsingle.cpp \
    mainpage/feedsetbyclearuppage.cpp \
    global/myautofeed.cpp \
    factoryset/onekeyupdatefpgawidget.cpp \
    factoryset/selectupdatefilewidget.cpp \
    arithprofilewidget/teacolorsensesetwidget.cpp \    
	systeminfo/videtectboardinfowidget.cpp \
    engineerset/svmimagewidget.cpp \
    systeminfo/wificonfigwidget.cpp \
    systeminfo/wifiinterface.cpp \
    mainpage/modelselectwidget.cpp \
    mainpage/remoteControl.cpp \
    engineerset/onekeyfixdialog.cpp \
    autocontrol/plcautoctrlmanager.cpp \
    autocontrol/plcctrlwidget.cpp \
    systeminfo/autoctrlboardipsetwidget.cpp \
    systeminfo/tcpserverobj.cpp\
    systeminfo/syscamcaliinfowidget.cpp \
    factoryset/groupsetdialog.cpp \
    factoryset/fpgamodesetwidget.cpp \
    systeminfo/sysoutputwidget.cpp \
    systeminfo/ipcclasssetwidget.cpp \
    systeminfo/qualitychart.cpp \
    systeminfo/piechart.cpp \
    systeminfo/histogramwidget.cpp \
    systeminfo/targetimagewidget.cpp

HEADERS  += \
    mainwindow/zksort.h \
    mysortwidget/basewidget.h \
    mainpage/userwidget.h \
    mainwindow/bottomwidget.h \
    mainwindow/topwidget.h \
    mainwindow/middlewidgetmanager.h \
    global/global.h \
    plastic/accelerationsensorsetwidget.h \
    plastic/irbadpointsetwidget.h \
    plastic/iroffsetwidget.h \
    systeminfo/sysstatewidget.h \
    mainpage/engineerwidget.h \
    global/runtime.h \
    factoryset/factorysetwidget.h \
    factoryset/languagesetwidget.h \
    mainpage/feedsetwidget.h \
    mainpage/sensesetwidget.h \
    mainpage/modemanagewidget.h \
    mysortwidget/usersensesetpage.h \
    global/constant.h \
    mysortwidget/mywavewidget.h \
    mysortwidget/myswitchwidget.h \
    data/jsondataconvert.h \
    data/myjson.h \
    factoryset/machinesetwidget.h \
    global/globalflow.h \
    bus/myqextserialport.h \
    global/mylanguage.h \
    global/globalparams.h \
    global/material.h \
    mysortwidget/mylistwidgetitem.h \
    systeminfo/barchart.h \
    systeminfo/plotcurve.h \
    mysortwidget/mydelaycode.h \
    mysortwidget/myarithui.h \
    mysortwidget/baseui.h \
    mysortwidget/basearithwidget.h \
    factoryset/camerasetwidget.h \
    bus/mynetwork.h \
    mysortwidget/mylightcontrol.h \
    bus/myqueue.h \
    systeminfo/sysinfowidget.h \
    systeminfo/sysversioninfowidget.h \
    systeminfo/alarmsetwidget.h \
    systeminfo/sysinfogeneralpage.h \
    systeminfo/sysinfolog.h \
    mysortwidget/mylineedit.h \
    mysortwidget/myinputdialog.h \
    arithprofilewidget/graysensesetwidget.h \
    arithprofilewidget/svmsensesetwidget.h \
    arithprofilewidget/circlelongsensesetwidget.h \
    arithprofilewidget/bigsmallsensesetwidget.h \
    arithprofilewidget/longshortsensesetwidget.h \
    mysortwidget/renameprofiledialog.h \
    mysortwidget/messagedialog.h \
    mainpage/mainpagewidget.h \
    mysortwidget/mytoolbutton.h \
    mysortwidget/mywid.h \
    mainpage/wipesetwidget.h \
    global/mythread.h \
    systeminfo/vavletest.h \
    mysortwidget/seteditvaluewidget.h \
    factoryset/camerasignalwidget.h \
    factoryset/dividepixelwidget.h \
    engineerset/backgroundsetwidget.h \
    engineerset/ejectwidget.h \
    engineerset/identifywidget.h \
    engineerset/imagemanagewidget.h \
    engineerset/imagewidget.h \
    engineerset/lightsetwidget.h \
    factoryset/pixeldivisionshowwidget.h \
    engineerset/imagecapturewidget.h \
    engineerset/imageanalysiswidget.h \
    mysortwidget/switchbtn.h \
    factoryset/machinefunction.h \
    mainpage/dccryptwidget.h \
    mainpage/selfcheckwidget.h \
    mainpage/ejectdelay.h \
    mainpage/simulatewidget.h \
    systeminfo/ipsetwidget.h \
    systeminfo/remotewidget.h \
    systeminfo/machineinfowidget.h \
    factoryset/fpga.h \
    factoryset/pixeladjust.h \
    mysortwidget/mysetdatetime.h \
    factoryset/lampboardcfg.h \
    mainpage/newmainpage.h \
    data/mysqlite.h \
    mysortwidget/myinputmethod.h \
    mysortwidget/mypinyin.h \
    mainpage/feedsetbygrouppage.h \
    mainpage/feedsetbychutepage.h \
    mysortwidget/setsvmusedialog.h \
    bus/myhttpfileclient.h \
    factoryset/nirdotcorrectivelistwidget.h \
    arithprofilewidget/colorsatsensesetwidget.h \
    arithprofilewidget/colorscalesensesetwidget.h \
    arithprofilewidget/colorhuesensesetwidget.h \
    arithprofilewidget/discolorsensesetwidget.h \
    mainpage/feederaotuajust.h \
    mainpage/aiwipesetwidget.h \
    mainpage/createnewmodewidget.h \
    factoryset/feedersensorsetwidget.h \
    factoryset/tempsensorsetwidget.h \
    engineerset/aisetdialog.h \
    factoryset/updatefpgawidget.h \
    mysortwidget/hsvcolorcircle.h \
    arithprofilewidget/hsvsensesetwidget.h \
    arithprofilewidget/inhibitsensesetwidget.h \
    arithprofilewidget/straightsensesetwidget.h \
    arithprofilewidget/mildewsensessetwidget.h \
    arithprofilewidget/outlinesensesetwidget.h \
    arithprofilewidget/linesensesetwidget.h \
    arithprofilewidget/budsensesetwidget.h \
    arithprofilewidget/colorvaluesensesetwidget.h \
    arithprofilewidget/roundsensesetwidget.h \
    arithprofilewidget/splmildewsensesetwidget.h \
    arithprofilewidget/whitepeanutsensesetwidget.h \
    engineerset/doublelightsetwidget.h \
    arithprofilewidget/shapesvmsensesetwidget.h \
    arithprofilewidget/pepperhandlesensewidget.h \
    bus/myudpthread.h \
    systeminfo/ipcinfowidget.h \
    factoryset/ipccamerasetwidget.h \
    factoryset/ipcsetwidget.h \
    factoryset/cameracolorrestore.h \
    mainpage/ipcclassparams.h \
    mainpage/modelchangewidget.h \
    mysortwidget/downloaddialog.h \
    engineerset/imagedatachartrgbwidget.h \
    engineerset/imagedatachartwidget.h \
    engineerset/imagedatachartdiscolorwidget.h \
    engineerset/upgrademodelwidget.h \
    factoryset/ipcalarmcontrol.h \
    systeminfo/sysinfodebug.h \
    engineerset/ipcejectwidget.h \
    systeminfo/sysipcbadpointswidget.h \
    engineerset/imageinforgbwidget.h \
    systeminfo/sysinfonet.h \
    mysortwidget/mylistwidget.h \
    factoryset/updateacswidget.h \
    arithprofilewidget/suppresslargesensesetwidget.h \
    arithprofilewidget/wheatsproutsensesetwidget.h \
    factoryset/ipcaisetwidget.h \
    systeminfo/ipcclassinfowidget.h \
    mysortwidget/mysvmsampleclass.h \
    global/myfscheck.h \    
    engineerset/imagefastcapturewidget.h \
    bus/myfastnetwork.h \
    mainpage/feedbanlanceaimsingle.h \
    mainpage/feedsetbyclearuppage.h \
    global/myautofeed.h \
    factoryset/onekeyupdatefpgawidget.h \
    factoryset/selectupdatefilewidget.h \
    arithprofilewidget/teacolorsensesetwidget.h \
    systeminfo/videtectboardinfowidget.h \
    engineerset/svmimagewidget.h \
    systeminfo/wificonfigwidget.h \
    systeminfo/wifiinterface.h \
    mainpage/modelselectwidget.h \
    mainpage/remoteControl.h \
    engineerset/onekeyfixdialog.h \
    autocontrol/plcautoctrlmanager.h \
    autocontrol/plcctrlwidget.h \
    systeminfo/autoctrlboardipsetwidget.h \
    systeminfo/tcpserverobj.h\
    systeminfo/syscamcaliinfowidget.h \
    global/custom.h \
    factoryset/groupsetdialog.h \
    factoryset/fpgamodesetwidget.h \
    systeminfo/sysoutputwidget.h \
    systeminfo/ipcclasssetwidget.h \
    systeminfo/qualitychart.h \
    systeminfo/piechart.h \
    systeminfo/histogramwidget.h \
    systeminfo/targetimagewidget.h

FORMS    += \
    mainwindow/zksort.ui \
    mainpage/userwidget.ui \
    mainwindow/bottomwidget.ui \
    mainwindow/topwidget.ui \
    plastic/accelerationsensorsetwidget.ui \
    plastic/irbadpointsetwidget.ui \
    plastic/iroffsetwidget.ui \
    systeminfo/sysstatewidget.ui \
    mainpage/engineerwidget.ui \
    factoryset/factorysetwidget.ui \
    factoryset/languagesetwidget.ui \
    mainpage/feedsetwidget.ui \
    mainpage/sensesetwidget.ui \
    mainpage/modemanagewidget.ui \
    mysortwidget/usersensesetpage.ui \
    mysortwidget/myswitchwidget.ui \
    factoryset/machinesetwidget.ui \
    mysortwidget/myarithui.ui \
    factoryset/camerasetwidget.ui \
    mysortwidget/mylightcontrol.ui \
    systeminfo/sysinfowidget.ui \
    systeminfo/sysversioninfowidget.ui \
    systeminfo/alarmsetwidget.ui \
    systeminfo/sysinfolog.ui \
    mysortwidget/myinputdialog.ui \
    arithprofilewidget/graysensesetwidget.ui \
    arithprofilewidget/svmsensesetwidget.ui \
    arithprofilewidget/circlelongsensesetwidget.ui \
    arithprofilewidget/bigsmallsensesetwidget.ui \
    arithprofilewidget/longshortsensesetwidget.ui \
    mysortwidget/renameprofiledialog.ui \
    mysortwidget/messagedialog.ui \
    mainpage/mainpagewidget.ui \
    mainpage/wipesetwidget.ui \
    systeminfo/vavletest.ui \
    mysortwidget/seteditvaluewidget.ui \
    factoryset/camerasignalwidget.ui \
    factoryset/dividepixelwidget.ui \
    engineerset/backgroundsetwidget.ui \
    engineerset/ejectwidget.ui \
    engineerset/identifywidget.ui \
    engineerset/imagemanagewidget.ui \
    engineerset/imagewidget.ui \
    engineerset/lightsetwidget.ui \
    factoryset/pixeldivisionshowwidget.ui \
    engineerset/imagecapturewidget.ui \
    engineerset/imageanalysiswidget.ui \
    factoryset/machinefunction.ui \
    mainpage/dccryptwidget.ui \
    mainpage/selfcheckwidget.ui \
    mainpage/ejectdelay.ui \
    mainpage/simulatewidget.ui \
    systeminfo/ipsetwidget.ui \
    systeminfo/remotewidget.ui \
    systeminfo/machineinfowidget.ui \
    factoryset/pixeladjust.ui \
    mysortwidget/mysetdatetime.ui \
    factoryset/lampboardcfg.ui \
    mainpage/newmainpage.ui \ 
    mainpage/feedsetbygrouppage.ui \
    mainpage/feedsetbychutepage.ui \
    mysortwidget/setsvmusedialog.ui \
    factoryset/nirdotcorrectivelistwidget.ui \
    arithprofilewidget/colorsatsensesetwidget.ui \
    arithprofilewidget/colorscalesensesetwidget.ui \
    arithprofilewidget/colorhuesensesetwidget.ui \
    arithprofilewidget/discolorsensesetwidget.ui \
    mainpage/feederaotuajust.ui \
    mainpage/aiwipesetwidget.ui \
    mainpage/createnewmodewidget.ui \
    factoryset/feedersensorsetwidget.ui \
    factoryset/tempsensorsetwidget.ui \
    engineerset/aisetdialog.ui \
    factoryset/updatefpgawidget.ui \
    arithprofilewidget/hsvsensesetwidget.ui \
    arithprofilewidget/inhibitsensesetwidget.ui \
    arithprofilewidget/straightsensesetwidget.ui \
    arithprofilewidget/mildewsensessetwidget.ui \
    arithprofilewidget/outlinesensesetwidget.ui \
    arithprofilewidget/linesensesetwidget.ui \
    arithprofilewidget/budsensesetwidget.ui \
    arithprofilewidget/colorvaluesensesetwidget.ui \
    arithprofilewidget/roundsensesetwidget.ui \
    arithprofilewidget/splmildewsensesetwidget.ui \
    arithprofilewidget/whitepeanutsensesetwidget.ui \
    engineerset/doublelightsetwidget.ui \
    arithprofilewidget/shapesvmsensesetwidget.ui \
    arithprofilewidget/pepperhandlesensewidget.ui \
    systeminfo/ipcinfowidget.ui \
    factoryset/ipccamerasetwidget.ui \
    factoryset/ipcsetwidget.ui \
    factoryset/cameracolorrestore.ui \
    mainpage/ipcclassparams.ui \
    mainpage/modelchangewidget.ui \
    mysortwidget/downloaddialog.ui \
    engineerset/imagedatachartrgbwidget.ui \
    engineerset/imagedatachartwidget.ui \
    engineerset/imagedatachartdiscolorwidget.ui \
    engineerset/upgrademodelwidget.ui \
    factoryset/ipcalarmcontrol.ui \
    systeminfo/sysinfodebug.ui \
    engineerset/ipcejectwidget.ui \
    systeminfo/sysipcbadpointswidget.ui \
    systeminfo/ipcruntestwidget.ui \
    engineerset/imageinforgbwidget.ui \
    systeminfo/sysinfonet.ui \
    factoryset/updateacswidget.ui \
    arithprofilewidget/suppresslargesensesetwidget.ui \
    arithprofilewidget/wheatsproutsensesetwidget.ui \
    factoryset/ipcaisetwidget.ui \
    systeminfo/ipcclassinfowidget.ui \
    mysortwidget/mysvmsampleclass.ui \
    engineerset/imagefastcapturewidget.ui \
    mainpage/feedbanlanceaimsingle.ui \
    mainpage/feedsetbyclearuppage.ui \
    factoryset/onekeyupdatefpgawidget.ui \
    factoryset/selectupdatefilewidget.ui \
    arithprofilewidget/teacolorsensesetwidget.ui \
    systeminfo/videtectboardinfowidget.ui \
    engineerset/svmimagewidget.ui \
    systeminfo/wificonfigwidget.ui \
    mainpage/modelselectwidget.ui \
    mainpage/remoteControl.ui \
    engineerset/onekeyfixdialog.ui \
    autocontrol/plcctrlwidget.ui \
    systeminfo/autoctrlboardipsetwidget.ui\
    systeminfo/syscamcaliinfowidget.ui \
    factoryset/groupsetdialog.ui \
    factoryset/fpgamodesetwidget.ui \
    systeminfo/sysoutputwidget.ui \
    systeminfo/ipcclasssetwidget.ui \
    systeminfo/targetimagewidget.ui

# 柬埔寨语 波兰 波斯 瑞典 日语
TRANSLATIONS += resource/languages/Chinese_lan.ts \
                resource/languages/English_lan.ts \
                resource/languages/Italy_lan.ts \#意大利语
                resource/languages/Burma_lan.ts \
                resource/languages/Russian_lan.ts  \
                resource/languages/Vietnam_lan.ts \
                resource/languages/Thai_lan.ts    \
                resource/languages/Spanish_lan.ts \
                resource/languages/Chinese_tw_lan.ts \
                resource/languages/Korean_lan.ts \
                resource/languages/Turkey_lan.ts  \
                resource/languages/Cambodia_lan.ts \
                resource/languages/Poland_lan.ts \
                resource/languages/Persia_lan.ts \
                resource/languages/Ukraine_lan.ts \#乌克兰语
                resource/languages/Greek_lan.ts \#希腊
                resource/languages/Uighur_lan.ts \#维吾尔语
                resource/languages/Arabic_lan.ts \#阿拉伯语
                resource/languages/Hungary_lan.ts \#匈牙利语
                resource/languages/Bulgarian_lan.ts \#保加利亚
                resource/languages/French_lan.ts \
                resource/languages/Swedish_lan.ts \
                resource/languages/Japanese_lan.ts \
                resource/languages/Portuguese_lan.ts #葡萄牙语

QMAKE_CXXFLAGS +=  -Wno-unused-parameter \
    -Wno-unused-variable \
    -Wno-unused-but-set-variable \
    -Wno-deprecated-copy \
    -Wno-narrowing


DEFINES  += log4qt_IMPORTS \    #import log4qt
            QEXTSERIALPORT_USING_SHARED \ #import qextserialport
            JSON_DLL \ #import json
            UIKITS_SHARED_IMPORTS \

DEFINES += MY_CF_VERSION=\\\"V2.05PRE\\\" \
                       MY_TD_VERSION=\\\"V1.00\\\" \
                       LCD_WIDTH=1024
                       #LCD_HEIGHT=768
                       #LCD_WIDTH=800 \
INCLUDEPATH += $$PWD/../3rdparty/log4qt/include \
               $$PWD/../3rdparty/log4qt/include/helpers \
               $$PWD/../3rdparty/log4qt/include/spi \
               $$PWD/../3rdparty/log4qt/include/varia \
               $$PWD/../3rdparty/qextserialport/src \
               $$PWD/../uikits/src \
               $$PWD/../3rdparty/jsoncpp/json \
               $$PWD/../3rdparty/InputMethod/hangul/include

win32:{
    log4qtLIBDIR = $$PWD/../3rdparty/log4qt/lib/win32
    QextPortLIBDIR = $$PWD/../3rdparty/qextserialport/lib/win32
    uiKitsLIBDIR = $$PWD/../uikits/lib/win32
    jsoncppLIBDIR = $$PWD/../3rdparty/jsoncpp/lib/win32
    LIBS+= libwsock32 libws2_32
    LIBS += -L$$PWD/../3rdparty/svm/win -lsvmtool
    LIBS += -L$$PWD/../3rdparty/qrencode/lib/win32 -lqrencoded
}
unix:{
    SOURCES +=  bus/mqttsrv.cpp
    HEADERS +=  bus/mosquitto.h \
                bus/mosquittopp.h \
                bus/mqttsrv.h
    log4qtLIBDIR = $$PWD/../3rdparty/log4qt/lib/unix
    QextPortLIBDIR = $$PWD/../3rdparty/qextserialport/lib/unix
    uiKitsLIBDIR = $$PWD/../uikits/lib/unix
    jsoncppLIBDIR = $$PWD/../3rdparty/jsoncpp/lib/unix
    LIBS +=-lrt
    LIBS += -L$$PWD/../3rdparty/encrypt -ltime_encrypt -lmyeeprom
    #LIBS += -L$$PWD/../3rdparty/svm -lsvmtool -lWheat
    # [zcy 2026.06.15] mqtt config: 启用 MQTT 库链接
    LIBS += -L$$PWD/../3rdparty/mqtt -lmosquittopp -lmosquitto -lssl -lcrypto
    #LIBS += -L$$PWD/../3rdparty/qrencode/lib/unix -lqrencode 
    # LIBS += -L$$PWD/../3rdparty/fToFp    -lTEA_converter
    #LIBS += -L$$PWD/../3rdparty/fToFp    -lTEA
    #LIBS += -L$$PWD/../3rdparty/InputMethod/hangul/lib    -lhangul
}


CONFIG(release, debug|release): {
                                LIBS += -L$$log4qtLIBDIR -llog4qt \
                                        -L$$QextPortLIBDIR -lqextserialport \
                                        -L$$uiKitsLIBDIR -luikits \
                                        -L$$jsoncppLIBDIR -ljsoncpp
    unix:!macx|!win32:{
                    #DEFINES += breakpad       #qbreakpad 功能添加
                    contains(DEFINES,breakpad){
                                               QMAKE_CXXFLAGS += -gstabs
                                                LIBS += -L$$PWD/../3rdparty/qbreakpad/handler/lib -lbreakpad_client_arm
                                                PRE_TARGETDEPS += $$PWD/../3rdparty/qbreakpad/handler/lib/libbreakpad_client_arm.a
                                                INCLUDEPATH += $$PWD/../3rdparty/qbreakpad/handler/include/breakpad
                                                DEPENDPATH += $$PWD/../3rdparty/qbreakpad/handler/include
                    }
                }

}

else:CONFIG(debug, debug|release):{ LIBS += -L$$log4qtLIBDIR -llog4qtd \
                                            -L$$QextPortLIBDIR -lqextserialportd \
                                            -L$$uiKitsLIBDIR -luikitsd \
                                            -L$$jsoncppLIBDIR -ljsoncppd
                                    DEFINES+= SKIP_TEST
#unix:!macx|!win32: LIBS += -L$$PWD/../3rdparty/qbreakpad/handler/lib -lbreakpad_client

#unix:!macx|!win32: PRE_TARGETDEPS += $$PWD/../3rdparty/qbreakpad/handler/lib/libbreakpad_client.a
}


#RESOURCES += \
#    resource/resource.qrc

INSTALLS += target
target.path = /app

#unix:!macx|!win32: LIBS += -L$$PWD/../3rdparty/svm/ -lsvmtool

INCLUDEPATH += $$PWD/../3rdparty/svm \
               $$PWD/../3rdparty/encrypt \
               $$PWD/../3rdparty/qrencode \
               $$PWD/../3rdparty/fToFp

DEPENDPATH += $$PWD/../3rdparty/svm \
              $$PWD/../3rdparty/encrypt \
              $$PWD/../3rdparty/qrencode \
              $$PWD/../3rdparty/fToFp

#win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../3rdparty/svm/svmtool.lib
#else:unix:!macx|win32-g++: PRE_TARGETDEPS += $$PWD/../3rdparty/svm/libsvmtool.a
