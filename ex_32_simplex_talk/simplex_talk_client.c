
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
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<string.h>


#define SERVER_PORT 5432
#define MAX_LINE 256



int main(int argc, char * argv[]) {
        FILE *fp ;
        struct hostent *hp ;
        struct sockaddr_in sin ;
        char * host ;
        char buf[MAX_LINE] ;
        int s ;
        int len ;

        if ( argc==2 ) {
                host = argv[1] ;
        } else {
                fprintf(stderr, "usage: simplex-talk host\n") ;
                exit(1) ;
        }
        // trabskates hosts name to its IP adderess
        hp = gethostbyname(host) ;
        if (!hp) {
                fprintf(stderr, "simplex-talk: unknown host: %s\n", host ) ;
                exit(1);
        }
        // build data structure of adderess
        bzero((char *)&sin,sizeof(sin)) ;
        sin.sin_family = AF_INET ;
        bcopy( hp->h_addr, (char *) &sin.sin_addr, hp->h_length) ;
        sin.sin_port = htons( SERVER_PORT) ;

        // enable oppening
        if ( (s = socket(PF_INET,SOCK_STREAM,0)) < 0) {
                perror("simplex-talk: socket") ;
                exit(1) ;
        }
        if ( connect(s, (struct sockaddr *) &sin, sizeof(sin) ) < 0 ) {
                perror("simplex-talk: connect") ;
                //close(s);
                exit(1) ;
        }
        // main loop: get and send text lines
        while ( fgets(buf, sizeof(buf), stdin) )  {
                buf[MAX_LINE-1] = '\0' ;
                len = strlen(buf) + 1 ;
                send(s, buf, len, 0 ) ;
        }
}

