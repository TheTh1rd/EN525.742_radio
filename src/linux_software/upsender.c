#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080  // Default port for UDP communication
#define PACKET_SIZE 1028 // Total UDP data bytes per frame
#define COUNTER_BYTES 4

// takes in ip address and packet count
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <IP_ADDRESS> <NUM_PACKETS>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *ip_address = argv[1];
    int num_packets = atoi(argv[2]);

    if (num_packets <= 0) {
        fprintf(stderr, "Error: Number of packets must be greater than 0.\n");
        return EXIT_FAILURE;
    }

    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[PACKET_SIZE] = "Hello, this is a UDP packet!";
    uint32_t counter = 0;

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        return EXIT_FAILURE;
    }

    memset(&server_addr, 0, sizeof(server_addr));

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, ip_address, &server_addr.sin_addr) <= 0) {
        perror("Invalid IP address");
        close(sockfd);
        return EXIT_FAILURE;
    }

    while (counter < num_packets ) {
        // Add 32-bit counter to the first 4 bytes
        memcpy(buffer, &counter, sizeof(counter));

        // Fill the rest of the buffer with dummy complex samples
        for (int i = 4; i < PACKET_SIZE; i += 4) {
            int16_t real = rand() % 32768; // Random 16-bit real part
            int16_t imag = rand() % 32768; // Random 16-bit imaginary part
            memcpy(buffer + i, &real, sizeof(real));
            memcpy(buffer + i + 2, &imag, sizeof(imag));
        }

        // Send the packet
        if (sendto(sockfd, buffer, PACKET_SIZE, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("Failed to send packet");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        printf("Sent packet with counter: %u\n", counter);

        counter++; // Increment the counter
        usleep(100000); // Sleep for 100ms 
    }

    close(sockfd);
    printf("All packets sent successfully.\n");
    return EXIT_SUCCESS;
}












