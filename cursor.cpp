//
// Created by Adar on 04/10/2020.
//

#include "cursor.h"

Cursor* table_start(Table *table) {
    auto cursor = (Cursor *)malloc(sizeof(Cursor));
    cursor->table = table;
    cursor->end_of_table = (table->num_rows == 0);
    cursor->row_num = 0;

    return cursor;
}

Cursor* table_end(Table *table) {
    auto cursor = (Cursor *)malloc(sizeof(Cursor));
    cursor->table = table;
    cursor->end_of_table = true;
    cursor->row_num = table->num_rows;

    return cursor;
}

void advance_cursor(Cursor *cursor) {
    cursor->row_num++;
    cursor->end_of_table = (cursor->row_num == cursor->table->num_rows);
}