//
// Created by Alexandr on 27.05.2023.
//

#include <iostream>
#include <vector>
#include <limits>
#include <sstream>
#include <regex>
#include "Editing.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"




Editing::Editing(const std::string& pathToFile, const std::string &pass) {
    clearTerminal();
    filePath = pathToFile;
    password = pass;
    std::string opening="Choose option: \n"
                        "1.Add password\n"
                        "2.Remove password\n"
                        "3.Edit password\n"
                        "4.Find passwords\n"
                        "5.Sort passwords\n"
                        "6.Add Category\n"
                        "7.Remove Category\n"
                        "8.List records\n"
                        "9.Exit\n"
                        ": ";
    file.open(filePath, std::ios::in | std::ios::out);
    while (true){
        std::string choice;
        std::string input;
        std::cout<<opening;
        std::cin>>choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if(choice=="1"){
            clearTerminal();
            addPassword();
        } else if(choice=="2"){
            clearTerminal();
            removePassword();
        }else if(choice=="3"){
            clearTerminal();
            editPassword();
        }else if(choice=="4"){
            searchPasswords();
        }else if(choice=="5"){
            sortPassword();
        }else if(choice=="6"){
            clearTerminal();
            std::cout<<RED<<"Provide name of a category :"<<RESET;
            std::cin>>input;
            addCategory(input);
        }else if(choice=="7"){
            clearTerminal();
            removeCategory(input);
        }else if(choice=="8"){
            listRecors();
        }else if(choice=="9"){
            exit(0);
        }else{
            clearTerminal();
            std::cout<<RED<<"Input error, try again"<<RESET<<std::endl;
        }
    }
}
std::string Editing::encodeString(const std::string& source, const std::string& key) {
    std::string encodedString;
    for (size_t i = 0; i < source.size(); ++i) {
        char sourceChar = source[i];
        char charOfTheKey = key[i % key.size()];
        char encodedChar = sourceChar ^ charOfTheKey;
        encodedChar = encodedChar % 128;
        encodedString.push_back(encodedChar);
    }
    return encodedString;
}
std::string Editing::decodeString(const std::string& source, const std::string& key) {
    std::string decodedString;
    for (size_t i = 0; i < source.size(); ++i) {
        char encCharecter = source[i];
        char charOfTheKey = key[i % key.size()];
        char decCharecter = encCharecter ^ charOfTheKey;
        decodedString.push_back(decCharecter);
    }
    return decodedString;
}





void Editing::clearTerminal() {
    std::cout << "\033[2J\033[1;1H";
}

bool Editing::containsSubstring(const std::string &target, const std::string &pool) {
    if(target.find(pool) != std::string::npos){
        return true;
    }else{
        return false;
    }
}


void Editing::addCategory(const std::string &name) {
    std::string inputLine=name+",";
    std::string line;
    line= getLineNumber(2);
    line= decodeString(line,password);
    if (containsSubstring(line,inputLine)){
        clearTerminal();
        std::cout<<RED<<"There is already a category like this"<<RESET<<std::endl;
        return;
    }else{
        clearTerminal();
        std::cout<<GREEN<<"Category added successfully!"<<RESET<<std::endl;
        line.append(inputLine);
    }
    line= encodeString(line,password);
    changeLineNumber(2, line);
}

