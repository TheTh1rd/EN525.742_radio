#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

// --- Peripheral Definitions (from other C files) ---
#define RADIO_TUNER_FAKE_ADC_PINC_OFFSET 0
#define RADIO_TUNER_TUNER_PINC_OFFSET 1
#define RADIO_TUNER_CONTROL_REG_OFFSET 2
#define RADIO_PERIPH_ADDRESS 0x43c00000

// --- Hardware Access Functions (from other C files) ---


volatile unsigned int * get_a_pointer(unsigned int phys_addr)
{
    int mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (mem_fd == -1) {
        // Cannot open /dev/mem.
        return NULL;
    }
    void *map_base = mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, phys_addr);
    if (map_base == MAP_FAILED) {
        // mmap failed.
        close(mem_fd);
        return NULL;
    }
    // The file descriptor can be closed after mmap is successful
    close(mem_fd);
    return (volatile unsigned int *)map_base;
}


void radioTuner_tuneRadio(volatile unsigned int *ptrToRadio, float tune_frequency)
{
    // The phase increment calculation
    float pinc = (-1.0 * tune_frequency) * (float)(1 << 27) / 125.0e6;
    *(ptrToRadio + RADIO_TUNER_TUNER_PINC_OFFSET) = (int)pinc;
}


void radioTuner_setAdcFreq(volatile unsigned int* ptrToRadio, float freq)
{
    // The phase increment calculation
    float pinc = freq * (float)(1 << 27) / 125.0e6;
    *(ptrToRadio + RADIO_TUNER_FAKE_ADC_PINC_OFFSET) = (int)pinc;
}

// --- Main CGI Logic ---

int main(void)
{
    char *len_str;
    int len;
    char *postdata;
    
    // Default values
    int adc_freq_hz = 0;
    int tune_freq_hz = 0;
    int streaming = 0;

    // Print the HTTP header required for CGI
    printf("Content-type:text/html\r\n\r\n");

    // Print the HTML document start
    printf("<html>\n<head>\n<title>Radio Configurator</title>\n</head>\n<body>\n");
    printf("<h2>Radio Configurator (C Version)</h2>\n");

    len_str = getenv("CONTENT_LENGTH");
    if (len_str == NULL || (len = atoi(len_str)) == 0) {
        printf("<p>Error: No form data received.</p>\n");
    } else {
        postdata = (char *)malloc(len + 1);
        if (postdata) {
            fgets(postdata, len + 1, stdin);
            
            // Parse the POST data robustly
            char *adc_ptr = strstr(postdata, "adc_freq_hz=");
            if (adc_ptr) sscanf(adc_ptr, "adc_freq_hz=%d", &adc_freq_hz);
            char *tune_ptr = strstr(postdata, "tune_freq_hz=");
            if (tune_ptr) sscanf(tune_ptr, "tune_freq_hz=%d", &tune_freq_hz);
            if (strstr(postdata, "streaming=streaming")) {
                streaming = 1;
            }

            printf("<p>Received Configuration:</p>\n<ul>\n");
            printf("<li>ADC Frequency: %d Hz</li>\n", adc_freq_hz);
            printf("<li>Tune Frequency: %d Hz</li>\n", tune_freq_hz);
            printf("<li>Streaming: %s</li>\n</ul>\n", streaming ? "Enabled" : "Disabled");

            printf("<p><b>Configuring hardware...</b></p>\n");
            volatile unsigned int *radio_periph = get_a_pointer(RADIO_PERIPH_ADDRESS);

            if (radio_periph) {
                radioTuner_setAdcFreq(radio_periph, (float)adc_freq_hz);
                radioTuner_tuneRadio(radio_periph, (float)tune_freq_hz);
                printf("<p>Frequencies set.</p>\n");

                // Kill any previously running streamer process.
                // Note: The process name must match the executable name from stream_udp_data.c
                system("killall stream_udp_data > /dev/null 2>&1");

                if (streaming) {
                    // The streamer needs the client's IP address to send data to.
                    // We get this from the REMOTE_ADDR CGI environment variable.
                    char *remote_addr = getenv("REMOTE_ADDR");
                    if (remote_addr) {
                        char command[256];
                        // NOTE: Assumes 'stream_udp_data' executable is in the same cgi-bin directory.
                        snprintf(command, sizeof(command), "./stream_udp_data %s &", remote_addr);
                        printf("<p>Starting UDP streamer to %s...</p>\n", remote_addr);
                        system(command);
                    } else {
                        printf("<p style='color:orange;'>Warning: Could not determine client IP address (REMOTE_ADDR not set). UDP streamer not started.</p>\n");
                    }
                } else {
                    printf("<p>UDP streamer is disabled.</p>\n");
                }
                printf("<p style='color:green;'>Configuration complete!</p>\n");
            } else {
                printf("<p style='color:red;'>Error: Could not map radio peripheral. Run with sufficient permissions.</p>\n");
            }
            free(postdata);
        }
    }

    printf("<br><a href=\"../index.html\">Back to main page</a>\n</body>\n</html>\n");

    return 0;
}