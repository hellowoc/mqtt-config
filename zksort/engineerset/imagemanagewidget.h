#ifndef IMAGEMANAGEWIDGET_H
#define IMAGEMANAGEWIDGET_H

#include "basewidget.h"
#include "mynetwork.h"
#include "globalflow.h"
#include "svmtool.h"
#include <QThread>
#include <QRadioButton>
class ImageManageWidget;
class SvmThread:public QThread
{
public:
    SvmThread() {
        m_pParentWidget = NULL;
    }
protected:
    void run();
public:
    ImageManageWidget *m_pParentWidget;
};

class ExportThread:public QThread
{
public:
    ExportThread(){
        m_pParentWidget = NULL;
    }

protected:
    void run();
public:
    ImageManageWidget *m_pParentWidget;
};


namespace Ui {
class ImageManageWidget;
}

class ImageManageWidget : public basewidget
{
    Q_OBJECT

public:
    explicit ImageManageWidget(QWidget *parent = 0);
    ~ImageManageWidget();
protected:
    void retranslateUiWidget();
    void showPage(bool bshow);
private:
    void updateImgList();                             /*!<< 私有函数：更新当前页面图片列表 */
    void updateImgName(int indexOfImg, bool bFlagSet);/*!<< 私有函数：更新当前页面图片属性 */
    void updateBtnStatus();                           /*!<< 私有函数：更新按钮状态 */
    void displayCurImg(int indexOfImg);               /*!<< 私有函数：显示当前图片状态 */
    void deleteCurImg(int indexOfImg);                /*!<< 私有函数：删除当前图片 */
    void initImgProperty();
private slots:
    void onImageListBtnClicked(int);
    void onImageProperBtnClicked(int);
    void onImageCheckBtnClicked(int);
    void onAiRadioBtnClicked(int);

    void on_delBtn_clicked(bool checked);

    void imageListChangeSlot(int index);
    void on_studyBtn_clicked();

//    void on_autoRadioBtn_clicked(bool checked);

//    void on_manuRadioBtn_clicked(bool checked);

    void on_exportAllBtn_clicked();

    void on_applyBtn_clicked();

    void studyFinished();

    void on_btn_nextPage_clicked();

    void on_btn_prePage_clicked();

    void on_saveAllBakBtn_clicked();

    void on_sampleSetBtn_clicked();

    void on_allInCBox_clicked(bool checked);

    void on_exportBtn_clicked();

    void on_importBtn_clicked();

    void on_thispageallInCBox_clicked(bool checked);

private:
    Ui::ImageManageWidget *ui;
    QPushButton   *m_imgListBtn[MAX_PER_LIST];     /*!<< 按钮：图片列表按钮 */
    QPushButton   *m_imgPropertyBtn[MAX_PER_LIST]; /*!<< 按钮：图片列表属性 */
    QLabel        *m_imgListLabel[MAX_PER_LIST];  /* 图片属于哪一视第几个相机*/
    QCheckBox     *m_imgCheckBox[MAX_PER_LIST];/*图片勾选*/
    QRadioButton  *m_aiRadionBtn[MAX_AI_CHOICE];
public:
    QMap<int,int> m_autoStatOfImg; /*!<< 参数：图片状态：设置好料，设置坏料，无设置 */
    int m_nNumOfImg;                                 /*!<< 参数：采集图片数目 */
    int m_nIndexOfImg;                               /*!<< 参数：当前图片索引号 */
    int m_nIndexOfPage;                              /*!<< 参数：当前页面索引号 */
    int m_nTotalPage;                                /*!<< 参数：总页面数 */
    QList<int>    m_checkIndexList;

    QSignalMapper *m_pImageBtnsignalMapper;
    QSignalMapper *m_pImagePropertyBtnMapper;
    QSignalMapper *m_pImageCheckBtnMapper;
    QSignalMapper *m_pAIRadioBtnMapper;
    bool pixelIsEdgeImage(QImage *image, int x, int y,int layer,int view);
    void autoStudy();

    void updateImagePropertyBtnState(int index);
    SvmThread m_studythread;
    svm_result m_studyResult;
    ExportThread m_imgExportThread;
};

#endif // IMAGEMANAGEWIDGET_H