void Editing::removeCategory(const std::string &name) {
    std::string line;
    std::string input;
    removeEmptyLines();
    line = getLineNumber(2);
    if (line != "") {
        std::cout << RED << "WARNING!!! Removing category can cause removing all records that belong to this category"
                  << RESET << std::endl;
        std::cout << "Do you want to continue?    y/n" << std::endl;
        std::cin >> input;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (input == "y" || input == "Y") {
            clearTerminal();
            line = decodeString(line, password);
            std::vector<std::string> substrings= getCategories(line,1);
            for (const auto& num:substrings) {
                std::cout<<num<<std::endl;
            }
            substrings= getCategories(line,2);
            int option;
            bool isGood = false;
            bool isMistake = false;

            while (!isGood) {
                if (isMistake) {
                    std::cout << RED << "Choose one of the options: " << RESET;
                } else {
                    std::cout << YELLOW << "Choose one to be removed: " << RESET;
                }

                std::string provided;
                std::getline(std::cin, provided);

                try {
                    option = std::stoi(provided);
                    if (option >= 1 && option <= substrings.size()) {
                        isGood = true;
                    } else {
                        isGood = false;
                        isMistake = true;
                    }
                } catch (std::exception exception) {
                    isGood = false;
                    isMistake = true;
                }
            }
            input=substrings[--option];
            input.append(",");
            if (containsSubstring(line, input)) {
                size_t foundPos = line.find(input);
                if (foundPos != std::string::npos) {
                    line.erase(foundPos, input.length());
                }
            }else{
                clearTerminal();
                std::cout<<RED<<"There is not such category"<<RESET<<std::endl;
                return;
            }
            line = encodeString(line, password);
            changeLineNumber(2, line);

            std::vector<std::string> lines = getRecords(2);
            removeIfContains(input);
            std::cout<<GREEN<<"Category removed successfully"<<RESET<<std::endl;
            return;
        } else {
            clearTerminal();
            return;
        }
    } else {
        clearTerminal();
        std::cout << RED << "There is no category to remove" << RESET << std::endl;
        return;
    }
}

std::string Editing::getLineNumber(int index) {
    if (!file.is_open())
        file.open(filePath, std::ios::in | std::ios::out);
    std::string line;
    int currentLine = 0;
    while (currentLine < index && std::getline(file, line)) {
        currentLine++;
    }
    file.close();
    return line;
}

void Editing::changeLineNumber(int lineIndex, const std::string& content) {
    std::ifstream inputFile(filePath);
    std::vector<std::string> lines;
    std::string line;

    if (inputFile) {
        while (std::getline(inputFile, line)) {
            lines.push_back(line);
        }
        inputFile.close();
        if (lineIndex > 0 && lineIndex <= lines.size()) {
            lines[lineIndex - 1] = content;
        } else if (lineIndex == lines.size() + 1) {
            lines.push_back(content);
        }
    } else
        return;


    std::ofstream outputFile(filePath);
    if (outputFile) {
        for (const auto& newLine : lines) {
            outputFile << newLine << std::endl;
        }
        outputFile.close();
    } else
        return;
}

void Editing::addPassword() {
    std::string line;
    std::string  userNaming;
    std::string  userCategory;
    std::string  userLogin;
    std::string  userPassword;
    std::cout<<"Provide naming of the record or www: ";
    std::cin>>userNaming;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout<<"Provide category of the record: ";
    std::cin>>userCategory;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string lineOfCategories= getLineNumber(2);
    lineOfCategories= decodeString(lineOfCategories,password);
    if(!containsSubstring(lineOfCategories,userCategory)){
        clearTerminal();
        std::cout<<RED<<"There is no such category"<<RESET<<std::endl;
        return;
    }
    std::cout<<"Provide login: ";
    std::cin>>userLogin;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout<<"Provide password: ";
    std::cin>>userPassword;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    line=userNaming+",,"+userCategory+",,"+userLogin+",,"+userPassword+",,";
    line= encodeString(line,password);
    int x= getFirstEmptyLine(filePath);
    changeLineNumber(getFirstEmptyLine(filePath), line);
    clearTerminal();
    std::cout<<GREEN<<"Record added successfully!"<<RESET<<std::endl;
    }

int Editing::getFirstEmptyLine(const std::string &pathToFile) {
    std::ifstream tempFile(pathToFile);
    std::string line;
    int lineNumber = 0;
    int lastLineNumber = 0;

    while (std::getline(tempFile, line)) {
        lineNumber++;
        if (line.empty()) {
            tempFile.close();
            return lineNumber;
        }
        lastLineNumber = lineNumber;
    }

    tempFile.close();
    return lastLineNumber + 1;
}

