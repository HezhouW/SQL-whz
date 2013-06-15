#include <QString>
#include <QThread>
#include <QtSql/QtSql>
#include <QtSql/QSqlDatabase>
#include <QTextStream>
#include <QTime>
#include <QMutex>
#include <iostream>

using namespace std;

#define RANGE 1000*10
#define SQLLINE 100
class SqlwriteThread: public QThread
{
    Q_OBJECT
public:
    SqlwriteThread(QString hostname, QString databasename, QString usrname, QString psword, int index);
    ~SqlwriteThread(){}
    void run();
    void SetRange(int range);
	
private:
	QString m_hostname;                   //数据库的HOSTNAME
    QString m_databasename;               //数据库名字
    QString m_usrname;                    //数据库登录用户名
    QString m_psword;
    int m_index;                        //线程块儿序号
    int m_range;                        //循环内for的范围。        更改为大字节数据
};

