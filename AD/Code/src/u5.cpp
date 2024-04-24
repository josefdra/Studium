#include "u5.hpp"

std::vector<std::vector<int>> add(std::vector<std::vector<int>> m11, std::vector<std::vector<int>> m12, std::vector<std::vector<int>> m21, std::vector<std::vector<int>> m22)
{
    std::vector<std::vector<int>> vec;
    std::vector<int> line;

    for (int y = 0; y < m11.size(); y++)
    {
        vec.push_back(line);
        for (int x = 0; x < m11.size(); x++)
        {
            vec[y].push_back(m11[y][x]);
        }
    }
    for (int y = 0; y < m11.size(); y++)
    {
        for (int x = m11.size(); x < m11.size() * 2; x++)
        {
            vec[y].push_back(m12[y][x - m11.size()]);
        };
    }
    for (int y = m11.size(); y < m11.size() * 2; y++)
    {
        vec.push_back(line);
        for (int x = 0; x < m11.size(); x++)
        {
            vec[y].push_back(m21[y - m11.size()][x]);
        }
    }
    for (int y = m11.size(); y < m11.size() * 2; y++)
    {
        for (int x = m11.size(); x < m11.size() * 2; x++)
        {
            vec[y].push_back(m22[y - m11.size()][x - m11.size()]);
        };
    }
    return vec;
}

std::vector<std::vector<int>> split(std::vector<std::vector<int>> m, int n1, int n2)
{
    std::vector<std::vector<int>> vec;
    std::vector<int> line;

    for (int y = 0 + n1 * m.size() / 2; y < m.size() / 2 + n1 * m.size() / 2; y++)
    {
        for (int x = 0 + n2 * m.size() / 2; x < m.size() / 2 + n2 * m.size() / 2; x++)
        {
            line.push_back(m[y][x]);
        }
        vec.push_back(line);
        line.clear();
    }
    return vec;
}

std::vector<std::vector<int>> strassen(std::vector<std::vector<int>> m1, std::vector<std::vector<int>> m2)
{
    if (m1.size() == 2)
    {
        std::vector<std::vector<int>> vec;
        std::vector<int> line;
        int h1, h2, h3, h4, h5, h6, h7, o11, o12, o21, o22;
        h1 = (m1[0][0] + m1[1][1]) * (m2[0][0] + m2[1][1]);
        h2 = (m1[1][0] + m1[1][1]) * m2[0][0];
        h3 = m1[0][0] * (m2[0][1] - m2[1][1]);
        h4 = m1[1][1] * (m2[1][0] - m2[0][0]);
        h5 = (m1[0][0] + m1[0][1]) * m2[1][1];
        h6 = (m1[1][0] + m1[0][0]) * (m2[0][0] + m2[0][1]);
        h7 = (m1[0][1] + m1[1][1]) * (m2[1][0] + m2[1][1]);
        o11 = h1 + h4 - h5 + h7;
        o12 = h3 + h5;
        o21 = h2 + h4;
        o22 = h1 - h2 + h3 + h6;
        line.push_back(o11);
        line.push_back(o12);
        vec.push_back(line);
        line.clear();
        line.push_back(o21);
        line.push_back(o22);
        vec.push_back(line);
        return vec;
    }
    else
    {
        return add(strassen(split(m1, 0, 0), split(m2, 0, 0)), strassen(split(m1, 0, 1), split(m2, 0, 1)), strassen(split(m1, 1, 0), split(m2, 1, 0)), strassen(split(m1, 1, 1), split(m2, 1, 1)));
    }
}

void print_2d_vector(std::vector<std::vector<int>> m)
{
    for (auto &line : m)
    {
        for (auto &value : line)
        {
            std::cout << std::setw(3) << value << " ";
        }
        std::cout << std::endl;
    }
}