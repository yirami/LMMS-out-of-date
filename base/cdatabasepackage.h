#ifndef CDATABASEPACKAGE_H
#define CDATABASEPACKAGE_H

#include <QDebug>

#include <QFile>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlRecord>

#include <QStandardItemModel>
#include <QMessageBox>


class CDatabasePackage
{
public:
    QString databasePath;
    QString connectName;
    QString sqlDriver;
    QSqlDatabase database;
    QString oneCommand;
    static QString prepareFormat;
public:
    CDatabasePackage(QString databasePath, QString connectName, QString sqlDriver=QString("QSQLITE"));
    bool doOneQuery();
    QVector<QVariantList> data2QVariantList(QStandardItemModel *dataModel);
    void doQueryInBatch(QVector<QVariantList> stdData);
    int getResultNum();
    int getNewID();
    QVector<int> getAllKey();
    QVector<QString> getAllAgentName();
    QVector<int> getKeyByAgentName(QString agentName);
    QVector<QVariant> getItemsByKey(QVector<int> keys, QString fieldName);
    QVector<int> filtKeyByField(QVector<int> keys, QString fieldName, QString fieldValue);
    void queryFirst();
    QVariant getOneField(QString fieldName);
private:
    QSqlQuery query;
private:
    bool creatEmptyTable();
};

#endif // CDATABASEPACKAGE_H
