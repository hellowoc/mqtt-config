#ifndef TEA_CONVERTER_H
#define TEA_CONVERTER_H

#include <cmath>
#include <stdint.h>

class Converter
{

public:
    /*--------------Float转FP16------------*/
    /*时间：2023-12-04*/
    /*描述：将float型参数转换为FP16发送给下位机*/
    uint16_t Float_to_fp16(float float_num);

    /*---------------RGB转TEA--------------*/
    /*时间：2023-12-04*/
    /*描述：将非归一化前的RGB值转换为TEA值*/
    void getTEA(float r, float g, float b, float *CT, float *CE, float *CA);




    /*-----------------------2023.12.08新添-------------------------------*/

    /*---------------------------私有接口-------------------------------*/
private:
    /*---------------获取精度--------------*/
    /*时间：2023-12-08*/
    /*描述：获取精度*/
    float getDis(float CT, float CE, float CA, float center1, float center2, float center3);


    /*---------------------------公有接口-------------------------------*/
public:
    /*---------------TEA仿真--------------*/
    /*时间：2023-12-08*/
    /*描述：输入背景，输出坏点信息*/
    /*参数：TEA:设置的TEA参数; dis_thre：精度值; sort_mode：正选反选，>为1,<为0; pOutFlag：坏点标记信息，1为坏点。*/
    void TEA_Signed(const unsigned char *pInData, const bool *pBkgData, unsigned char *pOutFlag, int nHeight, int nWidth, float *TEA, float dis_thre, bool sort_mode);



public:
    Converter();
    ~Converter();



};

#endif // TEA_CONVERTER_H
