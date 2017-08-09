#ifndef CMAINTAINDLG_H
#define CMAINTAINDLG_H

#include "base/cdirecteditdlg.h"

#include <QMessageBox>
#include <QSqlError>
#include <QDebug>


class CMaintainDlg : public CDirectEditDlg
{
    Q_OBJECT
public:
    CMaintainDlg(CDatabasePackage *dbPackage);
private:
    QGroupBox *actionBox;
    QPushButton *deleteB, *submitB;
private:
    void setUI();
private slots:
    void on_deleteB_clicked();
    void on_submitB_clicked();

};

#endif // CMAINTAINDLG_H
