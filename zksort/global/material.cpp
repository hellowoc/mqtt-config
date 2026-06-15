#include "globalparams.h"
#include "globalflow.h"
#include "TEA.h"




/***************************************************************************************************
    名称：      发送给FPGA的物料参数的组包
    功能：      根据物料算法，物料识别参数发送包组包
    nCmd:      命令编码
     nLayerId:  识别组所在的层编号
    nGroupId： 识别组的组号
    nIndex:    使用同一个结构体的几个并列算法的数组标号，如灰度杂质A、灰度杂质B、色差杂质A、色差杂质B、差分算法
***************************************************************************************************/
void GlobalFlow::materialIdentifyParamsSend(int nCmd, int nLayerId, int nViewId, int nGroupId, int nIndex)
{
    int k;
    int camNo = 0;
    int nInt = 0, nUnitAddr;
    int nMin = 0, nMax = 0;
    int nMain = 0, nVice = 0, resGreyColor, tmpDiffColor;

// add new
    int arithIndex;
    int nParamsLimit;
    int nSignSvm[3];    // svm符号位
    int nSignCons, nSvmCons;
    char data[SEND_PACKET_DATA_SIZE];
    int unitSensAddr = 0;   //单个相机的灵敏度索引， 0：全部，或左 1：右2
    int tmpMaxSensValue;
    int tmpEnable, tmpLEnable, tmpREnable, tmpSens, tmpSvmCons;
    int highHue, lowHue;
    int tmpSensUp = 0;
    int tmpSensLow = 0;
    int spliceType = 0; //0-不分割， 1-通道左边， 2-通道右边

    float value1, value2;

//    if( (nViewId > 1) && (struCnfg.bCrossMode2==1) )
//    {
//        camNo = struCnfg.struLayerInfo[ nLayerId].stuViewInfo[nViewId].stuIdenGupInfo[nGroupId].nGroupUnitTotal + 1;
//    }
//    else if( (nViewId > 1) && (struCnfg.bCrossMode1==1) )
//    {
//        camNo = struCnfg.struLayerInfo[ nLayerId].stuViewInfo[nViewId].stuIdenGupInfo[nGroupId].nGroupUnitTotal - 1;
//    }
//    else
    {
        camNo = struCnfg.struLayerInfo[ nLayerId].stuViewInfo[nViewId].stuIdenGupInfo[nGroupId].nGroupUnitTotal;
    }


    for (k = 0; k < camNo; k++) {
       nUnitAddr =  getIdentifyGroupAddr( nLayerId, nViewId, nGroupId, k);  //针对通道分割机型，只发送左半边,20200812,
       spliceType = struCnfg.struLayerInfo[nLayerId].stuViewInfo[nViewId].stuIdenGupInfo[nGroupId].nUnitGupSplice[k];

//       if(nUnitAddr/128 > 0)
//       {
//           continue;
//       }
//       if((struCnfg.nMachineSpliceType != MACHINE_SPLICE_TYPE_NULL)&&(struCnfg.struLayerInfo[nLayerId].stuViewInfo[nViewId].stuIdenGupInfo[nGroupId].nUnitGupSplice[k]!=0))
//       {
//            spliceType  = 1;
//       }
//       else
//       {
//           spliceType = 0;
//       }

       if(nUnitAddr/64 == 0)
       {
            unitSensAddr = 0;
       }
       else if(nUnitAddr/64 == 1)   //相机分割 左半边
       {
            unitSensAddr = 0;
       }
       else if(nUnitAddr/64 == 2)   //相机分割 右半边
       {
            unitSensAddr = 1;
       }


       memset(data, 0, SEND_PACKET_DATA_SIZE);
       switch (nCmd) {
       case CMD_CAMERA_ARITH_GREY_1:  //灰度算法1
       case CMD_CAMERA_ARITH_GREY_2:  //灰度算法2
       case CMD_CAMERA_ARITH_GREY_3:  //灰度算法3， 选玻璃算法
       case CMD_CAMERA_ARITH_GREY_4:  //灰度算法4,  腹白算法， 可见+850相机用
       case CMD_CAMERA_ARITH_GREY_5:  //灰度算法5， 选恶杂, 1450相机用
           /* !!! 初始化各算法切边 */
           struCnfp.nGroupArithEdgeEnable[nLayerId][nViewId][nGroupId][ARITH_GREY_A+nIndex] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].nEdge;
           if(!struGsh.allArithParamsResetCnts)  break;

           /* 1. 使能+主色 */
           tmpMaxSensValue = ARITH_GREY_MAX_SENS;
           tmpEnable = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].nArithEnable;
           nMain = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].nMain;

           /* 互换黑白与红外顺序 */
           nMain = myFlow.changeColorBlackWhite2Nir(nMain);

//           if(spliceType == 0)
//           {
//                data[0] = (tmpEnable<<7)|(nMain<<4);
//           }
//           else
//           {
//               tmpEnable  = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].nDispEnable;
//               tmpLEnable = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].nArithEnable;
//               tmpREnable = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId+1].struGreyColor[nIndex].nArithEnable;
//               data[0] = (tmpEnable<<7)|(nMain<<4)|(tmpREnable<<1)|tmpLEnable;
//           }

            data[0] = (tmpEnable<<7)|(nMain<<4);

           /* 2. 尺寸 */
           data[1] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].nRow-1;
           /* 3. 纯度 */
           data[2] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].nPercent-1)/256;
           data[3] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].nPercent-1)%256;

           /* 4. 灵敏度上阈/下阈 */
           if (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].nMode == 1) {
                nMin = tmpMaxSensValue-(struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].nSensUp+struCnfp.struUnitSens[nLayerId][nViewId][nUnitAddr][unitSensAddr].greyColor[nIndex]);
                nMin = (nMin <= 0) ? 0 : nMin;
                nMax = tmpMaxSensValue;
           }
           else {
               nMin = 0;
               nMax = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].nSensLow+struCnfp.struUnitSens[nLayerId][nViewId][nUnitAddr][unitSensAddr].greyColor[nIndex];
               nMax = (nMax > tmpMaxSensValue) ? tmpMaxSensValue : nMax;
           }
           data[4] = nMin;
           data[5] = nMax;

           /* 6. 保留算法 */
           /* 红外1450没有保留 */
//           if(struCnfg.struLayerInfo[nLayerId].nViewSensorType[nViewId]==SENSOR_C1)
//           {
//               mySerial.pushCom1CmdData(nCmd, nInt, struCnfg.struLayerInfo[nLayerId].nViewBoardType[nViewId], nUnitAddr, data, 3);
//               break;
//           }


           if((struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].nResType == 2)
                   ||(nCmd == CMD_CAMERA_ARITH_GREY_3)||(nCmd == CMD_CAMERA_ARITH_GREY_4))      //比例保留, 灰度3,灰度4
           {
                data[6] = ((int)(struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].nPropResSens*0.01*ARITH_PROPOTION_MAX_SENS+0.5))/256;
                data[7] = ((int)(struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].nPropResSens*0.01*ARITH_PROPOTION_MAX_SENS+0.5))%256;
           }
           else
           {
               resGreyColor = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].struResGreyColor.nColor1;
               resGreyColor = myFlow.changeColorBlackWhite2Nir(resGreyColor);
               tmpDiffColor = myFlow.getDiffArithColor(struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].struResGreyColor.nResDiffColor, false);
               nMain = tmpDiffColor>>4;
               nVice = tmpDiffColor%16;

               /* 灰度保留 */
               data[6] = resGreyColor<<5;
               data[7] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].struResGreyColor.nSensLow1;
               data[8] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].struResGreyColor.nSensUp1;

               /* 色差保留 */
               tmpMaxSensValue = ARITH_COLOR_MAX_SENS;
               data[9] = (nMain<<5)|(nVice<<2);
               nMin = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].struResGreyColor.nSensLow2;
               nMax = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].struResGreyColor.nSensUp2;
               nMin = (nMin <= 0) ? 0 : nMin;
               nMax = (nMax > tmpMaxSensValue) ? tmpMaxSensValue : nMax;

               data[10] = nMin/4;   //灵敏度下限高8位
               data[11] = ((nMin%4)<<6)|(nMax/256);   //灵敏度下限低2位|灵敏度上限高2位
               data[12] = nMax%256;
           }

           mySerial.pushCom1CmdData(nCmd, nInt, struCnfg.struLayerInfo[nLayerId].nViewBoardType[nViewId], nUnitAddr, data, 3);

           //灰度1比例使能
           tmpEnable  = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].nPropotionEnable;
           if(nCmd == CMD_CAMERA_ARITH_GREY_1){
               memset(data, 0, SEND_PACKET_DATA_SIZE);
               /* 1.比例使能 */
               data[0] = tmpEnable<<7;
               /* 2. 算法类型（废弃） */
               data[1] = 0;
               /* 3. 精度下阈 */
               data[2] = ((int)(struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].fPropotionSensLow*0.01*ARITH_PROPOTION_MAX_SENS+0.5))/256;
               data[3] = ((int)(struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].fPropotionSensLow*0.01*ARITH_PROPOTION_MAX_SENS+0.5))%256;
               /* 4. 物料下限 */
               data[4] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].nPropotionParaSensLow)/256;
               data[5] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].nPropotionParaSensLow)%256;
               /* 5. 精度上阈 */
               data[6] = ((int)(struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].fPropotionSensUp*0.01*ARITH_PROPOTION_MAX_SENS+0.5))/256;
               data[7] = ((int)(struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].fPropotionSensUp*0.01*ARITH_PROPOTION_MAX_SENS+0.5))%256;

               mySerial.pushCom1CmdData(CMD_CAMERA_ARITH_SCALE_GREY_1, nInt, struCnfg.struLayerInfo[nLayerId].nViewBoardType[nViewId], nUnitAddr, data, 3);
           }

           //算法切边
           memset(data, 0, SEND_PACKET_DATA_SIZE);
           /* 1. 序号，默认为0 */
           data[0] = 0;
           /* 2. Byte1-3：色差1-色差3切边 Byte4-6：灰度1-灰度3切边 Byte7-9：智能1-智能3切边 */
           data[1] = struCnfp.nGroupArithEdgeEnable[nLayerId][nViewId][nGroupId][ARITH_DISCOLOR_A];
           data[2] = struCnfp.nGroupArithEdgeEnable[nLayerId][nViewId][nGroupId][ARITH_DISCOLOR_B];
           data[3] = struCnfp.nGroupArithEdgeEnable[nLayerId][nViewId][nGroupId][ARITH_CROSS];
           data[4] = struCnfp.nGroupArithEdgeEnable[nLayerId][nViewId][nGroupId][ARITH_GREY_A];
           data[5] = struCnfp.nGroupArithEdgeEnable[nLayerId][nViewId][nGroupId][ARITH_GREY_B];
           data[6] = struCnfp.nGroupArithEdgeEnable[nLayerId][nViewId][nGroupId][ARITH_GREY_C];
           data[7] = struCnfp.nGroupArithEdgeEnable[nLayerId][nViewId][nGroupId][ARITH_SVM_A];
           data[8] = struCnfp.nGroupArithEdgeEnable[nLayerId][nViewId][nGroupId][ARITH_SVM_B];
           data[9] = struCnfp.nGroupArithEdgeEnable[nLayerId][nViewId][nGroupId][ARITH_SVM_C];
           data[10] = struCnfp.nGroupArithEdgeEnable[nLayerId][nViewId][nGroupId][ARITH_SVM_D];

           mySerial.pushCom1CmdData(CMD_CAMERA_ARITH_EDGE_CUT, nInt, struCnfg.struLayerInfo[nLayerId].nViewBoardType[nViewId], nUnitAddr, data, 3);

           // 延时选择
           memset(data, 0, SEND_PACKET_DATA_SIZE);
           for(int m=0; m<4; m++){
               data[0] += struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].delayType<<m;
           }
           mySerial.pushCom1CmdData(CMD_CAMERA_ARITH_DELAY_TYPE, nInt, struCnfg.struLayerInfo[nLayerId].nViewBoardType[nViewId], nUnitAddr, data, 3);

           break;
       case CMD_CAMERA_ARITH_DISCOLOR_1:  //色差算法1
       case CMD_CAMERA_ARITH_DISCOLOR_2:  //色差算法2
       case CMD_CAMERA_ARITH_DISCOLOR_3:  //色差算法3
           /* !!! 初始化各算法切边 */
           struCnfp.nGroupArithEdgeEnable[nLayerId][nViewId][nGroupId][ARITH_GREY_A+nIndex] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].nEdge;
           if(!struGsh.allArithParamsResetCnts)  break;

            /* 1. 算法使能+主色+辅色 */
           tmpMaxSensValue = ARITH_COLOR_MAX_SENS;
//           if(spliceType == 0)
//           {
//                tmpEnable = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].nArithEnable;
//           }
//           else
//           {
//                tmpEnable = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].nDispEnable;
//                tmpLEnable = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].nArithEnable;
//                tmpREnable = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId+1].struGreyColor[nIndex].nArithEnable;
//                data[15] = (tmpREnable<<1)|tmpLEnable;
//           }

            tmpEnable = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].nArithEnable;
           tmpDiffColor = myFlow.getDiffArithColor(struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].nDiffColor, false);
           nMain = tmpDiffColor>>4;
           nVice = tmpDiffColor%16;

//           nMain = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].nMain;
//           nVice = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].nVice;
           data[0] = (tmpEnable<<7)|(nMain<<4|(nVice<<1));

           /* 2. 尺寸 */
           data[1] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].nRow-1;
           /* 3. 纯度 */
           data[2] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].nPercent-1)/256;
           data[3] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].nPercent-1)%256;

           /* 4. 灵敏度上阈/下阈 */
           if (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].nMode == 1) {
                nMin = tmpMaxSensValue-(struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].nSensUp+struCnfp.struUnitSens[nLayerId][nViewId][nUnitAddr][unitSensAddr].greyColor[nIndex]);
                nMin = (nMin <= 0) ? 0 : nMin;
                nMax = tmpMaxSensValue;
           }
           else {
                nMin = 0;
                nMax = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].nSensLow+struCnfp.struUnitSens[nLayerId][nViewId][nUnitAddr][unitSensAddr].greyColor[nIndex];
                nMax = (nMax > tmpMaxSensValue) ? tmpMaxSensValue : nMax;
           }
           data[4] = nMin/4;
           data[5] = ((nMin%4)<<6)|(nMax/256);
           data[6] = nMax%256;
           data[7] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].lightLimit;

           /* 5. 保留算法颜色 */
           tmpDiffColor = myFlow.getDiffArithColor(struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].struResGreyColor.nResDiffColor, false);
           nMain = tmpDiffColor>>4;
           nVice = tmpDiffColor%16;

           resGreyColor = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].struResGreyColor.nColor1;
           /* 互换黑白与红外顺序 */
           resGreyColor = myFlow.changeColorBlackWhite2Nir(resGreyColor);

           data[8] = resGreyColor<<5;
           data[11] = (nMain<<5)|(nVice<<2);

           /* 灰度保留 */
           tmpMaxSensValue = ARITH_GREY_MAX_SENS;
           nMin = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].struResGreyColor.nSensLow1;
           nMax = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].struResGreyColor.nSensUp1;
           nMin = (nMin <= 0) ? 0 : nMin;
           nMax = (nMax > tmpMaxSensValue) ? tmpMaxSensValue : nMax;
           data[9] = nMin;
           data[10] = nMax;

           /* 色差保留 */
           tmpMaxSensValue = ARITH_COLOR_MAX_SENS;
           nMin = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].struResGreyColor.nSensLow2;
           nMax = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].struResGreyColor.nSensUp2;
           nMin = (nMin <= 0) ? 0 : nMin;
           nMax = (nMax > tmpMaxSensValue) ? tmpMaxSensValue : nMax;

           data[12] = nMin/4;                   //灵敏度下限高8位
           data[13] = ((nMin%4)<<6)|(nMax/256); //灵敏度下限低2位|灵敏度上限高2位
           data[14] = nMax%256;                 //灵敏度上限低8位
           mySerial.pushCom1CmdData(nCmd, nInt, struCnfg.struLayerInfo[nLayerId].nViewBoardType[nViewId], nUnitAddr, data, 3);

           tmpEnable  = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].nPropotionEnable;
           if(nCmd == CMD_CAMERA_ARITH_DISCOLOR_1 ){
               memset(data, 0, SEND_PACKET_DATA_SIZE);
               /* 1.比例使能 */
                data[0] = tmpEnable<<7;
               /* 2. 算法类型（废弃） */
               data[1] = 0;
               /* 3. 精度下阈 */
               data[2] = ((int)(struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].fPropotionSensLow*0.01*ARITH_PROPOTION_MAX_SENS+0.5))/256;
               data[3] = ((int)(struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].fPropotionSensLow*0.01*ARITH_PROPOTION_MAX_SENS+0.5))%256;
               /* 4. 物料下限 */
               data[4] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].nPropotionParaSensLow)/256;
               data[5] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].nPropotionParaSensLow)%256;
               /* 5. 精度上阈 */
               data[6] = ((int)(struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].fPropotionSensUp*0.01*ARITH_PROPOTION_MAX_SENS+0.5))/256;
               data[7] = ((int)(struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[nIndex].fPropotionSensUp*0.01*ARITH_PROPOTION_MAX_SENS+0.5))%256;

               mySerial.pushCom1CmdData(CMD_CAMERA_ARITH_SCALE_DISCOLOR_1, nInt, struCnfg.struLayerInfo[nLayerId].nViewBoardType[nViewId], nUnitAddr, data, 3);
           }

           //算法切边
           /* 1. 序号，默认为0 */
           memset(data, 0, SEND_PACKET_DATA_SIZE);
           data[0] = 0;
           /* 2. Byte1-3：色差1-色差3切边 Byte4-6：灰度1-灰度3切边 Byte7-9：智能1-智能3切边 */
           data[1] = struCnfp.nGroupArithEdgeEnable[nLayerId][nViewId][nGroupId][ARITH_DISCOLOR_A];
           data[2] = struCnfp.nGroupArithEdgeEnable[nLayerId][nViewId][nGroupId][ARITH_DISCOLOR_B];
           data[3] = struCnfp.nGroupArithEdgeEnable[nLayerId][nViewId][nGroupId][ARITH_CROSS];
           data[4] = struCnfp.nGroupArithEdgeEnable[nLayerId][nViewId][nGroupId][ARITH_GREY_A];
           data[5] = struCnfp.nGroupArithEdgeEnable[nLayerId][nViewId][nGroupId][ARITH_GREY_B];
           data[6] = struCnfp.nGroupArithEdgeEnable[nLayerId][nViewId][nGroupId][ARITH_GREY_C];
           data[7] = struCnfp.nGroupArithEdgeEnable[nLayerId][nViewId][nGroupId][ARITH_SVM_A];
           data[8] = struCnfp.nGroupArithEdgeEnable[nLayerId][nViewId][nGroupId][ARITH_SVM_B];
           data[9] = struCnfp.nGroupArithEdgeEnable[nLayerId][nViewId][nGroupId][ARITH_SVM_C];
           data[10] = struCnfp.nGroupArithEdgeEnable[nLayerId][nViewId][nGroupId][ARITH_SVM_D];

           mySerial.pushCom1CmdData(CMD_CAMERA_ARITH_EDGE_CUT, nInt, struCnfg.struLayerInfo[nLayerId].nViewBoardType[nViewId], nUnitAddr, data, 3);
           // 延时选择
           memset(data, 0, SEND_PACKET_DATA_SIZE);
           for(int m=0; m<3; m++){
               data[1] += struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struGreyColor[4+m].delayType<<m;
           }
           mySerial.pushCom1CmdData(CMD_CAMERA_ARITH_DELAY_TYPE, nInt, struCnfg.struLayerInfo[nLayerId].nViewBoardType[nViewId], nUnitAddr, data, 3);

           break;
       case CMD_CAMERA_ARITH_SCALE:
       case CMD_CAMERA_ARITH_SCALE_2:
           if(!struGsh.allArithParamsResetCnts)  break;
            /* 1. 大小算法 */
//           if(spliceType == 0)
//           {
//                data[0] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struBigSmall[nIndex].nArithEnable;
//           }
//           else
//           {
//                data[0] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struBigSmall[nIndex].nDispEnable;
//                tmpLEnable = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struBigSmall[nIndex].nArithEnable;
//                tmpREnable = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId+1].struBigSmall[nIndex].nArithEnable;
//                data[15] = (tmpREnable<<1)|tmpLEnable;
//           }

           data[0] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struBigSmall[nIndex].nArithEnable;

           data[1] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struBigSmall[nIndex].nMode;
           data[2] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struBigSmall[nIndex].nLimit/256;
           data[3] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struBigSmall[nIndex].nLimit%256;

            tmpMaxSensValue = struCnfg.nBigSmallArithMaxValue;

            if(struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struBigSmall[nIndex].nMode == 0) //选小
            {
                tmpSens = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struBigSmall[nIndex].nAreaLow+struCnfp.struUnitSens[nLayerId][nViewId][nUnitAddr][unitSensAddr].bigSmall[nIndex];
            }
            else
            {
                tmpSens = tmpMaxSensValue-(struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struBigSmall[nIndex].nAreaUp+struCnfp.struUnitSens[nLayerId][nViewId][nUnitAddr][unitSensAddr].bigSmall[nIndex]);
            }
            tmpSens = (tmpSens < 0) ? 0 : tmpSens;
            tmpSens = (tmpSens > tmpMaxSensValue) ? tmpMaxSensValue : tmpSens;

            data[4] = tmpSens/256;
            data[5] = tmpSens%256;

            mySerial.pushCom1CmdData(nCmd, nInt, struCnfg.struLayerInfo[nLayerId].nViewBoardType[nViewId], nUnitAddr, data, 3);
            // 延时选择
            memset(data, 0, SEND_PACKET_DATA_SIZE);
            for(int m=0; m<2; m++){
                data[4] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struBigSmall[m].delayType<<m;
            }
            mySerial.pushCom1CmdData(CMD_CAMERA_ARITH_DELAY_TYPE, nInt, struCnfg.struLayerInfo[nLayerId].nViewBoardType[nViewId], nUnitAddr, data, 3);

            break;
       case CMD_CAMERA_ARITH_CIRCLE_LONG:
       case CMD_CAMERA_ARITH_CIRCLE_LONG_2:
           if(!struGsh.allArithParamsResetCnts)  break;
            /* 1. 选圆选长 */
