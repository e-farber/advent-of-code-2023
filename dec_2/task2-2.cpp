#include <iostream>
#include <fstream>

/**
 * December 2nd, task 2-2
 * adventofcode.com/2023/day/2
 * 
 * Short summary:
 * The _power_ of a set of cubes is equal to the numbers of
 * red, green, and blue cubes multiplied together.
 * For each game, find the minimum set of cubes that must have been present.
 * What is the sum of the power of these sets?
*/

// ##### Solution #####

bool points_at_red  (const std::string::iterator &iterator);
bool points_at_green(const std::string::iterator &iterator);
bool points_at_blue (const std::string::iterator &iterator);

size_t get_amount(const std::string::iterator &iterator);


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
        std::cout << fileContent << std::endl;

        size_t red_amount   {0};
        size_t green_amount {0};
        size_t blue_amount  {0};

        size_t red_min      {0};
        size_t green_min    {0};
        size_t blue_min     {0};

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
                red_min   = std::max(red_min, red_amount);
                green_min = std::max(green_min, green_amount);
                blue_min  = std::max(blue_min, blue_amount);

                red_amount   = 0;
                green_amount = 0;
                blue_amount  = 0;
            }
            
            ++iterator;
        }
        
        size_t power {red_min * green_min * blue_min};
        result += power;
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