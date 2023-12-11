#include <iostream>
#include <fstream>
#include <vector>

/**
 * December 9th, task 9-1
 * adventofcode.com/2023/day/9
 * 
 * Short summary:
 * Each line in your report contains the history of a single value. To best
 * protect the oasis, your environmental report should include a prediction
 * of the next value in each history. To do this, start by making a new
 * sequence from the difference at each step of your history. If that sequence
 * is not all zeroes, repeat this process, using the sequence you just
 * generated as the input sequence. Once all of the values in your latest
 * sequence are zeroes, you can extrapolate what the next value of the original
 * history should be.
 * What is the sum of these extrapolated values?
*/

// ##### Solution #####

bool is_digit(const char input);
// registers next number in a line and advances iterator one position beyond its end
int get_next_number(std::string::iterator &iterator);
int extrapolate_value(std::vector<int> sequence);


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
        std::vector<int> current_sequence;
        auto iterator {current_line.begin()};
        while (iterator != current_line.end()) {
            current_sequence.push_back(get_next_number(iterator));
        }
        result += extrapolate_value(current_sequence);
    }

    std::cout << "result: " << result << std::endl;
    return 0;
}



bool is_digit(const char input) {
    const int ascii_dec {static_cast<int>(input)};
    return (ascii_dec >= 48 && ascii_dec <= 57);
}


int get_next_number(std::string::iterator &iterator) {
    std::vector<size_t> digits;
    int result {0};
    bool is_negative {false};

    while (!is_digit(*iterator)) {
        if (*iterator == '-') {
            is_negative = true;
        }
        ++iterator;
    }

    while (is_digit(*iterator)) {
        for (auto &digit : digits) {
            digit *= 10;
        }
        digits.push_back(static_cast<int>(*iterator) - 48);
        ++iterator;
    }

    for (auto &digit : digits) {
        result += digit;
    }

    if (is_negative) {
        result *= -1;
    }

    return result;
}


int extrapolate_value(std::vector<int> sequence) {
    bool all_equal {true};
    std::vector<int> next_sequence;

    for (size_t index {0}; index < sequence.size() - 1; ++index) {
        int difference {sequence[index + 1] - sequence[index]};
        if (difference != 0) {
            all_equal = false;
        }
        next_sequence.push_back(difference);
    }

    if (all_equal) {
        return sequence[0];
    }

    sequence.push_back(extrapolate_value(next_sequence) + sequence[sequence.size() - 1]);
    return *std::prev(sequence.end());
}
