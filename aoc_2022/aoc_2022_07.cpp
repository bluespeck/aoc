#include "../outf.h"

#include <algorithm>
#include <cassert>
#include <string>
#include <vector>

struct file_t
{
    std::string name;
    size_t size;
};

struct dir_tree_t
{
    std::string name;
    std::vector<file_t> files;
    dir_tree_t* parent_dir = nullptr;
    std::vector < dir_tree_t*> sub_dirs;
    size_t size;
};

std::vector<dir_tree_t> dir_storage;

using input_t = dir_tree_t*;
input_t read()
{
    dir_storage.reserve(1024);
    dir_storage.push_back(dir_tree_t{});
    dir_storage.back().name = "/";

    input_t input = &dir_storage.back();

    bool parsing_ls = false;
    dir_tree_t* current_dir = input;
    dir_tree_t* root_dir = input;

    std::string line;
    while (std::getline(std::cin, line))
    {
        if (!line.empty())
        {
            if (line[0] == '$')
            {
                parsing_ls = false;
                if (line[2] == 'c')
                {
                    if (line[5] == '/')
                    {
                        current_dir = root_dir;
                    }
                    else if (line[5] == '.')
                    {
                        current_dir = current_dir->parent_dir;
                    }
                    else
                    {
                        auto dir_name_to_change_to = line.substr(5);
                        for (auto e : current_dir->sub_dirs)
                        {
                            if (e->name == dir_name_to_change_to)
                            {
                                current_dir = e;
                                break;
                            }
                        }
                    }

                }
                else
                {
                    parsing_ls = true;
                }
            }
            else
            {
                assert(parsing_ls && "There's another command with output beside 'ls' ??");
                if (line[0] == 'd')
                {
                    dir_storage.push_back(dir_tree_t{});
                    dir_tree_t* new_dir = &dir_storage.back();
                    
                    new_dir->name = line.substr(4);
                    new_dir->parent_dir = current_dir;

                    current_dir->sub_dirs.push_back(new_dir);
                }
                else
                {
                    auto parts = split_string(line, " ");
                    current_dir->files.emplace_back(parts[1], std::stoull(parts[0]));
                }
            }
        }
    }

    return input;
}

size_t traverse_tree_1(const dir_tree_t* node, size_t& global_accumulator)
{
    size_t current_dir_size = 0;

    for (auto& e : node->files)
    {
        current_dir_size += e.size;
    }

    if (node->sub_dirs.size() > 0)
    {
        for (auto e : node->sub_dirs)
        {
            current_dir_size += traverse_tree_1(e, global_accumulator);
        }
    }
    if (current_dir_size <= 100'000)
    {
        global_accumulator += current_dir_size;
    }

    return current_dir_size;
}


void solve_1(const input_t& input)
{
    size_t accumulator = 0;
    traverse_tree_1(input, accumulator);

    outf("Sum of dir sizes below 100'000 is {}\n", accumulator);
}

size_t traverse_tree_2(dir_tree_t* node)
{
    size_t current_dir_size = 0;

    for (auto& e : node->files)
    {
        current_dir_size += e.size;
    }

    if (node->sub_dirs.size() > 0)
    {
        for (auto e : node->sub_dirs)
        {
            current_dir_size += traverse_tree_2(e);
        }
    }
    
    node->size = current_dir_size;

    return current_dir_size;
}

struct dir_info_t
{
    std::string name;
    size_t size;
};

void solve_2(const input_t& input)
{
    traverse_tree_2(input);
    size_t disk_space_available = 70'000'000;
    size_t disk_space_to_have_unused = 30'000'000;
    size_t disk_space_unused = disk_space_available - input->size;

    std::vector<dir_info_t> dir_infos;

    for (auto& e : dir_storage)
    {
        dir_infos.emplace_back(e.name, e.size);
    }

    std::sort(dir_infos.begin(), dir_infos.end(), [](const auto& a1, const auto& a2) 
        {
            return a1.size < a2.size;
        });
    for (auto& e : dir_infos)
    {
        if (e.size + disk_space_unused >= disk_space_to_have_unused)
        {
            outf("Have to delete {} with size {}\n",e.name, e.size);
            break;
        }
    }
}

int main()
{
    input_t input = read();
    solve_1(input);
    solve_2(input);

    return 0;
}