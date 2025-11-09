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

// Main profiler structure containing all profiling data
typedef struct
{
    function_profile *functions;
    double cpu_duration;
    double total_time;
    int count;
    int capacity;
} profiler;

// Global profiler instance
profiler global_profiler = {NULL, 0, 0, 0, 0};

// Private function prototype
int profiler_extend_capacity(void);

int profiler_init(void)
{
    // Initialize profiler structure
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
    // Check if profiler is initialized
    if (global_profiler.functions == NULL)
    {
        return ERROR_NOT_INITIALIZED;
    }

    // Expand capacity if needed
    if (global_profiler.count >= global_profiler.capacity)
    {
        if (profiler_extend_capacity() != SUCCESS)
        {
            return ERROR_MEMORY_ALLOCATION;
        }
    }

    // Validate function name
    if (fct_name == NULL)
    {
        return ERROR_NULL_VALUE;
    }

    // Check if function already exists
    for (int i = 0; i < global_profiler.count; i++)
    {
        // Function exists and not running - restart timing
        if (global_profiler.functions[i].name != NULL && strcmp(global_profiler.functions[i].name, fct_name) == 0 &&
            global_profiler.functions[i].is_running == false)
        {
            global_profiler.functions[i].start_time = clock();
            global_profiler.functions[i].call_count += 1;
            global_profiler.functions[i].is_running = true;
            return SUCCESS;
        }
        // Function found but already running - error
        else if (global_profiler.functions[i].name != NULL && strcmp(global_profiler.functions[i].name, fct_name) == 0 &&
                 global_profiler.functions[i].is_running == true)
        {
            return ERROR_ALREADY_RUNNING;
        }
    }

    // New function - allocate memory for name
    global_profiler.functions[global_profiler.count].name  = malloc(strlen(fct_name) +1);
    if (global_profiler.functions[global_profiler.count].name  == NULL)
    {
        return ERROR_MEMORY_ALLOCATION;
    }
    // Copy function name
    strcpy(global_profiler.functions[global_profiler.count].name, fct_name);

    // Initialize new function entry
    global_profiler.functions[global_profiler.count].start_time = clock();
    global_profiler.functions[global_profiler.count].exec_time = 0.0;
    global_profiler.functions[global_profiler.count].call_count = 1;
    global_profiler.functions[global_profiler.count].is_running = true;
    global_profiler.count++;
    return SUCCESS;
}

int profiler_stop(char *fct_name)
{
    // Check if profiler is initialized
    if (global_profiler.functions == NULL)
    {
        return ERROR_NOT_INITIALIZED;
    }

    bool found = false;
    clock_t end = clock();

    // Validate function name
    if (fct_name == NULL)
    {
        return ERROR_NULL_VALUE;
    }

    // Search for the running function
    for (int i = 0; i < global_profiler.count; i++)
    {
        // Function found and running - stop it
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
        // Function found but not running
        else if (global_profiler.functions[i].name != NULL && strcmp(global_profiler.functions[i].name, fct_name) == 0 &&
                 global_profiler.functions[i].is_running == false)
        {
            return ERROR_NOT_RUNNING;
        }
    }

    // Function not found in profiler
    if (found == false)
    {
        return ERROR_NOT_FOUND;
    }

    return SUCCESS;
}

int profiler_save_data(char *program_name)
{
    // Create timestamp
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%S", t);

    // Create filename
    char filename[256];
    snprintf(filename, sizeof(filename), "profiler_data_%s.json", program_name);

    // Open file in writing mode
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        return ERROR_COULD_NOT_OPEN;
    }

    // Write JSON opening and metadata
    fprintf(file, "{\n");
    fprintf(file, "  \"metadata\": {\n");
    fprintf(file, "    \"total_time\": %f,\n", global_profiler.total_time);
    fprintf(file, "    \"program_name\": \"%s\",\n", program_name);
    fprintf(file, "    \"timestamp\": \"%s\"\n", timestamp);
    fprintf(file, "  },\n");
    fprintf(file, "  \"functions\": [\n");

    // Write each function's data
    for (int i = 0; i < global_profiler.count; i++)
    {
        double avg_time = global_profiler.functions[i].exec_time / global_profiler.functions[i].call_count;

        fprintf(file, "    {\"name\": \"%s\", \"exec_time\": %f, \"call_count\": %i, \"avg_time\": %f}",
                global_profiler.functions[i].name, global_profiler.functions[i].exec_time, global_profiler.functions[i].call_count, avg_time);
        // Add comma between elements, but not after last one
        if (i != global_profiler.count - 1)
        {
            fprintf(file, ",\n");
        }
        else
        {
            fprintf(file, "\n");
        }
    }
    // Close JSON structure
    fprintf(file, "  ]\n");
    fprintf(file, "}\n");

    // Close file
    fclose(file);

    return SUCCESS;
}

int profiler_extend_capacity(void)
{
    // Double the capacity
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
    // Free each function's name string
    for (int i = 0; i < global_profiler.count; i ++)
    {
        free(global_profiler.functions[i].name);
    }
    // Free functions array
    free(global_profiler.functions);
    // Reset profiler to initial state
    global_profiler = (profiler) {NULL, 0, 0, 0, 0};

    return SUCCESS;
}
