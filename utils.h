//
// Created by Adar on 30/09/2020.
//

#ifndef SQLITECLONE_UTILS_H
#define SQLITECLONE_UTILS_H

#include <string>
#include <iostream>
#include <vector>
#include <cstring>
#include <io.h>
#include <fcntl.h>
#include <cstdlib>

// Constants, typedefs, macros and general functions useful for more than one module implemented below

#define TABLE_MAX_PAGES 100
#define COL_USERNAME_SIZE 32
#define COL_EMAIL_SIZE 255
#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)

struct Row {
    uint32_t id;
    char username[COL_USERNAME_SIZE + 1]; // TODO fix insertion of too long usernames
    char email[COL_EMAIL_SIZE + 1];
};

typedef struct {
    int fd;
    uint32_t file_length;
    char *pages[TABLE_MAX_PAGES];
} Pager;

struct Table {
    uint32_t num_rows;
    Pager *pager;
};

typedef struct {
    Table *table;
    size_t row_num;
    bool end_of_table;
} Cursor;

const uint32_t ID_SIZE = size_of_attribute(Row, id);
const uint32_t USERNAME_SIZE = size_of_attribute(Row, username);
const uint32_t EMAIL_SIZE = size_of_attribute(Row, email);

const uint32_t ID_OFFSET = 0;
const uint32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
const uint32_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
const uint32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;

const uint32_t PAGE_SIZE = 4096;
const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

using string = std::string;

std::vector<string> split_input(const string &input);

#endif //SQLITECLONE_UTILS_H
