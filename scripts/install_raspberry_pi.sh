#!/usr/bin/env bash

for hostname in `cat ./scripts/raspberry_hostnames`; do
  echo "Installing raspberryPi with IP: " $hostname
  echo "Creating folders into raspberryPi"
  ssh $hostname "mkdir -p ~/arbreole/src"
  echo "Copying local project to remote"
  scp -p -r ./client/* $hostname:~/arbreole/src/
  echo "--- Done ---"
done

