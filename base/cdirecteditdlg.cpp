#include "cdirecteditdlg.h"
QRegExp CDirectEditDlg::allCapitalRE("^[A-Z]+$");
QRegExp CDirectEditDlg::allChineseRE("^[\u4e00-\u9fa5]+$");
QRegExp CDirectEditDlg::beginDigitRE("^[0-9]*[1-9][0-9]*[A-Za-z\u4e00-\u9fa5]+$");
QRegExp CDirectEditDlg::nnintegerRE = QRegExp("^\\d+$");
QRegExp CDirectEditDlg::nnfloatRE = QRegExp("^\\d+(\\.\\d+)?$");
CLineEditDelegate *CDirectEditDlg::agentDelegate = new CLineEditDelegate(CDirectEditDlg::allCapitalRE,true);
CLineEditDelegate *CDirectEditDlg::nameDelegate = new CLineEditDelegate(CDirectEditDlg::allChineseRE,true);
CLineEditDelegate *CDirectEditDlg::madeDelegate = new CLineEditDelegate(CDirectEditDlg::allChineseRE,true);
CLineEditDelegate *CDirectEditDlg::specDelegate = new CLineEditDelegate(CDirectEditDlg::beginDigitRE);
CSpinDelegate *CDirectEditDlg::ioDelegate = new CSpinDelegate;
CSpinDelegate *CDirectEditDlg::stockDelegate = new CSpinDelegate;
CDoubleSpinDelegate *CDirectEditDlg::priceDelegate = new CDoubleSpinDelegate;


CDirectEditDlg::CDirectEditDlg(CDatabasePackage *dbPackage, QWidget *parent):QDialog(parent), mainPackage(dbPackage)
{
    database = mainPackage->database;
    if (!mainPackage->database.isOpen())
        database.open();
    dataModel = new QSqlTableModel(this,mainPackage->database);
    dataModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    setUI();
    setAssistant();
    showAllRecords();

    normalText.setBrush(QPalette::Text, QBrush(Qt::black));
    errorText.setBrush(QPalette::Text, QBrush(Qt::red));

    connect(allB,SIGNAL(clicked()),this,SLOT(on_allB_clicked()));
    connect(nameEdit,SIGNAL(textChanged(QString)),this,SLOT(queryChanged()));
    connect(madeEdit,SIGNAL(textChanged(QString)),this,SLOT(queryChanged()));
    connect(stockLEdit,SIGNAL(textChanged(QString)),this,SLOT(queryChanged()));
    connect(stockHEdit,SIGNAL(textChanged(QString)),this,SLOT(queryChanged()));
    connect(priceLEdit,SIGNAL(textChanged(QString)),this,SLOT(queryChanged()));
    connect(priceHEdit,SIGNAL(textChanged(QString)),this,SLOT(queryChanged()));
}

