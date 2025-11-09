#include "../profiler.h"

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

    profiler_start("test_function_slow");
    test_function_slow();
    profiler_stop("test_function_slow");

    profiler_save_data("test1");
    profiler_cleanup();
    return 0;
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
