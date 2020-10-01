//
// Created by Adar on 01/10/2020.
//

#include "row.h"

void set_row_id(Row *row, uint32_t row_id) {
    row->id = row_id;
}

void set_row_username(Row *row, const std::string &username, size_t size) {
    for (int i = 0; i < size; i++) {
        row->username[i] = username[i];
    }
    row->username[size] = '\0';
}

void set_row_email(Row *row, const std::string &email, size_t size) {
    for (int i = 0; i < size; i++) {
        row->email[i] = email[i];
    }
    row->email[size] = '\0';
}

void serialize_row(Row* source, char* destination) {

    // Flush row into memory

    memcpy(destination + ID_OFFSET, &(source->id), ID_SIZE);
    memcpy(destination + USERNAME_OFFSET, &(source->username), USERNAME_SIZE);
    memcpy(destination + EMAIL_OFFSET, &(source->email), EMAIL_SIZE);
}

void deserialize_row(char* source, Row* destination) {

    // Flush row from memory

    memcpy(&(destination->id), source + ID_OFFSET, ID_SIZE);
    memcpy(&(destination->username), source + USERNAME_OFFSET, USERNAME_SIZE);
    memcpy(&(destination->email), source + EMAIL_OFFSET, EMAIL_SIZE);
}