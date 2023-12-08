#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>

/**
 * December 3rd, task 3-2
 * adventofcode.com/2023/day/3
 * 
 * Short summary:
 * A gear is any * symbol that is adjacent to exactly two part numbers.
 * Its gear ratio is the result of multiplying those two numbers together.
 * What is the sum of all of the gear ratios in your engine schematic?
*/

// ##### Solution #####

struct gearData {
    bool is_gear;
    size_t number1;
    size_t number2;

    gearData(bool is_gear, size_t number1, size_t number2)
        : is_gear {is_gear}, number1 {number1}, number2 {number2} {
    }
};

bool is_digit(const char input);
gearData is_gear(const std::string &top_line,
                 const std::string &middle_line,
                 const std::string &bottom_line,
                 std::string::const_iterator iterator
);
// assumes input iterator is pointing at first digit of a number
size_t get_number(const std::string::iterator &iterator
);

static size_t result {0};

int main() {
    std::ifstream inputFile;
    std::string top_line(140, '.');
    std::string middle_line(140, '.');
    std::string bottom_line;

    inputFile.open("./input.txt");

    if (!inputFile.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    // iterate over each line in input document
    while (std::getline(inputFile, bottom_line)) {

        // iterate over each character in a line 
        auto iterator {middle_line.begin()};
        while (iterator != middle_line.end()) {
            auto gear_data {is_gear(top_line, middle_line, bottom_line, iterator)};
            if (gear_data.is_gear) {
                size_t gear_ratio {gear_data.number1 * gear_data.number2};
                if (gear_ratio == 0) {
                }
                result += gear_ratio;
            }

            ++iterator;
        }

    top_line = middle_line;
    middle_line = bottom_line;
    }

    std::cout << "result: " << result << std::endl;
    return 0;
}



bool is_digit(const char input) {
    int ascii_dec {static_cast<int>(input)};
    return (ascii_dec >= 48 && ascii_dec <= 57);
}


size_t get_number(const std::string::const_iterator &iterator)
{
    size_t first_digit  {static_cast<size_t>(*iterator) - 48};
    size_t second_digit {0};
    size_t third_digit  {0};

    if (is_digit(*std::next(iterator))) {
        //       ^ sometimes leads to undefined behaviour but without consequence
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

gearData is_gear(const std::string &top_line,
                 const std::string &middle_line,
                 const std::string &bottom_line,
                 std::string::const_iterator iterator)
{
    size_t number1 {0};
    size_t number2 {0};
    if (*iterator != '*') {
        return gearData(false, number1, number2);
    }
    
    size_t counter {0};
    ptrdiff_t asterisk_pos {std::distance(middle_line.begin(), iterator)};
    auto    top_search_it  {std::next(top_line   .begin(), asterisk_pos - 1)};
    auto bottom_search_it  {std::next(bottom_line.begin(), asterisk_pos - 1)};

    // search left 
    if(iterator != middle_line.begin()) {
        if (is_digit(*std::prev(iterator))) {
            ++counter;
            auto search_iterator {std::prev(iterator)};
            while (is_digit(*std::prev(search_iterator))) {
                --search_iterator;
            }
            if (number1 == 0) {
                number1 = get_number(search_iterator);
            }
            else {
                number2 = get_number(search_iterator);
            }
            
        }       
    }
    
    // search right
    if (std::next(iterator) != middle_line.end()) {
        if (is_digit(*std::next(iterator))) {
            ++counter;
            if (number1 == 0) {
                number1 = get_number(std::next(iterator));
            }
            else {
                number2 = get_number(std::next(iterator));
            }
        }
    }
    
    // search top
    for (ptrdiff_t pos {0}; pos < 3; ++pos) {
        if (is_digit(*top_search_it)) {
            ++counter;
            auto search_iterator {(top_search_it)};
            ptrdiff_t distance_travelled {0};
            while (is_digit(*std::prev(search_iterator))) {
                --search_iterator;
            }
            
            if (number1 == 0) {
                number1 = get_number(search_iterator);
            }
            else {
                number2 = get_number(search_iterator);
            }

            while (*top_search_it != '.' && pos < 3) {
                pos += 1;
                ++top_search_it;
            }
        }

        if (std::next(top_search_it) != top_line.end()) {
            ++top_search_it;
        }
    }
      
    // search bottom
    for (ptrdiff_t pos {0}; pos < 3; ++pos) {
        if (is_digit(*bottom_search_it)) {
            ++counter;
            auto search_iterator {bottom_search_it};
            while (is_digit(*std::prev(search_iterator))) {
                --search_iterator;
            }
           
            if (number1 == 0) {
                number1 = get_number(search_iterator);
            }
            else {
                number2 = get_number(search_iterator);
            }
 
            while (*bottom_search_it != '.' && pos < 3) {
                ++pos;
                ++bottom_search_it;
            }
        }

        if (std::next(bottom_search_it) != bottom_line.end()) {
            ++bottom_search_it;
        }
    }

    if (counter == 2) {
        return gearData(true, number1, number2);
    }

    return gearData(false, number1, number2);
}
