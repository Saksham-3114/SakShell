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
      break;
    }else{
      cout<<input<<": command not found\n";
    }

  }
}
