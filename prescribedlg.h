#ifndef PRESCRIBEDLG_H
#define PRESCRIBEDLG_H

#include <QDialog>
#include <QGroupBox>
#include <QTableView>
#include <QPushButton>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QSqlDatabase>
#include <QSqlQuery>

#include <QStandardItemModel>
#include <QHeaderView>

extern QString mainName;
extern QStandardItemModel *prescribeData;

class prescribeDlg : public QDialog
{
    Q_OBJECT
public:
    prescribeDlg(QWidget *parent=0);
private:
    bool hangFlag;
    QGroupBox *actionBox;
    QPushButton *addB,*deleteB,*cancelB,*checkB,*submitB;
    QTableView *tabV;
    QSqlDatabase mainDB;
private:
    void setUI();
    void configModel();
private slots:
    void on_cancelB_clicked();
    void on_addB_clicked();
    void on_deleteB_clicked();
    void on_checkB_clicked();
};

#endif // PRESCRIBEDLG_H
