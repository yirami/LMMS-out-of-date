#ifndef CLINEEDITDELEGATE_H
#define CLINEEDITDELEGATE_H
#include <QItemDelegate>
#include <QLineEdit>
#include <QRegExpValidator>
#include <QCompleter>

class CLineEditDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    QRegExp inputRE;
    QStringList itemList;
public:
    CLineEditDelegate(QRegExp RE, bool use=false, QObject *parent=0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index);
    void completerStatus(bool use);
private:
    bool useCompleter;
};

#endif // CLINEEDITDELEGATE_H
