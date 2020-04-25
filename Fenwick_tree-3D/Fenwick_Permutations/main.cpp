#include <stdio.h>
#include <utility>
#include <vector>
#include <algorithm>

template <typename Type>
class Fenwick_Tree
{
public:
    Fenwick_Tree (Type* array, int size);
    
    Type count_sum_prefix (int x);
    Type count_sum (int x1, int x2);
    void update (int x, const Type& delta);
    
    ~Fenwick_Tree (void);
private:

    Type* sums;
    int size;

    int and_func (int arg);
    int or_func  (int arg);

};

template <typename Type>
Fenwick_Tree <Type>:: Fenwick_Tree (Type* array, int size_p) : size (size_p)
{
    this->sums = new Type [size];

    for (int i = 0; i < size_p; ++i)  
        this->sums [i] = 0;

    /*for (int i = 0; i < size; ++i)
        for (int j = this->and_func (i); j <= i; ++j)
            this->sums [i] += array [j];*/
}

template <typename Type>
int Fenwick_Tree <Type> :: and_func (int arg)
{
    return (arg & (arg + 1));
}

template <typename Type>
int Fenwick_Tree <Type> :: or_func (int arg)
{
    return (arg | (arg + 1));
}

template <typename Type>
Type Fenwick_Tree <Type> :: count_sum_prefix (int x)
{
    if (x < 0)
        return 0;

    Type answer = 0;

    for (int i = x; i >= 0; i = this->and_func (i) - 1)
        answer += this->sums [i];

    return answer;
}

template <typename Type>
Type Fenwick_Tree <Type> :: count_sum (int x1, int x2)
{
    --x1;

    Type sum2 = this->count_sum_prefix (x2);
    Type sum1 = this->count_sum_prefix (x1);
    sum2 -= sum1;

    return sum2;
}

template <typename Type>
void Fenwick_Tree <Type> :: update (int x, const Type& delta)
{
    if (x < 0)
        return;

    for (int i = x; i < this->size; i = this->or_func (i))
        this->sums [i] += delta;

    return;
}

template <typename Type>
Fenwick_Tree <Type> :: ~Fenwick_Tree (void)
{
    delete [] this->sums;
}

int main()
{
    setbuf (stdout, NULL);
    FILE* f = fopen ("input.txt", "r");
    //FILE* f = stdin;

    int n = 0;
    fscanf (f, "%d", &n);

    std::vector <std::pair <int, int> > array (n, (std::pair <int, int> (0, 0)));
    std::vector <std::pair <int, int> > adapter (n, (std::pair <int, int> (0, 0)));

    int strength = 0;

    for (int i = 0; i < n; ++i)
    {
        fscanf (f, "%d ", &strength);
        array[i].first = strength;
        adapter[i].first = strength;
        adapter[i].second = i;
    }

    std::sort (adapter.begin(), adapter.end());

    int j = 0;
    for (int i = 0; i < n; ++i)
    {   
        if ((i > 0) && (adapter [i].first != adapter [i - 1].first))
            ++j;

        array [adapter [i].second].second = j;
    }

    int* real  = new int (j + 2);
    for (int i = 0; i < j + 2; ++i)
        real[i] = 0;

    Fenwick_Tree <int> tree (real, j + 2);

    std::vector <int> answer (n, 0);
    int index = 0;

    for (int i = n - 1; i >= 0; --i)
    {
        index = array[i].second;
        //printf ("i %d, index %d\n", i, index);
        answer[i] = tree.count_sum_prefix (index - 1);
        real [index] += 1;
        tree.update (index, 1);
    }
    //for (int i = 0; i < n; ++i)
        //printf ("%d - %d\n", i, answer [i]);


    fclose (f);

    delete [] real;

    return 0;
}