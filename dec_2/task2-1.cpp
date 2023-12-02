#include <iostream>
#include <fstream>

/**
 * December 2nd, task 2-1
 * adventofcode.com/2023/day/2
 * 
 * Short summary:
 * Determine which games would have been possible if the bag had been
 * loaded with only 12 red cubes, 13 green cubes, and 14 blue cubes.
 * What is the sum of the IDs of those games?
 * 
*/

// ##### Solution #####

bool points_at_red  (const std::string::iterator &iterator);
bool points_at_green(const std::string::iterator &iterator);
bool points_at_blue (const std::string::iterator &iterator);

size_t get_amount(const std::string::iterator &iterator);
bool   is_valid(const size_t red, const size_t green, const size_t blue);


int main() {
    size_t result {0};
    std::ifstream inputFile;
    std::string fileContent;
    size_t line_number {0}; // = game number
    
    inputFile.open("./input.txt");

    if (!inputFile.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    // iterate over each line in input document
    while (std::getline(inputFile, fileContent)) {
        ++line_number;

        bool valid_game {true};
        size_t red_amount   {0};
        size_t green_amount {0};
        size_t blue_amount  {0};

        // iterate over each character in one line
        auto iterator {fileContent.begin()};
        while (iterator != fileContent.end()) {
            if (points_at_red(iterator)) {
                red_amount += get_amount(iterator);
            }
            if (points_at_green(iterator)) {
                green_amount += get_amount(iterator);
            }
            if (points_at_blue(iterator)) {
                blue_amount += get_amount(iterator);
            }
            
            if (*iterator == ';' || iterator == std::prev(fileContent.end())) {
                if (!is_valid(red_amount, green_amount, blue_amount)) {
                    valid_game = false;
                    break;
                }
                red_amount   = 0;
                green_amount = 0;
                blue_amount  = 0;
            }

            ++iterator;
        }
        
        if (valid_game) {
            result += line_number;
        }
    }

    std::cout << "result: " << result << std::endl;
    return 0;
}



bool points_at_red(const std::string::iterator &iterator) {
    return (*iterator == 'r' && *std::next(iterator, 2) == 'd');
}


bool points_at_green(const std::string::iterator &iterator) {
    return (*iterator == 'g');

}
bool points_at_blue(const std::string::iterator &iterator) {
    return (*iterator == 'b');
}


size_t get_amount(const std::string::iterator &iterator) {
    size_t result {0};
    int first_digit  {static_cast<int>(*std::prev(iterator, 3)) - 48};
    int second_digit {static_cast<int>(*std::prev(iterator, 2)) - 48};

    if (first_digit >= 0 && first_digit <= 9) { // character might be ':' or ' '
        result += 10* static_cast<size_t>(first_digit);
    }

    return result += static_cast<size_t>(second_digit);
}


bool is_valid(const size_t red, const size_t green, const size_t blue) {
    return (
        red   <= 12 &&
        green <= 13 &&
        blue  <= 14
    );
}
