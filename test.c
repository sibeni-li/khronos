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
    if (global_profiler.initialized == false)
    {
        printf("Profiler not initialized\n");
        return;
    }

    if (global_profiler.count >= global_profiler.capacity)
    {
        profiler_extend_capacity();
    }

    if (fct_name == NULL)
    {
        printf("Function name is NULL\n");
        return;
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
        else if (functions[i].name != NULL && strcmp(functions[i].name, fct_name) == 0 && functions[i].is_running == true)
        {
            printf("Function %s is already running\n", fct_name);
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
    if (global_profiler.initialized == false)
    {
        printf("Profiler not initialized\n");
        return;
    }

    bool found = false;
    end = clock();

    if (fct_name == NULL)
    {
        printf("Function name is NULL\n");
        return;
    }

    for (int i = 0; i < global_profiler.count; i++)
    {
        if (functions[i].name != NULL && strcmp(functions[i].name, fct_name) == 0 && functions[i].is_running == true)
        {
            functions[i].is_running = false;
            cpu_duration = (double) (end - functions[i].start_time) / CLOCKS_PER_SEC;
            functions[i].exec_time += cpu_duration;
            total_time += cpu_duration;
            found = true;
            break;
        }
        else if (functions[i].name != NULL && strcmp(functions[i].name, fct_name) == 0 && functions[i].is_running == false)
        {
            printf("Function %s was not running\n", fct_name);
            return;
        }
    }

    if (found == false)
    {
        printf("Function %s not found in profiler\n", fct_name);
    }
}

void save_data(void)
{
    FILE *file = fopen("profiler_data.json", "w");
    if (!file)
    {
        printf("The file couldn't open");
        return;
    }

    fprintf(file, "{\n");
    fprintf(file, "  \"total_time\": %f,\n", total_time);
    fprintf(file, "  \"functions\": [\n");

    for (int i = 0; i < global_profiler.count; i++)
    {
        fprintf(file, "    {\"name\": \"%s\", \"exec_time\": %f, \"call_count\": %i}", functions[i].name, functions[i].exec_time, functions[i].call_count);
        if (i != global_profiler.count - 1)
        {
            fprintf(file, ",\n");
        }
        else
        {
            fprintf(file, "\n");
        }
    }
    fprintf(file, "  ]\n");
    fprintf(file, "}\n");

    fclose(file);
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
    functions = NULL;
    global_profiler = (profiler){0, 0, false};
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
