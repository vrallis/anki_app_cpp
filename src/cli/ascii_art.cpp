#include "ascii_art.h"
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>

/**
 * @brief Generates ASCII art from the given text using the `figlet` command.
 * 
 * This function takes a string input and generates ASCII art by executing the
 * `figlet` command-line tool. The resulting ASCII art is captured and returned
 * as a string.
 * 
 * @param text The input text to be converted into ASCII art.
 * @return A string containing the generated ASCII art.
 * @throws std::runtime_error if the `popen` call fails.
 */
std::string generateAsciiArt(const std::string& text) {
    std::string command = "figlet " + text;
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}