#include "../utility.h"

#include <algorithm>
#include <array>
#include <ranges>
#include <string>
#include <vector>
#include <unordered_map>

using Card_Map = std::unordered_map<char, int32_t>;
Card_Map card_map = {
	{ '2', 0 },
	{ '3', 1 },
	{ '4', 2 },
	{ '5', 3 },
	{ '6', 4 },
	{ '7', 5 },
	{ '8', 6 },
	{ '9', 7 },
	{ 'T', 8 },
	{ 'J', 9 },
	{ 'Q', 10 },
	{ 'K', 11 },
	{ 'A', 12 },
};

Card_Map card_map_2 = {
	{ 'J', 0 },
	{ '2', 1 },
	{ '3', 2 },
	{ '4', 3 },
	{ '5', 4 },
	{ '6', 5 },
	{ '7', 6 },
	{ '8', 7 },
	{ '9', 8 },
	{ 'T', 9 },
	{ 'Q', 10 },
	{ 'K', 11 },
	{ 'A', 12 },
};

struct Hand {
	std::string hand;
	int64_t bid;
	std::array<size_t, 5> card_counts;
	size_t j_count;
	int64_t type;
	int64_t type_2;
};

using Input = std::vector<Hand>;

bool is_five_of_a_kind(const Hand& hand) {
	return hand.card_counts[0] == 5;
}

bool is_four_of_a_kind(const Hand& hand) {
	return hand.card_counts[0] == 4 || hand.card_counts[1] == 4;
}

bool is_full_house(const Hand& hand) {
	return std::ranges::count(hand.card_counts, 2) == 2 && std::ranges::count(hand.card_counts, 3) == 3;
}

bool is_three_of_a_kind(const Hand& hand) {
	return std::ranges::count(hand.card_counts, 2) == 0 && std::ranges::count(hand.card_counts, 3) == 3;
}

bool is_two_pair(const Hand& hand) {
	return std::ranges::count(hand.card_counts, 2) == 4;
}

bool is_one_pair(const Hand& hand) {
	return std::ranges::count(hand.card_counts, 2) == 2 && std::ranges::count(hand.card_counts, 3) == 0;
}

bool is_high_card(const Hand& hand) {
	return std::ranges::count(hand.card_counts, 1) == 5 - std::ranges::count(hand.card_counts, 0);
}

bool hand_less_than(const Hand& h1, const Hand& h2) {
	if (h1.type < h2.type) {
		return true;
	}
	else if (h1.type == h2.type) {
		for (size_t i = 0; i < h1.hand.size(); i++) {
			if (card_map[h1.hand[i]] < card_map[h2.hand[i]]) {
				return true;
			}
			else if (card_map[h1.hand[i]] > card_map[h2.hand[i]]) {
				return false;
			}
		}
	}
	return false;
}

Hand hand_without_j(const Hand& hand) {
	Hand noj = hand;
	for (size_t i = 0; i < hand.hand.size(); i++) {
		if (hand.hand[i] == 'J') {
			noj.card_counts[i] = 0;
		}
	}
	return noj;
}

bool is_five_of_a_kind_2(const Hand& hand)
{
	Hand noj_hand = hand_without_j(hand);
	switch (hand.j_count) {
		case 5: return true;
		case 4: return true;
		case 3: return is_one_pair(noj_hand);
		case 2: return is_three_of_a_kind(noj_hand);
		case 1: return is_four_of_a_kind(noj_hand);
		case 0: return is_five_of_a_kind(noj_hand);
	}

	return false;
}

bool is_four_of_a_kind_2(const Hand& hand) {
	Hand noj_hand = hand_without_j(hand);
	switch (hand.j_count) {
		case 5: return false;
		case 4: return false;
		case 3: return is_high_card(noj_hand);
		case 2: return is_one_pair(noj_hand);
		case 1: return is_three_of_a_kind(noj_hand);
		case 0: return is_four_of_a_kind(noj_hand);
	}
	return false;
}

bool is_full_house_2(const Hand& hand) {
	Hand noj_hand = hand_without_j(hand);
	switch (hand.j_count) {
		case 5: return false;
		case 4: return false;
		case 3: return false;
		case 2: return is_one_pair(noj_hand);
		case 1: return is_two_pair(noj_hand);
		case 0: return is_full_house(noj_hand);
	}
	return false;
}

