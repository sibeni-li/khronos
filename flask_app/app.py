import json
import os

from flask import Flask, flash, redirect, render_template, request, send_file, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
from werkzeug.utils import secure_filename


from helpers import login_required, validate_json_struct
from schema import create_table, get_user_by_username, insert_user, insert_analysis, insert_function, get_history, get_analyses, get_analysis_by_id, get_functions_by_analysis_id

app = Flask(__name__)

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

create_table()


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
def index():
    return render_template("index.html")


@app.route("/dashboard")
@login_required
def dashboard():
    analyses = get_analyses(session["user_id"])
    if not analyses:
        flash("You have any analyses uploaded")
        return redirect("/upload")

    nb_analyses = 0
    total_time_analyzed = 0

    for analysis in analyses:
        nb_analyses += 1
        total_time_analyzed += analysis["total_time"]

    avg_t_analysis = total_time_analyzed / nb_analyses

    return render_template("dashboard.html", analyses=analyses, nb_analyses=nb_analyses, total_time_analyzed=total_time_analyzed, avg_t_analysis=avg_t_analysis)


@app.route("/download")
def download():
    return send_file(
        'utils/khronoslib.zip',
        mimetype='application/zip',
        as_attachment=True,
        download_name=f'khronoslib.zip'
    )


@app.route("/history")
@login_required
def history():
    analyses = get_history(session["user_id"])

    return render_template("history.html", analyses=analyses)


@app.route("/login", methods=["GET", "POST"])
def login():
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        username = request.form.get("username")
        password = request.form.get("password")
        if not username:
            flash("must provide username")
            return render_template("login.html")

        # Ensure password was submitted
        elif not password:
            flash("must provide password")
            return render_template("login.html")

        # Query database for username
        rows = get_user_by_username(username)
        if not rows:
            flash("invalid username and/or password")
            return render_template("login.html")

        # Ensure username exists and password is correct
        if not check_password_hash(
            rows["hash_password"], password
        ):
            flash("invalid username and/or password")
            return render_template("login.html")

        # Remember which user has logged in
        session["user_id"] = rows["id"]

        # Redirect user to home page
        return redirect("/")

    return render_template("login.html")


@app.route("/logout")
@login_required
def logout():
    session.clear()

    return redirect("/")


@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "POST":

        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        if not username:
            flash("Must provide username")
            return render_template("register.html")
        elif not password or not confirmation or password != confirmation:
            flash("Must provide and confirm password")
            return render_template("register.html")

        try:
            insert_user(username, generate_password_hash(password))
        except:
            flash("Username already exists")
            return render_template("register.html")

        rows = get_user_by_username(username)

        session["user_id"] = rows["id"]

        return redirect("/")

    return render_template("register.html")


@app.route("/report/<int:analysis_id>")
@login_required
def report(analysis_id):

    analysis = get_analysis_by_id(session["user_id"], analysis_id)
    if not analysis:
        flash("Wrong analysis")
        return redirect("/dashboard")
    functions = get_functions_by_analysis_id(analysis_id)

    prgm_name = analysis["program_name"]
    prgm_time = analysis["total_time"]
    nb_functions = 0
    most_call = 0

    for function in functions:
        nb_functions += 1
        if function["call_count"] > most_call:
            most_call = function["call_count"]
        print(function["name"])

    functions_json = json.dumps([dict(function) for function in functions])

    return render_template("report.html", prgm_name=prgm_name, prgm_time=prgm_time, nb_functions=nb_functions, most_call=most_call, functions=functions, functions_json=functions_json)


@app.route("/upload", methods=["GET", "POST"])
@login_required
def upload():
    if request.method == "POST":

        if "file" not in request.files:
            flash("Provide a file")
            return render_template("upload.html")

        req = request.files["file"]

        if not req:
            flash("Provide a file")
            return render_template("upload.html")

        if req.filename == "":
            flash("Provide a file")
            return render_template("upload.html")

        if not req.filename.endswith(".json"):
            flash("Provide a json file")
            return render_template("upload.html")

        filename = secure_filename(req.filename)
        req.save(f"uploads/{filename}")

        with open(f"uploads/{filename}", 'r') as file:

            data = json.load(file)

            try:
                validate_json_struct(data)
            except ValueError:
                flash(str(ValueError))
                os.remove(f"uploads/{filename}")
                return render_template("upload.html")

            analysis_id = insert_analysis(
                session["user_id"], data["metadata"]["program_name"], data["metadata"]["total_time"], data["metadata"]["timestamp"])

            for function in data["functions"]:
                insert_function(
                    analysis_id, function["name"], function["exec_time"], function["call_count"], function["avg_time"])

        os.remove(f"uploads/{filename}")

        return redirect(f"/report/{analysis_id}")

    return render_template("upload.html")
