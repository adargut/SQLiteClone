//
// Created by Adar on 30/09/2020.
//

#include "compiler.h"

PrepareResult prepare_statement(const string &input, Statement *statement) {
    auto splitted_string = split_input(input);
    int argc = splitted_string.size();

    if (input.substr(0, 6) == "select") {
        statement->type = STATEMENT_SELECT;
        statement->where_clause = false;

        if (argc != 2 && argc != 4) return PREPARE_SYNTAX_ERROR;

        if (argc == 4) {
            statement->where_clause = true;
            statement->args = (char*) malloc(CHAR_MAX); // todo make sure its freed
            memset(statement->args, 0, CHAR_MAX);
            strcpy(statement->args, splitted_string[3].c_str());
        }
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
    Cursor *cursor = table_find_by_id(table, row_id);

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

static bool id_filter(char *filter_val, Row *row) {
    return row->id == std::stoi(filter_val) == 0;
}

static bool name_filter(char *filter_val, Row *row) {
    return strcmp(filter_val, row->username) == 0;
}

static bool email_filter(char *filter_val, Row *row) {
    return strcmp(filter_val, row->email) == 0;
}

static bool pass_filter(char *filter_by, char *filter_val, Row *row) {
    if (strcmp(filter_by, "id") == 0) return id_filter(filter_val, row);
    if (strcmp(filter_by, "name") == 0) return name_filter(filter_val, row);
    if (strcmp(filter_by, "email") == 0) return email_filter(filter_val, row);

    printf("Critical Error in filter\n");
    exit(EXIT_FAILURE);
}

ExecuteResult execute_select(Statement *statement, Table *table) {
    Row *curr_row = (Row *)malloc(sizeof(Row));
    char *filter_by, *filter_val;

    // Apply where clause to select

    if (statement->where_clause) {
        filter_by = strtok(statement->args, FILTER_TOKEN);
        filter_val = strtok(NULL, FILTER_TOKEN);
    }


    // Deserialize all rows of table and print them

    Cursor *cursor = table_start(table);

    while (!cursor->end_of_table) {
        deserialize_row(cursor_value(cursor), curr_row);

        if (statement->where_clause && !pass_filter(filter_by, filter_val, curr_row)) {
            advance_cursor(cursor);
            continue;
        }

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