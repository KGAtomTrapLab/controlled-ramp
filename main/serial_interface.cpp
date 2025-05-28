#include <Arduino.h>
#include "global_variables.h"

// Checks the serial monitor for input commands
void check_for_input()
{
  if (Serial.available() > 0) // Check for input
  {
    String input = Serial.readStringUntil('\n');
    input.trim();

    //Serial.println("Took input");

    if (input.startsWith("help"))                                 // Check for help command
    {
      Serial.println("Available Commands:");
      Serial.println("******************************");
      Serial.println("period [period in ms]");
      Serial.println("pot [digital pot. value (1-127)]");
      Serial.println(" ");
    }

    else if (input.startsWith("period"))                          // Check for period command
    {
      input.remove(0, 6); // Remove command from string

      unsigned long newPeriod = input.toInt(); // Read new period
      if (newPeriod > 0)
      {
        PERIOD = newPeriod; // Accept new period
        calc_time_step();
        Serial.print("New Period: ");
        Serial.println(newPeriod);
      }
      else // Reject new period
      {
        Serial.println("Period must be valid integer");
      }

    }

    else if (input.startsWith("pot"))                             // Check for pot command
    {

      input.remove(0, 3); // Remove command from string

      uint8_t newPosition = input.toInt();

      if (newPosition >= 0 && newPosition <= 127) // Check for valid position
      {
        set_pot(newPosition);
        Serial.print("New Wiper Position: ");
        Serial.println((newPosition));
      }
      else // Invalid Wiper Position
      {
        Serial.println("Invalid Wiper Position, Range: (0 - 127)");
      }

    }

    else 
    {
      Serial.println("Invalid Command");
    }

  }

}
