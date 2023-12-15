#include <iostream>
#include <fstream>
#include <vector>


/**
 * December 11th, task 11-1
 * adventofcode.com/2023/day/11
 * 
 * Short summary:
 * The puzzle input is a single giant image. The image includes empty space (.)
 * and galaxies (#). Any rows or columns that contain no galaxies should all
 * actually be one million times as big.
 * Expand the universe, then find the length of the shortest path between every
 * pair of galaxies. What is the sum of these lengths?
*/

// ##### Solution #####

using image = std::vector<std::vector<char>>;

struct coords {
    size_t row {0};
    size_t col {0};

    coords(size_t row, size_t col) : row {row}, col {col} {};
    bool operator==(const coords &other) const { return (row == other.row && col == other.col); }
};

struct space_data {
    std::vector<size_t> empty_rows;
    std::vector<size_t> empty_cols;

    space_data(std::vector<size_t> rows, std::vector<size_t> cols) : empty_rows {rows}, empty_cols {cols} {}
};

std::vector<coords> get_coords(const image &grid);
space_data get_space_data(const image &grid);
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
        grid.push_back(std::vector<char>(current_line.begin(), current_line.end()));
    }

    std::vector<coords> galaxy_coords {get_coords(grid)};

    space_data empty_spots {get_space_data(grid)};

    for (size_t left_index {0}; left_index < galaxy_coords.size() - 1; ++left_index) {
        for (size_t right_index {left_index + 1}; right_index < galaxy_coords.size(); ++right_index) {
            result += min_distance(galaxy_coords[left_index], galaxy_coords[right_index]);
            for (auto &empty_row : empty_spots.empty_rows) {
                if (galaxy_coords[ left_index].row < empty_row &&
                    galaxy_coords[right_index].row > empty_row) {
                        result += 999'999;
                }
            }
            for (auto &empty_col : empty_spots.empty_cols) {
                if ((galaxy_coords[ left_index].col < empty_col &&
                     galaxy_coords[right_index].col > empty_col) ||
                    (galaxy_coords[ left_index].col > empty_col &&
                     galaxy_coords[right_index].col < empty_col)) {
                        result += 999'999;
                }
            }
        }
    }
    
    std::cout << "result: " << result << std::endl;
    return 0;
}



std::vector<coords> get_coords(const image &grid) {
    std::vector<coords> result;

    for (size_t row_index {0}; row_index < grid.size(); ++row_index) {
        for (size_t col_index {0}; col_index < grid.size(); ++col_index) {
            if (grid[row_index][col_index] == '#') {
                result.emplace_back(row_index, col_index);
            }
        }
    }
    
    return result;
}


space_data get_space_data(const image &grid) {
    std::vector<size_t> relevant_rows;
    std::vector<size_t> relevant_cols;

    std::vector<bool> empty_cols(grid.size(), true);
    for (size_t row_index {0}; row_index < grid.size(); ++row_index) {
        bool row_is_empty {true};
        for (size_t col_index {0}; col_index < grid.size(); ++col_index) {
            if (grid[row_index][col_index] == '#') {
                row_is_empty = false;
                empty_cols[col_index] = false;
            }
        }
        
        if (row_is_empty) {
            relevant_rows.push_back(row_index);
        }
    }

    for (size_t index {0}; index < empty_cols.size(); ++index) {
        if (empty_cols[index]) {
            relevant_cols.push_back(index);
        }
    }
    
   return space_data(relevant_rows, relevant_cols);
}


size_t min_distance(const coords &lhs, const coords &rhs) {
    int horizontal_dist {static_cast<int>(lhs.col) - static_cast<int>(rhs.col)};
    int vertical_dist   {static_cast<int>(lhs.row) - static_cast<int>(rhs.row)};
    return static_cast<size_t>(std::abs(horizontal_dist) + std::abs(vertical_dist));
}