//           if(spliceType == 0)
//           {
//                data[0] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struCircleLong[nIndex].nArithEnable;
//           }
//           else
//           {
//                data[0] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struCircleLong[nIndex].nDispEnable;
//                tmpLEnable = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struCircleLong[nIndex].nArithEnable;
//                tmpREnable = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId+1].struCircleLong[nIndex].nArithEnable;
//                data[15] = (tmpREnable<<1)|tmpLEnable;
//           }
           data[0] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struCircleLong[nIndex].nArithEnable;
           data[1] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struCircleLong[nIndex].nMode;

            tmpMaxSensValue = ARITH_LONG_CIRCLE_MAX_VALUE;
            if(struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struCircleLong[nIndex].nMode == 0)   //选长
            {
                tmpSens = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struCircleLong[nIndex].nCiloLow+struCnfp.struUnitSens[nLayerId][nViewId][nUnitAddr][unitSensAddr].circleLong[nIndex]);
            }
            else
            {
                tmpSens = ARITH_LONG_CIRCLE_MAX_VALUE - (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struCircleLong[nIndex].nCilosUp+struCnfp.struUnitSens[nLayerId][nViewId][nUnitAddr][unitSensAddr].circleLong[nIndex]);
            }

            tmpSens = (tmpSens < 0) ? 0 : tmpSens;
            tmpSens = (tmpSens > tmpMaxSensValue) ? tmpMaxSensValue : tmpSens;

            data[2] = tmpSens;

            nParamsLimit = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struCircleLong[nIndex].nLimit;
            data[3] = nParamsLimit/256;
            data[4] = nParamsLimit%256;

            mySerial.pushCom1CmdData(nCmd, nInt, struCnfg.struLayerInfo[nLayerId].nViewBoardType[nViewId], nUnitAddr, data, 3);

            // 延时选择
            memset(data, 0, SEND_PACKET_DATA_SIZE);
            for(int m=0; m<2; m++){
                data[5] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struCircleLong[m].delayType<<m;
            }
            mySerial.pushCom1CmdData(CMD_CAMERA_ARITH_DELAY_TYPE, nInt, struCnfg.struLayerInfo[nLayerId].nViewBoardType[nViewId], nUnitAddr, data, 3);

           break;
       case CMD_CAMERA_ARITH_LONG_SHORT:
       case CMD_CAMERA_ARITH_LONG_SHORT_2:
           if(!struGsh.allArithParamsResetCnts)  break;
            /* 1. 选长选短 */
            tmpMaxSensValue = ARITH_LENGTH_MAX_VALUE;
            data[0] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struLongShort[nIndex].nArithEnable;


            data[1] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struLongShort[nIndex].nLengthMode;

            if(struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struLongShort[nIndex].nLengthMode == 0)  //选短
            {
                tmpSens = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struLongShort[nIndex].nLengthLow+struCnfp.struUnitSens[nLayerId][nViewId][nUnitAddr][unitSensAddr].longShort[nIndex];
            }
            else
            {
                tmpSens = tmpMaxSensValue-(struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struLongShort[nIndex].nLengthUp+struCnfp.struUnitSens[nLayerId][nViewId][nUnitAddr][unitSensAddr].longShort[nIndex]);
            }
            tmpSens = (tmpSens < 0) ? 0 : tmpSens;
            tmpSens = (tmpSens > tmpMaxSensValue) ? tmpMaxSensValue : tmpSens;

            data[2] = tmpSens/256;
            data[3] = tmpSens%256;

            nParamsLimit = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struLongShort[nIndex].nLimit;
            data[4] = nParamsLimit/256;
            data[5] = nParamsLimit%256;

            mySerial.pushCom1CmdData(nCmd, nInt, struCnfg.struLayerInfo[nLayerId].nViewBoardType[nViewId], nUnitAddr, data, 3);
           break;
       case CMD_CAMERA_ARITH_HSV:
       case CMD_CAMERA_ARITH_HSV_2:
       case CMD_CAMERA_ARITH_HSV_3:
           if(!struGsh.allArithParamsResetCnts)  break;
            /* 1. Hsv算法 */
//            if(spliceType == 0)
//            {
//                data[0] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nArithEnable<<7;
//            }
//            else
//            {
//                data[0] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nDispEnable<<7;
//                tmpLEnable = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nArithEnable;
//                tmpREnable = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId+1].struHsv[nIndex].nArithEnable;
//                data[15] = (tmpREnable<<1)|tmpLEnable;
//            }
            data[0] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nArithEnable<<7;
            data[1] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nRow-1;
            data[2] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nPercent-1)/256;
            data[3] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nPercent-1)%256;

           switch(struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nHsvColor)
           {
           case 0:
               if(struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nHsvMode == 0)
               {
                   data[4] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nHsvSensUpH/256;
                   data[5] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nHsvSensUpH%256;

                   tmpSens = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nHsvSensLowH+struCnfp.struUnitSens[nLayerId][nViewId][nUnitAddr][unitSensAddr].hsv[nIndex];
                   tmpSens = (tmpSens < 0) ? 0 : tmpSens;
                   tmpSens = (tmpSens > 3599) ? 3599 : tmpSens;
                   data[6] = tmpSens/256;
                   data[7] = tmpSens%256;
               }
               else
               {
                   tmpSens = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nHsvSensUpH+struCnfp.struUnitSens[nLayerId][nViewId][nUnitAddr][unitSensAddr].hsv[nIndex];
                   tmpSens = (tmpSens < 0) ? 0 : tmpSens;
                   tmpSens = (tmpSens > 3599) ? 3599 : tmpSens;

                   data[4] = tmpSens/256;
                   data[5] = tmpSens%256;
                   data[6] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nHsvSensLowH/256;
                   data[7] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nHsvSensLowH%256;
               }

               data[8] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nHsvSensUpS;
               data[9] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nHsvSensLowS;
               data[10] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nHsvSensUpV;
               data[11] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nHsvSensLowV;
               break;
           case 1:
               data[4] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nHsvSensUpH/256;
               data[5] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nHsvSensUpH%256;
               data[6] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nHsvSensLowH/256;
               data[7] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nHsvSensLowH%256;
               if(struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nHsvMode == 0)
               {
                   data[8] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nHsvSensUpS;

                   tmpSens = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nHsvSensLowS+struCnfp.struUnitSens[nLayerId][nViewId][nUnitAddr][unitSensAddr].hsv[nIndex];
                   tmpSens = (tmpSens < 0) ? 0 : tmpSens;
                   tmpSens = (tmpSens > 255) ? 255 : tmpSens;
                   data[9] = tmpSens;
               }
               else
               {
                   tmpSens = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nHsvSensUpS+struCnfp.struUnitSens[nLayerId][nViewId][nUnitAddr][unitSensAddr].hsv[nIndex];
                   tmpSens = (tmpSens < 0) ? 0 : tmpSens;
                   tmpSens = (tmpSens > 255) ? 255 : tmpSens;
                   data[8] = tmpSens;
                   data[9] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nHsvSensLowS;
               }
               data[10] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nHsvSensUpV;
               data[11] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nHsvSensLowV;
               break;
           case 2:
               data[4] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nHsvSensUpH/256;
               data[5] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nHsvSensUpH%256;
               data[6] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nHsvSensLowH/256;
               data[7] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nHsvSensLowH%256;
               data[8] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nHsvSensUpS;
               data[9] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nHsvSensLowS;
               if(struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nHsvMode == 0)
               {
                   data[10] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nHsvSensUpV;
                   tmpSens = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nHsvSensLowV+struCnfp.struUnitSens[nLayerId][nViewId][nUnitAddr][unitSensAddr].hsv[nIndex];
                   tmpSens = (tmpSens < 0) ? 0 : tmpSens;
                   tmpSens = (tmpSens > 255) ? 255 : tmpSens;
                   data[11] = tmpSens;
               }
               else
               {
                   tmpSens = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nHsvSensUpV+struCnfp.struUnitSens[nLayerId][nViewId][nUnitAddr][unitSensAddr].hsv[nIndex];
                   tmpSens = (tmpSens < 0) ? 0 : tmpSens;
                   tmpSens = (tmpSens > 255) ? 255 : tmpSens;
                   data[10] = tmpSens;
                   data[11] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].nHsvSensLowV;
               }
               break;
           }
           mySerial.pushCom1CmdData(nCmd, nInt, struCnfg.struLayerInfo[nLayerId].nViewBoardType[nViewId], nUnitAddr, data, 3);

           //新增Hsv算法保留参数
           memset(data, 0, SEND_PACKET_DATA_SIZE);
           data[0] = nIndex;
           /* 灰度保留 */
           resGreyColor = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].struResGreyColor.nColor1;
           if(struCnfp.nSensorDim[nLayerId][nViewId] <= 3)
           {
               resGreyColor = myFlow.changeColorBlackWhite2Nir(resGreyColor);
           }
           data[1] = resGreyColor<<5;
           tmpMaxSensValue = ARITH_GREY_MAX_SENS;
           nMin = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].struResGreyColor.nSensLow1;
           nMax = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].struResGreyColor.nSensUp1;
           nMin = (nMin <= 0) ? 0 : nMin;
           nMax = (nMax > tmpMaxSensValue) ? tmpMaxSensValue : nMax;
           data[2] = nMin;
           data[3] = nMax;

           /* 色差保留 */
           tmpDiffColor = myFlow.getDiffArithColor(struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].struResGreyColor.nResDiffColor, false);
           nMain = tmpDiffColor>>4;
           nVice = tmpDiffColor%16;
           data[4] = (nMain<<5)|(nVice<<2);
           tmpMaxSensValue = ARITH_COLOR_MAX_SENS;
           nMin = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].struResGreyColor.nSensLow2;
           nMax = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsv[nIndex].struResGreyColor.nSensUp2;
           nMin = (nMin <= 0) ? 0 : nMin;
           nMax = (nMax > tmpMaxSensValue) ? tmpMaxSensValue : nMax;
           data[5] = nMin/4;                   //灵敏度下限高8位
           data[6] = ((nMin%4)<<6)|(nMax/256); //灵敏度下限低2位|灵敏度上限高2位
           data[7] = nMax%256;                 //灵敏度上限低8位
           mySerial.pushCom1CmdData(CMD_HSV_ARITH_RESERVE_PARAM, nInt, struCnfg.struLayerInfo[nLayerId].nViewBoardType[nViewId], nUnitAddr, data, 3);

           // 新增hsv信号通道选择命令
//           memset(data, 0, SEND_PACKET_DATA_SIZE);
//           for(int paramNum = 0; paramNum < MAX_HSV_B_TOTAL; paramNum++)
//           {
//               data[paramNum] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsvB[paramNum].signalPath;
//           }
//           for(int paramNum = 0; paramNum < MAX_HSV_C_TOTAL; paramNum++)
//           {
//               data[paramNum+MAX_HSV_C_TOTAL] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struHsvC[paramNum].signalPath;
//           }
//           mySerial.pushCom1CmdData(CMD_CAMERA_SIGNAL_PATH, nInt, struCnfg.struLayerInfo[nLayerId].nViewBoardType[nViewId], nUnitAddr, data, 3);
           break;
       case CMD_CAMERA_ARITH_SVM_1:
       case CMD_CAMERA_ARITH_SVM_2:
       case CMD_CAMERA_ARITH_SVM_3:
       case CMD_CAMERA_ARITH_SVM_4:
           /* 0. 初始化各算法切边 */
           struCnfp.nGroupArithEdgeEnable[nLayerId][nViewId][nGroupId][ARITH_SVM_A+nIndex] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nEdge;
           if(!struGsh.allArithParamsResetCnts)  break;
           for(int quo = 1; quo <= 4; quo++)
           {
               if(nIndex >= MAX_SVM_ARITH_TOTAL)
                   nIndex = 0;
               memset(data, 0, SEND_PACKET_DATA_SIZE);
               data[0] = quo;
               switch(data[0])
               {
               case 1:
//                   if(spliceType == 0)
//                   {
//                       data[1] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nArithEnable;
//                   }
//                   else
//                   {
//                       data[1] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nDispEnable;
//                       tmpLEnable = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nArithEnable;
//                       tmpREnable = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId+1].struSvm[nIndex].nArithEnable;
//                       data[15] = (tmpREnable<<1)|tmpLEnable;
//                   }
                   data[1] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nArithEnable;
                   data[2] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nRow-1;
                   data[3] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nPercent-1)/256;
                   data[4] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nPercent-1)%256;

                   nSignCons = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoCons[0];
                   tmpSvmCons = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoCons[1];
                   //tmpSens = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nSens;
                   tmpSens = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nSens + struCnfp.struUnitSens[nLayerId][nViewId][nUnitAddr][unitSensAddr].svm[nIndex];
                   if(struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nModeSvm == 1) { //反选时，变换符号位标志
                       nSignCons = (nSignCons == 1) ? 0:1;
                   }
                   if(nSignCons == 1) {//综合正反选后，常数项为负数
                       /* 灵敏度越大，常数项绝对值越大，选的越多 */
                       if (tmpSens >= 100) {
                           nSvmCons = tmpSvmCons*(1+(tmpSens-100)/100.0);
                       }
                       else {   // 灵敏度越小，常数项绝对值越小，选的越小
                           nSvmCons = tmpSvmCons*(1-(100-tmpSens)/100.0);
                       }
                   }
                   else {
                       /* 灵敏度越大，常数项越小，选的越多 */
                       if (tmpSens >= 100) {
                           nSvmCons = tmpSvmCons*(1-(tmpSens-100)/100.0);
                       }
                       else {   // 灵敏度越小，常数项越大，选的越少
                           nSvmCons = tmpSvmCons*(1+(100-tmpSens)/100.0);
                       }
                   }

                   //智能模式３，需要对常数项放大
                   if ( struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nModeType ==2){
                       tmpSvmCons = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoCons[0] == 0?struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoCons[1]:struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoCons[1]*(-1);
                       nSvmCons = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nConsExpandedValue*tmpSvmCons*((tmpSens-100)/100.0);
                       nSignCons = (nSvmCons > 0)? 0 : 1;
                       nSvmCons = (nSvmCons >0)?nSvmCons:(-1*nSvmCons);
                   }

                   data[5] = (nSignCons<<7)|((nSvmCons&0x7F000000)>>24);
                   data[6] = (nSvmCons&0x00FF0000)>>16;
                   data[7] = (nSvmCons&0x0000FF00)>>8;
                   data[8] = (nSvmCons&0x000000FF);
                   data[11] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].brightLimit;
                   break;
               case 2:
                   nSignSvm[0] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoR[0];
                   data[4] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoR[1]&0x000000FF);
                   data[3] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoR[1]&0x0000FF00)>>8;
                   data[2] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoR[1]&0x00FF0000)>>16;
                   data[1] = (nSignSvm[0]<<7)|((struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoR[1]&0x7F000000)>>24);

                   nSignSvm[1] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoG[0];
                   data[8] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoG[1]&0x000000FF);
                   data[7] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoG[1]&0x0000FF00)>>8;
                   data[6] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoG[1]&0x00FF0000)>>16;
                   data[5] = (nSignSvm[1]<<7)|((struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoG[1]&0x7F000000)>>24);

                   nSignSvm[2] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoB[0];
                   data[12] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoB[1]&0x000000FF);
                   data[11] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoB[1]&0x0000FF00)>>8;
                   data[10] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoB[1]&0x00FF0000)>>16;
                   data[9] = (nSignSvm[2]<<7)|((struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoB[1]&0x7F000000)>>24);
                   break;
               case 3:
                   nSignSvm[0] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoRR[0];
                   data[4] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoRR[1]&0x000000FF);
                   data[3] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoRR[1]&0x0000FF00)>>8;
                   data[2] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoRR[1]&0x00FF0000)>>16;
                   data[1] = (nSignSvm[0]<<7)|((struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoRR[1]&0x7F000000)>>24);

                   nSignSvm[1] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoGG[0];
                   data[8] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoGG[1]&0x000000FF);
                   data[7] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoGG[1]&0x0000FF00)>>8;
                   data[6] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoGG[1]&0x00FF0000)>>16;
                   data[5] = (nSignSvm[1]<<7)|((struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoGG[1]&0x7F000000)>>24);

                   nSignSvm[2] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoBB[0];
                   data[12] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoBB[1]&0x000000FF);
                   data[11] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoBB[1]&0x0000FF00)>>8;
                   data[10] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoBB[1]&0x00FF0000)>>16;
                   data[9] = (nSignSvm[2]<<7)|((struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoBB[1]&0x7F000000)>>24);
                   break;
               case 4:
                   nSignSvm[0] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoRG[0];
                   data[4] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoRG[1]&0x000000FF);
                   data[3] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoRG[1]&0x0000FF00)>>8;
                   data[2] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoRG[1]&0x00FF0000)>>16;
                   data[1] = (nSignSvm[0]<<7)|((struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoRG[1]&0x7F000000)>>24);

                   nSignSvm[1] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoGB[0];
                   data[8] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoGB[1]&0x000000FF);
                   data[7] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoGB[1]&0x0000FF00)>>8;
                   data[6] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoGB[1]&0x00FF0000)>>16;
                   data[5] = (nSignSvm[1]<<7)|((struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoGB[1]&0x7F000000)>>24);

                   nSignSvm[2] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoRB[0];
                   data[12] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoRB[1]&0x000000FF);
                   data[11] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoRB[1]&0x0000FF00)>>8;
                   data[10] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoRB[1]&0x00FF0000)>>16;
                   data[9] = (nSignSvm[2]<<7)|((struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoRB[1]&0x7F000000)>>24);
                   break;
               }
               mySerial.pushCom1CmdData(nCmd, nInt, struCnfg.struLayerInfo[nLayerId].nViewBoardType[nViewId], nUnitAddr, data, 3);
           }

           tmpEnable  = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nPropotionEnable;
           if(nCmd == CMD_CAMERA_ARITH_SVM_2){
               memset(data, 0, SEND_PACKET_DATA_SIZE);
               /* 1.比例使能 */
               data[0] = tmpEnable<<7;
               /* 2. 算法类型（废弃） */
               data[1] = 0;
               /* 3. 精度下阈 */
               data[2] = ((int)(struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].fPropotionSensLow*0.01*ARITH_PROPOTION_MAX_SENS+0.5))/256;
               data[3] = ((int)(struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].fPropotionSensLow*0.01*ARITH_PROPOTION_MAX_SENS+0.5))%256;
               /* 4. 物料下限 */
               data[4] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nPropotionParaSensLow)/256;
               data[5] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nPropotionParaSensLow)%256;
               /* 5. 精度上阈 */
               data[6] = ((int)(struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].fPropotionSensUp*0.01*ARITH_PROPOTION_MAX_SENS+0.5))/256;
               data[7] = ((int)(struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].fPropotionSensUp*0.01*ARITH_PROPOTION_MAX_SENS+0.5))%256;

               mySerial.pushCom1CmdData(CMD_CAMERA_ARITH_SCALE_SVM_2, nInt, struCnfg.struLayerInfo[nLayerId].nViewBoardType[nViewId], nUnitAddr, data, 3);
           }

           //算法切边
           /* 1. 序号，默认为0 */
           memset(data, 0, SEND_PACKET_DATA_SIZE);
           data[0] = 0;
           /* 2. Byte1-3：色差1-色差3切边 Byte4-6：灰度1-灰度3切边 Byte7-9：智能1-智能3切边 */
           data[1] = struCnfp.nGroupArithEdgeEnable[nLayerId][nViewId][nGroupId][ARITH_DISCOLOR_A];
           data[2] = struCnfp.nGroupArithEdgeEnable[nLayerId][nViewId][nGroupId][ARITH_DISCOLOR_B];
           data[3] = struCnfp.nGroupArithEdgeEnable[nLayerId][nViewId][nGroupId][ARITH_CROSS];
           data[4] = struCnfp.nGroupArithEdgeEnable[nLayerId][nViewId][nGroupId][ARITH_GREY_A];
           data[5] = struCnfp.nGroupArithEdgeEnable[nLayerId][nViewId][nGroupId][ARITH_GREY_B];
           data[6] = struCnfp.nGroupArithEdgeEnable[nLayerId][nViewId][nGroupId][ARITH_GREY_C];
           data[7] = struCnfp.nGroupArithEdgeEnable[nLayerId][nViewId][nGroupId][ARITH_SVM_A];
           data[8] = struCnfp.nGroupArithEdgeEnable[nLayerId][nViewId][nGroupId][ARITH_SVM_B];
           data[9] = struCnfp.nGroupArithEdgeEnable[nLayerId][nViewId][nGroupId][ARITH_SVM_C];
           data[10] = struCnfp.nGroupArithEdgeEnable[nLayerId][nViewId][nGroupId][ARITH_SVM_D];

           mySerial.pushCom1CmdData(CMD_CAMERA_ARITH_EDGE_CUT, nInt, struCnfg.struLayerInfo[nLayerId].nViewBoardType[nViewId], nUnitAddr, data, 3);

           // 延时选择
           memset(data, 0, SEND_PACKET_DATA_SIZE);
           for(int m=0; m<3; m++){
               data[2] += struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[m].delayType<<m;
           }
           mySerial.pushCom1CmdData(CMD_CAMERA_ARITH_DELAY_TYPE, nInt, struCnfg.struLayerInfo[nLayerId].nViewBoardType[nViewId], nUnitAddr, data, 3);

           break;

       case CMD_CAMERA_ARITH_MULTY_SVM:
           /* 0. 初始化各算法切边 */
           struCnfp.nGroupArithEdgeEnable[nLayerId][nViewId][nGroupId][ARITH_SVM_A+nIndex] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nEdge;
           if(!struGsh.allArithParamsResetCnts)  break;
           for(int quo = 1; quo <= 4; quo++)
           {
               if(nIndex >= MAX_SVM_ARITH_TOTAL)
                   nIndex = 0;
               memset(data, 0, SEND_PACKET_DATA_SIZE);
               data[0] = quo;
               switch(data[0])
               {
               case 1:
//                   if(spliceType == 0)
//                   {
//                       data[1] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nArithEnable;
//                   }
//                   else
//                   {
//                       data[1] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nDispEnable;
//                       tmpLEnable = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nArithEnable;
//                       tmpREnable = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId+1].struSvm[nIndex].nArithEnable;
//                       data[15] = (tmpREnable<<1)|tmpLEnable;
//                   }
                   data[1] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nArithEnable;
                   data[2] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nRow-1;
                   data[3] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nPercent-1)/256;
                   data[4] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nPercent-1)%256;

                   nSignCons = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoCons[0];
                   tmpSvmCons = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoCons[1];
                   //tmpSens = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nSens;
                   tmpSens = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nSens + struCnfp.struUnitSens[nLayerId][nViewId][nUnitAddr][unitSensAddr].svm[nIndex];
                   if(struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nModeSvm == 1) { //反选时，变换符号位标志
                       nSignCons = (nSignCons == 1) ? 0:1;
                   }
                   if(nSignCons == 1) {//综合正反选后，常数项为负数
                       /* 灵敏度越大，常数项绝对值越大，选的越多 */
                       if (tmpSens >= 100) {
                           nSvmCons = tmpSvmCons*(1+(tmpSens-100)/100.0);
                       }
                       else {   // 灵敏度越小，常数项绝对值越小，选的越小
                           nSvmCons = tmpSvmCons*(1-(100-tmpSens)/100.0);
                       }
                   }
                   else {
                       /* 灵敏度越大，常数项越小，选的越多 */
                       if (tmpSens >= 100) {
                           nSvmCons = tmpSvmCons*(1-(tmpSens-100)/100.0);
                       }
                       else {   // 灵敏度越小，常数项越大，选的越少
                           nSvmCons = tmpSvmCons*(1+(100-tmpSens)/100.0);
                       }
                   }

                   //智能模式３，需要对常数项放大
