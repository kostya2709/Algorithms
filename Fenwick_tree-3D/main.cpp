#include <stdio.h>

template <typename Type>
class Fenwick_Tree
{
public:
    Fenwick_Tree (Type*** array, int size);
    
    Type count_sum_prefix (int x, int y, int z);
    Type count_sum (int x1, int y1, int z1, int x2, int y2, int z2);
    void update (int x, int y, int z, const Type& delta);
    
    ~Fenwick_Tree (void);
private:

    Type*** sums;
    int size;

    int and_func (int arg);
    int or_func  (int arg);

};

template <typename Type>
Fenwick_Tree <Type>:: Fenwick_Tree (Type*** array, int size_p) : size (size_p)
{
    this->sums = new Type** [size];
    for (int i = 0; i < size; ++i)
    {
        this->sums[i] = new Type* [size];
        for (int j = 0; j < size; ++j)
            this->sums[i][j] = new Type [size];
    }

    for (int i = 0; i < size_p; ++i)
    {
        for (int j = 0; j < size_p; ++j)
        {
            for (int k = 0; k < size_p; ++k)
            {    
                this->sums [i][j][k] = 0;
            }
        }
    }


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
Type Fenwick_Tree <Type> :: count_sum_prefix (int x, int y, int z)
{
    Type answer = 0;

    for (int i = x; i >= 0; i = this->and_func (i) - 1)
    {
        for (int j = y; j >= 0; j = this->and_func (j) - 1)
        {
            for (int k = z; k >= 0; k = this->and_func (k) - 1)
                {
                    answer += this->sums [i][j][k];
                }
        }
    }

    return answer;
}

template <typename Type>
Type Fenwick_Tree <Type> :: count_sum (int x1, int y1, int z1, int x2, int y2, int z2)
{
    Type* sum = new Type [8];

    --x1;
    --y1;
    --z1;

    sum [0] = this->count_sum_prefix (x2, y2, z2);

    sum [1] = (-1) * this->count_sum_prefix (x1, y2, z2);
    sum [2] = (-1) * this->count_sum_prefix (x2, y1, z2);
    sum [3] = (-1) * this->count_sum_prefix (x2, y2, z1);

    sum [4] = this->count_sum_prefix (x2, y1, z1);
    sum [5] = this->count_sum_prefix (x1, y2, z1);
    sum [6] = this->count_sum_prefix (x1, y1, z2);

    sum [7] = (-1) * this->count_sum_prefix (x1, y1, z1);

    Type main_sum = 0;
    for (int i = 0; i < 8; ++i)
            main_sum += sum[i];

    delete [] sum;

    return main_sum;
}

template <typename Type>
void Fenwick_Tree <Type> :: update (int x, int y, int z, const Type& delta)
{
    for (int i = x; i < this->size; i = this->or_func (i))
    {
        for (int j = y; j < this->size; j = this->or_func (j))
        {
            for (int k = z; k < this->size; k = this->or_func (k))
                this->sums [i][j][k] += delta;
        }
    }

    return;
}

template <typename Type>
Fenwick_Tree <Type> :: ~Fenwick_Tree (void)
{
    for (int i = 0; i < this->size; ++i)
        {
            for (int j = 0; j < this->size; ++j)
                delete [] this->sums [i][j];
            delete [] this->sums [i];
        }
    delete [] this->sums;
}

int main()
{
    setbuf (stdout, NULL);
    //FILE* f = fopen ("input.txt", "r");
    FILE* f = stdin;

    int n = 0;
    fscanf (f, "%d", &n);


    int*** array = new int** [n];

    for (int i = 0; i < n; ++i)
    {
        array[i] = new int* [n];
        for (int j = 0; j < n; ++j)
            array[i][j] = new int [n];
    }

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            for (int k = 0; k < n; ++k)
                array [i][j][k] = 0;

    Fenwick_Tree <int> tree (array, n);

    int command = 0;
    int x1 = 0;
    int x2 = 0;
    int y1 = 0;
    int y2 = 0;
    int z1 = 0;
    int z2 = 0;
    int delta = 0;

    bool staying_alive = 1;

    while (staying_alive)
    {
        fscanf (f, "%d", &command);
        switch (command)
        {
            case 1:
            fscanf (f, "%d %d %d %d", &x1, &y1, &z1, &delta);
            //printf ("add %d %d %d %d\n", x1, y1, z1, delta);
            tree.update (x1, y1, z1, delta);
            break;

            case 2:
            fscanf (f, "%d %d %d %d %d %d", &x1, &y1, &z1, &x2, &y2, &z2);
            //printf ("check %d %d %d %d %d %d\n", x1, y1, z1, x2, y2, z2);
            printf ("%d\n", tree.count_sum (x1, y1, z1, x2, y2, z2));
            break;

            case 3:
            staying_alive = 0;
            break;
        }
    }

    fclose (f);

    for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j)
                delete [] array [i][j];
            delete [] array [i];
        }
    delete [] array;

    return 0;
}