#include <stdio.h> //printf
#include <string.h>    //strlen
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr
 
int main(int argc , char *argv[])
{
    //Declarações para os sockets
    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];
     
    //Criação do socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    server.sin_addr.s_addr = inet_addr("10.0.0.103"); //Alterar para o ip atual
    server.sin_family = AF_INET;
    server.sin_port = htons( 5000 );
 
    //Conecta com o server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
     
    puts("Connected\n");
     
    //Mantem a conexão com o server
    while(1)
    {
        bzero(message,sizeof(message)); //Zera o buffer que contem o envio da mensagem desejada
        bzero(server_reply,sizeof(server_reply)); //Zera o buffer que recebe o retorno do server
        printf("Enter message : ");
        // scanf("%s" , message);
        gets(message); //Não é seguro
         
        //Envia um dado
        if( send(sock , message , strlen(message) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
         
        //Recebe uma mensagem do servidor
        if( recv(sock , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }
         
         printf("\nServer reply: %s\n",server_reply );
        // puts("\nServer reply :");
        // puts(server_reply);
    }
     
    close(sock);
    return 0;
}