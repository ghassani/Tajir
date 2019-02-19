#include "TajirStoreJson.h"
#include "TajirStoreCatalogJson.h"

TajirStoreJson::TajirStoreJson() :
	id( 0 ),
	name( "" ),
	type( "default" ),
	log_transactions( 1 ),
	save_every_minutes( 10 ),
	restock_every_minutes( 30 ),
	show_out_of_stock( 0 ),
	allowed_conditions(),
	catalogs()
{

}

TajirStoreJson::~TajirStoreJson()
{
	qDeleteAll(catalogs);
}

TajirStoreJson::TajirStoreJson( const QJsonObject & entry ) :
	id( 0 ),
	name( "" ),
	type( "default" ),
	log_transactions( 1 ),
	save_every_minutes( 10 ),
	restock_every_minutes( 30 ),
	show_out_of_stock( 0 ),
	allowed_conditions(),
	catalogs()
{
    if ( entry.contains( "id" ) )
    {
        id = entry["id"].toInt();
    }

    if ( entry.contains( "name" ) )
    {
        name = entry["name"].toString();
    }

    if ( entry.contains( "type" ) )
    {
        type = entry["type"].toString();
    }

    if ( entry.contains( "log_transactions" ) )
    {
        log_transactions = entry["log_transactions"].toInt() > 0 ? 1 : 0;
    }

    if ( entry.contains( "save_every_minutes" ) )
    {
        save_every_minutes = entry["save_every_minutes"].toInt();
    }

    if ( entry.contains( "restock_every_minutes" ) )
    {
        restock_every_minutes = entry["restock_every_minutes"].toInt();
    }

    if ( entry.contains( "show_out_of_stock" ) )
    {
        show_out_of_stock = entry["show_out_of_stock"].toInt() > 0 ? 1 : 0;
    }

    if ( entry.contains( "allowed_conditions" ) && entry["allowed_conditions"].isArray() )
    {
        QJsonArray conditions = entry["allowed_conditions"].toArray();

        foreach( const QJsonValue & condition, conditions )
        {
        	allowed_conditions.append( condition.toInt() );
        }
    }

    if ( entry.contains( "catalogs" ) && entry["catalogs"].isArray() )
    {
        QJsonArray cats = entry["catalogs"].toArray();

        foreach (const QJsonValue &catalog, cats )
        {
            if ( catalog.isObject() )
            {
                catalogs.append( new TajirStoreCatalogJson( this, catalog.toObject() ) );
            }
        }
    }
}

QJsonObject TajirStoreJson::toJSON()
{
    QJsonObject ret;

    ret[ "id" ]                     = id;
    ret[ "name" ]                   = name;
    ret[ "type" ]                   = type;
    ret[ "log_transactions" ]       = log_transactions > 0 ? 1 : 0;
    ret[ "save_every_minutes" ]     = save_every_minutes;
    ret[ "restock_every_minutes" ]  = restock_every_minutes;
    ret[ "show_out_of_stock" ]      = show_out_of_stock > 0 ? 1 : 0;;

    QJsonArray conditions;

    foreach( int condition, allowed_conditions )
    {
        conditions.append( condition );
    }

    ret[ "allowed_conditions" ]  = conditions;

    if ( catalogs.count() )
    {
        QJsonArray cats;

        foreach( TajirStoreCatalogJson * catalog, catalogs )
        {
            cats.append( catalog->toJSON() );
        }

        ret[ "catalogs" ] = cats;
    }

    return ret;
}
