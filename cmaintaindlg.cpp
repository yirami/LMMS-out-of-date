#include "cmaintaindlg.h"

CMaintainDlg::CMaintainDlg(CDatabasePackage *dbPackage):CDirectEditDlg(dbPackage)
{
    setUI();
    setDelegate();
    connect(deleteB,SIGNAL(clicked()),this,SLOT(on_deleteB_clicked()));
    connect(submitB,SIGNAL(clicked()),this,SLOT(on_submitB_clicked()));
}

void CMaintainDlg::setUI()
{
    this->setFixedSize(1000,600);
    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::WindowMinimizeButtonHint;
    flags |=Qt::WindowCloseButtonHint;
    setWindowFlags(flags);

    actionBox = new QGroupBox(this);
    actionBox->setFixedWidth(220);
    actionBox->setTitle("操作");
    deleteB = new QPushButton(this);
    deleteB->setMinimumHeight(50);
    deleteB->setText("删除");
    submitB = new QPushButton(this);
    submitB->setMinimumHeight(50);
    submitB->setText("提交");
    QVBoxLayout *actionboxlayout = new QVBoxLayout(this);
    actionboxlayout->addStretch(1);
    actionboxlayout->addWidget(deleteB);
    actionboxlayout->addStretch(2);
    actionboxlayout->addWidget(queryBox);
    actionboxlayout->addStretch(2);
    actionboxlayout->addWidget(submitB);
    actionboxlayout->addStretch(1);
    actionBox->setLayout(actionboxlayout);

    tabV = new QTableView(this);
    tabV->setModel(dataModel);
    tabV->setColumnWidth(0,60);
    tabV->setColumnHidden(0,true);
    tabV->setColumnWidth(1,100);
    tabV->setColumnWidth(2,180);
    tabV->setColumnWidth(3,180);
    tabV->setColumnWidth(4,100);
    tabV->setColumnWidth(5,80);
    tabV->setColumnWidth(6,80);
    tabV->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    tabV->verticalHeader()->setMinimumWidth(22);
    tabV->verticalHeader()->hide();
    tabV->setSelectionBehavior(QAbstractItemView::SelectRows);
    tabV->setSelectionMode( QAbstractItemView::MultiSelection);
    tabV->show();

    mainBox = new QGroupBox(this);
    QHBoxLayout *mlayout = new QHBoxLayout(this);
    mlayout->addWidget(tabV);
    mlayout->addWidget(actionBox);
//    mainBox->setLayout(mlayout);

//    QVBoxLayout *wlayout = new QVBoxLayout(this);
//    wlayout->addWidget(mainBox);
//    wlayout->addWidget(progressBox);
//    this->setLayout(wlayout);
    this->setLayout(mlayout);

    this->setWindowTitle(QString("维护"));
}

void CMaintainDlg::on_deleteB_clicked()
{
    QItemSelectionModel *thisSelectionM = tabV->selectionModel();
    if (thisSelectionM->hasSelection())
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
            dataModel->removeRows(rowToDel,1);
        }
        thisSelectionM->clearSelection();
        switch(QMessageBox::information(this,QString("确认删除"),QString("确认删除选中项吗？Yes：删除；No：再瞅瞅！"),QMessageBox::Yes|QMessageBox::No))
        {
        case QMessageBox::Yes:
            dataModel->submitAll();
            break;
        default:
            dataModel->revertAll();
            break;
        }
    }
}

void CMaintainDlg::on_submitB_clicked()
{
    if (dataModel->rowCount() != 0)
    {
        switch(QMessageBox::information(this,QString("确认"),QString("还需提交修改吗？Yes：提交；No：再瞅瞅！"),QMessageBox::Yes|QMessageBox::No,QMessageBox::No))
        {
        case QMessageBox::Yes:
            if (!dataModel->submitAll())
                qDebug() << dataModel->lastError();
            break;
        default:
            break;
        }
    }
    reject();
}

void CMaintainDlg::setDelegate()
{
    nameDelegate->itemList = nameList;
    madeDelegate->itemList = madeList;
    tabV->setItemDelegateForColumn(1,agentDelegate);
    tabV->setItemDelegateForColumn(2,nameDelegate);
    tabV->setItemDelegateForColumn(3,madeDelegate);
    tabV->setItemDelegateForColumn(4,specDelegate);
    tabV->setItemDelegateForColumn(5,stockDelegate);
    tabV->setItemDelegateForColumn(6,priceDelegate);
}
