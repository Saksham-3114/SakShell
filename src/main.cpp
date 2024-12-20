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
    // Step 1: Handle quoted input by splitting it into executable and arguments
    vector<string> args;
    bool inQuotes = false;
    string currentArg;

    // Parse the input for quoted executable and its arguments
    if (input[0] == '\'') {
      string out="";
      int i=1;
      while(input[i]!='\''){
        out+=input[i];
        i++;
      }
      // cout<<out<<endl;
      args.push_back(out);
      // cout<<args[0]<<endl;
    }
    else if (input[5] == '\"') {
      string out = parseDQ(input);
      args.push_back(out);
    }

    // Add the last argument if there's any left
    // if (!currentArg.empty()) {
    //     args.push_back(currentArg);
    // }

    // Step 2: Resolve the executable path
    if (args.empty()) {
        cout << "No executable specified." << endl;
        return false;
    }
    // cout<<args[0]<<endl;
    string path = getPath(args[0]);
    // cout<<path<<endl;
    if (path.empty()) {
        cout << "Executable not found in PATH." << endl;
        return false;
    }
    string filename;
    int i;
    for (i = 0; i < args.size(); i++) {
    }
    for(int j=1;j<i;j++){
      filename.push_back(input[j]);
    }
    string command = "exec " + path+" "+filename;
    system(command.c_str());
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
