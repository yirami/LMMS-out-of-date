#ifndef PANEL_H
#define PANEL_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QLabel>
#include "base/cdatabasepackage.h"
#include "cstoragedlg.h"
#include "cprescribedlg.h"
#include "cmaintaindlg.h"
#include "cdetaildlg.h"

namespace Ui {
class Panel;
}

class Panel : public QMainWindow
{
    Q_OBJECT
public:
    QString dbPath;
    QString dbConnectName;
    CDatabasePackage *dbPackage;
public:
    explicit Panel(QWidget *parent = 0);
    ~Panel();
private slots:
    void on_storageButton_clicked();
    void on_prescribeButton_clicked();
    void on_maintainButton_clicked();
    void on_detailButton_clicked();
private:
    QStandardItemModel *initStorageTable();
    QStandardItemModel *initPrescribeTable();
private:
    Ui::Panel *ui;
private:
    void setCopyRight();
};

#endif // PANEL_H
