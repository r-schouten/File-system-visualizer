#include "directoryscanner.h"
//https://github.com/fenbf/articles/blob/master/cpp17/filesystemTest.cpp

ScanState DirectoryScanner::scanState = ScanState::NOT_SCANNING;

DirectoryScanner::DirectoryScanner()
{
    scanState = ScanState::NOT_SCANNING;
    fs::space_info C = fs::space("C:/");
    totalSize =  C.capacity - C.free;
    stopFlag = new bool;
    *stopFlag = false;
}
DirectoryScanner::~DirectoryScanner()
{

}
void DirectoryScanner::stopScan()
{
    *stopFlag = true;
    scanState = ScanState::NOT_SCANNING;
}
void DirectoryScanner::scanTree(DirectoryEntry* baseDirectory, ScanState& _scanState)
{
    _scanState = ScanState::SCANNING;
    try
    {
        baseDirectory->scanSize();
        cout << baseDirectory->directorySize << endl;
    }
    catch (const std::exception &ex)
    {
      std::cout << "Thread exited with exception: " << ex.what() << "\n";
    }
    _scanState = ScanState::DONE;

}

void DirectoryScanner::startScan(string basePathString)
{
    if(baseDirectory)
    {
        delete baseDirectory;
    }
    *stopFlag = false;
    fs::path basePath(basePathString);
    if (fs::exists(basePath) && fs::is_directory(basePath))
    {
        baseDirectory = new DirectoryEntry(basePath, nullptr, 0, stopFlag);
        thread* scanThread = new thread(scanTree, ref(baseDirectory),ref(scanState));
        scanThread->detach();
    }
}


int64_t DirectoryScanner::getTotalSize()
{
    return totalSize;
}
