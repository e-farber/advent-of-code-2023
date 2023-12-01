#include <iostream>
#include <string>
#include <fstream>
#include <cctype>


/**
 * December 1st, task 1-1
 * adventofcode.com/2023/day/1
 * 
 * Short summary: 
 * On each line, the calibration value can be found by combining the first digit and the last digit
 * (in that order) to form a single two-digit number.
 * 
 * Consider your entire calibration document. What is the sum of all of the calibration values?
*/

// ##### Solution #####

using digit_t = size_t;

digit_t get_first_digit(std::string_view input_string);
digit_t get_last_digit(std::string_view input_string);


int main() {
    size_t result {0};
    std::ifstream inputFile;
    std::string content;

    inputFile.open("./input.txt");
    if (inputFile.is_open()) {
        while (std::getline(inputFile, content)) {
            digit_t first_digit {get_first_digit(content)};
            digit_t second_digit {get_last_digit(content)};
            result += 10 * first_digit + second_digit;
        }
    }
    
    std::cout << result << std::endl;
    return 0;
}



digit_t get_first_digit(std::string_view input_string) {
    for (auto &it : input_string) {
        if (std::isdigit(static_cast<unsigned char>(it))) {
            std::string result {it};
            return std::stoull(result.c_str(), nullptr);
        }
    }
}


digit_t get_last_digit(std::string_view input_string) {
    for (size_t right_index {input_string.length() - 1}; right_index >= 0; --right_index) {
        if (std::isdigit(static_cast<unsigned char>(input_string[right_index]))) {
            std::string result {input_string[right_index]};
            return std::stoull(result.c_str(), nullptr);
        }
    }
}