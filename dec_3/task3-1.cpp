#include <iostream>
#include <fstream>

/**
 * December 3rd, task 3-1
 * adventofcode.com/2023/day/3
 * 
 * Short summary:
 * Any number adjacent to a symbol, even diagonally, is a "part number"
 * and should be included in your sum. (Periods (.) do not count as a symbol.)
 * What is the sum of all of the part numbers in the engine schematic?
*/

// ##### Solution #####
// Note: input.txt was modified to feature an additional line of '.' at the end

bool is_symbol(const char input);
bool is_digit(const char input);
// checks if digit that iterator points to is next to a symbol
bool next_to_symbol(const std::string &top_line,
                    const std::string &middle_line,
                    const std::string &bottom_line,
                    std::string::const_iterator iterator
);
// assumes input iterator is pointing at first digit of a number
size_t get_number(const std::string::iterator &iterator);


int main() {
    std::ifstream inputFile;
    std::string top_line   (140, '.');
    std::string middle_line(140, '.');
    std::string bottom_line;
    size_t result {0};

    inputFile.open("./input.txt");

    if (!inputFile.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    // iterate over each line in input document
    while (std::getline(inputFile, bottom_line)) {

        // iterate over each character in a line 
        auto iterator {middle_line.begin()};
        while (iterator != middle_line.end()) {
            if (is_digit(*iterator)) {
                size_t number        {get_number(iterator)};
                size_t number_length {std::to_string(number).length()};

                if (next_to_symbol(top_line, middle_line, bottom_line,
                                   iterator) ||
                    next_to_symbol(top_line, middle_line, bottom_line,
                                   std::next(iterator, number_length - 1)) ) {
                    result += number;
                }
               
                ptrdiff_t distance_to_end {std::distance(iterator, middle_line.end())};
                iterator = std::next(iterator,
                                     std::min(number_length,
                                              static_cast<size_t>(distance_to_end) - 1));
            }
            
            ++iterator;
        }

        top_line = middle_line;
        middle_line = bottom_line;
    }

    std::cout << "result: " << result << std::endl;
    return 0;
}



bool is_symbol(const char input) {
    int ascii_dec {static_cast<int>(input)};
    return (
        (ascii_dec >= 33 && ascii_dec <= 45)   ||
        (ascii_dec == 47) || (ascii_dec >= 58)
    );
}


bool is_digit(const char input) {
    int ascii_dec {static_cast<int>(input)};
    return (ascii_dec >= 48 && ascii_dec <= 57);
}


size_t get_number(const std::string::iterator &iterator) {
    size_t first_digit  {static_cast<size_t>(*iterator) - 48};
    size_t second_digit {0};
    size_t third_digit  {0};
    
    if (is_digit(*std::next(iterator))) {
        first_digit *= 10;
        second_digit = *std::next(iterator) - 48;
        if (is_digit(*std::next(iterator, 2))) {
            first_digit  *= 10;
            second_digit *= 10;
            third_digit = *std::next(iterator, 2) - 48;
        }
    }

    return first_digit + second_digit + third_digit;
}

bool next_to_symbol(const std::string &top_line,
                    const std::string &middle_line,
                    const std::string &bottom_line,
                    std::string::const_iterator iterator)
{
    ptrdiff_t first_digit_pos  {std::distance(middle_line.begin(), iterator)};
    auto      top_search_it    {std::next(top_line   .begin(), first_digit_pos - 1)};
    auto      bottom_search_it {std::next(bottom_line.begin(), first_digit_pos - 1)};

    // search left 
    if(iterator != middle_line.begin()) {
        if (is_symbol(*std::prev(iterator))) {
            return true;
        }       
    }

    // search right
    if (std::next(iterator) != middle_line.end()) {
        if (is_symbol(*std::next(iterator))) {
        return true;
        }
    }
    
    // search top and bottom line
    for (size_t pos {0}; pos < 3; ++pos) {
        if (is_symbol(*top_search_it) || is_symbol(*bottom_search_it)) {
            return true;
        }
        if (std::next(top_search_it)    == top_line.end() ||
            std::next(bottom_search_it) == bottom_line.end()) {
            break;
        }
        ++top_search_it;
        ++bottom_search_it;
    }

    return false;
}
