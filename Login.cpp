//
// Created by Alexandr on 21.05.2023.
//

#include <ctime>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <regex>
#include "Login.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"


Login::Login(){
    bool innerLoop = true;
    std::string choice;
    std::cout << "\033[2J\033[1;1H";  // Clear screen and move cursor to top-left corner

    while (innerLoop){
        //try{
        std::cout<<"Choose option:"<<std::endl;
        std::cout<<"1. Choose one of the files in local folder"<<std::endl;
        std::cout<<"2. Provide path to file"<<std::endl;
            std::cout<<":";
            std::cin>>choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (choice == "1"){
                clearTerminal();
                std::vector<std::filesystem::path> txtFiles=getTxtFilesInCurrentDirectory();
                listAndSelectFiles(txtFiles);
                innerLoop= false;
            }else if (choice == "2"){
                clearTerminal();
                std::string customPath;
                bool isFound = false;
                while (!isFound){
                    std::cout<<"Provide path to the file: ";
                    std::cin>>customPath;
                    file= getCustomFile(customPath);
                    if (file.good()){
                        isFound = true;
                    }else{
                        std::cout<<"File is not found"<<std::endl;
                    }
                }
                if(!checkFirstLine(customPath)){
                    std::cout<<RED<<"This is not a file with passwords"<<RESET;
                    continue;
                }else{
                    path=customPath;
                    enterTheFile(customPath);
                    innerLoop=false;
                }
            }else{
                clearTerminal();
                std::cout<<RED<<"Input error, try again"<<RESET<<std::endl;
            }
        //}catch (std::exception exception){
        //    std::cout<<"Stack is full";
      //  }
    }
}

std::string Login::getTimestampOfCurrentMoment() {
        std::time_t now = std::time(nullptr);
        char container[80];
        std::strftime(container, sizeof(container), "%H:%M:%S %d/%m/%Y", std::localtime(&now));
        return container;
}

std::vector<std::filesystem::path> Login::getTxtFilesInCurrentDirectory() {
    std::filesystem::path curPath = std::filesystem::current_path();
    std::vector<std::filesystem::path> txtFiles = getTxtFilesInDirectory(curPath.string());
    return txtFiles;
}

std::vector<std::filesystem::path> Login::getTxtFilesInDirectory(const std::string &directoryPath) {
    std::vector<std::filesystem::path> txtFiles;

    if (std::filesystem::is_directory(directoryPath)) {
        for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
            if (entry.path().extension() == ".txt") {
                txtFiles.push_back(entry.path());
            }
        }
    } else {
        std::cout << RED<<"Directory not found: " <<RESET<< directoryPath << std::endl;
    }
    return txtFiles;
}

std::fstream Login::getCustomFile(const std::string &filePath) {
    std::fstream file(filePath);
    if (file.good()) {
        return file;
    } else {
        return nullptr;
    }
}

bool Login::checkFirstLine(const std::string &pathToFile) {
    std::ifstream curFile(pathToFile);
    if (!curFile.is_open()) {
        std::cout << RED << "File not found: " << RESET << pathToFile << std::endl;
        return false;
    }
    std::string line;
    if (std::getline(curFile, line)) {
        std::regex pattern(R"(\S+\s+\d{2}:\d{2}:\d{2}\s+\d{2}/\d{2}/\d{4})");
        std::smatch matches;
        if (std::regex_search(line, matches, pattern)) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

void Login::enterTheFile(const std::string &filePath) {
    std::string filePassword;
    std::cout<<"Enter password to the file: ";
    bool isCorrect = false;
    while (!isCorrect){
        std::cin>>filePassword;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if(checkThePassword(filePath, filePassword)){
            std::cout<<GREEN<<"Password is correct!"<<RESET<<std::endl;
            isCorrect=true;
            password=filePassword;
        }else {
            std::cout<<RED<<"Incorrect password, try again : "<<RESET;
        }
    }
}

bool Login::checkThePassword(const std::string &pathToFile, const std::string &possiblePassword) {
    std::ifstream file(pathToFile);
    std::string line;
    changeTheTimeStamp(pathToFile);
    if (std::getline(file, line)) {
        size_t spaceIndex = line.find(' ');
        if (spaceIndex != std::string::npos) {
            std::string firstWord = line.substr(0, spaceIndex);
            char* unModifiedFirstWord = new char[firstWord.length() + 1];
            std::strcpy(unModifiedFirstWord, firstWord.c_str());
            for (int i = 0; i < firstWord.length(); i++) {
                unModifiedFirstWord[i] -= 65;
            }
            std::string modifiedFirstWord(unModifiedFirstWord);

            delete[] unModifiedFirstWord;
            return (modifiedFirstWord == possiblePassword);
        }
    }else{
        return false;
    }
}

void Login::changeTheTimeStamp(const std::string& pathToFile) {
    std::ifstream file(pathToFile);
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    file.close();
    if (lines.empty()) {
        std::cout << RED << "File is empty." << RESET << std::endl;
        return;
    }

    std::regex pattern(R"(\b\d{2}:\d{2}:\d{2} \d{2}/\d{2}/\d{4}\b)");
    std::smatch match;
    if (std::regex_search(lines[0], match, pattern)) {
        std::string timestamp = getTimestampOfCurrentMoment();
        lines[0] = std::regex_replace(lines[0], pattern, timestamp);
    }

    std::ofstream outFile(pathToFile);
    if (!outFile) {
        std::cout << RED << "Failed to open the file for writing." << RESET << std::endl;
        return;
    }

    for (const auto& line : lines) {
        outFile << line << std::endl;
    }
    outFile.close();
}
void Login::listAndSelectFiles(const std::vector<std::filesystem::path> &files) {
    if (files.empty()) {
        std::cout <<RED<< "No files found." <<RESET<< std::endl;
        return;
    }

    for (size_t i = 0; i < files.size(); ++i) {
        std::cout << i + 1 << ". " << files[i].filename().string() << std::endl;
    }
    int selection;
    bool isChoosen=false;
    std::string customPath;
    while(!isChoosen){
        std::cout << "Provide index of a desired file: ";
        std::string input;
        std::cin >> input;

        try {
            selection = std::stoi(input);
            if (selection < 1 || selection > static_cast<int>(files.size())) {
                throw std::invalid_argument("");
            }else{
                customPath = files[selection - 1].string();

                if (!checkFirstLine(customPath)) {
                    std::cout << RED<<"This is not a file with passwords." <<RESET<< std::endl;
                }else{
                    path=customPath;
                    isChoosen=true;
                }

            }
        } catch (std::exception& e) {
            std::cout <<RED<< "Incorrect input. Please enter a valid file number."<<RESET<< std::endl;
        }

    }
    enterTheFile(customPath);
}

void Login::clearTerminal() {
    std::cout << "\033[2J\033[1;1H";
}

std::string Login::getPath() {
    return path;
}

std::string Login::getPassword() {
    return password;
}







