#include "HALmod.h"

int main()
{
    string tokens[MAX_COMMAND_LINE_ARGUMENTS];
    int tokenCount;
    int commandCounter = 1;
    vector<string> history;

    do
    {
        tokenCount = GetCommand(tokens, commandCounter);
    } while (ProcessCommand(tokens, tokenCount, history));

    return 0;
}
