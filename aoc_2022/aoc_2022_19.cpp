#include "../outf.h"

#include <string>
#include <vector>
#include <thread>

struct blueprint_t
{
    int32_t ore_bot_ore;
    
    int32_t clay_bot_ore;
    
    int32_t obsidian_bot_ore;
    int32_t obsidian_bot_clay;

    int32_t geode_bot_ore;
    int32_t geode_bot_obsidian;

    int32_t delta_resources[5][3];
    size_t index;
};

using input_t = std::vector<blueprint_t>;
input_t read()
{
    input_t input;
    std::string line;
    size_t index = 0;

    while (std::getline(std::cin, line))
    {
        auto parts = split_string(line.substr(std::string("Blueprint 1:").size()), ".");
        blueprint_t blueprint;
        
        auto ore_bot_parts = split_string(parts[0], " ");
        blueprint.ore_bot_ore = std::stoi(ore_bot_parts[5]);

        auto clay_bot_parts = split_string(parts[1], " ");
        blueprint.clay_bot_ore = std::stoi(clay_bot_parts[5]);

        auto obsidian_bot_parts = split_string(parts[2], " ");
        blueprint.obsidian_bot_ore = std::stoi(obsidian_bot_parts[5]);
        blueprint.obsidian_bot_clay = std::stoi(obsidian_bot_parts[8]);

        auto geode_bot_parts = split_string(parts[3], " ");
        blueprint.geode_bot_ore = std::stoi(geode_bot_parts[5]);
        blueprint.geode_bot_obsidian = std::stoi(geode_bot_parts[8]);

        blueprint.delta_resources[0][0] = 0;
        blueprint.delta_resources[0][1] = 0;
        blueprint.delta_resources[0][2] = 0;

        blueprint.delta_resources[1][0] = -blueprint.ore_bot_ore;
        blueprint.delta_resources[1][1] = 0;
        blueprint.delta_resources[1][2] = 0;

        blueprint.delta_resources[2][0] = -blueprint.clay_bot_ore;
        blueprint.delta_resources[2][1] = 0;
        blueprint.delta_resources[2][2] = 0;

        blueprint.delta_resources[3][0] = -blueprint.obsidian_bot_ore;
        blueprint.delta_resources[3][1] = -blueprint.obsidian_bot_clay;
        blueprint.delta_resources[3][2] = 0;

        blueprint.delta_resources[4][0] = -blueprint.geode_bot_ore;
        blueprint.delta_resources[4][1] = 0;
        blueprint.delta_resources[4][2] = -blueprint.geode_bot_obsidian;
        blueprint.index = index++;
        input.push_back(blueprint);
    }

    input_t p = input;
    return p;
}

enum class activity_type_t : uint32_t
{
    do_nothing,
    build_ore_bot,
    build_clay_bot,
    build_obsidian_bot,
    build_geode_bot
};

struct state_t
{
    int32_t resources[4] = { 1,0,0,0 };
    int32_t bots[4] = { 1,0,0,0 };
    int32_t time = 1;
};

int32_t time_limit = 24;

void recurse(state_t state, const blueprint_t& bp, int32_t& max_geode_count)
{
    if (state.time == time_limit)
    {
        if (state.resources[3] > max_geode_count)
        {
            max_geode_count = state.resources[3];
        }

        return;
    }
   
    auto time_remaining = (time_limit - state.time);
    int32_t required_ore_to_make_only_geode_bots = (time_remaining - 1) * bp.delta_resources[3][0];
    int32_t ore_we_can_produce_until_the_end = (time_remaining - 1) * state.bots[0] + state.resources[0];
    int32_t required_obsidian_to_make_only_geode_bots = (time_remaining - 1) * bp.delta_resources[3][2];
    int32_t obsidian_we_can_produce_until_the_end = (time_remaining - 1) * state.bots[2] + state.resources[2];
    int32_t max_geodes_we_can_produce_until_the_end = state.resources[3] + time_remaining * state.bots[3] + (time_remaining - 1) * time_remaining / 2;
    if (max_geodes_we_can_produce_until_the_end <= max_geode_count
        || (
            max_geodes_we_can_produce_until_the_end == max_geode_count + 1
            &&
            (
                (obsidian_we_can_produce_until_the_end + required_obsidian_to_make_only_geode_bots < 0)
                || (ore_we_can_produce_until_the_end + required_ore_to_make_only_geode_bots < 0)
                )
            )
        )
    {
        return;
    }

    for (activity_type_t e : { activity_type_t::build_geode_bot, activity_type_t::build_obsidian_bot, activity_type_t::build_clay_bot, activity_type_t::build_ore_bot })
    {
        auto new_state = state;
        new_state.time++;

        if ((new_state.resources[0] += bp.delta_resources[static_cast<uint32_t>(e)][0]) < 0)
            continue;
        if ((new_state.resources[1] += bp.delta_resources[static_cast<uint32_t>(e)][1]) < 0)
            continue;
        if ((new_state.resources[2] += bp.delta_resources[static_cast<uint32_t>(e)][2]) < 0)
            continue;

        for (size_t i = 0; i < 4; i++)
        {
            new_state.resources[i] += new_state.bots[i];
        }

        ++new_state.bots[static_cast<uint32_t>(e) - 1];
        recurse(new_state, bp, max_geode_count);
    }

    { // do_nothing
        auto new_state = state;
        new_state.time++;
        for (size_t i = 0; i < 4; i++)
        {
            new_state.resources[i] += new_state.bots[i];
        }

        recurse(new_state, bp, max_geode_count);
    }
}

int32_t get_blueprint_max_geodes(const blueprint_t& bp, int32_t initial_max_val = 0)
{
    std::vector<std::vector<int32_t>> max_geodes;

    state_t initial_state;

    int32_t max_geode_count = initial_max_val;
    recurse(initial_state, bp, max_geode_count);
    
    return max_geode_count;
}

void solve_1(const input_t& input)
{
    time_limit = 24;
    int32_t sum = 0;

    std::vector<std::jthread> threads;
    std::vector<int32_t> results;
    results.resize(input.size(), 0);
    for (size_t i = 0; i < input.size(); i++)
    {
        auto& bp = input[i];
        threads.emplace_back([&bp, &results, i]() {
            results[i] = (i + 1) * get_blueprint_max_geodes(bp);
            });
    }
    for (auto& e : threads)
    {
        e.join();
    }
    for (auto e : results)
        sum += e;

    outf("{}\n", sum);
}

void solve_2(const input_t& input)
{
    time_limit = 32;
    int32_t prod = 1;

    std::vector<std::jthread> threads;
    std::vector<int32_t> max_vals = { 26, 4, 17 }; // gotten iteratively by gradually increasing the time limit
    std::vector<int32_t> results;
    results.resize(input.size(), 0);
    for (size_t i = 0; i < 3; i++)
    {
        auto& bp = input[i];
        threads.emplace_back([&bp, &results, &max_vals, i]() {
            results[i] = get_blueprint_max_geodes(bp, max_vals[i]);
            });
    }
    for (auto& e : threads)
    {
        e.join();
    }
    for (auto e : results)
    {
        outf("{} * ", e);
        prod *= e;
    }
    outf(" = {}\n", prod);

}

int main()
{
    input_t input = read();
    solve_1(input);
    solve_2(input);

    return 0;
}