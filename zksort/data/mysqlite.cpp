#include "mysqlite.h"
#include <QDebug>
#include <ctime>
#include "mqttsrv.h"
#include "global/globalflow.h"

queryStatisticThread *myQueryStatisticThread;
getImpurityThread *getThread;
MySqlite::MySqlite(QObject *parent) : QObject(parent)
{
    viewName <<"MainFront"<<"MainRear"<<"ViceFront"<<"ViceRear"<<"MainFront2"<<"MainRear2"<<"ViceFront2"<<"ViceRear2";

}
MySqlite::~MySqlite()
{

}

/* 建立连接，并打开本地数据库 */
bool MySqlite::openDatabase()
{
    if(db.isOpen())
        return true;
    if (db.contains("qt_sql_default_connection"))
    {
        db = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
    }
    db.setDatabaseName(STATISTIC_DATABASE);
    if(!db.open())
    {
       qDebug()<<"STATISTIC_DATABASE open fail!";
       db.close();
       return false;
    }
    return true;
}

/* 断开连接，关闭本地数据库 */
void MySqlite::closeDatabase()
{
    if (db.isOpen())
        db.close();
}

/* 创建数据库表结构 */
bool MySqlite::creatTable(QString tableName)
{
    if(!openDatabase())
    {
        return false;
    }
    /* 表已存在，直接退出 */
    if(isTableExist(tableName))
    {
        db.close();
        return true;
    }
    stu_ipc_model_class modelClassIf;
    QString classNameStr;
    memcpy(&modelClassIf, &struIpcShare.struIpcModelClass[0],sizeof(modelClassIf));

    QSqlQuery query(db);
    QString sql = "CREATE TABLE ";
    sql.append(tableName);
    sql.append("( CreatedTime TimeStamp NOT NULL DEFAULT (date('now','localtime')) ,");

    for(int i = 0;i < struCnfp.ipcModelClassCount[0][0];i++)
    {
        classNameStr = modelClassIf.className[i];
        if(classNameStr.left(3).contains("_"))
            classNameStr.remove(classNameStr.left(3));
        qDebug()<<modelClassIf.className[i]<<classNameStr;
        headName.append(QString("%1 INTEGER, ").arg(classNameStr));
    }

    headName.remove(headName.length() - 2,1);
    headName.append(")");  //把最后一个字符串里面的逗号去除，并添加右括号
    sql.append(headName);
    query.exec("DROP TABLE " + tableName); //先清空一下表
    //    qDebug()<<sql;
    int ret = query.exec(sql);          //建立一张统计信息表
    if(!ret)
    {
        qDebug()<<"create table fail";
        db.close();
        return false;
    }
    db.close();
    return true;
}

