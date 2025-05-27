# make a http server that accepts all requests, return a generic response, and print request details to console
# now print all request details to console, including headers and body
from http.server import BaseHTTPRequestHandler, HTTPServer

class SimpleHTTPRequestHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_response(200)
        self.send_header('Content-type', 'text/plain')
        self.end_headers()
        response = "Hello, World!"
        self.wfile.write(response.encode('utf-8'))
        
        # Print request details to console
        print("----------------------------------------------------")
        print(f"Received GET request: {self.path}")
        print(f"Headers: {self.headers}")
        print(f"Body: (no body for GET request)")
        print("")
    
    def do_POST(self):
        self.send_response(200)
        self.send_header('Content-type', 'text/plain')
        self.end_headers()
        response = "Hello, World!"
        self.wfile.write(response.encode('utf-8'))
        
        # Print request details to console
        print("----------------------------------------------------")
        print(f"Received POST request: {self.path}")
        content_length = int(self.headers['Content-Length'])
        post_data = self.rfile.read(content_length)
        print(f"Headers: {self.headers}")
        print(f"Body: {post_data.decode('utf-8')}")
        
def run(server_class=HTTPServer, handler_class=SimpleHTTPRequestHandler, port=80):
    server_address = ('', port)
    httpd = server_class(server_address, handler_class)
    print(f'Starting httpd server on port {port}...')
    httpd.serve_forever()
if __name__ == "__main__":
    run()
# To run this server, save the code in a file named `a.py` and execute it using Python.
# You can then send GET and POST requests to `http://localhost:8080/` to see the responses and request details in the console.