void CDirectEditDlg::setUI()
{
    nameBox = new QGroupBox(this);
    nameBox->setFixedWidth(180);
    nameLable = new QLabel(this);
    nameLable->setFixedWidth(30);
    nameLable->setText("药名：");
    nameEdit = new QLineEdit(this);
    nameEdit->setFixedWidth(130);
    QHBoxLayout *namelayout = new QHBoxLayout(this);
    namelayout->addStretch(2);
    namelayout->addWidget(nameLable);
    namelayout->addStretch(1);
    namelayout->addWidget(nameEdit);
    namelayout->addStretch(2);
    nameBox->setLayout(namelayout);

    madeBox = new QGroupBox(this);
    madeBox->setFixedWidth(180);
    madeLable = new QLabel(this);
    madeLable->setFixedWidth(30);
    madeLable->setText("厂商：");
    madeEdit = new QLineEdit(this);
    madeEdit->setFixedWidth(130);
    QHBoxLayout *madelayout = new QHBoxLayout(this);
    madelayout->addStretch(1);
    madelayout->addWidget(madeLable);
    madelayout->addStretch(1);
    madelayout->addWidget(madeEdit);
    madelayout->addStretch(1);
    madeBox->setLayout(madelayout);

    stockBox = new QGroupBox(this);
    stockBox->setFixedWidth(180);
    stockLable = new QLabel(this);
    stockLable->setFixedWidth(30);
    stockLable->setText("库存：");
    stockLEdit = new QLineEdit(this);
    stockLEdit->setFixedWidth(50);
    stockDotLable = new QLabel(this);
    stockDotLable->setFixedWidth(15);
    stockDotLable->setText("->");
    stockHEdit = new QLineEdit(this);
    stockHEdit->setFixedWidth(50);
    QHBoxLayout *stocklayout = new QHBoxLayout(this);
    stocklayout->addStretch(1);
    stocklayout->addWidget(stockLable);
    stocklayout->addStretch(2);
    stocklayout->addWidget(stockLEdit);
    stocklayout->addStretch(1);
    stocklayout->addWidget(stockDotLable);
    stocklayout->addStretch(1);
    stocklayout->addWidget(stockHEdit);
    stocklayout->addStretch(2);
    stockBox->setLayout(stocklayout);

    priceBox = new QGroupBox(this);
    priceBox->setFixedWidth(180);
    priceLable = new QLabel(this);
    priceLable->setFixedWidth(30);
    priceLable->setText("单价：");
    priceLEdit = new QLineEdit(this);
    priceLEdit->setFixedWidth(50);
    priceDotLable = new QLabel(this);
    priceDotLable->setFixedWidth(15);
    priceDotLable->setText("->");
    priceHEdit = new QLineEdit(this);
    priceHEdit->setFixedWidth(50);
    QHBoxLayout *pricelayout = new QHBoxLayout(this);
    pricelayout->addStretch(1);
    pricelayout->addWidget(priceLable);
    pricelayout->addStretch(2);
    pricelayout->addWidget(priceLEdit);
    pricelayout->addStretch(1);
    pricelayout->addWidget(priceDotLable);
    pricelayout->addStretch(2);
    pricelayout->addWidget(priceHEdit);
    pricelayout->addStretch(1);
    priceBox->setLayout(pricelayout);

    allB = new QPushButton(this);
    allB->setMinimumHeight(50);
    allB->setText("所有");

    queryBox = new QGroupBox(this);
    queryBox->setFixedWidth(200);
    queryBox->setFixedHeight(350);
    queryBox->setTitle("条件查询");
    QVBoxLayout *queryboxlayout = new QVBoxLayout(this);
    queryboxlayout->addStretch(1);
    queryboxlayout->addWidget(nameBox);
    queryboxlayout->addStretch(1);
    queryboxlayout->addWidget(madeBox);
    queryboxlayout->addStretch(1);
    queryboxlayout->addWidget(stockBox);
    queryboxlayout->addStretch(1);
    queryboxlayout->addWidget(priceBox);
    queryboxlayout->addStretch(2);
    queryboxlayout->addWidget(allB);
    queryboxlayout->addStretch(1);
    queryBox->setLayout(queryboxlayout);

    clearAllEdit();
}

void CDirectEditDlg::on_allB_clicked()
{
    showAllRecords();
}

void CDirectEditDlg::queryChanged()
{
    recoveryTextColor();
    QString queryStatement = queryParse();
    if (!queryStatement.isEmpty())
    {
        dataModel->setFilter(queryStatement);
        if (!dataModel->select())
        {
            qDebug()<<QString("查询失败！");
        }
    }
}

void CDirectEditDlg::showAllRecords()
{
    clearAllEdit();
    dataModel->setTable("Medicine");
    dataModel->select();

}

bool CDirectEditDlg::parseOne(QLineEdit *edit, QRegExp rx, int level)
{
    bool isNeed = false;
    if (!edit->text().isEmpty())
    {
        QRegExpValidator validator(rx);
        int pos = 0;
        Q_ASSERT(level==0 || level==1);
        QString text = edit->text();
        if (level==0)
        {
            if (validator.validate(text,pos)==QValidator::Invalid)
                edit->setPalette(CDirectEditDlg::errorText);
            else
                isNeed = true;
        }
        else
        {
            if (validator.validate(text,pos)==QValidator::Acceptable)
                isNeed = true;
            else
                edit->setPalette(CDirectEditDlg::errorText);
        }
    }
    return isNeed;
}

