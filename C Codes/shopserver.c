#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#define MAX_LEN 100

struct format{
    char fruit_name[20];
    unsigned int quantity;
    time_t last_sold;
};

void main(){
	time_t start_time;
    int client_list[MAX_LEN];
    int list_len=0,unique=1;
    char len_str[5],line[100];
    struct format fruit[3];
    fruit[0].last_sold=fruit[1].last_sold=fruit[2].last_sold=time(NULL);
    strcpy(fruit[0].fruit_name,"Apple");
    fruit[0].quantity=100;
    strcpy(fruit[1].fruit_name,"Mango");
    fruit[1].quantity=100;
    strcpy(fruit[2].fruit_name,"Orange");
    fruit[2].quantity=100;
    printf("Fruit_Name\tQuantity\n");
    int z;
    for(z=0;z<3;z++)
	printf("%s\t\t%d\n",fruit[z].fruit_name,fruit[z].quantity);
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
    if(listen(welcomeSocket,5)==0){
        printf("Listening\n");
    }
    else{
        printf("Error\n");
    }
    while(1){
        FILE *purchase=fopen("purchase.txt","a");
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
         printf("\nClient padding characters are (should be blank): ");
        int i,j,k,sum=0;
        for (i=0;i<8;i++){
            printf("%c", serverStorage.sin_zero[i]);
        }
        printf("\nGot a client connection from IP, port: <%s, %d> (can be used for client identification)\n", inet_ntoa(
serverStorage.sin_addr), serverStorage.sin_port);
        if(list_len==0){
            client_list[0]=serverStorage.sin_port;
            list_len++;
        }
        else{
            unique=1;
            for(i=0;i<list_len;i++){
                if(client_list[i]==serverStorage.sin_port){
                    unique=0;
                    break;
                }
            }
            if(unique){
                client_list[list_len-1]=serverStorage.sin_port;
                list_len++;
            }
        }

        strcpy(buffer, "Enter fruit name: ");
        send(newSocket, buffer, 1024, 0);
        printf("\nFruit name request sent\n");
        recv(newSocket, buffer, 1024, 0);
        for(i=0;i<3;i++){
            if(strcmp(fruit[i].fruit_name,buffer)==0){
                k=i;
                break;
            }
        }
        strcpy(buffer, "Enter amount required: ");
        send(newSocket, buffer, 1024, 0);
        printf("\nAmount request sent\n");
        recv(newSocket, buffer, 1024, 0);
        printf("\nAmount required received\n");
        j=0;
        while(buffer[j]!='\0'){
            sum=10*sum+(buffer[j]-48);
            j++;
        }
        printf("\nExited while loop\n");
        if(fruit[i].quantity<sum){
            strcpy(buffer,"Amount requested greater than available!");
            send(newSocket, buffer, 1024, 0);
        }
        else{
            fruit[i].quantity=fruit[i].quantity-sum;
            fprintf(purchase,"Item: %s Qty_purchased: %d <Client IP,port>: <%s,%d>\n",fruit[i].fruit_name, sum, inet_ntoa(serverStorage.sin_addr), serverStorage.sin_port);
        }
        strcpy(len_str,"");
        snprintf(len_str,sizeof(len_str),"%d",list_len);
        strcpy(buffer,"Number of unique clients: ");
        send(newSocket,buffer, 1024, 0);
        strcpy(buffer,len_str);
        send(newSocket,buffer, 1024, 0);
        close(newSocket);
        fclose(purchase);
        printf("\n");
        for(i=0;i<3;i++){
            fruit[i].last_sold=time(NULL);
            if(i==k){
                fruit[i].last_sold=fruit[i].last_sold-start_time;
            }
            printf("Item: %s Quantity_left: %d Last sold: %d secs ago\n",fruit[i].fruit_name,fruit[i].quantity,fruit[i].last_sold);
        }
        purchase=fopen("purchase.txt","r");
        printf("\n");
        while(fgets(line, sizeof(line), purchase) != NULL){
            printf("%s\n", line);
        }
        fclose(purchase);
    }
}
