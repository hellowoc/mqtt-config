#include "basewidget.h"
#include "global.h"
#include "seteditvaluewidget.h"
basewidget::basewidget(QWidget *parent) :BaseUi(parent)
{
    m_pGroupWidget = NULL;
    m_pChuteWidget = NULL;
    m_CurrentPageId = 0;
    m_PreviousPageId = 0;
    for(int i = 0;i < MAX_VIEW;i++){
        m_pViewBtn[i] = NULL;
    }
    m_pLayerBtn[0] = NULL;
    m_pLayerBtn[1] = NULL;
    m_pGroupLbl = NULL;
    m_chuteShowNum = 1;
    m_chuteShowNum = 0;
}

basewidget::~basewidget()
{
}

void basewidget::setSlotConnect()
{
    for(int i = 0;i < MAX_VIEW;i++){
        if(m_pViewBtn[i]){
            m_pViewBtn[i]->setCheckable(true);
            connect(m_pViewBtn[i],SIGNAL(clicked(bool)),this,SLOT(viewClickedSlots()));
        }
    }

    if(m_pViewBtn[0])
        m_pViewBtn[0]->setObjectName("frontMainViewBtn");
    if(m_pViewBtn[1])
        m_pViewBtn[1]->setObjectName("rearMainViewBtn");
    if(m_pViewBtn[2])
        m_pViewBtn[2]->setObjectName("frontViceViewBtn");
    if(m_pViewBtn[3])
        m_pViewBtn[3]->setObjectName("rearViceViewBtn");
    if(m_pViewBtn[4])
        m_pViewBtn[4]->setObjectName("frontMainViewBtn");
    if(m_pViewBtn[5])
        m_pViewBtn[5]->setObjectName("rearMainViewBtn");
    if(m_pViewBtn[6])
        m_pViewBtn[6]->setObjectName("frontViceViewBtn");
    if(m_pViewBtn[7])
        m_pViewBtn[7]->setObjectName("rearViceViewBtn");
    for(int i = 0;i < 2;i++){
        if(m_pLayerBtn[i]){
            m_pLayerBtn[i]->setCheckable(true);
            connect(m_pLayerBtn[i],SIGNAL(clicked(bool)),this,SLOT(layerClickedSlots()));
        }
    }
    if(m_pGroupWidget){
        connect(m_pGroupWidget,SIGNAL(clickedButton(int)),this,SLOT(groupBtnClick(int)));
    }
}

void basewidget::updateViewButtonVisible()
{

    if(CUSTOM == CUSTOM_YANSI)
    {
        for(int i = 0; i < MAX_VIEW; i++)
        {
            if(struCnfg.struLayerInfo[0].nViewIsUsed[i])
            {
                struGsh.nView = i;
                break;
            }

        }
    }


    if(m_pViewBtn[0] == NULL)
        return;//当前界面不支持view的切换
    for(int i = 0;i < MAX_VIEW;i++){
        m_pViewBtn[i]->setChecked(false);
        m_pViewBtn[i]->setVisible((bool)struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[i]);
    }
    m_pViewBtn[struGsh.nView]->setChecked(true);

    m_pViewBtn[0]->setText(g_myLan().ids_fm);
    m_pViewBtn[1]->setText(g_myLan().ids_bm);
    m_pViewBtn[2]->setText(g_myLan().ids_fs);
    m_pViewBtn[3]->setText(g_myLan().ids_bs);
    if (struCnfg.nMachineType == MACHINE_LD2_ZL_GP){
        m_pViewBtn[0]->setText(g_myLan().lower_layer+g_myLan().ids_fm);
        m_pViewBtn[1]->setText(g_myLan().lower_layer+g_myLan().ids_bm);
        m_pViewBtn[2]->setText(g_myLan().lower_layer+g_myLan().ids_fs);
        m_pViewBtn[3]->setText(g_myLan().lower_layer+g_myLan().ids_bs);
        m_pViewBtn[4]->setText(g_myLan().upper_layer+g_myLan().ids_fm);
        m_pViewBtn[5]->setText(g_myLan().upper_layer+g_myLan().ids_bm);
        m_pViewBtn[6]->setText(g_myLan().upper_layer+g_myLan().ids_fs);
        m_pViewBtn[7]->setText(g_myLan().upper_layer+g_myLan().ids_bs);
    }
}

