from json import JSONEncoder as jEncoder
from typing import Any

class MouseSettings:
    def __init__(self, dpi:float = 1.0, btn1 : int = 0, btn2 : int = 0) -> None:
        self.dpi:float = dpi
        self.btn1:int = btn1
        self.btn2:int = btn2

class MouseSettingsEncoder(jEncoder):
    def default(self, settingObj:MouseSettings) -> Any:
        return "{\"dpi\":" + f"{settingObj.dpi}," + "\"btn1:\"" + f"{settingObj.btn1}," + "\"btn2:\"" + f"{settingObj.btn2}" + "}"