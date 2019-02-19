#include "TajirStoreCatalogItemJson.h"
#include "TajirStoreCatalogJson.h"

TajirStoreCatalogItemJson::TajirStoreCatalogItemJson( TajirStoreCatalogJson * catalog ) :
    catalog( catalog ),
    item_id( 0 ),
    can_sell( 0 ),
    can_buy( 0 ),
    quantity( 0 ),
    restock_quantity( 0 )
{

}

TajirStoreCatalogItemJson::TajirStoreCatalogItemJson( TajirStoreCatalogJson * catalog, const QJsonObject & entry ) :
    catalog( catalog ),
    item_id( 0 ),
    can_sell( 0 ),
    can_buy( 0 ),
    quantity( 0 ),
    restock_quantity( 0 )
{

    if ( entry.contains( "item_id" ) )
    {
        item_id = entry["item_id"].toInt();
    }

    if ( entry.contains( "can_sell" ) )
    {
        can_sell = entry["can_sell"].toInt() > 0 ? 1 : 0;
    }

    if ( entry.contains( "can_buy" ) )
    {
        can_buy = entry["can_buy"].toInt() > 0 ? 1 : 0;
    }

    if ( entry.contains( "quantity" ) )
    {
        quantity = entry["quantity"].toInt();
    }

    if ( entry.contains( "restock_quantity" ) )
    {
        restock_quantity = entry["restock_quantity"].toInt();
    }
}

QJsonObject TajirStoreCatalogItemJson::toJSON()
{
    QJsonObject ret;

    ret[ "item_id" ]            = item_id;
    ret[ "can_sell" ]           = can_sell > 0 ? 1 : 0;
    ret[ "can_buy" ]            = can_buy > 0 ? 1 : 0;
    ret[ "quantity" ]           = quantity;
    ret[ "restock_quantity" ]   = restock_quantity;

    return ret;
}
