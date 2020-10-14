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

uint32_t* internal_node_num_keys(char* node) {
    return reinterpret_cast<uint32_t *>(node + INTERNAL_NODE_NUM_KEYS_OFFSET);
}

uint32_t* internal_node_right_child(char* node) {
    return reinterpret_cast<uint32_t *>(node + INTERNAL_NODE_RIGHT_CHILD_OFFSET);
}

uint32_t* internal_node_cell(char* node, uint32_t cell_num) {
    return reinterpret_cast<uint32_t *>(node + INTERNAL_NODE_HEADER_SIZE + cell_num * INTERNAL_NODE_CELL_SIZE);
}

uint32_t* internal_node_child(char* node, uint32_t child_num) {
    uint32_t num_children = *internal_node_num_keys(node);

    if (child_num > num_children) {
        std::cout << "Critical Error: tried to access child num wrongfully\n";
        exit(EXIT_FAILURE);
    }
    else if (child_num == num_children) {
        return internal_node_right_child(node);
    }
    return internal_node_cell(node, child_num);
}

uint32_t* internal_node_key(char* node, uint32_t key_num) {
    return internal_node_cell(node, key_num) + INTERNAL_NODE_CHILD_SIZE;
}

static uint32_t get_node_max_key(char* node) {

    // Since we sort by keys, max key is always last

    if (get_node_type(node) == NODE_INTERNAL) {
        return *internal_node_key(node, *internal_node_num_keys(node) - 1);
    }
    return *leaf_node_key(node, *leaf_node_num_cells(node) - 1);
}

char* leaf_node_value(char* node, uint32_t cell_num) {
    return leaf_node_key(node, cell_num) + LEAF_NODE_KEY_SIZE;
}

bool is_node_root(const char* node) {
    uint8_t value = *((uint8_t*)(node + IS_ROOT_OFFSET));
    return (bool)value;
}

void set_node_root(char* node, bool is_root) {
    uint8_t value = is_root;
    *((uint8_t*)(node + IS_ROOT_OFFSET)) = value;
}

void initialize_leaf_node(char* node) {
    set_node_type(node, NODE_LEAF);
    set_node_root(node, false);
    *(leaf_node_num_cells(node)) = 0;
}

void leaf_node_insert(Cursor* cursor, uint32_t key, Row* value) {
    auto pager = cursor->table->pager;
    auto node = pager->pages[cursor->page_num];

    uint32_t num_cells = *leaf_node_num_cells(node);
    if (num_cells >= LEAF_NODE_MAX_CELLS) {

        // Node is full

        leaf_node_split_insert(cursor, key, value);
        return;
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

void initialize_internal_node(char* node) {
    set_node_type(node, NODE_INTERNAL);
    set_node_root(node, false);
    *internal_node_num_keys(node) = 0;
}

static void create_new_root(Table *table, uint32_t right_page_num) {

    // Handle splitting of root: old root copied to new page, becomes left child

    auto root = get_page(table->pager, table->root_page_num);
    auto left_page_num = get_unused_page(table->pager);
    auto left_child = get_page(table->pager, left_page_num);

    // Copy root to left child

    memcpy(left_child, root, PAGE_SIZE);
    set_node_root(left_child, false);

    // Initialize root page

    initialize_internal_node(root);
    set_node_root(root, true);
    *internal_node_num_keys(root) = 1;
    *internal_node_child(root, 0) = left_page_num;
    uint32_t left_child_max_key = get_node_max_key(left_child);
    *internal_node_key(root, 0) = left_child_max_key;
    *internal_node_right_child(root) = right_page_num;
}

void leaf_node_split_insert(Cursor* cursor, uint32_t key, Row* value) {

    // Create a new node then insert the upper halves into it

    auto old_node = get_page(cursor->table->pager, cursor->page_num);
    uint32_t new_page_num = get_unused_page(cursor->table->pager); // TODO implement me!
    auto new_node = get_page(cursor->table->pager, new_page_num);
    initialize_leaf_node(new_node);

    // Divide keys between old and new node

    for (int32_t i = LEAF_NODE_MAX_CELLS; i >= 0; i--) {
        char* destination_node;
        if (i >= LEAF_NODE_LEFT_SPLIT_COUNT) {
            destination_node = new_node;
            } else {
            destination_node = old_node;
            }
        uint32_t index_within_node = i % LEAF_NODE_LEFT_SPLIT_COUNT;
        char* destination = leaf_node_cell(destination_node, index_within_node);

        if (i == cursor->cell_num) {
            serialize_row(value, destination);
            } else if (i > cursor->cell_num) {
            memcpy(destination, leaf_node_cell(old_node, i - 1), LEAF_NODE_CELL_SIZE);
            } else {
            memcpy(destination, leaf_node_cell(old_node, i), LEAF_NODE_CELL_SIZE);
            }
        }

    // Update cell count on both leaf nodes

    *(leaf_node_num_cells(old_node)) = LEAF_NODE_LEFT_SPLIT_COUNT;
    *(leaf_node_num_cells(new_node)) = LEAF_NODE_RIGHT_SPLIT_COUNT;

    if (is_node_root(old_node)) {
        return create_new_root(cursor->table, new_page_num);
    }
    else {
        printf("Implement updating parent after split\n");
        exit(EXIT_SUCCESS);
    }
}