#pragma once

#include <iostream>
#include <vector>
#include <iomanip>
#include <chrono>

void swap(int &, int &);
void heapify(std::vector<int> &, int, int);
void heap_sort(std::vector<int> &, int);
std::vector<std::vector<int>> combine(std::vector<std::vector<int>> &, std::vector<std::vector<int>> &, std::vector<std::vector<int>> &, std::vector<std::vector<int>> &);
std::vector<std::vector<int>> split(std::vector<std::vector<int>> &, int, int);
std::vector<std::vector<int>> randomCreate(int);
std::vector<std::vector<int>> operator+(std::vector<std::vector<int>>, std::vector<std::vector<int>>);
std::vector<std::vector<int>> operator-(std::vector<std::vector<int>>, std::vector<std::vector<int>>);
std::vector<std::vector<int>> operator*(std::vector<std::vector<int>>, std::vector<std::vector<int>>);
std::ostream &operator<<(std::ostream &out, const std::vector<std::vector<int>> &);
std::ostream &operator<<(std::ostream &out, const std::vector<int> &);
