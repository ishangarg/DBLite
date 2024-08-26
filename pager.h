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

void* get_page(Pager* pager, uint32_t page_num){

    if (page_num > TABLE_MAX_PAGES){
        cout << "Page Number Out of Bounds\n";
        exit(EXIT_FAILURE);
    }

    if(pager->pages[page_num] == nullptr){
        void* page = malloc(PAGE_SIZE);
        uint32_t num_pages = pager->file_length/PAGE_SIZE;

        if(pager->file_length%PAGE_SIZE){
            num_pages += 1;
        }

        if (page_num <= num_pages){
            lseek(pager->file_descriptor, page_num*PAGE_SIZE, SEEK_SET);
            ssize_t bytes_read = read(pager->file_descriptor, page, PAGE_SIZE);
            if (bytes_read == -1){
                cout << "Error Reading File\n";
                exit(EXIT_FAILURE);
            }
        }

        pager->pages[page_num] = page;

    }

    return pager->pages[page_num];

}

#endif //DBLITE_PAGER_H
