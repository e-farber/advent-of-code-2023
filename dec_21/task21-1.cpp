#include <iostream>
#include <fstream>
#include <vector>

/**
 * December 21th, task 21-1
 * adventofcode.com/2023/day/21
 * 
 * Short summary:
 * Your map shows the starting position (S), garden plots (.), and rocks (#).
 * Starting from S, one step takes one one tile north, south, east, or west, but
 * only onto tiles that are garden plots.
 * How many garden plots can be reached in exactly 64 steps?
*/

// ##### Solution #####

using garden = std::vector<std::string>;

struct coords {
    size_t row {0};
    size_t col {0};

    coords(size_t row, size_t col) : row {row}, col {col} {};
};


void mark_options(garden &plot, bool first_step);
size_t count_spaces(const garden &plot);

static coords start_pos {666, 666};


int main() {
    std::ifstream inputFile;
    std::string current_line;

    inputFile.open("./input.txt");

    if (!inputFile.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    size_t row_index {0};
    garden plot;
    plot.push_back("");
    while (std::getline(inputFile, current_line)) {
        plot.push_back(std::string(1, '*').append(current_line).append("*"));
        auto iterator {current_line.begin()};
        for (size_t col_index {0}; col_index < current_line.size() + 2; ++col_index) {
            if (plot[row_index][col_index] == 'S') {
                start_pos = coords(row_index, col_index);
            }
        }
        ++row_index;
    }
    plot[0] = std::string(plot[1].size(), '*');
    plot.push_back(std::string(plot[1].size(), '*'));

    bool first_step {true};
    for (size_t step {0}; step < 64; ++step) {
        mark_options(plot, first_step);
        first_step = false;
    }

    size_t result {count_spaces(plot)};
 
    std::cout << "result: " << result << std::endl;
    return 0;
}



void mark_options(garden &plot, bool first_step) {
    if (first_step) {
        plot[start_pos.row][start_pos.col] = '.';
        if (plot[start_pos.row - 1][start_pos.col] == '.') {
            plot[start_pos.row - 1][start_pos.col] = 'O';
        }
        if (plot[start_pos.row + 1][start_pos.col] == '.') {
            plot[start_pos.row + 1][start_pos.col] = 'O';
        }
        if (plot[start_pos.row][start_pos.col + 1] == '.') {
            plot[start_pos.row][start_pos.col + 1] = 'O';
        }
        if (plot[start_pos.row][start_pos.col - 1] == '.') {
            plot[start_pos.row][start_pos.col - 1] = 'O';
        }
        return;
    }

    auto plot_copy {plot};

    for (size_t row_index {0}; row_index < plot.size(); ++row_index) {
        for (size_t col_index {0}; col_index < plot[0].size(); ++col_index) {
            if (plot_copy[row_index][col_index] == 'O') {
                plot[row_index][col_index] = '.';
                if (plot_copy[row_index - 1][col_index] == '.') {
                    plot[row_index - 1][col_index] = 'O';
                }
                if (plot_copy[row_index + 1][col_index] == '.') {
                    plot[row_index + 1][col_index] = 'O';
                }
                if (plot_copy[row_index][col_index + 1] == '.') {
                    plot[row_index][col_index + 1] = 'O';
                }
                if (plot_copy[row_index][col_index - 1] == '.') {
                    plot[row_index][col_index - 1] = 'O';
                }
            }
        }
    }
}


size_t count_spaces(const garden &plot) {
    size_t result {0};
    for (auto &row : plot) {
        for (auto &symbol : row) {
            if (symbol == 'O') {
                ++result;
            }
        }
    }
    return result;
}
