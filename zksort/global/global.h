#ifndef GLOBAL_H
#define GLOBAL_H

#include "middlewidgetmanager.h"
#include "global/runtime.h"
#include "global/mylanguage.h"
#include "global/globalparams.h"
#include "global/globalflow.h"
#include "mysortwidget/myinputdialog.h"
#include "mywid.h"
#include "myinputmethod.h"

MiddleWidgetManager &g_MainManager();
Runtime &g_Runtime();
MyInputDialog &g_InputDialog();
myInfoWidget &g_infoWidget();
myInputMethod &g_inputmethod();
MyLanguage &g_myLan();
#endif // GLOBAL_H
