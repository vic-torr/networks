#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<time.h>
#include<sys/time.h>
#include<math.h>

#define SERVER_PORT 54321
#define MAX_LINE 256
#define N_BYTES 32000

/*Create Message*/
char * gen_str(int msg_size){
    char * msg;
    msg = malloc(sizeof(char) * msg_size);
    int i;
    for(i=0;i<msg_size - 1;i++){
        msg[i]='a';
    }
    msg[msg_size - 1] = '\0';
    return msg;
}

/*Save results in file*/
int save_to_file(unsigned n,unsigned m,double mat[n][m],double mat2[n],double mat3[n]){
FILE * pFile;
pFile = fopen("result.csv","w");
if(pFile!=NULL)
{
    fprintf(pFile,"tamanho(bytes),latencia(s),vazao(bps), Lost Packages");
    int i;
    for(i=0;i<n;i++){
        fprintf(pFile,"\n %f, %f, %f, %f",mat[i][0],mat[i][1],mat2[i],mat3[i]);
    }
    fclose(pFile);
}
}

int main(int argc,char * argv[]){
    char *host;
    int s;
    int len;
    int success_ip;
    int server_len, count;
    struct timeval t1, t2, tv;
    struct hostent *server;
    struct sockaddr_in sin;
    int status;
    fd_set read_sd_set;
    double latencia[43][2]= {{1,0},{100,0},{200,0},{300,0},{400,0},{500,0},{600,0}, {700,0},{800,0},{900,0},{1000,0},{300,0},{400,0},{500,0},{600,0},{700,0},{800,0},{900,0},{100,0},{300,0},{400,0}, {500,0},{600,0},{700,0},{800,0},{900,0},{100,0},{300,0},{400,0},{500,0},{600,0},{700,0},{800,0},{900,0},{100,0}, {300,0},{400,0},{500,0},{600,0},{700,0},{800,0},{900,0}, {100,0}};
    double vazao[43];
    double contaPacote[43];
    int it;

    /*Start latencia*/
    for(it=0;it<32;it++)
    {
        latencia[it+11][0]=(it+1) * pow(2,10);
    }

    /*Get IP address*/
    host=argv[1];
    if(argc==2){
        host=argv[1];
    }
    else{
        fprintf(stderr, "Uso: simplex-talk host \n");
        exit(1);
    }

    /*Create socket*/
    s=socket(AF_INET,SOCK_DGRAM,0);
    if(s<0){
        perror("simplex-talk: socket");
        exit(1);
    }

    /*Get host name and data structure*/
    server=gethostbyname(host);
    if(server==NULL){
        fprintf(stderr,"simplex-talk: unknown host: %s \n",host);
        exit(1);
    }
    bzero((char *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char*)&sin.sin_addr.s_addr, server->h_length);
    sin.sin_port = htons(SERVER_PORT);
    server_len=sizeof(sin);

    /*Variables*/
    double aux_lat[3];//Latency vector
    int size,size_it;
    int i,j;
    int n_it=100000;//Number of Iterations
    int conta=0;
    double elapsedTime;

    /*Test for each package size*/
    for(size_it=0; size_it<43; size_it++){

        /*Create message*/
        size = latencia[size_it][0];
        char * buf = gen_str(size);
        printf("\n\n Packe Size: %d Bytes", size);

        /*Start lost packages count*/
        conta=0;

        /*Three reps for test*/
        for(j=0;j<3;j++){

            /*Get time*/
            gettimeofday(&t1,NULL);

            /*Start Test*/
            for(i=0;i<n_it;i++){

                /*Send Message*/
                count=sendto(s,buf,strlen(buf),0,(struct sockaddr *) &sin, server_len);
                if(count<0)
                    perror("Erro no envio");
                    FD_ZERO (&read_sd_set);
                    FD_SET (s,&read_sd_set);
                    tv.tv_sec = 1;
                    tv.tv_usec = 0;
                    status = select(s+1,&read_sd_set,NULL,NULL,&tv);
                    if(FD_ISSET(s, &read_sd_set)){

                        /*Get Message*/
                        count = recvfrom(s,buf,strlen(buf),0,(struct sockaddr *) &sin, &server_len);
                        if(count<0)
                            perror("Erro na recepcao");
                        }
                        else{
                            printf("TIMEOUT UDP");
                            conta=conta+1;
                        }
                    }

                /*Lantecy in ms*/
                gettimeofday(&t2, NULL);
                elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;
                elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;
                aux_lat[j] = elapsedTime / n_it;
            }

            /*Measures avarage*/
            latencia[size_it][1]=(aux_lat[0] + aux_lat[1] + aux_lat[2]) / 3;
            vazao[size_it]=(latencia[size_it][0] * 8) / latencia[size_it][1];
            contaPacote[size_it]=conta;
            printf("\n MeanLatency: %.f miliseconds \n",latencia[size_it][1]);
            printf("\n Mean Throughput: %f bps\n",vazao[size_it]);

            /*Save Results*/
            save_to_file(43,2,latencia,vazao,contaPacote);
            free(buf);
        }
    close(s);
}
