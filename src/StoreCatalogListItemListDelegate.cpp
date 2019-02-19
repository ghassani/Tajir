#include "StoreCatalogListItemListDelegate.h"
#include <QComboBox>
#include "TajirStoreCatalogItemJson.h"

StoreCatalogListItemListDelegate::StoreCatalogListItemListDelegate( ItemList * itemList, QObject *parent) :
    QStyledItemDelegate(parent),
    itemList( itemList )
{
}


StoreCatalogListItemListDelegate::~StoreCatalogListItemListDelegate()
{
}


QWidget *StoreCatalogListItemListDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox * choices = new QComboBox( parent );

    choices->setEditable( true );

    foreach ( const TajirItemJson * item, itemList->getItems() )
    {
        if ( item->object_name.length() )
        {
            choices->addItem( QString( "[%1] %2" ).arg( item->id ).arg( item->object_name ), item->id );
        }
        else if ( item->display_name.length() )
        {
            choices->addItem( QString( "[%1] %2" ).arg( item->id ).arg( item->object_name ), item->id );
        }
        else
        {
            choices->addItem( QString( "[%1] NO NAME" ).arg( item->id ), item->id );
        }
    }

    return choices;
}


void StoreCatalogListItemListDelegate::setEditorData( QWidget * editor, const QModelIndex &index ) const
{
    QComboBox * choices = qobject_cast<QComboBox *>( editor );

    if ( choices )
    {
        TajirStoreCatalogItemJson * data = static_cast<TajirStoreCatalogItemJson*>( index.internalPointer() );

         if ( data )
         {
             int idx = choices->findData( data->item_id );

             if ( idx >= 0 )
             {
                 choices->setCurrentIndex( idx );
             }
         }
    }
}


void StoreCatalogListItemListDelegate::setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const
{
    QComboBox * choices = qobject_cast<QComboBox *>( editor );

    if ( choices )
    {
        int id = choices->currentData().toInt();

        if ( id )
        {
            model->setData( index, id, Qt::EditRole );
        }
        else
        {
            id = choices->currentText().toInt();

            if ( id > 0 )
            {
                model->setData( index, id, Qt::EditRole );
            }
        }
    }
    
}
