#include <stdio.h> //printf
#include <string.h>    //strlen
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr
 
int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];
     
    //Criação do socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    //Teste para verificar se foi criado com sucesso
    if (sock == -1)
    {
        printf("Erro ao criar socket \n");
        return 1;
    }
    puts("Socket criado com sucesso ! \n");
     
    server.sin_addr.s_addr = inet_addr("192.168.0.114"); //Alterar para o ip atual
    server.sin_family = AF_INET;
    server.sin_port = htons( 5000 );
 
    //Conecta com o server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("Erro ao conectar \n");
        return 1;
    }
     
    puts("Conectado com sucesso !\n");
     
    //Mantem a conexão com o server
    while(1)
    {
        bzero(message,sizeof(message)); //Zera o buffer que contem o envio da mensagem desejada
        bzero(server_reply,sizeof(server_reply)); //Zera o buffer que recebe o retorno do server
        printf("Coloque sua mensagem: ");
        scanf("%s" , message);  
        // fgets(message, 1000, stdin); //Teste no lugar do gets, pelo menos ta compilando
         
        //Envia um dado
        if( send(sock , message , strlen(message) , 0) < 0)
        {
            puts("Envio falhou \n");
            return 1;
        }
         
        //Recebe uma mensagem do servidor
        if( recv(sock , server_reply , 2000 , 0) < 0)
        {
            puts("Houve uma falha ao receber a mensagem \n");
            break;
        }
         
        puts("\n Mensagem do Servidor: ");
        puts(server_reply);
    }
     
    close(sock);
    return 0;
}