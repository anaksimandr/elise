#include "checkboxdelegate.h"


CheckBoxDelegate::CheckBoxDelegate(QObject* parent)
	: QItemDelegate(parent)
{
}

QSize CheckBoxDelegate::sizeHint(const QStyleOptionViewItem&, const QModelIndex&) const
{
	return QSize(200,20);
}

/*QWidget* CheckBoxDelegate::createEditor(QWidget* parent,
	const QStyleOptionViewItem&,
	const QModelIndex&) const
{
	QCheckBox* editor = new QCheckBox(parent);

	return editor;
}

void CheckBoxDelegate::setEditorData(QWidget* editor,
									const QModelIndex& index) const
{
	QCheckBox *spinBox = static_cast<QCheckBox*>(editor);
	int value = index.model()->data(index, Qt::EditRole).toInt();
	if (value == 0)
		spinBox->setCheckState(Qt::Unchecked);
	else
		spinBox->setCheckState(Qt::Checked);
}

void CheckBoxDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
								   const QModelIndex& index) const
{
	QCheckBox *spinBox = static_cast<QCheckBox*>(editor);
	//spinBox->interpretText();
	int value = spinBox->checkState();

	model->setData(index, value, Qt::EditRole);
}

void CheckBoxDelegate::updateEditorGeometry(QWidget* editor,
	const QStyleOptionViewItem& option, const QModelIndex&) const
{
	editor->setGeometry(option.rect);
}*/
