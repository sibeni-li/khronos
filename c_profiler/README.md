# Khronos Profiler Library

A lightweight, easy-to-use profiling library for C programs. Measure execution time, track function calls, and generate performance data JSON file for analysis.

## Features

- **Minimal overhead** - Lightweight design with nanosecond precision
- **JSON export** - Easy integration with visualization tools
- **Simple API** - Just 5 functions to profile your code
- **Robust** - Proper error handling and memory management
- **Accurate timing** - Uses `clock_gettime(CLOCK_MONOTONIC)` for precise measurements

## Requirements

- **C Compiler** with C99 support or later
- **POSIX.1b-1993** or newer (for `clock_gettime`)
- **Operating System**: Linux, macOS, BSD, or other POSIX-compliant system

## ⚠️ Important Limitations

### **SINGLE-THREADED ONLY**
This profiler is **NOT thread-safe**. It uses a global profiler instance and is designed for single-threaded programs only.

**DO NOT use this library in multi-threaded applications.** Race conditions will occur and results will be incorrect or cause crashes.

### Other Limitations
- Cannot profile recursive functions (will return `ERROR_ALREADY_RUNNING`)
- Requires POSIX-compliant system (Linux, macOS, BSD, etc.)

## Installation

1. Download `profiler.h` and `profiler.c` from this repository 
2. Add them to your project directory
3. Include the header in your C files:

```c
#include "profiler.h"
```

## Quick Start

```c
#include "profiler.h"

int main(void)
{
    // Initialize profiler
    profiler_init();
    
    // Profile a function
    profiler_start("my_function");
    my_function();
    profiler_stop("my_function");
    
    // Save results to JSON
    profiler_save_data("my_program");
    
    // Cleanup
    profiler_cleanup();
    
    return 0;
}
```

This generates `profiler_data_my_program.json` with timing data.

## API Documentation

### `int profiler_init(void)`
Initialize the profiler. **Must be called before any other profiler functions.**

**Returns:**
- `SUCCESS` (0) on success
- `ERROR_MEMORY_ALLOCATION` if initialization fails

---

### `int profiler_start(char *fct_name)`
Start timing a function. Can be called multiple times for the same function (tracks call count).

**Parameters:**
- `fct_name` - Name of the function to profile (must not be NULL)

**Returns:**
- `SUCCESS` on success
- `ERROR_NOT_INITIALIZED` if profiler not initialized
- `ERROR_NULL_VALUE` if function name is NULL
- `ERROR_ALREADY_RUNNING` if function is already being timed
- `ERROR_MEMORY_ALLOCATION` if memory allocation fails
- `ERROR_CLOCK_FAILURE` if system clock access fails

---

### `int profiler_stop(char *fct_name)`
Stop timing a function and record its execution time.

**Parameters:**
- `fct_name` - Name of the function to stop profiling

**Returns:**
- `SUCCESS` on success
- `ERROR_NOT_INITIALIZED` if profiler not initialized
- `ERROR_NULL_VALUE` if function name is NULL
- `ERROR_NOT_RUNNING` if function is not currently being timed
- `ERROR_NOT_FOUND` if function was never started
- `ERROR_CLOCK_FAILURE` if system clock access fails

---

### `int profiler_save_data(char *program_name)`
Export profiling data to a JSON file named `profiler_data_<program_name>.json`.

**Parameters:**
- `program_name` - Name used for the output file

**Returns:**
- `SUCCESS` on success
- `ERROR_COULD_NOT_OPEN` if file cannot be created
- `ERROR_MEMORY_ALLOCATION` if string escaping fails

**Output format:**
```json
{
  "metadata": {
    "total_time": 0.006897,
    "program_name": "test",
    "timestamp": "2025-12-23 11:44:10"
  },
  "functions": [
    {
      "name": "function_name",
      "exec_time": 0.004315,
      "call_count": 1,
      "avg_time": 0.004315
    }
  ]
}
```

---

### `int profiler_cleanup(void)`
Free all profiler memory. **Must be called before program exit.**

**Returns:**
- `SUCCESS` on success

## Compilation

### Basic compilation:
```bash
cc -o my_program my_program.c profiler.c
```

### On older Linux systems, you may need to link the realtime library:
```bash
cc -o my_program my_program.c profiler.c -lrt
```

## Error Codes

All profiler functions return integer status codes:

| Code | Constant | Meaning |
|------|----------|---------|
| 0 | `SUCCESS` | Operation successful |
| 1 | `ERROR_MEMORY_ALLOCATION` | Memory allocation failed |
| 2 | `ERROR_NOT_INITIALIZED` | Profiler not initialized |
| 3 | `ERROR_NULL_VALUE` | NULL parameter provided |
| 4 | `ERROR_ALREADY_RUNNING` | Function already being profiled |
| 5 | `ERROR_NOT_RUNNING` | Function not currently running |
| 6 | `ERROR_NOT_FOUND` | Function not found |
| 7 | `ERROR_COULD_NOT_OPEN` | File operation failed |
| 8 | `ERROR_CLOCK_FAILURE` | System clock access failed |


## License

