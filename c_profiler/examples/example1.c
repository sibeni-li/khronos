#include "../profiler.h"

void example_function_speed(void);
void example_function_med(void);
void example_function_slow(void);

int main(void)
{
    profiler_init();

    profiler_start("example_function_speed");
    example_function_speed();
    profiler_stop("example_function_speed");

    profiler_start("example_function_med");
    example_function_med();
    profiler_stop("example_function_med");

    profiler_start("example_function_slow");
    example_function_slow();
    profiler_stop("example_function_slow");

    profiler_save_data("example1");
    profiler_cleanup();
    return 0;
}

void example_function_speed(void)
{
    int i = 0;
    while (i < 1000000)
    {
        i++;
    }
}

void example_function_med(void)
{
    int i = 0;
    while (i < 5000000)
    {
        i++;
    }
}

void example_function_slow(void)
{
    int i = 0;
    while (i < 10000000)
    {
        i++;
    }
}
