#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>
#include "enumerate.h"
#include "file_parse.h"
/*
 *Five of a kind, where all five cards have the same label: AAAAA
 *Four of a kind, where four cards have the same label and one card has a different label: AA8AA
 *Full house, where three cards have the same label, and the remaining two cards share a different label: 23332
 *Three of a kind, where three cards have the same label, and the remaining two cards are each different from any other card in the hand: TTT98
 *Two pair, where two cards share one label, two other cards share a second label, and the remaining card has a third label: 23432
 *One pair, where two cards share one label, and the other three cards have a different label from the pair and each other: A23A4
 *High card, where all cards' labels are distinct: 23456
 *Joker is a wild card
 */
enum class HandType {
    high_card,
    one_pair,
    two_pairs,
    triple,
    full_house,
    quadruple,
    quintuple,
};
HandType determineQuality(std::vector<int> cards) {
    std::ranges::sort(cards);
    auto uniques = std::vector<int>{};
    std::ranges::unique_copy(cards, std::back_inserter(uniques));

    switch (uniques.size()) {
    case 1:
        return HandType::quintuple;
    case 5:
        return HandType::high_card;
    case 4:
        return HandType::one_pair;
    default:;
    }

    auto unique_counts = std::vector<int>{};
    for (const int u : uniques) {
        int count = 0;
        for (const int c : cards) {
            count = u == c ? count + 1 : count;
        }
        unique_counts.push_back(count);
    }
    const auto max_count = *std::ranges::max_element(unique_counts);

    switch (uniques.size()) {
    case 2:
        // full house or quadruple and single
        return max_count == 4 ? HandType::quadruple : HandType::full_house;
    case 3:
        // two pair and random or triple and two random
        return max_count == 2 ? HandType::two_pairs : HandType::triple;
    default:
        throw std::exception("should not be possible");
    }
}

std::vector<int> resolveJoker(std::vector<int>&& cards) {
    auto index = size_t{0};
    for (const auto& [i,v] : enumerate(cards)) {
        if (v == 1) {
            index = i;
            break;
        }
    }
    if (index==0) {
        return cards;
    }

    auto current_cards = cards;
    while (cards[index]<14) {
        cards[index] +=1;

        if (index!=cards.size()-1) {
            // can be another J present
            auto next_joker = index+1;
            while (next_joker<cards.size()) {
                if (cards[next_joker]==1) {
                    cards = resolveJoker(std::move(cards));
                    break;       
                }
                next_joker++;
            }
        }
        if (determineQuality(cards)>determineQuality(current_cards)) {
            std::swap_ranges(cards.begin(), cards.end(),current_cards.begin());
            cards[index] = current_cards[index];
        }

    }
    return current_cards;
}

class PokerHand {
    std::vector<int> cards;
    HandType quality;

public:
    PokerHand(const std::string& line) {
        cards.reserve(line.size());
        for (const char c : line) {
            switch (c) {
            case 'K':
                cards.push_back(13);
                break;
            case 'Q':
                cards.push_back(12);
                break;
            case 'J':
                cards.push_back(1);
                break;
            case 'T':
                cards.push_back(10);
                break;
            case 'A':
                cards.push_back(14);
                break;
            default:
                cards.push_back(std::stoll(&c));
            }
        }
        auto fn = [](const int lhs) { return lhs == 1; };
        if (std::ranges::any_of(cards, fn))
            cards = resolveJoker(std::move(cards));
        quality = determineQuality(cards);
    }

    friend std::strong_ordering operator<=>(const PokerHand& lhs, const PokerHand& rhs) {
        if (lhs.quality != rhs.quality) {
            return lhs.quality <=> rhs.quality;
        }
        // find high card
        auto lhs_iter = lhs.cards.begin();
        auto rhs_iter = rhs.cards.begin();
        while (lhs_iter != lhs.cards.end()) {
            if (*lhs_iter > *rhs_iter) {
                return std::strong_ordering::greater;
            }
            if (*lhs_iter < *rhs_iter) {
                return std::strong_ordering::less;
            }
            ++lhs_iter;
            ++rhs_iter;
        }
        return std::strong_ordering::equal;
    }
};

struct FullBid {
    int bid;
    PokerHand hand;

    FullBid(const std::string& line)
        : bid(std::stoi(line.substr(6)))
        , hand(line.substr(0, 5)) {
    }
};

int main() {
    // notes: each stage is always in order
    auto file_handle = FileHandle("input.txt");
    std::function<FullBid(std::string)> fn = [](const std::string& l) { return FullBid(l); };
    std::vector<FullBid> data = parse(file_handle, fn);

    auto fn_sort = [](const FullBid& lhs, const FullBid& rhs) { return lhs.hand < rhs.hand; };
    std::ranges::sort(data, fn_sort);
    int output = 0;
    for (auto [i, d] : enumerate(data)) {
        output += (i + 1) * d.bid;
    }
    // 5905
    std::cout << "final=" << output << " \n";
}
