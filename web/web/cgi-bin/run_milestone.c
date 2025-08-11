                                   // reads the FIFO count and then the FIFO itself until 480000 words have been read, 
// this can be as simple as reading the FIFO count register – if it is greater than 0, 
// then reading the FIFO data register that many times, and then incrementing the total count of words that you have read….
// Until you get to 480000 total words read


#include <stdio.h>
#include <sys/mman.h> 
#include <fcntl.h> 
#include <unistd.h>
#define _BSD_SOURCE

#define AXI_FIFO_STREAM_BASE_ADDR 0x43C10000
#define RADIO_TUNER_FAKE_ADC_PINC_OFFSET 0
#define RADIO_TUNER_TUNER_PINC_OFFSET 1
#define RADIO_TUNER_CONTROL_REG_OFFSET 2
#define RADIO_TUNER_TIMER_REG_OFFSET 3
#define RADIO_PERIPH_ADDRESS 0x43c00000

// Offsets are in bytes. Divide by 4 for word-based addressing with unsigned int*
#define XLLF_ISR_WORD_OFFSET  (0x00000000 / 4)  /**< Interrupt Status */
#define XLLF_IER_WORD_OFFSET  (0x00000004 / 4)  /**< Interrupt Enable */

#define XLLF_TDFR_WORD_OFFSET (0x00000008 / 4)  /**< Transmit Reset */
#define XLLF_TDFV_WORD_OFFSET (0x0000000c / 4)  /**< Transmit Vacancy */
#define XLLF_TDFD_WORD_OFFSET (0x00000010 / 4)  /**< Transmit Data */
#define XLLF_TLF_WORD_OFFSET  (0x00000014 / 4)  /**< Transmit Length */

#define XLLF_RDFR_WORD_OFFSET (0x00000018 / 4)  /**< Receive Reset */
#define XLLF_RDFO_WORD_OFFSET (0x0000001c / 4)  /**< Receive Occupancy */
#define XLLF_RDFD_WORD_OFFSET (0x00000020 / 4)  /**< Receive Data */
#define XLLF_RLF_WORD_OFFSET  (0x00000024 / 4)  /**< Receive Length */
#define XLLF_LLR_WORD_OFFSET  (0x00000028 / 4)  /**< Local Link Reset */
#define XLLF_TDR_WORD_OFFSET  (0x0000002C / 4)  /**< Transmit Destination  */
#define XLLF_RDR_WORD_OFFSET  (0x00000030 / 4)  /**< Receive Destination  */

// the below code uses a device called /dev/mem to get a pointer to a physical
// address.  We will use this pointer to read/write the custom peripheral
volatile unsigned int * get_a_pointer(unsigned int phys_addr)
{
	int mem_fd = open("/dev/mem", O_RDWR | O_SYNC); 
	void *map_base = mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, phys_addr); 
	volatile unsigned int *radio_base = (volatile unsigned int *)map_base; 
	return (radio_base);
}

void radioTuner_tuneRadio(volatile unsigned int *ptrToRadio, float tune_frequency)
{
	float pinc = (-1.0*tune_frequency)*(float)(1<<27)/125.0e6;
	*(ptrToRadio+RADIO_TUNER_TUNER_PINC_OFFSET)=(int)pinc;
}

void radioTuner_setAdcFreq(volatile unsigned int* ptrToRadio, float freq)
{
	float pinc = freq*(float)(1<<27)/125.0e6;
	*(ptrToRadio+RADIO_TUNER_FAKE_ADC_PINC_OFFSET) = (int)pinc;
}

void play_tune(volatile unsigned int *ptrToRadio, float base_frequency)
{
	int i;
	float freqs[1] = {1760.0};
	float durations[1] = {15};
    radioTuner_setAdcFreq(ptrToRadio,freqs[0]+base_frequency);
}

int main()
{

// first, get a pointer to the peripheral base address using /dev/mem and the function mmap
    volatile unsigned int *my_periph = get_a_pointer(RADIO_PERIPH_ADDRESS);	
    volatile unsigned int *fifo_base = get_a_pointer(AXI_FIFO_STREAM_BASE_ADDR);
    unsigned int fifo_occupancy;
    volatile unsigned int received_data;
    printf("\r\n\r\n\r\nLab 11 Lorenzo Pizarro - milestone 2\n\r");
    *(my_periph+RADIO_TUNER_CONTROL_REG_OFFSET) = 3;
    *(my_periph+RADIO_TUNER_CONTROL_REG_OFFSET) = 0; // make sure radio isn't in reset and radio is enabled 0b10
    radioTuner_tuneRadio(my_periph,30e6);
    printf("Playing Tune at near 30MHz\r\n");
    play_tune(my_periph,30e6);
    // check recievie occupancy, read recieve data that many times, repeat until read 480000 words from recievedata    
    int total_words_read = 0;
    const int target_words = 480000;

    while (total_words_read < target_words) {
        // Read the receive occupancy (number of words available in the FIFO)
        fifo_occupancy = *(fifo_base + XLLF_RDFO_WORD_OFFSET);
        printf("Fifo occupancy: %d .\n", fifo_occupancy);
        if (fifo_occupancy > 0) {
            // Read data from the FIFO
            for (int i = 0; i < fifo_occupancy; i++) {
                received_data = *(fifo_base + XLLF_RDFD_WORD_OFFSET);
                // You can process the received_data here if needed
                total_words_read++;
                if (total_words_read >= target_words) {
                    break; // Exit if target reached
                }
            }
        } else {
            // If FIFO is empty, wait a bit before checking again to avoid busy-waiting
            usleep(100); // Sleep for 100 microseconds
        }
        printf("Successfully read %d words from the FIFO.\n", total_words_read);
    }
    printf("Finished reading %d words from the FIFO.\n", total_words_read);
  
    return 0;
}
