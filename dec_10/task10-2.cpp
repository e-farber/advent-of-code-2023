#include <iostream>
#include <fstream>
#include <vector>

/**
 * December 10th, task 10-2
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
 * Find the single giant loop starting at S. How many tiles are on the inside
 * of the curve described by the loop?
*/

// ##### Solution #####

using square_map = std::vector<std::vector<char>>;
using offset_vec = std::vector<std::pair<int, int>>;

struct coords {
    size_t row {0};
    size_t col {0};

    coords(size_t row, size_t col) : row {row}, col {col} {};
    bool operator==(const coords &other) const {return (row == other.row && col == other.col); }
};

/**
 * Moves current position along grid in accordance to pipe directions.
 * @param grid        Contains arrangement of pipes.
 * @param current_pos Current position on grid.
 * @param old_pos     current_pos prior to being changed by this function.
 *                    Relevant for movement direction.
 * @param offset      Details what happens when current_pos == 'S', by giving
 *                    instructions which way to move.
*/
bool move_along(
    const square_map &grid,
    coords &current_pos,
    coords &old_pos,
    const std::pair<int, int> &offset
);
void mark_inside (
    const square_map &grid,
    square_map &grid_with_pipe,
    const coords current_pos,
    const coords old_pos
);
void fill_neighbors(const coords &position, square_map &grid);
void fill_inside(square_map &grid);
// Returns pair of (inside_count, outside_count). Assumes inside_count < outside_count.
std::pair<size_t, size_t> count_inside(square_map &grid);



int main() {
    std::ifstream inputFile;
    std::string current_line;
    square_map grid;

    inputFile.open("./input.txt");

    if (!inputFile.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    coords start_pos {0, 0};
    size_t row {0};
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
    
    // find pipe loop and mark it in grid_with_pipe
    auto grid_with_pipe {grid};
    std::pair<int, int> correct_offset {666, 666};
    offset_vec offset_values {{0, 1}, {-1, 0}, {0, -1}, {1, 0}};
    size_t max_steps {0};
    for (auto &offset_val : offset_values) {
        auto grid_copy {grid};
        coords old_pos {0, 0};
        coords current_pos {start_pos};
        coords offset_start {
            start_pos.row + offset_val.first,
            start_pos.col + offset_val.second
        };
        bool passed_S {false};

        size_t steps {0};
        while (move_along(grid, current_pos, old_pos, offset_val)) {
            ++steps;
            grid_copy[current_pos.row][current_pos.col] = '#';

            if (grid[current_pos.row][current_pos.col] == 'S') {
                passed_S = true;
            }
            if (passed_S && current_pos == offset_start) {
                if (max_steps < steps / 2) {
                    max_steps = steps / 2;
                    correct_offset = offset_val;
                    grid_with_pipe = grid_copy;
                }
                break;
            }
        }
    }

    // fill inside or outside, then count
    coords old_pos {0, 0};
    coords offset_start {
        start_pos.row + correct_offset.first,
        start_pos.col + correct_offset.second
    };
    coords current_pos {start_pos};
    bool passed_S {false};
    while (move_along(grid, current_pos, old_pos, correct_offset)) {
        if (grid[current_pos.row][current_pos.col] == 'S') {
            passed_S = true;
        }
        mark_inside(grid, grid_with_pipe, current_pos, old_pos);

        if (passed_S && current_pos == offset_start) {
            break;
        }
    }

    fill_inside(grid_with_pipe);
    size_t result {count_inside(grid_with_pipe).first};

    std::cout << "result: " << result << std::endl;
    return 0;
}



bool move_along(
    const square_map &grid,
    coords &current_pos,
    coords &old_pos,
    const std::pair<int, int> &offset
    ) {
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
            current_pos.row += offset.first;
            current_pos.col += offset.second;
            break;
        default:
            return false;
    }
    
    if (current_pos.row >= grid.size() || current_pos.col >= grid[0].size()) {
        current_pos = temp;
        return false;
    }
    
    old_pos = temp;
    return true;
}


