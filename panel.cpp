#include "panel.h"
#include "ui_panel.h"

#include <QDebug>
#include <QBrush>

Panel::Panel(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Panel)
{
    ui->setupUi(this);
    setCopyRight();
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

void Panel::setCopyRight()
{
    this->setFixedSize(631,424);
    ui->statusBar->setStyleSheet(QString("QStatusBar::item{border:0px}"));
    ui->statusBar->setSizeGripEnabled(false);
    QLabel *right = new QLabel(QString("Copyright 2017 Yirami. All rights reserved."),this);
    QFont copyFont("Times", 9, QFont::Normal);
    right->setFont(copyFont);
    ui->statusBar->addPermanentWidget(right);
    ui->statusBar->showMessage(QString("药品管理系统启动中..."), 1000);
}
