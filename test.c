#include <stdio.h>
#include <time.h>

// Data structure for a function profile
typedef struct
{
    char *name;
    double exec_time;
} function;

// Array to store all the profiled functions
function functions[3];

// Variables
int count = 0;
clock_t start, end;
double cpu_duration;
double total_time;

// Prototypes
void start_profiler(char *fct_name);
void stop_profiler(void);
void save_data(void);

void test_function_speed(void);
void test_function_med(void);
void test_function_slow(void);

int main(void)
{   
    start_profiler("test_function_speed");
    test_function_speed();
    stop_profiler();

    start_profiler("test_function_med");
    test_function_med();
    stop_profiler();

    start_profiler("test_function_slow");
    test_function_slow();
    stop_profiler();

    save_data();   
}

void start_profiler(char *fct_name)
{
    functions[count].name = fct_name;
    start = clock();
}

void stop_profiler(void)
{
    end = clock();
    cpu_duration = (double) (end - start) / CLOCKS_PER_SEC;
    functions[count].exec_time = cpu_duration;
    count ++;
    total_time += cpu_duration;
}

void save_data(void)
{
    for (int i = 0; i < count; i++)
    {
        printf("The function's name is %s\n", functions[i].name);
        printf("The function execution time is %f\n", functions[i].exec_time);   
    }
    printf("This program was executed in %f seconds\n", total_time);
}

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