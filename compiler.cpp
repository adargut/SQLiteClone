//
// Created by Adar on 30/09/2020.
//

#include "compiler.h"
#include "utils.h"

PrepareResult prepare_statement(const string &input, Statement *statement) {
    auto splitted_string = split_input(input);
    int argc = splitted_string.size();

    if (input.substr(0, 6) == "select") {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    if (input.substr(0, 6) == "insert") {
        if (argc < 3) {
            return PREPARE_SYNTAX_ERROR;
        }
        statement->type = STATEMENT_INSERT;

        // Fill Row with data from insert

        set_row_id(&statement->row_to_insert, std::stoi(splitted_string[1]));
        set_row_username(&statement->row_to_insert, splitted_string[2], splitted_string[2].size());
        set_row_email(&statement->row_to_insert,  splitted_string[3], splitted_string[3].size());

        return PREPARE_SUCCESS;
    }
    return PREPARE_UNRECOGNIZED_STATEMENT;
}

ExecuteResult execute_insert(Statement *statement, Table *table) {
    if (table->num_rows == TABLE_MAX_ROWS) return EXECUTE_TABLE_FULL;

    // Serialize Row into virtual memory

    Row row_to_insert = statement->row_to_insert;
    serialize_row(&row_to_insert, locate_row_in_memory(table, table->num_rows));
    table->num_rows++;

    return EXECUTE_SUCCESS;
}

ExecuteResult execute_select(Statement *statement, Table *table) { // TODO fix select or insert? not working
    Row *curr_row = (Row *)malloc(sizeof(Row));

    // Deserialize all rows of table and print them

    for (int i = 0; i < table->num_rows; i++) {
        deserialize_row(locate_row_in_memory(table, i), curr_row);
        string row_data = std::to_string(curr_row->id) + " " + curr_row->username + " " + curr_row->email;
        std::cout << row_data << std::endl;
    }
    free(curr_row);
    return EXECUTE_SUCCESS;
}

ExecuteResult execute_statement(const string &input, Statement *statement, Table *table) {
    switch (statement->type) {
        case STATEMENT_SELECT:
            return execute_select(statement, table);
        case STATEMENT_INSERT:
            return execute_insert(statement, table);
        default:
            std::cout << "Unrecognized keyword or bad syntax '" << input << "'\n";
    }
}