import pygetwindow
import pyautogui
from enum import Enum
import time

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
    def __init__(self, autodeskWindowManager : AutodeskWindowManager) -> None:
        self.windowManager = autodeskWindowManager
        self.hotkeys = {
            self.ButtonActions.HOME : 'f6',
            self.ButtonActions.ROTATE : 'f4'
            }
        
    class ButtonActions(Enum):
        NONE = 0
        HOME = 1
        ROTATE = 2
        
    
    def performActions(self, rotX : int = None, rotY : int = None, zoom : int = None, dpi : float = None , button1 : ButtonActions = None , button2 : ButtonActions = None) -> None:
        ########################  DPI CHECK   ########################
        if dpi is None:
            dpi = 1
            
            
        ########################  ZOOM CHECK   ########################
        znn = False
        if zoom is not None: 
            pyautogui.scroll(zoom*dpi*-1)
            znn = True
        ########################  X,Y AXIS CHECK   ########################
        # if rotX is not None and rotY is not None:
            
        #     if rotX == 0 and rotY == 0 :
        #         return
        
        #     if znn and zoom == 0: 
        #         ########################  BUTTON CHECK   ########################
        #         if button1 is not None: 
        #             match button1:
                        
        #                 case self.ButtonActions.HOME:
        #                     pyautogui.hotkey(self.hotkeys[self.ButtonActions.HOME]) 
                            
                        
        #         if button2 is not None:
        #             match button2:
                        
        #                 case self.ButtonActions.HOME:
        #                     pyautogui.hotkey(self.hotkeys[self.ButtonActions.HOME])
                          
                
                ########################  PERFORM ACTIONS   ########################
                #self.rotate(rotX*dpi, rotY*dpi)
                # TODO: dokoncit ostatne funkcie

    def rotate(self, rotate_x_degrees : int, rotate_y_degrees : int) -> None:
        if(not self.windowManager.windowExists):
            raise AutodeskWindowManager.WindowDoesNotExistException(f"\"{self.windowManager.windowTitle}\" window does not exist")
        
        pyautogui.keyDown(self.hotkeys[self.ButtonActions.ROTATE])
        pyautogui.moveTo(x=self.windowManager.window[0].width/2, y=self.windowManager.window[0].height/2)
        pyautogui.drag(xOffset=rotate_x_degrees, yOffset=rotate_y_degrees)
        pyautogui.keyUp(self.hotkeys[self.ButtonActions.ROTATE])       