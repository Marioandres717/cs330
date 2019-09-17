#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <iterator>
#include <algorithm>
#include <vector>

using namespace std;

//The following two lines come from HALglobals.h
const int MAX_COMMAND_LINE_ARGUMENTS = 8;
const int SLEEP_DELAY = 100000;
const vector<string> TERMINATING_COMMANDS{
    "shutdown",
    "restart",
    "stop",
    "exit"};

int GetCommand(string tokens[], int &commandCounter, string &terminator);
int TokenizeCommandLine(string tokens[], string commandLine);
// bool CheckForCommand();
int ProcessCommand(string tokens[], int tokenCount);

static volatile sig_atomic_t cullProcess = 0;
