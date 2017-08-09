#include "cstoragedlg.h"

CStorageDlg::CStorageDlg(CDatabasePackage *dbPackage):CBaseEditDlg(dbPackage)
{
    setWindowTitle(QString("药品入库"));
    dataModel->setHeaderData(0,Qt::Horizontal,QString("药品代码"));
    dataModel->setHeaderData(1,Qt::Horizontal,QString("药品名"));
    dataModel->setHeaderData(2,Qt::Horizontal,QString("厂商"));
    dataModel->setHeaderData(3,Qt::Horizontal,QString("规格"));
    dataModel->setHeaderData(4,Qt::Horizontal,QString("入库量"));
    dataModel->setHeaderData(5,Qt::Horizontal,QString("库存"));
    dataModel->setHeaderData(6,Qt::Horizontal,QString("单价"));
    setDelegateForIN();
    agentLineDelegate->itemList =  mainPackage->getAllAgentName().toList();
    connect(dataModel,SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),this,SLOT(refreshList(QModelIndex)));
}

bool CStorageDlg::checkOneRecord(const int idx)
{
    bool hasError = false;
    QString Name = dataModel->data(dataModel->index(idx,1)).toString();
    QString MadeIn = dataModel->data(dataModel->index(idx,2)).toString();
    mainPackage->oneCommand = QString("SELECT * FROM Medicine WHERE 药品名 = '%1' AND 厂商 = '%2'").arg(Name).arg(MadeIn);
    Q_ASSERT(mainPackage->doOneQuery());
    int resultNum = mainPackage->getResultNum();
    Q_ASSERT(resultNum==0 || resultNum==1);
    if (resultNum==1)
    {
        // 成功匹配，于数据库定位记录
        mainPackage->queryFirst();
        bool ok;
        // ->Stage 1. 验证“药品代码”
        QString agentName = dataModel->data(dataModel->index(idx,0)).toString().toUpper();
        QString agentNameSrc = mainPackage->getOneField(QString("药品代码")).toString().toUpper();
        if (agentName!=agentNameSrc)
        {
            hasError = true;
            dataModel->setData(dataModel->index(idx,0),CBaseEditDlg::errorForeground,Qt::TextColorRole);
        }
        // ->Stage 2. 验证“规格”
        QString spec = dataModel->data(dataModel->index(idx,3)).toString();
        QString specSrc = mainPackage->getOneField(QString("规格")).toString();
        if (spec!=specSrc)
        {
            hasError = true;
            dataModel->setData(dataModel->index(idx,3),CBaseEditDlg::errorForeground,Qt::TextColorRole);
        }
        // ->Stage 3. 验证“入库量”
        int in = dataModel->data(dataModel->index(idx,4)).toString().toInt(&ok,10);
        if (!ok || in<=0)
        {
            hasError = true;
            dataModel->setData(dataModel->index(idx,4),CBaseEditDlg::errorForeground,Qt::TextColorRole);
        }
        else if (in>100)
            dataModel->setData(dataModel->index(idx,4),CBaseEditDlg::warningForeground,Qt::TextColorRole);
        // ->Stage 4. 验证“库存”
        int stock = dataModel->data(dataModel->index(idx,5)).toString().toInt(&ok,10);
        int stockSrc = mainPackage->getOneField(QString("库存")).toInt();
        if (!ok || stock!=stockSrc)
        {
            hasError = true;
            dataModel->setData(dataModel->index(idx,5),CBaseEditDlg::errorForeground,Qt::TextColorRole);
        }
        // ->Stage 5. 验证“单价”
        float price = dataModel->data(dataModel->index(idx,6)).toString().toFloat(&ok);
        float priceSrc = mainPackage->getOneField(QString("单价")).toFloat();
        if (!ok || price!=priceSrc)
        {
            hasError = true;
            dataModel->setData(dataModel->index(idx,6),CBaseEditDlg::errorForeground,Qt::TextColorRole);
        }
    }
    else
    {
        // ->Stage 1. 设置所有值为“警告”级别
        for (int i=0;i<dataModel->columnCount();i++)
            dataModel->setData(dataModel->index(idx,i),CBaseEditDlg::warningForeground,Qt::TextColorRole);
        // ->Stage 2. 检验“库存”
        bool ok;
        int stock = dataModel->data(dataModel->index(idx,5)).toString().toInt(&ok,10);
        if (!ok || stock!=0)
        {
            hasError = true;
            dataModel->setData(dataModel->index(idx,5),CBaseEditDlg::errorForeground,Qt::TextColorRole);
        }
    }
    return hasError;
}

