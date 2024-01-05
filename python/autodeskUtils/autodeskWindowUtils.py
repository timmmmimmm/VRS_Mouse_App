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
        self.aquireWindow()
        if len(self.window) > 0:
            try:
                if self.window[0].isActive and not self.window[0].isMinimized and self.window[0].visible:
                    return True
                else:
                    return False
                
            except pygetwindow.PyGetWindowException:
                return False
            
        else:
           raise AutodeskWindowManager.WindowDoesNotExistException
    
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
            self.ButtonActions.CANCEL : Key.esc,
            self.ButtonActions.PREVIOUS : Key.f5,
            self.ButtonActions.ZOOM_ALL : Key.home,
            self.ButtonActions.ROTATE : Key.f4
            }
        self.hotkeysPyGUI = {
            self.ButtonActions.HOME : 'f6',
            self.ButtonActions.CANCEL : 'esc',
            self.ButtonActions.PREVIOUS : 'f5',
            self.ButtonActions.ZOOM_ALL : 'home',
            self.ButtonActions.ROTATE : 'f4'
            }
        self.hotkeyStatus = {
            self.ButtonActions.HOME : False,
            self.ButtonActions.CANCEL : False,
            self.ButtonActions.PREVIOUS : False,
            self.ButtonActions.ZOOM_ALL : False,
            self.ButtonActions.ROTATE : False
        }

        
    class ButtonActions(Enum):
        NONE = 0
        HOME = 1
        CANCEL = 2
        PREVIOUS = 3
        ZOOM_ALL = 4
        ROTATE = 5
        
    def start(self) -> None:
        while True:
            try:
                if self.windowManager.isAutodeskWindowInFocus():
                    self.data = self.portParser.getData()
                    if self.data is not None:
                        self.performActions(rotX=int(self.data.get('RotX')), rotY=int(self.data.get('RotY')), zoom=int(self.data.get('Zoom')), button1=self.ButtonActions(int(self.data.get('Button0'))), button2=self.ButtonActions(int(self.data.get('Button1'))))
                else:
                    time.sleep(0.5)
            except AutodeskWindowManager.WindowDoesNotExistException:
                self.windowManager.aquireWindow()
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
                case self.ButtonActions.CANCEL:
                    keyboard.press(self.hotkeys[self.ButtonActions.CANCEL])
                    keyboard.release(self.hotkeys[self.ButtonActions.CANCEL])
                case self.ButtonActions.PREVIOUS:
                    keyboard.press(self.hotkeys[self.ButtonActions.PREVIOUS])
                    keyboard.release(self.hotkeys[self.ButtonActions.PREVIOUS])
                case self.ButtonActions.ZOOM_ALL:
                    keyboard.press(self.hotkeys[self.ButtonActions.ZOOM_ALL])
                    keyboard.release(self.hotkeys[self.ButtonActions.ZOOM_ALL])

                
        if button2 is not None:
            match button2:
                
                case self.ButtonActions.HOME:
                    keyboard.press(self.hotkeys[self.ButtonActions.HOME])
                    keyboard.release(self.hotkeys[self.ButtonActions.HOME])
                case self.ButtonActions.CANCEL:
                    keyboard.press(self.hotkeys[self.ButtonActions.CANCEL])
                    keyboard.release(self.hotkeys[self.ButtonActions.CANCEL])
                case self.ButtonActions.PREVIOUS:
                    keyboard.press(self.hotkeys[self.ButtonActions.PREVIOUS])
                    keyboard.release(self.hotkeys[self.ButtonActions.PREVIOUS])
                case self.ButtonActions.ZOOM_ALL:
                    keyboard.press(self.hotkeys[self.ButtonActions.ZOOM_ALL])
                    keyboard.release(self.hotkeys[self.ButtonActions.ZOOM_ALL])
                    
                
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
                try:
                    self.rotate()
                except AutodeskWindowManager.WindowDoesNotExistException:
                    self.windowManager.aquireWindow()
                    return

    def rotate(self) -> None:
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
            try:
                bordersX = self.getWindowBorderX()
                bordersY = self.getWindowBorderY()
                
                if currentMouseX < bordersX[0] or currentMouseX > bordersX[1] or currentMouseY < bordersY[0] or currentMouseY > bordersY[1]:
                    pyautogui.mouseUp()
                    pyautogui.moveTo(x=self.windowManager.window[0].width/2, y=self.windowManager.window[0].height/2)
                    pyautogui.mouseDown()
                    continue
                if rotx is None:
                    rotx = 0
                
                if roty is None:
                    roty = 0

                pyautogui.moveRel(xOffset = rotx, yOffset = roty)
                
            except IndexError:
                break
    
    def getWindowBorderX(self) -> []:
        return [100, self.windowManager.window[0].width-30]
    
    def getWindowBorderY(self) -> []:
        return [50, self.windowManager.window[0].height-40]
            

