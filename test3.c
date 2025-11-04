#include <stdio.h>
#include "profiler.h"

void task1(void);
void task2(void);
void task3(void);
void task4(void);
void task5(void);
void task6(void);
void task7(void);
void task8(void);
void task9(void);
void task10(void);
void task11(void);
void task12(void);

int main(void) {
    profiler_init();
    
    profiler_start("task1"); task1(); profiler_stop("task1");
    profiler_start("task2"); task2(); profiler_stop("task2");
    profiler_start("task3"); task3(); profiler_stop("task3");
    profiler_start("task4"); task4(); profiler_stop("task4");
    profiler_start("task5"); task5(); profiler_stop("task5");
    profiler_start("task6"); task6(); profiler_stop("task6");
    profiler_start("task7"); task7(); profiler_stop("task7");
    profiler_start("task8"); task8(); profiler_stop("task8");
    profiler_start("task9"); task9(); profiler_stop("task9");
    profiler_start("task10"); task10(); profiler_stop("task10");
    profiler_start("task11"); task11(); profiler_stop("task11");
    profiler_start("task12"); task12(); profiler_stop("task12");
    
    profiler_save_data();
    profiler_cleanup();
    return 0;
}

void task1(void) { for (int i = 0; i < 1000000; i++); }
void task2(void) { for (int i = 0; i < 1000000; i++); }
void task3(void) { for (int i = 0; i < 1000000; i++); }
void task4(void) { for (int i = 0; i < 1000000; i++); }
void task5(void) { for (int i = 0; i < 1000000; i++); }
void task6(void) { for (int i = 0; i < 1000000; i++); }
void task7(void) { for (int i = 0; i < 1000000; i++); }
void task8(void) { for (int i = 0; i < 1000000; i++); }
void task9(void) { for (int i = 0; i < 1000000; i++); }
void task10(void) { for (int i = 0; i < 1000000; i++); }
void task11(void) { for (int i = 0; i < 1000000; i++); }
void task12(void) { for (int i = 0; i < 1000000; i++); }
