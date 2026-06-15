#ifndef SUBPIXEL_H
#define SUBPIXEL_H

/*	功能：计算颜色偏移参数
    pInData：输入图像，要求是4通道，按照bgrn存储
    nWidth：图像宽度(pixels)
    nHeight：图像高度(pixels)
    channels：有效图像通道数（输入必须是4通道，<=4）
    offset：输出的像素偏移，依次是rgbn的x、y偏移值(rx,ry,gx,gy,bx,by,nx,ny)
    max_offset：最大偏移范围
    base_color：基准色（0-r,1-g,2-b,3-n），基准色的像素偏移输出为0
*/
void GetColorOffset(const unsigned char* pInData, int nWidth, int nHeight,
    int channels, float* offset, int max_offset = 5, int base_color = 0);


/*	功能：根据偏移参数校正图像
    pInData：输入图像，要求是4通道，按照bgrn存储
    pOutData：校正后图像
    nWidth：图像宽度(pixels)
    nHeight：图像高度(pixels)
    channels：有效图像通道数（输入必须是4通道，<=4）
    offset：像素偏移，依次是rgbn的x、y偏移值(rx,ry,gx,gy,bx,by,nx,ny)
    base_color：基准色（0-r,1-g,2-b,3-n），基准色不需要校正
*/
void CorrectImage(const unsigned char* pInData, unsigned char* pOutData, int nWidth, int nHeight,
    int channels,float* offset, int base_color = 0);


#endif // SUBPIXEL_H
