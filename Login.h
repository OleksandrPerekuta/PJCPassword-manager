//
// Created by Alexandr on 21.05.2023.
//

#ifndef UNTITLED_LOGIN_H
#define UNTITLED_LOGIN_H
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
class Login {
private:
    ///Working file
    std::fstream file;
    ///Password to the file
    std::string password;
    ///Path to the working file
    std::string path;
    ///@brief Helper function that generates a timestamp of the current time.
    ///@return Current time in the format "hh:mm:ss DD/MM/YYYY".
    ///@example 12:52:13 01/06/2023

    static std::string getTimestampOfCurrentMoment();
    /**
     * @brief Gets all .txt files in current directory
     * @return Vector of txt files in current directory
     */
    static std::vector<std::filesystem::path> getTxtFilesInCurrentDirectory();
    /**
     * @param directoryPath is a path to a desired directory
     * @return Vector of txt files in custom directory
     */
    static std::vector<std::filesystem::path> getTxtFilesInDirectory(const std::string& directoryPath);
    /**
     * @param filePath is a path to a desired file
     * @return Returns file if it is possible to open
     */
    static std::fstream getCustomFile(const std::string& filePath);
    /**
     * @brief Checks the first line by regex to validate if it is a file with passwords or not
     * @param pathToFile is path to a desired file
     * @return @b true if it is file with passwords, @b false if it is not
     */
    bool checkFirstLine(const std::string& pathToFile);
    /**
     * @brief Asks user for a password to the file
     * @param filePath is path to a desired file
     */
    void enterTheFile(const std::string& filePath);
    /**
     *
     * @param pathToFile is a path to a desired file
     * @param possiblePassword is a password that was provided by user to access the file
     * @return @b true if provided by user password is an actual one, @b false if it is not
     */
    static bool checkThePassword(const std::string& pathToFile, const std::string& possiblePassword);
    /**
     * @brief changes timestamp if it was recognized as a file with passwords, regardless if password was correct
     * @param pathToFile is path to a desired file
     */
    static void changeTheTimeStamp(const std::string& pathToFile);
    /**
     * @brief Asks user what is a working file
     * @param files is a vector of .txt files in a directory
     */
    void listAndSelectFiles(const std::vector<std::filesystem::path>& files);
    /// @brief Decorative function that cleans current page of terminal
    static void clearTerminal();
public:
    /// @brief Constructor of the class
    Login();
    /// @brief Returns path to the working file
    std::string getPath();
    /// @brief Returns @b original password to the file that will be used in the future
    std::string getPassword();
};


#endif //UNTITLED_LOGIN_H