void mark_inside (
    const square_map &grid,
    square_map &grid_with_pipe,
    const coords current_pos,
    const coords old_pos
) {
    /**
     * This is a complete mess. TODO: I'm sure I can save 100 lines if I insert
     * a frame of empty rows and colons around my grid, eliminating the need for
     * most if-statements preventing segmentation faults in mark_inside and
     * fill_neighbours. My solution 18-1 does this.
    */
    switch (grid[current_pos.row][current_pos.col]) {
        case '|':
            if (old_pos.row < current_pos.row) {
                if (current_pos.col == 0) { break; }
                if (grid_with_pipe[current_pos.row][current_pos.col - 1] != '#') {
                    grid_with_pipe[current_pos.row][current_pos.col - 1] = 'i';
                }
            }
            else {
                if (current_pos.col == grid[0].size() - 1) { break; }
                if (grid_with_pipe[current_pos.row][current_pos.col + 1] != '#') {
                    grid_with_pipe[current_pos.row][current_pos.col + 1] = 'i';
                }
            }
            break;
        case '-':
            if (old_pos.col < current_pos.col) {
                if (current_pos.row == grid.size() - 1) { break; }
                if (grid_with_pipe[current_pos.row + 1][current_pos.col] != '#') {
                    grid_with_pipe[current_pos.row + 1][current_pos.col] = 'i';
                }
            }
            else {
                if (current_pos.row == 0) { break; }
                if (grid_with_pipe[current_pos.row - 1][current_pos.col] != '#') {
                    grid_with_pipe[current_pos.row - 1][current_pos.col] = 'i';
                }
            }
            break;
        case 'L':
            if (old_pos.row < current_pos.row) {
                if (current_pos.col != 0) {
                if (grid_with_pipe[current_pos.row][current_pos.col - 1] != '#') {
                    grid_with_pipe[current_pos.row][current_pos.col - 1] = 'i';
                }
                }
                if (current_pos.row == grid.size() - 1) { break; }
                if (grid_with_pipe[current_pos.row + 1][current_pos.col] != '#') {
                    grid_with_pipe[current_pos.row + 1][current_pos.col] = 'i';
                }
                if (current_pos.col != 0) {
                if (grid_with_pipe[current_pos.row + 1][current_pos.col - 1] != '#') {
                    grid_with_pipe[current_pos.row + 1][current_pos.col - 1] = 'i';
                }
                }
            }
            break;
        case 'J':
            if (old_pos.col < current_pos.col) {
                if (current_pos.col != grid[0].size() - 1) {
                if (grid_with_pipe[current_pos.row][current_pos.col + 1] != '#') {
                    grid_with_pipe[current_pos.row][current_pos.col + 1] = 'i';
                }
                }
                if (current_pos.row == grid.size() - 1) { break; }
                if (grid_with_pipe[current_pos.row + 1][current_pos.col] != '#') {
                    grid_with_pipe[current_pos.row + 1][current_pos.col] = 'i';
                }
                if (current_pos.col != grid[0].size() - 1) {
                if (grid_with_pipe[current_pos.row + 1][current_pos.col + 1] != '#') {
                    grid_with_pipe[current_pos.row + 1][current_pos.col + 1] = 'i';
                }
                }
            }
            break;
        case '7':
            if (old_pos.row > current_pos.row) {
                if (current_pos.col != grid[0].size() - 1) {
                if (grid_with_pipe[current_pos.row][current_pos.col + 1] != '#') {
                    grid_with_pipe[current_pos.row][current_pos.col + 1] = 'i';
                }
                }
                if (current_pos.row == 0) { break; }
                if (grid_with_pipe[current_pos.row - 1][current_pos.col] != '#') {
                    grid_with_pipe[current_pos.row - 1][current_pos.col] = 'i';
                }
                if (current_pos.col != grid[0].size() - 1) {
                if (grid_with_pipe[current_pos.row - 1][current_pos.col + 1] != '#') {
                    grid_with_pipe[current_pos.row - 1][current_pos.col + 1] = 'i';
                }
                }
            }
            break;
        case 'F':
            if (old_pos.col > current_pos.col) {
                if (current_pos.col != 0) {
                if (grid_with_pipe[current_pos.row][current_pos.col - 1] != '#') {
                    grid_with_pipe[current_pos.row][current_pos.col - 1] = 'i';
                }
                }
                if (current_pos.row == 0) { break; }
                if (grid_with_pipe[current_pos.row - 1][current_pos.col] != '#') {
                    grid_with_pipe[current_pos.row - 1][current_pos.col] = 'i';
                }
                if (current_pos.col != 0) {
                if (grid_with_pipe[current_pos.row - 1][current_pos.col - 1] != '#') {
                    grid_with_pipe[current_pos.row - 1][current_pos.col - 1] = 'i';
                }
                }
            }
            break;
        default:
            break;
    }
}


