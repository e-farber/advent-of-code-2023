#include <iostream>
#include <fstream>
#include <vector>

/**
 * December 24th, task 24-1
 * adventofcode.com/2023/day/24
 * 
 * Short summary:
 * Each line of text corresponds to the position and velocity of a single
 * hailstone (x_pos, y_pos, z_pos @ x_vel, y_vel, z_vel). Consider only the x
 * and y axes; ignore the z axis. Looking forward in time, how many of the
 * hailstones' paths will intersect within the 2-dimensional interval
 * [200'000'000'000'000, 400'000'000'000'000]² ⊂ ℝ²?
*/

// ##### Solution #####

struct vec2D {
    float x {0};
    float y {0};
    vec2D(float x, float y) : x {x}, y {y} {};
};

struct particle {
    vec2D position;
    vec2D velocity;
    particle(vec2D pos, vec2D vel) : position {pos}, velocity {vel} {}
};

bool is_digit(const char input);
float get_next_number(std::string::iterator &iterator);
vec2D calculate_intersection(const particle &left, const particle &right);

constexpr size_t lower_bound {200'000'000'000'000};
constexpr size_t upper_bound {400'000'000'000'000};


int main() {
    std::ifstream inputFile;
    std::string current_line;
    size_t result {0};

    inputFile.open("./input.txt");

    if (!inputFile.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    std::vector<particle> particles;
    while (std::getline(inputFile, current_line)) {
        auto iterator {current_line.begin()};
        vec2D position {get_next_number(iterator), get_next_number(iterator)};
        const float ignore_z {get_next_number(iterator)};
        vec2D velocity {get_next_number(iterator), get_next_number(iterator)};
        particles.emplace_back(position, velocity);
    }

    auto left_it {particles.begin()};
    while (left_it != std::prev(particles.end())) {
        auto right_it {std::next(left_it)};
        while (right_it != particles.end()) {
            vec2D intersection {calculate_intersection(*left_it, *right_it)};
            if (intersection.x >= lower_bound && intersection.x <= upper_bound &&
                intersection.y >= lower_bound && intersection.y <= upper_bound) {
                ++result;
            }
            ++right_it;
        }
        ++left_it;
    }

    std::cout << "result: " << result << std::endl;
    return 0;
}



bool is_digit(const char input) {
    int ascii_dec {static_cast<int>(input)};
    return (ascii_dec >= 48 && ascii_dec <= 57);
}


float get_next_number(std::string::iterator &iterator) {
    std::vector<size_t> digits;
    float result {0};
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


vec2D calculate_intersection(const particle &left, const particle &right) {
    float l_time {
        ((left.position.y - right.position.y) * right.velocity.x -
         (left.position.x - right.position.x) * right.velocity.y) /
        (left.velocity.x * right.velocity.y - left.velocity.y * right.velocity.x)
    };
    float r_time {
        (left.position.x - right.position.x + l_time * left.velocity.x) / right.velocity.x
    };

    if (l_time < 0 || r_time < 0) {
        return vec2D(0, 0);
    }

    vec2D result {
        left.position.x + left.velocity.x * l_time,
        left.position.y + left.velocity.y * l_time
    };
    return result;
}
