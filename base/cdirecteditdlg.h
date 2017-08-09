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
// base
#include "base/cdatabasepackage.h"
#include "base/clineeditdelegate.h"
#include "base/cspindelegate.h"
#include "base/cdoublespindelegate.h"

class CDirectEditDlg : public QDialog
{
    Q_OBJECT
public:
    QSqlTableModel *dataModel;
    CDatabasePackage *mainPackage;
    static QRegExp allCapitalRE;
    static QRegExp allChineseRE;
    static QRegExp beginDigitRE;
    static QRegExp nnintegerRE;
    static QRegExp nnfloatRE;
    static CLineEditDelegate *agentDelegate;
    static CLineEditDelegate *nameDelegate;
    static CLineEditDelegate *madeDelegate;
    static CLineEditDelegate *specDelegate;
    static CSpinDelegate *ioDelegate;
    static CSpinDelegate *stockDelegate;
    static CDoubleSpinDelegate *priceDelegate;
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
    QPalette normalText;
    QPalette errorText;
    QStringList nameList, madeList;
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
    void setAssistant();
};

#endif // CDIRECTEDITDLG_H
