#ifndef PRODUCER_H
#define PRODUCER_H

#include <QString>
#include <QDebug>
#include <QVector>
#include <QSemaphore>
#include <QWaitCondition>
#include <QMutex>
#include <QQueue>
#include <QFile>
#include <QtSql/QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

#include <QThread>

#define DATABLOCKSIZE 1024*10

class mysqldata
{
public:
	QVector <QString > idvector;
	QVector <QString>	taskseqvector;
	QVector <QString> typevector;
	QVector <QString>  threadnumvector;
	QVector <QString>  idinthreadvector;
	QVector <QString>  framesizevector;
	QVector <QString> DestMacvector;
	QVector <QString> SourMacvector;
	QVector <QString> TypeorLengthvector;	
	QVector< QString > datavector;	
	QVector <QString > uplayervector;
};

class Producer:public QThread
{
public:
    void run();
};

#endif
