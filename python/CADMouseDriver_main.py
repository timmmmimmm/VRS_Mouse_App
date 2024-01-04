from threading import Thread
import usart.usartUtils as usartUtils
from usart.usartUtils import PortParser as PP   
from server.settingServer import SettingServer     


def main(portParser):
    portParser.start()
    
# Define a custom handler to serve JSON data

def start_http_server(portParser):
    
    server = SettingServer(12345, portParser)
    server.start()


if __name__ == "__main__":
    
    windowTitle ='Autodesk Inventor Professional 2024'
    pp = PP(usartUtils.detect_stm32_port(), windowTitle)
    
   
    # Create a thread to run the main() function concurrently
    main_thread = Thread(target=main, args=[pp])
    main_thread.start()

    # Create a thread to run the HTTP server concurrently
    server_thread = Thread(target=start_http_server, args=[pp])
    server_thread.start()
    
    main_thread.join()
    server_thread.join()