void Editing::editPassword() {
    std::vector<std::string> lines = getRecords(1);
    if (lines.empty()) {
        clearTerminal();
        std::cout << RED << "There are no records to edit" << RESET << std::endl;
        return;
    }
    for (const auto &line: lines)
        std::cout << line << std::endl;
    std::cout << YELLOW << "Provide index of the record you want to change: " << RESET;
    bool isChoosen = false;
    bool isError = false;
    int index;
    while (!isChoosen) {
        if (isError) {
            std::cout << RED << "Provide correct index of the record you want to change: " << RESET;
        }
        std::string input;
        std::cin >> input;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        try {
            index = std::stoi(input);
        } catch (const std::invalid_argument &) {
            isError = true;
        } catch (const std::out_of_range &) {
            isError = true;
        }
        if (index < 1 || index > lines.size()) {
            isError = true;
            continue;
        } else {
            isChoosen=true;
        }
    }
    clearTerminal();
    lines= getRecords(2);
    int remember=--index;
    std::string line=lines[index];
    size_t pos;
    std::vector<std::string> substrings;
    while ((pos = line.find(",,", 0)) != std::string::npos) {
        line.replace(pos, 2, ";");
    }
    std::istringstream iss(line);
    std::string substring;

    while (std::getline(iss, substring, ';')) {
        substrings.push_back(substring);
    }
    for(int i=0;i<substrings.size();i++){
        std::cout<<"("<<++i<<")"<<substrings[--i]<<std::endl;
    }
    std::cout<<"Choose which part you would like to edit:";
    isChoosen=false;
    while (!isChoosen) {
        if (isError) {
            std::cout << RED << "Provide correct index of the record you want to change: " << RESET;
        }
        std::string input;
        std::cin >> input;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        try {
            index = std::stoi(input);
        } catch (const std::invalid_argument &) {
            isError = true;
        } catch (const std::out_of_range &) {
            isError = true;
        }
        if (index < 1 || index > substrings.size()) {
            isError = true;
            continue;
        } else {
            isChoosen=true;
        }
    }

    std::string userInput;
    if(index==1){
        std::cout<<"Provide new name: ";
        std::cin>>userInput;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        substrings[--index]=userInput;
    }else if(index==2){
        std::cout<<"Provide new category: ";
        std::cin>>userInput;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::string lineOfCategories= getLineNumber(2);
        lineOfCategories= decodeString(lineOfCategories,password);
        if(!containsSubstring(lineOfCategories,userInput)){
            clearTerminal();
            std::cout<<RED<<"There is no such category"<<RESET<<std::endl;
            return;
        }else{
            substrings[--index]=userInput;
        }
    }else if(index==3){
        std::cout<<"Provide new login: ";
        std::cin>>userInput;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        substrings[--index]=userInput;
    }else if(index==4){
        std::cout<<"Provide new password: ";
        std::cin>>userInput;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        substrings[--index]=userInput;
    }
    line="";
    for (auto &stt:substrings) {
        line+=stt+",,";
    }
    line= encodeString(line,password);
    changeLineNumber(remember + 3, line);
    clearTerminal();
    std::cout<<GREEN<<"Record changed succesfully!"<<RESET<<std::endl;
}

std::vector<std::string> Editing::getRecords(int option) {
        std::ifstream innerFile(filePath);
        std::string line;
        std::vector<std::string> lines;

        std::getline(innerFile, line);
        std::getline(innerFile, line);

        while (std::getline(innerFile, line)) {
            line = decodeString(line, password);
            lines.push_back(line);
        }
        innerFile.close();

        std::vector<std::string> indexedLines;
        for (int i = 0; i < lines.size(); i++) {
            indexedLines.push_back("(" + std::to_string(i + 1) + ") " + lines[i]);
        }
        if (option == 1) {
            return indexedLines;
        } else if (option == 2)
            return lines;
    }

void Editing::removePassword() {
        removeEmptyLines();
        std::vector<std::string> lines = getRecords(1);
        if (lines.empty()) {
            std::cout << RED << "There are no records to remove" << RESET << std::endl;
            return;
        }
        for (const auto &line: lines)
            std::cout << line << std::endl;
        std::cout << YELLOW << "Provide index of the record you want to remove: " << RESET;
        bool isChoosen = false;
        bool isError = false;
        while (!isChoosen) {
            if (isError) {
                std::cout << RED << "Provide correct index of the record you want to remove: " << RESET;
            }
            std::string input;
            std::cin >> input;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            int index;
            try {
                index = std::stoi(input);
            } catch (const std::invalid_argument &) {
                isError = true;
            } catch (const std::out_of_range &) {
                isError = true;
            }
            if (index < 1 || index > lines.size()) {
                isError = true;
                continue;
            } else {
                isChoosen = true;
                lines = getRecords(2);
                std::string line = lines[index - 1];
                line = "";
                changeLineNumber(index + 2, line);
                removeEmptyLines();
                clearTerminal();
                std::cout << GREEN << "Record removed successfully!" << RESET << std::endl;
            }
        }
    }

