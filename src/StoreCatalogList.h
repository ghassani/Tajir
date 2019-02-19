#pragma once

#include <QTreeView>
#include <QContextMenuEvent>
#include <QMenu>
#include "StoreCatalogListModel.h"
#include "TajirStoreJson.h"
#include "ItemList.h"

class StoreCatalogList : public QTreeView
{
    Q_OBJECT

    TajirStoreJson * store;
    ItemList * itemList;
    StoreCatalogListModel * model;


public:
    StoreCatalogList( TajirStoreJson * store, ItemList * itemList, QWidget * parent = nullptr );

    void setStore( TajirStoreJson * store );

    TajirStoreJson * getStore() const;

    void contextMenuEvent( QContextMenuEvent *event ) override;

    void addCatalog();

    void addItem( const QModelIndex& catalogIndex );
};
