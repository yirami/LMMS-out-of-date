#ifndef CSTORAGEDLG_H
#define CSTORAGEDLG_H

#include "base/cbaseeditdlg.h"

class CStorageDlg : public CBaseEditDlg
{
    Q_OBJECT
public:
    CStorageDlg(CDatabasePackage *dbPackage);
private:
    virtual void submitData();
    virtual bool checkOneRecord(const int idx);
private slots:
    void refreshList(QModelIndex TL);

};

#endif // CSTORAGEDLG_H
