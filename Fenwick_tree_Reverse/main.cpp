#include <stdio.h>

template <typename Type>
class Fenwick_Tree
{
public:
    Fenwick_Tree (Type* array, int size);
    
    Type count_sum_prefix (int x);
    Type count_sum_suffix (int x);
    Type count_sum (int x1, int x2);
    int count_zeros (int left, int right, int pos);
    void update (int x, const Type& delta);
    
    ~Fenwick_Tree (void);
private:

    Type* sums;
    Type* values;
    int size;

    int and_func (int arg);
    int or_func  (int arg);

};

template <typename Type>
Fenwick_Tree <Type>:: Fenwick_Tree (Type* array, int size) : size (size)
{
    this->values = array;
    this->sums = new Type [size];

    for (int i = 0; i < size; ++i)
        this->sums[i] = 0;

    for (int i = 0; i < size; ++i)
        for (int j = i; (j < this->or_func (i)) && (j < this->size); ++j)
            {
                //printf ("%d %d %d\n", i, j, this->or_func (i));
                if (array [j] == 0)
                    ++this->sums [i];
            }

    //for (int i = 0; i < size; ++i)
        //printf ("s[%d]: %d\n", i, this->sums [i]);
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
Type Fenwick_Tree <Type> :: count_sum_suffix (int x)
{
    Type answer = 0;

    for (int i = x; i < this->size; i = this->or_func (i))
    {
        //printf ("a dodik %d\n", i);
        answer += this->sums [i];
    }
    //printf ("\n");

    return answer;
}

/*
template <typename Type>
Type Fenwick_Tree <Type> :: count_sum_prefix (int x)
{
    Type answer = 0;
printf ("x = %d\n", x);
    for (int i = x; i >= 0; i = this->and_func (i) - 1)
    {
        printf ("%d\n", i);
        answer += this->sums [i];
    }
    printf ("\n");

    return answer;
}

*/

template <typename Type>
Type Fenwick_Tree <Type> :: count_sum (int x1, int x2)
{
    ++x2;

    Type sum1 = this->count_sum_suffix (x1);
    Type sum2 = this->count_sum_suffix (x2);

    sum1 -= sum2;

    return sum1;
}

template <typename Type>
int Fenwick_Tree <Type> :: count_zeros (int left, int right, int pos)
{
    Type counter = pos;
    
    if (left < 0)
        left = 0;

    int i = 0;

    for (i = left; (i < this->size) && (i <= right); i = this->or_func (i))
    {
        //printf ("a dodik %d\n", i);
        if (counter - this->sums [i] <= 0)
            break;

        counter -= this->sums [i];
        //printf ("i = %d, count =  %d\n", i, counter);

    }

    if ((i >= this->size) || (i > right) || (this->sums [i] == 1488))
        return -2;

    for (; i <= right; ++i)
    {
        //printf ("so j %d\n", j);
        if (this->values [i] == 0)
            --counter;

        if (counter == 0)
            return i;
    }

    return -2;
}

template <typename Type>
void Fenwick_Tree <Type> :: update (int x, const Type& delta)
{
    if (this->values [x] == 0)
    {
        if (delta == 0)
        {
            return;
        }

        for (int i = x; i >= 0; i = this->and_func (i) - 1)
        {
            this->sums [i] -= 1;
        }
    }
    else
    {
        if (delta == 0)
        {
            for (int i = x; i >= 0; i = this->and_func (i) - 1)
            {
                this->sums [i] += 1;
            } 
        }
    }
    
    this->values [x] = delta;

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
    setbuf (stdin, NULL);
    //FILE* f = fopen ("input.txt", "r");
    FILE* f = stdin;

    int n = 0;
    fscanf (f, "%d", &n);

    int* array = new int [n];

    for (int i = 0; i < n; ++i)
        fscanf (f, "%d", &array [i]);

    Fenwick_Tree <int> tree (array, n);

    int m = 0;
    fscanf (f, "%d ", &m);

    char command = 0;
    int x1 = 0;
    int x2 = 0;
    int delta = 0;

    for (int i = 0; i < m; ++i)
    {
        fscanf (f, "%c ", &command);
        //printf ("%c\n", command);
        switch (command)
        {
            case 'u':
            fscanf (f, "%d %d ", &x1, &delta);
            //printf ("HELL %d %d\n", x1, delta);
            tree.update (x1 - 1, delta);
            break;

            case 's':
            fscanf (f, "%d %d %d ", &x1, &x2, &delta);
            printf ("%d ", 1 + tree.count_zeros (x1 - 1, x2 - 1, delta));
            break;
        }
    }

    fclose (f);

    delete [] array;

    return 0;
}