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
std::string getWord(const std::string& input, size_t& offset) {
  std::string result = "";
  auto index = input.find(' ', offset);
  if (index != std::string::npos) {
    result = input.substr(offset, index);
    offset = index + 1;
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
    result = input;
    offset = input.size();
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
Expand 7 lines
  return TypeEnum::NOT_FOUND;
}
bool checkExit(const std::string& str, size_t offset) {
  return (offset == str.size()-1 && str[offset] == '0');
bool checkExit(const std::string& input) {
  return input == "exit 0";
}
void updateBackSlash(std::string& str, int state) {
Expand 22 lines
    if (j > 0) {
      str = str.substr(0, j+1);
    }
  } else if (state == 1) {
    // for (int i = 0; i < str.size(); i++) {
      // if (!(i > 0 && str[i] == '\\' && str[i-1] == '\\')) {
      //   j++;
      // }
      // if (i > j) {
      //   str[j] = str[i];
      // }
    // }
    // str = str.substr(0, j+1);
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
void tokenize(std::string& result, const std::string& input) {
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
  for (int i = 0; i < input.size(); i++) {
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
void doEcho(const std::string& input) {
void doEcho(const std::string& input, size_t offset) {
  skipSymbol(input, offset, ' ');
  std::string result;
  tokenize(result, input);
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
void doType(const std::string& word, const std::unordered_set<std::string>& path) {
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
void doCd(const std::string& path) {
  const char* env_p = std::getenv("HOME");
  std::string homePath = "";
  int i = 0;
  while (env_p[i] != '\0') {
    homePath += env_p[i++];
  }
void doCd(const std::string& input, size_t offset) {
  skipSymbol(input, offset, ' ');
  std::string pathUpd = path;
  if (path[0] == '~') {
    pathUpd = homePath + path.substr(1);
  std::string path;
  if (input[offset] == '~') {
    path = std::getenv("HOME") + input.substr(offset+1);
  } else {
    path = input.substr(offset);
  }
  if (std::filesystem::exists(pathUpd)) {
    std::filesystem::current_path(pathUpd);
  if (std::filesystem::exists(path)) {
    std::filesystem::current_path(path);
  } else {
    std::cout << "cd: " << pathUpd << ": No such file or directory" << std::endl;
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
bool tryRunExec(const std::string& word, const std::string& str, 
  size_t offset, const std::unordered_set<std::string>& path) {
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
    // std::cout << "command = " << (filePath + str.substr(offset-1)) << std::endl;
    std::cout << doExecute(filePath + str.substr(offset-1));
    // std::cout << "command = " << (filePath + str) << std::endl;
    std::cout << doExecute(filePath + " " + str);
    return true;
  }
  return false;
}
// std::string getFileContent(const std::string filePath) {
//   std::ifstream f(filePath, std::ios_base::in);
//   std::string tmp, result;
//   std::stringstream buffer;
//   buffer << f.rdbuf();
//   while (std::getline(buffer, tmp)) {
//     result += tmp;
//   }
//   return result;
// }
// void doCat(const std::string& input) {
//   std::list<std::string> tokens;
//   tokenize(tokens, input);
//   for (auto& t : tokens) {
//     std::cout << getFileContent(t);
//   }
//   std::cout << std::endl;
// }
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
  std::string word;
  std::string command;
  while (true) {
    std::getline(std::cin, input);
    offset = 0;
    error = false;
    word = getWord(input, offset);
    command = getWord(input, offset);
    // std::cout << "command = '"  << command <<"', offset = " << offset << std::endl;
    switch (getType(word)) {
    switch (getType(command)) {
      case TypeEnum::EXIT: {
        if (checkExit(input, offset)) {
          exit = true;
        } else {
          error = true;
        }
        exit = checkExit(input);
      }
      break;
      case TypeEnum::ECHO: {
        doEcho(input.substr(offset));
        doEcho(input, offset);
      }
      break;
      case TypeEnum::TYPE: {
        doType(input.substr(offset), path);
        doType(input, offset, path);
      }
      break;
      case TypeEnum::PWD: {
        doPwd();
      }
      break;
      case TypeEnum::CD: {
        doCd(input.substr(offset));
        doCd(input, offset);
      }
      break;
 
      case TypeEnum::NOT_FOUND: {
        if (!tryRunExec(word, input, offset, path)) {
          error = true;
        if (command != "cat" && checkExeWith(command, input, offset)) {
          command = "cat";
        }
        error = !tryRunCommand(command, input.substr(offset), path);
      }
      break;
    }
    if (error) {
      doCommandNotFound(word);
      doCommandNotFound(command);
    }
    if (exit) {
      break;
    }
    std::cout << "$ "; 
  }
}