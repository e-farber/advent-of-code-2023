#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <algorithm> // reverse

/**
 * December 1st, task 1-2
 * adventofcode.com/2023/day/1
 * 
 * Short summary: 
 * On each line, the calibration value can be found by combining the first digit and the last digit
 * (in that order) to form a single two-digit number.
 * "one", "two", "three", ..., and "nine" also count as valid "digits".
 * Consider your entire calibration document. What is the sum of all of the calibration values?
*/

// ##### Solution #####

using digit_t = size_t;
using digit_pair = std::pair<std::string, digit_t>;

digit_t get_first_digit(std::string_view input_string);
digit_t get_last_digit(std::string       input_string);

const std::unordered_map<std::string, digit_t> digits {
    digit_pair("1", 1),
    digit_pair("2", 2),
    digit_pair("3", 3),
    digit_pair("4", 4),
    digit_pair("5", 5),
    digit_pair("6", 6),
    digit_pair("7", 7),
    digit_pair("8", 8),
    digit_pair("9", 9),
    digit_pair("one",   1),
    digit_pair("two",   2),
    digit_pair("three", 3),
    digit_pair("four",  4),
    digit_pair("five",  5),
    digit_pair("six",   6),
    digit_pair("seven", 7),
    digit_pair("eight", 8),
    digit_pair("nine",  9)
};

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
    size_t pos {666};
    digit_t result {1337};

    for (auto & [key, value] : digits) {
        size_t new_pos {input_string.find(key)};
        if (new_pos < pos) {
            pos = new_pos;
            result = value;
        }
    }

    return result;
}


digit_t get_last_digit(std::string input_string) {
    size_t pos {666};
    digit_t result {1337};
    std::reverse(input_string.begin(), input_string.end());

    for (auto &[key, value] : digits) {
        std::string key_copy {key};
        std::reverse(key_copy.begin(), key_copy.end());
        size_t new_pos {input_string.find(key_copy)};
        if (new_pos < pos) {
            pos = new_pos;
            result = value;
        }
    }

    return result;
}