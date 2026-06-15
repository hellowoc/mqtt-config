#ifndef TEA_H
#define TEA_H

#include <cmath>
#include <stdint.h>
#include <cstring>
// #include <cstdint>


/*--------------Tea库重做------------*/


/*
 *
 * 时间：2024-06-26
 * 描述：
 *      1.提供float转FP16的函数
 *      2.Tea仿真，包括切边后过尺寸纯度
 *
*/


class Tea
{

/*---------------------------公有接口-------------------------------*/
public:
    /*--------------Float转FP16------------*/
    /*
     * 描述：将float型参数转换为FP16型数据
     *
    */
    uint16_t Float_to_fp16(float float_num);

    /*---------------TEA仿真--------------*/
    /*
     * 描述：输入图像BGRa和背景点数据
     *      并将下发给下位机的16位数据地址传入
     *      获取该TEA切边+尺寸纯度后的结果
     * 参数：
     *      pInData：四通道图像BGRa的地址
     *      pBkgData：背景点信息，1为背景，0为物料点
     *      pOutFlag：仿真结果，1为标记，0不标记
     *      nHeight：图像高度
     *      nWidth：图像宽度
     *      Data_16Bytes:下发给下为机的16字节数据地址
     */

    void TEA_Signed(const unsigned char *pInData, const bool *pBkgData, unsigned char *pOutFlag,
                    int nHeight, int nWidth, const unsigned char *Data_16Bytes);




//    void TEA_Signed(const unsigned char *pInData, const bool *pBkgData, unsigned char *pOutFlag,
//                    int nHeight, int nWidth, float *TEA, float dis_thre, bool sort_mode, int size, int area, int erode_size);


/*---------------------------私有接口-------------------------------*/
public:

    void getTEA(float r, float g, float b, float *CT, float *CE, float *CA);

    float fp16ToFloat(uint16_t fp16);

    void Erode(int size, bool *res, const bool *img_thre, int height, int width);

    float getDis(float CT, float CE, float CA, float center1, float center2, float center3);

public:
    Tea();
    ~Tea();

//public:
//    Tea(){

//    };

//    ~Tea(){

//    };



};

#endif // TEA_H
