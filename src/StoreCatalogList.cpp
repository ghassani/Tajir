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
    setSelectionMode(QAbstractItemView::SelectionMode::MultiSelection);
    setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
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
    QModelIndexList list = selectionModel()->selectedRows();

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

    if ( list.count() > 1 )
    {
        int itemcount = 0;
        int catalogcount = 0;

        foreach( const QModelIndex& selected, list )
        {
            if ( !model->isCatalog( selected ) )
            {
                itemcount++;
            }
            else
            {
                catalogcount++;
            }
        }

        if ( catalogcount > 0 )
        {
            menu->addAction( QString("Delete %1 Catalogs").arg( catalogcount ), this, std::bind( &StoreCatalogList::deleteSelectedCatalogs, this, list )  );
            menu->addAction( QString("Set %1 Catalogs Hidden").arg( catalogcount ), this, std::bind( &StoreCatalogList::setSelectedCatalogsHiddenState, this, list, true )  );
            menu->addAction( QString("Set %1 Catalogs Visible").arg( catalogcount ), this, std::bind( &StoreCatalogList::setSelectedCatalogsHiddenState, this, list, false )  );
        }

        if ( itemcount > 0 )
        {
            menu->addAction( QString("Delete %1 Items").arg( itemcount ), this, std::bind( &StoreCatalogList::deleteSelectedItems, this, list )  );
            menu->addAction( QString("Set %1 Items As Buyable").arg( itemcount ), this, std::bind( &StoreCatalogList::setSelectedItemsBuyStatus, this, list, true )  );
            menu->addAction( QString("Set %1 Items As Not Buyable").arg( itemcount ), this, std::bind( &StoreCatalogList::setSelectedItemsBuyStatus, this, list, false )  );
            menu->addAction( QString("Set %1 Items As Sellable").arg( itemcount ), this, std::bind( &StoreCatalogList::setSelectedItemsSellStatus, this, list, true )  );
            menu->addAction( QString("Set %1 Items As Not Sellable").arg( itemcount ), this, std::bind( &StoreCatalogList::setSelectedItemsSellStatus, this, list, false )  );
        }
    }

    menu->exec( event->globalPos() );

    delete menu;
}

void StoreCatalogList::deleteSelectedItems( const QModelIndexList& selected )
{
    foreach( const QModelIndex& index, selected )
    {
        if ( !model->isCatalog( index ) )
        {
            model->removeItem( index );
        }
    }
}

void StoreCatalogList::deleteSelectedCatalogs( const QModelIndexList& selected )
{
    foreach( const QModelIndex& index, selected )
    {
        if ( model->isCatalog( index ) )
        {
            model->removeCatalog( index );
        }
    }
}

void StoreCatalogList::setSelectedCatalogsHiddenState( const QModelIndexList& selected, bool state )
{
    foreach( const QModelIndex& index, selected )
    {
        if ( model->isCatalog( index ) )
        {
            TajirStoreCatalogJson * catalog = model->getCatalog( index );

            if ( catalog )
            {
                catalog->hidden = state;
            }
        }
    }
}

void StoreCatalogList::setSelectedItemsBuyStatus( const QModelIndexList& selected, bool state )
{
    foreach( const QModelIndex& index, selected )
    {
        if ( !model->isCatalog( index ) )
        {
            TajirStoreCatalogItemJson * item = model->getItem( index );

            if ( item )
            {
                item->can_buy = state;
            }
        }
    }
}

void StoreCatalogList::setSelectedItemsSellStatus( const QModelIndexList& selected, bool state )
{
    foreach( const QModelIndex& index, selected )
    {
        if ( !model->isCatalog( index ) )
        {
            TajirStoreCatalogItemJson * item = model->getItem( index );

            if ( item )
            {
                item->can_sell = state;
            }
        }
    }
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
