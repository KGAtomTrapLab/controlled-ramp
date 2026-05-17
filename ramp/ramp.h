#ifndef _RAMP_H
#define _RAMP_H

// Varibles and functions used across multiple files relating to global Varibles
// Definitions in main file

extern volatile unsigned long PERIOD;

extern volatile uint8_t POT_POSITION;

//void set_pot_pos(uint8_t position);

void calc_time_step();

void set_pot(uint8_t position);

void set_ramp_start(int position);
void set_ramp_end(int position);

// Position within the arrays
int16_t data_array_position = 0;

#endif /* _RAMP_H */
