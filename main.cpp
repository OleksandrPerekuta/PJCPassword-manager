#include <iostream>
#include "Registration.h"
#include "Login.h"
#include "Editing.h"
#include <limits>
#include <vector>
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

int main() {
    std::string pathToFile;
    std::string choice;
    bool isRunning= true;
    int option;
    std::string password;
    std::cout<<"Welcome to password manager!"<<std::endl;
    std::cout<<"1. Register as a new user\n";
    std::cout<<"2. Login as an existing user\n";
    while (isRunning){
        try{
            std::cout<<"Input: ";
            std::cin>>choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (choice == "1"){
                option=1;
                isRunning = false;
            }else if(choice == "2"){
                isRunning = false;
                option=2;
            }else{
                std::cout<<RED<<"Input error, try again"<<RESET<<std::endl;
            }
        }catch (std::exception exception){
            std::cout<<"Stack is full\n";
        }
    }
    if (option==1){
        Registration registration;
        pathToFile=registration.getPath();
        password=registration.getPassword();
    }else if (option==2){
        Login login;
        pathToFile=login.getPath();
        password=login.getPassword();
    }
    Editing editing(pathToFile,password);

    return 0;
}
