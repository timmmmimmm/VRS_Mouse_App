from serial import Serial
from multiprocessing import Lock
from serial.tools import list_ports
from serial.serialutil import SerialException, PortNotOpenError
import jsonUtils
import time

class PortParser:
    
    def __init__(self, port:str, windowTitle:str) -> None:
        self.port = port
        self.BAUD_RATE = 115200
        self.portTimeout = 5
        self.mutex = Lock()
        self.mutexIsLocked = False
        self.data = b''
        
    def start(self) -> None: 
        try:
            self.ser = Serial(self.port,self.BAUD_RATE, timeout=self.portTimeout)
            self.ser.reset_input_buffer()
            while True:
                try:
                    
                    db = self.commSerial()
                    self.data = jsonUtils.to_json(db)
                    # print(self.data)
                except SerialException:
                    self.reaquirePort()
                    continue
        except (PortNotOpenError, SerialException):
            self.reaquirePort()
            self.start_protected()
            return

    def start_protected(self):
        try:
            self.ser.reset_input_buffer()
            while True:
                try:
                    
                    db = self.commSerial()
                    self.data = jsonUtils.to_json(db)
                    # print(self.data)
                except SerialException:
                    self.reaquirePort()
                    continue
        except (PortNotOpenError, SerialException):
            self.reaquirePort()
            self.start_protected()
            return
        
    def getData(self):
        return self.data
    
    def commSerial(self, message : bytes = None) -> bytes | str:
        self.mutex.acquire() 
        self.mutexIsLocked = True
        if message is not None:
            self.ser.reset_input_buffer()
            self.ser.write(message)

            newSetupData = self.ser.read(5000)
            
            newSetupDataStr = str(newSetupData)
            newSetupDataStr = newSetupDataStr[newSetupDataStr.find("{\"dpi") : ]
            newSetupDataStr = newSetupDataStr[:newSetupDataStr.find("}")+1]
            self.ser.reset_input_buffer()
            
            self.mutex.release()
            return newSetupDataStr

        
        serialData = self.ser.readline()
        self.ser.reset_input_buffer()
        
        self.mutex.release()  
        self.mutexIsLocked = False  
        return serialData
        
    
    def reaquirePort(self) -> None:
        print('Reaquireing port')
        self.port = None
        
        while(self.port is None):
            self.port = detect_stm32_port()
        
        time.sleep(5)
        
        isPrinted = False
        while True:
            try:
                self.ser = Serial(self.port,self.BAUD_RATE, timeout=self.portTimeout)
                break
            except SerialException:
                if not isPrinted:
                    print("The STM port is taken up by a serial monitor, please disconnect it!")
                    isPrinted = True
                    
                time.sleep(1)
                continue
            
        if self.mutexIsLocked:
            self.mutex.release()
        
    
               
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

        