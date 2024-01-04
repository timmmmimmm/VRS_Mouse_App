from serial import Serial
from multiprocessing import Lock
from serial.tools import list_ports
from serial.serialutil import SerialException
import jsonUtils
from autodeskUtils.autodeskWindowUtils import AutodeskWindowManager as AWM
from autodeskUtils.autodeskWindowUtils import AutodeskWindowActionManager as AWAM
import time

class PortParser:
    
    def __init__(self, port:str, windowTitle:str) -> None:
        self.port = port
        self.BAUD_RATE = 115200
        self.portTimeout = 5
        self.ser = Serial(self.port,self.BAUD_RATE, timeout=self.portTimeout)
        self.windowManager = AWM(windowTitle)
        self.windowActionManager = AWAM(self.windowManager)
        self.mutex = Lock()
    
    def start(self) -> None: 
        self.ser.reset_input_buffer()
        while True:
            try:
                
                db = self.commSerial()
                data = jsonUtils.to_json(db)

                if data is not None:
                    if self.windowManager.isAutodeskWindowInFocus():
                        try:
                            self.windowActionManager.performActions(rotX=int(data.get('RotX')), rotY=int(data.get('RotY')), zoom=int(data.get('Zoom')), button1=AWAM.ButtonActions(int(data.get('Button1'))), button2=AWAM.ButtonActions(int(data.get('Button2'))) )
                        except(AWM.WindowDoesNotExistException):
                            pass
                        
                    #print(data)
                
            except SerialException:
                self.reaquirePort()
                continue

    
    def commSerial(self, message : bytes = None) -> bytes | str:
        self.mutex.acquire() 
        if message is not None:
            a = None
            self.ser.reset_input_buffer()
            self.ser.write(message)

            newSetupData = self.ser.read(5000)
            
            newSetupDataStr = str(newSetupData)
            newSetupDataStr = newSetupDataStr[newSetupDataStr.find("{\"dpi") : ]
            newSetupDataStr = newSetupDataStr[:newSetupDataStr.find("}")+1]
            return newSetupDataStr

            
            
        serialData = self.ser.readline()

        self.ser.reset_input_buffer()
        
        self.mutex.release()    
        return serialData
        
    
    def reaquirePort(self) -> None:
        print('Reaquireing port')
        self.port = None
        
        while(self.port is None):
            self.port = detect_stm32_port()
        
        time.sleep(5)
        self.ser = Serial(self.port, self.BAUD_RATE, timeout=self.portTimeout)
        
    
               
def detect_stm32_port() -> str:
    try:
        # Get a list of available serial ports
        available_ports = list_ports.comports()

        # Iterate through the available ports and check for STM32 device
        for port in available_ports:
            if "STMICROELECTRONICS STLINK VIRTUAL COM PORT" in port.description.upper():
                print("Connecting to port: ", port)
                return port.device  # Return the COM port device name as a string

        # If no STM32 device is found, return None
        return None
    except Exception as e:
        print(f"Error detecting STM32 port: {e}")
        return None

        