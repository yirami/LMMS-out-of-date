#include "cbaseeditdlg.h"

QBrush CBaseEditDlg::normalForeground(QBrush(Qt::black));
QBrush CBaseEditDlg::warningForeground(QBrush(Qt::green));
QBrush CBaseEditDlg::errorForeground(QBrush(Qt::red));
QBrush CBaseEditDlg::normal0Background(QBrush(Qt::white));
QBrush CBaseEditDlg::normal1Background(QBrush(Qt::lightGray));
QBrush CBaseEditDlg::errorBackground(QBrush(Qt::red));
QRegExp CBaseEditDlg::allCapitalRE("^[A-Z]+$");
QRegExp CBaseEditDlg::allChineseRE("^[\u4e00-\u9fa5]+$");
QRegExp CBaseEditDlg::beginDigitRE("^[0-9]*[1-9][0-9]*[A-Za-z\u4e00-\u9fa5]+$");
CLineEditDelegate *CBaseEditDlg::agentLineDelegate = new CLineEditDelegate(CBaseEditDlg::allCapitalRE,true);
CLineEditDelegate *CBaseEditDlg::nameLineDelegate = new CLineEditDelegate(CBaseEditDlg::allChineseRE,true);
CLineEditDelegate *CBaseEditDlg::madeLineDelegate = new CLineEditDelegate(CBaseEditDlg::allChineseRE,true);
CLineEditDelegate *CBaseEditDlg::specLineDelegate = new CLineEditDelegate(CBaseEditDlg::beginDigitRE);
CComboDelegate *CBaseEditDlg::nameComboDelegate = new CComboDelegate;
CComboDelegate *CBaseEditDlg::madeComboDelegate = new CComboDelegate;
CSpinDelegate *CBaseEditDlg::ioDelegate = new CSpinDelegate;
CSpinDelegate *CBaseEditDlg::stockDelegate = new CSpinDelegate;
CDoubleSpinDelegate *CBaseEditDlg::priceDelegate = new CDoubleSpinDelegate;

CBaseEditDlg::CBaseEditDlg(CDatabasePackage *dbPackage, QWidget *parent)
    :QDialog(parent), mainPackage(dbPackage)
{
    setUI();
    configModel();
    connect(addB,SIGNAL(clicked()),this,SLOT(on_addB_clicked()));
    connect(deleteB,SIGNAL(clicked()),this,SLOT(on_deleteB_clicked()));
    connect(cancelB,SIGNAL(clicked()),this,SLOT(on_cancelB_clicked()));
    connect(checkB,SIGNAL(clicked()),this,SLOT(on_checkB_clicked()));

    on_addB_clicked();
}

void CBaseEditDlg::setUI()
{
    this->setFixedSize(1000,600);
    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::WindowMinimizeButtonHint;
    flags |=Qt::WindowCloseButtonHint;
    setWindowFlags(flags);

    actionBox = new QGroupBox(this);
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
    actboxlayout->addStretch(1);

    actionBox->setLayout(actboxlayout);

    mainBox = new QGroupBox(this);
    mainBox->setStyleSheet("QGroupBox{border:none;}");
    QHBoxLayout *mlayout = new QHBoxLayout(this);
    mlayout->addWidget(tabV);
    mlayout->addWidget(actionBox);
    mainBox->setLayout(mlayout);

    progressBox = new QGroupBox(this);
    progressBox->setFixedSize(1000,30);
    progressBox->setStyleSheet("QGroupBox{border:none;}");
    progressInfo = new QLabel(this);
    progressInfo->setFixedHeight(20);
    progressInfo->setFixedWidth(200);
    progressInfo->setText(QString(""));
    progressShow = new QProgressBar(this);
    progressShow->setFixedHeight(20);
    progressShow->setFixedWidth(750);
    progressShow->setHidden(true);
    QHBoxLayout *playout = new QHBoxLayout(this);
    playout->addWidget(progressInfo);
    playout->addWidget(progressShow);
    progressBox->setLayout(playout);


    QVBoxLayout *wlayout = new QVBoxLayout(this);
    wlayout->addWidget(mainBox);
    wlayout->addWidget(progressBox);

    this->setLayout(wlayout);
}

void CBaseEditDlg::configModel()
{
    dataModel = new QStandardItemModel(0,7);
    tabV->setModel(dataModel);
    tabV->setColumnWidth(0,100);
    tabV->setColumnWidth(1,180);
    tabV->setColumnWidth(2,180);
    tabV->setColumnWidth(3,100);
    tabV->setColumnWidth(4,80);
    tabV->setColumnWidth(5,80);
    tabV->setColumnWidth(6,60);
    tabV->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    tabV->verticalHeader()->setMinimumWidth(22);
    tabV->setSelectionBehavior(QAbstractItemView::SelectRows);
    tabV->setSelectionMode( QAbstractItemView::MultiSelection);
    tabV->show();
}

void CBaseEditDlg::on_cancelB_clicked()
{
    reject();
}

