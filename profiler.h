#ifndef PROFILER_H
#define PROFILER_H

// Public prototypes
void profiler_init(void);
void profiler_start(char *fct_name);
void profiler_stop(char *fct_name);
void profiler_save_data(void);
void profiler_cleanup(void);

#endif