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

void potWrite(uint8_t position)
{
  uint16_t command = (0x00 << 8) | position; // Command byte (0x00) and data byte (position)

  digitalWrite(POT_CS_PIN, LOW);

  SPI.transfer16(command);

  digitalWrite(POT_CS_PIN, HIGH);

}




