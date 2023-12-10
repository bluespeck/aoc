#include "../utility.h"


#include <algorithm>
#include <array>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>
#include <unordered_map>

using Grid = std::vector < std::string >;
using Input = Grid;

[[nodiscard]] Input read() {
	Input input;
	std::string line;
	while (std::getline(std::cin, line)) {
		input.push_back(line);
	}
	add_padding(input, '.', 1);
	return input;
}

using Position = std::pair<int32_t, int32_t>;
constexpr Position invalid_pos = { -1, -1 };

Position find_start_position(const Grid& grid) {
	for (size_t i = 0; i < grid.size(); i++) {
		for (size_t j = 0; j < grid[i].size(); j++) {
			if (grid[i][j] == 'S') return { i, j };
		}
	}
	return invalid_pos;
}

char get_pipe(const Grid& grid, Position p){
	return (p.first == -1 || p.second == -1) ? '.' : grid[p.first][p.second];
}

Position get_next_step(char c, Position p, Position prev) {
	switch (c)	{
		case '|': return prev.first < p.first ? Position{ p.first + 1, p.second } : Position{ p.first - 1, p.second };
		case '-': return prev.second < p.second ? Position{ p.first, p.second + 1 } : Position{ p.first, p.second - 1 };
		case 'F': return prev.first == p.first ? Position{ p.first + 1, p.second } : Position{ p.first, p.second + 1 };
		case '7': return prev.first == p.first ? Position{ p.first + 1, p.second } : Position{ p.first, p.second - 1 };
		case 'L': return prev.first == p.first ? Position{ p.first - 1, p.second } : Position{ p.first, p.second + 1 };
		case 'J': return prev.first == p.first ? Position{ p.first - 1, p.second } : Position{ p.first, p.second - 1 };
	}
	return invalid_pos;
}

std::pair<Position, Position> get_neighboring_positions(char c, Position p){
	switch (c)	{
		case '|': return { { p.first - 1, p.second }, { p.first + 1, p.second } };
		case '-': return { { p.first, p.second -1}, { p.first, p.second + 1} };
		case 'F': return { { p.first + 1, p.second }, { p.first, p.second + 1} };
		case '7': return { { p.first + 1, p.second }, { p.first, p.second - 1} };
		case 'L': return { { p.first - 1, p.second }, { p.first, p.second + 1} };
		case 'J': return { { p.first - 1, p.second }, { p.first, p.second - 1 } };
	}
	return { invalid_pos, invalid_pos };
}

bool are_connected(const Grid& grid, Position p1, Position p2)
{
	auto [p11, p12] = get_neighboring_positions(get_pipe(grid, p1), p1);
	auto [p21, p22] = get_neighboring_positions(get_pipe(grid, p2), p2);
	return (p11 == p2 || p12 == p2) && (p21 == p1 || p22 == p1);
}

void solve_1(const Input& input) {
	int32_t max_dist = 0;
	Position start_pos = find_start_position(input);
	for (char c : "|-LJ7F" ) {
		auto grid = input;
		int32_t dist = 1;
		auto [p1, p2] = get_neighboring_positions(c, start_pos);
		Position prev1 = start_pos;
		Position prev2 = start_pos;
		while (get_pipe(grid, p2) != '.' && get_pipe(grid, p1) != '.' && get_pipe(grid, p1) != 'S' && get_pipe(grid, p2) != 'S' && p1 != p2 && !are_connected(grid, p1, p2)) {
			char c1 = get_pipe(grid, p1);
			char c2 = get_pipe(grid, p2);
			auto p1n = get_next_step(c1, p1, prev1);
			auto p2n = get_next_step(c2, p2, prev2);
			if (p1n == invalid_pos || p2n == invalid_pos) {
				break;
			}
			prev1 = p1;
			p1 = p1n;
			prev2 = p2;
			p2 = p2n;
			dist++;
		};
		if (dist > max_dist && (p1 == p2 || are_connected(grid, p1, p2))) {
			max_dist = dist;
		}
	}

	outf("{}\n", max_dist);
}

