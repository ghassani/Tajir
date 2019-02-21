#include "StoreCatalogListModel.h"

StoreCatalogListModel::StoreCatalogListModel( TajirStoreJson * store, ItemList * itemList, QObject *parent )
    : QAbstractItemModel( parent ),
      store( store ),
      itemList( itemList )
{

}

StoreCatalogListModel::~StoreCatalogListModel()
{

}

int StoreCatalogListModel::columnCount( const QModelIndex & /* parent */ ) const
{
    return 7;
}

QVariant StoreCatalogListModel::data( const QModelIndex &index, int role ) const
{
    TajirItemJson * refItem;

    if ( !index.isValid() )
    {
        return QVariant();
    }
    else if ( index.parent().isValid() )
    {
        TajirStoreCatalogItemJson * item = getItem( index );

        if ( item )
        {

            if ( role == Qt::CheckStateRole )
            {
                switch( index.column() )
                {
                    case StoreCatalogListModelHeaderCanSell:
                        return item->can_sell ? Qt::Checked : Qt::Unchecked;;
                    case StoreCatalogListModelHeaderCanBuy:
                        return item->can_buy ? Qt::Checked : Qt::Unchecked;;
                }
            }

            if ( role == Qt::DisplayRole )
            {
                switch( index.column() )
                {
                    case StoreCatalogListModelHeaderItemId:
                        refItem = itemList->getItemById( item->item_id );

                        if ( refItem )
                        {
                            if ( refItem->object_name.length() )
                            {
                                return QString( "[%1] %2" ).arg( item->item_id ).arg( refItem->object_name );
                            }
                            else if ( refItem->display_name.length() )
                            {
                                return QString( "[%1] %2" ).arg( item->item_id ).arg( refItem->display_name );
                            }

                            return QString( "[%1] MISCONFIGURATION!" ).arg( item->item_id );
                        }

                        return QString( "[%1] MISCONFIGURATION!" ).arg( item->item_id );
                    case StoreCatalogListModelHeaderCanSell:
                        return item->can_sell;
                    case StoreCatalogListModelHeaderCanBuy:
                        return item->can_buy;
                    case StoreCatalogListModelHeaderQuantity:
                        return item->quantity;
                    case StoreCatalogListModelHeaderRestockQuantity:
                        return item->restock_quantity;
                }
            }

            if ( role == Qt::EditRole )
            {
                switch( index.column() )
                {
                    case StoreCatalogListModelHeaderCanSell:
                        return item->can_sell;
                    case StoreCatalogListModelHeaderCanBuy:
                        return item->can_buy;
                    case StoreCatalogListModelHeaderQuantity:
                        return item->quantity;
                    case StoreCatalogListModelHeaderRestockQuantity:
                        return item->restock_quantity;
                }
            }
        }

        return QVariant();
    }
    else
    {

        TajirStoreCatalogJson * catalog = getCatalog( index );

        if ( catalog )
        {
            if ( role == Qt::CheckStateRole )
            {
                switch( index.column() )
                {
                    case StoreCatalogListModelHeaderHidden: 	return catalog->hidden ? Qt::Checked : Qt::Unchecked;
                }

            }

            if ( role == Qt::DisplayRole )
            {
                switch( index.column() )
                {
                    case 0:
                        return catalog->name;
                    case StoreCatalogListModelHeaderHidden:
                        return catalog->hidden;
                }
            }

            if ( role == Qt::EditRole )
            {
                switch( index.column() )
                {
                    case StoreCatalogListModelHeaderName:
                        return catalog->name;
                    case StoreCatalogListModelHeaderHidden:
                        return catalog->hidden;
                }
            }
        }
    }

    return QVariant();
}

