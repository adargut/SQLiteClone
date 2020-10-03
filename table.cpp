//
// Created by Adar on 01/10/2020.
//

#include "table.h"

void free_table(Table *table) {
    for (auto &page : table->pager->pages) if (page) free(page);
    free(table->pager);
    free(table);
}

Table *db_open(const char* filename) {
    auto table = (Table *)malloc(sizeof(Table));
    auto pager = pager_open(filename);

    table->pager = pager;
    table->num_rows = pager->file_length / ROW_SIZE;

    return table;
}

void db_close(Table *table) {
    uint32_t num_full_pages = table->num_rows / ROWS_PER_PAGE;
    auto pager = table->pager;

    // Flush all cached pages to disc

    for (int i = 0; i < num_full_pages; i++) {
        if (pager->pages[i]) {
            pager_flush(pager, i, PAGE_SIZE);
            free(pager->pages[i]);
            pager->pages[i] = nullptr;
        }
    }

    // Flush partial pages to disc

    uint32_t num_additional_rows = table->num_rows % ROWS_PER_PAGE;
    if (num_additional_rows > 0) {
        uint32_t page_num = num_full_pages;
        if (pager->pages[page_num]) {
            pager_flush(pager, page_num, num_additional_rows * ROW_SIZE);
            free(pager->pages[page_num]);
            pager->pages[page_num] = nullptr;
            }
        }

    // Close fd and free all allocated memory

    int err = close(pager->fd);
    if (err == -1) {
        std::cout << "Critical Error: could not close db file\n";
        exit(EXIT_FAILURE);
    }
    free_table(table);
}

char *cursor_value(Cursor *cursor) {
    uint32_t row_num = cursor->row_num;
    uint32_t page_num = row_num / ROWS_PER_PAGE;
    auto page = get_page(cursor->table->pager, page_num);

    // Compute offset for Row in page

    uint32_t row_offset = row_num % ROWS_PER_PAGE;
    uint32_t byte_offset = row_offset * ROW_SIZE;

    return page + byte_offset;
}