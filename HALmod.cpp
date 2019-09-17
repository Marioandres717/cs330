#include "HALmod.h"
#include <cstring>

int GetCommand(string tokens[], int &commandCounter)
{
    string commandLine;
    int tokenCount;

    do
    {
        PrintCommandPrompt(commandCounter);
        while (1)
        {
            getline(cin, commandLine);
            tokenCount = TokenizeCommandLine(tokens, commandLine);
            if (tokenCount > 0)
            {
                commandCounter++;
                break;
            }
            break;
        }
    } while (commandLine.length() == 0);

    return tokenCount;
}

void PrintCommandPrompt(int commandCounter)
{
    string terminator;
    string shellName;
    // Look for files created
    ifstream shellNameFile("shellname.txt");
    ifstream terminatorFile("terminator.txt");
    if (shellNameFile.is_open())
    {
        getline(shellNameFile, shellName);
    }
    else
    {
        // if they aren't created. create output file
        shellName = "toyshell";
        WriteToFile("shellname.txt", "toyshell");
    }

    // Two separate files are created for each property

    if (terminatorFile.is_open())
    {
        getline(terminatorFile, terminator);
    }
    else
    {
        terminator = ">";
        WriteToFile("terminator.txt", ">");
    }

    // print command prompt
    cout << shellName << "[" << commandCounter << "]" << terminator << " ";
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

    return tokenCount;
}

int ProcessCommand(string tokens[], int tokenCount, vector<string> &history)
{
    auto isATerminatingCommand = find(begin(TERMINATING_COMMANDS), end(TERMINATING_COMMANDS), tokens[0]);
    if (isATerminatingCommand == end(TERMINATING_COMMANDS))
    {
        // Add command to history
        string commandString = ReconstructCommand(tokens, tokenCount);
        AddToHistory(commandString, history);

        auto isAShellCommand = find(begin(SHELL_COMMANDS), end(SHELL_COMMANDS), tokens[0]);
        if (isAShellCommand != end(SHELL_COMMANDS))
        {
            if (tokenCount > 1)
            {
                if (tokens[0] == SHELL_COMMANDS[0])
                {
                    WriteToFile("shellname.txt", tokens[1]);
                }
                else if (tokens[0] == SHELL_COMMANDS[1])
                {
                    WriteToFile("terminator.txt", tokens[1]);
                }
            }
            else
            {
                if (tokens[0] == SHELL_COMMANDS[2])
                {
                    PrintHistory(history);
                    return 1;
                }
            }
            return 1;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        cout << endl;
        cout << "toyshell terminating ..." << endl;
        return 0;
    }
}

void AddToHistory(string commandToAdd, vector<string> &history)
{
    if (history.size() == MAX_HISTORY_COMMANDS)
    {
        history.erase(history.begin());
    }
    history.push_back(commandToAdd);
}

void PrintHistory(vector<string> &history)
{
    for (int i = 0; i < history.size(); i++)
    {
        int temp = i + 1;
        cout << temp << " " << history[i] << endl;
    }
}

void WriteToFile(string filename, string input)
{
    ofstream newfile;
    newfile.open(filename, ios::out | ios::trunc);
    newfile << input.append("\n");
    newfile.close();
}

string ReconstructCommand(string tokens[], int tokenCount)
{
    string command = "";
    for (int i = 0; i < tokenCount; i++)
    {
        command.append(tokens[i] + " ");
    }
    return command;
}