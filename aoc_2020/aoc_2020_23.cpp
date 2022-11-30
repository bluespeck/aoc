import std.core;

using input_t = std::vector<int>;

input_t
read_input()
{
    input_t input;

    char c;
    while (std::cin >> c)
    {
        if (isdigit(c))
        {
            input.push_back(c - '0');
        }
    }

    return input;
}

void print_cups(const input_t& cups, int current_cup_label)
{
    std::cout << "cups: ";
    for (auto e : cups)
    {
        std::cout << (e == current_cup_label ? "(" : "") <<e << (e == current_cup_label ? ") " : " ");
    }
    std::cout << std::endl;
}

void print_n_cups(const input_t& cups, int current_cup_label, int n)
{
    std::cout << "cups: ";
    for (int i = 0; i < n; i++)
    {
        auto e = cups[(cups.size() - 10 + i)%cups.size()];
        std::cout << (e == current_cup_label ? "(" : "") << e << (e == current_cup_label ? ") " : " ");
    }
    std::cout << std::endl;
}

void
solve_1(const input_t& input)
{
    auto cups = input;

    int current_cup_index = 0;
    const auto [min_cup_label, max_cup_label] = std::ranges::minmax(cups);

    for (int turn = 1; turn <= 100; turn++)
    {
        //print_cups(cups, cups[current_cup_index]);

        int found_cup_index = -1;
        int cup_label_to_find = cups[current_cup_index];
        while (found_cup_index == -1)
        {
            cup_label_to_find--;
            if (cup_label_to_find < min_cup_label)
            {
                cup_label_to_find = max_cup_label;
            }

            for (int j = (current_cup_index + 4) % cups.size(); j != current_cup_index; j = (j + 1) % cups.size())
            {
                if (cups[j] == cup_label_to_find 
                    && j != (current_cup_index + 1)  % cups.size()
                    && j != (current_cup_index + 2)  % cups.size()
                    && j != (current_cup_index + 3)  % cups.size())
                     
                {
                    found_cup_index = j;
                    break;
                }
            }
        }

        int three_cups[3];
        for (int k = 0; k < 3; k++)
        {
            three_cups[k] = cups[(current_cup_index + k + 1) % cups.size()];
        }
        int j = (current_cup_index + 1) % cups.size();
        for (;
            (j + 3) % cups.size() != (found_cup_index + 1) % cups.size();
            j = (j + 1) % cups.size())
        {
            cups[j] = cups[(j + 3) % cups.size()];
        }
        for (int k = 0; k < 3; k++)
        {
            cups[(j + k) % cups.size()] = three_cups[k];
        }

        current_cup_index = (current_cup_index + 1) % cups.size();
   }
    print_cups(cups, cups[current_cup_index]);

}

