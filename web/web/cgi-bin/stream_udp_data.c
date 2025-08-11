/**
 * @file stream_udp_data.c
 * @brief Streams data from an AXI FIFO to a specified IP address via UDP.
 *
 * This program runs continuously, monitoring the contents of the AXI FIFO.
 * When enough data is available for a full UDP packet, it reads the data,
 * prepends a 32-bit packet counter, and sends it to the destination.
 *
 * It combines the hardware access logic from milestone2.c and the UDP
 * networking logic from upsender.c.
 *
 * Usage: ./stream_udp_data <IP_ADDRESS>
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <unistd.h>

// Use _DEFAULT_SOURCE for modern glibc to get definitions for usleep, etc.
#define _BSD_SOURCE

// --- Peripheral and Memory Map Definitions ---
#define AXI_FIFO_STREAM_BASE_ADDR 0x43C10000

// FIFO Register Offsets (as word offsets for uint* pointers)
#define XLLF_RDFO_WORD_OFFSET (0x0000001c / 4)  /**< Receive Occupancy */
#define XLLF_RDFD_WORD_OFFSET (0x00000020 / 4)  /**< Receive Data */

// --- UDP Packet Configuration ---
#define UDP_PORT 8080
#define COUNTER_BYTES 4
#define SAMPLES_PER_PACKET 256 // Number of 32-bit samples
#define PAYLOAD_BYTES (SAMPLES_PER_PACKET * 4)
#define PACKET_SIZE (PAYLOAD_BYTES + COUNTER_BYTES)

/**
 * @brief Maps a physical memory address into virtual memory.
 *
 * Uses /dev/mem and mmap to get a userspace pointer to a physical address.
 *
 * @param phys_addr The physical address to map.
 * @return A volatile unsigned int pointer to the mapped memory, or NULL on error.
 */
volatile unsigned int * get_a_pointer(unsigned int phys_addr)
{
    int mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (mem_fd == -1) {
        perror("Error opening /dev/mem");
        return NULL;
    }
    void *map_base = mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, phys_addr);
    if (map_base == MAP_FAILED) {
        perror("mmap failed");
        close(mem_fd);
        return NULL;
    }
    close(mem_fd);
    return (volatile unsigned int *)map_base;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <IP_ADDRESS>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *ip_address = argv[1];
    uint32_t packet_counter = 0;

    // --- Setup UDP Socket ---
    int sockfd;
    struct sockaddr_in server_addr;
    unsigned char udp_buffer[PACKET_SIZE];

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        return EXIT_FAILURE;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_PORT);
    if (inet_pton(AF_INET, ip_address, &server_addr.sin_addr) <= 0) {
        perror("Invalid IP address");
        close(sockfd);
        return EXIT_FAILURE;
    }

    // --- Map Hardware ---
    volatile unsigned int *fifo_base = get_a_pointer(AXI_FIFO_STREAM_BASE_ADDR);
    if (fifo_base == NULL) {
        fprintf(stderr, "Failed to map FIFO memory.\n");
        close(sockfd);
        return EXIT_FAILURE;
    }

    printf("Starting UDP stream to %s:%d\n", ip_address, UDP_PORT);

    // --- Main Streaming Loop ---
    while (1) {
        unsigned int fifo_occupancy = *(fifo_base + XLLF_RDFO_WORD_OFFSET);

        if (fifo_occupancy >= SAMPLES_PER_PACKET) {
            memcpy(udp_buffer, &packet_counter, COUNTER_BYTES);
            volatile unsigned int *payload_ptr = (volatile unsigned int *)(udp_buffer + COUNTER_BYTES);
            for (int i = 0; i < SAMPLES_PER_PACKET; i++) {
                payload_ptr[i] = *(fifo_base + XLLF_RDFD_WORD_OFFSET);
            }

            sendto(sockfd, udp_buffer, PACKET_SIZE, 0, (const struct sockaddr *)&server_addr, sizeof(server_addr));
            packet_counter++;
        } else {
            usleep(100); // Wait for 100 microseconds to avoid busy-polling
        }
    }

    close(sockfd); // Unreachable, but good practice
    return EXIT_SUCCESS;
}