from flask import redirect, session
from functools import wraps


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
    if "metadata" not in data or "functions" not in data:
        raise ValueError("Invalid JSON structure")

    if "total_time" not in data["metadata"] or "program_name" not in data["metadata"] or "timestamp" not in data["metadata"]:
        raise ValueError("Invalid JSON structure")

    if not isinstance(data["functions"], list):
        raise ValueError("Invalid JSON structure")

    for d in data["functions"]:
        if "name" not in d or "exec_time" not in d or "call_count" not in d or "avg_time" not in d:
            raise ValueError("Invalid JSON structure")
