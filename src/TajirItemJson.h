#pragma once

#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QList>

class TajirItemJson
{
public:
    int id;
    QString object_name;
    QString display_name;
    QString description;
    QString display_object_name;
    int sell_price;
    int buy_price;

    // for bundle
    int quantity;
    TajirItemJson * root;
    TajirItemJson * parent;
    QList<TajirItemJson *> items;

    TajirItemJson();
    
    TajirItemJson( const QJsonObject & entry );

    TajirItemJson( TajirItemJson * root, TajirItemJson * parent = nullptr );

    TajirItemJson( const QJsonObject &entry, TajirItemJson * root, TajirItemJson * parent = nullptr );

    ~TajirItemJson();

    bool isBundle();

    bool isBundlesItem();

    QJsonObject toJSON();
};
