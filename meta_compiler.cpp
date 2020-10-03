//
// Created by Adar on 02/10/2020.
//

#include "meta_compiler.h"

void parse_meta_command(const string &input, MetaCommand *metaCommand) {
    if (input == ".exit") {
        metaCommand->metaCommandType = MetaCommandType::EXIT;
        return;
    }
    metaCommand->metaCommandType = MetaCommandType::UNRECOGNIZED;
}

void execute_meta_command(MetaCommand *metaCommand, Table *table) {
    if (metaCommand->metaCommandType == MetaCommandType::EXIT) {
        db_close(table);
        free(metaCommand);
        exit(EXIT_SUCCESS);
    }
}