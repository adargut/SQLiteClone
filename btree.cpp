//
// Created by Adar on 04/10/2020.
//

#include "btree.h"

uint32_t* leaf_node_num_cells(char* node) {
    return (uint32_t *)(node + LEAF_NODE_NUM_CELLS_OFFSET);
}

uint32_t* leaf_node_next_leaf(char* node) {
    return (uint32_t *)(node + LEAF_NODE_NEXT_LEAF_OFFSET);
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
    return reinterpret_cast<uint32_t *>((char *) internal_node_cell(node, key_num) + INTERNAL_NODE_CHILD_SIZE);
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

char* node_parent(char* node) {
    return node + PARENT_POINTER_OFFSET;
}

void initialize_leaf_node(char* node) {
    set_node_type(node, NODE_LEAF);
    set_node_root(node, false);
    *(leaf_node_num_cells(node)) = 0;
    *(leaf_node_next_leaf(node)) = 0;
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

    auto cursor = (Cursor *)malloc(sizeof(Cursor));
    cursor->page_num = page_num;
    cursor->table = table;
    cursor->end_of_table = false;
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

uint32_t internal_node_find_child(char* node, uint32_t key) {

    // Binary search to find key in node

    auto num_keys = *internal_node_num_keys(node);
    size_t l = 0;
    size_t r = num_keys;

    while (l <= r) {
        size_t mid = (l + r) / 2;
        auto key_to_compare = *internal_node_key(node, mid);
        if (key >= key_to_compare) {
            r = mid ;
        }
        else {
            l = mid + 1;
        }
    }
    return l;
}

Cursor* internal_node_find(Table* table, uint32_t root_page_num, uint32_t key) {
    auto node = get_page(table->pager, root_page_num);
    auto child_idx = internal_node_find_child(node, key);
    auto child_num = *internal_node_child(node, child_idx);
    auto child = get_page(table->pager, child_num);

    // Check type of node to decide which search function to call recursively

    if (get_node_type(child) == NODE_INTERNAL) {
        return internal_node_find(table, *node, key);
    }
    return leaf_node_find(table, *node, key);
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

    // Set parent pointers correctly

    auto right_child = get_page(table->pager, right_page_num);
    *node_parent(left_child) = table->root_page_num;
    *node_parent(right_child) = table->root_page_num;
}

static void update_internal_node(Table* table, char* node, size_t old_key, size_t new_key) {
    auto old_key_pos = internal_node_find_child(node, old_key);
    *internal_node_key(node, old_key_pos) = new_key;
}

static void internal_node_insert(Table *table, uint32_t parent_page_num, uint32_t child_page_num) {

    // Add a new child/key pair to a parent that corresponds to a child

    auto parent = get_page(table->pager, parent_page_num);
    auto child = get_page(table->pager, child_page_num);

    auto child_max_key = get_node_max_key(child);
    auto max_key_pos = internal_node_find_child(parent, child_max_key);

    // Parent has one more new key

    auto parent_num_keys = *internal_node_num_keys(parent);
    *internal_node_num_keys(parent) = parent_num_keys + 1;

    if (parent_num_keys >= INTERNAL_NODE_MAX_CELLS) {
        printf("Critical Error: need to implement internal node split\n");
        exit(EXIT_FAILURE);
    }

    auto right_child_page_num = *internal_node_right_child(parent);
    auto right_child = get_page(table->pager, right_child_page_num);

    // Case new child would be rightmost child

    if (child_max_key > get_node_max_key(right_child)) {
        *internal_node_child(parent, parent_num_keys) = right_child_page_num;
        *internal_node_key(parent, parent_num_keys) = get_node_max_key(right_child);
        *internal_node_right_child(parent) = child_page_num;
        return;
    }

    // Case new child is a "sandwich" child somewhere between left and right

    for (int i = *internal_node_num_keys(parent); i > max_key_pos; i--) {

        // Shift cells right

        auto dest = internal_node_cell(parent, i);
        auto src = internal_node_cell(parent, i - 1);
        memcpy(dest, src, INTERNAL_NODE_CELL_SIZE);
    }

    // Set new child in parent where in the correct index

    *internal_node_child(parent, max_key_pos) = child_page_num;
    *internal_node_key(parent, max_key_pos) = child_max_key;
}

void leaf_node_split_insert(Cursor* cursor, uint32_t key, Row* value) {

    // Create a new node then insert the upper halves into it

    auto old_node = get_page(cursor->table->pager, cursor->page_num);

    // Get max val in node

    auto node_max_val = get_node_max_key(old_node);

    uint32_t new_page_num = get_unused_page(cursor->table->pager);
    auto new_node = get_page(cursor->table->pager, new_page_num);
    initialize_leaf_node(new_node);
    *node_parent(new_node) = *old_node;

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
            serialize_row(value, leaf_node_value(destination_node, index_within_node));
            *leaf_node_key(destination_node, index_within_node) = key;
            } else if (i > cursor->cell_num) {
            memcpy(destination, leaf_node_cell(old_node, i - 1), LEAF_NODE_CELL_SIZE);
            } else {
            memcpy(destination, leaf_node_cell(old_node, i), LEAF_NODE_CELL_SIZE);
            }
        }

    // Update cell count on both leaf nodes

    *(leaf_node_num_cells(old_node)) = LEAF_NODE_LEFT_SPLIT_COUNT;
    *(leaf_node_num_cells(new_node)) = LEAF_NODE_RIGHT_SPLIT_COUNT;

    // Update next leaf pointers as relevant

    *(leaf_node_next_leaf(new_node)) = *(leaf_node_next_leaf(old_node));
    *(leaf_node_next_leaf(old_node)) = new_page_num;

    // Handle case of splitting root

    if (is_node_root(old_node)) {
        return create_new_root(cursor->table, new_page_num);
    }
    else {
        auto parent_page_num = *node_parent(old_node);
        auto new_max = get_node_max_key(old_node);
        auto parent_node = get_page(cursor->table->pager, parent_page_num);
//        update_internal_node_key(parent_node, old_max, new_max);
        internal_node_insert(cursor->table, parent_page_num, new_page_num);
    }
}