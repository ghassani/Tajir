#pragma once

#include <QTreeView>
#include "ItemListModel.h"

class ItemList : public QTreeView
{
    Q_OBJECT

    ItemListModel * model;

public:
    ItemList( QWidget * parent = nullptr );

    void loadJson( const QString& filepath );

    void contextMenuEvent( QContextMenuEvent *event ) override;

    void addItem();

    void addBundleItem( const QModelIndex& index );

    void deleteItem( const QModelIndex& index );

    TajirItemJson * getItemById( int id );

    const QList<TajirItemJson *>& getItems();
};
