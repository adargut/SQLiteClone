#include <string>
#include "utils.h"
#include "compiler.h"

#pragma ide diagnostic ignored "EndlessLoop"

int main() {
    string input;
    Statement *statement;
    auto table = create_table();

    while (true) {
        std::getline(std::cin, input);

        // Input is a meta command

        if (input[0] == '.') {
            auto *metaCommand = (MetaCommand *)malloc(sizeof(MetaCommand));
            parse_meta_command(input, metaCommand);
            execute_meta_command(metaCommand);
            free(metaCommand);
        }

        // Input is a DB command such as select, insert etc.

        else {
            statement = (Statement *)malloc(sizeof(Statement));
            prepare_statement(input, statement);
            switch(execute_statement(input, statement, table)) {
                case EXECUTE_TABLE_FULL:
                    std::cout << "Error: table was full\n";
                case EXECUTE_SUCCESS:
                    std::cout << "Executed successfully\n";
            }
            free(statement);
        }
    }
}
