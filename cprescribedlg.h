#ifndef CPRESCRIBEDLG_H
#define CPRESCRIBEDLG_H

#include "base/cbaseeditdlg.h"

class CPrescribeDlg : public CBaseEditDlg
{
    Q_OBJECT
public:
    CPrescribeDlg(CDatabasePackage *dbPackage);
private:
    virtual void submitData();
    virtual bool checkOneRecord(const int idx);
private slots:
    void refreshList(QModelIndex TL);
};

#endif // CPRESCRIBEDLG_H