//                   if ( struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nModeType ==2){
//                       tmpSvmCons = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoCons[0] == 0?struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoCons[1]:struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoCons[1]*(-1);
//                       nSvmCons = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nConsExpandedValue*tmpSvmCons*((tmpSens-100)/100.0);
//                       nSignCons = (nSvmCons > 0)? 0 : 1;
//                       nSvmCons = (nSvmCons >0)?nSvmCons:(-1*nSvmCons);
//                   }

                   data[5] = (nSignCons<<7)|((nSvmCons&0x7F000000)>>24);
                   data[6] = (nSvmCons&0x00FF0000)>>16;
                   data[7] = (nSvmCons&0x0000FF00)>>8;
                   data[8] = (nSvmCons&0x000000FF);
                   data[11] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].brightLimit;
                   data[12] = nIndex;
                   data[13] = 4;
                   break;
               case 2:
                   nSignSvm[0] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoR[0];
                   data[4] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoR[1]&0x000000FF);
                   data[3] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoR[1]&0x0000FF00)>>8;
                   data[2] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoR[1]&0x00FF0000)>>16;
                   data[1] = (nSignSvm[0]<<7)|((struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoR[1]&0x7F000000)>>24);

                   nSignSvm[1] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoG[0];
                   data[8] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoG[1]&0x000000FF);
                   data[7] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoG[1]&0x0000FF00)>>8;
                   data[6] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoG[1]&0x00FF0000)>>16;
                   data[5] = (nSignSvm[1]<<7)|((struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoG[1]&0x7F000000)>>24);

                   nSignSvm[2] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoB[0];
                   data[12] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoB[1]&0x000000FF);
                   data[11] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoB[1]&0x0000FF00)>>8;
                   data[10] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoB[1]&0x00FF0000)>>16;
                   data[9] = (nSignSvm[2]<<7)|((struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoB[1]&0x7F000000)>>24);
                   break;
               case 3:
                   nSignSvm[0] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoRG[0];
                   data[4] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoRG[1]&0x000000FF);
                   data[3] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoRG[1]&0x0000FF00)>>8;
                   data[2] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoRG[1]&0x00FF0000)>>16;
                   data[1] = (nSignSvm[0]<<7)|((struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSvm[nIndex].nQuoRG[1]&0x7F000000)>>24);

                   break;
               }
               mySerial.pushCom1CmdData(nCmd, nInt, struCnfg.struLayerInfo[nLayerId].nViewBoardType[nViewId], nUnitAddr, data, 3);
           }
           break;

       case CMD_CAMERA_ARITH_SHAPE_SVM_1:
//       case CMD_CAMERA_ARITH_SHAPE_SVM_2:
//       case CMD_CAMERA_ARITH_SHAPE_SVM_3:
           /* 0. 初始化各算法切边 */
           struCnfp.nGroupArithEdgeEnable[nLayerId][nViewId][nGroupId][ARITH_SHAPE_SVM_A+nIndex] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nEdge;
           if(!struGsh.allArithParamsResetCnts)  break;
           data[0] = nIndex+1;
           for(int quo = 1; quo <= 3; quo++)
           {
               memset(data, 0, SEND_PACKET_DATA_SIZE);
               data[1] = quo;
               switch(data[1])
               {
               case 1:
//                   if(spliceType == 0)
//                   {
//                       data[2] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nArithEnable;
//                   }
//                   else
//                   {
//                       data[2] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nDispEnable;
//                       tmpLEnable = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nArithEnable;
//                       tmpREnable = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId+1].struShapeSvm[nIndex].nArithEnable;
//                       data[15] = (tmpREnable<<1)|tmpLEnable;
//                   }
                    data[2] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nArithEnable;
                   data[3] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nAreaLow/256;
                   data[4] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nAreaLow%256;
                   data[5] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nAreaUp/256;
                   data[6] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nAreaUp%256;

                   nSignCons = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nQuoCons[0];
                   tmpSvmCons = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nQuoCons[1];
                   tmpSens = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nSens;
                   if(struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nModeSvm == 1) { //反选时，变换符号位标志
                       nSignCons = (nSignCons == 1) ? 0:1;
                   }
                   if(nSignCons == 1) {//综合正反选后，常数项为负数
                       /* 灵敏度越大，常数项绝对值越大，选的越多 */
                       if (tmpSens >= 100) {
                           nSvmCons = tmpSvmCons*(1+(tmpSens-100)/100.0);
                       }
                       else {   // 灵敏度越小，常数项绝对值越小，选的越小
                           nSvmCons = tmpSvmCons*(1-(100-tmpSens)/100.0);
                       }
                   }
                   else {
                       /* 灵敏度越大，常数项越小，选的越多 */
                       if (tmpSens >= 100) {
                           nSvmCons = tmpSvmCons*(1-(tmpSens-100)/100.0);
                       }
                       else {   // 灵敏度越小，常数项越大，选的越少
                           nSvmCons = tmpSvmCons*(1+(100-tmpSens)/100.0);
                       }
                   }

                   data[7] = (nSignCons<<7)|((nSvmCons&0x7F000000)>>24);
                   data[8] = (nSvmCons&0x00FF0000)>>16;
                   data[9] = (nSvmCons&0x0000FF00)>>8;
                   data[10] = (nSvmCons&0x000000FF);
                   break;
               case 2:
                   nSignSvm[0] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nQuoA[0];
                   data[5] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nQuoA[1]&0x000000FF);
                   data[4] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nQuoA[1]&0x0000FF00)>>8;
                   data[3] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nQuoA[1]&0x00FF0000)>>16;
                   data[2] = (nSignSvm[0]<<7)|((struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nQuoA[1]&0x7F000000)>>24);

                   nSignSvm[1] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nQuoB[0];
                   data[9] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nQuoB[1]&0x000000FF);
                   data[8] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nQuoB[1]&0x0000FF00)>>8;
                   data[7] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nQuoB[1]&0x00FF0000)>>16;
                   data[6] = (nSignSvm[1]<<7)|((struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nQuoB[1]&0x7F000000)>>24);

                   nSignSvm[2] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nQuoC[0];
                   data[13] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nQuoC[1]&0x000000FF);
                   data[12] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nQuoC[1]&0x0000FF00)>>8;
                   data[11] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nQuoC[1]&0x00FF0000)>>16;
                   data[10] = (nSignSvm[2]<<7)|((struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nQuoC[1]&0x7F000000)>>24);
                   break;
               case 3:
                   nSignSvm[0] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nQuoD[0];
                   data[5] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nQuoD[1]&0x000000FF);
                   data[4] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nQuoD[1]&0x0000FF00)>>8;
                   data[3] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nQuoD[1]&0x00FF0000)>>16;
                   data[2] = (nSignSvm[0]<<7)|((struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nQuoD[1]&0x7F000000)>>24);

                   nSignSvm[1] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nQuoE[0];
                   data[9] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nQuoE[1]&0x000000FF);
                   data[8] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nQuoE[1]&0x0000FF00)>>8;
                   data[7] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nQuoE[1]&0x00FF0000)>>16;
                   data[6] = (nSignSvm[1]<<7)|((struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nQuoE[1]&0x7F000000)>>24);

                   nSignSvm[2] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nQuoF[0];
                   data[13] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nQuoF[1]&0x000000FF);
                   data[12] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nQuoF[1]&0x0000FF00)>>8;
                   data[11] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nQuoF[1]&0x00FF0000)>>16;
                   data[10] = (nSignSvm[2]<<7)|((struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struShapeSvm[nIndex].nQuoF[1]&0x7F000000)>>24);
                   break;
               }
               mySerial.pushCom1CmdData(nCmd, nInt, struCnfg.struLayerInfo[nLayerId].nViewBoardType[nViewId], nUnitAddr, data, 3);
           }
           break;

       case CMD_CAMERA_SENS_SAT:
       {
           if(!struGsh.allArithParamsResetCnts)  break;
           const stru_color_sat sat = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struColorSat;
           /* 饱和算法 */
           tmpMaxSensValue = ARITH_SAT_MAX_SENSE;
           data[0] = sat.nArithEnable<<7;

           /* 2. 尺寸 */
           data[1] = sat.nRow-1;
           /* 3. 纯度 */
           data[2] = (sat.nPercent-1)/256;
           data[3] =(sat.nPercent-1)%256;

           /* 4. 灵敏度上阈/下阈 */
           if (sat.nMode == 1) {
               nMin = tmpMaxSensValue-(sat.nSensUp+struCnfp.struUnitSens[nLayerId][nViewId][nUnitAddr][unitSensAddr].sat);
               nMin = (nMin <= 0) ? 0 : nMin;
               nMax = tmpMaxSensValue;
           }
           else {
               nMin = 0;
               nMax =sat.nSensLow+struCnfp.struUnitSens[nLayerId][nViewId][nUnitAddr][unitSensAddr].sat;
               nMax = (nMax > tmpMaxSensValue) ? tmpMaxSensValue : nMax;
           }
           data[4] = nMin;
           data[5] = nMax;
           /* 限值 */
//           nParamsLimit =sat.lightLimit;
//           data[6] = nParamsLimit/256;
//           data[7] = nParamsLimit%256;
           data[6] = sat.lightLimit;
           mySerial.pushCom1CmdData(nCmd, nInt, struCnfg.struLayerInfo[nLayerId].nViewBoardType[nViewId], nUnitAddr, data, 3);
       }
           break;
       case CMD_CAMERA_SENS_HUE:
           if(!struGsh.allArithParamsResetCnts)  break;
           data[0] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struColorHue.nArithEnable<<7;

           /* 2. 尺寸 */
           data[1] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struColorHue.nRow-1;
           /* 3. 纯度 */
           data[2] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struColorHue.nPercent-1)/256;
           data[3] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struColorHue.nPercent-1%256;


           data[4] = nMin;
           data[5] = nMax;



           /* 亮度下限值 */
           nParamsLimit = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struColorSat.lightLimit;
           data[12] = nParamsLimit/256;
           data[13] = nParamsLimit%256;
           mySerial.pushCom1CmdData(nCmd, nInt, struCnfg.struLayerInfo[nLayerId].nViewBoardType[nViewId], nUnitAddr, data, 3);

           break;
       case CMD_CAMERA_SENS_SCALE:
           if(!struGsh.allArithParamsResetCnts)  break;
            /* 比例算法 */
            tmpMaxSensValue = ARITH_SCALE_MAX_SENSE;
            data[0] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struColorScale.nArithEnable<<7;

            /* 算法类型 */
            data[1] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struColorScale.nSenIdx;

            /* 2. 灵敏度 */
            tmpSens = 1023-(struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struColorScale.nSen+struCnfp.struUnitSens[nLayerId][nViewId][nUnitAddr][unitSensAddr].scale);
            data[2] = tmpSens/256;
            data[3] = tmpSens%256;
            /* 限值 */
            nParamsLimit = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struColorScale.nLimit;
            data[4] = nParamsLimit/256;
            data[5] = nParamsLimit%256;

            mySerial.pushCom1CmdData(nCmd, nInt, struCnfg.struLayerInfo[nLayerId].nViewBoardType[nViewId], nUnitAddr, data, 3);
           break;

       case CMD_CAMERA_SENS_MILDEW:
           if(!struGsh.allArithParamsResetCnts)  break;
           /* 霉点算法 */
           data[0] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struMildew.nArithEnable<<7;

           /*灵敏度*/
           if(struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struMildew.nMark == 0)
           {
               data[1] = 255 - (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struMildew.nSens+struCnfp.struUnitSens[nLayerId][nViewId][nUnitAddr][unitSensAddr].mildew);
           }
           else
           {
               data[1] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struMildew.nSens+struCnfp.struUnitSens[nLayerId][nViewId][nUnitAddr][unitSensAddr].mildew;
           }

           /*纯度*/
           tmpSens = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struMildew.nPercent;
           data[2] = tmpSens/256;
           data[3] = tmpSens%256;

           /*下限*/
           nParamsLimit = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struMildew.nLimit;
           data[4]  = nParamsLimit/256;
           data[5]  = nParamsLimit%256;

           data[6]  = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struMildew.nMark;
           data[7]  = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struMildew.nType;

           mySerial.pushCom1CmdData(nCmd, nInt, struCnfg.struLayerInfo[nLayerId].nViewBoardType[nViewId], nUnitAddr, data, 3);
           break;
       case CMD_CAMERA_SENS_OUTLINE:
           if(!struGsh.allArithParamsResetCnts)  break;
           /* 轮廓算法 */
           data[0]  = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struOutline.nArithEnable<<7;

           /* 灵敏度*/
           data[1]  = 255-(struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struOutline.nSens+struCnfp.struUnitSens[nLayerId][nViewId][nUnitAddr][unitSensAddr].outline);

           /*纯度*/
           data[2]  = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struOutline.nPercent;

           /*上限*/
           data[3]  = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struOutline.nHighLimit;

           /*下限*/
           data[4]  = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struOutline.nLowLimit;

           mySerial.pushCom1CmdData(nCmd, nInt, struCnfg.struLayerInfo[nLayerId].nViewBoardType[nViewId], nUnitAddr, data, 3);
           break;
       case CMD_CAMERA_SENS_LINE:
           if(!struGsh.allArithParamsResetCnts)  break;
           /*纹理算法*/
           data[0] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struLine.nArithEnable<<7;

           /*纹理颜色*/
           data[1] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struLine.nColor;

           /*尺寸*/
           data[2] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struLine.nRow-1;

           /*纯度*/
           tmpSens = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struLine.nPercent;
           data[3] = tmpSens/256;
           data[4] = tmpSens%256;

           /*精度1*/
           data[5] = 255-(struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struLine.nSens1+struCnfp.struUnitSens[nLayerId][nViewId][nUnitAddr][unitSensAddr].line);

           /*精度2*/
           data[6] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struLine.nSens2;

           mySerial.pushCom1CmdData(nCmd, nInt, struCnfg.struLayerInfo[nLayerId].nViewBoardType[nViewId], nUnitAddr, data, 3);
           break;
       case CMD_CAMERA_SENS_BUD:
//       case CMD_CAMERA_SENS_BUD_B:
           if(!struGsh.allArithParamsResetCnts)  break;
           /*选芽算法*/
           data[0] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struBud[nIndex].nArithEnable<<7;

           /*尺寸*/
           data[1] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struBud[nIndex].nSize-1;

           /*精度*/
           tmpSens = 1023-(struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struBud[nIndex].nSens + struCnfp.struUnitSens[nLayerId][nViewId][nUnitAddr][unitSensAddr].bud[nIndex]);
           data[2] = tmpSens/256;
           data[3] = tmpSens%256;

           /*下限*/
           data[4] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struBud[nIndex].nLimit;

           /*黄芽精度*/
           data[5] = 255-struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struBud[nIndex].nYellowSens;

           /*黑芽精度*/
           data[6] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struBud[nIndex].nBlackSens;


           mySerial.pushCom1CmdData(nCmd, nInt, struCnfg.struLayerInfo[nLayerId].nViewBoardType[nViewId], nUnitAddr, data, 3);
           break;
       case CMD_CAMERA_SENS_ROUND:
       case CMD_CAMERA_SENS_ROUND_2:
           if(!struGsh.allArithParamsResetCnts)  break;
            /* 1. 选圆选非圆 */
           data[0] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struRound[nIndex].nArithEnable;
           data[1] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struRound[nIndex].nSelectMode;

            tmpMaxSensValue = ARITH_ROUND_MAX_SENSE;
            if(struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struRound[nIndex].nSelectMode == 0)   //选圆
            {
                tmpSens = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struRound[nIndex].nSensUp+struCnfp.struUnitSens[nLayerId][nViewId][nUnitAddr][unitSensAddr].round[nIndex]);
            }
            else
            {
                tmpSens = ARITH_ROUND_MAX_SENSE - (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struRound[nIndex].nSensLow+struCnfp.struUnitSens[nLayerId][nViewId][nUnitAddr][unitSensAddr].round[nIndex]);
            }

            tmpSens = (tmpSens < 0) ? 0 : tmpSens;
            tmpSens = (tmpSens > tmpMaxSensValue) ? tmpMaxSensValue : tmpSens;

            data[2] = tmpSens/256;
            data[3] = tmpSens%256;

            nParamsLimit = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struRound[nIndex].nLimit;
            data[4] = nParamsLimit/256;
            data[5] = nParamsLimit%256;

            mySerial.pushCom1CmdData(nCmd, nInt, struCnfg.struLayerInfo[nLayerId].nViewBoardType[nViewId], nUnitAddr, data, 3);
           break;
//       case CMD_CAMERA_SENS_VALUE:
//       {
//           const stru_color_value value = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struColorValue;
//           /* 灰度算法 */
//           tmpMaxSensValue = ARITH_VALUE_MAX_SENSE;
//           data[0] = value.nArithEnable<<7;

//           /* 2. 尺寸 */
//           data[1] = value.nRow-1;
//           /* 3. 纯度 */
//           data[2] = (value.nPercent-1)/256;
//           data[3] =(value.nPercent-1)%256;

//           /* 4. 灵敏度上阈/下阈 */
//           if (value.nMode == 1) {
//               nMin = tmpMaxSensValue-(value.nSensUp+struCnfp.struUnitSens[nLayerId][nViewId][nUnitAddr][unitSensAddr].value);
//               nMin = (nMin <= 0) ? 0 : nMin;
//               nMax = tmpMaxSensValue;
//           }
//           else {
//               nMin = 0;
//               nMax =value.nSensLow+struCnfp.struUnitSens[nLayerId][nViewId][nUnitAddr][unitSensAddr].value;
//               nMax = (nMax > tmpMaxSensValue) ? tmpMaxSensValue : nMax;
//           }
//           data[4] = nMax/256;
//           data[5] = nMax%256;

//           data[6] = nMin/256;
//           data[7] = nMin%256;
//           mySerial.pushCom1CmdData(nCmd, nInt, struCnfg.struLayerInfo[nLayerId].nViewBoardType[nViewId], nUnitAddr, data, 3);
//       }
//           break;

       case CMD_CAMERA_SENS_SPL_MILDEW:
           if(!struGsh.allArithParamsResetCnts)  break;
           /* 葫芦籽霉点算法 */
           data[0] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSplMildew.nArithEnable<<7;

           /*灵敏度*/
           data[1] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSplMildew.nSens+struCnfp.struUnitSens[nLayerId][nViewId][nUnitAddr][unitSensAddr].splMildew);

           mySerial.pushCom1CmdData(nCmd, nInt, struCnfg.struLayerInfo[nLayerId].nViewBoardType[nViewId], nUnitAddr, data, 3);
           break;
       case CMD_CAMERA_SENS_W_PEANUT:
           if(!struGsh.allArithParamsResetCnts)  break;
           /*白沙果算法*/
//           data[0] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struWhitePeanut.nArithEnable<<7;
           data[0] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struWhitePeanut.nArithEnable;

           /*灵敏度下限*/
           tmpSensLow = ARITH_WHITE_PEANUT_MAX_SENSE-(struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struWhitePeanut.nSensLow+struCnfp.struUnitSens[nLayerId][nViewId][nUnitAddr][unitSensAddr].whitePeanut);
           data[1] = tmpSensLow/256;
           data[2] = tmpSensLow%256;

           /*灵敏度上限*/
           tmpSensUp = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struWhitePeanut.nSensUp;
           data[3] = tmpSensUp/256;
           data[4] = tmpSensUp%256;
           mySerial.pushCom1CmdData(nCmd, nInt, struCnfg.struLayerInfo[nLayerId].nViewBoardType[nViewId], nUnitAddr, data, 3);
           break;           

       case CMD_CAMERA_ARITH_PEPPER_HANDLE:
           if(!struGsh.allArithParamsResetCnts)  break;
           /*算法*/
