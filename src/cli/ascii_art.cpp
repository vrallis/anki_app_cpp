#include "ascii_art.h"
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>

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