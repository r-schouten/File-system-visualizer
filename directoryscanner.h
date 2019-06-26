#pragma once

#include "directoryentry.h"
#include <thread>
#include <iostream>
#include <string>

#ifdef EXPERIMENTAL

    #include <experimental/filesystem>
    namespace fs = std::experimental::filesystem::v1;

#else
    #include <filesystem>
    namespace fs = std::filesystem;
#endif
using namespace std;

enum class ScanState
{
    NOT_SCANNING,
    SCANNING,
    DONE
};
class DirectoryScanner
{
public:
    DirectoryScanner();
    ~DirectoryScanner();
    void startScan(string directory);
    void stopScan();
    DirectoryEntry* baseDirectory = nullptr;

    static ScanState scanState;

    int64_t getTotalSize();
    int64_t totalSize;
private:
    static thread scanThread;
    bool *stopFlag;
    static void scanTree(DirectoryEntry *baseDirectory, ScanState &scanState);

};

