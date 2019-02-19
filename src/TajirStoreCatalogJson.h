#pragma once

#include <QList>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

class TajirStoreJson;
class TajirStoreCatalogItemJson;

class TajirStoreCatalogJson
{
public:
    TajirStoreJson * store;
    QString name;
    int hidden;
    QList<TajirStoreCatalogItemJson *> items;

    TajirStoreCatalogJson( TajirStoreJson * store );

    ~TajirStoreCatalogJson();

    TajirStoreCatalogJson( TajirStoreJson * store, const QJsonObject & entry );

    QJsonObject toJSON();
};
