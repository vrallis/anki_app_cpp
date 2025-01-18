#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <string>
#include <vector>

/**
 * @class FileHandler
 * @brief A class responsible for handling file operations related to card import.
 */

/**
 * @brief Imports cards from a specified file.
 * 
 * @param filePath The path to the file from which cards are to be imported.
 * @return A vector of pairs, where each pair contains a question and its corresponding answer.
 */
class FileHandler {
public:
    static std::vector<std::pair<std::string, std::string>> importCards(const std::string& filePath);
};

#endif // FILE_HANDLER_H