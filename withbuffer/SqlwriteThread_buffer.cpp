#include "SqlwriteThread_buffer.h"


extern QVector<mysqldata> &g_buffer;
extern int g_rIndex ;
extern QMutex mutex;
//1vn
extern QSemaphore freeBuffer;
extern QSemaphore usedBuffer;
// 1vn End


SqlwriteThread_buffer::SqlwriteThread_buffer(QString hostname, QString databasename, QString usrname, QString psword, int index)
{
    m_hostname = hostname;
    m_databasename = databasename;
    m_usrname = usrname;
    m_psword = psword;
    m_index = index;
    m_range = RANGE;
}

void  SqlwriteThread_buffer::SetRange(int range)
{
    m_range = range;
}

void SqlwriteThread_buffer::run()
{
    QString databaseseq;

    QSqlDatabase database = QSqlDatabase::addDatabase("QMYSQL",databaseseq.setNum(m_index));
    database.setHostName(m_hostname);
    database.setDatabaseName(m_databasename);
    database.setUserName(m_usrname);
    database.setPassword(m_psword);

    if (!database.open())
    {
        database.close();
        qDebug()<<trUtf8("数据库打不开")<<endl;
        return ;
    }
    else
    {
        qDebug()<<trUtf8("线程内数据已经打开")<<endl;
    }


    /*
    QString i2c;
    QFile timelog("timelog"+i2c.setNum(m_index)+".txt");
    if (!timelog.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        qDebug()<<trUtf8("LOG日志没有打开")<<endl;
        return;
    }
    timelog.close();
    */

    QSqlQuery sqlquery(database);
    //    QTime timerecord;
    //    timerecord.start();
    qDebug()<<"sqltest thread runing...";

    mysqldata *data=g_buffer.data();
    mysqldata tempdata;
    bool flag = true;
    while(g_rIndex < DATABLOCKSIZE)
    {
        mutex.lock();
        if(usedBuffer.tryAcquire())
        {            
            flag = false;
            int index=g_rIndex;
            ++g_rIndex;
            qDebug()<<QThread::currentThreadId()<<trUtf8("线程读写第")
                    <<index<<trUtf8("块数据，now数据缓存区内存有 ")<<usedBuffer.available()+1<<" blocks!";

            mutex.unlock();     //先锁住rindex，如果能获取得到资源，就取走该资源。rndex指向该资源。
            tempdata = data[index % 10];

            //qDebug()<<"-";//
            freeBuffer.release();
        }
        else
        {
            mutex.unlock();
            continue;
        }

        int range = tempdata.datavector.size();
        for(int i=0;i <= range/SQLLINE ; ++i)		//i要改成大数，同range
        {
            QString sqlstatement = "";
            int statementnum =( (i+1)*SQLLINE > range ) ? range - i*SQLLINE: SQLLINE;
            sqlstatement="insert into InsertTest("
                    "id,"
                    "taskseq,"
                    "protocoltype ,"
                    "threadnumintask,"
                    "idinthread,"
                    "framesize,"
                    "DestMac,"
                    "SourMac,"
                    "TypeorLength,"
                    "Data,"
                    "Upperlayer) VALUES";


            for(int j=0; j < statementnum; j++)
            {
                sqlstatement += "("+tempdata.idvector.at(i*SQLLINE + j);
                sqlstatement += "," + tempdata.taskseqvector.at(i*SQLLINE + j);
                sqlstatement += ", '" + tempdata.typevector.at(i*SQLLINE + j)+"'";
                sqlstatement += "," + tempdata.threadnumvector.at(i*SQLLINE + j);
                sqlstatement += "," + tempdata.idinthreadvector.at(i*SQLLINE + j);
                sqlstatement += "," + tempdata.framesizevector.at(i*SQLLINE + j);
                sqlstatement += ", '" + tempdata.DestMacvector.at(i*SQLLINE + j)+"'";
                sqlstatement += ", '" + tempdata.SourMacvector.at(i*SQLLINE + j)+"'";
                sqlstatement += ", '" + tempdata.TypeorLengthvector.at(i*SQLLINE + j)+"'";
                sqlstatement += ", '" + tempdata.datavector.at(i*SQLLINE + j)+"'";
                sqlstatement += ", '" + tempdata.uplayervector.at(i*SQLLINE + j)+"')";
                if( j == statementnum -1) sqlstatement += ";";
                else sqlstatement += ",";
            }
            if( statementnum != 0 )
            {
                if(!sqlquery.exec(sqlstatement))
                {
                    qDebug()<<"block--"<<i<<"\t\n"<<sqlquery.lastError().databaseText();
                    qDebug()<<trUtf8("发生语句错误")<<sqlstatement;
                    return;
                }

            }
            else
            {
                break;
            }
        }


    }




    //    timelog.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
    //    QTextStream logout(&timelog);
    //    logout << "Insert "<<m_range<<" Lines to SQL need time : " << timerecord.elapsed() << "(ms)\n";
    //    timelog.close();


    database.close();
    qDebug()<<m_index<<"_thread run over!"<<endl;
}

