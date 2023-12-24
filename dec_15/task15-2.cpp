#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <algorithm>

/**
 * December 15th, task 15-2
 * adventofcode.com/2023/day/15
 * 
 * Short summary:
 * The initialization sequence is a comma-separated list. Each entry in the
 * sequence details lens information. It consists of a label followed by an
 * operation ('=' or '-'). Additionally, there are 256 boxes with space for
 * multiple lenses. The result of running the HASH algorithm described in part 1
 * on the label indicates the correct box for that lens.
 * If the operation is '-', remove the label from the relevant box while
 * preserving the order of all other lenses.
 * If the operation is '=', it is followed by a focal length. If the box doesn't
 * currently contain a lens with that label, add the lens to the box. If the box
 * does contain a lens with the same label, replace its focal length with the
 * new one. The focusing power of a single lens is the product of
 *  - One plus its box number,
 *  - The slot number of the lens within the box,
 *  - The focal length of the lens.
 * What is the focusing power of the resulting lens configuration?
*/

// ##### Solution #####

struct lens {
    std::string label {""};
    size_t focal_length {0};
    lens(std::string_view label, const size_t focal_length)
        : label {label}, focal_length {focal_length} {}
    bool operator== (const lens &other) { return label == other.label; }
};

using box = std::vector<lens>;

std::string get_next_label(
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
    
    std::vector<box> boxes(256);
    auto iterator {sequence.begin()};
    while (iterator != sequence.end()) {
        std::string current_label {get_next_label(iterator, sequence.end())};
        size_t box_num {hash(current_label)};
        char operation {*iterator};
        ++iterator;

        auto search_iterator {
            std::find(
                boxes[box_num].begin(),
                boxes[box_num].end(),
                lens(current_label, 0)
            )
        };

        if (operation == '=') {
            size_t focal_length = static_cast<size_t>(*iterator) - 48;
            ++iterator;

            if (search_iterator != boxes[box_num].end()) {
                search_iterator->focal_length = focal_length;
            }
            else {
                boxes[box_num].emplace_back(current_label, focal_length);
            }
        }

        else { // (operation == '-')
            if (search_iterator != boxes[box_num].end()) {
                boxes[box_num].erase(search_iterator);
            }
        }
    }

    for (size_t box_index {0}; box_index < 256; ++box_index) {
        for (size_t lens_index {0}; lens_index < boxes[box_index].size(); ++lens_index) {
            result += 
                (box_index + 1)  *  (lens_index + 1) * 
                boxes[box_index][lens_index].focal_length;
        }
    }

    std::cout << "result: " << result << std::endl;
    return 0;
}



std::string get_next_label(
    std::string::iterator &iterator,
    const std::string::iterator &end_pos
) {
    std::string result {""};
    if (*iterator == ',') {
        ++iterator;
    }

    while (iterator != end_pos) {
        if (*iterator == '=' || *iterator == '-') {
            break;
        }

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
