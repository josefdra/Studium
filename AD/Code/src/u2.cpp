#include "u2.hpp"

signed int working_register = 0;
uint16_t program_counter = 0;
std::array<signed int, 8> storage_registers = {0};
bool running = true;
bool jump = false;
std::vector<std::tuple<std::string, uint16_t>> commands;
std::ostringstream message;

void add(signed int n)
{
    message << "add value in register " << n << " to value in working register";
    working_register = working_register + storage_registers[n];
}

void sub(signed int n)
{
    message << "subtract value in register " << n << " from value in working register";
    working_register = working_register - storage_registers[n];
}

void mul(signed int n)
{
    message << "multiply value in working register with value in register " << n;
    working_register = working_register * storage_registers[n];
}

void divi(signed int n)
{
    message << "divide value in working register by value in register " << n;
    working_register = working_register / storage_registers[n];
}

void lda(signed int n)
{
    message << "load value in register " << n << " to working register";
    working_register = storage_registers[n];
}

void ldk(signed int n)
{
    message << "load value " << n << " to working register";
    working_register = n;
}

void sta(signed int n)
{
    message << "store value in working register to register " << n;
    storage_registers[n] = working_register;
}

void inp(signed int n)
{
    message << "store user input value to register " << n;
    std::cout << "Input eingeben: " << std::endl;
    std::cin >> storage_registers[n];
}

void out(signed int n)
{
    message << "print value in register " << n;
    std::cout << "Output: " << storage_registers[n] << std::endl;
}

void hlt(signed int n)
{
    message << "end";
    running = false;
}

void jmp(signed int n)
{
    message << "jump, PC = " << n;
    jump = true;
    program_counter = n;
}

void jez(signed int n)
{
    message << "jump equal zero";
    if (working_register == 0)
    {
        message << ", jump to " << n;
        jump = true;
        program_counter = n;
    }
    else
    {
        message << ", no jump";
    }
}

void jne(signed int n)
{
    message << "jump not equal zero";
    if (working_register != 0)
    {
        message << ", jump to " << n;
        jump = true;
        program_counter = n;
    }
    else
    {
        message << ", no jump";
    }
}

void jlz(signed int n)
{
    message << "jump lower zero";
    if (working_register < 0)
    {
        message << ", jump to " << n;
        jump = true;
        program_counter = n;
    }
    else
    {
        message << ", no jump";
    }
}

void jle(signed int n)
{
    message << "jump lower equal zero";
    if (working_register <= 0)
    {
        message << ", jump to " << n;
        jump = true;
        program_counter = n;
    }
    else
    {
        message << ", no jump";
    }
}

void jgz(signed int n)
{
    message << "jump greater zero";
    if (working_register > 0)
    {
        message << ", jump to " << n;
        jump = true;
        program_counter = n;
    }
    else
    {
        message << ", no jump";
    }
}

void jge(signed int n)
{
    message << "jump greater equal zero";
    if (working_register > 0)
    {
        message << ", jump to " << n;
        jump = true;
        program_counter = n;
    }
    else
    {
        message << ", no jump";
    }
}

void readString(const std::string inputfile)
{
    std::ifstream inputFile(inputfile);
    std::stringstream string;
    string << inputFile.rdbuf();
    while (string)
    {
        std::string temp;
        uint16_t n = 0;
        string >> temp;
        string >> n;
        if (temp[0] != '\0')
        {
            commands.push_back(std::make_tuple(temp, n));
        }
    }
}

std::unordered_map<std::string, void (*)(signed int)> string_int = {
    {"ADD", add},
    {"SUB", sub},
    {"MUL", mul},
    {"DIV", divi},
    {"LDA", lda},
    {"LDK", ldk},
    {"STA", sta},
    {"INP", inp},
    {"OUT", out},
    {"HLT", hlt},
    {"JMP", jmp},
    {"JEZ", jez},
    {"JNE", jne},
    {"JLZ", jlz},
    {"JLE", jle},
    {"JGZ", jgz},
    {"JGE", jge}};

void print_registers()
{
    std::cout << "rw: " << working_register << " | r0: " << storage_registers[0] << " | r1: " << storage_registers[1] << " | r2: " << storage_registers[2] << " | r3: " << storage_registers[3] << " | r4: " << storage_registers[4] << " | r5: " << storage_registers[5] << " | r6: " << storage_registers[6] << " | r7: " << storage_registers[7] << std::endl;
    ;
}

void start()
{
    while (running)
    {
        std::string temp_string;
        signed int temp_int;
        uint16_t temp_counter;
        std::tie(temp_string, temp_int) = commands[program_counter];
        auto func = string_int.find(temp_string);
        if (func != string_int.end())
        {
            temp_counter = program_counter;
            func->second(temp_int);
            if (jump == false)
            {
                program_counter++;
            }
            else
            {
                jump = false;
            }
            std::cout << "PC: " << temp_counter << ", Command: " << message.str() << std::endl;
            print_registers();
            message.str("");
            if (program_counter >= commands.size())
            {
                running = false;
                std::cout << "end" << std::endl;
            }
        }
        else
        {
            running = false;
            std::cout << "function not found" << std::endl;
        }
    }
}

void run(const std::string inputfile)
{
    readString(inputfile);
    start();
}