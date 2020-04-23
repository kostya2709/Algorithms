#include <stdio.h>
#include <iostream>
#include <cmath>

using std::pair;

template <typename Type>
class Segment_Tree
{
public:
    
    Segment_Tree (const Type* array, const int size);
    ~Segment_Tree ();

    const pair <Type, int>& find (int left, int right);
    void change (int left, int right, Type new_val);

    friend std::ostream& operator<< (std::ostream& os, const Segment_Tree <int> & obj);
    const Type& operator [] (int index) const;
    void graph (void);

private:

    pair <Type, int>* array;
    Type* source;
    Type* set;

    int elem_num;
    int size = 0;

    const pair <Type, int> create_tree (int left, int right, int index);
    const pair <Type, int>& tree_cond (const pair <Type, int>& first, const pair <Type, int>& second);
    const pair <Type, int>& find (int left, int right, int tree_left, int tree_right, int index);
    void change (int left, int right, Type new_val, int tree_left, int tree_right, int index);
    void recalculate (int index);
    void push (int index, int tree_left, int tree_right);
    void rec_graph (FILE* f, int tree_left, int tree_right, int index);
};

template <typename Type>
Segment_Tree <Type> :: Segment_Tree (const Type* array, const int size)
{
    this->array = new pair <Type, int> [4 * size];

    for (int i = 0; i < 4 * size; ++i)
        this->array[i].first = 0;

    this->source = (Type*)array;
    this->size = 0;
    this->elem_num = size;

    this->create_tree (0, size, 0);

    this->set = new Type [this->size + 1];

    for (int i = 0; i <= this->size; ++i)
        this->set[i] = 0;

    this->array [this->size + 1] = pair <Type, int> (0, 0);
}

template <typename Type>
const Type& Segment_Tree <Type> :: operator [] (int index) const
{
    if (index < 0)
    {
        printf ("The index is too low!\n");
        return this->array[0].first;
    }
    else if (index > this->size)
    {
        printf ("The index is out of range!\n");
        return this->array[0].first;
    }

    return this->array[index].first;
}


//template <typename Type>
std::ostream& operator<< (std::ostream& os, const Segment_Tree <int>& obj)
{
    for (int i = 0; i <= obj.size; ++i)
        os << i << ": (" << obj.array[i].first << ", " << obj.array[i].second << ");     ";
    os << "\n";

    return os;
}

template <typename Type>
const std::pair <Type, int> Segment_Tree <Type> :: create_tree (int left, int right, int index)
{
    if (right - left <= 1)
    {
        this->array[index].first = this->source[left];
        this->array[index].second = left;
        
        if (index > this->size)
            this->size = index;
        
        return this->array[index];
    }

    auto left_res = this->create_tree (left, (right + left) / 2, (index << 1) + 1);
    auto right_res = this->create_tree ((right + left) / 2, right, (index << 1) + 2);

    auto result = this->tree_cond (left_res, right_res);
    this->array [index] = result;
    
    
    if (index > this->size)
        this->size = index;

    return result;
}
/*
template <typename Type>
const Type& Segment_Tree <Type> :: tree_cond (const Type& first, const Type& second)
{
    if (first < second)
        return first;
    else
        return second;   
}
*/

/*
template <typename Type>
const std::pair <Type, int>& Segment_Tree <Type> :: tree_cond (const std::pair <Type, int>& first, const std::pair <Type, int>& second)
{
    if (first.first == second.first)
    {
        if (first.second < second.second)
            return first;
        else
            return second;
    }
        
    if (second.first > first.first)
        return second;
    else
        return first;   
}
*/

template <typename Type>
const std::pair <Type, int>& Segment_Tree <Type> :: tree_cond (const std::pair <Type, int>& first, const std::pair <Type, int>& second)
{
    auto result = new pair <Type, int> (first.first + second.first, 0);
    return *result;
}

template <typename Type>
const pair <Type, int>& Segment_Tree <Type> :: find (int left, int right, int tree_left, int tree_right, int index)
{
    printf ("left %d, right %d\n", tree_left, tree_right);
    if (tree_left >= left && tree_right - 1 <= right)
    {
        printf ("put %d, %d\n", this->array [index].first, this->array [index].second);
        return this->array [index];
    }

    if (tree_right <= left || tree_left > right)
    {
        printf ("PUT %d, %d\n", this->array [this->size + 1].first, this->array [index].second);
        return this->array[this->size + 1];
    }

    this->push (index, tree_left, tree_right);

    const pair <Type, int>& left_el = this->find (left, right, tree_left, (tree_left + tree_right) / 2, index * 2 + 1);
    const pair <Type, int>& right_el = this->find (left, right, (tree_left + tree_right) / 2, tree_right, index * 2 + 2);

    const pair <Type, int>& result = this->tree_cond (left_el, right_el);
    printf ("REAL PUT %d, %d\n", result.first, result.second);

    return result;
}

