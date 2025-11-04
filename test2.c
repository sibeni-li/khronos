#include <stdio.h>
#include "profiler.h"

void repeated_task(void);

int main(void) {
    profiler_init();
    
    for (int i = 0; i < 5; i++) {
        profiler_start("repeated_task");
        repeated_task();
        profiler_stop("repeated_task");
    }
    
    save_data();
    profiler_cleanup();
    return 0;
}

void repeated_task(void) {
    for (int i = 0; i < 2000000; i++);
}
