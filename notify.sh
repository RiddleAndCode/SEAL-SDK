#!/bin/sh


while inotifywait -e modify se.py SEAL.py ; do
scp se.py SEAL.py pi@192.168.23.105:/home/pi/dodo/machine-identity-demo-data
done