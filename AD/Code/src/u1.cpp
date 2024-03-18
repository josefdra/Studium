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
        primes.push_back(a);
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