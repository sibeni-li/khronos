#include <stdio.h>
#include <time.h>


// Create a data structure for a function profile
typedef struct
{
    char *name;
    double exec_time;
} function;

// Create an array to store all the profiled functions (only one for now)
function functions[1];

// Prototypes
void test_function_speed(void);
void test_function_med(void);
void test_function_slow(void);

// Simpler program to analyse the execution time of a function
int main(void)
{
    functions[0].name = "test_function";
    clock_t start = clock();

    test_function();

    clock_t end = clock();
    double cpu_duration = (double) (end - start) / CLOCKS_PER_SEC;
    functions[0].exec_time = cpu_duration;

    printf("This program is executed in %f\n", cpu_duration);
    printf("The function's name is %s\n", functions[0].name);
    printf("The function execution time is %f\n", functions[0].exec_time);
}

// Function to test my program
void test_function_speed(void)
{
    int i = 0;
    while (i < 1000000)
    {
        i++;
    }
}

void test_function_med(void)
{
    int i = 0;
    while (i < 5000000)
    {
        i++;
    }
}

void test_function_slow(void)
{
    int i = 0;
    while (i < 10000000)
    {
        i++;
    }
}