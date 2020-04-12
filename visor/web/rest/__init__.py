import csv
import json
import os
import sys

from flask import Flask, request, Response, redirect, session
from functools import reduce

print(sys.path)
from utilities import getLogger
from queue import Queue
import threading
import numpy as np
import traceback
import re
import subprocess
import pandas as pd

# from flask_socketio import SocketIO
# from flask_socketio import send, emit

SQLHOST = '3.15.33.87'
INDEXHTML='/static/html/index.html'
LOGINHTML='/static/html/login.html'
logger = getLogger('edu.cornell')
app = Flask(__name__, root_path=os.path.abspath(os.path.curdir))
app.secret_key = 'abcd123456789efgh'


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
    def sqltojson(sql):
        return 'SELECT json_agg(row_to_json(t)) FROM (' + sql + ') as t'
    @staticmethod
    def runsql(sql):
        command = f'psql -h {SQLHOST} -p 15432 -U cmmppt -t -c "' + sql + '"'
        return runbashcommand(command)

    @staticmethod
    @app.route('/visor/hello', methods=['GET'])
    def hello():
        return "hello"

    @staticmethod
    def getinsertsql(table, form):
        values = [str(f['value']) for f in form.values()]
        sql = f'INSERT INTO {table} ("'
        sql += '","'.join(form.keys()) + '"' + ") VALUES ('" + "','".join(values) + "')"
        sql = sql.replace('"', '\\"')
        return sql

    @staticmethod
    def getupdatesql(table, form):
        where = []
        set = []
        for e in form:
            if form[e]['keyfield']:
                where.append(f'"{e}"' + "='" + form[e]['value'] + "'")
            else:
                set.append(f'"{e}"' + "='" + str(form[e]['value']) + "'")
        where = ' and '.join(where)
        set = ', '.join(set)
        sql = f'UPDATE  {table}  SET {set} WHERE {where} '
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
            # command = f'psql -h {SQLHOST} -p 15432 -U cmmppt -c "' + sql + '"'
        if request.args['action'] == 'update':
            sql = application.getupdatesql(table, form)
            # command = f'psql -h {SQLHOST} -p 15432 -U cmmppt -c "' + sql + '"'
            # command = None
        if request.args['action'] == 'delete':
            sql = application.getdeletesql(table, form)
            command = f'psql -h {SQLHOST} -p 15432 -U cmmppt -c "' + sql + '"'
            sql=None
        return sql
    @staticmethod
    def gettable(table) :
        sql = f'select * from {table}'
        (stdout, stderr) = application.runsql(application.sqltojson(sql))
        sql = f'select column_name from information_schema.columns where table_name=\'{table}\''
        rows = stdout
        error = stderr
        (stdout, stderr) = application.runsql(application.sqltojson(sql))
        error += stderr
        columns = stdout
        return {'status': 0, 'columns': columns, 'rows': rows, 'error': error}
    @staticmethod
    @app.route('/visor/table', methods=['GET'])
    def table():
        return application.gettable(request.args['table'])
    @staticmethod
    @app.route('/visor/download', methods=['GET'])
    def download():
        table=request.args['table']
        data= application.gettable(table)
        rows=json.loads(data['rows'])
        columns=json.loads(data['columns'])
        location=None
        dictionary = {}
        csvrows=[]
        header=[]
        for i,d in enumerate(columns):
            header.append(d['column_name'])
            dictionary[d['column_name']]=i
        for r in rows:
            row=[0]*len(columns)
            for k in r.keys():
                row[dictionary[k]] = r[k]
            csvrows.append(row)
        df = pd.DataFrame(csvrows, columns=header)
        user = session['user']
        os.makedirs(f'static/data/{user}', 0o755, True)
        location=f'static/data/{user}/{table}.csv'
        df.to_csv(location,index=False, quoting=csv.QUOTE_NONNUMERIC)
        return {"status":0, "url": f'/{location}'}
    @staticmethod
    @app.route('/visor/printer', methods=['POST'])
    def printer():
        table = 'printer'
        form = json.loads(request.form['json'])
        sql = application.getsqlcommand(request, table, form)
        if sql:
            (stdout, stderr) = application.runsql(sql)
            payload = {'status': 0, 'sql': sql, 'stdout': stdout, 'stderr': stderr}
        else:
            payload = {'status': 0, 'sql': sql, 'stdout': "", 'stderr': "Not run"}
        return payload

    @staticmethod
    @app.route('/visor/material', methods=['POST'])
    def material():
        table = 'onhandmaterial'
        form = json.loads(request.form['json'])
        sql = application.getsqlcommand(request, table, form)
        if sql:
            (stdout, stderr) = application.runsql(sql)
            payload = {'status': 0, 'sql': sql, 'stdout': stdout, 'stderr': stderr}
        else:
            payload = {'status': 0, 'sql': sql, 'stdout': "", 'stderr': "Not run"}
        return payload

    @staticmethod
    @app.route('/visor/request', methods=['POST'])
    def request():
        table = 'requestquantity'
        form = json.loads(request.form['json'])
        sql = application.getsqlcommand(request, table, form)
        if sql:
            (stdout, stderr) = application.runsql(sql)
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
        (stdout, stderr)=application.runsql(application.sqltojson(sql))
        payload = {'status': 0, 'sql': sql, 'stdout': stdout, 'stderr': stderr}
        return payload

    @staticmethod
    def savedata(name, data):
        f = open(name, "wb")
        f.write(data)
        f.close()
        return

    @staticmethod
    @app.route('/visor/senddata', methods=['POST'])
    def senddata():
        try:
            files = request.files
            for k in files.keys():
                filestorage = files[k]
                data = filestorage.read()
                application.savedata(k, data)
            # return getResponse(
            #     {"contents": {"xfilename": xfilename, "yfilename": yfilename, "betafilename": betafilename},
            #      "encoding": "base64"})
            return {'status': 0}
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

    @staticmethod
    @app.route('/visor/login', methods=['POST'])
    def login():
        password = request.form['psw']
        user = request.form['uname']
        (stdout, stderr)  = application.runsql(application.sqltojson('SELECT * FROM USERS WHERE "name"=' + "'" + user + "'"))
        if len(stdout.strip()) == 0:
            return {"status": 1, "message": "Unknown user"}
        result = json.loads(stdout)
        result = result[0]
        if result['password'] == password:
            (stdout, stderr)  = application.runsql(application.sqltojson('select roles.role from users INNER JOIN roles on users.name=roles.name where users.name='
                                                                         + "'" + user + "'"))
            session['user'] = user
            session['password'] = password
            session['roles'] = reduce(lambda a, v: a + v, (map(lambda d: list(d.values()), json.loads(stdout))))
            return {"status": 0, "url": INDEXHTML}
        return {"status": 1, "message": "Incorrect password"}

    @staticmethod
    @app.route('/visor/authenticate', methods=['GET'])
    def authenticate():
        if 'user' in session and 'password' in session:
            return {'status': 0, 'roles': session['roles']}
        return {'status': 1, 'url': LOGINHTML}

    @staticmethod
    @app.route('/visor/logout', methods=['GET'])
    def logout():
        session.pop('user')
        session.pop('password')
        return {'status': 1, 'url': LOGINHTML}

    @staticmethod
    @app.route('/visor/tables', methods=['GET'])
    def tables():
        sql="select table_name from information_schema.tables where table_catalog='cmmppt' and table_schema='public'"
        (stdout, stderr) = application.runsql(application.sqltojson(sql))
        return {'status': 0, 'sql': sql, 'stdout': stdout, 'stderr': stderr}

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
