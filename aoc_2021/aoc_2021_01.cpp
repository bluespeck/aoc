#include <iostream>
#include <vector>
using Input = std::vector<int>;

Input read_input()
{
	Input input;

	while (std::cin)
	{
		int val;
		std::cin >> val;
		std::cin.ignore(1);
		input.push_back(val);		
	}

	return input;
}

void solve_1(const Input& input)
{
	int count = 0;
	for (int i = 0; i < input.size() - 1; i++) {
		if (input[i] < input[i + 1])
		{
			count++;
		}
	}
	std::cout << count << "\n";
}

void solve_2(const Input& input)
{
	int count = 0;
	for(int i = 0; i < input.size() - 3; i++){
		if (input[i] < input[i + 3])
		{
			count++;
		}
	}
	std::cout << count << "\n";
}

int main()
{
	auto input = read_input();
	solve_1(input);
	solve_2(input);
	return 0;
}