void flood_fill(Grid& grid, Position pos, std::vector<Position> loop)
{
	if (pos.first < 0 || pos.first >= grid.size() || pos.second < 0 || pos.second >= grid[0].size()) return;
	for (const Position& p : loop) {
		if (pos == p) {
			return;
		}
	}
	grid[pos.first][pos.second] = 'O';
	flood_fill(grid, { pos.first + 1, pos.second }, loop);
	flood_fill(grid, { pos.first - 1, pos.second }, loop);
	flood_fill(grid, { pos.first, pos.second + 1}, loop);
	flood_fill(grid, { pos.first, pos.second + 1}, loop);
}

void solve_2(const Input& input) {
	int32_t max_dist = 0;
	std::vector < Position > max_loop_positions;
	char max_start_char = '.';
	Position start_pos = find_start_position(input); 
	for (char c : "|-LJ7F" ) {
		auto grid = input;
		int32_t dist = 1;
		auto [p1, p2] = get_neighboring_positions(c, start_pos);
		std::vector < Position > loop_positions = { start_pos, p1 };
		std::vector < Position > loop_positions_2 = { p2 };
		Position prev1 = start_pos;
		Position prev2 = start_pos;
		while (get_pipe(grid, p2) != '.' && get_pipe(grid, p1) != '.' && get_pipe(grid, p1) != 'S' && get_pipe(grid, p2) != 'S' && p1 != p2 && !are_connected(grid, p1, p2)) {
			char c1 = get_pipe(grid, p1);
			char c2 = get_pipe(grid, p2);
			auto p1n = get_next_step(c1, p1, prev1);
			auto p2n = get_next_step(c2, p2, prev2);
			if (p1n == invalid_pos || p2n == invalid_pos) {
				break;
			}
			prev1 = p1;
			p1 = p1n;
			loop_positions.push_back(p1);
			prev2 = p2;
			p2 = p2n;
			loop_positions_2.push_back(p2);
			dist++;
		};
		if (dist > max_dist && (p1 == p2 || are_connected(grid, p1, p2))) {
			max_dist = dist;
			max_loop_positions = loop_positions;
			if (p1 == p2) loop_positions_2.pop_back();
			std::ranges::reverse(loop_positions_2);
			for (auto& pos : loop_positions_2) {
				max_loop_positions.push_back(pos);
			}
			max_start_char = c;
		}
	}

	outf("max path: ");
	for (auto& pos: max_loop_positions)	{
		outf("{} ", get_pipe(input, pos));
	}
	outf("\n");

	//for (size_t i = 0; i < max_loop_positions.size() - 1;
	Grid grid = input;
	for (int32_t i = 0; i < grid.size(); i++) {
		for (int32_t j = 0; j < grid[i].size(); j++) {
			grid[i][j] = '.';
		}
	}
	for (auto& loop_pos: max_loop_positions) {
		grid[loop_pos.first][loop_pos.second] = input[loop_pos.first][loop_pos.second];
	}
	grid[start_pos.first][start_pos.second] = max_start_char;

	for (int32_t i = 0; i < grid.size(); i++) {
		outf("{}\n", grid[i]);
	}
	outf("\n");

	size_t count = 0;
	for (int32_t i = 0; i < grid.size(); i++) {
		int32_t transition_count = 0;
		size_t between_transitions_count = 0;
	
		for (int32_t j = 0; j < grid[i].size(); j++) {
			bool found = false;
			for (auto& pair : max_loop_positions) {
				if (i == pair.first && j == pair.second) {
					found = true;
					break;
				}
			}
			// skip horizontals
			if (found && grid[i][j] == 'F' || grid[i][j] == 'L')
			{
				bool from_below = grid[i][j] == 'F';
				for (j++; j < grid[i].size() && grid[i][j] == '-' && grid[i][j] != '7' && grid[i][j] != 'J'; j++);
				bool to_below = grid[i][j] == '7';
				transition_count += from_below != to_below;
			}
			else
			{
				transition_count += found;
			}

			if (!found && transition_count %2 == 1) {
				between_transitions_count++;
			}
			else if (transition_count % 2 == 0 && between_transitions_count > 0) {
				count += between_transitions_count; 
				between_transitions_count = 0;
			}
			
		}
	}

	outf("{}\n", count);
}

int main() {
	Input input = read();
	solve_1(input);
	solve_2(input);

	return 0;
}