#include <string>
#include "utils.h"
#include "compiler.h"
#include "meta_compiler.h"
#pragma ide diagnostic ignored "EndlessLoop" // TODO remove this after finishing

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Critical Error: must provide db name\n";
        exit(EXIT_FAILURE);
    }

    auto table = db_open(argv[1]);
    string input;
    Statement statement;

    start:
    while (true) {
        std::getline(std::cin, input);

        // Input is a meta command

        if (input[0] == '.') {
            auto metaCommand = (MetaCommand *)malloc(sizeof(MetaCommand));
            parse_meta_command(input, metaCommand);
            execute_meta_command(metaCommand, table);
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
            if (error == PREPARE_STRING_MAX_LENGTH_EXCEEDED) {
                std::cout <<"Error: string given is too long\n";
                goto start;
            }
            if (error == PREPARE_NEGATIVE_ID) {
                std::cout << "Error: id must be a positive integer\n";
                goto start;
            }
            switch(execute_statement(input, &statement, table)) {
                case EXECUTE_TABLE_FULL:
                    std::cout << "Error: table was full\n";
                    break;
                case EXECUTE_SUCCESS:
                    std::cout << "Executed successfully\n";
                    break;
                case EXECUTE_DUPLICATE_KEY:
                    std::cout << "Error: cannot insert duplicate id into table\n";
                    break;
                case EXECUTE_FAILURE:
                    std::cout << "Execution failed\n";
                    break;
            }
        }
    }
}
