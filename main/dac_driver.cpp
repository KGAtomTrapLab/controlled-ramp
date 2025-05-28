//#include "pins_arduino.h"
#include <Arduino.h>
#include <SPI.h>
#include "dac_driver.h"

/*
  PORTENTIA H7 / H7 LITE

  MOSI: pin SPI1
  MISO: pin SPI1
  SCK: pin SPI1

  DUE

  MOSI: SPI0
  MISO: SPI0
  SCK:  SPI0

  CS (DAC:    8
  CS (POT):   7

*/


/****************************** Fixed Varibles ******************************/


/****************************** Global Varibles ******************************/


const int DAC_CS_PIN = 8;

const int POT_CS_PIN = 7;

const int ADC_CS_PIN = 10;

const uint8_t SPI_CONFIG = 0b01110000; // Config bits for DAC



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

  SPI.begin();

  // SPISettings settings(20000000, MSBFIRST, SPI_MODE0);

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

void potWrite(uint8_t position, uint32_t cs_pin)
{
  uint16_t command = (0x00 << 8) | position; // Command byte (0x00) and data byte (position)

  digitalWrite(POT_CS_PIN, LOW);

  SPI.transfer16(command);

  digitalWrite(POT_CS_PIN, HIGH);

}

uint16_t readMCP3202(uint8_t channel, uint32_t cs_pin) 
{
  // channel: 0 or 1
  if (channel > 1) return 0;

  // MCP3202 protocol
  // First byte: start bit (1), single-ended (1), channel (channel << 6)
  byte command = 0b00000110 | (channel << 1);

  digitalWrite(cs_pin, LOW); // Select the ADC

  SPI.transfer(command);                // Start bit + single-ended + channel
  uint8_t highByte = SPI.transfer(0x00); // Receive high bits
  uint8_t lowByte  = SPI.transfer(0x00); // Receive low bits

  digitalWrite(cs_pin, HIGH); // Deselect the ADC

  // Combine the result (only 12 bits are used)
  uint16_t result = ((highByte & 0x0F) << 8) | lowByte;
  return result;
}



