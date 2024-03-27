#include "u2.hpp"

std::vector<std::array<uint8_t, 3>> command_vec;

void readString(const std::string inputfile)
{
    std::unordered_map<std::string, uint8_t> string_int;
    string_int["ADD"] = 1;
    string_int["SUB"] = 2;
    string_int["MUL"] = 3;
    string_int["DIV"] = 4;
    string_int["LDA"] = 5;
    string_int["LDK"] = 6;
    string_int["STA"] = 7;
    string_int["INP"] = 8;
    string_int["OUT"] = 9;
    string_int["HLT"] = 10;
    string_int["JMP"] = 11;
    string_int["JEZ"] = 12;
    string_int["JNE"] = 13;
    string_int["JLZ"] = 14;
    string_int["JLE"] = 15;
    string_int["JGZ"] = 16;
    string_int["JGE"] = 17;
    std::ifstream inputFile(inputfile);
    std::stringstream string;
    string << inputFile.rdbuf();
    while (string)
    {
        std::string temp;
        std::array<uint8_t, 3> command;
        string >> temp[0];
        string >> temp[1];
        string >> temp[2];
        std::cout << temp[0] << temp[1] << temp[2] << std::endl;
        switch (string_int[temp])

            string >> command[1];
        string >> command[2];
        command_vec.push_back(command);
    }
    command_vec.pop_back();
}

void printString()
{
    for (auto command : command_vec)
    {
        std::cout << command[0] << " " << command[1] << command[2] << std::endl;
    }
}

void one(const std::string inputfile)
{
    readString(inputfile);
    printString();
}