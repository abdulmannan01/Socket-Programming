import socket
from os import system, name
import threading
import time

#To clear screen
def clear():

    # for windows
    if name == 'nt':
        _ = system('cls')

    # for mac and linux(here, os.name is 'posix')
    else:
        _ = system('clear')


#Thread handler for receiving
def receive_server(mainSocket):
    while True:
        message = mainSocket.recv(1024).decode()
        #time.sleep(1)
        if message == "Exit":
            return
        print("\n<", socket.gethostbyname(socket.gethostname()), "> ", message, sep = "")


#Main function
clear()

mainSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
host = socket.gethostname()
port = 12345

mainSocket.connect((host, port))

message = mainSocket.recv(1024).decode()
print(message)

t = []

t.append(threading.Thread(target=receive_server, args=(mainSocket, )))

t[0].start()

while True:
    reply = input()
    print("\n<You>", reply)
    mainSocket.send(reply.encode())

    if reply == "Exit" or reply == "exit":
        break

t[0].join()
mainSocket.close()
