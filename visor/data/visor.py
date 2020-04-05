import os
import sys
import subprocess
import threading
import psutil
import time
import openpyxl
import pandas as pd
import re
import csv

stdout=""
stderr=""
PSQL='psql -U cmmppt '
def worker(name, array, pipe, proc) :
    global stdout, stderr
    data=pipe.read()
    array+=data.decode("UTF-8")
    if name == "stdout":
        stdout=array
    if name == "stderr":
        stderr=array
    pass
def runbashcommand(command, **kwargs) :
    return runcommand(["bash","-c", command], **kwargs)

def runcommand(command, **kwargs) :
    #
    # run command with output stored in arrays
    #
    with subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, **kwargs) as proc:
        global stdout, stderr
        stdout=""
        stderr=""
        t1 = threading.Thread(target=worker, args=["stdout", stdout, proc.stdout, proc], daemon=True)
        t2 = threading.Thread(target=worker, args=["stderr", stderr, proc.stderr, proc], daemon=True)
        t1.start()
        t2.start()
        t1.join()
        t2.join()
    return stdout,stderr
def dumpdata(directory) :
    if not os.path.exists(directory):
        os.makedirs(directory)
    files=["printer.csv", "requestQuantity.csv", "onHandMaterial.csv"]
    commands=['COPY printer(name, location, "prodRate", "F175", "F285", "PETG", "PLA", "ABS", "ONYX") TO STDOUT WITH (FORMAT CSV, HEADER, FORCE_QUOTE(name,location,"F175", "F285", "PETG", "PLA", "ABS", "ONYX"));',
              'COPY requestQuantity TO STDOUT  WITH (FORMAT CSV, HEADER, FORCE_QUOTE(location));',
              'COPY onHandMaterial TO STDOUT WITH (FORMAT CSV, HEADER, FORCE_QUOTE(location, "filamentSize", "plasticType", "quantity", "share"));']
    for k,command in enumerate(commands):
        command=f'psql -U cmmppt -c \'{command}\''
        #    print( command)
        (stdout,stderr)=runbashcommand(command)
        #    print(stdout)
        #    print(stderr)
        lines=stdout.split("\n")
        f = open(os.path.join(directory,files[k]),"w")
        for i,l in enumerate(lines):
            #print(f'{i} {l}')
            if i==0:
                print('"'+l.replace(',','","')+'"')
                f.write('"'+l.replace(',','","')+'"')
                f.write('\n')
            else:
                print(l.replace('"t"','"yes"').replace('"f"','"no"'))
                f.write(l.replace('"t"','"yes"').replace('"f"','"no"'))
                f.write('\n')
        f.close()
def clearresultstables() :
    for t in ['prodVol','subVol','shipQuantity']:
        (stdout,stderr)=runbashcommand(PSQL+f'-c "DELETE FROM {t}"')
    
def loadresults(directory) :
    prodVolPath=os.path.abspath(os.path.join(directory,'prodVol.csv'))
    command='COPY prodVol ("printerName","printerLocation","period","quantity") FROM '+"'"+prodVolPath+"' DELIMITER ',' CSV HEADER;"
    f = open("temp.sql","w")
    f.write(command)
    f.close()
    command='psql -U cmmppt -f temp.sql'
    (stdout,stderr)=runbashcommand(command)
    print(stdout)
    print(stderr)
    subVolPath=os.path.abspath(os.path.join(directory,'subVol.csv'))
    command='COPY subVol ("printerName","printerLocation","materialLocation","filamentSize","plasticType","period","quantity","own") FROM '+"'"+subVolPath+"' DELIMITER ',' CSV HEADER;"
    f = open("temp.sql","w")
    f.write(command)
    f.close()
    command='psql -U cmmppt -f temp.sql'
    (stdout,stderr)=runbashcommand(command)
    print(stdout)
    print(stderr)
    shipQuantityPath=os.path.abspath(os.path.join(directory,'shipQuantity.csv'))
    command='COPY shipQuantity ( "requestingLocation","producingLocation","period","shipQuantity") FROM '+"'"+shipQuantityPath+"' DELIMITER ',' CSV HEADER;"
    f = open("temp.sql","w")
    f.write(command)
    f.close()
    command='psql -U cmmppt -f temp.sql'
    (stdout,stderr)=runbashcommand(command)
    print(stdout)
    print(stderr)
def runvisor(directory):
    command = f'../linux/visor {directory} opt'
    (stdout,stderr)=runbashcommand(command)
    print(stdout)
    print(stderr)    
if __name__=='__main__':
    directory='dumpdata'
    dumpdata(directory)
    runvisor(directory)
    clearresultstables()
    loadresults(directory)
