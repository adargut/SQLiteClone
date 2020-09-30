//
// Created by Adar on 30/09/2020.
//

#ifndef SQLITECLONE_UTILS_H
#define SQLITECLONE_UTILS_H

#include <string>
#include <iostream>

using string = std::string;

enum MetaCommandType {
    EXIT,
    UNRECOGNIZED
};

struct MetaCommand {
    MetaCommandType metaCommandType;
};

void parse_meta_command(const string &input, MetaCommand *metaCommand) {
    if (input == ".exit") {
        metaCommand->metaCommandType = MetaCommandType::EXIT;
        return;
    }
    metaCommand->metaCommandType = MetaCommandType::UNRECOGNIZED;
}

void execute_meta_command(MetaCommand *metaCommand) {
    if (metaCommand->metaCommandType == MetaCommandType::EXIT) {
        free(metaCommand);
        exit(EXIT_SUCCESS);
    }
}

#endif //SQLITECLONE_UTILS_H
