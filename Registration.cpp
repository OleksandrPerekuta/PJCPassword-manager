//
// Created by Alexandr on 20.05.2023.
//
#include <iostream>
#include <fstream>
#include <filesystem>
#include <random>
#include <ctime>
#include "Registration.h"
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"

Registration::Registration() {
    clearTerminal();
    bool innerLoop = true;
    while (innerLoop){
        std::string pass1;
        std::string pass2;
        std::cout<<"Create new password :";
        std::cin>>pass1;
        std::cout<<"Confirm password :";
        std::cin>>pass2;
        if(pass1 == pass2){
            password=pass1;
            innerLoop= false;
        }else{
            clearTerminal();
            std::cout<<RED<<"Something went wrong, try again"<<RESET<<std::endl;
        }
    }
    std::filesystem::path curPath = std::filesystem::current_path();
    std::cout<<GREEN<<"Welcome, your password to the createdFile is \""<<getPassword()<<"\" don't forget it!"<<RESET<<std::endl;
    std::string nameOfFile= generateRandomNameOfTheFile();
    std::fstream createdFile;
    createdFile.open(nameOfFile + ".txt",   std::ios::out);
    if (!createdFile){
        std::cout<<RED<<"Error of creation a createdFile, try again later :("<<RESET<<std::endl;
        std::exit(0);
    }else{
        std::cout << YELLOW << "Notice: createdFile with passwords was created in a standard directory " << curPath << "\\\\" << nameOfFile << RESET << std::endl;
        std::string firstLineOfTheFile=
                encodePasswordInTheFirstLine(password) + " " + getTimestampOfCurrentMoment() + "\n";
        createdFile << firstLineOfTheFile;
    }
    createdFile.close();
    std::string fullPath = (curPath / (nameOfFile + ".txt")).string();
    file.open(fullPath, std::ios::in | std::ios::out);
    if (!file.is_open()) {
        std::cout << "Failed to open file: " << fullPath << std::endl;
    }
    path=fullPath;
}
std::string Registration::encodePasswordInTheFirstLine(const std::string& inputS){
    const char* arrayOfCharsOfPassword=inputS.c_str();
    char modifiedArrayOfCharsOfPassword[inputS.length() + 1];
    std::copy(arrayOfCharsOfPassword, arrayOfCharsOfPassword + inputS.length() + 1, modifiedArrayOfCharsOfPassword);
    for (int i = 0; modifiedArrayOfCharsOfPassword[i] != '\0'; ++i) {
        modifiedArrayOfCharsOfPassword[i] += 65;
    }
    std::string modifiedStringPassword(modifiedArrayOfCharsOfPassword);
    return modifiedStringPassword;
}
std::string Registration::getPassword() {
    return password;
}
std::string Registration::getTimestampOfCurrentMoment() {
    std::time_t now = std::time(nullptr);
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%H:%M:%S %d/%m/%Y", std::localtime(&now));
    return buffer;
}
std::string Registration::generateRandomNameOfTheFile() {
    std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(characters.begin(), characters.end(), gen);
    return characters.substr(0, 16);
}

void Registration::clearTerminal() {
    std::cout << "\033[2J\033[1;1H";
}

std::string Registration::getPath() {
    return path;
}