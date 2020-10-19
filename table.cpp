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
    table->root_page_num = 0;

    // db is an empty tree, initialize its root as page 0

    if (pager->num_pages == 0) {
        char* root_node = get_page(pager, 0);
        initialize_leaf_node(root_node);
        set_node_root(root_node, true);
    }

    return table;
}

void db_close(Table *table) {
    auto pager = table->pager;

    // Flush all cached pages to disc

    for (int i = 0; i < pager->num_pages; i++) {
        if (pager->pages[i]) {
            pager_flush(pager, i);
            free(pager->pages[i]);
            pager->pages[i] = nullptr;
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