void basewidget::updateViewBtnState()
{
    if(struCnfg.nDPMachine == 0)
    {
        for(int i=0;i<MAX_VIEW;i++){
            if(struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[i]){
                if(i % 2)
                    m_pViewBtn[i]->setEnabled(!(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT));
                else
                    m_pViewBtn[i]->setEnabled(true);
            }
        }
    }
    else
    {
        if(struGsh.isInModelchangePage){
            if(struCnfp.nDualLayerModelSame==1) { //整机相同
                for(int i=1;i<MAX_VIEW;i++){
                    if(struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[i])
                        m_pViewBtn[i]->setEnabled(false);


                }
            }
            else if(struCnfp.nDualLayerModelSame==2) // 前后相同
            {
                for(int i=0;i<MAX_VIEW;i++){
                    if(struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[i]){
                        if(i % 2)
                            m_pViewBtn[i]->setEnabled(false);
                        else
                            m_pViewBtn[i]->setEnabled(true);
                    }
                }
            }
            else if(struCnfp.nDualLayerModelSame==3 ||struCnfp.nDualLayerModelSame==4 ) // 前后相同
            {
                for(int i=0;i<MAX_VIEW;i++){
                    if(struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[i]){

                            m_pViewBtn[i]->setEnabled(true);
                    }
                }
            }
        }
        else if( struGsh.isInIpcClassPage == 1)
        {
            if(struCnfp.nIpcCameraClassSameMode == 1)
            {
                for(int i=0;i<MAX_VIEW;i++){
                    if(struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[i]){
                        if(i % 2)
                            m_pViewBtn[i]->setEnabled(false);
                        else
                            m_pViewBtn[i]->setEnabled(true);
                    }
                }
            }
            else  if(struCnfp.nIpcCameraClassSameMode == 2)
            {
                for(int i=0;i<MAX_VIEW;i++){
                    if(struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[i]){
                        m_pViewBtn[i]->setEnabled(i == 0);
                    }
                }
            }else {

                for(int i=0;i<MAX_VIEW;i++){
                    if(struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[i]){
                        m_pViewBtn[i]->setEnabled(true);
                    }
                }
            }
        }
        else{
            for(int i=0;i<MAX_VIEW;i++){
                if(struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[i]){
                    if(i % 2)
                        m_pViewBtn[i]->setEnabled(!(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT));
                    else
                        m_pViewBtn[i]->setEnabled(true);
                }
            }
        }
    }
}

void basewidget::updateChuteNum()
{
    if(m_pChuteWidget){
        m_pChuteWidget->setTitle(g_myLan().ids_channel);
        m_chuteShowNum = struGsh.nUnit+1;
        m_pChuteWidget->setValue(m_chuteShowNum);
        //选择交叉相机功能时，主视和辅视相机数目不同
//        if(1 == struCnfg.bCrossCam)
//        {
            //前主0，后主1，前辅2，后辅3
//            if(1 == struCnfg.bCrossMode1)//主视比辅视多1个相机
//            {
//                if(0 == struGsh.nView || 1 == struGsh.nView || 4 == struGsh.nView || 5 == struGsh.nView)
//                {
//                    m_pChuteWidget->setParams(this,1,struCnfg.nChuteTotal,chute_change,&m_chuteShowNum);
//                }
//                else if(2 == struGsh.nView || 3 == struGsh.nView || 6 == struGsh.nView || 7 == struGsh.nView)
//                {
//                    m_pChuteWidget->setParams(this,1,struCnfg.nChuteTotal-1,chute_change,&m_chuteShowNum);
//                    if(m_chuteShowNum > struCnfg.nChuteTotal)
//                    {
//                        m_chuteShowNum = struCnfg.nChuteTotal;
//                        struGsh.nUnit = m_chuteShowNum - 1;
//                        m_pChuteWidget->setValue(m_chuteShowNum);
//                    }
//                }
//            }
//            else if (1 == struCnfg.bCrossMode2) //辅视比主视多1个相机
//            {
//                if(0 == struGsh.nView || 1 == struGsh.nView || 4 == struGsh.nView || 5 == struGsh.nView)
//                {
//                    m_pChuteWidget->setParams(this,1,struCnfg.nChuteTotal,chute_change,&m_chuteShowNum);
//                    if(m_chuteShowNum > struCnfg.nChuteTotal)
//                    {
//                        m_chuteShowNum = struCnfg.nChuteTotal;
//                        struGsh.nUnit = m_chuteShowNum - 1;
//                        m_pChuteWidget->setValue(m_chuteShowNum);
//                    }
//                }
//                else if(2 == struGsh.nView || 3 == struGsh.nView || 6 == struGsh.nView || 7 == struGsh.nView)
//                {
//                    m_pChuteWidget->setParams(this,1,struCnfg.nChuteTotal+1,chute_change,&m_chuteShowNum);
//                }
//            }
//        }
//        else
        {
            m_pChuteWidget->setParams(this,1,struCnfg.struLayerInfo[struGsh.nLayer].nViewUnitTotal[struGsh.nView],chute_change,&m_chuteShowNum);
        }
        if(m_chuteShowNum > struCnfg.struLayerInfo[struGsh.nLayer].nViewUnitTotal[struGsh.nView])
        {
            m_chuteShowNum = struCnfg.struLayerInfo[struGsh.nLayer].nViewUnitTotal[struGsh.nView];
        }
        struGsh.nUnit = m_chuteShowNum - 1;
        m_pChuteWidget->setValue(m_chuteShowNum);
    }
}

