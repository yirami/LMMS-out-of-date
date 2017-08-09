#include "cspindelegate.h"

CSpinDelegate::CSpinDelegate(QObject *parent) : QItemDelegate(parent)
{
    Minimum = 0;
    Maximum = 10000;
}

QWidget * CSpinDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/) const
{
    QSpinBox *editor = new QSpinBox(parent);
    editor->setRange(Minimum,Maximum);
    editor->installEventFilter(const_cast<CSpinDelegate*>(this));
    return editor;
}

void CSpinDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    int value = index.model()->data(index).toInt();
    QSpinBox *box = static_cast<QSpinBox*>(editor);
    box->setValue(value);
}

void CSpinDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QSpinBox *box = static_cast<QSpinBox*>(editor);
    int value = box->value();
    model->setData(index, value);
}

void CSpinDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/*index*/)
{
    editor->setGeometry(option.rect);
}
