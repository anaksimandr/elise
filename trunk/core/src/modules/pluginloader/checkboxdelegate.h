#ifndef ELISE_CORE_MODULES_PLUGINLOADER_CHECKBOXDELEGATE_H_
#define ELISE_CORE_MODULES_PLUGINLOADER_CHECKBOXDELEGATE_H_

#include <QtWidgets/QtWidgets>

class CheckBoxDelegate : public QStyledItemDelegate
{
public:
	CheckBoxDelegate(QObject* parent);

	QSize sizeHint(const QStyleOptionViewItem&, const QModelIndex&) const;
};

/*
class CheckBoxDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	CheckBoxDelegate(QObject *parent = 0);

	QSize sizeHint(const QStyleOptionViewItem&, const QModelIndex&) const;

	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option,
						  const QModelIndex& index) const;

	void setEditorData(QWidget* editor, const QModelIndex& index) const;
	void setModelData(QWidget* editor, QAbstractItemModel* model,
					  const QModelIndex& index) const;

	void updateEditorGeometry(QWidget* editor,
							  const QStyleOptionViewItem& option, const QModelIndex& index) const;
};*/

#endif // ELISE_CORE_MODULES_PLUGINLOADER_CHECKBOXDELEGATE_H_
