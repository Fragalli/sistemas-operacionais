#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

int main(int argc, char *argv[])
{
    // Declaraçao do socket
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    // struct com os dados do host a quem vamos nos comunicar, é preenchida durante o código
    struct sockaddr_in serv_addr; 

    if(argc != 2)
    {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    } 

    memset(recvBuff, '0',sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) //criacao do socket, o 0 no final se refere ao IP(Internet Protocol)
    {
        printf("\n Error : Could not create socket \n");//caso dê erro
        return 1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET; //ARPA INTERNET PROTOCOLS
    serv_addr.sin_port = htons(5000); //porta que estamos usando

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");//caso dê erro
        return 1;
    } 

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)//coneccao do cliente com um serviço
    {
       printf("\n Error : Connect Failed \n");//caso dê erro
       return 1;
    } 

    while ( (n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0)
    {
        recvBuff[n] = 0;
        if(fputs(recvBuff, stdout) == EOF)
        {
            printf("\n Error : Fputs error\n");//caso dê erro
        }
    } 

    if(n < 0)
    {
        printf("\n Read error \n");
    } 

    return 0;
}