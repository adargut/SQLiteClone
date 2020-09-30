#include <string>
#include "utils.h"
#include "compiler.h"

int main() {
    string input;
    Statement *statement;
    while (true) {
        std::cin >> input;

        // Input is a meta command

        if (input[0] == '.') {
            MetaCommand *metaCommand = (MetaCommand *)malloc(sizeof(MetaCommand));
            parse_meta_command(input, metaCommand);
            execute_meta_command(metaCommand);
            free(metaCommand);
        }
        else {
            statement = (Statement *)malloc(sizeof(Statement));
            prepare_statement(input, statement);
            execute_statement(statement);
            free(statement);
        }
    }
    end:
    return 0;
}
