from typing import List
from AbstractIperf import AbstractIperf
from flask import Flask, request
from IperfServer import IperfServer
import json
from PyQt5.QtCore import QProcess


app = Flask(__name__)


def start():
    print("DDDDDDDD")


processes: List[AbstractIperf] = []
pr = QProcess()
pr.started.connect(start)


@app.route('/server/start', methods=['POST'])
def start_iperf_as_server():
    body: dict = request.get_json().get('server')
    args: list[str] = ['-s']

    if "port" in body:
        args.append(f'-p {str(body["port"])}')
    if "protocol" in body:
        args.append(f'{str(body["protocol"])}')
    if "interval" in body:
        args.append(f'-i {str(body["interval"])}')
    if "time" in body:
        args.append(f'-t {str(body["time"])}')

    pr.start("iperf", args)

    print(pr.state().Starting)
    print(pr.state())
    # iperf_server = IperfServer("iperf", args)
    # iperf_server.server_started.connect(lambda _: iperf_server.start())
    # processes.append(iperf_server)
    # iperf_server.start()

    return json.dumps({'success': True}), 200, {'ContentType': 'application/json'}




if __name__ == '__main__':
    app.run()
