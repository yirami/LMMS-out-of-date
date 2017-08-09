#ifndef CDOUBLESPINDELEGATE_H
#define CDOUBLESPINDELEGATE_H
#include <QItemDelegate>
#include <QDoubleSpinBox>

class CDoubleSpinDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    int Minimum, Maximum, Decimals;
public:
    CDoubleSpinDelegate(QObject *parent=0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index);
};

#endif // CDOUBLESPINDELEGATE_H
