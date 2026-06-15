#include "targetimagewidget.h"
#include "ui_targetimagewidget.h"
#include <QTime>
#include "global.h"
#include "aisetdialog.h"
#include <QDateTime>
#include <sys/time.h>
#include "globalflow.h"

CircularQueue myCircularQueue;


CircularQueue::CircularQueue(int capacity)
    : m_buffer(capacity)
    , m_head(0)
    , m_tail(0)
    , m_count(0)
    , m_capacity(capacity)
{
    qDebug()<<"myCircularQueue size"<<m_capacity;
}

CircularQueue::~CircularQueue()
{
}

void CircularQueue::enqueue(const QString& value)
{
    QMutexLocker locker(&m_mutex);

    m_buffer[m_tail] = value;
    m_tail = (m_tail + 1) % m_capacity;

    if (m_count < m_capacity) {
        ++m_count;
    } else {
        // 队列已满，覆盖最旧元素，队首指针移动
        m_head = (m_head + 1) % m_capacity;
    }
}

QString CircularQueue::dequeue()
{
    QMutexLocker locker(&m_mutex);

    if (m_count == 0) {
        return QString();
    }

    QString result = m_buffer[m_head];
    m_head = (m_head + 1) % m_capacity;
    --m_count;

    return result;
}

int CircularQueue::size() const
{
    QMutexLocker locker(&m_mutex);
    return m_count;
}

bool CircularQueue::isEmpty() const
{
    QMutexLocker locker(&m_mutex);
    return m_count == 0;
}

bool CircularQueue::isFull() const
{
    QMutexLocker locker(&m_mutex);
    return m_count == m_capacity;
}

void CircularQueue::clear()
{
    QMutexLocker locker(&m_mutex);
    m_head = 0;
    m_tail = 0;
    m_count = 0;
}



TargetImageWidget::TargetImageWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::TargetImageWidget)
{
    ui->setupUi(this);

    m_imgListBtn[0] = ui->pushButton;
    m_imgListBtn[1] = ui->pushButton_2;
    m_imgListBtn[2] = ui->pushButton_3;
    m_imgListBtn[3] = ui->pushButton_4;

    initUi();
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showImageSlot()));
    timer->stop();
}

TargetImageWidget::~TargetImageWidget()
{
    delete ui;
}

void TargetImageWidget::showPage(bool bshow)
{

    if(bshow)
    {
        qDebug()<<"connect";
    }
    else
    {
        qDebug()<<"disconnect";
    }
}


void TargetImageWidget::retranslateUiWidget()
{

}

void TargetImageWidget::receiveMsg(int msg1, int msg2, int msg3)
{

}

void TargetImageWidget::setTimer(bool status)
{
    if(status)
        timer->start(500);
    else
        timer->stop();

    myCircularQueue.clear();
}

void TargetImageWidget::initUi()
{
    for(int i = 0 ; i < 4 ; i++)
    {
        m_imgListBtn[i%4]->setIcon(QPixmap());
        m_imgListBtn[i%4]->setEnabled(false);
        m_imgListBtn[i%4]->setObjectName("targetBtn");
    }
    index = 0;
}


void TargetImageWidget::showImageSlot()
{
    if(myCircularQueue.isEmpty())
        return;

    QPixmap showImage;

//    if((imapath1.size() == 0))
//    {
//        return;
//    }

    QString imapath1 = myCircularQueue.dequeue();
    if(showImage.load(imapath1,"bmp",Qt::AutoColor))
    {
        m_imgListBtn[index%4]->setIconSize(showImage.size());
        m_imgListBtn[index%4]->setIcon(showImage);

        index++;
    }


    //    bIsImg = image.load(g_Runtime().nimageNameVec[j],"bmp",Qt::AutoColor);

    //    QStringList list1 = imapath1.split("/");
    //    QString name1 = list1.last().split(".").first();
    //    QString name1_show = name1.split("_").at(1);
    //    QString name1_add = name1.split("_").at(2);
    //    QStringList list2 = imgpath2.split("/");
    //    QString name2 = list2.last().split(".").first();
    //    QString name2_show = name2.split("_").at(1);
    //    QString name2_add = name2.split("_").at(2);
    //#if 1
    //    //if(image.load(imapath1,"png") && image2.load(imgpath2,"png"))
    //    {

    //        QImage image(imgkoutuData+preparedBatchID*myFlow.koutuImageSize*2,struCnfp.koutu_width,struCnfp.koutu_height,QImage::Format_RGB888);

    //        QPixmap pixmap = QPixmap::fromImage(image.scaled(150,150));

    //        m_imgListLabel[index%8]->setPixmap(pixmap);
    //        if(name1_add=="255"){
    //            struGsh.frontBu++;
    ////            m_nameImgLabel[index%8]->setText(g_myLan().ids_frontCamera+name1_show+"_00");
    //            //m_nameImgLabel[index%8]->setText(g_myLan().ids_frontCamera+name1_show);

    //        }else{
    //           // m_nameImgLabel[index%8]->setText(name1_add+g_myLan().ids_frontCamera+name1_show);
    //        }

    //        index++;
    //        QImage image2(imgkoutuData+preparedBatchID*myFlow.koutuImageSize*2+myFlow.koutuImageSize,struCnfp.koutu_width,struCnfp.koutu_height,QImage::Format_RGB888);
    //        QPixmap pixmap2 = QPixmap::fromImage(image2.scaled(150,150));

    //        m_imgListLabel[index%8]->setPixmap(pixmap2);

    //        if(name2_add=="255"){
    //            struGsh.rearBu++;
    ////            m_nameImgLabel[index%8]->setText(g_myLan().ids_backCamera+name2_show+"_00");
    //            //m_nameImgLabel[index%8]->setText(g_myLan().ids_backCamera+name2_show);
    //        }else{
    //           // m_nameImgLabel[index%8]->setText(name2_add+g_myLan().ids_backCamera+name2_show);
    //        }
    //        //qDebug("show  image");
    //        index++;
    //    }
    //#endif
}
