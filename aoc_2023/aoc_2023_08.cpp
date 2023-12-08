#include "../utility.h"

#include <algorithm>
#include <array>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>
#include <unordered_map>

struct Input {
	mutable std::unordered_map<std::string, std::pair<std::string, std::string>> tree;
	std::string sequence;
};

[[nodiscard]] Input read() {
	Input input;
	std::string line;
	std::getline(std::cin, line);
	input.sequence = line;
	std::getline(std::cin, line);
	while (std::getline(std::cin, line)) {
		auto tokens = split_string(line.substr(0, line.size() - 1), " = (");
		auto other_tokens = split_string(tokens[1], ", ");
		input.tree[tokens[0]] = std::make_pair(other_tokens[0], other_tokens[1]);
	}

	return input;
}

void solve_1(const Input& input) {
	std::string node = "AAA";
	int step = 0;
	while (true) {
		bool found = false;
		for (auto c: input.sequence) {
			if (c == 'L') node = input.tree[node].first;
			else node = input.tree[node].second;
			step++;
			if (node == "ZZZ"){
				found = true;
				break;
			} 
		}
		if (found) break;
	}
	outf("{}\n", step);
}

void solve_2(const Input& input) {
	std::vector<std::string> nodes;
	for (auto& [str, val]: input.tree) {
		if (str[2] == 'A') {
			nodes.push_back(str);
		}
	}
	size_t step = 0;
	std::vector<int64_t> periods(nodes.size(), 0);
	size_t index = 0;
	for (auto & starting_node : nodes)
	{
		int step = 0;
		std::string node = starting_node;
		while (true) {
			bool found = false;
			for (size_t i = 0; i < input.sequence.size(); i++) {
				char c = input.sequence[i];
				if (c == 'L') node = input.tree[node].first;
				else node = input.tree[node].second;
				step++;
				if (node[2] == 'Z' && i == input.sequence.size() - 1){
					found = true;
					break;
				} 
			}
			if (found) break;
		}
		periods[index++] = step;
	}
	
	int64_t lcm = periods[0];
	for (size_t i = 1; i < periods.size(); i++)
	{
		lcm = std::lcm(lcm, periods[i]);
	}

	outf("{}\n", lcm);

	//while (true) {
	//	bool found = false;
	//	for (auto c: input.sequence) {
	//		bool any_non_z = false;
	//		for (size_t i = 0; i < nodes.size(); i++)
	//		{
	//			if (c == 'L') nodes[i] = input.tree[nodes[i]].first;
	//			else nodes[i] = input.tree[nodes[i]].second;
	//			if (nodes[i][2] != 'Z')	{
	//				any_non_z = true;
	//			}
	//			//outf("{} ", current_nodes[i]);
	//		}
	//		//outf("\n");
	//		step++;
	//
	//		if (!any_non_z) {
	//			found = true;
	//			break;
	//		}
	//	}
	//	if (found) {
	//		break;
	//	}
	//}
	//outf("{}\n", step);
}

int main() {
	Input input = read();
	solve_1(input);
	solve_2(input);

	return 0;
}