/*

 I've extended the serial event example a bit, so that it takes
 commands received on the serial interface and executes them
 in a function called handle_command().
 
 Try to open the "Serial Monitor" and try to write:
 
   led 255
 
 to turn on the led, and
 
   led 0
 
 to turn it off. The number sets the brightness of the LED.
 
 jpp 2012

 ---

 Serial Event example
 
 When new serial data arrives, this sketch adds it to a String.
 When a newline is received, the loop prints the string and 
 clears it.
 
 A good test for this is to try it with a GPS receiver 
 that sends out NMEA 0183 sentences. 
 
 Created 9 May 2011
 by Tom Igoe
 
 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/SerialEvent
 
 */

const int max_length = 20;
char *inputString = (char *)malloc(max_length);
int str_pos = 0;
boolean stringComplete = false;  // whether the string is complete

// For demo
const int ledPin =  13;      // the number of the LED pin

void setup() {
  // initialize serial:
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT); // Built in LED
}

void move_steps(int steps)
{
  Serial.print("Moving: ");
  Serial.println(steps);
}

void handle_command(char* cmd, char* arg)
{
  Serial.print("Command: ");
  Serial.println(cmd);
  Serial.print("Argument: ");
  Serial.println(arg);
  if (strcmp("move", cmd)==0)
  {
    int steps = atoi(arg);
    move_steps(steps);
    Serial.println("OK");
  }
  if (strcmp("led", cmd)==0)
  {
    int value = atoi(arg);
    if ((value>=0) and (value<256))
    {
      analogWrite(13, value);
      Serial.println("OK");
    }
    else
    {
      Serial.println("Out of range (0<=value<=255)");
    }
  }
  else
  {
    Serial.println("ERR: UNKNOWN COMMAND");
  }
}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    char *command = inputString;
    char *arg = inputString;
    for (int i=0; i<max_length; i++)
    {
      if (inputString[i]=='\0')
        break;
      if (inputString[i]==' ')
      {
        inputString[i] = '\0';
        arg++;
        break;
      }
      arg++;
    }
    handle_command(command, arg);
    // Reset input str
    str_pos = 0;
    stringComplete = false;
  }
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    if ((str_pos<max_length) && (inChar!='\n'))
    {
      inputString[str_pos] = inChar;
      str_pos++;
    }
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
      // End a string with a null
      if (str_pos<max_length)
        inputString[str_pos]='\0';
      else
        inputString[max_length-1]='\0';
      str_pos = 0;
      }
  }
}


