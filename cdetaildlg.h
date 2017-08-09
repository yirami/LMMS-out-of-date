#ifndef CDETAILDLG_H
#define CDETAILDLG_H

#include "base/cdirecteditdlg.h"

class CDetailDlg : public CDirectEditDlg
{
    Q_OBJECT
public:
    CDetailDlg(CDatabasePackage *dbPackage);
private:
    QGroupBox *actionBox;
    QPushButton *printB;
private:
    void setUI();
private slots:
    void on_printB_clicked();
};

#endif // CDETAILDLG_H