//           data[0] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struWhitePeanut.nArithEnable<<7;
           data[0] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struPepperHandle.nArithEnable;

           /*灵敏度*/
           tmpSensLow = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struPepperHandle.nSens+struCnfp.struUnitSens[nLayerId][nViewId][nUnitAddr][unitSensAddr].pepperHandle;
           data[1] = tmpSensLow/256;
           data[2] = tmpSensLow%256;

           /*斑点比例*/
           data[3] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struPepperHandle.nPercent;

           /*面积下限*/
           nParamsLimit = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struPepperHandle.nLimit;
           data[4] = nParamsLimit/256;
           data[5] = nParamsLimit%256;
           mySerial.pushCom1CmdData(nCmd, nInt, struCnfg.struLayerInfo[nLayerId].nViewBoardType[nViewId], nUnitAddr, data, 3);
           break;

       case CMD_ARITH_SUPPRESS_LARGE:
           if(!struGsh.allArithParamsResetCnts)  break;
            data[0] = nIndex + 1;
            data[1] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSuppressLarge[nIndex].nArithEnable;
            data[2] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSuppressLarge[nIndex].nSize1-1;
            data[3] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSuppressLarge[nIndex].nSize2-1;
            data[4] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSuppressLarge[nIndex].nPercent1-1)/256;
            data[5] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSuppressLarge[nIndex].nPercent1-1)%256;
            data[6] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSuppressLarge[nIndex].nPercent2-1)/256;
            data[7] = (struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSuppressLarge[nIndex].nPercent2-1)%256;
            data[8] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSuppressLarge[nIndex].nSenIdx+1;
            data[9] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struSuppressLarge[nIndex].nTurns;
            mySerial.pushCom1CmdData(nCmd, nInt, struCnfg.struLayerInfo[nLayerId].nViewBoardType[nViewId], nUnitAddr, data, 3);
           break;
       case CMD_ARITH_WHEAT_SPROUT:
           if(!struGsh.allArithParamsResetCnts)  break;
           data[0] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struWheatSprout.nArithEnable;
           data[1] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struWheatSprout.nLow;
           data[2] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struWheatSprout.nPercent;
           data[3] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struWheatSprout.nSense;
           data[4] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struWheatSprout.nH0/256;
           data[5] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struWheatSprout.nH0%256;
           data[6] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struWheatSprout.nH1/256;
           data[7] = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struWheatSprout.nH1%256;

           mySerial.pushCom1CmdData(nCmd, nInt, struCnfg.struLayerInfo[nLayerId].nViewBoardType[nViewId], nUnitAddr, data, 3);
           break;

       case CMD_CAMERA_TEA_COLOR://表示tea的压制
       {
           if(!struGsh.allArithParamsResetCnts)  break;
#ifdef Q_OS_UNIX
           stru_tea_color tea_color = struCnfp.struGroupIdentify[nLayerId][nViewId][nGroupId].struTeaColor[nIndex];
           data[0] = tea_color.nArithEnable;

#if 0
           Tea converter;
           u_int16_t tc = converter.Float_to_fp16(tea_color.tc);
           u_int16_t ec = converter.Float_to_fp16(tea_color.ec);
           u_int16_t ac = converter.Float_to_fp16(tea_color.ac);
           u_int16_t sense = converter.Float_to_fp16(tea_color.sense);

           data[1] = tc/256;
           data[2] = tc%256;
           data[3] = ec/256;
           data[4] = ec%256;
           data[5] = ac/256;
           data[6] = ac%256;

           data[7] = sense/256;
           data[8] = sense%256;
           if(nIndex != TEA_ARITH_COLOR_5){
               data[9] = tea_color.nRow-1;
               data[10] = (tea_color.nPercent-1)/256;
               data[11] = (tea_color.nPercent-1)%256;
               data[13] = tea_color.nEdge;
               data[14] = tea_color.nInhibit;
           }
           data[12] = tea_color.nMode;
           data[15] = nIndex;
#endif


#endif
           mySerial.pushCom1CmdData(nCmd, nInt, struCnfg.struLayerInfo[nLayerId].nViewBoardType[nViewId], nUnitAddr, data, 3);
       }
           break;

       default:
           break;
       }
    }
}

/***************************************************************************************************
    名称：         重置物料参数
    功能：         发送物料参数给FPGA
     nLayerId:     识别组所在的层编号
    nViewId:      识别组所在的视编号
    nGroupId：    识别组的组号
    bIsAllArith  判断是发送灵敏度设置界面中的所有算法的灵敏度，还是方案设置中某个识别算法的参数
    nArithSeq：   识别算法序号
    bIsReserved:  是否是发送保留参数
***************************************************************************************************/
void GlobalFlow::resetMaterial(int nLayerId, int nViewId, int nGroupId, bool bIsAllArith, int nArithSeq, int bIsReserved)
{
//    int nArithmeticEnable[MAX_LEVEL][ARITHMETIC_TOTAL];
    int nArithBegin, nArithEnd;

    /* get arithmetic range */
    if (bIsAllArith == false) {
        nArithBegin = nArithSeq;
        nArithEnd   = nArithSeq+1;
    }
    else {
        nArithBegin = 0;
//        nArithEnd   = struCnfg.struLayerInfo[nLayerId].stuViewInfo[nViewId].nViewArithTotal;    //每视的算法总数不一样
        nArithEnd   = ARITHMETIC_TOTAL;    //每视的算法总数不一样

    }

    for (int i = nArithBegin; i < nArithEnd; i++)
    {
        if(struCnfg.struLayerInfo[nLayerId].stuViewInfo[nViewId].nViewArithVisiable[i] == 1)
       {
            switch (i) {
            case ARITH_GREY_A :
                materialIdentifyParamsSend(CMD_CAMERA_ARITH_GREY_1 ,nLayerId, nViewId, nGroupId, ARITH_GREY_A);
                break;
            case ARITH_GREY_B:
                materialIdentifyParamsSend(CMD_CAMERA_ARITH_GREY_2 ,nLayerId, nViewId, nGroupId, ARITH_GREY_B);
                break;
            case ARITH_GREY_C:
                materialIdentifyParamsSend(CMD_CAMERA_ARITH_GREY_3 ,nLayerId, nViewId, nGroupId, ARITH_GREY_C);
                break;
            case ARITH_GREY_D:
                materialIdentifyParamsSend(CMD_CAMERA_ARITH_GREY_4 ,nLayerId, nViewId, nGroupId, ARITH_GREY_D);
                break;
            case ARITH_DISCOLOR_A:
                materialIdentifyParamsSend(CMD_CAMERA_ARITH_DISCOLOR_1 ,nLayerId, nViewId, nGroupId, ARITH_DISCOLOR_A);
                break;
            case ARITH_DISCOLOR_B:
                materialIdentifyParamsSend(CMD_CAMERA_ARITH_DISCOLOR_2 ,nLayerId, nViewId, nGroupId, ARITH_DISCOLOR_B);
                break;
            case ARITH_CROSS:
                materialIdentifyParamsSend(CMD_CAMERA_ARITH_DISCOLOR_3 ,nLayerId, nViewId, nGroupId, ARITH_CROSS);
                break;
            case ARITH_SVM_A :
                if(struCnfg.n_svmType == 2)
                    materialIdentifyParamsSend(CMD_CAMERA_ARITH_MULTY_SVM ,nLayerId, nViewId, nGroupId, 0);
                else
                    materialIdentifyParamsSend(CMD_CAMERA_ARITH_SVM_1 ,nLayerId, nViewId, nGroupId, 0);
                break;
            case ARITH_SVM_B :
                if(struCnfg.n_svmType == 2)
                    materialIdentifyParamsSend(CMD_CAMERA_ARITH_MULTY_SVM ,nLayerId, nViewId, nGroupId, 1);
                else
                    materialIdentifyParamsSend(CMD_CAMERA_ARITH_SVM_2 ,nLayerId, nViewId, nGroupId, 1);
                break;
            case ARITH_SVM_C :
                if(struCnfg.n_svmType == 2)
                    materialIdentifyParamsSend(CMD_CAMERA_ARITH_MULTY_SVM ,nLayerId, nViewId, nGroupId, 2);
                else
                    materialIdentifyParamsSend(CMD_CAMERA_ARITH_SVM_3 ,nLayerId, nViewId, nGroupId, 2);
                break;
            case ARITH_SVM_D :
                if(struCnfg.n_svmType == 2)
                    materialIdentifyParamsSend(CMD_CAMERA_ARITH_MULTY_SVM ,nLayerId, nViewId, nGroupId, 3);
                else
                    materialIdentifyParamsSend(CMD_CAMERA_ARITH_SVM_4 ,nLayerId, nViewId, nGroupId, 3);
                break;
            case ARITH_SHAPE_SVM_A:
                materialIdentifyParamsSend(CMD_CAMERA_ARITH_SHAPE_SVM_1 ,nLayerId, nViewId, nGroupId, 0);
                break;
//            case ARITH_SHAPE_SVM_B:
//                materialIdentifyParamsSend(CMD_CAMERA_ARITH_SHAPE_SVM_2 ,nLayerId, nViewId, nGroupId, 1);
//                break;
//            case ARITH_SHAPE_SVM_C:
//                materialIdentifyParamsSend(CMD_CAMERA_ARITH_SHAPE_SVM_3 ,nLayerId, nViewId, nGroupId, 2);
//                break;
            case ARITH_BIG_SMALL:
                materialIdentifyParamsSend(CMD_CAMERA_ARITH_SCALE ,nLayerId, nViewId, nGroupId, 0);
                break;
            case ARITH_BIG_SMALL_B:
                materialIdentifyParamsSend(CMD_CAMERA_ARITH_SCALE_2 ,nLayerId, nViewId, nGroupId, 1);
                break;
            case ARITH_LONG_SHORT:
                materialIdentifyParamsSend(CMD_CAMERA_ARITH_LONG_SHORT ,nLayerId, nViewId, nGroupId, 0);
                break;
            case ARITH_LONG_SHORT_B:
                materialIdentifyParamsSend(CMD_CAMERA_ARITH_LONG_SHORT_2 ,nLayerId, nViewId, nGroupId, 1);
                break;
            case ARITH_CIRCLE_LONG:
                materialIdentifyParamsSend(CMD_CAMERA_ARITH_CIRCLE_LONG ,nLayerId, nViewId, nGroupId, 0);
                break;
            case ARITH_CIRCLE_LONG_B:
                materialIdentifyParamsSend(CMD_CAMERA_ARITH_CIRCLE_LONG_2 ,nLayerId, nViewId, nGroupId, 1);
                break;
            case ARITH_HSV:
                materialIdentifyParamsSend(CMD_CAMERA_ARITH_HSV ,nLayerId, nViewId, nGroupId, 0);
                break;
            case ARITH_HSV_B:
                materialIdentifyParamsSend(CMD_CAMERA_ARITH_HSV_2 ,nLayerId, nViewId, nGroupId, 1);
                break;
            case ARITH_HSV_C:
                materialIdentifyParamsSend(CMD_CAMERA_ARITH_HSV_3 ,nLayerId, nViewId, nGroupId, 2);
                break;
            case ARITH_SAT:
                materialIdentifyParamsSend(CMD_CAMERA_SENS_SAT, nLayerId, nViewId, nGroupId, 0);
                break;
            case ARITH_SCALE:
                materialIdentifyParamsSend(CMD_CAMERA_SENS_SCALE, nLayerId, nViewId, nGroupId, 0);
                break;
            case ARITH_HUE:
                materialIdentifyParamsSend(CMD_CAMERA_SENS_HUE, nLayerId, nViewId, nGroupId, 0);
                break;
            case ARITH_MILDEW:
                materialIdentifyParamsSend(CMD_CAMERA_SENS_MILDEW, nLayerId, nViewId, nGroupId, 0);
                break;
            case ARITH_OUTLINE:
                materialIdentifyParamsSend(CMD_CAMERA_SENS_OUTLINE, nLayerId, nViewId, nGroupId, 0);
                break;
            case ARITH_LINE:
                materialIdentifyParamsSend(CMD_CAMERA_SENS_LINE, nLayerId, nViewId, nGroupId, 0);
                break;
            case ARITH_BUD:
                materialIdentifyParamsSend(CMD_CAMERA_SENS_BUD, nLayerId, nViewId, nGroupId, 0);
                break;
            case ARITH_BUD_B:
//                materialIdentifyParamsSend(CMD_CAMERA_SENS_BUD_B, nLayerId, nViewId, nGroupId, 1);
                break;
            case ARITH_ROUND:
                materialIdentifyParamsSend(CMD_CAMERA_SENS_ROUND, nLayerId, nViewId, nGroupId, 0);
                break;
            case ARITH_ROUND_B:
                materialIdentifyParamsSend(CMD_CAMERA_SENS_ROUND_2, nLayerId, nViewId, nGroupId, 1);
                break;
            case ARITH_SPL_MILDEW:
                materialIdentifyParamsSend(CMD_CAMERA_SENS_SPL_MILDEW, nLayerId, nViewId, nGroupId, 0);
                break;
            case ARITH_W_PEANUT:
                materialIdentifyParamsSend(CMD_CAMERA_SENS_W_PEANUT, nLayerId, nViewId, nGroupId, 0);
                break;
            case ARITH_PEPPER_HANDLE:
                materialIdentifyParamsSend(CMD_CAMERA_ARITH_PEPPER_HANDLE, nLayerId, nViewId, nGroupId, 0);
                break;
            case ARITH_SUPPRESS_LARGE_A :
                materialIdentifyParamsSend(CMD_ARITH_SUPPRESS_LARGE ,nLayerId, nViewId, nGroupId, 0);
                break;
            case ARITH_SUPPRESS_LARGE_B :
                materialIdentifyParamsSend(CMD_ARITH_SUPPRESS_LARGE ,nLayerId, nViewId, nGroupId, 1);
                break;
            case ARITH_SUPPRESS_LARGE_C :
                materialIdentifyParamsSend(CMD_ARITH_SUPPRESS_LARGE ,nLayerId, nViewId, nGroupId, 2);
                break;
            case ARITH_WHEAT_SPROUT :
                materialIdentifyParamsSend(CMD_ARITH_WHEAT_SPROUT ,nLayerId, nViewId, nGroupId, 0);
                break;
            case TEA_ARITH_COLOR :
                materialIdentifyParamsSend(CMD_CAMERA_TEA_COLOR ,nLayerId, nViewId, nGroupId, 0);
                break;
            case TEA_ARITH_COLOR_2 :
                materialIdentifyParamsSend(CMD_CAMERA_TEA_COLOR ,nLayerId, nViewId, nGroupId, 1);
                break;
            case TEA_ARITH_COLOR_3 :
                materialIdentifyParamsSend(CMD_CAMERA_TEA_COLOR ,nLayerId, nViewId, nGroupId, 2);
                break;
            case TEA_ARITH_COLOR_4 :
                materialIdentifyParamsSend(CMD_CAMERA_TEA_COLOR ,nLayerId, nViewId, nGroupId, 3);
                break;
            case TEA_ARITH_COLOR_5 :
                materialIdentifyParamsSend(CMD_CAMERA_TEA_COLOR ,nLayerId, nViewId, nGroupId, 4);
                break;
//            case ARITH_VALUE:
//                materialIdentifyParamsSend(CMD_CAMERA_SENS_VALUE, nLayerId, nViewId, nGroupId, 0);
//                break;
            default:
                break;
            }
        }
    }
}

/***************************************************************************************************
    名称：         物料参数显示使能组间复制
    功能：         物料参数不同识别组直间的复制
    nLayerIdSrc:  准备复制的源识别组所在的层编号
    nViewIdSrc:   准备复制的源识别组所在的视编号
    nGroupIdSrc： 准备复制的源识别组编号
    nLayerIdDst： 准备复制的目标识别组所在的层编号
    nViewIdDst:   准备复制的目标识别组所在的视编号
    nGroupDst：   准备复制的目标识别组编号
    nArithSeq：   识别算法序号
***************************************************************************************************/
void GlobalFlow::materialDispEnableCopy(int nLayerIdSrc, int nViewIdSrc, int nGroupIdSrc, int nLayerIdDst, int nViewDst, int nGroupDst, int nArithSeq)
{
    int l1, l2, v1, v2, g1, g2;

    l1 =  nLayerIdSrc;
    l2 =  nLayerIdDst;

    v1 =  nViewIdSrc;
    v2 =  nViewDst;

    g1 = nGroupIdSrc;
    g2 = nGroupDst;

    switch(nArithSeq)
    {
    case ARITH_GREY_A:
    case ARITH_GREY_B:
    case ARITH_GREY_C:
    case ARITH_GREY_D:
    case ARITH_DISCOLOR_A:
    case ARITH_DISCOLOR_B:
    case ARITH_CROSS:
        struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[nArithSeq].nDispEnable = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[nArithSeq].nDispEnable;
        struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[nArithSeq].nArithEnable = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[nArithSeq].nArithEnable;
        break;
    case ARITH_CIRCLE_LONG:
    case ARITH_CIRCLE_LONG_B:
        struCnfp.struGroupIdentify[l2][v2][g2].struCircleLong[nArithSeq-ARITH_CIRCLE_LONG].nDispEnable = struCnfp.struGroupIdentify[l1][v1][g1].struCircleLong[nArithSeq-ARITH_CIRCLE_LONG].nDispEnable;
        struCnfp.struGroupIdentify[l2][v2][g2].struCircleLong[nArithSeq-ARITH_CIRCLE_LONG].nArithEnable = struCnfp.struGroupIdentify[l1][v1][g1].struCircleLong[nArithSeq-ARITH_CIRCLE_LONG].nArithEnable;
        break;
    case ARITH_LONG_SHORT:
    case ARITH_LONG_SHORT_B:
        struCnfp.struGroupIdentify[l2][v2][g2].struLongShort[nArithSeq-ARITH_LONG_SHORT].nArithEnable  = struCnfp.struGroupIdentify[l1][v1][g1].struLongShort[nArithSeq-ARITH_LONG_SHORT].nArithEnable;
        struCnfp.struGroupIdentify[l2][v2][g2].struLongShort[nArithSeq-ARITH_LONG_SHORT].nDispEnable  = struCnfp.struGroupIdentify[l1][v1][g1].struLongShort[nArithSeq-ARITH_LONG_SHORT].nDispEnable;
        break;
    case ARITH_BIG_SMALL:
    case ARITH_BIG_SMALL_B:
        struCnfp.struGroupIdentify[l2][v2][g2].struBigSmall[nArithSeq-ARITH_BIG_SMALL].nArithEnable       = struCnfp.struGroupIdentify[l1][v1][g1].struBigSmall[nArithSeq-ARITH_BIG_SMALL].nArithEnable;
        struCnfp.struGroupIdentify[l2][v2][g2].struBigSmall[nArithSeq-ARITH_BIG_SMALL].nDispEnable       = struCnfp.struGroupIdentify[l1][v1][g1].struBigSmall[nArithSeq-ARITH_BIG_SMALL].nDispEnable;
        break;
    case ARITH_HSV:
    case ARITH_HSV_B:
    case ARITH_HSV_C:
        struCnfp.struGroupIdentify[l2][v2][g2].struHsv[nArithSeq-ARITH_HSV].nArithEnable       = struCnfp.struGroupIdentify[l1][v1][g1].struHsv[nArithSeq-ARITH_HSV].nArithEnable;
        struCnfp.struGroupIdentify[l2][v2][g2].struHsv[nArithSeq-ARITH_HSV].nDispEnable       = struCnfp.struGroupIdentify[l1][v1][g1].struHsv[nArithSeq-ARITH_HSV].nDispEnable;
        break;
    case ARITH_SVM_A:
    case ARITH_SVM_B:
    case ARITH_SVM_C:
    case ARITH_SVM_D:
        struCnfp.struGroupIdentify[l2][v2][g2].struSvm[nArithSeq-ARITH_SVM_A].nDispEnable = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[nArithSeq-ARITH_SVM_A].nDispEnable;
        struCnfp.struGroupIdentify[l2][v2][g2].struSvm[nArithSeq-ARITH_SVM_A].nArithEnable = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[nArithSeq-ARITH_SVM_A].nArithEnable;
        break;
    case ARITH_SHAPE_SVM_A:
    case ARITH_SHAPE_SVM_B:
    case ARITH_SHAPE_SVM_C:
        struCnfp.struGroupIdentify[l2][v2][g2].struShapeSvm[nArithSeq-ARITH_SHAPE_SVM_A].nDispEnable = struCnfp.struGroupIdentify[l1][v1][g1].struShapeSvm[nArithSeq-ARITH_SHAPE_SVM_A].nDispEnable;
        struCnfp.struGroupIdentify[l2][v2][g2].struShapeSvm[nArithSeq-ARITH_SHAPE_SVM_A].nArithEnable = struCnfp.struGroupIdentify[l1][v1][g1].struShapeSvm[nArithSeq-ARITH_SHAPE_SVM_A].nArithEnable;
        break;
    case ARITH_SAT:
        struCnfp.struGroupIdentify[l2][v2][g2].struColorSat.nArithEnable       = struCnfp.struGroupIdentify[l1][v1][g1].struColorSat.nArithEnable;
        struCnfp.struGroupIdentify[l2][v2][g2].struColorSat.nDispEnable       = struCnfp.struGroupIdentify[l1][v1][g1].struColorSat.nDispEnable;
        break;
    case ARITH_HUE:
        struCnfp.struGroupIdentify[l2][v2][g2].struColorHue.nArithEnable       = struCnfp.struGroupIdentify[l1][v1][g1].struColorHue.nArithEnable;
        struCnfp.struGroupIdentify[l2][v2][g2].struColorHue.nDispEnable       = struCnfp.struGroupIdentify[l1][v1][g1].struColorHue.nDispEnable;
        break;
    case ARITH_SCALE:
        struCnfp.struGroupIdentify[l2][v2][g2].struColorScale.nArithEnable       = struCnfp.struGroupIdentify[l1][v1][g1].struColorScale.nArithEnable;
        struCnfp.struGroupIdentify[l2][v2][g2].struColorScale.nDispEnable       = struCnfp.struGroupIdentify[l1][v1][g1].struColorScale.nDispEnable;
        break;
    case ARITH_MILDEW:
        struCnfp.struGroupIdentify[l2][v2][g2].struMildew.nArithEnable       = struCnfp.struGroupIdentify[l1][v1][g1].struMildew.nArithEnable;
        struCnfp.struGroupIdentify[l2][v2][g2].struMildew.nDispEnable       = struCnfp.struGroupIdentify[l1][v1][g1].struMildew.nDispEnable;
        break;
    case ARITH_OUTLINE:
        struCnfp.struGroupIdentify[l2][v2][g2].struOutline.nArithEnable       = struCnfp.struGroupIdentify[l1][v1][g1].struOutline.nArithEnable;
        struCnfp.struGroupIdentify[l2][v2][g2].struOutline.nDispEnable       = struCnfp.struGroupIdentify[l1][v1][g1].struOutline.nDispEnable;
        break;
    case ARITH_LINE:
        struCnfp.struGroupIdentify[l2][v2][g2].struLine.nArithEnable       = struCnfp.struGroupIdentify[l1][v1][g1].struLine.nArithEnable;
        struCnfp.struGroupIdentify[l2][v2][g2].struLine.nDispEnable       = struCnfp.struGroupIdentify[l1][v1][g1].struLine.nDispEnable;
        break;
    case ARITH_BUD:
    case ARITH_BUD_B:
        struCnfp.struGroupIdentify[l2][v2][g2].struBud[nArithSeq-ARITH_BUD].nArithEnable       = struCnfp.struGroupIdentify[l1][v1][g1].struBud[nArithSeq-ARITH_BUD].nArithEnable;
        struCnfp.struGroupIdentify[l2][v2][g2].struBud[nArithSeq-ARITH_BUD].nDispEnable       = struCnfp.struGroupIdentify[l1][v1][g1].struBud[nArithSeq-ARITH_BUD].nDispEnable;
        break;
    case ARITH_ROUND:
    case ARITH_ROUND_B:
        struCnfp.struGroupIdentify[l2][v2][g2].struRound[nArithSeq-ARITH_ROUND].nArithEnable   = struCnfp.struGroupIdentify[l1][v1][g1].struRound[nArithSeq-ARITH_ROUND].nArithEnable;
        struCnfp.struGroupIdentify[l2][v2][g2].struRound[nArithSeq-ARITH_ROUND].nDispEnable    = struCnfp.struGroupIdentify[l1][v1][g1].struRound[nArithSeq-ARITH_ROUND].nDispEnable;
        break;
    case ARITH_SPL_MILDEW:
        struCnfp.struGroupIdentify[l2][v2][g2].struSplMildew.nArithEnable       = struCnfp.struGroupIdentify[l1][v1][g1].struSplMildew.nArithEnable;
        struCnfp.struGroupIdentify[l2][v2][g2].struSplMildew.nDispEnable       = struCnfp.struGroupIdentify[l1][v1][g1].struSplMildew.nDispEnable;
        break;
    case ARITH_W_PEANUT:
        struCnfp.struGroupIdentify[l2][v2][g2].struWhitePeanut.nArithEnable       = struCnfp.struGroupIdentify[l1][v1][g1].struWhitePeanut.nArithEnable;
        struCnfp.struGroupIdentify[l2][v2][g2].struWhitePeanut.nDispEnable       = struCnfp.struGroupIdentify[l1][v1][g1].struWhitePeanut.nDispEnable;
        break;
    case ARITH_PEPPER_HANDLE:
        struCnfp.struGroupIdentify[l2][v2][g2].struPepperHandle.nArithEnable       = struCnfp.struGroupIdentify[l1][v1][g1].struPepperHandle.nArithEnable;
        struCnfp.struGroupIdentify[l2][v2][g2].struPepperHandle.nDispEnable       = struCnfp.struGroupIdentify[l1][v1][g1].struPepperHandle.nDispEnable;
        break;
    case ARITH_SUPPRESS_LARGE_A:
    case ARITH_SUPPRESS_LARGE_B:
    case ARITH_SUPPRESS_LARGE_C:
        struCnfp.struGroupIdentify[l2][v2][g2].struSuppressLarge[nArithSeq-ARITH_SUPPRESS_LARGE_A].nDispEnable = struCnfp.struGroupIdentify[l1][v1][g1].struSuppressLarge[nArithSeq-ARITH_SUPPRESS_LARGE_A].nDispEnable;
        struCnfp.struGroupIdentify[l2][v2][g2].struSuppressLarge[nArithSeq-ARITH_SUPPRESS_LARGE_A].nArithEnable = struCnfp.struGroupIdentify[l1][v1][g1].struSuppressLarge[nArithSeq-ARITH_SUPPRESS_LARGE_A].nArithEnable;
         break;
    case ARITH_WHEAT_SPROUT:
        struCnfp.struGroupIdentify[l2][v2][g2].struWheatSprout.nArithEnable       = struCnfp.struGroupIdentify[l1][v1][g1].struWheatSprout.nArithEnable;
        struCnfp.struGroupIdentify[l2][v2][g2].struWheatSprout.nDispEnable       = struCnfp.struGroupIdentify[l1][v1][g1].struWheatSprout.nDispEnable;
        break;
    case TEA_ARITH_COLOR:
    case TEA_ARITH_COLOR_2:
    case TEA_ARITH_COLOR_3:
    case TEA_ARITH_COLOR_4:
    case TEA_ARITH_COLOR_5:
        struCnfp.struGroupIdentify[l2][v2][g2].struTeaColor[nArithSeq-TEA_ARITH_COLOR].nArithEnable       = struCnfp.struGroupIdentify[l1][v1][g1].struTeaColor[nArithSeq-TEA_ARITH_COLOR].nArithEnable;
        struCnfp.struGroupIdentify[l2][v2][g2].struTeaColor[nArithSeq-TEA_ARITH_COLOR].nDispEnable       = struCnfp.struGroupIdentify[l1][v1][g1].struTeaColor[nArithSeq-TEA_ARITH_COLOR].nDispEnable;
        break;

    default:
       break;
    }
}

