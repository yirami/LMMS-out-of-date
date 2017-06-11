#include "storagedlg.h"

#include <QDebug>
#include <QCloseEvent>
#include <QSqlRecord>

storageDlg::storageDlg(QWidget *parent)
    : QDialog(parent)
{
    setUI();
    configModel();
    tabV->show();

    connect(addB,SIGNAL(clicked()),this,SLOT(on_addB_clicked()));
    connect(deleteB,SIGNAL(clicked()),this,SLOT(on_deleteB_clicked()));
    connect(cancelB,SIGNAL(clicked()),this,SLOT(on_cancelB_clicked()));
    connect(checkB,SIGNAL(clicked()),this,SLOT(on_checkB_clicked()));
    connect(submitB,SIGNAL(clicked()),this,SLOT(accept()));
}

void storageDlg::setUI()
{
    this->setFixedSize(1000,600);
    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::WindowMinimizeButtonHint;
    flags |=Qt::WindowCloseButtonHint;
    setWindowFlags(flags);

    QGroupBox *actionBox = new QGroupBox(this);
    actionBox->setFixedWidth(150);
    actionBox->setTitle("操作");
    addB = new QPushButton(this);
    addB->setMinimumHeight(50);
    addB->setText("添加");
    deleteB = new QPushButton(this);
    deleteB->setMinimumHeight(50);
    deleteB->setText("删除");
    cancelB = new QPushButton(this);
    cancelB->setMinimumHeight(50);
    cancelB->setText("取消");
    checkB = new QPushButton(this);
    checkB->setMinimumHeight(50);
    checkB->setText("检查");
    submitB = new QPushButton(this);
    submitB->setMinimumHeight(50);
    submitB->setText("提交");
    tabV = new QTableView(this);

    QVBoxLayout *actboxlayout = new QVBoxLayout(this);
    actboxlayout->addStretch(1);
    actboxlayout->addWidget(addB);
    actboxlayout->addStretch(2);
    actboxlayout->addWidget(deleteB);
    actboxlayout->addStretch(8);
    actboxlayout->addWidget(cancelB);
    actboxlayout->addStretch(2);
    actboxlayout->addWidget(checkB);
    actboxlayout->addStretch(2);
    actboxlayout->addWidget(submitB);
    actboxlayout->addStretch(1);

    actionBox->setLayout(actboxlayout);

    QHBoxLayout *mlayout = new QHBoxLayout(this);
    mlayout->addWidget(tabV);
    mlayout->addWidget(actionBox);

    this->setLayout(mlayout);
    this->setWindowTitle(QString("药品入库"));
    submitB->setEnabled(false);
}

void storageDlg::configModel()
{
    tabV->setModel(storageData);
    tabV->setColumnWidth(0,100);
    tabV->setColumnWidth(1,180);
    tabV->setColumnWidth(2,180);
    tabV->setColumnWidth(3,100);
    tabV->setColumnWidth(4,80);
    tabV->setColumnWidth(5,80);
    tabV->setColumnWidth(6,60);
    tabV->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    tabV->setSelectionBehavior(QAbstractItemView::SelectRows);
    tabV->setSelectionMode( QAbstractItemView::SingleSelection);
}

void storageDlg::on_cancelB_clicked()
{
    int rowEnd = storageData->rowCount();
    storageData->removeRows(0,rowEnd);
    reject();
}

void storageDlg::on_addB_clicked()
{
    int rowNum = storageData->rowCount();
    int columnNum = storageData->columnCount();
    storageData->insertRow(rowNum);

    for (int i=0;i<columnNum;i++)
    {
        storageData->setItem(rowNum,i,new QStandardItem(QString("")));
        storageData->item(rowNum,i)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        switch (columnNum)
        {
        case 3:case 5:case 6:
            storageData->item(rowNum,i)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            if (rowNum%2 == 1)
                storageData->item(rowNum,i)->setBackground(normal1Background);
            break;
        default:
            storageData->item(rowNum,i)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
            if (rowNum%2 == 1)
                storageData->item(rowNum,i)->setBackground(normal1Background);
        }
    }
}

void storageDlg::on_deleteB_clicked()
{
    QItemSelectionModel *thisSelectionM = tabV->selectionModel();
    if (!thisSelectionM->hasSelection())
    {

    }
    else
    {
        QModelIndexList selections = thisSelectionM->selectedIndexes();
        QMap<int, int> rowMap;
        foreach (QModelIndex index, selections)
        {
            rowMap.insert(index.row(), 0);
        }
        QMapIterator<int, int> rowMapIterator(rowMap);
        rowMapIterator.toBack();
        while (rowMapIterator.hasPrevious())
        {
            rowMapIterator.previous();
            int rowToDel = rowMapIterator.key();
            storageData->removeRow(rowToDel);
        }
        recoveryBackground();
    }
}

