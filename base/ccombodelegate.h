#ifndef CCOMBODELEGATE_H
#define CCOMBODELEGATE_H
#include <QItemDelegate>
#include <QComboBox>

class CComboDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    QStringList itemList;
public:
    CComboDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex	&index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const  QModelIndex &index) const;
};

#endif // CCOMBODELEGATE_H
