#include <iostream>
#include <fstream>

/**
 * December 15th, task 15-1
 * adventofcode.com/2023/day/15
 * 
 * Short summary:
 * The initialization sequence is a comma-separated list. Run the HASH
 * algorithm on each step in the initialization sequence:
 *   Determine the ASCII code for the current character of the string.
 *   Increase the current value by the ASCII code you just determined.
 *   Set the current value to itself multiplied by 17.
 *   Set the current value to the remainder of dividing itself by 256.
 * What is the sum of the results?
*/

// ##### Solution #####

std::string get_next_step(
    std::string::iterator &iterator,
    const std::string::iterator &end_pos
);
size_t hash(std::string_view input);


int main() {
    std::ifstream inputFile;
    std::string sequence;
    size_t result {0};

    inputFile.open("./input.txt");

    if (!inputFile.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    std::getline(inputFile, sequence);
    
    auto iterator {sequence.begin()};
    while (iterator != sequence.end()) {
        std::string current_step {get_next_step(iterator, sequence.end())};
        result += hash(current_step);
    }

    std::cout << "result: " << result << std::endl;
    return 0;
}



std::string get_next_step(
    std::string::iterator &iterator,
    const std::string::iterator &end_pos
) {
    std::string result {""};
    if (*iterator == ',') {
        ++iterator;
    }

    while (*iterator != ',' && iterator != end_pos) {
        result += *iterator;
        ++iterator;
    }

    return result;
}


size_t hash(std::string_view input) {
    size_t result {0};
    for (auto &symbol : input) {
        result += static_cast<size_t>(symbol);
        result *= 17;
        result %= 256;
    }
    return result;
}
