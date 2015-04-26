#include <stdio.h>
#include <string.h>    
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <unistd.h>    
 
//Arquivo principal, deve ser chamado index.html
FILE *arquivo;

//Arquivo de suporte
FILE *suporte;

//Buffer para pegar linhas dos arquivos
char str[2000];

//Função que copia o conteudo do arquivo de suporte para o arquivo principal
int saveToHtml();

//Main
int main(int argc , char *argv[])
{
    //Declarações para os sockets
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[2000];

    // Linha usada para percorrer o arquivo
    int linha = 1;
     
    //Cria o socket e retorna um erro caso não for possivel
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
        // Abre o index html para leitura
        if ((arquivo = fopen("index.html", "r")) == NULL)
        {
            printf("Não foi possivel abrir o arquivo.\n");
            return 1;
        }

        // Abre/Cria o arquivo suporte que é usado para inserir as novas mensagens no html
        if ((suporte = fopen("suporte.html", "w")) == NULL)
        {
            printf("Não foi possivel abrir o arquivo.\n");
            return 1;
        }

        // Prepara a mensagem que deve ser enviada para o html para que essa va no formato certo
        char mensagemASerEnviada[2000] = "\t\t";
        strcat(mensagemASerEnviada, "<div class=\"texto\">");
        strcat(mensagemASerEnviada, client_message);
        strcat(mensagemASerEnviada, "</div>\n");

        //Salva a primeira parte do html no arquivo suporte
        while(linha <= 13){
            fgets(str, 3000, arquivo);
            fputs(str, suporte);
            linha ++;
        }

        //Salva a nova mensagem no html
        fputs(mensagemASerEnviada, suporte); 

        //Salva o restante do index html no arquivo suporte
        while(fgets(str, 3000, arquivo)){
            fputs(str, suporte);
        }

        //Reseta a linha
        linha = 1;

        //Limpa o buffer para a proxima a mensagem
        bzero(mensagemASerEnviada, sizeof(mensagemASerEnviada));
        fclose(arquivo); //Fecha o arquivo
        fclose(suporte); //Fecha o arquivo de suporte
        
        //Substitui o conteudo do index.html pelo conteudo do suporte.html
        saveToHtml();

        //Devolve uma mensagem para o cliente
        write(client_sock, "Noticia enviada com sucesso!!",500);

        // Zera o buffer que devolve a mensagem pro cliente.
        bzero(client_message,sizeof(client_message)); 
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

// Implementação da função que copia o conteudo do arquivo suporte para o arquivo principal
int saveToHtml() {

    if ((arquivo = fopen("index.html", "w")) == NULL){
        printf("Não foi possivel abrir o arquivo.\n");
        return 0;
    }

    if ((suporte = fopen("suporte.html", "r")) == NULL) {
        printf("Não foi possivel abrir o arquivo.\n");
        return 0;
    }

    // Sobrescreve o conteudo do index.html pelo conteudo do suporte.html
    while(fgets(str, 3000, suporte)){
        fputs(str, arquivo);
    }

    fclose(arquivo);
    fclose(suporte);
    return 1;
}