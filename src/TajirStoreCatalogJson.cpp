#include "TajirStoreCatalogJson.h"
#include "TajirStoreJson.h"
#include "TajirStoreCatalogItemJson.h"

TajirStoreCatalogJson::TajirStoreCatalogJson( TajirStoreJson * store ) :
    store( store ),
    name( "" ),
    hidden( 0 ),
    items()
{

}

TajirStoreCatalogJson::~TajirStoreCatalogJson()
{
    qDeleteAll(items);
}

TajirStoreCatalogJson::TajirStoreCatalogJson( TajirStoreJson * store, const QJsonObject & entry ) :
    store( store ),
    name( "" ),
    hidden( 0 ),
    items()
{
    TajirStoreCatalogItemJson * catitem;

    if ( entry.contains( "name" ) )
    {
        name = entry["name"].toString();
    }
    
    if ( entry.contains( "hidden" ) )
    {
        hidden = entry["hidden"].toInt();
    }

    if ( entry.contains( "items" ) && entry["items"].isArray() )
    {
        QJsonArray itms = entry["items"].toArray();

        foreach (const QJsonValue &item, itms )
        {
            if ( item.isObject() )
            {
                catitem = new TajirStoreCatalogItemJson( this, item.toObject() );

                items.append( catitem );
            }
        }
    }
}

QJsonObject TajirStoreCatalogJson::toJSON()
{
    QJsonObject ret;
    ret[ "name" ]    = name;
    ret[ "hidden" ]  = hidden > 0 ? 1 : 0;

    if ( items.count() )
    {
        QJsonArray itms;

        foreach( TajirStoreCatalogItemJson * item, items )
        {
            itms.append( item->toJSON() );
        }

        ret[ "items" ] = itms;
    }


    return ret;
}
