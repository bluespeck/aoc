#include "../utility.h"

#include <string>
#include <vector>

using Input = std::vector<std::vector<int64_t>>;

[[nodiscard]] Input read() {
	Input input;
	std::string line;
	while (std::getline(std::cin, line)) {
		input.push_back(split_string_and_convert_to_numbers(line, " "));
	}

	return input;
}

void solve_1(const Input& input) {
	int64_t sum = 0;
	for (auto sequence:input) {
		std::vector<int64_t> last_values = { sequence.back() };
		std::vector<int64_t> new_sequence;

		while (true) {
			bool is_all_zeros = true;
			for (size_t i = 1; i < sequence.size(); i++) {
				new_sequence.push_back(sequence[i] - sequence[i-1]);
				if (new_sequence.back() != 0) {
					is_all_zeros = false;
				}
			}
			last_values.push_back(new_sequence.back());
		
			if (is_all_zeros) {
				break;
			}
			sequence = new_sequence;
			new_sequence.clear();
		}
		int64_t predicted_value = 0;
		for (int32_t i = last_values.size() - 1; i >= 0; i--) {
			predicted_value = last_values[i] + predicted_value;
		}
		sum += predicted_value;
	}
	
	outf("{}\n", sum);
}

void solve_2(const Input& input) {
	int64_t sum = 0;
	for (auto sequence : input) {
		std::vector<int64_t> first_values = { sequence.front() };
		std::vector<int64_t> new_sequence;

		while (true) {
			bool is_all_zeros = true;
			for (size_t i = 1; i < sequence.size(); i++) {
				new_sequence.push_back(sequence[i] - sequence[i - 1]);
				if (new_sequence.back() != 0) {
					is_all_zeros = false;
				}
			}
			first_values.push_back(new_sequence.front());

			if (is_all_zeros) {
				break;
			}
			sequence = new_sequence;
			new_sequence.clear();
		}

		int64_t predicted_value = 0;
		for (int32_t i = first_values.size() - 1; i >= 0; i--) {
			predicted_value = first_values[i] - predicted_value;
		}
		sum += predicted_value;
	}

	outf("{}\n", sum);
}

int main() {
	Input input = read();
	solve_1(input);
	solve_2(input);

	return 0;
}