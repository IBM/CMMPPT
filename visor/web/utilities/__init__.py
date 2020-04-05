import json
import logging
import os
import sys
import subprocess
import threading
import psutil
import time
from io import StringIO


def getLogger(name, level=logging.INFO):
    logger = logging.getLogger(name)
    if not len(logger.handlers):
        logger.setLevel(level)
        # logger.propagate=False
        handler = logging.StreamHandler()
        handler.setFormatter(logging.Formatter(
            '%(asctime)s - %(name)s - %(levelname)s - %(message)s'))
        logger.addHandler(handler)
        filehandler = logging.FileHandler("recv.log")
        filehandler.setFormatter(logging.Formatter(
            '%(asctime)s - %(name)s - %(levelname)s - %(message)s'))
        logger.addHandler(filehandler)
        logger.propagate = False
    return logger


class Config(object):
    class o(object):
        def __init__(self, name, value):
            self.__name__ = name
            self.__value__ = value
            pass

        def __str__(self, indent=0):
            io = StringIO()
            prefix = ' ' * indent
            for k in self.__dict__.keys():
                nl = '\n'
                if isinstance(getattr(self, k), self.__class__):
                    io.write(f'{prefix}{k}:{{{chr(10)}')
                    io.write(getattr(self, k).__str__(indent + 1))
                    io.write(f'{prefix}}}{chr(10)}')
                else:
                    io.write(f'{prefix}{k}: {str(getattr(self, k))}{chr(10)}')
            return io.getvalue()

        def __iter__(self):
            self.ii = list(self.__value__.keys())
            return self

        def __next__(self):
            try:
                return getattr(self, self.ii.pop())
            except Exception as e:
                raise StopIteration

    def __init__(self, c, defaults={}):
        config = defaults.copy()

        def mixin(c, cc, attr_):
            for k in c.keys():
                if type(c[k]) is dict:
                    s = self.o(k, c[k])
                    setattr(attr_, k, s)
                    if k not in cc:
                        cc[k] = {}
                    mixin(c[k], cc[k], s)
                else:
                    cc[k] = c[k]
                    setattr(attr_, k, c[k])

        mixin(c, config, self)
        mixin(config, {}, self)  # Make sure all attributes are added
        self.c = config
        pass

    def __getitem__(self, key):
        return self.config[key]

    def __str__(self):
        return json.dumps(self.config, indent=2)

    def getpath(self, path):
        p = path.split("/")
        cc = self.c
        for k in p:
            cc = cc[k]
        return cc


def timeit(f, *args, **kwargs):
    t0 = time.time()
    result = f(*args, **kwargs)
    t1 = time.time()
    return (result, t1 - t0)


class myPipe:
    pipe = None
    buffer = ""

    def fileno(self):
        return self.pipe[1]

    def __init__(self):
        self.pipe = os.pipe()
        #         print("myPipe")
        return

    def print(self, str):
        return

    def readline(self):
        line = ""
        while (self.buffer.find('\n') == -1):
            next = self.read().decode("UTF-8")
            if (len(next) == 0):
                return self.buffer
            self.buffer = self.buffer + next
        if self.buffer.find('\n') != -1:
            line = self.buffer[0:self.buffer.find('\n')]
            self.buffer = self.buffer[self.buffer.find('\n') + 1:]
        return line

    def read(self):
        return os.read(self.pipe[0], 1024)

    def write(self, str):
        return os.write(self.pipe[1], str)


class processMonitor(object):
    def __init__(self, command):
        self.command = command
        self.command.semaphores[0].acquire()
        self.pid = command.process.pid
        self.process = psutil.Process(self.pid)

    def monitor(self):
        self.command.semaphores[1].release()
        self.command.cpu_times = [0, 0]
        while (psutil.pid_exists(self.process.pid)):
            self.command.cpu_times = self.process.cpu_times()
            message = "%d: %s : %f %f " % (
            self.process.pid, self.process.status(), self.command.cpu_times[0], self.command.cpu_times[1])
            print(message)
            self.command.callback(threading.currentThread().name, "stdout", message, 0)
            time.sleep(5)
        message = "%d: %s : %f %f " % (
            self.process.pid, 0, self.command.cpu_times[0], self.command.cpu_times[1])
        self.command.callback(threading.currentThread().name, "stdout", message, 0)
        message = "Done"
        self.command.callback(threading.currentThread().name, "stdout", message, 0)
        return


class Command(object):
    def __init__(self, command=None, callback=None, env=None):
        self.command = command
        self.callback = callback
        self.env = env
        self.semaphores = [threading.Semaphore(0), threading.Semaphore(0)];
        return

    def run(self):
        self.runCommand(self.command, self.callback, self.env)

    def runCommand(self, command, callback, env):
        self.semaphores[0].acquire()
        verbose = False
        threadName = threading.currentThread().name
        if verbose:
            callback(threading.currentThread().name, "stdout", '%s: %s: %s' % (threadName, "stdout", command), 0)
        stdout = myPipe()
        stderr = myPipe()
        #     p1 = subprocess.Popen(["bash", "-c", command],stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        p1 = subprocess.Popen(["bash", "-c", command], stdout=stdout, stderr=stderr, env=env)
        self.process = p1
        self.semaphores[1].release()

        def worker(name, pipe, process, threadName):
            if verbose:
                callback(threading.currentThread().name, "stdout", '%s: %s: %s' % (threadName, name, worker), 0)
                callback(threading.currentThread().name, "stdout", '%s: %s: %s' % (threadName, name, command), 0)
            while (True):
                line = pipe.readline()
                if len(line) > 0:
                    #                 print("%s: %s" % (name,line))
                    if callback:
                        callback(threadName, name, line, 0)
                else:
                    if not process.poll() is None:
                        callback(threadName, name, line, 1)
                        return
            return

        t1 = threading.Thread(target=worker, args=["stdout", stdout, p1, threadName], daemon=True)
        t2 = threading.Thread(target=worker, args=["stderr", stderr, p1, threadName], daemon=True)
        t1.start()
        t2.start()
        if verbose:
            callback(threading.currentThread().name, "stdout",
                     "%s: %s: %d" % (threading.currentThread().name, "Got here 1", p1.pid), 0)
            print("%s: %s: %d" % (threading.currentThread().name, "Got here 1", p1.pid))
        p1.wait()
        if verbose:
            callback(threading.currentThread().name, "stdout",
                     "%s: %s: %d" % (threading.currentThread().name, "Got here 2", p1.pid), 0)
            print("%s: %s: %d" % (threading.currentThread().name, "Got here 2", p1.pid))
        os.close(stdout.pipe[1])
        os.close(stderr.pipe[1])
        t1.join()
        t2.join()
        if verbose:
            callback(threading.currentThread().name, "stdout",
                     "%s: %s: %d" % (threading.currentThread().name, "Got here 3", p1.pid), 0)
            print("%s: %s: %d" % (threading.currentThread().name, "Got here 3", p1.pid))


def callback(threadName, name, line, status):
    if status == 0:
        print("%s: %s: %s" % (threadName, name, line))
        sys.stdout.flush()
    else:
        print("%s: %s: %s" % (threadName, name, "done"))
        sys.stdout.flush()


class Dataframe(object):
    #
    # Generate MODO' where D diagonal, O orthogonal, M'M=I
    # Range of values
    def __init__(self, config):
        name = config.data.dataframe.parameters.name
        self.Xdf = locals()[name]
        m, n = self.Xdf.shape
        self.ydf = self.Xdf.iloc[:, -1]
        self.X = None
        self.Y = None
        pass

    def matrix(self):
        return self.X

    def rhs(self):
        return self.y


