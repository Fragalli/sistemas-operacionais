#include <stdio.h>
#include <string.h>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
 
int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[2000];

    //Arquivo
    FILE *arq;
     
    //Cria o socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    //Prepara o socket
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 5000 );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print de erro
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
     
    //Listen
    listen(socket_desc , 3);
     
    //Aguarda conexão
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
     
    //aceita conexão de um client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    puts("Connection accepted");
     
    //Recebe uma mensagem do cliente
    while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
    {
        // Abre/Cria o arquivo para escrita
        if ((arq = fopen("TESTE.txt", "w")) == NULL)
        {
            printf("Não foi possivel abrir o arquivo.\n");
            return 1;
        }

        fputs(client_message, arq); //Salva a mensagem no arquivo
        fclose(arq); //Fecha o arquivo
        
        // write(client_sock , client_message , strlen(client_message)); //devolve a mensagem pro cliente
        write(client_sock, "Noticia enviada com sucesso!!\n",strlen(client_message));
        bzero(client_message,sizeof(client_message)); // Zera o buffer que devolve a mensagem pro cliente.
    }
     
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
     
    return 0;
}