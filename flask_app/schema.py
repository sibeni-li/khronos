import sqlite3

# Create tables if not exists
def create_table():
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
    conn.close()

# Get data
def get_user_by_username(username):
    conn = sqlite3.connect('profiles.db')
    conn.row_factory = sqlite3.Row
    c = conn.cursor()
    c.execute("SELECT * FROM users WHERE username = ?", (username,))
    user = c.fetchone()
    conn.close()
    return user

# Insert into tables
def insert_user(username, hash_password):
    conn = sqlite3.connect('profiles.db')
    c = conn.cursor()
    c.execute("INSERT INTO users (username, hash_password) VALUES (?, ?)",
              (username, hash_password))
    conn.commit()
    user_id = c.lastrowid
    conn.close()
    return user_id

def insert_analysis(user_id, program_name, total_time, timestamp):
    conn = sqlite3.connect('profiles.db')
    c = conn.cursor()
    c.execute(" INSERT INTO analysis (user_id, program_name, total_time, timestamp) VALUES (?, ?, ?, ?)",
              (user_id, program_name, total_time, timestamp))
    conn.commit()
    analysis_id = c.lastrowid
    conn.close()
    return analysis_id

def insert_function(analysis_id, name, exec_time, call_count, avg_time):
    conn = sqlite3.connect('profiles.db')
    c = conn.cursor()
    c.execute(" INSERT INTO functions (analysis_id, exec_time, name, call_count, avg_time) VALUES (?, ?, ?, ?, ?)",
              (analysis_id, name, exec_time, call_count, avg_time))
    conn.commit()
    function_id = c.lastrowid
    conn.close()
    return function_id
