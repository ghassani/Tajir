#include "TajirItemJson.h"

TajirItemJson::TajirItemJson() :
    id(0),
    object_name(""),
    description(""),
    display_object_name(""),
    sell_price(0),
    buy_price(0),
    quantity(0),
    root(nullptr),
    parent(nullptr),
    items()
{
}

TajirItemJson::~TajirItemJson()
{
    qDeleteAll(items);
}

TajirItemJson::TajirItemJson( const QJsonObject & entry ) :
    id(0),
    object_name(""),
    description(""),
    display_object_name(""),
    sell_price(0),
    buy_price(0),
    quantity(0),
    root(nullptr),
    parent(nullptr),
    items()
{
    if ( entry.contains( "id" ) )
    {
        id = entry["id"].toInt();
    }

    if ( entry.contains( "object_name" ) )
    {
        object_name = entry["object_name"].toString();
    }

    if ( entry.contains( "description" ) )
    {
        description = entry["description"].toString();
    }

    if ( entry.contains( "display_object_name" ) )
    {
        display_object_name = entry["display_object_name"].toString();
    }

    if ( entry.contains( "sell_price" ) )
    {
        sell_price = entry["sell_price"].toInt();
    }

    if ( entry.contains( "buy_price" ) )
    {
        buy_price = entry["buy_price"].toInt();
    }

    if ( entry.contains( "items" ) && entry["items"].isArray() )
    {
        QJsonArray bundleItems = entry["items"].toArray();

        foreach (const QJsonValue &bundleItem, bundleItems )
        {
            if ( bundleItem.isObject() )
            {
                items.append( new TajirItemJson( bundleItem.toObject(), this, nullptr ) );
            }
        }
    }
}

TajirItemJson::TajirItemJson( TajirItemJson * root, TajirItemJson * parent ) :
    id(0),
    object_name(""),
    description(""),
    display_object_name(""),
    sell_price(0),
    buy_price(0),
    quantity(0),
    root(root),
    parent(parent),
    items()
{

}

TajirItemJson::TajirItemJson( const QJsonObject &entry, TajirItemJson * root, TajirItemJson * parent ) :
    id(0),
    object_name(""),
    description(""),
    display_object_name(""),
    sell_price(0),
    buy_price(0),
    quantity(0),
    root(root),
    parent(parent),
    items()
{
    if ( entry.contains( "object_name" ) )
    {
        object_name = entry["object_name"].toString();
    }

    if ( entry.contains( "quantity" ) )
    {
        quantity = entry["quantity"].toInt();
    }

    if ( entry.contains("items") && entry["items"].isArray() )
    {
        QJsonArray children = entry["items"].toArray();

        foreach (const QJsonValue &child, children )
        {
            if ( child.isObject() )
            {
                items.append( new TajirItemJson( child.toObject(), root, this ) );
            }
        }
    }
}

bool TajirItemJson::isBundle()
{
    return ( !root && !parent && items.count() );
}

bool TajirItemJson::isBundlesItem()
{
    return parent != nullptr || root != nullptr;
}

QJsonObject TajirItemJson::toJSON()
{
    QJsonObject ret;

    if ( isBundle() )
    {
        ret[ "id" ]                     = id;
        ret[ "object_name" ]            = object_name;
        ret[ "description" ]            = description;
        ret[ "display_object_name" ]    = display_object_name;
        ret[ "sell_price" ]             = sell_price;
        ret[ "buy_price" ]              = buy_price;
    }
    else if ( isBundlesItem() )
    {
        ret[ "object_name" ] = object_name;
        ret[ "quantity" ]    = quantity;
    }
    else
    {
        ret[ "id" ]                     = id;
        ret[ "object_name" ]            = object_name;
        ret[ "description" ]            = description;
        ret[ "display_object_name" ]    = display_object_name;
        ret[ "sell_price" ]             = sell_price;
        ret[ "buy_price" ]              = buy_price;
    }

    if ( items.count() )
    {
        QJsonArray array;

        foreach ( TajirItemJson * item, items )
        {
            array.append( item->toJSON() );
        }

        ret[ "items" ] = array;
    }

    return ret;
}
