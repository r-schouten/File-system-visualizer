#pragma once

#include <iostream>
#include <string>
#include <sstream>
#ifdef EXPERIMENTAL

    #include <experimental/filesystem>
    namespace fs = std::experimental::filesystem::v1;

#else
    #include <filesystem>
    namespace fs = std::filesystem;
#endif
using namespace std;
class DirectoryEntry
{
public:

    int64_t directorySize = 0;
    static int64_t currentTotalSize;
    static DirectoryEntry* lastItem;
    int depth = 0;
    bool scanDone = false;
    void scanSize();
    DirectoryEntry* parent = nullptr;
    DirectoryEntry* child = nullptr;
    DirectoryEntry* left = nullptr;
    DirectoryEntry* right = nullptr;

    DirectoryEntry* next = nullptr;
    DirectoryEntry* previous = nullptr;
    fs::path directory;
    DirectoryEntry(fs::path directory, DirectoryEntry *parent, int depth);
private:
    int64_t ComputeFileSize(fs::path pathToCheck);
};

