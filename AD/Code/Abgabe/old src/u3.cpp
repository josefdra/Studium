#include "u3.hpp"

void max_part_sum_2d(std::vector<std::vector<int>> &m)
{
    int s = m.size();    
    int f = m[0].size(); 
    int max = INT_MIN;
    int up = 0, down = 0, left = 0, right = 0;
    for (int i = 0; i < s; i++)
    {
        for (int j = 0; j < f; j++)
        {
            for (int k = 0; k < s; k++)
            {
                for (int l = 0; l < f; l++)
                {
                    int curr = 0;
                    for (int x = i; x <= k; x++)
                    {
                        for (int y = j; y <= l; y++)
                        {
                            curr += m[x][y];
                        }
                    }
                    if (curr > max)
                    {
                        max = curr;
                        up = i;
                        left = j;
                        right = k;
                        down = l;
                    }
                }
            }
        }
    }
    std::cout << "(Oben, Links)"
              << "(" << up << ", " << left << ")" << std::endl;
    std::cout << "(Down, Right)"
              << "(" << down << ", " << right << ")"
              << std::endl;
    std::cout << "The sum of this rectangle is: " << max
              << std::endl;
}
