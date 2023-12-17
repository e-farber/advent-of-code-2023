#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

/**
 * December 12th, task 12-1
 * adventofcode.com/2023/day/12
 * 
 * Short summary:
 * Outside, springs are arranged into rows. For each row, the input shows every
 * spring and whether it is operational (.) or damaged (#). For some springs,
 * it is simply unknown (?) whether the spring is operational or damaged.
 * After the list of springs for a given row, the size of each contiguous group
 * of damaged springs is listed in the order those groups appear in the row.
 * For each row, count all of the different arrangements of operational and
 * broken springs that meet the given criteria. What is the sum of those counts?
*/

// ##### Solution #####


bool is_digit(const char input);
// registers next number in a line and advances iterator one position beyond its end
size_t get_next_number(std::string::iterator &iterator);
std::vector<size_t> contiguous_groups(std::string_view functional_list);
size_t count_possibilities(
    std::string_view operation_info,
    const std::vector<size_t> &group_sizes
);


int main() {
    std::ifstream inputFile;
    std::string current_line;
    size_t result {0};

    inputFile.open("./input.txt");

    if (!inputFile.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    while (std::getline(inputFile, current_line)) {
        auto iterator {current_line.begin()};
        std::vector<size_t> group_sizes;

        while (*iterator != ' ') {
            ++iterator;
        }
        std::string operation_info {current_line.begin(), iterator};
        while (iterator != current_line.end()) {
            group_sizes.push_back(get_next_number(iterator));
        }

    result += count_possibilities(operation_info, group_sizes);
    }
 
    std::cout << "result: " << result << std::endl;
    return 0;
}



bool is_digit(const char input) {
    int ascii_dec {static_cast<int>(input)};
    return (ascii_dec >= 48 && ascii_dec <= 57);
}


size_t get_next_number(std::string::iterator &iterator) {
    std::vector<size_t> digits;
    size_t result {0};

    while (!is_digit(*iterator)) {
        ++iterator;
    }

    while (is_digit(*iterator)) {
        for (auto &digit : digits) {
            digit *= 10;
        }
        digits.push_back(static_cast<size_t>(*iterator) - 48);
        ++iterator;
    }

    for (auto &digit : digits) {
        result += digit;
    }

    return result;
}


std::vector<size_t> contiguous_groups(std::string_view functional_list) {
    std::vector<size_t> result;
    auto iterator {functional_list.begin()};
    while (*iterator != ' ' && iterator != functional_list.end()) {
        size_t group_size {0};
        while (*iterator == '.') {
            ++iterator;
        }
        while (*iterator == '#') {
            ++group_size;
            ++iterator;
        }
        if (group_size != 0) {
            result.push_back(group_size);
        }
    }
    return result;
}


size_t count_possibilities(
    std::string_view operation_info,
    const std::vector<size_t> &group_sizes
    )
{
    size_t result {0};
    std::vector<size_t> variable_positions;
    for (size_t index {0}; index < operation_info.length(); ++index) {
        if (operation_info[index] == '?') {
            variable_positions.push_back(index);
        }
    }

    // TODO : next 7 lines super inelegant, find simpler solution
    std::string functional_info {operation_info};
    for (size_t index {0}; index < variable_positions.size(); ++index) {
        functional_info[variable_positions[index]] = '.';
    }
    if (contiguous_groups(functional_info) == group_sizes) {
        ++result;
    }

    for (size_t flipped_bits {0}; flipped_bits < variable_positions.size(); ++flipped_bits) {
        // bitset marking '?'s to be set to '#'
        std::vector<bool> qmarks(variable_positions.size());
        for (size_t index {0}; index <= flipped_bits; ++index) {
            qmarks[qmarks.size() - 1 - index] = true;
        }

        do /* (std::next_permutation(qmarks.begin(), qmarks.end())) */ {
            std::string fixed_operation {operation_info};
            for (size_t index {0}; index < variable_positions.size(); ++index) {
                if (qmarks[index]) {
                    fixed_operation[variable_positions[index]] = '#';
                }
                else {
                    fixed_operation[variable_positions[index]] = '.';
                }
            }

            if (contiguous_groups(fixed_operation) == group_sizes) {
                ++result;
            }

        } while (std::next_permutation(qmarks.begin(), qmarks.end()));
    }

    return result;
}
