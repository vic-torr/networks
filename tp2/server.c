#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#define N_BYTES 32000
#define PORT 33333

int main(int argc, char **argv)
{
    struct sockaddr_in server; //server address
    struct sockaddr_in client; //client address
    int udp_socket;                     //socket
    int clen;                  //Address size in bytes
    char buf[N_BYTES];         //messagem buffer
    int opt;                   //socket flag
    int n;                     //recfrom and sendto return

    //Create socket UDP
    udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_socket < 0)
    {
        perror("Socket connection error!");
    };
    opt = 1;
    setsockopt(udp_socket, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(int));

    //Serever configuration
    bzero((char *)&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons((unsigned short)PORT);

    //socket bind
    if (bind(udp_socket, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("Binding error!");
    };

    clen = sizeof(client);

    //Send and receive message
    while (1)
    {
        bzero(buf, N_BYTES);

        //Receive
        n = recvfrom(udp_socket, buf, N_BYTES, 0, (struct sockaddr *)&client, &clen);
        if (n == -1)
        {
            perror("ERROR in recvfrom");
        };

        //Send
        n = sendto(udp_socket, buf, strlen(buf), 0, (struct sockaddr *)&client, clen);
        if (n == -1)
        {
            perror("ERROR in sendto");
        };
    }
}
