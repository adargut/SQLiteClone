//
// Created by Adar on 01/10/2020.
//

#ifndef SQLITECLONE_TABLE_H
#define SQLITECLONE_TABLE_H

#include "utils.h"

#define TABLE_MAX_PAGES 100

const uint32_t PAGE_SIZE = 4096;
const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

struct Table {
    uint32_t num_rows;
    char *pages[TABLE_MAX_PAGES];
};

Table *create_table();
void free_table(Table *table);
char *locate_row_in_memory(Table *table, uint32_t row_num);

#endif //SQLITECLONE_TABLE_H