Qt::ItemFlags StoreCatalogListModel::flags( const QModelIndex &index ) const
{
    if ( !index.isValid() )
    {
        return 0;
    }

   QFlags<Qt::ItemFlag> flags = Qt::ItemIsEnabled | QAbstractItemModel::flags( index );

   if ( isCatalog( index ) )
   {
       TajirStoreCatalogJson * catalog = getCatalog( index );

       if ( catalog )
       {
           switch( index.column() )
           {
               case StoreCatalogListModelHeaderName:
                   flags |= Qt::ItemIsEditable;
                   break;
               case StoreCatalogListModelHeaderHidden:
                   flags |= Qt::ItemIsUserCheckable;
                   break;
           }
       }
   }
   else
   {
       TajirStoreCatalogItemJson * item = getItem( index );

       if ( item )
       {
           switch( index.column() )
           {
                case StoreCatalogListModelHeaderCanSell:
                case StoreCatalogListModelHeaderCanBuy:
                   flags |= Qt::ItemIsUserCheckable;
                   break;
                case StoreCatalogListModelHeaderItemId:
                case StoreCatalogListModelHeaderQuantity:
                case StoreCatalogListModelHeaderRestockQuantity:
                   flags |= Qt::ItemIsEditable;
                   break;
           }
       }
   }

   return flags;
}

TajirStoreCatalogJson * StoreCatalogListModel::getCatalog(const QModelIndex &index ) const
{
    if ( index.isValid() )
    {
        TajirStoreCatalogJson * item = static_cast< TajirStoreCatalogJson * >( index.internalPointer() );

        if ( item )
        {
            return item;
        }
    }

    return nullptr;
}

TajirStoreCatalogItemJson * StoreCatalogListModel::getItem(const QModelIndex &index ) const
{
    if ( index.isValid() )
    {
        TajirStoreCatalogItemJson * item = static_cast< TajirStoreCatalogItemJson * >( index.internalPointer() );

        if ( item )
        {
            return item;
        }
    }

    return nullptr;
}

QVariant StoreCatalogListModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if ( orientation == Qt::Horizontal && role == Qt::DisplayRole )
    {
        switch ( section )
        {
            case StoreCatalogListModelHeaderName:               return "Catalog";
            case StoreCatalogListModelHeaderHidden:             return "Hidden";
            case StoreCatalogListModelHeaderItemId:             return "Item";
            case StoreCatalogListModelHeaderCanSell:            return "Can Sell";
            case StoreCatalogListModelHeaderCanBuy:             return "Can Buy";
            case StoreCatalogListModelHeaderQuantity:           return "Quantity";
            case StoreCatalogListModelHeaderRestockQuantity:    return "Restock Quantity";
            default: return QVariant();
        }
    }

    if ( role == Qt::StatusTipRole || role == Qt::ToolTipRole || role == Qt::WhatsThisRole )
    {
        switch( section )
        {
            case StoreCatalogListModelHeaderName:
                return "The catalog name.";
            case StoreCatalogListModelHeaderHidden:
                return "Check if you want this catalog to be hidden from the player.";
            case StoreCatalogListModelHeaderItemId:
                return "Select an item from the item list that this catalog item will represent.";
            case StoreCatalogListModelHeaderCanSell:
                return "Check if you want the store to sell the item to players";
            case StoreCatalogListModelHeaderCanBuy:
                return "Check if you want the store to buy the item from players. Doesnt apply to bundles.";
            case StoreCatalogListModelHeaderQuantity:
                return "The starting quantity the store has on hand for sale. Later updated by saved values server side.";
            case StoreCatalogListModelHeaderRestockQuantity:
                return "If restocking is enabled on the store, this item will replenish its stock to this level every restock cycle.";
        }
    }
    return QVariant();
}

QModelIndex StoreCatalogListModel::index( int row, int column, const QModelIndex &parent ) const
{
    if ( parent.isValid() )
    {
        TajirStoreCatalogJson * catalog = getCatalog( parent );

        if ( catalog )
        {
            TajirStoreCatalogItemJson * item = catalog->items.value( row, nullptr );

            if ( item )
            {
                return createIndex( row, column, item );
            }
        }

        return QModelIndex();
    }
    else if ( !parent.isValid() )
    {
        TajirStoreCatalogJson * entry = store->catalogs.value( row, nullptr );

        if ( entry )
        {
            return createIndex( row, column, entry );
        }
    }

    return QModelIndex();
}

QModelIndex StoreCatalogListModel::parent( const QModelIndex &index ) const
{
    if ( !index.isValid() )
    {
        return QModelIndex();
    }

    if ( isCatalog( index ) )
    {
        return QModelIndex();
    }

    TajirStoreCatalogItemJson * item = getItem( index );

    if ( item && item->catalog )
    {
        return createIndex( store->catalogs.indexOf( item->catalog ), 0, item->catalog );
    }

    return QModelIndex();
}

