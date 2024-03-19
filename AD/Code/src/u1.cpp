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
            // x = ggt_rek(i, j);
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

void two()
{
    int n = 10000;
    std::vector<int> tab(n);
    std::iota(tab.begin(), tab.end(), 1);
    erasthotenes(tab);
}

Matrix::Matrix(int x, int y)
{
    init(x, y);
}
Matrix::~Matrix() {}

void Matrix::init(int x, int y)
{
    for (int i = 0; i < x; i++)
    {
        numbers.push_back(0);
    }
    for (int i = 0; i < y; i++)
    {
        lines.push_back(numbers);
    }
}

void Matrix::print()
{
    for (int i = 0; i < lines.size(); i++)
    {
        for (int j = 0; j < numbers.size(); j++)
        {
            std::cout << lines.at(i).at(j) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Matrix::input()
{
    for (int i = 0; i < lines.size(); i++)
    {
        for (int j = 0; j < numbers.size(); j++)
        {
            std::cout << "Type value for field (" << i + 1 << ", " << j + 1 << "): ";
            std::cin >> lines.at(i).at(j);
        }
    }
    std::cout << std::endl;
}

void Matrix::randomFill()
{
    for (int i = 0; i < lines.size(); i++)
    {
        for (int j = 0; j < numbers.size(); j++)
        {
            lines.at(i).at(j) = rand() % 100;
        }
    }
}

void Matrix::add(Matrix m)
{
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    int moves = 0;
    for (int i = 0; i < lines.size(); i++)
    {
        for (int j = 0; j < numbers.size(); j++)
        {
            lines.at(i).at(j) = lines.at(i).at(j) + m.lines.at(i).at(j);
            moves++;
        }
    }
    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Moves: " << moves << ", Duration: " << duration.count() << "s" << std::endl;
    std::cout << std::endl;
}

void Matrix::mult(Matrix m)
{
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    unsigned long int moves = 0;
    if (numbers.size() == m.lines.size())
    {
        Matrix temp(m.numbers.size(), lines.size());
        for (int counter = 0; counter < m.numbers.size(); counter++)
        {
            for (int i = 0; i < lines.size(); i++)
            {
                for (int j = 0; j < numbers.size(); j++)
                {
                    temp.lines.at(i).at(counter) += lines.at(i).at(j) * m.lines.at(j).at(counter);
                    moves++;
                }
            }
        }
        numbers = temp.numbers;
        lines = temp.lines;
        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        std::cout << "Moves: " << moves << ", Duration: " << duration.count() << "s" << std::endl;
        std::cout << std::endl;
    }
    else
    {
        std::cout << "invalid" << std::endl;
        std::cout << std::endl;
    }
}

void three()
{
    int x = 100;
    Matrix m1(x, x);
    Matrix m2(x, x);
    m1.randomFill();
    m2.randomFill();
    m2.add(m2);
    m1.mult(m2);
    m1.print();
}

/*
Addition 1min:
Addition 2min:
Addition 5min:
Addition 10min:

Multiplikation 1min: (1135x1135) Moves: 1462135375, Duration: 59.5013s
Multiplikation 2min: (1420x1420) Moves: 2863288000, Duration: 120.004s
Multiplikation 5min: (1851x1851) Moves: 2046930755, Duration: 299.324s
Multiplikation 10min: (2301x2301) Moves: 3592942309, Duration: 599.701s
*/