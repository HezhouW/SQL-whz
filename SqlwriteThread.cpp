#include "SqlwriteThread.h"

 SqlwriteThread::SqlwriteThread(QString hostname, QString databasename, QString usrname, QString psword, int index)
 {
	m_hostname = hostname;
	m_databasename = databasename;
	m_usrname = usrname;
	m_psword = psword;
	m_index = index;
	m_range = RANGE;
 }

void  SqlwriteThread::SetRange(int range)
{
	m_range = range;
}

void SqlwriteThread::run()
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

    QString i2c;
    QFile timelog("timelog"+i2c.setNum(m_index)+".txt");
    if (!timelog.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        qDebug()<<trUtf8("LOG日志没有打开")<<endl;
        return;
    }
    timelog.close();

    QSqlQuery sqlquery(database);
//    QTime timerecord;
//    timerecord.start();
    qDebug()<<"sqltest thread runing...";
    for(int i=0;i <= m_range/SQLLINE ; ++i)		//i要改成大数，同range
    {
        QString sqlstatement = "";
        int statementnum =( (i+1)*SQLLINE > m_range ) ? m_range - i*SQLLINE: SQLLINE;
        sqlstatement="insert into TestInsert("
                "id,"
                "threadnum,"
                "idinthread,"
                "Data) VALUES";
        for(int j=0; j < statementnum; j++)
        {
            QString inttemp;
            sqlstatement += "(null,";                             //id
            sqlstatement += inttemp.setNum(m_index);                //threadnum
            sqlstatement += ","+inttemp.setNum( i*SQLLINE + j + m_index*m_range );		//idinthread
            sqlstatement += ",'dagnoansdongfnalksngnalksndlfkja;ngaldkfalnlkgnaldnfja')"; //framesize
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
//        qDebug()<<i<<"--Block insert over!";
    }

//    timelog.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
//    QTextStream logout(&timelog);
//    logout << "Insert "<<m_range<<" Lines to SQL need time : " << timerecord.elapsed() << "(ms)\n";
//    timelog.close();
    database.close();
    qDebug()<<m_index<<"_thread run over!"<<endl;
}
