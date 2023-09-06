import base64
from http.server import BaseHTTPRequestHandler, HTTPServer

x = 5

# Create a custom request handler that extends BaseHTTPRequestHandler
class CustomRequestHandler(BaseHTTPRequestHandler):
    def do_POST(self):
        global x
        content_length = int(self.headers['Content-Length'])
        post_data = self.rfile.read(content_length)
        
        # Decode the base64 data
        decoded_data = base64.b64decode(post_data)

        slika = str(x) + 'received_image.png'
        x = x + 1
        # Save the decoded data as a file
        with open(slika, 'wb') as file:
            file.write(decoded_data)
        
        # Print a success message
        print("Image saved as 'received_image.png'")
        
        # Send a response back to the client
        self.send_response(200)
        self.send_header('Content-type', 'text/plain')
        self.end_headers()
        self.wfile.write(b'Hello, world!')

    def do_GET(self):
        path = self.path
        path = path[1:]

        print(path)

        self.send_response(200)
        self.send_header('Content-type', 'text/plain')
        self.end_headers()
        self.wfile.write(path.encode())

def run(server_class=HTTPServer, handler_class=CustomRequestHandler, port=9998):
    server_address = ("192.168.100.7", port)
    httpd = server_class(server_address, handler_class)
    print(f'Starting server on port {port}...')
    httpd.serve_forever()

if __name__ == '__main__':
    run()