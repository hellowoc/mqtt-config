#ifndef SVMIMAGEWIDGET_H
#define SVMIMAGEWIDGET_H

#include "basewidget.h"
#include <QVector>
#include <QRadioButton>
#include <QSignalMapper>
#include <QThread>
#include "svmtool.h"
#include <QModelIndex>
#include <QFile>

#define IMAGE_WIDTH 2048
#define ImageLabelWidth 1024
#define ImageLabelHeight 416
#define MAX_PIXEL_NUM 1000000
#define MAX_MAT_NUM 10000
#define MaxZoom 8
#define MinZoom 1
namespace EnumType {
enum {
    PIXEL_MATTER = 0,
    PIXEL_BACKGROUND = 1,
    PIXEL_MATTER_BAD = 2
};
enum SamplesType{
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
static uchar *oriBuf = NULL;          // 用于存储原始图片的数据
static uchar *chgBuf = NULL;         // 用于存储被修改的图片数据
static uchar *disBuf = NULL;     // 用于存储显示的图片信息
static uchar *bgPoint = NULL;
}
using namespace EnumType;
namespace Ui {
class SvmImageWidget;
}
class SvmImageWidget;
class SvmSimulateThread:public QThread
{
public:
    SvmSimulateThread(SvmImageWidget*);
    void run();
private:
    SvmImageWidget*m_pParent;
};

class SvmLearnThread:public QThread
{
public:
    SvmLearnThread(SvmImageWidget *);
    void run();
private:
    SvmImageWidget *m_pParent;
};
class SvmMarkThread:public QThread
{
public:
    SvmMarkThread(SvmImageWidget *);
    void run();
private:
    SvmImageWidget *m_pParent;
};


class SvmImageWidget : public basewidget
{
    Q_OBJECT

public:
    explicit SvmImageWidget(QWidget *parent = 0);
    ~SvmImageWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
    void setImage(QString path,int layer,int view,int unit);
protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
public:
    void retranslateUiWidget();
    void displaySimulate();             //仿真
    void colorSimulate(uchar *pStatus);  // 普通智能算法下的仿真
    void changeImgBySimulate(uchar *pBadAll,uchar *pStatus);    // 根据仿真的效果修改显示的图像
private:
    void updateBtnState();
    void updateShowPage();
    void updateImageSize();
private slots:

    void on_enlarge_clicked();

    void on_reduce_clicked();

    void on_checkBox_clicked();

    void on_revokeBtn_clicked();

    void on_goodTypeBtn_clicked(bool checked);

    void on_badTypeBtn_clicked(bool checked);

    void on_saveSampleBtn_clicked();

    void on_cleanSampleBtn_clicked();

    void learnFinished();

    void on_checkBox_No_Bg_clicked(bool checked);

    void imageListItemSelect(QModelIndex index);

    void on_svmLearnBtn_clicked();

    void on_applyBtn_clicked();

    void on_importSampleBtn_clicked();

    void simulateFinished();

    void on_simulateCheckBox_clicked(bool checked);

    void on_returnBtn_clicked();

    void on_comboBoxMarkIndex_activated(int index);

    void on_comboBoxSvmType_activated(int index);

    void on_checkBox_svmNew_clicked(bool checked);

    void markFinished();

    void on_comboBoxImageType_activated(int index);

private:
    Ui::SvmImageWidget *ui;
    float m_CenterX;
    float m_CenterY;

    bool m_bMoveImage;//平移图像标志
    QPoint posStart,posTmp;
    QPoint m_StartOriPoint,m_EndOriPoint;

    int m_OriRectWidth;
    int m_OriRectHeight;

    EnumType::SamplesType m_simpleType;
    QVector<QPoint> m_PointVec[2];
    QVector<int> m_PointNumVec[2];

    int m_tempSelectNum;

    QMap<int,EnumType::SamplesType> m_MatMap;

    QImage m_image,m_tempimage,m_oriImage,m_rgbnImage;
    int m_ZoomValue;
    int m_colorType;
    void adapteImageCenterPos();//调整图片中心点位置，处理越界问题
    int readSamplesNumsByType(EnumType::SamplesType mode,int layer,int svmIndex);
    int readSamplesNums(EnumType::SamplesType mode,const char *file);
    QString getSamplesFilePath(EnumType::SamplesType mode, int view,int svmIndex,bool all);
    void setSamplesFileName(EnumType::SamplesType mode);
    void markAndInitImage();

    QFile m_goodSamplesFile,m_badSamplesFile;
    void updateSvmSimulateEditValue();
public:
    int m_imageWidth,m_imageHeight,m_imageDim;

    void updateZoomValueText();

    int  m_imageLayer,m_imageView,m_imageUnit,m_imageArithGroup;

    void showGrayColor();

    void updateSamplesBtnState();

    void judgeGreyDiscolorPropotion(quint8 index);
    int m_matCount;//物料个数总数
    int m_badMatCount;//坏物料总数
    int m_badPixCount;//坏物料像素点总数
    int m_matPixCount;//物料像素点总数
    bool m_bIsPurSim;
    bool m_bSimulate;
    bool m_bNoBackground;
    bool m_bRGB;
    bool pixelIsEdge(int x, int y);
    bool pixelIsSelected(int x,int y);
    bool pixelIsOutThreshold(int x,int y);
    bool pixelIsReSelected(uchar r, uchar g, uchar b);
    void updatePixelCountLbl();

    void updatePixColor();
    SvmLearnThread *m_pThread;
    void updateImageList();
    int m_imageIndex;
    void updateCurrentImage(int row ,bool show = false);
    void markMatColor();
    svm_result m_studyResult;
	svm_result svmtrain(const int* dataGood, int countGood, const int* dataBad, int countBad,
     svm_type type,QString dirTool, double coefs[COEF_COUNT]);

    SvmSimulateThread *m_pSimulateThread;
    SvmMarkThread *m_pSvmMarkThread;
    int m_markIndex;
    bool marking;
};

#endif // SVMIMAGEWIDGET_H
