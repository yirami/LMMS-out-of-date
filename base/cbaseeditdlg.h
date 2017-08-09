#ifndef CBASEEDITDLG_H
#define CBASEEDITDLG_H

// UI
#include <QDialog>
#include <QGroupBox>
#include <QPushButton>
#include <QTableView>
#include <QHeaderView>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
// Data
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include "base/cdatabasepackage.h"
#include "base/clineeditdelegate.h"
#include "base/ccombodelegate.h"
#include "base/cspindelegate.h"
#include "base/cdoublespindelegate.h"
// Debug
#include <QMessageBox>
#include <QDebug>



class CBaseEditDlg : public QDialog
{
    Q_OBJECT
public:
    // 背景、前景色，用于标识检查结果
    static QBrush normalForeground;
    static QBrush errorForeground;
    static QBrush warningForeground;
    static QBrush normal0Background;
    static QBrush normal1Background;
    static QBrush errorBackground;
    static QRegExp allCapitalRE;
    static QRegExp allChineseRE;
    static QRegExp beginDigitRE;
    static CLineEditDelegate *agentLineDelegate;
    static CLineEditDelegate *nameLineDelegate;
    static CLineEditDelegate *madeLineDelegate;
    static CLineEditDelegate *specLineDelegate;
    static CComboDelegate *nameComboDelegate;
    static CComboDelegate *madeComboDelegate;
    static CSpinDelegate *ioDelegate;
    static CSpinDelegate *stockDelegate;
    static CDoubleSpinDelegate *priceDelegate;
    QStandardItemModel *dataModel;
    CDatabasePackage *mainPackage;
public:
    CBaseEditDlg(CDatabasePackage *dbPackage, QWidget *parent=0);
    ~CBaseEditDlg();
protected:
    QGroupBox *actionBox;
    QPushButton *addB,*deleteB,*cancelB,*checkB;
    QTableView *tabV;
protected:
    void setUI();
    void configModel();
    void setDelegateForIN();
    void setDelegateForOUT();
    void recoveryBackground();
    virtual bool checkOneRecord(const int idx);
    virtual void submitData();
protected slots:
    void on_addB_clicked();
    void on_deleteB_clicked();
    void on_cancelB_clicked();
    void on_checkB_clicked();
    virtual void refreshList(QModelIndex TL);

};

#endif // CBASEEDITDLG_H
