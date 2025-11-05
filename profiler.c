#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "profiler.h"

// Data structure for a function profile
typedef struct
{
    char *name;
    clock_t start_time;
    double exec_time;
    int call_count;
    bool is_running;
} function_profile;

// Data structure for the profiler
typedef struct
{
    function_profile *functions;
    double cpu_duration;
    double total_time;
    int count;
    int capacity;
} profiler;

// Global variables
profiler global_profiler = {NULL, 0, 0, 0, 0};

// Private prototype
int profiler_extend_capacity(void);

int profiler_init(void)
{
    global_profiler.count = 0;
    global_profiler.capacity = 10;

    global_profiler.functions = malloc(global_profiler.capacity * sizeof(function_profile));
    if (global_profiler.functions == NULL)
    {
        return ERROR_MEMORY_ALLOCATION;
    }
    return SUCCESS;
}

int profiler_start(char *fct_name)
{
    if (global_profiler.functions == NULL)
    {
        return ERROR_NOT_INITIALIZED;
    }

    if (global_profiler.count >= global_profiler.capacity)
    {
        profiler_extend_capacity();
        if (profiler_extend_capacity() != SUCCESS)
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
        if (global_profiler.functions[i].name != NULL && strcmp(global_profiler.functions[i].name, fct_name) == 0 &&
            global_profiler.functions[i].is_running == false)
        {
            global_profiler.functions[i].start_time = clock();
            global_profiler.functions[i].call_count += 1;
            global_profiler.functions[i].is_running = true;
            return SUCCESS;
        }
        else if (global_profiler.functions[i].name != NULL && strcmp(global_profiler.functions[i].name, fct_name) == 0 &&
                 global_profiler.functions[i].is_running == true)
        {
            return ERROR_ALREADY_RUNNING;
        }
    }

    global_profiler.functions[global_profiler.count].name  = malloc(strlen(fct_name) +1);
    if (global_profiler.functions[global_profiler.count].name  == NULL)
    {
        return ERROR_MEMORY_ALLOCATION;
    }
    strcpy(global_profiler.functions[global_profiler.count].name, fct_name);

    global_profiler.functions[global_profiler.count].start_time = clock();
    global_profiler.functions[global_profiler.count].exec_time = 0.0;
    global_profiler.functions[global_profiler.count].call_count = 1;
    global_profiler.functions[global_profiler.count].is_running = true;
    global_profiler.count++;
    return SUCCESS;
}

int profiler_stop(char *fct_name)
{
    if (global_profiler.functions == NULL)
    {
        return ERROR_NOT_INITIALIZED;
    }

    bool found = false;
    clock_t end = clock();

    if (fct_name == NULL)
    {
        return ERROR_NULL_VALUE;
    }

    for (int i = 0; i < global_profiler.count; i++)
    {
        if (global_profiler.functions[i].name != NULL && strcmp(global_profiler.functions[i].name, fct_name) == 0 &&
            global_profiler.functions[i].is_running == true)
        {
            global_profiler.functions[i].is_running = false;
            global_profiler.cpu_duration = (double) (end - global_profiler.functions[i].start_time) / CLOCKS_PER_SEC;
            global_profiler.functions[i].exec_time += global_profiler.cpu_duration;
            global_profiler.total_time += global_profiler.cpu_duration;
            found = true;
            break;
        }
        else if (global_profiler.functions[i].name != NULL && strcmp(global_profiler.functions[i].name, fct_name) == 0 &&
                 global_profiler.functions[i].is_running == false)
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
    fprintf(file, "  \"total_time\": %f,\n", global_profiler.total_time);
    fprintf(file, "  \"functions\": [\n");

    for (int i = 0; i < global_profiler.count; i++)
    {
        fprintf(file, "    {\"name\": \"%s\", \"exec_time\": %f, \"call_count\": %i}",
                global_profiler.functions[i].name, global_profiler.functions[i].exec_time, global_profiler.functions[i].call_count);
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
    global_profiler.functions = realloc(global_profiler.functions, global_profiler.capacity * sizeof(function_profile));
    if (global_profiler.functions == NULL)
    {
        return ERROR_MEMORY_ALLOCATION;
    }

    return SUCCESS;
}

int profiler_cleanup(void)
{
    for (int i = 0; i < global_profiler.count; i ++)
    {
        free(global_profiler.functions[i].name);
    }
    free(global_profiler.functions);
    global_profiler.functions = NULL;
    global_profiler = (profiler) {NULL, 0, 0, 0, 0};

    return SUCCESS;
}
