#include <math.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>

#define SERVER_PORT 33333
#define MAX_LINE 256
#define N_BYTES 32000

// Save results in file
int save_to_file(unsigned n, unsigned m, float mat[n][m], float mat2[n], float mat3[n], char *file_name)
{
    FILE *pFile;

    pFile = fopen(file_name, "w");

    puts(file_name);
    if (pFile != NULL)
    {
        fprintf(pFile, "tamanho(bytes),latencia(s),vazao(bps), Lost packet");
        int i;
        for (i = 0; i < n; i++)
        {
            fprintf(pFile, "\n %f, %f, %f, %f", mat[i][0], mat[i][1], mat2[i], mat3[i]);
        }
        fclose(pFile);
    }
}

// Create Message
char *generate_message(int size)
{
    char *message;
    message = malloc(sizeof(char) * size);
    for (int i = 0; i < size - 1; i++)
    {
        message[i] = chat((' ' + ((90 << i) | 'a' >> i ^ i << 2) % 90)) // pseudorandom character
    }
    message[size - 1] = '\0';
    return message;
}

int main(int argc, char *argv[])
{
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
    float latencia[size_array][2] = {{1, 0}, {100, 0}, {200, 0}, {300, 0}, {400, 0}, {500, 0}, {600, 0}, {700, 0}, {800, 0}, {900, 0}, {1000, 0}, {300, 0}, {400, 0}, {500, 0}, {600, 0}, {700, 0}, {800, 0}, {900, 0}, {100, 0}, {300, 0}, {400, 0}, {500, 0}, {600, 0}, {700, 0}, {800, 0}, {900, 0}, {100, 0}, {300, 0}, {400, 0}, {500, 0}, {600, 0}, {700, 0}, {800, 0}, {900, 0}, {100, 0}, {300, 0}, {400, 0}, {500, 0}, {600, 0}, {700, 0}, {800, 0}, {900, 0}, {100, 0}};
    int latency_packet_size[32];
    float latency_time[32];
    int throughput_packet_size[32];
    float throughput_time[32];
    float vazao[size_array];
    float contaPacote[size_array];
    int it;
    // Variables
    float aux_lat; //Latency
    int size, size_it;
    int i, j;
    int n_it = 100; //Number of Iterations
    int conta = 0;
    float elapsedTime;

    // Get last test

    char *file_name = calloc(10, 1);
    int experiment_number = 0;
    sprintf(file_name, "result_%d.csv", experiment_number);
    puts(file_name);

    while (fopen(file_name, "r") != NULL) // check if file exists
    {
        experiment_number++;
        sprintf(file_name, "result_%d.csv", experiment_number);
        puts(file_name);
    }

    puts(file_name);

    // Start latency
    size_t size_array = sizeof(a) / sizeof(int);
    for (it = 0; it < size_array; it++)
    {
        latency_packet_size[it] = i * 50
    }

    // Get IP address
    host = argv[1];
    if (argc == 2)
    {
        host = argv[1];
    }
    else
    {
        fprintf(stderr, "Uso: simplex-talk host \n");
        exit(1);
    }

    // Create socket
    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0)
    {
        perror("simplex-talk: socket");
        exit(1);
    }

    // Get host name and data structure
    server = gethostbyname(host);
    if (server == NULL)
    {
        fprintf(stderr, "simplex-talk: unknown host: %s \n", host);
        exit(1);
    }
    bzero((char *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&sin.sin_addr.s_addr, server->h_length);
    sin.sin_port = htons(SERVER_PORT);
    server_len = sizeof(sin);

    // Test for each package size
    for (size_it = 0; size_it < size_array; size_it++)
    {

        // Create message
        size = latency_packet_size[size_it - 1];
        char *buf = generate_message(size);
        printf("\n\n Packet Size: %d Bytes", size);

        // Start lost packet count
        conta = 0;

        // Get time
        gettimeofday(&t1, NULL);

        // Start Test
        for (i = 0; i < n_it; i++)
        {

            // Send Message
            count = sendto(s, buf, strlen(buf), 0, (struct sockaddr *)&sin, server_len);
            if (count < 0)
                perror("Transmiting error.");
            FD_ZERO(&read_sd_set);
            FD_SET(s, &read_sd_set);
            tv.tv_sec = 1;
            tv.tv_usec = 0;
            status = select(s + 1, &read_sd_set, NULL, NULL, &tv);
            if (FD_ISSET(s, &read_sd_set))
            {

                // Get Message
                count = recvfrom(s, buf, strlen(buf), 0, (struct sockaddr *)&sin, &server_len);
                if (count < 0)
                    perror("Receiving error.");
            }
            else
            {
                printf("TIMEOUT UDP");
                conta = conta + 1;
            }
        }

        // Lantecy in ms
        gettimeofday(&t2, NULL);
        elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;
        elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;
        aux_lat = elapsedTime / n_it;

        // Measures avarage
        latency_time[i] = aux_lat;
        vazao[i ] = (latencia[size_it][0] * 8) / latency_time[size_it];
        contaPacote[size_it] = conta;
        printf("\n MeanLatency: %.f miliseconds \n", latency_time[size_it]);
        printf("\n Mean Throughput: %f bps\n", vazao[size_it]);

        // Save Results
        save_to_file(size_array, 2, latency_time, vazao, contaPacote, file_name);

        FILE *pFile;

        pFile = fopen(file_name, "w");

        puts(file_name);
        if (pFile != NULL)
        {
            fprintf(pFile, "tamanho(bytes),latencia(s),vazao(bps), Lost packet");
            int i;
            for (i = 0; i < n; i++)
            {
                fprintf(pFile, "\n %f, %f, %f, %f", mat[i][0], mat[i][1], mat2[i], mat3[i]);
            }
            fclose(pFile);
        }
        free(buf);
    }
    close(s);
}
