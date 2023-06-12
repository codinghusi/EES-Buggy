import socket

from flask import Flask, render_template, request

HOST = "127.0.0.1"
PORT = 8080
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

s.connect((HOST, PORT))
app = Flask(__name__)

@app.route("/", methods=['POST', 'GET'])
def index():
    print("loading", request.method)
    if request.method == 'POST':
        request_data = request.form
        data = request_data[list(request_data.keys())[0]]
        s.sendall(data.encode())
        print(f"{data}")

    return render_template('index.html')
