#include <iostream>
#include <array>
#include <cstdlib>
#include <unordered_set>
#include <list>
#include <filesystem>
#include <fstream>
#include <sstream>
enum class TypeEnum {
  NOT_FOUND,
  EXIT,
  ECHO,
  TYPE,
  PWD,
  CD,
};

struct TypeDscr {
  TypeEnum type;
  std::string_view typeStr;
};
std::array<TypeDscr, 5> types = {{
  { TypeEnum::EXIT, "exit" },
  { TypeEnum::ECHO, "echo" },
  { TypeEnum::TYPE, "type" },
  { TypeEnum::PWD,  "pwd"  },
  { TypeEnum::CD,   "cd"   },
}};

void removeSymbFromStart(std::string& str, char c) {
  size_t start = 0;
  while (str[start] == c && start < str.size()) {
    start++;
  }
  str = str.substr(start);
}
void skipSymbol(const std::string& input, size_t& offset, char c) {
  while (input[offset] == c && offset < input.size()) {
    offset++;
  }
}
void removeSymbFromEnd(std::string& str, char c) {
  size_t end = str.size()-1;
  while (str[end] == c && end >= 0) {
    end--;
  }
  if (end > 0) {
    str = str.substr(0, end+1);
  } else {
        str = "";
  }
}
std::string getWord(const std::string& input, size_t& offset) {
  skipSymbol(input, offset, ' ');
  size_t start = offset;
  std::string result = input;
  while (offset != input.size()) {
    if (input[offset] == ' ') {
      result = input.substr(start, offset-start);
      break;
    }
    offset++;
  }
  return result;
} 

TypeEnum getType(const std::string& word) {
  for (auto& t : types) {
    if (t.typeStr == word) {
      return t.type;
    }
  }
  return TypeEnum::NOT_FOUND;
}

