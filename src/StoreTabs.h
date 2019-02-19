#pragma once

#include <QTabWidget>
#include "StoreTab.h"
#include "TajirStoreJson.h"
#include "ItemList.h"

class StoreTabs : public QTabWidget
{
    Q_OBJECT
    ItemList * itemList;

    QList<StoreTab *> stores;
public:
    StoreTabs( ItemList * itemList, QWidget * parent = nullptr );

    void addStore( TajirStoreJson * store );

    const QList<TajirStoreJson *> getStores() const;

    void removeAll();
 public slots:
    void onTabCloseRequested( int index );
};
