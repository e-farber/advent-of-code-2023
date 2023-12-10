#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

/**
 * December 7th, task 7-2
 * adventofcode.com/2023/day/7
 * 
 * Short summary:
 * For this task, you get a list of hands, and your goal is to order them based
 * on the strength of each hand. A hand consists of five cards labeled one of
 * A, K, Q, T, 9, ..., 2, J. The relative strength of each card follows this order,
 * where A is the highest and 2 is the lowest. J cards are jokers - wildcards
 * that can act like whatever card would make the hand the strongest type possible.
 * To balance this, J cards are the weakest individual cards, weaker even than 2.
 * Every hand is exactly one type. From strongest to weakest, they are:
 * Five of a kind, Four of a kind, Full house, Three of a kind, Two pair, One pair, High card.
 * If two hands have the same type, a second ordering rule takes effect:
 * Compare the first differing card in each hand and decide based on its strength.
 * You are given a list of hands and their corresponding bid. Each hand wins an
 * amount equal to its bid multiplied by its rank, where the weakest hand gets
 * rank 1, the second-weakest hand gets rank 2, and so on up to the strongest hand.
 * Find the rank of every hand in your set. What are the total winnings?
*/

// ##### Solution #####

/**
 * categorizing hand types as cycle types:
 * numbers denote amounts of distinct card multiples
 * hand type         ranking     cycle type
 * five of a kind       1        5
 * four of a kind       2        4 1
 * full house           3        3 2
 * three of a kind      4        3 1 1
 * two pair             5        2 2 1
 * one pair             6        2 1 1 1
 * high card            7        1 1 1 1 1
*/
using cycle_type = std::vector<size_t>;

struct hand {
    std::string cards;
    cycle_type  hand_type;
    size_t      bid;

    hand(std::string_view cards, size_t bid)
        : cards {cards}, bid {bid} {
        determine_type();
    }

    // sets this hand's type
    void determine_type();
};

struct handComparator {
    bool operator()(const hand &lhs, const hand &rhs);
};

bool is_digit(const char input);
// registers next number in a line and advances iterator one position beyond its end
size_t get_next_number(std::string::iterator &iterator);
size_t card_to_value(const char);


int main() {
    std::ifstream inputFile;
    std::string current_line;
    size_t result {0};
    std::vector<hand> all_hands;
    all_hands.reserve(1000);

    inputFile.open("./input.txt");

    if (!inputFile.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    // iterate over each line in input document
    while (std::getline(inputFile, current_line)) {
        std::string current_cards {""};
        current_cards.append(current_line.substr(0, 5));
        
        auto iterator {std::next(current_line.begin(), 6)};
        size_t current_bid {get_next_number(iterator)};

        all_hands.emplace_back(current_cards, current_bid);
    }

    std::sort(all_hands.begin(), all_hands.end(), handComparator());

    size_t rank {1000};
    for (auto &hand : all_hands) {
        result += rank * hand.bid;
        --rank;
    }

    std::cout << "result: " << result << std::endl;
    return 0;
}



bool is_digit(const char input) {
    const int ascii_dec {static_cast<int>(input)};
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


size_t card_to_value(const char card) {
    if (is_digit(card)) {
        return static_cast<size_t>(card) - 48;
    }
    switch (card) {
        case 'T':
            return 10;
        case 'J':
            return 1; 
        case 'Q':
            return 12;
        case 'K':
            return 13;
        case 'A':
            return 14;
        default:
            throw std::invalid_argument("Could not convert card");
    }
}


bool handComparator::operator()(const hand &lhs, const hand &rhs) {
    size_t cycle_length {std::min(lhs.hand_type.size(), rhs.hand_type.size())};
    for (size_t index {0}; index < cycle_length; ++index) {
        if (lhs.hand_type[index] != rhs.hand_type[index]) {
            return lhs.hand_type[index] > rhs.hand_type[index];
        }
    }

    for (size_t index {0}; index < 5; ++index) {
        if (lhs.cards[index] != rhs.cards[index]) {
            return card_to_value(lhs.cards[index]) > card_to_value(rhs.cards[index]);
        }
    }

    throw std::invalid_argument("Could not rank hands");
}


void hand::determine_type() {
    std::string cards_copy {cards};
    size_t equal_cards  {1};
    size_t joker_amount {0};
    size_t total_cards  {0};

    for (size_t index {0}; index < 5; ++index) {
        if (cards[index] == 'J') {
            cards_copy.erase(std::next(cards_copy.begin(), index - joker_amount));
            ++joker_amount;
        }
    }

    if (joker_amount >= 4) {
        hand_type.push_back(5);
        return;
    }

    std::sort(cards_copy.begin(), cards_copy.end());

    cycle_type combination_case;
    for (size_t index {0}; index < cards_copy.size() - 1; ++index) {
        while (cards_copy[index] == cards_copy[index + 1]) {
            ++index;
            ++equal_cards;
        }
        combination_case.push_back(equal_cards);
        total_cards += equal_cards;
        equal_cards = 1;
    }

    if (total_cards != 5 - joker_amount) {
        combination_case.push_back(1);
    }

    std::sort(combination_case.begin(), combination_case.end(), std::greater<size_t>());
    
    size_t index {0};
    for (size_t point_increase {0}; point_increase < joker_amount; ++point_increase) {
        if (combination_case[index] <= 5) {
            ++combination_case[index];
        } 
        else {
            ++index;
            --point_increase;
        }
    }

    hand_type = std::move(combination_case);
}
