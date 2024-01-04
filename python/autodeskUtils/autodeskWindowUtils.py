import pyautogui
import pygetwindow
from pynput.keyboard import Key, Controller as KeyboardController
from pynput.mouse import Button, Controller as MouseController
from usart.usartUtils import PortParser
from enum import Enum
import itertools as it
import time

keyboard = KeyboardController()
mouse = MouseController()

class AutodeskWindowManager:
    def __init__(self, windowTitle : str) -> None:
        self.windowTitle = windowTitle
        self.window = pygetwindow.getWindowsWithTitle(windowTitle)

      
    def focusAutodeskWindow(self):
        if len(self.window) > 0:
            try:
                if not self.isAutodeskWindowInFocus():
                    self.window[0].activate()
            except pygetwindow.PyGetWindowException:
                pass
    
    def isAutodeskWindowInFocus(self) -> bool:
        if len(self.window) > 0:
            try:
                if self.window[0].isActive and not self.window[0].isMinimized and self.window[0].visible:
                    return True
                else:
                    return False
                
            except pygetwindow.PyGetWindowException:
                return False
            
        else:
            return False
    
    def windowExists(self) -> bool:
        return self.window.count > 0
    
    def aquireWindow(self) -> None:
        self.window = pygetwindow.getWindowsWithTitle(self.windowTitle)
    
    def setWindowTitle(self, windowTitle : str) -> None:
        self.windowTitle = windowTitle
    
    class WindowDoesNotExistException (Exception):
        pass

class AutodeskWindowActionManager:
    def __init__(self, autodeskWindowManager : AutodeskWindowManager, portParser : PortParser) -> None:
        self.windowManager = autodeskWindowManager
        self.portParser = portParser
        self.data = b''
        self.hotkeys = {
            self.ButtonActions.HOME : Key.f6,
            self.ButtonActions.ROTATE : Key.f4
            }
        self.hotkeysPyGUI = {
            self.ButtonActions.HOME : 'f6',
            self.ButtonActions.ROTATE : 'f4'
            }
        self.hotkeyStatus = {
            self.ButtonActions.HOME : False,
            self.ButtonActions.ROTATE : False
        }
        self.bordersX = [50, self.windowManager.window[0].width-30]
        self.bordersY = [50, self.windowManager.window[0].height-40]

        
    class ButtonActions(Enum):
        NONE = 0
        HOME = 1
        ROTATE = 2
        
    def start(self) -> None:
        while True:
            if self.windowManager.isAutodeskWindowInFocus():
                self.data = self.portParser.getData()
                if self.data is not None:
                    self.performActions(rotX=int(self.data.get('RotX')), rotY=int(self.data.get('RotY')), zoom=int(self.data.get('Zoom')), button1=self.ButtonActions(int(self.data.get('Button0'))), button2=self.ButtonActions(int(self.data.get('Button1'))))
            else:
                time.sleep(0.5)
            
    def performActions(self, rotX : int = None, rotY : int = None, zoom : int = None, button1 : ButtonActions = None , button2 : ButtonActions = None) -> None:
        ########################  DPI CHECK   ########################
            
        ########################  ZOOM CHECK   ########################
        znn = False
        if zoom is not None: 
            if self.hotkeyStatus[self.ButtonActions.ROTATE]:
                keyboard.release(self.hotkeys[self.ButtonActions.ROTATE])
                self.hotkeyStatus[self.ButtonActions.ROTATE] = False
                mouse.release(Button.left)
            
            # pynput doesn't support scrolling, so we'll use the wheel button
            znn = True
            if zoom != 0:
                if zoom < 0:
                    mouse.scroll(0, 1)
                else:
                    mouse.scroll(0, -1)
                time.sleep(0.10)
        
        ########################  BUTTON CHECK   ########################
        if button1 is not None: 
            match button1:
                
                case self.ButtonActions.HOME:
                    keyboard.press(self.hotkeys[self.ButtonActions.HOME])
                    keyboard.release(self.hotkeys[self.ButtonActions.HOME])
                    
                
        if button2 is not None:
            match button2:
                
                case self.ButtonActions.HOME:
                    keyboard.press(self.hotkeys[self.ButtonActions.HOME])
                    keyboard.release(self.hotkeys[self.ButtonActions.HOME])
                        
        ########################  X,Y AXIS CHECK   ########################
        if znn and zoom == 0: 
            if rotX is not None and rotY is not None:
                if rotX == 0 and rotY == 0 :
                    if self.hotkeyStatus[self.ButtonActions.ROTATE] :
                        pyautogui.keyUp(self.hotkeysPyGUI[self.ButtonActions.ROTATE])
                        self.hotkeyStatus[self.ButtonActions.ROTATE] = False
                        pyautogui.mouseUp()
                    return

                ########################  PERFORM ACTIONS   ########################
            
                self.rotate(rotX, rotY)

    def rotate(self, rotate_x_degrees : int, rotate_y_degrees : int) -> None:
        if(not self.windowManager.windowExists):
            raise AutodeskWindowManager.WindowDoesNotExistException(f"\"{self.windowManager.windowTitle}\" window does not exist")
        
        if not self.hotkeyStatus[self.ButtonActions.ROTATE] :
            pyautogui.keyDown(self.hotkeysPyGUI[self.ButtonActions.ROTATE])
            self.hotkeyStatus[self.ButtonActions.ROTATE] = True
            pyautogui.moveTo(x=self.windowManager.window[0].width/2, y=self.windowManager.window[0].height/2)
            pyautogui.mouseDown()

        while True:
            jsonData = self.portParser.getData()
            rotx = int(jsonData.get('RotX'))
            roty = int(jsonData.get('RotY'))
            if jsonData is not None:
                if rotx == 0 and roty == 0 or jsonData.get('Button0') != 0 or jsonData.get('Button1') != 0:
                    self.data = jsonData 
                    break
            currentMouseX, currentMouseY = pyautogui.position()
            if currentMouseX < self.bordersX[0] or currentMouseX > self.bordersX[1] or currentMouseY < self.bordersY[0] or currentMouseY > self.bordersY[1]:
                pyautogui.mouseUp()
                pyautogui.moveTo(x=self.windowManager.window[0].width/2, y=self.windowManager.window[0].height/2)
                pyautogui.mouseDown()
                continue
            if rotx is None:
                rotx = 0
            
            if roty is None:
                roty = 0
            
            # if rotate_x_degrees > 0:
            #     rotx *= -1
        
            # if rotate_y_degrees < 0:
            #     roty *= -1
            pyautogui.moveRel(xOffset = rotx, yOffset = roty)
            

