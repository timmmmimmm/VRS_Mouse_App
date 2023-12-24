import json

def to_json(data: bytes):
    return parse_json_data(data.decode('utf-8'))


def parse_json_data(json_input : str):
    try:
        data = json.loads(json_input)
        return data
    except json.JSONDecodeError:
        return None
