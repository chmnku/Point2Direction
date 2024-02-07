/* 
 * HC-06 AT Command Reference Table
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
  Serial.begin(9600);
  bluetooth.begin(9600);

  /* Uncomment when using AT Commands" */
  // Serial.println("Enter AT Commands: ");
  /* --------------------------------- */
}

void loop()
{
  /* AT Command Usage for HC-06 Initial Configuration */
  // ROLE - SLAVE | NAME - Robot | PIN - 8888 | 98:DA:60:09:48:B9
  // ROLE  - MASTER |  NAME - Gesture | PIN - 8888 | 98:DA:60:03:B8:F8

  // if(bluetooth.available())
  // {
  //   Serial.write(bluetooth.read());
  // }

  // if(Serial.available())
  // {
  //   bluetooth.write(Serial.read());
  // }
  
  // /* ------------------------------------------------ */
  
  // Slave Device Code Snippet
  /* AT Command Usage for HC-06 Initial Configuration */
  if (bluetooth.available())
  {
    char receivedChar = bluetooth.read();
    Serial.write(receivedChar);

    // Check if 'w' was received from the Master
    if (receivedChar == 'w')
    {
      // If 'w' is received, print "Connection" to the Serial Monitor
      Serial.println("Connection");
    }
  }

  if (Serial.available())
  {
    bluetooth.write(Serial.read());
  }
  /* ------------------------------------------------ */

}