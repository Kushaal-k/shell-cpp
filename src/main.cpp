#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <filesystem>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fstream>
#include <algorithm>

namespace fs = std::filesystem;
struct stat info;

std::vector<std::string> builtIn = {"echo", "type", "exit", "pwd", "cd"};

bool isBuiltIn(std::vector<std::string>& builtin, std::string target){
  for(auto& item: builtin){
    if(item == target){
      return true;
    }
  }

  return false;
}

void parseCommand(const std::string& input, std::vector<std::string>& tokens) {
    std::string cur = "";
    bool inQuotes = false;
    bool indQuotes = false;

    for (size_t i = 0; i < input.size(); ++i) {
        char c = input[i];

        if(c== '\"'){
            indQuotes = !indQuotes;
        }
        else if (c == '\'' && !indQuotes) {
            inQuotes = !inQuotes;
        }
        else if (c == ' ' && !inQuotes && !indQuotes) {
            if (!cur.empty()) {
                tokens.push_back(cur);
                cur.clear();
            }
        }
        else {
            cur += c;
        }
    }

    if (!cur.empty())
        tokens.push_back(cur);
}


//Finding Executable File
std::string findExec(std::string text){
  std::string pathEnv = getenv("PATH");
  std::stringstream ss(pathEnv);
  std::string dir;

  while(getline(ss, dir, ':')){
    fs::path fullPath = fs::path(dir)/text;

    if(fs::exists(fullPath)){
      if(access(fullPath.string().c_str(), X_OK) == 0){
        // std::cout << text << " is " << fullPath.string() << std::endl;
        return fullPath.string();
        break;
      }
    }
  }

  return "";
}

// Running Executable File 
bool runExec(const std::vector<std::string>& splittedCommand)
{
    pid_t pid = fork();

    if(pid == 0)
    {
        std::vector<char*> args;
        for(auto& s: splittedCommand){
            args.push_back(const_cast<char*>(s.c_str()));
        }
        args.push_back(nullptr);

        execvp(args[0], args.data());

        std::cout << args[0] << ": command not found\n";
        exit(1);
    }
    else{
        wait(nullptr);
        
    }
    return true;
}

bool directoryExist(const fs::path& path){
    if(fs::is_directory(path)){
        return true;
    }
    return false;
}

void readFile(const std::string& path)
{
    std::string line;
    std::ifstream myfile (path);

    if(myfile.is_open()){
        while( getline(myfile,line)){
            std::cout << line << '\n';
        }
        myfile.close();
    }

    else{
        std::cout << "Unable to open file" << '\n';
    }
}

int main() {
    // Flush after every std::cout / std:cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

   
    std::string command;
    while(true){
    std::cout << "$ ";
    
    //Splitting the command in string vector
    std::getline(std::cin, command);
    
    std::vector<std::string> splittedCommand;

    parseCommand(command, splittedCommand);

    if (splittedCommand.empty()) {
        continue;
    }

    //Checking different condition on command
    if(command == "exit")
    {
        return 0;
    }
    else if(splittedCommand[0] == "echo")
    {
        for (int i = 1; i < splittedCommand.size(); i++) {
            std::cout << splittedCommand[i];
            if (i + 1 < splittedCommand.size()) std::cout << " ";
        }
        std::cout << "\n";

        continue;
    }

    else if(splittedCommand[0] == "type")
    {
        std::string text = command.substr(5,command.length());
        if(isBuiltIn(builtIn, text)){
            std::cout << text << " is a shell builtin" << '\n';
            continue;
        }

        std::string fullPath = findExec(text);
        if(fullPath.empty()){
            std::cout << text << ": not found" << '\n';
        }
        else{
            std::cout << text << " is " << fullPath << '\n';
        }
        continue;
    }

    else if(!findExec(splittedCommand[0]).empty())
    {
        if(runExec(splittedCommand))
            continue;
    }

    else if(splittedCommand[0] == "pwd")
    {
        std::cout << get_current_dir_name() << '\n';
        continue;
    }

    else if(splittedCommand[0] == "cd")
    {
        
        if(splittedCommand.size() > 1) 
        {
            std::string path = splittedCommand[1];

            if(path == "~"){
                std::string home = getenv("HOME");
                path = home;
            }
            
            fs::path file_path = fs::absolute(fs::path(path));

            if(directoryExist(file_path)){
                try{
                    fs::current_path(file_path); 
                }
                catch(...){
                    std::cout << "Access Denied" << '\n';
                }
            }
            else{
                std::cout << "cd: " << splittedCommand[1] <<  ": No such file or directory" << "\n";
            }
        }
        continue;
    }
    else if(splittedCommand[0] == "cat")
{
    for(int i = 1; i < splittedCommand.size(); i++) {
        readFile(splittedCommand[i]);
    }
    continue;
}
    std::cout << command << ": command not found" << std::endl;
  }
}
