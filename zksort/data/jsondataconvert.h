#ifndef JSONDATACONVERT_H
#define JSONDATACONVERT_H

#include "myjson.h"
#include "../global/globalparams.h"

class JsonDataConvert
{
public:
    JsonDataConvert();

    static bool JsonToCnfGlobal(const Json::Value &jsonVal,struCnfGlobal &CnfGlobal OUT);
    static void CnfGlobalToJson(const struCnfGlobal &CnfGlobal,Json::Value &jsonVal OUT);

    static bool JsonTostruCnfProfile(const Json::Value &jsonVal,struCnfProfile &cnfProfile OUT);
    static bool StruCnfProfileToJson(const struCnfProfile &cnfProfile,Json::Value &jsonVal OUT);

private:


    static bool profileIndexToJson(int arraySize,const stru_Profile_Index * profile,Json::Value &jsonVal OUT);
    static bool layerInfoToJson(int arraySize,const stru_layer_info* layerinfoArr,Json::Value &jsonVal OUT);
    static bool stuViewInfoToJson(int arraySize,const stru_view_info* struViewInfoArr,Json::Value &jsonVal OUT);
    static bool stuVavleInfoToJson(int arraySize,const stru_vavle_info* struValveInfoArr,Json::Value &jsonVal OUT);
    static bool stuLampCfgToJson(const stru_layer_lamp_cfg &struLayerLampInfoArr,Json::Value &jsonVal OUT);
    static bool stuAlarmCfgToJson(const stu_alarm_cfg &struAlarmCfg,Json::Value &jsonVal OUT);
    static bool stuIntelWipeToJson(const stu_intel_wipe_cfg &struIntelWipeArr,Json::Value &jsonVal OUT);
    static bool stuCameraInfoToJson(int arraySize,const stru_view_camera_Info *stuCameraInfoArr,Json::Value &jsonVal OUT);
    static bool stuIdenGupInfoToJson(int arraySize,const stru_iden_group_info *stuIdenGroupArr,Json::Value &jsonVal OUT);
    static bool stuVavleGroupInfoToJson(int arraySize,const stru_vavle_group_info *stuTickGupInfo,Json::Value &jsonVal OUT);

    static bool stuModelSelectCfgToJson(const stru_modelabel_select &stumodeLableSelect,Json::Value &jsonVal OUT);

    static bool JsonToStuModelSelectCfg(const Json::Value &jsonVal,stru_modelabel_select &stumodeLableSelect OUT);

    static bool JsonToProfileindex(int arraySize,const Json::Value &jsonVal,stru_Profile_Index *profile OUT);
    static bool JsonToLayerInfo(int arraySize,const Json::Value &jsonVal,stru_layer_info *layerinfoArr OUT);
    static bool JsonToStuViewInfo(int arraySize,const Json::Value &jsonVal, stru_view_info *stru_view_info OUT);
    static bool JsonToStuValveInfo(int arraySize,const Json::Value &jsonVal,stru_vavle_info *struValveInfoArr OUT);
    static bool JsonToStuLampCfg(const Json::Value &jsonVal,stru_layer_lamp_cfg &struLayerLampInfoArr OUT);
    static bool JsonToStuIntelWipeCfg(const Json::Value &jsonVal,stu_intel_wipe_cfg &struIntelWipeArr OUT);
    static bool JSonToStuUseLimitCfg(const Json::Value &jsonVal,stu_use_limit_cfg &struselimitcfg OUT);
    static bool JsonToStuCameraInfo(int arraySize,const Json::Value &jsonVal, stru_view_camera_Info *stuCameraInfoArr OUT);
    static bool JsonToIdenGupInfo(int arraySize,const Json::Value &jsonVal,stru_iden_group_info *stuIdenGroupArr OUT);
    static bool JsonToStuVavleGroupInfo(int arraySize,const Json::Value &jsonVal,stru_vavle_group_info *stuTickGupInfo OUT);
    static bool JsonToStuAlarmCfg(const Json::Value &jsonVal,stu_alarm_cfg &stru_alarm OUT);
    static bool JSonToStuFeedAutoControl(const Json::Value &jsonVal,stru_feed_auto_ctrl &struFeedAutoCtrl OUT);

