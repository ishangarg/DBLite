#include <iostream>
#include <cstring>
#include <cstdlib>

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

InputBuffer* new_input_buffer();
void print_startup();
void print_prompt();
void read_input(InputBuffer*);
void close_input_buffer();

int main() {
    InputBuffer* input_buffer = new_input_buffer();
    print_startup();
    while (true){
        print_prompt();
        read_input(input_buffer);
        if (strcmp(input_buffer->buffer, ".exit") == 0){
            cout << "Bye!\n";
            exit(EXIT_SUCCESS);
        }else{
            cout << "Unrecognized Command - " << input_buffer->buffer << endl;
        }

    }
    return 0;
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
