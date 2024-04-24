from AbstractIperf import AbstractIperf
from PySide6.QtCore import QProcess


class IperfClient(AbstractIperf):
    def __init__(self):
        self.process = QProcess()

    def process_output(self):
        print('IperfClient started')

    def process_started(self):
        print('IperfClient started')

    def process_finished(self):
        print('IperfClient finished')
