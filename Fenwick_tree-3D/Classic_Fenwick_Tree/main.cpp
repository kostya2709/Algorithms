#include <stdio.h>

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

    int* array = new int [n];

    for (int i = 0; i < n; ++i)
        array [i] = 0;

    Fenwick_Tree <int> tree (array, n);

    fclose (f);

    delete [] array;

    return 0;
}