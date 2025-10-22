#include <SPI.h>

// MCP3202 configuration
const int CS_PIN = 10; // Chip select pin

void setup() {
  Serial.begin(9600);
  
  // Set CS pin as output
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH); // Deselect the chip

  // Initialize SPI
  SPI.begin();
  SPI.setClockDivider(16); // Adjust as needed for stability
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
}

uint16_t readMCP3202(uint8_t channel) {
  // channel: 0 or 1
  if (channel > 1) return 0;

  // MCP3202 protocol
  // First byte: start bit (1), single-ended (1), channel (channel << 6)
  byte command = 0b00000110 | (channel << 1);

  digitalWrite(CS_PIN, LOW); // Select the ADC

  SPI.transfer(command);                // Start bit + single-ended + channel
  uint8_t highByte = SPI.transfer(0x00); // Receive high bits
  uint8_t lowByte  = SPI.transfer(0x00); // Receive low bits

  digitalWrite(CS_PIN, HIGH); // Deselect the ADC

  // Combine the result (only 12 bits are used)
  uint16_t result = ((highByte & 0x0F) << 8) | lowByte;
  return result;
}

void loop() {
  uint16_t adc0 = readMCP3202(0);
  // uint16_t adc1 = readMCP3202(1);
  Serial.print("Channel 0: ");
  for (int i = 0; i < adc0 / 100; i ++)
    Serial.print("o");
  Serial.println();
  // Serial.print(" | Channel 1: ");
  // Serial.println(adc1);
  
}
