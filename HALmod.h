#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <iterator>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <exception>
#include <string>
#include <cctype>
#include <map>
#include <utility>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <errno.h>
#include <ctime>
#include <cstring>
#include <iomanip>
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;

const int MAX_COMMAND_LINE_ARGUMENTS = 15;
const int MAX_HISTORY_COMMANDS = 10;
const int MAX_ALIASES = 10;
const int SLEEP_DELAY = 100000;
const int MAX_NUMBER_OF_BG_PROCESS = 10;
const string PIPE_OPERATOR = "|";
const string COMMENT_OPERATOR = "$";
const string PATH_VARIABLE = "PATH";
const string BACKGROUND_CHAR = "-";
const vector<string> TERMINATING_COMMANDS{
    "shutdown",
    "restart",
    "stop",
    "exit",
    "quit",
};

const vector<string> SHELL_COMMANDS{
    "setshellname",
    "setterminator",
    "history",
    "!",
    "newname",
    "newnames",
    "savenewnames",
    "readnewnames",
    "backjobs",
    "frontjob",
    "cond",
    "notcond",
};

const vector<string> CONDITIONAL_COMMANDS{
    "checkr",
    "checke",
    "checkx",
    "checkd",
    "checkw",
};

extern char **environ;

bool isNumber(string tokenString);
bool inRange(unsigned low, unsigned high, unsigned x);
bool CheckIfCommandInAliases(string alias, map<string, string> &aliases);
int GetCommand(string tokens[], int &commandCounter);
int TokenizeCommandLine(string tokens[], string commandLine);
int ProcessCommand(string tokens[], int tokenCount, vector<string> &history, map<string, string> &aliases, map<int, vector<string>> &bgProcesses);
string ReconstructCommand(string tokens[], int tokenCount);
string ReconstructOldName(string tokens[], int tokenCount, map<string, string> &aliases);
void PrintCommandPrompt(int commandCounter);
void WriteToFile(string filename, string input);
void PrintHistory(vector<string> &history);
void AddToHistory(string commandToAdd, vector<string> &history);
void AddToAliases(string newName, string oldName, map<string, string> &aliases);
void RemoveFromAliases(string aliasToRemove, map<string, string> &aliases);
void PrintAliases(map<string, string> &aliases);
void SaveAliasesToFile(string filename, map<string, string> &aliases);
void ReadNewNames(string filename, map<string, string> &aliases);
void ParseAliasFile(string tokens[], string alias);
void OsCommand(string tokens[], int tokenCount, map<int, vector<string>> &backJobs);
void PrintBackJobs(map<int, vector<string>> &backJobs);
void AddToBackJobs(string tokens[], int tokenCount, int processID, map<int, vector<string>> &backJobs);
template <typename T>
void PrintElement(T t);
string ReadableTimestamp();
void RemoveFromBackJobs(int processID, map<int, vector<string>> &backJobs);
void FrontJob(string processID, map<int, vector<string>> &backJobs);
bool Conditional(string conditionalCommand, string filename);
bool CanReadFile(string filename);
bool CanWriteFile(string filename);
bool CanExecuteFile(string filename);
bool IsaDirectory(string filename);
bool DoesItExist(string filename);