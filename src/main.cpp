#include <iostream>
#include <string>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::string command;
  do{
    std::cout << "$ ";
    
    std::getline(std::cin, command);
    std::string builtIn[3] = {"echo", "type", "exit"};

    if(command == "exit"){
      return 0;
    }
    else if(command.rfind("echo", 0) == 0){
      std::string text = command.substr(5, command.size());
      std::cout << text << "\n";
      continue;
    }
    else if(command.rfind("type", 0) == 0){
      std::string text = command.substr(5,command.length());
      bool found = false;
      for(auto item : builtIn){
        if(text == item){
          std::cout << text << " is a shell builtin" << '\n';
          found = true;
          break;
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
