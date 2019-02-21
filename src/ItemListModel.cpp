#include "ItemListModel.h"
#include <QColor>
#include <QAbstractItemView>

ItemListModel::ItemListModel( QObject *parent )
    : QAbstractItemModel( parent ),
      items()
{

}

ItemListModel::~ItemListModel()
{

}

int ItemListModel::columnCount( const QModelIndex & /* parent */ ) const
{
    return 8;
}

QVariant ItemListModel::data( const QModelIndex &index, int role ) const
{
    if ( !index.isValid() )
    {
        return QVariant();
    }

    TajirItemJson * item = getItem( index );

    if ( item )
    {
        if ( role == Qt::BackgroundRole )
        {
            if ( !item->isBundlesItem() )
            {
                if ( !item->object_name.length() && !item->isBundle() )
                {
                    return QColor( "red" );
                }
                else  if ( item->isBundle() && !item->object_name.length() && !item->display_name.length() )
                {
                    return QColor( "red" );
                }
                else
                {
                    if ( item->isBundle() )
                    {
                        return QColor( "green" );
                    }
                }
            }
        }

        if ( role == Qt::DisplayRole )
        {
            if ( item->isBundlesItem() )
            {
                switch( index.column() )
                {
                    case ItemListModelHeaderObject:
                        return item->object_name;
                    case ItemListModelHeaderQuantity:
                        return item->quantity;
                }
            }
            else
            {
                switch( index.column() )
                {
                    case ItemListModelHeaderId:
                        return item->id;
                    case ItemListModelHeaderObject:
                        return item->object_name;
                    case ItemListModelHeaderDisplayName:
                        return item->display_name;
                    case ItemListModelHeaderDescription:
                        return item->description;
                    case ItemListModelHeaderDisplayObject:
                        return item->display_object_name;
                    case ItemListModelHeaderSellPrice:
                        return item->sell_price;
                    case ItemListModelHeaderBuyPrice:
                        return item->buy_price;
                }
            }
        }

        if ( role == Qt::EditRole )
        {
            if ( item->isBundlesItem() )
            {
                switch( index.column() )
                {
                    case ItemListModelHeaderObject:
                        return item->object_name;
                    case ItemListModelHeaderQuantity:
                        return item->quantity;
                }
            }
            else
            {
                switch( index.column() )
                {
                    case ItemListModelHeaderId:
                        return item->id;
                    case ItemListModelHeaderObject:
                        return item->object_name;
                    case ItemListModelHeaderDisplayName:
                        return item->display_name;
                    case ItemListModelHeaderDescription:
                        return item->description;
                    case ItemListModelHeaderDisplayObject:
                        return item->display_object_name;
                    case ItemListModelHeaderSellPrice:
                        return item->sell_price;
                    case ItemListModelHeaderBuyPrice:
                        return item->buy_price;
                }
            }
        }
    }

    return QVariant();
}

Qt::ItemFlags ItemListModel::flags( const QModelIndex &index ) const
{
    if ( !index.isValid() )
    {
        return 0;
    }

   QFlags<Qt::ItemFlag> flags = Qt::ItemIsEnabled | QAbstractItemModel::flags( index );

   TajirItemJson * item = getItem( index );

   if ( item )
   {
       if ( item->isBundlesItem() )
       {
           if ( index.column() == ItemListModelHeaderObject ||
                     index.column() == ItemListModelHeaderQuantity )
           {
               flags |= Qt::ItemIsEditable;
           }
       }
       else
       {
           if ( index.column() != ItemListModelHeaderId && index.column() != ItemListModelHeaderQuantity )
           {
               flags |= Qt::ItemIsEditable;
           }
       }
   }

   return flags;
}

TajirItemJson * ItemListModel::getItem(const QModelIndex &index ) const
{
    if ( index.isValid() )
    {
        TajirItemJson * item = static_cast< TajirItemJson * >( index.internalPointer() );

        if ( item )
        {
            return item;
        }
    }

    return nullptr;
}

QVariant ItemListModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if ( orientation == Qt::Horizontal && role == Qt::DisplayRole )
    {
        switch ( section )
        {
            case ItemListModelHeaderId:             return "ID";
            case ItemListModelHeaderObject:         return "Object";
            case ItemListModelHeaderDisplayName:    return "Display Name";
            case ItemListModelHeaderDescription:    return "Description";
            case ItemListModelHeaderDisplayObject:  return "Display Object Name";
            case ItemListModelHeaderQuantity:       return "Quantity (Bundle Only)";
            case ItemListModelHeaderSellPrice:      return "Sell Price";
            case ItemListModelHeaderBuyPrice:       return "Buy Price";
            default: return QVariant();
        }
    }

    if ( role == Qt::StatusTipRole || role == Qt::ToolTipRole || role == Qt::WhatsThisRole )
    {
        switch( section )
        {
            case ItemListModelHeaderId:
                return "Internal ID of the item. Used to reference items in stores.";
            case ItemListModelHeaderObject:
                return "The game object name this item represents. Not required if making bundled items with multiple root items.";
            case ItemListModelHeaderDisplayName:
                return "Override the display name in the store menu or use a custom name for multi root level bundle items.";
            case ItemListModelHeaderDescription:
                return "Override the object description in the store menu or use a custom description for multi root level bundle items.";
            case ItemListModelHeaderDisplayObject:
                return "Force a different item to be rendered in the item preview. Specify a game object name";
            case ItemListModelHeaderQuantity:
                return "For bundled items, specify the quantity to include of the item when purchase.";
            case ItemListModelHeaderSellPrice:
                return "The amount the store sells the item for. This is the amount the player pays.";
            case ItemListModelHeaderBuyPrice:
                return "The amount the store buys the item for. This is the amount the player receives.";
        }
    }

    return QVariant();
}

