#pragma once 

#include <iostream>
#include <vector>
#include <iomanip>

std::vector<std::vector<int>> split(std::vector<std::vector<int>>, int, int);
std::vector<std::vector<int>> strassen(std::vector<std::vector<int>>, std::vector<std::vector<int>>);
void print_2d_vector(std::vector<std::vector<int>>);