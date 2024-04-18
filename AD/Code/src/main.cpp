#include "u4.hpp"

int main()
{
    int int_max{std::numeric_limits<int>::max()};
    int int_min{std::numeric_limits<int>::min()};
    
    int n = 1000000;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(int_min, int_max);

    // Zufallsreihen:

    std::vector<int> vec;
    for (int i = 0; i < n; i++)
    {
        vec.push_back(dis(gen));
    }

    // Gleichverteilte Zufallsreihen:

    // std::vector<int> vec2;
    // for (int i = -(n/2); i < n/2; i++)
    // {
    //     vec2.push_back(i);
    // }
    // std::shuffle(vec2.begin(), vec2.end(), gen);

    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    // insertion_sort(vec);
    // modified_insertion_sort(vec);
    recursive_insertion_sort(vec);
    // bubble_sort(vec);
    // modified_bubble_sort(vec);
    // selection_sort(vec);
    // modified_selection_sort(vec);
    // quick_sort(vec, 0, vec.size() - 1);
    // modified_quick_sort(vec, 0, vec.size() - 1);
    // merge_sort(vec, 0, vec.size() - 1);
    // iterative_merge_sort(vec);
    // find_sum_n_log_n(vec, 1);
    // find_sum_n(vec, 1);
    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Quick Sort: Number count: " << n << ", Duration: " << duration.count() << "s" << std::endl;
    std::cout << std::endl;
}