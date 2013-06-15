#include <QtCore/QCoreApplication>
#include <QtSql/QtSql>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QTime>
#include <stdlib.h>
#include <sys/time.h>;
#include <QDebug>
#include <QString>
#include <QThread>
#include <iostream>
#include "SqlwriteThread.h"

#define THREADNUM 10

using namespace std;

void initaldatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL","first");
    db.setHostName("localhost");
    db.setDatabaseName("test");
    db.setUserName("root");
    db.setPassword("7260336");
    if (!db.open())
    {
        cout<<"OPEN error~!"<<endl;
        exit(1);
    }
    else
    {
        qDebug()<<"Initial OK!";
        QSqlQuery query(db);
        if( !query.exec("create table if not exists TestInsert("
                       "symbol VARCHAR(40) default '<img src=\"images/details_open.png\">',"
                       "id BIGINT AUTO_INCREMENT PRIMARY KEY ,"
                       "threadnum int NOT NULL,"
                       "idinthread int NOT NULL,"
                       "Data VARCHAR(3000) DEFAULT NULL)") )
            qDebug()<<"exec error:"<< query.lastError().databaseText();
    }
    db.close();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    initaldatabase();//初始化数据库
    QSqlDatabase::removeDatabase("first");
    /*单线程测试代码
    SqlwriteThread sqltest("localhost","test","root","7260336",1);
    sqltest.SetRange(1024*10*10);
    QTime t;
    t.start();

    sqltest.start();
    qDebug()<<"sqltest thread start!";
    sqltest.wait();
    */
    SqlwriteThread *threadvector[10];
    for(int i=0; i< THREADNUM; ++i)
    {
        threadvector[i] = new SqlwriteThread("localhost","test","root","7260336",i);
        threadvector[i]->SetRange(1024*10);
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

    qDebug()<<QObject::trUtf8("插入 %1 条记录，耗时：%2 ms").arg(1024*100).arg(t.elapsed());
    return a.exec();
}
