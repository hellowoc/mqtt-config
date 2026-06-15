#ifndef MYSVMSAMPLECLASS_H
#define MYSVMSAMPLECLASS_H

#include "basewidget.h"
#include "logger.h"

namespace EnumType2 {
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
}
using namespace EnumType2;

namespace Ui {
class MySvmSampleClass;
}
//enum SamplesType{
//    Good,
//    Bad,
//    NoUse
//};
typedef struct SvmSampleObj{
    int mode;int r;int g;int b;int n;
//    bool operator ==(const SvmSampleObj &obj){
//        return (r == obj.r)&&(g==obj.g)&& (b==obj.b);
//    }
//    bool operator <(const SvmSampleObj &obj)const{
//        if(r != obj.r)
//            return r < obj.r;
//        if(g != obj.g)
//            return g < obj.g;
//        return b < obj.b;
//    }
}SvmSampleObj;
class MySvmSampleClass : public basewidget
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

public:
    explicit MySvmSampleClass(QWidget *parent = 0);
    ~MySvmSampleClass();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
protected:
    void retranslateUiWidget();
private slots:

    void on_svmrefreshBtn_clicked();

    void on_svmimportBtn_clicked();

    void on_svmsaveBtn_clicked();

    void on_svmdeleteBtn_clicked();

    void on_svmrenameBtn_clicked();

private:
    Ui::MySvmSampleClass *ui;
    void appendUpdateResult(QString str,bool ok = true);
    int readSamplesNums(const char *file);
//    QString getSamplesFilePath(EnumType::SamplesType mode);
//    void setSamplesFileName(EnumType::SamplesType mode);
//    void markAndInitImage();

    QFile m_goodSamplesFile,m_badSamplesFile;

};

#endif // MYSVMSAMPLECLASS_H
