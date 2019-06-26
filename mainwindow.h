#pragma once

#include "directoryscanner.h"
#include "middlecircle.h"
#include "visualfolder.h"
#include "config.h"
#include "folderpropertieslabel.h"

#include <QDirModel>
#include <QMainWindow>
#include <QTimer>
#include <filesystem>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QGraphicsScene* dataScene;
    MiddleCircle* middleCircle;
    FolderPropertiesLabel* propertiesLabel = nullptr;
public slots:
    void updateProgress();


private slots:

    void on_startScanning_clicked();

private:
    Ui::MainWindow *ui;
    QTimer* timer = nullptr;
    DirectoryScanner* scanner;
    DirectoryEntry* currentItem = nullptr;
    QDirModel* directoryViewModel;
    VisualFolder* lastSelectedVisualFolder = nullptr;
    VisualFolder* lastVisualRootFolder = nullptr;
    int64_t currentSize = 0;
    bool firstFolderScanned = false;
    void checkForNewScannedFolders();
    void scanFolder(DirectoryEntry *item, int dept, int64_t currentSize, int color);
    void startNewScan(DirectoryEntry *baseDirectory);
    VisualFolder* visualizeFolder(DirectoryEntry *item, int dept, int64_t currentSize, int parentColor);
};

