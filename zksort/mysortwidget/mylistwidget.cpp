#include "mylistwidget.h"
#include <QDir>
#include <QDebug>
#include <QDateTime>
#include "global.h"

mylistwidget::mylistwidget(QWidget *parent) :
    QListWidget(parent)
{
    currentitem = NULL;
    this->setAcceptDrops(true);
    path="/media";
    iscut = false;
    setStyleSheet("QListView {font: 25 24pt \"Microsoft YaHei\";"
                             "border: 1px solid white;"
                             "border-radius: 10px;}");
}

void mylistwidget::updateList()
{
    isUpdating = true;
    QDir myDir(path);
    if (!myDir.exists()) {
        path = "/meida";
    }
    this->clear();

    QFileInfoList list ;
    QStringList filters;
    filters.clear();
    if(path == "/media")
    {
        filters<<"sd*";
        list = myDir.entryInfoList(filters, QDir::NoDotAndDotDot| QDir::Dirs);
    }
    else if(path == "/sdcard")
    {
        filters<<"[1-9]*";
        list = myDir.entryInfoList(filters,QDir::NoDotAndDotDot| QDir::Dirs);
    }
    else
    {
        list = myDir.entryInfoList(myDir.filter() | QDir::NoDotAndDotDot);
    }

    QString filename;
    float filesize;
    QDateTime filedate;
    QStringList filelist;
    QString str;
    int minLength = 0,maxLength = 0;

    filelist.clear();
    for(int i=0;i< list.size();i++)
    {
        filename = list.at(i).fileName();
        if(i == 0) minLength = filename.length();
        if(filename.length() >maxLength)
            maxLength = filename.length();
        if(filename.length() <minLength)
            minLength = filename.length();
    }
    for(int i=0;i< list.size();i++)
    {
        filename = list.at(i).fileName();
        filesize = list.at(i).size();
        filedate = list.at(i).lastModified();
        filename = g_Runtime().fillIn(filename,maxLength+8,' ');
        if (filesize > 1024*1024)
        {
            filesize = filesize /1024 /1024;
            str= filename +filedate.toString("yyyy/MM/dd hh:mm:ss")+ "        "+QString::number(filesize,'f',1)+"M";
        }
        else
        {
            filesize = filesize/1024;
            str = filename +filedate.toString("yyyy/MM/dd hh:mm:ss")+ "        "+QString::number(filesize,'f',1)+"K";
        }
        filelist.append(str);
    }
    this->insertItems(0, filelist);

    isUpdating = false;
}

#if 0
void mylistwidget::mouseDoubleClickEvent(QMouseEvent *e){
    if(isUpdating) return;
    currentitem = this->currentItem()->clone();
    QString currentitemName = currentitem->text().split("  ").first();
    if(currentitemName == ".."){
        QDir dir(path);
        if(dir.cdUp() && path.right(1)!="/")
            path.append("/");
        path= dir.absolutePath();
    }
    else{
        QFileInfo fileinfo(path+"/"+currentitemName);
        if(fileinfo.isDir())
        {
            path = fileinfo.absoluteFilePath();
        }
    }
    qDebug()<<"--->"<<path;
    updateList();
}

void mylistwidget::mousePressEvent(QMouseEvent *event){
    QListWidget::mousePressEvent(event);
    start = event->pos();
}
void mylistwidget::mouseMoveEvent(QMouseEvent *e){
    currentitem = this->currentItem();
    int distance = (e->pos() - start).manhattanLength();
    if (distance > QApplication::startDragDistance()){
        //获取当前被选中的值
        if (currentitem){
            //准备需要被传输的数据
            QMimeData *mimedata = new QMimeData();
            mimedata->setText(path+"/"+currentitem->text().split(" ").first());

            //准备传输的对象
            QDrag *mydrag = new QDrag(this);
            mydrag->setMimeData(mimedata);
            //设置传输的图标
            //      mydrag->setPixmap(QPixmap(":/new/prefix1/F:/123.jpg"));
            //进行拖动操作
            if(Qt::MoveAction == mydrag->exec(Qt::MoveAction)){
                delete currentitem;
                currentitem = NULL;
            }
            QListWidget::mouseMoveEvent(e);
        }
    }
}
void mylistwidget::dragEnterEvent(QDragEnterEvent *event){
    QListWidget *src = qobject_cast<QListWidget *>(event->source());

    if (src != NULL && src != this){
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}
void mylistwidget::dragMoveEvent(QDragMoveEvent *e){
    QListWidget *src = qobject_cast<QListWidget *>(e->source());
    if (src != NULL && src != this){
        e->setDropAction(Qt::MoveAction);
        e->accept();
    }
}
void mylistwidget::dropEvent(QDropEvent *event){
    QListWidget *src = qobject_cast<QListWidget *>(event->source());
    if (src != NULL && src != this){
        QString str = event->mimeData()->text().split("  ").first();
        str = str.split("/").last();
        QListWidgetItem *lw1 = new QListWidgetItem(str);
        this->addItem(lw1);
        this->setCurrentItem(lw1);

        QFile::copy(event->mimeData()->text().split("  ").first(),path+"/"+str);
        if(iscut)
            QFile::remove(event->mimeData()->text().split("  ").first());

        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}
#endif
