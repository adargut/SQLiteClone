//
// Created by Adar on 01/10/2020.
//

#ifndef SQLITECLONE_ROW_H
#define SQLITECLONE_ROW_H

#include "utils.h"

#define COL_USERNAME_SIZE 32
#define COL_EMAIL_SIZE 255
#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)

struct Row {
    uint32_t id;
    char username[COL_USERNAME_SIZE];
    char email[COL_EMAIL_SIZE];
};

const uint32_t ID_SIZE = size_of_attribute(Row, id);
const uint32_t USERNAME_SIZE = size_of_attribute(Row, username);
const uint32_t EMAIL_SIZE = size_of_attribute(Row, email);
const uint32_t ID_OFFSET = 0;
const uint32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
const uint32_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
const uint32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;

void set_row_id(Row *row, uint32_t row_id);
void set_row_username(Row *row, const std::string &username, size_t size);
void set_row_email(Row *row, const std::string &email, size_t size);
void serialize_row(Row* source, char* destination);
void deserialize_row(char* source, Row* destination);

#endif //SQLITECLONE_ROW_H
