#include "cdetaildlg.h"

CDetailDlg::CDetailDlg(CDatabasePackage *dbPackage):CDirectEditDlg(dbPackage)
{
    setUI();

    connect(printB,SIGNAL(clicked()),this,SLOT(on_printB_clicked()));
}

void CDetailDlg::setUI()
{
    this->setFixedSize(1000,600);
    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::WindowMinimizeButtonHint;
    flags |=Qt::WindowCloseButtonHint;
    setWindowFlags(flags);

    actionBox = new QGroupBox(this);
    actionBox->setFixedWidth(220);
    actionBox->setTitle("操作");
    printB = new QPushButton(this);
    printB->setMinimumHeight(50);
    printB->setText("打印");
    QVBoxLayout *actionboxlayout = new QVBoxLayout(this);
    actionboxlayout->addStretch(1);
    actionboxlayout->addWidget(queryBox);
    actionboxlayout->addStretch(2);
    actionboxlayout->addWidget(printB);
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
    tabV->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tabV->show();

    QHBoxLayout *mlayout = new QHBoxLayout(this);
    mlayout->addWidget(tabV);
    mlayout->addWidget(actionBox);
    this->setLayout(mlayout);

    this->setWindowTitle(QString("清单"));
}

void CDetailDlg::on_printB_clicked()
{

}
