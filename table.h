//
// Created by Adar on 01/10/2020.
//

#ifndef SQLITECLONE_TABLE_H
#define SQLITECLONE_TABLE_H

#include "utils.h"
#include "pager.h"
#include "table.h"

Table *db_open(const char* filename);
void db_close(Table *table);
void free_table(Table *table);
char *cursor_value(Cursor *cursor);

#endif //SQLITECLONE_TABLE_H
