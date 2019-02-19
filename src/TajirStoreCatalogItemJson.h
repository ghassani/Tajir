#pragma once

#include <QList>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

class TajirStoreCatalogJson;

class TajirStoreCatalogItemJson
{
public:
    TajirStoreCatalogJson * catalog;
    int item_id;
    int can_sell;
    int can_buy;
    int quantity;
    int restock_quantity;

    TajirStoreCatalogItemJson( TajirStoreCatalogJson * catalog );

    TajirStoreCatalogItemJson( TajirStoreCatalogJson * catalog, const QJsonObject & entry );   

    QJsonObject toJSON();
};
