#include "../utility.h"

#include <array>
#include <string>
#include <vector>
#include <print>
#include <numeric>

struct entry_t
{
	int64_t ax, ay;
	int64_t bx, by;
	int64_t px, py;
};
using input_t = std::vector<entry_t>;
[[nodiscard]] input_t read()
{
	input_t input;
	std::string line;
	entry_t entry;
	while (std::getline(std::cin, line))
	{
		if (line.size() == 0) continue;
		auto parts = split_string(line, ": ");
		if (parts[0] == "Prize")
		{
			auto axes = split_string(parts[1], ", ");
			auto t1 = split_string(axes[0], "=");
			auto t2 = split_string(axes[1], "=");
			entry.px = std::stoll(t1[1]);
			entry.py = std::stoll(t2[1]);
			input.push_back(entry);
		}
		else
		{
			auto axes = split_string(parts[1], ", ");
			auto t1 = split_string(axes[0], "+");
			auto t2 = split_string(axes[1], "+");
			if (parts[0].back() == 'A')
			{
				entry.ax = std::stoll(t1[1]);
				entry.ay = std::stoll(t2[1]);
			}
			else
			{
				entry.bx = std::stoll(t1[1]);
				entry.by = std::stoll(t2[1]);
			}
		}
	}

	return input;
}


void solve_1(const input_t& input)
{
	int64_t sum = 0;
	for (const entry_t& e : input)
	{
		for(int64_t a = 0; a <= e.px / e.ax && a <= 100; a++)
		{
			int64_t remainder = e.px - a * e.ax;

			if (remainder % e.bx == 0)
			{
				int64_t b = remainder / e.bx;
				if (a * e.ay + b * e.by == e.py)
				{
					sum += a * 3 + b;
					//std::println("{}*3 + {} * 1 = {}", a, b, a * 3 + b);
					break;
				}
			}

		}
	}
	std::print("{}\n", sum);
}

void solve_2(const input_t& input_)
{
	int64_t sum = 0;
	input_t input = input_;
	for (auto& e : input)
	{
		e.px += 10'000'000'000'000ll;
		e.py += 10'000'000'000'000ll;
		
		// solving the system of two eq:
		// a * e.ax + b * e.bx = e.px
		// a * e.ay + b * e.by = e.py
		int64_t denom = e.bx * e.ay - e.by * e.ax;
		if (denom != 0)
		{
			int64_t nom = (e.px * e.ay - e.py * e.ax);
			if (nom % denom == 0)
			{
				int64_t b = nom / denom;
				int64_t a = (e.px - b * e.bx);
				if (a % e.ax == 0)
				{
					a /= e.ax;
					sum += a * 3 + b;
					//std::println("{}*3 + {} * 1 = {}", a, b, a * 3 + b);
				}
			}
		}

	}
	std::print("{}\n", sum);
}

int main()
{
	input_t input = read();
	solve_1(input);
	solve_2(input);

	return 0;
}