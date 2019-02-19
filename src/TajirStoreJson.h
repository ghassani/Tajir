#pragma once

#include <QList>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

class TajirStoreCatalogJson;

class TajirStoreJson
{
public:
	int id;
	QString name;
	QString type;
	int log_transactions;
	int save_every_minutes;
	int restock_every_minutes;
	int show_out_of_stock;
	QList<int> allowed_conditions;
	QList<TajirStoreCatalogJson *> catalogs;

	TajirStoreJson();

	~TajirStoreJson();
	
	TajirStoreJson( const QJsonObject & entry );

    QJsonObject toJSON();
};
