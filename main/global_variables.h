#ifndef _GLOBAL_VARIABLES_H
#define _GLOBAL_VARIABLES_H

// Varibles and functions used across multiple files relating to global Varibles
// Definitions in main file

extern volatile unsigned long PERIOD;

extern volatile uint8_t POT_POSITION;

//void set_pot_pos(uint8_t position);

void calc_time_step();

void set_pot(uint8_t position);

#endif /* _GLOBAL_VARIABLES_H */
