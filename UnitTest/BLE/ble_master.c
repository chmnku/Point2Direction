/* 
 * HC-05 AT Command Reference Table
 * ------------------------------------------------------------------
 * | Command          | AT Command Input    | Expected Response      |
 * ------------------------------------------------------------------
 * | Basic Command    | "AT"                | "OK"                   |
 * | Name Setting     | "AT+NAME"           | "OKsetname"            |
 * | PIN Code Setting | "AT+PIN"            | "OKsetPIN"             |
 * | Baud Rate Change | "AT+BAUD"           | "OK<baudrate_value>"   |
 * | Version Info     | "AT+VERSION"        | "OKlinvorV1.8"         |
 * | Set a Role       | "AT+ROLE=(M or S)"  | "OK+ROLE:M or S"       |
 * ------------------------------------------------------------------
 */

#include <SoftwareSerial.h>

// Define UART PIN
#define TXD 8
#define RXD 9

// Object Creation and Initialization
// SoftwareSerial(uint8_t receivePin, uint8_t transmitPin, bool inverse_logic = false);
SoftwareSerial bluetooth(RXD, TXD);

void setup()
{
  // pinMode(7, OUTPUT);
  // digitalWrite(7, HIGH);
  Serial.begin(9600);
  bluetooth.begin(9600);

  /* Uncomment when using AT Commands" */
  // Serial.println("Enter AT Commands: ");
  /* --------------------------------- */
}

void loop()
{
  /* AT Command Usage for HC-06 Initial Configuration */
  // SLAVE: NAME - Robot | PIN - 0000 | ROLE - Slave
  // MASTER:  NAME - Gesture | PIN - 0000 | ROLE - Master

  // if(bluetooth.available())
  // {
  //   Serial.write(bluetooth.read());
  // }

  // if(Serial.available())
  // {
  //   bluetooth.write(Serial.read());
  // }
  
  // /* ------------------------------------------------ */

  // Master Device Code Snippet
  /* AT Command Usage for HC-06 Initial Configuration */
  if (bluetooth.available())
  {
    Serial.write(bluetooth.read()); // Existing code: Echo whatever is received
  }

  if (Serial.available())
  {
    char receivedChar = Serial.read();
    bluetooth.write(receivedChar);

    // Check if 'w' was received from the Serial Monitor
    if (receivedChar == 'w')
    {
      // Send 'w' to the Slave
      bluetooth.write('w');
    }
  }
}