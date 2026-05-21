//#include "pins_arduino.h"
#include <Arduino.h>
#include <SPI.h>
#include "spi_devices.h"
#include "ramp.h"

/****************************** Fixed Varibles ******************************/
const int DAC_CS_PIN = 8;

const int POT_CS_PIN = 7;

const uint8_t SPI_CONFIG = 0b01110000; // Config bits for DAC

// Dedicated pins for talking to the ADCs - set these based on the connected pin on the board
int adc_1_cs_pin = 48;
int adc_2_cs_pin = 50;

// Arrays for storing photodiode data

int16_t data_array_0[5000];
int16_t data_array_1[5000];

/****************************** Functions ******************************/

// Initalize SPI conneciton
void init_spi()
{
  // Initalize I/O
  pinMode(DAC_CS_PIN, OUTPUT);

  pinMode(POT_CS_PIN, OUTPUT);

  // Start DAC Chip Select HIGH
  digitalWrite(DAC_CS_PIN, HIGH);

  // Start POT Chip Select HIGH
  digitalWrite(POT_CS_PIN, HIGH);

  // Setup for ADCs
  pinMode(adc_1_cs_pin, OUTPUT);
  digitalWrite(adc_1_cs_pin, HIGH);
  pinMode(adc_2_cs_pin, OUTPUT);
  digitalWrite(adc_2_cs_pin, HIGH);

  SPI.begin();

  // Delay for initilizatino
  delay(1000);

}

// Outputs a 12 bit int to the external DAC via SPI
void dacWrite(uint16_t value)
{
  uint16_t spiOutput = (SPI_CONFIG << 8) | value;

  digitalWrite(DAC_CS_PIN, LOW);

  SPI.transfer16(spiOutput);

  digitalWrite(DAC_CS_PIN, HIGH);

}

void potWrite(uint8_t position)
{
  uint16_t command = (0x00 << 8) | position; // Command byte (0x00) and data byte (position)

  digitalWrite(POT_CS_PIN, LOW);

  SPI.transfer16(command);

  digitalWrite(POT_CS_PIN, HIGH);

}

/****************************** PHOTODIODE READER FUNCTIONS ******************************/

/* Read from the ADC. sends 12 bits.
 *  TODO: Is the null bit included?
 */
uint16_t read_adc(int cs_pin)
{

    digitalWrite(cs_pin, LOW);

    uint8_t b0 = SPI.transfer(0x01);                  
    uint8_t b1 = SPI.transfer(0x80);                  
    uint8_t b2 = SPI.transfer(0x00);      

    // Serial.println("Begin");
    // Serial.println(b1 & 0X0F);
    // Serial.println(b2);            

    uint16_t result = ((b1 & 0x0F) << 8) | b2;

    // Serial.println(result);

    digitalWrite(cs_pin, HIGH);

    return result;
}

int16_t get_data_dual_channel(uint8_t chnl)
{
    /*
        Get the data from just one channel.

        Channel should be either 0 or 1
    
    */
    if (chnl == 0)
        return (int16_t) read_adc(adc_1_cs_pin);
    if (chnl == 1)
        return (int16_t) read_adc(adc_2_cs_pin);
    else
        return 0;
}

void write_short(int16_t input_value)
{
    Serial.write((uint8_t *)&input_value, sizeof(int16_t));
}

// Collect the Photodiode feedback, and append to the array
void collect_feedback()
{
    // Safety code to prevent writing outside of the array.
    // If the code is working perfectly, this will never be true,
    // But it verifies correct writing of data.
    if (data_array_position >= 4096) return;
    // Read from PD 0
    int16_t channel_0 = get_data_dual_channel(0);
    // Serial.println(channel_0);

    // Read from PD 1
    int16_t channel_1 = get_data_dual_channel(1);
    // Serial.println(channel_1);

    // Store
    data_array_0[data_array_position] = channel_0;
    data_array_1[data_array_position] = channel_1;
    // Increment position
    data_array_position++;

}

void write_array(uint16_t array_number)
{
    int16_t* selected_array;
    if (array_number == 0) selected_array = data_array_0;
    else selected_array = data_array_1;
    for (int i = 0; i < data_array_position; i++)
    {
        write_short(selected_array[i]);
    }
}

/* Print the data array to the serial terminal.
    FORMAT: 
        HEADER 0xCC for single channel, 0xCD for dual channel
        length x 16 bit signed values for channel 0
        if dual channel, length x 16 bit signed values for channel 1
        FOOTER 0xCB
    
*/
void print_data_array()
{
    // Write length of array
    write_short(data_array_position);


    write_array(0);
    write_array(1);

}



