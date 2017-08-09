#ifndef CDIRECTEDITDLG_H
#define CDIRECTEDITDLG_H

// UI
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QGroupBox>
#include <QPushButton>
#include <QTableView>
#include <QHeaderView>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
// Database
#include <QSqlDatabase>
#include <QSqlTableModel>

#include "base/cdatabasepackage.h"

class CDirectEditDlg : public QDialog
{
    Q_OBJECT
public:
    QSqlTableModel *dataModel;
    CDatabasePackage *mainPackage;
public:
    CDirectEditDlg(CDatabasePackage *dbPackage, QWidget *parent=0);
protected:
    QGroupBox *queryBox, *nameBox, *madeBox, *stockBox, *priceBox;
    QLabel *nameLable, *madeLable, *stockLable, *stockDotLable, *priceLable, *priceDotLable;
    QLineEdit *nameEdit, *madeEdit, *stockLEdit, *stockHEdit, *priceLEdit, *priceHEdit;
    QPushButton *allB;
    QTableView *tabV;
protected:
    QSqlDatabase database;
    QRegExp allCapitalRE;
    QRegExp allChineseRE;
    QRegExp beginDigitRE;
    QRegExp nnintegerRE;
    QRegExp nnfloatRE;
    QPalette normalText;
    QPalette errorText;
protected:
    void showAllRecords();
    bool parseOne(QLineEdit *edit, QRegExp rx, int level=0);
    QString queryParse();
    void clearAllEdit();
    void recoveryTextColor();
    bool checkOneRecordFormat(const int idx);
protected slots:
    void on_allB_clicked();
    void queryChanged();
private:
    void setUI();
    void configModel();
};

#endif // CDIRECTEDITDLG_H
