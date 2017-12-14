#!/usr/bin/env bash

for hostname in `cat ./raspberry_hostnames`; do
  echo $hostname
  scp -p -r ../../raspberryPi/config/config.ini $hostname:~/Arbreole/src/config/
done

