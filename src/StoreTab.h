#pragma once

#include <QWidget>
#include "TajirStoreJson.h"
#include "ItemList.h"
#include "StoreCatalogList.h"

namespace Ui {
class StoreTab;
}

class StoreTabs;

class StoreTab : public QWidget
{
    Q_OBJECT
    
    ItemList * itemList;
    TajirStoreJson * store;
    StoreCatalogList * catalogList;
    StoreTabs * tabs;
public:
    StoreTab( ItemList * itemList, TajirStoreJson * store, StoreTabs * parent );
    ~StoreTab();
    TajirStoreJson * getStore();
protected slots:
    void onFieldChange();
private:
    Ui::StoreTab *ui;
};
