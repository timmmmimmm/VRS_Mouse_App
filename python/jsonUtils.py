import json
from types import SimpleNamespace

def to_json(data: bytes):
    try:
        return parse_json_data(data.decode('utf-8'))
    except UnicodeDecodeError:
        return None

def parse_json_data(json_input : str):
    try:
        data = json.loads(json_input)
        return data
    except json.JSONDecodeError:
        return None

def parseJsonMouseData(JsonInput:str):
    try:
        return json.loads(JsonInput, object_hook=lambda d: SimpleNamespace(**d))
    except (json.JSONDecodeError):
        return None