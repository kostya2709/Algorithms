#include <stdio.h>
#include <vector>
#include <deque>
#include <tuple>

void show_map (const std::vector <std::vector <bool>>& map, int lines, int columns);
unsigned long long count_fields (std::vector <std::vector <bool>>& map, unsigned long long start_x, unsigned long long start_y, unsigned long long max_left, unsigned long long max_right);

int main()
{
    FILE* f = stdin;
    //FILE* f = fopen ("input.txt", "r");

    unsigned long long lines     = 0;
    unsigned long long columns   = 0;
    unsigned long long start_x   = 0;
    unsigned long long start_y   = 0;
    unsigned long long max_left  = 0;
    unsigned long long max_right = 0;

    fscanf (f, "%llu %llu ", &lines, &columns);
    fscanf (f, "%llu %llu ", &start_x, &start_y);
    fscanf (f, "%llu %llu ", &max_left, &max_right);

    std::vector <std::vector <bool> > map (lines + 2, std::vector <bool> (columns + 2, 0));  

    for (unsigned long long i = 0; i < columns + 2; ++i)
    {
        map [0][i] = 1;
        map [lines + 1][i] = 1;
    }

    for (unsigned long long i = 0; i < lines + 2; ++i)
    {
        map [i][0] = 1;
        map [i][columns + 1] = 1;
    }

    char* char_map = new char [lines * (columns + 1) + 1];

    fread (char_map, 1, lines * (columns + 1) - 1, f);

    unsigned int pos = 0;

    for (unsigned long long i = 1; i <= lines; ++i)
    {
        for (unsigned long long j = 1; j <= columns; ++j)
        {
            if (char_map [pos] == '*')
                map[i][j] = 1;
            else
                map[i][j] = 0;
            ++pos;
        }
        ++pos;
    }

    fclose (f);

    //show_map (map, lines, columns);

    unsigned long long counter = count_fields (map, start_x, start_y, max_left, max_right);

    //show_map (map, lines, columns);

    printf ("%llu\n", counter);

}

void show_map (const std::vector <std::vector <bool>>& map, int lines, int columns)
{
    printf ("\n");
    for (int i = 0; i < lines + 2; ++i)
    {
        for (int j = 0; j < columns + 2; ++j)
            printf ("%d ", map[i][j]);
        printf ("\n");
    }
    printf ("\n");
}

unsigned long long count_fields (std::vector <std::vector <bool>>& map, unsigned long long start_i, unsigned long long start_j, unsigned long long max_left, unsigned long long max_right)
{

#define GO(new_i, new_j, command)                                       \
if (map[new_i][new_j] != locked) {                                      \
    command                                                             \
    map[new_i][new_j] = locked;                                         \
    ++counter;                                                          \
}

    const bool locked  = 1;

    std::deque <std::tuple <unsigned long long, unsigned long long, unsigned long long, unsigned long long, unsigned long long> > deque; 
    deque.push_back ({start_i, start_j, max_left, max_right, 0});
    map [start_i][start_j] = locked;

    auto the_tuple = deque.front();

    unsigned long long i = start_i;
    unsigned long long j = start_j;

    unsigned long long depth = 0;

    unsigned long long left  = max_left;
    unsigned long long right = max_right;

    unsigned long long counter = 1;

    while (!deque.empty())
    {
        the_tuple = deque.front();
        deque.pop_front();

        i     = std::get <0> (the_tuple);
        j     = std::get <1> (the_tuple);
        left  = std::get <2> (the_tuple);
        right = std::get <3> (the_tuple);
        depth = std::get <4> (the_tuple);

        //printf ("IN <%u, %u, %d, %d, %d>\n", i, j, left, right, depth); 


        if (right > 0)
            GO (i, j + 1, deque.push_back ({i, j + 1, left, right - 1, depth + 1});)
        
        if (left > 0)
            GO (i, j - 1, deque.push_back ({i, j - 1, left - 1, right, depth + 1});)
        
            GO (i + 1, j, deque.push_front ({i + 1, j, left, right, depth + 1});)
        
            GO (i - 1, j, deque.push_front ({i - 1, j, left, right, depth + 1});)

    }
        
    //printf ("i %d, j %d, pos %llu\n", i, j, counter);
return counter;

}
