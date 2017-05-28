#ifndef PANEL_H
#define PANEL_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>

#include <QStandardItemModel>


#include "storagedlg.h"
#include "prescribedlg.h"
#include "maintaindlg.h"
#include "detailsdlg.h"

extern QStandardItemModel *prescribeData;

namespace Ui {
class Panel;
}

class Panel : public QMainWindow
{
    Q_OBJECT


public:
    explicit Panel(QWidget *parent = 0);
    ~Panel();
private slots:
    void on_storageButton_clicked();

    void on_prescribeButton_clicked();

    void on_maintainButton_clicked();

    void on_detailsButton_clicked();
private:
    void initStorageTable();
private:
    Ui::Panel *ui;
};

#endif // PANEL_H
