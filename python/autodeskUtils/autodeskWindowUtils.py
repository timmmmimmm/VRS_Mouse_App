# import pygetwindow
# import pyautogui
# from enum import Enum
# import itertools as it
# import time

# class AutodeskWindowManager:
#     def __init__(self, windowTitle : str) -> None:
#         self.windowTitle = windowTitle
#         self.window = pygetwindow.getWindowsWithTitle(windowTitle)
      
#     def focusAutodeskWindow(self):
#         if len(self.window) > 0:
#             try:
#                 if not self.isAutodeskWindowInFocus():
#                     self.window[0].activate()
#             except pygetwindow.PyGetWindowException:
#                 pass
    
#     def isAutodeskWindowInFocus(self) -> bool:
#         if len(self.window) > 0:
#             try:
#                 if self.window[0].isActive and not self.window[0].isMinimized and self.window[0].visible:
#                     return True
#                 else:
#                     return False
                
#             except pygetwindow.PyGetWindowException:
#                 return False
            
#         else:
#             return False
    
#     def windowExists(self) -> bool:
#         return self.window.count > 0
    
#     def aquireWindow(self) -> None:
#         self.window = pygetwindow.getWindowsWithTitle(self.windowTitle)
    
#     def setWindowTitle(self, windowTitle : str) -> None:
#         self.windowTitle = windowTitle
    
#     class WindowDoesNotExistException (Exception):
#         pass
    
# class AutodeskWindowActionManager:
#     def __init__(self, autodeskWindowManager : AutodeskWindowManager) -> None:
#         self.windowManager = autodeskWindowManager
        
#         self.hotkeys = {
#             self.ButtonActions.HOME : 'f6',
#             self.ButtonActions.ROTATE : 'f4'
#             }
#         self.hotkeyStatus = {
#             self.ButtonActions.HOME : False,
#             self.ButtonActions.ROTATE : False
#         }
        
#     class ButtonActions(Enum):
#         NONE = 0
#         HOME = 1
#         ROTATE = 2
        
    
#     def performActions(self, rotX : int = None, rotY : int = None, zoom : int = None, dpi : float = None , button1 : ButtonActions = None , button2 : ButtonActions = None) -> None:
#         ########################  DPI CHECK   ########################
#         if dpi is None:
#             dpi = 3
            
            
#         ########################  ZOOM CHECK   ########################
#         znn = False
#         if zoom is not None: 
            
#             if self.hotkeyStatus[self.ButtonActions.ROTATE]:
#                 pyautogui.keyUp(self.hotkeys[self.ButtonActions.ROTATE])
#                 self.hotkeyStatus[self.ButtonActions.ROTATE] = False
#                 pyautogui.mouseUp()
            
#             pyautogui.scroll(zoom*dpi*-1) #By default the Zoom axis is inverted soo *-1 it is
#             znn = True
        
        
#         ########################  BUTTON CHECK   ########################
#         if button1 is not None: 
#             match button1:
                
#                 case self.ButtonActions.HOME:
#                     pyautogui.hotkey(self.hotkeys[self.ButtonActions.HOME]) 
                    
                
#         if button2 is not None:
#             match button2:
                
#                 case self.ButtonActions.HOME:
#                     pyautogui.hotkey(self.hotkeys[self.ButtonActions.HOME])
                        
#         ########################  X,Y AXIS CHECK   ########################
#         if znn and zoom == 0: 
#             if rotX is not None and rotY is not None:
#                 if rotX == 0 and rotY == 0 :
#                     if self.hotkeyStatus[self.ButtonActions.ROTATE] :
#                         pyautogui.keyUp(self.hotkeys[self.ButtonActions.ROTATE])
#                         self.hotkeyStatus[self.ButtonActions.ROTATE] = False
#                         pyautogui.mouseUp()
#                     return
                
#                 ########################  PERFORM ACTIONS   ########################
#                 self.rotate(rotX*dpi, rotY*dpi)

