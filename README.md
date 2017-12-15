###Prerequisites
1) Download git :

    Mac: http://sourceforge.net/projects/git-osx-installer/

    Linux: `apt-get install git`
2) Ask project admins (Adrien or Yannick) for access to the project sources via ssh. You need to have an ssh key on your machine.
3) Having a monitor with HDMI cable for raspberryPi section.

###Get the project!

In your favorite terminal (for Mac we advice you to get iTerm2, for linux the default terminal with [ohmyzsh](https://github.com/robbyrussell/oh-my-zsh))

(1) Go to home folder (2) create a project directory and (3) retrieve data sources:

`cd ~` (tilde means home)

`mkdir arbreole && cd arbreole` (mkdir: make directory, cd: change directory)

`git clone git@bitbucket.org:supertactyk/architecture.git`

###Project architecture

You can now see the project structure in your 'arbreole' folder
``ls`` (i.e. list the current folder)

###Development tools

- Download and install [PyCharm Community Edition](https://www.jetbrains.com/pycharm/download/) 

- Configuration:

    Preferences > Project > Project Interpreter >
    1) Click on the setting wheel and
    2) Choose a Python 3 interpreter
    ![Choose a Python 3 interpreter](./_doc/img/python_interpreter.png)

###RaspberryPi (rPi) initialization:

Please follow [this link](https://www.imore.com/how-get-started-using-raspberry-pi) to format SD card and install Raspbian (Debian version for rPi)

###RaspberryPi (rPi) connection:

Please make sure that your raspberryPi is connected to the same wifi as your computer sharing connection between both via ethernet cable.

Linux (Ubuntu): https://roughlea.wordpress.com/raspberry-pi-experiences/configure-the-raspberry-pi-to-share-a-linux-internet-connection/

Mac: https://medium.com/@tzhenghao/how-to-ssh-into-your-raspberry-pi-with-a-mac-and-ethernet-cable-636a197d055` 

Find RaspberryPi IP:

- into the raspberryPi (need to cable a monitor with an HDMI), open a terminal and type
    `ifconfig`
- copy the raspberryPi (en0) IP  

- From your computer you can now connect to your raspberryPi via ssh.

`ssh pi@{raspberry-ip}.local` {raspberry-ip} means it is a variable that you have to change with the raspberry IP (192.23.98.10 for example)

###RaspberryPi (rPi) installation:

In this project, we use **python3**. In order order to install python3 packages, please use pip3 packages installer: 
- It should be already in rPi but nervertheless :

    - Update packages 
    
        `sudo apt-get update`

    - Install python 3
    
        `sudo apt-get install python3-pip`
        
- Install packages
    To communicate with the server, we will use websockets for each rPi, we gonna use webclient-socket client, a well-proof library for websocket used. 
    `sudo install websocket-client`  
    
    These packages will be installed in a  
    NB: If you want to list all python packages install for python 3 please type `pip3 list`

    Each module will be recognize by your IDE (PyCharm or other) because it knows the PYTHONPATH gloabl environment var.
    If you needed to put modules in a special folder. You should add its path to PYTHONPATH var to avoid errors in your IDE:
    `export PYTHONPATH="${PYTHONPATH}:/my/other/path"`    


###Server installation:

In order to have a Python Webserver and an easy websocket usage, we gonna use python Tornado framework.  

- Prerequisites are same than for rPi session.

- Install tornado:

    `pip3 install tornado`

###Usage

Everything is now installed !

  
###Run the local server

You can now run the local webserver typing in your project folder :
`python3 ./server/webserver.py`

You should see a `Server listening on port: 8080` message if everyhting is going well.

Go on `http://{host-ip}:8080/` to see the interface page.

A first client is now connected to the server via websocket.

NB: Open the developper console tool to see logs for debugging.

#### Run the rPi client server

1) SSH Connection to rPi

    - `ssh pi@tiepspi.local`
    
2) Go on project folder
    - `cd Arbreole/src/`

3) Add your server IP
    - From your server host:
    
    `ifconfig` > Copy your en0 IP
    
    - From Arbreole/src/
    
    `nano webSocketClient.py` > Paste your IP in the ws//XX.XX.XX.XX/ws address
    
3) Run the client
    - `python3 webSocketClient.py`
    
    you should see `Server connection opened` in your rPi terminal side and `New connection` at the server host side termainal.
    
    
Everything is now connected by websocket. Try to add a text in your interface, enter and see every logs in terminal / console.

#### Client Config

You can find client config file into the raspberryPi/config/ folder, named config.ini. It is handled by the module configparser that can retrieve all nested config separated by sections.
See [configparser doc](https://docs.python.org/3/library/configparser.html) to use it.
  

####Scripts

You can find some useful scripts into server/script/ folder. To run it, **from this folder**, please type ``./server/scripts/{script-name}.sh`` 

- update_config.sh : change your local config.ini 
