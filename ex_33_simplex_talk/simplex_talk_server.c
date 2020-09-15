
/*
Exercicio 34:
Modifique o programa baseado em sockets denominado socket simplex-talk de modo que ele
use UDP como protocolo de transporte em vez de TCP. Você terá que substituir o parâmetro
SOCK_STREAM pelo parâmetro SOCK_DGRAM no cliente e no servidor. Depois, no servidor,
remova as chamadas a listen() e accept() e substitua os dois laços aninhados no final por um
único laço que invoca recv() com o socket s. Finalmente, veja o que acontece quando dois desses
clientes UDP se conectam simultaneamente ao mesmo servidor UDP e compare isso com o
comportamento do TCP.
 */


#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

#define SERVER_PORT 5432
#define MAX_PENDING 5
#define MAX_LINE 256

int main(int argc, char * argv[]) {
        struct sockaddr_in sin ;
        char buf[MAX_LINE] ;
        int len ;
        int s, new_s ;

        // mount data structure of addreress
        bzero((char *)&sin,sizeof(sin)) ;
        sin.sin_family = AF_INET ;
        sin.sin_addr.s_addr = INADDR_ANY ;
        sin.sin_port = htons( SERVER_PORT) ;

        // prepare passive open
        if ( (s = socket(PF_INET,SOCK_STREAM,0)) < 0) {
                perror("simplex-talk: socket") ;
                exit(1) ;
        }
        if   (bind( s, (struct sockaddr *) &sin, sizeof(sin) ) < 0 ) {
                perror("simplex-talk: bind") ;
                exit(1) ;
        }
        listen( s, MAX_PENDING ) ;
        // wait connection, then receive and print text
        while(1) {
                if ( (new_s = accept(s,(struct sockaddr *)&sin,
                        (socklen_t *) &len) ) < 0 ) {
                        perror("simplex-talk: accept") ;
                        exit(1) ;
                }
                while ( (len = recv(new_s, buf, sizeof(buf), 0 )) )
                        fputs(buf, stdout) ;
                close(new_s) ;
        }
}


