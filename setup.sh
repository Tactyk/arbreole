#!/bin/bash

#Color variables
R=`tput setaf 1`  #Set text color to RED
G=`tput setaf 2`  #Set text color to GREEN
Y=`tput setaf 3`  #Set text color to YELLOW
RESET=`tput sgr0` #Reset text color

#Config variables
config_path=config/config.ini

echo "--------------------------------"
echo "-- Setup the Arbreole project --"
echo "--------------------------------"

echo -e "\n${Y}* Generating ./$config_path file${RESET}"
IP=$(ifconfig | grep "inet" | grep -v "inet6" | grep -v "127.0.0.1" | grep -oE '[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}' | head -1)

mkdir -p config
cat > $config_path << EOF
# Auto-generated file -- Do not edit
[server]
ServerIP=$IP
ServerPort=8080
EOF
echo "${G}*** SUCCESS: ./$config_path file was generated ***${RESET}"