template <typename Type>
void Segment_Tree <Type> :: push (int index, int tree_left, int tree_right)
{
    if (tree_right - tree_left <= 1)
        return;

    if (this->set [index] == 0)
        return;

    Type new_val = this->set [index];
    int left_index = 2 * index + 1;

    this->array [left_index].first = new_val;
    this->array [left_index + 1].second = new_val;

    this->set [left_index] = new_val;
    this->set [left_index + 1] = new_val;
    this->set [index] = 0;
}

template <typename Type>
const pair <Type, int>& Segment_Tree <Type> :: find (int left, int right)
{
    const pair <Type, int>& result = this->find (left - 1, right - 1, 0, this->elem_num, 0);

    return result;
}

template <typename Type>
void Segment_Tree <Type> :: change (int left, int right, Type new_val, int tree_left, int tree_right, int index)
{
    if (tree_left >= left && tree_right - 1 <= right)
    {
        this->set [index] = new_val;
        this->array [index].first += new_val;
        return;
    }

    if (tree_right <= left || tree_left > right)
        return;

    this->push (index, tree_left, tree_right);

    this->change (left, right, new_val, tree_left, (tree_left + tree_right) / 2, index * 2 + 1);
    this->change (left, right, new_val, (tree_left + tree_right) / 2, tree_right, index * 2 + 2);
    
    this->recalculate (index);

    return;

}

template <typename Type>
void Segment_Tree <Type> :: recalculate (int index)
{
    this->array[index] = this->tree_cond (this->array [2 * index + 1], this->array [2 * index + 2]);
}

template <typename Type>
void Segment_Tree <Type> :: change (int left, int right, Type new_val)
{
    this->change (left - 1, right -1, new_val, 0, this->elem_num, 0);
}

template <typename Type>
Segment_Tree <Type> :: ~Segment_Tree ()
{
    delete [] this->array;
    delete [] this->set;
}

template <typename Type>
void Segment_Tree <Type> :: rec_graph (FILE* f, int tree_left, int tree_right, int index)
{
    if (tree_right - tree_left == 1)
    {
        fprintf (f, "\"node%d\" [ fillcolor = \"lightblue\", label = \"{index = %d|value = %d|real_index = %d}\" ];\n", index, index, this->array [index].first, tree_left);
        --tree_right;
    }
    else if (tree_right - tree_left == 0)
    {
        fprintf (f, "\"node%d\" [ fillcolor = \"lightgray\", label = \"{index = %d|value = %d|Imaginary node}\" ];\n", index, index, this->array [index].first);
    }
    else
    {
        fprintf (f, "\"node%d\" [ label = \"{index = %d|value = %d|range [%d, %d)}\" ];\n", index, index, this->array [index].first, tree_left, tree_right);
    }

    if (index * 2 + 1 < this->size)
    {
        this->rec_graph (f, tree_left, (tree_left + tree_right) / 2, 2 * index + 1);
        fprintf (f, "edge [color = \"green\"];\n");
        fprintf (f, "\"node%d\"->\"node%d\";\n", index, 2 * index + 1);
    }

    if (index * 2 + 2 <= this->size)
    {
        this->rec_graph (f, (tree_left + tree_right) / 2, tree_right, 2 * index + 2);
        fprintf (f, "edge [color = \"black\"];\n");
        fprintf (f, "\"node%d\"->\"node%d\";\n", index, 2 * index + 2);
    }
    
}

template <typename Type>
void Segment_Tree <Type> :: graph (void)
{
    char file_name[] = "graph.gv";
    char pic_name[] = "graph.ps";

    FILE* f = fopen (file_name, "w");

    fprintf (f, "digraph {");
    fprintf (f, "node [shape=\"record\", style=\"filled\", fillcolor=\"green\"];\n\n");
    fprintf (f, "dpi=\"300\";\n");

    this->rec_graph (f, 0, this->elem_num, 0);

    fprintf (f, "}");
    fclose (f);
}

int main()
{
    FILE* f = fopen ("index-max.in", "r");
    //FILE* f = stdin;

    int N = 0;
    fscanf (f, "%d", &N);

    int* array = new int [N + 1];
    for (int i = 0; i < N + 1; ++i)
        array [i] = 0;

    Segment_Tree <int> tree (array, N);

    int command = 0;
    int x = 0;
    int y = 0;
    int z = 0;
    int x1 = 0;
    int y1 = 0;
    int z1 = 0;
    int delta = 0;
    int leave = 0;

    pair <int, int> result;

    while (!leave)
    {
        fscanf (f, "%d", &command);

        switch (command)
        {
            case 1:
                fscanf (f, "%d %d", &x, &delta);
                tree.change (x, x, delta);
            break;

            case 2:
                fscanf (f, "%d %d", &x, &x1);
                result = tree.find (x, x1);
                printf ("returned %d", result.first);
            break;

            case 3:
                leave = 1;
            break;
        }
        
        //std::cout << tree;
    }
        tree.graph();

    fclose (f);

    return 0;
}