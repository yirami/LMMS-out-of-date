#include "detailsdlg.h"

detailsDlg::detailsDlg(QWidget *parent)
    : QDialog(parent)
{
    this->setFixedSize(1000,600);
    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::WindowMinimizeButtonHint;
    flags |=Qt::WindowCloseButtonHint;
    setWindowFlags(flags);

    QGroupBox *actionBox = new QGroupBox(this);
    actionBox->setFixedWidth(150);
    actionBox->setTitle("操作");
    deleteB = new QPushButton(this);
    deleteB->setMinimumHeight(50);
    deleteB->setText("删除");
    cancelB = new QPushButton(this);
    cancelB->setMinimumHeight(50);
    cancelB->setText("取消");
    submitB = new QPushButton(this);
    submitB->setMinimumHeight(50);
    submitB->setText("提交");
    tabV = new QTableView(this);

    QVBoxLayout *actboxlayout = new QVBoxLayout(this);
    actboxlayout->addWidget(deleteB);
    actboxlayout->addStretch(7);
    actboxlayout->addWidget(cancelB);
    actboxlayout->addStretch(2);
    actboxlayout->addWidget(submitB);
    actboxlayout->addStretch(1);

    actionBox->setLayout(actboxlayout);

    QHBoxLayout *mlayout = new QHBoxLayout(this);
    mlayout->addWidget(tabV);
    mlayout->addWidget(actionBox);

    this->setLayout(mlayout);
    this->setWindowTitle(QString("库存清单"));
    // 数据部分
    // 主数据库
    QSqlDatabase mainDB = QSqlDatabase::database(mainName);
    // 功能临时数据库

    QSqlTableModel *detailsModel = new QSqlTableModel(this,mainDB);
    detailsModel->setTable("Medicine");
    detailsModel->select();//选取表的所有行
    tabV->setModel(detailsModel);
    tabV->show();
}
detailsDlg::~detailsDlg()
{

}
