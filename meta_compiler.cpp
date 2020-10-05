//
// Created by Adar on 02/10/2020.
//

#include "meta_compiler.h"

void parse_meta_command(const string &input, MetaCommand *metaCommand) {
    if (input == ".exit") {
        metaCommand->metaCommandType = MetaCommandType::EXIT;
        return;
    }
    if (input == ".constants") {
        metaCommand->metaCommandType = MetaCommandType::CONSTANTS;
        return;
    }
    metaCommand->metaCommandType = MetaCommandType::UNRECOGNIZED;
}

void print_constants() {
    printf("ROW_SIZE: %d\n", ROW_SIZE);
    printf("COMMON_NODE_HEADER_SIZE: %d\n", COMMON_NODE_HEADER_SIZE);
    printf("LEAF_NODE_HEADER_SIZE: %d\n", LEAF_NODE_HEADER_SIZE);
    printf("LEAF_NODE_CELL_SIZE: %d\n", LEAF_NODE_CELL_SIZE);
    printf("LEAF_NODE_SPACE_FOR_CELLS: %d\n", LEAF_NODE_SPACE_FOR_CELLS);
    printf("LEAF_NODE_MAX_CELLS: %d\n", LEAF_NODE_MAX_CELLS);
}

void execute_meta_command(MetaCommand *metaCommand, Table *table) {
    if (metaCommand->metaCommandType == MetaCommandType::EXIT) {
        db_close(table);
        free(metaCommand);
        exit(EXIT_SUCCESS);
    }
    if (metaCommand->metaCommandType == MetaCommandType::CONSTANTS) {
        print_constants();
        free(metaCommand);
    }
}