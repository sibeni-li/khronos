import sqlite3
from helpers import db_execute

# Create tables if not exists


def create_table():
    conn = None
    try:
        conn = sqlite3.connect('profiles.db')
        c = conn.cursor()
        c.execute('''
            CREATE TABLE IF NOT EXISTS users (
                id INTEGER PRIMARY KEY NOT NULL,
                username TEXT NOT NULL UNIQUE,
                hash_password TEXT NOT NULL
            )
        ''')
        c.execute('''
            CREATE TABLE IF NOT EXISTS analysis (
                id INTEGER PRIMARY KEY NOT NULL,
                user_id INTEGER NOT NULL,
                program_name TEXT NOT NULL,
                total_time NUMERIC NOT NULL,
                timestamp DATETIME NOT NULL,
                FOREIGN KEY (user_id) REFERENCES users(id)
            )
        ''')
        c.execute('''
            CREATE TABLE IF NOT EXISTS functions (
                id INTEGER PRIMARY KEY NOT NULL,
                analysis_id INTEGER NOT NULL,
                name TEXT NOT NULL,
                exec_time NUMERIC NOT NULL,
                call_count INTEGER NOT NULL,
                avg_time NUMERIC NOT NULL,
                FOREIGN KEY (analysis_id) REFERENCES analysis(id)
            )
        ''')
        conn.commit()
    except sqlite3.Error as e:
        print(f"Database error during table creation: {e}")
        raise
    finally:
        if conn:
            conn.close()

# Get data


@db_execute
def get_user_by_username(conn, username):
    conn.row_factory = sqlite3.Row
    c = conn.cursor()
    c.execute("SELECT * FROM users WHERE username = ?", (username,))
    return c.fetchone()


@db_execute
def get_analyses(conn, user_id):
    conn.row_factory = sqlite3.Row
    c = conn.cursor()
    c.execute("SELECT * FROM analysis WHERE user_id = ?", (user_id,))
    return c.fetchall()


@db_execute
def get_analysis_by_id(conn, user_id, analysis_id):
    conn.row_factory = sqlite3.Row
    c = conn.cursor()
    c.execute("SELECT * FROM analysis WHERE user_id = ? AND id = ?", (user_id, analysis_id,))
    return c.fetchone()


@db_execute
def get_functions_by_analysis_id(conn, analysis_id):
    conn.row_factory = sqlite3.Row
    c = conn.cursor()
    c.execute("SELECT * FROM functions WHERE analysis_id = ?", (analysis_id,))
    return c.fetchall()


@db_execute
def get_history(conn, user_id):
    conn.row_factory = sqlite3.Row
    c = conn.cursor()
    c.execute("SELECT program_name, total_time, timestamp FROM analysis WHERE user_id = ? ORDER BY timestamp DESC", (user_id,))
    return c.fetchall()

# Insert into tables


@db_execute
def insert_user(conn, username, hash_password):
    c = conn.cursor()
    c.execute("INSERT INTO users (username, hash_password) VALUES (?, ?)",
              (username, hash_password))
    conn.commit()
    return c.lastrowid


@db_execute
def insert_analysis(conn, user_id, program_name, total_time, timestamp):
    c = conn.cursor()
    c.execute("INSERT INTO analysis (user_id, program_name, total_time, timestamp) VALUES (?, ?, ?, ?)",
              (user_id, program_name, total_time, timestamp))
    conn.commit()
    return c.lastrowid


@db_execute
def insert_function(conn, analysis_id, name, exec_time, call_count, avg_time):
    c = conn.cursor()
    c.execute("INSERT INTO functions (analysis_id, name, exec_time, call_count, avg_time) VALUES (?, ?, ?, ?, ?)",
              (analysis_id, name, exec_time, call_count, avg_time))
    conn.commit()
    return c.lastrowid
