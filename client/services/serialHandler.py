import serial
import subprocess
import time

# global variables for module
startMarker = 60
endMarker = 62


# ========================

def setupSerial():
    global ser

    dev = subprocess.check_output('ls /dev/tty.usbmodem*', shell=True)
    print("Arduino Detected: " + dev.decode('utf-8').strip())

    ser = serial.Serial(dev.decode('utf-8').strip(), 9600)

    waitForArduino()


# ========================

def closeSerial():
    global ser
    if 'ser' in globals():
        ser.close()
        print("Serial Port Closed")
    else:
        print("Serial Port Not Opened")


# ========================

def write(message):
    global startMarker, endMarker, ser

    ser.write(chr(startMarker).encode('utf-8'))
    ser.write(message.encode('utf-8'))
    ser.write(chr(endMarker).encode('utf-8'))


# ===========================

def recvFromArduino(timeOut):  # timeout in seconds eg 1.5

    global startMarker, endMarker, ser

    # print("Called with timeOut {}".format(timeOut))

    dataBuf = ""
    x = "z"  # any value that is not an end or startMarker
    startTime = time.time()

    # wait for the start marker
    while ord(x) != startMarker:
        if time.time() - startTime >= timeOut:
            return '<<'
        if ser.inWaiting() > 0:  # because ser.read() blocks
            x = ser.read().decode('utf-8')

    # save data until the end marker is found
    while ord(x) != endMarker:
        if time.time() - startTime >= timeOut:
            return '>>'
        if ord(x) != startMarker:
            dataBuf = dataBuf + x
        if ser.inWaiting() > 0:
            x = ser.read().decode('utf-8')
        else:
            x = chr(startMarker)  # crude way to prevent repeat characters
            #   when no data is received

    return dataBuf


# ============================

def waitForArduino():

    # wait until the Arduino sends 'Arduino Ready' - allows time for Arduino reset
    # it also ensures that any bytes left over from a previous message are discarded

    print("Waiting for Arduino to reset")

    msg = ""
    while msg.find("Arduino is ready") == -1:
        msg = recvFromArduino(10)

        print(msg)
        print()