bool StoreCatalogListModel::isCatalog( const QModelIndex &index ) const
{
    if ( index.isValid() )
    {
        foreach( TajirStoreCatalogJson * catalog, store->catalogs )
        {
            if ( catalog == index.internalPointer() )
            {
                return true;
            }
        }
    }

    return false;
}

int StoreCatalogListModel::rowCount( const QModelIndex &parent ) const
{   
    if ( isCatalog( parent ) )
    {
        TajirStoreCatalogJson * catalog = getCatalog( parent );

        if ( catalog )
        {
            return catalog->items.count();
        }
    }
    else if ( !parent.isValid() )
    {
        return store->catalogs.count();
    }

    return 0;
}

void StoreCatalogListModel::addItem( TajirStoreCatalogJson * catalog, TajirStoreCatalogItemJson * item )
{
    beginResetModel();

    catalog->items.append( item );

    endResetModel();
}

void StoreCatalogListModel::addCatalog( TajirStoreCatalogJson * catalog )
{
    beginResetModel();

    store->catalogs.append( catalog );

    endResetModel();
}

void StoreCatalogListModel::setStore( TajirStoreJson * newStore )
{
    beginResetModel();

    store = newStore;

    endResetModel();
}

TajirStoreJson * StoreCatalogListModel::getStore()
{
    return store;
}

bool StoreCatalogListModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
    if ( !index.isValid() )	return false;

    bool updated = false;

    if ( isCatalog( index ) )
    {
        TajirStoreCatalogJson * catalog = getCatalog( index );

        if ( catalog )
        {
            switch( index.column() )
            {

                case StoreCatalogListModelHeaderName:
                    catalog->name = value.toString();
                    updated = true;
                    break;
                case StoreCatalogListModelHeaderHidden:
                    catalog->hidden = value.toInt() > 0 ? 1 : 0;
                    updated = true;
                    break;
            }
        }
    }
    else
    {
        TajirStoreCatalogItemJson * item = getItem( index );

        if ( item )
        {
            switch( index.column() )
            {
                case StoreCatalogListModelHeaderItemId:
                    item->item_id = value.toInt();
                    updated = true;
                    break;
                case StoreCatalogListModelHeaderCanSell:
                    item->can_sell = value.toInt() > 0 ? 1 : 0;
                    updated = true;
                    break;
                case StoreCatalogListModelHeaderCanBuy:
                    item->can_buy = value.toInt() > 0 ? 1 : 0;
                    updated = true;
                    break;
                case StoreCatalogListModelHeaderQuantity:
                    item->quantity = value.toInt();
                    updated = true;
                    break;
                case StoreCatalogListModelHeaderRestockQuantity:
                    item->restock_quantity = value.toInt();
                    updated = true;
                    break;
            }

            TajirItemJson * refItem = itemList->getItemById( item->item_id );

            if ( refItem && refItem->isBundle() )
            {
                if ( item->can_buy )
                {
                    item->can_buy = 0;
                    updated = true;
                }
            }
        }
    }

    return updated;
}

void StoreCatalogListModel::removeCatalog( const QModelIndex& index )
{
    if ( !index.isValid() || !isCatalog( index ) )
    {
        return;
    }

    TajirStoreCatalogJson * catalog = getCatalog( index );

    if ( catalog )
    {
        beginResetModel();

        store->catalogs.removeOne( catalog );

        endResetModel();

        delete catalog;
    }
}

void StoreCatalogListModel::removeItem( const QModelIndex& index )
{
    if ( !index.isValid() || isCatalog( index ) )
    {
        return;
    }

    TajirStoreCatalogItemJson * item = getItem( index );

    if ( item )
    {
        beginResetModel();

        item->catalog->items.removeOne( item );

        endResetModel();

        delete item;
    }
}

bool StoreCatalogListModel::setHeaderData( int section, Qt::Orientation orientation, const QVariant &value, int role )
{
    return false;
}

bool StoreCatalogListModel::insertColumns( int position, int columns, const QModelIndex &parent )
{
    return false;
}

bool StoreCatalogListModel::removeColumns( int position, int columns, const QModelIndex &parent )
{
    return false;
}

bool StoreCatalogListModel::insertRows( int position, int rows, const QModelIndex &parent )
{
    return false;
}

bool StoreCatalogListModel::removeRows( int position, int rows, const QModelIndex &parent )
{
    return false;
}

