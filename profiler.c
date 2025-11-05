#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "profiler.h"

// Data structure for the profiler
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

// Global variables
profiler global_profiler = {0, 0, false};
function_profile *functions = NULL;
clock_t end;
double cpu_duration;
double total_time;

// Private prototype
int profiler_extend_capacity(void);

int profiler_init(void)
{
    global_profiler.count = 0;
    global_profiler.capacity = 10;
    global_profiler.initialized = true;

    functions = malloc(global_profiler.capacity * sizeof(function_profile));
    if (functions == NULL)
    {
        return ERROR_MEMORY_ALLOCATION;
    }
    return SUCCESS;
}

int profiler_start(char *fct_name)
{
    if (global_profiler.initialized == false)
    {
        return ERROR_NOT_INITIALIZED;
    }

    if (global_profiler.count >= global_profiler.capacity)
    {
        profiler_extend_capacity();
        if (profiler_extend_capacity != SUCCESS)
        {
            return ERROR_MEMORY_ALLOCATION;
        }
    }

    if (fct_name == NULL)
    {
        return ERROR_NULL_VALUE;
    }

    for (int i = 0; i < global_profiler.count; i++)
    {
        if (functions[i].name != NULL && strcmp(functions[i].name, fct_name) == 0 &&
            functions[i].is_running == false)
        {
            functions[i].start_time = clock();
            functions[i].call_count += 1;
            functions[i].is_running = true;
            return SUCCESS;
        }
        else if (functions[i].name != NULL && strcmp(functions[i].name, fct_name) == 0 &&
                 functions[i].is_running == true)
        {
            return ERROR_ALREADY_RUNNING;
        }
    }

    functions[global_profiler.count].name = strdup(fct_name);

    if (functions[global_profiler.count].name == NULL)
    {
        return ERROR_MEMORY_ALLOCATION;
    }

    functions[global_profiler.count].start_time = clock();
    functions[global_profiler.count].exec_time = 0.0;
    functions[global_profiler.count].call_count = 1;
    functions[global_profiler.count].is_running = true;
    global_profiler.count++;
    return SUCCESS;
}

int profiler_stop(char *fct_name)
{
    if (global_profiler.initialized == false)
    {
        return ERROR_NOT_INITIALIZED;
    }

    bool found = false;
    end = clock();

    if (fct_name == NULL)
    {
        return ERROR_NULL_VALUE;
    }

    for (int i = 0; i < global_profiler.count; i++)
    {
        if (functions[i].name != NULL && strcmp(functions[i].name, fct_name) == 0 &&
            functions[i].is_running == true)
        {
            functions[i].is_running = false;
            cpu_duration = (double) (end - functions[i].start_time) / CLOCKS_PER_SEC;
            functions[i].exec_time += cpu_duration;
            total_time += cpu_duration;
            found = true;
            break;
        }
        else if (functions[i].name != NULL && strcmp(functions[i].name, fct_name) == 0 &&
                 functions[i].is_running == false)
        {
            return ERROR_NOT_RUNNING;
        }
    }

    if (found == false)
    {
        return ERROR_NOT_FOUND;
    }

    return SUCCESS;
}

int profiler_save_data(void)
{
    FILE *file = fopen("profiler_data.json", "w");
    if (!file)
    {
        return ERROR_COULD_NOT_OPEN;
    }

    fprintf(file, "{\n");
    fprintf(file, "  \"total_time\": %f,\n", total_time);
    fprintf(file, "  \"functions\": [\n");

    for (int i = 0; i < global_profiler.count; i++)
    {
        fprintf(file, "    {\"name\": \"%s\", \"exec_time\": %f, \"call_count\": %i}",
                functions[i].name, functions[i].exec_time, functions[i].call_count);
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

    return SUCCESS;
}

int profiler_extend_capacity(void)
{
    global_profiler.capacity *= 2;
    functions = realloc(functions, global_profiler.capacity * sizeof(function_profile));
    if (functions == NULL)
    {
        return ERROR_MEMORY_ALLOCATION;
    }

    return SUCCESS;
}

int profiler_cleanup(void)
{
    for (int i = 0; i < global_profiler.count; i ++)
    {
        free(functions[i].name);
    }
    free(functions);
    functions = NULL;
    global_profiler = (profiler) {0, 0, false};

    return SUCCESS;
}