QString CDirectEditDlg::queryParse()
{
    QString queryStatement;
    QVector<QString> cache;
    if (parseOne(nameEdit,allChineseRE))
        cache.append(QString("药品名 LIKE '%%1%'").arg(nameEdit->text()));
    if (parseOne(madeEdit,allChineseRE))
        cache.append(QString("厂商 LIKE '%%1%'").arg(madeEdit->text()));
    bool ok;
    bool low = parseOne(stockLEdit,nnintegerRE,1);
    bool high = parseOne(stockHEdit,nnintegerRE,1);
    int lowNum = stockLEdit->text().toInt(&ok);
    if (low && !ok)
    {
        stockLEdit->setPalette(CDirectEditDlg::errorText);
        low = false;
    }
    int highNum = stockHEdit->text().toInt(&ok);
    if (high && !ok)
    {
        stockHEdit->setPalette(CDirectEditDlg::errorText);
        high = false;
    }
    if (low)
        cache.append(QString("库存 >= '%1'").arg(lowNum));
    if (high)
        cache.append(QString("库存 <= '%1'").arg(highNum));
    low = parseOne(priceLEdit,nnfloatRE,1);
    high = parseOne(priceHEdit,nnfloatRE,1);
    float lowF = priceLEdit->text().toFloat(&ok);
    if (low && !ok)
    {
        priceLEdit->setPalette(CDirectEditDlg::errorText);
        low = false;
    }
    float highF = priceHEdit->text().toFloat(&ok);
    if (high && !ok)
    {
        priceHEdit->setPalette(CDirectEditDlg::errorText);
        high = false;
    }
    if (low)
        cache.append(QString("单价 >= '%1'").arg(lowF));
    if (high)
        cache.append(QString("单价 <= '%1'").arg(highF));

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

void CDirectEditDlg::clearAllEdit()
{
    recoveryTextColor();
    nameEdit->setText("");
    madeEdit->setText("");
    stockLEdit->setText("");
    stockHEdit->setText("");
    priceLEdit->setText("");
    priceHEdit->setText("");
}

void CDirectEditDlg::recoveryTextColor()
{
    nameEdit->setPalette(CDirectEditDlg::normalText);
    madeEdit->setPalette(CDirectEditDlg::normalText);
    stockLEdit->setPalette(CDirectEditDlg::normalText);
    stockHEdit->setPalette(CDirectEditDlg::normalText);
    priceLEdit->setPalette(CDirectEditDlg::normalText);
    priceHEdit->setPalette(CDirectEditDlg::normalText);
}

void CDirectEditDlg::setAssistant()
{
    QRegExpValidator *vali1 = new QRegExpValidator(allChineseRE,this);
    QIntValidator *vali2 = new QIntValidator(0,10240,this);
    QDoubleValidator *vali3 = new QDoubleValidator(0,100,4,this);
    nameEdit->setValidator(vali1);
    madeEdit->setValidator(vali1);
    stockLEdit->setValidator(vali2);
    stockHEdit->setValidator(vali2);
    priceLEdit->setValidator(vali3);
    priceHEdit->setValidator(vali3);

    agentDelegate->itemList =  mainPackage->getAllAgentName().toList();
    QVector<int> keys = mainPackage->getAllKey();
    QVector<QVariant> nameVector = mainPackage->getItemsByKey(keys,QString("药品名"));
    for (QVariant &name:nameVector)
        nameList<<name.toString();
    QVector<QVariant> madeVector = mainPackage->getItemsByKey(keys,QString("厂商"));
    for (QVariant &made:madeVector)
        madeList<<made.toString();
    QCompleter *nameCompleter = new QCompleter(nameList,this);
    QCompleter *madeCompleter = new QCompleter(madeList,this);
    nameEdit->setCompleter(nameCompleter);
    madeEdit->setCompleter(madeCompleter);
}
