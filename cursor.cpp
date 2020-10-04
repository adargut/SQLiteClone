//
// Created by Adar on 04/10/2020.
//

#include "cursor.h"

Cursor* table_start(Table *table) {
    auto cursor = (Cursor *)malloc(sizeof(Cursor));
    cursor->table = table;
    cursor->page_num = table->root_page_num;
    cursor->cell_num = 0;

    auto root_node = get_page(table->pager, table->root_page_num);
    uint32_t num_cells = *leaf_node_num_cells(root_node);
    cursor->end_of_table = (num_cells == 0);

    return cursor;
}

Cursor* table_end(Table *table) {
    auto cursor = (Cursor *)malloc(sizeof(Cursor));
    cursor->table = table;
    cursor->end_of_table = true;

    cursor->page_num = table->root_page_num;
    auto root_node = get_page(table->pager, table->root_page_num);
    cursor->cell_num = *leaf_node_num_cells(root_node);

    return cursor;
}

void advance_cursor(Cursor *cursor) {
    if (cursor->end_of_table) {
        // TODO handle this case
        return;
    }

    auto pager = cursor->table->pager;
    auto root_node = get_page(pager, cursor->table->root_page_num);
    uint32_t max_cells = *leaf_node_num_cells(root_node);
    cursor->cell_num++;

    if (cursor->cell_num >= max_cells) {
        cursor->end_of_table = true;
    }
}

char *cursor_value(Cursor *cursor) {
    uint32_t page_num = cursor->page_num;
    auto page = get_page(cursor->table->pager, page_num);

    return leaf_node_value(page, cursor->cell_num)
}