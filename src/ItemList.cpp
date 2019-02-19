#include "ItemList.h"
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMenu>
#include <QContextMenuEvent>

ItemList::ItemList( QWidget * parent ) :
    QTreeView( parent ),
    model( new ItemListModel() )
{
    setModel( model );

}

void ItemList::loadJson( const QString& filepath )
{
    QFile file( filepath );

    if ( !file.open( QIODevice::ReadOnly ) )
    {
        QMessageBox::warning(this, "", "Error Opening File For Reading", QMessageBox::Ok);
        return;
    }

    QString contents = file.readAll();

    QJsonDocument doc = QJsonDocument::fromJson(contents.toUtf8());

    if ( !doc.isArray() )
    {
        QMessageBox::warning(this, "", "Expecting An Array of Objects", QMessageBox::Ok);
        return;
    }

    QList<TajirItemJson *> items;

    foreach (const QJsonValue &value, doc.array())
    {
        if ( !value.isObject() )
        {
            QMessageBox::warning(this, "", "Expecting An Array of Objects", QMessageBox::Ok);
            return;
        }

        QJsonObject entry = value.toObject();

        TajirItemJson * item = new TajirItemJson( entry );

        items.append( item );
    }

    model->setItems(items);

    resizeColumnToContents( ItemListModelHeaderId );
    resizeColumnToContents( ItemListModelHeaderObject );
    resizeColumnToContents( ItemListModelHeaderDisplayName );
}

void ItemList::contextMenuEvent( QContextMenuEvent * event )
{
    QMenu * menu        = new QMenu( this );
    QModelIndex index   = indexAt( event->pos() );

    menu->addAction( "Add Item", this, std::bind( &ItemList::addItem, this )  );

    if( index.isValid() )
    {
        menu->addAction( "Delete Item", this, std::bind( &ItemList::deleteItem, this, index )  );
        menu->addAction( "Add Bundle Item", this, std::bind( &ItemList::addBundleItem, this, index )  );
    }

    menu->exec( event->globalPos() );

    delete menu;
}

void ItemList::addItem()
{
    model->addItem( new TajirItemJson() );
}

void ItemList::addBundleItem( const QModelIndex& index )
{
    model->addItemAsChild( index );
}

void ItemList::deleteItem( const QModelIndex& index )
{
    model->removeItem( index );
}

TajirItemJson * ItemList::getItemById( int id )
{
    foreach ( TajirItemJson * item, model->getItems() )
    {
        if ( item->id == id )
        {
            return item;
        }
    }

    return nullptr;
}

const QList<TajirItemJson *>& ItemList::getItems()
{
    return model->getItems();
}