QModelIndex ItemListModel::index( int row, int column, const QModelIndex &parent ) const
{
    if ( parent.isValid() )
    {
        TajirItemJson * entry_parent = getItem( parent );

        if ( entry_parent )
        {
            TajirItemJson * child_entry = entry_parent->items.value( row, nullptr );

            if ( child_entry )
            {
                return createIndex( row, column, child_entry );
            }
        }

        return QModelIndex();
    }

    TajirItemJson * entry = items.value( row, nullptr );

    if ( entry )
    {
        return createIndex( row, column, entry );
    }

    return QModelIndex();
}

QModelIndex ItemListModel::parent( const QModelIndex &index ) const
{
    if ( !index.isValid() )
    {
        return QModelIndex();
    }

    TajirItemJson * entry = getItem( index );

    if ( entry )
    {
        int row = 0;

        if ( entry->parent )
        {
            return createIndex( row, 0, entry->parent );
        }
        else if ( entry->root )
        {
            return createIndex( row, 0, entry->root );
        }
    }

    return QModelIndex();
}

int ItemListModel::rowCount( const QModelIndex &parent ) const
{
    if ( !parent.isValid() )
    {
        return items.count();
    }

    TajirItemJson * p = getItem( parent );

    if ( p )
    {
        return p->items.count();
    }

       return 0;
}

void ItemListModel::removeItem( const QModelIndex& index )
{
    if ( !index.isValid() )
    {
        return;
    }

    TajirItemJson * item = getItem( index );

    if ( item )
    {
        beginResetModel();

        if ( index.parent().isValid() )
        {
            item->parent->items.removeOne( item );
        }
        else
        {
            items.removeOne( item );
        }

        endResetModel();

        delete item;
    }
}

void ItemListModel::addItemAsChild( const QModelIndex& index )
{
    if ( !index.isValid() )
    {
        return;
    }

    beginResetModel();

    TajirItemJson * parent = getItem( index );

    if ( parent )
    {
        parent->items.append( new TajirItemJson( parent->root, parent ) );
    }

    endResetModel();
}

void ItemListModel::addItem( TajirItemJson * item )
{
    beginResetModel();

    int largestid = 0;

    foreach( TajirItemJson * item, items )
    {
        if ( item->id > largestid )
        {
            largestid = item->id;
        }
    }

    item->id = largestid+1;

    items.append( item );

    endResetModel();
}

void ItemListModel::setItems( QList<TajirItemJson *> newItems )
{
    beginResetModel();

    qDeleteAll(items);
    items.clear();

    items = newItems;

    endResetModel();
}

bool ItemListModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
    if ( !index.isValid() )	return false;

    TajirItemJson * item = getItem( index );

    if ( !item ) return false;

    bool updated = false;

    if ( index.parent().isValid() )
    {
        switch( index.column() )
        {
            case ItemListModelHeaderObject:
            {
                item->object_name = value.toString();
                updated = true;
                break;
            }
            case ItemListModelHeaderQuantity:
            {
                item->quantity = value.toInt();
                updated = true;
                break;            
            }
            default:
            {
                break;
            }
        }
    }
    else
    {
        switch( index.column() )
        {
            default:
            {
                break;
            }
            case ItemListModelHeaderObject:
            {
                item->object_name = value.toString();
                updated = true;
                break;
            }
            case ItemListModelHeaderDisplayName:
            {
                item->display_name = value.toString();
                updated = true;
                break;
            }
            case ItemListModelHeaderDescription:
            {
                item->description = value.toString();
                updated = true;
                break;
            }
            case ItemListModelHeaderDisplayObject:
            {
                item->display_object_name = value.toString();
                updated = true;
                break;
            }
            case ItemListModelHeaderSellPrice:
            {
                item->sell_price = value.toInt();
                updated = true;
                break;
            }
            case ItemListModelHeaderBuyPrice:
            {
                item->buy_price = value.toInt();
                updated = true;
                break;
            }
        }
    }

    return updated;
}

bool ItemListModel::setHeaderData( int section, Qt::Orientation orientation, const QVariant &value, int role )
{
    return false;
}

bool ItemListModel::insertColumns( int position, int columns, const QModelIndex &parent )
{
    return false;
}

bool ItemListModel::removeColumns( int position, int columns, const QModelIndex &parent )
{
    return false;
}

bool ItemListModel::insertRows( int position, int rows, const QModelIndex &parent )
{
    return false;
}

bool ItemListModel::removeRows( int position, int rows, const QModelIndex &parent )
{
    return false;
}

const QList<TajirItemJson *>& ItemListModel::getItems() const
{
    return items;
}

