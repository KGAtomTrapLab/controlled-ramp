#include <Arduino.h>
#include "global_variables.h"
#include "dac_driver.h"
#include "serial_interface.h"

/****************************** Fixed Varibles ******************************/

#define RAMP_PIN                A6
#define PD1_PIN                 A0
#define PD2_PIN                 A1

#define ANALOG_RESOLUTION       12
#define MAX_DIGITAL             4095 // 2^(ANALOG_RESOLUTION) - 1

#define MAX_TIME                4294967295 // Max allowed unsigned long

#define FALL_RATIO_DIV          4 // Percentage of the ramp period that will be dropped across

#define FALL_SIZE               SAMPLE_SIZE / FALL_RATIO_DIV // Size of falling ramp


/****************************** Global Varibles ******************************/

volatile unsigned long CURRENT_TIME = 0; // global time varible in microseconds counted by timer

volatile int MAX_VOLTAGE = 5000; // Max Voltage (mV)

volatile int MIN_VOLTAGE = 0; // Min Voltage (mV)

volatile int CURRENT_VOLTAGE = 0; // Current voltage being output (mV)

volatile int DIGITAL_OUT = 0; // Current digital value being output

volatile unsigned long PERIOD = 200; // period in mS

volatile uint8_t POT_POSITION;

volatile double timeStep = 0;

volatile unsigned long time1 = 0; // used to time outputs

volatile unsigned long time2 = 0; // used as current time

volatile bool FALL_FLAG = false; 

// const int chipSelectPin = 7;

// const uint8_t SPI_CONFIG = 0b01110000;



/****************************** Function Prototypes ******************************/ 
void inc_output();
void output_voltage(unsigned int voltage);
void check_for_input();
void init_timer();

/****************************** Setup ******************************/
void setup() {
  //init_timer();  

  calc_time_step(); // Calculate initial time step

  init_spi(); // Initalize SPI for ext. DAC

  Serial.begin(115200); // Begin Serial
  while(!Serial);

  Serial.println("Serial Initilized");
  Serial.println(micros());
  Serial.println(micros());

}

/****************************** Loop ******************************/
void loop()
{
  // Take current time
  time2 = micros();

  // Compare with time of previous output
  if (time2 >= (time1 + timeStep))
  {
    dacWrite(DIGITAL_OUT);

    // Increment output voltage
    inc_output();

    // set new output time
    time1 = time2; 

  }
  //Serial.println(time1);

  check_for_input();

}

/****************************** Function Definitions ******************************/


// Increments current output and toggles falling flag
void inc_output()
{ 
  if (FALL_FLAG) // Count down if we want to ramp down
  {
    DIGITAL_OUT -= 4;

    if (DIGITAL_OUT <= 0)
    {
      DIGITAL_OUT = 0; // Reset output

      FALL_FLAG = false; // Reset falling flag
    }
  }
  else // Count up normally
  {
    DIGITAL_OUT += 1;
    
    if (DIGITAL_OUT >= MAX_DIGITAL)
    {
      FALL_FLAG = true; // Toggle falling flag to ramp down
    }
  }

}

// Recalculates global time step varible
void calc_time_step()
{
  timeStep = (PERIOD * 1000) / MAX_DIGITAL;
}

// Takes a voltage in mV and outputs a corrisponding digital value to the DAC via SPI
void output_voltage(unsigned int voltage)
{
  unsigned int output = map(voltage, MIN_VOLTAGE, MAX_VOLTAGE, 0, MAX_DIGITAL);
  dacWrite(output);
}

// Takes in a wiper position from 0-127 and sets the pot accordingly
void set_pot(uint8_t position)
{
  POT_POSITION = position;

  potWrite(POT_POSITION); // might be redundent but want to keep track of position globally

}
