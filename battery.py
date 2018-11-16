#!/usr/bin/python

import re
import subprocess
import serial
import time
import os

while True:

    port = "";
    while len(port) == 0:
        port = re.sub('\n','',subprocess.check_output('/.../port.sh').decode())
        os.system('beep')
        time.sleep(0.5);

    ser = serial.Serial("/dev/" + port,115200)
    print "hi"
    ser.write('1')

    while True:
        try:
            result = subprocess.check_output('/.../power_info')
            result = result.decode()

            result = re.sub(' +','',result)
            result = result.strip()

            lines = result.split('\n')

            for line in lines:
                if line.find("percentage") != -1:
                    percentage_location = lines.index(line)

            split = lines[percentage_location].split(':')
            percentage = int(split[1].replace('%',''))
            state = lines[0].split(':')[1]

#            print percentage, state

            if percentage <= 50 and state != 'charging':
                ser.write('1')

            if state == 'fully-charged':
                ser.write('0')

            try:
                f = open('/.../Arduino_command')
                fl = f.readline()
                if len(fl) > 0:
                    print 'in', fl
                    ser.write(fl)
                    f = open('/.../Arduino_command','w')
                    f.write('')
                f.close()
            except Exception:
                break

        except Exception:
            break
        time.sleep(1)
