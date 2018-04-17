#!/bin/bash

R=`tput setaf 1`  #Set text color to RED
G=`tput setaf 2`  #Set text color to GREEN
Y=`tput setaf 3`  #Set text color to YELLOW
RESET=`tput sgr0` #Reset text color

# This script must be executed from the root folder

# This script deploys the local source code to either:
# - a specific raspberry pi if ARGS $user and $host are given, ex: `./scripts/deploy_raspberry.sh pi raspberrypi.local`
# - all the raspberryPis that are in the raspberry_hosts file, ex: `./scripts/deploy_raspberry.sh`

deploy() {
    user=$1
    host=$2

    echo "${Y}* Deploying local code to RaspberryPi $user@$host${RESET}"
    ping -q -c1 $host &> /dev/null # &> /dev/null hide the output of the ping command, including errors.
    if [ $? -eq 0 ]; then
        #ping is successful
        echo "${G}* RaspberryPi $host is reachable${RESET}"
        echo "${Y}** Deleting remote code and deploying local code to RaspberryPi: $user@$host${RESET}"
        ssh $user@$host "rm -rf ~/arbreole/src && rm -rf ~/arbreole/config" < /dev/null # added "< /dev/null" so that the script goes to next file line
        scp -p -r ./client $user@$host:~/arbreole/src
        scp -p -r ./config $user@$host:~/arbreole/config
        echo "${G}*** SUCCESS: $host was deployed for user $user***${RESET}"
    else
        echo "${R}** FAIL: impossible to connect to RaspberryPi: $user@$host${RESET}"
    fi
}

deploy_all() {
    echo "-----------------------------------------------------------------------------"
    echo "-- Deploying code to RaspberryPis that are in the raspberry_hosts file --"
    echo "-----------------------------------------------------------------------------"

    #IFS stands for "internal field separator". It is used by the shell to determine how to do word splitting, i.e. how to recognize word boundaries.
    while IFS=' ': read -r user host || [[ -n "$user" ]]; do
        echo ""
        deploy $user $host
    done < "./scripts/raspberry_hosts"

    echo ""
    echo "${G}--------------------------------------------------------------------${RESET}"
    echo "${G}-- The local code has been deployed to all connected RaspberryPis --${RESET}"
    echo "${G}--------------------------------------------------------------------${RESET}"
}

if [ -z $1 ] || [ -z $2 ]; then
    deploy_all
else
    deploy $1 $2
fi
