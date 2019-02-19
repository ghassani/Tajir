#include "StoreTab.h"
#include "ui_StoreTab.h"
#include "StoreTabs.h"
#include <QVBoxLayout>

StoreTab::StoreTab( ItemList * itemList, TajirStoreJson * store, StoreTabs * parent ) :
    QWidget( parent ),
    itemList( itemList ),
    store( store ),
    catalogList( new StoreCatalogList( store, itemList, this ) ),
    tabs( parent ),
    ui( new Ui::StoreTab )
{
    ui->setupUi( this );

    layout()->addWidget( catalogList );

    ui->id->setText( QString::number( store->id ) );
    ui->name->setText( store->name );
    ui->logTransactions->setChecked( store->log_transactions > 0 );
    ui->showOutOfStock->setChecked( store->show_out_of_stock > 0 );
    ui->saveEvery->setText( QString( "%1" ).arg( store->save_every_minutes ) );
    ui->restockEvery->setText( QString( "%1" ).arg( store->restock_every_minutes ) );

    ui->type->clear();
    ui->type->addItem( "default" );

    ui->type->setCurrentIndex(0);

    ui->allowedConditions->clear();
    ui->allowedConditions->addItem( "Prestine" );
    ui->allowedConditions->addItem( "Worn" );
    ui->allowedConditions->addItem( "Damaged" );
    ui->allowedConditions->addItem( "Badly Damaged" );

    if ( store->allowed_conditions.contains( 0 ) )
    {
        ui->allowedConditions->setItemSelected( ui->allowedConditions->item( 0 ), true );
    }

    if ( store->allowed_conditions.contains( 1 ) )
    {
        ui->allowedConditions->setItemSelected( ui->allowedConditions->item( 1 ), true );
    }

    if ( store->allowed_conditions.contains( 2 ) )
    {
        ui->allowedConditions->setItemSelected( ui->allowedConditions->item( 2 ), true );
    }

    if ( store->allowed_conditions.contains( 3 ) )
    {
        ui->allowedConditions->setItemSelected( ui->allowedConditions->item( 3 ), true );
    }

    QObject::connect( ui->name, SIGNAL( editingFinished() ), this, SLOT( onFieldChange() ) );
    QObject::connect( ui->type, SIGNAL( currentIndexChanged(int) ), this, SLOT( onFieldChange() ) );
    QObject::connect( ui->logTransactions, SIGNAL( stateChanged(int) ), this, SLOT( onFieldChange() ) );
    QObject::connect( ui->saveEvery, SIGNAL( editingFinished() ), this, SLOT( onFieldChange() ) );
    QObject::connect( ui->restockEvery, SIGNAL( editingFinished() ), this, SLOT( onFieldChange() ) );
    QObject::connect( ui->showOutOfStock, SIGNAL( stateChanged(int) ), this, SLOT( onFieldChange() ) );
    QObject::connect( ui->allowedConditions, SIGNAL( itemSelectionChanged() ), this, SLOT( onFieldChange() ) );
}

void StoreTab::onFieldChange()
{
    store->name                  = ui->name->text();
    store->type                  = ui->type->currentText();
    store->log_transactions      = ui->logTransactions->isChecked() ? 1 : 0;
    store->save_every_minutes    = ui->saveEvery->text().toInt();
    store->restock_every_minutes = ui->restockEvery->text().toInt();
    store->show_out_of_stock     = ui->showOutOfStock->isChecked() ? 1 : 0;

    store->allowed_conditions.clear();

    if ( ui->allowedConditions->item( 0 )->isSelected() )
    {
        store->allowed_conditions.append( 0 );
    }

    if ( ui->allowedConditions->item( 1 )->isSelected() )
    {
        store->allowed_conditions.append( 1 );
    }
    if ( ui->allowedConditions->item( 2 )->isSelected() )
    {
        store->allowed_conditions.append( 2 );
    }
    if ( ui->allowedConditions->item( 3 )->isSelected() )
    {
        store->allowed_conditions.append( 3 );
    }
}

StoreTab::~StoreTab()
{
    delete store;
    delete catalogList;
    delete ui;
}

TajirStoreJson * StoreTab::getStore()
{
    return store;
}
