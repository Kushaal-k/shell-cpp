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

    std::string prefix = "echo";
    if(command == "exit"){
      return 0;
    }
    else if(command.rfind(prefix, 0) == 0){
      std::string text = command.substr(5, command.size());
      std::cout << text << "\n";
      continue;
    }
    std::cout << command << ": command not found" << std::endl;
  }while(true);
}
