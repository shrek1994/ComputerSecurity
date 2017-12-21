import BaseHTTPServer, SimpleHTTPServer
import ssl
import os.path
import os
import mimetypes
from cgi import parse_header, parse_multipart
from urlparse import parse_qs
import sqlite3
import time

CERTIFICATE='./signed_certificate.student.pwr.edu.pl.pem'
MAIN_PAGE="login.html"
HOST_NAME='localhost'
PORT=80
DATABASE = sqlite3.connect('database.sql')

USER='b516bdde133b8628e160cb4b55698156ab76b2edbf3c84ed1825375772b94606' #maciek
PASSWORD='abe31fe1a2113e7e8bf174164515802806d388cf4f394cceace7341a182271ab' #haslo

def setupDatabase():
    cursor = DATABASE.cursor()
#    cursor.execute('CREATE TABLE users(user TEXT, password TEXT)')
#    cursor.execute('INSERT INTO users(user, password) VALUES(?,?)', (USER, PASSWORD))
#    DATABASE.commit()

def validate(user, password):
    cursor = DATABASE.cursor()
    print "user=" + user
    cursor.execute('SELECT password FROM users WHERE user=\'' + user + '\'')
    for row in cursor:
        print "pass=", row[0]  
        return password == row[0]
    return false

class Bank(SimpleHTTPServer.SimpleHTTPRequestHandler):
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
        #print("post request: " + self.path)
        postvars = self.parse_POST()
        print(postvars)
        if self.path == '/main_page.html':
            print "validate: ", validate(postvars['user'][0], postvars['password'][0])
            
        
        f = open('./data' + self.path)
        mimetype, _ = mimetypes.guess_type(self.path)
        self.send_response(200)
        self.send_header('Content-type', mimetype)
        self.end_headers()
        self.wfile.write(f.read())
        f.close()
   
    def do_GET(self):
        #print("request: " + self.path)
        if self.path == '/':
            f = open('./data/' + MAIN_PAGE)
        else:
            f = open('./data' + self.path)
        mimetype, _ = mimetypes.guess_type(self.path)
        self.send_response(200)
        self.send_header('Content-type', mimetype)
        self.end_headers()
        self.wfile.write(f.read())
        f.close()


setupDatabase()
httpd = BaseHTTPServer.HTTPServer((HOST_NAME, PORT), Bank)
#httpd.socket = ssl.wrap_socket(httpd.socket, certfile=CERTIFICATE, server_side=True)

print time.asctime(), "Server Starts - %s:%s" % (HOST_NAME, PORT)
try:
    httpd.serve_forever()
except KeyboardInterrupt:
    pass
DATABASE.close()
httpd.server_close()
print time.asctime(), "Server Stops - %s:%s" % (HOST_NAME, PORT)

