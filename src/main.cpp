#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <filesystem>
#include <unistd.h>

namespace fs = std::filesystem;

std::vector<std::string> builtIn = {"echo", "type", "exit"};

bool isBuiltIn(std::vector<std::string>& builtin, std::string target){
  for(auto& item: builtin){
    if(item == target){
      return true;
    }
  }

  return false;
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::string command;
  do{
    std::cout << "$ ";
    
    std::getline(std::cin, command);
    

    if(command == "exit"){
      return 0;
    }
    else if(command.rfind("echo", 0) == 0){
      std::string text = command.substr(5, command.size());
      std::cout << text << "\n";
      continue;
    }
    else if(command.rfind("type", 0) == 0){
      std::string pathEnv = getenv("PATH");
      std::stringstream ss(pathEnv);
      std::string dir;
      bool found = false;

      std::string text = command.substr(5,command.length());
      if(isBuiltIn(builtIn, text)){
        std::cout << text << " is a shell builtin" << '\n';
        continue;
      }

      while(getline(ss, dir, ':')){
        fs::path fullPath = fs::path(dir)/text;

        if(fs::exists(fullPath)){
          if(access(fullPath.string().c_str(), X_OK) == 0){
            std::cout << text << " is " << fullPath << std::endl;
            found = true;
            break;
          }
        }
      }
      if(!found){
        std::cout << text << ": not found" << '\n';
      }
      continue;
    }
    std::cout << command << ": command not found" << std::endl;
  }while(true);
}
