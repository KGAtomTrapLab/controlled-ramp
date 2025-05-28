#include <Arduino.h>
#include  <cstring>
#include "global_variables.h"
#include "dac_driver.h"

// Checks the serial monitor for input commands
void check_for_input()
{
  if (Serial.available() > 0)
  {
    // Read in the input and trim it. It input string shoulc be exactly what was entered by the user
    String input = Serial.readStringUntil('\n');
    input.trim();

    // Convert input to modifiable char array. This will let us use strtok.
    // Note that this buffer can only hold a max command size od 100 characters
    // this may need to be increased in the future if we want commands to be hella long (pain)
    char inputBuffer[100];
    input.toCharArray(inputBuffer, sizeof(inputBuffer));

    // Get the first token, should be the command name
    char* token = strtok(inputBuffer, " ");

    // Check for help command
    if (token != NULL && String(token).equals("help"))
    {
      Serial.println("Available Commands:");
      Serial.println("******************************");
      Serial.println("period [period in ms]");
      Serial.println("pot [digital pot. value (1-127)]");
      Serial.println(" ");
    }

    // check for period command.
    else if (token != NULL && String(token).equals("period"))
    {
      // get the next token, should the the period we want
      char* arg = strtok(NULL, " ");
      if (arg != NULL)
      {
        unsigned long newPeriod = String(arg).toInt(); // convert the token to a string so we can use toInt to get it as a number
        if (newPeriod > 0)
        {
          // set the period
          PERIOD = newPeriod;
          calc_time_step();
          Serial.print("New Period: ");
          Serial.println(newPeriod);
        }
        else
        {
          Serial.println("Period must be valid integer");
        }
      }
    }

    // check for pot command
    else if (token != NULL && String(token).equals("pot"))
    {
      // get the next token, it should be the period length
      char* arg = strtok(NULL, " ");
      if (arg != NULL)
      {
        int newPosition = String(arg).toInt(); // convert the token to a string so we can use toInt to get it as a number
        if (newPosition >= 0 && newPosition <= 127)
        {
          set_pot(newPosition); //TODO: will want to add another argument so the pin can be set as well, will be easy :/
          Serial.print("New Wiper Position: ");
          Serial.println(newPosition);
        }
        else
        {
          Serial.println("Invalid Wiper Position, Range: (0 - 127)");
        }
      }
    }

    else
    {
      Serial.println("Invalid Command");
    }
  }
}
