//
// Created by Adar on 04/10/2020.
//

#include "cursor.h"

Cursor* table_start(Table *table) {
    auto cursor = table_find_by_id(table, 0);

    auto node = get_page(table->pager, cursor->page_num);
    size_t num_cells = *leaf_node_num_cells(node);
    cursor->end_of_table = (num_cells == 0);

    return cursor;
}

Cursor* table_find_by_id(Table *table, size_t id) {
    auto root_node = get_page(table->pager, table->root_page_num);

    if (get_node_type(root_node) == NODE_LEAF) {
        return leaf_node_find(table, table->root_page_num, id);
    }
    return internal_node_find(table, table->root_page_num, id);
}

void advance_cursor(Cursor *cursor) {
    if (cursor->end_of_table) {
        return;
    }

    auto pager = cursor->table->pager;
//    auto root_node = get_page(pager, cursor->table->root_page_num);
    auto node = get_page(cursor->table->pager, cursor->page_num);
    uint32_t max_cells = *leaf_node_num_cells(node);
    cursor->cell_num++;

    // Check if cursor reached end of current leaf

    if (cursor->cell_num >= max_cells) {
        uint32_t next_leaf_page_num = *leaf_node_next_leaf(node);

        // No next leaf, cursor is done

        if (next_leaf_page_num == 0) {
            cursor->end_of_table = true;
            return;
        }

        // Next leaf exists, we advance cursor towards it

        cursor->cell_num = 0;
        cursor->page_num = next_leaf_page_num;
    }
}

char *cursor_value(Cursor *cursor) {
    uint32_t page_num = cursor->page_num;
    auto page = get_page(cursor->table->pager, page_num);

    return leaf_node_value(page, cursor->cell_num);
}