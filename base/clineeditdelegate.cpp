#include "clineeditdelegate.h"

CLineEditDelegate::CLineEditDelegate(QRegExp RE, bool use, QObject *parent) : QItemDelegate(parent), inputRE(RE), useCompleter(use)
{
    itemList.clear();
}

QWidget * CLineEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/) const
{
    QLineEdit *editor = new QLineEdit(parent);
    QRegExpValidator *inputValidate = new QRegExpValidator(inputRE, parent);
    editor->setValidator(inputValidate);
    if (useCompleter)
    {
        QCompleter *inputCompleter = new QCompleter(itemList, parent);
        editor->setCompleter(inputCompleter);
    }
    editor->installEventFilter(const_cast<CLineEditDelegate*>(this));
    return editor;
}

void CLineEditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString str = index.model()->data(index).toString();
    QLineEdit *box = static_cast<QLineEdit*>(editor);
    box->setText(str);
}

void CLineEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *box = static_cast<QLineEdit*>(editor);
    QString str = box->text();
    model->setData(index, str);
}

void CLineEditDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/*index*/)
{
    editor->setGeometry(option.rect);
}

void CLineEditDelegate::completerStatus(bool use)
{
    if (use)
        useCompleter = true;
    else
        useCompleter = false;
}
