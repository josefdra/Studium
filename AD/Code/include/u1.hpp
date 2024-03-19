#include <iostream>
#include <vector>
#include <numeric>
#include <chrono>

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
void two(int n);

class Matrix
{
public:
    Matrix(int x, int y);
    ~Matrix();
    void init();
    void print();
    void input();
    void randomFill();
    void add(Matrix m);
    void mult(Matrix m);

private:
    int size_x;
    int size_y;
    std::vector<int> numbers;
    std::vector<std::vector<int>> lines;
};

void three();