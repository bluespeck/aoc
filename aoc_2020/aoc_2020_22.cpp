import std.core;

using deck_t = std::vector<int64_t>;
struct player_t
{
    deck_t cards;
};

using input_t = std::vector<player_t>;

input_t
read_input()
{
    input_t input;

    while (std::cin)
    {
        player_t player;

        std::string line;
        if (!std::getline(std::cin, line))
            break;
        while (std::getline(std::cin, line))
        {
            if (line.size() == 0)
                break;
            player.cards.push_back(std::stoll(line));
        }

        input.push_back(player);
    };

    return input;
}

bool decks_are_equal(const deck_t& d1, const deck_t& d2)
{
    if (d1.size() != d2.size())
        return false;

    for(auto it1 = d1.cbegin(), it2 = d2.cbegin(); it1 != d1.cend(); it1++, it2++)
    { 
        if (*it1 != *it2)
            return false;
    }
    return true;
}

void pop_front(deck_t& deck)
{
    for (int i = 1; i < deck.size(); i++)
    {
        deck[i - 1] = deck[i];
    }
    deck.pop_back();
    //deck.pop_front();
}

void
solve_1(input_t& input_)
{
    auto input = input_;
    while (input[0].cards.size() > 0 && input[1].cards.size() > 0)
    {
        int64_t current_card_0 = input[0].cards.front();
        int64_t current_card_1 = input[1].cards.front();
        pop_front(input[0].cards);
        pop_front(input[1].cards);

        size_t winner = 0;
        if (current_card_0 < current_card_1)
        {
            winner = 1;
            input[winner].cards.push_back(current_card_1);
            input[winner].cards.push_back(current_card_0);
        }
        else
        {
            winner = 0;
            input[winner].cards.push_back(current_card_0);
            input[winner].cards.push_back(current_card_1);
        }
    }

    size_t winner = 0;
    if (input[0].cards.size() == 0)
    {
        winner = 1;
    }

    int64_t score = 0;
    size_t num_cards = input[winner].cards.size();
    size_t index = 0;
    for (; input[winner].cards.size() > 0; index++)
    {
        int64_t card = input[winner].cards.front();
        pop_front(input[winner].cards);
        score += (num_cards - index) * card;
    }

    std::cout << score << std::endl;
}

size_t recursive_game(const deck_t& deck1, const deck_t& deck2, int level)
{
    auto deck_0 = deck1;
    auto deck_1 = deck2;

    std::vector<deck_t> prev_state_0, prev_state_1;

    auto check_deck_0 = [&deck_0](const auto& deck) {
        return decks_are_equal(deck_0, deck);
    };
    auto check_deck_1 = [&deck_1](const auto& deck) {
        return decks_are_equal(deck_1, deck);
    };

    int round_index = 1;
    while (deck_0.size() > 0 && deck_1.size() > 0)
    {
        //std::cout << "\nr" << round_index++ << " g" << level + 1 << "\n";
        //for (auto e : deck_0)
        //    std::cout << e << " ";
        //std::cout << "\n";
        //for (auto e : deck_1)
        //    std::cout << e << " ";
        //std::cout << "\n";

        size_t winner;

        if (std::ranges::find_if(prev_state_0, check_deck_0) != prev_state_0.end() ||
            std::ranges::find_if(prev_state_1, check_deck_1) != prev_state_1.end())
        {
            return 0;
        }
        else
        {
            prev_state_0.push_back(deck_0);
            prev_state_1.push_back(deck_1);

            int64_t current_card_0 = deck_0.front();
            int64_t current_card_1 = deck_1.front();
            pop_front(deck_0);
            pop_front(deck_1);

            //std::cout << "p1 " << current_card_0 << "\n";
            //std::cout << "p2 " << current_card_1 << "\n";

            if (current_card_0 <= deck_0.size() && current_card_1 <= deck_1.size())
            {
                deck_t new_deck_0;
                auto it0 = deck_0.begin();
                for (int i = 0; i < current_card_0; i++)
                {
                    new_deck_0.push_back(*it0);
                    it0++;
                }
                deck_t new_deck_1;
                auto it1 = deck_1.begin();
                for (int i = 0; i < current_card_1; i++)
                {
                    new_deck_1.push_back(*it1);
                    it1++;
                }

                winner = recursive_game(new_deck_0, new_deck_1, level + 1);
            }
            else
            {
                winner = current_card_0 < current_card_1;
            }
            //std::cout << "w " << winner + 1 << "\n";

            auto& winning_deck = (winner == 1 ? deck_1 : deck_0);
            if(winner == 1)
            {
                winning_deck.push_back(current_card_1);
                winning_deck.push_back(current_card_0);
            }
            else
            {
                winning_deck.push_back(current_card_0);
                winning_deck.push_back(current_card_1);
            }
        }
    }

    if (level == 0)
    {
        auto& winning_deck = (deck_0.size() > 0) ? deck_0 : deck_1;
        {
            size_t score = 0;
            int64_t index = 1;
            for (auto e : winning_deck)
            {
                score += (winning_deck.size() - index++ + 1) * e;
            }
            std::cout << score << std::endl;
        }
    }

    if (deck_0.size() == 0)
        return 1;
    else
        return 0;
}

void
solve_2(const input_t& input_)
{
    auto input = input_;

    size_t winner = recursive_game(input[0].cards, input[1].cards, 0);
}

int
main()
{
    input_t input = read_input();

    solve_1(input);
    solve_2(input);

    return 0;
}