#ifndef CSPINDELEGATE_H
#define CSPINDELEGATE_H

#include <QItemDelegate>
#include <QSpinBox>

class CSpinDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    int Minimum, Maximum;
public:
    CSpinDelegate(QObject *parent=0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index);
};

#endif // CSPINDELEGATE_H
