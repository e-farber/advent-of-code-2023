#include <iostream>
#include <fstream>
#include <vector>
#include <list>

/**
 * December 11th, task 11-1
 * adventofcode.com/2023/day/11
 * 
 * Short summary:
 * The puzzle input is a single giant image. The image includes empty space (.)
 * and galaxies (#). Any rows or columns that contain no galaxies should all
 * actually be twice as big.
 * Expand the universe, then find the length of the shortest path between every
 * pair of galaxies. What is the sum of these lengths?
*/

// ##### Solution #####

using image = std::list<std::list<char>>;

struct coords {
    size_t row {0};
    size_t col {0};

    coords(size_t row, size_t col) : row {row}, col {col} {};
    bool operator==(const coords &other) const { return (row == other.row && col == other.col); }
};

void expand_space(image &grid);
std::vector<coords> get_coords(const image &grid);
// calculates distance with regard to taxicab norm
size_t min_distance(const coords &lhs, const coords &rhs);


int main() {
    std::ifstream inputFile;
    std::string current_line;
    image grid;
    size_t result {0};

    inputFile.open("./input.txt");

    if (!inputFile.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    while (std::getline(inputFile, current_line)) {
        grid.push_back(std::list<char>(current_line.begin(), current_line.end()));
    }

    expand_space(grid);

    std::vector<coords> galaxy_coords {get_coords(grid)};

    for (size_t left_index {0}; left_index < galaxy_coords.size() - 1; ++left_index) {
        for (size_t right_index {left_index + 1}; right_index < galaxy_coords.size(); ++right_index) {
            result += min_distance(galaxy_coords[left_index], galaxy_coords[right_index]);
        }
    }
    
    std::cout << "result: " << result << std::endl;
    return 0;
}



void expand_space(image &grid) {
    // gather expansion information
    auto row_iterator {grid.begin()};
    std::vector<bool> empty_cols(row_iterator->size(), true);
    while (row_iterator != grid.end()) {
        bool row_is_empty {true};
        auto col_iterator {row_iterator->begin()};
        while (col_iterator != row_iterator->end()) {
            if (*col_iterator == '#') {
                ptrdiff_t index {std::distance(row_iterator->begin(), col_iterator)};
                empty_cols[index] = false;
                row_is_empty = false;
            }
            ++col_iterator;
        }
        // vertical expansion
        if (row_is_empty) {
            grid.insert(row_iterator, 1, std::list<char>(row_iterator->size(), '_'));
        }
        ++row_iterator;
    }
    // horizontal expansion
    row_iterator = grid.begin();
    while (row_iterator != grid.end()) {
        for (int index {static_cast<int>(row_iterator->size()) - 1}; index >= 0; --index) {
            if (empty_cols[index]) {
                auto col_iterator {std::next(row_iterator->begin(), index)};
                row_iterator->insert(col_iterator, 1, '|');
            }
        }
        ++row_iterator;
    }
}


std::vector<coords> get_coords(const image &grid) {
    std::vector<coords> result;
    auto row_iterator {grid.begin()};
    while (row_iterator != grid.end()) {
        auto col_iterator {row_iterator->begin()};
        while (col_iterator != row_iterator->end()) {
            if (*col_iterator == '#') {
                size_t row {static_cast<size_t>(std::distance(grid.begin(),          row_iterator))};
                size_t col {static_cast<size_t>(std::distance(row_iterator->begin(), col_iterator))};
                result.emplace_back(row, col);
            }
            ++col_iterator;
        }
        ++row_iterator;
    }
    return result;
}


size_t min_distance(const coords &lhs, const coords &rhs) {
    int horizontal_dist {static_cast<int>(lhs.col) - static_cast<int>(rhs.col)};
    int vertical_dist   {static_cast<int>(lhs.row) - static_cast<int>(rhs.row)};
    return static_cast<size_t>(std::abs(horizontal_dist) + std::abs(vertical_dist));
}
