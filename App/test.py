
"""
import requests

r = requests.get('http://127.0.0.1:1234/hello')
print (r.status_code)
print (r.headers)
"""

from websocket import create_connection
ws = create_connection("ws://127.0.0.1:2345/web")
print("Sending 'Hello, World'...")
ws.send("Hello, World")
print("Sent")
print("Receiving...")
result =  ws.recv()
print("Received '%s'" % result)
ws.close()