#include "../outf.h"

#include <algorithm>
#include <string>
#include <vector>
#include <list>

using file_t = std::list<int64_t>;

using input_t = std::vector<int64_t>;
input_t read()
{
    input_t input;
    std::string line;

    while (std::getline(std::cin, line))
    {
        input.push_back(std::stoi(line));
    }


    return input;
}



void print_file(const file_t& values, const input_t& input)
{
    for (auto e : values)
    {
        outf("{} ", input[e]);
    }
    outf("\n");
}

void solve_1(const input_t& input)
{
    file_t mixed;
    int64_t index = 0;
    for (auto& e : input)
    {
        mixed.push_back(index++);
    }

    for (int64_t i = 0; i < input.size(); i++)
    {
        //outf("{} moves\n", input[i]);
        auto it = std::find(mixed.begin(), mixed.end(), i);
        auto initial_pos = it;
        it = mixed.erase(initial_pos);
        auto e = input[i];
        if (e > 0)
        {
            for (size_t i = 0; i < e; i++)
            {
                if (it == mixed.end())
                    it = mixed.begin();
                it++;
            }
        }
        else if (e < 0)
        {
            for (size_t i = 0; i < -e; i++)
            {
                if (it == mixed.begin())
                    it = mixed.end();
                it--;
            }
        }
        
        mixed.insert(it, i);
       
        //print_file(mixed, input);
        //outf("\n");
    }
    
    //print_file(mixed, input);
    outf("\n");

    {
        int64_t sum = 0;
        for (int64_t e = 1000; e <= 3000; e += 1000)
        {
            auto it = std::find_if(mixed.begin(), mixed.end(), [&input](auto elem) { return input[elem] == 0; });
            for (size_t i = 0; i < e; i++)
            {
                if (it == mixed.end())
                    it = mixed.begin();
                it++;
            }
        
            sum += input[*it];
            outf("{} ", input[*it]);
        }

        outf(" -> {}\n", sum);
    }
}

void solve_2(const input_t& input_)
{
    auto input = input_;

    file_t mixed;
    int64_t index = 0;
    for (auto& e : input)
    {
        e *= 811589153;
        mixed.push_back(index++);
    }
    for (int64_t k = 0; k < 10; k++)
    {
        for (int64_t i = 0; i < input.size(); i++)
        {
            //outf("{} moves\n", input[i]);
            auto it = std::find(mixed.begin(), mixed.end(), i);
            auto initial_pos = it;
            it = mixed.erase(initial_pos);
            auto e = input[i];
            if (e > 0)
            {
                e = e % mixed.size();
                for (size_t i = 0; i < e; i++)
                {
                    if (it == mixed.end())
                        it = mixed.begin();
                    it++;
                }
            }
            else if (e < 0)
            {
                e = (-e) % mixed.size();
                for (size_t i = 0; i < e; i++)
                {
                    if (it == mixed.begin())
                        it = mixed.end();
                    it--;
                }
            }

            mixed.insert(it, i);

            //print_file(mixed, input);
            //outf("\n");
        }
    }
    //print_file(mixed, input);
    outf("\n");

    {
        int64_t sum = 0;
        for (int64_t e = 1000; e <= 3000; e += 1000)
        {
            auto it = std::find_if(mixed.begin(), mixed.end(), [&input](auto elem) { return input[elem] == 0; });
            for (size_t i = 0; i < e; i++)
            {
                if (it == mixed.end())
                    it = mixed.begin();
                it++;
            }

            sum += input[*it];
            outf("{} ", input[*it]);
        }

        outf(" -> {}\n", sum);
    }

}

int main()
{
    input_t input = read();
    solve_1(input);
    solve_2(input);

    return 0;
}