#include "panel.h"
#include "ui_panel.h"

#include <QDebug>
#include <QBrush>

Panel::Panel(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Panel)
{
    ui->setupUi(this);

    dbPath = "./MedicineManagement.db";
    dbConnectName = "main";
    dbPackage = new CDatabasePackage(dbPath, dbConnectName);
}

Panel::~Panel()
{
    delete ui;
    delete dbPackage;
}

void Panel::on_storageButton_clicked()
{
    CStorageDlg stoDlg(dbPackage);
    if (stoDlg.exec() == QDialog::Accepted)
    {
        qDebug()<<QString("成功");
    }
}

void Panel::on_prescribeButton_clicked()
{
    CPrescribeDlg preDlg(dbPackage);
    preDlg.exec();
}

void Panel::on_maintainButton_clicked()
{
    CMaintainDlg mtDlg(dbPackage);
    mtDlg.exec();
}

void Panel::on_detailButton_clicked()
{
    CDetailDlg detDlg(dbPackage);
    detDlg.exec();
}
