//
// Created by Ishan Garg on 17/08/24.
//
// Header file to have all filesystem files
//
//


#include "table.h"

#ifndef DBLITE_PAGER_H
#define DBLITE_PAGER_H

#include <cstdint>

typedef struct {
    int file_descriptor;
    uint32_t file_length;
    void* pages[TABLE_MAX_PAGES];

} Pager;

#endif //DBLITE_PAGER_H
