#include <stdio.h>
#include <vector>

using std::vector;
using std::pair;

int main()
{
    //FILE* f = fopen ("input.txt", "r");
    FILE* f = stdin;

    int n = 0;
    int m = 0;

    fscanf (f, "%d %d ", &n, &m);

    vector <vector <int> > array (n + 1, (vector <int> (m + 1, 0)));
    pair <int, int> stones [n + 1];

    for (int i = 1; i <= n; ++i)
        fscanf (f, "%d ", &stones[i].first);

    for (int i = 1; i <= n; ++i)
        fscanf (f, "%d", &stones[i].second);

    int change = 0;
    int no_change = 0;

    int i = 1;
    int j = 1;

    for ( ; i <= n; ++i)
        for ( j = 1; j <= m; ++j)
        {   
            change = 0;
            if (j >= stones[i].first)
                change = array [i - 1][j - stones[i].first] + stones[i].second;
            
            no_change = array[i - 1][j];

            if (change > no_change)
                array [i][j] = change;
            else  
                array [i][j] = no_change;
        }
//printf ("FINE\n");
    vector <int> answer;
    answer.reserve (m);

    j = m;
    for ( i = n; i > 0; --i)
    {
        //printf ("%d %d\n", i, j);
        change = -1;
        if (j >= stones[i].first)
            change = array [i - 1][j - stones[i].first]  + stones[i].second;
            
        no_change = array[i - 1][j];
        //printf ("chng %d, nochng %d\n", change, no_change);


        if (change >= no_change)
        {
            j -= stones[i].first;
            answer.push_back (i);
        }
    }

    int counter = 0;
    for (int i = answer.size() - 1; i >= 0; --i)
        if (answer[i])
            ++counter;
    printf ("%d\n", counter);

    for (int i = answer.size() - 1; i >= 0; --i)
        if (answer[i])
            printf ("%d ", answer[i]);

/*
    for (int i = 0; i < n + 1; ++i)
    {
        for (int j = 1; j < m + 1; ++j)
            printf ("%d ", array[i][j]);
        printf ("\n");
    }

    printf ("%d\n", array [n][m]);
*/
    fclose (f);

    return 0;
}