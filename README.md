### Before starting

In this documentation, we will use the terminal to perform some command lines. Here, every command will be prefixed by **$**.

### Get the project!

In your favorite terminal (for Mac we advice you to get iTerm2, for linux the default terminal with [ohmyzsh](https://github.com/robbyrussell/oh-my-zsh))

```sh
$ cd ~  # tilde means home
$ mkdir arbreole && cd arbreole # mkdir: make directory, cd: change directory
$ git clone git@github.com:Tactyk/arbreole.git # retrieve code
$ ./setup.sh # Set up ip address and port
```

### Project architecture

You can see now the project structure in your 'arbreole' folder

```sh
$ ls # list the current folder
```

The `_doc` folder contains more informations about the technologies used in this project.

The `arduino` folder contains the code related to manipulating the leds and the sounds.

The `client` folder contains the webSocket client hosted on the Raspberry Pi.

The `config` folder contains configuration data, which can be edited by hand but is meant to be auto-generated by `setup.sh`.

The `interface` folder contains the code used in the web client.

The `scripts` folder contains scripts used to update the RaspberryPi.

The `server` folder contains the web server interacting with the RaspberryPi and web clients.

### Development tools

* Download and install [PyCharm Community Edition](https://www.jetbrains.com/pycharm/download/)

* Configuration:

  Preferences > Project > Project Interpreter >

  1. Click on the setting wheel and
  2. Choose a Python 3 interpreter ![Choose a Python 3 interpreter](./_doc/img/python_interpreter.png)

## Setup the RaspberryPi

#### 1. RaspberryPi Initialization

Follow [this link](https://www.raspberrypi.org/documentation/installation/installing-images/README.md) to initialize the Raspberry Pi (use the Raspbian Stretch LITE image)

#### 2. SSH to your RaspberryPi

* Make sure that your rPi and your computer are connected to the same network (via ethernet cable or WIFI dongle).

  - If you have a WIFI Dongle, follow [this link](https://www.raspberrypi.org/documentation/configuration/wireless/wireless-cli.md) to configure it
  - If you don't have a router, you can share your computer's Ethernet connection, see:
    - [Linux (Ubuntu)](https://roughlea.wordpress.com/raspberry-pi-experiences/configure-the-raspberry-pi-to-share-a-linux-internet-connection/)
    - [Mac OS](https://medium.com/@tzhenghao/how-to-ssh-into-your-raspberry-pi-with-a-mac-and-ethernet-cable-636a197d055)

* **Enable the ssh on your rPi** by following [this link](https://www.raspberrypi.org/documentation/remote-access/ssh/)

* From your computer you should now be able to connect to your raspberryPi via ssh.

  ```sh
  $ ssh pi@raspberry.local
  ```

  - `pi` is the rPi user
  - `raspberry` is the rPi hostname
  - `raspberry.local` is the rPi host address
  - The default password for the pi user is: **raspberry**

  Follow [this link](https://thepihut.com/blogs/raspberry-pi-tutorials/19668676-renaming-your-raspberry-pi-the-hostname) if you want to change the rPi hostname

#### 3. RaspberryPi (rPi) client installation:

In this project, we use **python3**. In order to install python3 packages, use the pip3 packages installer. It should be already in rPi but nevertheless:

* SSH into your pi, then

  ```sh
  $ sudo apt-get update # Update packages
  $ sudo apt-get install python3-pip # Install pip3 and python3
  $ pip3 install websocket-client pyserial # Install packages
  ```

  To communicate with the server, we will use websockets, with the **websocket-client** library.

  Each module will be recognize by your IDE (PyCharm or other) because it knows the PYTHONPATH global environment var.
  If you needed to put modules in a special folder. You should add its path to PYTHONPATH var to avoid errors in your IDE: `$ export PYTHONPATH="${PYTHONPATH}:/my/other/path"`

* In **your machine** from **your project root folder** (arbreole/), execute the following script to copy the raspberry :

  ```sh
  $ ./scripts/deploy_raspberry_pi.sh pi raspberrypi.local #raspberrypi.local is the rPi host address
  ```

  It may ask you for the rPi password (default = raspberry)


### Server installation:

In order to have a Python Webserver and an easy websocket usage, we're going to use the python Tornado framework.

**In your machine:**

* Prerequisites are same than for rPi session.

* Install tornado for your web server and tinydb to handle module states:

  ```
  $ pip3 install tornado tinydb
  ```

### Usage

Everything is now installed !

#### Run the local server

To run the local webserver, go to your project folder and type

```sh
python3 ./server/webserver.py
```

You should see something along the lines of `Server listening on ip: 192.168.1.20 at port 8080`.

In your browser, got o `http://{host-ip}:{host-port}/` to see the interface page.

A first client is now connected to the server via websocket.

NB: Open the developer console tool to see logs for debugging. (Right click > inspect > Console tab)

#### Run the rPi client server

SSH into the RaspberryPi and run

```sh
$ cd arbreole/src
$ python3 webSocketClient.py
```

you should see `Server connection opened` in your rPi terminal side and `New connection` at the server host side terminal.

Everything is now connected by websocket. Try to add a text in your web client, enter and see every logs in terminal/console.

#### Scripts

You can find some useful scripts into ./scripts/ folder. To run it, **from your project root folder**, type `./scripts/{script-name}.sh`

* [`./scripts/deploy_raspberry.sh`](./scripts/deploy_raspberry.sh): deploy the local client code to all the raspberry pis
* [`./scripts/deploy_raspberry.sh pi raspberry.local`](./scripts/deploy_raspberry.sh): deploy the local client code to pi@raspberry.local

### Contributing

If you want to add your code to the project, submit a Pull Request. Please follow the instructions [here](./_doc/using_git.md#pull-requests)

##### TODO

* [ ] TroubleShootings  - Import not recognize
* [ ] Open one folder at once with PyCharm
