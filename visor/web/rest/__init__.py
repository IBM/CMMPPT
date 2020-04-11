from flask import Flask, render_template, request, jsonify, url_for, Response, redirect, session
import json
import os
import sys

print(sys.path)
from utilities import getLogger
from queue import Queue
import threading
import base64
import pandas as pd
import numpy as np
import traceback
from io import StringIO
import re
import time
import subprocess

# from flask_socketio import SocketIO
# from flask_socketio import send, emit
from logging import Handler, DEBUG, INFO

logger = getLogger('edu.cornell')
app = Flask(__name__, root_path=os.path.abspath(os.path.curdir))


# socketio = SocketIO(app)

def worker(name, array, pipe, proc):
    global stdout, stderr
    data = pipe.read()
    array += data.decode("UTF-8")
    if name == "stdout":
        stdout = array
    if name == "stderr":
        stderr = array
    pass


def runbashcommand(command, **kwargs):
    return runcommand(["bash", "-c", command], **kwargs)


def runcommand(command, **kwargs):
    #
    # run command with output stored in arrays
    #
    with subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, **kwargs) as proc:
        global stdout, stderr
        stdout = ""
        stderr = ""
        t1 = threading.Thread(target=worker, args=["stdout", stdout, proc.stdout, proc], daemon=True)
        t2 = threading.Thread(target=worker, args=["stderr", stderr, proc.stderr, proc], daemon=True)
        t1.start()
        t2.start()
        t1.join()
        t2.join()
    return stdout, stderr


def getsocketio():
    global socketio
    return socketio


# clients=[]
applicationinstance = None


def getResponse(e, mimetype='application/json'):
    if isinstance(e, Exception):
        return Response(json.dumps({"status": 1,
                                    "message": str(e),
                                    "traceback": traceback.format_exception(*sys.exc_info())}
                                   ),
                        status=500)
    else:
        return Response(json.dumps(e), status=200, mimetype=mimetype, direct_passthrough=False)


q = Queue()
threads = []


