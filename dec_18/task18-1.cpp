#include <iostream>
#include <fstream>
#include <vector>
#include <deque>

/**
 * December 18th, task 18-1
 * adventofcode.com/2023/day/18
 * 
 * Short summary:
 * The dig plan contains instructions for a digger that starts in a 1 meter cube
 * hole in the ground. Each line in the dig plan then dictates in which
 * direction to dig the specified amount of meters. After completing all
 * instructions, the digger will arrive at the starting position. The interiour
 * is then also dug out.
 * How many cubic meters of lava could the dug out space hold?
*/

// ##### Solution #####

using dig_ground = std::deque<std::deque<char>>;

struct coords {
    size_t row {0};
    size_t col {0};

    coords(size_t row, size_t col) : row {row}, col {col} {};
};

bool is_digit(const char input);
size_t get_next_number(std::string::iterator &iterator);
void adjust(
    dig_ground &map,
    coords &current_position,
    const char direction,
    const size_t amount
);
void dig(
    dig_ground &map,
    coords &current_position,
    const char direction,
    const size_t amount
);
void fill_neighbors(const coords &position, dig_ground &map);
void fill_in(dig_ground &map);
size_t count_volume(const dig_ground &map);


int main() {
    std::ifstream inputFile;
    std::string current_line;

    inputFile.open("./input.txt");

    if (!inputFile.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    dig_ground map(20, std::deque<char>(20, '.'));
    coords current_pos {0, 0};
    while (std::getline(inputFile, current_line)) {
        auto iterator {current_line.begin()};
        char direction {*iterator};
        size_t amount {get_next_number(iterator)};
        dig(map, current_pos, direction, amount);
    }

    fill_in(map);
    size_t result {count_volume(map)};

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


void adjust(
    dig_ground &map,
    coords &current_position,
    const char direction,
    const size_t amount
) {
    switch (direction) {
        case 'D':
            if (current_position.row + amount < map.size()) {
                return;
            }
            else {
                for (size_t row_amount {0}; row_amount < amount; ++row_amount) {
                    map.push_back (std::deque<char>(map[0].size(), '.'));
                }
            }
            break;
        case 'U':
            if (current_position.row - amount < map.size()) {
                return;
            }
            else {
                for (size_t row_amount {0}; row_amount < amount; ++row_amount) {
                    map.push_front(std::deque<char>(map[0].size(), '.'));
                }
                current_position.row += amount;
            }
            break;
        case 'L':
            if (current_position.col - amount < map[0].size()) {
                return;
            }
            else {
                for (auto &row : map) {
                    for (size_t col_amount {0}; col_amount < amount; ++col_amount) {
                        row.push_front('.');
                    }
                }
                current_position.col += amount;
            }
            break;
        case 'R':
            if (current_position.col + amount < map[0].size()) {
                return;
            }
            else {
                for (auto &row : map) {
                    for (size_t col_amount {0}; col_amount < amount; ++col_amount) {
                        row.push_back('.');
                    }
                }
            }
            break;
        default:
            throw std::invalid_argument("Unknown direction");
    }
}


void dig(
    dig_ground &map,
    coords &current_position,
    const char direction,
    const size_t amount
) {
    adjust(map, current_position, direction, amount);

    for (size_t step {0}; step < amount; ++step) {
        switch (direction) {
            case 'D':
                map.at(current_position.row + step).at(current_position.col) = '#';
                continue;
            case 'U':
                map.at(current_position.row - step).at(current_position.col) = '#';
                continue;
            case 'L':
                map.at(current_position.row).at(current_position.col - step) = '#';
                continue;
            case 'R':
                map.at(current_position.row).at(current_position.col + step) = '#';
                continue;
        }
    }
    switch (direction) {
        case 'D': { current_position.row += amount; break; }
        case 'U': { current_position.row -= amount; break; }
        case 'L': { current_position.col -= amount; break; }
        case 'R': { current_position.col += amount; break; }
    }
}


void fill_neighbors(const coords &position, dig_ground &map) {
    coords up    {position.row - 1, position.col};
    coords down  {position.row + 1, position.col};
    coords left  {position.row, position.col - 1};
    coords right {position.row, position.col + 1};
    char &top_tile    {map[up.row][up.col]};
    char &bottom_tile {map[down.row][down.col]};
    char &left_tile   {map[left.row][left.col]};
    char &right_tile  {map[right.row][right.col]};

    if ((top_tile    == '#' || top_tile    == 'o') &&
        (bottom_tile == '#' || bottom_tile == 'o') &&
        (left_tile   == '#' || left_tile   == 'o') &&
        (right_tile  == '#' || right_tile  == 'o')) {
            return;
        }
    
    if (top_tile    == '.') {
        top_tile = 'o';
        fill_neighbors(up, map);
    }
    if (bottom_tile == '.') {
        bottom_tile = 'o';
        fill_neighbors(down, map);
    }
    if (left_tile   == '.') {
        left_tile = 'o';
        fill_neighbors(left, map);
    }
    if (right_tile  == '.') {
        right_tile = 'o';
        fill_neighbors(right, map);
    }
}


void fill_in(dig_ground &map) {
    coords current_pos {0, 0};
    char current_dir {'R'};
    while (map[current_pos.row][current_pos.col] != '#') {
        ++current_pos.col;
    }
    ++current_pos.col;
    while (map[current_pos.row][current_pos.col] == '#') {
        ++current_pos.row;
    }

    fill_neighbors(current_pos, map);
}


size_t count_volume(const dig_ground &map) {
    size_t result {0};
    for (auto &row : map) {
        for (auto &symbol : row) {
            if (symbol == '#' || symbol == 'o') {
                ++result;
            }
        }
    }
    return result;
}
