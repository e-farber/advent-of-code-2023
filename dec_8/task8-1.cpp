#include <iostream>
#include <fstream>
#include <vector>
#include <map>

/**
 * December 8th, task 8-1
 * adventofcode.com/2023/day/8
 * 
 * Short summary:
 * Use the left/right instructions to navigate the network. Navigate from AAA
 * to ZZZ by looking up each next element based on the next L/R instruction
 * from the first line. How many steps are required to reach ZZZ?
*/

// ##### Solution #####

using paper_sheet = std::map<std::string, std::pair<std::string, std::string>>;

paper_sheet network;


int main() {
    std::ifstream inputFile;
    std::string current_line;
    size_t line_number {0};
    std::string instructions;

    inputFile.open("./test_input.txt");

    if (!inputFile.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    // iterate over each line in input document
    while (std::getline(inputFile, current_line)) {
        ++line_number;

        if (line_number == 1) {
            instructions = current_line;
        }

        if (line_number >= 3) {
            std::string start_node {current_line.substr(0, 3)};
            std::string next_left  {current_line.substr(7, 3)};
            std::string next_right {current_line.substr(12, 3)};
            network[start_node] = std::make_pair(next_left, next_right);
        }
    }

    std::string current_position {"AAA"};
    size_t steps {0};
    size_t index {0};
    while (current_position != "ZZZ") {
        std::cout << current_position << " (" << instructions[index] << ")-> ";
        if (instructions[index] == 'L') {
            std::cout << network[current_position].first << std::endl;
            current_position = network[current_position].first;
        }
        else {
            std::cout << network[current_position].second << std::endl;
            current_position = network[current_position].second;
        }
        ++steps;
        index = (index + 1) % instructions.length();
    }


    std::cout << "result: " << steps << std::endl;
    return 0;
}
