#include "directoryentry.h"
//#define DEBUG
int64_t DirectoryEntry::currentTotalSize = 0;
DirectoryEntry* DirectoryEntry::lastItem = nullptr;

DirectoryEntry::DirectoryEntry(fs::path directory, DirectoryEntry* parent, int depth)
    :directory(directory),parent(parent),depth(depth){}
inline int64_t DirectoryEntry::ComputeFileSize(fs::path pathToCheck)
{
    auto err = std::error_code{};

    auto filesize = fs::file_size(pathToCheck, err);

    return filesize;
}
void DirectoryEntry::scanSize()
{
    auto err = std::error_code{};
    DirectoryEntry* tail = child;
    for (const fs::directory_entry entry : fs::directory_iterator(directory, fs::directory_options::skip_permission_denied, err))
    {
        fs::path PATH = entry.path();
        try {
            if (fs::is_directory(entry.status()))
            {
    #ifdef DEBUG
                cout << entry.path() << "[+] " << endl;
    #endif
                if(entry.path().string().length()>=240)break;//program crashed is path is longer than 260 chars
                if(child == nullptr)
                {
                    child = new DirectoryEntry(PATH, this,depth+1);
                    child->scanSize();
                    directorySize += child->directorySize;
                    tail = child;
                }
                else {
                    DirectoryEntry* toAdd = new DirectoryEntry(entry.path(), this,depth+1);
                    tail->right = toAdd;
                    toAdd->left = tail;
                    toAdd->scanSize();
                    tail = tail->right;
                    directorySize += toAdd->directorySize;
                }
            }
            else if(fs::is_regular_file(entry.status()))
            {
    #ifdef DEBUG
                cout << entry.path() << "[|] "   << ComputeFileSize(entry) <<endl;
    #endif
                int64_t size = ComputeFileSize(entry.path());
                directorySize += size;
                currentTotalSize += size;
            }
            else
            {
                //cout << entry.path() << "[?]" << endl;
            }
        } catch (fs::filesystem_error error) {
            //cout <<  error.what() << "   " << error.path1() << endl;
        }
    }
    if(depth < 2)
    {
        //cout << "   " << directory <<" "<< directorySize << endl;
    }

    scanDone = true;
    previous = lastItem;
    if(lastItem!=nullptr)lastItem->next = this;
    lastItem = this;
}
