//
// Created by Alexandr on 27.05.2023.
//

#ifndef UNTITLED_EDITING_H
#define UNTITLED_EDITING_H

#include <fstream>
#include <string>

class Editing {
private:
    /**
     * @brief Working file
     */
    std::fstream file;
    /**
     * @brief Password to the file
     */
    std::string password;
    /**
     * @brief Path to the working file
     */
    std::string filePath;
    /**
     * @brief Encodes a string using password, by shifting sequent characters of a string by sequent character of the key
     * @param source a string that should be encoded by the key
     * @param key a string by which source is encoded
     * @return Returns encoded source
     */
    std::string encodeString(const std::string& source, const std::string& key);
    /**
     * @brief Decodes a string using password, by shifting (back) sequent characters of a string by sequent character of the key
     * @param source a string that should be encoded by the key
     * @param key a string by which source is encoded
     * @return Returns decoded source
     */
    std::string decodeString(const std::string& source, const std::string& key);
    /**
     * @brief Adds a category, adding the ability to use it when adding a password
     * @param name is a name of the added category
     */
    void addCategory(const std::string& name);
    /**
     * @brief Removes a category, removing the ability to use it when adding a password
     * @param name is a name of the removed category
     * @note Removes all records that have this category as a parameter
     */
    void removeCategory(const std::string& name);
    /**
     * @brief checks if string @b pool contain string @b target
     * @param target is what is being searched
     * @param pool is where target is searched
     * @return @b true if @e target is found in @e pool, @b false if @e target is not found in @e pool
     */
    static bool containsSubstring(const std::string& target, const std::string& pool);
    /// @brief Decorative function that cleans current page of terminal
    void clearTerminal();
    /**
     * @param index is number of the line in the file that should be returned
     * @return string of @e line in the file
     */
    std::string getLineNumber(int index);
    /**
     * @brief Changes contents of line @e #lineIndex in the file by @e content
     * @param lineIndex
     * @param content
     */
    void changeLineNumber(int lineIndex, const std::string& content);
    /**
     * @brief Creates a record that contains description, category, login, password and writes it to the end of a working file
     */
    void addPassword();
    /**
     * @brief Changes @b one parameter of a @b one record in the file
     */
    void editPassword();
    /**
     * @brief Removes @b one record with password
     */
    void removePassword();
    /**
     * @brief Helper function that removes empty lines in the file. Used in @ref removeCategory() and @ref removePassword()
     */
    void removeEmptyLines();
    /**
     * @brief Removes records with passwords that contain @b std
     * @param std
     */
    void removeIfContains(std::string & std);
    /**
     * @brief Gets index of the first empty line, used in @ref addPassword()
     * @param pathToFile is path to a file in which empty lines are looked for
     * @return returns index of the first empty line @b if @b found, or index is 1 more than the last line if the file is full
     */
    static int getFirstEmptyLine(const std::string& pathToFile);
    /**
     *
     * @param option @b flag that defines whether or not records with passwords should be @e indexed or @e not
     * @return @e indexed/not records @b to @b work @b with or @b to @b display
     */
    std::vector<std::string> getRecords(int option);
    /**
     * @brief Basically, separates categories from one line to a vector of strings
     * @param string of categories
     * @param flag  defines whether or not categories should be @e indexed or @e not
     * @return vector of separated categories
     */
    static std::vector<std::string> getCategories(const std::string& string, int flag);
    /**
     * @brief Displays all records with passwords
     */
    void listRecors();
    /**
     *@brief Finds and removes in file lines that were provided as a vector
     * @param vec lines that should be removed
     */
    void findAndRemove(std::vector<std::string> vec);
    /**
     * @brief Sorts records by a chosen option: description, category, login, password
     * @note Records added @b after sorting files @b will @b not be sorted
     */
    void sortPassword();
    /**
     * @brief Searches and displays records that contain @b input (provided by a user)
     */
    void searchPasswords();
    /**
     *
     * @param target is what is searched
     * @param pool is where @b target searched
     * @return @b true if target is found, @b false is it was not
     */
    static bool containsTarget(const std::string& target, const std::string& pool);
public:
    /**
     * @brief Constructor of the class
     * @param pathToFile is a path to the working file
     * @param pass is the password to the working file
     */
    Editing(const std::string& pathToFile, const std::string& pass);
};


#endif //UNTITLED_EDITING_H
