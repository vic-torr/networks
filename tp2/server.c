#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<netdb.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define PORT 54321
#define N_BYTES 32000

int main(int argc, char **argv){
    int s; /*socket*/
    int clen; /*Address size in bytes*/
    int n; /*recfrom and sendto return*/
    char buf[N_BYTES]; /*messagem buffer*/
    int opt; /*socket flag*/
    struct sockaddr_in server; /*server address*/
    struct sockaddr_in client; /*client address*/

    /*Create socket UDP*/
    s = socket(AF_INET, SOCK_DGRAM, 0);
    if(s<0) {
        perror("Erro na abertura do socket")};
    opt=1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const void*)&opt, sizeof(int));

    /*Serever configuration*/
    bzero((char*)&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons((unsigned short)PORT);

    /*socket bind*/
    if(bind(s,(struct sockaddr *)&server, sizeof(server))<0) {
        perror("Falha no bind.")};

    clen = sizeof(client);

    /*Send and receive message*/
    while(1){
        bzero(buf,N_BYTES);

        /*Receive*/
        n=recvfrom(s,buf,N_BYTES,0,(struct sockaddr*)&client,&clen);
        if(n == -1){
            perror("ERROR in recvfrom")};

        /*Send*/
        n=sendto(s,buf,strlen(buf),0,(structsockaddr∗)&client,clen);
        if(n == -1){
            perror("ERROR in sendto")};
    }
}