void CStorageDlg::submitData()
{
    for (int idx = 0;idx<dataModel->rowCount();idx++)
    {
        QString Name = dataModel->data(dataModel->index(idx,1)).toString();
        QString MadeIn = dataModel->data(dataModel->index(idx,2)).toString();
        mainPackage->oneCommand = QString("SELECT * FROM Medicine WHERE 药品名 = '%1' AND 厂商 = '%2'").arg(Name).arg(MadeIn);
        Q_ASSERT(mainPackage->doOneQuery());
        int resultNum = mainPackage->getResultNum();
        Q_ASSERT(resultNum==0 || resultNum==1);
        if (resultNum==1)
        {
            mainPackage->queryFirst();
            int in = dataModel->data(dataModel->index(idx,4)).toString().toInt();
            int Stock = mainPackage->getOneField(QString("库存")).toInt() + in;
            mainPackage->oneCommand = QString("UPDATE Medicine SET 库存 = '%1' WHERE 药品名 = '%2' AND 厂商 = '%3'").arg(Stock).arg(Name).arg(MadeIn);
            Q_ASSERT(mainPackage->doOneQuery());
        }
        else
        {
            int Id = mainPackage->getNewID();
            QString AgentName = dataModel->data(dataModel->index(idx,0)).toString().toUpper();
            QString Spec = dataModel->data(dataModel->index(idx,3)).toString();
            int Stock = dataModel->data(dataModel->index(idx,4)).toString().toInt();
            float Price = dataModel->data(dataModel->index(idx,6)).toString().toFloat();
            mainPackage->oneCommand = QString("INSERT INTO Medicine VALUES (%1, '%2', '%3', '%4', '%5', %6, %7)").arg(Id).arg(AgentName).arg(Name).arg(MadeIn).arg(Spec).arg(Stock).arg(Price);
            Q_ASSERT(mainPackage->doOneQuery());
        }
    }
}

void CStorageDlg::refreshList(QModelIndex TL)
{
    int thisRow = TL.row();
    QString thisAgentName = dataModel->data(dataModel->index(thisRow,0)).toString();
    QString thisName = dataModel->data(dataModel->index(thisRow,1)).toString();
    QString thisMadeIn = dataModel->data(dataModel->index(thisRow,2)).toString();
    if (!thisAgentName.isEmpty())
    {
        QVector<int> keys = mainPackage->getKeyByAgentName(thisAgentName);
        if (keys.count()==0)
        {
            dataModel->blockSignals(true);
            dataModel->setData(dataModel->index(thisRow,5),0);
            dataModel->blockSignals(false);
        }
        else if (keys.count()==1)
        {
            QString SrcName = mainPackage->getItemsByKey(keys,QString("药品名")).at(0).toString();
            QString SrcMadeIn = mainPackage->getItemsByKey(keys,QString("厂商")).at(0).toString();
            if (thisName.isEmpty() && thisMadeIn.isEmpty())
            {
                QString SrcSpec = mainPackage->getItemsByKey(keys,QString("规格")).at(0).toString();
                int SrcStock = mainPackage->getItemsByKey(keys,QString("库存")).at(0).toInt();
                float SrcPrice = mainPackage->getItemsByKey(keys,QString("单价")).at(0).toFloat();
                dataModel->blockSignals(true);
                dataModel->setData(dataModel->index(thisRow,1),SrcName);
                dataModel->setData(dataModel->index(thisRow,2),SrcMadeIn);
                dataModel->setData(dataModel->index(thisRow,3),SrcSpec);
                dataModel->setData(dataModel->index(thisRow,5),SrcStock);
                dataModel->setData(dataModel->index(thisRow,6),SrcPrice);
                dataModel->blockSignals(false);
            }
            else
            {
                nameLineDelegate->itemList.clear();
                nameLineDelegate->itemList<<SrcName;
                madeLineDelegate->itemList.clear();
                madeLineDelegate->itemList<<SrcMadeIn;
            }
        }
        else
        {
            QVector<QVariant> nameVector, madeVector;
            bool freshName = false, freshMade = false;
            if (thisName.isEmpty() && thisMadeIn.isEmpty())
            {
                nameVector = mainPackage->getItemsByKey(keys,QString("药品名"));
                madeVector = mainPackage->getItemsByKey(keys,QString("厂商"));
                freshName = true;
                freshMade = true;
            }
            else if (thisName.isEmpty() || thisMadeIn.isEmpty())
            {
                if (thisName.isEmpty())
                {
                    QVector<int> filtedKeys = mainPackage->filtKeyByField(keys,QString("厂商"),thisMadeIn);
                    if (filtedKeys.count()==0)
                        nameLineDelegate->itemList.clear();
                    else
                    {
                        nameVector = mainPackage->getItemsByKey(filtedKeys,QString("药品名"));
                        freshName = true;
                    }
                }
                else
                {
                    QVector<int> filtedKeys = mainPackage->filtKeyByField(keys,QString("药品名"),thisName);
                    if (filtedKeys.count()==0)
                        madeLineDelegate->itemList.clear();
                    else
                    {
                        madeVector = mainPackage->getItemsByKey(filtedKeys,QString("厂商"));
                        freshMade = true;
                    }
                }
            }
            if (freshName)
            {
                QStringList nameL;
                for (QVariant &name:nameVector)
                    nameL<<name.toString();
                nameLineDelegate->itemList = nameL;
            }
            if (freshMade)
            {
                QStringList madeL;
                for (QVariant &made:madeVector)
                    madeL<<made.toString();
                madeLineDelegate->itemList = madeL;
            }
        }
    }
}
