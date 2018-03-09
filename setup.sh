#!/bin/sh

IP=$(ifconfig | grep "inet" | grep -v "inet6" | grep -v "127.0.0.1" | grep -oE '[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}' | head -1)

# Config template
cat > config/config.ini << EOF
# Auto-generated file -- Do not edit
[server]
ServerIP=$IP
ServerPort=8080
EOF
