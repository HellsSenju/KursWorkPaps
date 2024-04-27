import abc


class AbstractIperf:
    __metaclass__ = abc.ABCMeta

    @abc.abstractmethod
    def process_output(self):
        pass

    @abc.abstractmethod
    def process_started(self):
        pass

    @abc.abstractmethod
    def process_finished(self):
        pass
