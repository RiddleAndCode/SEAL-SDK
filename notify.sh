#!/bin/sh


while inotifywait -e modify SDK_test.py SEAL.py ; do
scp SDK_test.py SEAL.py pi@192.168.23.105:/home/pi/dodo/SEAL-SDK
done