//
// Created by Adar on 04/10/2020.
//

#include "btree.h"

uint32_t* leaf_node_num_cells(char* node) {
    return (uint32_t *)(node + LEAF_NODE_NUM_CELLS_OFFSET);
}

char* leaf_node_cell(char* node, uint32_t cell_num) {
    return node + LEAF_NODE_HEADER_SIZE + LEAF_NODE_CELL_SIZE * cell_num;
}

char* leaf_node_key(char* node, uint32_t cell_num) {
    return leaf_node_cell(node, cell_num);
}

char* leaf_node_value(char* node, uint32_t cell_num) {
    return leaf_node_key(node, cell_num) + LEAF_NODE_KEY_SIZE;
}

void initialize_leaf_node(char* node) {
    set_node_type(node, NODE_LEAF);
    *(leaf_node_num_cells(node)) = 0;
}

void leaf_node_insert(Cursor* cursor, uint32_t key, Row* value) {
    auto pager = cursor->table->pager;
    auto node = pager->pages[cursor->page_num];

    uint32_t num_cells = *leaf_node_num_cells(node);
    if (num_cells >= LEAF_NODE_MAX_CELLS) {

        // Node is full

        printf("Here we will put split node functionality..\n");
        exit(EXIT_FAILURE);
    }

    if (cursor->cell_num < num_cells) {
        for (uint32_t i = num_cells; i > cursor->cell_num; i--) {

            // Make room for new cell

            memcpy(leaf_node_cell(node, i), leaf_node_cell(node, i-1), LEAF_NODE_CELL_SIZE);
        }
    }

    *leaf_node_num_cells(node) += 1;
    *leaf_node_key(node, cursor->cell_num) = key;
    serialize_row(value, leaf_node_value(node, cursor->cell_num));
}

void print_leaf_node(char* node) {
    auto num_cells = *leaf_node_num_cells(node);
    printf("Number of cells for node: %d\n", num_cells);
    for (uint32_t i = 0; i < num_cells; i++) {
        printf("Key of cell %d is %d\n", i, *leaf_node_key(node, i));
    }
}

Cursor* leaf_node_find(Table* table, uint32_t page_num, size_t id) {
    auto root = get_page(table->pager, page_num);
    size_t num_cells = *leaf_node_num_cells(root);

    Cursor *cursor = (Cursor *)malloc(sizeof(Cursor));
    cursor->page_num = page_num;
    cursor->table = table;
    size_t l = 0, r = num_cells;

    // Perform binary search to find leaf node

    while (l < r) {
        size_t mid = (l + r) / 2;
        size_t key_at_mid = *leaf_node_key(root, mid);
        if (key_at_mid == id) {
            cursor->cell_num = mid;
            return cursor;
        }
        else if (key_at_mid > id) {
            r = mid;
        }
        else l = mid + 1;
    }
    cursor->cell_num = l;
    // TODO this if clause should never be true, probably can be removed after testing
    if (cursor->cell_num == -1) {
        std::cout << "Critical Error: binary search implementation is wrong\n";
        exit(EXIT_SUCCESS);
    }
    return cursor;
}

NodeType get_node_type(const char* node) {
    uint8_t value = *((uint8_t*)(node + NODE_TYPE_OFFSET));
    return (NodeType)value;
}

void set_node_type(char* node, NodeType nodeType) {

    // Cast to 8 bits in order to make sure node type is stored in one continuous byte

    uint8_t type = nodeType;
    *((uint8_t*)(node + NODE_TYPE_OFFSET)) = type;
}