#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

/**
 * December 6th, task 6-1
 * adventofcode.com/2023/day/6
 * 
 * Short summary:
 * Your puzzle input lists the time allowed for each toy boat race and also
 * the best distance ever recorded in that race. To guarantee you win
 * the grand prize, you need to make sure you go farther in each race
 * than the current record holder. 
 * Your toy boat has a starting speed of zero millimeters per millisecond.
 * For each whole millisecond you spend at the beginning of the race holding
 * down the button, the boat's speed increases by one millimeter per millisecond.
 * Determine the number of ways you can beat the record in each race.
 * What do you get if you multiply these numbers together? 
*/

// ##### Solution #####

struct paper_sheet {
    std::vector<size_t> time;
    std::vector<size_t> distance;
};

bool is_digit(const char input);
// registers next number in a line and advances iterator one position beyond its end
size_t get_next_number(std::string::iterator &iterator);


int main() {
    std::ifstream inputFile;
    std::string current_line;
    paper_sheet race_data;
    size_t result {1};

    inputFile.open("./input.txt");

    if (!inputFile.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    bool first_line {true};
    while (std::getline(inputFile, current_line)) {
        auto iterator {current_line.begin()};
        while (iterator != current_line.end()) {
            if (first_line) {
                race_data.time.push_back(get_next_number(iterator));
            }
            else {
                race_data.distance.push_back(get_next_number(iterator));
            }
        }
        first_line = false;
    }
    
    /**
     * @param n total_time
     * @param d distance
     * @details:
     * Quadratic function describing travelled distance in relation to time is given by
     * f(t) = -t² + nt. Note that its maximum lies at n²/4.
     * We're looking for the set of t, so that f(t) > d. That is the interval
     * [(n - sqrt(n² - 4d)) / 2,   (n + sqrt(n² - 4d)) / 2] = I.
     * Since we only need amount of integer numbers in this set, we're looking for
     * | I ∩ ℕ | = upper_bound - lower_bound, 
     * while being careful with float to integer conversions.
    */
    for (size_t race_index {0}; race_index < race_data.time.size(); ++race_index) {
        float total_time_n {static_cast<float>(race_data.time    [race_index])};
        float distance_d   {static_cast<float>(race_data.distance[race_index])};

        float lower_bound {
            (total_time_n - std::sqrt(total_time_n * total_time_n - 4 * distance_d)) / 2
        };
        float upper_bound {
            (total_time_n + std::sqrt(total_time_n * total_time_n - 4 * distance_d)) / 2
        };
        float cardinality {std::ceil(upper_bound - 1) - std::floor(lower_bound)};
        result *= static_cast<size_t>(cardinality);
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
