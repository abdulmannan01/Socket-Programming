#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
int main(){
  int clientSocket;
  char buffer[1024];
  struct sockaddr_in serverAddr, clientAddr;
  socklen_t addr_size;
  clientSocket = socket(PF_INET, SOCK_STREAM, 0);
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(7891);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
  connect(clientSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
  recv(clientSocket, buffer, 1024, 0);
  printf("\nData received from server: %s\n",buffer);
  printf("What do you want to send? \n");
  gets(buffer);
  printf("Data sent to server: %s\n", buffer);   
  send(clientSocket,buffer, 1024, 0);
  return 0;

}

