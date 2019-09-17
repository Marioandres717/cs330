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

using namespace std;

const int MAX_COMMAND_LINE_ARGUMENTS = 8;
const int MAX_HISTORY_COMMANDS = 10;
const int SLEEP_DELAY = 100000;
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
};

int GetCommand(string tokens[], int &commandCounter);
int TokenizeCommandLine(string tokens[], string commandLine);
int ProcessCommand(string tokens[], int tokenCount, vector<string> &history);
void PrintCommandPrompt(int commandCounter);
void WriteToFile(string filename, string input);
void PrintHistory(vector<string> &history);
void AddToHistory(string commandToAdd, vector<string> &history);
string ReconstructCommand(string tokens[], int tokenCount);
bool isNumber(string tokenString);
bool inRange(unsigned low, unsigned high, unsigned x);