#     def rotate(self, rotate_x_degrees : int, rotate_y_degrees : int) -> None:
#         if(not self.windowManager.windowExists):
#             raise AutodeskWindowManager.WindowDoesNotExistException(f"\"{self.windowManager.windowTitle}\" window does not exist")
        
#         if not self.hotkeyStatus[self.ButtonActions.ROTATE] :
#             pyautogui.keyDown(self.hotkeys[self.ButtonActions.ROTATE])
#             self.hotkeyStatus[self.ButtonActions.ROTATE] = True
#             pyautogui.moveTo(x=self.windowManager.window[0].width/2, y=self.windowManager.window[0].height/2)
#             pyautogui.mouseDown()
                
#         for rotx, roty in it.zip_longest(range(1, abs(rotate_x_degrees)), range(1, abs(rotate_y_degrees))):
            
#             if rotx is None:
#                 rotx = 0
            
#             if roty is None:
#                 roty = 0
            
#             if rotate_x_degrees < 0:
#                 rotx *= -1
            
#             if rotate_y_degrees < 0:
#                 roty *=-1
            
#             pyautogui.moveRel(xOffset = rotx, yOffset = roty)

import pygetwindow
from pynput.keyboard import Key, Controller as KeyboardController
from pynput.mouse import Button, Controller as MouseController
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
    def __init__(self, autodeskWindowManager : AutodeskWindowManager) -> None:
        self.windowManager = autodeskWindowManager
        
        self.hotkeys = {
            self.ButtonActions.HOME : Key.f6,
            self.ButtonActions.ROTATE : Key.f4
            }
        self.hotkeyStatus = {
            self.ButtonActions.HOME : False,
            self.ButtonActions.ROTATE : False
        }
        
    class ButtonActions(Enum):
        NONE = 0
        HOME = 1
        ROTATE = 2
        
    
    def performActions(self, rotX : int = None, rotY : int = None, zoom : int = None, dpi : float = None , button1 : ButtonActions = None , button2 : ButtonActions = None) -> None:
        ########################  DPI CHECK   ########################
        if dpi is None:
            dpi = 3
            
            
        ########################  ZOOM CHECK   ########################
        znn = False
        if zoom is not None: 
            if self.hotkeyStatus[self.ButtonActions.ROTATE]:
                keyboard.release(self.hotkeys[self.ButtonActions.ROTATE])
                self.hotkeyStatus[self.ButtonActions.ROTATE] = False
                mouse.release(Button.left)
            
            # pynput doesn't support scrolling, so we'll use the wheel button
            for _ in range(int(zoom*dpi*-1)):
                mouse.press(Button.middle)
                mouse.release(Button.middle)
            znn = True
        
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
                        keyboard.release(self.hotkeys[self.ButtonActions.ROTATE])
                        self.hotkeyStatus[self.ButtonActions.ROTATE] = False
                        mouse.release(Button.left)
                    return
                
                ########################  PERFORM ACTIONS   ########################
                self.rotate(rotX*dpi, rotY*dpi)

    def rotate(self, rotate_x_degrees : int, rotate_y_degrees : int) -> None:
        if(not self.windowManager.windowExists):
            raise AutodeskWindowManager.WindowDoesNotExistException(f"\"{self.windowManager.windowTitle}\" window does not exist")
        
        if not self.hotkeyStatus[self.ButtonActions.ROTATE] :
            keyboard.press(self.hotkeys[self.ButtonActions.ROTATE])
            self.hotkeyStatus[self.ButtonActions.ROTATE] = True
            mouse.position = (self.windowManager.window[0].width/2, self.windowManager.window[0].height/2)
            mouse.press(Button.left)
                
        for rotx, roty in it.zip_longest(range(1, abs(rotate_x_degrees)), range(1, abs(rotate_y_degrees))):
            
            if rotx is None:
                rotx = 0
            
            if roty is None:
                roty = 0
            
            if rotate_x_degrees < 0:
                rotx *= -1
            
            if rotate_y_degrees < 0:
                roty *=-1
            mouse.position = (self.windowManager.window[0].width/2+rotx, self.windowManager.window[0].height/2+roty)
            mouse.press(Button.left)
            # mouse.move(rotx, roty)
