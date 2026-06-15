#ifndef WHEAT_H
#define WHEAT_H


class Wheat
{

public:
    //Wheat();

/*

    imgData：	BGRA输入
    bkgData：	背景参数，0为背景
    outData：	输出结果，1为标记点，0为其他点
    filter：	下限
    sens：      精度
    purity：	纯度
    h0：		标记范围下限
    h1：		标记范围上限


*/
    void Wheat_Main(const unsigned char *imgData, bool *bkgData, bool *outData, int width, int height, float filter, float sens, int purity, float h0, float h1);

};

#endif // WHEAT_H