#if 0
/***************************************************************************************************
    名称：         识别组显示使能和发送使能复制设置
    功能：         根据物料参数设置方式，计算获取准备发送物料参数的识别组编号
    nLayerId:     识别组所在的层编号
    nViewId:      识别组所在的视编号
    nGroupId：    识别组的组号
    nArithSeq：   识别算法序号
    bIsDisp:      是否是算法显示使能
***************************************************************************************************/
void GlobalFlow::materialArithEnableCopy(int nLayerId, int nViewId, int nGroupId, int nArithSeq, bool bIsDisp)
{
    int i, j;

    switch (struCnfg.nMachineType) {
    case MACHINE_VERTICAL_DM_RG:
    case MACHINE_VERTICAL_DM_ZK:
    case MACHINE_VERTICAL_ZL_CS:
    case MACHINE_VERTICAL_ZL_CV:
    case MACHINE_LD_ZL_GP:
    case MACHINE_LD2_ZL_GP:
        switch (struCnfp.nMatCopySetMode)
        {
        case MAT_COPY_ALL_ALONE:
            break;
        case MAT_COPY_REAR_ALIKE_FRONT:
            switch(nViewId)
            {
            case LCD_VERI_MAIN_VIEW_FRONT: //前视/前主
                struCnfp.nGroupArithEnable[nLayerId][LCD_VERI_MAIN_VIEW_REAR][nGroupId][nArithSeq] = struCnfp.nGroupArithEnable[nLayerId][nViewId][nGroupId][nArithSeq];
                if(bIsDisp == true)
                {
                    materialDispEnableCopy(nLayerId, LCD_VERI_MAIN_VIEW_FRONT, nGroupId, nLayerId, LCD_VERI_MAIN_VIEW_REAR, nGroupId, nArithSeq);
                }
                break;
            case LCD_VERI_VICE_VIEW_FRONT: //前辅
                struCnfp.nGroupArithEnable[nLayerId][LCD_VERI_VICE_VIEW_REAR][nGroupId][nArithSeq] = struCnfp.nGroupArithEnable[nLayerId][nViewId][nGroupId][nArithSeq];
                if(bIsDisp == true)
                {
                    materialDispEnableCopy(nLayerId, LCD_VERI_VICE_VIEW_FRONT, nGroupId, nLayerId, LCD_VERI_VICE_VIEW_REAR, nGroupId, nArithSeq);
                }
                break;
            default:
                break;
            }
            break;
        case MAT_COPY_SECOND_ALIKE_FIRST:
            if(nGroupId == 0)
            {
                for(i = 1; i < MAX_GROUP_IDENTIFY; i++)
                {
                    struCnfp.nGroupArithEnable[nLayerId][nViewId][i][nArithSeq] = struCnfp.nGroupArithEnable[nLayerId][nViewId][nGroupId][nArithSeq];

                    if(bIsDisp == true)
                    {
                        materialDispEnableCopy(nLayerId, nViewId, nGroupId, nLayerId, nViewId, i, nArithSeq);
                    }
                }
            }
            break;
        case MAT_COPY_ALL_SAME:
            if((nViewId == LCD_VERI_MAIN_VIEW_FRONT)&&(nGroupId == 0))
            {
                for(i = 0; i < struCnfg.struLayerInfo[nLayerId].nViewChainTotal; i++)
                {
                    for(j = 0; j < struCnfg.struLayerInfo[nLayerId].stuViewInfo[i].nIdentifyGroupTotal; j++)
                    {
                        if((i == 0)&&(j == 0))
                        {
                            continue;   //第一视的第一次已经在函数开始时发送过
                        }

                        struCnfp.nGroupArithEnable[nLayerId][i][j][nArithSeq] = struCnfp.nGroupArithEnable[nLayerId][nViewId][nGroupId][nArithSeq];

                        if(bIsDisp == true)
                        {
                            materialDispEnableCopy(nLayerId, nViewId, nGroupId, nLayerId, i, j, nArithSeq);
                        }
                    }
                }
            }
            break;
        default:
            break;
        }
        break;
//    case MACHINE_LD_ZL_GP:
//        switch (struCnfp.nMatCopySetMode)
//        {
//        case MAT_COPY_ALL_ALONE:
//            break;
//        case MAT_COPY_REAR_ALIKE_FRONT:
//            switch(nViewId)
//            {
//            case LCD_LD_MAIN_VIEW_UP: //前视/前主

//                break;
//            case LCD_LD_VICE_VIEW_UP: //前辅

//                break;
//            default:
//                break;
//            }
//            break;
//        case MAT_COPY_SECOND_ALIKE_FIRST: // 暂无
//            break;
//        case MAT_COPY_ALL_SAME:
//            if((nViewId == LCD_LD_MAIN_VIEW_UP)&&(nGroupId == 0))
//            {
//                for(i = 0; i < struCnfg.struLayerInfo[nLayerId].nViewChainTotal; i++)
//                {
//                    for(j = 0; j < struCnfg.struLayerInfo[nLayerId].stuViewInfo[i].nIdentifyGroupTotal; j++)
//                    {
//                        if((i == 0)&&(j == 0))
//                        {
//                            continue;   //第一视的第一次已经在函数开始时发送过
//                        }

//                    }
//                }
//            }
//            break;
//        default:
//            break;
//        }
//        break;
//    case MACHINE_LD2_ZL_GP:
//        switch (struCnfp.nMatCopySetMode)
//        {
//        case MAT_COPY_ALL_ALONE:
//            break;
//        case MAT_COPY_REAR_ALIKE_FRONT:
//            switch(nViewId)
//            {
//            case LCD_LD2_UPLAYER_VIEW_UP: //上层上视

//                break;
//            case LCD_LD2_LOWLAYER_VIEW_UP: //下层上视

//                break;
//            default:
//                break;
//            }
//            break;
//        case MAT_COPY_SECOND_ALIKE_FIRST: // 暂无
//            break;
//        case MAT_COPY_ALL_SAME:
//            if((nViewId == LCD_LD2_UPLAYER_VIEW_UP)&&(nGroupId == 0))
//            {
//                for(i = 0; i < struCnfg.struLayerInfo[nLayerId].nViewChainTotal; i++)
//                {
//                    for(j = 0; j < struCnfg.struLayerInfo[nLayerId].stuViewInfo[i].nIdentifyGroupTotal; j++)
//                    {
//                        if((i == 0)&&(j == 0))
//                        {
//                            continue;   //第一视的第一次已经在函数开始时发送过
//                        }

//                    }
//                }
//            }
//            break;
//        default:
//            break;
//        }
//        break;
    default:
        break;

    }
}
#endif

void GlobalFlow::materialArithEnableCopy(int nLayerId, int nViewId, int nGroupId, int nArithSeq, bool bIsDisp)
{
    int i, j;

    switch (struCnfg.nMachineType) {
    case MACHINE_VERTICAL_DM_RG:
    case MACHINE_VERTICAL_DM_ZK:
    case MACHINE_VERTICAL_ZL_CS:
    case MACHINE_VERTICAL_ZL_CV:
        switch (struCnfp.nMatCopySetMode)
        {
        case MAT_COPY_ALL_ALONE:
            break;
        case MAT_COPY_REAR_ALIKE_FRONT:
            switch(nViewId)
            {
            case LCD_VERI_MAIN_VIEW_FRONT: //前视/前主
                for(j = 0; j < struCnfg.struLayerInfo[nLayerId].stuViewInfo[nViewId].nIdentifyGroupTotal; j++)
                {
                    struCnfp.nGroupArithEnable[nLayerId][LCD_VERI_MAIN_VIEW_REAR][j][nArithSeq] = struCnfp.nGroupArithEnable[nLayerId][LCD_VERI_MAIN_VIEW_FRONT][j][nArithSeq];
                    if(bIsDisp == true)
                    {
                        materialDispEnableCopy(nLayerId, LCD_VERI_MAIN_VIEW_FRONT, j, nLayerId, LCD_VERI_MAIN_VIEW_REAR, j, nArithSeq);
                    }
                }
                break;
            case LCD_VERI_VICE_VIEW_FRONT: //前辅
                for(j = 0; j < struCnfg.struLayerInfo[nLayerId].stuViewInfo[nViewId].nIdentifyGroupTotal; j++)
                {
                    struCnfp.nGroupArithEnable[nLayerId][LCD_VERI_VICE_VIEW_REAR][j][nArithSeq] = struCnfp.nGroupArithEnable[nLayerId][LCD_VERI_VICE_VIEW_FRONT][j][nArithSeq];
                    if(bIsDisp == true)
                    {
                        materialDispEnableCopy(nLayerId, LCD_VERI_VICE_VIEW_FRONT, j, nLayerId, LCD_VERI_VICE_VIEW_REAR, j, nArithSeq);
                    }
                }
                break;
            default:
                break;
            }
            break;
        case MAT_COPY_SECOND_ALIKE_FIRST:
            if(nGroupId == 0)
            {
                for(i = 1; i < struCnfg.struLayerInfo[nLayerId].stuViewInfo[nViewId].nIdentifyGroupTotal; i++)
                {
                    struCnfp.nGroupArithEnable[nLayerId][nViewId][i][nArithSeq] = struCnfp.nGroupArithEnable[nLayerId][nViewId][nGroupId][nArithSeq];

                    if(bIsDisp == true)
                    {
                        materialDispEnableCopy(nLayerId, nViewId, nGroupId, nLayerId, nViewId, i, nArithSeq);
                    }
                }
            }
            break;
        case MAT_COPY_ALL_SAME:
            if((nViewId == LCD_VERI_MAIN_VIEW_FRONT)&&(nGroupId == 0))
            {
                for(i = 0; i < struCnfg.struLayerInfo[nLayerId].nViewChainTotal; i++)
                {
                    if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[i]) continue;
                    for(j = 0; j < struCnfg.struLayerInfo[nLayerId].stuViewInfo[i].nIdentifyGroupTotal; j++)
                    {
                        if((i == 0)&&(j == 0))
                        {
                            continue;   //第一视的第一次已经在函数开始时发送过
                        }

                        struCnfp.nGroupArithEnable[nLayerId][i][j][nArithSeq] = struCnfp.nGroupArithEnable[nLayerId][nViewId][nGroupId][nArithSeq];

                        if(bIsDisp == true)
                        {
                            materialDispEnableCopy(nLayerId, nViewId, nGroupId, nLayerId, i, j, nArithSeq);
                        }
                    }
                }
            }
            break;
        default:
            break;
        }
        break;
    case MACHINE_LD_ZL_GP:
        switch (struCnfp.nMatCopySetMode)
        {
        case MAT_COPY_ALL_ALONE:
            break;
        case MAT_COPY_REAR_ALIKE_FRONT:
            switch(nViewId)
            {
            case LCD_LD_MAIN_VIEW_UP: //前视/前主

                break;
            case LCD_LD_VICE_VIEW_UP: //前辅

                break;
            default:
                break;
            }
            break;
        case MAT_COPY_SECOND_ALIKE_FIRST: // 暂无
            break;
        case MAT_COPY_ALL_SAME:
            if((nViewId == LCD_LD_MAIN_VIEW_UP)&&(nGroupId == 0))
            {
                for(i = 0; i < struCnfg.struLayerInfo[nLayerId].nViewChainTotal; i++)
                {
                    if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[i]) continue;
                    for(j = 0; j < struCnfg.struLayerInfo[nLayerId].stuViewInfo[i].nIdentifyGroupTotal; j++)
                    {
                        if((i == 0)&&(j == 0))
                        {
                            continue;   //第一视的第一次已经在函数开始时发送过
                        }
                        struCnfp.nGroupArithEnable[nLayerId][i][j][nArithSeq] = struCnfp.nGroupArithEnable[nLayerId][nViewId][nGroupId][nArithSeq];

                        if(bIsDisp == true)
                        {
                            materialDispEnableCopy(nLayerId, nViewId, nGroupId, nLayerId, i, j, nArithSeq);
                        }

                    }
                }
            }
            break;
        default:
            break;
        }
        break;
    case MACHINE_LD2_ZL_GP:
        switch (struCnfp.nMatCopySetMode)
        {
        case MAT_COPY_ALL_ALONE:
            break;
        case MAT_COPY_REAR_ALIKE_FRONT:
            switch(nViewId)
            {
            case LCD_VERI_MAIN_VIEW_FRONT: //前视/前主
                for(j = 0; j < struCnfg.struLayerInfo[nLayerId].stuViewInfo[nViewId].nIdentifyGroupTotal; j++)
                {
                    struCnfp.nGroupArithEnable[nLayerId][LCD_VERI_MAIN_VIEW_REAR][j][nArithSeq] = struCnfp.nGroupArithEnable[nLayerId][LCD_VERI_MAIN_VIEW_FRONT][j][nArithSeq];
                    if(bIsDisp == true)
                    {
                        materialDispEnableCopy(nLayerId, LCD_VERI_MAIN_VIEW_FRONT, j, nLayerId, LCD_VERI_MAIN_VIEW_REAR, j, nArithSeq);
                    }
                }
                break;
            case LCD_VERI_VICE_VIEW_FRONT: //前辅
                for(j = 0; j < struCnfg.struLayerInfo[nLayerId].stuViewInfo[nViewId].nIdentifyGroupTotal; j++)
                {
                    struCnfp.nGroupArithEnable[nLayerId][LCD_VERI_VICE_VIEW_REAR][j][nArithSeq] = struCnfp.nGroupArithEnable[nLayerId][LCD_VERI_VICE_VIEW_FRONT][j][nArithSeq];
                    if(bIsDisp == true)
                    {
                        materialDispEnableCopy(nLayerId, LCD_VERI_VICE_VIEW_FRONT, j, nLayerId, LCD_VERI_VICE_VIEW_REAR, j, nArithSeq);
                    }
                }
                break;

            case LCD_VERI_MAIN_VIEW_FRONT_2: //前视/前主
                for(j = 0; j < struCnfg.struLayerInfo[nLayerId].stuViewInfo[nViewId].nIdentifyGroupTotal; j++)
                {
                    struCnfp.nGroupArithEnable[nLayerId][LCD_VERI_MAIN_VIEW_REAR_2][j][nArithSeq] = struCnfp.nGroupArithEnable[nLayerId][LCD_VERI_MAIN_VIEW_FRONT_2][j][nArithSeq];
                    if(bIsDisp == true)
                    {
                        materialDispEnableCopy(nLayerId, LCD_VERI_MAIN_VIEW_FRONT_2, j, nLayerId, LCD_VERI_MAIN_VIEW_REAR_2, j, nArithSeq);
                    }
                }
                break;
            case LCD_VERI_VICE_VIEW_FRONT_2: //前辅
                for(j = 0; j < struCnfg.struLayerInfo[nLayerId].stuViewInfo[nViewId].nIdentifyGroupTotal; j++)
                {
                    struCnfp.nGroupArithEnable[nLayerId][LCD_VERI_VICE_VIEW_REAR_2][j][nArithSeq] = struCnfp.nGroupArithEnable[nLayerId][LCD_VERI_VICE_VIEW_FRONT_2][j][nArithSeq];
                    if(bIsDisp == true)
                    {
                        materialDispEnableCopy(nLayerId, LCD_VERI_VICE_VIEW_FRONT_2, j, nLayerId, LCD_VERI_VICE_VIEW_REAR_2, j, nArithSeq);
                    }
                }
                break;
            default:
                break;
            }
            break;
        case MAT_COPY_SECOND_ALIKE_FIRST: // 暂无
            break;
        case MAT_COPY_ALL_SAME:
            if((nViewId == LCD_VERI_MAIN_VIEW_FRONT)&&(nGroupId == 0))
            {
                for(i = 0; i < struCnfg.struLayerInfo[nLayerId].nViewChainTotal; i++)
                {
                    if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[i]) continue;
                    for(j = 0; j < struCnfg.struLayerInfo[nLayerId].stuViewInfo[i].nIdentifyGroupTotal; j++)
                    {
                        if((i == 0)&&(j == 0))
                        {
                            continue;   //第一视的第一次已经在函数开始时发送过
                        }

                        struCnfp.nGroupArithEnable[nLayerId][i][j][nArithSeq] = struCnfp.nGroupArithEnable[nLayerId][nViewId][nGroupId][nArithSeq];

                        if(bIsDisp == true)
                        {
                            materialDispEnableCopy(nLayerId, nViewId, nGroupId, nLayerId, i, j, nArithSeq);
                        }
                    }
                }
            }
            break;
        default:
            break;
        }
        break;
    default:
        break;

    }
}
/***************************************************************************************************
    名称：         识别组发送编号确定
    功能：         根据物料参数设置方式，计算获取准备发送物料参数的识别组编号
    nLayerId:     识别组所在的层编号
    nViewId:      识别组所在的视编号
    nGroupId：    识别组的组号
    bIsAllArith   是否全部算法一起发送
    nArithSeq：   识别算法序号
    bIsReserved:  是否是发送保留参数
***************************************************************************************************/
void GlobalFlow::materialResetGroupAssemble(int nLayerId, int nViewId, int nGroupId, bool bIsAllArith, int nArithSeq, int bIsReserved)
{
    int i, j;
    resetMaterial(nLayerId, nViewId, nGroupId, bIsAllArith, nArithSeq, bIsReserved);
    switch (struCnfg.nMachineType) {
    case MACHINE_VERTICAL_DM_RG:
    case MACHINE_VERTICAL_DM_ZK:
    case MACHINE_VERTICAL_ZL_CS:
    case MACHINE_VERTICAL_ZL_CV:
        switch (struCnfp.nMatCopySetMode)
        {
        case MAT_COPY_ALL_ALONE:
            break;
        case MAT_COPY_REAR_ALIKE_FRONT:
            switch(nViewId)
            {
            case LCD_VERI_MAIN_VIEW_FRONT: //前视/前主
                resetMaterial(nLayerId, LCD_VERI_MAIN_VIEW_REAR, nGroupId, bIsAllArith, nArithSeq, bIsReserved);
                break;
            case LCD_VERI_VICE_VIEW_FRONT: //前辅
                resetMaterial(nLayerId, LCD_VERI_VICE_VIEW_REAR, nGroupId, bIsAllArith, nArithSeq, bIsReserved);
                break;
            default:
                break;
            }
            break;
        case MAT_COPY_SECOND_ALIKE_FIRST:
            if(nGroupId == 0)
            {
                for(i = 1; i < struCnfg.struLayerInfo[nLayerId].stuViewInfo[nViewId].nIdentifyGroupTotal; i++)
                {
                    resetMaterial(nLayerId, nViewId, i, bIsAllArith, nArithSeq, bIsReserved);
                }
            }
            break;
        case MAT_COPY_ALL_SAME:
            if((nViewId == LCD_VERI_MAIN_VIEW_FRONT)&&(nGroupId == 0))
            {
                for(i = 0; i < struCnfg.struLayerInfo[nLayerId].nViewChainTotal; i++)
                {
                    if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[i]) continue;
                    for(j = 0; j < struCnfg.struLayerInfo[nLayerId].stuViewInfo[i].nIdentifyGroupTotal; j++)
                    {
                        if((i == 0)&&(j == 0))
                        {
                            continue;   //第一视的第一次已经在函数开始时发送过
                        }

                        resetMaterial(nLayerId, i, j, bIsAllArith, nArithSeq, bIsReserved);
                    }
                }
            }
            break;
        default:
            break;
        }
        break;
    case MACHINE_LD_ZL_GP:
        switch (struCnfp.nMatCopySetMode)
        {
        case MAT_COPY_ALL_ALONE:
            break;
        case MAT_COPY_REAR_ALIKE_FRONT:
            switch(nViewId)
            {
            case LCD_LD_MAIN_VIEW_UP: //前视/前主
                resetMaterial(nLayerId, LCD_LD_MAIN_VIEW_LOW, nGroupId, bIsAllArith, nArithSeq, bIsReserved);
                break;
            case LCD_LD_VICE_VIEW_UP: //前辅
                resetMaterial(nLayerId, LCD_LD_VICE_VIEW_LOW, nGroupId, bIsAllArith, nArithSeq, bIsReserved);
                break;
            default:
                break;
            }
            break;
        case MAT_COPY_SECOND_ALIKE_FIRST: // 暂无
            break;
        case MAT_COPY_ALL_SAME:
            if((nViewId == LCD_LD_MAIN_VIEW_UP)&&(nGroupId == 0))
            {
                for(i = 0; i < struCnfg.struLayerInfo[nLayerId].nViewChainTotal; i++)
                {
                    if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[i]) continue;
                    for(j = 0; j < struCnfg.struLayerInfo[nLayerId].stuViewInfo[i].nIdentifyGroupTotal; j++)
                    {
                        if((i == 0)&&(j == 0))
                        {
                            continue;   //第一视的第一次已经在函数开始时发送过
                        }

                        resetMaterial(nLayerId, i, j, bIsAllArith, nArithSeq, bIsReserved);
                    }
                }
            }
            break;
        default:
            break;
        }
        break;
    case MACHINE_LD2_ZL_GP: //双层履带机
        switch (struCnfp.nMatCopySetMode)
        {
        case MAT_COPY_ALL_ALONE:
            break;
        case MAT_COPY_REAR_ALIKE_FRONT:
            switch(nViewId)
            {
            case LCD_VERI_MAIN_VIEW_FRONT: //前视/前主
                resetMaterial(nLayerId, LCD_VERI_MAIN_VIEW_REAR, nGroupId, bIsAllArith, nArithSeq, bIsReserved);
                break;
            case LCD_VERI_VICE_VIEW_FRONT: //前辅
                resetMaterial(nLayerId, LCD_VERI_VICE_VIEW_REAR, nGroupId, bIsAllArith, nArithSeq, bIsReserved);
                break;
            case LCD_VERI_MAIN_VIEW_FRONT_2: //前视/前主
                resetMaterial(nLayerId, LCD_VERI_MAIN_VIEW_REAR_2, nGroupId, bIsAllArith, nArithSeq, bIsReserved);
                break;
            case LCD_VERI_VICE_VIEW_FRONT_2: //前辅
                resetMaterial(nLayerId, LCD_VERI_VICE_VIEW_REAR_2, nGroupId, bIsAllArith, nArithSeq, bIsReserved);
                break;
            default:
                break;
            }
            break;
        case MAT_COPY_SECOND_ALIKE_FIRST: // 暂无
            break;
        case MAT_COPY_ALL_SAME:
            if((nViewId == LCD_VERI_MAIN_VIEW_FRONT)&&(nGroupId == 0))
            {
                for(i = 0; i < struCnfg.struLayerInfo[nLayerId].nViewChainTotal; i++)
                {
                    if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[i]) continue;
                    for(j = 0; j < struCnfg.struLayerInfo[nLayerId].stuViewInfo[i].nIdentifyGroupTotal; j++)
                    {
                        if((i == 0)&&(j == 0))
                        {
                            continue;   //第一视的第一次已经在函数开始时发送过
                        }

                        resetMaterial(nLayerId, i, j, bIsAllArith, nArithSeq, bIsReserved);
                    }
                }
            }
            break;
        default:
            break;
        }
        break;
    default:
        break;

    }
}

