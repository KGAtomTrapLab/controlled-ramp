#ifndef _SPI_DEVICES_H
#define _SPI_DEVICES_H

// Functions for interacting with spi devices (DAC, Pot.)

void init_spi();
void dacWrite(uint16_t value);
void potWrite(uint8_t position);

// FEEDBACK READER FUNCTIONS
void collect_feedback();

void print_data_array();

#endif /* _SPI_DEVICES_H */
