#include <SPI.h>
#include <Arduino.h>


/****************************** Global Varibles ******************************/


// Dedicated pins for talking to the ADCs - set these based on the connected pin on the board
int adc_1_cs_pin = 10;
int adc_2_cs_pin = 11;

// Channel mode: 1 for single channel, 2 for dual channel
int channel_mode = 2;

// Arrays for storing photodiode data
// Position within the arrays
int16_t data_array_position = 0;

int16_t data_array_0[4096];
int16_t data_array_1[4096];

int16_t valley_position = 0;

/****************************** Functions ******************************/

void reader_setup() {
  // Initialize the SPI
  pinMode(adc_1_cs_pin, OUTPUT);
  digitalWrite(adc_1_cs_pin, HIGH);
  pinMode(adc_2_cs_pin, OUTPUT);
  digitalWrite(adc_2_cs_pin, HIGH);
  SPI.begin();
  SPI.setClockDivider(12); // Adjust as needed for stability
  SPI.setDataMode(SPI_MODE1);
  SPI.setBitOrder(MSBFIRST);
  
  delay(1000);
}

/* Read from the ADC. sends 12 bits.
 *  TODO: Is the null bit included?
 */
uint16_t read_adc(int cs_pin)
{
  // Want to send 0x80:
  // Start, Pseudo-Differential, Sign, Most Significant Bit First
  digitalWrite(cs_pin, LOW);
  SPI.transfer(0x08);
  uint8_t null_to_5 = SPI.transfer(0x00);
  uint8_t bit4_to_0 = SPI.transfer(0x00);

  // Disable the ADC
  digitalWrite(cs_pin, HIGH);

  // Processing the information we received
  uint16_t result = 0x0;
  result = null_to_5;
  result = result << 4;
  result |= bit4_to_0 >> 4;

  return result;
}

int16_t get_data_single_channel()
{
    /*
        Gather the data from the ADCs and prepare a combined value to send.

        Takes both values, inverts the second value, and adds them together.
    
    */
    // Read both values, converting them into signed integers.
    int16_t adc_1_value = (int16_t) read_adc(adc_1_cs_pin);
    int16_t adc_2_value = (int16_t) read_adc(adc_2_cs_pin);

    // Flip the second one, add them together
    int16_t added_value = adc_1_value - adc_2_value;
    // Serial.print("a:");
    // Serial.println(added_value);

    return added_value;
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

void write_header()
{
    /*
        Write the header to indicate data is coming next
    */
    uint8_t header = 0xCC;
    Serial.write(&header, sizeof(uint8_t));
}

void write_footer()
{
    /*
        Write the header to indicate data is coming next
    */
    uint8_t footer = 0xCB;
    Serial.write(&footer, sizeof(uint8_t));
}

void write_short(int16_t input_value)
{
    Serial.write((uint8_t *)&input_value, sizeof(int16_t));
}

void output_data()
{
    /*
        Surround the value in a 0xCC and a 0xCB to make sure any mismatch does not occur.
    */

    // Make a buffer for the values:
    // First place the head character, then the first byte of the value, then the last byte of the value, then finally the tail signal
    write_header();
    if (channel_mode == 1)
    {
        write_short(get_data_single_channel());
    }
    // Print the dual channel value
    if (channel_mode == 2)
    {
        int16_t channel_0 = get_data_dual_channel(0);
        int16_t channel_1 = get_data_dual_channel(1);
        write_short(channel_0);
        write_short(channel_1);
    }

    write_footer();
}

// Collect the Photodiode feedback, and append to the array
void collect_feedback()
{
    // Read from PD 0
    int16_t channel_0 = get_data_dual_channel(0);

    // Read from PD 1
    int16_t channel_1 = get_data_dual_channel(1);

    // Store
    data_array_0[data_array_position] = channel_0;
    data_array_1[data_array_position] = channel_1;
    // Increment position
    data_array_position++;

}

// Record the valley position
void record_valley_pos()
{
    valley_position = data_array_position;
}

/* Print the data array to the serial terminal.
    FORMAT: 
        HEADER 0xCC for single channel, 0xCD for dual channel
        16 bit length of each array
        16 bit position of the valley point
        length x 16 bit signed values for channel 0
        if dual channel, length x 16 bit signed values for channel 1
        FOOTER 0xCB
    
*/
void print_data_array()
{
    write_header();
    // Write length of array
    write_short(data_array_position);

    write_short(valley_position);

    write_footer();
}

write_array(uint16_t array_number)
{
    uint16_t * selected_array;
    if (array_number == 0) selected_array = data_array_0;
    else selected_array = data_array_1;
    for (int i = 0; i < data_array_position; i++)
    {
        write_short(selected_array[i]);
    }
}
