#pragma once

#include "directoryscanner.h"
#include "middlecircle.h"
#include "visualfolder.h"

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
    QList<VisualFolder*> folders;
public slots:
    void updateProgress();


private:
    Ui::MainWindow *ui;
    DirectoryScanner* scanner;
    DirectoryEntry* currentItem = nullptr;
    QDirModel* directoryViewModel;
    VisualFolder* lastSelectedVisualFolder = nullptr;
    void checkForNewScannedFolders();
    void visualizeFolder(DirectoryEntry *directory, int dept, double startPercentage);
    void scanFolder(DirectoryEntry *item, int dept, double startPercentage);
};

