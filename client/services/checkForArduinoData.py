import threading
import time

import services.serialHandler as serialHandler

# ======================

# global variables for module

inputData = ""
threadRun = True
checkDelay = 2  # seconds


# ======================

def checkForData():
    global threadRun, checkDelay
    print("Starting to Listen")
    while threadRun == True:
        dataInput = serialHandler.recvFromArduino(0.1)
        print("DataInput {}".format(dataInput))
        if dataInput == "<<" or dataInput == ">>":
            dataInput = "nothing"
        processData(dataInput)
        time.sleep(checkDelay)
    print("Finished Listening")


# ======================

# function to illustrate the concept of dealing with the data
def processData(dataRecvd):
    inputData = dataRecvd
    print("DataReceived {}".format(inputData))


# ======================

def listenForData():
    t = threading.Thread(target=checkForData)
    t.daemon = True
    t.start()


# ======================

def stopListening():
    global threadRun, checkDelay
    threadRun = False
    time.sleep(checkDelay + 0.1)  # allow Thread to detect end
