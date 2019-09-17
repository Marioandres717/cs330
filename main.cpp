#include "HALmod.h"

// Part 1 -
// a) white space " "
// b) 1) dynamic allocation, 2) Increased readability

int main()
{
    string tokens[MAX_COMMAND_LINE_ARGUMENTS];
    int tokenCount;
    int commandCounter = 1;
    string terminator = ">";

    do
    {
        tokenCount = GetCommand(tokens, commandCounter, terminator);
    } while (ProcessCommand(tokens, tokenCount));

    return 0;
}
