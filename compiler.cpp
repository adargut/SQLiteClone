//
// Created by Adar on 30/09/2020.
//

#include "compiler.h"
#include "utils.h"

PrepareResult prepare_statement(const string &input, Statement *statement) {
    if (input.substr(0, 6) == "insert") {
        statement->type = STATEMENT_INSERT;
        return PREPARE_SUCCESS;
    }
    if (input.substr(0, 6) == "select") {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    return PREPARE_UNRECOGNIZED_STATEMENT;
}

void execute_statement(Statement *statement) {
    switch (statement->type) {
        case STATEMENT_SELECT:
            std::cout << "Selecting..\n";
            break;
        case STATEMENT_INSERT:
            std::cout << "Inserting..\n";
            break;
        default:
            break;
    }
}