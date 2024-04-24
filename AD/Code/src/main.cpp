#include "u5.hpp"

int main()
{
    std::vector<int> line_m1;
    std::vector<std::vector<int>> m1;
    std::vector<int> line_m2;
    std::vector<std::vector<int>> m2;
    std::vector<std::vector<int>> m3 = strassen(m1, m2);
    print_2d_vector(m3);
}