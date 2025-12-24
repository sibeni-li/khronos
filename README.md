# Khronos
> **C Profiler + Web Visualization Platform**

A complete profiling solution for C programs. Profile your code with a lightweight C library, then upload and visualize performance data through an interactive web dashboard.

## Overview

Khronos consists of two integrated components:

1. **C Profiler Library** - Lightweight profiling for C programs with nanosecond precision
2. **Flask Web Application** - Upload, analyze, and visualize profiling data with interactive charts

## Features

### C Profiler Library
- Minimal overhead with nanosecond precision timing
- JSON export for easy data portability
- Simple 5-function API
- Robust error handling
- Uses `clock_gettime(CLOCK_MONOTONIC)` for accuracy

### Web Application
- Interactive charts (bar charts, pie charts)
- Analysis history and dashboard
- User authentication and session management
- CSRF protection and rate limiting
- Responsive Bootstrap UI

## Project Structure

```
khronos/
├── c_profiler/          # C profiling library
│   ├── profiler.h       # Header file
│   ├── profiler.c       # Implementation
│   ├── examples/        # Example programs
│   └── README.md        # C library documentation
│
├── flask_app/           # Web application
│   ├── app.py           # Main Flask application
│   ├── config.py        # Configuration
│   ├── helpers.py       # Helper functions
│   ├── schema.py        # Database operations
│   ├── templates/       # HTML templates
│   ├── static/          # CSS, JS, assets
│   ├── utils/           # Library zip folder
│   ├── .env             # Environment secrets variables
│   └── requirements.txt # Python dependencies
│
├── .gitignore           # Files ignored to git operations
└── README.md            # Web app documentation
```

## How It Works

```
┌─────────────────┐
│  Your C Program │
│   + profiler.h  │
└────────┬────────┘
         │
         │ 1. Profile functions
         ▼
┌─────────────────┐
│  profiler_data  │
│   .json file    │
└────────┬────────┘
         │
         │ 2. Upload to web app
         ▼
┌─────────────────┐
│  Flask Web App  │
│   + Database    │
└────────┬────────┘
         │
         │ 3. View reports
         ▼
┌─────────────────┐
│  Interactive    │
│  Dashboard      │
│  + Charts       │
└─────────────────┘
```

## Getting Started

### C Profiler Library

1. Navigate to `c_profiler/`
2. See [c_profiler/README.md](c_profiler/README.md) for detailed instructions
3. Quick start:
```c
#include "profiler.h"

int main(void) {
    profiler_init();
    
    profiler_start("my_function");
    my_function();
    profiler_stop("my_function");
    
    profiler_save_data("my_program");
    profiler_cleanup();
}
```

### Flask Web Application

Visit the deployed application at [https://khronos.sibeni.dev] to:
- Register an account
- Upload your JSON profiling data
- View interactive reports and dashboards


## 🔧 Tech Stack

### C Profiler
- **Language**: C (C99+)
- **Standard**: POSIX.1b-1993
- **Timing**: `clock_gettime(CLOCK_MONOTONIC)`

### Web Application
- **Backend**: Flask (Python)
- **Database**: SQLite
- **Frontend**: Bootstrap 5, Plotly.js
- **Security**: Flask-WTF (CSRF), Flask-Limiter (rate limiting)
- **Session**: Flask-Session (filesystem)

## ⚠️ Important Notes

- The C profiler is **single-threaded only** and not thread-safe
- Requires POSIX-compliant system (Linux, macOS, BSD)

## License
