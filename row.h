//
// Created by Adar on 01/10/2020.
//

#ifndef SQLITECLONE_ROW_H
#define SQLITECLONE_ROW_H

#include "utils.h"

void set_row_id(Row *row, uint32_t row_id);
void set_row_username(Row *row, const std::string &username, size_t size);
void set_row_email(Row *row, const std::string &email, size_t size);
void serialize_row(Row* source, char* destination);
void deserialize_row(char* source, Row* destination);

#endif //SQLITECLONE_ROW_H
