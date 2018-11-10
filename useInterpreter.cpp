//**************************  useInterpreter.cpp   ***********************

#include "interpreter.h"

int main() {
    Statement statement;
    cout << "The program processes statements of the following format:\n"
         << "\t<id> = <expr>;\n\tprint <id>\n\tstatus\n\tend\n\n";
    while (true)                  // This infinite loop is broken by exit(1)
        statement.getStatement(); // in getStatement() or upon finding 
    return 0;                     // an error.
}
