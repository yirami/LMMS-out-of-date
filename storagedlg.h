#ifndef STORAGEDLG_H
#define STORAGEDLG_H

#include <QDialog>
#include <QGroupBox>
#include <QTableView>
#include <QPushButton>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>

extern QString mainName;

class storageDlg : public QDialog
{
    Q_OBJECT
public:
    storageDlg(QWidget *parent=0);
public:
    QString stoName = "stodb";
private:
    bool hangFlag;
    QSqlDatabase stoDB;
    QSqlTableModel *storageModel;
    QSqlDatabase mainDB;
private:
    QGroupBox *actionBox;
    QPushButton *addB,*deleteB,*cancelB,*checkB,*submitB;
    QTableView *tabV;
private:
    void setUI();
    QVector <int> checkRecord(const QSqlRecord &record);
private slots:
    void on_addB_clicked();
    void on_deleteB_clicked();
    void on_cancelB_clicked();
    void on_checkB_clicked();
};

#endif // STORAGEDLG_H
