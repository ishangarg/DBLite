//
// Created by Ishan Garg on 12/08/24.
//

typedef struct {
    uint32_t id;
    char username[COLUMN_USERNAME_SIZE];
    char email[COLUMN_EMAIL_SIZE];
} Row;

#define size_of_attribute(Struct, Attribute) sizeof(((Struct)*0))->Attribute)
const uint32_t ID_SIZE = size_of_attribute(Row, id);
