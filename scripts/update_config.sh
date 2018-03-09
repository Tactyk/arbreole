#!/usr/bin/env bash

for hostname in `cat ./raspberry_hostnames`; do
  echo $hostname
  scp -p -r ../config/config.ini $hostname:~/arbreole/src/config/
done
