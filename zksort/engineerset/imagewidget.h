#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include "basewidget.h"
#include <QVector>
#include <QRadioButton>
#include <QSignalMapper>
#include "svmtool.h"
#include "wheat.h"
#include "TEA.h"
#include <QThread>

#define IMAGE_WIDTH 2048
#define ImageLabelWidth 1024
//#define ImageLabelHeight 416
#define MAX_PIXEL_NUM 1000000
#define MAX_MAT_NUM 10000
enum ImageType{
    captureType,
    analyzeType,
    simulateType
};
enum {
    PIXEL_BACKGROUND = 0,
    PIXEL_MATTER = 1,
    PIXEL_MATTER_BAD = 2
};
enum SimpleType{
    Good,
    Bad,
    NoUse
};
enum colorType{
    origin = 0,
    rgb = 1,
    r = 2,
    g,b,n,nrg,nrb,ngb
};
typedef struct MatBord{
    int left_x;
    int left_y;
    int top_x;
    int top_y;
    int right_x;
    int right_y;
    int bottom_x;
    int bottom_y;
}MatBord;

/* 用于图片显示的缓冲区 */
static uchar srcBuf[IMAGE_WIDTH*IMAGE_HEIGHT*4];          // 用于存储原始图片的数据
static uchar oriBuf[IMAGE_WIDTH*IMAGE_HEIGHT*4];          // 用于存储二级原始图片的数据
static uchar chgBuf[IMAGE_WIDTH*IMAGE_HEIGHT*4];         // 用于存储被修改的图片数据
static uchar disBuf[IMAGE_WIDTH*IMAGE_HEIGHT*4];     // 用于存储显示的图片信息
static uchar bgPoint[IMAGE_WIDTH*IMAGE_HEIGHT];
static int Percent_Label_Buffer[IMAGE_WIDTH*IMAGE_HEIGHT];
static int matSingleBadPixels[MAX_MAT_NUM];
static int matSingleTotalPixels[MAX_MAT_NUM];
static int matSingleGoodOrBad[MAX_MAT_NUM];
static MatBord MatBordPoints[MAX_MAT_NUM];

class ImageWidget;

namespace Ui {
class ImageWidget;
}

class ImageWidget : public basewidget
{
    Q_OBJECT

public:
    explicit ImageWidget(QWidget *parent = 0);
    ~ImageWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
    void updateSvmSimulateEditValue();
    void setImage(QImage image,int layer,int view,int unit);
    void setImage(QString path,int layer,int view,int unit);
    void updateImage();
    void setWidgetType(ImageType type);
protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
public:
    void retranslateUiWidget();
private:
    void updateShowPage();
    void updateImageSize();
private slots:

    void on_enlarge_clicked();

    void on_reduce_clicked();

    void on_checkBox_clicked();

    void on_revokeBtn_clicked();

    void colorTypeRadioBtnClicked(int index);

    void on_goodTypeBtn_clicked(bool checked);

    void on_badTypeBtn_clicked(bool checked);

    void on_checkBox_2_clicked(bool checked);

    void on_simulateCheckBox_clicked(bool checked);

    void on_saveSampleBtn_clicked();

    void on_cleanSampleBtn_clicked();

    void on_imgDataBtn_clicked();

    void on_imgDataBtn_2_clicked();

    void on_imgDataBtn_3_clicked();

    void on_checkBox_No_Bg_clicked(bool checked);

    void on_pushButton_clicked();

    void on_sampleClassBtn_clicked();

    void on_lastPicBtn_clicked();

    void on_nextPicBtn_clicked();

    void m_saturationSlider_valueChanged(int value);

    void m_saturationSlider_released();

    void m_saturationSliderTimeOut();

private:
    float m_CenterX;
    float m_CenterY;

    bool m_bMoveImage;//平移图像标志
    QPoint posStart,posTmp;
    QPoint m_StartOriPoint,m_EndOriPoint;

    int m_OriRectWidth;
    int m_OriRectHeight;

    int ImageLabelHeight;
    SimpleType m_simpleType;
    QVector<QPoint> m_PointVec[2];
    QVector<int> m_PointNumVec[2];
    QVector<QPoint> m_NoSlectPointVec[2];
    QVector<int> m_NoSlectPointNumVec[2];

    QVector<QPoint> m_SamplePointVec[2];
    int m_tempSelectNum;
    QVector<QPoint> m_tempSelectPointVec;
    int m_tempNoSelectNum;
    QVector<QPoint> m_tempNoSelectPointVec;


    QImage m_image,m_tempimage,m_oriImage;
    int m_ZoomValue;
    Ui::ImageWidget *ui;
    void adapteImageCenterPos();//调整图片中心点位置，处理越界问题

public:
    int m_imageWidth,m_imageHeight;
    ImageType m_widgetType;

    void updateZoomValueText();

    void displaySimulate();             //仿真
    void colorSimulate(uchar *pStatus,int arithIndex);  // 普通智能算法下的仿真
    void changeImgBySim(uchar *pStatus,int arithIndex);    // 根据仿真的效果修改显示的图像
    int  m_imageLayer,m_imageView,m_imageUnit,m_imageArithGroup;

    colorType m_colorType;
    QSignalMapper *m_pColorTypeBtnMapper;
    QRadioButton *m_pColorRadioBtn[9];
    void showGrayColor();

    void updateSimpleBtnState();

    void judgeGreyDiscolorPropotion(quint8 index);
    int m_matCount;//物料个数总数
    int m_badMatCount;//坏物料总数
    int m_badPixCount;//坏物料像素点总数
    int m_matPixCount;//物料像素点总数
    bool m_bIsPurSim;
    bool m_bSimulate;
    bool m_bRGB;
    bool pixelIsEdge(int x, int y,int cnt=1); //cnt::切边圈数
    bool pixelIsSelected(int x,int y);
    bool pixelIsOutThreshold(int x,int y);
    bool pixelIsReSelected(uchar r, uchar g, uchar b);
    void updatePixelCountLbl();

    void updateSampleCount();
    void updatePixColor();
    QTimer *m_timer;
};


#endif // IMAGEWIDGET_H
