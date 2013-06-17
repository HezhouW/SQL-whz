#include	"producer.h"

const QVector <mysqldata>::size_type g_BufferSize =10;  //缓存区大小
QVector<mysqldata> g_filestrings(g_BufferSize);
QVector<mysqldata> &g_buffer=g_filestrings;

QMutex mutex;
QMutex outhold;
int g_numUsedBytes = 0;
int g_rIndex = 0;


//1vn
QSemaphore freeBuffer(g_BufferSize);
QSemaphore usedBuffer(0);
// 1vn End

	
void getdata(mysqldata &datablock, int blocknum)
{	
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL","input");
    db.setHostName("localhost");
    db.setDatabaseName("whztest");
    db.setUserName("root");
    db.setPassword("infonet313");
    if (!db.open())
    {
        qDebug()<<"inputlink OPEN error~!"<<endl;
        return;
    }
    else
    {
        qDebug()<<"freebuffer.available =="<<freeBuffer.available()+1
               << ", then Producer Get Data from ["<<10*blocknum<<"~~"<<10*(blocknum+1)<<")";
        QSqlQuery sqlquery(db);
		QString strtmp;
		QString sqlstatement="select * from TableEthernet where ";
            sqlstatement+="id>=10*"+strtmp.setNum(blocknum);
            sqlstatement+=" and id<10*"+strtmp.setNum(blocknum+1);
            sqlstatement+=" order by id;";
            if(!sqlquery.exec(sqlstatement))
            {
                qDebug()<<sqlquery.lastError().databaseText();
				qDebug()<<"ERROR sql:\t"<<sqlstatement;
            }
			else
			{
				while(sqlquery.next())
				{
					//其他字段
                    datablock.idvector.push_back(sqlquery.value(1).toString());
                    datablock.taskseqvector.push_back(sqlquery.value(2).toString());
                    datablock.typevector.push_back(sqlquery.value(3).toString());
                    datablock.threadnumvector.push_back(sqlquery.value(4).toString());
                    datablock.idinthreadvector.push_back(sqlquery.value(5).toString());
                    datablock.framesizevector.push_back(sqlquery.value(6).toString());
                    datablock.DestMacvector.push_back(sqlquery.value(7).toString());
                    datablock.SourMacvector.push_back(sqlquery.value(8).toString());
                    datablock.TypeorLengthvector.push_back(sqlquery.value(9).toString());
                    datablock.datavector.push_back(sqlquery.value(10).toString());
                    datablock.uplayervector.push_back(sqlquery.value(11).toString());
				}
			}
    }
    db.close();

}





void Producer::run()
{
	for(int i = 0; i  < DATABLOCKSIZE; ++i)
	{
		freeBuffer.acquire();
		mysqldata temp;
		getdata(temp, i);
		QSqlDatabase::removeDatabase("input");
		g_buffer[i % 10] = temp;
		usedBuffer.release();
	}
}
