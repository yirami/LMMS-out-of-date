#include "panel.h"
#include "ui_panel.h"

#include <QDebug>

Panel::Panel(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Panel)
{
    ui->setupUi(this);

    initStorageTable();


}

Panel::~Panel()
{
    delete ui;
}

void Panel::on_storageButton_clicked()
{
    storageDlg stoDlg;
    if (stoDlg.exec() == QDialog::Accepted)
    {
        qDebug()<<QString("成功");
    }
}


void Panel::on_prescribeButton_clicked()
{
    prescribeDlg preDlg;
    preDlg.exec();
}

void Panel::on_maintainButton_clicked()
{
    maintainDlg maiDlg;
    maiDlg.exec();
}

void Panel::on_detailsButton_clicked()
{
    detailsDlg detDlg;
    detDlg.exec();
}

void Panel::initStorageTable()
{
    prescribeData = new QStandardItemModel(0,7);
    prescribeData->setHeaderData(0,Qt::Horizontal,QString("药品代码"));
    prescribeData->setHeaderData(1,Qt::Horizontal,QString("药品名"));
    prescribeData->setHeaderData(2,Qt::Horizontal,QString("厂商"));
    prescribeData->setHeaderData(3,Qt::Horizontal,QString("规格"));
    prescribeData->setHeaderData(4,Qt::Horizontal,QString("数量"));
    prescribeData->setHeaderData(5,Qt::Horizontal,QString("库存"));
    prescribeData->setHeaderData(6,Qt::Horizontal,QString("单价"));

    QStandardItem *sName = new QStandardItem(QString("CBS"));
    sName->setFlags(Qt::ItemIsEnabled);
    sName->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QStandardItem *Name = new QStandardItem(QString("炒白术"));
    Name->setFlags(Qt::ItemIsEnabled);
    Name->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QStandardItem *MadeIn = new QStandardItem(QString("江阴天江药业有限公司"));
    MadeIn->setFlags(Qt::ItemIsEnabled);
    MadeIn->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    MadeIn->setForeground(QBrush(Qt::red));
    QStandardItem *Spec = new QStandardItem(QString("10g"));
    Spec->setFlags(Qt::ItemIsEnabled);

    QVariant Q(1);
    QStandardItem *Quant = new QStandardItem(Q.toString());
    Quant->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
    QStandardItem *Stock = new QStandardItem(Q.toString());
    Stock->setFlags(Qt::ItemIsEnabled);
    QStandardItem *Price = new QStandardItem(Q.toString());
    Price->setFlags(Qt::ItemIsEnabled);


    prescribeData->setItem(0,0,sName);
    prescribeData->setItem(0,1,Name);
    prescribeData->setItem(0,2,MadeIn);
    prescribeData->setItem(0,3,Spec);
    prescribeData->setItem(0,4,Quant);
    prescribeData->setItem(0,5,Stock);
    prescribeData->setItem(0,6,Price);
}
