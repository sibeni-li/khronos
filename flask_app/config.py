import os
from dotenv import load_dotenv

load_dotenv()

class Config:
    SESSION_TYPE = 'filesystem'
    SESSION_PERMANENT = False
    MAX_CONTENT_LENGTH = 16 * 1024 * 1024  

class DevelopmentConfig(Config):
    DEBUG = True
    SECRET_KEY = 'dev_secret_key'

class ProductionConfig(Config):
    DEBUG = False
    SECRET_KEY = os.getenv('SECRET_KEY')