void basewidget::updateLayerBtnState()
{
    if(m_pLayerBtn[0]){
        for(int i = 0;i < 2;i++){
            m_pLayerBtn[i]->setChecked(i == struGsh.nLayer);
            if(struCnfg.nLayerTotal >= 2){
                m_pLayerBtn[i]->show();
            }else{
                m_pLayerBtn[i]->hide();
            }
        }
        m_pLayerBtn[0]->setText(g_myLan().upper_layer);
        m_pLayerBtn[1]->setText(g_myLan().lower_layer);
        m_pLayerBtn[0]->setObjectName("mainLayerBtn");
        m_pLayerBtn[1]->setObjectName("viceLayerBtn");
    }
}

void basewidget::updateGroupWidget()
{
    if(m_pGroupWidget){
        groupItems.clear();
        QStringList groupNumlist;
        groupNumlist<<g_myLan().ids_one<<g_myLan().ids_two<<g_myLan().ids_three<<g_myLan().ids_four<<g_myLan().ids_five;
        for(int i=0;i<struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].nIdentifyGroupTotal;i++)
        {
            GroupItem item;
            item.itemName = groupNumlist[i];
            item.tag = i;
            item.subTag = 0;
            groupItems.append(item);
        }


        if(groupItems.count() > 0)
        {
            int findIdx = 0;
            for(int idx=0;idx<groupItems.count();idx++)
            {
                if(groupItems[idx].tag == struGsh.nMatGroup)
                {
                    findIdx = idx;
                    break;
                }
            }
            if(struGsh.nMatGroup >= groupItems.count())
            {
                struGsh.nMatGroup = 0;
            }

            m_pGroupWidget->setGroupContents(groupItems);
            m_pGroupWidget->setSelected_pos(findIdx);
        }
    }
}

void basewidget::updateGroupWidgetState()
{
    if(struGsh.isInModelchangePage == 1)
    {
        if(m_pGroupWidget){

            if(struCnfp.nDualLayerModelSame == 1 || struCnfp.nDualLayerModelSame == 3)
            {
                struGsh.nMatGroup = 0;
                m_pGroupWidget->setEnabled(false);
            }
            else
            {
                m_pGroupWidget->setEnabled(true);
            }

            m_pGroupWidget->setSelected_pos(struGsh.nMatGroup);
        }
    }
    else if( struGsh.isInIpcClassPage == 1)
    {
        m_pGroupWidget->setEnabled(struCnfp.nIpcCameraClassSameMode == 0 || struCnfp.nIpcCameraClassSameMode == 1);
    }
    else
    {
        if(m_pGroupWidget){
            m_pGroupWidget->setEnabled(!(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_SECOND_ALIKE_FIRST));
            if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_SECOND_ALIKE_FIRST){
                struGsh.nMatGroup = 0;
            }
            m_pGroupWidget->setSelected_pos(struGsh.nMatGroup);
        }
    }
}

