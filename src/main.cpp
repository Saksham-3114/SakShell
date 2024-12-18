#include <iostream>
#include <bits/stdc++.h>
#include <filesystem>
#include <sstream>
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
        // cout<<input.substr(5)<<": not found\n";
      }
    }
    else{
      cout<<input<<": command not found\n";
    }

  }
}
