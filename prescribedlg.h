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
#include <QSqlRecord>

#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QHeaderView>

#include <QMessageBox>

extern QString mainName;
extern QStandardItemModel *prescribeData;
extern QBrush normalForeground;
extern QBrush errorForeground;
extern QBrush warningForeground;
extern QBrush normal0Background;
extern QBrush normal1Background;
extern QBrush errorBackground;

class prescribeDlg : public QDialog
{
    Q_OBJECT
public:
    prescribeDlg(QWidget *parent=0);
private:
    QGroupBox *actionBox;
    QPushButton *addB,*deleteB,*cancelB,*checkB,*submitB;

    QTableView *tabV;
    bool hangFlag;
    QSqlDatabase mainDB;
private:
    void setUI();
    void configModel();
    void recoveryBackground();
    void checkOneRecord(const int idx);
    bool isDiditStr(QString thisString);
private slots:
    void on_cancelB_clicked();
    void on_addB_clicked();
    void on_deleteB_clicked();
    void on_checkB_clicked();
};

#endif // PRESCRIBEDLG_H