void Editing::removeEmptyLines() {
        std::ifstream inputFile(filePath);
        std::vector<std::string> lines;

        std::string line;
        while (std::getline(inputFile, line)) {
            if (!line.empty()) {
                lines.push_back(line);
            }
        }

        inputFile.close();

        std::ofstream outFile(filePath);
        for (const auto &lll: lines) {
            outFile << lll << std::endl;
        }
        outFile.close();
    }
std::vector<std::string> Editing::getCategories(const std::string &input, int flag) {
        std::string workingString = input;

        size_t found = workingString.find(",,");
        while (found != std::string::npos) {
            workingString.replace(found, 2, ",");
            found = workingString.find(",,");
        }

        std::vector<std::string> substrings;
        std::istringstream iss(workingString);
        std::string substring;
        while (std::getline(iss, substring, ',')) {
            substrings.push_back(substring);
        }

        if (flag == 1) {
            for (int i = 0; i < substrings.size(); i++) {
                substrings[i] = "(" + std::to_string(i + 1) + ") " + substrings[i];
            }
        }

        return substrings;
    }

void Editing::listRecors() {
        clearTerminal();
        std::ifstream innerFile(filePath);
        std::string line;
        std::vector<std::string> lines;

        std::getline(innerFile, line);
        std::getline(innerFile, line);

        while (std::getline(innerFile, line)) {
            line = decodeString(line, password);
            size_t pos;
            while ((pos = line.find(",,", 0)) != std::string::npos) {
                line.replace(pos, 2, ";");
            }
            lines.push_back(line);
        }
        innerFile.close();
        for (std::string &str: lines) {
            std::stringstream ss(str);
            std::string substring;
            std::vector<std::string> substrings;

            while (std::getline(ss, substring, ';')) {
                substrings.push_back(substring);
            }

            if (substrings.size() == 4) {
                str = "Name: " + substrings[0] + "  Category: " + substrings[1] + "  Login: " + substrings[2] + "  Password: " +
                      substrings[3];
            } else {
                std::cout << "Invalid string format: " << str << std::endl;
            }
        }
        if(lines.empty()){
            clearTerminal();
            std::cout<<RED<<"There are no records"<<RESET<<std::endl;
            return;
        }
        for (int i = 0; i < lines.size(); ++i) {
            std::cout << "(" << i + 1 << ") " << lines[i] << std::endl;
        }
        std::string rAnDoMNoThInG;

        std::cout << YELLOW << "Type anything to continue: " << RESET;
        std::cin >> rAnDoMNoThInG;
        clearTerminal();
   }

void Editing::removeIfContains(std::string &str) {
        std::ifstream inFile(filePath);
        std::vector<std::string> lines;
        std::string line;
        std::regex pattern(R"(\w+,,\w+,,\w+,,\w+,,)");


        while (std::getline(inFile, line)) {
            std::string decodedLine = decodeString(line, password);
            std::string copyLine=decodedLine;
            if (!std::regex_search(decodedLine, pattern)) {
                continue;
            }
            size_t pos;
            while ((pos = decodedLine.find(",,", 0)) != std::string::npos) {
                decodedLine.replace(pos, 2, ";");
            }
            std::vector<std::string> substrings;
            std::istringstream iss(decodedLine);
            std::string substring;

            while (std::getline(iss, substring, ';')) {
                substrings.push_back(substring);
            }
            std::string withoutLastChar = str.substr(0, str.size() - 1);
            if(substrings[1]==withoutLastChar){
                lines.push_back(copyLine);
            }
        }
        inFile.close();
        findAndRemove(lines);
    }

void Editing::findAndRemove(std::vector<std::string> vec) {
    for(auto &line:vec)
        line= encodeString(line,password);
    int currentLine = 1;
    std::string line;
    if (!file.is_open())
        file.open(filePath,std::ios::in | std::ios::out);
    while(std::getline(file,line)){
        for(auto& vecLine:vec){
            if (vecLine==line){
                changeLineNumber(currentLine, "");
            }
        }
        currentLine++;
    }
    removeEmptyLines();
}

