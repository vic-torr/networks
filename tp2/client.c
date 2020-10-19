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
#define SIZE_ARR 64
#define TOTAL_ITER 100000

// Create Message
char* generate_message(int size)
{
    char *message;
    message = malloc(sizeof(char) * size);
    for (int i = 0; i < size - 1; i++)
    {
        message[i] = (' ' + ((90 << i) | 'a' >> i ^ i << 2) % 90); // pseudorandom character
    }
    message[size - 1] = '\0';
    return message;
}

int main(int argc, char *argv[])
{
    char *host;
    int udp_socket;
    int len;
    int success_ip;
    int server_len, count;
    struct timeval t1, t2, tv;
    struct hostent *server;
    struct sockaddr_in sin;
    int status;
    fd_set read_sd_set;
    
    // Variables
    int packet_size[SIZE_ARR];
    float latency_time[SIZE_ARR];
    float throughput[SIZE_ARR];
    float lost_packets_count[SIZE_ARR];
    int lost_packets = 0;
    float elapsedTime;

    //get current file name
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

    // Get IP address
    host = argv[1];
    if (argc == 2)
    {
        host = argv[1];
    }
    else
    {
        fprintf(stderr, "Wrong input!\n Input as in: ./client host \n");
        exit(1);
    }

    // Create socket
    udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_socket < 0)
    {
        perror("Couldn't create socket");
        exit(1);
    }

    // Get host name and data structure
    server = gethostbyname(host);
    if (server == NULL)
    {
        fprintf(stderr, "Unknown host: %s \n", host);
        exit(1);
    }
    bzero((char *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&sin.sin_addr.s_addr, server->h_length);
    sin.sin_port = htons(SERVER_PORT);
    server_len = sizeof(sin);

    // Get packet size
    size_t size_array = sizeof(packet_size) / sizeof(int);
    for (int i = 0; i < 31; i++) // 0 32 64 96 ... 992
    {
        packet_size[i] = i * 32;
        //printf("%d\n",packet_size[i]);
    }
    for (int i = 31; i < 64; i++) // 1024 2048 3072 ... 32700
    {
        packet_size[i] = (i - 31) * 1024;
        //printf("%d, %d\n",packet_size[i], i);
    }
    int size = 0;
    // Test for each package size

    for (int i_pkt = 0; i_pkt < size_array; i_pkt++)
    {
        //printf("%d, %d\n", packet_size[i_pkt], i_pkt);
        // Create message
        size = packet_size[i_pkt];
        char *buf = generate_message(size);
        //printf("\n\n Packet Size: %d Bytes", size);

        // Start lost packet count
        lost_packets = 0;

        // Get start time
        gettimeofday(&t1, NULL);

        // Start Test
        for (int i = 0; i < TOTAL_ITER; i++)
        {
            // Send Message
            count = sendto(udp_socket, buf, strlen(buf), 0, (struct sockaddr *)&sin, server_len);
            if (count < 0)
                perror("Transmiting error.");
            FD_ZERO(&read_sd_set);
            FD_SET(udp_socket, &read_sd_set);
            tv.tv_sec = 1;
            tv.tv_usec = 0;
            status = select(udp_socket + 1, &read_sd_set, NULL, NULL, &tv);
            if (FD_ISSET(udp_socket, &read_sd_set))
            {
                // Receive Message
                count = recvfrom(udp_socket, buf, strlen(buf), 0, (struct sockaddr *)&sin, &server_len);
                if (count < 0)
                    perror("Receiving error.");
            }
            else
            {
                printf("TIMEOUT UDP");
                lost_packets = lost_packets + 1;
            }
            //printf("%d, %d\n", packet_size[i_pkt], i_pkt);
        }

        // Lantecy in ms
        gettimeofday(&t2, NULL);
        elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;
        elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;

        // Measures avarage
        latency_time[i_pkt] = elapsedTime / TOTAL_ITER;
        throughput[i_pkt] = (packet_size[i_pkt] * 8) / latency_time[i_pkt];
        lost_packets_count[i_pkt] = lost_packets;
        printf("\n Packet size %d \n", packet_size[i_pkt]);
        printf("\n MeanLatency: %.f miliseconds \n", latency_time[i_pkt]);
        printf("\n Mean Throughput: %f bps\n", throughput[i_pkt]);
        free(buf);
    }

    // Save Results to csv

    FILE *pFile;

    pFile = fopen(file_name, "w");

    if (pFile != NULL)
    {
        fprintf(pFile, "packetSize(bytes),latency(s),throughput(bps), LostPacket");
        for (int i = 0; i < size_array; i++)
        {
            fprintf(pFile, "\n %d, %f, %f, %f", packet_size[i], latency_time[i],
                    throughput[i], lost_packets_count[i]);
        }
        fclose(pFile);
    }
    close(udp_socket);
}
