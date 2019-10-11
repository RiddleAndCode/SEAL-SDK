#!/bin/sh
TARGET_IP = 192.168.0.214


PKG_OK=$(dpkg-query -W --showformat='${Status}\n' inotify-tools|grep "install ok installed")
echo Checking for inotify-tools: $PKG_OK
if [ "" == "$PKG_OK" ]; then
  echo "No somelib. Setting up somelib."
  sudo apt-get --force-yes --yes install inotify-tools
fi

cat  ~/.ssh/id_rsa.pub | ssh pi@$TARGET_IP -T "cat > ~/.ssh/authorized_keys"
ssh -t pi@$TARGET_IP "chmod 600 ~/.ssh/authorized_keys"

while inotifywait -e modify SDK_test.py SEAL.py ; do
scp SDK_test.py SEAL.py pi@$TARGET_IP:/home/pi/SEAL-SDK ;
#ssh -t pi@$TARGET_IP "python3 /home/pi/dodo/SEAL-SDK/SDK_test.py -h"
#&& python3 /home/pi/dodo/SEAL-SDK/SDK_test.py -i \"rsp\" -s
done