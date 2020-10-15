//
// Created by Adar on 04/10/2020.
//

#ifndef SQLITECLONE_BTREE_H
#define SQLITECLONE_BTREE_H

#include "utils.h"
#include "row.h"

// Common node header layout

const uint32_t NODE_TYPE_SIZE = sizeof(uint8_t);
const uint32_t NODE_TYPE_OFFSET = 0;
const uint32_t IS_ROOT_SIZE = sizeof(uint8_t);
const uint32_t IS_ROOT_OFFSET = NODE_TYPE_SIZE;
const uint32_t PARENT_POINTER_SIZE = sizeof(uint32_t);
const uint32_t PARENT_POINTER_OFFSET = IS_ROOT_OFFSET + IS_ROOT_SIZE;
const uint8_t COMMON_NODE_HEADER_SIZE = NODE_TYPE_SIZE + IS_ROOT_SIZE + PARENT_POINTER_SIZE;

// Leaf node header layout

const uint32_t LEAF_NODE_NUM_CELLS_SIZE = sizeof(uint32_t);
const uint32_t LEAF_NODE_NUM_CELLS_OFFSET = COMMON_NODE_HEADER_SIZE;
const uint32_t LEAF_NODE_NEXT_LEAF_SIZE = sizeof(uint32_t);
const uint32_t LEAF_NODE_NEXT_LEAF_OFFSET = LEAF_NODE_NUM_CELLS_OFFSET + LEAF_NODE_NUM_CELLS_SIZE;
const uint32_t LEAF_NODE_HEADER_SIZE = COMMON_NODE_HEADER_SIZE + LEAF_NODE_NUM_CELLS_SIZE + LEAF_NODE_NEXT_LEAF_SIZE;

// Leaf node body layout

const uint32_t LEAF_NODE_KEY_SIZE = sizeof(uint32_t);
const uint32_t LEAF_NODE_KEY_OFFSET = 0;
const uint32_t LEAF_NODE_VALUE_SIZE = ROW_SIZE;
const uint32_t LEAF_NODE_VALUE_OFFSET = LEAF_NODE_KEY_OFFSET + LEAF_NODE_KEY_SIZE;
const uint32_t LEAF_NODE_CELL_SIZE = LEAF_NODE_KEY_SIZE + LEAF_NODE_VALUE_SIZE;
const uint32_t LEAF_NODE_SPACE_FOR_CELLS = PAGE_SIZE - LEAF_NODE_HEADER_SIZE;
const uint32_t LEAF_NODE_MAX_CELLS = LEAF_NODE_SPACE_FOR_CELLS / LEAF_NODE_CELL_SIZE;

// Internal Node Header Layout

const uint32_t INTERNAL_NODE_NUM_KEYS_SIZE = sizeof(uint32_t);
const uint32_t INTERNAL_NODE_NUM_KEYS_OFFSET = COMMON_NODE_HEADER_SIZE;
const uint32_t INTERNAL_NODE_RIGHT_CHILD_SIZE = sizeof(uint32_t);
const uint32_t INTERNAL_NODE_RIGHT_CHILD_OFFSET =   INTERNAL_NODE_NUM_KEYS_OFFSET +
                                                    INTERNAL_NODE_NUM_KEYS_SIZE;
const uint32_t INTERNAL_NODE_HEADER_SIZE =  COMMON_NODE_HEADER_SIZE +
                                            INTERNAL_NODE_NUM_KEYS_SIZE +
                                            INTERNAL_NODE_RIGHT_CHILD_SIZE;

// Internal node body layout

const uint32_t INTERNAL_NODE_KEY_SIZE = sizeof(uint32_t);
const uint32_t INTERNAL_NODE_CHILD_SIZE = sizeof(uint32_t);
const uint32_t INTERNAL_NODE_CELL_SIZE = INTERNAL_NODE_CHILD_SIZE + INTERNAL_NODE_KEY_SIZE;

// Split load between nodes

const uint32_t LEAF_NODE_RIGHT_SPLIT_COUNT = (LEAF_NODE_MAX_CELLS + 1) / 2;
const uint32_t LEAF_NODE_LEFT_SPLIT_COUNT = (LEAF_NODE_MAX_CELLS + 1) - LEAF_NODE_RIGHT_SPLIT_COUNT;

// See https://cstack.github.io/db_tutorial/assets/images/leaf-node-format.png for graphic description of layout

typedef enum {
    NODE_INTERNAL,
    NODE_LEAF
} NodeType;

// Declarations to help us from outside
char *get_page(Pager *pager, int page_num);
uint32_t get_unused_page(Pager *pager);

// TODO some of these can be made static to make header cleaner
uint32_t* leaf_node_num_cells(char* node);
char* leaf_node_cell(char* node, uint32_t cell_num);
char* leaf_node_key(char* node, uint32_t cell_num);
Cursor* leaf_node_find(Table* table, uint32_t page_num, size_t id);
NodeType get_node_type(const char* node);
void set_node_type(char* node, NodeType nodeType);
char* leaf_node_value(char* node, uint32_t cell_num);
void leaf_node_split_insert(Cursor* cursor, uint32_t key, Row* value);
void initialize_leaf_node(char* node);
void leaf_node_insert(Cursor* cursor, uint32_t key, Row* value);
void print_leaf_node(char* node);
void set_node_root(char* node, bool is_root);
Cursor* internal_node_find(Table* table, uint32_t root_page_num, uint32_t key);
uint32_t* leaf_node_next_leaf(char* node);

#endif //SQLITECLONE_BTREE_H
