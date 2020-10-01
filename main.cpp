#include <string>
#include "utils.h"
#include "compiler.h"
#include "meta_compiler.h"
#pragma ide diagnostic ignored "EndlessLoop" // TODO remove this after finishing

int main() {
    string input;
    Statement statement;
    auto table = create_table();

    start:
    while (true) {
        std::getline(std::cin, input);

        // Input is a meta command

        if (input[0] == '.') {
            auto metaCommand = (MetaCommand *)malloc(sizeof(MetaCommand));
            parse_meta_command(input, metaCommand);
            execute_meta_command(metaCommand); // TODO free table on exit
            free(metaCommand);
        }

        // Input is a DB command such as select, insert etc.

        else {
            auto error = prepare_statement(input, &statement);
            if (error == PREPARE_SYNTAX_ERROR) {
                std::cout << "Error: syntax error\n";
                goto start;
            }
            if (error == PREPARE_UNRECOGNIZED_STATEMENT) {
                std::cout << "Error: unrecognized statement\n";
                goto start;
            }
            switch(execute_statement(input, &statement, table)) {
                case EXECUTE_TABLE_FULL:
                    std::cout << "Error: table was full\n";
                case EXECUTE_SUCCESS:
                    std::cout << "Executed successfully\n";
            }
        }
    }
}
