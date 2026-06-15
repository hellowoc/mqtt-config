#include "global.h"


struct struCnfEngineer struCnfe, _t_struCnfe;
struct struCnfGlobal struCnfg, _t_struCnfg;
struct struCnfProfile struCnfp, _t_struCnfp;
struct struCnfFileStatus	struCnfs;
struct struShare struGsh;
struct struIpcShareInfo struIpcShare;

MiddleWidgetManager &g_MainManager()
{
    static MiddleWidgetManager manager;
    return manager;
}

Runtime &g_Runtime()
{
    static Runtime runtime;
    return runtime;
}


MyInputDialog &g_InputDialog()
{
    static MyInputDialog inputdialog;
    return inputdialog;
}

myInfoWidget &g_infoWidget()
{
    static myInfoWidget infoWidget;

    return infoWidget;
}

myInputMethod &g_inputmethod()
{
    static myInputMethod inputMethod;
    return inputMethod;
}

MyLanguage &g_myLan()
{
    static MyLanguage mylan;
    return mylan;
}
