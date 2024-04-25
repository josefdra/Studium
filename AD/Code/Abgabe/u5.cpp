#include "u5.hpp"

void swap(int &a, int &b){
    int temp = a;
    a = b;
    b = temp;
}

void heapify(std::vector<int> &arr, int n, int i)
{
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n && arr[l] > arr[largest])
        largest = l;

    if (r < n && arr[r] > arr[largest])
        largest = r;

    if (largest != i)
    {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heap_sort(std::vector<int> &arr, int n)
{
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (int i = n - 1; i >= 0; i--)
    {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

std::vector<std::vector<int>> combine(std::vector<std::vector<int>> &m11, std::vector<std::vector<int>> &m12, std::vector<std::vector<int>> &m21, std::vector<std::vector<int>> &m22)
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

std::vector<std::vector<int>> split(std::vector<std::vector<int>> &m, int n1, int n2)
{
    std::vector<std::vector<int>> matrix;
    std::vector<int> line;

    for (int y = 0 + n1 * m.size() / 2; y < m.size() / 2 + n1 * m.size() / 2; y++)
    {
        for (int x = 0 + n2 * m.size() / 2; x < m.size() / 2 + n2 * m.size() / 2; x++)
        {
            line.push_back(m[y][x]);
        }
        matrix.push_back(line);
        line.clear();
    }
    return matrix;
}

std::vector<std::vector<int>> randomCreate(int size)
{
    std::vector<std::vector<int>> matrix;
    for (int i = 0; i < size; i++)
    {
        std::vector<int> line;
        for (int j = 0; j < size; j++)
        {
            line.push_back(rand() % 100);
        }
        matrix.push_back(line);
    }
    return matrix;
}

std::vector<std::vector<int>> operator+(std::vector<std::vector<int>> m, std::vector<std::vector<int>> n)
{
    std::vector<std::vector<int>> ret;
    std::vector<int> line;
    for (int i = 0; i < m.size(); i++)
    {
        for (int j = 0; j < m.size(); j++)
        {
            line.push_back(m[i][j] + n[i][j]);
        }
        ret.push_back(line);
        line.clear();
    }
    return ret;
}

std::vector<std::vector<int>> operator-(std::vector<std::vector<int>> m, std::vector<std::vector<int>> n)
{
    std::vector<std::vector<int>> ret;
    std::vector<int> line;
    for (int i = 0; i < m.size(); i++)
    {
        for (int j = 0; j < m.size(); j++)
        {
            line.push_back(m[i][j] - n[i][j]);
        }
        ret.push_back(line);
        line.clear();
    }
    return ret;
}

std::vector<std::vector<int>> operator*(std::vector<std::vector<int>> m, std::vector<std::vector<int>> n)
{
    if (m.size() == 1)
    {
        std::vector<std::vector<int>> vec;
        std::vector<int> line;
        line.push_back(m[0][0] * n[0][0]);
        vec.push_back(line);
        return vec;
    }
    std::vector<std::vector<int>> m11, m12, m21, m22, n11, n12, n21, n22, h1, h2, h3, h4, h5, h6, h7, o11, o12, o21, o22;
    m11 = split(m, 0, 0);
    m12 = split(m, 0, 1);
    m21 = split(m, 1, 0);
    m22 = split(m, 1, 1);
    n11 = split(n, 0, 0);
    n12 = split(n, 0, 1);
    n21 = split(n, 1, 0);
    n22 = split(n, 1, 1);
    h1 = (m11 + m22) * (n11 + n22);
    h2 = (m21 + m22) * n11;
    h3 = m11 * (n12 - n22);
    h4 = m22 * (n21 - n11);
    h5 = (m11 + m12) * n22;
    h6 = (m21 - m11) * (n11 + n12);
    h7 = (m12 - m22) * (n21 + n22);
    o11 = h1 + h4 - h5 + h7;
    o12 = h3 + h5;
    o21 = h2 + h4;
    o22 = h1 - h2 + h3 + h6;

    return combine(o11, o12, o21, o22);
}

std::ostream &operator<<(std::ostream &out, const std::vector<std::vector<int>> &m)
{
    for (auto &line : m)
    {
        for (auto &value : line)
        {
            out << std::setw(3) << value << " ";
        }
        out << std::endl;
    }
    return out;
}

std::ostream &operator<<(std::ostream &out, const std::vector<int> &v)
{
    for (auto &value : v)
    {
        out << value << " ";
    }
    out << std::endl;

    return out;
}
