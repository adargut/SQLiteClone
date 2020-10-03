//
// Created by Adar on 01/10/2020.
//

#ifndef SQLITECLONE_TABLE_H
#define SQLITECLONE_TABLE_H

#include "utils.h"
#include "pager.h"

struct Table {
    uint32_t num_rows;
    Pager *pager;
};

Table *db_open(const char* filename);
void db_close(Table *table);
void free_table(Table *table);
char *locate_row_in_memory(Table *table, uint32_t row_num);

#endif //SQLITECLONE_TABLE_H
