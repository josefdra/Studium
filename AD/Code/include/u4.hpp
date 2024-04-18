#ifndef U4_HPP
#define U4_HPP

#include <vector>
#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <map>

void insertion_sort(std::vector<int> &);
void modified_insertion_sort(std::vector<int> &);
void recursive_insertion_sort(std::vector<int> &);
void bubble_sort(std::vector<int> &);
void modified_bubble_sort(std::vector<int> &);
void selection_sort(std::vector<int> &);
void modified_selection_sort(std::vector<int> &);
void swap(int &, int &);
void prepare_partition(std::vector<int> &, int, int, int &);
void quick_sort(std::vector<int> &, int, int);
void modified_prepare_partition(std::vector<int> &, int, int, int &);
void modified_quick_sort(std::vector<int> &, int, int);
void merge(std::vector<int> &, int, int, int);
void merge_sort(std::vector<int> &, int, int);
void iterative_merge_sort(std::vector<int> &, int, int);
bool find_sum_n_log_n(std::vector<int> &, int);
bool find_sum_n(std::vector<int>, int);

#endif // U4_HPP