void CBaseEditDlg::on_addB_clicked()
{
    int i = dataModel->rowCount();
    dataModel->insertRow(i);
    if (i%2 == 0)
    {
        for (int j=0;j<dataModel->columnCount();j++)
            dataModel->setData(dataModel->index(i,j),CBaseEditDlg::normal1Background,Qt::BackgroundColorRole);
    }
    else
    {
        for (int j=0;j<dataModel->columnCount();j++)
            dataModel->setData(dataModel->index(i,j),CBaseEditDlg::normal0Background,Qt::BackgroundColorRole);
    }

}

void CBaseEditDlg::on_deleteB_clicked()
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
            dataModel->removeRow(rowToDel);
        }
        thisSelectionM->clearSelection();
        recoveryBackground();
    }
}

void CBaseEditDlg::on_checkB_clicked()
{
    // 清除所有选中
    QItemSelectionModel *thisSelectionM = tabV->selectionModel();
    thisSelectionM->clearSelection();
    //
    if (dataModel->rowCount() != 0)
    {
        addB->setEnabled(false);
        deleteB->setEnabled(false);
        tabV->setEditTriggers(QAbstractItemView::NoEditTriggers);
        bool hasError = false;
        recoveryBackground();
        for (int nextOne = 0;nextOne<dataModel->rowCount();nextOne++)
            hasError = checkOneRecord(nextOne) || hasError;
        if (hasError)
        {
            QMessageBox::critical(NULL,QString("警告"),QString("当前列表存在严重问题，请修改确认后再次检查并提交！"));
            addB->setEnabled(true);
            deleteB->setEnabled(true);
            tabV->setEditTriggers(QAbstractItemView::AllEditTriggers);
        }
        else
        {
            switch(QMessageBox::information(this,QString("确认"),QString("确定提交吗？Yes：准备提交；No：再瞅瞅！"),QMessageBox::Yes|QMessageBox::No,QMessageBox::No))
            {
            case QMessageBox::Yes:
            {
                progressInfo->setText(QString("正在提交至数据库..."));
                submitData();
                QTime timer = QTime::currentTime().addMSecs(2000);
                while(QTime::currentTime()<timer);
                accept();
            }
            default:
                addB->setEnabled(true);
                deleteB->setEnabled(true);
                tabV->setEditTriggers(QAbstractItemView::AllEditTriggers);
                break;
            }
        }
    }
}

void CBaseEditDlg::setDelegateForIN()
{
    tabV->setItemDelegateForColumn(0,agentLineDelegate);
    tabV->setItemDelegateForColumn(1,nameLineDelegate);
    tabV->setItemDelegateForColumn(2,madeLineDelegate);
    tabV->setItemDelegateForColumn(3,specLineDelegate);
    tabV->setItemDelegateForColumn(4,ioDelegate);
    tabV->setItemDelegateForColumn(5,stockDelegate);
    tabV->setItemDelegateForColumn(6,priceDelegate);
}

void CBaseEditDlg::setDelegateForOUT()
{
    tabV->setItemDelegateForColumn(0,agentLineDelegate);
    tabV->setItemDelegateForColumn(1,nameComboDelegate);
    tabV->setItemDelegateForColumn(2,madeComboDelegate);
    tabV->setItemDelegateForColumn(3,specLineDelegate);
    tabV->setItemDelegateForColumn(4,ioDelegate);
    tabV->setItemDelegateForColumn(5,stockDelegate);
    tabV->setItemDelegateForColumn(6,priceDelegate);
}

void CBaseEditDlg::refreshList(QModelIndex TL)
{
    qDebug()<<QString("信号发生行：")<<TL.row();
    qDebug("This indicates that the virtual function is called abnormally");
}

void CBaseEditDlg::refreshAllForIn()
{
    agentLineDelegate->itemList =  mainPackage->getAllAgentName().toSet().toList();
    nameLineDelegate->itemList = mainPackage->getAllName().toSet().toList();
    madeLineDelegate->itemList = mainPackage->getAllMadeIn().toSet().toList();
}

void CBaseEditDlg::refreshAllForOut()
{
    agentLineDelegate->itemList =  mainPackage->getAllAgentName().toSet().toList();
    nameComboDelegate->itemList = mainPackage->getAllName().toSet().toList();
    madeComboDelegate->itemList = mainPackage->getAllMadeIn().toSet().toList();
}

QString CBaseEditDlg::parsePrecisely(QString *agentName, QString *name, QString *madeIn)
{
    QString queryStatement;
    QVector<QString> cache;
    if (!agentName->isEmpty())
        cache.append(QString("药品代码 = '%1'").arg(*agentName));
    if (!name->isEmpty())
        cache.append(QString("药品名 = '%1'").arg(*name));
    if (!madeIn->isEmpty())
        cache.append(QString("厂商 = '%1'").arg(*madeIn));
    if (!cache.isEmpty())
    {
        queryStatement += cache.first();
        cache.removeFirst();
        while(!cache.isEmpty())
        {
            queryStatement += QString(" AND ");
            queryStatement += cache.first();
            cache.removeFirst();
        }
    }
    return queryStatement;
}

