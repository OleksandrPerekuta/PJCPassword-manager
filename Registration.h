//
// Created by Alexandr on 20.05.2023.
//

#ifndef UNTITLED_REGISTRATION_H
#define UNTITLED_REGISTRATION_H

#include <fstream>
#include <string>

class Registration {
private:
    ///Password to the file
    std::string password;
    ///Working file
    std::fstream file;
    ///Path to the working file
    std::string path;
    /// @brief Generates a random file name.
/// @return Random file name.
    std::string generateRandomNameOfTheFile();

/**
 * @brief Helper function that generates a timestamp of the current time.
 * @return Current time in the format "hh:mm:ss DD/MM/YYYY".
 * @example 12:52:13 01/06/2023
 */
    static std::string getTimestampOfCurrentMoment();

/**
 *@brief Helper function that encodes the password of the working file.
 * @return Encoded password string.
 * @example abcd123 -> ўЈ¤Ґrst
 */

    std::string encodePasswordInTheFirstLine(const std::string& inputS);
    /// @brief Decorative function that cleans current page of terminal
    void clearTerminal();
public:
     /// @brief Constructor of the class
    Registration();
     /// @brief Returns @b original password to the file that will be used in the future
    std::string getPassword();

    /// @brief Returns path to the working file
    std::string getPath();
};


#endif //UNTITLED_REGISTRATION_H
