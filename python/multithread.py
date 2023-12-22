from serial import Serial
import serial.tools.list_ports
import pyautogui
import time
import pygetwindow
import json
import http.server
import socketserver
import threading


# Define SOFTWARE as 'inventor' or 'fusion' to switch between Autodesk Inventor and Autodesk Fusion
SOFTWARE = 'Autodesk Inventor Professional 2024'
# Flag to indicate if the Autodesk window is in focus
autodesk_window_in_focus = False  # Initialize to False


class PortParser:
    
    def __init__(self, port) -> None:
        self.port = port
        self.BAUD_RATE = 115200
        self.payload = ""
        self.lock = False
        self.response = b''
        self.ser = Serial(self.port,self.BAUD_RATE, timeout=5)
    
    def start(self) -> None: 
        buffer = b''

        while True:
            while self.lock:
                pass
            
            json_bytes = self.ser.read(60)
            buffer += json_bytes

            if b'{' in buffer and b'}' in buffer:
                json_start = buffer.index(b'{')
                json_end = buffer.index(b'}') + 1
                json_obj = buffer[json_start:json_end]
                json_input = json_obj.decode('utf-8')
                data = parse_json_data(json_input)

                if data is not None:
                    #if autodesk_window_in_focus:
                    focus_autodesk_window()
                    perform_actions(data)
                        
                print(data)
                buffer = buffer[json_end:]
    
    def addPayload(self, payload)-> bytearray:
        self.lock = True
        self.payload = payload
        
        self.ser.write(payload)
        self.response = self.ser.readline()
        self.lock = False
        return self.response
        
        


def main():
    global portParser 
    portParser = PortParser(detect_stm32_port())
    portParser.start()
    

def detect_stm32_port():
    try:
        # Get a list of available serial ports
        available_ports = serial.tools.list_ports.comports()

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

def focus_autodesk_window():
    autodesk_window_title = SOFTWARE  # Form window title based on SOFTWARE
    global window
    window = pygetwindow.getWindowsWithTitle(autodesk_window_title)

    global autodesk_window_in_focus  # Use the global flag

    if len(window) > 0:
        if not window[0].isActive or window[0].isMinimized or not window[0].visible:  # Check if the window is not already in focus
            try:
                window[0].activate()
                if not autodesk_window_in_focus:  # Check if it was previously not in focus
                    print(f'Focused {SOFTWARE.capitalize()} window')
                autodesk_window_in_focus = True  # Window is now in focus
            except pygetwindow.PyGetWindowException as e:
                if autodesk_window_in_focus:  # Check if it was previously in focus
                    print(f'Error activating window(window is unfocused): {e}')
                autodesk_window_in_focus = False  # Window is not in focus
        else:
            if not autodesk_window_in_focus:  # Check if it was previously not in focus
                print(f'Focused {SOFTWARE.capitalize()} window')
            autodesk_window_in_focus = True  # Window is already in focus
    else:
        if autodesk_window_in_focus:  # Check if it was previously in focus
            print(f'{SOFTWARE.capitalize()} window not found')
        autodesk_window_in_focus = False  # Window is not in focus

# Define a custom handler to serve JSON data
class MyHandler(http.server.BaseHTTPRequestHandler):
    def do_GET(self):
        if self.path == '/json_data':
            self.send_response(200)
            self.send_header('Content-type', 'application/json')
            self.end_headers()
            
            json_data = {
                'Rotate_x': 30,
                'Rotate_y': 45,
                'Rotate_z': 0,
                'Button': 'none',
                'Zoom': -10
            }

            json_str = json.dumps(json_data)
            self.wfile.write(json_str.encode('utf-8'))
        else:
            self.send_response(404)
            self.end_headers()
            self.wfile.write(b'Not Found')

def start_http_server():
    host = 'localhost'
    port = 12345

    with socketserver.TCPServer((host, port), MyHandler) as httpd:
        print(f'Server listening on {host}:{port}')
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            pass

def parse_json_data(json_input):
    try:
        data = json.loads(json_input)
        return data
    except json.JSONDecodeError as e:
        print(f"JSON decoding error: {e}")
        return None

def perform_actions(data):
    rotate_x_degrees = data.get('RotX')
    rotate_y_degrees = data.get('RotY')
    #rotate_z_degrees = data.get('RotZ')
    #sensitivity = data.get('Sensitivity')
    button1 = data.get('Button0')
    button2 = data.get('Button1')
    zoom_value = data.get('Zoom')

    #if all(val is not None for val in [rotate_x_degrees, rotate_y_degrees, rotate_z_degrees, sensitivity, button, zoom_value]):
    if all(val is not None for val in [rotate_x_degrees, rotate_y_degrees, button1, button2, zoom_value]):
        # print(f'Rotate_x: {rotate_x_degrees}')
        # print(f'Rotate_y: {rotate_y_degrees}')
        # #print(f'Rotate_z: {rotate_z_degrees}')
        # #print(f'Sensitivity: {sensitivity}')
        # print(f'Button: {button1}')
        # print(f'Button: {button2}')
        # print(f'Zoom: {zoom_value}')
        
        pyautogui.scroll(zoom_value)
        
        
        if zoom_value == 0: 
            if button1 == 'home':
                pyautogui.hotkey('f6')  # Modify based on your function key
                return

            # Add your action logic here based on the parsed data
            #rotate(rotate_x_degrees, rotate_y_degrees, rotate_z_degrees)
            rotate(rotate_x_degrees, rotate_y_degrees)
            # TODO: dokoncit ostatne funkcie

#def rotate(rotate_x_degrees, rotate_y_degrees, rotate_z_degrees):
def rotate(rotate_x_degrees, rotate_y_degrees):
    pyautogui.keyDown('f4')
    pyautogui.moveTo(x=window[0].width/2, y=window[0].height/2)
    pyautogui.drag(xOffset=rotate_x_degrees, yOffset=rotate_y_degrees)
    pyautogui.keyUp('f4')



if __name__ == "__main__":
    # Create a thread to run the main() function concurrently
    main_thread = threading.Thread(target=main)
    main_thread.start()

    # Create a thread to run the HTTP server concurrently
    # server_thread = threading.Thread(target=start_http_server)
    # server_thread.start()

    # Run the focus_autodesk_window() function in the main thread
    # while True:
    #     focus_autodesk_window()
    #     time.sleep(0.5)
