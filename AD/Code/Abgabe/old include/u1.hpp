#ifndef U1_HPP
#define U1_HPP

#include <iostream>
#include <vector>
#include <numeric>
#include <chrono>
#include <immintrin.h>

void ggt();
int ggt(int a, int b);
int ggt_rek(int a, int b);
std::ostream &operator<<(std::ostream &out, const std::vector<int> &vec);
std::vector<int> primes(int a);
int kgV(int a, int b);
void one();
// template <typename T, size_t N>
// std::ostream &operator<<(std::ostream &out, const std::array<T, N> &arr);
void erasthotenes(std::vector<int> &vec);
void two();

class Matrix
{
public:
    Matrix(double x, double y);
    ~Matrix();
    void init(double x, double y);
    void print();
    void input();
    void randomFill();
    void add(Matrix m);
    void mult(Matrix m);

private:
    std::vector<double> numbers;
    std::vector<std::vector<double>> lines;
};

void three();

#endif // U1_HPP