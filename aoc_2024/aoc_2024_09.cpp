#include "../utility.h"

#include <array>
#include <string>
#include <vector>
#include <print>

using input_t = std::string;
using disk_t = std::vector<int64_t>;

[[nodiscard]] input_t read()
{
	input_t input;
	std::string line;
	std::getline(std::cin, line);
	input = line;
	return input;
}

void print_disk(const disk_t& disk)
	{
		for (int32_t i = 0; i < disk.size(); i++)
		{
			if (disk[i] == -1)
				std::print(".");
			else
				std::print("{}", disk[i]);
		}
		std::println();
	};

void solve_1(const input_t& input)
{
	disk_t disk;
	disk.reserve(10 * input.size());
	for (int32_t i = 0; i < input.size(); i += 2)
	{
		int32_t id = i / 2;
		for (int32_t j = 0; j < input[i] - '0'; j++)
		{
			disk.push_back(id);
		}
		for (int32_t j = 0; j < input[i+1] - '0'; j++)
		{
			disk.push_back(-1);
		}
	}

	auto find_free = [&]() -> int32_t
		{
			for (int32_t i = 0; i < disk.size(); i++)
			{
				if (disk[i] == -1) return i;
			}
			return -1;
		};

	auto find_last_file_block = [&]() -> int32_t
		{
			for (int32_t i = disk.size() - 1; i >= 0; i--)
			{
				if (disk[i] != -1) return i;
			}
			return -1;
		};

	//print_disk(disk);
	while (true)
	{
		int32_t free_it = find_free();
		int32_t block_it = find_last_file_block();
		if (block_it < free_it) break;
		std::swap(disk[free_it], disk[block_it]);
		//print_disk(disk);
	}

	int64_t sum = 0;

	int32_t block_it = find_last_file_block();
	for (int64_t i = 0; i <= block_it; i++)
	{
		sum += i * disk[i];
	}

	std::print("{}\n", sum);
}

void solve_2(const input_t& input)
{
	disk_t disk;
	disk.reserve(10 * input.size());
	for (int32_t i = 0; i < input.size(); i += 2)
	{
		int32_t id = i / 2;
		for (int32_t j = 0; j < input[i] - '0'; j++)
		{
			disk.push_back(id);
		}
		for (int32_t j = 0; j < input[i + 1] - '0'; j++)
		{
			disk.push_back(-1);
		}
	}

	auto find_free = [&](int32_t size) -> int32_t
		{
			for (int32_t i = 0; i < disk.size(); i++)
			{
				if (disk[i] == -1)
				{
					int32_t sz = size;
					for (int32_t j = i; j < disk.size() && disk[j] == -1 && sz > 0; j++, sz--);
					if (sz == 0)
					{
						return i;
					}
				}
			}
			return -1;
		};

	auto find_file_start = [&](int32_t id) -> int32_t
		{
			for (int32_t i = disk.size() - 1; i >= 0; i--)
			{
				if (disk[i] == id)
				{
					for (int32_t j = i; j > 0; j--)
					{
						if (disk[j] != id)
						{
							return j + 1;
						}
					}
				}
			}
			return -1;
		};
	
	//print_disk(disk);
	for(int32_t id = input.size() / 2; id > 0; id--)
	{
		int32_t block_it = find_file_start(id);
		int32_t file_size = input[id * 2] - '0';
		int32_t free_it = find_free(file_size);
		if (free_it == -1 || free_it >= block_it)
		{
			continue;
		}	
		for (int32_t i = 0; i < file_size; i++)
		{
			disk[free_it + i] = disk[block_it + i];
			disk[block_it + i] = -1;
		}
		//print_disk(disk);
	}

	int64_t sum = 0;

	for (int64_t i = 0; i < disk.size(); i++)
	{
		sum += i * (disk[i] == -1 ? 0 : disk[i]);
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