    static bool JsonTostruGreyColor(int arraySize,const Json::Value &jsonVal,stu_color_arith *struGreyColor OUT);
    static bool JsonTostruIntel(int arraySize,const Json::Value &jsonVal,stu_svm *struIntel OUT);
    static bool JsonTostruShapeIntel(int arraySize,const Json::Value &jsonVal,stu_shape_svm *struShapeIntel OUT);
//    static bool JsonTostruCross(const Json::Value &jsonVal,stu_cs &struCross OUT);
    static bool JsonTostruCircleLong(int arraySize,const Json::Value &jsonVal,stu_circle_long *struCircleLong OUT);
    static bool JsonTostruLongShort(int arraySize,const Json::Value &jsonVal,stu_long_short *struLongShort OUT);
    static bool JsonTostruBigSmall(int arraySize,const Json::Value &jsonVal,stu_big_small *struBigSmall OUT);
    static bool JsonTostruHsv(int arraySize,const Json::Value &jsonVal,stu_hsv *struHsv OUT);
//    static bool JsonTostruBud(int arraySize,const Json::Value &jsonVal,stu_bd *struBud OUT);
    static bool JsonTostruColorSat(const Json::Value &jsonVal, stru_color_sat &struColorSat OUT);
    static bool JsonTostruColorHue(const Json::Value &jsonVal, stru_color_hue &struColorHue OUT);
    static bool JsonTostruColorScale(const Json::Value &jsonVal,stru_color_scale &struColorScale OUT);
    static bool JsonTostruMildew(const Json::Value &jsonVal,stru_mildew &struMildew OUT);
    static bool JsonTostruOutline(const Json::Value &jsonVal,stru_outline &struOutline OUT);
    static bool JsonTostruLine(const Json::Value &jsonVal, stru_line &struLine OUT);
    static bool JsonTostruBud(int arraySize,const Json::Value &jsonVal, stru_bud *struBud OUT);
    static bool JsonTostruRound(int arraySize,const Json::Value &jsonVal, stru_round *struRound OUT);
    static bool JsonTostruColorValue(const Json::Value &jsonVal, stru_color_value &struColorValue OUT);
    static bool JsonTostruSplMildew(const Json::Value &jsonVal, stru_spl_mildew &struSplMildew OUT);
    static bool JsonTostruWhitePeanut(const Json::Value &jsonVal, stru_white_peanut &struWhitePeanut OUT);
    static bool JsonTostruPepperHandle(const Json::Value &jsonVal, stru_pepper_handle &strupepperhandle OUT);
    static bool JsonTostruSuppressLarge(int arraySize,const Json::Value &jsonVal,stru_suppress_large *struSuppressLarge OUT);
    static bool JsonTostruWheatSprout(const Json::Value &jsonVal, stru_wheat_sprout &struwheatsprout OUT);
    static bool JsonToStruTeaColor(int arraySize,const Json::Value &jsonVal,stu_tea_color *struTeaColor OUT);

    static bool JsonTostruGroupCtrl(int arraySize,const Json::Value &jsonVal,stu_group_ctrl *struGroupCtrl OUT);
    static bool JsonTostruReserved(const Json::Value &jsonVal,stu_res_grey_color &struReserved OUT);

    static bool struGreyColorToJson(int arraySize,Json::Value &jsonVal,const stu_color_arith *struGreyColor);
    static bool struIntelToJson(int arraySize,Json::Value &jsonVal,const stu_svm *struIntel);
    static bool struShapeIntelToJson(int arraySize,Json::Value &jsonVal,const stu_shape_svm *struIntel);
//    static bool struCrossToJson(Json::Value &jsonVal,const stu_cs &struCross );
    static bool struCircleLongToJson(int arraySize,Json::Value &jsonVal,const stu_circle_long *struCircleLong);
    static bool struLongShortToJson(int arraySize,Json::Value &jsonVal,const stu_long_short *struLongShort);
    static bool struBigSmallToJson(int arraySize,Json::Value &jsonVal,const stu_big_small *struBigSmall);
    static bool struHsvToJson(int arraySize,Json::Value &jsonVal,const stu_hsv *struHsv );
//    static bool struBudToJson(int arraySize,Json::Value &jsonVal,const stu_bd *struBud);

    static bool struColorSatToJson(Json::Value &jsonVal,const stru_color_sat &struColorSat);
    static bool struColorHueToJson(Json::Value &jsonVal,const stru_color_hue &struColorHue);
    static bool struScaleToJson(Json::Value &jsonVal,const stru_color_scale &struScale);
    static bool struMildewToJson(Json::Value &jsonVal, const stru_mildew &struMildew);
    static bool struOutlineToJson(Json::Value &jsonVal, const stru_outline &struOutline);
    static bool struLineToJson(Json::Value &jsonVal, const stru_line &struLine);
    static bool struBudToJson(int arraySize,Json::Value &jsonVal, const stru_bud *struBud);
    static bool struColorValueToJson(Json::Value &jsonVal, const stru_color_value &struColorValue);
    static bool struRoundToJson(int arraySize,Json::Value &jsonVal, const stru_round *struRound);
    static bool struSplMildewToJson(Json::Value &jsonVal, const stru_spl_mildew &struSplMildew);
    static bool struWhitePeanutToJson(Json::Value &jsonVal, const stru_white_peanut &struWhitePeanut);
    static bool struPepperHandleToJson(Json::Value &jsonVal, const stru_pepper_handle &struPepperHandle);
    static bool struSuppressLargeToJson(int arraySize,Json::Value &jsonVal,const stru_suppress_large *struSuppressLarge);
    static bool struWhearSproutToJson(Json::Value &jsonVal, const stru_wheat_sprout &struWheatSprout);
    static bool struTeaColorToJson(int arraySize,Json::Value &jsonVal,const stru_tea_color *struTeaColor);

    static bool struGroupCtrlToJson(int arraySize,Json::Value &jsonVal,const stu_group_ctrl *struGroupCtrl);
    static bool struReservedToJson(Json::Value &jsonVal,const stu_res_grey_color &struReserved);
};

#endif // JSONCONVERT_H
