#ifndef _DAC_DRIVER_H
#define _DAC_DRIVER_H

// Functions for interacting with spi devices (DAC, Pot.)

void init_spi();
void dacWrite(uint16_t value);
void potWrite(uint8_t position);

#endif /* _DAC_DRIVER_H */
