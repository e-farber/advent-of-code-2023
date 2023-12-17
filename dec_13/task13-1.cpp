#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

/**
 * December 13th, task 13-1
 * adventofcode.com/2023/day/13
 * 
 * Short summary:
 * To find the reflection in each pattern, you need to find a perfect
 * reflection across either a horizontal line between two rows or across a
 * vertical line between two columns. 
 * Add up the number of columns to the left of each vertical line of reflection;
 * to that, also add 100 multiplied by the number of rows above each horizontal
 * line of reflection. What number do you get?
*/

// ##### Solution #####

using landscape = std::vector<std::string>;

landscape rotated(const landscape &pattern);
size_t get_horizontal_axis(const landscape &pattern);


int main() {
    std::ifstream inputFile;
    std::string current_line;
    size_t result {0};

    inputFile.open("./input.txt");

    if (!inputFile.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    landscape pattern;
    while (std::getline(inputFile, current_line)) {
        bool pattern_complete {false};
        if (current_line.length() > 0) {
            pattern.push_back(current_line);
        }
        else {
            pattern_complete = true;
        }

        if (!pattern_complete) {
            continue;
        }

        result += get_horizontal_axis(pattern) * 100;
        result += get_horizontal_axis(rotated(pattern));

        pattern.clear();
    }
 
    std::cout << "result: " << result << std::endl;
    return 0;
}



landscape rotated(const landscape &pattern) {
    landscape rotated_pattern;
    for (size_t row_index {0}; row_index < pattern.size(); ++row_index) {
        for (size_t col_index {0}; col_index < pattern[0].length(); ++col_index) {
            rotated_pattern.push_back(std::string());
            rotated_pattern[col_index]+= pattern[row_index][col_index];
        }
    }
    while (*std::prev(rotated_pattern.end()) == "") {
        rotated_pattern.pop_back();
    }
    return rotated_pattern;
}


size_t get_horizontal_axis(const landscape &pattern) {
    size_t size {pattern.size()};

    size_t bottom_index {size - 1}; 
    for (size_t top_index {0}; top_index < bottom_index; ++top_index) {
        bottom_index = size - 1;

        if ((pattern[top_index] == pattern[bottom_index]) &&
            (bottom_index - top_index) % 2 == 1) {
            size_t search_ind_top {top_index};
            size_t search_ind_bot {bottom_index};
            while (pattern[search_ind_top] == pattern[search_ind_bot]) {
                    ++search_ind_top;
                    --search_ind_bot;
                if (search_ind_top >= search_ind_bot) {
                    return search_ind_bot + 1;
                }
            }
        }
    }

    int top_index {0};
    for (int bottom_index {static_cast<int>(size) - 1}; bottom_index > top_index; --bottom_index) {
        top_index = 0;
        if ((pattern[top_index] == pattern[bottom_index]) &&
            (bottom_index - top_index) % 2 == 1) {
            int search_ind_top {top_index};
            int search_ind_bot {bottom_index};
            while (pattern[search_ind_top] == pattern[search_ind_bot]) {
                    ++search_ind_top;
                    --search_ind_bot;
                if (search_ind_top >= search_ind_bot) {
                    return search_ind_bot + 1;
                }
            }
        }
    }

    return 0;
}
