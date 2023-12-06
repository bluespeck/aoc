#include "../utility.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <ctype.h>
#include <numeric>
#include <string>
#include <vector>

struct Card
{
	int32_t id = 0;
	std::vector < int32_t > winning_numbers;
	std::vector < int32_t > your_numbers;
};

using input_t = std::vector<Card>;
[[nodiscard]] input_t read()
{
	input_t input;
	std::string line;
	while (std::getline(std::cin, line))
	{
		if (line.size() == 0)
		{
			continue;
		}
		auto card_line_tokens = split_string(line, { ": " });
		assert(card_line_tokens.size() == 2);
		Card card;
		auto card_id_tokens = split_string(card_line_tokens[0], " ");
		card.id = std::stoi(card_id_tokens.back());
		auto numbers_strings = split_string(card_line_tokens[1], " | ");
		assert(numbers_strings.size() == 2);
		{
			auto number_strs = split_string(numbers_strings[0], " ");
			for (auto& num: number_strs)
			{
				if (num.size())
				{
					card.winning_numbers.push_back(std::stoi(num));
				}
			}
		}
		{
			auto number_strs = split_string(numbers_strings[1], " ");
			for (auto& num: number_strs)
			{
				if (num.size())
				{
					card.your_numbers.push_back(std::stoi(num));
				}
			}
		}
		input.emplace_back(card);
	}
	return input;
}

void solve_1(const input_t& input)
{
	int64_t sum = 0;
	for (const auto& card: input)
	{
		int64_t count = std::count_if(card.your_numbers.cbegin(), card.your_numbers.cend(), [&nums = card.winning_numbers](int32_t num) {
		              auto it = std::find(nums.cbegin(), nums.cend(), num);
					  return it != nums.cend();
		});
		if (count > 0)
		{
			sum += 1ll << (count - 1);
		}

	}
	outf("{}\n", sum);
}

void solve_2(const input_t& input)
{
	std::vector < int32_t > card_counts(input.size(), 1);

	for (int32_t i = input.size() - 1; i >= 0; --i)
	{
		auto& card = input[i];
		
		int32_t cards_won = std::count_if(card.your_numbers.cbegin(), card.your_numbers.cend(), [&nums = card.winning_numbers](int32_t num) {
										auto it = std::find(nums.cbegin(), nums.cend(), num);
										return it != nums.cend();
		});
		for (int32_t j = i + 1; j <= i + cards_won; j++)
		{
			assert(j < card_counts.size());
			assert(card_counts[j] != -1);
			//outf("won card {} which is worth {} cards\n", j, card_counts[j]);
			card_counts[i] += card_counts[j];
		}
		//outf("totalling {}.\n", card_counts[i]);
		
	}
	
	outf("{}\n", std::accumulate(card_counts.cbegin(), card_counts.cend(), 0));
}

int main()
{
	input_t input = read();
	solve_1(input);
	solve_2(input);

	return 0;
}