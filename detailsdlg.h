#ifndef DETAILSDLG_H
#define DETAILSDLG_H

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

class detailsDlg : public QDialog
{
    Q_OBJECT
public:
    detailsDlg(QWidget *parent=0);
    ~detailsDlg();
public:
    QTableView *tabV;
private:
    QGroupBox *actionBox;
    QPushButton *deleteB,*cancelB,*submitB;
};

#endif // DETAILSDLG_H
