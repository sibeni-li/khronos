#ifndef PROFILER_H
#define PROFILER_H

#define SUCCESS 0
#define ERROR_MEMORY_ALLOCATION 1
#define ERROR_NOT_INITIALIZED 2
#define ERROR_NULL_VALUE 3
#define ERROR_ALREADY_RUNNING 4
#define ERROR_NOT_RUNNING 5
#define ERROR_NOT_FOUND 6
#define ERROR_COULD_NOT_OPEN 7

// Public prototypes
int profiler_init(void);
int profiler_start(char *fct_name);
int profiler_stop(char *fct_name);
int profiler_save_data(char *program_name);
int profiler_cleanup(void);

#endif
