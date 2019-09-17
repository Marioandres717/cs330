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

int ProcessCommand(string tokens[], int tokenCount, vector<string> &history, map<string, string> &aliases)
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
                else if (tokens[0] == SHELL_COMMANDS[3] && tokens[1] == "|" && !tokens[2].empty())
                {
                    if (isNumber(tokens[2]))
                    {
                        int index = stoi(tokens[2]);
                        if (inRange(0, MAX_HISTORY_COMMANDS, index))
                        {
                            cout << history[index] << endl;
                        }
                    }
                }
                else if (tokens[0] == SHELL_COMMANDS[4] && !tokens[1].empty())
                {
                    if (tokens[2].empty())
                    {
                        RemoveFromAliases(tokens[1], aliases);
                    }
                    else if (tokens[2] == "|" && !tokens[3].empty())
                    {
                        AddToAliases(tokens[1], tokens[3], aliases);
                    }
                }
            }
            else
            {
                if (tokens[0] == SHELL_COMMANDS[2])
                {
                    PrintHistory(history);
                }
                else if (tokens[0] == SHELL_COMMANDS[5])
                {
                    PrintAliases(aliases);
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

bool isNumber(string tokenString)
{
    for (int i = 0; i < tokenString.length(); i++)
        if (isdigit(tokenString[i]) == false)
            return false;

    return true;
}

bool inRange(unsigned low, unsigned high, unsigned x)
{
    return ((x - low) <= (high - low));
}

void AddToAliases(string newName, string oldName, map<string, string> &aliases)
{
    if (aliases.size() <= MAX_ALIASES)
    {
        pair<map<string, string>::iterator, bool> ret;
        ret = aliases.insert(pair<string, string>(newName, oldName));
        if (ret.second == false)
        {
            aliases.erase(newName);
            aliases.insert(pair<string, string>(newName, oldName));
        }
    }
}

void RemoveFromAliases(string aliasToRemove, map<string, string> &aliases)
{
    map<string, string>::iterator it;
    it = aliases.find(aliasToRemove);
    if (it != aliases.end())
    {
        aliases.erase(aliasToRemove);
    }
}

void PrintAliases(map<string, string> &aliases)
{
    map<string, string>::iterator it;
    for (it = aliases.begin(); it != aliases.end(); ++it)
    {
        cout << it->first << " => " << it->second << "\n";
    }
}