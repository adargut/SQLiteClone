//
// Created by Adar on 02/10/2020.
//

#ifndef SQLITECLONE_META_COMPILER_H
#define SQLITECLONE_META_COMPILER_H

#include "utils.h"
#include "table.h"

enum MetaCommandType {
    EXIT,
    CONSTANTS,
    UNRECOGNIZED
};

struct MetaCommand {
    MetaCommandType metaCommandType;
};

void parse_meta_command(const string &input, MetaCommand *metaCommand);
void execute_meta_command(MetaCommand *metaCommand, Table *table);

#endif //SQLITECLONE_META_COMPILER_H
