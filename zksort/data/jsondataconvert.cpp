#include "jsondataconvert.h"
#include <QDebug>

JsonDataConvert::JsonDataConvert()
{
}

bool JsonDataConvert::JsonToCnfGlobal(const Json::Value &jsonVal, struCnfGlobal &CnfGlobal)
{
    INT_ARRAY_KEY outArray;
    vector<int> intVec;

    CMyJson::ReadIntKey(jsonVal,"nLang",(int&)CnfGlobal.nLang);
    CMyJson::ReadIntKey(jsonVal,"nCounter",(int&)CnfGlobal.nCounter);
    CMyJson::ReadIntKey(jsonVal,"nProfile",CnfGlobal.nProfile);
    CMyJson::ReadIntKey(jsonVal,"nProfileTotal",CnfGlobal.nProfileTotal);

    CMyJson::ReadIntKey(jsonVal,"nInterfaceBoardMode",CnfGlobal.nInterfaceBoardMode);
    CMyJson::ReadIntKey(jsonVal,"nCtrlBoardMode",CnfGlobal.nCtrlBoardMode);
    CMyJson::ReadIntKey(jsonVal,"nLampBoardMode",CnfGlobal.nLampBoardMode);

    JsonToProfileindex(MAX_PROFILE,jsonVal["stuProfileIndex"],CnfGlobal.stuProfileIndex);

    CMyJson::ReadIntKey(jsonVal,"nMachineAttr",CnfGlobal.nMachineAttr);
    CMyJson::ReadIntKey(jsonVal,"nMachineType",(int&)CnfGlobal.nMachineType);
    CMyJson::ReadIntKey(jsonVal,"nLampBoardQuoType",(int&)CnfGlobal.nLampBoardQuoType);
    CMyJson::ReadIntKey(jsonVal,"nMachineSerial",(int&)CnfGlobal.nMachineSerial);
    CMyJson::ReadIntKey(jsonVal,"nLayerTotal",(int&)CnfGlobal.nLayerTotal);
    JsonToLayerInfo(MAX_LAYER,jsonVal["struLayerInfo"],CnfGlobal.struLayerInfo);
    CMyJson::ReadIntKey(jsonVal,"nMachineSpliceType",CnfGlobal.nMachineSpliceType);
    CMyJson::ReadIntKey(jsonVal,"nInterfaceBoardTotal",CnfGlobal.nInterfaceBoardTotal);
    CMyJson::ReadIntKey(jsonVal,"nChuteTotal",CnfGlobal.nChuteTotal);
    CMyJson::ReadIntKey(jsonVal,"nCtrlBoardTotal",CnfGlobal.nCtrlBoardTotal);
    CMyJson::ReadIntKey(jsonVal,"nCtrlBoardType",CnfGlobal.nCtrlBoardType);
    CMyJson::ReadIntKey(jsonVal,"nFeedMainTotal",(int&)CnfGlobal.nFeedMainTotal);
    CMyJson::ReadIntKey(jsonVal,"nFeedViceTotal",(int&)CnfGlobal.nFeedViceTotal);
    CMyJson::ReadIntKey(jsonVal,"nLampBoardType",CnfGlobal.nLampBoardType);
    CMyJson::ReadIntKey(jsonVal,"nWipeEnable",CnfGlobal.nWipeEnable);
    CMyJson::ReadIntKey(jsonVal,"nControlAlarmSet",CnfGlobal.nControlAlarmSet);
    CMyJson::ReadIntKey(jsonVal,"nVavleTimeSetMode",CnfGlobal.nVavleTimeSetMode);
    CMyJson::ReadIntKey(jsonVal,"nEnableDispArithSetByTimes",CnfGlobal.nEnableDispArithSetByTimes);
    CMyJson::ReadIntKey(jsonVal,"nFeedControlType1",CnfGlobal.nFeedControlType1);
    CMyJson::ReadIntKey(jsonVal,"nHasUps",CnfGlobal.nHasUps);
    CMyJson::ReadIntKey(jsonVal,"nSpray",CnfGlobal.nSpray);
    CMyJson::ReadIntKey(jsonVal,"nWheelRunType",CnfGlobal.nWheelRunType);
    CMyJson::ReadIntKey(jsonVal,"nVibType",CnfGlobal.nVibType);
    CMyJson::ReadIntKey(jsonVal,"nVibFreq",CnfGlobal.nVibFreq);
    CMyJson::ReadIntKey(jsonVal,"bigVibIsUse",CnfGlobal.bigVibIsUse);
    CMyJson::ReadIntKey(jsonVal,"nBigVibPos",CnfGlobal.nBigVibPos);
    CMyJson::ReadIntKey(jsonVal,"nVibFreq_2",CnfGlobal.nVibFreq_2);
    CMyJson::ReadIntKey(jsonVal,"bCrossCam",CnfGlobal.bCrossCam);
    CMyJson::ReadIntKey(jsonVal,"bCrossMode1",CnfGlobal.bCrossMode1);
    CMyJson::ReadIntKey(jsonVal,"bCrossMode2",CnfGlobal.bCrossMode2);

    CMyJson::ReadIntKey(jsonVal,"nBigSmallArithMaxValue",CnfGlobal.nBigSmallArithMaxValue);
    CMyJson::ReadIntKey(jsonVal,"nEdgeCutMaxValue",CnfGlobal.nEdgeCutMaxValue);
    CMyJson::ReadIntKey(jsonVal,"nImpuredMaxValue",CnfGlobal.nImpuredMaxValue);
    CMyJson::ReadFloatKey(jsonVal,"nEjectDurationMaxValue",CnfGlobal.nEjectDurationMaxValue);
    CMyJson::ReadIntKey(jsonVal,"ntotalAlarmCount",CnfGlobal.ntotalAlarmCount);
    CMyJson::ReadIntKey(jsonVal,"ntotalFeedTime",CnfGlobal.ntotalFeedTime);
    CMyJson::ReadIntKey(jsonVal,"ntotalWipeCount",CnfGlobal.ntotalWipeCount);
    CMyJson::ReadIntKey(jsonVal,"nTickDirect",CnfGlobal.nTickDirect);
    CMyJson::ReadIntKey(jsonVal,"nChuteSpliceMode",CnfGlobal.nChuteSpliceMode);
    CMyJson::ReadIntKey(jsonVal,"nChuteSpliceAddr",CnfGlobal.nChuteSpliceAddr);
    CMyJson::ReadIntKey(jsonVal,"sTotalStaticsGetDuration",CnfGlobal.sTotalStaticsGetDuration);
    CMyJson::ReadIntKey(jsonVal,"nCmdSendTimeout",CnfGlobal.nCmdSendTimeout);
    CMyJson::ReadIntKey(jsonVal,"nUGSendTimeout",CnfGlobal.nUGSendTimeout);
    CMyJson::ReadIntKey(jsonVal,"nUgSendInterval",CnfGlobal.nUgSendInterval);
    CMyJson::ReadIntKey(jsonVal,"nWaitTimeout",CnfGlobal.nWaitTimeout);
    CMyJson::ReadIntKey(jsonVal,"nAutoStaticsEnable",CnfGlobal.nAutoStaticsEnable);
    CMyJson::ReadIntKey(jsonVal,"nAutoStaticsWorking",CnfGlobal.nAutoStaticsWorking);
    CMyJson::ReadIntKey(jsonVal,"nAutoImageCaptureEnable",CnfGlobal.nAutoImageCaptureEnable);
    CMyJson::ReadFloatKey(jsonVal,"n_coff_user",CnfGlobal.n_coff_user);
    CMyJson::ReadIntKey(jsonVal,"nOnoffDelay",CnfGlobal.nOnoffDelay);
    CMyJson::ReadIntKey(jsonVal,"nOnoffDelay_2",CnfGlobal.nOnoffDelay_2);
    CMyJson::ReadIntKey(jsonVal,"nValveSite",CnfGlobal.nValveSite);
    CMyJson::ReadIntKey(jsonVal,"nValveType",CnfGlobal.nValveType);
    CMyJson::ReadIntKey(jsonVal,"nValveRatio",CnfGlobal.nValveRatio);
    CMyJson::ReadIntKey(jsonVal,"nCorssEjector",CnfGlobal.nCorssEjector);
    CMyJson::ReadIntKey(jsonVal,"bIpcCrossCam",CnfGlobal.bIpcCrossCam);
    CMyJson::ReadIntKey(jsonVal,"nIpcCount",CnfGlobal.nIpcCount);
    CMyJson::ReadIntKey(jsonVal,"nIpcCamera",CnfGlobal.nIpcCamera);
    CMyJson::ReadIntKey(jsonVal,"nIpcModelCount",CnfGlobal.nIpcModelCount);
    CMyJson::ReadIntKey(jsonVal,"nUseIpcEnable",CnfGlobal.nUseIpcEnable);
    CMyJson::ReadIntKey(jsonVal,"n_svmType",CnfGlobal.n_svmType);
    CMyJson::ReadIntKey(jsonVal,"nLVMachine",CnfGlobal.nLVMachine);
    CMyJson::ReadIntKey(jsonVal,"nPlcControl",CnfGlobal.nPlcControl);
    CMyJson::ReadIntKey(jsonVal,"nDPMachine",CnfGlobal.nDPMachine);
    CMyJson::ReadIntKey(jsonVal,"nUseBMModule",CnfGlobal.nUseBMModule);
    CMyJson::ReadIntKey(jsonVal,"nUseLowcfgSync",CnfGlobal.nUseLowcfgSync);
    CMyJson::ReadIntKey(jsonVal,"nIpcIpAddrBase",CnfGlobal.nIpcIpAddrBase);
    CMyJson::ReadIntKey(jsonVal,"nIpcBadMatStaticsCount",CnfGlobal.nIpcBadMatStaticsCount);
    CMyJson::ReadIntKey(jsonVal,"nIpcAlarmDealMode",CnfGlobal.nIpcAlarmDealMode);
    CMyJson::ReadIntKey(jsonVal,"nIpcAlarmTurnOff",CnfGlobal.nIpcAlarmTurnOff);
    CMyJson::ReadIntKey(jsonVal,"nIpcCameraClassSameMode",CnfGlobal.nIpcCameraClassSameMode);
    CMyJson::ReadIntKey(jsonVal,"nAllInOne",CnfGlobal.nAllInOne);
    CMyJson::ReadIntKey(jsonVal,"nValveDefend",CnfGlobal.nValveDefend);
    CMyJson::ReadIntKey(jsonVal,"nValveDefendTime",CnfGlobal.nValveDefendTime);
    CMyJson::ReadIntKey(jsonVal,"nValveUndefendTime",CnfGlobal.nValveUndefendTime);
    CMyJson::ReadIntKey(jsonVal,"nScreenSet",CnfGlobal.nScreenSet);
    // CMyJson::ReadIntKey(jsonVal,"nIsCameraConpress",CnfGlobal.nIsCameraConpress);
    CMyJson::ReadIntKey(jsonVal,"camParaInCnf",CnfGlobal.camParaInCnf);
    CMyJson::ReadIntKey(jsonVal,"bUseAutoFeeder",CnfGlobal.bUseAutoFeeder);
    CMyJson::ReadIntKey(jsonVal,"alarmCmdType",CnfGlobal.alarmCmdType);
    CMyJson::ReadIntKey(jsonVal,"accChecked",CnfGlobal.accChecked);
    CMyJson::ReadIntKey(jsonVal,"imgSaturationFactor",CnfGlobal.imgSaturationFactor);
    CMyJson::ReadIntKey(jsonVal,"imgSaturationEnable",CnfGlobal.imgSaturationEnable);
    
    CMyJson::ReadIntKey(jsonVal,"nPowerCut",CnfGlobal.nPowerCut);
    CMyJson::ReadIntKey(jsonVal,"nDefaultPlcCtrl",CnfGlobal.nDefaultPlcCtrl);
    //CMyJson::ReadIntKey(jsonVal,"nPlcCtrl",CnfGlobal.nPlcCtrl);
    CMyJson::ReadIntKey(jsonVal,"bautoplcctrlEnable",CnfGlobal.bautoplcctrlEnable);
    CMyJson::ReadIntKey(jsonVal,"blampctrlEnable",CnfGlobal.blampctrlEnable);
    //CMyJson::ReadIntKey(jsonVal,"imageEnahnce",CnfGlobal.imageEnahnce);
    CMyJson::ReadIntKey(jsonVal,"bLogoLightNum",CnfGlobal.bLogoLightNum);
    CMyJson::ReadIntKey(jsonVal,"overRatio",CnfGlobal.overRatio);
    CMyJson::ReadIntKey(jsonVal,"areaThres",CnfGlobal.areaThres);
    CMyJson::ReadIntKey(jsonVal,"bMainpageShow",CnfGlobal.bMainpageShow);

    CMyJson::ReadIntKey(jsonVal,"trigArea",CnfGlobal.trigArea);
    CMyJson::ReadIntKey(jsonVal,"cutHeight",CnfGlobal.cutHeight);
    CMyJson::ReadIntKey(jsonVal,"imgDim",CnfGlobal.imgDim);
    CMyJson::ReadIntKey(jsonVal,"autoTargetImage",CnfGlobal.autoTargetImage);
    //红外部分
    CMyJson::ReadIntKey(jsonVal,"nRealTimeCorrection",CnfGlobal.nRealTimeCorrection);

    intVec.clear();
    intVec.push_back(MAX_LAMP_PER_BOARD);
    CMyJson::ReadIntArrayKey(jsonVal,"bLogoLightMode",intVec,CnfGlobal.bLogoLightMode,outArray);
    intVec.clear();
    intVec.push_back(MAX_LAMP_PER_BOARD);
    CMyJson::ReadIntArrayKey(jsonVal,"bLogoLightLength",intVec,CnfGlobal.bLogoLightLength,outArray);


    intVec.clear();
    intVec.push_back(MAX_ALARM);
    CMyJson::ReadIntArrayKey(jsonVal,"nAlarmValid",intVec,CnfGlobal.nAlarmValid,outArray);


    //CMyJson::ReadIntArrayKey(jsonVal,"nCameraCompressMode",intVec,(int*)CnfGlobal.nCameraCompressMode,outArray);

    intVec.clear();
    intVec.push_back(MAX_LAYER);
    intVec.push_back(MAX_VAVLE);
    CMyJson::ReadIntArrayKey(jsonVal,"nVavleBoardMode",intVec,(int*)CnfGlobal.nVavleBoardMode,outArray);

    intVec.clear();
    intVec.push_back(12);
    intVec.push_back(80);
    CMyJson::ReadIntArrayKey(jsonVal,"totalblowcount",intVec,(int*)CnfGlobal.totalblowcount,outArray);

    JsonToStuAlarmCfg(jsonVal["struAlarmConfig"],CnfGlobal.struAlarmConfig);
    JsonToStuLampCfg(jsonVal["stuLayerLampCfg"],CnfGlobal.stuLayerLampCfg);
    JsonToStuIntelWipeCfg(jsonVal["struIntelWipe"],CnfGlobal.struIntelWipe);
    JSonToStuUseLimitCfg(jsonVal["stuUseLimitCfg"],CnfGlobal.stuUseLimitCfg);
    JSonToStuFeedAutoControl(jsonVal["stuFeedAutoCtrl"],CnfGlobal.stuFeedAutoCtrl);
    JsonToStuModelSelectCfg(jsonVal["stumodeLableSelect"],CnfGlobal.stumodeLableSelect);

    for(int i = 0;i< MAX_LAYER;i++){
        for(int j = 0;j < MAX_VIEW;j++){
            CMyJson::ReadIntKey(jsonVal["nStaticsConfig"][i][j],"nStaticsTotal",CnfGlobal.nStaticsConfig[i][j].nStaticsTotal);
            intVec.clear();
            intVec.push_back(MAX_STATICS_TYPE);
            CMyJson::ReadIntArrayKey(jsonVal["nStaticsConfig"][i][j],"nIndex",intVec,(int*)CnfGlobal.nStaticsConfig[i][j].nIndex,outArray);
            CMyJson::ReadIntArrayKey(jsonVal["nStaticsConfig"][i][j],"nBytes",intVec,(int*)CnfGlobal.nStaticsConfig[i][j].nBytes,outArray);
        }
    }

    CMyJson::ReadIntKey(jsonVal["struIntelWipe"],"feederOnffCount",CnfGlobal.struIntelWipe.feederOnffCount);
    CMyJson::ReadIntKey(jsonVal["struIntelWipe"],"wiperDurationPerChute",CnfGlobal.struIntelWipe.wiperDurationPerChute);
    CMyJson::ReadIntKey(jsonVal["struIntelWipe"],"wiperWidth",CnfGlobal.struIntelWipe.wiperWidth);
    CMyJson::ReadIntKey(jsonVal["struIntelWipe"],"wipeTimeout",CnfGlobal.struIntelWipe.wipeTimeout);
    CMyJson::ReadIntKey(jsonVal["wiperDefaultPosi"],"wipeTimeout",CnfGlobal.struIntelWipe.wiperDefaultPosi);
    CMyJson::ReadIntKey(jsonVal["struIntelWipe"],"wiperConfirmThread",CnfGlobal.struIntelWipe.wiperConfirmThread);

    intVec.clear();
    intVec.push_back(MAX_LAYER);
    intVec.push_back(MAX_VIEW);
    intVec.push_back(MAX_GROUP_IDENTIFY);
    CMyJson::ReadIntArrayKey(jsonVal,"sStaticsGetGroupEnable",intVec,(int*)CnfGlobal.sStaticsGetGroupEnable,outArray);

    for(int i = 0;i< MAX_LAYER;i++){
        for(int j = 0;j < MAX_VIEW;j++){
            for(int k = 0; k < MAX_VIEW_UNIT; k++)
            {
                CMyJson::ReadIntKey(jsonVal["struIpcCameraParams"][i][j][k],"ipcType",CnfGlobal.struIpcCameraParams[i][j][k].ipcType);
                CMyJson::ReadIntKey(jsonVal["struIpcCameraParams"][i][j][k],"cameraId",CnfGlobal.struIpcCameraParams[i][j][k].cameraId);
                CMyJson::ReadIntKey(jsonVal["struIpcCameraParams"][i][j][k],"nHoriPixel",CnfGlobal.struIpcCameraParams[i][j][k].nHoriPixel);
                CMyJson::ReadIntKey(jsonVal["struIpcCameraParams"][i][j][k],"nVertPixel",CnfGlobal.struIpcCameraParams[i][j][k].nVertPixel);
                CMyJson::ReadIntKey(jsonVal["struIpcCameraParams"][i][j][k],"nHoriOffset",CnfGlobal.struIpcCameraParams[i][j][k].nHoriOffset);
                CMyJson::ReadIntKey(jsonVal["struIpcCameraParams"][i][j][k],"nHoriWidth",CnfGlobal.struIpcCameraParams[i][j][k].nHoriWidth);
                CMyJson::ReadIntKey(jsonVal["struIpcCameraParams"][i][j][k],"nHoriWidth_2",CnfGlobal.struIpcCameraParams[i][j][k].nHoriWidth_2);
                CMyJson::ReadIntKey(jsonVal["struIpcCameraParams"][i][j][k],"nVertOffset",CnfGlobal.struIpcCameraParams[i][j][k].nVertOffset);
                CMyJson::ReadIntKey(jsonVal["struIpcCameraParams"][i][j][k],"nVertWidth",CnfGlobal.struIpcCameraParams[i][j][k].nVertWidth);
                CMyJson::ReadIntKey(jsonVal["struIpcCameraParams"][i][j][k],"nSaveImgEnable",CnfGlobal.struIpcCameraParams[i][j][k].nSaveImgEnable);
                intVec.clear();
                intVec.push_back(4);
                CMyJson::ReadIntArrayKey(jsonVal["struIpcCameraParams"][i][j][k],"nIpAddr",intVec,(int*)CnfGlobal.struIpcCameraParams[i][j][k].nIpAddr,outArray);
            }
        }
    }
    for(int i = 0;i< MAX_VIEW;i++){
        for(int j= 0; j< MAX_VIEW_UNIT;j++){
            CMyJson::ReadIntKey(jsonVal["stuIpcConfigFileInfo"][i][j],"cameraCount",CnfGlobal.stuIpcConfigFileInfo[i][j].cameraCount);
            CMyJson::ReadIntKey(jsonVal["stuIpcConfigFileInfo"][i][j],"batchSize",CnfGlobal.stuIpcConfigFileInfo[i][j].batchSize);
            CMyJson::ReadIntKey(jsonVal["stuIpcConfigFileInfo"][i][j],"roiHeight",CnfGlobal.stuIpcConfigFileInfo[i][j].roiHeight);
            CMyJson::ReadIntKey(jsonVal["stuIpcConfigFileInfo"][i][j],"roiRepeat",CnfGlobal.stuIpcConfigFileInfo[i][j].roiRepeat);
        }
    }

    intVec.clear();
    intVec.push_back(MAX_VIEW);
    intVec.push_back(MAX_VIEW_UNIT);
    CMyJson::ReadIntArrayKey(jsonVal,"nIpcStartPixel",intVec,(int*)CnfGlobal.nIpcStartPixel,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nIpcEndPixel",intVec,(int*)CnfGlobal.nIpcEndPixel,outArray);

    intVec.clear();
    intVec.push_back(MAX_VIEW);
    CMyJson::ReadIntArrayKey(jsonVal,"nXsystemBoard",intVec,(int*)CnfGlobal.nXsystemBoard,outArray);

    intVec.clear();
    intVec.push_back(MAX_ACCELERATION);
    CMyJson::ReadIntArrayKey(jsonVal,"accelerationEnable",intVec,(int*)CnfGlobal.accelerationEnable,outArray);
    CMyJson::ReadIntKey(jsonVal,"accelerationTolerance",CnfGlobal.accelerationTolerance);
    CMyJson::ReadIntKey(jsonVal,"accelerationNum",CnfGlobal.accelerationNum);

    CMyJson::ReadIntArrayKey(jsonVal,"accelerationEnable2",intVec,(int*)CnfGlobal.accelerationEnable2,outArray);
    CMyJson::ReadIntKey(jsonVal,"accelerationTolerance2",CnfGlobal.accelerationTolerance2);
    CMyJson::ReadIntKey(jsonVal,"accelerationNum2",CnfGlobal.accelerationNum2);
    CMyJson::ReadIntKey(jsonVal,"viBoardTotalCount",CnfGlobal.viBoardTotalCount);
    CMyJson::ReadIntKey(jsonVal,"viBoardAlarmEnable",CnfGlobal.viBoardAlarmEnable);

    intVec.clear();
    intVec.push_back(MAX_VI_BOARD);
    intVec.push_back(MAX_VI_BOARD_INFO);
    CMyJson::ReadIntArrayKey(jsonVal,"viBoardInfo",intVec,(int*)CnfGlobal.viBoardInfo,outArray);

    intVec.clear();
    intVec.push_back(MAX_VIEW);
    intVec.push_back(MAX_CALIBRATE_SETS);
    CMyJson::ReadIntArrayKey(jsonVal,"nSenseBrightSelection_calb",intVec,(int*)CnfGlobal.nSenseBrightSelection_calb,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nSensorRowFre_calb",intVec,(int*)CnfGlobal.nSensorRowFre_calb,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"bCaliTDmode_calb",intVec,(int*)CnfGlobal.bCaliTDmode_calb,outArray);
    intVec.clear();
    intVec.push_back(MAX_CALIBRATE_SETS);
    CMyJson::ReadIntArrayKey(jsonVal,"bCaliGainEnable_calb",intVec,(int*)CnfGlobal.bCaliGainEnable_calb,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"bCaliBiasEnable_calb",intVec,(int*)CnfGlobal.bCaliBiasEnable_calb,outArray);


    return true;
}

void JsonDataConvert::CnfGlobalToJson(const struCnfGlobal &cnfGlobal, Json::Value &jsonVal)
{
    vector<int> intVec;

    CMyJson::WriteIntKey("nLang",cnfGlobal.nLang,jsonVal);
    CMyJson::WriteIntKey("nCounter",cnfGlobal.nCounter,jsonVal);
    CMyJson::WriteIntKey("nProfile",cnfGlobal.nProfile,jsonVal);
    CMyJson::WriteIntKey("nProfileTotal",cnfGlobal.nProfileTotal,jsonVal);

    CMyJson::WriteIntKey("nInterfaceBoardMode",cnfGlobal.nInterfaceBoardMode,jsonVal);
    CMyJson::WriteIntKey("nCtrlBoardMode",cnfGlobal.nCtrlBoardMode,jsonVal);
    CMyJson::WriteIntKey("nLampBoardMode",cnfGlobal.nLampBoardMode,jsonVal);

    profileIndexToJson(MAX_PROFILE,cnfGlobal.stuProfileIndex,jsonVal["stuProfileIndex"]);

    CMyJson::WriteIntKey("nMachineAttr",cnfGlobal.nMachineAttr,jsonVal);
    CMyJson::WriteIntKey("nMachineType",cnfGlobal.nMachineType,jsonVal);
    CMyJson::WriteIntKey("nLampBoardQuoType",cnfGlobal.nLampBoardQuoType,jsonVal);
    CMyJson::WriteIntKey("nMachineSerial",cnfGlobal.nMachineSerial,jsonVal);
    CMyJson::WriteIntKey("nLayerTotal",cnfGlobal.nLayerTotal,jsonVal);
    layerInfoToJson(MAX_LAYER,cnfGlobal.struLayerInfo,jsonVal["struLayerInfo"]);
    CMyJson::WriteIntKey("nMachineSpliceType",cnfGlobal.nMachineSpliceType,jsonVal);
    CMyJson::WriteIntKey("nInterfaceBoardTotal",cnfGlobal.nInterfaceBoardTotal,jsonVal);
    CMyJson::WriteIntKey("nChuteTotal",cnfGlobal.nChuteTotal,jsonVal);
    CMyJson::WriteIntKey("nCtrlBoardTotal",cnfGlobal.nCtrlBoardTotal,jsonVal);
    CMyJson::WriteIntKey("nCtrlBoardType",cnfGlobal.nCtrlBoardType,jsonVal);
    CMyJson::WriteIntKey("nFeedMainTotal",cnfGlobal.nFeedMainTotal,jsonVal);
    CMyJson::WriteIntKey("nFeedViceTotal",cnfGlobal.nFeedViceTotal,jsonVal);
    CMyJson::WriteIntKey("nLampBoardType",cnfGlobal.nLampBoardType,jsonVal);
    CMyJson::WriteIntKey("nWipeEnable",cnfGlobal.nWipeEnable,jsonVal);
    CMyJson::WriteIntKey("nControlAlarmSet",cnfGlobal.nControlAlarmSet,jsonVal);
    CMyJson::WriteIntKey("nVavleTimeSetMode",cnfGlobal.nVavleTimeSetMode,jsonVal);
    CMyJson::WriteIntKey("nEnableDispArithSetByTimes",cnfGlobal.nEnableDispArithSetByTimes,jsonVal);
    CMyJson::WriteIntKey("nFeedControlType1",cnfGlobal.nFeedControlType1,jsonVal);
    CMyJson::WriteIntKey("nHasUps",cnfGlobal.nHasUps,jsonVal);
    CMyJson::WriteIntKey("nSpray",cnfGlobal.nSpray,jsonVal);
    CMyJson::WriteIntKey("nWheelRunType",cnfGlobal.nWheelRunType,jsonVal);
    CMyJson::WriteIntKey("nVibType",cnfGlobal.nVibType,jsonVal);
    CMyJson::WriteIntKey("nVibFreq",cnfGlobal.nVibFreq,jsonVal);
    CMyJson::WriteIntKey("bigVibIsUse",cnfGlobal.bigVibIsUse,jsonVal);
    CMyJson::WriteIntKey("nBigVibPos",cnfGlobal.nBigVibPos,jsonVal);
    CMyJson::WriteIntKey("nVibFreq_2",cnfGlobal.nVibFreq_2,jsonVal);
    CMyJson::WriteIntKey("bCrossCam",cnfGlobal.bCrossCam,jsonVal);
    CMyJson::WriteIntKey("bCrossMode1",cnfGlobal.bCrossMode1,jsonVal);
    CMyJson::WriteIntKey("bCrossMode2",cnfGlobal.bCrossMode2,jsonVal);

    CMyJson::WriteIntKey("ntotalAlarmCount",cnfGlobal.ntotalAlarmCount,jsonVal);
    CMyJson::WriteIntKey("ntotalWipeCount",cnfGlobal.ntotalWipeCount,jsonVal);
    CMyJson::WriteIntKey("ntotalFeedTime",cnfGlobal.ntotalFeedTime,jsonVal);
    CMyJson::WriteIntKey("nBigSmallArithMaxValue",cnfGlobal.nBigSmallArithMaxValue,jsonVal);
    CMyJson::WriteIntKey("nEdgeCutMaxValue",cnfGlobal.nEdgeCutMaxValue,jsonVal);
    CMyJson::WriteIntKey("nImpuredMaxValue",cnfGlobal.nImpuredMaxValue,jsonVal);
    CMyJson::WriteFloatKey("nEjectDurationMaxValue",cnfGlobal.nEjectDurationMaxValue,jsonVal);
    CMyJson::WriteIntKey("nTickDirect",cnfGlobal.nTickDirect,jsonVal);
    CMyJson::WriteIntKey("nChuteSpliceMode",cnfGlobal.nChuteSpliceMode,jsonVal);
    CMyJson::WriteIntKey("nChuteSpliceAddr",cnfGlobal.nChuteSpliceAddr,jsonVal);
    CMyJson::WriteIntKey("sTotalStaticsGetDuration",cnfGlobal.sTotalStaticsGetDuration,jsonVal);
    CMyJson::WriteIntKey("nCmdSendTimeout",cnfGlobal.nCmdSendTimeout,jsonVal);
    CMyJson::WriteIntKey("nUGSendTimeout",cnfGlobal.nUGSendTimeout,jsonVal);
    CMyJson::WriteIntKey("nUgSendInterval",cnfGlobal.nUgSendInterval,jsonVal);
    CMyJson::WriteIntKey("nWaitTimeout",cnfGlobal.nWaitTimeout,jsonVal);
    CMyJson::WriteIntKey("nAutoStaticsEnable",cnfGlobal.nAutoStaticsEnable,jsonVal);
    CMyJson::WriteIntKey("nAutoImageCaptureEnable",cnfGlobal.nAutoImageCaptureEnable,jsonVal);
    CMyJson::WriteIntKey("nAutoStaticsWorking",cnfGlobal.nAutoStaticsWorking,jsonVal);
    CMyJson::WriteFloatKey("n_coff_user",cnfGlobal.n_coff_user,jsonVal);
    CMyJson::WriteIntKey("nOnoffDelay",cnfGlobal.nOnoffDelay,jsonVal);
    CMyJson::WriteIntKey("nOnoffDelay_2",cnfGlobal.nOnoffDelay_2,jsonVal);
    CMyJson::WriteIntKey("nValveSite",cnfGlobal.nValveSite,jsonVal);
    CMyJson::WriteIntKey("nValveType",cnfGlobal.nValveType,jsonVal);
    CMyJson::WriteIntKey("nValveRatio",cnfGlobal.nValveRatio,jsonVal);
    CMyJson::WriteIntKey("nCorssEjector",cnfGlobal.nCorssEjector,jsonVal);
    CMyJson::WriteIntKey("bIpcCrossCam",cnfGlobal.bIpcCrossCam,jsonVal);
    CMyJson::WriteIntKey("nIpcCount",cnfGlobal.nIpcCount,jsonVal);
    CMyJson::WriteIntKey("nIpcCamera",cnfGlobal.nIpcCamera,jsonVal);
    CMyJson::WriteIntKey("nIpcModelCount",cnfGlobal.nIpcModelCount,jsonVal);
    CMyJson::WriteIntKey("nUseIpcEnable",cnfGlobal.nUseIpcEnable,jsonVal);
    CMyJson::WriteIntKey("n_svmType",cnfGlobal.n_svmType,jsonVal);
    CMyJson::WriteIntKey("nLVMachine",cnfGlobal.nLVMachine,jsonVal);
    CMyJson::WriteIntKey("nPlcControl",cnfGlobal.nPlcControl,jsonVal);
    CMyJson::WriteIntKey("nDPMachine",cnfGlobal.nDPMachine,jsonVal);
    CMyJson::WriteIntKey("nUseBMModule",cnfGlobal.nUseBMModule,jsonVal);
    CMyJson::WriteIntKey("nUseLowcfgSync",cnfGlobal.nUseLowcfgSync,jsonVal);
    CMyJson::WriteIntKey("nIpcIpAddrBase",cnfGlobal.nIpcIpAddrBase,jsonVal);
    CMyJson::WriteIntKey("nIpcBadMatStaticsCount",cnfGlobal.nIpcBadMatStaticsCount,jsonVal);
    CMyJson::WriteIntKey("nIpcAlarmDealMode",cnfGlobal.nIpcAlarmDealMode,jsonVal);
    CMyJson::WriteIntKey("nIpcAlarmTurnOff",cnfGlobal.nIpcAlarmTurnOff,jsonVal);
    CMyJson::WriteIntKey("nIpcCameraClassSameMode",cnfGlobal.nIpcCameraClassSameMode,jsonVal);
    CMyJson::WriteIntKey("nAllInOne",cnfGlobal.nAllInOne,jsonVal);
    CMyJson::WriteIntKey("nValveDefend",cnfGlobal.nValveDefend,jsonVal);
    CMyJson::WriteIntKey("nValveDefendTime",cnfGlobal.nValveDefendTime,jsonVal);
    CMyJson::WriteIntKey("nValveUndefendTime",cnfGlobal.nValveUndefendTime,jsonVal);
    CMyJson::WriteIntKey("nScreenSet",cnfGlobal.nScreenSet,jsonVal);
    //CMyJson::WriteIntKey("nIsCameraConpress",cnfGlobal.nIsCameraConpress,jsonVal);
    CMyJson::WriteIntKey("camParaInCnf",cnfGlobal.camParaInCnf,jsonVal);
    CMyJson::WriteIntKey("bUseAutoFeeder",cnfGlobal.bUseAutoFeeder,jsonVal);
    CMyJson::WriteIntKey("alarmCmdType",cnfGlobal.alarmCmdType,jsonVal);
    CMyJson::WriteIntKey("accChecked",cnfGlobal.accChecked,jsonVal);
    CMyJson::WriteIntKey("imgSaturationFactor",cnfGlobal.imgSaturationFactor,jsonVal);
    CMyJson::WriteIntKey("imgSaturationEnable",cnfGlobal.imgSaturationEnable,jsonVal);

    CMyJson::WriteIntKey("nPowerCut",cnfGlobal.nPowerCut,jsonVal);
    CMyJson::WriteIntKey("nDefaultPlcCtrl",cnfGlobal.nDefaultPlcCtrl,jsonVal);
   // CMyJson::WriteIntKey("nPlcCtrl",cnfGlobal.nPlcCtrl,jsonVal);
    CMyJson::WriteIntKey("bautoplcctrlEnable",cnfGlobal.bautoplcctrlEnable,jsonVal);
    CMyJson::WriteIntKey("blampctrlEnable",cnfGlobal.blampctrlEnable,jsonVal);
    //CMyJson::WriteIntKey("imageEnahnce",cnfGlobal.imageEnahnce,jsonVal);
    CMyJson::WriteIntKey("overRatio",cnfGlobal.overRatio,jsonVal);
    CMyJson::WriteIntKey("areaThres",cnfGlobal.areaThres,jsonVal);
    CMyJson::WriteIntKey("bMainpageShow",cnfGlobal.bMainpageShow,jsonVal);
    // 红外部分
    CMyJson::WriteIntKey("nRealTimeCorrection",cnfGlobal.nRealTimeCorrection,jsonVal);
    CMyJson::WriteIntKey("bLogoLightNum",cnfGlobal.bLogoLightNum,jsonVal);


    CMyJson::WriteIntKey("trigArea",cnfGlobal.trigArea,jsonVal);
    CMyJson::WriteIntKey("cutHeight",cnfGlobal.cutHeight,jsonVal);
    CMyJson::WriteIntKey("imgDim",cnfGlobal.imgDim,jsonVal);
    CMyJson::WriteIntKey("autoTargetImage",cnfGlobal.autoTargetImage,jsonVal);


    intVec.clear();
    intVec.push_back(MAX_LAMP_PER_BOARD);
    CMyJson::WriteIntArrayKey("bLogoLightMode",intVec,(int*)cnfGlobal.bLogoLightMode,jsonVal);

    intVec.clear();
    intVec.push_back(MAX_LAMP_PER_BOARD);
    CMyJson::WriteIntArrayKey("bLogoLightLength",intVec,(int*)cnfGlobal.bLogoLightLength,jsonVal);


    intVec.clear();
    intVec.push_back(MAX_ALARM);
    CMyJson::WriteIntArrayKey("nAlarmValid",intVec,(int*)cnfGlobal.nAlarmValid,jsonVal);


    //CMyJson::WriteIntArrayKey("nCameraCompressMode",intVec,(int*)cnfGlobal.nCameraCompressMode,jsonVal);
    intVec.clear();
    intVec.push_back(MAX_LAYER);
    intVec.push_back(MAX_VAVLE);
    CMyJson::WriteIntArrayKey("nVavleBoardMode",intVec,(int*)cnfGlobal.nVavleBoardMode,jsonVal);

    stuAlarmCfgToJson(cnfGlobal.struAlarmConfig,jsonVal["struAlarmConfig"]);
    stuLampCfgToJson(cnfGlobal.stuLayerLampCfg,jsonVal["stuLayerLampCfg"]);
    stuModelSelectCfgToJson(cnfGlobal.stumodeLableSelect,jsonVal["stumodeLableSelect"]);


    for(int i = 0;i< MAX_LAYER;i++){
        for(int j = 0;j < MAX_VIEW;j++){
            CMyJson::WriteIntKey("nStaticsTotal",cnfGlobal.nStaticsConfig[i][j].nStaticsTotal,jsonVal["nStaticsConfig"][i][j]);
            intVec.clear();
            intVec.push_back(MAX_STATICS_TYPE);
            CMyJson::WriteIntArrayKey("nIndex",intVec,(int*)cnfGlobal.nStaticsConfig[i][j].nIndex,jsonVal["nStaticsConfig"][i][j]);
            CMyJson::WriteIntArrayKey("nBytes",intVec,(int*)cnfGlobal.nStaticsConfig[i][j].nBytes,jsonVal["nStaticsConfig"][i][j]);
        }
    }

    CMyJson::WriteIntKey("feederOnffCount",cnfGlobal.struIntelWipe.feederOnffCount,jsonVal["struIntelWipe"]);
    CMyJson::WriteIntKey("wiperDurationPerChute",cnfGlobal.struIntelWipe.wiperDurationPerChute,jsonVal["struIntelWipe"]);
    CMyJson::WriteIntKey("wiperWidth",cnfGlobal.struIntelWipe.wiperWidth,jsonVal["struIntelWipe"]);
    CMyJson::WriteIntKey("wipeTimeout",cnfGlobal.struIntelWipe.wipeTimeout,jsonVal["struIntelWipe"]);
    CMyJson::WriteIntKey("wiperDefaultPosi",cnfGlobal.struIntelWipe.wiperDefaultPosi,jsonVal["struIntelWipe"]);
    CMyJson::WriteIntKey("wiperConfirmThread",cnfGlobal.struIntelWipe.wiperConfirmThread,jsonVal["struIntelWipe"]);

    CMyJson::WriteIntKey("valve_start_use",cnfGlobal.stuUseLimitCfg.valve_start_use,jsonVal["stuUseLimitCfg"]);
    CMyJson::WriteIntKey("valve_life",cnfGlobal.stuUseLimitCfg.valve_life,jsonVal["stuUseLimitCfg"]);
    CMyJson::WriteIntKey("filter_start_use",cnfGlobal.stuUseLimitCfg.filter_start_use,jsonVal["stuUseLimitCfg"]);
    CMyJson::WriteIntKey("filter_life",cnfGlobal.stuUseLimitCfg.filter_life,jsonVal["stuUseLimitCfg"]);
    CMyJson::WriteIntKey("wipe_start_use",cnfGlobal.stuUseLimitCfg.wipe_start_use,jsonVal["stuUseLimitCfg"]);
    CMyJson::WriteIntKey("wipe_life",cnfGlobal.stuUseLimitCfg.wipe_life,jsonVal["stuUseLimitCfg"]);
    CMyJson::WriteIntKey("lamp_start_use",cnfGlobal.stuUseLimitCfg.lamp_start_use,jsonVal["stuUseLimitCfg"]);
    CMyJson::WriteIntKey("lamp_life",cnfGlobal.stuUseLimitCfg.lamp_life,jsonVal["stuUseLimitCfg"]);
    CMyJson::WriteIntKey("isHasRadar",cnfGlobal.stuFeedAutoCtrl.isHasRadar,jsonVal["stuFeedAutoCtrl"]);
    CMyJson::WriteIntKey("riceContainerDepth",cnfGlobal.stuFeedAutoCtrl.riceContainerDepth,jsonVal["stuFeedAutoCtrl"]);
    CMyJson::WriteIntKey("levelEmptyPosi",cnfGlobal.stuFeedAutoCtrl.levelEmptyPosi,jsonVal["stuFeedAutoCtrl"]);
    CMyJson::WriteIntKey("levelMorePosi",cnfGlobal.stuFeedAutoCtrl.levelMorePosi,jsonVal["stuFeedAutoCtrl"]);
    CMyJson::WriteIntKey("levelFullPosi",cnfGlobal.stuFeedAutoCtrl.levelFullPosi,jsonVal["stuFeedAutoCtrl"]);
    CMyJson::WriteIntKey("levelLowPosi",cnfGlobal.stuFeedAutoCtrl.levelLowPosi,jsonVal["stuFeedAutoCtrl"]);
    CMyJson::WriteIntKey("feedMoreStartValue",cnfGlobal.stuFeedAutoCtrl.feedMoreStartValue,jsonVal["stuFeedAutoCtrl"]);
    CMyJson::WriteIntKey("feedEmpytLowLimitValue",cnfGlobal.stuFeedAutoCtrl.feedEmpytLowLimitValue,jsonVal["stuFeedAutoCtrl"]);
    CMyJson::WriteIntKey("feedFullToNormalContinueTime",cnfGlobal.stuFeedAutoCtrl.feedFullToNormalContinueTime,jsonVal["stuFeedAutoCtrl"]);
    CMyJson::WriteIntKey("feedBalanceClearUpValue",cnfGlobal.stuFeedAutoCtrl.feedBalanceClearUpValue,jsonVal["stuFeedAutoCtrl"]);
    CMyJson::WriteIntKey("feedLevelChangeWaitTime",cnfGlobal.stuFeedAutoCtrl.feedLevelChangeWaitTime,jsonVal["stuFeedAutoCtrl"]);
    CMyJson::WriteIntKey("feedPreFullConstValue",cnfGlobal.stuFeedAutoCtrl.feedPreFullConstValue,jsonVal["stuFeedAutoCtrl"]);
    CMyJson::WriteIntKey("feedClearModeValue",cnfGlobal.stuFeedAutoCtrl.feedClearModeValue,jsonVal["stuFeedAutoCtrl"]);
    CMyJson::WriteIntKey("feedLevelPreFullWaitTime",cnfGlobal.stuFeedAutoCtrl.feedLevelPreFullWaitTime,jsonVal["stuFeedAutoCtrl"]);
    CMyJson::WriteIntKey("feedBalAdjustDuration",cnfGlobal.stuFeedAutoCtrl.feedBalAdjustDuration,jsonVal["stuFeedAutoCtrl"]);
    CMyJson::WriteIntKey("feedBalAdjustStep",cnfGlobal.stuFeedAutoCtrl.feedBalAdjustStep,jsonVal["stuFeedAutoCtrl"]);
    CMyJson::WriteIntKey("posiChgThres",cnfGlobal.stuFeedAutoCtrl.posiChgThres,jsonVal["stuFeedAutoCtrl"]);
    CMyJson::WriteIntKey("posiChgWait",cnfGlobal.stuFeedAutoCtrl.posiChgWait,jsonVal["stuFeedAutoCtrl"]);
    CMyJson::WriteIntKey("adjustPosuUnit",cnfGlobal.stuFeedAutoCtrl.adjustPosuUnit,jsonVal["stuFeedAutoCtrl"]);
    CMyJson::WriteIntKey("balUpFeedflow",cnfGlobal.stuFeedAutoCtrl.balUpFeedflow,jsonVal["stuFeedAutoCtrl"]);
    CMyJson::WriteIntKey("balLowFeedflow",cnfGlobal.stuFeedAutoCtrl.balLowFeedflow,jsonVal["stuFeedAutoCtrl"]);

    CMyJson::WriteIntKey("autoFeedMode",cnfGlobal.stuFeedAutoCtrl.autoFeedMode,jsonVal["stuFeedAutoCtrl"]);
    CMyJson::WriteIntKey("mode1FeedTargetPartCount",cnfGlobal.stuFeedAutoCtrl.mode1FeedTargetPartCount,jsonVal["stuFeedAutoCtrl"]);
    CMyJson::WriteIntKey("balUpLowFlow",cnfGlobal.stuFeedAutoCtrl.balUpLowFlow,jsonVal["stuFeedAutoCtrl"]);
    CMyJson::WriteIntKey("balUpFeedWhileLowFlow",cnfGlobal.stuFeedAutoCtrl.balUpFeedWhileLowFlow,jsonVal["stuFeedAutoCtrl"]);

    intVec.clear();
    intVec.push_back(MAX_FEEDER);
    CMyJson::WriteIntArrayKey("nFeedValueBias",intVec,cnfGlobal.stuFeedAutoCtrl.nFeedValueBias,jsonVal["stuFeedAutoCtrl"]);

    intVec.clear();
    intVec.push_back(MAX_LAYER);
    intVec.push_back(MAX_VIEW);
    intVec.push_back(MAX_GROUP_IDENTIFY);
    CMyJson::WriteIntArrayKey("sStaticsGetGroupEnable",intVec,(int*)cnfGlobal.sStaticsGetGroupEnable,jsonVal);

    for(int i = 0;i< MAX_LAYER;i++){
        for(int j = 0;j < MAX_VIEW;j++){
            for(int k = 0; k < MAX_VIEW_UNIT; k++)
            {
                CMyJson::WriteIntKey("ipcType",cnfGlobal.struIpcCameraParams[i][j][k].ipcType,jsonVal["struIpcCameraParams"][i][j][k]);
                CMyJson::WriteIntKey("cameraId",cnfGlobal.struIpcCameraParams[i][j][k].cameraId,jsonVal["struIpcCameraParams"][i][j][k]);
                CMyJson::WriteIntKey("nHoriPixel",cnfGlobal.struIpcCameraParams[i][j][k].nHoriPixel,jsonVal["struIpcCameraParams"][i][j][k]);
                CMyJson::WriteIntKey("nVertPixel",cnfGlobal.struIpcCameraParams[i][j][k].nVertPixel,jsonVal["struIpcCameraParams"][i][j][k]);
                CMyJson::WriteIntKey("nHoriOffset",cnfGlobal.struIpcCameraParams[i][j][k].nHoriOffset,jsonVal["struIpcCameraParams"][i][j][k]);
                CMyJson::WriteIntKey("nHoriWidth",cnfGlobal.struIpcCameraParams[i][j][k].nHoriWidth,jsonVal["struIpcCameraParams"][i][j][k]);
                CMyJson::WriteIntKey("nHoriWidth_2",cnfGlobal.struIpcCameraParams[i][j][k].nHoriWidth_2,jsonVal["struIpcCameraParams"][i][j][k]);
                CMyJson::WriteIntKey("nVertOffset",cnfGlobal.struIpcCameraParams[i][j][k].nVertOffset,jsonVal["struIpcCameraParams"][i][j][k]);
                CMyJson::WriteIntKey("nVertWidth",cnfGlobal.struIpcCameraParams[i][j][k].nVertWidth,jsonVal["struIpcCameraParams"][i][j][k]);
                CMyJson::WriteIntKey("nSaveImgEnable",cnfGlobal.struIpcCameraParams[i][j][k].nSaveImgEnable,jsonVal["struIpcCameraParams"][i][j][k]);
                intVec.clear();
                intVec.push_back(4);
                CMyJson::WriteIntArrayKey("nIpAddr",intVec,(int*)cnfGlobal.struIpcCameraParams[i][j][k].nIpAddr,jsonVal["struIpcCameraParams"][i][j][k]);
            }
        }
    }
    for(int i = 0;i< MAX_VIEW;i++){
        for(int j = 0; j <MAX_VIEW_UNIT;j++){
            CMyJson::WriteIntKey("cameraCount",cnfGlobal.stuIpcConfigFileInfo[i][j].cameraCount,jsonVal["stuIpcConfigFileInfo"][i][j]);
            CMyJson::WriteIntKey("batchSize",cnfGlobal.stuIpcConfigFileInfo[i][j].batchSize,jsonVal["stuIpcConfigFileInfo"][i][j]);
            CMyJson::WriteIntKey("roiHeight",cnfGlobal.stuIpcConfigFileInfo[i][j].roiHeight,jsonVal["stuIpcConfigFileInfo"][i][j]);
            CMyJson::WriteIntKey("roiRepeat",cnfGlobal.stuIpcConfigFileInfo[i][j].roiRepeat,jsonVal["stuIpcConfigFileInfo"][i][j]);
        }
    }
    intVec.clear();
    intVec.push_back(12);
    intVec.push_back(80);
    CMyJson::WriteIntArrayKey("totalblowcount",intVec,(int*)cnfGlobal.totalblowcount,jsonVal);

    intVec.clear();
    intVec.push_back(MAX_VIEW);
    intVec.push_back(MAX_VIEW_UNIT);
    CMyJson::WriteIntArrayKey("nIpcStartPixel",intVec,(int*)cnfGlobal.nIpcStartPixel,jsonVal);
    CMyJson::WriteIntArrayKey("nIpcEndPixel",intVec,(int*)cnfGlobal.nIpcEndPixel,jsonVal);

    intVec.clear();
    intVec.push_back(MAX_VIEW);
    CMyJson::WriteIntArrayKey("nXsystemBoard",intVec,(int*)cnfGlobal.nXsystemBoard,jsonVal);

    intVec.clear();
    intVec.push_back(MAX_ACCELERATION);
    CMyJson::WriteIntArrayKey("accelerationEnable",intVec,(int*)cnfGlobal.accelerationEnable,jsonVal);
    CMyJson::WriteIntKey("accelerationTolerance",cnfGlobal.accelerationTolerance,jsonVal);
    CMyJson::WriteIntKey("accelerationNum",cnfGlobal.accelerationNum,jsonVal);

    CMyJson::WriteIntArrayKey("accelerationEnable2",intVec,(int*)cnfGlobal.accelerationEnable2,jsonVal);
    CMyJson::WriteIntKey("accelerationTolerance2",cnfGlobal.accelerationTolerance2,jsonVal);
    CMyJson::WriteIntKey("accelerationNum2",cnfGlobal.accelerationNum2,jsonVal);
    CMyJson::WriteIntKey("viBoardTotalCount",cnfGlobal.viBoardTotalCount,jsonVal);
    CMyJson::WriteIntKey("viBoardAlarmEnable",cnfGlobal.viBoardAlarmEnable,jsonVal);

    intVec.clear();
    intVec.push_back(MAX_VI_BOARD);
    intVec.push_back(MAX_VI_BOARD_INFO);
    CMyJson::WriteIntArrayKey("viBoardInfo",intVec,(int*)cnfGlobal.viBoardInfo,jsonVal);

    intVec.clear();
    intVec.push_back(MAX_VIEW);
    intVec.push_back(MAX_CALIBRATE_SETS);
    CMyJson::WriteIntArrayKey("nSenseBrightSelection_calb",intVec,(int*)cnfGlobal.nSenseBrightSelection_calb,jsonVal);
    CMyJson::WriteIntArrayKey("nSensorRowFre_calb",intVec,(int*)cnfGlobal.nSensorRowFre_calb,jsonVal);
    CMyJson::WriteIntArrayKey("bCaliTDmode_calb",intVec,(int*)cnfGlobal.bCaliTDmode_calb,jsonVal);
    intVec.clear();
    intVec.push_back(MAX_CALIBRATE_SETS);
    CMyJson::WriteIntArrayKey("bCaliGainEnable_calb",intVec,(int*)cnfGlobal.bCaliGainEnable_calb,jsonVal);
    CMyJson::WriteIntArrayKey("bCaliBiasEnable_calb",intVec,(int*)cnfGlobal.bCaliBiasEnable_calb,jsonVal);

}

bool JsonDataConvert::JsonTostruCnfProfile(const Json::Value &jsonVal, struCnfProfile &cnfProfile)
{
    JSON_RETURN_VAL jRet;
    INT_ARRAY_KEY outArray;
    vector<int> intVec;

    //    string modelName;
    //    if(JSON_FAILED_TYPE == CMyJson::ReadStringKey(jsonVal, "sCurModelName", modelName))
    //        return false;
    //    memcpy(cnfProfile.sCurModelName, modelName.data(), modelName.length());

    //    string modelDate;
    //    if(JSON_FAILED_TYPE == CMyJson::ReadStringKey(jsonVal, "sCurModelDate", modelDate))
    //        return false;
    //    memcpy(cnfProfile.sCurModelDate, modelDate.data(), modelDate.length());

    intVec.clear();
    intVec.push_back(MAX_LAYER);
    intVec.push_back(MAX_VIEW);
    intVec.push_back(3);
    CMyJson::ReadIntArrayKey(jsonVal,"nBackgroundColor",intVec,(int*)cnfProfile.nBackgroundColor,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nBackgroundColorValueLow",intVec,(int*)cnfProfile.nBackgroundColorValueLow,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nBackgroundColorValueUp",intVec,(int*)cnfProfile.nBackgroundColorValueUp,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nBackgroundColorValueVLow",intVec,(int*)cnfProfile.nBackgroundColorValueVLow,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nBackgroundColorValueVUp",intVec,(int*)cnfProfile.nBackgroundColorValueVUp,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nBackgroundColorProLow",intVec,(int*)cnfProfile.nBackgroundColorProLow,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nBackgroundColorProUp",intVec,(int*)cnfProfile.nBackgroundColorProUp,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nBackgroundColorSatUp",intVec,(int*)cnfProfile.nBackgroundColorSatUp,outArray);

    intVec.clear();
    intVec.push_back(MAX_LAYER);
    intVec.push_back(MAX_VIEW);
    CMyJson::ReadIntArrayKey(jsonVal,"nIpcDelayBufRowCount",intVec,(int*)cnfProfile.nIpcDelayBufRowCount,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nSensorD2Mode",intVec,(int*)cnfProfile.nSensorD2Mode,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nSenseBrightSelection",intVec,(int*)cnfProfile.nSenseBrightSelection,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nSensorRowFre",intVec,(int*)cnfProfile.nSensorRowFre,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nSensResizeMode",intVec,(int*)cnfProfile.nSensResizeMode,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nTimeDivisionScheme",intVec,(int*)cnfProfile.nTimeDivisionScheme,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nViewSensorType",intVec,(int*)cnfProfile.nViewSensorType,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nSensorPixel",intVec,(int*)cnfProfile.nSensorPixel,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nSensorDim",intVec,(int*)cnfProfile.nSensorDim,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nSensorDimD2",intVec,(int*)cnfProfile.nSensorDimD2,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nSensBitWidth",intVec,(int*)cnfProfile.nSensBitWidth,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nSensorDataType",intVec,(int*)cnfProfile.nSensorDataType,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nImageSplice",intVec,(int*)cnfProfile.nImageSplice,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nMirror",intVec,(int*)cnfProfile.nMirror,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nViewChainAddr",intVec,(int*)cnfProfile.nViewChainAddr,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nSensDropLine",intVec,(int*)cnfProfile.nSensDropLine,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nSensCmdSpeed",intVec,(int*)cnfProfile.nSensCmdSpeed,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nRealTimeCorrection",intVec,(int*)cnfProfile.nRealTimeCorrection,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nCameraCompressMode",intVec,(int*)cnfProfile.nCameraCompressMode,outArray);
    // CMyJson::ReadIntArrayKey(jsonVal,"nCameraBoardMode",intVec,(int*)cnfProfile.nCameraBoardMode,outArray);

    intVec.clear();
    intVec.push_back(MAX_LAYER);
    intVec.push_back(MAX_VIEW);
    intVec.push_back(4);
    CMyJson::ReadIntArrayKey(jsonVal,"nSensorColorChoose",intVec,(int*)cnfProfile.nSensorColorChoose,outArray);

    //    intVec.clear();
    //    intVec.push_back(MAX_LAYER);
    //    intVec.push_back(MAX_VAVLE);
    //    CMyJson::ReadIntArrayKey(jsonVal,"nVavleBoardMode",intVec,(int*)cnfProfile.nVavleBoardMode,outArray);

    CMyJson::ReadIntKey(jsonVal,"nMatCopySetMode",cnfProfile.nMatCopySetMode);
    CMyJson::ReadIntKey(jsonVal,"nIsCameraConpress",cnfProfile.nIsCameraConpress);
    intVec.clear();
    intVec.push_back(MAX_LAYER);
    intVec.push_back(MAX_LAMP_BOARD_PER_INTERFACE);
    intVec.push_back(MAX_LAMP_PER_BOARD);
    CMyJson::ReadIntArrayKey(jsonVal,"nLampValue",intVec,(int*)cnfProfile.nLampValue,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nLampEnable",intVec,(int*)cnfProfile.nLampEnable,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nLampSeq",intVec,(int*)cnfProfile.nLampSeq,outArray);


    intVec.clear();
    intVec.push_back(MAX_LAYER);
    intVec.push_back(MAX_VIEW);
    intVec.push_back(MAX_GROUP_IDENTIFY);
    intVec.push_back(ARITHMETIC_TOTAL);
    CMyJson::ReadIntArrayKey(jsonVal,"nGroupArithEnable",intVec,(int*)cnfProfile.nGroupArithEnable,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nGroupArithEdgeEnable",intVec,(int*)cnfProfile.nGroupArithEdgeEnable,outArray);

    intVec.clear();
    intVec.push_back(MAX_LAYER);
    intVec.push_back(MAX_VIEW);
    intVec.push_back(MAX_VIEW_UNIT);
    CMyJson::ReadIntArrayKey(jsonVal,"nProfileAnaGainRed",intVec,(int*)cnfProfile.nProfileAnaGainRed,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nProfileAnaGainGreen",intVec,(int*)cnfProfile.nProfileAnaGainGreen,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nProfileAnaGainBlue",intVec,(int*)cnfProfile.nProfileAnaGainBlue,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nProfileAnaGainNir",intVec,(int*)cnfProfile.nProfileAnaGainNir,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nProfileGainTargetRed",intVec,(int*)cnfProfile.nProfileGainTargetRed,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nProfileGainTargetGreen",intVec,(int*)cnfProfile.nProfileGainTargetGreen,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nProfileGainTargetBlue",intVec,(int*)cnfProfile.nProfileGainTargetBlue,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nProfileGainTargetNir",intVec,(int*)cnfProfile.nProfileGainTargetNir,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nProfileDigiGainRed",intVec,(int*)cnfProfile.nProfileDigiGainRed,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nProfileDigiGainGreen",intVec,(int*)cnfProfile.nProfileDigiGainGreen,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nProfileDigiGainBlue",intVec,(int*)cnfProfile.nProfileDigiGainBlue,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nProfileDigiGainNir",intVec,(int*)cnfProfile.nProfileDigiGainNir,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nCameraBoardUserMode",intVec,(int*)cnfProfile.nCameraBoardUserMode,outArray);



    for(int i = 0;i< MAX_LAYER;i++){
        for(int j = 0;j < MAX_VIEW;j++){
            for(int k = 0; k < MAX_GROUP_IDENTIFY;k++){
                intVec.clear();
                intVec.push_back(2);
                CMyJson::ReadIntArrayKey(jsonVal["struGroupIdentify"][i][j][k],"nEdgeCut",intVec,(int*)cnfProfile.struGroupIdentify[i][j][k].nEdgeCut,outArray);
                CMyJson::ReadIntArrayKey(jsonVal["struGroupIdentify"][i][j][k],"nImpured",intVec,(int*)cnfProfile.struGroupIdentify[i][j][k].nImpured,outArray);
                CMyJson::ReadIntArrayKey(jsonVal["struGroupIdentify"][i][j][k],"nColseCal",intVec,(int*)cnfProfile.struGroupIdentify[i][j][k].nColseCal,outArray);
                CMyJson::ReadIntArrayKey(jsonVal["struGroupIdentify"][i][j][k],"nReverseSort",intVec,(int*)cnfProfile.struGroupIdentify[i][j][k].nReverseSort,outArray);
                CMyJson::ReadIntArrayKey(jsonVal["struGroupIdentify"][i][j][k],"nCenterSize",intVec,(int*)cnfProfile.struGroupIdentify[i][j][k].nCenterSize,outArray);
                intVec.clear();
                intVec.push_back(MAX_IMPURED_TOTAL);
                CMyJson::ReadIntArrayKey(jsonVal["struGroupIdentify"][i][j][k],"nImpuredIpc",intVec,(int*)cnfProfile.struGroupIdentify[i][j][k].nImpuredIpc,outArray);

                CMyJson::ReadIntKey(jsonVal["struGroupIdentify"][i][j][k],"nEdgeMat",cnfProfile.struGroupIdentify[i][j][k].nEdgeMat);
                CMyJson::ReadIntKey(jsonVal["struGroupIdentify"][i][j][k],"nEjectDelay",cnfProfile.struGroupIdentify[i][j][k].nEjectDelay);
                CMyJson::ReadIntKey(jsonVal["struGroupIdentify"][i][j][k],"nEjectDelayShape",cnfProfile.struGroupIdentify[i][j][k].nEjectDelayShape);
                CMyJson::ReadIntKey(jsonVal["struGroupIdentify"][i][j][k],"nEjectDelay3",cnfProfile.struGroupIdentify[i][j][k].nEjectDelay3);
                CMyJson::ReadIntKey(jsonVal["struGroupIdentify"][i][j][k],"nEjectDelay4",cnfProfile.struGroupIdentify[i][j][k].nEjectDelay4);
                CMyJson::ReadIntKey(jsonVal["struGroupIdentify"][i][j][k],"nEjectDelayModel",cnfProfile.struGroupIdentify[i][j][k].nEjectDelayModel);
                CMyJson::ReadIntKey(jsonVal["struGroupIdentify"][i][j][k],"nBadNum",cnfProfile.struGroupIdentify[i][j][k].nBadNum);
                CMyJson::ReadIntKey(jsonVal["struGroupIdentify"][i][j][k],"nSensUpdateGeneral",cnfProfile.struGroupIdentify[i][j][k].nSensUpdateGeneral);
                CMyJson::ReadIntKey(jsonVal["struGroupIdentify"][i][j][k],"arithRejectRatio",cnfProfile.struGroupIdentify[i][j][k].arithRejectRatio);

                JsonTostruGreyColor(MAX_GREY_COLOR_ARITH_TOTAL,jsonVal["struGroupIdentify"][i][j][k]["struGreyColor"],cnfProfile.struGroupIdentify[i][j][k].struGreyColor);
                JsonTostruIntel(4,jsonVal["struGroupIdentify"][i][j][k]["struSvm"],cnfProfile.struGroupIdentify[i][j][k].struSvm);
                JsonTostruShapeIntel(3,jsonVal["struGroupIdentify"][i][j][k]["struShapeSvm"],cnfProfile.struGroupIdentify[i][j][k].struShapeSvm);
                JsonTostruCircleLong(2,jsonVal["struGroupIdentify"][i][j][k]["struCircleLong"],cnfProfile.struGroupIdentify[i][j][k].struCircleLong);
                JsonTostruLongShort(2,jsonVal["struGroupIdentify"][i][j][k]["struLongShort"],cnfProfile.struGroupIdentify[i][j][k].struLongShort);
                JsonTostruBigSmall(2,jsonVal["struGroupIdentify"][i][j][k]["struBigSmall"],cnfProfile.struGroupIdentify[i][j][k].struBigSmall);
                JsonTostruHsv(3,jsonVal["struGroupIdentify"][i][j][k]["struHsv"],cnfProfile.struGroupIdentify[i][j][k].struHsv);
                JsonTostruColorSat(jsonVal["struGroupIdentify"][i][j][k]["struColorSat"],cnfProfile.struGroupIdentify[i][j][k].struColorSat);
                JsonTostruColorHue(jsonVal["struGroupIdentify"][i][j][k]["struColorHue"],cnfProfile.struGroupIdentify[i][j][k].struColorHue);
                JsonTostruColorScale(jsonVal["struGroupIdentify"][i][j][k]["struColorScale"],cnfProfile.struGroupIdentify[i][j][k].struColorScale);
                JsonTostruMildew(jsonVal["struGroupIdentify"][i][j][k]["struMildew"],cnfProfile.struGroupIdentify[i][j][k].struMildew);
                JsonTostruOutline(jsonVal["struGroupIdentify"][i][j][k]["struOutline"],cnfProfile.struGroupIdentify[i][j][k].struOutline);
                JsonTostruLine(jsonVal["struGroupIdentify"][i][j][k]["struLine"],cnfProfile.struGroupIdentify[i][j][k].struLine);
                JsonTostruBud(2,jsonVal["struGroupIdentify"][i][j][k]["struBud"],cnfProfile.struGroupIdentify[i][j][k].struBud);
                JsonTostruRound(2,jsonVal["struGroupIdentify"][i][j][k]["struRound"],cnfProfile.struGroupIdentify[i][j][k].struRound);
                JsonTostruColorValue(jsonVal["struGroupIdentify"][i][j][k]["struColorValue"], cnfProfile.struGroupIdentify[i][j][k].struColorValue);
                JsonTostruSplMildew(jsonVal["struGroupIdentify"][i][j][k]["struSplMildew"], cnfProfile.struGroupIdentify[i][j][k].struSplMildew);
                JsonTostruWhitePeanut(jsonVal["struGroupIdentify"][i][j][k]["struWhitePeanut"], cnfProfile.struGroupIdentify[i][j][k].struWhitePeanut);
                JsonTostruPepperHandle(jsonVal["struGroupIdentify"][i][j][k]["struPepperHandle"], cnfProfile.struGroupIdentify[i][j][k].struPepperHandle);
                JsonTostruSuppressLarge(3,jsonVal["struGroupIdentify"][i][j][k]["struSuppressLarge"],cnfProfile.struGroupIdentify[i][j][k].struSuppressLarge);
                JsonTostruWheatSprout(jsonVal["struGroupIdentify"][i][j][k]["struWheatSprout"], cnfProfile.struGroupIdentify[i][j][k].struWheatSprout);
                JsonToStruTeaColor(MAX_TEA_ARITH_TOTAL,jsonVal["struGroupIdentify"][i][j][k]["struTeaColor"],cnfProfile.struGroupIdentify[i][j][k].struTeaColor);
            }
            for(int k = 0; k < MAX_VIEW_UNIT;k++){
                for(int l = 0;l < 2;l++){
                    intVec.clear();
                    intVec.push_back(MAX_GREY_COLOR_ARITH_TOTAL);
                    CMyJson::ReadIntArrayKey(jsonVal["struUnitSens"][i][j][k][l],"greyColor",intVec,(int*)cnfProfile.struUnitSens[i][j][k][l].greyColor,outArray);
                    intVec.clear();
                    intVec.push_back(3);
                    CMyJson::ReadIntArrayKey(jsonVal["struUnitSens"][i][j][k][l],"svm",intVec,(int*)cnfProfile.struUnitSens[i][j][k][l].svm,outArray);
                    CMyJson::ReadIntArrayKey(jsonVal["struUnitSens"][i][j][k][l],"suppressLarge",intVec,(int*)cnfProfile.struUnitSens[i][j][k][l].suppressLarge,outArray);
                    CMyJson::ReadIntArrayKey(jsonVal["struUnitSens"][i][j][k][l],"hsv",intVec,cnfProfile.struUnitSens[i][j][k][l].hsv,outArray);
                    intVec.clear();
                    intVec.push_back(2);
                    CMyJson::ReadIntArrayKey(jsonVal["struUnitSens"][i][j][k][l],"bigSmall",intVec,cnfProfile.struUnitSens[i][j][k][l].bigSmall,outArray);
                    CMyJson::ReadIntArrayKey(jsonVal["struUnitSens"][i][j][k][l],"circleLong",intVec,cnfProfile.struUnitSens[i][j][k][l].circleLong,outArray);
                    CMyJson::ReadIntArrayKey(jsonVal["struUnitSens"][i][j][k][l],"longShort",intVec,(int*)cnfProfile.struUnitSens[i][j][k][l].longShort,outArray);
                    CMyJson::ReadIntKey(jsonVal["struUnitSens"][i][j][k][l],"sat",cnfProfile.struUnitSens[i][j][k][l].sat);
                    CMyJson::ReadIntKey(jsonVal["struUnitSens"][i][j][k][l],"hueH",cnfProfile.struUnitSens[i][j][k][l].hueH);
                    CMyJson::ReadIntKey(jsonVal["struUnitSens"][i][j][k][l],"hueL",cnfProfile.struUnitSens[i][j][k][l].hueL);
                    CMyJson::ReadIntKey(jsonVal["struUnitSens"][i][j][k][l],"scale",cnfProfile.struUnitSens[i][j][k][l].scale);
                    CMyJson::ReadIntKey(jsonVal["struUnitSens"][i][j][k][l],"mildew",cnfProfile.struUnitSens[i][j][k][l].mildew);
                    CMyJson::ReadIntKey(jsonVal["struUnitSens"][i][j][k][l],"outline",cnfProfile.struUnitSens[i][j][k][l].outline);
                    CMyJson::ReadIntKey(jsonVal["struUnitSens"][i][j][k][l],"line",cnfProfile.struUnitSens[i][j][k][l].line);
                    CMyJson::ReadIntArrayKey(jsonVal["struUnitSens"][i][j][k][l],"bud",intVec,(int*)cnfProfile.struUnitSens[i][j][k][l].bud,outArray);
                    CMyJson::ReadIntArrayKey(jsonVal["struUnitSens"][i][j][k][l],"round",intVec,cnfProfile.struUnitSens[i][j][k][l].round,outArray);
                    CMyJson::ReadIntKey(jsonVal["struUnitSens"][i][j][k][l], "value", cnfProfile.struUnitSens[i][j][k][l].value);
                    CMyJson::ReadIntKey(jsonVal["struUnitSens"][i][j][k][l],"splMildew",cnfProfile.struUnitSens[i][j][k][l].splMildew);
                    CMyJson::ReadIntKey(jsonVal["struUnitSens"][i][j][k][l],"whitePeanut",cnfProfile.struUnitSens[i][j][k][l].whitePeanut);
                }
            }
        }

        for(int j= 0;j < MAX_VAVLE_BOARD;j++){
            for(int k = 0;k < MAX_GROUP_TICK;k++){
                CMyJson::ReadIntKey(jsonVal["struGroupTick"][i][j][k],"nEjectDuration",cnfProfile.struGroupTick[i][j][k].nEjectDuration);
                CMyJson::ReadIntKey(jsonVal["struGroupTick"][i][j][k],"nEjectDuration2",cnfProfile.struGroupTick[i][j][k].nEjectDuration2);
                CMyJson::ReadIntKey(jsonVal["struGroupTick"][i][j][k],"nFrontRearMode",cnfProfile.struGroupTick[i][j][k].nFrontRearMode);
                CMyJson::ReadIntKey(jsonVal["struGroupTick"][i][j][k],"nEjwidth",cnfProfile.struGroupTick[i][j][k].nEjwidth);
                CMyJson::ReadIntKey(jsonVal["struGroupTick"][i][j][k],"nWaitTime",cnfProfile.struGroupTick[i][j][k].nWaitTime);
                CMyJson::ReadIntKey(jsonVal["struGroupTick"][i][j][k],"nTailTime",cnfProfile.struGroupTick[i][j][k].nTailTime);
                CMyJson::ReadIntKey(jsonVal["struGroupTick"][i][j][k],"nEjectTimesLimit",cnfProfile.struGroupTick[i][j][k].nEjectTimesLimit);
            }
        }
    }
    for(int i = 0;i < MAX_LAYER;i++){
        for(int j = 0;j < MAX_GROUP_TICK;j++){
            CMyJson::ReadIntKey(jsonVal["nAutoFeed"][i][j],"bEnable",cnfProfile.nAutoFeed[i][j].bEnable);
            CMyJson::ReadIntKey(jsonVal["nAutoFeed"][i][j],"target_feeder",cnfProfile.nAutoFeed[i][j].target_feeder);
            CMyJson::ReadIntKey(jsonVal["nAutoFeed"][i][j],"start_feeder",cnfProfile.nAutoFeed[i][j].start_feeder);
            CMyJson::ReadIntKey(jsonVal["nAutoFeed"][i][j],"stop_feeder",cnfProfile.nAutoFeed[i][j].stop_feeder);
            CMyJson::ReadIntKey(jsonVal["nAutoFeed"][i][j],"stop_duration",cnfProfile.nAutoFeed[i][j].stop_duration);
            CMyJson::ReadIntKey(jsonVal["nAutoFeed"][i][j],"ajust_feeder_range",cnfProfile.nAutoFeed[i][j].ajust_feeder_range);
        }
    }
    CMyJson::ReadIntKey(jsonVal,"n_coff",cnfProfile.n_coff);
    CMyJson::ReadIntKey(jsonVal,"bColorRestoreEnable",cnfProfile.bColorRestoreEnable);
    CMyJson::ReadIntKey(jsonVal,"nImpuredIpcCount",cnfProfile.nImpuredIpcCount);
    CMyJson::ReadIntKey(jsonVal,"nEjectDurationIpcCount",cnfProfile.nEjectDurationIpcCount);
    CMyJson::ReadFloatKey(jsonVal,"ipcBadPointscoff",cnfProfile.ipcBadPointscoff);

    CMyJson::ReadIntKey(jsonVal,"bCurCal",cnfProfile.bCurCal);
    vector<string> strsNameVec;
    //    if(JSON_SUCCESS == CMyJson::ReadStringArrayKey(jsonVal, "sCurModelName", strsNameVec)){
    //        for(int i = 0;i < MAX_VIEW;i++){
    //            memset(cnfProfile.sCurModelName[i],0,sizeof(cnfProfile.sCurModelName[i]));
    //            memcpy(cnfProfile.sCurModelName[i],strsNameVec[i].data(),strsNameVec[i].length());
    //        }
    //    }



    if(JSON_SUCCESS == CMyJson::ReadStringArrayKey(jsonVal, "sCurModelName", strsNameVec)){
        int index = 0;
        for(int i = 0;i < MAX_VIEW;i++){
            for(int j = 0; j < MAX_GROUP_IDENTIFY; j++)
            {
                memset(cnfProfile.sCurModelName[i][j],0,sizeof(cnfProfile.sCurModelName[i][j]));

                if(strsNameVec.size() > index){
                    string strsName = strsNameVec[index++];
                    memcpy(cnfProfile.sCurModelName[i][j],strsName.data(),strsName.length());
                }
            }
        }
    }

    strsNameVec.clear();
    if(JSON_SUCCESS == CMyJson::ReadStringArrayKey(jsonVal, "sCurModelDate", strsNameVec)){
        int index = 0;
        for(int i = 0;i < MAX_VIEW;i++){
            for(int j = 0; j < MAX_GROUP_IDENTIFY; j++)
            {
                memset(cnfProfile.sCurModelDate[i][j],0,sizeof(cnfProfile.sCurModelDate[i][j]));

                if(strsNameVec.size() > index){
                    string strsName = strsNameVec[index++];
                    memcpy(cnfProfile.sCurModelDate[i][j], strsName.data(),strsName.length());
                }
            }
        }
    }

    strsNameVec.clear();
    if(JSON_SUCCESS == CMyJson::ReadStringArrayKey(jsonVal, "sCurBigstatisname", strsNameVec)){
        for(int i = 0;i < MAX_ONEPAGE_IPCMODEL_CLASS;i++){
            memset(cnfProfile.sCurBigstatisname[i],0,sizeof(cnfProfile.sCurBigstatisname[i]));
            memcpy(cnfProfile.sCurBigstatisname[i],strsNameVec[i].data(),strsNameVec[i].length());
        }
    }
    //    }

    CMyJson::ReadIntKey(jsonVal,"sStaticsGetDuration",cnfProfile.sStaticsGetDuration);
    CMyJson::ReadIntKey(jsonVal,"nHasSaveModel",cnfProfile.nHasSaveModel);
    CMyJson::ReadIntKey(jsonVal,"nIpcBadMatAlarmCount",cnfProfile.nIpcBadMatAlarmCount);
    CMyJson::ReadIntKey(jsonVal,"nIpcHasMatPixelPercert",cnfProfile.nIpcHasMatPixelPercert);
    CMyJson::ReadIntKey(jsonVal,"nIpcStatisticsBigclass",cnfProfile.nIpcStatisticsBigclass);
    intVec.clear();
    intVec.push_back(MAX_VIEW);
    //CMyJson::ReadIntArrayKey(jsonVal,"ipcModelClassCount",intVec,(int*)cnfProfile.ipcModelClassCount,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nIpcRoiHeight",intVec,(int*)cnfProfile.nIpcRoiHeight,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nIpcRoiWidth",intVec,(int*)cnfProfile.nIpcRoiWidth,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nIpcRoiRepeat",intVec,(int*)cnfProfile.nIpcRoiRepeat,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nIpcRoiRepeat2",intVec,(int*)cnfProfile.nIpcRoiRepeat2,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"imageEnahnce",intVec,(int*)cnfProfile.imageEnahnce,outArray);
    intVec.clear();
    intVec.push_back(MAX_VIEW);
    intVec.push_back(MAX_GROUP_IDENTIFY);
    CMyJson::ReadIntArrayKey(jsonVal,"ipcModelClassCount",intVec,(int*)cnfProfile.ipcModelClassCount,outArray);

    intVec.clear();
    intVec.push_back(MAX_IPC_MODEL_CLASS);
    CMyJson::ReadIntArrayKey(jsonVal,"nIpcStatisticsclassId",intVec,(int*)cnfProfile.nIpcStatisticsclassId,outArray);
    intVec.clear();
    intVec.push_back(MAX_IPC_MODEL_CLASS);
    CMyJson::ReadIntArrayKey(jsonVal,"nIpcStatisticsbigclassCoeff",intVec,(int*)cnfProfile.nIpcStatisticsbigclassCoeff,outArray);
    intVec.clear();
    intVec.push_back(MAX_IPC_MODEL_CLASS);
    CMyJson::ReadIntArrayKey(jsonVal,"bstaticClassAlarm",intVec,(int*)cnfProfile.bstaticClassAlarm,outArray);

    Float_ARRAY_KEY floatoutArray;
    CMyJson::ReadFloatArrayKey(jsonVal,"nIpcRoiRepeatf",intVec,(float*)cnfProfile.nIpcRoiRepeatf,floatoutArray);
    CMyJson::ReadIntKey(jsonVal,"nDualLayerModelSame",cnfProfile.nDualLayerModelSame);
    CMyJson::ReadIntKey(jsonVal,"nIpcCameraClassSameMode",cnfProfile.nIpcCameraClassSameMode);
    CMyJson::ReadIntKey(jsonVal,"bCaliGainEnable",cnfProfile.bCaliGainEnable);
    CMyJson::ReadIntKey(jsonVal,"bCaliBiasEnable",cnfProfile.bCaliBiasEnable);


    strsNameVec.clear();
    for(int j = 0;j < MAX_VIEW;j++){
        for(int t = 0;t < MAX_GROUP_IDENTIFY;t++){
            intVec.clear();
            intVec.push_back(MAX_IPC_MODEL_CLASS);
            strsNameVec.clear();
            if(JSON_SUCCESS == CMyJson::ReadStringArrayKey(jsonVal["struCameraIpcClassGroupParams"][j][t], "className", strsNameVec)){
                //for(int k = 0;k < MAX_IPC_MODEL_CLASS;k++){
                for(int k = 0;k < strsNameVec.size();k++){
                    if (k == MAX_IPC_MODEL_CLASS) break;
                    memset(cnfProfile.struCameraIpcClassGroupParams[j][t].className[k],0,sizeof(cnfProfile.struCameraIpcClassGroupParams[j][t].className[k]));
                    string strsName = strsNameVec[k];
                    memcpy(cnfProfile.struCameraIpcClassGroupParams[j][t].className[k],strsName.data(),strsName.length());
                }
            }

            CMyJson::ReadIntArrayKey(jsonVal["struCameraIpcClassGroupParams"][j][t],"classEnable",intVec,(int*)cnfProfile.struCameraIpcClassGroupParams[j][t].classEnable,outArray);
            CMyJson::ReadIntArrayKey(jsonVal["struCameraIpcClassGroupParams"][j][t],"classSens",intVec,(int*)cnfProfile.struCameraIpcClassGroupParams[j][t].classSens,outArray);
            CMyJson::ReadIntArrayKey(jsonVal["struCameraIpcClassGroupParams"][j][t],"classThreshold",intVec,(int*)cnfProfile.struCameraIpcClassGroupParams[j][t].classThreshold,outArray);
            CMyJson::ReadIntArrayKey(jsonVal["struCameraIpcClassGroupParams"][j][t],"classLabelBox",intVec,(int*)cnfProfile.struCameraIpcClassGroupParams[j][t].classLabelBox,outArray);
            CMyJson::ReadIntArrayKey(jsonVal["struCameraIpcClassGroupParams"][j][t],"classLabelBox2",intVec,(int*)cnfProfile.struCameraIpcClassGroupParams[j][t].classLabelBox2,outArray);
            CMyJson::ReadIntArrayKey(jsonVal["struCameraIpcClassGroupParams"][j][t],"classTickMode",intVec,(int*)cnfProfile.struCameraIpcClassGroupParams[j][t].classTickMode,outArray);
            CMyJson::ReadIntArrayKey(jsonVal["struCameraIpcClassGroupParams"][j][t],"classFixMode",intVec,(int*)cnfProfile.struCameraIpcClassGroupParams[j][t].classFixMode,outArray);
            CMyJson::ReadIntArrayKey(jsonVal["struCameraIpcClassGroupParams"][j][t],"classFixMode2",intVec,(int*)cnfProfile.struCameraIpcClassGroupParams[j][t].classFixMode2,outArray);
            CMyJson::ReadIntArrayKey(jsonVal["struCameraIpcClassGroupParams"][j][t],"classMode",intVec,(int*)cnfProfile.struCameraIpcClassGroupParams[j][t].classMode,outArray);
            CMyJson::ReadIntArrayKey(jsonVal["struCameraIpcClassGroupParams"][j][t],"classDelayMode",intVec,(int*)cnfProfile.struCameraIpcClassGroupParams[j][t].classDelayMode,outArray);
            CMyJson::ReadIntArrayKey(jsonVal["struCameraIpcClassGroupParams"][j][t],"classHWRatio",intVec,(int*)cnfProfile.struCameraIpcClassGroupParams[j][t].classHWRatio,outArray);
            CMyJson::ReadIntArrayKey(jsonVal["struCameraIpcClassGroupParams"][j][t],"classPosAdjust",intVec,(int*)cnfProfile.struCameraIpcClassGroupParams[j][t].classPosAdjust,outArray);
            CMyJson::ReadIntArrayKey(jsonVal["struCameraIpcClassGroupParams"][j][t],"classBD",intVec,(int*)cnfProfile.struCameraIpcClassGroupParams[j][t].classBD,outArray);
            CMyJson::ReadIntArrayKey(jsonVal["struCameraIpcClassGroupParams"][j][t],"classBX",intVec,(int*)cnfProfile.struCameraIpcClassGroupParams[j][t].classBX,outArray);
            CMyJson::ReadIntArrayKey(jsonVal["struCameraIpcClassGroupParams"][j][t],"selRerverse",intVec,(int*)cnfProfile.struCameraIpcClassGroupParams[j][t].selRerverse,outArray);
            CMyJson::ReadIntArrayKey(jsonVal["struCameraIpcClassGroupParams"][j][t],"classANDEnable",intVec,(int*)cnfProfile.struCameraIpcClassGroupParams[j][t].classANDEnable,outArray);
            CMyJson::ReadFloatArrayKey(jsonVal["struCameraIpcClassGroupParams"][j][t],"selLimit1",intVec,(float*)cnfProfile.struCameraIpcClassGroupParams[j][t].selLimit1,floatoutArray);
            CMyJson::ReadFloatArrayKey(jsonVal["struCameraIpcClassGroupParams"][j][t],"selLimit2",intVec,(float*)cnfProfile.struCameraIpcClassGroupParams[j][t].selLimit2,floatoutArray);

            CMyJson::ReadIntKey(jsonVal["struCameraIpcClassGroupParams"][j][t],"classCount",cnfProfile.struCameraIpcClassGroupParams[j][t].classCount);
            CMyJson::ReadIntKey(jsonVal["struCameraIpcClassGroupParams"][j][t],"lengthLimit",cnfProfile.struCameraIpcClassGroupParams[j][t].lengthLimit);
        }
    }

    strsNameVec.clear();
    if(JSON_SUCCESS == CMyJson::ReadStringArrayKey(jsonVal, "sD2ImgModeName", strsNameVec)){
        for(int i = 0;i < 4;i++){
            memset(cnfProfile.sD2ImgModeName[i],0,sizeof(cnfProfile.sD2ImgModeName[i]));
            memcpy(cnfProfile.sD2ImgModeName[i],strsNameVec[i].data(),strsNameVec[i].length());
        }
    }

    JsonTostruGroupCtrl(MAX_CTRL_BOARD_PER_INTERFACE,jsonVal["struGroupCtrl"],cnfProfile.struGroupCtrl);
    JsonToStuCameraInfo(MAX_VIEW,jsonVal["stuCameraInfo"],cnfProfile.stuCameraInfo);

    return true;
}

bool JsonDataConvert::StruCnfProfileToJson(const struCnfProfile &cnfProfile, Json::Value &jsonVal)
{
    vector<int> intVec;

    //    QByteArray arrayModelName(cnfProfile.sCurModelName);
    //    QString strModelName = QString::fromUtf8(arrayModelName);
    //    CMyJson::WriteStringKey("sCurModelName",strModelName.toStdString(),jsonVal);

    //    QByteArray arrayModelDate(cnfProfile.sCurModelDate);
    //    QString strModelDate = QString::fromUtf8(arrayModelDate);
    //    CMyJson::WriteStringKey("sCurModelDate",strModelDate.toStdString(),jsonVal);

    intVec.clear();
    intVec.push_back(MAX_LAYER);
    intVec.push_back(MAX_VIEW);
    intVec.push_back(3);
    CMyJson::WriteIntArrayKey("nBackgroundColor",intVec,(int*)cnfProfile.nBackgroundColor,jsonVal);
    CMyJson::WriteIntArrayKey("nBackgroundColorValueLow",intVec,(int*)cnfProfile.nBackgroundColorValueLow,jsonVal);
    CMyJson::WriteIntArrayKey("nBackgroundColorValueUp",intVec,(int*)cnfProfile.nBackgroundColorValueUp,jsonVal);
    CMyJson::WriteIntArrayKey("nBackgroundColorValueVLow",intVec,(int*)cnfProfile.nBackgroundColorValueVLow,jsonVal);
    CMyJson::WriteIntArrayKey("nBackgroundColorValueVUp",intVec,(int*)cnfProfile.nBackgroundColorValueVUp,jsonVal);
    CMyJson::WriteIntArrayKey("nBackgroundColorProLow",intVec,(int*)cnfProfile.nBackgroundColorProLow,jsonVal);
    CMyJson::WriteIntArrayKey("nBackgroundColorProUp",intVec,(int*)cnfProfile.nBackgroundColorProUp,jsonVal);
    CMyJson::WriteIntArrayKey("nBackgroundColorSatUp",intVec,(int*)cnfProfile.nBackgroundColorSatUp,jsonVal);

    intVec.clear();
    intVec.push_back(MAX_LAYER);
    intVec.push_back(MAX_VIEW);
    CMyJson::WriteIntArrayKey("nIpcDelayBufRowCount",intVec,(int*)cnfProfile.nIpcDelayBufRowCount,jsonVal);
    CMyJson::WriteIntArrayKey("nSensorD2Mode",intVec,(int*)cnfProfile.nSensorD2Mode,jsonVal);
    CMyJson::WriteIntArrayKey("nSenseBrightSelection",intVec,(int*)cnfProfile.nSenseBrightSelection,jsonVal);
    CMyJson::WriteIntArrayKey("nSensorRowFre",intVec,(int*)cnfProfile.nSensorRowFre,jsonVal);
    CMyJson::WriteIntArrayKey("nSensResizeMode",intVec,(int*)cnfProfile.nSensResizeMode,jsonVal);
    CMyJson::WriteIntArrayKey("nTimeDivisionScheme",intVec,(int*)cnfProfile.nTimeDivisionScheme,jsonVal);
    CMyJson::WriteIntArrayKey("nViewSensorType",intVec,(int*)cnfProfile.nViewSensorType,jsonVal);
    CMyJson::WriteIntArrayKey("nSensorPixel",intVec,(int*)cnfProfile.nSensorPixel,jsonVal);
    CMyJson::WriteIntArrayKey("nSensorDim",intVec,(int*)cnfProfile.nSensorDim,jsonVal);
    CMyJson::WriteIntArrayKey("nSensorDimD2",intVec,(int*)cnfProfile.nSensorDimD2,jsonVal);
    CMyJson::WriteIntArrayKey("nSensBitWidth",intVec,(int*)cnfProfile.nSensBitWidth,jsonVal);
    CMyJson::WriteIntArrayKey("nSensorDataType",intVec,(int*)cnfProfile.nSensorDataType,jsonVal);
    CMyJson::WriteIntArrayKey("nImageSplice",intVec,(int*)cnfProfile.nImageSplice,jsonVal);
    CMyJson::WriteIntArrayKey("nMirror",intVec,(int*)cnfProfile.nMirror,jsonVal);
    CMyJson::WriteIntArrayKey("nViewChainAddr",intVec,(int*)cnfProfile.nViewChainAddr,jsonVal);
    CMyJson::WriteIntArrayKey("nSensDropLine",intVec,(int*)cnfProfile.nSensDropLine,jsonVal);
    CMyJson::WriteIntArrayKey("nSensCmdSpeed",intVec,(int*)cnfProfile.nSensCmdSpeed,jsonVal);
    CMyJson::WriteIntArrayKey("nRealTimeCorrection",intVec,(int*)cnfProfile.nRealTimeCorrection,jsonVal);
    CMyJson::WriteIntArrayKey("nCameraCompressMode",intVec,(int*)cnfProfile.nCameraCompressMode,jsonVal);
    //CMyJson::WriteIntArrayKey("nCameraBoardMode",intVec,(int*)cnfProfile.nCameraBoardMode,jsonVal);
    intVec.clear();
    intVec.push_back(MAX_LAYER);
    intVec.push_back(MAX_VIEW);
    intVec.push_back(4);
    CMyJson::WriteIntArrayKey("nSensorColorChoose",intVec,(int*)cnfProfile.nSensorColorChoose,jsonVal);

    //    intVec.clear();
    //    intVec.push_back(MAX_LAYER);
    //    intVec.push_back(MAX_VAVLE);
    //    CMyJson::WriteIntArrayKey("nVavleBoardMode",intVec,(int*)cnfProfile.nVavleBoardMode,jsonVal);
    CMyJson::WriteIntKey("nIsCameraConpress",cnfProfile.nIsCameraConpress,jsonVal);
    CMyJson::WriteIntKey("nMatCopySetMode",cnfProfile.nMatCopySetMode,jsonVal);
    intVec.clear();
    intVec.push_back(MAX_LAYER);
    intVec.push_back(MAX_LAMP_BOARD_PER_INTERFACE);
    intVec.push_back(MAX_LAMP_PER_BOARD);
    CMyJson::WriteIntArrayKey("nLampValue",intVec,(int*)cnfProfile.nLampValue,jsonVal);
    CMyJson::WriteIntArrayKey("nLampEnable",intVec,(int*)cnfProfile.nLampEnable,jsonVal);
    CMyJson::WriteIntArrayKey("nLampSeq",intVec,(int*)cnfProfile.nLampSeq,jsonVal);

    intVec.clear();
    intVec.push_back(MAX_LAYER);
    intVec.push_back(MAX_VIEW);
    intVec.push_back(MAX_GROUP_IDENTIFY);
    intVec.push_back(ARITHMETIC_TOTAL);
    CMyJson::WriteIntArrayKey("nGroupArithEnable",intVec,(int*)cnfProfile.nGroupArithEnable,jsonVal);
    CMyJson::WriteIntArrayKey("nGroupArithEdgeEnable",intVec,(int*)cnfProfile.nGroupArithEdgeEnable,jsonVal);

    intVec.clear();
    intVec.push_back(MAX_LAYER);
    intVec.push_back(MAX_VIEW);
    intVec.push_back(MAX_VIEW_UNIT);
    CMyJson::WriteIntArrayKey("nProfileAnaGainRed",intVec,(int*)cnfProfile.nProfileAnaGainRed,jsonVal);
    CMyJson::WriteIntArrayKey("nProfileAnaGainGreen",intVec,(int*)cnfProfile.nProfileAnaGainGreen,jsonVal);
    CMyJson::WriteIntArrayKey("nProfileAnaGainBlue",intVec,(int*)cnfProfile.nProfileAnaGainBlue,jsonVal);
    CMyJson::WriteIntArrayKey("nProfileAnaGainNir",intVec,(int*)cnfProfile.nProfileAnaGainNir,jsonVal);
    CMyJson::WriteIntArrayKey("nProfileGainTargetRed",intVec,(int*)cnfProfile.nProfileGainTargetRed,jsonVal);
    CMyJson::WriteIntArrayKey("nProfileGainTargetGreen",intVec,(int*)cnfProfile.nProfileGainTargetGreen,jsonVal);
    CMyJson::WriteIntArrayKey("nProfileGainTargetBlue",intVec,(int*)cnfProfile.nProfileGainTargetBlue,jsonVal);
    CMyJson::WriteIntArrayKey("nProfileGainTargetNir",intVec,(int*)cnfProfile.nProfileGainTargetNir,jsonVal);
    CMyJson::WriteIntArrayKey("nProfileDigiGainRed",intVec,(int*)cnfProfile.nProfileDigiGainRed,jsonVal);
    CMyJson::WriteIntArrayKey("nProfileDigiGainGreen",intVec,(int*)cnfProfile.nProfileDigiGainGreen,jsonVal);
    CMyJson::WriteIntArrayKey("nProfileDigiGainBlue",intVec,(int*)cnfProfile.nProfileDigiGainBlue,jsonVal);
    CMyJson::WriteIntArrayKey("nProfileDigiGainNir",intVec,(int*)cnfProfile.nProfileDigiGainNir,jsonVal);
    CMyJson::WriteIntArrayKey("nCameraBoardUserMode",intVec,(int*)cnfProfile.nCameraBoardUserMode,jsonVal);

    for(int i = 0;i< MAX_LAYER;i++){
        for(int j = 0;j < MAX_VIEW;j++){
            for(int k = 0; k < MAX_GROUP_IDENTIFY;k++){
                intVec.clear();
                intVec.push_back(2);
                CMyJson::WriteIntArrayKey("nEdgeCut",intVec,(int*)cnfProfile.struGroupIdentify[i][j][k].nEdgeCut,jsonVal["struGroupIdentify"][i][j][k]);
                CMyJson::WriteIntArrayKey("nImpured",intVec,(int*)cnfProfile.struGroupIdentify[i][j][k].nImpured,jsonVal["struGroupIdentify"][i][j][k]);
                CMyJson::WriteIntArrayKey("nColseCal",intVec,(int*)cnfProfile.struGroupIdentify[i][j][k].nColseCal,jsonVal["struGroupIdentify"][i][j][k]);
                CMyJson::WriteIntArrayKey("nReverseSort",intVec,(int*)cnfProfile.struGroupIdentify[i][j][k].nReverseSort,jsonVal["struGroupIdentify"][i][j][k]);
                CMyJson::WriteIntArrayKey("nCenterSize",intVec,(int*)cnfProfile.struGroupIdentify[i][j][k].nCenterSize,jsonVal["struGroupIdentify"][i][j][k]);
                intVec.clear();
                intVec.push_back(MAX_IMPURED_TOTAL);
                CMyJson::WriteIntArrayKey("nImpuredIpc",intVec,(int*)cnfProfile.struGroupIdentify[i][j][k].nImpuredIpc,jsonVal["struGroupIdentify"][i][j][k]);

                CMyJson::WriteIntKey("nEdgeMat",cnfProfile.struGroupIdentify[i][j][k].nEdgeMat,jsonVal["struGroupIdentify"][i][j][k]);
                CMyJson::WriteIntKey("nEjectDelay",cnfProfile.struGroupIdentify[i][j][k].nEjectDelay,jsonVal["struGroupIdentify"][i][j][k]);
                CMyJson::WriteIntKey("nEjectDelay3",cnfProfile.struGroupIdentify[i][j][k].nEjectDelay3,jsonVal["struGroupIdentify"][i][j][k]);
                CMyJson::WriteIntKey("nEjectDelay4",cnfProfile.struGroupIdentify[i][j][k].nEjectDelay4,jsonVal["struGroupIdentify"][i][j][k]);
                CMyJson::WriteIntKey("nEjectDelayShape",cnfProfile.struGroupIdentify[i][j][k].nEjectDelayShape,jsonVal["struGroupIdentify"][i][j][k]);
                CMyJson::WriteIntKey("nEjectDelayModel",cnfProfile.struGroupIdentify[i][j][k].nEjectDelayModel,jsonVal["struGroupIdentify"][i][j][k]);
                CMyJson::WriteIntKey("nBadNum",cnfProfile.struGroupIdentify[i][j][k].nBadNum,jsonVal["struGroupIdentify"][i][j][k]);
                CMyJson::WriteIntKey("nSensUpdateGeneral",cnfProfile.struGroupIdentify[i][j][k].nSensUpdateGeneral,jsonVal["struGroupIdentify"][i][j][k]);
                CMyJson::WriteIntKey("arithRejectRatio",cnfProfile.struGroupIdentify[i][j][k].arithRejectRatio,jsonVal["struGroupIdentify"][i][j][k]);

                struGreyColorToJson(MAX_GREY_COLOR_ARITH_TOTAL,jsonVal["struGroupIdentify"][i][j][k]["struGreyColor"],cnfProfile.struGroupIdentify[i][j][k].struGreyColor);
                struIntelToJson(4,jsonVal["struGroupIdentify"][i][j][k]["struSvm"],cnfProfile.struGroupIdentify[i][j][k].struSvm);
                struShapeIntelToJson(3,jsonVal["struGroupIdentify"][i][j][k]["struShapeSvm"],cnfProfile.struGroupIdentify[i][j][k].struShapeSvm);
                struCircleLongToJson(2,jsonVal["struGroupIdentify"][i][j][k]["struCircleLong"],cnfProfile.struGroupIdentify[i][j][k].struCircleLong);
                struLongShortToJson(2,jsonVal["struGroupIdentify"][i][j][k]["struLongShort"],cnfProfile.struGroupIdentify[i][j][k].struLongShort);
                struBigSmallToJson(2,jsonVal["struGroupIdentify"][i][j][k]["struBigSmall"],cnfProfile.struGroupIdentify[i][j][k].struBigSmall);
                struHsvToJson(3,jsonVal["struGroupIdentify"][i][j][k]["struHsv"],cnfProfile.struGroupIdentify[i][j][k].struHsv);
                struColorSatToJson(jsonVal["struGroupIdentify"][i][j][k]["struColorSat"],cnfProfile.struGroupIdentify[i][j][k].struColorSat);
                struScaleToJson(jsonVal["struGroupIdentify"][i][j][k]["struColorScale"],cnfProfile.struGroupIdentify[i][j][k].struColorScale);
                struColorHueToJson(jsonVal["struGroupIdentify"][i][j][k]["struColorHue"],cnfProfile.struGroupIdentify[i][j][k].struColorHue);
                struMildewToJson(jsonVal["struGroupIdentify"][i][j][k]["struMildew"],cnfProfile.struGroupIdentify[i][j][k].struMildew);
                struOutlineToJson(jsonVal["struGroupIdentify"][i][j][k]["struOutline"],cnfProfile.struGroupIdentify[i][j][k].struOutline);
                struLineToJson(jsonVal["struGroupIdentify"][i][j][k]["struLine"],cnfProfile.struGroupIdentify[i][j][k].struLine);
                struBudToJson(2,jsonVal["struGroupIdentify"][i][j][k]["struBud"],cnfProfile.struGroupIdentify[i][j][k].struBud);
                struRoundToJson(2,jsonVal["struGroupIdentify"][i][j][k]["struRound"],cnfProfile.struGroupIdentify[i][j][k].struRound);
                struColorValueToJson(jsonVal["struGroupIdentify"][i][j][k]["struColorValue"], cnfProfile.struGroupIdentify[i][j][k].struColorValue);
                struSplMildewToJson(jsonVal["struGroupIdentify"][i][j][k]["struSplMildew"], cnfProfile.struGroupIdentify[i][j][k].struSplMildew);
                struWhitePeanutToJson(jsonVal["struGroupIdentify"][i][j][k]["struWhitePeanut"], cnfProfile.struGroupIdentify[i][j][k].struWhitePeanut);
                struPepperHandleToJson(jsonVal["struGroupIdentify"][i][j][k]["struPepperHandle"], cnfProfile.struGroupIdentify[i][j][k].struPepperHandle);
                struSuppressLargeToJson(3,jsonVal["struGroupIdentify"][i][j][k]["struSuppressLarge"],cnfProfile.struGroupIdentify[i][j][k].struSuppressLarge);
                struWhearSproutToJson(jsonVal["struGroupIdentify"][i][j][k]["struWheatSprout"], cnfProfile.struGroupIdentify[i][j][k].struWheatSprout);
                struTeaColorToJson(MAX_TEA_ARITH_TOTAL,jsonVal["struGroupIdentify"][i][j][k]["struTeaColor"],cnfProfile.struGroupIdentify[i][j][k].struTeaColor);
            }
            for(int k = 0; k < MAX_VIEW_UNIT;k++){
                for(int l = 0;l < 2;l++){
                    intVec.clear();
                    intVec.push_back(MAX_GREY_COLOR_ARITH_TOTAL);
                    CMyJson::WriteIntArrayKey("greyColor",intVec,(int*)cnfProfile.struUnitSens[i][j][k][l].greyColor,jsonVal["struUnitSens"][i][j][k][l]);
                    intVec.clear();
                    intVec.push_back(3);
                    CMyJson::WriteIntArrayKey("svm",intVec,(int*)cnfProfile.struUnitSens[i][j][k][l].svm,jsonVal["struUnitSens"][i][j][k][l]);
                    CMyJson::WriteIntArrayKey("suppressLarge",intVec,(int*)cnfProfile.struUnitSens[i][j][k][l].suppressLarge,jsonVal["struUnitSens"][i][j][k][l]);
                    CMyJson::WriteIntArrayKey("hsv",intVec,(int*)cnfProfile.struUnitSens[i][j][k][l].hsv, jsonVal["struUnitSens"][i][j][k][l]);
                    intVec.clear();
                    intVec.push_back(2);
                    CMyJson::WriteIntArrayKey("bigSmall",intVec,(int*)cnfProfile.struUnitSens[i][j][k][l].bigSmall,jsonVal["struUnitSens"][i][j][k][l]);
                    CMyJson::WriteIntArrayKey("circleLong",intVec,(int*)cnfProfile.struUnitSens[i][j][k][l].circleLong,jsonVal["struUnitSens"][i][j][k][l]);
                    CMyJson::WriteIntArrayKey("longShort",intVec,(int*)cnfProfile.struUnitSens[i][j][k][l].longShort,jsonVal["struUnitSens"][i][j][k][l]);
                    CMyJson::WriteIntKey("sat",cnfProfile.struUnitSens[i][j][k][l].sat,jsonVal["struUnitSens"][i][j][k][l]);
                    CMyJson::WriteIntKey("hueH",cnfProfile.struUnitSens[i][j][k][l].hueH,jsonVal["struUnitSens"][i][j][k][l]);
                    CMyJson::WriteIntKey("hueL",cnfProfile.struUnitSens[i][j][k][l].hueL,jsonVal["struUnitSens"][i][j][k][l]);
                    CMyJson::WriteIntKey("scale",cnfProfile.struUnitSens[i][j][k][l].scale,jsonVal["struUnitSens"][i][j][k][l]);
                    CMyJson::WriteIntKey("mildew",cnfProfile.struUnitSens[i][j][k][l].mildew,jsonVal["struUnitSens"][i][j][k][l]);
                    CMyJson::WriteIntKey("outline",cnfProfile.struUnitSens[i][j][k][l].outline,jsonVal["struUnitSens"][i][j][k][l]);
                    CMyJson::WriteIntKey("line",cnfProfile.struUnitSens[i][j][k][l].line,jsonVal["struUnitSens"][i][j][k][l]);
                    CMyJson::WriteIntArrayKey("bud",intVec,(int*)cnfProfile.struUnitSens[i][j][k][l].bud,jsonVal["struUnitSens"][i][j][k][l]);
                    CMyJson::WriteIntArrayKey("round",intVec,(int*)cnfProfile.struUnitSens[i][j][k][l].round,jsonVal["struUnitSens"][i][j][k][l]);
                    CMyJson::WriteIntKey("value", cnfProfile.struUnitSens[i][j][k][l].value, jsonVal["struUnitSens"][i][j][k][l]);

                    CMyJson::WriteIntKey("splMildew",cnfProfile.struUnitSens[i][j][k][l].splMildew, jsonVal["struUnitSens"][i][j][k][l]);
                    CMyJson::WriteIntKey("whitePeanut",cnfProfile.struUnitSens[i][j][k][l].whitePeanut, jsonVal["struUnitSens"][i][j][k][l]);
                }
            }
        }

        for(int j= 0;j < MAX_VAVLE_BOARD;j++){
            for(int k = 0;k < MAX_GROUP_TICK;k++){
                CMyJson::WriteIntKey("nEjectDuration",cnfProfile.struGroupTick[i][j][k].nEjectDuration,jsonVal["struGroupTick"][i][j][k]);
                CMyJson::WriteIntKey("nEjectDuration2",cnfProfile.struGroupTick[i][j][k].nEjectDuration2,jsonVal["struGroupTick"][i][j][k]);
                CMyJson::WriteIntKey("nFrontRearMode",cnfProfile.struGroupTick[i][j][k].nFrontRearMode,jsonVal["struGroupTick"][i][j][k]);
                CMyJson::WriteIntKey("nEjwidth",cnfProfile.struGroupTick[i][j][k].nEjwidth,jsonVal["struGroupTick"][i][j][k]);
                CMyJson::WriteIntKey("nWaitTime",cnfProfile.struGroupTick[i][j][k].nWaitTime,jsonVal["struGroupTick"][i][j][k]);
                CMyJson::WriteIntKey("nTailTime",cnfProfile.struGroupTick[i][j][k].nTailTime,jsonVal["struGroupTick"][i][j][k]);
                CMyJson::WriteIntKey("nEjectTimesLimit",cnfProfile.struGroupTick[i][j][k].nEjectTimesLimit,jsonVal["struGroupTick"][i][j][k]);
            }
        }
    }
    for(int i = 0;i< MAX_LAYER;i++){
        for(int j = 0;j < MAX_GROUP_TICK;j++){
            CMyJson::WriteIntKey("bEnable",cnfProfile.nAutoFeed[i][j].bEnable,jsonVal["nAutoFeed"][i][j]);
            CMyJson::WriteIntKey("target_feeder",cnfProfile.nAutoFeed[i][j].target_feeder,jsonVal["nAutoFeed"][i][j]);
            CMyJson::WriteIntKey("start_feeder",cnfProfile.nAutoFeed[i][j].start_feeder,jsonVal["nAutoFeed"][i][j]);
            CMyJson::WriteIntKey("stop_feeder",cnfProfile.nAutoFeed[i][j].stop_feeder,jsonVal["nAutoFeed"][i][j]);
            CMyJson::WriteIntKey("stop_duration",cnfProfile.nAutoFeed[i][j].stop_duration,jsonVal["nAutoFeed"][i][j]);
            CMyJson::WriteIntKey("ajust_feeder_range",cnfProfile.nAutoFeed[i][j].ajust_feeder_range,jsonVal["nAutoFeed"][i][j]);
        }
    }
    CMyJson::WriteIntKey("sStaticsGetDuration",cnfProfile.sStaticsGetDuration,jsonVal);

    //    intVec.clear();
    //    intVec.push_back(MAX_IPC_NUM);
    //    intVec.push_back(MAX_IPC_MODEL_CLASS);

    //    vector<string> strsNameVec;
    //    strsNameVec.clear();
    //    for(int i = 0;i< MAX_VIEW;i++)
    //    {
    //        string strsName = QString::fromUtf8(cnfProfile.sCurModelName[i]).toStdString();
    //        strsNameVec.push_back(strsName);
    //    }
    //    CMyJson::WriteStringArrayKey("sCurModelName",strsNameVec,jsonVal);

    //    strsNameVec.clear();
    //    for(int i = 0;i< MAX_VIEW;i++)
    //    {
    //        string strsName = QString::fromUtf8(cnfProfile.sCurModelDate[i]).toStdString();
    //        strsNameVec.push_back(strsName);
    //    }
    //    CMyJson::WriteStringArrayKey("sCurModelDate",strsNameVec,jsonVal);

    vector<string> strsNameVec;
    for(int i = 0; i < MAX_VIEW; i++){
        for(int j = 0; j < MAX_GROUP_IDENTIFY; j++){
            string strsName = QString::fromUtf8(cnfProfile.sCurModelName[i][j]).toStdString();
            strsNameVec.push_back(strsName);
        }
    }
    CMyJson::WriteStringArrayKey("sCurModelName",strsNameVec,jsonVal);

    strsNameVec.clear();
    for(int i = 0; i < MAX_VIEW; i++){
        for(int j = 0; j < MAX_GROUP_IDENTIFY; j++){
            string strsName = QString::fromUtf8(cnfProfile.sCurModelDate[i][j]).toStdString();
            strsNameVec.push_back(strsName);
        }
    }
    CMyJson::WriteStringArrayKey("sCurModelDate",strsNameVec,jsonVal);

    strsNameVec.clear();
    for(int i=0;i < MAX_ONEPAGE_IPCMODEL_CLASS;i++){
        string strsName = QString::fromUtf8(cnfProfile.sCurBigstatisname[i]).toStdString();
        strsNameVec.push_back(strsName);
    }
    CMyJson::WriteStringArrayKey("sCurBigstatisname",strsNameVec,jsonVal);

    for(int j = 0;j< MAX_VIEW;j++)
    {
        for(int t = 0;t< MAX_GROUP_IDENTIFY;t++)
        {
            intVec.clear();
            intVec.push_back(MAX_IPC_MODEL_CLASS);

            strsNameVec.clear();
            for(int k=0;k < MAX_IPC_MODEL_CLASS;k++){
                string strsName = QString::fromUtf8(cnfProfile.struCameraIpcClassGroupParams[j][t].className[k]).toStdString();
                strsNameVec.push_back(strsName);
            }
            CMyJson::WriteStringArrayKey("className",strsNameVec,jsonVal["struCameraIpcClassGroupParams"][j][t]);

            CMyJson::WriteIntArrayKey("classEnable",intVec,(int*)cnfProfile.struCameraIpcClassGroupParams[j][t].classEnable,jsonVal["struCameraIpcClassGroupParams"][j][t]);
            CMyJson::WriteIntArrayKey("classSens",intVec,(int*)cnfProfile.struCameraIpcClassGroupParams[j][t].classSens,jsonVal["struCameraIpcClassGroupParams"][j][t]);
            CMyJson::WriteIntArrayKey("classThreshold",intVec,(int*)cnfProfile.struCameraIpcClassGroupParams[j][t].classThreshold,jsonVal["struCameraIpcClassGroupParams"][j][t]);
            CMyJson::WriteIntArrayKey("classLabelBox",intVec,(int*)cnfProfile.struCameraIpcClassGroupParams[j][t].classLabelBox,jsonVal["struCameraIpcClassGroupParams"][j][t]);
            CMyJson::WriteIntArrayKey("classLabelBox2",intVec,(int*)cnfProfile.struCameraIpcClassGroupParams[j][t].classLabelBox2,jsonVal["struCameraIpcClassGroupParams"][j][t]);
            CMyJson::WriteIntArrayKey("classTickMode",intVec,(int*)cnfProfile.struCameraIpcClassGroupParams[j][t].classTickMode,jsonVal["struCameraIpcClassGroupParams"][j][t]);
            CMyJson::WriteIntArrayKey("classFixMode",intVec,(int*)cnfProfile.struCameraIpcClassGroupParams[j][t].classFixMode,jsonVal["struCameraIpcClassGroupParams"][j][t]);
            CMyJson::WriteIntArrayKey("classFixMode2",intVec,(int*)cnfProfile.struCameraIpcClassGroupParams[j][t].classFixMode2,jsonVal["struCameraIpcClassGroupParams"][j][t]);
            CMyJson::WriteIntArrayKey("classMode",intVec,(int*)cnfProfile.struCameraIpcClassGroupParams[j][t].classMode,jsonVal["struCameraIpcClassGroupParams"][j][t]);
            CMyJson::WriteIntArrayKey("classDelayMode",intVec,(int*)cnfProfile.struCameraIpcClassGroupParams[j][t].classDelayMode,jsonVal["struCameraIpcClassGroupParams"][j][t]);
            CMyJson::WriteIntArrayKey("classHWRatio",intVec,(int*)cnfProfile.struCameraIpcClassGroupParams[j][t].classHWRatio,jsonVal["struCameraIpcClassGroupParams"][j][t]);
            CMyJson::WriteIntArrayKey("classPosAdjust",intVec,(int*)cnfProfile.struCameraIpcClassGroupParams[j][t].classPosAdjust,jsonVal["struCameraIpcClassGroupParams"][j][t]);
            CMyJson::WriteIntArrayKey("classBD",intVec,(int*)cnfProfile.struCameraIpcClassGroupParams[j][t].classBD,jsonVal["struCameraIpcClassGroupParams"][j][t]);
            CMyJson::WriteIntArrayKey("classBX",intVec,(int*)cnfProfile.struCameraIpcClassGroupParams[j][t].classBX,jsonVal["struCameraIpcClassGroupParams"][j][t]);
            CMyJson::WriteIntArrayKey("selRerverse",intVec,(int*)cnfProfile.struCameraIpcClassGroupParams[j][t].selRerverse,jsonVal["struCameraIpcClassGroupParams"][j][t]);
            CMyJson::WriteIntArrayKey("classANDEnable",intVec,(int*)cnfProfile.struCameraIpcClassGroupParams[j][t].classANDEnable,jsonVal["struCameraIpcClassGroupParams"][j][t]);
            CMyJson::WriteFloatArrayKey("selLimit1",intVec,(float*)cnfProfile.struCameraIpcClassGroupParams[j][t].selLimit1,jsonVal["struCameraIpcClassGroupParams"][j][t]);
            CMyJson::WriteFloatArrayKey("selLimit2",intVec,(float*)cnfProfile.struCameraIpcClassGroupParams[j][t].selLimit2,jsonVal["struCameraIpcClassGroupParams"][j][t]);

            CMyJson::WriteIntKey("classCount",cnfProfile.struCameraIpcClassGroupParams[j][t].classCount,jsonVal["struCameraIpcClassGroupParams"][j][t]);
            CMyJson::WriteIntKey("lengthLimit",cnfProfile.struCameraIpcClassGroupParams[j][t].lengthLimit,jsonVal["struCameraIpcClassGroupParams"][j][t]);
        }
    }


    CMyJson::WriteIntKey("n_coff",cnfProfile.n_coff,jsonVal);
    CMyJson::WriteIntKey("bColorRestoreEnable",cnfProfile.bColorRestoreEnable,jsonVal);
    CMyJson::WriteIntKey("nImpuredIpcCount",cnfProfile.nImpuredIpcCount,jsonVal);
    CMyJson::WriteIntKey("nEjectDurationIpcCount",cnfProfile.nEjectDurationIpcCount,jsonVal);
    CMyJson::WriteFloatKey("ipcBadPointscoff",cnfProfile.ipcBadPointscoff,jsonVal);

    CMyJson::WriteIntKey("nHasSaveModel",cnfProfile.nHasSaveModel,jsonVal);
    CMyJson::WriteIntKey("bCurCal",cnfProfile.bCurCal,jsonVal);
    CMyJson::WriteIntKey("nIpcBadMatAlarmCount",cnfProfile.nIpcBadMatAlarmCount,jsonVal);
    CMyJson::WriteIntKey("nIpcHasMatPixelPercert",cnfProfile.nIpcHasMatPixelPercert,jsonVal);
    CMyJson::WriteIntKey("nDualLayerModelSame",cnfProfile.nDualLayerModelSame,jsonVal);
    CMyJson::WriteIntKey("nIpcCameraClassSameMode",cnfProfile.nIpcCameraClassSameMode,jsonVal);
    CMyJson::WriteIntKey("bCaliGainEnable",cnfProfile.bCaliGainEnable,jsonVal);
    CMyJson::WriteIntKey("bCaliBiasEnable",cnfProfile.bCaliBiasEnable,jsonVal);
    CMyJson::WriteIntKey("nIpcStatisticsBigclass",cnfProfile.nIpcStatisticsBigclass,jsonVal);

    intVec.clear();
    intVec.push_back(MAX_IPC_MODEL_CLASS);
    CMyJson::WriteIntArrayKey("nIpcStatisticsclassId",intVec,(int*)cnfProfile.nIpcStatisticsclassId,jsonVal);
    intVec.clear();
    intVec.push_back(MAX_IPC_MODEL_CLASS);
    CMyJson::WriteIntArrayKey("nIpcStatisticsbigclassCoeff",intVec,(int*)cnfProfile.nIpcStatisticsbigclassCoeff,jsonVal);
    intVec.clear();
    intVec.push_back(MAX_IPC_MODEL_CLASS);
    CMyJson::WriteIntArrayKey("bstaticClassAlarm",intVec,(int*)cnfProfile.bstaticClassAlarm,jsonVal);
    intVec.clear();
    intVec.push_back(MAX_VIEW);
    //CMyJson::WriteIntArrayKey("ipcModelClassCount",intVec,(int*)cnfProfile.ipcModelClassCount,jsonVal);
    CMyJson::WriteIntArrayKey("nIpcRoiHeight",intVec,(int*)cnfProfile.nIpcRoiHeight,jsonVal);
    CMyJson::WriteIntArrayKey("nIpcRoiWidth",intVec,(int*)cnfProfile.nIpcRoiWidth,jsonVal);
    CMyJson::WriteIntArrayKey("nIpcRoiRepeat",intVec,(int*)cnfProfile.nIpcRoiRepeat,jsonVal);
    CMyJson::WriteIntArrayKey("nIpcRoiRepeat2",intVec,(int*)cnfProfile.nIpcRoiRepeat2,jsonVal);
    CMyJson::WriteIntArrayKey("imageEnahnce",intVec,(int*)cnfProfile.imageEnahnce,jsonVal);
    CMyJson::WriteFloatArrayKey("nIpcRoiRepeatf",intVec,(float*)cnfProfile.nIpcRoiRepeatf,jsonVal);
    intVec.clear();
    intVec.push_back(MAX_VIEW);
    intVec.push_back(MAX_GROUP_IDENTIFY);
    CMyJson::WriteIntArrayKey("ipcModelClassCount",intVec,(int*)cnfProfile.ipcModelClassCount,jsonVal);

    strsNameVec.clear();
    for(int i=0;i < 4;i++){
        string strsName = QString::fromUtf8(cnfProfile.sD2ImgModeName[i]).toStdString();
        strsNameVec.push_back(strsName);
    }
    CMyJson::WriteStringArrayKey("sD2ImgModeName",strsNameVec,jsonVal);

    struGroupCtrlToJson(MAX_CTRL_BOARD_PER_INTERFACE,jsonVal["struGroupCtrl"],cnfProfile.struGroupCtrl);
    stuCameraInfoToJson(MAX_VIEW,cnfProfile.stuCameraInfo,jsonVal["stuCameraInfo"]);

    return true;
}

bool JsonDataConvert::profileIndexToJson(int arraySize, const stru_Profile_Index *profile, Json::Value &jsonVal)
{
    for(int i = 0; i < arraySize; i++)
    {
        QByteArray arrayProfile(profile[i].sProfileName);
        QString strsProfileName = QString::fromUtf8(arrayProfile);
        CMyJson::WriteStringKey("sProfileName",strsProfileName.toStdString(),jsonVal[i]);

        QByteArray arrayMaterial(profile[i].sMaterialName);
        QString strsMaterialName = QString::fromUtf8(arrayMaterial);
        CMyJson::WriteStringKey("sMaterialName",strsMaterialName.toStdString(),jsonVal[i]);

        CMyJson::WriteIntKey("nProfileLock",profile[i].nProfileLock,jsonVal[i]);
    }
    return true;
}

bool JsonDataConvert::layerInfoToJson(int arraySize, const stru_layer_info *layerinfoArr, Json::Value &jsonVal)
{
    for(int i = 0; i < arraySize; i++)
    {
        CMyJson::WriteIntKey("nLayerIndex",layerinfoArr[i].nLayerIndex,jsonVal[i]);
        CMyJson::WriteIntKey("nViewChainTotal",layerinfoArr[i].nViewChainTotal,jsonVal[i]);

        CMyJson::WriteIntKey("nSplice",layerinfoArr[i].nSplice,jsonVal[i]);
        CMyJson::WriteIntKey("nOnlyDispFirst",layerinfoArr[i].nOnlyDispFirst,jsonVal[i]);
        CMyJson::WriteIntKey("nSpliceChuteAddr",layerinfoArr[i].nSpliceChuteAddr,jsonVal[i]);
        CMyJson::WriteIntKey("nSpliceType",layerinfoArr[i].nSpliceType,jsonVal[i]);
        vector<int> intVec;
        intVec.push_back(MAX_VIEW);

        CMyJson::WriteIntArrayKey("nViewIsUsed",intVec,(int *)layerinfoArr[i].nViewIsUsed,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nViewId",intVec,(int *)layerinfoArr[i].nViewId,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nViewBoardType",intVec,(int *)layerinfoArr[i].nViewBoardType,jsonVal[i]);
        //CMyJson::WriteIntArrayKey("nViewSensorType",intVec,(int *)layerinfoArr[i].nViewSensorType,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nViewUnitTotal",intVec,(int *)layerinfoArr[i].nViewUnitTotal,jsonVal[i]);


        intVec.clear();
        intVec.push_back(MAX_VAVLE_TYPE);
        CMyJson::WriteIntArrayKey("nVavleType",intVec,(int *)layerinfoArr[i].nVavleType,jsonVal[i]);

        intVec.clear();
        intVec.push_back(MAX_VAVLE);
        CMyJson::WriteIntArrayKey("nVavleBoardType",intVec,(int *)layerinfoArr[i].nVavleBoardType,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nVavleBoardTotal",intVec,(int *)layerinfoArr[i].nVavleBoardTotal,jsonVal[i]);
        CMyJson::WriteIntKey("nVavleChainTotal",layerinfoArr[i].nVavleChainTotal,jsonVal[i]);

        CMyJson::WriteIntKey("nEjectorsPerChute",layerinfoArr[i].nEjectorsPerChute,jsonVal[i]);
        CMyJson::WriteIntKey("nEjectorsPosition",layerinfoArr[i].nEjectorsPosition,jsonVal[i]);

        stuViewInfoToJson(MAX_VIEW,layerinfoArr[i].stuViewInfo,jsonVal[i]["stuViewInfo"]);
        stuVavleInfoToJson(MAX_VAVLE,layerinfoArr[i].stuVavleInfo,jsonVal[i]["stuVavleInfo"]);
        stuCameraInfoToJson(MAX_VIEW,layerinfoArr[i].stuCameraInfo,jsonVal[i]["stuCameraInfo"]);

    }
    return true;
}

bool JsonDataConvert::stuViewInfoToJson(int arraySize, const stru_view_info *struViewInfoArr, Json::Value &jsonVal)
{
    for(int i = 0;i< arraySize;i++){
        CMyJson::WriteIntKey("nLayerIndex",struViewInfoArr[i].nLayerIndex,jsonVal[i]);
        CMyJson::WriteIntKey("nNameIndex",struViewInfoArr[i].nNameIndex,jsonVal[i]);

        vector<int> intVec;
        intVec.push_back(MAX_VIEW_UNIT);
        CMyJson::WriteIntArrayKey("nViewUnitId",intVec,(int*)struViewInfoArr[i].nViewUnitId,jsonVal[i]);
        CMyJson::WriteIntArrayKey("bViewSplice",intVec,(int*)struViewInfoArr[i].bViewSplice,jsonVal[i]);
        CMyJson::WriteIntKey("nEjectorsPerCamera",struViewInfoArr[i].nEjectorsPerCamera,jsonVal[i]);

        intVec.clear();
        intVec.push_back(4);
        // CMyJson::WriteIntArrayKey("nSensorColorChoose",intVec,(int*)struViewInfoArr[i].nSensorColorChoose,jsonVal[i]);
        //CMyJson::WriteIntKey("nSensorDim",struViewInfoArr[i].nSensorDim,jsonVal[i]);
        //CMyJson::WriteIntKey("nSensorDimD2",struViewInfoArr[i].nSensorDimD2,jsonVal[i]);
        CMyJson::WriteIntKey("nSensorD2Mode",struViewInfoArr[i].nSensorD2Mode,jsonVal[i]);
        //CMyJson::WriteIntKey("nSensBitWidth",struViewInfoArr[i].nSensBitWidth,jsonVal[i]);
        //CMyJson::WriteIntKey("nSensorRowFre",struViewInfoArr[i].nSensorRowFre,jsonVal[i]);
        //CMyJson::WriteIntKey("nSensorPixel",struViewInfoArr[i].nSensorPixel,jsonVal[i]);
        CMyJson::WriteIntKey("rowColQuo",struViewInfoArr[i].rowColQuo,jsonVal[i]);
        //  CMyJson::WriteIntKey("nImageSplice",struViewInfoArr[i].nImageSplice,jsonVal[i]);
        //CMyJson::WriteIntKey("nSensorDataType",struViewInfoArr[i].nSensorDataType,jsonVal[i]);
        //CMyJson::WriteIntKey("nMirror",struViewInfoArr[i].nMirror,jsonVal[i]);
        //CMyJson::WriteIntKey("nSenseBrightSelection",struViewInfoArr[i].nSenseBrightSelection,jsonVal[i]);
        //CMyJson::WriteIntKey("nSensDropLine",struViewInfoArr[i].nSensDropLine,jsonVal[i]);
        //CMyJson::WriteIntKey("nSensCmdSpeed",struViewInfoArr[i].nSensCmdSpeed,jsonVal[i]);
        CMyJson::WriteIntKey("nSensResizeMode",struViewInfoArr[i].nSensResizeMode,jsonVal[i]);
        //CMyJson::WriteIntKey("nRealTimeCorrection",struViewInfoArr[i].nRealTimeCorrection,jsonVal[i]);

        CMyJson::WriteIntKey("nBrightRefRed",struViewInfoArr[i].nBrightRefRed,jsonVal[i]);
        CMyJson::WriteIntKey("nBrightRefGreen",struViewInfoArr[i].nBrightRefGreen,jsonVal[i]);
        CMyJson::WriteIntKey("nBrightRefBlue",struViewInfoArr[i].nBrightRefBlue,jsonVal[i]);
        CMyJson::WriteIntKey("nBrightRefNir",struViewInfoArr[i].nBrightRefNir,jsonVal[i]);
        CMyJson::WriteIntKey("nIdentifyGroupTotal",struViewInfoArr[i].nIdentifyGroupTotal,jsonVal[i]);

        intVec.clear();
        intVec.push_back(ARITHMETIC_TOTAL);
        CMyJson::WriteIntArrayKey("nViewArithVisiable",intVec,(int *)struViewInfoArr[i].nViewArithVisiable,jsonVal[i]);
        stuIdenGupInfoToJson(MAX_GROUP_IDENTIFY,struViewInfoArr[i].stuIdenGupInfo,jsonVal[i]["stuIdenGupInfo"]);
    }
    return true;
}

bool JsonDataConvert::stuVavleInfoToJson(int arraySize, const stru_vavle_info *struValveInfoArr, Json::Value &jsonVal)
{
    vector<int> intVec;
    for(int i = 0; i < arraySize; i++)
    {
        CMyJson::WriteIntKey("nLayerIndex",struValveInfoArr[i].nLayerIndex,jsonVal[i]);
        CMyJson::WriteIntKey("nNameIndex",struValveInfoArr[i].nNameIndex,jsonVal[i]);
        CMyJson::WriteIntKey("nViewTotal",struValveInfoArr[i].nViewTotal,jsonVal[i]);

        intVec.clear();
        intVec.push_back(MAX_VAVLE_BOARD);
        CMyJson::WriteIntArrayKey("nVavleBoardId",intVec,(int*)struValveInfoArr[i].nVavleBoardId,jsonVal[i]);

        intVec.clear();
        intVec.push_back(MAX_VIEW);
        CMyJson::WriteIntArrayKey("nVavleViewBoardType",intVec,(int*)struValveInfoArr[i].nVavleViewBoardType,jsonVal[i]);
        //        CMyJson::WriteIntKey("nEjectorsPerVavleBoard",struValveInfoArr[i].nEjectorsPerVavleBoard,jsonVal[i]);

        CMyJson::WriteIntKey("nTickGroupTotal",struValveInfoArr[i].nTickGroupTotal,jsonVal[i]);
        stuVavleGroupInfoToJson(MAX_GROUP_TICK,struValveInfoArr[i].stuTickGupInfo,jsonVal[i]["stuIdenGupInfo"]);
    }
    return true;
}

bool JsonDataConvert::stuLampCfgToJson(const stru_layer_lamp_cfg &struLayerLampInfoArr, Json::Value &jsonVal)
{
    vector<int> intVec;

    CMyJson::WriteIntKey("nLampBoardTotal", struLayerLampInfoArr.nLampBoardTotal,jsonVal);
    CMyJson::WriteFloatKey("nLampValueQuo", struLayerLampInfoArr.nLampValueQuo,jsonVal);
    intVec.clear();
    intVec.push_back(MAX_LAMP_BOARD_PER_INTERFACE);
    intVec.push_back(MAX_LAMP_PER_BOARD);
    CMyJson::WriteIntArrayKey("nLampPortEnable",intVec,(int*)struLayerLampInfoArr.nLampPortEnable,jsonVal);
    CMyJson::WriteIntArrayKey("nLampPortType",intVec,(int*)struLayerLampInfoArr.nLampPortType,jsonVal);
    CMyJson::WriteIntArrayKey("nLampLayerAddr",intVec,(int*)struLayerLampInfoArr.nLampLayerAddr,jsonVal);
    CMyJson::WriteIntArrayKey("nLampViewAddr",intVec,(int*)struLayerLampInfoArr.nLampViewAddr,jsonVal);
    CMyJson::WriteIntArrayKey("nLampBoardLimit",intVec,(int*)struLayerLampInfoArr.nLampBoardLimit,jsonVal);
    CMyJson::WriteFloatArrayKey("fLampBoardQuo", intVec, (float*)struLayerLampInfoArr.fLampBoardQuo,jsonVal);
    intVec.push_back(MAX_CALIBRATE_SETS);
    CMyJson::WriteIntArrayKey("nLampValue", intVec, (int*)struLayerLampInfoArr.nLampValue,jsonVal);
    CMyJson::WriteIntArrayKey("nLampEnable", intVec, (int*)struLayerLampInfoArr.nLampEnable,jsonVal);
    CMyJson::WriteIntArrayKey("nLampSeq", intVec, (int*)struLayerLampInfoArr.nLampSeq,jsonVal);

    vector<string> strsNameVec;
    for(int i = 0; i < MAX_LAMP_BOARD_PER_INTERFACE; i++){
        for(int j = 0; j < MAX_LAMP_PER_BOARD; j++){
            string strsName = QString::fromUtf8(struLayerLampInfoArr.sLampName[i][j]).toStdString();
            strsNameVec.push_back(strsName);
        }
    }
    CMyJson::WriteStringArrayKey("sLampName",strsNameVec,jsonVal);
    return true;
}

bool JsonDataConvert::stuAlarmCfgToJson(const stu_alarm_cfg &struAlarmCfg, Json::Value &jsonVal)
{
    vector<string> strsNameVec;
    for(int i=0;i < CTRL_ALARM_MAX_FEED_SENSOR;i++){
        string strsName = QString::fromUtf8(struAlarmCfg.nFeederLevelAlarmName[i]).toStdString();
        strsNameVec.push_back(strsName);
    }
    CMyJson::WriteStringArrayKey("nFeederLevelAlarmName",strsNameVec,jsonVal);
    strsNameVec.clear();
    for(int i=0;i < CTRL_ALARM_MAX_TEMP_SENSOR;i++){
        string strsName = QString::fromUtf8(struAlarmCfg.nTemptureAlarmName[i]).toStdString();
        strsNameVec.push_back(strsName);
    }
    CMyJson::WriteStringArrayKey("nTemptureAlarmName",strsNameVec,jsonVal);
    CMyJson::WriteIntKey("nAirPressureAlarmType", struAlarmCfg.nAirPressureAlarmType,jsonVal);
    CMyJson::WriteIntKey("nLightVoiceAlarmType", struAlarmCfg.nLightVoiceAlarmType,jsonVal);
    CMyJson::WriteIntKey("nFeederLevelAlarmDealMode", struAlarmCfg.nFeederLevelAlarmDealMode,jsonVal);
    CMyJson::WriteIntKey("nTemptureAlarmDealMode", struAlarmCfg.nTemptureAlarmDealMode,jsonVal);

    vector<int> intVec;
    intVec.clear();
    intVec.push_back(CTRL_ALARM_MAX_FEED_SENSOR);
    CMyJson::WriteIntArrayKey("nFeederLevelAlarmEnable",intVec,(int*)struAlarmCfg.nFeederLevelAlarmEnable,jsonVal);
    CMyJson::WriteIntArrayKey("nFeederLevelAlarmPosi",intVec,(int*)struAlarmCfg.nFeederLevelAlarmPosi,jsonVal);
    intVec.clear();
    intVec.push_back(CTRL_ALARM_MAX_TEMP_SENSOR);
    CMyJson::WriteIntArrayKey("nTemptureAlarmEnable",intVec,(int*)struAlarmCfg.nTemptureAlarmEnable,jsonVal);
    CMyJson::WriteIntArrayKey("nTemptureAlarmLimit",intVec,(int*)struAlarmCfg.nTemptureAlarmLimit,jsonVal);
    return true;
}

bool JsonDataConvert::stuIntelWipeToJson(const stu_intel_wipe_cfg &struIntelWipeArr,Json::Value &jsonVal OUT)
{
    CMyJson::WriteIntKey("feederOnffCount", struIntelWipeArr.feederOnffCount,jsonVal);
    CMyJson::WriteIntKey("wiperDurationPerChute", struIntelWipeArr.wiperDurationPerChute,jsonVal);
    CMyJson::WriteIntKey("wiperWidth", struIntelWipeArr.wiperWidth,jsonVal);
    CMyJson::WriteIntKey("wipeTimeout", struIntelWipeArr.wipeTimeout,jsonVal);
    CMyJson::WriteIntKey("wiperConfirmThread", struIntelWipeArr.wiperConfirmThread,jsonVal);

    return true;
}

bool JsonDataConvert::stuCameraInfoToJson(int arraySize, const stru_view_camera_Info *stuCameraInfoArr, Json::Value &jsonVal)
{
    vector<int> intVec;
    for(int i = 0; i < arraySize; i++)
    {
        CMyJson::WriteIntKey("nCalibratePixelRed",stuCameraInfoArr[i].nCalibratePixelRed,jsonVal[i]);
        // CMyJson::WriteIntKey("nCalibratePixelRedVert",stuCameraInfoArr[i].nCalibratePixelRedVert,jsonVal[i]);
        CMyJson::WriteIntKey("nCalibratePixelGreen",stuCameraInfoArr[i].nCalibratePixelGreen,jsonVal[i]);
        // CMyJson::WriteIntKey("nCalibratePixelGreenVert",stuCameraInfoArr[i].nCalibratePixelGreenVert,jsonVal[i]);
        CMyJson::WriteIntKey("nCalibratePixelBlue",stuCameraInfoArr[i].nCalibratePixelBlue,jsonVal[i]);
        //CMyJson::WriteIntKey("nCalibratePixelBlueVert",stuCameraInfoArr[i].nCalibratePixelBlueVert,jsonVal[i]);
        CMyJson::WriteIntKey("nCalibratePixelNir",stuCameraInfoArr[i].nCalibratePixelNir,jsonVal[i]);
        //CMyJson::WriteIntKey("nCalibratePixelNirVert",stuCameraInfoArr[i].nCalibratePixelNirVert,jsonVal[i]);

        intVec.clear();
        intVec.push_back(MAX_VIEW_UNIT);
        CMyJson::WriteIntArrayKey("nCalibratePixelRedVert",intVec,(int*)stuCameraInfoArr[i].nCalibratePixelRedVert,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nCalibratePixelGreenVert",intVec,(int*)stuCameraInfoArr[i].nCalibratePixelGreenVert,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nCalibratePixelBlueVert",intVec,(int*)stuCameraInfoArr[i].nCalibratePixelBlueVert,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nCalibratePixelNirVert",intVec,(int*)stuCameraInfoArr[i].nCalibratePixelNirVert,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nChannelBegin",intVec,(int*)stuCameraInfoArr[i].nChannelBegin,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nChannelEnd",intVec,(int*)stuCameraInfoArr[i].nChannelEnd,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nChannelBegin2",intVec,(int*)stuCameraInfoArr[i].nChannelBegin2,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nChannelEnd2",intVec,(int*)stuCameraInfoArr[i].nChannelEnd2,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nEjectBegin",intVec,(int*)stuCameraInfoArr[i].nEjectBegin,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nEjectEnd",intVec,(int*)stuCameraInfoArr[i].nEjectEnd,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nEjectBegin2",intVec,(int*)stuCameraInfoArr[i].nEjectBegin2,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nEjectEnd2",intVec,(int*)stuCameraInfoArr[i].nEjectEnd2,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nBiasingRed",intVec,(int*)stuCameraInfoArr[i].nBiasingRed,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nBiasingGreen",intVec,(int*)stuCameraInfoArr[i].nBiasingGreen,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nBiasingBlue",intVec,(int*)stuCameraInfoArr[i].nBiasingBlue,jsonVal[i]);

        CMyJson::WriteIntArrayKey("nBiasingNir",intVec,(int*)stuCameraInfoArr[i].nBiasingNir,jsonVal[i]);
        CMyJson::WriteIntArrayKey("anaGainLockStatus",intVec,(int*)stuCameraInfoArr[i].anaGainLockStatus,jsonVal[i]);
        CMyJson::WriteIntArrayKey("anaGainLockColor",intVec,(int*)stuCameraInfoArr[i].anaGainLockColor,jsonVal[i]);

        CMyJson::WriteIntArrayKey("nGainTargetRed",intVec,(int*)stuCameraInfoArr[i].nGainTargetRed,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nGainTargetGreen",intVec,(int*)stuCameraInfoArr[i].nGainTargetGreen,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nGainTargetBlue",intVec,(int*)stuCameraInfoArr[i].nGainTargetBlue,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nGainTargetNir",intVec,(int*)stuCameraInfoArr[i].nGainTargetNir,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nDigiGainRed",intVec,(int*)stuCameraInfoArr[i].nDigiGainRed,jsonVal[i]);

        CMyJson::WriteIntArrayKey("nDigiGainGreen",intVec,(int*)stuCameraInfoArr[i].nDigiGainGreen,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nDigiGainBlue",intVec,(int*)stuCameraInfoArr[i].nDigiGainBlue,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nDigiGainNir",intVec,(int*)stuCameraInfoArr[i].nDigiGainNir,jsonVal[i]);


        // 红外部分
        CMyJson::WriteIntArrayKey("nNirGreenOffsetX",intVec,(int*)stuCameraInfoArr[i].nNirGreenOffsetX,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nNirGreenOffsetY",intVec,(int*)stuCameraInfoArr[i].nNirGreenOffsetY,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nNirBlueOffsetX",intVec,(int*)stuCameraInfoArr[i].nNirBlueOffsetX,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nNirBlueOffsetY",intVec,(int*)stuCameraInfoArr[i].nNirBlueOffsetY,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nNirRedOffsetX",intVec,(int*)stuCameraInfoArr[i].nNirRedOffsetX,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nNirRedOffsetY",intVec,(int*)stuCameraInfoArr[i].nNirRedOffsetY,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nNirNOffsetX",intVec,(int*)stuCameraInfoArr[i].nNirNOffsetX,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nNirNOffsetY",intVec,(int*)stuCameraInfoArr[i].nNirNOffsetY,jsonVal[i]);

        intVec.push_back(MAX_EJECTOR);
        CMyJson::WriteIntArrayKey("nChannelEjectorBegin",intVec,(int*)stuCameraInfoArr[i].nChannelEjectorBegin,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nChannelEjectorEnd",intVec,(int*)stuCameraInfoArr[i].nChannelEjectorEnd,jsonVal[i]);
        intVec.clear();
        intVec.push_back(MAX_VIEW_UNIT);
        intVec.push_back(MAX_CALIBRATE_SETS);
        CMyJson::WriteIntArrayKey("nAnaGainRed",intVec,(int*)stuCameraInfoArr[i].nAnaGainRed,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nAnaGainGreen",intVec,(int*)stuCameraInfoArr[i].nAnaGainGreen,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nAnaGainBlue",intVec,(int*)stuCameraInfoArr[i].nAnaGainBlue,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nAnaGainNir",intVec,(int*)stuCameraInfoArr[i].nAnaGainNir,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nCalibrateRed",intVec,(int*)stuCameraInfoArr[i].nCalibrateRed,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nCalibrateGreen",intVec,(int*)stuCameraInfoArr[i].nCalibrateGreen,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nCalibrateBlue",intVec,(int*)stuCameraInfoArr[i].nCalibrateBlue,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nCalibrateNir",intVec,(int*)stuCameraInfoArr[i].nCalibrateNir,jsonVal[i]);

        intVec.clear();
        intVec.push_back(3);
        CMyJson::WriteIntArrayKey("nColorRestoreRed",intVec,(int*)stuCameraInfoArr[i].nColorRestoreRed,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nColorRestoreGreen",intVec,(int*)stuCameraInfoArr[i].nColorRestoreGreen,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nColorRestoreBlue",intVec,(int*)stuCameraInfoArr[i].nColorRestoreBlue,jsonVal[i]);
    }
    return true;
}

bool JsonDataConvert::stuIdenGupInfoToJson(int arraySize, const stru_iden_group_info *stuCameraInfoArr, Json::Value &jsonVal)
{
    vector<int> intVec;
    intVec.push_back(MAX_VIEW_UNIT);
    for(int i = 0; i < arraySize; i++)
    {
        CMyJson::WriteIntKey("nLayerIndex",stuCameraInfoArr[i].nLayerIndex,jsonVal[i]);
        CMyJson::WriteIntKey("nViewIndex",stuCameraInfoArr[i].nViewIndex,jsonVal[i]);
        CMyJson::WriteIntKey("nNameIndex",stuCameraInfoArr[i].nNameIndex,jsonVal[i]);
        CMyJson::WriteIntKey("nGroupUnitTotal",stuCameraInfoArr[i].nGroupUnitTotal,jsonVal[i]);

        CMyJson::WriteIntArrayKey("nUnitGupId",intVec,(int *)stuCameraInfoArr[i].nUnitGupId,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nUnitGupSplice",intVec,(int *)stuCameraInfoArr[i].nUnitGupSplice,jsonVal[i]);
    }
    return true;
}

bool JsonDataConvert::stuVavleGroupInfoToJson(int arraySize, const stru_vavle_group_info *stuTickGupInfo, Json::Value &jsonVal)
{
    vector<int> intVec;
    intVec.push_back(MAX_VAVLE_BOARD);
    for(int i = 0; i < arraySize; i++)
    {
        CMyJson::WriteIntKey("nLayerIndex",stuTickGupInfo[i].nLayerIndex,jsonVal[i]);
        CMyJson::WriteIntKey("nVavleIndex",stuTickGupInfo[i].nVavleIndex,jsonVal[i]);
        CMyJson::WriteIntKey("nNameIndex",stuTickGupInfo[i].nNameIndex,jsonVal[i]);
        CMyJson::WriteIntKey("nEjBoardGupTotal",stuTickGupInfo[i].nEjBoardGupTotal,jsonVal[i]);

        CMyJson::WriteIntArrayKey("nEjBoardGupId",intVec,(int *)stuTickGupInfo[i].nEjBoardGupId,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nEjBoardGupSplice",intVec,(int *)stuTickGupInfo[i].nEjBoardGupSplice,jsonVal[i]);
    }
    return true;
}

bool JsonDataConvert::stuModelSelectCfgToJson(const stru_modelabel_select &stumodeLableSelect,Json::Value &jsonVal)
{

    vector<int> intVec;
    intVec.clear();
    intVec.push_back(MAX_MODE_BIGLABLE_NUM);
    CMyJson::WriteIntArrayKey("labelindex",intVec,(int*)stumodeLableSelect.labelindex,jsonVal);
    return true;
}


bool JsonDataConvert::JsonToStuModelSelectCfg(const Json::Value &jsonVal,stru_modelabel_select &stumodeLableSelect)
{

    INT_ARRAY_KEY outArray;
    vector<int> intVec;
    intVec.push_back(MAX_MODE_BIGLABLE_NUM);
    CMyJson::ReadIntArrayKey(jsonVal,"labelindex",intVec,stumodeLableSelect.labelindex,outArray);

    return true;
}

bool JsonDataConvert::JsonToProfileindex(int arraySize, const Json::Value &jsonVal, stru_Profile_Index *profile)
{
    for(int i = 0;i< arraySize;i++){
        string strsProfileName;
        if(JSON_FAILED_TYPE == CMyJson::ReadStringKey(jsonVal[i], "sProfileName", strsProfileName))
            return false;
        memcpy(profile[i].sProfileName,strsProfileName.data(),strsProfileName.length());


        string strsMaterialName;
        if(JSON_FAILED_TYPE == CMyJson::ReadStringKey(jsonVal[i], "sMaterialName", strsMaterialName))
            return false;
        memcpy(profile[i].sMaterialName,strsMaterialName.data(),strsMaterialName.length());

        CMyJson::ReadIntKey(jsonVal[i],"nProfileLock",profile[i].nProfileLock);
    }
    return true;
}

bool JsonDataConvert::JsonToLayerInfo(int arraySize, const Json::Value &jsonVal, stru_layer_info *layerinfoArr)
{
    JSON_RETURN_VAL jRet;
    INT_ARRAY_KEY outArray;
    vector<int> intVec;
    for(int i = 0;i< arraySize;i++){
        CMyJson::ReadIntKey(jsonVal[i],"nLayerIndex",layerinfoArr[i].nLayerIndex);
        CMyJson::ReadIntKey(jsonVal[i],"nViewChainTotal",layerinfoArr[i].nViewChainTotal);
        CMyJson::ReadIntKey(jsonVal[i],"nSplice",layerinfoArr[i].nSplice);
        CMyJson::ReadIntKey(jsonVal[i],"nOnlyDispFirst",layerinfoArr[i].nOnlyDispFirst);
        CMyJson::ReadIntKey(jsonVal[i],"nSpliceChuteAddr",layerinfoArr[i].nSpliceChuteAddr);
        CMyJson::ReadIntKey(jsonVal[i],"nSpliceType",layerinfoArr[i].nSpliceType);
        intVec.clear();
        intVec.push_back(MAX_VIEW);

        jRet =CMyJson::ReadIntArrayKey(jsonVal[i],"nViewIsUsed",intVec,(int*)layerinfoArr[i].nViewIsUsed,outArray);
        if(jRet == JSON_FAILED_TYPE)
            return false;
        if(jRet == JSON_FAILED_ARRAY_NUM)
        {
            return false;
        }

        jRet =CMyJson::ReadIntArrayKey(jsonVal[i],"nViewId",intVec,(int*)layerinfoArr[i].nViewId,outArray);
        if(jRet == JSON_FAILED_TYPE)
            return false;
        if(jRet == JSON_FAILED_ARRAY_NUM)
        {
            return false;
        }

        jRet =CMyJson::ReadIntArrayKey(jsonVal[i],"nViewBoardType",intVec,(int*)layerinfoArr[i].nViewBoardType,outArray);
        if(jRet == JSON_FAILED_TYPE)
            return false;
        if(jRet == JSON_FAILED_ARRAY_NUM)
        {
            return false;
        }

        // jRet =CMyJson::ReadIntArrayKey(jsonVal[i],"nViewSensorType",intVec,(int*)layerinfoArr[i].nViewSensorType,outArray);
        if(jRet == JSON_FAILED_TYPE)
            return false;
        if(jRet == JSON_FAILED_ARRAY_NUM)
        {
            return false;
        }


        jRet =CMyJson::ReadIntArrayKey(jsonVal[i],"nViewUnitTotal",intVec,(int*)layerinfoArr[i].nViewUnitTotal,outArray);
        if(jRet == JSON_FAILED_TYPE)
            return false;
        if(jRet == JSON_FAILED_ARRAY_NUM)
        {
            return false;
        }
        intVec.clear();
        intVec.push_back(MAX_VAVLE_TYPE);
        jRet =CMyJson::ReadIntArrayKey(jsonVal[i],"nVavleType",intVec,(int*)layerinfoArr[i].nVavleType,outArray);
        if(jRet == JSON_FAILED_TYPE)
            return false;
        if(jRet == JSON_FAILED_ARRAY_NUM)
        {
            return false;
        }

        intVec.clear();
        intVec.push_back(MAX_VAVLE);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nVavleBoardType",intVec,(int*)layerinfoArr[i].nVavleBoardType,outArray);
        jRet =CMyJson::ReadIntArrayKey(jsonVal[i],"nVavleBoardTotal",intVec,(int*)layerinfoArr[i].nVavleBoardTotal,outArray);
        if(jRet == JSON_FAILED_TYPE)
            return false;
        if(jRet == JSON_FAILED_ARRAY_NUM)
        {
            return false;
        }

        CMyJson::ReadIntKey(jsonVal[i],"nVavleChainTotal",layerinfoArr[i].nVavleChainTotal);
        //        CMyJson::ReadIntKey(jsonVal[i],"nCtrlBoardTotal",layerinfoArr[i].nCtrlBoardTotal);
        //        CMyJson::ReadIntKey(jsonVal[i],"nCtrlBoardType",layerinfoArr[i].nCtrlBoardType);
        //        CMyJson::ReadIntKey(jsonVal[i],"nLampBoardType",layerinfoArr[i].nLampBoardType);
        CMyJson::ReadIntKey(jsonVal[i],"nEjectorsPerChute",layerinfoArr[i].nEjectorsPerChute);
        CMyJson::ReadIntKey(jsonVal[i],"nEjectorsPosition",layerinfoArr[i].nEjectorsPosition);

        JsonToStuViewInfo(MAX_VIEW,jsonVal[i]["stuViewInfo"],layerinfoArr[i].stuViewInfo);
        JsonToStuValveInfo(MAX_VAVLE,jsonVal[i]["stuVavleInfo"],layerinfoArr[i].stuVavleInfo);
        JsonToStuCameraInfo(MAX_VIEW,jsonVal[i]["stuCameraInfo"],layerinfoArr[i].stuCameraInfo);
    }
    return true;
}

bool JsonDataConvert::JsonToStuViewInfo(int arraySize, const Json::Value &jsonVal, stru_view_info *stru_view_info)
{
    JSON_RETURN_VAL jRet;
    INT_ARRAY_KEY outArray;
    for(int i = 0;i< arraySize;i++){
        CMyJson::ReadIntKey(jsonVal[i],"nLayerIndex",stru_view_info[i].nLayerIndex);
        CMyJson::ReadIntKey(jsonVal[i],"nNameIndex",stru_view_info[i].nNameIndex);
        vector<int> intVec;
        intVec.push_back(MAX_VIEW_UNIT);

        jRet =CMyJson::ReadIntArrayKey(jsonVal[i],"nViewUnitId",intVec,(int*)stru_view_info[i].nViewUnitId,outArray);
        if(jRet == JSON_FAILED_TYPE)
            return false;
        if(jRet == JSON_FAILED_ARRAY_NUM)
        {
            return false;
        }

        jRet =CMyJson::ReadIntArrayKey(jsonVal[i],"bViewSplice",intVec,(int*)stru_view_info[i].bViewSplice,outArray);
        if(jRet == JSON_FAILED_TYPE)
            return false;
        if(jRet == JSON_FAILED_ARRAY_NUM)
        {
            return false;
        }

        intVec.clear();
        intVec.push_back(4);
        //jRet =CMyJson::ReadIntArrayKey(jsonVal[i],"nSensorColorChoose",intVec,(int*)stru_view_info[i].nSensorColorChoose,outArray);
        if(jRet == JSON_FAILED_TYPE)
            return false;
        if(jRet == JSON_FAILED_ARRAY_NUM)
        {
            return false;
        }

        CMyJson::ReadIntKey(jsonVal[i],"nEjectorsPerCamera",stru_view_info[i].nEjectorsPerCamera);
        // CMyJson::ReadIntKey(jsonVal[i],"nSensorDim",stru_view_info[i].nSensorDim);
        // CMyJson::ReadIntKey(jsonVal[i],"nSensorDimD2",stru_view_info[i].nSensorDimD2);
        CMyJson::ReadIntKey(jsonVal[i],"nSensorD2Mode",stru_view_info[i].nSensorD2Mode);
        //CMyJson::ReadIntKey(jsonVal[i],"nSensBitWidth",stru_view_info[i].nSensBitWidth);
        //CMyJson::ReadIntKey(jsonVal[i],"nSensorRowFre",stru_view_info[i].nSensorRowFre);
        //CMyJson::ReadIntKey(jsonVal[i],"nSensorPixel",stru_view_info[i].nSensorPixel);
        CMyJson::ReadIntKey(jsonVal[i],"rowColQuo",stru_view_info[i].rowColQuo);
        // CMyJson::ReadIntKey(jsonVal[i],"nImageSplice",stru_view_info[i].nImageSplice);
        // CMyJson::ReadIntKey(jsonVal[i],"nSensorDataType",stru_view_info[i].nSensorDataType);
        //CMyJson::ReadIntKey(jsonVal[i],"nMirror",stru_view_info[i].nMirror);
        //CMyJson::ReadIntKey(jsonVal[i],"nSenseBrightSelection",stru_view_info[i].nSenseBrightSelection);
        // CMyJson::ReadIntKey(jsonVal[i],"nSensDropLine",stru_view_info[i].nSensDropLine);
        //CMyJson::ReadIntKey(jsonVal[i],"nSensCmdSpeed",stru_view_info[i].nSensCmdSpeed);
        CMyJson::ReadIntKey(jsonVal[i],"nSensResizeMode",stru_view_info[i].nSensResizeMode);
        // CMyJson::ReadIntKey(jsonVal[i],"nRealTimeCorrection",stru_view_info[i].nRealTimeCorrection);

        CMyJson::ReadIntKey(jsonVal[i],"nRefRed",stru_view_info[i].nBrightRefRed);
        CMyJson::ReadIntKey(jsonVal[i],"nRefGreen",stru_view_info[i].nBrightRefGreen);
        CMyJson::ReadIntKey(jsonVal[i],"nRefBlue",stru_view_info[i].nBrightRefBlue);
        CMyJson::ReadIntKey(jsonVal[i],"nRefNir",stru_view_info[i].nBrightRefNir);
        CMyJson::ReadIntKey(jsonVal[i],"nIdentifyGroupTotal",stru_view_info[i].nIdentifyGroupTotal);
        intVec.clear();
        intVec.push_back(ARITHMETIC_TOTAL);

        jRet =CMyJson::ReadIntArrayKey(jsonVal[i],"nViewArithVisiable",intVec,(int*)stru_view_info[i].nViewArithVisiable,outArray);
        if(jRet == JSON_FAILED_TYPE)
            return false;
        if(jRet == JSON_FAILED_ARRAY_NUM)
        {
            if(outArray.Dimensions.size() == 1){
                int length = sizeof(stru_view_info[i].nViewArithVisiable)/sizeof(int);
                if(length > outArray.Dimensions[0])
                {
                    for(int k = 0;k < outArray.Dimensions[0];k++)
                        stru_view_info[i].nViewArithVisiable[k] = outArray.Datas[k];
                }
            }
            return true;
        }

        JsonToIdenGupInfo(MAX_GROUP_IDENTIFY,jsonVal[i]["stuIdenGupInfo"],stru_view_info[i].stuIdenGupInfo);



    }
    return true;
}

bool JsonDataConvert::JsonToStuValveInfo(int arraySize, const Json::Value &jsonVal, stru_vavle_info *struValveInfoArr)
{
    INT_ARRAY_KEY outArray;
    vector<int> intVec;
    for(int i = 0;i< arraySize;i++){
        CMyJson::ReadIntKey(jsonVal[i],"nLayerIndex",struValveInfoArr[i].nLayerIndex);
        CMyJson::ReadIntKey(jsonVal[i],"nNameIndex",struValveInfoArr[i].nNameIndex);
        CMyJson::ReadIntKey(jsonVal[i],"nViewTotal",struValveInfoArr[i].nViewTotal);

        intVec.clear();
        intVec.push_back(MAX_VAVLE_BOARD);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nVavleBoardId",intVec,struValveInfoArr[i].nVavleBoardId,outArray);
        intVec.clear();
        intVec.push_back(MAX_VIEW);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nVavleViewBoardType",intVec,struValveInfoArr[i].nVavleViewBoardType,outArray);
        //        CMyJson::ReadIntKey(jsonVal[i],"nEjectorsPerVavleBoard",struValveInfoArr[i].nEjectorsPerVavleBoard);
        CMyJson::ReadIntKey(jsonVal[i],"nTickGroupTotal",struValveInfoArr[i].nTickGroupTotal);

        JsonToStuVavleGroupInfo(MAX_GROUP_TICK,jsonVal[i]["stuIdenGupInfo"],struValveInfoArr[i].stuTickGupInfo);
    }
    return true;
}
//int nLampPortEnable[MAX_LAMP_BOARD_PER_INTERFACE][MAX_LAMP_PER_BOARD];      // 每个灯光板上的输出口的使能状态，0-不使能，1-使能，
//int nLampPortType[MAX_LAMP_BOARD_PER_INTERFACE][MAX_LAMP_PER_BOARD];        // 每个灯光板上的输出口的输出类型，0-物料灯  1-背景灯，
//int nLampLayerAddr[MAX_LAMP_BOARD_PER_INTERFACE][MAX_LAMP_PER_BOARD];       // 每个灯光板上的输出口所在的层地址， 0：第一层， 1-第二层...
//int nLampViewAddr[MAX_LAMP_BOARD_PER_INTERFACE][MAX_LAMP_PER_BOARD];        // 每个灯光板上的输出口所在的视地址， 0：前分选室/上分选室， 1-后分选室/下分选室
//int nLampBoardLimit[MAX_LAMP_BOARD_PER_INTERFACE][MAX_LAMP_PER_BOARD];      // 每个灯光板上的输出口的灯光

bool JsonDataConvert::JsonToStuLampCfg(const Json::Value &jsonVal, stru_layer_lamp_cfg &struLayerLampInfoArr)
{
    INT_ARRAY_KEY outArray;
    Float_ARRAY_KEY floatoutArray;
    vector<int> intVec;

    CMyJson::ReadIntKey(jsonVal,"nLampBoardTotal",struLayerLampInfoArr.nLampBoardTotal);
    CMyJson::ReadFloatKey(jsonVal,"nLampValueQuo",struLayerLampInfoArr.nLampValueQuo);
    intVec.clear();
    intVec.push_back(MAX_LAMP_BOARD_PER_INTERFACE);
    intVec.push_back(MAX_LAMP_PER_BOARD);
    CMyJson::ReadIntArrayKey(jsonVal,"nLampPortEnable",intVec,(int*)struLayerLampInfoArr.nLampPortEnable,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nLampPortType",intVec,(int*)struLayerLampInfoArr.nLampPortType,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nLampLayerAddr",intVec,(int*)struLayerLampInfoArr.nLampLayerAddr,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nLampViewAddr",intVec,(int*)struLayerLampInfoArr.nLampViewAddr,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nLampBoardLimit",intVec,(int*)struLayerLampInfoArr.nLampBoardLimit,outArray);

    CMyJson::ReadFloatArrayKey(jsonVal,"fLampBoardQuo",intVec,(float*)struLayerLampInfoArr.fLampBoardQuo,floatoutArray);
    intVec.push_back(MAX_CALIBRATE_SETS);
    CMyJson::ReadIntArrayKey(jsonVal,"nLampValue",intVec,(int*)struLayerLampInfoArr.nLampValue,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nLampEnable",intVec,(int*)struLayerLampInfoArr.nLampEnable,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nLampSeq",intVec,(int*)struLayerLampInfoArr.nLampSeq,outArray);

    vector<string> strsNameVec;
    if(JSON_SUCCESS == CMyJson::ReadStringArrayKey(jsonVal, "sLampName", strsNameVec)){
        int index = 0;
        for(int i = 0;i < MAX_LAMP_BOARD_PER_INTERFACE;i++){
            for(int j = 0; j < MAX_LAMP_PER_BOARD; j++){
                memset(struLayerLampInfoArr.sLampName[i][j],0,sizeof(struLayerLampInfoArr.sLampName[i][j]));
                if(strsNameVec.size() > index){
                    string strsName = strsNameVec[index++];
                    memcpy(struLayerLampInfoArr.sLampName[i][j],strsName.data(),strsName.length());
                }
            }
        }
    }
    return true;
}

bool JsonDataConvert::JsonToStuIntelWipeCfg(const Json::Value &jsonVal,stu_intel_wipe_cfg &struIntelWipeArr OUT)
{

    CMyJson::ReadIntKey(jsonVal,"feederOnffCount",struIntelWipeArr.feederOnffCount);
    CMyJson::ReadIntKey(jsonVal,"wiperDurationPerChute",struIntelWipeArr.wiperDurationPerChute);
    CMyJson::ReadIntKey(jsonVal,"wiperWidth",struIntelWipeArr.wiperWidth);
    CMyJson::ReadIntKey(jsonVal,"wipeTimeout",struIntelWipeArr.wipeTimeout);
    CMyJson::ReadIntKey(jsonVal,"wiperConfirmThread",struIntelWipeArr.wiperConfirmThread);

    return true;
}


bool JsonDataConvert::JSonToStuUseLimitCfg(const Json::Value &jsonVal,stu_use_limit_cfg &struselimitcfg OUT)
{
    CMyJson::ReadIntKey(jsonVal,"valve_start_use",struselimitcfg.valve_start_use);
    CMyJson::ReadIntKey(jsonVal,"valve_life",struselimitcfg.valve_life);
    CMyJson::ReadIntKey(jsonVal,"filter_start_use",struselimitcfg.filter_start_use);
    CMyJson::ReadIntKey(jsonVal,"filter_life",struselimitcfg.filter_life);
    CMyJson::ReadIntKey(jsonVal,"wipe_start_use",struselimitcfg.wipe_start_use);
    CMyJson::ReadIntKey(jsonVal,"wipe_life",struselimitcfg.wipe_life);
    CMyJson::ReadIntKey(jsonVal,"lamp_start_use",struselimitcfg.lamp_start_use);
    CMyJson::ReadIntKey(jsonVal,"lamp_life",struselimitcfg.lamp_life);

    return true;
}
bool JsonDataConvert::JsonToStuCameraInfo(int arraySize, const Json::Value &jsonVal, stru_view_camera_Info *stuCameraInfoArr)
{
    INT_ARRAY_KEY outArray;
    vector<int> intVec;
    for(int i = 0;i< arraySize;i++){
        CMyJson::ReadIntKey(jsonVal[i],"nCalibratePixelRed",stuCameraInfoArr[i].nCalibratePixelRed);
        //  CMyJson::ReadIntKey(jsonVal[i],"nCalibratePixelRedVert",stuCameraInfoArr[i].nCalibratePixelRedVert);
        CMyJson::ReadIntKey(jsonVal[i],"nCalibratePixelGreen",stuCameraInfoArr[i].nCalibratePixelGreen);
        // CMyJson::ReadIntKey(jsonVal[i],"nCalibratePixelGreenVert",stuCameraInfoArr[i].nCalibratePixelGreenVert);
        CMyJson::ReadIntKey(jsonVal[i],"nCalibratePixelBlue",stuCameraInfoArr[i].nCalibratePixelBlue);
        //CMyJson::ReadIntKey(jsonVal[i],"nCalibratePixelBlueVert",stuCameraInfoArr[i].nCalibratePixelBlueVert);
        CMyJson::ReadIntKey(jsonVal[i],"nCalibratePixelNir",stuCameraInfoArr[i].nCalibratePixelNir);
        //  CMyJson::ReadIntKey(jsonVal[i],"nCalibratePixelNirVert",stuCameraInfoArr[i].nCalibratePixelNirVert);

        intVec.clear();
        intVec.push_back(MAX_VIEW_UNIT);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nCalibratePixelRedVert",intVec,stuCameraInfoArr[i].nCalibratePixelRedVert,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nCalibratePixelGreenVert",intVec,stuCameraInfoArr[i].nCalibratePixelGreenVert,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nCalibratePixelBlueVert",intVec,stuCameraInfoArr[i].nCalibratePixelBlueVert,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nCalibratePixelNirVert",intVec,stuCameraInfoArr[i].nCalibratePixelNirVert,outArray);

        CMyJson::ReadIntArrayKey(jsonVal[i],"nChannelBegin",intVec,stuCameraInfoArr[i].nChannelBegin,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nChannelEnd",intVec,stuCameraInfoArr[i].nChannelEnd,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nChannelBegin2",intVec,stuCameraInfoArr[i].nChannelBegin2,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nChannelEnd2",intVec,stuCameraInfoArr[i].nChannelEnd2,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nEjectBegin",intVec,stuCameraInfoArr[i].nEjectBegin,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nEjectEnd",intVec,stuCameraInfoArr[i].nEjectEnd,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nEjectBegin2",intVec,stuCameraInfoArr[i].nEjectBegin2,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nEjectEnd2",intVec,stuCameraInfoArr[i].nEjectEnd2,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nBiasingRed",intVec,stuCameraInfoArr[i].nBiasingRed,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nBiasingGreen",intVec,stuCameraInfoArr[i].nBiasingGreen,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nBiasingBlue",intVec,stuCameraInfoArr[i].nBiasingBlue,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nBiasingNir",intVec,stuCameraInfoArr[i].nBiasingNir,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"anaGainLockStatus",intVec,stuCameraInfoArr[i].anaGainLockStatus,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"anaGainLockColor",intVec,stuCameraInfoArr[i].anaGainLockColor,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nGainTargetRed",intVec,stuCameraInfoArr[i].nGainTargetRed,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nGainTargetGreen",intVec,stuCameraInfoArr[i].nGainTargetGreen,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nGainTargetBlue",intVec,stuCameraInfoArr[i].nGainTargetBlue,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nDigiGainRed",intVec,stuCameraInfoArr[i].nDigiGainRed,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nDigiGainGreen",intVec,stuCameraInfoArr[i].nDigiGainGreen,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nDigiGainBlue",intVec,stuCameraInfoArr[i].nDigiGainBlue,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nDigiGainNir",intVec,stuCameraInfoArr[i].nDigiGainNir,outArray);

        // 红外部分
        CMyJson::ReadIntArrayKey(jsonVal[i],"nNirGreenOffsetX",intVec,stuCameraInfoArr[i].nNirGreenOffsetX,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nNirGreenOffsetY",intVec,stuCameraInfoArr[i].nNirGreenOffsetY,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nNirBlueOffsetX",intVec,stuCameraInfoArr[i].nNirBlueOffsetX,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nNirBlueOffsetY",intVec,stuCameraInfoArr[i].nNirBlueOffsetY,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nNirRedOffsetX",intVec,stuCameraInfoArr[i].nNirRedOffsetX,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nNirRedOffsetY",intVec,stuCameraInfoArr[i].nNirRedOffsetY,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nNirNOffsetX",intVec,stuCameraInfoArr[i].nNirNOffsetX,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nNirNOffsetY",intVec,stuCameraInfoArr[i].nNirNOffsetY,outArray);

        intVec.push_back(MAX_EJECTOR);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nChannelEjectorBegin",intVec,(int*)stuCameraInfoArr[i].nChannelEjectorBegin,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nChannelEjectorEnd",intVec,(int*)stuCameraInfoArr[i].nChannelEjectorEnd,outArray);
        intVec.clear();
        intVec.push_back(MAX_VIEW_UNIT);
        intVec.push_back(MAX_CALIBRATE_SETS);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nAnaGainRed",intVec,(int*)stuCameraInfoArr[i].nAnaGainRed,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nAnaGainGreen",intVec,(int*)stuCameraInfoArr[i].nAnaGainGreen,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nAnaGainBlue",intVec,(int*)stuCameraInfoArr[i].nAnaGainBlue,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nAnaGainNir",intVec,(int*)stuCameraInfoArr[i].nAnaGainNir,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nCalibrateRed",intVec,(int*)stuCameraInfoArr[i].nCalibrateRed,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nCalibrateGreen",intVec,(int*)stuCameraInfoArr[i].nCalibrateGreen,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nCalibrateBlue",intVec,(int*)stuCameraInfoArr[i].nCalibrateBlue,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nCalibrateNir",intVec,(int*)stuCameraInfoArr[i].nCalibrateNir,outArray);

        intVec.clear();
        intVec.push_back(3);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nColorRestoreRed",intVec,(int*)stuCameraInfoArr[i].nColorRestoreRed,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nColorRestoreGreen",intVec,(int*)stuCameraInfoArr[i].nColorRestoreGreen,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nColorRestoreBlue",intVec,(int*)stuCameraInfoArr[i].nColorRestoreBlue,outArray);
    }
    return true;
}

bool JsonDataConvert::JsonToIdenGupInfo(int arraySize, const Json::Value &jsonVal, stru_iden_group_info *stuIdenGroupArr)
{
    INT_ARRAY_KEY outArray;
    vector<int> intVec;
    intVec.push_back(MAX_VIEW_UNIT);
    for(int i = 0;i< arraySize;i++){
        CMyJson::ReadIntKey(jsonVal[i],"nLayerIndex",stuIdenGroupArr[i].nLayerIndex);
        CMyJson::ReadIntKey(jsonVal[i],"nViewIndex",stuIdenGroupArr[i].nViewIndex);

        CMyJson::ReadIntKey(jsonVal[i],"nNameIndex",stuIdenGroupArr[i].nNameIndex);
        CMyJson::ReadIntKey(jsonVal[i],"nGroupUnitTotal",stuIdenGroupArr[i].nGroupUnitTotal);

        CMyJson::ReadIntArrayKey(jsonVal[i],"nUnitGupId",intVec,stuIdenGroupArr[i].nUnitGupId,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nUnitGupSplice",intVec,stuIdenGroupArr[i].nUnitGupSplice,outArray);

    }
    return true;
}

bool JsonDataConvert::JsonToStuVavleGroupInfo(int arraySize, const Json::Value &jsonVal, stru_vavle_group_info *stuTickGupInfo)
{
    vector<int> intVec;
    intVec.push_back(MAX_VAVLE_BOARD);
    INT_ARRAY_KEY outArray;
    for(int i = 0;i< arraySize;i++){
        CMyJson::ReadIntKey(jsonVal[i],"nLayerIndex",stuTickGupInfo[i].nLayerIndex);
        CMyJson::ReadIntKey(jsonVal[i],"nVavleIndex",stuTickGupInfo[i].nVavleIndex);
        CMyJson::ReadIntKey(jsonVal[i],"nNameIndex",stuTickGupInfo[i].nNameIndex);
        CMyJson::ReadIntKey(jsonVal[i],"nEjBoardGupTotal",stuTickGupInfo[i].nEjBoardGupTotal);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nEjBoardGupId",intVec,stuTickGupInfo[i].nEjBoardGupId,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nEjBoardGupSplice",intVec,stuTickGupInfo[i].nEjBoardGupSplice,outArray);
    }
    return true;
}

bool JsonDataConvert::JsonToStuAlarmCfg(const Json::Value &jsonVal, stu_alarm_cfg &stru_alarm)
{
    vector<string> strsNameVec;
    if(JSON_SUCCESS == CMyJson::ReadStringArrayKey(jsonVal, "nFeederLevelAlarmName", strsNameVec)){

        for(int i = 0;i < CTRL_ALARM_MAX_FEED_SENSOR;i++){
            memset(stru_alarm.nFeederLevelAlarmName[i],0,sizeof(stru_alarm.nFeederLevelAlarmName[i]));
            string strsName = strsNameVec[i];
            memcpy(stru_alarm.nFeederLevelAlarmName[i],strsName.data(),strsName.length());
        }
    }

    strsNameVec.clear();
    if(JSON_SUCCESS == CMyJson::ReadStringArrayKey(jsonVal, "nTemptureAlarmName", strsNameVec)){
        for(int i = 0;i < CTRL_ALARM_MAX_TEMP_SENSOR;i++){
            memset(stru_alarm.nTemptureAlarmName[i],0,sizeof(stru_alarm.nTemptureAlarmName[i]));
            memcpy(stru_alarm.nTemptureAlarmName[i],strsNameVec[i].data(),strsNameVec[i].length());
        }
    }
    INT_ARRAY_KEY outArray;
    vector<int> intVec;
    intVec.push_back(CTRL_ALARM_MAX_FEED_SENSOR);
    CMyJson::ReadIntArrayKey(jsonVal,"nFeederLevelAlarmEnable",intVec,stru_alarm.nFeederLevelAlarmEnable,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nFeederLevelAlarmPosi",intVec,stru_alarm.nFeederLevelAlarmPosi,outArray);
    CMyJson::ReadIntKey(jsonVal,"nAirPressureAlarmType",stru_alarm.nAirPressureAlarmType);
    CMyJson::ReadIntKey(jsonVal,"nLightVoiceAlarmType",stru_alarm.nLightVoiceAlarmType);
    CMyJson::ReadIntKey(jsonVal,"nFeederLevelAlarmDealMode",stru_alarm.nFeederLevelAlarmDealMode);
    CMyJson::ReadIntKey(jsonVal,"nTemptureAlarmDealMode",stru_alarm.nTemptureAlarmDealMode);

    intVec.clear();
    intVec.push_back(CTRL_ALARM_MAX_TEMP_SENSOR);
    CMyJson::ReadIntArrayKey(jsonVal,"nTemptureAlarmEnable",intVec,stru_alarm.nTemptureAlarmEnable,outArray);
    CMyJson::ReadIntArrayKey(jsonVal,"nTemptureAlarmLimit",intVec,stru_alarm.nTemptureAlarmLimit,outArray);
    return true;
}

bool JsonDataConvert::JsonTostruGreyColor(int arraySize, const Json::Value &jsonVal,stu_color_arith *struGreyColor)
{
    for(int i = 0;i < arraySize;i++){
        string strsName;
        if(JSON_FAILED_TYPE == CMyJson::ReadStringKey(jsonVal[i], "sName", strsName))
            return false;
        memset(struGreyColor[i].sName,0,sizeof(struGreyColor[i].sName));
        memcpy(struGreyColor[i].sName,strsName.data(),strsName.length());
        CMyJson::ReadIntKey(jsonVal[i],"nArithEnable",struGreyColor[i].nArithEnable);
        CMyJson::ReadIntKey(jsonVal[i],"nDispEnable",struGreyColor[i].nDispEnable);
        //        CMyJson::ReadIntKey(jsonVal[i],"nEnable",struGreyColor[i].nEnable);
        CMyJson::ReadIntKey(jsonVal[i],"nVavle",struGreyColor[i].nVavle);
        CMyJson::ReadIntKey(jsonVal[i],"nRow",struGreyColor[i].nRow);
        CMyJson::ReadIntKey(jsonVal[i],"nColumn",struGreyColor[i].nColumn);
        CMyJson::ReadIntKey(jsonVal[i],"nPercent",struGreyColor[i].nPercent);
        CMyJson::ReadIntKey(jsonVal[i],"nMain",struGreyColor[i].nMain);
        CMyJson::ReadIntKey(jsonVal[i],"nVice",struGreyColor[i].nVice);
        CMyJson::ReadIntKey(jsonVal[i],"nDiffColor",struGreyColor[i].nDiffColor);
        CMyJson::ReadIntKey(jsonVal[i],"nSensLow",struGreyColor[i].nSensLow);
        CMyJson::ReadIntKey(jsonVal[i],"nSensUp",struGreyColor[i].nSensUp);
        CMyJson::ReadFloatKey(jsonVal[i],"fPropotionSensLow",struGreyColor[i].fPropotionSensLow);
        CMyJson::ReadFloatKey(jsonVal[i],"fPropotionSensUp",struGreyColor[i].fPropotionSensUp);

        CMyJson::ReadIntKey(jsonVal[i],"nLineDefectEnable",struGreyColor[i].nLineDefectEnable);
        CMyJson::ReadIntKey(jsonVal[i],"nLineDefectIndex",struGreyColor[i].nLineDefectIndex);
        CMyJson::ReadIntKey(jsonVal[i],"fnLineDefectAreaLow",struGreyColor[i].fnLineDefectAreaLow);
        CMyJson::ReadIntKey(jsonVal[i],"fnLineDefectAreaUp",struGreyColor[i].fnLineDefectAreaUp);
        CMyJson::ReadIntKey(jsonVal[i],"fnLineDefectLengthLow",struGreyColor[i].fnLineDefectLengthLow);
        CMyJson::ReadIntKey(jsonVal[i],"fnLineDefectLengthUp",struGreyColor[i].fnLineDefectLengthUp);

        CMyJson::ReadIntKey(jsonVal[i],"nMode",struGreyColor[i].nMode);
        CMyJson::ReadIntKey(jsonVal[i],"lightLimit",struGreyColor[i].lightLimit);
        CMyJson::ReadIntKey(jsonVal[i],"nResType",struGreyColor[i].nResType);
        CMyJson::ReadIntKey(jsonVal[i],"nSensUpdate",struGreyColor[i].nSensUpdate);
        CMyJson::ReadIntKey(jsonVal[i],"nEdgeType",struGreyColor[i].nEdgeType);
        CMyJson::ReadIntKey(jsonVal[i],"nImpureType",struGreyColor[i].nImpureType);
        CMyJson::ReadIntKey(jsonVal[i],"nPropotionEnable",struGreyColor[i].nPropotionEnable);
        CMyJson::ReadFloatKey(jsonVal[i],"nPropResSens",struGreyColor[i].nPropResSens);

        CMyJson::ReadIntKey(jsonVal[i],"nEdge",struGreyColor[i].nEdge);
        CMyJson::ReadIntKey(jsonVal[i],"nPropotionParaSensLow",struGreyColor[i].nPropotionParaSensLow);
        CMyJson::ReadIntKey(jsonVal[i],"delayType",struGreyColor[i].delayType);
        JsonTostruReserved(jsonVal[i]["struResGreyColor"],struGreyColor[i].struResGreyColor);
    }
    return true;
}

bool JsonDataConvert::JsonTostruIntel(int arraySize, const Json::Value &jsonVal, stu_svm *struSvm)
{
    INT_ARRAY_KEY outArray;
    vector<int> intVec;
    intVec.push_back(2);
    for(int i = 0;i < arraySize;i++){
        string strsName;
        if(JSON_FAILED_TYPE == CMyJson::ReadStringKey(jsonVal[i], "sName", strsName))
            return false;
        memset(struSvm[i].sName,0,sizeof(struSvm[i].sName));
        memcpy(struSvm[i].sName,strsName.data(),strsName.length());
        CMyJson::ReadIntKey(jsonVal[i],"nArithEnable",struSvm[i].nArithEnable);
        CMyJson::ReadIntKey(jsonVal[i],"nDispEnable",struSvm[i].nDispEnable);
        //        CMyJson::ReadIntKey(jsonVal[i],"nEnable",struSvm[i].nEnable);
        CMyJson::ReadIntKey(jsonVal[i],"nTypeIndex",struSvm[i].nTypeIndex);
        CMyJson::ReadIntKey(jsonVal[i],"nVavle",struSvm[i].nVavle);
        CMyJson::ReadIntKey(jsonVal[i],"nSizeType",struSvm[i].nSizeType);
        CMyJson::ReadIntKey(jsonVal[i],"nRow",struSvm[i].nRow);
        CMyJson::ReadIntKey(jsonVal[i],"nColumn",struSvm[i].nColumn);
        CMyJson::ReadIntKey(jsonVal[i],"nPercent",struSvm[i].nPercent);
        CMyJson::ReadIntKey(jsonVal[i],"nPropotionEnable",struSvm[i].nPropotionEnable);

        CMyJson::ReadIntKey(jsonVal[i],"nEdgeType",struSvm[i].nEdgeType);
        CMyJson::ReadIntKey(jsonVal[i],"nImpureType",struSvm[i].nImpureType);
        CMyJson::ReadIntKey(jsonVal[i],"nSens",struSvm[i].nSens);
        CMyJson::ReadIntKey(jsonVal[i],"nModeSvm",struSvm[i].nModeSvm);
        CMyJson::ReadFloatKey(jsonVal[i],"fPropotionSensLow",struSvm[i].fPropotionSensLow);
        CMyJson::ReadFloatKey(jsonVal[i],"fPropotionSensUp",struSvm[i].fPropotionSensUp);
        CMyJson::ReadIntKey(jsonVal[i],"nPropotionParaSensLow",struSvm[i].nPropotionParaSensLow);

        CMyJson::ReadIntKey(jsonVal[i],"nLineDefectEnable",struSvm[i].nLineDefectEnable);
        CMyJson::ReadIntKey(jsonVal[i],"nLineDefectIndex",struSvm[i].nLineDefectIndex);
        CMyJson::ReadIntKey(jsonVal[i],"fnLineDefectAreaLow",struSvm[i].fnLineDefectAreaLow);
        CMyJson::ReadIntKey(jsonVal[i],"fnLineDefectAreaUp",struSvm[i].fnLineDefectAreaUp);
        CMyJson::ReadIntKey(jsonVal[i],"fnLineDefectLengthLow",struSvm[i].fnLineDefectLengthLow);
        CMyJson::ReadIntKey(jsonVal[i],"fnLineDefectLengthUp",struSvm[i].fnLineDefectLengthUp);

        CMyJson::ReadIntArrayKey(jsonVal[i],"nQuoCons",intVec,struSvm[i].nQuoCons,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nQuoR",intVec,struSvm[i].nQuoR,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nQuoG",intVec,struSvm[i].nQuoG,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nQuoB",intVec,struSvm[i].nQuoB,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nQuoN",intVec,struSvm[i].nQuoN,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nQuoRG",intVec,struSvm[i].nQuoRG,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nQuoRB",intVec,struSvm[i].nQuoRB,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nQuoGB",intVec,struSvm[i].nQuoGB,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nQuoRR",intVec,struSvm[i].nQuoRR,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nQuoGG",intVec,struSvm[i].nQuoGG,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nQuoBB",intVec,struSvm[i].nQuoBB,outArray);

        CMyJson::ReadIntArrayKey(jsonVal[i],"nQuoRN",intVec,struSvm[i].nQuoRN,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nQuoGN",intVec,struSvm[i].nQuoGN,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nQuoBN",intVec,struSvm[i].nQuoBN,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nQuoNN",intVec,struSvm[i].nQuoNN,outArray);

        CMyJson::ReadIntKey(jsonVal[i],"nEdge",struSvm[i].nEdge);
        CMyJson::ReadIntKey(jsonVal[i],"nModeType",struSvm[i].nModeType);
        CMyJson::ReadIntKey(jsonVal[i],"nConsExpandedValue",struSvm[i].nConsExpandedValue);
        CMyJson::ReadIntKey(jsonVal[i],"delayType",struSvm[i].delayType);
        CMyJson::ReadIntKey(jsonVal[i],"maxSenseValue",struSvm[i].maxSenseValue);
        CMyJson::ReadIntKey(jsonVal[i],"impuritPos",struSvm[i].impuritPos);
        CMyJson::ReadIntKey(jsonVal[i],"brightLimit",struSvm[i].brightLimit);
    }
    return true;
}

bool JsonDataConvert::JsonTostruShapeIntel(int arraySize,const Json::Value &jsonVal, stu_shape_svm *struShapeSvm)
{
    INT_ARRAY_KEY outArray;
    vector<int> intVec;
    intVec.push_back(2);
    for(int i = 0;i < arraySize;i++){
        string strsName;
        if(JSON_FAILED_TYPE == CMyJson::ReadStringKey(jsonVal[i], "sName", strsName))
            return false;
        memset(struShapeSvm[i].sName,0,sizeof(struShapeSvm[i].sName));
        memcpy(struShapeSvm[i].sName,strsName.data(),strsName.length());
        CMyJson::ReadIntKey(jsonVal[i],"nArithEnable",struShapeSvm[i].nArithEnable);
        CMyJson::ReadIntKey(jsonVal[i],"nDispEnable",struShapeSvm[i].nDispEnable);

        CMyJson::ReadIntKey(jsonVal[i],"nAreaLow",struShapeSvm[i].nAreaLow);
        CMyJson::ReadIntKey(jsonVal[i],"nAreaUp",struShapeSvm[i].nAreaUp);
        CMyJson::ReadIntKey(jsonVal[i],"nSens",struShapeSvm[i].nSens);
        CMyJson::ReadIntKey(jsonVal[i],"nModeSvm",struShapeSvm[i].nModeSvm);

        CMyJson::ReadIntArrayKey(jsonVal[i],"nQuoCons",intVec,struShapeSvm[i].nQuoCons,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nQuoA",intVec,struShapeSvm[i].nQuoA,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nQuoB",intVec,struShapeSvm[i].nQuoB,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nQuoC",intVec,struShapeSvm[i].nQuoC,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nQuoD",intVec,struShapeSvm[i].nQuoD,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nQuoE",intVec,struShapeSvm[i].nQuoE,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nQuoF",intVec,struShapeSvm[i].nQuoF,outArray);

        CMyJson::ReadIntKey(jsonVal[i],"nEdge",struShapeSvm[i].nEdge);
    }

    return true;
}

#if 0
bool JsonDataConvert::JsonTostruCross(const Json::Value &jsonVal, stu_cs &struCross)
{
    string strsName;
    if(JSON_FAILED_TYPE == CMyJson::ReadStringKey(jsonVal, "sName", strsName))
        return false;
    memcpy(struCross.sName,strsName.data(),strsName.length());
    CMyJson::ReadIntKey(jsonVal,"nVavle",struCross.nVavle);
    CMyJson::ReadIntKey(jsonVal,"nRow",struCross.nRow);
    CMyJson::ReadIntKey(jsonVal,"nColumn",struCross.nColumn);
    CMyJson::ReadIntKey(jsonVal,"nPercent",struCross.nPercent);
    CMyJson::ReadIntKey(jsonVal,"nSens",struCross.nSens);
    CMyJson::ReadIntKey(jsonVal,"nColor",struCross.nColor);
    return true;
}
#endif

bool JsonDataConvert::JsonTostruCircleLong(int arraySize,const Json::Value &jsonVal, stu_circle_long *struCircleLong)
{
    if (jsonVal.isArray()){
        for(int i = 0;i<arraySize;i++){
            string strsName;
            if(JSON_FAILED_TYPE == CMyJson::ReadStringKey(jsonVal[i], "sName", strsName))
                return false;
            memset(struCircleLong[i].sName,0,sizeof(struCircleLong[i].sName));
            memcpy(struCircleLong[i].sName,strsName.data(),strsName.length());
            CMyJson::ReadIntKey(jsonVal[i],"nArithEnable",struCircleLong[i].nArithEnable);
            CMyJson::ReadIntKey(jsonVal[i],"nDispEnable",struCircleLong[i].nDispEnable);
            //    CMyJson::ReadIntKey(jsonVal,"nEnable",struCircleLong.nEnable);
            CMyJson::ReadIntKey(jsonVal[i],"nTypeIndex",struCircleLong[i].nTypeIndex);
            CMyJson::ReadIntKey(jsonVal[i],"nVavle",struCircleLong[i].nVavle);
            CMyJson::ReadIntKey(jsonVal[i],"nCiloLow",struCircleLong[i].nCiloLow);
            CMyJson::ReadIntKey(jsonVal[i],"nCilosUp",struCircleLong[i].nCilosUp);
            CMyJson::ReadIntKey(jsonVal[i],"nMode",struCircleLong[i].nMode);
            CMyJson::ReadIntKey(jsonVal[i],"nLimit",struCircleLong[i].nLimit);
            CMyJson::ReadIntKey(jsonVal[i],"delayType",struCircleLong[i].delayType);
        }
    }
    else{
        string strsName;
        if(JSON_FAILED_TYPE == CMyJson::ReadStringKey(jsonVal, "sName", strsName))
            return false;
        memset(struCircleLong[0].sName,0,sizeof(struCircleLong[0].sName));
        memcpy(struCircleLong[0].sName,strsName.data(),strsName.length());
        CMyJson::ReadIntKey(jsonVal,"nArithEnable",struCircleLong[0].nArithEnable);
        CMyJson::ReadIntKey(jsonVal,"nDispEnable",struCircleLong[0].nDispEnable);
        //    CMyJson::ReadIntKey(jsonVal,"nEnable",struCircleLong.nEnable);
        CMyJson::ReadIntKey(jsonVal,"nTypeIndex",struCircleLong[0].nTypeIndex);
        CMyJson::ReadIntKey(jsonVal,"nVavle",struCircleLong[0].nVavle);
        CMyJson::ReadIntKey(jsonVal,"nCiloLow",struCircleLong[0].nCiloLow);
        CMyJson::ReadIntKey(jsonVal,"nCilosUp",struCircleLong[0].nCilosUp);
        CMyJson::ReadIntKey(jsonVal,"nMode",struCircleLong[0].nMode);
        CMyJson::ReadIntKey(jsonVal,"nLimit",struCircleLong[0].nLimit);
        CMyJson::ReadIntKey(jsonVal,"delayType",struCircleLong[0].delayType);

    }

    return true;
}

bool JsonDataConvert::JsonTostruLongShort(int arraySize,const Json::Value &jsonVal, stu_long_short *struLongShort)
{
    if (jsonVal.isArray()){
        for(int i = 0;i < arraySize;i++){
            string strsName;
            if(JSON_FAILED_TYPE == CMyJson::ReadStringKey(jsonVal[i], "sName", strsName))
                return false;
            memset(struLongShort[i].sName,0,sizeof(struLongShort[i].sName));
            memcpy(struLongShort[i].sName,strsName.data(),strsName.length());
            CMyJson::ReadIntKey(jsonVal[i],"nArithEnable",struLongShort[i].nArithEnable);
            CMyJson::ReadIntKey(jsonVal[i],"nDispEnable",struLongShort[i].nDispEnable);
            //    CMyJson::ReadIntKey(jsonVal,"nEnable",struLongShort.nEnable);
            CMyJson::ReadIntKey(jsonVal[i],"nTypeIndex",struLongShort[i].nTypeIndex);
            CMyJson::ReadIntKey(jsonVal[i],"nVavle",struLongShort[i].nVavle);
            CMyJson::ReadIntKey(jsonVal[i],"nLengthLow",struLongShort[i].nLengthLow);
            CMyJson::ReadIntKey(jsonVal[i],"nLengthUp",struLongShort[i].nLengthUp);
            CMyJson::ReadIntKey(jsonVal[i],"nLengthMode",struLongShort[i].nLengthMode);
            CMyJson::ReadIntKey(jsonVal[i],"nLimit",struLongShort[i].nLimit);
            CMyJson::ReadIntKey(jsonVal[i],"delayType",struLongShort[i].delayType);
        }
    }
    else{
        string strsName;
        if(JSON_FAILED_TYPE == CMyJson::ReadStringKey(jsonVal, "sName", strsName))
            return false;
        memset(struLongShort[0].sName,0,sizeof(struLongShort[0].sName));
        memcpy(struLongShort[0].sName,strsName.data(),strsName.length());
        CMyJson::ReadIntKey(jsonVal,"nArithEnable",struLongShort[0].nArithEnable);
        CMyJson::ReadIntKey(jsonVal,"nDispEnable",struLongShort[0].nDispEnable);
        //    CMyJson::ReadIntKey(jsonVal,"nEnable",struLongShort.nEnable);
        CMyJson::ReadIntKey(jsonVal,"nTypeIndex",struLongShort[0].nTypeIndex);
        CMyJson::ReadIntKey(jsonVal,"nVavle",struLongShort[0].nVavle);
        CMyJson::ReadIntKey(jsonVal,"nLengthLow",struLongShort[0].nLengthLow);
        CMyJson::ReadIntKey(jsonVal,"nLengthUp",struLongShort[0].nLengthUp);
        CMyJson::ReadIntKey(jsonVal,"nLengthMode",struLongShort[0].nLengthMode);
        CMyJson::ReadIntKey(jsonVal,"nLimit",struLongShort[0].nLimit);
        CMyJson::ReadIntKey(jsonVal,"delayType",struLongShort[0].delayType);
    }
    return true;
}

bool JsonDataConvert::JsonTostruBigSmall(int arraySize, const Json::Value &jsonVal, stu_big_small *struBigSmall)
{
    if (jsonVal.isArray()){
        for(int i = 0;i < arraySize;i++){
            string strsName;
            if(JSON_FAILED_TYPE == CMyJson::ReadStringKey(jsonVal[i], "sName", strsName))
                return false;
            memset(struBigSmall[i].sName,0,sizeof(struBigSmall[i].sName));
            memcpy(struBigSmall[i].sName,strsName.data(),strsName.length());
            CMyJson::ReadIntKey(jsonVal[i],"nArithEnable",struBigSmall[i].nArithEnable);
            CMyJson::ReadIntKey(jsonVal[i],"nDispEnable",struBigSmall[i].nDispEnable);
            //    CMyJson::ReadIntKey(jsonVal,"nEnable",struBigSmall.nEnable);
            CMyJson::ReadIntKey(jsonVal[i],"nTypeIndex",struBigSmall[i].nTypeIndex);
            CMyJson::ReadIntKey(jsonVal[i],"nVavle",struBigSmall[i].nVavle);
            CMyJson::ReadIntKey(jsonVal[i],"nMode",struBigSmall[i].nMode);
            CMyJson::ReadIntKey(jsonVal[i],"nAreaLow",struBigSmall[i].nAreaLow);
            CMyJson::ReadIntKey(jsonVal[i],"nAreaUp",struBigSmall[i].nAreaUp);
            CMyJson::ReadIntKey(jsonVal[i],"nLimit",struBigSmall[i].nLimit);
            CMyJson::ReadIntKey(jsonVal[i],"delayType",struBigSmall[i].delayType);
        }
    }
    else{
        string strsName;
        if(JSON_FAILED_TYPE == CMyJson::ReadStringKey(jsonVal, "sName", strsName))
            return false;
        memset(struBigSmall[0].sName,0,sizeof(struBigSmall[0].sName));
        memcpy(struBigSmall[0].sName,strsName.data(),strsName.length());
        CMyJson::ReadIntKey(jsonVal,"nArithEnable",struBigSmall[0].nArithEnable);
        CMyJson::ReadIntKey(jsonVal,"nDispEnable",struBigSmall[0].nDispEnable);
        //    CMyJson::ReadIntKey(jsonVal,"nEnable",struBigSmall.nEnable);
        CMyJson::ReadIntKey(jsonVal,"nTypeIndex",struBigSmall[0].nTypeIndex);
        CMyJson::ReadIntKey(jsonVal,"nVavle",struBigSmall[0].nVavle);
        CMyJson::ReadIntKey(jsonVal,"nMode",struBigSmall[0].nMode);
        CMyJson::ReadIntKey(jsonVal,"nAreaLow",struBigSmall[0].nAreaLow);
        CMyJson::ReadIntKey(jsonVal,"nAreaUp",struBigSmall[0].nAreaUp);
        CMyJson::ReadIntKey(jsonVal,"nLimit",struBigSmall[0].nLimit);
        CMyJson::ReadIntKey(jsonVal,"delayType",struBigSmall[0].delayType);
    }

    return true;
}

bool JsonDataConvert::JsonTostruColorSat(const Json::Value &jsonVal, stru_color_sat &struColorSat)
{
    string strsName;
    if(JSON_FAILED_TYPE == CMyJson::ReadStringKey(jsonVal, "sName", strsName))
        return false;
    memset(struColorSat.sName,0,sizeof(struColorSat.sName));
    memcpy(struColorSat.sName,strsName.data(),strsName.length());
    CMyJson::ReadIntKey(jsonVal,"nArithEnable",struColorSat.nArithEnable);
    CMyJson::ReadIntKey(jsonVal,"nDispEnable",struColorSat.nDispEnable);
    CMyJson::ReadIntKey(jsonVal,"nVavle",struColorSat.nVavle);
    CMyJson::ReadIntKey(jsonVal,"nRow",struColorSat.nRow);
    CMyJson::ReadIntKey(jsonVal,"nColumn",struColorSat.nColumn);
    CMyJson::ReadIntKey(jsonVal,"nPercent",struColorSat.nPercent);
    CMyJson::ReadIntKey(jsonVal,"nMode",struColorSat.nMode);
    CMyJson::ReadIntKey(jsonVal,"nSensLow",struColorSat.nSensLow);
    CMyJson::ReadIntKey(jsonVal,"nSensUp",struColorSat.nSensUp);
    CMyJson::ReadIntKey(jsonVal,"lightLimit",struColorSat.lightLimit);
    return true;
}

bool JsonDataConvert::JsonTostruWhitePeanut(const Json::Value &jsonVal, stru_white_peanut &struWhitePeanut)
{
    string strsName;
    if(JSON_FAILED_TYPE == CMyJson::ReadStringKey(jsonVal, "sName", strsName))
        return false;
    memset(struWhitePeanut.sName,0,sizeof(struWhitePeanut.sName));
    memcpy(struWhitePeanut.sName,strsName.data(),strsName.length());
    CMyJson::ReadIntKey(jsonVal,"nArithEnable",struWhitePeanut.nArithEnable);
    CMyJson::ReadIntKey(jsonVal,"nDispEnable",struWhitePeanut.nDispEnable);
    CMyJson::ReadIntKey(jsonVal,"nSensUp",struWhitePeanut.nSensUp);
    CMyJson::ReadIntKey(jsonVal,"nSensLow",struWhitePeanut.nSensLow);
    return true;
}

bool JsonDataConvert::JsonTostruPepperHandle(const Json::Value &jsonVal, stru_pepper_handle &strupepperhandle)
{
    string strsName;
    if(JSON_FAILED_TYPE == CMyJson::ReadStringKey(jsonVal, "sName", strsName))
        return false;
    memset(strupepperhandle.sName,0,sizeof(strupepperhandle.sName));
    memcpy(strupepperhandle.sName,strsName.data(),strsName.length());
    CMyJson::ReadIntKey(jsonVal,"nArithEnable",strupepperhandle.nArithEnable);
    CMyJson::ReadIntKey(jsonVal,"nDispEnable",strupepperhandle.nDispEnable);
    CMyJson::ReadIntKey(jsonVal,"nSens",strupepperhandle.nSens);
    CMyJson::ReadIntKey(jsonVal,"nPercent",strupepperhandle.nPercent);
    CMyJson::ReadIntKey(jsonVal,"nLimit",strupepperhandle.nLimit);
    return true;
}

bool JsonDataConvert::JsonTostruSuppressLarge(int arraySize,const Json::Value &jsonVal,stru_suppress_large *struSuppressLarge OUT)
{
    if (jsonVal.isArray()){
        for(int i = 0;i < arraySize;i++){
            string strsName;
            if(JSON_FAILED_TYPE == CMyJson::ReadStringKey(jsonVal[i], "sName", strsName))
                return false;
            memset(struSuppressLarge[i].sName,0,sizeof(struSuppressLarge[i].sName));
            memcpy(struSuppressLarge[i].sName,strsName.data(),strsName.length());
            CMyJson::ReadIntKey(jsonVal[i],"nArithEnable",struSuppressLarge[i].nArithEnable);
            CMyJson::ReadIntKey(jsonVal[i],"nDispEnable",struSuppressLarge[i].nDispEnable);
            CMyJson::ReadIntKey(jsonVal[i],"nSize1",struSuppressLarge[i].nSize1);
            CMyJson::ReadIntKey(jsonVal[i],"nSize2",struSuppressLarge[i].nSize2);
            CMyJson::ReadIntKey(jsonVal[i],"nPercent1",struSuppressLarge[i].nPercent1);
            CMyJson::ReadIntKey(jsonVal[i],"nPercent2",struSuppressLarge[i].nPercent2);
            CMyJson::ReadIntKey(jsonVal[i],"nSenIdx",struSuppressLarge[i].nSenIdx);
            CMyJson::ReadIntKey(jsonVal[i],"nTurns",struSuppressLarge[i].nTurns);
        }
    }
    else{

        string strsName;
        if(JSON_FAILED_TYPE == CMyJson::ReadStringKey(jsonVal, "sName", strsName))
            return false;
        memset(struSuppressLarge[0].sName,0,sizeof(struSuppressLarge[0].sName));
        memcpy(struSuppressLarge[0].sName,strsName.data(),strsName.length());
        CMyJson::ReadIntKey(jsonVal,"nArithEnable",struSuppressLarge[0].nArithEnable);
        CMyJson::ReadIntKey(jsonVal,"nDispEnable",struSuppressLarge[0].nDispEnable);
        CMyJson::ReadIntKey(jsonVal,"nSize1",struSuppressLarge[0].nSize1);
        CMyJson::ReadIntKey(jsonVal,"nSize2",struSuppressLarge[0].nSize2);
        CMyJson::ReadIntKey(jsonVal,"nPercent1",struSuppressLarge[0].nPercent1);
        CMyJson::ReadIntKey(jsonVal,"nPercent2",struSuppressLarge[0].nPercent2);
        CMyJson::ReadIntKey(jsonVal,"nSenIdx",struSuppressLarge[0].nSenIdx);
        CMyJson::ReadIntKey(jsonVal,"nTurns",struSuppressLarge[0].nTurns);
    }
    return true;
}

bool JsonDataConvert::JsonTostruWheatSprout(const Json::Value &jsonVal, stru_wheat_sprout &struwheatsprout)
{
    string strsName;
    if(JSON_FAILED_TYPE == CMyJson::ReadStringKey(jsonVal, "sName", strsName))
        return false;
    memset(struwheatsprout.sName,0,sizeof(struwheatsprout.sName));
    memcpy(struwheatsprout.sName,strsName.data(),strsName.length());
    CMyJson::ReadIntKey(jsonVal,"nArithEnable",struwheatsprout.nArithEnable);
    CMyJson::ReadIntKey(jsonVal,"nDispEnable",struwheatsprout.nDispEnable);
    CMyJson::ReadIntKey(jsonVal,"nLow",struwheatsprout.nLow);
    CMyJson::ReadIntKey(jsonVal,"nPercent",struwheatsprout.nPercent);
    CMyJson::ReadIntKey(jsonVal,"nSense",struwheatsprout.nSense);
    CMyJson::ReadIntKey(jsonVal,"nH0",struwheatsprout.nH0);
    CMyJson::ReadIntKey(jsonVal,"nH1",struwheatsprout.nH1);
    return true;
}

bool JsonDataConvert::JsonToStruTeaColor(int arraySize,const Json::Value &jsonVal, stu_tea_color *struTeaColor)
{
    if((int)jsonVal.size() != arraySize){
        arraySize = qMin((int)jsonVal.size(),arraySize);
    }
    for(int i = 0;i < arraySize;i++){
        string strsName;
        if(JSON_FAILED_TYPE == CMyJson::ReadStringKey(jsonVal[i], "sName", strsName))
            return false;
        memset(struTeaColor[i].sName,0,sizeof(struTeaColor[i].sName));
        memcpy(struTeaColor[i].sName,strsName.data(),strsName.length());
        CMyJson::ReadIntKey(jsonVal[i],"nArithEnable",struTeaColor[i].nArithEnable);
        CMyJson::ReadIntKey(jsonVal[i],"nDispEnable",struTeaColor[i].nDispEnable);
        CMyJson::ReadFloatKey(jsonVal[i],"tc",struTeaColor[i].tc);
        CMyJson::ReadFloatKey(jsonVal[i],"ec",struTeaColor[i].ec);
        CMyJson::ReadFloatKey(jsonVal[i],"ac",struTeaColor[i].ac);
        CMyJson::ReadFloatKey(jsonVal[i],"sense",struTeaColor[i].sense);
        CMyJson::ReadIntKey(jsonVal[i],"nRow",struTeaColor[i].nRow);
        CMyJson::ReadIntKey(jsonVal[i],"nColumn",struTeaColor[i].nColumn);
        CMyJson::ReadIntKey(jsonVal[i],"nPercent",struTeaColor[i].nPercent);
        CMyJson::ReadIntKey(jsonVal[i],"nMode",struTeaColor[i].nMode);
        CMyJson::ReadIntKey(jsonVal[i],"nEdge",struTeaColor[i].nEdge);
        CMyJson::ReadIntKey(jsonVal[i],"nInhibit",struTeaColor[i].nInhibit);
        CMyJson::ReadIntKey(jsonVal[i],"nTeaIndex",struTeaColor[i].nTeaIndex);
        CMyJson::ReadIntKey(jsonVal[i],"nTeaMode",struTeaColor[i].nTeaMode);
    }
    return true;
}

bool JsonDataConvert::JsonTostruColorHue(const Json::Value &jsonVal, stru_color_hue &struColorHue)
{
    string strsName;
    if(JSON_FAILED_TYPE == CMyJson::ReadStringKey(jsonVal, "sName", strsName))
        return false;
    memset(struColorHue.sName,0,sizeof(struColorHue.sName));
    memcpy(struColorHue.sName,strsName.data(),strsName.length());
    CMyJson::ReadIntKey(jsonVal,"nArithEnable",struColorHue.nArithEnable);
    CMyJson::ReadIntKey(jsonVal,"nDispEnable",struColorHue.nDispEnable);
    CMyJson::ReadIntKey(jsonVal,"nRow",struColorHue.nRow);
    CMyJson::ReadIntKey(jsonVal,"nColumn",struColorHue.nColumn);
    CMyJson::ReadIntKey(jsonVal,"nPercent",struColorHue.nPercent);
    CMyJson::ReadIntKey(jsonVal,"nHighHue",struColorHue.nHighHue);
    CMyJson::ReadIntKey(jsonVal,"nLowHue",struColorHue.nLowHue);
    CMyJson::ReadIntKey(jsonVal,"lightLimit",struColorHue.lightLimit);
    return true;
}

bool JsonDataConvert::JsonTostruColorScale(const Json::Value &jsonVal, stru_color_scale &struColorScale)
{
    string strsName;
    if(JSON_FAILED_TYPE == CMyJson::ReadStringKey(jsonVal,"sName", strsName))
        return false;
    memset(struColorScale.sName,0,sizeof(struColorScale.sName));
    memcpy(struColorScale.sName,strsName.data(),strsName.length());
    CMyJson::ReadIntKey(jsonVal,"nArithEnable",struColorScale.nArithEnable);
    CMyJson::ReadIntKey(jsonVal,"nDispEnable",struColorScale.nDispEnable);
    CMyJson::ReadIntKey(jsonVal,"nSenIdx",struColorScale.nSenIdx);
    CMyJson::ReadIntKey(jsonVal,"nSen",struColorScale.nSen);
    CMyJson::ReadIntKey(jsonVal,"nLimit",struColorScale.nLimit);
    return true;
}

bool JsonDataConvert::JsonTostruColorValue(const Json::Value &jsonVal, stru_color_value &struColorValue)
{
    string strsName;
    if(JSON_FAILED_TYPE == CMyJson::ReadStringKey(jsonVal, "sName", strsName))
        return false;
    memcpy(struColorValue.sName,strsName.data(),strsName.length());
    CMyJson::ReadIntKey(jsonVal,"nArithEnable",struColorValue.nArithEnable);
    CMyJson::ReadIntKey(jsonVal,"nDispEnable",struColorValue.nDispEnable);
    CMyJson::ReadIntKey(jsonVal,"nRow",struColorValue.nRow);
    CMyJson::ReadIntKey(jsonVal,"nColumn",struColorValue.nColumn);
    CMyJson::ReadIntKey(jsonVal,"nPercent",struColorValue.nPercent);
    CMyJson::ReadIntKey(jsonVal,"nMode",struColorValue.nMode);
    CMyJson::ReadIntKey(jsonVal,"nSensLow",struColorValue.nSensLow);
    CMyJson::ReadIntKey(jsonVal,"nSensUp",struColorValue.nSensUp);
    return true;
}

bool JsonDataConvert::JsonTostruMildew(const Json::Value &jsonVal, stru_mildew &struMildew)
{
    string strsName;
    if(JSON_FAILED_TYPE == CMyJson::ReadStringKey(jsonVal,"sName",strsName))
        return false;
    memset(struMildew.sName,0,sizeof(struMildew.sName));
    memcpy(struMildew.sName,strsName.data(),strsName.length());
    CMyJson::ReadIntKey(jsonVal,"nArithEnable",struMildew.nArithEnable);
    CMyJson::ReadIntKey(jsonVal,"nDispEnable",struMildew.nDispEnable);
    CMyJson::ReadIntKey(jsonVal,"nPercent",struMildew.nPercent);
    CMyJson::ReadIntKey(jsonVal,"nSens",struMildew.nSens);
    CMyJson::ReadIntKey(jsonVal,"nMark",struMildew.nMark);
    CMyJson::ReadIntKey(jsonVal,"nLimit",struMildew.nLimit);
    CMyJson::ReadIntKey(jsonVal,"nType",struMildew.nType);
    return true;
}

bool JsonDataConvert::JsonTostruSplMildew(const Json::Value &jsonVal, stru_spl_mildew &struSplMildew)
{
    string strsName;
    if(JSON_FAILED_TYPE == CMyJson::ReadStringKey(jsonVal,"sName",strsName))
        return false;
    memset(struSplMildew.sName,0,sizeof(struSplMildew.sName));
    memcpy(struSplMildew.sName,strsName.data(),strsName.length());
    CMyJson::ReadIntKey(jsonVal,"nArithEnable",struSplMildew.nArithEnable);
    CMyJson::ReadIntKey(jsonVal,"nDispEnable",struSplMildew.nDispEnable);
    CMyJson::ReadIntKey(jsonVal,"nSens",struSplMildew.nSens);
    return true;
}

bool JsonDataConvert::JsonTostruOutline(const Json::Value &jsonVal, stru_outline &struOutline)
{
    string strsName;
    if(JSON_FAILED_TYPE == CMyJson::ReadStringKey(jsonVal,"sName",strsName))
        return false;
    memset(struOutline.sName,0,sizeof(struOutline.sName));
    memcpy(struOutline.sName,strsName.data(),strsName.length());
    CMyJson::ReadIntKey(jsonVal,"nArithEnable",struOutline.nArithEnable);
    CMyJson::ReadIntKey(jsonVal,"nDispEnable",struOutline.nDispEnable);
    CMyJson::ReadIntKey(jsonVal,"nSens",struOutline.nSens);
    CMyJson::ReadIntKey(jsonVal,"nPercent",struOutline.nPercent);
    CMyJson::ReadIntKey(jsonVal,"nHighLimit",struOutline.nHighLimit);
    CMyJson::ReadIntKey(jsonVal,"nLowLimit",struOutline.nLowLimit);
    return true;
}

bool JsonDataConvert::JsonTostruLine(const Json::Value &jsonVal, stru_line &struLine)
{
    string strsName;
    if(JSON_FAILED_TYPE == CMyJson::ReadStringKey(jsonVal,"sName",strsName))
        return false;
    memset(struLine.sName,0,sizeof(struLine.sName));
    memcpy(struLine.sName,strsName.data(),strsName.length());
    CMyJson::ReadIntKey(jsonVal,"nArithEnable",struLine.nArithEnable);
    CMyJson::ReadIntKey(jsonVal,"nDispEnable",struLine.nDispEnable);
    CMyJson::ReadIntKey(jsonVal,"nColor",struLine.nColor);
    CMyJson::ReadIntKey(jsonVal,"nSize",struLine.nSize);
    CMyJson::ReadIntKey(jsonVal,"nRow",struLine.nRow);
    CMyJson::ReadIntKey(jsonVal,"nPercent",struLine.nPercent);
    CMyJson::ReadIntKey(jsonVal,"nColumn",struLine.nColumn);
    CMyJson::ReadIntKey(jsonVal,"nSens1",struLine.nSens1);
    CMyJson::ReadIntKey(jsonVal,"nSens2",struLine.nSens2);
    return true;
}

bool JsonDataConvert::JsonTostruBud(int arraySize,const Json::Value &jsonVal, stru_bud *struBud)
{
    for(int i = 0;i < arraySize;i++){
        string strsName;
        if(JSON_FAILED_TYPE == CMyJson::ReadStringKey(jsonVal[i],"sName",strsName))
            return false;
        memset(struBud[i].sName,0,sizeof(struBud[i].sName));
        memcpy(struBud[i].sName,strsName.data(),strsName.length());
        CMyJson::ReadIntKey(jsonVal[i],"nArithEnable",struBud[i].nArithEnable);
        CMyJson::ReadIntKey(jsonVal[i],"nDispEnable",struBud[i].nDispEnable);
        CMyJson::ReadIntKey(jsonVal[i],"nSens",struBud[i].nSens);
        CMyJson::ReadIntKey(jsonVal[i],"nSize",struBud[i].nSize);
        CMyJson::ReadIntKey(jsonVal[i],"nLimit",struBud[i].nLimit);
        CMyJson::ReadIntKey(jsonVal[i],"nYellowSens",struBud[i].nYellowSens);
        CMyJson::ReadIntKey(jsonVal[i],"nBlackSens",struBud[i].nBlackSens);
    }
    return true;
}

bool JsonDataConvert::JsonTostruRound(int arraySize,const Json::Value &jsonVal, stru_round *struRound)
{
    if (jsonVal.isArray()){
        for(int i = 0;i < arraySize;i++){
            string strsName;
            if(JSON_FAILED_TYPE == CMyJson::ReadStringKey(jsonVal[i],"sName",strsName))
                return false;
            memset(struRound[i].sName,0,sizeof(struRound[i].sName));
            memcpy(struRound[i].sName,strsName.data(),strsName.length());
            CMyJson::ReadIntKey(jsonVal[i],"nArithEnable",struRound[i].nArithEnable);
            CMyJson::ReadIntKey(jsonVal[i],"nDispEnable",struRound[i].nDispEnable);
            CMyJson::ReadIntKey(jsonVal[i],"nSensUp",struRound[i].nSensUp);
            CMyJson::ReadIntKey(jsonVal[i],"nSensLow",struRound[i].nSensLow);
            CMyJson::ReadIntKey(jsonVal[i],"nLimit",struRound[i].nLimit);
            CMyJson::ReadIntKey(jsonVal[i],"nSelectMode",struRound[i].nSelectMode);
        }
    }
    else{
        string strsName;
        if(JSON_FAILED_TYPE == CMyJson::ReadStringKey(jsonVal,"sName",strsName))
            return false;
        memset(struRound[0].sName,0,sizeof(struRound[0].sName));
        memcpy(struRound[0].sName,strsName.data(),strsName.length());
        CMyJson::ReadIntKey(jsonVal,"nArithEnable",struRound[0].nArithEnable);
        CMyJson::ReadIntKey(jsonVal,"nDispEnable",struRound[0].nDispEnable);
        CMyJson::ReadIntKey(jsonVal,"nSensUp",struRound[0].nSensUp);
        CMyJson::ReadIntKey(jsonVal,"nSensLow",struRound[0].nSensLow);
        CMyJson::ReadIntKey(jsonVal,"nLimit",struRound[0].nLimit);
        CMyJson::ReadIntKey(jsonVal,"nSelectMode",struRound[0].nSelectMode);
    }

    return true;
}

bool JsonDataConvert::JsonTostruHsv(int arraySize, const Json::Value &jsonVal, stu_hsv *struHsv)
{
    for(int i = 0;i < arraySize;i++)
    {
        string strsName;
        if(JSON_FAILED_TYPE == CMyJson::ReadStringKey(jsonVal[i], "sName", strsName))
            return false;
        memset(struHsv[i].sName,0,sizeof(struHsv[i].sName));
        memcpy(struHsv[i].sName,strsName.data(),strsName.length());
        CMyJson::ReadIntKey(jsonVal[i],"nArithEnable",struHsv[i].nArithEnable);
        CMyJson::ReadIntKey(jsonVal[i],"nDispEnable",struHsv[i].nDispEnable);
        CMyJson::ReadIntKey(jsonVal[i],"nVavle",struHsv[i].nVavle);
        CMyJson::ReadIntKey(jsonVal[i],"nRow",struHsv[i].nRow);
        CMyJson::ReadIntKey(jsonVal[i],"nColumn",struHsv[i].nColumn);
        CMyJson::ReadIntKey(jsonVal[i],"nPercent",struHsv[i].nPercent);
        CMyJson::ReadIntKey(jsonVal[i],"nHsvColor",struHsv[i].nHsvColor);
        CMyJson::ReadIntKey(jsonVal[i],"nHsvMode",struHsv[i].nHsvMode);
        CMyJson::ReadIntKey(jsonVal[i],"nHsvSensLowH",struHsv[i].nHsvSensLowH);
        CMyJson::ReadIntKey(jsonVal[i],"nHsvSensUpH",struHsv[i].nHsvSensUpH);
        CMyJson::ReadIntKey(jsonVal[i],"nHsvSensLowS",struHsv[i].nHsvSensLowS);
        CMyJson::ReadIntKey(jsonVal[i],"nHsvSensUpS",struHsv[i].nHsvSensUpS);
        CMyJson::ReadIntKey(jsonVal[i],"nHsvSensLowV",struHsv[i].nHsvSensLowV);
        CMyJson::ReadIntKey(jsonVal[i],"nHsvSensUpV",struHsv[i].nHsvSensUpV);
        CMyJson::ReadIntKey(jsonVal[i],"delayType",struHsv[i].delayType);
        JsonTostruReserved(jsonVal[i]["struHsv"],struHsv[i].struResGreyColor);

    }
    return true;
}

bool JsonDataConvert::JsonTostruGroupCtrl(int arraySize, const Json::Value &jsonVal, stu_group_ctrl *struGroupCtrl)
{
    INT_ARRAY_KEY outArray;
    vector<int> intVec;
    intVec.push_back(MAX_FEEDER);
    for(int i = 0; i < arraySize; i++)
    {
        CMyJson::ReadIntArrayKey(jsonVal[i],"nFeederValueMain",intVec,struGroupCtrl[i].nFeederValueMain,outArray);
        CMyJson::ReadIntArrayKey(jsonVal[i],"nFeederValueVice",intVec,struGroupCtrl[i].nFeederValueVice,outArray);
        CMyJson::ReadIntKey(jsonVal[i],"nWipeDuration",struGroupCtrl[i].nWipeDuration);
        CMyJson::ReadIntKey(jsonVal[i],"nWipeInterval",struGroupCtrl[i].nWipeInterval);
        CMyJson::ReadIntKey(jsonVal[i],"nWipeUnion",struGroupCtrl[i].nWipeUnion);
        CMyJson::ReadIntKey(jsonVal[i],"nWipeTimes",struGroupCtrl[i].nWipeTimes);
        CMyJson::ReadIntKey(jsonVal[i],"nWipeWindDuration",struGroupCtrl[i].nWipeWindDuration);
        CMyJson::ReadIntKey(jsonVal[i],"nWipeWindInterval",struGroupCtrl[i].nWipeWindInterval);
        CMyJson::ReadIntKey(jsonVal[i],"nWipeWaterDuration",struGroupCtrl[i].nWipeWaterDuration);
        CMyJson::ReadIntKey(jsonVal[i],"nConveyorIsRun",struGroupCtrl[i].nConveyorIsRun);
        CMyJson::ReadIntKey(jsonVal[i],"nConveyorSpeed",struGroupCtrl[i].nConveyorSpeed);
        CMyJson::ReadIntKey(jsonVal[i],"nBackgroundFront",struGroupCtrl[i].nBackgroundFront);
        CMyJson::ReadIntKey(jsonVal[i],"nBackgroundRear",struGroupCtrl[i].nBackgroundRear);
        CMyJson::ReadIntKey(jsonVal[i],"nFeedSetType",struGroupCtrl[i].nFeedSetType);
        CMyJson::ReadIntKey(jsonVal[i],"nFeederTea",struGroupCtrl[i].nFeederTea);
        CMyJson::ReadIntKey(jsonVal[i],"nWipeSortOnOff",struGroupCtrl[i].nWipeSortOnOff);
        CMyJson::ReadIntKey(jsonVal[i],"nWipeFeedOnOff",struGroupCtrl[i].nWipeFeedOnOff);
    }
    return true;
}

bool JsonDataConvert::JsonTostruReserved(const Json::Value &jsonVal, stu_res_grey_color &struReserved)
{
    CMyJson::ReadIntKey(jsonVal,"nColor1",struReserved.nColor1);
    CMyJson::ReadIntKey(jsonVal,"nSensLow1",struReserved.nSensLow1);
    CMyJson::ReadIntKey(jsonVal,"nSensUp1",struReserved.nSensUp1);
    CMyJson::ReadIntKey(jsonVal,"nMode1",struReserved.nMode1);
    CMyJson::ReadIntKey(jsonVal,"nGreyResEnable",struReserved.nGreyResEnable);
    CMyJson::ReadIntKey(jsonVal,"nMain2",struReserved.nMain2);
    CMyJson::ReadIntKey(jsonVal,"nVice2",struReserved.nVice2);
    CMyJson::ReadIntKey(jsonVal,"nResDiffColor",struReserved.nResDiffColor);
    CMyJson::ReadIntKey(jsonVal,"nSensLow2",struReserved.nSensLow2);
    CMyJson::ReadIntKey(jsonVal,"nSensUp2",struReserved.nSensUp2);
    CMyJson::ReadIntKey(jsonVal,"nMode2",struReserved.nMode2);
    CMyJson::ReadIntKey(jsonVal,"nDiscolorResEnable",struReserved.nDiscolorResEnable);
    return true;
}

bool JsonDataConvert::struGreyColorToJson(int arraySize, Json::Value &jsonVal, const stu_color_arith *struGreyColor)
{
    for(int i = 0;i<arraySize;i++){
        string strsName = QString::fromUtf8(struGreyColor[i].sName).toStdString();
        CMyJson::WriteStringKey("sName",strsName,jsonVal[i]);
        CMyJson::WriteIntKey("nArithEnable",struGreyColor[i].nArithEnable,jsonVal[i]);
        CMyJson::WriteIntKey("nDispEnable",struGreyColor[i].nDispEnable,jsonVal[i]);
        //        CMyJson::WriteIntKey("nEnable",struGreyColor[i].nEnable,jsonVal[i]);
        CMyJson::WriteIntKey("nVavle",struGreyColor[i].nVavle,jsonVal[i]);
        CMyJson::WriteIntKey("nRow",struGreyColor[i].nRow,jsonVal[i]);
        CMyJson::WriteIntKey("nColumn",struGreyColor[i].nColumn,jsonVal[i]);
        CMyJson::WriteIntKey("nPercent",struGreyColor[i].nPercent,jsonVal[i]);
        CMyJson::WriteIntKey("nMain",struGreyColor[i].nMain,jsonVal[i]);
        CMyJson::WriteIntKey("nVice",struGreyColor[i].nVice,jsonVal[i]);
        CMyJson::WriteIntKey("nDiffColor",struGreyColor[i].nDiffColor,jsonVal[i]);
        CMyJson::WriteIntKey("nSensLow",struGreyColor[i].nSensLow,jsonVal[i]);
        CMyJson::WriteIntKey("nSensUp",struGreyColor[i].nSensUp,jsonVal[i]);
        CMyJson::WriteFloatKey("fPropotionSensLow",struGreyColor[i].fPropotionSensLow,jsonVal[i]);
        CMyJson::WriteFloatKey("fPropotionSensUp",struGreyColor[i].fPropotionSensUp,jsonVal[i]);
        CMyJson::WriteIntKey("nMode",struGreyColor[i].nMode,jsonVal[i]);
        CMyJson::WriteIntKey("lightLimit",struGreyColor[i].lightLimit,jsonVal[i]);
        CMyJson::WriteIntKey("nLineDefectEnable",struGreyColor[i].nLineDefectEnable,jsonVal[i]);
        CMyJson::WriteIntKey("nLineDefectIndex",struGreyColor[i].nLineDefectIndex,jsonVal[i]);
        CMyJson::WriteIntKey("fnLineDefectAreaLow",struGreyColor[i].fnLineDefectAreaLow,jsonVal[i]);
        CMyJson::WriteIntKey("fnLineDefectAreaUp",struGreyColor[i].fnLineDefectAreaUp,jsonVal[i]);
        CMyJson::WriteIntKey("fnLineDefectLengthLow",struGreyColor[i].fnLineDefectLengthLow,jsonVal[i]);
        CMyJson::WriteIntKey("fnLineDefectLengthUp",struGreyColor[i].fnLineDefectLengthUp,jsonVal[i]);

        CMyJson::WriteIntKey("nResType",struGreyColor[i].nResType,jsonVal[i]);
        CMyJson::WriteIntKey("nSensUpdate",struGreyColor[i].nSensUpdate,jsonVal[i]);
        CMyJson::WriteIntKey("nEdgeType",struGreyColor[i].nEdgeType,jsonVal[i]);
        CMyJson::WriteIntKey("nImpureType",struGreyColor[i].nImpureType,jsonVal[i]);
        CMyJson::WriteIntKey("nPropotionEnable",struGreyColor[i].nPropotionEnable,jsonVal[i]);
        CMyJson::WriteFloatKey("nPropResSens",struGreyColor[i].nPropResSens,jsonVal[i]);

        CMyJson::WriteIntKey("nEdge",struGreyColor[i].nEdge,jsonVal[i]);
        CMyJson::WriteIntKey("nPropotionParaSensLow",struGreyColor[i].nPropotionParaSensLow,jsonVal[i]);
        CMyJson::WriteIntKey("delayType",struGreyColor[i].delayType,jsonVal[i]);
        struReservedToJson(jsonVal[i]["struResGreyColor"],struGreyColor[i].struResGreyColor);
    }
    return true;
}

bool JsonDataConvert::struIntelToJson(int arraySize, Json::Value &jsonVal, const stu_svm *struSvm)
{
    vector<int> intVec;
    intVec.clear();
    intVec.push_back(2);
    for(int i = 0;i<arraySize;i++){
        string strsName = QString::fromUtf8(struSvm[i].sName).toStdString();
        CMyJson::WriteStringKey("sName",strsName,jsonVal[i]);
        CMyJson::WriteIntKey("nArithEnable",struSvm[i].nArithEnable,jsonVal[i]);
        CMyJson::WriteIntKey("nDispEnable",struSvm[i].nDispEnable,jsonVal[i]);
        //        CMyJson::WriteIntKey("nEnable",struSvm[i].nEnable,jsonVal[i]);
        CMyJson::WriteIntKey("nTypeIndex",struSvm[i].nTypeIndex,jsonVal[i]);
        CMyJson::WriteIntKey("nVavle",struSvm[i].nVavle,jsonVal[i]);
        CMyJson::WriteIntKey("nSizeType",struSvm[i].nSizeType,jsonVal[i]);
        CMyJson::WriteIntKey("nRow",struSvm[i].nRow,jsonVal[i]);
        CMyJson::WriteIntKey("nColumn",struSvm[i].nColumn,jsonVal[i]);
        CMyJson::WriteIntKey("nPercent",struSvm[i].nPercent,jsonVal[i]);
        CMyJson::WriteIntKey("nPropotionEnable",struSvm[i].nPropotionEnable,jsonVal[i]);
        CMyJson::WriteIntKey("nEdgeType",struSvm[i].nEdgeType,jsonVal[i]);
        CMyJson::WriteIntKey("nImpureType",struSvm[i].nImpureType,jsonVal[i]);

        CMyJson::WriteIntKey("nSens",struSvm[i].nSens,jsonVal[i]);
        CMyJson::WriteIntKey("nModeSvm",struSvm[i].nModeSvm,jsonVal[i]);
        CMyJson::WriteFloatKey("fPropotionSensLow",struSvm[i].fPropotionSensLow,jsonVal[i]);
        CMyJson::WriteFloatKey("fPropotionSensUp",struSvm[i].fPropotionSensUp,jsonVal[i]);
        CMyJson::WriteIntKey("nPropotionParaSensLow",struSvm[i].nPropotionParaSensLow,jsonVal[i]);

        CMyJson::WriteIntKey("nLineDefectEnable",struSvm[i].nLineDefectEnable,jsonVal[i]);
        CMyJson::WriteIntKey("nLineDefectIndex",struSvm[i].nLineDefectIndex,jsonVal[i]);
        CMyJson::WriteFloatKey("fnLineDefectAreaLow",struSvm[i].fnLineDefectAreaLow,jsonVal[i]);
        CMyJson::WriteFloatKey("fnLineDefectAreaUp",struSvm[i].fnLineDefectAreaUp,jsonVal[i]);
        CMyJson::WriteFloatKey("fnLineDefectLengthLow",struSvm[i].fnLineDefectLengthLow,jsonVal[i]);
        CMyJson::WriteFloatKey("fnLineDefectLengthUp",struSvm[i].fnLineDefectLengthUp,jsonVal[i]);

        CMyJson::WriteIntArrayKey("nQuoCons",intVec,(int *)struSvm[i].nQuoCons,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nQuoR",intVec,(int *)struSvm[i].nQuoR,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nQuoG",intVec,(int *)struSvm[i].nQuoG,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nQuoB",intVec,(int *)struSvm[i].nQuoB,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nQuoN",intVec,(int *)struSvm[i].nQuoN,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nQuoRG",intVec,(int *)struSvm[i].nQuoRG,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nQuoRB",intVec,(int *)struSvm[i].nQuoRB,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nQuoGB",intVec,(int *)struSvm[i].nQuoGB,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nQuoRR",intVec,(int *)struSvm[i].nQuoRR,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nQuoGG",intVec,(int *)struSvm[i].nQuoGG,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nQuoBB",intVec,(int *)struSvm[i].nQuoBB,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nQuoRN",intVec,(int *)struSvm[i].nQuoRN,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nQuoGN",intVec,(int *)struSvm[i].nQuoGN,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nQuoBN",intVec,(int *)struSvm[i].nQuoBN,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nQuoNN",intVec,(int *)struSvm[i].nQuoNN,jsonVal[i]);

        CMyJson::WriteIntKey("nEdge",struSvm[i].nEdge,jsonVal[i]);
        CMyJson::WriteIntKey("nModeType",struSvm[i].nModeType,jsonVal[i]);
        CMyJson::WriteIntKey("nConsExpandedValue",struSvm[i].nConsExpandedValue,jsonVal[i]);
        CMyJson::WriteIntKey("delayType",struSvm[i].delayType,jsonVal[i]);
        CMyJson::WriteIntKey("maxSenseValue",struSvm[i].maxSenseValue,jsonVal[i]);
        CMyJson::WriteIntKey("impuritPos",struSvm[i].impuritPos,jsonVal[i]);
        CMyJson::WriteIntKey("brightLimit",struSvm[i].brightLimit,jsonVal[i]);
    }
    return true;
}
bool JsonDataConvert::struShapeIntelToJson(int arraySize, Json::Value &jsonVal, const stu_shape_svm *struShapeSvm)
{
    vector<int> intVec;
    intVec.clear();
    intVec.push_back(2);
    for(int i = 0;i<arraySize;i++){
        string strsName = QString::fromUtf8(struShapeSvm[i].sName).toStdString();
        CMyJson::WriteStringKey("sName",strsName,jsonVal[i]);
        CMyJson::WriteIntKey("nArithEnable",struShapeSvm[i].nArithEnable,jsonVal[i]);
        CMyJson::WriteIntKey("nDispEnable",struShapeSvm[i].nDispEnable,jsonVal[i]);
        CMyJson::WriteIntKey("nAreaLow",struShapeSvm[i].nAreaLow,jsonVal[i]);
        CMyJson::WriteIntKey("nDispEnable",struShapeSvm[i].nDispEnable,jsonVal[i]);

        CMyJson::WriteIntKey("nSens",struShapeSvm[i].nSens,jsonVal[i]);
        CMyJson::WriteIntKey("nModeSvm",struShapeSvm[i].nModeSvm,jsonVal[i]);

        CMyJson::WriteIntArrayKey("nQuoCons",intVec,(int *)struShapeSvm[i].nQuoCons,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nQuoA",intVec,(int *)struShapeSvm[i].nQuoA,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nQuoB",intVec,(int *)struShapeSvm[i].nQuoB,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nQuoC",intVec,(int *)struShapeSvm[i].nQuoC,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nQuoD",intVec,(int *)struShapeSvm[i].nQuoD,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nQuoE",intVec,(int *)struShapeSvm[i].nQuoE,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nQuoF",intVec,(int *)struShapeSvm[i].nQuoF,jsonVal[i]);

        CMyJson::WriteIntKey("nEdge",struShapeSvm[i].nEdge,jsonVal[i]);
    }

    return true;
}

#if 0
bool JsonDataConvert::struCrossToJson(Json::Value &jsonVal, const stu_cs &struCross)
{
    string strsName = QString::fromUtf8(struCross.sName).toStdString();
    CMyJson::WriteStringKey("sName",strsName,jsonVal);
    CMyJson::WriteIntKey("nVavle",struCross.nVavle,jsonVal);
    CMyJson::WriteIntKey("nRow",struCross.nRow,jsonVal);
    CMyJson::WriteIntKey("nColumn",struCross.nColumn,jsonVal);
    CMyJson::WriteIntKey("nPercent",struCross.nPercent,jsonVal);
    CMyJson::WriteIntKey("nSens",struCross.nSens,jsonVal);
    CMyJson::WriteIntKey("nColor",struCross.nColor,jsonVal);
    return true;
}
#endif

bool JsonDataConvert::struCircleLongToJson(int arraySize,Json::Value &jsonVal, const stu_circle_long *struCircleLong)
{
    for(int i = 0;i<arraySize;i++){
        QByteArray sname(struCircleLong[i].sName);
        string strsName = QString::fromUtf8(sname).toStdString();
        CMyJson::WriteStringKey("sName",strsName,jsonVal[i]);
        CMyJson::WriteIntKey("nArithEnable",struCircleLong[i].nArithEnable,jsonVal[i]);
        CMyJson::WriteIntKey("nDispEnable",struCircleLong[i].nDispEnable,jsonVal[i]);
        //    CMyJson::WriteIntKey("nEnable",struCircleLong.nEnable,jsonVal);
        CMyJson::WriteIntKey("nTypeIndex",struCircleLong[i].nTypeIndex,jsonVal[i]);
        CMyJson::WriteIntKey("nVavle",struCircleLong[i].nVavle,jsonVal[i]);
        CMyJson::WriteIntKey("nCiloLow",struCircleLong[i].nCiloLow,jsonVal[i]);
        CMyJson::WriteIntKey("nCilosUp",struCircleLong[i].nCilosUp,jsonVal[i]);
        CMyJson::WriteIntKey("nMode",struCircleLong[i].nMode,jsonVal[i]);
        CMyJson::WriteIntKey("nLimit",struCircleLong[i].nLimit,jsonVal[i]);
        CMyJson::WriteIntKey("delayType",struCircleLong[i].delayType,jsonVal[i]);
    }
    return true;
}

bool JsonDataConvert::struLongShortToJson(int arraySize,Json::Value &jsonVal, const stu_long_short *struLongShort)
{
    for(int i = 0;i<arraySize;i++){
        string strsName = QString::fromUtf8(struLongShort[i].sName).toStdString();
        CMyJson::WriteStringKey("sName",strsName,jsonVal[i]);
        CMyJson::WriteIntKey("nArithEnable",struLongShort[i].nArithEnable,jsonVal[i]);
        CMyJson::WriteIntKey("nDispEnable",struLongShort[i].nDispEnable,jsonVal[i]);
        //    CMyJson::WriteIntKey("nEnable",struLongShort.nEnable,jsonVal);
        CMyJson::WriteIntKey("nTypeIndex",struLongShort[i].nTypeIndex,jsonVal[i]);
        CMyJson::WriteIntKey("nVavle",struLongShort[i].nVavle,jsonVal[i]);
        CMyJson::WriteIntKey("nLengthLow",struLongShort[i].nLengthLow,jsonVal[i]);
        CMyJson::WriteIntKey("nLengthUp",struLongShort[i].nLengthUp,jsonVal[i]);
        CMyJson::WriteIntKey("nLengthMode",struLongShort[i].nLengthMode,jsonVal[i]);
        CMyJson::WriteIntKey("nLimit",struLongShort[i].nLimit,jsonVal[i]);
        CMyJson::WriteIntKey("delayType",struLongShort[i].delayType,jsonVal[i]);
    }
    return true;
}

bool JsonDataConvert::struBigSmallToJson(int arraySize,Json::Value &jsonVal, const stu_big_small *struBigSmall)
{
    for(int i = 0;i<arraySize;i++){
        string strsName = QString::fromUtf8(struBigSmall[i].sName).toStdString();
        CMyJson::WriteStringKey("sName",strsName,jsonVal[i]);
        CMyJson::WriteIntKey("nArithEnable",struBigSmall[i].nArithEnable,jsonVal[i]);
        CMyJson::WriteIntKey("nDispEnable",struBigSmall[i].nDispEnable,jsonVal[i]);
        //    CMyJson::WriteIntKey("nEnable",struBigSmall.nEnable,jsonVal);
        CMyJson::WriteIntKey("nTypeIndex",struBigSmall[i].nTypeIndex,jsonVal[i]);
        CMyJson::WriteIntKey("nVavle",struBigSmall[i].nVavle,jsonVal[i]);
        CMyJson::WriteIntKey("nMode",struBigSmall[i].nMode,jsonVal[i]);
        CMyJson::WriteIntKey("nAreaLow",struBigSmall[i].nAreaLow,jsonVal[i]);
        CMyJson::WriteIntKey("nAreaUp",struBigSmall[i].nAreaUp,jsonVal[i]);
        CMyJson::WriteIntKey("nLimit",struBigSmall[i].nLimit,jsonVal[i]);
        CMyJson::WriteIntKey("delayType",struBigSmall[i].delayType,jsonVal[i]);
    }
    return true;
}

bool JsonDataConvert::struHsvToJson(int arraySize,Json::Value &jsonVal, const stu_hsv *struHsv)
{
    for(int i = 0;i<arraySize;i++)
    {
        string strsName = QString::fromUtf8(struHsv[i].sName).toStdString();
        CMyJson::WriteStringKey("sName",strsName,jsonVal[i]);
        CMyJson::WriteIntKey("nArithEnable",struHsv[i].nArithEnable,jsonVal[i]);
        CMyJson::WriteIntKey("nDispEnable",struHsv[i].nDispEnable,jsonVal[i]);
        CMyJson::WriteIntKey("nVavle",struHsv[i].nVavle,jsonVal[i]);
        CMyJson::WriteIntKey("nRow",struHsv[i].nRow,jsonVal[i]);
        CMyJson::WriteIntKey("nColumn",struHsv[i].nColumn,jsonVal[i]);
        CMyJson::WriteIntKey("nPercent",struHsv[i].nPercent,jsonVal[i]);
        //CMyJson::WriteIntKey("nRadioMode",struHsv.nRadioMode,jsonVal);
        CMyJson::WriteIntKey("nHsvColor",struHsv[i].nHsvColor,jsonVal[i]);
        CMyJson::WriteIntKey("nHsvMode",struHsv[i].nHsvMode,jsonVal[i]);
        CMyJson::WriteIntKey("nHsvSensLowH",struHsv[i].nHsvSensLowH,jsonVal[i]);
        CMyJson::WriteIntKey("nHsvSensUpH",struHsv[i].nHsvSensUpH,jsonVal[i]);
        CMyJson::WriteIntKey("nHsvSensLowS",struHsv[i].nHsvSensLowS,jsonVal[i]);
        CMyJson::WriteIntKey("nHsvSensUpS",struHsv[i].nHsvSensUpS,jsonVal[i]);
        CMyJson::WriteIntKey("nHsvSensLowV",struHsv[i].nHsvSensLowV,jsonVal[i]);
        CMyJson::WriteIntKey("nHsvSensUpV",struHsv[i].nHsvSensUpV,jsonVal[i]);
        CMyJson::WriteIntKey("delayType",struHsv[i].delayType,jsonVal[i]);
        struReservedToJson(jsonVal[i]["struHsv"],struHsv[i].struResGreyColor);

    }
    return true;
}

bool JsonDataConvert::struColorSatToJson(Json::Value &jsonVal, const stru_color_sat &struColorSat)
{
    string strsName = QString::fromUtf8(struColorSat.sName).toStdString();
    CMyJson::WriteStringKey("sName",strsName,jsonVal);
    CMyJson::WriteIntKey("nArithEnable",struColorSat.nArithEnable,jsonVal);
    CMyJson::WriteIntKey("nDispEnable",struColorSat.nDispEnable,jsonVal);
    CMyJson::WriteIntKey("nVavle",struColorSat.nVavle,jsonVal);
    CMyJson::WriteIntKey("nRow",struColorSat.nRow,jsonVal);
    CMyJson::WriteIntKey("nColumn",struColorSat.nColumn,jsonVal);
    CMyJson::WriteIntKey("nPercent",struColorSat.nPercent,jsonVal);
    CMyJson::WriteIntKey("nMode",struColorSat.nMode,jsonVal);
    CMyJson::WriteIntKey("nSensLow",struColorSat.nSensLow,jsonVal);
    CMyJson::WriteIntKey("nSensUp",struColorSat.nSensUp,jsonVal);
    CMyJson::WriteIntKey("lightLimit",struColorSat.lightLimit,jsonVal);
    return true;
}

bool JsonDataConvert::struWhitePeanutToJson(Json::Value &jsonVal, const stru_white_peanut &struWhitePeanut)
{
    string strsName = QString::fromUtf8(struWhitePeanut.sName).toStdString();
    CMyJson::WriteStringKey("sName",strsName,jsonVal);
    CMyJson::WriteIntKey("nArithEnable",struWhitePeanut.nArithEnable,jsonVal);
    CMyJson::WriteIntKey("nDispEnable",struWhitePeanut.nDispEnable,jsonVal);
    CMyJson::WriteIntKey("nSensUp",struWhitePeanut.nSensUp,jsonVal);
    CMyJson::WriteIntKey("nSensLow",struWhitePeanut.nSensLow,jsonVal);
    return true;
}

bool JsonDataConvert::struPepperHandleToJson(Json::Value &jsonVal, const stru_pepper_handle &struPepperHandle)
{
    string strsName = QString::fromUtf8(struPepperHandle.sName).toStdString();
    CMyJson::WriteStringKey("sName",strsName,jsonVal);
    CMyJson::WriteIntKey("nArithEnable",struPepperHandle.nArithEnable,jsonVal);
    CMyJson::WriteIntKey("nDispEnable",struPepperHandle.nDispEnable,jsonVal);
    CMyJson::WriteIntKey("nSens",struPepperHandle.nSens,jsonVal);
    CMyJson::WriteIntKey("nPercent",struPepperHandle.nPercent,jsonVal);
    CMyJson::WriteIntKey("nLimit",struPepperHandle.nLimit,jsonVal);
    return true;
}

bool JsonDataConvert::struSuppressLargeToJson(int arraySize,Json::Value &jsonVal,const stru_suppress_large *struSuppressLarge)
{
    for(int i = 0;i<arraySize;i++){
        string strsName = QString::fromUtf8(struSuppressLarge[i].sName).toStdString();
        CMyJson::WriteStringKey("sName",strsName,jsonVal[i]);
        CMyJson::WriteIntKey("nArithEnable",struSuppressLarge[i].nArithEnable,jsonVal[i]);
        CMyJson::WriteIntKey("nDispEnable",struSuppressLarge[i].nDispEnable,jsonVal[i]);
        CMyJson::WriteIntKey("nSize1",struSuppressLarge[i].nSize1,jsonVal[i]);
        CMyJson::WriteIntKey("nSize2",struSuppressLarge[i].nSize2,jsonVal[i]);
        CMyJson::WriteIntKey("nPercent1",struSuppressLarge[i].nPercent1,jsonVal[i]);
        CMyJson::WriteIntKey("nPercent2",struSuppressLarge[i].nPercent2,jsonVal[i]);
        CMyJson::WriteIntKey("nSenIdx",struSuppressLarge[i].nSenIdx,jsonVal[i]);
        CMyJson::WriteIntKey("nTurns",struSuppressLarge[i].nTurns,jsonVal[i]);
    }
    return true;
}
bool JsonDataConvert::struWhearSproutToJson(Json::Value &jsonVal, const stru_wheat_sprout &struWheatSprout)
{
    string strsName = QString::fromUtf8(struWheatSprout.sName).toStdString();
    CMyJson::WriteStringKey("sName",strsName,jsonVal);
    CMyJson::WriteIntKey("nArithEnable",struWheatSprout.nArithEnable,jsonVal);
    CMyJson::WriteIntKey("nDispEnable",struWheatSprout.nDispEnable,jsonVal);
    CMyJson::WriteIntKey("nLow",struWheatSprout.nLow,jsonVal);
    CMyJson::WriteIntKey("nPercent",struWheatSprout.nPercent,jsonVal);
    CMyJson::WriteIntKey("nSense",struWheatSprout.nSense,jsonVal);
    CMyJson::WriteIntKey("nH0",struWheatSprout.nH0,jsonVal);
    CMyJson::WriteIntKey("nH1",struWheatSprout.nH1,jsonVal);
    return true;
}

bool JsonDataConvert::struTeaColorToJson(int arraySize,Json::Value &jsonVal, const stru_tea_color *struTeaColor)
{
    for(int i = 0;i<arraySize;i++){
        string strsName = QString::fromUtf8(struTeaColor[i].sName).toStdString();
        CMyJson::WriteStringKey("sName",strsName,jsonVal[i]);
        CMyJson::WriteIntKey("nArithEnable",struTeaColor[i].nArithEnable,jsonVal[i]);
        CMyJson::WriteIntKey("nDispEnable",struTeaColor[i].nDispEnable,jsonVal[i]);
        CMyJson::WriteFloatKey("tc",struTeaColor[i].tc,jsonVal[i]);
        CMyJson::WriteFloatKey("ec",struTeaColor[i].ec,jsonVal[i]);
        CMyJson::WriteFloatKey("ac",struTeaColor[i].ac,jsonVal[i]);
        CMyJson::WriteFloatKey("sense",struTeaColor[i].sense,jsonVal[i]);
        CMyJson::WriteIntKey("nRow",struTeaColor[i].nRow,jsonVal[i]);
        CMyJson::WriteIntKey("nPercent",struTeaColor[i].nPercent,jsonVal[i]);
        CMyJson::WriteIntKey("nMode",struTeaColor[i].nMode,jsonVal[i]);
        CMyJson::WriteIntKey("nEdge",struTeaColor[i].nEdge,jsonVal[i]);
        CMyJson::WriteIntKey("nInhibit",struTeaColor[i].nInhibit,jsonVal[i]);
        CMyJson::WriteIntKey("nTeaIndex",struTeaColor[i].nTeaIndex,jsonVal[i]);
        CMyJson::WriteIntKey("nTeaMode",struTeaColor[i].nTeaMode,jsonVal[i]);
    }
    return true;
}

bool JsonDataConvert::struColorHueToJson(Json::Value &jsonVal, const stru_color_hue &struColorHue)
{
    string strsName = QString::fromUtf8(struColorHue.sName).toStdString();
    CMyJson::WriteStringKey("sName",strsName,jsonVal);
    CMyJson::WriteIntKey("nArithEnable",struColorHue.nArithEnable,jsonVal);
    CMyJson::WriteIntKey("nDispEnable",struColorHue.nDispEnable,jsonVal);
    CMyJson::WriteIntKey("nRow",struColorHue.nRow,jsonVal);
    CMyJson::WriteIntKey("nColumn",struColorHue.nColumn,jsonVal);
    CMyJson::WriteIntKey("nPercent",struColorHue.nPercent,jsonVal);
    CMyJson::WriteIntKey("nHighHue",struColorHue.nHighHue,jsonVal);
    CMyJson::WriteIntKey("nLowHue",struColorHue.nLowHue,jsonVal);
    CMyJson::WriteIntKey("lightLimit",struColorHue.lightLimit,jsonVal);
    return true;
}

bool JsonDataConvert::struScaleToJson(Json::Value &jsonVal, const stru_color_scale &struScale)
{
    string strsName = QString::fromUtf8(struScale.sName).toStdString();
    CMyJson::WriteStringKey("sName",strsName,jsonVal);
    CMyJson::WriteIntKey("nArithEnable",struScale.nArithEnable,jsonVal);
    CMyJson::WriteIntKey("nDispEnable",struScale.nDispEnable,jsonVal);
    CMyJson::WriteIntKey("nVavle",struScale.nVavle,jsonVal);
    CMyJson::WriteIntKey("nSenIdx",struScale.nSenIdx,jsonVal);
    CMyJson::WriteIntKey("nSen",struScale.nSen,jsonVal);
    CMyJson::WriteIntKey("nLimit",struScale.nLimit,jsonVal);
    return true;
}

bool JsonDataConvert::struColorValueToJson(Json::Value &jsonVal, const stru_color_value &struColorValue)
{
    string strsName = QString::fromUtf8(struColorValue.sName).toStdString();
    CMyJson::WriteStringKey("sName",strsName,jsonVal);
    CMyJson::WriteIntKey("nArithEnable",struColorValue.nArithEnable,jsonVal);
    CMyJson::WriteIntKey("nDispEnable",struColorValue.nDispEnable,jsonVal);
    CMyJson::WriteIntKey("nRow",struColorValue.nRow,jsonVal);
    CMyJson::WriteIntKey("nColumn",struColorValue.nColumn,jsonVal);
    CMyJson::WriteIntKey("nPercent",struColorValue.nPercent,jsonVal);
    CMyJson::WriteIntKey("nMode",struColorValue.nMode,jsonVal);
    CMyJson::WriteIntKey("nSensLow",struColorValue.nSensLow,jsonVal);
    CMyJson::WriteIntKey("nSensUp",struColorValue.nSensUp,jsonVal);
    return true;
}

bool JsonDataConvert::struRoundToJson(int arraySize,Json::Value &jsonVal, const stru_round *struRound)
{
    for(int i = 0;i<arraySize;i++){
        string strsName = QString::fromUtf8(struRound[i].sName).toStdString();
        CMyJson::WriteStringKey("sName",strsName,jsonVal[i]);
        CMyJson::WriteIntKey("nArithEnable",struRound[i].nArithEnable,jsonVal[i]);
        CMyJson::WriteIntKey("nDispEnable",struRound[i].nDispEnable,jsonVal[i]);
        CMyJson::WriteIntKey("nSelectMode",struRound[i].nSelectMode,jsonVal[i]);
        CMyJson::WriteIntKey("nSensUp",struRound[i].nSensUp,jsonVal[i]);
        CMyJson::WriteIntKey("nSensLow",struRound[i].nSensLow,jsonVal[i]);
        CMyJson::WriteIntKey("nLimit",struRound[i].nLimit,jsonVal[i]);
    }
    return true;
}

bool JsonDataConvert::struMildewToJson(Json::Value &jsonVal, const stru_mildew &struMildew)
{
    string strsName = QString::fromUtf8(struMildew.sName).toStdString();
    CMyJson::WriteStringKey("sName", strsName, jsonVal);
    CMyJson::WriteIntKey("nArithEnable", struMildew.nArithEnable, jsonVal);
    CMyJson::WriteIntKey("nDispEnable", struMildew.nDispEnable, jsonVal);
    CMyJson::WriteIntKey("nSens", struMildew.nSens, jsonVal);
    CMyJson::WriteIntKey("nPercent", struMildew.nPercent, jsonVal);
    CMyJson::WriteIntKey("nLimit", struMildew.nLimit, jsonVal);
    CMyJson::WriteIntKey("nMark", struMildew.nMark, jsonVal);
    CMyJson::WriteIntKey("nType", struMildew.nType, jsonVal);
    return true;
}

bool JsonDataConvert::struSplMildewToJson(Json::Value &jsonVal, const stru_spl_mildew &struSplMildew)
{
    string strsName = QString::fromUtf8(struSplMildew.sName).toStdString();
    CMyJson::WriteStringKey("sName", strsName, jsonVal);
    CMyJson::WriteIntKey("nArithEnable", struSplMildew.nArithEnable, jsonVal);
    CMyJson::WriteIntKey("nDispEnable", struSplMildew.nDispEnable, jsonVal);
    CMyJson::WriteIntKey("nSens", struSplMildew.nSens, jsonVal);
    return true;
}

bool JsonDataConvert::struOutlineToJson(Json::Value &jsonVal, const stru_outline &struOutline)
{
    string strsName = QString::fromUtf8(struOutline.sName).toStdString();
    CMyJson::WriteStringKey("sName", strsName, jsonVal);
    CMyJson::WriteIntKey("nArithEnable", struOutline.nArithEnable, jsonVal);
    CMyJson::WriteIntKey("nDispEnable", struOutline.nDispEnable, jsonVal);
    CMyJson::WriteIntKey("nSens", struOutline.nSens, jsonVal);
    CMyJson::WriteIntKey("nPercent", struOutline.nPercent, jsonVal);
    CMyJson::WriteIntKey("nHighLimit", struOutline.nHighLimit, jsonVal);
    CMyJson::WriteIntKey("nLowLimit", struOutline.nLowLimit, jsonVal);
    return true;
}

bool JsonDataConvert::struLineToJson(Json::Value &jsonVal, const stru_line &struLine)
{
    string strsName = QString::fromUtf8(struLine.sName).toStdString();
    CMyJson::WriteStringKey("sName", strsName, jsonVal);
    CMyJson::WriteIntKey("nArithEnable", struLine.nArithEnable, jsonVal);
    CMyJson::WriteIntKey("nDispEnable", struLine.nDispEnable, jsonVal);
    CMyJson::WriteIntKey("nColor", struLine.nColor, jsonVal);
    CMyJson::WriteIntKey("nSize", struLine.nSize, jsonVal);
    CMyJson::WriteIntKey("nRow", struLine.nRow, jsonVal);
    CMyJson::WriteIntKey("nColum", struLine.nColumn, jsonVal);
    CMyJson::WriteIntKey("nPercent", struLine.nPercent, jsonVal);
    CMyJson::WriteIntKey("nSens1", struLine.nSens1, jsonVal);
    CMyJson::WriteIntKey("nSens2", struLine.nSens2, jsonVal);
    return true;
}

bool JsonDataConvert::struBudToJson(int arraySize,Json::Value &jsonVal, const stru_bud *struBud)
{
    for(int i = 0; i<arraySize; i++){
        string strsName = QString::fromUtf8(struBud[i].sName).toStdString();
        CMyJson::WriteStringKey("sName", strsName, jsonVal[i]);
        CMyJson::WriteIntKey("nArithEnable", struBud[i].nArithEnable, jsonVal[i]);
        CMyJson::WriteIntKey("nDispEnable", struBud[i].nDispEnable, jsonVal[i]);
        CMyJson::WriteIntKey("nSize", struBud[i].nSize, jsonVal[i]);
        CMyJson::WriteIntKey("nSens", struBud[i].nSens, jsonVal[i]);
        CMyJson::WriteIntKey("nLimit", struBud[i].nLimit, jsonVal[i]);
        CMyJson::WriteIntKey("nYellowSens", struBud[i].nYellowSens, jsonVal[i]);
        CMyJson::WriteIntKey("nBlackSens", struBud[i].nBlackSens, jsonVal[i]);
    }
    return true;
}

#if 0
bool JsonDataConvert::struBudToJson(int arraySize, Json::Value &jsonVal, const stu_bd *struBud)
{
    vector<int> intVec;
    intVec.clear();
    intVec.push_back(2);
    for(int i = 0;i<arraySize;i++){
        string strsName = QString::fromUtf8(struBud[i].sName).toStdString();
        CMyJson::WriteStringKey("sName",strsName,jsonVal[i]);
        CMyJson::WriteIntKey("nVavle",struBud[i].nVavle,jsonVal[i]);
        CMyJson::WriteIntKey("nRow",struBud[i].nRow,jsonVal[i]);
        CMyJson::WriteIntKey("nColumn",struBud[i].nColumn,jsonVal[i]);
        CMyJson::WriteIntKey("nPercent",struBud[i].nPercent,jsonVal[i]);
        CMyJson::WriteIntKey("nPercentLimit",struBud[i].nPercentLimit,jsonVal[i]);
        CMyJson::WriteIntKey("nArea",struBud[i].nArea,jsonVal[i]);

    }
    return true;
}
#endif
bool JsonDataConvert::struGroupCtrlToJson(int arraySize, Json::Value &jsonVal, const stu_group_ctrl *struGroupCtrl)
{
    vector<int> intVec;
    intVec.push_back(MAX_FEEDER);
    for(int i = 0; i < arraySize; i++)
    {
        CMyJson::WriteIntArrayKey("nFeederValueMain",intVec,struGroupCtrl[i].nFeederValueMain,jsonVal[i]);
        CMyJson::WriteIntArrayKey("nFeederValueVice",intVec,struGroupCtrl[i].nFeederValueVice,jsonVal[i]);

        CMyJson::WriteIntKey("nWipeDuration",struGroupCtrl[i].nWipeDuration,jsonVal[i]);
        CMyJson::WriteIntKey("nWipeInterval",struGroupCtrl[i].nWipeInterval,jsonVal[i]);
        CMyJson::WriteIntKey("nWipeUnion",struGroupCtrl[i].nWipeUnion,jsonVal[i]);
        CMyJson::WriteIntKey("nWipeTimes",struGroupCtrl[i].nWipeTimes,jsonVal[i]);
        CMyJson::WriteIntKey("nWipeWindDuration",struGroupCtrl[i].nWipeWindDuration,jsonVal[i]);
        CMyJson::WriteIntKey("nWipeWindInterval",struGroupCtrl[i].nWipeWindInterval,jsonVal[i]);
        CMyJson::WriteIntKey("nConveyorIsRun",struGroupCtrl[i].nConveyorIsRun,jsonVal[i]);
        CMyJson::WriteIntKey("nConveyorSpeed",struGroupCtrl[i].nConveyorSpeed,jsonVal[i]);
        CMyJson::WriteIntKey("nWipeWaterDuration",struGroupCtrl[i].nWipeWaterDuration,jsonVal[i]);
        CMyJson::WriteIntKey("nBackgroundFront",struGroupCtrl[i].nBackgroundFront,jsonVal[i]);
        CMyJson::WriteIntKey("nBackgroundRear",struGroupCtrl[i].nBackgroundRear,jsonVal[i]);
        CMyJson::WriteIntKey("nFeedSetType",struGroupCtrl[i].nFeedSetType,jsonVal[i]);
        CMyJson::WriteIntKey("nFeederTea",struGroupCtrl[i].nFeederTea,jsonVal[i]);
        CMyJson::WriteIntKey("nWipeSortOnOff",struGroupCtrl[i].nWipeSortOnOff,jsonVal[i]);
        CMyJson::WriteIntKey("nWipeFeedOnOff",struGroupCtrl[i].nWipeFeedOnOff,jsonVal[i]);
    }
    return true;
}

bool JsonDataConvert::struReservedToJson(Json::Value &jsonVal, const stu_res_grey_color &struReserved)
{
    CMyJson::WriteIntKey("nColor1",struReserved.nColor1,jsonVal);
    CMyJson::WriteIntKey("nSensLow1",struReserved.nSensLow1,jsonVal);
    CMyJson::WriteIntKey("nSensUp1",struReserved.nSensUp1,jsonVal);
    CMyJson::WriteIntKey("nMode1",struReserved.nMode1,jsonVal);
    CMyJson::WriteIntKey("nGreyResEnable",struReserved.nGreyResEnable,jsonVal);
    CMyJson::WriteIntKey("nMain2",struReserved.nMain2,jsonVal);
    CMyJson::WriteIntKey("nVice2",struReserved.nVice2,jsonVal);
    CMyJson::WriteIntKey("nResDiffColor",struReserved.nResDiffColor,jsonVal);
    CMyJson::WriteIntKey("nSensLow2",struReserved.nSensLow2,jsonVal);
    CMyJson::WriteIntKey("nSensUp2",struReserved.nSensUp2,jsonVal);
    CMyJson::WriteIntKey("nMode2",struReserved.nMode2,jsonVal);
    CMyJson::WriteIntKey("nDiscolorResEnable",struReserved.nDiscolorResEnable,jsonVal);
    return true;
}

bool JsonDataConvert::JSonToStuFeedAutoControl(const Json::Value &jsonVal,stru_feed_auto_ctrl &struFeedAutoCtrl OUT)
{
    CMyJson::ReadIntKey(jsonVal,"isHasRadar",struFeedAutoCtrl.isHasRadar);
    CMyJson::ReadIntKey(jsonVal,"riceContainerDepth",struFeedAutoCtrl.riceContainerDepth);
    CMyJson::ReadIntKey(jsonVal,"levelEmptyPosi",struFeedAutoCtrl.levelEmptyPosi);
    CMyJson::ReadIntKey(jsonVal,"levelMorePosi",struFeedAutoCtrl.levelMorePosi);
    CMyJson::ReadIntKey(jsonVal,"levelFullPosi",struFeedAutoCtrl.levelFullPosi);
    CMyJson::ReadIntKey(jsonVal,"levelLowPosi",struFeedAutoCtrl.levelLowPosi);
    CMyJson::ReadIntKey(jsonVal,"feedMoreStartValue",struFeedAutoCtrl.feedMoreStartValue);
    CMyJson::ReadIntKey(jsonVal,"feedEmpytLowLimitValue",struFeedAutoCtrl.feedEmpytLowLimitValue);
    CMyJson::ReadIntKey(jsonVal,"feedFullToNormalContinueTime",struFeedAutoCtrl.feedFullToNormalContinueTime);
    CMyJson::ReadIntKey(jsonVal,"feedBalanceClearUpValue",struFeedAutoCtrl.feedBalanceClearUpValue);
    CMyJson::ReadIntKey(jsonVal,"feedLevelChangeWaitTime",struFeedAutoCtrl.feedLevelChangeWaitTime);
    CMyJson::ReadIntKey(jsonVal,"feedPreFullConstValue",struFeedAutoCtrl.feedPreFullConstValue);
    CMyJson::ReadIntKey(jsonVal,"feedClearModeValue",struFeedAutoCtrl.feedClearModeValue);
    CMyJson::ReadIntKey(jsonVal,"feedLevelPreFullWaitTime",struFeedAutoCtrl.feedLevelPreFullWaitTime);
    CMyJson::ReadIntKey(jsonVal,"mode1FeedTargetPartCount",struFeedAutoCtrl.mode1FeedTargetPartCount);
    CMyJson::ReadIntKey(jsonVal,"feedBalAdjustStep",struFeedAutoCtrl.feedBalAdjustStep);
    CMyJson::ReadIntKey(jsonVal,"feedBalAdjustDuration",struFeedAutoCtrl.feedBalAdjustDuration);
    CMyJson::ReadIntKey(jsonVal,"posiChgThres",struFeedAutoCtrl.posiChgThres);
    CMyJson::ReadIntKey(jsonVal,"posiChgWait",struFeedAutoCtrl.posiChgWait);
    CMyJson::ReadIntKey(jsonVal,"adjustPosuUnit",struFeedAutoCtrl.adjustPosuUnit);
    CMyJson::ReadIntKey(jsonVal,"balUpFeedflow",struFeedAutoCtrl.balUpFeedflow);
    CMyJson::ReadIntKey(jsonVal,"balLowFeedflow",struFeedAutoCtrl.balLowFeedflow);
    CMyJson::ReadIntKey(jsonVal,"autoFeedMode",struFeedAutoCtrl.autoFeedMode);
    CMyJson::ReadIntKey(jsonVal,"balUpLowFlow",struFeedAutoCtrl.balUpLowFlow);
    CMyJson::ReadIntKey(jsonVal,"balUpFeedWhileLowFlow",struFeedAutoCtrl.balUpFeedWhileLowFlow);

    INT_ARRAY_KEY outArray;
    vector<int> intVec;
    intVec.push_back(MAX_FEEDER);

    CMyJson::ReadIntArrayKey(jsonVal,"nFeedValueBias",intVec, struFeedAutoCtrl.nFeedValueBias,outArray);

    return true;
}