void fill_neighbors(const coords &position, square_map &grid) {
    bool top_done    {false};
    bool bottom_done {false};
    bool left_done   {false};
    bool right_done  {false};

    if (position.row == 0) { top_done = true; }
    else                   { top_done =
        (grid[position.row - 1][position.col] == '#' ||
         grid[position.row - 1][position.col] == 'i');
    }
    if (position.row == grid.size() - 1) { bottom_done = true; }
    else                                 { bottom_done =
        (grid[position.row + 1][position.col] == '#' ||
         grid[position.row + 1][position.col] == 'i');
    }
    if (position.col == 0) { left_done = true; }
    else                   { left_done =
        (grid[position.row][position.col - 1] == '#' ||
         grid[position.row][position.col - 1] == 'i');
    }
    if (position.col == grid[0].size() - 1) { right_done = true; }
    else                                    { right_done =
        (grid[position.row][position.col + 1] == '#' ||
         grid[position.row][position.col + 1] == 'i');
    }

    if (top_done && bottom_done && left_done && right_done) {
        return;
    }
    
    if (!top_done) {
        if (grid[position.row - 1][position.col] != '#') {
            grid[position.row - 1][position.col] = 'i';
            fill_neighbors(coords(position.row - 1, position.col), grid);
        }
    }
    if (!bottom_done) {
        if (grid[position.row + 1][position.col] != '#') {
            grid[position.row + 1][position.col] = 'i';
            fill_neighbors(coords(position.row + 1, position.col), grid);
        }
    }
    if (!left_done) {
        if (grid[position.row][position.col - 1] != '#') {
            grid[position.row][position.col - 1] = 'i';
            fill_neighbors(coords(position.row, position.col - 1), grid);
        }
    }
    if (!right_done) {
        if (grid[position.row][position.col + 1] != '#') {
            grid[position.row][position.col + 1] = 'i';
            fill_neighbors(coords(position.row, position.col + 1), grid);
        }
    }
}


void fill_inside(square_map &grid) {
    for (size_t row_index {0}; row_index < grid.size(); ++row_index) {
        for (size_t col_index {0}; col_index < grid[0].size(); ++col_index) {
            if (grid[row_index][col_index] == 'i') {
                fill_neighbors(coords(row_index, col_index), grid);
            }
        }
    }
}


std::pair<size_t, size_t> count_inside(square_map &grid) {
    size_t i_count {0};
    size_t o_count {0};
    for (size_t row_index {0}; row_index < grid.size(); ++row_index) {
        for (size_t col_index {0}; col_index < grid[0].size(); ++col_index) {
            if (grid[row_index][col_index] == 'i') {
                ++i_count;
            }
            if (grid[row_index][col_index] != 'i' && grid[row_index][col_index] != '#') {
                ++o_count;
            }
        }
    }
    return std::make_pair(std::min(i_count, o_count), std::max(i_count, o_count));
}
