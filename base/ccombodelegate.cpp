#include "ccombodelegate.h"

CComboDelegate::CComboDelegate(QObject *parent) : QItemDelegate(parent)
{
    itemList<<QString("占位标志");
}

QWidget *CComboDelegate::createEditor(QWidget *parent,const QStyleOptionViewItem &/*option*/,const QModelIndex &/*index*/) const
{
    QComboBox *editor = new QComboBox(parent);
    editor->addItems(itemList);
    editor->installEventFilter(const_cast<CComboDelegate*>(this));
    return editor;
}

void CComboDelegate::setEditorData(QWidget *editor,const QModelIndex &index) const
{
    QString str =index.model()->data(index).toString();

    QComboBox *box = static_cast<QComboBox*>(editor);
    int i = box->findText(str);
    box->setCurrentIndex(i);
}

void CComboDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *box = static_cast<QComboBox*>(editor);
    QString str = box->currentText();
    model->setData(index,str);
}

void CComboDelegate::updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    editor->setGeometry(option.rect);
}
