//
// Created by Ishan Garg on 12/08/24.
//


#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255

#define TABLE_MAX_PAGES 100



typedef struct {
    uint32_t id;
    char username[COLUMN_USERNAME_SIZE];
    char email[COLUMN_EMAIL_SIZE];
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


const uint32_t PAGE_SIZE = 4096;
const uint32_t ROWS_PER_PAGE = PAGE_SIZE/ROW_SIZE;
const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

typedef struct Table{
    uint32_t num_rows;
    void* pages[TABLE_MAX_PAGES];
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
    void* page = table->pages[page_num];
    if (page== nullptr){
        page = table->pages[page_num] = malloc(PAGE_SIZE);
    }
    uint32_t row_offset = row_num % ROWS_PER_PAGE;
    uint32_t byte_offset = row_offset * ROW_SIZE;
    return static_cast<uint32_t*>(page) + byte_offset;
}

Table* new_table(){
    Table* table = new Table();
    table->num_rows = 0;
    for(uint32_t i = 0; i<TABLE_MAX_PAGES; i++){
        table->pages[i] = nullptr;
    }
    return table;
}

void free_table(Table* table){
    for(int i = 0; table->pages[i]; i++){
        free(table->pages[i]);
    }
    free(table);
}

void print_row(Row* row){
    std::cout << "(" << row->id << ", " << row->username << ", " << row->email << ")" << std::endl;
}