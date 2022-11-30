import std.core;

enum opcode_t
{
    op_nop,
    op_acc,
    op_jmp
};

std::map<std::string, opcode_t> name_to_opcode = {
    {"nop", op_nop},
    {"acc", op_acc},
    {"jmp", op_jmp}
};

std::array<std::string, 3> opcode_to_name = { "nop", "acc", "jmp" };

struct operation_t
{
    opcode_t opcode;
    std::array<int64_t, 1> params; // only one param used for now
};

struct computer_t
{
    int64_t accumulator;
    size_t instruction_pointer;
    std::vector<operation_t> program;
};

using input_t = std::vector<operation_t>;

void
decode_opcode(input_t& input)
{
    opcode_t opcode;
    std::string opcode_name;
    int64_t value;
    
    std::cin >> opcode_name;
    
    opcode = name_to_opcode[opcode_name];

    switch (opcode)
    {
    case op_nop: std::cin >> value; break;
    case op_acc: std::cin >> value; break;
    case op_jmp: std::cin >> value; break;
    }

    input.push_back({ opcode, {value} });
}

input_t
read_input()
{
    input_t input;
    while (std::cin)
    {
        decode_opcode(input);
    }

    return input;
}

bool
run_program_until_infinite_loop_or_end(computer_t& computer)
{
    computer.instruction_pointer = 0;
    std::set<size_t> operations_ran;
    while (true)
    {
        if (operations_ran.contains(computer.instruction_pointer))
            return false;
        operations_ran.insert(computer.instruction_pointer);

        if (computer.instruction_pointer >= computer.program.size())
            return true;

        auto opcode = computer.program[computer.instruction_pointer].opcode;
        auto value = computer.program[computer.instruction_pointer].params[0];
        switch (opcode)
        {
        case op_nop: computer.instruction_pointer++; break;
        case op_acc: computer.accumulator += value; computer.instruction_pointer++; break;
        case op_jmp: computer.instruction_pointer += value; break;
        }
    }

    return false;
}

void
solve_1(input_t& input)
{
    computer_t computer;
    computer.accumulator = 0;
    computer.program = input;
    run_program_until_infinite_loop_or_end(computer);

    std::cout << computer.accumulator << std::endl;
}

void
solve_2(input_t& input)
{
    computer_t computer;
    computer.accumulator = 0;
    computer.program = input;

    for (size_t i = 0; i < computer.program.size(); i++)
    {
        auto& e = computer.program[i];
        if (e.opcode == op_nop)
        {
            e.opcode = op_jmp;
        }
        else if (e.opcode == op_jmp)
        {
            e.opcode = op_nop;
        }

        computer.instruction_pointer = 0;
        computer.accumulator = 0;
        if (run_program_until_infinite_loop_or_end(computer))
        {
            std::cout << computer.accumulator << std::endl;
            break;
        }

        e.opcode = input[i].opcode;
    }
}

int
main()
{
    input_t input = read_input();
    
    solve_1(input);
    solve_2(input);

    return 0;
}