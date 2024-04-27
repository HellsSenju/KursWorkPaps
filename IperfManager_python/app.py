from typing import List

from flask import Flask, request
from flask_api import status
from PyQt5.QtCore import QProcess
import slots

app = Flask(__name__)

processes: List[QProcess] = []


@app.route('/server/start', methods=['POST'])
def start_iperf_as_server():
    body: dict = request.get_json()
    args: List[str] = ['-s']

    if "port" in body:
        args.append(f'-p {str(body["port"])}')
    if "protocol" in body:
        args.append(f'{str(body["protocol"])}')
    if "interval" in body:
        args.append(f'-i {str(body["interval"])}')
    if "time" in body:
        args.append(f'-t {str(body["time"])}')

    processes.append(QProcess())
    iperf_server = processes[len(processes) - 1]
    iperf_server.start("iperf", args)

    iperf_server.started.connect(slots.process_started())
    iperf_server.finished.connect(slots.process_finished())

    iperf_server.readyRead.connect(slots.process_output())

    if iperf_server.started
    return status.


if __name__ == '__main__':
    app.run()
