#pragma once

#include <QMainWindow>
#include "TajirItemJson.h"
#include "StoreTabs.h"

#pragma once

namespace Ui {
class MainWindow;
}

class ItemListModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    StoreTabs * stores;
    bool hasLoaded = false;
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void browseConfigDir();
    void loadConfigDir();
    void saveConfigDir();

    void loadItemsJsonFile( const QString& filepath );
    void loadStoresJsonFile( const QString& filepath );

    void saveItemsJson( const QString& outfile );
    void saveStoresJson( const QString& outfile );

    void addStore();
private:
    Ui::MainWindow *ui;
};
