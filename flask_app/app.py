import json
import os

from flask import Flask, flash, redirect, render_template, request, send_file, session
from flask_limiter import Limiter
from flask_limiter.util import get_remote_address
from flask_session import Session
from flask_wtf.csrf import CSRFProtect
from werkzeug.security import check_password_hash, generate_password_hash

import config
from helpers import login_required, validate_json_struct, validate_json_values, validate_upload_file,flash_and_render
from schema import create_table, get_user_by_username, insert_user, insert_analysis, insert_function, get_history, get_analyses, get_analysis_by_id, get_functions_by_analysis_id

app = Flask(__name__)
csrf = CSRFProtect(app)

app.config.from_object(config.DevelopmentConfig if os.getenv("FLASK_ENV") == "development" else config.ProductionConfig)

Session(app)

# Initialize database tables
create_table()

limiter = Limiter(
    app=app,
    key_func=get_remote_address,
    default_limits=["200 per day", "50 per hour"],
    storage_uri="memory://"
)


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
def index():
    """
    Displays information about the Khronos profiling library
    """
    return render_template("index.html")


@app.route("/dashboard")
@login_required
def dashboard():
    """
    Displays user's analysis summary
    Shows statistics and list of all analyses for logged-in user
    """
    # Get all analyses for current user
    analyses = get_analyses(session["user_id"])
    if not analyses:
        flash("You don't have any analyses uploaded")
        return redirect("/upload")

    # Calculate stats
    nb_analyses = 0
    total_time_analyzed = 0

    for analysis in analyses:
        nb_analyses += 1
        total_time_analyzed += analysis["total_time"]

    avg_t_analysis = total_time_analyzed / nb_analyses

    return render_template("dashboard.html", analyses=analyses, nb_analyses=nb_analyses, total_time_analyzed=total_time_analyzed, avg_t_analysis=avg_t_analysis)


@app.route("/download")
def download():
    """
    Serve the Khronos library zip file
    """
    return send_file(
        'utils/khronoslib.zip',
        mimetype='application/zip',
        as_attachment=True,
        download_name=f'khronoslib.zip'
    )


@app.route("/history")
@login_required
def history():
    """
    Displays list of all user's analyses ordered by timestamp
    """
    analyses = get_history(session["user_id"])

    return render_template("history.html", analyses=analyses)


@app.route("/login", methods=["GET", "POST"])
@limiter.limit("5 per minute")
def login():
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        username = request.form.get("username")
        password = request.form.get("password")
        if not username:
            return flash_and_render("must provide username", "login.html")

        # Ensure password was submitted
        elif not password:
            return flash_and_render("must provide password", "login.html")

        # Query database for username
        rows = get_user_by_username(username)
        if not rows:
            return flash_and_render("invalid username and/or password", "login.html")

        # Ensure username exists and password is correct
        if not check_password_hash(
            rows["hash_password"], password
        ):
            return flash_and_render("invalid username and/or password", "login.html")

        # Remember which user has logged in
        session["user_id"] = rows["id"]

        # Redirect user to home page
        return redirect("/")

    return render_template("login.html")


@app.route("/logout")
@login_required
def logout():
    """
    Clear user session
    """
    session.clear()

    return redirect("/")


@app.route("/register", methods=["GET", "POST"])
@limiter.limit("5 per minute")
def register():
    if request.method == "POST":

        # Get form data
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Validate input
        if not username:
            return flash_and_render("Must provide username", "register.html")
        elif not password or not confirmation or password != confirmation:
            return flash_and_render("Must provide and confirm password", "register.html")

        # Try to create new user
        try:
            insert_user(username, generate_password_hash(password))
        except:
            return flash_and_render("Username already exists", "register.html")

        # Get newly created user
        rows = get_user_by_username(username)

        # Log user in automatically
        session["user_id"] = rows["id"]

        return redirect("/")

    return render_template("register.html")


@app.route("/report/<int:analysis_id>")
@login_required
def report(analysis_id):
    """
    Displays detailed analysis for a specific program
    Shows statistics, charts, and function-level profiling data
    """

    # Verify user owns this analysis
    analysis = get_analysis_by_id(session["user_id"], analysis_id)
    if not analysis:
        flash("Wrong analysis")
        return redirect("/dashboard")
    # Get all functions for this analysis
    functions = get_functions_by_analysis_id(analysis_id)

    # Extract metadata
    prgm_name = analysis["program_name"]
    prgm_time = analysis["total_time"]
    nb_functions = 0
    most_call = 0

    # Calculate stats
    for function in functions:
        nb_functions += 1
        if function["call_count"] > most_call:
            most_call = function["call_count"]

    # Convert functions to JSON for JavaScript charts
    functions_json = json.dumps([dict(function) for function in functions])

    return render_template("report.html", prgm_name=prgm_name, prgm_time=prgm_time, nb_functions=nb_functions, most_call=most_call, functions=functions, functions_json=functions_json)


@app.route("/upload", methods=["GET", "POST"])
@login_required
@limiter.limit("50 per day")
def upload():
    """
    Handle profiler data file uploads
    """
    if request.method == "POST":

        # Validate upload file
        valid, result = validate_upload_file(request)
        if not valid:
            return flash_and_render(result, "upload.html")

        try:
            # Read JSON directly from the file stream without saving to disk
            data = json.load(result.stream)

            # Validate if the JSON structure and values matches with expected format
            try:
                validate_json_struct(data)
                validate_json_values(data)
            except ValueError as e:
                return flash_and_render(f"Invalid JSON structure: {str(e)}", "upload.html")

            # Save to DB
            try:
                # Insert analysis metadata
                analysis_id = insert_analysis(
                    session["user_id"],
                    data["metadata"]["program_name"],
                    data["metadata"]["total_time"],
                    data["metadata"]["timestamp"]
                )

                # Insert each function's profiling data
                for function in data["functions"]:
                    insert_function(
                        analysis_id,
                        function["name"],
                        function["exec_time"],
                        function["call_count"],
                        function["avg_time"]
                    )

                # Redirect to report page for newly uploaded analysis
                return redirect(f"/report/{analysis_id}")

            except Exception as e:
                print(f"Database error in upload: {e}")
                return flash_and_render("Database error occurred while saving analysis", "upload.html")

        except json.JSONDecodeError:
            return flash_and_render("Invalid JSON file", "upload.html")
        except Exception as e:
            print(f"Unexpected error in upload: {e}")
            return flash_and_render("An error occurred processing your file", "upload.html")

    return render_template("upload.html")
