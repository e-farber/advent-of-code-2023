#include <iostream>
#include <fstream>
#include <vector>
#include <set>

/**
 * December 14th, task 14-2
 * adventofcode.com/2023/day/14
 * 
 * Short summary:
 * Your input depicts a platform with rounded rocks (O) that will roll when the
 * platform is tilted, and cube-shaped rocks (#) that will stay in place, as
 * well as the positions of all empty spaces (.). Activating one cycle tilts the
 * platform so that the rounded rocks roll north, then west, then south, then
 * east. After each tilt, the rounded rocks roll as far as they can before the
 * platform tilts in the next direction.
 * The amount of load caused by a single rounded rock (O) is equal to the number
 * of rows from the rock to the south edge of the platform, including the row
 * the rock is on. Cube-shaped rocks (#) don't contribute to load.
 * Calculate the total load after 1'000'000'000 cycles.
*/

// ##### Solution #####

using notes = std::vector<std::string>;

void tilt_north(notes &rock_positions);
void tilt_east (notes &rock_positions);
void tilt_south(notes &rock_positions);
void tilt_west (notes &rock_positions);
void cycle (notes &rock_positions, size_t times = 1);
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

    // cycle a bunch of times so rocks fall into cycles
    auto rock_positions_copy {rock_positions};
    cycle(rock_positions_copy, 500);
    // see how long it takes for pattern to repeat
    std::set<notes> copy_configurations;
    size_t cycle_length{0};
    while(copy_configurations.insert(rock_positions_copy).second) {
        cycle(rock_positions_copy);
        ++cycle_length;
    }

    // figure out cycle plus onset length
    std::set<notes> configurations;
    size_t cycle_length_plus_onset {0};
    while(configurations.insert(rock_positions).second) {
        cycle(rock_positions);
        ++cycle_length_plus_onset;
    }
    
    const size_t rest {(1'000'000'000 - cycle_length_plus_onset) % cycle_length};
    cycle(rock_positions, rest);

    size_t result {calculate_load(rock_positions)};

    std::cout << "result: " << result << std::endl;
    return 0;
}



void tilt_north(notes &rock_positions) {
    size_t width {rock_positions[0].size()};
    for (size_t col_index {0}; col_index < width; ++col_index) {
        for (size_t row_index {0}; row_index < rock_positions.size(); ++row_index) {
            if (row_index == 0 ||
                rock_positions[row_index][col_index] != 'O') {
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


void tilt_east(notes &rock_positions) {
    for (auto &row : rock_positions) {
        for (int col_index {static_cast<int>(row.length()) - 2};
             col_index >= 0;
             --col_index
             ) {
            if (row[col_index] != 'O') {
                continue;
            }

            int rock_index {col_index};
            while (row[rock_index] == 'O' && row[rock_index + 1] == '.') {
                row[rock_index] = '.';
                row[rock_index + 1] = 'O';
                ++rock_index;
                if (rock_index == row.length() - 1) {
                    break;
                }
            }
        }
    }
}


void tilt_south(notes &rock_positions) {
    size_t width {rock_positions[0].size()};
    for (size_t col_index {0}; col_index < width; ++col_index) {
        for (int row_index {static_cast<int>(rock_positions.size()) - 2};
             row_index >= 0;
             --row_index
             ) {
            if (rock_positions[row_index][col_index] != 'O') {
                continue;
            }
            int rock_index {row_index};
            while (rock_positions[rock_index    ][col_index] == 'O' &&
                   rock_positions[rock_index + 1][col_index] == '.'
                ) {
                rock_positions[rock_index    ][col_index] = '.';
                rock_positions[rock_index + 1][col_index] = 'O';
                ++rock_index;
                if (rock_index == rock_positions.size() - 1) {
                    break;
                }
            }
        }
    }
}


void tilt_west(notes &rock_positions) {
    for (auto &row : rock_positions) {
        for (size_t col_index {1}; col_index < row.length(); ++col_index) {
            if (row[col_index] != 'O') {
                continue;
            }

            size_t rock_index {col_index};
            while (row[rock_index] == 'O' && row[rock_index - 1] == '.') {
                row[rock_index] = '.';
                row[rock_index - 1] = 'O';
                --rock_index;
                if (rock_index == 0) {
                    break;
                }
            }
        }
    }
}


void cycle(notes &rock_positions, size_t times) {
    for (size_t amount {0}; amount < times; ++amount) {
        tilt_north(rock_positions);
        tilt_west (rock_positions);
        tilt_south(rock_positions);
        tilt_east (rock_positions);
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