/* 更新数据库表信息*/
bool MySqlite::updateTable(QString tableName)
{
#if 0
#ifdef Q_OS_UNIX
    //    myMqttMsgParaseThread->uploadStatistics();
    //    myMqttThread->uploadBlowCounts();
#endif
    if(!openDatabase())
    {
        return false;
    }
    if(!isTableExist(tableName))
    {
        closeDatabase();
        return false;
    }

    stu_ipc_model_class modelClassIf;
    QString classNameStr;
    memcpy(&modelClassIf, &struIpcShare.struIpcModelClass[0],sizeof(modelClassIf));

    if(isItemExist(tableName, QDateTime::currentDateTime().toString("yyyy-MM-dd")))
    {
        QSqlQuery query(db);
        QString sql = "UPDATE "+tableName+" SET ";
        for(int i = 0;i < struCnfp.ipcModelClassCount[0][0];i++)
        {
            classNameStr = modelClassIf.className[i];
            if(classNameStr.left(3).contains("_"))
                classNameStr.remove(classNameStr.left(3));
            sql.append(QString("%1 = %2,").arg(classNameStr).arg(struIpcShare.sCurModelClassBadPointsCount[i]+struIpcShare.sCurModelLastClassBadPointsCount[i]));
        }
        sql.remove(sql.length()-1, 1);
        sql.append(QString(" WHERE CreatedTime = '%1'").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")));
        query.prepare(sql);
        //        qDebug()<<sql;
        int ret = query.exec();
        if(!ret)
        {
            qDebug()<<"update table fail";
            db.close();
            return false;
        }
        db.close();
        return true;
    }
    else
    {
        headName.clear();
        value.clear();
        QSqlQuery query(db);
        QString sql = "INSERT INTO " +tableName+"(";

        for(int i = 0;i < struCnfp.ipcModelClassCount[0][0];i++)
        {
            classNameStr = modelClassIf.className[i];
            if(classNameStr.left(3).contains("_"))
                classNameStr.remove(classNameStr.left(3));
            headName.append(QString("%1 , ").arg(classNameStr));
            value.append(QString(":%1 , ").arg(classNameStr));
        }
        headName.remove(headName.length() - 2,1);
        headName.append(")");  //把最后一个字符串里面的逗号去除，并添加右括号

        value.remove(value.length() -2,1);
        value.append(")");

        sql.append(headName);
        sql.append("VALUES (");
        sql.append(value);

        query.prepare(sql); //为每一列标题添加绑定值

        //    db.transaction();  //开启事务
        for(int i = 0;i < struCnfp.ipcModelClassCount[0][0];i++)
        {
            classNameStr = modelClassIf.className[i];
            if(classNameStr.left(3).contains("_"))
                classNameStr.remove(classNameStr.left(3));
            query.bindValue(QString(":%1 ").arg(classNameStr),struIpcShare.sCurModelClassBadPointsCount[i]+struIpcShare.sCurModelLastClassBadPointsCount[i]);
        }
        //        qDebug()<<query.lastQuery();
        int ret = query.exec();
        if(!ret)
        {
            qDebug()<<"insert table fail";
            db.close();
            return false;
        }
        //   db.commit(); //提交
        db.close();
        return true;
    }
#endif
}
bool MySqlite::clearTable(QString tableName)
{
    if(!openDatabase())
    {
        return false;
    }
    /* 表不存在，直接退出 */
    if(!isTableExist(tableName))
    {
        db.close();
        return true;
    }
    QSqlQuery query(db);
    QString sql = "DELETE FROM "+tableName+" WHERE CreatedTime < datetime('now','start of day','+1 day')";

    int ret = query.exec(sql);
    if(!ret)
    {
        qDebug()<<"clear table fail";
        db.close();
        return false;
    }
    ret = query.exec("VACUUM");
    if(!ret)
    {
        qDebug()<<"vacuum table fail";
        db.close();
        return false;
    }

    db.close();
    return true;
}

bool MySqlite::creatstaticTable(QString tableName,int view,int unit)
{
    if(!openDatabase())
    {
        return false;
    }
    if(isTableExist(tableName))
    {
        QDate sixtyDaysAgo = QDate::currentDate().addDays(-60);
        QString tmpdate60 = sixtyDaysAgo.toString("yyyyMMdd");
        qDebug()<<"before 60 date"<<tmpdate60;
        deleteExpiredata(g_Runtime().modelFullNametest+ QString("_%1_%2").arg(view).arg(unit), tmpdate60 + QString::number(24) );
        deleteExpiredata(g_Runtime().modelFullName_Area+ QString("_%1_%2").arg(view).arg(unit), tmpdate60 + QString::number(24) );
        deleteExpiredata(g_Runtime().modelFullName_Frame+ QString("_%1_%2").arg(view).arg(unit), tmpdate60 + QString::number(24) );
        system("sync");
        db.close();
        return true;
    }
    QString classNameStr;
    QSqlQuery query(db);
    QString sql = "CREATE TABLE ";
    sql.append( tableName + "(");
    headName.clear();
    headName.append(QString("%1 varchar(20), ").arg("CreatedTime")); // 增加数据更新时间列
    headName.append(QString("%1 varchar(20), ").arg("piciname"));        // 数据批次列


    for(int i = 0; i < MAX_IPC_MODEL_CLASS; i++)
    {
        classNameStr =  QString(struIpcShare.struIpcModelClass[view][unit].className[i]);
        if(classNameStr.contains("_"))
        {
            QStringList tmplist = classNameStr.split("_");
            classNameStr= "X" + tmplist.at(1);
            if(tmplist.size() > 2)
                classNameStr+=tmplist.at(2);
        }
        if( i >= struCnfp.ipcModelClassCount[view][0] - 1 )
            break;
        headName.append(QString("%1 INTEGER, ").arg(classNameStr));
    }
    headName.append(QString("%1 INTEGER").arg(classNameStr));

    headName.append(")");  //把最后一个字符串里面的逗号去除，并添加右括号
    sql.append(headName);
    query.exec("DROP TABLE " + tableName); //先清空一下表
    int ret = query.exec(sql);          //建立一张统计信息表
    if(!ret)
    {
        qDebug()<<"create table fail";
        db.close();
        return false;
    }
    db.close();
    qDebug()<<"create table ok";
    return true;
}

bool MySqlite::updateTable_2(QVector<QString>  tableNamelist,int type)
{

  QString piciname = g_Runtime().piciname;
  QString tableName;
  stu_ipc_model_class modelClassIf;
  QString classNameStr;
  int view;
  int unit;
    if(!openDatabase())
    {
        return false;
    }
    // ========== 开启事务（批量插入必须用，速度提升100倍）==========
    if (!db.transaction()) {
        qDebug()  << " 开启事务失败：";
        return false;
    }


    for(int tmp1= 0; tmp1 < tableNamelist.size(); tmp1++)
    {
        qDebug()<<"tablelist"<<tableNamelist.size() <<tmp1<<tableNamelist.at(tmp1);
        tableName = tableNamelist.at(tmp1);
        if(!isTableExist(tableName))
        {
            continue;
        }
        QStringList tmplist = tableName.split("_");


        view = tmplist.at(tmplist.size()-2).toInt();
        unit =  tmplist.at(tmplist.size()-1).toInt();

        classNameStr.clear();
        memcpy(&modelClassIf, &struIpcShare.struIpcModelClass[view][unit],sizeof(modelClassIf));

        headName.clear();
        value.clear();
        QSqlQuery query(db);
        QString sql = "INSERT INTO " +tableName+"(";
        headName.append(QString("%1 , ").arg("CreatedTime"));
        value.append(QString(":%1 , ").arg("CreatedTime"));
        headName.append(QString("%1 , ").arg("piciname"));
        value.append(QString(":%1 , ").arg("piciname"));


        for(int i = 0;i < MAX_IPC_MODEL_CLASS;i++)
        {
            classNameStr = modelClassIf.className[i];
            if(classNameStr.contains("_"))
            {
                QStringList tmplist = classNameStr.split("_");
                classNameStr= "X" +tmplist.at(1);
                if(tmplist.size() > 2)
                    classNameStr+=tmplist.at(2);
            }
            if(i >=  struCnfp.ipcModelClassCount[view][0])
            {
                break;
            }
            headName.append(QString("%1 , ").arg(classNameStr));
            value.append(QString(":%1 , ").arg(classNameStr));
        }


        headName.remove(headName.length() - 2,1);
        headName.append(")");  //把最后一个字符串里面的逗号去除，并添加右括号
        value.remove(value.length() -2,1);
        value.append(")");
        sql.append(headName);
        sql.append("VALUES (");
        sql.append(value);
        query.prepare(sql); //为每一列标题添加绑定值
        QString fileName = QDateTime::currentDateTime().toString("yyyyMMddhh");
        query.bindValue(QString(":%1 ").arg("CreatedTime"), fileName);
        query.bindValue(QString(":%1 ").arg("piciname"), piciname);

        for(int i = 0;i < MAX_IPC_MODEL_CLASS ;i++)
        {
            if(i >=  struCnfp.ipcModelClassCount[view][0])
            {
                break;
            }
            classNameStr = modelClassIf.className[i];
            if(classNameStr.contains("_"))
            {
                QStringList tmplist = classNameStr.split("_");
                classNameStr= "X" +tmplist.at(1);
                if(tmplist.size() > 2)
                    classNameStr+=tmplist.at(2);
            }
            if(type == 0)
                query.bindValue(QString(":%1 ").arg( classNameStr), struIpcShare.sCurModelClassBadPointsCount[view][unit][i]);
            else if(type == 1)
                query.bindValue(QString(":%1 ").arg( classNameStr), struIpcShare.sCurModelClassBadPointsArea[view][unit][i]);
            else if(type == 2)
                query.bindValue(QString(":%1 ").arg( classNameStr), struIpcShare.sCurModelClassBadPointsFrameArea[view][unit][i]);

            if(view == 4 & unit ==0)
                qDebug()<<struIpcShare.sCurModelClassBadPointsCount[view][unit][i];
        }
        int ret = query.exec();
        if(!ret)
        {
            qDebug()<<"insert table fail";

        }
        else
        {
        }
    }
    db.commit();
    db.close();
    return true;
}

bool MySqlite::updateTable(QString tableName, int view, int unit , int type)
{

    QString piciname = g_Runtime().piciname;

    if(!openDatabase())
    {
        return false;
    }
    if(!isTableExist(tableName))
    {
        return false;

        if(!creatstaticTable(tableName,view,unit))
        {
            closeDatabase();
            return false;
        }
    }
    stu_ipc_model_class modelClassIf;
    QString classNameStr;
    classNameStr.clear();
    memcpy(&modelClassIf, &struIpcShare.struIpcModelClass[view][unit],sizeof(modelClassIf));

    headName.clear();
    value.clear();
    QSqlQuery query(db);
    QString sql = "INSERT INTO " +tableName+"(";
    headName.append(QString("%1 , ").arg("CreatedTime"));
    value.append(QString(":%1 , ").arg("CreatedTime"));
    headName.append(QString("%1 , ").arg("piciname"));
    value.append(QString(":%1 , ").arg("piciname"));


    for(int i = 0;i < MAX_IPC_MODEL_CLASS;i++)
    {
        classNameStr = modelClassIf.className[i];
        if(classNameStr.contains("_"))
        {
            QStringList tmplist = classNameStr.split("_");
            classNameStr= "X" +tmplist.at(1);
            if(tmplist.size() > 2)
                classNameStr+=tmplist.at(2);
        }
        if(i >=  struCnfp.ipcModelClassCount[view][0])
        {
            break;
        }
        headName.append(QString("%1 , ").arg(classNameStr));
        value.append(QString(":%1 , ").arg(classNameStr));
    }
    headName.remove(headName.length() - 2,1);
    headName.append(")");  //把最后一个字符串里面的逗号去除，并添加右括号
    value.remove(value.length() -2,1);
    value.append(")");
    sql.append(headName);
    sql.append("VALUES (");
    sql.append(value);
    query.prepare(sql); //为每一列标题添加绑定值
    QString fileName = QDateTime::currentDateTime().toString("yyyyMMddhh");
    query.bindValue(QString(":%1 ").arg("CreatedTime"), fileName);
    query.bindValue(QString(":%1 ").arg("piciname"), piciname);

    for(int i = 0;i < MAX_IPC_MODEL_CLASS ;i++)
    {
        if(i >=  struCnfp.ipcModelClassCount[view][0])
        {
            break;
        }
        classNameStr = modelClassIf.className[i];
        if(classNameStr.contains("_"))
        {
            QStringList tmplist = classNameStr.split("_");
            classNameStr= "X" +tmplist.at(1);
            if(tmplist.size() > 2)
                classNameStr+=tmplist.at(2);
        }
        if(type == 0)
            query.bindValue(QString(":%1 ").arg( classNameStr), struIpcShare.sCurModelClassBadPointsCount[view][unit][i]);
        else if(type == 1)
            query.bindValue(QString(":%1 ").arg( classNameStr), struIpcShare.sCurModelClassBadPointsArea[view][unit][i]);
        else if(type == 2)
            query.bindValue(QString(":%1 ").arg( classNameStr), struIpcShare.sCurModelClassBadPointsFrameArea[view][unit][i]);
    }
    int ret = query.exec();
    if(!ret)
    {
        qDebug()<<"insert table fail";
        db.close();
        return false;
    }
    else
    {
    }
    db.commit();
    db.close();
    return true;

}
bool MySqlite::deleteTable(QString tableName)
{
    if(!openDatabase())
    {
        return false;
    }
    if(!isTableExist(tableName))
    {
        closeDatabase();
        return true;
    }
    QSqlQuery query(db);
    QString sql = "DROP TABLE "+tableName;

    int ret = query.exec(sql);
    if(!ret)
    {
        qDebug()<<"delete table fail";
        db.close();
        return false;
    }
}
bool MySqlite::clearStatisTable(QString tableName,QString CreatedTime )
{
    if(!openDatabase())
    {
        return false;
    }
    if(!isTableExist(tableName))
    {
        db.close();
        return true;
    }
    QSqlQuery query(db);
    QString sql;
    if(CreatedTime != "")
    {
        sql = "DELETE FROM "+tableName+" WHERE CreatedTime >=" + CreatedTime ;
    }
    else
    {
        sql = "DELETE FROM "+tableName  ;
    }
    int ret = query.exec(sql);
    if(!ret)
    {
        qDebug()<<"clear table fail";
        db.close();
        return false;
    }
    ret = query.exec("VACUUM");
    if(!ret)
    {
        qDebug()<<"vacuum table fail";
        db.close();
        return false;
    }
    db.commit();
    db.close();
    return true;
}

bool MySqlite::clearStatisTable(QStringList tabellist, QString CreatedTime )
{
    if(!openDatabase())
    {
        return false;
    }


    QString sql;
    QString tableName;
    for(int tmp1 = 0; tmp1 < tabellist.size(); tmp1++)
    {
         db.transaction();
         QSqlQuery query(db);
         tableName = tabellist.at(tmp1);

         if(!isTableExist(tableName))
         {
            qDebug()<<"table no exist";
         }

         if(CreatedTime != "")
         {
             sql = "DELETE FROM "+tableName+" WHERE CreatedTime >=" + CreatedTime ;
         }
         else
         {
             sql = "DELETE FROM "+tableName  ;
         }
         int ret = query.exec(sql);
         if(!ret)
         {
             qDebug()<<"clear table fail";

         }
         ret = query.exec("VACUUM");
         if(!ret)
         {
             qDebug()<<"vacuum table fail";
         }
         db.commit();

    }

    db.close();
    return true;
}


bool MySqlite::deleteExpiredata(QString tableName,QString value)
{
    if(!openDatabase())
    {
        return false;
    }
    if(!isTableExist(tableName))
    {
        closeDatabase();
        return false;
    }
    QSqlQuery query(db);
    query.prepare(QString("DELETE FROM %1 WHERE CreatedTime < ?").arg(tableName));
    query.addBindValue(value);
    if (!query.exec()) {
        qDebug() << "[删除数据] 执行删除SQL失败";
        return false;
    }
    if (!db.commit()) {
        return false;
    }
    return true;
}

bool MySqlite::getCurItemInfo(QString tableName)
{
    memset(struIpcShare.sCurModelLastClassBadPointsCount,0,sizeof(struIpcShare.sCurModelLastClassBadPointsCount));
    if(!openDatabase())
    {
        return false;
    }
    if(!isTableExist(tableName))
    {
        closeDatabase();
        return true;
    }
    QSqlQuery query(db);
    QString sql = QString("select * from "+ tableName +" where CreatedTime = date('now','localtime');");
    query.exec(sql);
    while(query.next())
    {
        for(int i = 0;i < struCnfp.ipcModelClassCount[struGsh.nView][0];i++)
        {
            struIpcShare.sCurModelLastClassBadPointsCount[i] = query.value(i+1).toInt();
        }
    }

}

/* 计算指定时间段内某一列元素的平均值 */
double MySqlite::calculateSingleColumnAverage(QString t_begin, QString t_end, QString columnName)
{
    int cnt = 0;    //当前数据的个数
    double evr = 0; //当前的平均值
    if(!openDatabase())
    {
        qDebug()<<"STATISTICDB open fail!";
        return 0;
    }
    QSqlQuery query(db);
    QString sql = QString("select %1 from LAYER1_STATISTIC_DATA "
                          "where CreatedTime >= '%2' and CreatedTime <= '%3'")
            .arg(columnName).arg(t_begin).arg(t_end);

    qDebug()<<sql;
    query.exec(sql);

    while(query.next())
    {
        /* 大数求值防止溢出 */
        evr +=(double) ((query.value(0).toInt()- evr)/++cnt);
    }
    db.close();
    return evr;
}

/* 检查表是否存在 */
bool MySqlite::isTableExist(QString tableName)
{
    if(!openDatabase())
    {
        qDebug()<<"STATISTICDB open fail!";
        return 0;
    }
    QSqlQuery query(db);
    QString sql = QString("select * from sqlite_master where name='%1'").arg(tableName);
    query.exec(sql);
    return query.next();
}

/* 检查本日首条信息是否存在 */
bool MySqlite::isItemExist(QString table, QString item)
{
    if(!openDatabase())
    {
        qDebug()<<"STATISTICDB open fail!";
        return 0;
    }
    QSqlQuery query(db);
    QString sql = QString("select * from "+ table +" where CreatedTime = '%1'").arg(item);
    qDebug()<<sql;
    query.exec(sql);
    return query.next();
}

/* 产量信息为阶段时间内（数据库更新间隔）每一次查询的累计 */
void MySqlite::updateStagedThroughout()
{
    //暂只统计一次前主的产量信息
    int j = 0,index = 0;
    double tempvalue = 0.0;
    for(int i = 0;i < struCnfg.nLayerTotal;i++)
    {
        for(int j=0;j<struCnfg.struLayerInfo[i].nViewChainTotal;j++)
        {
            if (struCnfg.struLayerInfo[i].nViewIsUsed[j] && struCnfg.struLayerInfo[i].nViewBoardType[j] == BOARD_CAMERA_1_FRONT_MAIN)
            {
                index = j;//获取到前主真正的索引
            }
        }
    }

    if (struCnfg.sStaticsGetGroupEnable[0][index][0] == 1)
    {
        for(int m = 0;m <  struCnfg.struLayerInfo[0].stuViewInfo[index].stuIdenGupInfo[0].nGroupUnitTotal ; m++)
        {
            //产量累计存储单位均为千克
            tempvalue = (float)struGsh.struStatics[0][j][m].nThroughout/(float)struCnfp.n_coff;
            //          tempvalue = (float)struGsh.struStatics[0][j][m].nThroughout/(float)struCnfg.n_coff*36000/(float)struGsh.struStatics[0][j][m].nStaticsPeriod;
            //          qDebug()<<struGsh.struStatics[0][j][m].nThroughout<<"--"<<struCnfg.n_coff<<"--"<<struGsh.struStatics[0][j][m].nStaticsPeriod<<"--"<<tempvalue;
            struGsh.stagedThroughout[0][j][m] += tempvalue;
            //累计吹气次数默认使用前后视吹气次数之和一半
            struGsh.stagedTotalArithEjectTimes[0][j][m] += struGsh.chanelArithEjectTimes[0][index][m] ;
            //struGsh.stagedTotalArithEjectTimes[0][j][m] += (struGsh.struStatics[0][j][m].nTotalArithEjectTimes+struGsh.struStatics[0][j+1][m].nTotalArithEjectTimes)/2;
        }
    }
}

/* 更新数据库时将上一阶段时间内产量信息清零*/
void MySqlite::clearStagedThroughout()
{
    memset(struGsh.stagedThroughout,0,sizeof(struGsh.stagedThroughout));
    memset(struGsh.stagedTotalArithEjectTimes,0,sizeof(struGsh.stagedTotalArithEjectTimes));
    memset(struGsh.chanelArithEjectTimes,0,sizeof(struGsh.chanelArithEjectTimes));
}


//统计信息查询线程
queryStatisticThread::queryStatisticThread()
{
    clearDataMap();
    m_nStatisticType = Throughout;
    m_nQueryType = RealTime;
    m_nViewType = MainFront;

    isQuerying = false;
    isNeedInserting = false;
    isNeedRepainting = false;

    m_dataCount = 0;
    index = 0;

    memset(m_realTimeTotalDataInfo,0,sizeof(m_realTimeTotalDataInfo));
    //数据库初始化
    MySqlite mysql;
    mysql.creatTable();

}
queryStatisticThread::~queryStatisticThread()
{
}

void queryStatisticThread::clearDataMap()
{
    m_dataCount = 0;
    m_dataValueList.clear();
    m_dataStrList.clear();
    m_dataArithStrList.clear();

    m_nameStrList.clear();

}

void queryStatisticThread::setQueryInfo(StatisticType sType, QueryType qType,ViewType vType)
{
    m_nStatisticType = sType;
    m_nQueryType = qType;
    m_nViewType = vType;
}

void queryStatisticThread::updateCurrentAriths()
{
    for(int i = 0;i < struCnfg.nLayerTotal;i++)
    {
        for(int j = 0; j <struCnfg.struLayerInfo[i].nViewChainTotal;j++)
        {
            if (j != m_nViewType) //刷新当前视的使能算法
                continue;
            for(int k = 0; k < struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal; k++) //
            {
                if (struCnfg.sStaticsGetGroupEnable[i][j][k] == 1)
                {
                    for (int n = 0;n < ARITHMETIC_TOTAL;n++)
                    {
                        switch (n) {
                        case ARITH_GREY_A:
                        case ARITH_GREY_B:
                        case ARITH_GREY_C:
                        case ARITH_GREY_D:
                        case ARITH_DISCOLOR_A:
                        case ARITH_DISCOLOR_B:
                        case ARITH_CROSS:
                            if(struCnfp.struGroupIdentify[i][j][k].struGreyColor[n].nArithEnable)
                                getArithName(n);
                            break;
                        case ARITH_SVM_A:
                        case ARITH_SVM_B:
                        case ARITH_SVM_C:
                        case ARITH_SVM_D:
                            if(struCnfp.struGroupIdentify[i][j][k].struSvm[n-ARITH_SVM_A].nArithEnable)
                                getArithName(n);
                            break;
                        case ARITH_SHAPE_SVM_A:
                        case ARITH_SHAPE_SVM_B:
                        case ARITH_SHAPE_SVM_C:
                            if(struCnfp.struGroupIdentify[i][j][k].struShapeSvm[n-ARITH_SHAPE_SVM_A].nArithEnable)
                                getArithName(n);
                            break;
                        case ARITH_BIG_SMALL:
                        case ARITH_BIG_SMALL_B:
                            if(struCnfp.struGroupIdentify[i][j][k].struBigSmall[n-ARITH_BIG_SMALL].nArithEnable)
                                getArithName(n);
                            break;
                        case ARITH_LONG_SHORT:
                        case ARITH_LONG_SHORT_B:
                            if(struCnfp.struGroupIdentify[i][j][k].struLongShort[n-ARITH_LONG_SHORT].nArithEnable)
                                getArithName(n);
                            break;
                        case ARITH_CIRCLE_LONG:
                        case ARITH_CIRCLE_LONG_B:
                            if(struCnfp.struGroupIdentify[i][j][k].struCircleLong[n-ARITH_CIRCLE_LONG].nArithEnable)
                                getArithName(n);
                            break;
                        case ARITH_HSV:
                            if(struCnfp.struGroupIdentify[i][j][k].struHsv[n-ARITH_HSV].nArithEnable)
                                getArithName(n);
                            break;
                        case ARITH_SAT:
                            if(struCnfp.struGroupIdentify[i][j][k].struColorSat.nArithEnable)
                                getArithName(n);
                            break;
                        case ARITH_SCALE:
                            if(struCnfp.struGroupIdentify[i][j][k].struColorScale.nArithEnable)
                                getArithName(n);
                            break;
                        case ARITH_MILDEW:
                            if(struCnfp.struGroupIdentify[i][j][k].struMildew.nArithEnable)
                                getArithName(n);
                            break;
                        case ARITH_OUTLINE:
                            if(struCnfp.struGroupIdentify[i][j][k].struOutline.nArithEnable)
                                getArithName(n);
                            break;
                        case ARITH_LINE:
                            if(struCnfp.struGroupIdentify[i][j][k].struLine.nArithEnable)
                                getArithName(n);
                            break;
                        case ARITH_BUD:
                            if(struCnfp.struGroupIdentify[i][j][k].struBud[n-ARITH_BUD].nArithEnable)
                                getArithName(n);
                            break;
                        case ARITH_ROUND:
                            if(struCnfp.struGroupIdentify[i][j][k].struRound[n-ARITH_ROUND].nArithEnable)
                                getArithName(n);
                            break;
                        case ARITH_SPL_MILDEW:
                            if(struCnfp.struGroupIdentify[i][j][k].struSplMildew.nArithEnable)
                                getArithName(n);
                            break;
                        default:
                            break;
                        }
                    }
                }
            }
        }

    }

    m_arith_dataCount = m_arithNameStrList.count(); //所有使能算法数量
}

void queryStatisticThread::getArithName(int n)
{
    QStringList totalArithList,totalarithDBList;

    totalArithList<< g_myLan().arith_gray_a<< g_myLan().arith_gray_b<< g_myLan().arith_gray_c<< g_myLan().arith_gray_d
                  << g_myLan().arith_discolor_a<< g_myLan().arith_discolor_b <<g_myLan().arith_cross
                  << g_myLan().arith_svm_a<< g_myLan().arith_svm_b<< g_myLan().arith_svm_c
                  << g_myLan().shape+g_myLan().arith_svm_a<<g_myLan().shape+g_myLan().arith_svm_b<<g_myLan().shape+g_myLan().arith_svm_c
                  << g_myLan().arith_big_small << g_myLan().arith_long_short << g_myLan().arith_circle_long
                  << g_myLan().arith_hsv << g_myLan().arith_sat << g_myLan().arith_scale << g_myLan().arith_mildew
                  << g_myLan().arith_outline << g_myLan().arith_line << g_myLan().arith_bud << g_myLan().arith_round
                  << g_myLan().arith_spl_mildew;
    totalarithDBList << "GreyA"<<"GreyB"<<"GreyC"<<"GreyD"<<"DiscolorA"<<"DiscolorB"<<"Cross"
                     << "SvmA"<<"SvmB"<<"SvmC"<<"ShapeSvmA"<<"ShapeSvmB"<<"ShapeSvmC"
                     <<"BigSmall"<<"LongShort"<<"circlelong"<< "hue"<<"sat"<<"scale"<<"mildew"
                    <<"outline"<<"line"<<"bud"<<"round"<<"splMildew";

    if(!m_arithNameStrList.contains(totalArithList.at(n)))
        m_arithNameStrList.append(totalArithList.at(n));
    if(!m_arithDBNameStrList.contains(totalarithDBList.at(n)))
        m_arithDBNameStrList.append(totalarithDBList.at(n));
}


void queryStatisticThread::updateCurrentChutes()
{
    for(int i =0;i<struCnfg.nChuteTotal;i++ )
    {
        m_nameStrList.append(QString::number(i+1));
    }
    m_dataCount = m_nameStrList.count(); //所有通道数

}

void queryStatisticThread::run()
{
    int tempvalue = 0,tempcnt = 0; //一小时内的实际信息
    QString arithName,statisticName,sql;
    MySqlite mysql;
    int curHour;

    while (1)
    {
        if(!isQuerying && !isNeedInserting)
        {
            msleep(100);
            continue;
        }

        if(m_nQueryType != RealTime)
        {
            if(!mysql.openDatabase())
            {
                qDebug()<<"STATISTICDB open fail!";
                return ;
            }
        }
        QSqlQuery query(mysql.db);

        if (isQuerying)
        {
            switch (m_nStatisticType){
            case Throughout:
                index = 0;
                arithName = "ThroughOut" ;
                break;
            case ArithEjectTimes:
                arithName = "TotalEjectTimes" ;
                index = 1;
                break;
            }

            switch (index){
            case 0: //总产量查询
                switch (m_nQueryType){
                case LastDay:

                    memset(m_realTimeTotalDataInfo,0,sizeof(m_realTimeTotalDataInfo));
                    m_dataCount = 12;
                    curHour = QDateTime::currentDateTime().time().hour();
                    for(int i = -12; i <0; i++)
                    {
                        tempvalue = 0; int j = 0,k = 0;

                        /* 2020-06-10  去除总吹气次数的单独查询，总产量只累计一次前主*/
                        for(int m = 0;m < struCnfg.struLayerInfo[0].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++)
                        {
                            statisticName = QString("%1%2_%3").arg(mysql.viewName.at(j)).arg(m).arg(arithName);

                            sql = "select " + statisticName + " from LAYER1_STATISTIC_DATA where CreatedTime >= datetime('now','localtime','" + QString::number(i*2-curHour)
                                    + " hour') and CreatedTime < datetime('now','localtime','" + QString::number(i*2+2-curHour) + " hour')";

                            query.exec(sql);

                            while(query.next()&isQuerying)
                            {
                                tempvalue += query.value(0).toInt();
                            }
                            if (!isQuerying)
                            {
                                if(isNeedInserting)
                                {
                                    mysql.updateTable();
                                    mysql.clearStagedThroughout();
                                    isNeedInserting = false;
                                }

                                mysql.closeDatabase();
                                return;
                            }
                        }
                        m_dataValueList.append(tempvalue);
                        m_dataStrList.append(QString::number(24+i*2));

                        m_realTimeTotalDataInfo[0] += tempvalue; //总产量

                    }

                    //含杂查询
                    tempvalue = 0;
                    for(int i = -1 ;i < 0; i++ )
                    {
                        for(int j = 0; j <struCnfg.struLayerInfo[0].nViewChainTotal;j++)
                        {
                            for(int k = 0; k < struCnfg.struLayerInfo[0].stuViewInfo[j].nIdentifyGroupTotal; k++)
                            {
                                for(int m = 0;m < struCnfg.struLayerInfo[0].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++)
                                {
                                    statisticName = QString("%1%2_%3").arg(mysql.viewName.at(j)).arg(m).arg("TotalEjectTimes");

                                    sql = "select " + statisticName + " from LAYER1_STATISTIC_DATA where CreatedTime >= datetime('now','start of day','" + QString::number(i)
                                            + " day') and CreatedTime < datetime('now','start of day','" + QString::number(i+1) +" day')";

                                    query.exec(sql);

                                    while(query.next()&isQuerying)
                                    {
                                        if (m_nViewType == ViewType(j))
                                            tempvalue += query.value(0).toInt();

                                    }
                                    if (!isQuerying)
                                    {
                                        if(isNeedInserting)
                                        {
                                            mysql.updateTable();
                                            mysql.clearStagedThroughout();
                                            isNeedInserting = false;
                                        }

                                        mysql.closeDatabase();
                                        return;
                                    }
                                }
                            }
                        }

                        m_realTimeTotalDataInfo[1] += tempvalue; //总吹气次数
                    }
                    break;

                case LastWeek:
                    m_dataCount = 7;

                    memset(m_realTimeTotalDataInfo,0,sizeof(m_realTimeTotalDataInfo));
                    for(int i = -7; i < 0; i++)
                    {
                        tempvalue = 0;
                        int j = 0,k = 0;
                        for(int m = 0;m < struCnfg.struLayerInfo[0].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++)
                        {
                            statisticName = QString("%1%2_%3").arg(mysql.viewName.at(j)).arg(m).arg(arithName);

                            sql = "select " + statisticName + " from LAYER1_STATISTIC_DATA where CreatedTime >= datetime('now','start of day','" + QString::number(i)
                                    + " day') and CreatedTime < datetime('now','start of day','" + QString::number(i+1) + " day')";

                            query.exec(sql);

                            while(query.next()&isQuerying)
                            {
                                tempvalue += query.value(0).toInt();
                            }
                            if (!isQuerying)
                            {
                                if(isNeedInserting)
                                {
                                    mysql.updateTable();
                                    mysql.clearStagedThroughout();
                                    isNeedInserting = false;
                                }

                                mysql.closeDatabase();
                                return;
                            }
                        }

                        m_dataValueList.append(tempvalue);
                        m_dataStrList.append(QString::number(7+i));

                        m_realTimeTotalDataInfo[0] += tempvalue; //总产量
                    }


                    tempvalue = 0;
                    for(int i = -7 ;i < 0; i++ )
                    {
                        for(int j = 0; j <struCnfg.struLayerInfo[0].nViewChainTotal;j++)
                        {
                            for(int k = 0; k < struCnfg.struLayerInfo[0].stuViewInfo[j].nIdentifyGroupTotal; k++)
                            {
                                for(int m = 0;m < struCnfg.struLayerInfo[0].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++)
                                {
                                    statisticName = QString("%1%2_%3").arg(mysql.viewName.at(j)).arg(m).arg("TotalEjectTimes");

                                    sql = "select " + statisticName + " from LAYER1_STATISTIC_DATA where CreatedTime >= datetime('now','start of day','" + QString::number(i)
                                            + " day') and CreatedTime < datetime('now','start of day','" + QString::number(i+1) +" day')";
                                    query.exec(sql);

                                    while(query.next()&isQuerying)
                                    {
                                        if (m_nViewType == ViewType(j))
                                            tempvalue += query.value(0).toInt();
                                    }
                                    if (!isQuerying)
                                    {
                                        if(isNeedInserting)
                                        {
                                            mysql.updateTable();
                                            mysql.clearStagedThroughout();
                                            isNeedInserting = false;
                                        }

                                        mysql.closeDatabase();
                                        return;
                                    }
                                }

                            }
                        }

                        m_realTimeTotalDataInfo[1] += tempvalue; //总吹气次数
                    }

                    break;

                case Last3Month:
                    m_dataCount = 3;
                    memset(m_realTimeTotalDataInfo,0,sizeof(m_realTimeTotalDataInfo));
                    for(int i = -3; i < 0; i++)
                    {
                        tempvalue = 0;
                        int j = 0,k = 0;

                        for(int m = 0;m < struCnfg.struLayerInfo[0].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++)
                        {
                            statisticName = QString("%1%2_%3").arg(mysql.viewName.at(j)).arg(m).arg(arithName);

                            sql = "select " + statisticName + " from LAYER1_STATISTIC_DATA where CreatedTime >= date('now','" + QString::number(i)
                                    + " month') and CreatedTime < date('now','" + QString::number(i+1) + " month')";

                            query.exec(sql);

                            while(query.next()&isQuerying)
                            {
                                tempvalue += query.value(0).toInt();
                            }
                            if (!isQuerying)
                            {
                                if(isNeedInserting)
                                {
                                    mysql.updateTable();
                                    mysql.clearStagedThroughout();
                                    isNeedInserting = false;
                                }

                                mysql.closeDatabase();
                                return;
                            }
                        }

                        m_dataValueList.append(tempvalue);
                        m_dataStrList.append(QString::number(3+i));

                        m_realTimeTotalDataInfo[0] += tempvalue; //总产量
                    }


                    tempvalue = 0;
                    for(int i = -3 ;i < 0; i++ )
                    {
                        for(int j = 0; j <struCnfg.struLayerInfo[0].nViewChainTotal;j++)
                        {
                            for(int k = 0; k < struCnfg.struLayerInfo[0].stuViewInfo[j].nIdentifyGroupTotal; k++)
                            {
                                for(int m = 0;m < struCnfg.struLayerInfo[0].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++)
                                {
                                    statisticName = QString("%1%2_%3").arg(mysql.viewName.at(j)).arg(m).arg("TotalEjectTimes");

                                    sql = "select " + statisticName + " from LAYER1_STATISTIC_DATA where CreatedTime >= date('now','" + QString::number(i)
                                            + " month') and CreatedTime < date('now','" + QString::number(i+1) + " month')";

                                    query.exec(sql);

                                    while(query.next()&isQuerying)
                                    {
                                        if (m_nViewType == ViewType(j))
                                            tempvalue += query.value(0).toInt();
                                    }
                                    if (!isQuerying)
                                    {
                                        if(isNeedInserting)
                                        {
                                            mysql.updateTable();
                                            mysql.clearStagedThroughout();
                                            isNeedInserting = false;
                                        }

                                        mysql.closeDatabase();
                                        return;
                                    }
                                }
                            }
                        }
                        m_realTimeTotalDataInfo[1] += tempvalue; //总吹气次数
                    }
                    break;

                case RealTime:
                    tempcnt = 0;
                    int tempvalue2;
                    memset(m_realTimeTotalDataInfo,0,sizeof(m_realTimeTotalDataInfo));
                    m_dataCount = struCnfg.nChuteTotal;

                    int j = 0 ,k = 0;
                    for(int m = 0;m < struCnfg.struLayerInfo[0].nViewUnitTotal[j] ; m++)
                    {
                        //实时产量
                        if(struGsh.struStatics[0][j][m].nThroughout == 0)
                            tempvalue = 0;
                        else
                            tempvalue = (float)struGsh.struStatics[0][j][m].nThroughout/(float)struCnfp.n_coff*36000/(float)struGsh.struStatics[0][j][m].nStaticsPeriod;


                        //实时总含杂（总的吹气次数）
                        if(struGsh.struStatics[0][j][m].nTotalArithEjectTimes == 0)
                            tempvalue2 = 0;
                        else
                            tempvalue2 = (float)struGsh.struStatics[0][j][m].nTotalArithEjectTimes*36000/(float)struGsh.struStatics[0][j][m].nStaticsPeriod;

                        // qDebug()<<struGsh.struStatics[0][j][m].nThroughout<<"--"<<struCnfg.n_coff<<"--"<<struGsh.struStatics[0][j][m].nStaticsPeriod<<"--"<<tempvalue;

                        m_dataValueList.append(tempvalue);
                        m_dataStrList.append(m_nameStrList.at(m));

                        //只累计一次的总产量
                        if (m <  struCnfg.struLayerInfo[0].stuViewInfo[j].stuIdenGupInfo[0].nGroupUnitTotal)
                        {
                            m_realTimeTotalDataInfo[0] += tempvalue; //总产量
                            m_realTimeTotalDataInfo[1] += tempvalue2;//总吹气次数
                        }
                    }

                    break;
                }
                break;

            case 1: //单个含杂查询
                switch (m_nQueryType){
                case LastDay:
                    memset(m_realTimeTotalDataInfo,0,sizeof(m_realTimeTotalDataInfo));
                    for(int n = 0;n < m_dataStrList.count(); n++)
                    {
                        tempvalue = 0;
                        for(int i = -1 ;i < 0; i++ )
                        {
                            for(int j = 0; j <struCnfg.struLayerInfo[0].nViewChainTotal;j++)
                            {
                                for(int k = 0; k < struCnfg.struLayerInfo[0].stuViewInfo[j].nIdentifyGroupTotal; k++)
                                {
                                    for(int m = 0;m < struCnfg.struLayerInfo[0].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++)
                                    {
                                        statisticName = QString("%1%2_%3").arg(mysql.viewName.at(j)).arg(m).arg(m_dataStrList.at(n));

                                        sql = "select " + statisticName + " from LAYER1_STATISTIC_DATA where CreatedTime >= datetime('now','start of day','" + QString::number(i)
                                                + " day') and CreatedTime < datetime('now','start of day','" + QString::number(i+1) +" day')";

                                        query.exec(sql);

                                        while(query.next()&isQuerying)
                                        {
                                            if (m_nViewType == ViewType(j))
                                                tempvalue += query.value(0).toInt();

                                            m_realTimeTotalDataInfo[j] += query.value(0).toInt();
                                        }
                                        if (!isQuerying)
                                        {
                                            if(isNeedInserting)
                                            {
                                                mysql.updateTable();
                                                mysql.clearStagedThroughout();
                                                isNeedInserting = false;
                                            }

                                            mysql.closeDatabase();
                                            return;
                                        }
                                    }
                                }
                            }
                        }
                        m_dataValueList.append(tempvalue);
                    }
                    break;
                case LastWeek:
                    memset(m_realTimeTotalDataInfo,0,sizeof(m_realTimeTotalDataInfo));
                    for(int n = 0;n < m_dataStrList.count();n++)
                    {
                        tempvalue = 0;
                        for(int i = -7 ;i < 0; i++ )
                        {
                            for(int j = 0; j <struCnfg.struLayerInfo[0].nViewChainTotal;j++)
                            {
                                for(int k = 0; k < struCnfg.struLayerInfo[0].stuViewInfo[j].nIdentifyGroupTotal; k++)
                                {
                                    for(int m = 0;m < struCnfg.struLayerInfo[0].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++)
                                    {
                                        statisticName = QString("%1%2_%3").arg(mysql.viewName.at(j)).arg(m).arg(m_dataStrList.at(n));

                                        sql = "select " + statisticName + " from LAYER1_STATISTIC_DATA where CreatedTime >= datetime('now','start of day','" + QString::number(i)
                                                + " day') and CreatedTime < datetime('now','start of day','" + QString::number(i+1) +" day')";
                                        query.exec(sql);

                                        while(query.next()&isQuerying)
                                        {
                                            if (m_nViewType == ViewType(j))
                                                tempvalue += query.value(0).toInt();

                                            m_realTimeTotalDataInfo[j] += query.value(0).toInt();
                                        }
                                        if (!isQuerying)
                                        {
                                            if(isNeedInserting)
                                            {
                                                mysql.updateTable();
                                                mysql.clearStagedThroughout();
                                                isNeedInserting = false;
                                            }

                                            mysql.closeDatabase();
                                            return;
                                        }
                                    }

                                }
                            }
                        }

                        m_dataValueList.append(tempvalue);
                    }
                    break;

                case Last3Month:
                    memset(m_realTimeTotalDataInfo,0,sizeof(m_realTimeTotalDataInfo));
                    for(int n = 0;n < m_dataStrList.count();n++)
                    {
                        tempvalue = 0;
                        for(int i = -3 ;i < 0; i++ )
                        {
                            for(int j = 0; j <struCnfg.struLayerInfo[0].nViewChainTotal;j++)
                            {
                                for(int k = 0; k < struCnfg.struLayerInfo[0].stuViewInfo[j].nIdentifyGroupTotal; k++)
                                {
                                    for(int m = 0;m < struCnfg.struLayerInfo[0].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++)
                                    {
                                        statisticName = QString("%1%2_%3").arg(mysql.viewName.at(j)).arg(m).arg(m_dataStrList.at(n));

                                        sql = "select " + statisticName + " from LAYER1_STATISTIC_DATA where CreatedTime >= date('now','" + QString::number(i)
                                                + " month') and CreatedTime < date('now','" + QString::number(i+1) + " month')";

                                        query.exec(sql);

                                        while(query.next()&isQuerying)
                                        {
                                            if (m_nViewType == ViewType(j))
                                                tempvalue += query.value(0).toInt();

                                            m_realTimeTotalDataInfo[j] += query.value(0).toInt();
                                        }
                                        if (!isQuerying)
                                        {
                                            if(isNeedInserting)
                                            {
                                                mysql.updateTable();
                                                mysql.clearStagedThroughout();
                                                isNeedInserting = false;
                                            }

                                            mysql.closeDatabase();
                                            return;
                                        }
                                    }
                                }
                            }
                        }

                        m_dataValueList.append(tempvalue);
                    }

                    break;
                case RealTime:

                    //实时含杂查询时，只需对查询链的全部通道数相应使能算法分别累加，不用管分次分组概念
                    memset(m_realTimeTotalDataInfo,0,sizeof(m_realTimeTotalDataInfo));
                    for(int n = 0;n < m_dataStrList.count();n++)
                    {
                        tempvalue = 0;
                        for(int j = 0; j <struCnfg.struLayerInfo[0].nViewChainTotal;j++)
                        {
                            if (m_nViewType != ViewType(j) ) continue;
                            for(int m = 0;m < struCnfg.struLayerInfo[0].nViewUnitTotal[j] ; m++)
                            {
                                if (m_dataStrList.at(n) == "GreyA")
                                    tempvalue += struGsh.struStatics[0][j][m].nGreyBadData[0];
                                else if(m_dataStrList.at(n) == "GreyB")
                                    tempvalue += struGsh.struStatics[0][j][m].nGreyBadData[1];
                                else if(m_dataStrList.at(n) == "GreyC")
                                    tempvalue += struGsh.struStatics[0][j][m].nGreyBadData[2];
                                else if(m_dataStrList.at(n) == "DiscolorA")
                                    tempvalue += struGsh.struStatics[0][j][m].nDiscoloryBadData[0];
                                else if(m_dataStrList.at(n) == "DiscolorB")
                                    tempvalue += struGsh.struStatics[0][j][m].nDiscoloryBadData[1];
                                else if(m_dataStrList.at(n) == "SvmA")
                                    tempvalue += struGsh.struStatics[0][j][m].nSvmData[0];
                                else if(m_dataStrList.at(n) == "SvmB")
                                    tempvalue += struGsh.struStatics[0][j][m].nSvmData[1];
                                else if(m_dataStrList.at(n) == "SvmC")
                                    tempvalue += struGsh.struStatics[0][j][m].nSvmData[2];
                                else if(m_dataStrList.at(n) == "Shape")
                                    tempvalue += struGsh.struStatics[0][j][m].nShapeData;
                            }
                        }

                        m_dataValueList.append(tempvalue);
                    }

                    for(int j = 0; j <struCnfg.struLayerInfo[0].nViewChainTotal;j++)
                    {
                        if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
                        for(int m = 0;m < struCnfg.struLayerInfo[0].nViewUnitTotal[j] ; m++)
                        {
                            m_realTimeTotalDataInfo[j] += struGsh.struStatics[0][j][m].nTotalArithEjectTimes;
                        }
                    }
                    break;
                }

            default:
                break;
            }

            //  qDebug()<<"m_nStatisticType index is "<<index<<"m_dataCount is" <<m_dataCount<<"m_dataValueListCount is"<<m_dataValueList.count();
        }

        if(isNeedInserting)
        {
            mysql.updateTable();
            mysql.clearStagedThroughout();
            isNeedInserting = false;
        }

        if(m_nQueryType != RealTime)
        {
            mysql.closeDatabase();
        }
        isQuerying = false;
        isNeedRepainting = true;
        emit statisticQueryFinsishedSig(index);
    }
}




/******************************************** 深度学习含杂查询线程 ***********************************************/

//  获取表格中的所有的批次信息名称list


// 根据批次 时间等信息查询结果信息
void getImpurityThread::updatesearchdata(int view, int unit)
{
    qDebug()<<"piciname"<<currentpiciname<<"seektime"<<seektime<<"view"<<view<<"unit"<<unit;

    m_totalValueVec.clear();
    m_totalAreaVec.clear();
    for(int i = 0; i < MAX_IPC_MODEL_CLASS; i++)
    {
        m_totalValueVec.append(0.0);
        m_totalAreaVec.append(0.0);
    }

    m_alltimeValueVec.clear();

    for(int i = 0; i < 24; i++)  // 统计一天24小时的数据库数据
    {
        if(i< 10)
        {
            seektimeinTable = seektime + "0"+ QString::number(i);
        }
        else
        {
            seektimeinTable = seektime + ""+ QString::number(i);
        }
        //1、根据当前时间 i 统计数据库所有的含杂情况
        if(struCnfg.bGetMaterialType == 0)
            getdatafromtable( g_Runtime().modelFullNametest+ QString("_%1_%2").arg(view).arg(unit), view, unit);
        else if(struCnfg.bGetMaterialType == 1)
            getdatafromtable( g_Runtime().modelFullName_Area+ QString("_%1_%2").arg(view).arg(unit), view, unit);
        else if(struCnfg.bGetMaterialType == 2)
            getdatafromtable( g_Runtime().modelFullName_Frame+ QString("_%1_%2").arg(view).arg(unit), view, unit);

        getAreadatafromtable( g_Runtime().modelFullName_Area+ QString("_%1_%2").arg(view).arg(unit), view, unit);

        adddatatovector(view);
        seektimeinTable.clear();
    }

    /*更新统计类型数据信息*/
    for(int i = 0; i < struCnfp.ipcModelClassCount[view][0]; i++)
    {
        if(struCnfp.nIpcStatisticsBigclass != 0)
        {
            if(struCnfp.nIpcStatisticsclassId[i] > 0)
            {
                int index = struCnfp.nIpcStatisticsclassId[i]-1;
                m_statictotalValueVec[ index ] +=  m_totalValueVec.at(i);
                m_statictotalAreaVec[ index ] +=  m_totalAreaVec.at(i);
              //  qDebug()<<"m_totalAreaVec"<<m_totalAreaVec.at(i);
            }
        }
        else
        {
            m_statictotalValueVec[ i ] +=  m_totalValueVec.at(i);
            m_statictotalAreaVec[ i ] +=  m_totalAreaVec.at(i);
           // qDebug()<<"m_totalAreaVec"<<m_totalAreaVec.at(i);
        }
    }

    /*更新计算重量*/
    for(int i = 0; i < struCnfp.nIpcStatisticsBigclass; i++)
    {
        struGsh.totalArea[i] =  m_statictotalAreaVec[i]*struCnfp.nIpcStatisticsbigclassCoeff[i]/1000;
        qDebug()<<i<<struGsh.totalArea[i] ;
    }

}
/*获取所有数据后 更新界面显示*/
void getImpurityThread::updatestaticdata(int view)
{
    getsmallclassnamelist(view);
    getbigclassstaticlist();

    m_statictotalValueVec.clear();
    m_statictotalAreaVec.clear();
    for(int i = 0; i < MAX_IPC_MODEL_CLASS; i++)
    {
        m_statictotalValueVec.append(0.0);
        m_statictotalAreaVec.append(0.0);
    }

    for(int j = 0; j < struCnfg.struLayerInfo[0].nViewChainTotal; j++)
    {
        if(struCnfg.struLayerInfo[0].nViewIsUsed[j] == 1 && j == view) //当前视
        {
            for(int k = 0; k <struCnfg.struLayerInfo[0].nViewUnitTotal[j]; k++)
            {
                if(struCnfg.struIpcCameraParams[0][j][k].ipcType != IPC_CAMERA_TYPE_NULL)
                {
                    updatesearchdata(j, k);   // 从数据库获取数据
                }
            }

        }
    }

}



bool getImpurityThread::getdatafromtable(QString tableName, int view, int unit)
{

    MySqlite mysql;
    if(!mysql.openDatabase())
    {
        qDebug()<<"STATISTICDB open fail!";
        return false;
    }
    if(!mysql.isTableExist(tableName))
    {
        mysql.closeDatabase();
        qDebug()<<"table is not exit!";
        return false;
    }

    m_onetimeValueVec.clear();
    for(int i = 0; i < struCnfp.ipcModelClassCount[view][0]; i++)
    {
        m_onetimeValueVec.append(0.0);
    }

    QSqlQuery query(mysql.db);
    QString sql = "select ";

    if(currentpiciname == "ALL") // 按照时间范围搜索所有批次项目
    {
        sql.append("* from ");
        sql.append(tableName);
        sql.append(" where CreatedTime = '");
        sql.append(seektimeinTable);
        sql.append("'");
        //qDebug()<<sql;
        if(!query.exec(sql))
            return false;
    }
    else
    {
        sql.append(" * from ");
        sql.append(tableName);
        sql.append(" where CreatedTime = '");
        sql.append(seektimeinTable);
        sql.append("'");
        sql.append(" And  piciname = '");
        sql.append(currentpiciname);
        sql.append("'");
       //  qDebug()<<sql;
        if(!query.exec(sql))
            return false;
    }
    while( query.next() )
    {

        int j = 0;
        for(int i = 0; i < struCnfp.ipcModelClassCount[view][0]; i++)
        {
            m_onetimeValueVec[j++] += query.value(2+i).toDouble();
        }
    }
    mysql.closeDatabase();
    return true;
}


bool getImpurityThread::getAreadatafromtable(QString tableName, int view, int unit)
{

    MySqlite mysql;
    if(!mysql.openDatabase())
    {
        qDebug()<<"STATISTICDB open fail!";
        return false;
    }
    if(!mysql.isTableExist(tableName))
    {
        mysql.closeDatabase();
        qDebug()<<"table is not exit!";
        return  false;
    }

    m_onetimeAreaVec.clear();
    for(int i = 0; i < struCnfp.ipcModelClassCount[view][0]; i++)
    {
        m_onetimeAreaVec.append(0.0);
    }

    QSqlQuery query(mysql.db);
    QString sql = "select ";

    if(currentpiciname == "ALL") // 按照时间范围搜索所有批次项目
    {
        sql.append("* from ");
        sql.append(tableName);
        sql.append(" where CreatedTime = '");
        sql.append(seektimeinTable);
        sql.append("'");
        //qDebug()<<sql;
        if(!query.exec(sql))
            return false;
    }
    else
    {
        sql.append(" * from ");
        sql.append(tableName);
        sql.append(" where CreatedTime = '");
        sql.append(seektimeinTable);
        sql.append("'");
        sql.append(" And  piciname = '");
        sql.append(currentpiciname);
        sql.append("'");
        // qDebug()<<sql;
        if(!query.exec(sql))
            return false;
    }
    while( query.next() )
    {

        int j = 0;
        for(int i = 0; i < struCnfp.ipcModelClassCount[view][0]; i++)
        {
            m_onetimeAreaVec[j++] += query.value(2+i).toDouble();
        }
    }
    mysql.closeDatabase();
    return true;
}


void getImpurityThread::adddatatovector(int view)
{
    //2、 按照时间统计
    if( m_onetimeValueVec.size()!= 0 )
    {
        double tmp = 0.0;
        for(int j = 0; j < struCnfp.ipcModelClassCount[view][0]; j++ )
        {
            tmp += m_onetimeValueVec[j];
        }
        m_alltimeValueVec.append(tmp);
    }
    else
        m_alltimeValueVec.append(0.0);

    //3、按照小类型统计数据////////////////////////
    if( m_onetimeValueVec.size()!= 0 ) // 更新数据库)
    {
        for( int j = 0; j < m_onetimeValueVec.size(); j++ )
        {
            m_totalValueVec.replace( j, m_totalValueVec[j] + m_onetimeValueVec[j]);
            m_totalAreaVec.replace( j, m_totalAreaVec[j] + m_onetimeAreaVec[j]);
          //  qDebug()<<"m_onetimeAreaVec"<<m_onetimeAreaVec[j];
        }
    }
}

void getImpurityThread::getbigclassname()
{
    bigclassNameStrList.clear();
    for(int i = 0; i < struCnfp.nIpcStatisticsBigclass; i++)
    {
        QByteArray sMaterialArr(struCnfp.sCurBigstatisname[i]);
        QString tmp = QString::fromLocal8Bit(sMaterialArr);
        bigclassNameStrList.append(tmp); // 更新统计类型名称
    }

}
void getImpurityThread::getsmallclassnamelist(int view)
{

    classNameStrList.clear();
    for(int i = 0; i < 500;i++)
    {
        if(i >= struCnfp.ipcModelClassCount[view][0])
            break;
        classNameStrList.append(QString("%1").arg( struCnfp.struCameraIpcClassGroupParams[view][0].className[i] )); //
    }
}

void getImpurityThread::getbigclassstaticlist()
{
    getbigclassname();
    StaticClassNameList.clear();

    if(struCnfp.nIpcStatisticsBigclass != 0)
    {
        for(int i = 0; i < struCnfp.nIpcStatisticsBigclass ; i++)
        {
            StaticClassNameList.append(bigclassNameStrList.at(i));
        }
    }
    else
    {
        StaticClassNameList = classNameStrList;
    }

}




void getImpurityThread::getpicinfolistformtable(QString table)
{
    return;
    MySqlite mysql;
    if( !mysql.openDatabase() )
    {
        qDebug()<<"STATISTICDB open fail!";
        return;
    }
    if( !mysql.isTableExist(table) )
    {
        mysql.closeDatabase();
        qDebug()<<"table is not exit!";
        return;
    }

    QSqlQuery query( mysql.db);
    QString sql = "select ";
    sql.append("piciname from ");
    sql.append(table);
    sql.append(" where CreatedTime like '");
    sql.append(seektime + "%");
    sql.append("'");

    if( !query.exec(sql) )
    {
        qDebug()<<"select table failed";

    }
    else
    {
        // qDebug()<<"select table success";
    }


//    while( query.next() )
//    {
//        if(!m_pParentWidget->picinameList.contains(query.value(0).toString()) && query.value(0).toString()!= "")
//            m_pParentWidget->picinameList.append(query.value(0).toString());
//    }
//    m_pParentWidget->updatepicinfolist();

}


void getImpurityThread::run()
{

    MySqlite mysql;
    if(!mysql.openDatabase())
    {
        qDebug()<<"getImpurityThread STATISTICDB open fail!";
        return ;
    }
    while(1)
    {
        msleep(10);

//        g_Runtime().modelFullNametest = "TX_G3_HNYanYe_DG150_224_20251117_C8_2025112009_Count";
//        g_Runtime().modelFullName_Area = "TX_G3_HNYanYe_DG150_224_20251117_C8_2025112009_Area";
//        g_Runtime().modelFullName_Frame = "TX_G3_HNYanYe_DG150_224_20251117_C8_2025112009_Frame";
//        seektime = "20260317";

        if(struGsh.updateIpcStatisdata == 1)
        {
            getpicinfolistformtable(g_Runtime().modelFullNametest+QString("_%1_%2").arg(currentview).arg(0));  //
            updatestaticdata(currentview);
          //  m_pParentWidget->updateshowdata();
            emit sGetClassPointsFinished();
            struGsh.updateIpcStatisdata = 0;
            struGsh.IpcStatisdataisrun = 0;

        }
        else if(struGsh.deleteIpcStatisdata == 1)
        {


            QStringList tablelist;
            tablelist.clear();
            QString seektimeinTable;
            for(int time = 0; time < 1; time++)
            {
                if(time < 10)
                {
                    seektimeinTable = seektime + "0"+ QString::number(time);
                }
                else
                {
                    seektimeinTable = seektime + ""+ QString::number(time);
                }

                for(int i = 0; i < struCnfg.struLayerInfo[0].nViewChainTotal; i++)
                {
                    if(struCnfg.struLayerInfo[0].nViewIsUsed[i] == 1)
                    {
                        for(int j = 0; j <struCnfg.struLayerInfo[0].nViewUnitTotal[i]; j++)
                        {
                            if(struCnfg.struIpcCameraParams[0][i][j].ipcType != IPC_CAMERA_TYPE_NULL)
                            {

                                tablelist.append(g_Runtime().modelFullNametest + QString("_%1_%2").arg(i).arg(j) );
                                //  mysql.clearStatisTable(g_Runtime().modelFullNametest + QString("_%1_%2").arg(i).arg(j) ,seektimeinTable);
                                //  mysql.clearStatisTable(g_Runtime().modelFullName_Area + QString("_%1_%2").arg(i).arg(j),seektimeinTable);
                                //  mysql.clearStatisTable(g_Runtime().modelFullName_Frame + QString("_%1_%2").arg(i).arg(j),seektimeinTable);
                            }
                        }
                    }
                }
                if(time == 0)
                    break;

            }

            mysql.clearStatisTable(tablelist,seektimeinTable);
            system("sync");
            struGsh.deleteIpcStatisdata = 0;
        }
        else
        {
            continue;
        }

    }

}

