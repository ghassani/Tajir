#include "StoreCatalogList.h"
#include "StoreCatalogListItemListDelegate.h"
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

StoreCatalogList::StoreCatalogList( TajirStoreJson * store, ItemList * itemList, QWidget * parent ) :
    QTreeView( parent ),
    store( store ),
    itemList( itemList ),
    model( new StoreCatalogListModel( store, itemList ) )
{
    setModel( model );
    setItemDelegateForColumn( StoreCatalogListModelHeaderItemId, new StoreCatalogListItemListDelegate( itemList ) );
    
    resizeColumnToContents( StoreCatalogListModelHeaderName );
    resizeColumnToContents( StoreCatalogListModelHeaderHidden );
    setColumnWidth( StoreCatalogListModelHeaderItemId, 250 );

}

void StoreCatalogList::setStore( TajirStoreJson * newStore )
{
    store = newStore;

    model->setStore( store );

    resizeColumnToContents( StoreCatalogListModelHeaderName );
    resizeColumnToContents( StoreCatalogListModelHeaderHidden );
    setColumnWidth( StoreCatalogListModelHeaderItemId, 250 );
}

TajirStoreJson * StoreCatalogList::getStore() const
{
    return store;
}

void StoreCatalogList::contextMenuEvent( QContextMenuEvent * event )
{
    QMenu * menu        = new QMenu( this );
    QModelIndex index   = indexAt( event->pos() );

    menu->addAction( "Add Catalog", this, std::bind( &StoreCatalogList::addCatalog, this )  );

    if ( index.isValid() )
    {
        if ( index.parent().isValid() )
        {
            menu->addAction( "Add Item", this, std::bind( &StoreCatalogList::addItem, this, index.parent() )  );
        }
        else
        {
            menu->addAction( "Add Item", this, std::bind( &StoreCatalogList::addItem, this, index )  );
        }
    }

    menu->exec( event->globalPos() );

    delete menu;
}

void StoreCatalogList::addCatalog()
{
    TajirStoreCatalogJson * catalog = new TajirStoreCatalogJson( store );

    model->addCatalog( catalog );
}

void StoreCatalogList::addItem( const QModelIndex& catalogIndex )
{
    if ( !catalogIndex.isValid() )
    {
        return;
    }

    TajirStoreCatalogJson * catalog = static_cast<TajirStoreCatalogJson*>( catalogIndex.internalPointer() );

    if ( catalog )
    {
        model->addItem( catalog, new TajirStoreCatalogItemJson( catalog ) );
    }
}
