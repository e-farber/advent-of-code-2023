#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <functional>

/**
 * December 19th, task 19-1
 * adventofcode.com/2023/day/19
 * 
 * Short summary:
 * A group of Elves is organizing parts. Each part is rated in each of four
 * categories: 'x', 'm', 'a', and 's'.
 * Then, each part is sent through a series of workflows that will ultimately
 * accept ('A') or reject ('R') the part. Each workflow has a name and contains
 * a list of rules; each rule specifies a condition and where to send the part
 * if the condition is true. The first rule that matches the part being
 * considered is applied immediately, and the part moves on to the destination
 * described by the rule. The last rule in each workflow applies by default if
 * reached. What do you get if you add together all of the rating numbers for
 * all of the parts that ultimately get accepted?
*/

// ##### Solution #####

using part = std::vector<std::pair<char, size_t>>;
// Predicate.
using condition = std::function<bool(part)>;
// Pair of predicate and destination.
using rule      = std::pair<condition, std::string>;

bool is_digit(const char input);
size_t get_next_number(std::string::iterator &iterator);
rule   get_next_rule(std::string::iterator &iterator);
size_t get_rating(const part &input);


int main() {
    std::ifstream inputFile;
    std::string current_line;
    size_t result {0};

    inputFile.open("./input.txt");

    if (!inputFile.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    std::map<std::string, std::vector<rule>> workflows;
    std::vector<part> parts;
    bool parsing_workflows {true};
    const std::string xmas {"xmas"};
    while (std::getline(inputFile, current_line)) {
        if (current_line == "") {
            parsing_workflows = false;
            continue;
        }

        auto iterator {current_line.begin()};
        if (parsing_workflows) {
            std::string name {""};
            std::vector<rule> rules {};
            while (*iterator != '{') {
                name += *iterator;
                ++iterator;
            }
            while (*iterator != '}') {
                rules.push_back(get_next_rule(iterator));
            }
            workflows[name] = rules;
        }
        else {
            part current_part {};
            for (auto letter : xmas) {
                current_part.push_back(std::make_pair(letter, get_next_number(iterator)));
            }
            parts.push_back(current_part);
        }
    }

    for (auto &test_part : parts) {
        std::string next {"in"};
        while (next != "A" && next != "R") {
            for (auto &[predicate, destination] : workflows[next]) {
                if (predicate(test_part)) {
                    next = destination;
                    if (next == "A") {
                        result += get_rating(test_part);
                    }
                    break;
                }
            }
        }
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


rule get_next_rule(std::string::iterator &iterator) {
    bool last_condition {false};
    size_t forward_step {1};
    while (true) {
        if (*std::next(iterator, forward_step) == ',') {
            break;
        }
        if (*std::next(iterator, forward_step) == '}') {
            last_condition = true;
            break;
        }
        ++forward_step;
    }
    ++iterator;

    std::string name {""};
    if (last_condition) {
        while (*iterator != '}') {
            name += *iterator;
            ++iterator;
        }
        return std::make_pair([](const part &test_part) { return true; }, name);
    }

    char letter {*iterator};
    ++iterator;
    char operation {*iterator};
    size_t number {get_next_number(iterator)};
    condition test_function = [letter, operation, number](const part &test_part) {
        for (auto &[name, value] : test_part) {
            if (name == letter) {
                if (operation == '<') { return value < number; }
                else                  { return value > number; }
            }
        }
    };

    ++iterator;
    while (*iterator != ',') {
        name += *iterator;
        ++iterator;
    }
    return std::make_pair(test_function, name);
}


size_t get_rating(const part &input) {
    size_t result {0};
    for (auto &[name, value] : input) {
        result += value;
    }
    return result;
}
