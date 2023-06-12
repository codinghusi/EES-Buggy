import socket

from flask import Flask, render_template, request, current_app

HOST = "127.0.0.1"
PORT = 8080

app = Flask(__name__)
app.config.from_mapping(
    SOCKET=None
)

def send_over_socket(data):
    try:
        current_app.config["SOCKET"].send(data)
    except Exception as e:
        current_app.config["SOCKET"] = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        current_app.config["SOCKET"].connect((HOST, PORT))
        current_app.config["SOCKET"].send(data)

@app.route("/", methods=['POST', 'GET'])
def index():
    print("loading", request.method)
    if request.method == 'POST':
        request_data = request.form
        data = list(request_data.keys())[0]
        print(f"Sending {data}, {type(data)}, {len(data)}, {data.encode()}")
        send_over_socket(data.encode())

    return render_template('index.html')
