#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

/**
 * December 22th, task 22-1
 * adventofcode.com/2023/day/22
 * 
 * Short summary:
 * The snapshot shows bricks while they are still falling. Each line lists the
 * position of one brick. Each brick is a single straight line. Its position is
 * given as two x,y,z coordinates - one for each end of the brick - separated by
 * a tilde (~). The ground is at z=0 and perfectly flat.
 * Start by letting all bricks fall down as far as possible. Bricks never rotate
 * and a falling brick will come to rest upon the first other brick it meets.
 * A brick can be safely disintegrated if, after removing it, no other bricks
 * would fall further directly downward. After all bricks come to rest, for each
 * brick, decide if it is safe to disintegrate only that brick.
 * How many bricks could be safely chosen as the one to get disintegrated?
*/

// ##### Solution #####

struct coords {
    size_t x {0};
    size_t y {0};
    size_t z {0};

    coords(size_t x, size_t y, size_t z) : x {x}, y {y}, z {z} {};
    bool operator<(const coords &other) const {
        if      (z != other.z) { return (z < other.z); }
        else if (y != other.y) { return (y < other.y); }
        else                   { return (x < other.x); }
    }
};

struct brick {
    coords start;
    coords end;
    brick(const coords &start, const coords &end) : start {start}, end {end} {}
    bool operator<(const brick &other) const { return (start < other.start); }
};

bool is_digit(const char input);
size_t get_next_number(std::string::iterator &iterator);
// Assumes lhs's height is <= rhs's height.
bool second_on_first(const brick &lhs, const brick &rhs);
size_t count_bricks_below(
    const std::vector<brick> &bricks,
    const std::vector<brick>::iterator &brick_iterator

);


int main() {
    std::ifstream inputFile;
    std::string current_line;
    size_t result {0};

    inputFile.open("./input.txt");

    if (!inputFile.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    std::vector<brick> bricks;
    while (std::getline(inputFile, current_line)) {
        auto iterator {current_line.begin()};
        coords start {
            get_next_number(iterator),
            get_next_number(iterator),
            get_next_number(iterator)
        };
        coords end {
            get_next_number(iterator),
            get_next_number(iterator),
            get_next_number(iterator)
        };
        bricks.emplace_back(start, end);
    }

    std::sort(bricks.begin(), bricks.end());

    auto r_it {std::next(bricks.begin())};
    while (r_it != bricks.end()) {
        bool obstructed {false};
        while (!obstructed && r_it->start.z != 0) {
            // compare with all bricks below
            auto l_it {bricks.begin()};
            while (l_it != r_it) {
                if (second_on_first(*l_it, *r_it)) {
                    obstructed = true;
                    break;
                }
                ++l_it;
            }
            // lef fall if way down is free
            if (!obstructed) {
                --r_it->start.z;
                --r_it->end.z;
            }
        }
        ++r_it;
    }

    std::sort(bricks.begin(), bricks.end());

    auto it_to_erase {bricks.begin()};
    while (it_to_erase != bricks.end()) {
        bool erasable {true};

        auto it_to_compare {std::next(it_to_erase)};
        while (it_to_compare->start.z <= it_to_erase->end.z + 1) {
            if (second_on_first(*it_to_erase, *it_to_compare)) {
                if (count_bricks_below(bricks, it_to_compare) == 1) {
                    erasable = false;
                    break;
                }
            }
            if (it_to_compare == bricks.end()) {
                break;
            }
            ++it_to_compare;
        }
        if (erasable) {
            ++result;
        }
        ++it_to_erase;
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


bool second_on_first(const brick &lhs, const brick &rhs) {
    if (lhs.end.z + 1 != rhs.start.z) {
        return false;
    }
    bool crossing_x {false};
    bool crossing_y {false};

    for (size_t x_pos_r {rhs.start.x}; x_pos_r <= rhs.end.x; ++x_pos_r) {
        for (size_t x_pos_l {lhs.start.x}; x_pos_l <= lhs.end.x; ++x_pos_l) {
            if (x_pos_r == x_pos_l) {
                crossing_x = true;
                break;
            }
        }
        if (crossing_x) {
            break;
        }
    }

    for (size_t y_pos_r {rhs.start.y}; y_pos_r <= rhs.end.y; ++y_pos_r) {
        for (size_t y_pos_l {lhs.start.y}; y_pos_l <= lhs.end.y; ++y_pos_l) {
            if (y_pos_r == y_pos_l) {
                crossing_y = true;
                break;
            }
        }
        if (crossing_y) {
            break;
        }
    }
    
    return (crossing_x && crossing_y);
}


size_t count_bricks_below(
    const std::vector<brick> &bricks,
    const std::vector<brick>::iterator &brick_iterator
) {
    size_t result {0};
    auto brick_below {std::prev(brick_iterator)};
    while (true) {
        if (second_on_first(*brick_below, *brick_iterator)) {
            ++result;
        }
        if (brick_below == bricks.begin()) {
            break;
        }
        --brick_below;
    }
    return result;
}
