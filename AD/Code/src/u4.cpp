#include "u4.hpp"

void insertion_sort(std::vector<int> &vec)
{
    int i, j, key;

    for (j = 1; j < vec.size(); j++)
    {
        key = vec[j];
        i = j - 1;
        while (i >= 0 && vec[i] > key)
        {
            vec[i + 1] = vec[i];
            i = i - 1;
        }
        vec[i + 1] = key;
    }
}

void modified_insertion_sort(std::vector<int> &vec)
{
    int i, j, key;

    for (j = (vec.size() - 2); j >= 0; j--)
    {
        key = vec[j];
        i = j + 1;
        while (i < vec.size() && vec[i] < key)
        {
            vec[i - 1] = vec[i];
            i = i + 1;
        }
        vec[i - 1] = key;
    }
}

void recursive_insertion_sort(std::vector<int> &vec)
{
}

void bubble_sort(std::vector<int> &vec)
{
    for (int i = 0; i < vec.size(); i++)
    {
        for (int j = vec.size() - 2; j >= i; j--)
        {
            if (vec[j] > vec[j + 1])
            {
                int h = vec[j];
                vec[j] = vec[j + 1];
                vec[j + 1] = h;
            }
        }
    }
}

void modified_bubble_sort(std::vector<int> &vec)
{
    for (int i = vec.size() - 1; i >= 0; i--)
    {
        for (int j = 1; j <= i; j++)
        {
            if (vec[j] < vec[j - 1])
            {
                int h = vec[j];
                vec[j] = vec[j - 1];
                vec[j - 1] = h;
            }
        }
    }
}

void selection_sort(std::vector<int> &vec)
{
    int i, j, min;
    for (i = 0; i < vec.size(); i++)
    {
        min = i;
        for (j = i; j < vec.size(); j++)
        {
            if (vec[j] < vec[min])
                min = j;
        }
        int h = vec[i];
        vec[i] = vec[min];
        vec[min] = h;
    }
}

void modified_selection_sort(std::vector<int> &vec)
{
    int i, j, max;
    for (i = vec.size() - 1; i >= 0; i--)
    {
        max = i;
        for (j = 0; j < i; j++)
        {
            if (vec[j] > vec[max])
                max = j;
        }
        int h = vec[i];
        vec[i] = vec[max];
        vec[max] = h;
    }
}

void swap(int &a, int &b)
{
    int h = b;
    b = a;
    a = h;
}

void prepare_partition(std::vector<int> &vec, int first, int last, int &p)
{
    int pivot = vec[first];
    p = first - 1;
    for (int i = first; i <= last; i++)
    {
        if (vec[i] <= pivot)
        {
            p++;
            swap(vec[i], vec[p]);
        }
    }
    swap(vec[first], vec[p]);
}

void quick_sort(std::vector<int> &vec, int first, int last)
{
    int part;
    if (first < last)
    {
        prepare_partition(vec, first, last, part);
        quick_sort(vec, first, part - 1);
        quick_sort(vec, part + 1, last);
    }
}

void modified_prepare_partition(std::vector<int> &vec, int first, int last, int &p)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, last - first);
    p = first - 1;
    int random_index = first + dis(gen);
    int pivot = vec[random_index];
    swap(vec[first], vec[random_index]);
    for (int i = first; i <= last; i++)
    {
        if (vec[i] <= pivot)
        {
            p++;
            swap(vec[i], vec[p]);
        }
    }
    swap(vec[first], vec[p]);
}

void modified_quick_sort(std::vector<int> &vec, int first, int last)
{
    int part;
    if (first < last)
    {
        modified_prepare_partition(vec, first, last, part);
        modified_quick_sort(vec, first, part - 1);
        modified_quick_sort(vec, part + 1, last);
    }
}

void merge(std::vector<int> &vec, int first, int last, int m)
{
    int i, n = last - first + 1;
    int a1f = first, a1l = m - 1;
    int a2f = m, a2l = last;
    int *anew = new int[n];
    for (i = 0; i < n; i++)
    {
        if (a1f <= a1l)
        {
            if (a2f <= a2l)
            {
                if (vec[a1f] <= vec[a2f])
                    anew[i] = vec[a1f++];
                else
                    anew[i] = vec[a2f++];
            }
            else
                anew[i] = vec[a1f++];
        }
        else
            anew[i] = vec[a2f++];
    }
    for (i = 0; i < n; i++)
        vec[first + i] = anew[i];
    delete[] anew;
}

void merge_sort(std::vector<int> &vec, int first, int last)
{
    if (first < last)
    {
        int m = (first + last + 1) / 2;
        merge_sort(vec, first, m - 1);
        merge_sort(vec, m, last);
        merge(vec, first, last, m);
    }
}

void iterative_merge_sort(std::vector<int> &vec, int first, int last)
{
}

/*
Times for sorting:

Bubble Sort: Number count: 110000, Duration: 60.9915s
Quick Sort: Number count: 200000000, Duration: 62.4178s
Merge Sort: Number count: 25000000, Duration: 66.3246s
*/

bool find_sum_n_log_n(std::vector<int> &vec, int n)
{
    merge_sort(vec, 0, vec.size() - 1);
    if (n < (2 * vec[vec.size() - 1] + 1))
    {
        int pf = 0;
        int pl = vec.size() - 1;
        for (int i = 0; i < vec.size(); i++)
        {

            if (pl > pf)
            {
                if ((vec[pf] + vec[pl]) == n)
                {
                    std::cout << n << " is a sum of: " << vec[pf] << " + " << vec[pl] << std::endl;
                    return true;
                }
                else if ((vec[pf] + vec[pl]) > n)
                {
                    pl--;
                }
                else if ((vec[pf] + vec[pl]) < n)
                {
                    pf++;
                }
            }
        }
        return false;
    }
    else
    {
        return false;
    }
}

bool find_sum_n(std::vector<int> vec, int n)
{
    std::unordered_set<int> num;
    for (int i = 0; i < vec.size(); i++)
    {
        int diff = n - vec[i];
        if (num.find(diff) != num.end())
        {
            std::cout << n << " is a sum of: " << diff << " + " << vec[i] << std::endl;
            return true;
        }
        num.insert(diff);
    }
    return false;
}
