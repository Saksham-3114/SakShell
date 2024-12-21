#include <iostream>
#include <bits/stdc++.h>
#include <experimental/filesystem>
#include <sstream>
#include <cstdlib>
#include <fstream>
#include <unistd.h>
using namespace std;

string getPath(string filename) {
    string pathEnv = getenv("PATH");
    stringstream ss(pathEnv);
    string path;
    while (!ss.eof()) {
        getline(ss, path, ':');
        string absPath = path + "/" + filename;
        if (filesystem::exists(absPath)) {
            return absPath;
        }
    }
    return "";
}

bool execprog(string input) {
    stringstream ss(input);
    string t;
    vector<string> args;
    char del = ' ';
    while (getline(ss, t, del)) {
        args.push_back(t);
    }
    string path = getPath(args[0]);
    if (path.empty()) {
        return false;
    }
    ifstream file(path);
    if (file.good()) {
        string temp = "";
        for (int i = 1; i < args.size(); i++) {
            temp += args[i] + " ";
        }
        string command = "exec " + path + " " + temp;
        system(command.c_str());
    }
    return true;
}

vector<string> extractQuotedText(const string& input) {
    vector<string> quotedTexts;
    bool inQuotes = false;
    string currentText;

    for (size_t i = 0; i < input.size(); ++i) {
        if (input[i] == '"') {
            if (inQuotes) {
                quotedTexts.push_back(currentText);
                currentText.clear();
            }
            inQuotes = !inQuotes;
        } else if (inQuotes) {
            currentText += input[i];
        }
    }

    return quotedTexts;
}

string parseDQ(string input) {
    string res = "";
    bool escape = false;
    bool inQuotes = false;
    for (auto c : input) {
        if (escape) {
            if (inQuotes) {
                if ((c == '\\' or c == '"' or c == '\n' or c == '$')) {
                    res.push_back(c);
                } else {
                    res.push_back('\\');
                    res.push_back(c);
                }
            } else {
                res.push_back(c);
            }
            escape = false;
        } else if (c == '\\') {
            escape = true;
        } else if (c == '"') {
            inQuotes = !inQuotes;
        } else {
            res.push_back(c);
        }
    }
    return res;
}

bool execQprog(string input) {
    vector<string> args;
    string currentArg;
    bool inSingleQuotes = false, inDoubleQuotes = false, escape = false;

    for (size_t i = 0; i < input.size(); ++i) {
        char c = input[i];

        if (escape) {
            // Handle escaped characters (only in unquoted or double-quoted context)
            currentArg += c;
            escape = false;
        } else if (c == '\\') {
            // Start escape sequence
            if (inSingleQuotes) {
                // Backslashes are literal in single quotes
                currentArg += c;
            } else {
                escape = true;
            }
        } else if (c == '\'' && !inDoubleQuotes) {
            // Toggle single quotes
            inSingleQuotes = !inSingleQuotes;
        } else if (c == '"' && !inSingleQuotes) {
            // Toggle double quotes
            inDoubleQuotes = !inDoubleQuotes;
        } else if (c == ' ' && !inSingleQuotes && !inDoubleQuotes) {
            // Unquoted space ends the current argument
            if (!currentArg.empty()) {
                args.push_back(currentArg);
                currentArg.clear();
            }
        } else {
            // Add the character to the current argument
            currentArg += c;
        }
    }

    // Add the last argument if any
    if (!currentArg.empty()) {
        args.push_back(currentArg);
    }

    // Ensure there is at least one argument (the executable)
    if (args.empty()) {
        cout << "No executable specified." << endl;
        return false;
    }

    // Resolve the executable path
    string path = getPath(args[0]);
    if (path.empty()) {
        cout << args[0] << ": Executable not found in PATH." << endl;
        return false;
    }

    // Build the command string
    string command = "\"" + path + "\"";
    for (size_t i = 1; i < args.size(); ++i) {
        // Quote each argument for safety
        command += " \"" + args[i] + "\"";
    }

    // Execute the command
    int ret = system(command.c_str());
    if (ret == -1) {
        perror("Error executing command");
        return false;
    }
    return true;
}




int main() {
    // Flush after every std::cout / std:cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    // Uncomment this block to pass the first stage
    while (true) {
        std::cout << "$ ";

        std::string input;
        std::getline(std::cin, input);

        stringstream ss(input);
        string t;
        char del = ' ';
        vector<string> args;
        while (getline(ss, t, del)) {
            args.push_back(t);
        }

        if (input == "exit 0") {
            return 0;
        }
        else if (args[0] == "echo") {
            if (input[5] == '\'') {
                cout << input.substr(6, input.length() - 7) << endl;
            }
            else if (input[5] == '\"') {
                if (input.find('\\') != string::npos) {
                    string output = parseDQ(input.substr(5));
                    cout << output << endl;
                }
                else {
                    vector<string> qText = extractQuotedText(input);
                    for (int i = 0; i < qText.size(); i++) {
                        cout << qText[i] << " ";
                    }
                    cout << endl;
                }
            }
            else {
                if (input.find('\\') != string::npos) {
                    string res = "";
                    bool escape = false;
                    for (auto c : input.substr(5)) {
                        if (escape) {
                            res.push_back(c);
                            escape = false;
                        }
                        else if (c == '\\') {
                            escape = true;
                        }
                        else {
                            res.push_back(c);
                        }
                    }
                    cout << res << endl;
                }
                else {
                    for (int i = 1; i < args.size(); i++) {
                        if (!args[i].empty()) cout << args[i] << " ";
                    }
                    cout << endl;
                }
            }
        }
        else if (input.find("type ") == 0) {
            if (input.find("echo") != string::npos or input.find("exit") != string::npos or input.substr(5).find("type") != string::npos or input.substr(5).find("pwd") != string::npos) {
                cout << input.substr(5) << " is a shell builtin\n";
            }
            else {
                string path = getPath(input.substr(5));
                if (!path.empty()) {
                    cout << input.substr(5) << " is " << path << endl;
                }
                else {
                    cout << input.substr(5) << ": not found\n";
                }
            }
        }
        else if (args[0] == "pwd") {
            string currPath = filesystem::current_path().string();
            currPath = currPath.substr(0, currPath.length());
            cout << currPath << endl;
        }
        else if (args[0] == "cd") {
            if (args[1] == "~") {
                chdir(getenv("HOME"));
            }
            else if (filesystem::exists(args[1])) {
                filesystem::current_path(args[1]);
            }
            else {
                cout << "cd: " << args[1] << ": No such file or directory\n";
            }
        }
        else if (args[0] == "cat") {
            system(input.c_str());
        }
        else {
            if (!execprog(input) and !execQprog(input)) {
                cout << input << ": command not found\n";
            }
        }
    }
}
