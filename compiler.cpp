//
// Created by Adar on 30/09/2020.
//

#include "compiler.h"
#include "utils.h"

std::vector<string> split_input(const string &input) { // TODO move this to util? refactor util too
    std::vector<string> res;
    int l = 0;
    int r = 0;
    for (int i = 0; i < input.size(); i++) {
        if (input.at(i) != ' ') {
            r++;
        }
        else {
            res.push_back(input.substr(l, (r - l +1)));
            l = i + 1;
            r = l;
        }
    }
    return res;
}

PrepareResult prepare_statement(const string &input, Statement *statement) {
    auto splitted_string = split_input(input);
    int argc = splitted_string.size();

    if (argc < 3) {
        return PREPARE_SYNTAX_ERROR;
    }
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

void execute_statement(const string &input, Statement *statement) {
    switch (statement->type) {
        case STATEMENT_SELECT:
            std::cout << "Selecting..\n";
            break;
        case STATEMENT_INSERT:
            std::cout << "Inserting..\n";
            break;
        default:
            std::cout << "Unrecognized keyword or bad syntax '" << input << "'\n";
    }
}