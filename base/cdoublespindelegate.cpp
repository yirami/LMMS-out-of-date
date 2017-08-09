#include "cdoublespindelegate.h"

CDoubleSpinDelegate::CDoubleSpinDelegate(QObject *parent) : QItemDelegate(parent)
{
    Minimum = 0;
    Maximum = 100;
    Decimals = 4;
}

QWidget * CDoubleSpinDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/) const
{
    QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
    editor->setRange(Minimum,Maximum);
    editor->setDecimals(Decimals);
    editor->installEventFilter(const_cast<CDoubleSpinDelegate*>(this));
    return editor;
}

void CDoubleSpinDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    double value = index.model()->data(index).toDouble();
    QDoubleSpinBox *box = static_cast<QDoubleSpinBox*>(editor);
    box->setValue(value);
}

void CDoubleSpinDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDoubleSpinBox *box = static_cast<QDoubleSpinBox*>(editor);
    double value = box->value();
    model->setData(index, value);
}

void CDoubleSpinDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/*index*/)
{
    editor->setGeometry(option.rect);
}
