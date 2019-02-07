#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

void calculator(char a[], char b[], int flag){

    int op1,op2, ans;
    op1 = atoi(a);
    op2 = atoi(b);

    if(flag == 1){

        ans = op1 + op2;

    }

    if(flag == 2){

        ans = op1 - op2;

    }
    
    if(flag == 3){

        ans = op1 * op2;

    }
    
    if(flag == 4){

        ans = op1 / op2;

    }    

    printf("\nThe answer is: %d\n", ans);
    return;

}

void parsebuffer(char buffer[]){

    int i = 0;
    int j = 0;
    int k = 0;
    int flag = 0;
    int flag_1 = 1;
    char num1[10];
    char num2[10];
    while(buffer[i] != '\0'){

        if(buffer[i] == '+'){
            flag = 1;
            flag_1 = 2;
            i++;
        }

        if(buffer[i] == '-'){
            flag = 2;
            flag_1 = 2;
            i++;
        }

        if(buffer[i] == '*'){
            flag = 3;
            flag_1 = 2;
            i++;
        }

        if(buffer[i] == '/'){
            flag = 4;
            flag_1 = 2;
            i++;
        }

        
        else if(flag_1 == 1){

            num1[j] = buffer[i];
            i++;
            j++;

        }

        else if(flag_1 == 2){

            num2[k] = buffer[i];
            i++;
            k++;

        }


    }

    calculator(num1, num2, flag);
    return;

}

int main(){
  int welcomeSocket, newSocket;
  char buffer[1024];
  struct sockaddr_in serverAddr;
  struct sockaddr_in serverStorage;
  socklen_t addr_size;
  welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(7891);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
  bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
  if(listen(welcomeSocket,5)==0)

    printf("Listening\n");

  else

    printf("Error\n");

 while(1) {

  addr_size = sizeof serverStorage;
  newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);
  struct sockaddr_in* cliIP = (struct sockaddr_in*)&serverStorage;
  struct in_addr ipAddr = cliIP->sin_addr;
  char str[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &ipAddr, str, INET_ADDRSTRLEN);
  char* ID = cliIP->sin_zero;
  char str2[8];
  inet_ntop(AF_INET, &ID, str2, 8);
  printf("\nClient IP is: %s", str);
  printf("\nClient port is: %d", serverStorage.sin_port);
  strcpy(buffer, "Welcome from server");
  send(newSocket,buffer,23,0);
  recv(newSocket, buffer, 1024, 0);
  printf("\nData received from client : %s\n", buffer);   
  parsebuffer(buffer);
  close(newSocket);
 }

  return 0;

}

