import socket

from flask import Flask, render_template, request

HOST = "127.0.0.1"
PORT = 8080

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    app = Flask(__name__)

    @app.route("/", methods=['POST', 'GET'])
    def index():
        if request.method == 'POST':
            request_data = request.form
            if "key" in request_data and len(request_data["key"]) == 1:
                data = request_data["key"]
                s.sendall(data.encode('ascii'))
                print(f"{data}")

        return render_template('index.html')