#include <iostream>
#include <bits/stdc++.h>
#include <experimental/filesystem>
#include <sstream>
#include <cstdlib>
#include <fstream>
using namespace std;

string getPath(string filename){
  string pathEnv = getenv("PATH");
  stringstream ss(pathEnv);
  string path;
  while(!ss.eof()){
    getline(ss,path,':');
    string absPath=path+"/"+filename;
    if(filesystem::exists(absPath)){
      return absPath;
    }
  }
  return "";
}

bool execprog(string input){
  stringstream ss(input);
  string t;
  vector<string> args;
  char del=' ';
  while(getline(ss,t,del)){
    args.push_back(t);
  }
  string path=getPath(args[0]);
  if(path.empty()){
    return false;
  }
  ifstream file(path);
  if(file.good()){
    string temp="";
    for(int i=1;i<args.size();i++){
      temp+=args[i]+" ";
    }
    string command="exec "+path+" "+temp;
    system(command.c_str());
  }
  return true;
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // Uncomment this block to pass the first stage
  while(true){
    std::cout << "$ ";

    std::string input;
    std::getline(std::cin, input);

    stringstream ss(input);
    string t;
    char del=' ';
    vector<string> args;
    while(getline(ss,t,del)){
      args.push_back(t);
    }

    if(input == "exit 0"){
      return 0;
    }
    else if(input.substr(0,4)=="echo"){
      cout<<input.substr(5)<<endl;
    }
    else if(input.find("type ")==0){
      if(input.find("echo")!=string::npos or input.find("exit")!=string::npos or input.substr(5).find("type")!=string::npos){
        cout<<input.substr(5)<<" is a shell builtin\n";
      }
      else{
        string path = getPath(input.substr(5));
        if(!path.empty()){
          cout<<input.substr(5)<<" is "<<path<<endl;
        }
        else{
          cout<<input.substr(5)<<": not found\n";
        }
      }
    }
    else if(args[0]=="pwd"){
      cout<<filesystem::current_path()<<endl;
    }
    else{
      if(!execprog(input)){
        cout<<input<<": command not found\n";
      }
      // else cout<<input<<": command not found\n";
    }

  }
}
