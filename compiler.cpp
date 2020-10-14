//
// Created by Adar on 30/09/2020.
//

#include "compiler.h"

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
        if (statement->row_to_insert.id < 0) return PREPARE_NEGATIVE_ID;
        set_row_username(&statement->row_to_insert, splitted_string[2], splitted_string[2].size());
        if (strlen(statement->row_to_insert.username) > COL_USERNAME_SIZE) return PREPARE_STRING_MAX_LENGTH_EXCEEDED;
        set_row_email(&statement->row_to_insert,  splitted_string[3], splitted_string[3].size());
        if (strlen(statement->row_to_insert.email) > COL_EMAIL_SIZE) return PREPARE_STRING_MAX_LENGTH_EXCEEDED;

        return PREPARE_SUCCESS;
    }
    return PREPARE_UNRECOGNIZED_STATEMENT;
}

ExecuteResult execute_insert(Statement *statement, Table *table) {
    auto node = get_page(table->pager, table->root_page_num);
    uint32_t num_cells = *leaf_node_num_cells(node);

    size_t row_id = statement->row_to_insert.id;
    Cursor *cursor = table_find_by_id(table, row_id); //table_end(table); TODO remove table_end?

    // Check for duplicate id before insertion

    if (cursor->cell_num < num_cells) {
        if (*leaf_node_key(node, cursor->cell_num) == row_id) return EXECUTE_DUPLICATE_KEY;
    }

    // Serialize Row into virtual memory

    Row* row_to_insert = &(statement->row_to_insert);
    leaf_node_insert(cursor, row_to_insert->id, row_to_insert);

    free(cursor);
    return EXECUTE_SUCCESS;
}

ExecuteResult execute_select(Statement *statement, Table *table) {
    Row *curr_row = (Row *)malloc(sizeof(Row));

    // Deserialize all rows of table and print them

    Cursor *cursor = table_start(table);

    while (!cursor->end_of_table) {
        deserialize_row(cursor_value(cursor), curr_row);
        string row_data = std::to_string(curr_row->id) + " " + curr_row->username + " " + curr_row->email;
        std::cout << row_data << std::endl;
        advance_cursor(cursor);
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
            return EXECUTE_FAILURE;
    }
}