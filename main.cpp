#include <QApplication>
#include <QTextCodec>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTime>
#include <QSqlError>
#include <QtDebug>
#include <QSqlDriver>
#include <QSqlRecord>
#include <QFile>

#include <QStandardItemModel>

#include "panel.h"

QSqlDatabase mainDB;
QString dbPath = "./MedicineManagement.db";
QString mainName = "maindb";
QStandardItemModel *prescribeData;




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    //DataIn
    QFile dbFile(dbPath);
    if (dbFile.exists())
    {
        mainDB = QSqlDatabase::addDatabase("QSQLITE",mainName);
        mainDB.setDatabaseName(dbPath);
        mainDB.open();
    }
    else
    {
        //QMessageBox::critical(NULL,"打开数据库","数据库文件不存在");
        mainDB = QSqlDatabase::addDatabase("QSQLITE",mainName);
        mainDB.setDatabaseName(dbPath);
        mainDB.open();
        QSqlQuery query = QSqlQuery(mainDB);
        query.exec("create table Medicine(id int primary key,药品代码 varchar,药品名 varchar,厂商 varchar,规格 varchar,数量 int,单价 int)");
        query.prepare("insert into Medicine values(?,?,?,?,?,?,?)");

        long records=100;
        for(int i=0;i<records;i++)
        {
            query.bindValue(0,i);
            query.bindValue(1,"CBS");
            query.bindValue(2,"炒白术");
            query.bindValue(3,"江阴天江药业有限公司");
            query.bindValue(4,"10g");
            query.bindValue(5,rand()%100);
            query.bindValue(6,rand()%20);

            query.exec();
            /*if(!success)
            {
                QSqlError lastError=query.lastError();
                qDebug()<<lastError.driverText()<<QString("插入失败");
            }*/
        }

    }


    //
    //GuiShow
    Panel w;
    w.setWindowFlags(w.windowFlags()& ~Qt::WindowMaximizeButtonHint);
    w.show();


    return a.exec();
}
