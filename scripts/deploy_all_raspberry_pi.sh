#!/usr/bin/env bash

R=`tput setaf 1`  #Set text color to RED
G=`tput setaf 2`  #Set text color to GREEN
Y=`tput setaf 3`  #Set text color to YELLOW
RESET=`tput sgr0` #Reset text color

#This script copies the local source code to all the raspberryPis that are in the raspberry_hostnames file
echo "--------------------------------------------------------"
echo "-- Deploying local code to all connected RaspberryPis --"
echo "--------------------------------------------------------"


#IFS stands for "internal field separator". It is used by the shell to determine how to do word splitting, i. e. how to recognize word boundaries.
while IFS=' ': read -r user hostname || [[ -n "$user" ]]; do
    echo -e "\n${Y}* Pinging hostname: $hostname${RESET}"
    ping -q -c1 $hostname &> /dev/null #&> /dev/null hide the output of the ping command, including errors.
    if [ $? -eq 0 ]; then
        #ping is successful
        echo "${G}* RaspberryPi $hostname is reachable${RESET}"
        echo "${Y}** Deleting remote code and deploying local code to RaspberryPi: $hostname${RESET}"
        ssh $user@$hostname "rm -rf ~/arbreole/src && rm -rf ~/arbreole/config"
        scp -p -r ./client $user@$hostname:~/arbreole/src
        scp -p -r ./config $user@$hostname:~/arbreole/config
        echo "${G}*** SUCCESS: $hostname was deployed ***${RESET}"
    else
        echo "${R}** FAIL: impossible to connect to RaspberryPi: $hostname${RESET}"
    fi
done < "./scripts/raspberry_hostnames"

echo -e "\n${G}--------------------------------------------------------------------${RESET}"
echo "${G}-- The local code has been deployed to all connected RaspberryPis --${RESET}"
echo "${G}--------------------------------------------------------------------${RESET}"
