#ifndef MAINTAINDLG_H
#define MAINTAINDLG_H

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

class maintainDlg : public QDialog
{
    Q_OBJECT
public:
    maintainDlg(QWidget *parent=0);
    ~maintainDlg();
public:
    QTableView *tabV;
private:
    QGroupBox *actionBox;
    QPushButton *deleteB,*cancelB,*submitB;
};

#endif // MAINTAINDLG_H
