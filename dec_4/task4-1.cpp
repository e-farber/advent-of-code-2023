#include <iostream>
#include <fstream>
#include <set>
#include <algorithm>

/**
 * December 4th, task 4-1
 * adventofcode.com/2023/day/4
 * 
 * Short summary:
 * Each card (=line) has two lists of numbers separated by a vertical bar (|):
 * a list of winning numbers and then a list of numbers you have.
 * Figure out which of the numbers you have appear in the list of winning numbers.
 * The first match makes the card worth one point and each match after the first
 * doubles the point value of that card. How many points are they worth in total?
*/

// ##### Solution #####

bool is_digit(const char input);
// registers number and advances iterator to its end
size_t get_number(std::string::iterator &iterator);


int main() {
    std::ifstream inputFile;
    std::string current_line;
    size_t result {0};

    inputFile.open("./input.txt");

    if (!inputFile.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    // iterate over each line in input document
    while (std::getline(inputFile, current_line)) {
        bool iterating_over_winning_numbers {true};
        std::set<size_t> winning_numbers;
        std::set<size_t>      my_numbers;

        // iterate over each character in a line,
        // starting after ':'
        auto iterator {std::next(current_line.begin(), 10)};
        while (iterator != current_line.end()) {
            if (*iterator == '|') {
                std::advance(iterator, 2);
                iterating_over_winning_numbers = false;
            }

            if (*iterator != ' ') {
                size_t number = get_number(iterator);
                if (iterating_over_winning_numbers) {
                    winning_numbers.insert(number);
                }
                else {
                    my_numbers.insert(number);
                }
            }
            ++iterator;
        }

        std::vector<size_t> intersection;
        std::set_intersection(
            winning_numbers.begin(),
            winning_numbers.end(),
            my_numbers.begin(),
            my_numbers.end(),
            std::back_inserter(intersection)
        );

        size_t score {0};
        if (intersection.size() != 0) {
            score = (1ull << (intersection.size() - 1));
        }
        result += score;
    }

    std::cout << "result: " << result << std::endl;
    return 0;
}



bool is_digit(const char input) {
    int ascii_dec {static_cast<int>(input)};
    return (ascii_dec >= 48 && ascii_dec <= 57);
}


size_t get_number(std::string::iterator &iterator) {
    size_t first_digit  {static_cast<size_t>(*iterator) - 48};
    size_t second_digit {0};

    if (is_digit(*std::next(iterator))) {
        ++iterator;
        first_digit *= 10;
        second_digit = static_cast<size_t>(*iterator) - 48;
    }

    return first_digit + second_digit;
}
