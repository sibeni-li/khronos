import os
from dotenv import load_dotenv

load_dotenv()

class Config:
    SESSION_PERMANENT = False
    MAX_CONTENT_LENGTH = 16 * 1024 * 1024
    SESSION_COOKIE_HTTPONLY = True
    SESSION_COOKIE_SAMESITE = 'Lax'

class DevelopmentConfig(Config):
    DEBUG = True
    SESSION_COOKIE_SECURE = False
    SECRET_KEY = 'dev_secret_key'

class ProductionConfig(Config):
    DEBUG = False
    SESSION_COOKIE_SECURE = True
    SECRET_KEY = os.getenv('SECRET_KEY')