void storageDlg::on_checkB_clicked()
{
    int rowEnd = storageData->rowCount();
    if (rowEnd != 0)
    {
        addB->setEnabled(false);
        deleteB->setEnabled(false);
        tabV->setEditTriggers(QAbstractItemView::NoEditTriggers);
        hangFlag = false;
        for (int nextOne = 0;nextOne<rowEnd;nextOne++)
        {
            // 恢复所有背景设定
            recoveryBackground();
            // 检查
            checkOneRecord(nextOne);
        }
        if (hangFlag)
        {
            QMessageBox::about(NULL,QString("警告"),QString("当前列表存在严重问题，请修改确认后再次检查并提交！"));
            addB->setEnabled(true);
            deleteB->setEnabled(true);
            tabV->setEditTriggers(QAbstractItemView::AllEditTriggers);
        }
        else
        {
            switch(QMessageBox::information(this,QString("确认"),QString("还需要修改吗？Yes：返回修改；No：准备提交，不再修改！"),QMessageBox::Yes|QMessageBox::No))
            {
            case QMessageBox::No:
                checkB->setEnabled(false);
                submitB->setEnabled(true);
                break;
            default:
                addB->setEnabled(true);
                deleteB->setEnabled(true);
                tabV->setEditTriggers(QAbstractItemView::AllEditTriggers);
                break;
            }
        }
    }
}

void storageDlg::checkOneRecord(const int idx)
{
    // 检查是否为空
    for (int i=0;i<storageData->columnCount();i++)
    {
        if (storageData->item(idx,i)->text().isEmpty())
        {
            hangFlag = true;
            storageData->item(idx,i)->setBackground(errorBackground);
        }
    }
    // 检查数据能否正确匹配
    if (!(storageData->item(idx,0)->text().isEmpty() && storageData->item(idx,1)->text().isEmpty() && storageData->item(idx,2)->text().isEmpty()))
    {
        QString Name = storageData->item(idx,1)->text();
        QString MadeIn = storageData->item(idx,2)->text();
        int Quant, Stock;
        mainDB = QSqlDatabase::database(mainName);
        QString thisCMD = QString("SELECT * FROM Medicine WHERE 药品名 = '%1' AND 厂商 = '%2'").arg(Name).arg(MadeIn);
        QSqlQuery Query = QSqlQuery(mainDB);
        Query.exec(thisCMD);
        QSqlRecord record = Query.record();
        int hitNum = record.count();
        if (hitNum!=0)
        {
            // 成功匹配条目
            // ->Stage 1. 检查数据类型
            if (isDiditStr(storageData->item(idx,4)->text()))
            {
                bool ok;
                Quant = storageData->item(idx,4)->text().toInt(&ok,10);
                if (!ok)
                {
                    hangFlag = true;
                    storageData->item(idx,4)->setForeground(errorForeground);
                }
                else
                {
                    // ->Stage 2. 读取数据库中库存数量
                    Query.first();
                    Stock = Query.value(QString("库存")).toInt();
                    // ->Stage 3. 确认入库数量的合理性与合法性
                    if (Quant<=0 || Quant>Stock)
                    {
                        hangFlag = true;
                        storageData->item(idx,4)->setForeground(errorForeground);
                    }
                }
            }
            else
            {
                hangFlag = true;
                storageData->item(idx,4)->setForeground(errorForeground);
            }
        }
    }
}

void storageDlg::recoveryBackground()
{
    for (int i=0;i<storageData->rowCount();i++)
    {
        if (i%2 == 0)
        {
            for (int j=0;j<storageData->columnCount();j++)
            {
                storageData->item(i,j)->setBackground(normal0Background);
            }
        }
        else
        {
            for (int j=0;j<storageData->columnCount();j++)
            {
                storageData->item(i,j)->setBackground(normal1Background);
            }
        }
    }
}

bool storageDlg::isDiditStr(QString thisString)
{
    // QString 转换为 char*
    QByteArray cmp = thisString.toLatin1();
    const char *s = cmp.data();
    while(*s && *s>='0' && *s<='9') s++;

    if (*s)
    {
        return false;
    }
    else
    {
        return true;
    }
}
