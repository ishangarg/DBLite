//
// Created by Ishan Garg on 17/08/24.
//
// Header file to have all filesystem files
//
//

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstdlib>

#include "constants.h"


#ifndef DBLITE_PAGER_H
#define DBLITE_PAGER_H

#include <cstdint>

using namespace std;

typedef struct {
    int file_descriptor;
    uint32_t file_length;
    void* pages[TABLE_MAX_PAGES];

} Pager;

Pager* pager_open(const char* filename){
    int fd = open(filename, O_RDWR | O_CREAT,  S_IWUSR | S_IRUSR);
    if (fd == -1){
        cerr << "Unable to open file\n";
        exit(EXIT_FAILURE);
    }
    off_t file_length = lseek(fd, 0, SEEK_END);
    Pager* pager = new Pager();
    pager->file_descriptor = fd;
    pager->file_length = file_length;

    for(uint32_t i = 0; i<TABLE_MAX_PAGES; i++){
        pager->pages[i] = nullptr;
    }

    return pager;

}

#endif //DBLITE_PAGER_H
