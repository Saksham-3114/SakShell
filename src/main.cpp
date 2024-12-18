#include <iostream>
using namespace std;

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
      if(input.find("echo") or input.find("type") or input.find("exit")){
        cout<<input.substr(5)<<" is a shell builtin\n";
      }else{
        cout<<input.substr(5)<<": not found\n";
      }
    }
    else{
      cout<<input<<": command not found\n";
    }

  }
}
