#include "u1.hpp"

void ggt()
{
    int a, b;
    std::cout << "Zahl a eingeben:" << std::endl;
    std::cin >> a;
    std::cout << "Zahl b eingeben:" << std::endl;
    std::cin >> b;
    a = ggt(a, b);
    std::cout << "Der größte gemeinsame Teiler ist: " << a << std::endl;
}

int ggt(int a, int b)
{
    int r;
    do
    {
        r = a % b;
        a = b;
        b = r;
    } while (r != 0);
    return a;
}

int ggt_rek(int a, int b)
{
    int r;
    r = a % b;
    a = b;
    b = r;
    if (r == 0)
    {
        return a;
    }
    else
    {
        return ggt_rek(a, b);
    }
}

std::ostream &
operator<<(std::ostream &out, const std::vector<int> &vec)
{
    for (int i = 0; i < vec.size(); i++)
    {
        if ((i) % 25 == 0)
        {
            out << std::endl;
        }
        if ((i + 1) < vec.size())
        {
            out << vec[i] << ", ";
        }
        else
        {
            out << vec[i];
        }
    }
    return out;
}

std::vector<int> primes(int a)
{
    int x = 2;
    std::vector<int> primes;
    if (a != 1)
    {
        while (x * x <= a)
        {
            while (a % x == 0)
            {
                primes.push_back(x);
                a /= x;
            }
            x++;
        }
        if (a != 1)
        {
            primes.push_back(a);
        }
    }
    return primes;
}

int kgV(int a, int b)
{
    int x = 1;
    std::vector<int> total_primes;
    std::vector<int> primes_a = primes(a);
    std::vector<int> primes_b = primes(b);

    do
    {
        if (primes_a.size() == 0)
        {
            while (primes_b.size() != 0)
            {
                total_primes.push_back(primes_b[0]);
                primes_b.erase(primes_b.begin());
            }
        }
        else if (primes_b.size() == 0)
        {
            while (primes_a.size() != 0)
            {
                total_primes.push_back(primes_a[0]);
                primes_a.erase(primes_a.begin());
            }
        }
        else if (primes_a[0] == primes_b[0])
        {
            total_primes.push_back(primes_a[0]);
            primes_a.erase(primes_a.begin());
            primes_b.erase(primes_b.begin());
        }
        else if (primes_a[0] < primes_b[0])
        {
            total_primes.push_back(primes_a[0]);
            primes_a.erase(primes_a.begin());
        }
        else
        {
            total_primes.push_back(primes_b[0]);
            primes_b.erase(primes_b.begin());
        }
    } while (primes_a.size() != 0 || primes_b.size() != 0);

    for (int element : total_primes)
    {
        x *= element;
    }

    return x;
}

void one()
{
    int x, y, z;
    for (int i = 30; i < 41; i++)
    {
        for (int j = 30; j < 41; j++)
        {
            x = ggt(i, j);
            y = kgV(i, j);
            z = i * j;
            std::cout << "i: " << i << ", j: " << j << ", ggt: " << x << ", kgV: " << y << ", z: " << z << std::endl;
        }
    }
}

// template <typename T, size_t N>
// std::ostream &operator<<(std::ostream &out, const std::array<T, N> &arr)
// {
//   for (int i = 0; i < arr.size(); i++) {
//     if ((i + 1) % 50 == 0){
//         out << std::endl;
//     }
//     if ((i + 1) < arr.size()) {
//       out << arr[i] << ", ";
//     } else {
//       out << arr[i];
//     }
//   }
//   return out;
// }

void erasthotenes(std::vector<int> &vec)
{
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    std::vector<int> result;
    vec.erase(vec.begin());
    int max = vec.back();
    std::cout << max << std::endl;
    int a = 2;
    while (a < max)
    {
        for (int i = (a - 1); i < vec.size(); i++)
        {
            if ((vec[i] % a) == 0 && vec[i] != a)
            {
                vec[i] = 0;
            }
        }
        if (vec[a] == 0)
        {
            while (vec[a] == 0 && a < max)
                a++;
        }
        else
        {
            a++;
        }
    }
    for (int element : vec)
    {
        if (element != 0)
        {
            result.push_back(element);
        }
    }
    std::cout << result << std::endl;
    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Duration: " << duration.count() << std::endl;
}

void two(int n)
{
    std::vector<int> tab(n);
    std::iota(tab.begin(), tab.end(), 1);
    erasthotenes(tab);
}

Matrix::Matrix(int x, int y) : size_x(x), size_y(y), numbers(std::vector<int>(x)), lines(std::vector<std::vector<int>>(y)) {}
Matrix::~Matrix() {}

void Matrix::init()
{
    for (std::vector<int> line : lines)
    {
        for (int number : line)
        {
            number = 0;
        }
    }
}

void Matrix::print()
{
    for (std::vector<int> line : lines)
    {
        for (int number : line)
        {
            std::cout << number;
        }
        std::cout << std::endl;
    }
}

void Matrix::input()
{
    for (int i = size_y; i < lines.size(); i++)
    {
        for (int j = size_x; j < lines.size(); j++)
        {
            std::cout << "Type value for field (" << i + 1 << ", " << j + 1 << "): ";
            std::cin >> lines.at(i).at(j);
        }
    }
}

void Matrix::randomFill()
{
    for (std::vector<int> line : lines)
    {
        for (int number : line)
        {
            number = rand();
        }
    }
}

void Matrix::add(Matrix m)
{
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    int moves = 0;
    for (int i = size_y; i < lines.size(); i++)
    {
        for (int j = size_x; j < lines.size(); j++)
        {
            lines.at(i).at(j) = lines.at(i).at(j) + m.lines.at(i).at(j);
            moves++;
        }
    }
    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Moves: " << moves << "Duration: " << duration.count() << std::endl;
}

void Matrix::mult(Matrix m)
{
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    int moves = 0;
    if (numbers.size() == m.lines.size())
    {
        int temp;
        int counter = 0;
        while (counter < m.numbers.size())
        {
            for (int i = size_y; i < lines.size(); i++)
            {
                for (int j = size_x; j < lines.size(); j++)
                {
                    for (int x = numbers.size(); x >= 0; x--)
                    {
                        temp += lines.at(i).at(x) * m.lines.at(counter).at(x);
                        moves++;
                    }
                    lines.at(i).at(j) = temp;
                }
            }
            counter++;
        }
        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        std::cout << "Moves: " << moves << "Duration: " << duration.count() << std::endl;
    }
    else
    {
        std::cout << "invalid" << std::endl;
    }
}