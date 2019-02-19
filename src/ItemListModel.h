#pragma once

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QJsonDocument>
#include "TajirItemJson.h"

enum ItemListModelHeaders
{
    ItemListModelHeaderId = 0,
    ItemListModelHeaderObject,
    ItemListModelHeaderDisplayName,
    ItemListModelHeaderDescription,
    ItemListModelHeaderDisplayObject,
    ItemListModelHeaderQuantity,
    ItemListModelHeaderSellPrice,
    ItemListModelHeaderBuyPrice
};

class ItemListModel : public QAbstractItemModel
{
    Q_OBJECT

    QList<TajirItemJson *> items;

public:
    ItemListModel( QObject *parent = nullptr );

    ~ItemListModel();

    QVariant data(const QModelIndex &index, int role) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    QModelIndex index( int row, int column, const QModelIndex &parent = QModelIndex() ) const override;

    QModelIndex parent( const QModelIndex &index ) const override;

    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;

    int columnCount( const QModelIndex &parent = QModelIndex() ) const override;

    Qt::ItemFlags flags( const QModelIndex &index ) const override;

    void setItems( QList<TajirItemJson *> items );

    void addItem( TajirItemJson * item );

    void addItemAsChild( const QModelIndex& index );

    void removeItem( const QModelIndex& index );

    TajirItemJson * getItem( const QModelIndex &index ) const;

    bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole ) override;

    bool setHeaderData( int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole ) override;

    bool insertColumns( int position, int columns, const QModelIndex &parent = QModelIndex() ) override;

    bool removeColumns( int position, int columns, const QModelIndex &parent = QModelIndex() ) override;

    bool insertRows( int position, int rows, const QModelIndex &parent = QModelIndex() ) override;

    bool removeRows( int position, int rows, const QModelIndex &parent = QModelIndex() ) override;

    const QList<TajirItemJson *>& getItems() const;
private:

   // void setupModelData( const QDir& dir, ItemListItem * parent );
};

