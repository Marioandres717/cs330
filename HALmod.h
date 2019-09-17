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

using namespace std;

const int MAX_COMMAND_LINE_ARGUMENTS = 8;
const int MAX_HISTORY_COMMANDS = 10;
const int SLEEP_DELAY = 100000;
const vector<string> TERMINATING_COMMANDS{
    "shutdown",
    "restart",
    "stop",
    "exit",
};

const vector<string> SHELL_COMMANDS{
    "setshellname",
    "setterminator",
    "history",
};

int GetCommand(string tokens[], int &commandCounter);
int TokenizeCommandLine(string tokens[], string commandLine);
int ProcessCommand(string tokens[], int tokenCount);
void PrintCommandPrompt(int commandCounter);
void WriteToFile(string filename, string input);
int Ignore();