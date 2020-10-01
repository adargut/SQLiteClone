//
// Created by Adar on 01/10/2020.
//

#include "table.h"

Table *create_table() {
    auto table = (Table *)malloc(sizeof(Table));
    table->num_rows = 0;
    for (auto &page : table->pages) page = nullptr;

    return table;
}

void free_table(Table *table) {
    for (int i = 0; i < table->num_rows;) free(table->pages[i++]);
    free(table);
}

char *locate_row_in_memory(Table *table, uint32_t row_num) {
    uint32_t page_num = row_num / ROWS_PER_PAGE;
    char *page = table->pages[page_num];

    // Allocate memory for page only when we try to access it

    if (page == nullptr) {
        page = table->pages[page_num] = (char *)malloc(PAGE_SIZE);
    }

    // Compute offset for Row in page

    uint32_t row_offset = row_num % ROWS_PER_PAGE;
    uint32_t byte_offset = row_offset * ROW_SIZE;

    return page + byte_offset;
}