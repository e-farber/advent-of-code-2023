#include <iostream>
#include <fstream>
#include <vector>

/**
 * December 14th, task 14-1
 * adventofcode.com/2023/day/14
 * 
 * Short summary:
 * Your input depicts a platform with rounded rocks (O) that will roll when the
 * platform is tilted, and cube-shaped rocks (#) that will stay in place, as
 * well as the positions of all empty spaces (.). Start by tilting the platform
 * so all of the rocks will slide north as far as they will go. The amount of
 * load caused by a single rounded rock (O) is equal to the number of rows from
 * the rock to the south edge of the platform, including the row the rock is on.
 * Cube-shaped rocks (#) don't contribute to load.
 * What is the total load after tilting the platform?
*/

// ##### Solution #####

using notes = std::vector<std::string>;

void tilt_platform(notes &rock_positions);
size_t calculate_load (const notes &rock_positions);


int main() {
    std::ifstream inputFile;
    std::string current_line;

    inputFile.open("./input.txt");

    if (!inputFile.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    notes rock_positions;
    while (std::getline(inputFile, current_line)) {
        rock_positions.push_back(current_line);
    }
    tilt_platform(rock_positions);
    size_t result {calculate_load(rock_positions)};

    std::cout << "result: " << result << std::endl;
    return 0;
}



void tilt_platform(notes &rock_positions) {
    size_t width {rock_positions[0].size()};
    for (size_t col_index {0}; col_index < width; ++col_index) {
        for (size_t row_index {1}; row_index < rock_positions.size(); ++row_index) {
            if (rock_positions[row_index][col_index] != 'O') {
                continue;
            }

            size_t rock_index {row_index};
            while (rock_positions[rock_index    ][col_index] == 'O' &&
                   rock_positions[rock_index - 1][col_index] == '.'
                ) {
                rock_positions[rock_index    ][col_index] = '.';
                rock_positions[rock_index - 1][col_index] = 'O';
                --rock_index;

                if (rock_index == 0) {
                    break;
                }
            }
        }
    }
}


size_t calculate_load (const notes &rock_positions) {
    size_t result {0};
    size_t height {rock_positions.size()};
    size_t width  {rock_positions[0].size()};
    for (size_t row_index {0}; row_index < height; ++row_index) {
        for (size_t col_index {0}; col_index < width; ++col_index) {
            if (rock_positions[row_index][col_index] == 'O') {
                result += height - row_index;
            }
        }
    }
    return result;
}
