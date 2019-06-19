#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scanner = new DirectoryScanner();


    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateProgress()));
    timer->start(UPDATE_RATE);

    dataScene = new QGraphicsScene(this);
    ui->graphicsView->setScene(dataScene);

    middleCircle = new MiddleCircle(CIRCLE_X, CIRCLE_Y);
    middleCircle->setZValue(999999);
    dataScene->addItem(middleCircle);

    ui->graphicsView->setSceneRect(0, 0, FIELD_SIZE_X, FIELD_SIZE_Y);

    directoryViewModel = new QDirModel(this);
    ui->directoryView->setModel(directoryViewModel);
    //dont show collums like data and type
    //ui->directoryView->hideColumn(1);
    ui->directoryView->hideColumn(2);
    ui->directoryView->hideColumn(3);

    ui->rootDirectory->setText(DEFAULT_PATH);
#ifdef USE_TOTAL_DRIVE_SIZE
    VisualFolder::totalSize = scanner->totalSize;
#endif
}
void MainWindow::on_startScanning_clicked()
{
    if((DirectoryScanner::scanState == ScanState::NOT_SCANNING) || (DirectoryScanner::scanState == ScanState::DONE))
    {
        QString path = ui->rootDirectory->text();
        scanner->startScan(path.toStdString());
        currentItem = scanner->baseDirectory->child;
        middleCircle->path = QString::fromStdString(scanner->baseDirectory->directory.string());
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

                currentItem = VisualFolder::rootFolder->directory->child;
                QString path = QString::fromStdString(VisualFolder::rootFolder->directory->directory.string());
                if(currentItem == nullptr)cout << "current item is nullptr"<<endl;
                currentSize = 0;
                cout << "new root folder"<<endl;

                dataScene->clear();

                middleCircle = new MiddleCircle(CIRCLE_X, CIRCLE_Y);
                middleCircle->setZValue(999999);
                middleCircle->path = path;

                dataScene->addItem(middleCircle);
            }
        }
        if(middleCircle->doubleClicked)
        {
            middleCircle->doubleClicked = false;
            currentItem = scanner->baseDirectory->child;
            currentSize = 0;
            cout << "new root folder reset"<<endl;

            dataScene->clear();

            middleCircle = new MiddleCircle(CIRCLE_X, CIRCLE_Y);
            middleCircle->setZValue(999999);
            dataScene->addItem(middleCircle);

            middleCircle->path = QString::fromStdString(scanner->baseDirectory->directory.string());

        }
    }
#ifdef RESIZE_ON_REPAINT
    ui->graphicsView->setSceneRect(0, 0, FIELD_SIZE_X, FIELD_SIZE_Y);
    ui->graphicsView->fitInView(0, 0, FIELD_SIZE_X, FIELD_SIZE_Y, Qt::KeepAspectRatio);
#endif
    repaint();
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
            if(currentItem->right != nullptr)
            {
                if(currentItem->right->scanDone)
                {
                    currentItem = currentItem->right;
                    scanFolder(currentItem,1,currentSize);
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
            clock_t end = clock();
            double elapsed_msecs = double(end - begin) / CLOCKS_PER_SEC*1000;
            if(elapsed_msecs > UPDATE_RATE/2)
            {
              doNextIteration = false;
            }
        }
    }
}
void MainWindow::scanFolder(DirectoryEntry* item,int dept, int64_t currentSize)
{
    visualizeFolder(item,dept, currentSize);
    if(item->child != nullptr)
    {
        if(dept < MAX_DEPT)
        {
            scanFolder(item->child, dept+1, currentSize);
            currentSize += item->child->directorySize;
            item = item->child;
            while(item->right!=nullptr)
            {
                scanFolder(item->right, dept+1, currentSize);
                currentSize += item->right->directorySize;
                item = item->right;
            }
        }

    }
}
void MainWindow::visualizeFolder(DirectoryEntry* item, int dept, int64_t currentSize)
{
    cout << item->directory << dept <<endl;

    VisualFolder* folder = new VisualFolder(item, middleCircle, dept, currentSize, item->directorySize);
    folder->directory = item;

#ifndef USE_TOTAL_DRIVE_SIZE
    VisualFolder::totalSize = currentSize+item->directorySize;
#endif

    dataScene->addItem(folder);
}
MainWindow::~MainWindow()
{
    delete ui;
}


