#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <numeric>

/**
 * December 8th, task 8-2
 * adventofcode.com/2023/day/8
 * 
 * Short summary:
 * Use the left/right instructions to navigate the network.
 * Start at every node that ends with A and follow all of the paths at the
 * same time until they all simultaneously end up at nodes that end with Z.
 * Look up each next element based on the next L/R instruction from the first line.
 * How many steps are required to reach ZZZ?
*/

// ##### Solution #####

using paper_sheet = std::map<std::string, std::pair<std::string, std::string>>;

paper_sheet network;

struct least_common_multiple {
    size_t operator()(const size_t lhs, const size_t rhs) const {
        return std::lcm(lhs, rhs);
    }
};


int main() {
    std::ifstream inputFile;
    std::string current_line;
    size_t line_number {0};
    std::string instructions;
    std::vector<std::string> start_nodes;

    inputFile.open("./input.txt");

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
            if (start_node[2] == 'A') {
                start_nodes.push_back(start_node);
            }
            std::string next_left  {current_line.substr(7, 3)};
            std::string next_right {current_line.substr(12, 3)};
            network[start_node] = std::make_pair(next_left, next_right);
        }
    }

    std::vector<size_t> steps;
    for (size_t index {0}; index < start_nodes.size(); ++index) {
        std::string current_position {start_nodes[index]};
        steps.push_back(0);
        size_t direction_index {0};
        while (current_position[2] != 'Z') {
            if (instructions[direction_index] == 'L') {
                current_position = network[current_position].first;
            }
            else {
                current_position = network[current_position].second;
            }
            direction_index = (direction_index + 1) % instructions.length();
            ++steps[index];
        }
    }

    size_t result {
        std::accumulate(steps.begin(), steps.end(), 1ull, least_common_multiple())
    };
    std::cout << "result: " << result << std::endl;
    return 0;
}
