from http.server import BaseHTTPRequestHandler, HTTPServer
import json

import jsonUtils as JU
from usart.usartUtils import PortParser
from server.entities.mouse import MouseSettings
from server.entities.mouse import MouseSettingsEncoder as MSencoder




class SettingServerHandler(BaseHTTPRequestHandler):
    
        
    def do_GET(self):
        if self.path == '/ma/api/all':
            send_data = "$"
            data=portParser.commSerial(send_data.encode('utf-8'))
            print(data)
            data=JU.parseJsonMouseData(data)
            if data is None:
                self.send_response(400)
                self.send_header('Content-type', 'text/plain')
                self.end_headers()
                self.wfile.write(b'Invalid data format')
                return

            mouseSettings = MouseSettings(data.dpi, data.button0, data.button1)

            self.send_response(200)
            self.send_header('Content-type', 'application/json')
            self.end_headers()
            
            json_str = json.dumps(mouseSettings,cls=MSencoder)
            self.wfile.write(json_str.encode('utf-8'))
        else:
            self.send_response(404)
            self.end_headers()
            self.wfile.write(b'Not Found')

    def do_POST(self):
        if self.path == '/ma/api/all':
            content_length = int(self.headers['Content-Length'])
            post_data = self.rfile.read(content_length)
            mouseSettings = JU.parseJsonMouseData(post_data)

            # if not all(isinstance(value,int) and value >= 0 for value in [dpi_value, button0_value, button1_value]):
            #     # Respond with a 400 Bad Request status if any value is not a digit
            #     self.send_response(400)
            #     self.send_header('Content-type', 'text/plain')
            #     self.end_headers()
            #     self.wfile.write(b'Invalid data format')
            #     return
            

            self.send_response(200)
            self.send_header('Content-type', 'text/plain')
            self.end_headers()
            self.wfile.write(b'Success')
            send_data= f"{mouseSettings.dpi},{mouseSettings.btn1},{mouseSettings.btn2}," #same as yours Edko but speeeeed 
            portParser.commSerial(send_data.encode('utf-8'),response=False)

        else:
            self.send_response(404)
            self.end_headers()
            self.wfile.write(b'Not Found')      
            
class SettingServer:
    def __init__(self, port:int, usartPortParser : PortParser, hostName:str = "localhost") -> None:
        self.port = port
        self.name = hostName
        global portParser
        portParser = usartPortParser
        self.server = HTTPServer((hostName, port), SettingServerHandler)
        
    def start(self) -> None:
        try:
            print(f'Server listening on {self.name}:{self.port}')
            self.server.serve_forever()
        except KeyboardInterrupt:
            pass
