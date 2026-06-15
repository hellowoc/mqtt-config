#ifndef MATERIAL_H
#define MATERIAL_H
struct gsh;
struct cnfg;
struct cnfc;
struct cnfp;
#if 0
/* ---------------------------------------------------- */
/* reserved parameters struct for grey A,B, discolor A,B */
typedef struct stru_adv {
    int nDiscolor1;   		// 0-: r/(r+g), 1-: r/(r+b), 2-: g/(g+b)
    int nSensMin1;
    int nSensMax1;
    int nMode1;		   // 1: -- light mode, 0: -- dark mode
    int nColor2;
    int nSensMin2;
    int nSensMax2;
    int nMode2;
}stu_reserved;

/* ---------------------------------------------------- */
/* grey A,B&discolor A,B parameters struct */
typedef struct stru_discolor {
    int nRow;                // row, area = row*column
    int nColumn;             // column, column = row*2
    int nPercent;            // percent in row*column area
    int nColor;              // only in grey A,B color,  1-: red, 2-: green, 4-: blue
    int nDiscolor;           // only in discolor A,B color, 0-: r/(r+g), 1-: r/(r+b), 2-: g/(g+b)
    int nSensMin;           // dark mode sens, send to fpga is {0, sens_min}
    int nSensMax;           // light mode sens, send to fpga is {sens_max, 255}
    int nMode;               // 1: -- light mode, 0: -- dark mode
    int nLightLimit;        // only in discolor A,B
    stu_reserved struReserved;  // reserved params
}stu_grey_color;

/* ---------------------------------------------------- */
/* intelligent parameters struct */
typedef struct stru_intelligent {
    int nRow;            // row, area = row*column
    int nColumn;         // column, column = row*2
    int nPercent;        // percent in row*column area
    int nCons[2];        // const quotient
    int nQuo1[2];      // red quotient
    int nQuo2[2];      // green quotient
    int nQuo3[2];      // blue quotient
    stu_reserved struReserved;  // reserved params
}stu_it;

typedef struct stru_intelligent_material {
    int use;                    // 1 use , 0: unuse
    int good_bad;               // mat[0] :material 1, ... mat[3]: material 4, 0: bad, 1: good 2: null
    unsigned char name[128];    // name for 4 material
}stu_it_mat;

/* ---------------------------------------------------- */
/* infrared parameters struct */
typedef struct stru_infrared {
    int nRow;
    int nColumn;
    int nPercent;
    int nSens;
}stu_infrared;

/* ---------------------------------------------------- */
/* peanut bud parameters struct */
typedef struct stru_bud {
    int nRow;
    int nColumn;
    int nPercent;
    int nPercentLimit;
    int nArea;
}stu_bd;

/* ---------------------------------------------------- */
/* cross arithmetic parameters struct */
typedef struct stru_cross {
    int nRow;
    int nColumn;
    int nPercent;
    int nSens;
    int nColor;
}stu_cs;

/* ---------------------------------------------------- */
/* shape arithmetic parameters struct */
typedef struct stru_shape {
    int nAreaMin;
    int nAreaMax;
    int nMode;
}stu_sp;

/* ---------------------------------------------------- */
/* big&small arithmetic parameters struct */
typedef struct stru_scale {
    int nMode;			// 0: -- small mode, 1: --big mode
    int nValue;
    int nValueLimit;
}stu_sc;

/* ---------------------------------------------------- */
/* peanut a、b、c arithmetic parameters struct */
typedef struct stru_peanut_abc{
    int nRow;
    int nColumn;
    int nPercent;
    int nSensMin;
    int nSensMax;
    int nMode;
    int nSensReserved   ;
}stu_pn_abc;

/* ---------------------------------------------------- */
/* matermelon arithmetic parameters struct */
typedef struct stru_watermelon {
    int nWhiteSens;
    int nWhitePropMin;
    int nWhitePropMax;
    int nWhiteMode;
    int nEdgeRedSens;
    int nEdgeRedProp;
    int nEdgeDamagedSens;
    int nEdgeDamagedProp;
    int nRugged1Sens;
    int nRugged1Save;
    int nRugged2Sens;
    int nRugged2Save;
}stu_melon;

/* ---------------------------------------------------- */
/* arithmetic name and used parameters struct */
struct stu_arithmetic{
    char sName[64];
    int nIsable;
};
#endif
//void materialParamsPacketOrganize(int nCmd, int nLevelId,int nGroupId, int nIndex);

//void materialCopyAssemble(int nGroupId, int nLevelId, int bIsAllSens, int nArithSeq, int bIsReserved);
//void materialResetGroupAssemble(int nLevelId, int nGroupId, int bIsAllSens, int nArithSeq, int bIsReserved);
//void materialParamsCopy(int nLevelIdSrc, int nGroupIdSrc, int nLevelIdDst, int nGroupDst, int bIsAllSens, int nArithSeq, int bIsReserved);

//int ssbf_material_chart_color(_t_fbwid_val values[], int mat_stauts);	// draw chart in parameters
//int ssbf_material_chart_color_adv(_t_fbwid_val values[], int mat_status2);		// draw chart in parameters
//int ssbf_material_chart_infra(_t_fbwid_val values[], int k);
//int ssbf_material_chart_cross(_t_fbwid_val values[], int k);
//int ssbf_material_chart_shape(_t_fbwid_val values[], int k);
//int ssbf_material_chart_scale(int k);
//int ssbf_material_chart_earthpea(_t_fbwid_val values[], int k, int mat_status);
//int ssbf_material_chart_peanut_abc(_t_fbwid_val values[], int k, int mat_status);
//void ssbf_material_earthpea_get_profile();

//int ssbf_material_chart_intel(_t_fbwid_val values[], int mat_stauts);	// draw chart in parameters
//int ssbf_material_chart_intel_adv(_t_fbwid_val values[], int mat_stauts);	// draw chart in parameters


#endif // MATERIAL_H
