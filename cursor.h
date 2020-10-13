//
// Created by Adar on 04/10/2020.
//

#ifndef SQLITECLONE_CURSOR_H
#define SQLITECLONE_CURSOR_H

#include "utils.h"
#include "table.h"
#include "btree.h"

Cursor* table_start(Table *table);
Cursor* table_end(Table *table);
Cursor* table_find_by_id(Table *table, size_t id);
void advance_cursor(Cursor *cursor);
char *cursor_value(Cursor *cursor);

#endif //SQLITECLONE_CURSOR_H
