#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scanner = new DirectoryScanner();

    //generate a ui update signal
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateProgress()));
    timer->start(UPDATE_RATE);

    dataScene = new QGraphicsScene(this);
    ui->graphicsView->setScene(dataScene);

    middleCircle = new MiddleCircle(CIRCLE_X, CIRCLE_Y);
    dataScene->addItem(middleCircle);

    propertiesLabel = new FolderPropertiesLabel();
    dataScene->addItem(propertiesLabel);

    ui->graphicsView->setSceneRect(0, 0, FIELD_SIZE_X, FIELD_SIZE_Y);

    directoryViewModel = new QDirModel(this);
    ui->directoryView->setModel(directoryViewModel);
    //dont show collums like date and type
    //ui->directoryView->hideColumn(1);
    ui->directoryView->hideColumn(2);
    ui->directoryView->hideColumn(3);
    ui->directoryView->setColumnWidth(0,300);
    ui->rootDirectory->setText(DEFAULT_PATH);
#ifdef USE_TOTAL_DRIVE_SIZE
    VisualFolder::totalSize = scanner->totalSize;
#endif
}
MainWindow::~MainWindow()
{
    delete ui;
    delete timer;
    delete scanner;
    delete dataScene;
    delete directoryViewModel;
}
void MainWindow::on_startScanning_clicked()
{
    if((DirectoryScanner::scanState == ScanState::NOT_SCANNING) || (DirectoryScanner::scanState == ScanState::DONE))
    {
        QString path = ui->rootDirectory->text();
        scanner->startScan(path.toStdString());
        startNewScan(scanner->baseDirectory->child);
        ui->startScanning->setText("stop");
    }
    else {
        scanner->stopScan();
        ui->startScanning->setText("start");
    }
}
void MainWindow::updateProgress()
{
    if(scanner->baseDirectory != nullptr)
    {
        if(DirectoryScanner::scanState == ScanState::SCANNING)
        {
            checkForNewScannedFolders();

            int64_t currentSize = scanner->baseDirectory->currentTotalSize;
            ui->scanProgress->setValue((double)currentSize/scanner->totalSize*100.0);
        }
        if(DirectoryScanner::scanState == ScanState::DONE)
        {
            ui->scanProgress->setValue(100);//100% done
        }


        if(VisualFolder::selectedFolder != nullptr)
        {
            if(VisualFolder::selectedFolder != lastSelectedVisualFolder)//don't update again if the selected folder is not changed
            {
                ui->directoryView->collapseAll();
                lastSelectedVisualFolder = VisualFolder::selectedFolder;
                ui->directoryView->setCurrentIndex(directoryViewModel->index(QString::fromStdString(VisualFolder::selectedFolder->directory->directory.string())));
                ui->rootDirectory->setText(QString::fromStdString(VisualFolder::selectedFolder->directory->directory.string()));
            }
        }
        if(VisualFolder::rootFolder != nullptr)
        {
            if(VisualFolder::rootFolder != lastVisualRootFolder)//don't update again if the selected folder is not changed
            {
                lastVisualRootFolder = VisualFolder::rootFolder;

                startNewScan(VisualFolder::rootFolder->directory->child);
                checkForNewScannedFolders();
            }
        }
        if(middleCircle->doubleClicked)
        {
            middleCircle->doubleClicked = false;

            startNewScan(scanner->baseDirectory->child);
            checkForNewScannedFolders();
        }
    }
#ifdef RESIZE_ON_REPAINT
    ui->graphicsView->setSceneRect(0, 0, FIELD_SIZE_X, FIELD_SIZE_Y);
    ui->graphicsView->fitInView(0, 0, FIELD_SIZE_X, FIELD_SIZE_Y, Qt::KeepAspectRatio);
#endif
    repaint();
}
void MainWindow::startNewScan(DirectoryEntry* baseDirectory)
{
    currentItem = baseDirectory;
    currentSize = 0;

    VisualFolder::selectedFolder = nullptr;
    VisualFolder::rootFolder = nullptr;
    dataScene->clear();

    middleCircle = new MiddleCircle(CIRCLE_X, CIRCLE_Y);
    middleCircle->path = QString::fromStdString(scanner->baseDirectory->directory.string());
    dataScene->addItem(middleCircle);

    propertiesLabel = new FolderPropertiesLabel();
    propertiesLabel->setZValue(999999);
    dataScene->addItem(propertiesLabel);

    firstFolderScanned = false;
    cout << "new root folder"<<endl;

}
void MainWindow::checkForNewScannedFolders()
{
    clock_t begin = clock();
    bool doNextIteration = true;
    while(doNextIteration)
    {
        doNextIteration = false;
        if(currentItem != nullptr)
        {
            if(!firstFolderScanned)
            {
                if(currentItem->scanDone)
                {
                    scanFolder(currentItem,1,currentSize, -1);
                    currentSize += currentItem->directorySize;
                    firstFolderScanned = true;
                }
            }
            if(currentItem->right != nullptr)
            {
                if(currentItem->right->scanDone)
                {
                    currentItem = currentItem->right;
                    scanFolder(currentItem,1,currentSize,-1);
                    currentSize += currentItem->directorySize;
                    doNextIteration = true;
                }
            }
        }
        else {
            cout << "nullptr" << endl;
            currentItem = scanner->baseDirectory->child;
        }
        if(doNextIteration)
        {
            //prevent a not responding gui thread by
            clock_t end = clock();
            double elapsed_msecs = double(end - begin) / CLOCKS_PER_SEC*1000;
            if(elapsed_msecs > UPDATE_RATE/2)
            {
              doNextIteration = false;
            }
        }
    }
}
void MainWindow::scanFolder(DirectoryEntry* item,int dept, int64_t currentSize, int parentColor)
{
    VisualFolder* parent = visualizeFolder(item,dept, currentSize, parentColor);
    parentColor = parent->h;
    if(item->child)
    {
        if(dept < MAX_DEPT)
        {
            item = item->child;
            scanFolder(item, dept+1, currentSize,parentColor);
            currentSize += item->directorySize;
            while(item->right)
            {
                item = item->right;
                scanFolder(item, dept+1, currentSize, parentColor);
                currentSize += item->directorySize;
            }
        }

    }
}

VisualFolder* MainWindow::visualizeFolder(DirectoryEntry* item, int dept, int64_t currentSize, int parentColor)
{
    //cout << item->directory << dept <<endl;

    VisualFolder* folder = new VisualFolder(item, middleCircle, dept, currentSize, item->directorySize, parentColor);
    folder->directory = item;

#ifndef USE_TOTAL_DRIVE_SIZE
    VisualFolder::totalSize = currentSize+item->directorySize;
#endif

    dataScene->addItem(folder);
    return folder;
}