bool is_three_of_a_kind_2(const Hand& hand) {
	Hand noj_hand = hand_without_j(hand);
	switch (hand.j_count) {
		case 5: return false;
		case 4: return false;
		case 3: return false;
		case 2: return is_high_card(noj_hand);
		case 1: return is_one_pair(noj_hand);
		case 0: return is_three_of_a_kind(noj_hand);
	}
	return false;
}

bool is_two_pair_2(const Hand& hand)
{
	Hand noj_hand = hand_without_j(hand);
	switch (hand.j_count) {
		case 5: return false;
		case 4: return false;
		case 3: return false;
		case 2: return false; // we can make three of a kind
		case 1: return false; // if we have a pair without j, we can make three of a kind;
			// if no pair, we can't make two pairs
		case 0: return is_two_pair(noj_hand);
	}
	return false;
}

bool is_one_pair_2(const Hand& hand) {
	Hand noj_hand = hand_without_j(hand);
	switch (hand.j_count) {
		case 5: return false;
		case 4: return false;
		case 3: return false;
		case 2: return false;
		case 1: return is_high_card(noj_hand);
		case 0: return is_one_pair(noj_hand);
	}
	return false;
}

bool hand_less_than_2(const Hand& h1, const Hand& h2) {
	if (h1.type_2 < h2.type_2) {
		return true;
	}
	else if (h1.type_2 == h2.type_2) {
		for (size_t i = 0; i < h1.hand.size(); i++)	{
			if (card_map_2[h1.hand[i]] < card_map_2[h2.hand[i]]) {
				return true;
			}
			else if (card_map_2[h1.hand[i]] > card_map_2[h2.hand[i]]) {
				return false;
			}
		}
	}
	return false;
}

void update_card_counts(Hand& hand) {
	size_t index = 0;
	for (char c : hand.hand) {
		hand.card_counts[index++] = std::ranges::count(hand.hand, c);
	}
	hand.j_count = std::ranges::count(hand.hand, 'J');
}

void update_hand_type_1(Hand& hand) {
	if (is_five_of_a_kind(hand)) hand.type = 7;
	else if (is_four_of_a_kind(hand)) hand.type = 6;
	else if (is_full_house(hand)) hand.type = 5;
	else if (is_three_of_a_kind(hand)) hand.type = 4;
	else if (is_two_pair(hand)) hand.type = 3;
	else if (is_one_pair(hand)) hand.type = 2;
	else hand.type = 1;
}

void update_hand_type_2(Hand& hand) {
	if (is_five_of_a_kind_2(hand)) hand.type_2 = 7;
	else if (is_four_of_a_kind_2(hand)) hand.type_2 = 6;
	else if (is_full_house_2(hand)) hand.type_2 = 5;
	else if (is_three_of_a_kind_2(hand)) hand.type_2 = 4;
	else if (is_two_pair_2(hand)) hand.type_2 = 3;
	else if (is_one_pair_2(hand)) hand.type_2 = 2;
	else hand.type_2 = 1;
}

[[nodiscard]] Input read() {
	Input input;
	std::string line;
	while (std::getline(std::cin, line)) {
		auto tokens = split_string(line, " ");
		
		Hand hand;
		hand.hand = tokens[0];
		hand.bid = std::stoll(tokens[1]);
		update_card_counts(hand);
		update_hand_type_1(hand);
		update_hand_type_2(hand);

		input.push_back(hand);
	}
	
	return input;
}

void solve_1(const Input& input) {
	auto input1 = input;
	std::ranges::sort(input1, &hand_less_than);
	int64_t winnings = 0;
	for (size_t i = 0; i < input1.size(); i++) {
		winnings += input1[i].bid * (i + 1);
	}
	outf("{}\n", winnings);
}

void solve_2(const Input& input) {
	auto input2 = input;
	std::ranges::sort(input2, &hand_less_than_2);
	int64_t winnings = 0;
	for (size_t i = 0; i < input2.size(); i++) {
		winnings += input2[i].bid * (i + 1);
	}
	outf("{}\n", winnings);
}

int main() {
	Input input = read();
	solve_1(input);
	solve_2(input);

	return 0;
}