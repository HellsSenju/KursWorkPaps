import PyQt5

from AbstractIperf import AbstractIperf
from typing import List
# from PySide6.QtCore import QObject, Signal, Slot, QProcess
from PyQt5.QtCore import QProcess, QObject, pyqtSignal, pyqtSlot


class IperfServer(AbstractIperf, QObject):
    server_started = pyqtSignal(bool)

    def __init__(self, program: str, args: list[str], parent=None):
        super().__init__()
        self.process = QProcess()
        self.process.started.connect(self.process_started)
        self.process.finished.connect(self.process_finished)
        self.process.readyRead.connect(self.process_output)

        self.program = program
        self.args = args

        self.parent = parent

    def start(self):
        self.process.start(self.program, self.args)

    @pyqtSlot()
    def process_output(self):
        output = self.process.readAll()
        print(output)

    @pyqtSlot()
    def process_started(self):
        print('IperfServer started')
        self.server_started.emit(True)

    @pyqtSlot()
    def process_finished(self):
        print('IperfServer finished')