/***************************************************************************************************
    名称：         物料参数组间复制
    功能：         物料参数不同识别组直间的复制
    nLayerIdSrc:  准备复制的源识别组所在的层编号
    nViewIdSrc:   准备复制的源识别组所在的视编号
    nGroupIdSrc： 准备复制的源识别组编号
    nLayerIdDst： 准备复制的目标识别组所在的层编号
    nViewIdDst:   准备复制的目标识别组所在的视编号
    nGroupDst：   准备复制的目标识别组编号
    bIsAllArith：  不用
    nArithSeq：   识别算法序号
    bIsReserved:  是否是复制保留参数
***************************************************************************************************/
void GlobalFlow::materialParamsCopy(int nLayerIdSrc, int nViewIdSrc, int nGroupIdSrc, int nLayerIdDst, int nViewDst, int nGroupDst, bool bIsAllArith, int nArithSeq, int bIsReserved)
{
//    int nArithmeticEnable[MAX_LEVEL][ARITHMETIC_TOTAL];
    int nArithBegin, nArithEnd;
    int i, l1, l2, v1, v2, g1, g2;

    /* get arithmetic range */
    if (bIsAllArith == false) {
        nArithBegin = nArithSeq;
        nArithEnd   = nArithSeq+1;
    }
    else {
        nArithBegin = 0;
        nArithEnd   = ARITHMETIC_TOTAL;
    }

    l1 =  nLayerIdSrc;
    l2 =  nLayerIdDst;

    v1 =  nViewIdSrc;
    v2 =  nViewDst;

    g1 = nGroupIdSrc;
    g2 = nGroupDst;

    for (i = nArithBegin; i < nArithEnd; i++)
    {
//        if((struCnfp.nGroupArithEnable[l1][v1][g1][i] == 1)&&(struCnfp.nGroupArithEnable[l2][v2][g2][i] == 1))    //对应的组都使能，才允许复制
        {
           switch(i)
           {
            case ARITH_GREY_A:
            case ARITH_GREY_B:
            case ARITH_GREY_C:
            case ARITH_GREY_D:
            case ARITH_DISCOLOR_A:
            case ARITH_DISCOLOR_B:
            case ARITH_CROSS:
               memcpy(struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].sName, struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].sName,
                      sizeof(struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].sName));
               struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].nSensLow = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].nSensLow;
               struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].nSensUp = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].nSensUp;
               struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].nMode = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].nMode;
               struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].nRow = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].nRow;
               struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].nColumn  = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].nColumn;
               struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].nPercent = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].nPercent;
               struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].nVice = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].nVice;
               struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].nMain = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].nMain;
               struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].nDiffColor = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].nDiffColor;
               struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].lightLimit = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].lightLimit;

 //              struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].nDispEnable = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].nDispEnable;
//               struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].nEnable = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].nEnable;
               struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].nVavle = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].nVavle;
               struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].nResType = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].nResType;
               struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].nSensUpdate = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].nSensUpdate;
               struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].nEdgeType = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].nEdgeType;
               struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].nImpureType = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].nImpureType;
               struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].nPropResSens = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].nPropResSens;
               struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].nPropotionEnable = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].nPropotionEnable;
               struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].fPropotionSensLow = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].fPropotionSensLow;
               struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].fPropotionSensUp = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].fPropotionSensUp;
               struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].nLineDefectEnable = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].nLineDefectEnable;
               struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].nLineDefectIndex = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].nLineDefectIndex;
               struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].fnLineDefectAreaLow = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].fnLineDefectAreaLow;
               struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].fnLineDefectAreaUp = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].fnLineDefectAreaUp;
               struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].fnLineDefectLengthLow = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].fnLineDefectLengthLow;
               struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].fnLineDefectLengthUp = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].fnLineDefectLengthUp;

               struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].nEdge = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].nEdge;
               struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].nPropotionParaSensLow = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].nPropotionParaSensLow;
               struCnfp.nGroupArithEdgeEnable[l2][v2][g2][i] = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].nEdge;

               /* 颜色保留 */
               struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].struResGreyColor.nMode1 = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].struResGreyColor.nMode1;
               struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].struResGreyColor.nColor1 = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].struResGreyColor.nColor1;
               struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].struResGreyColor.nSensLow1 = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].struResGreyColor.nSensLow1;
               struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].struResGreyColor.nSensUp1 = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].struResGreyColor.nSensUp1;
               struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].struResGreyColor.nGreyResEnable = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].struResGreyColor.nGreyResEnable;
               struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].struResGreyColor.nMode2 = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].struResGreyColor.nMode2;
               struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].struResGreyColor.nMain2 = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].struResGreyColor.nMain2;
               struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].struResGreyColor.nVice2 = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].struResGreyColor.nVice2;
               struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].struResGreyColor.nResDiffColor = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].struResGreyColor.nResDiffColor;
               struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].struResGreyColor.nSensLow2 = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].struResGreyColor.nSensLow2;
               struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].struResGreyColor.nSensUp2 = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].struResGreyColor.nSensUp2;
               struCnfp.struGroupIdentify[l2][v2][g2].struGreyColor[i-ARITH_GREY_A].struResGreyColor.nDiscolorResEnable = struCnfp.struGroupIdentify[l1][v1][g1].struGreyColor[i-ARITH_GREY_A].struResGreyColor.nDiscolorResEnable;
               break;
           case ARITH_CIRCLE_LONG:
           case ARITH_CIRCLE_LONG_B:
               memcpy(struCnfp.struGroupIdentify[l2][v2][g2].struCircleLong[i-ARITH_CIRCLE_LONG].sName, struCnfp.struGroupIdentify[l1][v1][g1].struCircleLong[i-ARITH_CIRCLE_LONG].sName,
                      sizeof(struCnfp.struGroupIdentify[l1][v1][g1].struCircleLong[i-ARITH_CIRCLE_LONG].sName));
               struCnfp.struGroupIdentify[l2][v2][g2].struCircleLong[i-ARITH_CIRCLE_LONG].nCiloLow =  struCnfp.struGroupIdentify[l1][v1][g1].struCircleLong[i-ARITH_CIRCLE_LONG].nCiloLow;
               struCnfp.struGroupIdentify[l2][v2][g2].struCircleLong[i-ARITH_CIRCLE_LONG].nCilosUp = struCnfp.struGroupIdentify[l1][v1][g1].struCircleLong[i-ARITH_CIRCLE_LONG].nCilosUp;
               struCnfp.struGroupIdentify[l2][v2][g2].struCircleLong[i-ARITH_CIRCLE_LONG].nMode     = struCnfp.struGroupIdentify[l1][v1][g1].struCircleLong[i-ARITH_CIRCLE_LONG].nMode;
               struCnfp.struGroupIdentify[l2][v2][g2].struCircleLong[i-ARITH_CIRCLE_LONG].nTypeIndex     = struCnfp.struGroupIdentify[l1][v1][g1].struCircleLong[i-ARITH_CIRCLE_LONG].nTypeIndex;
               struCnfp.struGroupIdentify[l2][v2][g2].struCircleLong[i-ARITH_CIRCLE_LONG].nVavle     = struCnfp.struGroupIdentify[l1][v1][g1].struCircleLong[i-ARITH_CIRCLE_LONG].nVavle;
               struCnfp.struGroupIdentify[l2][v2][g2].struCircleLong[i-ARITH_CIRCLE_LONG].nLimit     = struCnfp.struGroupIdentify[l1][v1][g1].struCircleLong[i-ARITH_CIRCLE_LONG].nLimit;
               break;
           case ARITH_LONG_SHORT:
           case ARITH_LONG_SHORT_B:
               memcpy(struCnfp.struGroupIdentify[l2][v2][g2].struLongShort[i-ARITH_LONG_SHORT].sName, struCnfp.struGroupIdentify[l1][v1][g1].struLongShort[i-ARITH_LONG_SHORT].sName,
                      sizeof(struCnfp.struGroupIdentify[l1][v1][g1].struLongShort[i-ARITH_LONG_SHORT].sName));
               struCnfp.struGroupIdentify[l2][v2][g2].struLongShort[i-ARITH_LONG_SHORT].nLengthLow =  struCnfp.struGroupIdentify[l1][v1][g1].struLongShort[i-ARITH_LONG_SHORT].nLengthLow;
               struCnfp.struGroupIdentify[l2][v2][g2].struLongShort[i-ARITH_LONG_SHORT].nLengthUp = struCnfp.struGroupIdentify[l1][v1][g1].struLongShort[i-ARITH_LONG_SHORT].nLengthUp;
               struCnfp.struGroupIdentify[l2][v2][g2].struLongShort[i-ARITH_LONG_SHORT].nLengthMode  = struCnfp.struGroupIdentify[l1][v1][g1].struLongShort[i-ARITH_LONG_SHORT].nLengthMode;

               struCnfp.struGroupIdentify[l2][v2][g2].struLongShort[i-ARITH_LONG_SHORT].nTypeIndex  = struCnfp.struGroupIdentify[l1][v1][g1].struLongShort[i-ARITH_LONG_SHORT].nTypeIndex;
               struCnfp.struGroupIdentify[l2][v2][g2].struLongShort[i-ARITH_LONG_SHORT].nVavle  = struCnfp.struGroupIdentify[l1][v1][g1].struLongShort[i-ARITH_LONG_SHORT].nVavle;
               struCnfp.struGroupIdentify[l2][v2][g2].struLongShort[i-ARITH_LONG_SHORT].nLimit  = struCnfp.struGroupIdentify[l1][v1][g1].struLongShort[i-ARITH_LONG_SHORT].nLimit;
               break;
           case ARITH_BIG_SMALL:
           case ARITH_BIG_SMALL_B:
               memcpy(struCnfp.struGroupIdentify[l2][v2][g2].struBigSmall[i-ARITH_BIG_SMALL].sName, struCnfp.struGroupIdentify[l1][v1][g1].struBigSmall[i-ARITH_BIG_SMALL].sName,
                      sizeof(struCnfp.struGroupIdentify[l1][v1][g1].struBigSmall[i-ARITH_BIG_SMALL].sName));
               struCnfp.struGroupIdentify[l2][v2][g2].struBigSmall[i-ARITH_BIG_SMALL].nMode        = struCnfp.struGroupIdentify[l1][v1][g1].struBigSmall[i-ARITH_BIG_SMALL].nMode;
               struCnfp.struGroupIdentify[l2][v2][g2].struBigSmall[i-ARITH_BIG_SMALL].nAreaLow       = struCnfp.struGroupIdentify[l1][v1][g1].struBigSmall[i-ARITH_BIG_SMALL].nAreaLow;
               struCnfp.struGroupIdentify[l2][v2][g2].struBigSmall[i-ARITH_BIG_SMALL].nAreaUp       = struCnfp.struGroupIdentify[l1][v1][g1].struBigSmall[i-ARITH_BIG_SMALL].nAreaUp;
               struCnfp.struGroupIdentify[l2][v2][g2].struBigSmall[i-ARITH_BIG_SMALL].nTypeIndex       = struCnfp.struGroupIdentify[l1][v1][g1].struBigSmall[i-ARITH_BIG_SMALL].nTypeIndex;
               struCnfp.struGroupIdentify[l2][v2][g2].struBigSmall[i-ARITH_BIG_SMALL].nVavle       = struCnfp.struGroupIdentify[l1][v1][g1].struBigSmall[i-ARITH_BIG_SMALL].nVavle;
               struCnfp.struGroupIdentify[l2][v2][g2].struBigSmall[i-ARITH_BIG_SMALL].nLimit = struCnfp.struGroupIdentify[l1][v1][g1].struBigSmall[i-ARITH_BIG_SMALL].nLimit;
               break;
           case ARITH_HSV:
           case ARITH_HSV_B:
           case ARITH_HSV_C:
               memcpy(struCnfp.struGroupIdentify[l2][v2][g2].struHsv[i-ARITH_HSV].sName, struCnfp.struGroupIdentify[l1][v1][g1].struHsv[i-ARITH_HSV].sName,
                      sizeof(struCnfp.struGroupIdentify[l1][v1][g1].struHsv[i-ARITH_HSV].sName));
               struCnfp.struGroupIdentify[l2][v2][g2].struHsv[i-ARITH_HSV].nVavle        = struCnfp.struGroupIdentify[l1][v1][g1].struHsv[i-ARITH_HSV].nVavle;
               struCnfp.struGroupIdentify[l2][v2][g2].struHsv[i-ARITH_HSV].nRow       = struCnfp.struGroupIdentify[l1][v1][g1].struHsv[i-ARITH_HSV].nRow;
               struCnfp.struGroupIdentify[l2][v2][g2].struHsv[i-ARITH_HSV].nColumn       = struCnfp.struGroupIdentify[l1][v1][g1].struHsv[i-ARITH_HSV].nColumn;
               struCnfp.struGroupIdentify[l2][v2][g2].struHsv[i-ARITH_HSV].nPercent       = struCnfp.struGroupIdentify[l1][v1][g1].struHsv[i-ARITH_HSV].nPercent;
               struCnfp.struGroupIdentify[l2][v2][g2].struHsv[i-ARITH_HSV].nHsvColor       = struCnfp.struGroupIdentify[l1][v1][g1].struHsv[i-ARITH_HSV].nHsvColor;
               struCnfp.struGroupIdentify[l2][v2][g2].struHsv[i-ARITH_HSV].nHsvMode = struCnfp.struGroupIdentify[l1][v1][g1].struHsv[i-ARITH_HSV].nHsvMode;
               struCnfp.struGroupIdentify[l2][v2][g2].struHsv[i-ARITH_HSV].nHsvSensLowH = struCnfp.struGroupIdentify[l1][v1][g1].struHsv[i-ARITH_HSV].nHsvSensLowH;
               struCnfp.struGroupIdentify[l2][v2][g2].struHsv[i-ARITH_HSV].nHsvSensUpH = struCnfp.struGroupIdentify[l1][v1][g1].struHsv[i-ARITH_HSV].nHsvSensUpH;
               struCnfp.struGroupIdentify[l2][v2][g2].struHsv[i-ARITH_HSV].nHsvSensLowS = struCnfp.struGroupIdentify[l1][v1][g1].struHsv[i-ARITH_HSV].nHsvSensLowS;
               struCnfp.struGroupIdentify[l2][v2][g2].struHsv[i-ARITH_HSV].nHsvSensUpS = struCnfp.struGroupIdentify[l1][v1][g1].struHsv[i-ARITH_HSV].nHsvSensUpS;
               struCnfp.struGroupIdentify[l2][v2][g2].struHsv[i-ARITH_HSV].nHsvSensLowV = struCnfp.struGroupIdentify[l1][v1][g1].struHsv[i-ARITH_HSV].nHsvSensLowV;
               struCnfp.struGroupIdentify[l2][v2][g2].struHsv[i-ARITH_HSV].nHsvSensUpV = struCnfp.struGroupIdentify[l1][v1][g1].struHsv[i-ARITH_HSV].nHsvSensUpV;
               break;
           case ARITH_SVM_A:
           case ARITH_SVM_B:
           case ARITH_SVM_C:
           case ARITH_SVM_D:
                memcpy(struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].sName, struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].sName,
                      sizeof(struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].sName));
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nRow        = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nRow;
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nColumn     = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nColumn;
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nPercent    = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nPercent;
                struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nEdgeType        = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nEdgeType;
                struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nImpureType        = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nImpureType;
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nModeSvm        = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nModeSvm;
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nSens    = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nSens;

               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nQuoCons[0]    = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nQuoCons[0];
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nQuoCons[1]    = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nQuoCons[1];
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nQuoR[0]  = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nQuoR[0];
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nQuoR[1]  = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nQuoR[1];
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nQuoG[0]  = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nQuoG[0];
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nQuoG[1]  = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nQuoG[1];
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nQuoB[0]  = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nQuoB[0];
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nQuoB[1]  = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nQuoB[1];
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nQuoN[0]  = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nQuoN[0];
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nQuoN[1]  = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nQuoN[1];
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nQuoRR[0]    = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nQuoRR[0];
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nQuoRR[1]    = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nQuoRR[1];
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nQuoGG[0]  = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nQuoGG[0];
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nQuoGG[1]  = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nQuoGG[1];
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nQuoBB[0]  = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nQuoBB[0];
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nQuoBB[1]  = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nQuoBB[1];
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nQuoRG[0]  = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nQuoRG[0];
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nQuoRG[1]  = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nQuoRG[1];
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nQuoRB[0]    = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nQuoRB[0];
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nQuoRB[1]    = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nQuoRB[1];
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nQuoGB[0]  = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nQuoGB[0];
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nQuoGB[1]  = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nQuoGB[1];
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nQuoRN[0]  = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nQuoRN[0];
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nQuoRN[1]  = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nQuoRN[1];
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nQuoGN[0]  = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nQuoGN[0];
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nQuoGN[1]  = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nQuoGN[1];
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nQuoBN[0]  = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nQuoBN[0];
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nQuoBN[1]  = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nQuoBN[1];
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nQuoNN[0]  = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nQuoNN[0];
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nQuoNN[1]  = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nQuoNN[1];

               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nPropotionEnable = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nPropotionEnable;
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].fPropotionSensLow = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].fPropotionSensLow;
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].fPropotionSensUp = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].fPropotionSensUp;
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nPropotionParaSensLow = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A]. nPropotionParaSensLow;
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nLineDefectEnable = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nLineDefectEnable;
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nLineDefectIndex = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nLineDefectIndex;
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].fnLineDefectAreaLow = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].fnLineDefectAreaLow;
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].fnLineDefectAreaUp = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].fnLineDefectAreaUp;
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].fnLineDefectLengthLow = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].fnLineDefectLengthLow;
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].fnLineDefectLengthUp = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].fnLineDefectLengthUp;

