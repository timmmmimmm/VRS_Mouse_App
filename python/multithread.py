import json
import jsonUtils as JU
import http.server
import socketserver
import threading
import usartUtils
from usartUtils import PortParser as PP        

def main():
    windowTitle ='Autodesk Inventor Professional 2024'
    global portParser 
    portParser = PP(usartUtils.detect_stm32_port(), windowTitle)
    portParser.start()
    
# Define a custom handler to serve JSON data
class MyHandler(http.server.BaseHTTPRequestHandler):
    global portParser 
    def do_GET(self):
        if self.path == '/dpi':
            send_data = "$"
            dpi_data=portParser.addPayload(send_data.encode('utf-8'))
            print(dpi_data)
            dpi_data=JU.to_jason(dpi_data)
            if dpi_data is None:
                self.send_response(400)
                self.send_header('Content-type', 'text/plain')
                self.end_headers()
                self.wfile.write(b'Invalid data format')
                return

            self.send_response(200)
            self.send_header('Content-type', 'application/json')
            self.end_headers()
            json_data = dpi_data
            json_str = json.dumps(json_data)
            self.wfile.write(json_str.encode('utf-8'))
        else:
            self.send_response(404)
            self.end_headers()
            self.wfile.write(b'Not Found')

    def do_POST(self):
        if self.path == '/dpi_write':
            content_length = int(self.headers['Content-Length'])
            post_data = self.rfile.read(content_length)
            parsed_data = JU.parse_json_data(post_data)
            dpi_value = parsed_data.get('dpi')
            button0_value = parsed_data.get('button0')
            button1_value = parsed_data.get('button1')

            if not all(isinstance(value,int) and value >= 0 for value in [dpi_value, button0_value, button1_value]):
                # Respond with a 400 Bad Request status if any value is not a digit
                self.send_response(400)
                self.send_header('Content-type', 'text/plain')
                self.end_headers()
                self.wfile.write(b'Invalid data format')
                return
            

            self.send_response(200)
            self.send_header('Content-type', 'text/plain')
            self.end_headers()
            self.wfile.write(b'Success')
            send_data=str(dpi_value)+','+str(button0_value)+','+str(button1_value) 
            portParser.addPayload(send_data.encode('utf-8'),response=False)

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


if __name__ == "__main__":
    # Create a thread to run the main() function concurrently
    main_thread = threading.Thread(target=main)
    main_thread.start()

    # Create a thread to run the HTTP server concurrently
    # server_thread = threading.Thread(target=start_http_server)
    # server_thread.start()
