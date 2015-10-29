#ifndef FILE_FUNCTIONS_H
#define FILE_FUNCTIONS_H

std::set<std::string, std::greater<std::string>> files;

int dirTree(const char *pathname, const struct stat* sbuf, int type,
        struct FTW* ftwb) {

    if (type == FTW_F) files.insert(std::string(pathname));

    return 0;
}

void fileTraverse(const std::string& path) {

    int flags = 0;
    if (nftw(path.c_str(), dirTree, 10, flags) == -1) {
        throw "EXCEPTION: file traversal error";
    }
}

#endif
