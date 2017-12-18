#!/usr/bin/env bash

for hostname in `cat ./raspberry_hostnames`; do
  echo $hostname
  scp -p -r ../client/webSocketClient.py $hostname:~/arbreole/src/
done
