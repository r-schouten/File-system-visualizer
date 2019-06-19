#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scanner = new DirectoryScanner();
    scanner->startScan("C:/");
    currentItem = scanner->baseDirectory->child;

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateProgress()));
    timer->start(50);

    dataScene = new QGraphicsScene(this);
    ui->graphicsView->setScene(dataScene);

    middleCircle = new MiddleCircle(300, 300);
    middleCircle->setZValue(999999);
    dataScene->addItem(middleCircle);

    ui->graphicsView->setSceneRect(0, 0, 600, 600);

    directoryViewModel = new QDirModel(this);
    ui->directoryView->setModel(directoryViewModel);
    ui->directoryView->hideColumn(1);
    ui->directoryView->hideColumn(2);

    ui->directoryView->hideColumn(3);

}

void MainWindow::updateProgress()
{
    if(scanner->baseDirectory != nullptr)
    {
        checkForNewScannedFolders();

        uint64_t currentSize = scanner->baseDirectory->currentTotalSize;
        //qDebug("%lld   %lld",currentSize,scanner->totalSize);
        ui->scanProgress->setValue((double)currentSize/scanner->totalSize*100.0);

        if(VisualFolder::selectedFolder != nullptr)
        {
            if(VisualFolder::selectedFolder != lastSelectedVisualFolder)//don't update again if the selected folder is not changed
            {
                lastSelectedVisualFolder = VisualFolder::selectedFolder;
                ui->directoryView->setCurrentIndex(directoryViewModel->index(QString::fromStdString(VisualFolder::selectedFolder->directory->directory.string())));

                ui->lineEdit->setText(QString::fromStdString(VisualFolder::selectedFolder->directory->directory.string()));
            }
        }
    }
    repaint();
}
double startPercentage = 0;
void MainWindow::checkForNewScannedFolders()
{
    bool doNextIteration = true;
    while(doNextIteration)
    {
        doNextIteration = false;
        if(currentItem != nullptr)
        {
            if((currentItem->right)&&(currentItem->right->scanDone))
            {
                currentItem = currentItem->right;
                scanFolder(currentItem,1,startPercentage);
                startPercentage += (double)currentItem->directorySize/scanner->totalSize*100.0;
                doNextIteration = true;
            }
        }
        else {
            cout << "nullptr" << endl;
            currentItem = scanner->baseDirectory->child;
        }
    }
}
void MainWindow::scanFolder(DirectoryEntry* item,int dept, double startPercentage)
{
    visualizeFolder(item,dept, startPercentage);
    if(item->child != nullptr)
    {
        if(dept < 4)
        {
            scanFolder(item->child, dept+1, startPercentage);
            startPercentage += (double)item->child->directorySize/scanner->totalSize*100.0;
            item = item->child;
            while(item->right!=nullptr)
            {
                scanFolder(item->right, dept+1, startPercentage);
                startPercentage += (double)item->right->directorySize/scanner->totalSize*100.0;
                item = item->right;
            }
        }

    }
}
void MainWindow::visualizeFolder(DirectoryEntry* item, int dept, double startPercentage)
{
    cout << item->directory << dept <<endl;
    int hue = (int)(startPercentage*10)%359;
    double percentage = (double)item->directorySize/scanner->totalSize*100.0;
    if(percentage < 0.2)return;//file is to small to visualize

    VisualFolder* folder = new VisualFolder(item,300, 300, dept, hue);
    folder->startPercentage = startPercentage;
    folder->percentage = percentage;
    folders.append(folder);
    dataScene->addItem(folder);
}
//VisualFolder* MainWindow::scanAllDoneFolders(VisualFolder* currentFolder)
//{

//}
MainWindow::~MainWindow()
{
    delete ui;
}
