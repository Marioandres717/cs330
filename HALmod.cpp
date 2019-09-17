#include "HALmod.h"
#include <cstring>

int GetCommand(string tokens[], int &commandCounter, string &terminator)
{
    string commandLine;
    int tokenCount;
    do
    {
        cout << "toyshell[" << commandCounter << "]" << terminator;
        while (1)
        {
            getline(cin, commandLine);
            tokenCount = TokenizeCommandLine(tokens, commandLine);
            if (tokenCount)
            {
                commandCounter++;
                break;
            }
            break;
        }
    } while (commandLine.length() == 0);

    return tokenCount;
}

int TokenizeCommandLine(string tokens[], string commandLine)
{
    char *token[MAX_COMMAND_LINE_ARGUMENTS];
    char *workCommandLine = new char[commandLine.length() + 1];
    int i;
    int tokenCount;

    for (i = 0; i < MAX_COMMAND_LINE_ARGUMENTS; i++)
    {
        tokens[i] = "";
    }
    strcpy(workCommandLine, commandLine.c_str());
    i = 0;
    if ((token[i] = strtok(workCommandLine, " ")) != NULL)
    {
        i++;
        while ((token[i] = strtok(NULL, " ")) != NULL)
        {
            i++;
        }
    }
    tokenCount = i;

    for (i = 0; i < tokenCount; i++)
    {
        tokens[i] = token[i];
    }

    delete[] workCommandLine;
    cout << tokenCount << endl;

    return tokenCount;
}

int ProcessCommand(string tokens[], int tokenCount)
{
    auto isATerminatingCommand = find(begin(TERMINATING_COMMANDS), end(TERMINATING_COMMANDS), tokens[0]);
    if (isATerminatingCommand == end(TERMINATING_COMMANDS))
    {
        cout << tokenCount << endl;
        if (tokenCount > 1)
        {
            cout << "HALshell: " << tokens[0] << " does not require any arguments" << endl;
            return 1;
        }
        return 1;
    }
    else
    {
        cout << endl;
        cout << "toyshell terminating ..." << endl;
        return 0;
    }
}

void PrintReverse(char **cTokens, int tokenCount)
{
    for (int i = tokenCount - 1; i >= 0; i--)
    {
        cout << cTokens[i] << endl;
    }
}