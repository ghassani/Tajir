#include "StoreTabs.h"
#include <QVBoxLayout>
#include <QMessageBox>

StoreTabs::StoreTabs( ItemList * itemList, QWidget * parent ) :
    QTabWidget( parent ),
    itemList( itemList ),
    stores()
{
    QVBoxLayout * layout = new QVBoxLayout( this );

    setLayout( layout );
    setTabsClosable( true );

    layout->setSpacing(6);
    layout->setContentsMargins(11, 11, 11, 11);

    QObject::connect( this, SIGNAL( tabCloseRequested(int) ), this, SLOT( onTabCloseRequested(int) ) );
}

void StoreTabs::addStore( TajirStoreJson * store )
{
    store->id = stores.count() + 1;

    StoreTab * tab = new StoreTab( itemList, store, this );

    stores.append( tab );

    addTab( tab, QString("Store %1").arg( store->id ) );
}

const QList<TajirStoreJson *> StoreTabs::getStores() const
{
    QList<TajirStoreJson *> ret;

    foreach( auto tab, stores )
    {
        ret.append( tab->getStore() );
    }

    return ret;
}

void StoreTabs::removeAll()
{
    qDeleteAll( stores );
    stores.clear();
}

void StoreTabs::onTabCloseRequested( int index )
{
    int res = QMessageBox::warning(this, "", "Store data will be removed. Continue?",
                                   QMessageBox::Ok | QMessageBox::Cancel,
                                   QMessageBox::Ok);
    if ( res != QMessageBox::Ok )
    {
        return;
    }

    if ( !stores.count() || index > ( stores.count() - 1 ) )
    {
        return;
    }

    StoreTab * tab = stores.takeAt( index );

    if ( tab )
    {
        removeTab( index );

        delete tab;
    }
}
