import json
import sys
import jsonUtils as JU
import http.server
import socketserver
import threading
import usart.usartUtils as usartUtils
from usart.usartUtils import PortParser as PP   
from server.settingServer import SettingServer     

global init 
init = True

def main():
    windowTitle ='Autodesk Inventor Professional 2024'
    global portParser 
    global init
    portParser = PP(usartUtils.detect_stm32_port(), windowTitle)
    
    init = False
    
    portParser.start()
    
# Define a custom handler to serve JSON data

def start_http_server():
    while init:
        pass
    
    server = SettingServer(12345, portParser)
    server.start()


if __name__ == "__main__":
    # Create a thread to run the main() function concurrently
    main_thread = threading.Thread(target=main)
    main_thread.start()

    # Create a thread to run the HTTP server concurrently
    # server_thread = threading.Thread(target=start_http_server)
    # server_thread.start()
