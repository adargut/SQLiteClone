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
p
char* leaf_node_value(char* node, uint32_t cell_num) {
    return leaf_node_key(node, cell_num) + LEAF_NODE_KEY_SIZE;
}

void initialize_leaf_node(char* node) {
    *(leaf_node_num_cells(node)) = 0;
}