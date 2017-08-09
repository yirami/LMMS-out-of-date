#ifndef CDETAILDLG_H
#define CDETAILDLG_H
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QPrinter>
#include <QPainter>
#include <QPointF>
#include <QDateTime>
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
    QString outFileName;
    QPrinter textPrinter;
    QPainter textPainter;
private:
    void setUI();
    void printConfig();
    QString makeTime();
    QString formatOneRecord(int sel);
    QStringList prepareTextList();
    void writeText(QPointF &startP, QStringList &textList);
    QString alighChinese(const QString &raw, int fieldWidth=0);
private slots:
    void on_printB_clicked();
};

#endif // CDETAILDLG_H
