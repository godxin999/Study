#ifndef TCOMBOBOXDELEGATE_H
#define TCOMBOBOXDELEGATE_H

#include <QStyledItemDelegate>
#include <QStringList>
class TComboBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
private:
    QStringList m_itemList;//选项列表
    bool m_editable;//是否可编辑
public:
    explicit TComboBoxDelegate(QObject *parent = nullptr);
    void setItems(QStringList items,bool editable);//初始化设置列表内容，是否可编辑
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor,const QModelIndex &index)const;
    void setModelData(QWidget *editor,QAbstractItemModel *model,const QModelIndex &index)const;
    void updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option,const QModelIndex &index)const;
};

#endif // TCOMBOBOXDELEGATE_H
