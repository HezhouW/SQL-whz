#include <QtCore/QCoreApplication>
#include <QtSql/QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QTime>
#include <stdlib.h>
//#include <sys/time.h>;
#include <QDebug>
#include <QString>
#include <QThread>
#include <iostream>
#include "SqlwriteThread_buffer.h"
#include "producer.h"

#define THREADNUM 10

using namespace std;
extern QVector<mysqldata> &g_buffer;
extern int g_rIndex ;
extern QMutex mutex;
//1vn
extern QSemaphore freeBuffer;
extern QSemaphore usedBuffer;

void initaldatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL","first");
    db.setHostName("localhost");
    db.setDatabaseName("whztest");
    db.setUserName("root");
    db.setPassword("infonet313");
    if (!db.open())
    {
        cout<<"firstlink OPEN error~!"<<endl;
        exit(1);
    }
    else
    {
        qDebug()<<"Initial OK!";
        QSqlQuery query(db);
        query.exec("drop table if exists InsertTest;");
         if(!query.exec("create table if not exists InsertTest("                   
                   "id BIGINT AUTO_INCREMENT PRIMARY KEY ,"
                   "taskseq int NOT NULL,"
                   "protocoltype CHAR(10) DEFAULT 'Ethernet',"
                   "threadnumintask int NOT NULL,"
                   "idinthread int NOT NULL,"
                   "framesize int NOT NULL,"
                   "DestMac VARCHAR(60) DEFAULT NULL,"
                   "SourMac VARCHAR(60) DEFAULT NULL,"
                   "TypeorLength VARCHAR(20) DEFAULT NULL,"
                   "Data VARCHAR(6000) DEFAULT NULL,"
                   "Upperlayer VARCHAR(30) DEFAULT NULL)"))
        qDebug()<<"exec error:"<< query.lastError().databaseText();
    }
    db.close();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    initaldatabase();//初始化数据库
    QSqlDatabase::removeDatabase("first");

    Producer producer;
    producer.start();

    SqlwriteThread_buffer *threadvector[THREADNUM];
    for(int i=0; i< THREADNUM; ++i)
    {
        threadvector[i] = new SqlwriteThread_buffer("localhost","whztest","root","infonet313",i);
//        threadvector[i]->SetRange(1024*10);
    }
    QTime t;
    t.start();
    for(int i=0; i< THREADNUM; ++i)
    {
        threadvector[i]->start();
    }
    QTime blankspace;
    blankspace.start();

    for(int i=0; i< THREADNUM; ++i)
    {
        qDebug()<<QObject::trUtf8("第 %1 个线程和第 %2 个线程之间等待耗时：%3 ms").arg(i).arg(i+1).arg(blankspace.elapsed());
        threadvector[i]->wait();
        blankspace.restart();
    }
    producer.wait();

    qDebug()<<QObject::trUtf8("插入 %1 条记录，耗时：%2 ms").arg(1024*100).arg(t.elapsed());
    exit(0);
    return a.exec();
}
