#pragma once

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QJsonDocument>
#include "TajirStoreJson.h"
#include "TajirStoreCatalogJson.h"
#include "TajirStoreCatalogItemJson.h"
#include "ItemList.h"

enum StoreCatalogListModelHeaders
{
    StoreCatalogListModelHeaderName = 0,
    StoreCatalogListModelHeaderHidden,
    StoreCatalogListModelHeaderItemId,
    StoreCatalogListModelHeaderCanSell,
    StoreCatalogListModelHeaderCanBuy,
    StoreCatalogListModelHeaderQuantity,
    StoreCatalogListModelHeaderRestockQuantity
};

class StoreCatalogListModel : public QAbstractItemModel
{
    Q_OBJECT

    TajirStoreJson * store;
    ItemList * itemList;

public:
    StoreCatalogListModel( TajirStoreJson * store, ItemList * itemList, QObject *parent = nullptr );

    ~StoreCatalogListModel();

    QVariant data(const QModelIndex &index, int role) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    QModelIndex index( int row, int column, const QModelIndex &parent = QModelIndex() ) const override;

    QModelIndex parent( const QModelIndex &index ) const override;

    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;

    int columnCount( const QModelIndex &parent = QModelIndex() ) const override;

    Qt::ItemFlags flags( const QModelIndex &index ) const override;

    void setStore( TajirStoreJson * store );

    TajirStoreJson * getStore();

    void addCatalog( TajirStoreCatalogJson * catalog );

    bool isCatalog( const QModelIndex &index ) const;

    TajirStoreCatalogJson * getCatalog( const QModelIndex &index ) const;

    TajirStoreCatalogItemJson * getItem(const QModelIndex &index ) const;

    bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole ) override;

    bool setHeaderData( int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole ) override;

    bool insertColumns( int position, int columns, const QModelIndex &parent = QModelIndex() ) override;

    bool removeColumns( int position, int columns, const QModelIndex &parent = QModelIndex() ) override;

    bool insertRows( int position, int rows, const QModelIndex &parent = QModelIndex() ) override;

    bool removeRows( int position, int rows, const QModelIndex &parent = QModelIndex() ) override;

    void addItem( TajirStoreCatalogJson * catalog, TajirStoreCatalogItemJson * item );

    void removeCatalog( const QModelIndex& index );

    void removeItem( const QModelIndex& index );

};

