//
// Created by Adar on 30/09/2020.
//

#ifndef SQLITECLONE_COMPILER_H
#define SQLITECLONE_COMPILER_H

#include "utils.h"

typedef enum { PREPARE_SUCCESS, PREPARE_UNRECOGNIZED_STATEMENT, PREPARE_SYNTAX_ERROR } PrepareResult;
typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;

typedef struct {
    StatementType type;
    Row row_to_insert;
} Statement;

PrepareResult prepare_statement(const string &input, Statement *statement);
void execute_statement(const string &input, Statement *statement);

#endif //SQLITECLONE_COMPILER_H
