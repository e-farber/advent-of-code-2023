#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <set>

/**
 * December 16th, task 16-2
 * adventofcode.com/2023/day/16
 * 
 * Short summary:
 * The contraption is a flat, two-dimensional square grid containing empty space
 * (.), mirrors (/ and \), and splitters (| and -).
 * A beam may enter from any edge tile heading away from that edge.
 * Then, its behavior depends on what it encounters as it moves:
 *   empty space (.)   - it continues in the same direction.
 *   mirror   (/ or \) - it is reflected 90 degrees depending on the angle of
 *                       the mirror.
 *   splitter (| or -) - if it moves along the splitter, the beam passes through
 *                       the splitter as if through empty space.
 *                       if it hits the flat side, the beam is split into two
 *                       beams going in each direction the splitter points to.
 * A tile is energized if that tile has at least one beam pass through it,
 * reflect in it, or split in it.
 * Out of all possible entry points, find the beam that energizes as many tiles
 * as possible. How many tiles are energized in that configuration?
*/

// ##### Solution #####

using contraption = std::vector<std::string>;

struct coords {
    size_t row {0};
    size_t col {0};

    coords(size_t row, size_t col) : row {row}, col {col} {};
    bool operator==(const coords &other) const {return (row == other.row && col == other.col); }
    bool operator< (const coords &other) const {
        if (row != other.row) { return (row < other.row); } 
        else                  { return (col < other.col); }
    }
};

enum class compass {up, down, left, right};

class light {
public:
    inline static std::set<coords> creation_points;
    coords position;
    compass direction;

    light(const size_t row, const size_t col, const compass &direction)
        : position {row, col}, direction {direction} {
        creation_points.insert(position);
    }

    void move() {
        switch (direction) {
            case compass::up   : --position.row; break;
            case compass::down : ++position.row; break;
            case compass::left : --position.col; break;
            case compass::right: ++position.col; break;
            default:
                throw std::invalid_argument("Invalid direction");
        }
    } 
};

void shoot_beam(light &beam, contraption &tiles, const contraption &tiles_lookup);
size_t count_energized_tiles(const contraption &tiles);


int main() {
    std::ifstream inputFile;
    std::string current_line;
    size_t result {0};

    inputFile.open("./input.txt");

    if (!inputFile.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    contraption tiles;
    while (std::getline(inputFile, current_line)) {
        tiles.push_back(current_line);
    }

    for (size_t row_index {0}; row_index < tiles.size(); ++row_index) {
        if (row_index == 0) { // from up
            for (size_t col_index {0}; col_index < tiles.size(); ++col_index) {
                light beam {row_index, col_index, compass::down};
                auto tiles_copy {tiles};
                shoot_beam(beam, tiles_copy, tiles);
                result = std::max(result, count_energized_tiles(tiles_copy));
                light::creation_points.clear();
            }
        }
        if (row_index == tiles.size() - 1) { // from down
            for (size_t col_index {0}; col_index < tiles.size(); ++col_index) {
                light beam {row_index, col_index, compass::up};
                auto tiles_copy {tiles};
                shoot_beam(beam, tiles_copy, tiles);
                result = std::max(result, count_energized_tiles(tiles_copy));
                light::creation_points.clear();
            }
        }
        { // from left
            light beam {row_index, 0, compass::right};
            auto tiles_copy {tiles};
            shoot_beam(beam, tiles_copy, tiles);
            result = std::max(result, count_energized_tiles(tiles_copy));
            light::creation_points.clear();
        }
        { // from right
            light beam {row_index, tiles.size() - 1, compass::left};
            auto tiles_copy {tiles};
            shoot_beam(beam, tiles_copy, tiles);
            result = std::max(result, count_energized_tiles(tiles_copy));
            light::creation_points.clear();
        }
    }

    std::cout << "result: " << result << std::endl;
    return 0;
}



void shoot_beam(light &beam, contraption &tiles, const contraption &tiles_lookup) {
    bool obstructed {false};

    while (beam.position.row >= 0 && beam.position.row < tiles.size() &&
           beam.position.col >= 0 && beam.position.col < tiles.size()) {
        tiles[beam.position.row][beam.position.col] = '*';
        if (obstructed) {
            break;
        }
        switch (tiles_lookup[beam.position.row][beam.position.col]) {
            case '.':
                beam.move();
                break;
            case '|':
                if (beam.direction == compass::left ||
                    beam.direction == compass::right) {
                    if (light::creation_points.contains(beam.position)) {
                        obstructed = true;
                        break;
                    }
                    light extra_beam_up {
                        beam.position.row,
                        beam.position.col,
                        compass::up
                    };
                    light extra_beam_down {
                        beam.position.row,
                        beam.position.col,
                        compass::down
                    };
                    shoot_beam(extra_beam_up,   tiles, tiles_lookup);
                    shoot_beam(extra_beam_down, tiles, tiles_lookup);
                    obstructed = true;
                }
                else {
                    beam.move();
                }
                break;
            case '-':
                if (beam.direction == compass::up ||
                    beam.direction == compass::down) {
                    if (light::creation_points.contains(beam.position)) {
                        obstructed = true;
                        break;
                    }
                    light extra_beam_left {
                        beam.position.row,
                        beam.position.col,
                        compass::left
                    };
                    light extra_beam_right {
                        beam.position.row,
                        beam.position.col,
                        compass::right
                    };
                    shoot_beam(extra_beam_left,  tiles, tiles_lookup);
                    shoot_beam(extra_beam_right, tiles, tiles_lookup);
                    obstructed = true;
                }
                else {
                    beam.move();
                }
                break;
            case '/':
                if (beam.direction == compass::right) {
                    beam.direction = compass::up;
                    beam.move();
                }
                else if (beam.direction == compass::left) {
                    beam.direction = compass::down;
                    beam.move();
                }
                else if (beam.direction == compass::up) {
                    beam.direction = compass::right;
                    beam.move();
                }
                else if (beam.direction == compass::down) {
                    beam.direction = compass::left;
                    beam.move();
                }
                break;
            case '\\':
                if (beam.direction == compass::right) {
                    beam.direction = compass::down;
                    beam.move();
                }
                else if (beam.direction == compass::left) {
                    beam.direction = compass::up;
                    beam.move();
                }
                else if (beam.direction == compass::up) {
                    beam.direction = compass::left;
                    beam.move();
                }
                else if (beam.direction == compass::down) {
                    beam.direction = compass::right;
                    beam.move();
                }
                break;
            default:
                throw std::invalid_argument("Cannot move beam");
        }
    }
}


size_t count_energized_tiles(const contraption &tiles) {
    size_t result {0};
    for (auto &row : tiles) {
        for (auto &symbol : row) {
            if (symbol == '*') {
                ++result;
            }
        }
    }
    return result;
}
