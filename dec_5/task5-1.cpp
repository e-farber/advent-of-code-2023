#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

/**
 * December 5th, task 5-1
 * adventofcode.com/2023/day/5
 * 
 * Short summary:
 * The almanac starts by listing which seeds need to be planted.
 * The rest of the almanac contains a list of maps which describe
 * how to convert numbers from a source category into numbers in a destination category.
 * Each line within a map contains three numbers:
 * the destination range start, the source range start, and the range length.
 * Any source numbers that aren't mapped correspond to the same destination number.
 * What is the lowest location number that corresponds to any of the initial seed numbers?
*/

// ##### Solution #####


bool is_digit(const char input);
// registers next number in a line and advances iterator one position beyond its end
size_t get_next_number(std::string::iterator &iterator);

struct seed_data {
    std::vector<size_t> seed_storage;
    std::vector<size_t> was_moved;
};

int main() {
    std::ifstream inputFile;
    std::string current_line;
    seed_data seeds;

    inputFile.open("./input.txt");

    if (!inputFile.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    // iterate over each line in input document
    while (std::getline(inputFile, current_line)) {

        // handle seeds
        if (current_line.starts_with("seeds")) {
            auto iterator {std::next(current_line.begin(), 7)};
            while (iterator != current_line.end()) {
                seeds.seed_storage.push_back(get_next_number(iterator));
                seeds.was_moved   .push_back(false);
            }
            continue;
        }

        // handle map headlines
        if (*std::prev(current_line.end()) == ':') {
            for (size_t index {0}; index < seeds.seed_storage.size(); ++index) {
                seeds.was_moved[index] = false;
            }
            continue;
        }

        // destination start, source start, range length 
        int range_data[3] {-1, -1, -1};
        size_t index {0};
        // iterate over each character in a line
        auto iterator {current_line.begin()};
        while (iterator != current_line.end()) {
            range_data[index] = get_next_number(iterator);
            ++index;
        }

        for (size_t index {0}; index < seeds.seed_storage.size(); ++index) {
            if (!seeds.was_moved[index]
                && seeds.seed_storage[index] >= range_data[1] 
                && seeds.seed_storage[index]  < range_data[1] + range_data[2]) {
                seeds.seed_storage[index] = 
                    range_data[0] + seeds.seed_storage[index] - range_data[1];
                seeds.was_moved[index] = true;
            }
        }
    }

    auto result {std::min_element(seeds.seed_storage.begin(), seeds.seed_storage.end())};
    std::cout << "result: " << *result << std::endl;
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