void
solve_2_1(const input_t& input)
{
    auto cups = input;
    
    int current_cup_index = 0;
    const auto [min_cup_label, max_cup_label] = std::ranges::minmax(cups);

    for (int i = max_cup_label + 1; i <= 1'000'000; i++)
    {
        cups.push_back(i);
    }

    int prev_selected_index = current_cup_index + 1;
    for (int turn = 1; turn <= 10'000'000; turn++)
    {
        if (turn % 100'000 < 50)
        {
            std::cout << "turn " << turn << '\n';
            print_n_cups(cups, cups[current_cup_index], 50);
        }

        int found_cup_index = -1;
        int cup_label_to_find = cups[current_cup_index];
        while (found_cup_index == -1)
        {
            cup_label_to_find--;
            if (cup_label_to_find < min_cup_label)
            {
                cup_label_to_find = max_cup_label;
            }

            for (int j = prev_selected_index; j != current_cup_index; j = (j + 1) % cups.size())
            {
                if (cups[j] == cup_label_to_find)
                {
                    found_cup_index = j;
                    break;
                }
            }
            if( found_cup_index != -1 &&
                found_cup_index != (current_cup_index + 1) % cups.size()
            && found_cup_index != (current_cup_index + 2) % cups.size()
            && found_cup_index != (current_cup_index + 3) % cups.size())
            {
                prev_selected_index = (found_cup_index + cups.size() - 4) % cups.size();
            }
            else
            {
                prev_selected_index = (prev_selected_index + cups.size() - 10) % cups.size();
            }
        }

        int three_cups[3];
        for (int k = 0; k < 3; k++)
        {
            three_cups[k] = cups[(current_cup_index + k + 1) % cups.size()];
        }
        int j = (current_cup_index + 1) % cups.size();
        for (;
            (j + 3) % cups.size() != (found_cup_index + 1) % cups.size();
            j = (j + 1) % cups.size())
        {
            cups[j] = cups[(j + 3) % cups.size()];
        }
        for (int k = 0; k < 3; k++)
        {
            cups[(j + k) % cups.size()] = three_cups[k];
        }

        current_cup_index = (current_cup_index + 1) % cups.size();
    }
    for (int i = 0; i < cups.size(); i++)
    {
        if (cups[i] == 1)
        {
            auto i1 = cups[(i + 1) % cups.size()];
            auto i2 = cups[(i + 2) % cups.size()];
            std::cout << i1 << " " << i2 << std::endl;
            std::cout << i1 * i2 << std::endl;
            break;
        }
    }
}

struct node_t
{
    node_t* next = nullptr;
    node_t* prev = nullptr;
    int64_t label;
};

void
move_next_3_nodes(node_t* source, node_t* dest)
{
    auto start = source->next;
    auto end = source->next->next->next;
    
    auto after_end = end->next;
    auto after_dest = dest->next;

    source->next = after_end;
    after_end->prev = source;

    dest->next = start;
    start->prev = dest;
    
    end->next = after_dest;
    after_dest->prev = end;
}

int64_t
get_distance(node_t* node1, node_t* node2)
{
    int64_t dist = 0;
    for (auto it = node1; it != node2; it = it->next)
    {
        dist++;
    }
    return dist;
}

node_t* find_node(node_t* start, int64_t label_to_find)
{
    if (start->label == label_to_find)
    {
        return start;
    }
    else
    {
        for (auto it = start->next; it != start; it = it->next)
            if (it->label == label_to_find)
                return it;
    }
    return nullptr;
}

void
print_n_cups(node_t* first, int n, int current_cup_label)
{
    //std::cout << "Distance : " << get_distance(find_node(first, current_cup_label), find_node(first, (current_cup_label + 1'000'000 - 1) % 1'000'000)) << "\n";

    node_t* first_to_print = first;
    //while (current_cup_label->next->next->next->next->next->next->next->next->next != first)
    //{
    //    first_to_print = first_to_print->next;
    //}
    node_t* it = first_to_print;

    for (int i = 0; i < n; i++)
    {
        std::cout << (it->label == current_cup_label ? "(" : "") << it->label << (it->label == current_cup_label ? ") " : " ");
        it = it->next;
    }
    std::cout << "\n";
}

void
solve_2(const input_t& input)
{
    std::unordered_map<int64_t, node_t*> node_map;
    node_t* first = nullptr;
    node_t* current = nullptr;
    for (auto e : input)
    {
        if (first == nullptr)
        {
            first = new node_t;
            current = first;
            current->label = e;
            node_map[e] = current;
            current->next = first;
            current->prev = first;
        }
        else
        {
            auto prev = current;
            current->next = new node_t;
            current = current->next;
            current->prev = prev;

            current->label = e;
            node_map[e] = current;

            current->next = first;
            first->prev = current;
        }
    }
    for (int64_t i = 10; i <= 1'000'000; i++)
    {
        auto prev = current;
        current->next = new node_t;
        current = current->next;
        current->prev = prev;

        current->label = i;
        node_map[i] = current;

        current->next = first;
        first->prev = current;
    }

    auto current_cup_node = first;
    node_t* prev_selected_nodes[3] = { first, first, first };

    for (int turn = 1; turn != 10'000'000; turn++)
    {
        int64_t current_cup = current_cup_node->label;
        //if (turn % 100'000 < 50)
        //{
        //    std::cout << "turn " << turn << std::endl;
        //    print_n_cups(first, 20, current_cup);
        //}

        int64_t cup_to_find = current_cup;
        node_t* selected_cup_node = nullptr;
        while (selected_cup_node == nullptr)
        {
            cup_to_find--;
            if (cup_to_find < 1)
                cup_to_find = 1'000'000;

            auto candidate_node = node_map[cup_to_find];

            if (
                candidate_node != current_cup_node->next
                && candidate_node != current_cup_node->next->next
                && candidate_node != current_cup_node->next->next->next
                )
            {
                selected_cup_node = candidate_node;
            }
        }

        move_next_3_nodes(current_cup_node, selected_cup_node);

        current_cup_node = current_cup_node->next;
    }

    std::cout << node_map[1]->next->label * node_map[1]->next->next->label << "\n";
}

int
main()
{
    input_t input = read_input();

    solve_1(input);
    solve_2(input);

    return 0;
}