#ifndef CUSTOM_H
#define CUSTOM_H

/* 对定制机特殊功能在不填加新机型基础上进行代码共享适配，便于后期合并移植，也便于代码管理 */
/* ！！！！！！！！！！！！！！所有新机型需要注明定制功能点!!！！！！！！！！！！！！！! */


//无客户定制功能
#define CUSTOM_NONE                     "TY"

//1、主画面当中，模型图片需要更换为我们的设备三维图片。
//2、主画面的“产量”“清灰”选项，需要去掉或者隐藏起来，不显示在主画面当中。
//3、需要增加实时图像可查看功能，查看当前设备运行下的物料图像状态，此图像不做保存处理。
//4、开机自检后，需要自动进入此界面。
#define CUSTOM_YANSI                    "YS"

#define CUSTOM_TEST     "test"

#define CUSTOM  CUSTOM_TEST

#endif // CUSTOM_H

