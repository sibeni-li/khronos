# C Profiler Library

A lightweight C profiling library that tracks function execution times and call counts, with JSON export for visualization.

## Features

- Dynamic memory management with automatic capacity expansion
- Track multiple function calls with accumulated timing
- JSON output for integration with visualization tools
- Comprehensive error handling
- Simple API

## Installation

1. Include `profiler.h` and `profiler.c` in your project
2. Compile with your project files

Using the Makefile:
```bash
make
```

## Usage

### Basic Example

```c
#include "profiler.h"

void my_function(void) {
    // Your code here
}

int main(void) {
    // Initialize the profiler
    if (profiler_init() != SUCCESS) {
        return 1;
    }
    
    // Profile a function
    profiler_start("my_function");
    my_function();
    profiler_stop("my_function");
    
    // Save results to JSON
    profiler_save_data();
    
    // Cleanup
    profiler_cleanup();
    
    return 0;
}
```

### API Reference

#### `int profiler_init(void)`
Initializes the profiler. Must be called before any other profiler functions.

**Returns:** `SUCCESS` or error code

#### `int profiler_start(char *fct_name)`
Starts profiling a function. If the function was already profiled, increments its call count.

**Parameters:**
- `fct_name`: Name of the function to profile

**Returns:** `SUCCESS` or error code

#### `int profiler_stop(char *fct_name)`
Stops profiling a function and records its execution time.

**Parameters:**
- `fct_name`: Name of the function to stop profiling

**Returns:** `SUCCESS` or error code

#### `int profiler_save_data(void)`
Saves profiling data to `profiler_data.json`.

**Returns:** `SUCCESS` or error code

#### `int profiler_cleanup(void)`
Frees all allocated memory. Should be called when done profiling.

**Returns:** `SUCCESS` or error code

### Error Codes

```c
SUCCESS                    // 0 - Operation successful
ERROR_MEMORY_ALLOCATION    // 1 - Memory allocation failed
ERROR_NOT_INITIALIZED      // 2 - Profiler not initialized
ERROR_NULL_VALUE           // 3 - NULL parameter provided
ERROR_ALREADY_RUNNING      // 4 - Function already being profiled
ERROR_NOT_RUNNING          // 5 - Function not currently running
ERROR_NOT_FOUND            // 6 - Function not found
ERROR_COULD_NOT_OPEN       // 7 - Could not open file
```

## JSON Output Format

```json
{
  "total_time": 0.123456,
  "functions": [
    {
      "name": "function_name",
      "exec_time": 0.045678,
      "call_count": 3
    }
  ]
}
```

## Building and Running Tests

```bash
# Compile
make

# Run
./test

# Clean build files
make clean
```

## Requirements

- C compiler with C11 support
- Standard C library

## License

This project was created as a CS50 final project.

## Future Enhancements

- Flask web application for data visualization
- Interactive charts and graphs
- Real-time profiling dashboard