void Editing::sortPassword() {
    std::cout<<"(1) Name\n"
               "(2) Category\n"
               "(3) Login\n"
               "(4) Password\n"<<std::endl;
    bool isChoosen=false;
    bool isError= false;
    int index;
    while (!isChoosen) {
        if (isError) {
            std::cout << RED << "Provide correct index of the option you want to sort: " << RESET;
        }else{
            std::cout << YELLOW << "Provide index of the option you want to sort: " << RESET;
        }
        std::string input;
        std::cin >> input;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        try {
            index = std::stoi(input);
        } catch (const std::invalid_argument &) {
            isError = true;
        } catch (const std::out_of_range &) {
            isError = true;
        }
        if (index < 1 || index > 4) {
            isError = true;
            continue;
        } else {
            isChoosen=true;
        }
    }
    int  flag=index;
    bool ascending=true;
    std::ifstream innerFile(filePath);
    std::string line;
    std::vector<std::string> lines;

    std::getline(innerFile, line);
    std::getline(innerFile, line);

    while (std::getline(innerFile, line)) {
        line = decodeString(line, password);
        lines.push_back(line);
    }
    innerFile.close();



    std::vector<std::string> sortedLines(lines.begin(), lines.end());  // Copy all lines
    std::string delimiter = ",,";
    size_t flagIndex = flag - 1;

    std::sort(sortedLines.begin(), sortedLines.end(), [&](const std::string& a, const std::string& b) {
        std::vector<std::string> substringsA, substringsB;

        std::string lineA = a;
        size_t posA = 0;
        while ((posA = lineA.find(delimiter)) != std::string::npos) {
            substringsA.push_back(lineA.substr(0, posA));
            lineA = lineA.substr(posA + delimiter.length());
        }
        substringsA.push_back(lineA);

        std::string lineB = b;
        size_t posB = 0;
        while ((posB = lineB.find(delimiter)) != std::string::npos) {
            substringsB.push_back(lineB.substr(0, posB));
            lineB = lineB.substr(posB + delimiter.length());
        }
        substringsB.push_back(lineB);

        if (ascending)
            return substringsA[flagIndex] < substringsB[flagIndex];
        else
            return substringsA[flagIndex] > substringsB[flagIndex];
    });
    if(!file.is_open())
        file.open(filePath,std::ios::out | std::ios::in);
    line="";
    std::getline(file,line);
    std::getline(file,line);
    int curLine=3;
    while(std::getline(file,line)){
        std::string tmpline= encodeString(sortedLines[curLine-3],password);
        changeLineNumber(curLine, tmpline);
        curLine++;
    }
    listRecors();
}

void Editing::searchPasswords() {
    clearTerminal();
    std::cout<<"Provide searching parametr: ";
    std::string input;
    std::cin>>input;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::ifstream inFile(filePath);
    std::vector<std::string> lines;
    std::string line;
    std::getline(inFile, line);
    std::getline(inFile, line);
    while (std::getline(inFile, line)) {
        line= decodeString(line,password);
        size_t pos = 0;
        std::string doubleComma = ",,";
        std::string singleSemicolon = "; ";

        while ((pos = line.find(doubleComma, pos)) != std::string::npos) {
            line.replace(pos, doubleComma.length(), singleSemicolon);
            pos += singleSemicolon.length();
        }
        if(containsTarget(input,line))
            lines.push_back(line);
    }
    if(lines.size()==0){
        clearTerminal();
        std::cout<<RED<<"No records containing provided parameter"<<RESET<<std::endl;
        return;
    }else{
        clearTerminal();
        for (auto &fff:lines) {
            std::cout<<fff<<std::endl;
        }
        std::cout<<YELLOW<<"Type anything to continue: "<<RESET;
        std::cin>>input;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        clearTerminal();
        return;
    }
}
bool Editing::containsTarget(const std::string& target, const std::string& pool) {
    for (size_t i = 0; i <= pool.length() - target.length(); ++i) {
        if (pool.substr(i, target.length()) == target) {
            return true;
        }
    }
    return false;
}