//               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nDispEnable = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nDispEnable;
//               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nEnable    = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nEnable;
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nTypeIndex = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nTypeIndex;
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nVavle     = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nVavle;
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nSizeType  = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nSizeType;
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nEdge  = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nEdge;
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nModeType  = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nModeType;
               struCnfp.struGroupIdentify[l2][v2][g2].struSvm[i-ARITH_SVM_A].nConsExpandedValue  = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nConsExpandedValue;
               struCnfp.nGroupArithEdgeEnable[l2][v2][g2][i] = struCnfp.struGroupIdentify[l1][v1][g1].struSvm[i-ARITH_SVM_A].nEdge;

               break;

           case ARITH_SHAPE_SVM_A:
           case ARITH_SHAPE_SVM_B:
           case ARITH_SHAPE_SVM_C:
               memcpy(struCnfp.struGroupIdentify[l2][v2][g2].struShapeSvm[i-ARITH_SHAPE_SVM_A].sName, struCnfp.struGroupIdentify[l1][v1][g1].struShapeSvm[i-ARITH_SHAPE_SVM_A].sName,
                       sizeof(struCnfp.struGroupIdentify[l1][v1][g1].struShapeSvm[i-ARITH_SHAPE_SVM_A].sName));
               struCnfp.struGroupIdentify[l2][v2][g2].struShapeSvm[i-ARITH_SHAPE_SVM_A].nAreaLow        = struCnfp.struGroupIdentify[l1][v1][g1].struShapeSvm[i-ARITH_SHAPE_SVM_A].nAreaLow;
               struCnfp.struGroupIdentify[l2][v2][g2].struShapeSvm[i-ARITH_SHAPE_SVM_A].nAreaUp       = struCnfp.struGroupIdentify[l1][v1][g1].struShapeSvm[i-ARITH_SHAPE_SVM_A].nAreaUp;
               struCnfp.struGroupIdentify[l2][v2][g2].struShapeSvm[i-ARITH_SHAPE_SVM_A].nModeSvm        = struCnfp.struGroupIdentify[l1][v1][g1].struShapeSvm[i-ARITH_SHAPE_SVM_A].nModeSvm;
               struCnfp.struGroupIdentify[l2][v2][g2].struShapeSvm[i-ARITH_SHAPE_SVM_A].nSens    = struCnfp.struGroupIdentify[l1][v1][g1].struShapeSvm[i-ARITH_SHAPE_SVM_A].nSens;

               struCnfp.struGroupIdentify[l2][v2][g2].struShapeSvm[i-ARITH_SHAPE_SVM_A].nQuoCons[0]    = struCnfp.struGroupIdentify[l1][v1][g1].struShapeSvm[i-ARITH_SHAPE_SVM_A].nQuoCons[0];
               struCnfp.struGroupIdentify[l2][v2][g2].struShapeSvm[i-ARITH_SHAPE_SVM_A].nQuoCons[1]    = struCnfp.struGroupIdentify[l1][v1][g1].struShapeSvm[i-ARITH_SHAPE_SVM_A].nQuoCons[1];
               struCnfp.struGroupIdentify[l2][v2][g2].struShapeSvm[i-ARITH_SHAPE_SVM_A].nQuoA[0]  = struCnfp.struGroupIdentify[l1][v1][g1].struShapeSvm[i-ARITH_SHAPE_SVM_A].nQuoA[0];
               struCnfp.struGroupIdentify[l2][v2][g2].struShapeSvm[i-ARITH_SHAPE_SVM_A].nQuoA[1]  = struCnfp.struGroupIdentify[l1][v1][g1].struShapeSvm[i-ARITH_SHAPE_SVM_A].nQuoA[1];
               struCnfp.struGroupIdentify[l2][v2][g2].struShapeSvm[i-ARITH_SHAPE_SVM_A].nQuoB[0]  = struCnfp.struGroupIdentify[l1][v1][g1].struShapeSvm[i-ARITH_SHAPE_SVM_A].nQuoB[0];
               struCnfp.struGroupIdentify[l2][v2][g2].struShapeSvm[i-ARITH_SHAPE_SVM_A].nQuoB[1]  = struCnfp.struGroupIdentify[l1][v1][g1].struShapeSvm[i-ARITH_SHAPE_SVM_A].nQuoB[1];
               struCnfp.struGroupIdentify[l2][v2][g2].struShapeSvm[i-ARITH_SHAPE_SVM_A].nQuoC[0]  = struCnfp.struGroupIdentify[l1][v1][g1].struShapeSvm[i-ARITH_SHAPE_SVM_A].nQuoC[0];
               struCnfp.struGroupIdentify[l2][v2][g2].struShapeSvm[i-ARITH_SHAPE_SVM_A].nQuoC[1]  = struCnfp.struGroupIdentify[l1][v1][g1].struShapeSvm[i-ARITH_SHAPE_SVM_A].nQuoC[1];
               struCnfp.struGroupIdentify[l2][v2][g2].struShapeSvm[i-ARITH_SHAPE_SVM_A].nQuoD[0]  = struCnfp.struGroupIdentify[l1][v1][g1].struShapeSvm[i-ARITH_SHAPE_SVM_A].nQuoD[0];
               struCnfp.struGroupIdentify[l2][v2][g2].struShapeSvm[i-ARITH_SHAPE_SVM_A].nQuoD[1]  = struCnfp.struGroupIdentify[l1][v1][g1].struShapeSvm[i-ARITH_SHAPE_SVM_A].nQuoD[1];
               struCnfp.struGroupIdentify[l2][v2][g2].struShapeSvm[i-ARITH_SHAPE_SVM_A].nQuoE[0]  = struCnfp.struGroupIdentify[l1][v1][g1].struShapeSvm[i-ARITH_SHAPE_SVM_A].nQuoE[0];
               struCnfp.struGroupIdentify[l2][v2][g2].struShapeSvm[i-ARITH_SHAPE_SVM_A].nQuoE[1]  = struCnfp.struGroupIdentify[l1][v1][g1].struShapeSvm[i-ARITH_SHAPE_SVM_A].nQuoE[1];
               struCnfp.struGroupIdentify[l2][v2][g2].struShapeSvm[i-ARITH_SHAPE_SVM_A].nQuoF[0]  = struCnfp.struGroupIdentify[l1][v1][g1].struShapeSvm[i-ARITH_SHAPE_SVM_A].nQuoF[0];
               struCnfp.struGroupIdentify[l2][v2][g2].struShapeSvm[i-ARITH_SHAPE_SVM_A].nQuoF[1]  = struCnfp.struGroupIdentify[l1][v1][g1].struShapeSvm[i-ARITH_SHAPE_SVM_A].nQuoF[1];

               struCnfp.struGroupIdentify[l2][v2][g2].struShapeSvm[i-ARITH_SHAPE_SVM_A].nEdge  = struCnfp.struGroupIdentify[l1][v1][g1].struShapeSvm[i-ARITH_SHAPE_SVM_A].nEdge;
               struCnfp.nGroupArithEdgeEnable[l2][v2][g2][i] = struCnfp.struGroupIdentify[l1][v1][g1].struShapeSvm[i-ARITH_SHAPE_SVM_A].nEdge;

               break;
           case ARITH_SAT:
               memcpy(struCnfp.struGroupIdentify[l2][v2][g2].struColorSat.sName, struCnfp.struGroupIdentify[l1][v1][g1].struColorSat.sName,
                      sizeof(struCnfp.struGroupIdentify[l1][v1][g1].struColorSat.sName));
               struCnfp.struGroupIdentify[l2][v2][g2].struColorSat.nMode        = struCnfp.struGroupIdentify[l1][v1][g1].struColorSat.nMode;
               struCnfp.struGroupIdentify[l2][v2][g2].struColorSat.nRow         = struCnfp.struGroupIdentify[l1][v1][g1].struColorSat.nRow;
               struCnfp.struGroupIdentify[l2][v2][g2].struColorSat.nPercent     = struCnfp.struGroupIdentify[l1][v1][g1].struColorSat.nPercent;
               struCnfp.struGroupIdentify[l2][v2][g2].struColorSat.nSensLow     = struCnfp.struGroupIdentify[l1][v1][g1].struColorSat.nSensLow;
               struCnfp.struGroupIdentify[l2][v2][g2].struColorSat.nSensUp      = struCnfp.struGroupIdentify[l1][v1][g1].struColorSat.nSensUp;
               struCnfp.struGroupIdentify[l2][v2][g2].struColorSat.lightLimit   = struCnfp.struGroupIdentify[l1][v1][g1].struColorSat.lightLimit;
               break;
           case ARITH_SCALE:
               memcpy(struCnfp.struGroupIdentify[l2][v2][g2].struColorScale.sName, struCnfp.struGroupIdentify[l1][v1][g1].struColorScale.sName,
                      sizeof(struCnfp.struGroupIdentify[l1][v1][g1].struColorScale.sName));
               struCnfp.struGroupIdentify[l2][v2][g2].struColorScale.nSenIdx        = struCnfp.struGroupIdentify[l1][v1][g1].struColorScale.nSenIdx;
               struCnfp.struGroupIdentify[l2][v2][g2].struColorScale.nSen         = struCnfp.struGroupIdentify[l1][v1][g1].struColorScale.nSen;
               struCnfp.struGroupIdentify[l2][v2][g2].struColorScale.nLimit     = struCnfp.struGroupIdentify[l1][v1][g1].struColorScale.nLimit;
               break;
           case ARITH_HUE:
               memcpy(struCnfp.struGroupIdentify[l2][v2][g2].struColorHue.sName, struCnfp.struGroupIdentify[l1][v1][g1].struColorHue.sName,
                      sizeof(struCnfp.struGroupIdentify[l1][v1][g1].struColorHue.sName));
               struCnfp.struGroupIdentify[l2][v2][g2].struColorHue.nRow         = struCnfp.struGroupIdentify[l1][v1][g1].struColorHue.nRow;
               struCnfp.struGroupIdentify[l2][v2][g2].struColorHue.nColumn      = struCnfp.struGroupIdentify[l1][v1][g1].struColorHue.nColumn;
               struCnfp.struGroupIdentify[l2][v2][g2].struColorHue.nPercent     = struCnfp.struGroupIdentify[l1][v1][g1].struColorHue.nPercent;
               struCnfp.struGroupIdentify[l2][v2][g2].struColorHue.nHighHue     = struCnfp.struGroupIdentify[l1][v1][g1].struColorHue.nHighHue;
               struCnfp.struGroupIdentify[l2][v2][g2].struColorHue.nLowHue      = struCnfp.struGroupIdentify[l1][v1][g1].struColorHue.nLowHue;
               struCnfp.struGroupIdentify[l2][v2][g2].struColorHue.lightLimit   = struCnfp.struGroupIdentify[l1][v1][g1].struColorHue.lightLimit;
               break;
           case ARITH_MILDEW:
               memcpy(struCnfp.struGroupIdentify[l2][v2][g2].struMildew.sName, struCnfp.struGroupIdentify[l1][v1][g1].struMildew.sName,
                       sizeof(struCnfp.struGroupIdentify[l1][v1][g1].struMildew.sName));
               struCnfp.struGroupIdentify[l2][v2][g2].struMildew.nSens      = struCnfp.struGroupIdentify[l1][v1][g1].struMildew.nSens;
               struCnfp.struGroupIdentify[l2][v2][g2].struMildew.nPercent   = struCnfp.struGroupIdentify[l1][v1][g1].struMildew.nPercent;
               struCnfp.struGroupIdentify[l2][v2][g2].struMildew.nLimit     = struCnfp.struGroupIdentify[l1][v1][g1].struMildew.nLimit;
               struCnfp.struGroupIdentify[l2][v2][g2].struMildew.nMark      = struCnfp.struGroupIdentify[l1][v1][g1].struMildew.nMark;
               struCnfp.struGroupIdentify[l2][v2][g2].struMildew.nType      = struCnfp.struGroupIdentify[l1][v1][g1].struMildew.nType;
                break;
           case ARITH_OUTLINE:
               memcpy(struCnfp.struGroupIdentify[l2][v2][g2].struOutline.sName, struCnfp.struGroupIdentify[l1][v1][g1].struOutline.sName,
                       sizeof(struCnfp.struGroupIdentify[l1][v1][g1].struOutline.sName));
               struCnfp.struGroupIdentify[l2][v2][g2].struOutline.nSens         = struCnfp.struGroupIdentify[l1][v1][g1].struOutline.nSens;
               struCnfp.struGroupIdentify[l2][v2][g2].struOutline.nPercent      = struCnfp.struGroupIdentify[l1][v1][g1].struOutline.nPercent;
               struCnfp.struGroupIdentify[l2][v2][g2].struOutline.nHighLimit    = struCnfp.struGroupIdentify[l1][v1][g1].struOutline.nHighLimit;
               struCnfp.struGroupIdentify[l2][v2][g2].struOutline.nLowLimit     = struCnfp.struGroupIdentify[l1][v1][g1].struOutline.nLowLimit;
               break;
           case ARITH_LINE:
               memcpy(struCnfp.struGroupIdentify[l2][v2][g2].struLine.sName, struCnfp.struGroupIdentify[l1][v1][g1].struLine.sName,
                       sizeof(struCnfp.struGroupIdentify[l1][v1][g1].struLine.sName));
               struCnfp.struGroupIdentify[l2][v2][g2].struLine.nColor       = struCnfp.struGroupIdentify[l1][v1][g1].struLine.nColor;
               struCnfp.struGroupIdentify[l2][v2][g2].struLine.nRow         = struCnfp.struGroupIdentify[l1][v1][g1].struLine.nRow;
               struCnfp.struGroupIdentify[l2][v2][g2].struLine.nColumn      = struCnfp.struGroupIdentify[l1][v1][g1].struLine.nColumn;
               struCnfp.struGroupIdentify[l2][v2][g2].struLine.nSize        = struCnfp.struGroupIdentify[l1][v1][g1].struLine.nSize;
               struCnfp.struGroupIdentify[l2][v2][g2].struLine.nPercent     = struCnfp.struGroupIdentify[l1][v1][g1].struLine.nPercent;
               struCnfp.struGroupIdentify[l2][v2][g2].struLine.nSens1       = struCnfp.struGroupIdentify[l1][v1][g1].struLine.nSens1;
               struCnfp.struGroupIdentify[l2][v2][g2].struLine.nSens2       = struCnfp.struGroupIdentify[l1][v1][g1].struLine.nSens2;
               break;
           case ARITH_BUD:
           case ARITH_BUD_B:
               memcpy(struCnfp.struGroupIdentify[l2][v2][g2].struBud[i-ARITH_BUD].sName, struCnfp.struGroupIdentify[l1][v1][g1].struBud[i-ARITH_BUD].sName,
                       sizeof(struCnfp.struGroupIdentify[l1][v1][g1].struBud[i-ARITH_BUD].sName));
               struCnfp.struGroupIdentify[l2][v2][g2].struBud[i-ARITH_BUD].nSize         = struCnfp.struGroupIdentify[l1][v1][g1].struBud[i-ARITH_BUD].nSize;
               struCnfp.struGroupIdentify[l2][v2][g2].struBud[i-ARITH_BUD].nSens         = struCnfp.struGroupIdentify[l1][v1][g1].struBud[i-ARITH_BUD].nSens;
               struCnfp.struGroupIdentify[l2][v2][g2].struBud[i-ARITH_BUD].nLimit        = struCnfp.struGroupIdentify[l1][v1][g1].struBud[i-ARITH_BUD].nLimit;
               struCnfp.struGroupIdentify[l2][v2][g2].struBud[i-ARITH_BUD].nYellowSens   = struCnfp.struGroupIdentify[l1][v1][g1].struBud[i-ARITH_BUD].nYellowSens;
               struCnfp.struGroupIdentify[l2][v2][g2].struBud[i-ARITH_BUD].nBlackSens    = struCnfp.struGroupIdentify[l1][v1][g1].struBud[i-ARITH_BUD].nBlackSens;
               break;
           case ARITH_VALUE:
               memcpy(struCnfp.struGroupIdentify[l2][v2][g2].struColorValue.sName, struCnfp.struGroupIdentify[l1][v1][g1].struColorValue.sName,
                      sizeof(struCnfp.struGroupIdentify[l1][v1][g1].struColorValue.sName));
               struCnfp.struGroupIdentify[l2][v2][g2].struColorValue.nMode        = struCnfp.struGroupIdentify[l1][v1][g1].struColorValue.nMode;
               struCnfp.struGroupIdentify[l2][v2][g2].struColorValue.nRow         = struCnfp.struGroupIdentify[l1][v1][g1].struColorValue.nRow;
               struCnfp.struGroupIdentify[l2][v2][g2].struColorValue.nPercent     = struCnfp.struGroupIdentify[l1][v1][g1].struColorValue.nPercent;
               struCnfp.struGroupIdentify[l2][v2][g2].struColorValue.nSensLow     = struCnfp.struGroupIdentify[l1][v1][g1].struColorValue.nSensLow;
               struCnfp.struGroupIdentify[l2][v2][g2].struColorValue.nSensUp      = struCnfp.struGroupIdentify[l1][v1][g1].struColorValue.nSensUp;
               break;
           case ARITH_ROUND:
           case ARITH_ROUND_B:
               memcpy(struCnfp.struGroupIdentify[l2][v2][g2].struRound[i-ARITH_ROUND].sName, struCnfp.struGroupIdentify[l1][v1][g1].struRound[i-ARITH_ROUND].sName,
                      sizeof(struCnfp.struGroupIdentify[l1][v1][g1].struRound[i-ARITH_ROUND].sName));
               struCnfp.struGroupIdentify[l2][v2][g2].struRound[i-ARITH_ROUND].nSensUp =  struCnfp.struGroupIdentify[l1][v1][g1].struRound[i-ARITH_ROUND].nSensUp;
               struCnfp.struGroupIdentify[l2][v2][g2].struRound[i-ARITH_ROUND].nSensLow = struCnfp.struGroupIdentify[l1][v1][g1].struRound[i-ARITH_ROUND].nSensLow;
               struCnfp.struGroupIdentify[l2][v2][g2].struRound[i-ARITH_ROUND].nSelectMode     = struCnfp.struGroupIdentify[l1][v1][g1].struRound[i-ARITH_ROUND].nSelectMode;
               struCnfp.struGroupIdentify[l2][v2][g2].struRound[i-ARITH_ROUND].nLimit     = struCnfp.struGroupIdentify[l1][v1][g1].struRound[i-ARITH_ROUND].nLimit;
               break;
           case ARITH_SPL_MILDEW:
               memcpy(struCnfp.struGroupIdentify[l2][v2][g2].struSplMildew.sName, struCnfp.struGroupIdentify[l1][v1][g1].struSplMildew.sName,
                      sizeof(struCnfp.struGroupIdentify[l1][v1][g1].struSplMildew.sName));
               struCnfp.struGroupIdentify[l2][v2][g2].struSplMildew.nSens      = struCnfp.struGroupIdentify[l1][v1][g1].struSplMildew.nSens;
               break;
           case ARITH_W_PEANUT:
               memcpy( struCnfp.struGroupIdentify[l2][v2][g2].struWhitePeanut.sName, struCnfp.struGroupIdentify[l1][v1][g1].struWhitePeanut.sName,
                       sizeof(struCnfp.struGroupIdentify[l1][v1][g1].struWhitePeanut.sName));
               struCnfp.struGroupIdentify[l2][v2][g2].struWhitePeanut.nSensLow      = struCnfp.struGroupIdentify[l1][v1][g1].struWhitePeanut.nSensLow;
               struCnfp.struGroupIdentify[l2][v2][g2].struWhitePeanut.nSensUp       = struCnfp.struGroupIdentify[l1][v1][g1].struWhitePeanut.nSensUp;
               break;
           case ARITH_PEPPER_HANDLE:
               memcpy( struCnfp.struGroupIdentify[l2][v2][g2].struPepperHandle.sName, struCnfp.struGroupIdentify[l1][v1][g1].struPepperHandle.sName,
                       sizeof(struCnfp.struGroupIdentify[l1][v1][g1].struPepperHandle.sName));
               struCnfp.struGroupIdentify[l2][v2][g2].struPepperHandle.nSens      = struCnfp.struGroupIdentify[l1][v1][g1].struPepperHandle.nSens;
               struCnfp.struGroupIdentify[l2][v2][g2].struPepperHandle.nPercent   = struCnfp.struGroupIdentify[l1][v1][g1].struPepperHandle.nPercent;
               struCnfp.struGroupIdentify[l2][v2][g2].struPepperHandle.nLimit     = struCnfp.struGroupIdentify[l1][v1][g1].struPepperHandle.nLimit;
               break;
           case ARITH_SUPPRESS_LARGE_A:
           case ARITH_SUPPRESS_LARGE_B:
           case ARITH_SUPPRESS_LARGE_C:
               memcpy(struCnfp.struGroupIdentify[l2][v2][g2].struSuppressLarge[i-ARITH_SUPPRESS_LARGE_A].sName, struCnfp.struGroupIdentify[l1][v1][g1].struSuppressLarge[i-ARITH_SUPPRESS_LARGE_A].sName,
                       sizeof(struCnfp.struGroupIdentify[l1][v1][g1].struSuppressLarge[i-ARITH_SUPPRESS_LARGE_A].sName));
               struCnfp.struGroupIdentify[l2][v2][g2].struSuppressLarge[i-ARITH_SUPPRESS_LARGE_A].nSize1        = struCnfp.struGroupIdentify[l1][v1][g1].struSuppressLarge[i-ARITH_SUPPRESS_LARGE_A].nSize1;
               struCnfp.struGroupIdentify[l2][v2][g2].struSuppressLarge[i-ARITH_SUPPRESS_LARGE_A].nSize2        = struCnfp.struGroupIdentify[l1][v1][g1].struSuppressLarge[i-ARITH_SUPPRESS_LARGE_A].nSize2;
               struCnfp.struGroupIdentify[l2][v2][g2].struSuppressLarge[i-ARITH_SUPPRESS_LARGE_A].nPercent1     = struCnfp.struGroupIdentify[l1][v1][g1].struSuppressLarge[i-ARITH_SUPPRESS_LARGE_A].nPercent1;
               struCnfp.struGroupIdentify[l2][v2][g2].struSuppressLarge[i-ARITH_SUPPRESS_LARGE_A].nPercent2     = struCnfp.struGroupIdentify[l1][v1][g1].struSuppressLarge[i-ARITH_SUPPRESS_LARGE_A].nPercent2;
               struCnfp.struGroupIdentify[l2][v2][g2].struSuppressLarge[i-ARITH_SUPPRESS_LARGE_A].nSenIdx       = struCnfp.struGroupIdentify[l1][v1][g1].struSuppressLarge[i-ARITH_SUPPRESS_LARGE_A].nSenIdx;
               struCnfp.struGroupIdentify[l2][v2][g2].struSuppressLarge[i-ARITH_SUPPRESS_LARGE_A].nTurns        = struCnfp.struGroupIdentify[l1][v1][g1].struSuppressLarge[i-ARITH_SUPPRESS_LARGE_A].nTurns;
               break;
           case ARITH_WHEAT_SPROUT:
               memcpy( struCnfp.struGroupIdentify[l2][v2][g2].struWheatSprout.sName, struCnfp.struGroupIdentify[l1][v1][g1].struWheatSprout.sName,
                       sizeof(struCnfp.struGroupIdentify[l1][v1][g1].struWheatSprout.sName));
               struCnfp.struGroupIdentify[l2][v2][g2].struWheatSprout.nLow      = struCnfp.struGroupIdentify[l1][v1][g1].struWheatSprout.nLow;
               struCnfp.struGroupIdentify[l2][v2][g2].struWheatSprout.nPercent   = struCnfp.struGroupIdentify[l1][v1][g1].struWheatSprout.nPercent;
               struCnfp.struGroupIdentify[l2][v2][g2].struWheatSprout.nSense     = struCnfp.struGroupIdentify[l1][v1][g1].struWheatSprout.nSense;
               struCnfp.struGroupIdentify[l2][v2][g2].struWheatSprout.nH0     = struCnfp.struGroupIdentify[l1][v1][g1].struWheatSprout.nH0;
               struCnfp.struGroupIdentify[l2][v2][g2].struWheatSprout.nH1     = struCnfp.struGroupIdentify[l1][v1][g1].struWheatSprout.nH1;
               break;
           case TEA_ARITH_COLOR:
           case TEA_ARITH_COLOR_2:
           case TEA_ARITH_COLOR_3:
           case TEA_ARITH_COLOR_4:
           case TEA_ARITH_COLOR_5:
               memcpy( struCnfp.struGroupIdentify[l2][v2][g2].struTeaColor[i-TEA_ARITH_COLOR].sName, struCnfp.struGroupIdentify[l1][v1][g1].struTeaColor[i-TEA_ARITH_COLOR].sName,
                       sizeof(struCnfp.struGroupIdentify[l1][v1][g1].struTeaColor[i-TEA_ARITH_COLOR].sName));

               struCnfp.struGroupIdentify[l2][v2][g2].struTeaColor[i-TEA_ARITH_COLOR].tc       = struCnfp.struGroupIdentify[l1][v1][g1].struTeaColor[i-TEA_ARITH_COLOR].tc;
               struCnfp.struGroupIdentify[l2][v2][g2].struTeaColor[i-TEA_ARITH_COLOR].ec       = struCnfp.struGroupIdentify[l1][v1][g1].struTeaColor[i-TEA_ARITH_COLOR].ec;
               struCnfp.struGroupIdentify[l2][v2][g2].struTeaColor[i-TEA_ARITH_COLOR].ac       = struCnfp.struGroupIdentify[l1][v1][g1].struTeaColor[i-TEA_ARITH_COLOR].ac;
               struCnfp.struGroupIdentify[l2][v2][g2].struTeaColor[i-TEA_ARITH_COLOR].sense    = struCnfp.struGroupIdentify[l1][v1][g1].struTeaColor[i-TEA_ARITH_COLOR].sense;
               struCnfp.struGroupIdentify[l2][v2][g2].struTeaColor[i-TEA_ARITH_COLOR].nRow     = struCnfp.struGroupIdentify[l1][v1][g1].struTeaColor[i-TEA_ARITH_COLOR].nRow;
               struCnfp.struGroupIdentify[l2][v2][g2].struTeaColor[i-TEA_ARITH_COLOR].nColumn  = struCnfp.struGroupIdentify[l1][v1][g1].struTeaColor[i-TEA_ARITH_COLOR].nColumn;
               struCnfp.struGroupIdentify[l2][v2][g2].struTeaColor[i-TEA_ARITH_COLOR].nPercent = struCnfp.struGroupIdentify[l1][v1][g1].struTeaColor[i-TEA_ARITH_COLOR].nPercent;
               struCnfp.struGroupIdentify[l2][v2][g2].struTeaColor[i-TEA_ARITH_COLOR].nMode    = struCnfp.struGroupIdentify[l1][v1][g1].struTeaColor[i-TEA_ARITH_COLOR].nMode;
               struCnfp.struGroupIdentify[l2][v2][g2].struTeaColor[i-TEA_ARITH_COLOR].nEdge    = struCnfp.struGroupIdentify[l1][v1][g1].struTeaColor[i-TEA_ARITH_COLOR].nEdge;
               break;
           default:
               break;
           }
        }
    }
}

