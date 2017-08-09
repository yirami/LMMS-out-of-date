#include "cdatabasepackage.h"

QString CDatabasePackage::prepareFormat("INSERT INTO Medicine VALUES(?,?,?,?,?,?,?)");

CDatabasePackage::CDatabasePackage(QString databasePath, QString connectName, QString sqlDriver)
{
    this->databasePath = databasePath;
    this->connectName = connectName;
    this->sqlDriver = sqlDriver;
    QFile databaseFile(databasePath);
    if (databaseFile.exists())
    {
        this->database = QSqlDatabase::addDatabase(sqlDriver,connectName);
        this->database.setDatabaseName(databasePath);
        this->database.open();
        this->query = QSqlQuery(this->database);
    }
    else
    {
        QMessageBox::about(NULL,"打开数据库","数据库文件不存在！已重新创建！");
        if(!creatEmptyTable())
            qDebug()<<QString("创建空数据表失败！");
        this->query = QSqlQuery(this->database);
    }
}

bool CDatabasePackage::creatEmptyTable()
{
    this->database = QSqlDatabase::addDatabase(sqlDriver,connectName);
    this->database.setDatabaseName(databasePath);
    this->database.open();
    query = QSqlQuery(this->database);
    bool ok = query.exec("create table Medicine(id INT PRIMARY KEY NOT NULL,药品代码 TEXT NOT NULL,药品名 TEXT NOT NULL,厂商 TEXT NOT NULL,规格 TEXT,库存 INT,单价 REAL)");
    return ok;
}

bool CDatabasePackage::doOneQuery()
{
    query.prepare(oneCommand);
    bool ok = query.exec();
    return ok;
}

QVector<QVariantList> CDatabasePackage::data2QVariantList(QStandardItemModel *dataModel)
{
    QVariantList Id,AgentName,Name,MadeIn,Spec,Stock,Price;
    int startId = getNewID();
    for (int idx=0;idx<dataModel->rowCount();idx++)
    {
        Id.append(startId+idx);
        AgentName.append(dataModel->item(idx,0)->text().toUpper());
        Name.append(dataModel->item(idx,1)->text());
        MadeIn.append(dataModel->item(idx,2)->text());
        Spec.append(dataModel->item(idx,3)->text());
        Stock.append(dataModel->item(idx,4)->text().toInt());
        Price.append(dataModel->item(idx,6)->text().toFloat());
    }
    QVector<QVariantList> stdData;
    stdData.append(Id);
    stdData.append(AgentName);
    stdData.append(Name);
    stdData.append(MadeIn);
    stdData.append(Spec);
    stdData.append(Stock);
    stdData.append(Price);
    return stdData;
}

void CDatabasePackage::doQueryInBatch(QVector<QVariantList> stdData)
{
    oneCommand = CDatabasePackage::prepareFormat;
    query.prepare(oneCommand);
    Q_ASSERT(stdData.count()==7);
    query.bindValue(0,stdData.at(0));
    query.bindValue(1,stdData.at(1));
    query.bindValue(2,stdData.at(2));
    query.bindValue(3,stdData.at(3));
    query.bindValue(4,stdData.at(4));
    query.bindValue(5,stdData.at(5));
    query.bindValue(6,stdData.at(6));
    Q_ASSERT(query.execBatch());
}

int CDatabasePackage::getResultNum()
{
    int resultNum;
    Q_ASSERT(query.isActive() && query.isSelect());
    if (query.next())
    {
        Q_ASSERT(query.last());
        resultNum = query.at()+1;
    }
    else
        resultNum = 0;
    return resultNum;
}

int CDatabasePackage::getNewID()
{
    int NewID;
    oneCommand = QString("SELECT * FROM Medicine");
    Q_ASSERT(doOneQuery());
    int resultNum = getResultNum();
    if (resultNum==0)
        NewID = 0;
    else
    {
        oneCommand = QString("SELECT MAX(id) AS maxID FROM Medicine");
        Q_ASSERT(doOneQuery());
        Q_ASSERT(getResultNum()==1);
        queryFirst();
        NewID = getOneField(QString("maxID")).toInt() + 1;
    }
    return NewID;
}

QVector<int> CDatabasePackage::getAllKey()
{
    QVector<int> id;
    oneCommand = QString("SELECT %1 FROM Medicine").arg(QString("id"));
    Q_ASSERT(doOneQuery());
    if (getResultNum()!=0)
    {
        queryFirst();
        do
        {
            QVariant oneId = query.record().value("id");
            Q_ASSERT(oneId.isValid());
            id.append(oneId.toInt());
        }while(query.next());
    }
    return id;
}

QVector<QString> CDatabasePackage::getAllAgentName()
{
    QVector<QString> agentNames;
    oneCommand = QString("SELECT %1 FROM Medicine").arg(QString("药品代码"));
    Q_ASSERT(doOneQuery());
    if (getResultNum()!=0)
    {
        queryFirst();
        do
        {
            QVariant oneId = query.record().value("药品代码");
            Q_ASSERT(oneId.isValid());
            agentNames.append(oneId.toString());
        }while(query.next());
    }
    return agentNames;
}

QVector<int> CDatabasePackage::getKeyByAgentName(QString agentName)
{
    QVector<int> queryIndex;
    oneCommand = QString("SELECT id FROM Medicine WHERE 药品代码 = '%1'").arg(agentName);
    Q_ASSERT(doOneQuery());
    if (getResultNum()!=0)
    {
        queryFirst();
        do
        {
            QVariant oneId = query.record().value("id");
            Q_ASSERT(oneId.isValid());
            queryIndex.append(oneId.toInt());
        }while(query.next());
    }
    return queryIndex;
}

QVector<QVariant> CDatabasePackage::getItemsByKey(QVector<int> keys, QString fieldName)
{
    QVector<QVariant> Items;
    for (int &key:keys)
    {
        oneCommand = QString("SELECT %1 FROM Medicine WHERE id = %2").arg(fieldName).arg(key);
        Q_ASSERT(doOneQuery());
        Q_ASSERT(getResultNum()==1);
        queryFirst();
        QVariant oneItem = query.record().value(fieldName);
        Q_ASSERT(oneItem.isValid());
        Items.append(oneItem);
    }
    return Items;
}

QVector<int> CDatabasePackage::filtKeyByField(QVector<int> keys, QString fieldName, QString fieldValue)
{
    QVector<int> filtedKeys;
    for (int &key:keys)
    {
        QVector<int> oneKey{key};
        if (getItemsByKey(oneKey,fieldName).first().toString()==fieldValue)
            filtedKeys.append(key);
    }
    return filtedKeys;
}

void CDatabasePackage::queryFirst()
{
    Q_ASSERT(query.first());
}

QVariant CDatabasePackage::getOneField(QString fieldName)
{
    QVariant oneField = query.value(fieldName);
    return oneField;
}
