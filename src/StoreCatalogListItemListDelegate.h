#pragma once

#include <QStyledItemDelegate>
#include "ItemList.h"

class StoreCatalogListItemListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
    ItemList * itemList;
public:
    StoreCatalogListItemListDelegate( ItemList * itemList, QObject *parent = nullptr );
    
    ~StoreCatalogListItemListDelegate();

    QWidget *createEditor( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const override;
    
    void setEditorData( QWidget *editor, const QModelIndex &index ) const override;
    
    void setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const override;
};