/***************************************************************************************************
    名称：         识别组复制编号确定
    功能：         根据物料参数设置方式，计算获取物料参数需要复制的组编号
    nLayerId:    识别组所在的层编号
    nViewId:     识别组所在的视编号
    nGroupId：    识别组的组编号
    bIsAllArith：  不用
    nArithSeq：   识别算法序号
    bIsReserved:  是否是复制保留参数,暂时不用
    bNeedSend：   判断是否需要发送
***************************************************************************************************/
void GlobalFlow::materialParamsCopySendAssemble(int nLayerId, int nViewId, int nGroupId, bool bIsAllArith, int nArithSeq, int bIsReserved, bool bNeedSend)
{
    int i, j;

    switch (struCnfg.nMachineType) {
    case MACHINE_VERTICAL_DM_RG:    //立式大米机
    case MACHINE_VERTICAL_DM_ZK:
    case MACHINE_VERTICAL_ZL_CS:
    case MACHINE_VERTICAL_ZL_CV:
        switch (struCnfp.nMatCopySetMode)
        {
        case MAT_COPY_ALL_ALONE:   //整机各不相同
            if(struCnfg.nMachineSpliceType  == MACHINE_SPLICE_TYPE_BIG)
            {
                for(i = 0; i < struCnfg.struLayerInfo[nLayerId].stuViewInfo[nViewId].nIdentifyGroupTotal; i++)
                {
                    materialParamsCopy(nLayerId, nViewId, struGsh.nMatGroup, nLayerId, nViewId, i, bIsAllArith, nArithSeq, bIsReserved); //一次 >> 二次、三次、四次...
                }
            }
            break;
        case MAT_COPY_REAR_ALIKE_FRONT:  // 前后视相同,将前视各次复制给后视各次
            switch(nViewId)
            {
            case LCD_VERI_MAIN_VIEW_FRONT: //前视/前主
                for(i = 0; i < struCnfg.struLayerInfo[nLayerId].stuViewInfo[nViewId].nIdentifyGroupTotal; i++)
                {
                    materialParamsCopy(nLayerId, LCD_VERI_MAIN_VIEW_FRONT, i, nLayerId, LCD_VERI_MAIN_VIEW_REAR, i, bIsAllArith, nArithSeq, bIsReserved); //前主 >> 后主
                }
                break;
            case LCD_VERI_VICE_VIEW_FRONT: //前辅
                for(i = 0; i < struCnfg.struLayerInfo[nLayerId].stuViewInfo[nViewId].nIdentifyGroupTotal; i++)
                {
                    materialParamsCopy(nLayerId, LCD_VERI_VICE_VIEW_FRONT, i, nLayerId, LCD_VERI_VICE_VIEW_REAR, i, bIsAllArith, nArithSeq, bIsReserved); //前辅 >> 后辅
                }
                break;
             default:
                break;
            }
            break;
        case MAT_COPY_SECOND_ALIKE_FIRST: // 一二三...次相同，将一次各视的值复制给二、三...次对应的视
            if(nGroupId == 0)
            {
                for(i = 1; i < struCnfg.struLayerInfo[nLayerId].stuViewInfo[nViewId].nIdentifyGroupTotal; i++)
                {
                    materialParamsCopy(nLayerId, nViewId, nGroupId, nLayerId, nViewId, i, bIsAllArith, nArithSeq, bIsReserved); //一次 >> 二次、三次、四次...
                }
            }
            break;
         case MAT_COPY_ALL_SAME:  //整机相同
            if((nViewId == LCD_VERI_MAIN_VIEW_FRONT)&&(nGroupId == 0))
            {
                for(i = 0; i < struCnfg.struLayerInfo[nLayerId].stuViewInfo[nViewId].nIdentifyGroupTotal; i++)
                {
                    materialParamsCopy(nLayerId, LCD_VERI_MAIN_VIEW_FRONT, 0, nLayerId, LCD_VERI_MAIN_VIEW_FRONT, i, bIsAllArith, nArithSeq, bIsReserved); //前主 >> 后主
                    materialParamsCopy(nLayerId, LCD_VERI_MAIN_VIEW_FRONT, 0, nLayerId, LCD_VERI_MAIN_VIEW_REAR, i, bIsAllArith, nArithSeq, bIsReserved); //前主 >> 后主
                }
            }
            else if((nViewId == LCD_VERI_VICE_VIEW_FRONT)&&(nGroupId == 0))
            {
                for(i = 0; i < struCnfg.struLayerInfo[nLayerId].stuViewInfo[nViewId].nIdentifyGroupTotal; i++)
                {
                    materialParamsCopy(nLayerId, LCD_VERI_VICE_VIEW_FRONT, 0, nLayerId, LCD_VERI_VICE_VIEW_FRONT, i, bIsAllArith, nArithSeq, bIsReserved); //前辅 >> 后辅
                    materialParamsCopy(nLayerId, LCD_VERI_VICE_VIEW_FRONT, 0, nLayerId, LCD_VERI_VICE_VIEW_REAR, i, bIsAllArith, nArithSeq, bIsReserved); //前辅 >> 后辅
                }
            }
            break;

//            if((nViewId == LCD_VERI_MAIN_VIEW_FRONT)&&(nGroupId == 0))
//            {
//                for(i = 0; i < struCnfg.struLayerInfo[nLayerId].stuViewInfo[nViewId].nIdentifyGroupTotal; i++)
//                {
//                    materialParamsCopy(nLayerId, LCD_VERI_MAIN_VIEW_FRONT, i, nLayerId, LCD_VERI_MAIN_VIEW_REAR, i, bIsAllArith, nArithSeq, bIsReserved); //前主 >> 后主
//                }
//            }
//            else if((nViewId == LCD_VERI_VICE_VIEW_FRONT)&&(nGroupId == 0))
//            {
//                for(i = 0; i < struCnfg.struLayerInfo[nLayerId].stuViewInfo[nViewId].nIdentifyGroupTotal; i++)
//                {
//                    materialParamsCopy(nLayerId, LCD_VERI_VICE_VIEW_FRONT, i, nLayerId, LCD_VERI_VICE_VIEW_REAR, i, bIsAllArith, nArithSeq, bIsReserved); //前辅 >> 后辅
//                }
//            }
//            break;
        }
        break;
    case MACHINE_LD_ZL_GP:  //单层履带机
        switch (struCnfp.nMatCopySetMode)
        {
        case MAT_COPY_ALL_ALONE:   //整机各不相同
            break;
        case MAT_COPY_REAR_ALIKE_FRONT:  // 前后视相同,将前视各次复制给后视各次
            switch(nViewId)
            {
            case LCD_LD_MAIN_VIEW_UP: //上视/上主
                for(i = 0; i < struCnfg.struLayerInfo[nLayerId].stuViewInfo[nViewId].nIdentifyGroupTotal; i++)
                {
                    materialParamsCopy(nLayerId, LCD_LD_MAIN_VIEW_UP, i, nLayerId, LCD_LD_MAIN_VIEW_LOW, i, bIsAllArith, nArithSeq, bIsReserved); //前主 >> 后主
                }
                break;
            case LCD_LD_VICE_VIEW_UP: //上辅
                for(i = 0; i < struCnfg.struLayerInfo[nLayerId].stuViewInfo[nViewId].nIdentifyGroupTotal; i++)
                {
                    materialParamsCopy(nLayerId, LCD_LD_VICE_VIEW_UP, i, nLayerId, LCD_LD_VICE_VIEW_LOW, i, bIsAllArith, nArithSeq, bIsReserved); //前辅 >> 后辅
                }
                break;
             default:
                break;
            }
            break;
        case MAT_COPY_SECOND_ALIKE_FIRST: // 暂无
            break;
         case MAT_COPY_ALL_SAME:  //整机相同
            if((nViewId == LCD_LD_MAIN_VIEW_UP)&&(nGroupId == 0))
            {
                for(i = 0; i < struCnfg.struLayerInfo[nLayerId].stuViewInfo[nViewId].nIdentifyGroupTotal; i++)
                {
                    //                        materialParamsCopy(nLayerId, LCD_LD_MAIN_VIEW_UP, LCD_LD_MAIN_VIEW_LOW, nLayerId, i, j, bIsAllArith, nArithSeq, bIsReserved); //一次前主 >> 各视各次
                    materialParamsCopy(nLayerId, LCD_LD_MAIN_VIEW_UP, 0, nLayerId, LCD_LD_MAIN_VIEW_UP, i, bIsAllArith, nArithSeq, bIsReserved); //前主 >> 后主
                    materialParamsCopy(nLayerId, LCD_LD_MAIN_VIEW_UP, 0, nLayerId, LCD_LD_MAIN_VIEW_LOW, i, bIsAllArith, nArithSeq, bIsReserved); //前主 >> 后主
                    materialParamsCopy(nLayerId, LCD_LD_MAIN_VIEW_UP, 0, nLayerId, LCD_LD_VICE_VIEW_UP, i, bIsAllArith, nArithSeq, bIsReserved); //前主 >> 后主
                    materialParamsCopy(nLayerId, LCD_LD_MAIN_VIEW_UP, 0, nLayerId, LCD_LD_VICE_VIEW_LOW, i, bIsAllArith, nArithSeq, bIsReserved); //前主 >> 后主
                }
            }
            break;
        }
        break;
    case MACHINE_LD2_ZL_GP: //双层履带机
        switch (struCnfp.nMatCopySetMode)
        {
        case MAT_COPY_ALL_ALONE:   //整机各不相同
            break;
        case MAT_COPY_REAR_ALIKE_FRONT:  // 前后视相同,将上层上视拷贝给上层下视,将下层上视拷贝给下层下视
            switch(nViewId)
            {
            case LCD_VERI_MAIN_VIEW_FRONT: //前视/前主
                for(i = 0; i < struCnfg.struLayerInfo[nLayerId].stuViewInfo[nViewId].nIdentifyGroupTotal; i++)
                {
                    materialParamsCopy(nLayerId, LCD_VERI_MAIN_VIEW_FRONT, i, nLayerId, LCD_VERI_MAIN_VIEW_REAR, i, bIsAllArith, nArithSeq, bIsReserved); //前主 >> 后主
                }
                break;
            case LCD_VERI_VICE_VIEW_FRONT: //前辅
                for(i = 0; i < struCnfg.struLayerInfo[nLayerId].stuViewInfo[nViewId].nIdentifyGroupTotal; i++)
                {
                    materialParamsCopy(nLayerId, LCD_VERI_VICE_VIEW_FRONT, i, nLayerId, LCD_VERI_VICE_VIEW_REAR, i, bIsAllArith, nArithSeq, bIsReserved); //前辅 >> 后辅
                }
                break;
            case LCD_VERI_MAIN_VIEW_FRONT_2: //前视/前主
                for(i = 0; i < struCnfg.struLayerInfo[nLayerId].stuViewInfo[nViewId].nIdentifyGroupTotal; i++)
                {
                    materialParamsCopy(nLayerId, LCD_VERI_MAIN_VIEW_FRONT_2, i, nLayerId, LCD_VERI_MAIN_VIEW_REAR_2, i, bIsAllArith, nArithSeq, bIsReserved); //前主 >> 后主
                }
                break;
            case LCD_VERI_VICE_VIEW_FRONT_2: //前辅
                for(i = 0; i < struCnfg.struLayerInfo[nLayerId].stuViewInfo[nViewId].nIdentifyGroupTotal; i++)
                {
                    materialParamsCopy(nLayerId, LCD_VERI_VICE_VIEW_FRONT_2, i, nLayerId, LCD_VERI_VICE_VIEW_REAR_2, i, bIsAllArith, nArithSeq, bIsReserved); //前辅 >> 后辅
                }
                break;
             default:
                break;
            }
            break;
        case MAT_COPY_SECOND_ALIKE_FIRST: // 暂无
            break;
         case MAT_COPY_ALL_SAME:  //整机相同
            if((nViewId == LCD_VERI_MAIN_VIEW_FRONT)&&(nGroupId == 0))
            {
                for(i = 0; i < struCnfg.struLayerInfo[nLayerId].stuViewInfo[nViewId].nIdentifyGroupTotal; i++)
                {
                    materialParamsCopy(nLayerId, LCD_VERI_MAIN_VIEW_FRONT, 0, nLayerId, LCD_VERI_MAIN_VIEW_FRONT, i, bIsAllArith, nArithSeq, bIsReserved); //前主 >> 后主
                    materialParamsCopy(nLayerId, LCD_VERI_MAIN_VIEW_FRONT, 0, nLayerId, LCD_VERI_MAIN_VIEW_REAR, i, bIsAllArith, nArithSeq, bIsReserved); //前主 >> 后主
                    materialParamsCopy(nLayerId, LCD_VERI_MAIN_VIEW_FRONT, 0, nLayerId, LCD_VERI_VICE_VIEW_FRONT, i, bIsAllArith, nArithSeq, bIsReserved); //前主 >> 后主
                    materialParamsCopy(nLayerId, LCD_VERI_MAIN_VIEW_FRONT, 0, nLayerId, LCD_VERI_VICE_VIEW_REAR, i, bIsAllArith, nArithSeq, bIsReserved); //前主 >> 后主
                    materialParamsCopy(nLayerId, LCD_VERI_MAIN_VIEW_FRONT_2, 0, nLayerId, LCD_VERI_MAIN_VIEW_FRONT_2, i, bIsAllArith, nArithSeq, bIsReserved); //前主 >> 后主
                    materialParamsCopy(nLayerId, LCD_VERI_MAIN_VIEW_FRONT_2, 0, nLayerId, LCD_VERI_MAIN_VIEW_REAR_2, i, bIsAllArith, nArithSeq, bIsReserved); //前主 >> 后主
                    materialParamsCopy(nLayerId, LCD_VERI_MAIN_VIEW_FRONT_2, 0, nLayerId, LCD_VERI_VICE_VIEW_FRONT_2, i, bIsAllArith, nArithSeq, bIsReserved); //前主 >> 后主
                    materialParamsCopy(nLayerId, LCD_VERI_MAIN_VIEW_FRONT_2, 0, nLayerId, LCD_VERI_VICE_VIEW_REAR_2, i, bIsAllArith, nArithSeq, bIsReserved); //前主 >> 后主

                }
            }
//            else if((nViewId == LCD_VERI_VICE_VIEW_FRONT)&&(nGroupId == 0))
//            {
//                for(i = 0; i < struCnfg.struLayerInfo[nLayerId].stuViewInfo[nViewId].nIdentifyGroupTotal; i++)
//                {
//                    materialParamsCopy(nLayerId, LCD_VERI_VICE_VIEW_FRONT, 0, nLayerId, LCD_VERI_VICE_VIEW_FRONT, i, bIsAllArith, nArithSeq, bIsReserved); //前辅 >> 后辅
//                    materialParamsCopy(nLayerId, LCD_VERI_VICE_VIEW_FRONT, 0, nLayerId, LCD_VERI_VICE_VIEW_REAR, i, bIsAllArith, nArithSeq, bIsReserved); //前辅 >> 后辅
//                }
//            }
            break;
        }
        break;
    default:
        break;
    }

    if(bNeedSend == true)
    {
        if(struCnfg.nMachineSpliceType  == MACHINE_SPLICE_TYPE_BIG)
        {
            for(i = 0; i < struCnfg.struLayerInfo[nLayerId].stuViewInfo[nViewId].nIdentifyGroupTotal; i++)
            {
                materialResetGroupAssemble(nLayerId, nViewId, i, bIsAllArith,  nArithSeq, bIsReserved);
            }
        }else{
            materialResetGroupAssemble(nLayerId, nViewId, nGroupId, bIsAllArith,  nArithSeq, bIsReserved);
        }
    }
}

