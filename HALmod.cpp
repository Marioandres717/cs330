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

        // check if command is a predefined shell command
        auto isAShellCommand = find(begin(SHELL_COMMANDS), end(SHELL_COMMANDS), tokens[0]);
        if (isAShellCommand != end(SHELL_COMMANDS))
        {
            if (tokenCount > 1)
            {
                // SET SHELLNAME
                if (tokens[0] == SHELL_COMMANDS[0])
                {
                    WriteToFile("shellname.txt", tokens[1]);
                }
                // SET TERMINATOR
                else if (tokens[0] == SHELL_COMMANDS[1])
                {
                    WriteToFile("terminator.txt", tokens[1]);
                }
                // execute command in history (! | [n])
                else if (tokens[0] == SHELL_COMMANDS[3] && tokens[1] == "|" && !tokens[2].empty())
                {
                    if (isNumber(tokens[2]))
                    {
                        int index = stoi(tokens[2]);
                        if (inRange(0, MAX_HISTORY_COMMANDS, index))
                        {
                            tokenCount = TokenizeCommandLine(tokens, history[index]);
                            ProcessCommand(tokens, tokenCount, history, aliases);
                        }
                    }
                }
                // NEW NAME
                else if (tokens[0] == SHELL_COMMANDS[4] && !tokens[1].empty())
                {
                    if (tokens[2].empty())
                    {
                        RemoveFromAliases(tokens[1], aliases);
                    }
                    else
                    {
                        string oldName = "";
                        string newName = tokens[1];
                        for (int i = 2; i < tokenCount; ++i)
                        {
                            oldName.append(tokens[i] + " ");
                        }
                        AddToAliases(newName, oldName, aliases);
                    }
                }
                // SAVE NEW NAMES
                else if (tokens[0] == SHELL_COMMANDS[6] && !tokens[1].empty())
                {
                    string filename = tokens[1];
                    SaveAliasesToFile(filename, aliases);
                }
                // READ NEW NAMES
                else if (tokens[0] == SHELL_COMMANDS[7] && !tokens[1].empty())
                {
                    string filename = tokens[1];
                    ReadNewNames(filename, aliases);
                }
            }
            else
            {
                // HISTORY
                if (tokens[0] == SHELL_COMMANDS[2])
                {
                    PrintHistory(history);
                }
                // NEW NAMES
                else if (tokens[0] == SHELL_COMMANDS[5])
                {
                    PrintAliases(aliases);
                }
            }
            return 1;
        }
        else if (CheckIfCommandInAliases(tokens[0], aliases))
        {
            // alias command
            string oldName = aliases.find(tokens[0])->second;
            // replace alias command;
            tokens[0] = oldName;
            oldName = ReconstructCommand(tokens, tokenCount);
            tokenCount = TokenizeCommandLine(tokens, oldName);
            ProcessCommand(tokens, tokenCount, history, aliases);
            return 1;
        } else {
            // OS command
            string command = ReconstructCommand(tokens, tokenCount);
            OsCommand(command);
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
        cout << i << " " << history[i] << endl;
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
    } else {
        cout << "Max Number of aliases reached" << endl;
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

void SaveAliasesToFile(string filename, map<string, string> &aliases)
{
    ofstream newfile;
    newfile.open(filename, ios::out | ios::trunc);
    map<string, string>::iterator it;
    string delimiter = ":";
    for (it = aliases.begin(); it != aliases.end(); ++it)
    {
        newfile << it->first << delimiter << it->second << "\n";
    }
    newfile.close();
}

void ReadNewNames(string filename, map<string, string> &aliases)
{
    ifstream file;
    string alias;
    string tokens[MAX_COMMAND_LINE_ARGUMENTS];
    file.open(filename);
    if (file.is_open())
    {
        while (getline(file, alias))
        {
            ParseAliasFile(tokens, alias);
            pair<map<string, string>::iterator, bool> ret;
            ret = aliases.insert(pair<string, string>(tokens[0], tokens[1]));
        }
    }
}

void ParseAliasFile(string tokens[], string alias)
{
    string delimiter = ":";
    string key = alias.substr(0, alias.find(delimiter));
    string value = alias.substr(alias.find(delimiter) + 1, alias.length());
    tokens[0] = key;
    tokens[1] = value;
}

bool CheckIfCommandInAliases(string alias, map<string, string> &aliases) {
    map<string, string>::iterator it;
    it = aliases.find(alias);
    if (it != aliases.end()) {
        return true;
    }
    return false;
}

void OsCommand(string command) {
    const char *c = command.c_str();
    system(c);
}
