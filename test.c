#include <stdio.h>
#include <time.h>

// Simpler program to analyse the execution time of do-while loop
int main(void)
{
    int i = 0;
    
    clock_t start = clock();

    while (i < 1000000)
    {
        i++;
    }

    clock_t end = clock();

    double cpu_duration = (double) (end - start) / CLOCKS_PER_SEC;

    printf("This program is executed in %f\n", cpu_duration);
}