bool checkExit(const std::string& input) {
  return input == "exit 0";
}
void updateBackSlash(std::string& str, int state) {
  int j = 0;
  // std::cout << "state = " << state << ", str = " << str << std::endl;
  if (state == 0) {
    if (str == "\\") {
      str = " ";
      return;
    } else if (*str.rbegin() == '\\') {
      str += ' ';
    }
    bool flag = false;
    for (int i = 0; i < str.size(); i++) {
      if (i > 0 && str[i] != '\\' ) {
        j++;
      }
      if (i > j && str[i] != '\\' ) {
        str[j] = str[i];
      }
    }
    // std::cout << "j = " << j << std::endl;
    if (j > 0) {
      str = str.substr(0, j+1);
    }
    } else if (state == 2) {
    for (int i = 0; i < str.size(); i++) {      
      bool isSpecSymb = str[i] == '\\' && 
        (i < str.size() && (str[i+1] == '\\' || str[i+1] == '$' || str[i+1] == '\"') ) ;
      
      // std::cout << i << " " << j << " " << isSpecSymb << " " << str << std::endl;
      if (isSpecSymb) {
        i++;
      } 
      str[j++] = str[i];
    }
    str = str.substr(0, j);
  }
  
}
void tokenize(std::string& result, const std::string& input, size_t offset) {
  std::string token = "";
  result = "";
  auto addStrToResult = [&result] (const std::string& str)
  {
    // std::cout << "addStrToResult: result = '" << result <<"'" << std::endl; 
    result += str;
  };
  auto addSpaceToResult = [&result] ()
  {
    // std::cout << "addSpaceToResult: result = '" << result <<"'" << std::endl; 
    if (!result.empty() && *result.rbegin() != ' ') {
      result += ' ';
    } 
  };
  int state = 0;
  char prevC = 'x';
  for (size_t i = offset; i < input.size(); i++) {
    switch (state) {
      case 0: {
        // std::cout << i << ", '" << input[i] << "'" << std::endl;
        if (input[i] == ' ') {
          if (!token.empty()) {
            updateBackSlash(token, state);
            addStrToResult(token);
            token.clear();
          }
          addSpaceToResult();
        } else if (input[i] == '\'') {
          state = 1;
        } else if (input[i] == '"' && prevC != '\\') {
          state = 2;
        } else {
          token += input[i];
        }
      }
      break;
       case 1: {
        if (input[i] == '\'') {
          updateBackSlash(token, state);
          addStrToResult(token);
          token.clear();
          state = 0;
        } else {
          token += input[i];
        }
      }
      break;
       case 2: {
        if (input[i] == '"' && prevC != '\\') {
          updateBackSlash(token, state);
          addStrToResult(token);
          token.clear();
          state = 0;
        } else {
          token += input[i];
        }
      }
      break;
    }
    if (prevC == '\\' && input[i] == '\\') {
      prevC = 'X';
    } else {
      prevC = input[i];
    }
  }
  if (!token.empty()) {
    updateBackSlash(token, state);
    addStrToResult(token);
    token.clear();
  }
}
void doEcho(const std::string& input, size_t offset) {
  skipSymbol(input, offset, ' ');
  std::string result;
  tokenize(result, input, offset);
  std::cout << result;
  std::cout << std::endl;
}
bool findFilePath(std::string& filePath, const std::string& fileName, const std::unordered_set<std::string>& path) {
  for (auto& p : path) {
    if (std::filesystem::exists(p + "/" + fileName)) {
      filePath = p + "/" + fileName;
      return true;
    }
  }
  return false;
}
void doType(const std::string& input, size_t offset, const std::unordered_set<std::string>& path) {
  skipSymbol(input, offset, ' ');
  std::string word = input.substr(offset);
  if (getType(word) != TypeEnum::NOT_FOUND) {
    std::cout << word << " is a shell builtin" << std::endl;
  } else {
    std::string filePath;
    if (findFilePath(filePath, word, path)) {
      std::cout << word << " is " << filePath << std::endl;
    } else {
      std::cout << word << ": not found" << std::endl;
    }
  }
}
void doPwd() {
  std::cout << std::filesystem::current_path().string() << std::endl;
}
void doCd(const std::string& input, size_t offset) {
  skipSymbol(input, offset, ' ');
  std::string path;
  if (input[offset] == '~') {
    path = std::getenv("HOME") + input.substr(offset+1);
  } else {
    path = input.substr(offset);
  }
  if (std::filesystem::exists(path)) {
    std::filesystem::current_path(path);
  } else {
    std::cout << "cd: " << path << ": No such file or directory" << std::endl;
  }
}
void doCommandNotFound(const std::string& str) {
  std::cout << str << ": command not found" << std::endl; 
}
void parsePath(std::unordered_set<std::string>& path) {
  const char* env_p = std::getenv("PATH");
  // std::cout << "PATH is " << env_p << std::endl;
  int i = 0;
  std::string pathComp = "";
  while (env_p[i] != '\0') {
    if (env_p[i] == ':') {
      path.insert(pathComp);
      pathComp = "";
    } else {
      pathComp += env_p[i];
    }
    i++;
  }
}
std::string doExecute(const std::string& filePath) {
  std::string result = "";
  FILE* pipe = popen(filePath.c_str(), "r");
  char buf[2];
  while (fgets(buf, sizeof buf, pipe) != NULL) {
    // std::cout << "buf = " << buf << std::endl;
    result += buf;
  }
  pclose(pipe);
  // std::cout << "filePath = " << filePath << ", result = " << result << std::endl;
  return result;
}
bool checkExeWith(const std::string& word, const std::string& input, size_t& offset){
  if (word == "exe" || word == "'exe" || word == "\"exe") {
    while (input[offset] != '/') {
      offset++;
    }
    return true;
  }
  return false; 
}
bool tryRunCommand(const std::string& word, const std::string& str,
  const std::unordered_set<std::string>& path) {
  std::string filePath;

  if (findFilePath(filePath, word, path)) {
    std::cout << doExecute(filePath + " " + str);
    return true;
  }
  return false;
}
int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  // Uncomment this block to pass the first stage
  std::cout << "$ ";
  std::unordered_set<std::string> path;
  parsePath(path);
  
  std::string input;
  
  bool error;
  bool exit = false;
  size_t offset;
  std::string command;
  while (true) {
    std::getline(std::cin, input);
    offset = 0;
    error = false;
    command = getWord(input, offset);
    switch (getType(command)) {
      case TypeEnum::EXIT: {
        exit = checkExit(input);
      }
      break;
      case TypeEnum::ECHO: {
        doEcho(input, offset);
      }
      break;
      case TypeEnum::TYPE: {
        }
      break;
      case TypeEnum::PWD: {
        doPwd();
      }
      break;
      case TypeEnum::CD: {
        doCd(input, offset);
      }
      break;
      case TypeEnum::NOT_FOUND: {
        if (command != "cat" && checkExeWith(command, input, offset)) {
          command = "cat";
        }
        error = !tryRunCommand(command, input.substr(offset), path);
      }
      break;
    }
    if (error) {
       doCommandNotFound(command);
    }
    if (exit) {
      break;
    }
    std::cout << "$ "; 
  }
}