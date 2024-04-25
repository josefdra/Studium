#include "u5.hpp"

int main()
{
    // std::vector<int> a = {-5, 13, -32, 7, -3, 17, 23, 12, -35, 19};
    // heap_sort(a, a.size() - 1);
    // std::cout << a << std::endl;
    int size = 512;
    std::vector<std::vector<int>> m1 = randomCreate(size);
    std::vector<std::vector<int>> m2 = randomCreate(size);
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<int>> m3 = m1 * m2;
    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Size: " << size << ", Duration: " << duration.count() << "s" << std::endl;
    std::cout << std::endl;
}