class application(object):
    instance = None
    emit = None
    clients = None

    def __init__(self):
        self.instance = self
        # self.emit = emit
        self.clients = []

    # app = application
    def send_static_file(self, filename):
        if not applicationinstance.emit is None:
            applicationinstance.emit('send_static_file', {'stdout': filename, 'stderr': None},
                                     **applicationinstance.clients[0])
        return app.send_static_file(filename)

    # @staticmethod
    # def callback(threadName, name, line, status):
    #     print(line)
    #     q.put((threadName, name, line, status))
    #     return

    # @staticmethod
    # def runcommand(command):
    #     try:
    #         q = Queue()
    #
    #         def callback(threadName, name, line, status):
    #             print(line)
    #             q.put({"threadName": threadName, "name": name, "line": line, "status": status})
    #             return
    #
    #         c = Command(command=command, callback=callback,
    #                     env={**os.environ})
    #         c.semaphores[0].release()
    #         c.run()
    #
    #         lines = []
    #         while not q.empty():
    #             clients = applicationinstance.clients
    #             #        print(clients[0])
    #             line = q.get()
    #             if False:
    #                 #          print(clients[0])
    #                 emit('line', line, **clients[0])
    #                 time.sleep(0)  # attempt to get emit to actually fire
    #             lines.append(line)
    #         return lines
    #     except Exception as e:
    #         return e

    # @staticmethod
    # @app.route('/', methods=['GET'])
    # def indexhtml():
    #     return app.send_static_file('index.html')
    # @staticmethod
    # @app.route('/node_modules/<p>', methods=['GET'])
    # def node_modules(p):
    #     return app.send_static_file(f'/node_modules/{p}')
    # @staticmethod
    # @app.route('/node_modules', methods=['GET'])
    # def node_modules0():
    #     return app.send_static_file(f'/node_modules/{p}')
    @staticmethod
    @app.route('/visor/hello', methods=['GET'])
    def hello():
        return "hello"

    @staticmethod
    def getinsertsql(table, form):
        values = [f['value'] for f in form.values()]
        sql = f'INSERT INTO {table} ("'
        sql += '","'.join(form.keys()) + '"' + ") VALUES ('" + "','".join(values) + "')"
        sql = sql.replace('"', '\\"')
        return sql

    @staticmethod
    def getupdatesql(table, form):
        sql = f'INSERT INTO {table} ("'
        sql += '","'.join(form.keys()) + '"' + ") VALUES ('" + "','".join(form.values()) + "')"
        sql = sql.replace('"', '\\"')
        return sql

    @staticmethod
    def getdeletesql(table, form):
        sql = f'INSERT INTO {table} ("'
        sql += '","'.join(form.keys()) + '"' + ") VALUES ('" + "','".join(form.values()) + "')"
        sql = sql.replace('"', '\\"')
        return sql

    @staticmethod
    def getsqlcommand(request, table, form):
        if request.args['action'] == 'add':
            sql = application.getinsertsql(table, form)
            command = 'psql -h 13.59.186.14 -p 15432 -U cmmppt -c "' + sql + '"'
        if request.args['action'] == 'update':
            sql = application.getupdatesql(table, form)
            command = 'psql -h 13.59.186.14 -p 15432 -U cmmppt -c "' + sql + '"'
            command = None
        if request.args['action'] == 'update':
            sql = application.getdeletesql(table, form)
            command = 'psql -h 13.59.186.14 -p 15432 -U cmmppt -c "' + sql + '"'
            command = None
        return sql, command

    @staticmethod
    @app.route('/visor/printer', methods=['POST'])
    def printer():
        table = 'printer'
        form = json.loads(request.form['json'])
        sql, command = application.getsqlcommand(request,table,form)
        if command:
            (stdout, stderr) = runbashcommand(command)
            payload = {'status': 0, 'sql': sql, 'stdout': stdout, 'stderr': stderr}
        else:
            payload = {'status': 0, 'sql': sql, 'stdout': "", 'stderr': "Not run"}
        return payload

    @staticmethod
    @app.route('/visor/material', methods=['POST'])
    def material():
        table = 'onhandmaterial'
        form = json.loads(request.form['json'])
        sql, command = application.getsqlcommand(request,table,form)
        if command:
            (stdout, stderr) = runbashcommand(command)
            payload = {'status': 0, 'sql': sql, 'stdout': stdout, 'stderr': stderr}
        else:
            payload = {'status': 0, 'sql': sql, 'stdout': "", 'stderr': "Not run"}
        return payload

    @staticmethod
    @app.route('/visor/request', methods=['POST'])
    def request():
        table = 'requestquantity'
        form = json.loads(request.form['json'])
        sql, command = application.getsqlcommand(request, table, form)
        if command:
            (stdout, stderr) = runbashcommand(command)
            payload = {'status': 0, 'sql': sql, 'stdout': stdout, 'stderr': stderr}
        else:
            payload = {'status': 0, 'sql': sql, 'stdout': "", 'stderr': "Not run"}
        return payload
    @staticmethod
    @app.route('/visor/sql', methods=['POST'])
    def sql():
        form = json.loads(request.form['json'])
        sql = form['sql']['value']
        sql = sql.replace('"', '\\"')
        command = 'psql -h 13.59.186.14 -p 15432 -U cmmppt -c "' + sql + '"'
        (stdout, stderr) = runbashcommand(command)
        payload = {'status': 0, 'sql': sql, 'stdout': stdout, 'stderr': stderr}
        return payload

    @staticmethod
    @app.route('/visor/senddata', methods=['POST'])
    def senddata(self):
        try:
            data = request.json
            X = data['X']
            y = data['y']
            beta = data['beta']
            filepath = data['filepath']
            print(filepath)
            application.filepath = filepath
            os.makedirs(filepath, exist_ok=True)
            xfilename = os.path.abspath(os.path.join(filepath, 'X.csv'))
            np.savetxt(xfilename, X, delimiter=',')
            yfilename = os.path.abspath(os.path.join(filepath, 'y.csv'))
            np.savetxt(yfilename, y, delimiter=',')
            betafilename = os.path.abspath(os.path.join(filepath, 'beta.csv'))
            np.savetxt(betafilename, beta, delimiter=',')
            print(xfilename, yfilename, betafilename)
            result = 'OK'
            return getResponse(
                {"contents": {"xfilename": xfilename, "yfilename": yfilename, "betafilename": betafilename},
                 "encoding": "base64"})
        except Exception as e:
            return getResponse(e)

    @app.route('/visor/run', methods=['POST'])
    def regressionrun(self):
        try:
            data = request.json
            xfilename = data["xfilename"]
            yfilename = data["yfilename"]
            parameters = data["parameters"]
            print(parameters)
            options = ''
            if 'useLU' in parameters and parameters['useLU']:
                options = options + ' --useLU'
            if 'batchsize' in parameters:
                options = options + f' --set-training-batch-size {parameters["batchsize"]}'
            #      lines=applicationinstance.runcommand("(cd /cn-insight/demo;ls -l)")
            lines = applicationinstance.runcommand(f'(cd /cn-insight/demo;./run.sh {xfilename} {yfilename} {options})')
            #      print(lines)
            #      'ls -lR /cn-insight/demo/.out'
            results = {}
            path = '/cn-insight/demo/.out'
            for r, d, f in os.walk(path):
                #    print(f'r: {r},d: {d},f: {f}')
                for file_ in f:
                    result = re.match("client.*[.]csv", file_)
                    if result is not None:
                        print(r, d, f)
                        rr = os.path.basename(r)
                        if rr not in results:
                            results[rr] = {}
                        results[rr][file_] = np.loadtxt(os.path.join(r, file_)).tolist()
            print(results)
            print(lines)
            return getResponse({"status": 0, "message": "so far so good", "data": lines, "results": results})
        except Exception as e:
            traceback.print_exc(file=sys.stdout)
            logger.error(e)
            return getResponse(e)

    @app.route('/visor/runos', methods=['POST'])
    def runos(self):
        try:
            data = request.json
            command = data["command"]
            lines = applicationinstance.runcommand(command)
            return getResponse({"status": 0, "data": lines})
        except Exception as e:
            traceback.print_exc(file=sys.stdout)
            return getResponse(e)


applicationinstance = application()
