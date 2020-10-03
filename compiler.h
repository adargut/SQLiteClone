//
// Created by Adar on 30/09/2020.
//

#ifndef SQLITECLONE_COMPILER_H
#define SQLITECLONE_COMPILER_H

#include "utils.h"
#include "row.h"
#include "table.h"
#include "cursor.h"

typedef enum {
    PREPARE_SUCCESS,
    PREPARE_UNRECOGNIZED_STATEMENT,
    PREPARE_SYNTAX_ERROR,
    PREPARE_STRING_MAX_LENGTH_EXCEEDED,
    PREPARE_NEGATIVE_ID
} PrepareResult;

typedef enum {
    EXECUTE_TABLE_FULL,
    EXECUTE_SUCCESS,
    EXECUTE_FAILURE
} ExecuteResult;

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT
} StatementType;

typedef struct {
    StatementType type;
    Row row_to_insert;
} Statement;

PrepareResult prepare_statement(const string &input, Statement *statement);
ExecuteResult execute_statement(const string &input, Statement *statement, Table *table);

#endif //SQLITECLONE_COMPILER_H
