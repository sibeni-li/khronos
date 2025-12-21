from flask import redirect, session, render_template, flash
from functools import wraps
import sqlite3


def login_required(f):
    """
    Decorate routes to require login.

    https://flask.palletsprojects.com/en/latest/patterns/viewdecorators/
    """

    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect("/login")
        return f(*args, **kwargs)

    return decorated_function


def validate_json_struct(data):
    """
    Validate that uploaded JSON matches expected profiler data structure
    Checks for required metadata and function fields
    """
    # Check top level structure
    if "metadata" not in data or "functions" not in data:
        raise ValueError("Invalid JSON structure")

    # Check metadata fields
    if "total_time" not in data["metadata"] or "program_name" not in data["metadata"] or "timestamp" not in data["metadata"]:
        raise ValueError("Invalid JSON structure")

    # Check if functions is a list
    if not isinstance(data["functions"], list):
        raise ValueError("Invalid JSON structure")

    # Check if each function has required fields
    for d in data["functions"]:
        if "name" not in d or "exec_time" not in d or "call_count" not in d or "avg_time" not in d:
            raise ValueError("Invalid JSON structure")

def validate_json_values(data):
    """
    Validate types and ranges of JSON profiler data values.
    """
    meta = data["metadata"]
    
    # Validate metadata
    if not isinstance(meta["program_name"], str) or not meta["program_name"].strip():
        raise ValueError("Invalid JSON value")
    if not isinstance(meta["total_time"], (int, float)) or meta["total_time"] < 0:
        raise ValueError("Invalid JSON value")
    if not isinstance(meta["timestamp"], str):
        raise ValueError("Invalid JSON value")
    
    # Validate functions
    for func in data["functions"]:
        if not isinstance(func["name"], str) or not func["name"].strip():
            raise ValueError("Invalid JSON value")
        if not isinstance(func["exec_time"], (int, float)) or func["exec_time"] < 0:
            raise ValueError("Invalid JSON value")
        if not isinstance(func["call_count"], int) or func["call_count"] < 1:
            raise ValueError("Invalid JSON value")
        if not isinstance(func["avg_time"], (int, float)) or func["avg_time"] < 0:
            raise ValueError("Invalid JSON value")

def validate_upload_file(request):
    """
    Validate uploaded file from request
    Checks file exists and is JSON format
    """
    # Check file was in request
    if "file" not in request.files:
        return False, "Provide a file"

    req = request.files["file"]

    # Check file is not empty
    if not req or req.filename == "":
        return False, "Provide a file"

    # Check if file extension is JSON
    if not req.filename.endswith(".json"):
        return False, "Provide a json file"

    return True, req


def flash_and_render(message, template):
    """
    Flash a message and render a template.
    """
    flash(message)
    return render_template(template)


def db_execute(operation):
    """
    Decorator to handle database connection and error handling.
    """
    @wraps(operation)
    def wrapper(*args, **kwargs):
        conn = None
        try:
            # Create database connection
            conn = sqlite3.connect('profiles.db')
            # Execute operation with connection
            return operation(conn, *args, **kwargs)
        except sqlite3.IntegrityError:
            # Raise integrity errors
            raise
        except sqlite3.Error as e:
            print(f"Database error in {operation.__name__}: {e}")
            raise
        finally:
            # Always close connection
            if conn:
                conn.close()
    return wrapper
