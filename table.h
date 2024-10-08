//
// Created by Ishan Garg on 12/08/24.
//

#include "constants.h"
#include "pager.h"

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255


typedef struct {
    uint32_t id;
    char username[COLUMN_USERNAME_SIZE + 1];
    char email[COLUMN_EMAIL_SIZE + 1];
} Row;


template <typename T, typename U>
constexpr std::size_t size_of_attribute(U T::* member) {
    return sizeof(((T*)0)->*member);
}

const uint32_t ID_SIZE = size_of_attribute(&Row::id);
const uint32_t USERNAME_SIZE = size_of_attribute(&Row::username);
const uint32_t EMAIL_SIZE = size_of_attribute(&Row::email);

const uint32_t ID_OFFSET = 0;
const uint32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
const uint32_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
const uint32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;


const uint32_t ROWS_PER_PAGE = PAGE_SIZE/ROW_SIZE;
const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

typedef struct Table{
    uint32_t num_rows;
    Pager* pager;
};

void serialize_row(Row* row, void* destination){
    memcpy(static_cast<uint32_t*>(destination) + ID_OFFSET, &(row->id), ID_SIZE);
    memcpy(static_cast<uint32_t*>(destination)+USERNAME_OFFSET, &(row->username), USERNAME_SIZE);
    memcpy(static_cast<uint32_t*>(destination)+EMAIL_OFFSET, &(row->email), EMAIL_SIZE);
}

void deserialize_row(void* source, Row* destination){
    memcpy(&(destination->id), static_cast<uint32_t*>(source) + ID_OFFSET, ID_SIZE);
    memcpy(&(destination->username),static_cast<uint32_t*>(source) + USERNAME_OFFSET, USERNAME_SIZE);
    memcpy(&(destination->email), static_cast<uint32_t*>(source) + EMAIL_OFFSET, EMAIL_SIZE);
}

void* row_slot(Table* table, uint32_t row_num){
    uint32_t page_num = row_num/ROWS_PER_PAGE;
    void* page = get_page(table->pager, page_num);
    uint32_t row_offset = row_num % ROWS_PER_PAGE;
    uint32_t byte_offset = row_offset * ROW_SIZE;
    return static_cast<uint32_t*>(page) + byte_offset;
}

Table* db_open(const char* filename){
    Pager* pager = pager_open(filename);
    uint32_t num_rows = pager->file_length/ROW_SIZE;
    Table* table = new Table();
    table->pager = pager;
    table->num_rows = num_rows;
    return table;
}

void db_close(Table* table){
    Pager* pager = table->pager;
    uint32_t num_full_pages = table->num_rows/ROWS_PER_PAGE;

    for(uint32_t i =0; i<num_full_pages; i++){
        if (pager->pages[i] == nullptr){
            continue;
        }
        pager_flush(pager, i, PAGE_SIZE); /*TODO: PagerFlush function*/
        free(pager->pages[i]);
        pager->pages[i] == nullptr;
    }

}

//void free_table(Table* table){
//    for(int i = 0; table->pages[i]; i++){
//        free(table->pages[i]);
//    }
//    free(table);
//}

void print_row(Row* row){
    std::cout << "(" << row->id << ", " << row->username << ", " << row->email << ")" << std::endl;
}