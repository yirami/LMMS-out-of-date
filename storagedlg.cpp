#include "storagedlg.h"

#include <QDebug>
#include <QCloseEvent>
#include <QAbstractItemView>
#include <QSqlRecord>
#include <QVector>
#include <QMessageBox>

storageDlg::storageDlg(QWidget *parent)
    : QDialog(parent)
{
    setUI();
    mainDB = QSqlDatabase::database(mainName);
    if (QSqlDatabase::contains(stoName))
    {
        stoDB = QSqlDatabase::database(stoName);
    }
    else
    {
        stoDB = QSqlDatabase::addDatabase("QSQLITE",stoName);
        stoDB.setDatabaseName("");
        stoDB.open();
        QSqlQuery query = QSqlQuery(stoDB);
        query.exec("create table inMedicine(id int primary key,state int,药品代码 varchar,药品名 varchar,厂商 varchar,规格 varchar,新增数量 int,单价 int)");
    }
    storageModel = new QSqlTableModel(this,stoDB);
    storageModel->setTable("inMedicine");
    storageModel->select();
    storageModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    tabV->setModel(storageModel);
    tabV->setColumnWidth(0,40);
    tabV->setColumnWidth(1,60);
    tabV->setColumnWidth(2,160);
    tabV->setColumnWidth(3,200);
    tabV->setColumnWidth(4,200);
    tabV->setColumnWidth(5,60);
    tabV->setColumnWidth(6,80);
    tabV->setColumnWidth(7,60);
    tabV->setColumnHidden(0,true);
    tabV->setColumnHidden(1,true);
    tabV->sortByColumn(0,Qt::AscendingOrder);
    tabV->setSortingEnabled(true);
    tabV->setSelectionBehavior(QAbstractItemView::SelectRows);
    tabV->setSelectionMode( QAbstractItemView::SingleSelection);
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

void storageDlg::on_cancelB_clicked()
{
    int rowEnd = storageModel->rowCount();
    for (int nextOne = 0;nextOne<rowEnd;nextOne++)
    {
        storageModel->removeRow(nextOne);
    }
    storageModel->submitAll();
    reject();
}

void storageDlg::on_addB_clicked()
{
    int rowNum = storageModel->rowCount();
    int id = rowNum;
    storageModel->insertRow(rowNum);
    storageModel->setData(storageModel->index(rowNum,0),id);
    storageModel->setData(storageModel->index(rowNum,1),0);

    storageModel->submitAll();
}

void storageDlg::on_deleteB_clicked()
{
    QItemSelectionModel *thisSelectionM = tabV->selectionModel();
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
        storageModel->removeRow(rowToDel);
        int rowEnd = storageModel->rowCount();
        for (int nextFix = rowToDel+1;nextFix<rowEnd;nextFix++)
        {
            QVariant currid = storageModel->data(storageModel->index(nextFix,0));
            storageModel->setData(storageModel->index(nextFix,0),currid.toInt()-1);
        }
        storageModel->submitAll();
    }
}

void storageDlg::on_checkB_clicked()
{
    storageModel->submitAll();
    int rowEnd = storageModel->rowCount();
    if (rowEnd != 0)
    {
        addB->setEnabled(false);
        deleteB->setEnabled(false);
        tabV->setEditTriggers(QAbstractItemView::NoEditTriggers);
        hangFlag = false;
        for (int nextOne = 0;nextOne<rowEnd;nextOne++)
        {
            QSqlRecord thisRecord = storageModel->record(nextOne);
            QString Okey1 = thisRecord.value(QString("药品名")).toString();
            QString Okey2 = thisRecord.value(QString("厂商")).toString();
            QString thisCMD = QString("SELECT * FROM Medicine WHERE 药品名 = '%1' AND 厂商 = '%2'").arg(Okey1).arg(Okey2);
            QSqlQuery Query = QSqlQuery(mainDB);
            Query.exec(thisCMD);
            if (Query.at()!=QSql::BeforeFirstRow || Query.at()!=QSql::AfterLastRow)
            {
                Query.first();
                // 成功匹配，置标志位
                storageModel->setData(storageModel->index(nextOne,1),1);
                storageModel->submitAll();
                // 匹配-规格和单价，并在必要时设置预警

                // 检索空单元,如存在空单元，则设置标志位，并在后期强制阻止提交
                QVector <int> except = checkRecord(thisRecord);
                int num = except.count();
                if (num != 0)
                    hangFlag = true;

            }
            else
            {
                // 未匹配，置标志位,设置背景色提供预警
                storageModel->setData(storageModel->index(nextOne,1),2);
                storageModel->submitAll();
                // 检索空单元,如存在空单元，则设置标志位，并在后期强制阻止提交
                QVector <int> except = checkRecord(thisRecord);
                int num = except.count();
                if (num != 0)
                    hangFlag = true;
            }
        }
        if (hangFlag)
        {
            QMessageBox::about(NULL,QString("警告"),QString("当前列表存在未填项，请补齐后再检查！"));
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

QVector <int> storageDlg::checkRecord(const QSqlRecord &record)
{
    QVector <int> except;
    if (record.value(QString("药品代码")).toString().isEmpty())
        except<<record.indexOf(QString("药品代码"));
    if (record.value(QString("药品名")).toString().isEmpty())
        except<<record.indexOf(QString("药品名"));
    if (record.value(QString("厂商")).toString().isEmpty())
        except<<record.indexOf(QString("厂商"));
    if (record.value(QString("规格")).toString().isEmpty())
        except<<record.indexOf(QString("规格"));
    if (record.value(QString("新增数量")).toString().isEmpty())
        except<<record.indexOf(QString("新增数量"));
    if (record.value(QString("单价")).toString().isEmpty())
        except<<record.indexOf(QString("单价"));
    return except;
}
