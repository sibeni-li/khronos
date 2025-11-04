#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct
{
    int count;
    int capacity;
    bool initialized;
} profiler;

// Data structure for a function profile
typedef struct
{
    char *name;
    clock_t start_time;
    double exec_time;
    int call_count;
    bool is_running;
} function_profile;

profiler global_profiler = {0, 0, false};
function_profile *functions = NULL;

// Variables
clock_t end;
double cpu_duration;
double total_time;

// Prototypes
void profiler_init(void);
void profiler_start(char *fct_name);
void profiler_stop(char *fct_name);
void save_data(void);
void profiler_extend_capacity(void);
void profiler_cleanup(void);

void test_function_speed(void);
void test_function_med(void);
void test_function_slow(void);

int main(void)
{   
    profiler_init();
    
    profiler_start("test_function_speed");
    test_function_speed();
    profiler_stop("test_function_speed");
    
    profiler_start("test_function_med");
    test_function_med();
    profiler_stop("test_function_med");

    profiler_start("test_function_med");
    test_function_med();
    profiler_stop("test_function_med");

    profiler_start("test_function_speed");
    test_function_speed();
    profiler_stop("test_function_speed");
    
    profiler_start("test_function_slow");
    test_function_slow();
    profiler_stop("test_function_slow");
    
    save_data();
    profiler_cleanup();
    return 0;
}

void profiler_init(void)
{
    global_profiler.count = 0;
    global_profiler.capacity = 10;
    global_profiler.initialized = true;

    functions = malloc(global_profiler.capacity * sizeof(function_profile));
    if (functions == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }

}

void profiler_start(char *fct_name)
{
    if (global_profiler.count >= global_profiler.capacity)
    {
        profiler_extend_capacity();
    }

    for (int i = 0; i < global_profiler.count; i++)
    {
        if (functions[i].name != NULL && strcmp(functions[i].name, fct_name) == 0 && functions[i].is_running == false)
        {
            functions[i].start_time = clock();
            functions[i].call_count += 1;
            functions[i].is_running = true;
            return;
        }
    }
    
    functions[global_profiler.count].name = fct_name;
    functions[global_profiler.count].start_time = clock();
    functions[global_profiler.count].exec_time = 0.0;
    functions[global_profiler.count].call_count = 1;
    functions[global_profiler.count].is_running = true;
    global_profiler.count ++;

}

void profiler_stop(char *fct_name)
{
    end = clock();
    for (int i = 0; i < global_profiler.count; i++)
    {
        if (functions[i].name != NULL && strcmp(functions[i].name, fct_name) == 0 && functions[i].is_running == true)
        {
            functions[i].is_running = false;
            cpu_duration = (double) (end - functions[i].start_time) / CLOCKS_PER_SEC;
            functions[i].exec_time += cpu_duration;
            total_time += cpu_duration;
            break;
        }
    }
}

void save_data(void)
{
    for (int i = 0; i < global_profiler.count; i++)
    {
        printf("The function's name is %s\n", functions[i].name);
        printf("The function execution time is %f\n", functions[i].exec_time);
        printf("The function was called %d times\n", functions[i].call_count);
    }
    printf("This program was executed in %f seconds\n", total_time);
}

void profiler_extend_capacity(void)
{
    global_profiler.capacity *= 2;
    functions = realloc(functions, global_profiler.capacity * sizeof(function_profile));
    if (functions == NULL)
    {
        printf("Memory reallocation failed\n");
        exit(1);
    }
}

void profiler_cleanup(void)
{
    free(functions);
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