#!/bin/sh

PKG_OK=$(dpkg-query -W --showformat='${Status}\n' inotify-tools|grep "install ok installed")
echo Checking for inotify-tools: $PKG_OK
if [ "" == "$PKG_OK" ]; then
  echo "No somelib. Setting up somelib."
  sudo apt-get --force-yes --yes install inotify-tools
fi

while inotifywait -e modify SDK_test.py SEAL.py ; do
scp SDK_test.py SEAL.py pi@192.168.23.105:/home/pi/dodo/SEAL-SDK ; ssh -t pi@192.168.23.105 "python3 /home/pi/dodo/SEAL-SDK/SDK_test.py -h"
#&& python3 /home/pi/dodo/SEAL-SDK/SDK_test.py -i \"rsp\" -s
done