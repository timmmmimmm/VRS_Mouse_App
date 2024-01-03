from serial import Serial
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
        self.payload = ""
        self.lock = False
        self.response = b''
        self.ser = Serial(self.port,self.BAUD_RATE, timeout=self.portTimeout)
        self.windowManager = AWM(windowTitle)
        self.windowActionManager = AWAM(self.windowManager)
    
    def start(self) -> None: 
        self.ser.reset_input_buffer()
        while True:
            while self.lock:
                pass
        
            try:
                db = self.ser.readline()
                data = jsonUtils.to_json(db)

                if data is not None:
                    if self.windowManager.isAutodeskWindowInFocus():
                        try:
                            self.windowActionManager.performActions(rotX=int(data.get('RotX')), rotY=int(data.get('RotY')), zoom=int(data.get('Zoom')))
                        except(AWM.WindowDoesNotExistException):
                            pass
                        
                    print(data)
                self.ser.reset_input_buffer()
            except SerialException:
                self.reaquirePort()
                continue

    
    def addPayload(self, payload,response=True)-> bytes:
        self.lock = True
        self.payload = payload
        
        self.ser.write(payload)
        if response:
            self.response = self.ser.readline()
        self.lock = False
        return self.response
    
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

        