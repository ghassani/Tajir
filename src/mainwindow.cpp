#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>
#include "ItemListModel.h"

MainWindow::MainWindow( QWidget *parent ) :
    QMainWindow( parent ),
    ui(new Ui::MainWindow)
{
    ui->setupUi( this );

    stores = new StoreTabs( ui->itemList, ui->mainTabStores );

    ui->mainTabStoresLayout->addWidget( stores );

    setWindowTitle( "Tajir Mod Store Builder" );

    QObject::connect( ui->configDirBrowse, SIGNAL( pressed() ), this, SLOT( browseConfigDir() ) );
    QObject::connect( ui->configDirLoadButton, SIGNAL( pressed() ), this, SLOT( loadConfigDir() ) );
    QObject::connect( ui->configDirSaveButton, SIGNAL( pressed() ), this, SLOT( saveConfigDir() ) );
    QObject::connect( ui->addStoreButton, SIGNAL( pressed() ), this, SLOT( addStore() ) );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::browseConfigDir()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Config Directory"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks );

    if ( dir.length() )
    {
        ui->configDir->setText( dir );
    }    
}

void MainWindow::loadConfigDir()
{
    if ( hasLoaded )
    {
        int res = QMessageBox::warning(this, "", "Changes will be removed. Continue?",
                                       QMessageBox::Ok | QMessageBox::Cancel,
                                       QMessageBox::Ok);
        if ( res != QMessageBox::Ok )
        {
            return;
        }
    }

    if ( !ui->configDir->text().length() )
    {
        QMessageBox::warning(this, "", "Specify configuration directory", QMessageBox::Ok);
        return;
    }

    QDir dir( ui->configDir->text() );

    if ( !dir.exists() )
    {
        QMessageBox::warning(this, "", "Configuration does not exist", QMessageBox::Ok);
        return;
    }

    if ( !dir.exists( "items.json" ) )
    {
        QFile defaultItems( ":/json/items.json" );

        if ( !defaultItems.copy( dir.absoluteFilePath( "items.json") ) )
        {
            QMessageBox::warning(this, "", "Error copying default items.json", QMessageBox::Ok);
            return;
        }
    }

    if ( !dir.exists( "stores.json" ) )
    {
        QFile defaultItems( ":/json/stores.json" );

        if ( !defaultItems.copy( dir.absoluteFilePath( "stores.json") ) )
        {
            QMessageBox::warning(this, "", "Error copying default stores.json", QMessageBox::Ok);
            return;
        }
    }

    loadItemsJsonFile( dir.absoluteFilePath( "items.json") );
    loadStoresJsonFile( dir.absoluteFilePath( "stores.json") );

    QMessageBox::information(this, "", "Configuration Loaded", QMessageBox::Ok);
    hasLoaded = true;
}

void MainWindow::saveConfigDir()
{
    if ( !hasLoaded )
    {
        return;
    }

    if ( !ui->configDir->text().length() )
    {
        QMessageBox::warning(this, "", "Specify configuration directory", QMessageBox::Ok);
        return;
    }

    QDir dir( ui->configDir->text() );

    if ( !dir.exists() )
    {
        QMessageBox::warning(this, "", "Configuration does not exist", QMessageBox::Ok);
        return;
    }


    saveItemsJson( dir.absoluteFilePath( "items.json" ) );
    saveStoresJson( dir.absoluteFilePath( "stores.json" ) );

    QMessageBox::information(this, "", "Configuration Saved", QMessageBox::Ok);
}

void MainWindow::loadItemsJsonFile( const QString& filepath )
{
    QFile file( filepath );

    if ( !file.open( QIODevice::ReadOnly ) )
    {
        QMessageBox::warning(this, "", QString( "Error Opening File %1 For Reading" ).arg( filepath ), QMessageBox::Ok);
        return;
    }

    QString contents = file.readAll();

    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(contents.toUtf8());

    if ( !doc.isArray() )
    {
        QMessageBox::warning(this, "", "Expecting An Array of Objects", QMessageBox::Ok);
        return;
    }

    ui->itemList->loadJson( filepath );
}

void MainWindow::loadStoresJsonFile( const QString& filepath )
{
    QFile file( filepath );

    if ( !file.open( QIODevice::ReadOnly ) )
    {
        QMessageBox::warning(this, "", QString( "Error Opening File %1 For Reading" ).arg( filepath ), QMessageBox::Ok);
        return;
    }

    QString contents = file.readAll();

    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(contents.toUtf8());

    if ( !doc.isArray() )
    {
        QMessageBox::warning(this, "", "Expecting An Array of Objects", QMessageBox::Ok);
        return;
    }

    stores->removeAll();

    foreach (const QJsonValue &value, doc.array())
    {
        if ( !value.isObject() )
        {
            QMessageBox::warning(this, "", "Expecting An Array of Objects", QMessageBox::Ok);
            return;
        }

        QJsonObject entry = value.toObject();

        stores->addStore( new TajirStoreJson( entry ) );
    }
}


void MainWindow::saveItemsJson( const QString& outfile  )
{
    if ( outfile.length() )
    {
        QFile file( outfile );

        if ( file.exists() )
        {
            file.copy( "items.json.last" );
            file.remove();
        }

        if ( !file.open( QIODevice::ReadWrite | QIODevice::Truncate ) )
        {
            QMessageBox::warning(this, "", QString( "Error Opening File %1 For Writing" ).arg( outfile ), QMessageBox::Ok);
            return;
        }

        QJsonArray output;

        foreach ( auto item, ui->itemList->getItems() )
        {
            output.append( item->toJSON() );
        }

        QJsonDocument doc(output);

        file.write( doc.toJson() );

        QMessageBox::information(this, "", QString("Saved Items JSON to %1").arg( outfile ), QMessageBox::Ok);

        file.close();
    }
}

void MainWindow::saveStoresJson( const QString& outfile )
{
    if ( outfile.length() )
    {
        QFile file( outfile );

        if ( file.exists() )
        {
            file.copy( "stores.json.last" );
            file.remove();
        }

        if ( !file.open( QIODevice::ReadWrite | QIODevice::Truncate ) )
        {
            QMessageBox::warning(this, "", QString( "Error Opening File %1 For Writing" ).arg( outfile ), QMessageBox::Ok);
            return;
        }

        QJsonArray output;

        foreach ( auto store, stores->getStores() )
        {
            output.append( store->toJSON() );
        }

        QJsonDocument doc(output);

        file.write( doc.toJson() );

        file.close();

        QMessageBox::information(this, "", QString( "Saved Store JSON to %1" ).arg( outfile ), QMessageBox::Ok);
        return;
    }
}

void MainWindow::addStore()
{
    if ( !hasLoaded )   return;

    stores->addStore( new TajirStoreJson() );
}
