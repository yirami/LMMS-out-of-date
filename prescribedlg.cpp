#include "prescribedlg.h"

prescribeDlg::prescribeDlg(QWidget *parent)
    : QDialog(parent)
{
    setUI();
    configModel();
    QSqlDatabase mainDB = QSqlDatabase::database(mainName);

    tabV->show();

    connect(addB,SIGNAL(clicked()),this,SLOT(on_addB_clicked()));
    connect(deleteB,SIGNAL(clicked()),this,SLOT(on_deleteB_clicked()));
    connect(cancelB,SIGNAL(clicked()),this,SLOT(on_cancelB_clicked()));
    connect(checkB,SIGNAL(clicked()),this,SLOT(on_checkB_clicked()));
    connect(submitB,SIGNAL(clicked()),this,SLOT(accept()));
}
void prescribeDlg::setUI()
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
    this->setWindowTitle(QString("处方"));
}

void prescribeDlg::configModel()
{
    tabV->setModel(prescribeData);
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

void prescribeDlg::on_cancelB_clicked()
{
    int rowEnd = prescribeData->rowCount();
    prescribeData->removeRows(0,rowEnd);
    reject();
}

void prescribeDlg::on_addB_clicked()
{
    int rowNum = prescribeData->rowCount();
    int columnNum = prescribeData->columnCount();
    prescribeData->insertRow(rowNum);

    for (int i=0;i<columnNum;i++)
    {
        prescribeData->setItem(rowNum,i,new QStandardItem(QString("")));
        prescribeData->item(rowNum,i)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        switch (columnNum)
        {
        case 3:case 5:case 6:
            prescribeData->item(rowNum,i)->setFlags(Qt::ItemIsEnabled);
            break;
        default:
            prescribeData->item(rowNum,i)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
        }
    }

}

void prescribeDlg::on_deleteB_clicked()
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
        prescribeData->removeRow(rowToDel);
    }
}

void prescribeDlg::on_checkB_clicked()
{
    int rowEnd = prescribeData->rowCount();
    if (rowEnd != 0)
    {
        addB->setEnabled(false);
        deleteB->setEnabled(false);
        tabV->setEditTriggers(QAbstractItemView::NoEditTriggers);
        hangFlag = false;
        for (int nextOne = 0;nextOne<rowEnd;nextOne++)
        {
            QString sName(prescribeData->item(nextOne,0)->text());
            QString Name(prescribeData->item(nextOne,1)->text());
            QString MadeIn(prescribeData->item(nextOne,2)->text());
            QString Spec(prescribeData->item(nextOne,3)->text());
            QVariant Quant(prescribeData->item(nextOne,4)->text().toInt());
            if (!prescribeData->item(nextOne,5))
                prescribeData->setItem(0,5,new QStandardItem(QString("空的")));
            if (!prescribeData->item(nextOne,6))
                prescribeData->setItem(0,6,new QStandardItem(QString("空的")));

            /*QString thisCMD = QString("SELECT * FROM Medicine WHERE 药品名 = '%1' AND 厂商 = '%2'").arg(Okey1).arg(Okey2);
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
            }*/
        }
        /*if (hangFlag)
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
        }*/
    }
}

/*QVector <int> prescribeDlg::checkRecord(const QSqlRecord &record)
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
}*/