void CBaseEditDlg::parseStrictlyForIn(QString *agentName, QString *name, QString *madeIn)
{
    bool freshA = true, freshN = true, freshM = true;
    QVector<int> allKeys = mainPackage->getAllKey();
    if (allKeys.count()!=0)
    {
        QVector<int> filtKeys;
        if (!agentName->isEmpty() && allKeys.count()!=0)
        {
            filtKeys = mainPackage->filtKeyByField(allKeys, QString("药品代码"), *agentName);
            if (filtKeys.count()!=0)
            {
                allKeys = filtKeys;
                freshA = false;
            }
        }
        if (!name->isEmpty() && allKeys.count()!=0)
        {
            filtKeys = mainPackage->filtKeyByField(allKeys, QString("药品名"), *name);
            if (filtKeys.count()!=0)
            {
                allKeys = filtKeys;
                freshN = false;
            }
        }
        if (!madeIn->isEmpty() && allKeys.count()!=0)
        {
            filtKeys = mainPackage->filtKeyByField(allKeys, QString("厂商"), *madeIn);
            if (filtKeys.count()!=0)
            {
                allKeys = filtKeys;
                freshM = false;
            }
        }
        if (freshA && freshN && freshM)
            refreshAllForIn();
        else
        {
            if (freshA)
            {
                QVector<QVariant> aVector = mainPackage->getItemsByKey(allKeys,QString("药品代码"));
                QStringList agentL;
                for (QVariant &agent:aVector)
                    agentL<<agent.toString();
                agentLineDelegate->itemList = agentL.toSet().toList();
            }
            if (freshN)
            {
                QVector<QVariant> nVector = mainPackage->getItemsByKey(allKeys,QString("药品名"));
                QStringList nameL;
                for (QVariant &name:nVector)
                    nameL<<name.toString();
                nameLineDelegate->itemList = nameL.toSet().toList();
            }
            if (freshM)
            {
                QVector<QVariant> mVector = mainPackage->getItemsByKey(allKeys,QString("厂商"));
                QStringList madeL;
                for (QVariant &made:mVector)
                    madeL<<made.toString();
                madeLineDelegate->itemList = madeL.toSet().toList();
            }
        }
    }
}

void CBaseEditDlg::parseStrictlyForOut(QString *agentName, QString *name, QString *madeIn)
{
    bool freshA = true, freshN = true, freshM = true;
    QVector<int> allKeys = mainPackage->getAllKey();
    if (allKeys.count()!=0)
    {
        QVector<int> filtKeys;
        if (!agentName->isEmpty() && allKeys.count()!=0)
        {
            filtKeys = mainPackage->filtKeyByField(allKeys, QString("药品代码"), *agentName);
            if (filtKeys.count()!=0)
            {
                allKeys = filtKeys;
                freshA = false;
            }
        }
        if (!name->isEmpty() && allKeys.count()!=0)
        {
            filtKeys = mainPackage->filtKeyByField(allKeys, QString("药品名"), *name);
            if (filtKeys.count()!=0)
            {
                allKeys = filtKeys;
                freshN = false;
            }
        }
        if (!madeIn->isEmpty() && allKeys.count()!=0)
        {
            filtKeys = mainPackage->filtKeyByField(allKeys, QString("厂商"), *madeIn);
            if (filtKeys.count()!=0)
            {
                allKeys = filtKeys;
                freshM = false;
            }
        }
        if (freshA && freshN && freshM)
            refreshAllForOut();
        else
        {
            if (freshA)
            {
                QVector<QVariant> aVector = mainPackage->getItemsByKey(allKeys,QString("药品代码"));
                QStringList agentL;
                for (QVariant &agent:aVector)
                    agentL<<agent.toString();
                agentLineDelegate->itemList = agentL.toSet().toList();
            }
            if (freshN)
            {
                QVector<QVariant> nVector = mainPackage->getItemsByKey(allKeys,QString("药品名"));
                QStringList nameL;
                for (QVariant &name:nVector)
                    nameL<<name.toString();
                nameComboDelegate->itemList = nameL.toSet().toList();
            }
            if (freshM)
            {
                QVector<QVariant> mVector = mainPackage->getItemsByKey(allKeys,QString("厂商"));
                QStringList madeL;
                for (QVariant &made:mVector)
                    madeL<<made.toString();
                madeComboDelegate->itemList = madeL.toSet().toList();
            }
        }
    }
}

void CBaseEditDlg::submitData()
{
    qDebug("This indicates that the virtual function is called abnormally");
}

bool CBaseEditDlg::checkOneRecord(const int idx)
{
    qDebug("This indicates that the virtual function is called abnormally and input Var: %d",idx);
    return true;
}

void CBaseEditDlg::recoveryBackground()
{
    for (int i=0;i<dataModel->rowCount();i++)
    {
        if (i%2 == 0)
        {
            for (int j=0;j<dataModel->columnCount();j++)
                dataModel->setData(dataModel->index(i,j),CBaseEditDlg::normal1Background,Qt::BackgroundColorRole);
        }
        else
        {
            for (int j=0;j<dataModel->columnCount();j++)
                dataModel->setData(dataModel->index(i,j),CBaseEditDlg::normal0Background,Qt::BackgroundColorRole);
        }
    }
}

CBaseEditDlg::~CBaseEditDlg()
{

}