void basewidget::updateGroupLabelShow()
{
    if(m_pGroupLbl){
        if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_ALONE ||  struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT){
            QStringList grouplist;
            grouplist<<g_myLan().ids_one<<g_myLan().ids_two<<g_myLan().ids_three<<g_myLan().ids_four<<g_myLan().ids_five;
            m_pGroupLbl->setText(grouplist.at(struGsh.nMatGroup));
        }else{
            m_pGroupLbl->hide();
        }
    }else{
        logger()->error("group lbl is NULL");
    }
}

void basewidget::viewClickedSlots()
{
    for(int i=0; i<MAX_VIEW; i++ )
        if(sender() == m_pViewBtn[i])
        {
            onViewClicked(i);
            updateGroupWidget();
            updateGroupWidgetState();
            OnGroupChange(i);
            break;
        }
}

void basewidget::groupBtnClick(int index)
{
    if(index < groupItems.count() && index >= 0)
    {
        if(struGsh.nMatGroup != groupItems[index].tag)
        {
            struGsh.nMatGroup = groupItems[index].tag;
            OnGroupChange(index);
        }
    }
}

void basewidget::layerClickedSlots()
{
    for(int i=0; i<2; i++ )
        if(sender() == m_pLayerBtn[i])
        {
            onLayerClicked(i);
            break;
        }
}


void basewidget::onViewClicked(int view)
{
    m_pViewBtn[struGsh.nView]->setChecked(false);
    struGsh.nView = view;
    m_pViewBtn[view]->setChecked(true);
    myFlow.updateCameraTimeDivisionMode(struGsh.nLayer,struGsh.nView);
    myFlow.updateCurViewImgHight();
    OnViewChange(view);

    //    if(1 == struCnfg.bCrossCam)
    //    {
    //        updateChuteNum();
    //    }
    updateChuteNum();

    //    if(struGsh.nView == 0 ||struGsh.nView == 1 ||struGsh.nView == 4 ||struGsh.nView == 5){
    //        myFlow.imgHeight = 400 ;
    //    }
    //    else{
    //        myFlow.imgHeight = 416 ;
    //    }

}

void basewidget::onLayerClicked(int layer)
{
    m_pLayerBtn[struGsh.nLayer]->setChecked(false);
    struGsh.nLayer = layer;
    m_pLayerBtn[layer]->setChecked(true);
    updateViewButtonVisible();
    OnLayerChange();
}

void basewidget::showPage(bool bshow)
{
    if(bshow){
        updateViewButtonVisible();

        updateChuteNum();

        updateGroupWidget();

        updateLayerBtnState();
    }
}

void basewidget::returnBack()
{
    if(struGsh.nPageSm == SM_FACTORY_SET_WIDGET) //厂家模式返回需要进入校正后模式
    {
        struGsh.bFlagAutowipe = 1;  //恢复允许清灰
        //        if(struGsh.bCaliBiasEnable == 0 || struGsh.bCaliGainEnable == 0){
        //            struGsh.bCaliBiasEnable = 1;
        //            struGsh.bCaliGainEnable = 1;
        //            myFlow.cameraWaveCaliEnable();
        //        }
    }
    g_MainManager().ShowWidget(m_PreviousPageId, true);
    //    if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT)
    //    {
    //        struGsh.nView = 0;
    //    }
    if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME)
    {
        struGsh.nView = 0;
    }
    if(struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT)
    {
        //        struGsh.nView = (struGsh.nView >1)?(struGsh.nView/2+1):(struGsh.nView/2);

        if (struGsh.nView == 0 || struGsh.nView == 1 )
            struGsh.nView =  0;
        if (struGsh.nView == 2 || struGsh.nView == 3 )
            struGsh.nView =  2;
        if (struGsh.nView == 4 || struGsh.nView == 5 )
            struGsh.nView =  4;
        if (struGsh.nView == 6 || struGsh.nView == 7 )
            struGsh.nView =  6;
    }
}

void basewidget::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_DATA_CHANGED){
        if(msg2 == chute_change){
            struGsh.nUnit = m_chuteShowNum-1;
            OnChuteChange();
        }
    }
}

void basewidget::OnViewChange(int view)
{

}

void basewidget::OnGroupChange(int index)
{

}

void basewidget::OnChuteChange()
{

}

void basewidget::OnLayerChange()
{

}

void basewidget::setAndSendParams()
{
    myFlow.materialParamsCopySendAssemble(struGsh.nLayer, struGsh.nView, struGsh.nMatGroup, false, struGsh.nArithIndex, 0,true);
}

