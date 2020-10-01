//
// Created by Adar on 30/09/2020.
//

#ifndef SQLITECLONE_UTILS_H
#define SQLITECLONE_UTILS_H

#include <string>
#include <iostream>
#include <vector>
#define COL_USERNAME_SIZE 32
#define COL_EMAIL_SIZE 255
#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)

struct Row {
    uint32_t id;
    char username[COL_USERNAME_SIZE];
    char email[COL_EMAIL_SIZE];
};

const uint32_t ID_SIZE = size_of_attribute(Row, id);
const uint32_t USERNAME_SIZE = size_of_attribute(Row, username);
const uint32_t EMAIL_SIZE = size_of_attribute(Row, email);
const uint32_t ID_OFFSET = 0;
const uint32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
const uint32_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
const uint32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;

void serialize_row(char *destination, Row *row) {
    memcpy(destination + ID_OFFSET, &row->id, ID_SIZE);
    memcpy(destination + USERNAME_OFFSET, &row->username, USERNAME_SIZE);
    memcpy(destination + EMAIL_OFFSET, &row->email, EMAIL_SIZE);
}

void deserialize_row(char *source, Row *row) {
    memcpy(&row->id, source + ID_OFFSET, ID_SIZE);
    memcpy(&row->username, source + USERNAME_OFFSET, USERNAME_SIZE);
    memcpy(&row->email, source + EMAIL_OFFSET, EMAIL_SIZE);
}

using string = std::string;

enum MetaCommandType {
    EXIT,
    UNRECOGNIZED
};

struct MetaCommand {
    MetaCommandType metaCommandType;
};

static inline void parse_meta_command(const string &input, MetaCommand *metaCommand) {
    if (input == ".exit") {
        metaCommand->metaCommandType = MetaCommandType::EXIT;
        return;
    }
    metaCommand->metaCommandType = MetaCommandType::UNRECOGNIZED;
}

static inline void execute_meta_command(MetaCommand *metaCommand) {
    if (metaCommand->metaCommandType == MetaCommandType::EXIT) {
        free(metaCommand);
        exit(EXIT_SUCCESS);
    }
}

#endif //SQLITECLONE_UTILS_H
