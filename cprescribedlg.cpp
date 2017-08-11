#include "cprescribedlg.h"

CPrescribeDlg::CPrescribeDlg(CDatabasePackage *dbPackage):CBaseEditDlg(dbPackage)
{
    setWindowTitle(QString("处方出库"));
    dataModel->setHeaderData(0,Qt::Horizontal,QString("药品代码"));
    dataModel->setHeaderData(1,Qt::Horizontal,QString("药品名"));
    dataModel->setHeaderData(2,Qt::Horizontal,QString("厂商"));
    dataModel->setHeaderData(3,Qt::Horizontal,QString("规格"));
    dataModel->setHeaderData(4,Qt::Horizontal,QString("出库量"));
    dataModel->setHeaderData(5,Qt::Horizontal,QString("库存"));
    dataModel->setHeaderData(6,Qt::Horizontal,QString("单价"));
    setDelegateForOUT();
    refreshAllForOut();
    connect(dataModel,SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),this,SLOT(refreshList(QModelIndex)));
}

bool CPrescribeDlg::checkOneRecord(const int idx)
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
        // 成功匹配，定位记录于数据库
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
        // ->Stage 3. 验证“库存”
        int stock = dataModel->data(dataModel->index(idx,5)).toString().toInt(&ok,10);
        int stockSrc = mainPackage->getOneField(QString("库存")).toInt();
        if (!ok || stock!=stockSrc)
        {
            hasError = true;
            dataModel->setData(dataModel->index(idx,5),CBaseEditDlg::errorForeground,Qt::TextColorRole);
        }
        // ->Stage 4. 验证“出库量”
        int out = dataModel->data(dataModel->index(idx,4)).toString().toInt(&ok,10);
        if (!ok || out>stockSrc)
        {
            hasError = true;
            dataModel->setData(dataModel->index(idx,4),CBaseEditDlg::errorForeground,Qt::TextColorRole);
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
        hasError = true;
        // ->Stage 1. 匹配失败，设置“药品代码”、“药品名”和“厂商”为“错误”级别
        dataModel->setData(dataModel->index(idx,0),CBaseEditDlg::errorForeground,Qt::TextColorRole);
        dataModel->setData(dataModel->index(idx,1),CBaseEditDlg::errorForeground,Qt::TextColorRole);
        dataModel->setData(dataModel->index(idx,2),CBaseEditDlg::errorForeground,Qt::TextColorRole);
    }
    return hasError;
}

void CPrescribeDlg::submitData()
{
    progressShow->setRange(0,dataModel->rowCount());
    progressShow->setHidden(false);
    for (int idx = 0;idx<dataModel->rowCount();idx++)
    {
        progressShow->setValue(idx);
        QString Name = dataModel->data(dataModel->index(idx,1)).toString();
        QString MadeIn = dataModel->data(dataModel->index(idx,2)).toString();
        int out = dataModel->data(dataModel->index(idx,4)).toString().toInt();
        int Stock = mainPackage->getOneField(QString("库存")).toInt() - out;
        mainPackage->oneCommand = QString("UPDATE Medicine SET 库存 = '%1' WHERE 药品名 = '%2' AND 厂商 = '%3'").arg(Stock).arg(Name).arg(MadeIn);
        Q_ASSERT(mainPackage->doOneQuery());
    }
    progressShow->setValue(dataModel->rowCount());
}

void CPrescribeDlg::refreshList(QModelIndex TL)
{
    int thisRow = TL.row();
    QString thisAgentName = dataModel->data(dataModel->index(thisRow,0)).toString();
    QString thisName = dataModel->data(dataModel->index(thisRow,1)).toString();
    QString thisMadeIn = dataModel->data(dataModel->index(thisRow,2)).toString();
    if (thisAgentName.isEmpty() && thisName.isEmpty() && thisMadeIn.isEmpty())
        refreshAllForOut();
    else
    {
        mainPackage->oneCommand = QString("SELECT * FROM Medicine WHERE ");
        mainPackage->oneCommand.append(parsePrecisely(&thisAgentName, &thisName, &thisMadeIn));
        Q_ASSERT(mainPackage->doOneQuery());
        if (mainPackage->getResultNum()==1)
        {
            mainPackage->queryFirst();
            QString SrcAgentName = mainPackage->getOneField(QString("药品代码")).toString();
            QString SrcName = mainPackage->getOneField(QString("药品名")).toString();
            QString SrcMadeIn = mainPackage->getOneField(QString("厂商")).toString();
            QString SrcSpec = mainPackage->getOneField(QString("规格")).toString();
            int SrcStock = mainPackage->getOneField(QString("库存")).toInt();
            float SrcPrice = mainPackage->getOneField(QString("单价")).toFloat();
            dataModel->blockSignals(true);
            dataModel->setData(dataModel->index(thisRow,0),SrcAgentName);
            dataModel->setData(dataModel->index(thisRow,1),SrcName);
            dataModel->setData(dataModel->index(thisRow,2),SrcMadeIn);
            dataModel->setData(dataModel->index(thisRow,3),SrcSpec);
            dataModel->setData(dataModel->index(thisRow,5),SrcStock);
            dataModel->setData(dataModel->index(thisRow,6),SrcPrice);
            dataModel->blockSignals(false);
        }
        else
            parseStrictlyForOut(&thisAgentName, &thisName, &thisMadeIn);
    }
}
