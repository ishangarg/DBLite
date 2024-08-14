#include <iostream>
#include <cstring>
#include <cstdlib>
#include "table.h"

using namespace std;


struct InputBuffer {
    char* buffer;
    size_t buffer_length;
    ssize_t input_length;

    InputBuffer() : buffer(nullptr), buffer_length(0), input_length(0){};
    ~InputBuffer() {
        free(buffer);  // Free the allocated buffer
    }
};

typedef enum {
    EXECUTE_SUCCESS,
    EXECUTE_TABLE_FULL
} ExecuteResult;

typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum {
    PREPARE_SUCCESS,
    PREPARE_SYNTAX_ERROR,
    PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT
} StatementType;

typedef struct {
    StatementType type;
    Row row_to_insert;
} Statement;



InputBuffer* new_input_buffer();
void print_startup();
void print_prompt();
void read_input(InputBuffer*);
void close_input_buffer();
MetaCommandResult do_meta_command(InputBuffer*);
PrepareResult prepare_statement(InputBuffer*, Statement*);
void execute_statement(Statement*);
ExecuteResult execute_command(Statement*, Table*);




int main() {
    Table* table = new_table();
    InputBuffer* input_buffer = new_input_buffer();
    print_startup();
    while (true){
        print_prompt();
        read_input(input_buffer);
//        if (strcmp(input_buffer->buffer, ".exit") == 0){
//            cout << "Bye!\n";
//            exit(EXIT_SUCCESS);
//        }else{
//            cout << "Unrecognized Command - " << input_buffer->buffer << endl;
//        }

        if (input_buffer->buffer[0] == '.'){
            switch (do_meta_command(input_buffer)) {
                case META_COMMAND_SUCCESS:
                    continue;
                case META_COMMAND_UNRECOGNIZED_COMMAND:
                    cout << "Unrecognized Command - " << input_buffer->buffer << endl;
                    continue;
            }
        }

        Statement statement;
        switch (prepare_statement(input_buffer, &statement)) {
            case (PREPARE_SUCCESS):
                break;
            case (PREPARE_SYNTAX_ERROR):
                cout << "Syntax error! Could not parse statement" << endl;
                continue;
            case (PREPARE_UNRECOGNIZED_STATEMENT):
                cout << "Unrecognized keyword at start of " << input_buffer->buffer << endl;
                        continue;

        }

        switch (execute_command(&statement, table)) {
            case (EXECUTE_SUCCESS):
                cout << "Executed" << endl;
                continue;
            case (EXECUTE_TABLE_FULL):
                cout << "Error: Table Full" << endl;
                continue;
        }

    }
}

InputBuffer* new_input_buffer(){
    InputBuffer* input_buffer = new InputBuffer();
    return  input_buffer;
}

void print_startup(){
    cout << "DBList V0.0" << endl;
    cout << "Enter .help for usage" << endl;
    cout << "Author: Ishan Garg" << endl;
    cout << "MIT License" << endl;
    cout << "-------------------------------";
    cout << endl;
}

void print_prompt(){
    cout << "db > ";
}

void read_input(InputBuffer* input_buffer){
    ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

    if (bytes_read <= 0){
        cout << "Error reading input\n";
        exit(EXIT_FAILURE);
    }

    input_buffer->input_length = bytes_read - 1;
    input_buffer->buffer[bytes_read-1] = 0;
}

void close_input_buffer(InputBuffer* input_buffer){
    delete input_buffer->buffer;
    delete input_buffer;
}

MetaCommandResult do_meta_command(InputBuffer* input_buffer){
    if(strcmp(input_buffer->buffer, ".exit") == 0){
        cout << "Bye!" << endl;
        exit(EXIT_SUCCESS);
    }else{
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement){
    if(strncmp(input_buffer->buffer, "insert", 6) == 0){
        statement->type = STATEMENT_INSERT;
        int args_assigned = sscanf(input_buffer->buffer, "insert %d %s %s", &(statement->row_to_insert.id),
                                   statement->row_to_insert.username, statement->row_to_insert.email);
        if (args_assigned < 3){
            return PREPARE_SYNTAX_ERROR;
        }
        return PREPARE_SUCCESS;
    }
    if(strcmp(input_buffer->buffer, "select") == 0){
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }

    return PREPARE_UNRECOGNIZED_STATEMENT;
}

void execute_statement(Statement* statement){
    switch (statement->type) {
        case (STATEMENT_INSERT):
            cout << "This is where we do Insert" << endl;
            break;
        case (STATEMENT_SELECT):
            cout << "This is where we do Select" << endl;
            break;
    }
}

ExecuteResult execute_insert(Statement* statement, Table* table){
    if (table->num_rows >= TABLE_MAX_ROWS){
        return EXECUTE_TABLE_FULL;
    }
    Row* row_to_insert = &(statement->row_to_insert);
    serialize_row(row_to_insert, row_slot(table, table->num_rows));
    table->num_rows += 1;
    return EXECUTE_SUCCESS;
}

ExecuteResult execute_select(Statement* statement, Table* table){
    Row row;
    for(uint32_t i = 0; i<table->num_rows; i++){
        deserialize_row(row_slot(table, i), &row);
        print_row(&row);
    }
    return EXECUTE_SUCCESS;
}

ExecuteResult  execute_command(Statement* statement, Table* table){
    switch (statement->type) {
        case STATEMENT_INSERT:
            return execute_insert(statement, table);
        case STATEMENT_SELECT:
            return execute_select(statement, table);
    }
}

