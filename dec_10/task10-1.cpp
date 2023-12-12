#include <iostream>
#include <fstream>
#include <vector>

/**
 * December 10th, task 10-1
 * adventofcode.com/2023/day/10
 * 
 * Short summary:
 * You're standing on a field densely packed with pipes arranged in a 2-dim
 * grid of tiles:
 * | is a vertical pipe connecting north and south.
 * - is a horizontal pipe connecting east and west.
 * L is a 90-degree bend connecting north and east.
 * J is a 90-degree bend connecting north and west.
 * 7 is a 90-degree bend connecting south and west.
 * F is a 90-degree bend connecting south and east.
 * . is ground; there is no pipe in this tile.
 * S is the starting position; there is a pipe on this tile, but your sketch
 *   doesn't show what shape the pipe has.
 * The pipe you're looking for one large, continuous loop. Every pipe in the
 * main loop connects to its two neighbors.
 * Find the single giant loop starting at S. How many steps along the loop does
 * it take to get from the starting position to the point farthest from the
 * starting position? 
*/

// ##### Solution #####

using square_map = std::vector<std::vector<char>>;
using offset_vec = std::vector<std::pair<int, int>>;

struct coords {
    size_t row {0};
    size_t col {0};

    coords() = default;
    coords(size_t row, size_t col) : row {row}, col {col} {};
    bool operator==(const coords &other) const {return (row == other.row && col == other.col); }
};

bool move_along(const square_map &grid, coords &current_pos, coords &old_pos);


int main() {
    std::ifstream inputFile;
    std::string current_line;
    size_t result {0};
    square_map grid;

    inputFile.open("./input.txt");

    if (!inputFile.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    coords start_pos;
    size_t row {0};
    // iterate over each line in input document
    while (std::getline(inputFile, current_line)) {
        grid.push_back(std::vector<char>());
        for (size_t col {0}; col < current_line.length(); ++col) {
            grid[row].push_back(current_line[col]);
            if (current_line[col] == 'S') {
                start_pos = coords(row, col);
            }
        }
        ++row;
    }
    
    offset_vec offset_values {{0, 1}, {-1, 0}, {0, -1}, {1, 0}};
    for (auto &offset_val : offset_values) {
        if (result != 0) {
            break;
        }

        bool passed_S {false};
        coords old_pos {
            start_pos.row,
            start_pos.col
        };
        coords offset_start {
            start_pos.row + offset_val.first,
            start_pos.col + offset_val.second
        };
        coords current_pos {offset_start};

        size_t steps {1};
        while (move_along(grid, current_pos, old_pos)) {
            ++steps;
            if (grid[current_pos.row][current_pos.col] == 'S') {
                passed_S = true;
            }

            if (passed_S && current_pos == offset_start) {
                result = steps / 2;
                break;
            }
        }
    }

    std::cout << "result: " << result << std::endl;
    return 0;
}



bool move_along(const square_map &grid, coords &current_pos, coords &old_pos) {
    coords temp = current_pos;
    switch (grid[current_pos.row][current_pos.col]) {
        case '|':
            if (old_pos.row < current_pos.row)
                 { ++current_pos.row; }
            else { --current_pos.row; }
            break;
        case '-':
            if (old_pos.col < current_pos.col)
                 { ++current_pos.col; }
            else { --current_pos.col; }
            break;
        case 'L':
            if (old_pos.col > current_pos.col)
                 { --current_pos.row; }
            else { ++current_pos.col; }
            break;
        case 'J':
            if (old_pos.col < current_pos.col)
                 { --current_pos.row; }
            else { --current_pos.col; }
            break;
        case '7':
            if (old_pos.col < current_pos.col)
                 { ++current_pos.row; }
            else { --current_pos.col; }
            break;
        case 'F':
            if (old_pos.col > current_pos.col)
                 { ++current_pos.row; }
            else { ++current_pos.col; }
            break;
        case 'S':
            ++current_pos.row;
            break;
        default:
            return false;
    }
    
    if (current_pos.col >= grid.size() || current_pos.row >= grid.size()) {
        current_pos = temp;
        return false;
    }
    
    old_pos = temp;
    return true;
}
