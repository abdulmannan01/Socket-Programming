import socket
from os import system, name
import threading

#To clear screen
def clear():

    # for windows
    if name == 'nt':
        _ = system('cls')

    # for mac and linux(here, os.name is 'posix')
    else:
        _ = system('clear')


#Function to broadcast message
def broadcast(message, connectSocket):

    for clients in client_list:
        if clients != connectSocket:
            clients.send(message.encode())


#Function to delete a client
def remove_client(connectSocket):

    if connectSocket in client_list:
        client_list.remove(connectSocket)


#Client thread handler
def client_thread(connectSocket, addr):

    connectSocket.send("Welcome to this chat room! (Send 'Exit' to leave)".encode())

    while True:

        message = connectSocket.recv(1024).decode()
        #print(client_list)
        if message == "Exit" or message == "exit":
            message_to_send = "<" + addr[0] + "> " + "left the chat"
            broadcast(message_to_send, connectSocket)

            print("\n<", addr[0], "> left the chat", sep = "")

            connectSocket.send("Exit".encode())
            remove_client(connectSocket)
            connectSocket.close()

            return

        print("\n<", addr[0], "> ", message, sep ="")
        message_to_send = "<" + addr[0] + "> " + message
        broadcast(message, connectSocket)


#Function to reuse inactive threads
def check_thread(connectSocket, addr, t, i):
    if i > 0:
        for thread in t:
            if not thread.isAlive():
                thread = threading.Thread(target=client_thread, args=(connectSocket, addr))
                thread.start()
                return(t, i)

    t.append(threading.Thread(target=client_thread, args=(connectSocket, addr)))
    t[i].start()
    i = i + 1

    return(t, i)

#Main function
clear()
print("Chat Room Server\n")

mainSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
host = socket.gethostname()
port = 12345

mainSocket.bind((host, port))

client_list = []
no_of_clients = 0

mainSocket.listen(10)

i = 0
t = []

while True:
    connectSocket, addr = mainSocket.accept()
    client_list.append(connectSocket)
    message_to_send = "<" + addr[0] + "> joined the chat"
    broadcast(message_to_send, connectSocket)

    print("\n", addr, "joined the chat")

    (t, i) = check_thread(connectSocket, addr, t, i)

    print("Thread: ", i)

connectSocket.close()
mainSocket.close()
