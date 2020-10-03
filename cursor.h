//
// Created by Adar on 04/10/2020.
//

#ifndef SQLITECLONE_CURSOR_H
#define SQLITECLONE_CURSOR_H

#include "utils.h"
#include "table.h"

Cursor* table_start(Table *table);
Cursor* table_end(Table *table);
void advance_cursor(Cursor *cursor);

#endif //SQLITECLONE_CURSOR_H
