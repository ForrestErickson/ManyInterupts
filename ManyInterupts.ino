/* ManyInterupts
    A program to exersise many interupts
    Author: Forrest Lee Erickson
    Date: 20211228
    License: Public Domain,
    Warrantee: This program is designed to kill you but is not guarenteed to do so.
    It will also end all life on earth but may take a billion years or more.

*/

#define BAUDRATE  115200  // Baud rate of UART in bps

//Wink constants
const int HIGH_TIME_LED = 900;
const int LOW_TIME_LED = 100;
long lastLEDtime = 0;
long nextLEDchange = 100; //time in ms.

long lastPRINTtime = 0;
long nextPRINTchange = 1000; //time in ms.

long ANALOG_COMP_vect_counter = 0;

//Functions

void wink() {
  //Wink the LED
  if (((millis() - lastLEDtime) > nextLEDchange) || (millis() < lastLEDtime)) {
    if (digitalRead(LED_BUILTIN) == LOW) {
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
      nextLEDchange = HIGH_TIME_LED;
    } else {
      digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
      nextLEDchange = LOW_TIME_LED;
    }
    lastLEDtime = millis();
  }//end LED wink
}//wink


void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);      // set the LED pin mode
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  Serial.begin(BAUDRATE);
  delay(100);
  Serial.println("Beginning ManyInterupts");

  setup_ANALOG_COMP();

  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off
}

void loop() {
  // put your main code here, to run repeatedly:

//Print out the compaitor count every second.
  if (((millis() - lastPRINTtime) > nextPRINTchange) || (millis() < lastPRINTtime)) {
    Serial.println("ANALOG_COMP_vect_counter= " + String(ANALOG_COMP_vect_counter));
    lastPRINTtime = millis();
  }//Print once in a while

  wink();
}


void setup_ANALOG_COMP(void) {
  //Set up Analog Comparator Status Register
  ACSR =
    (0 << ACD) | // Analog Comparator: Enabled
    (0 << ACBG) | // Analog Comparator Bandgap Select: AIN0 is applied to the positive input
    (0 << ACO) | // Analog Comparator Output: Off
    (1 << ACI) | // Analog Comparator Interrupt Flag: Clear Pending Interrupt
    (1 << ACIE) | // Analog Comparator Interrupt: Enabled
    (0 << ACIC) | // Analog Comparator Input Capture: Disabled
    (1 << ACIS1) | (1 < ACIS0); // Analog Comparator Interrupt Mode: Comparator Interrupt on Rising Output Edge

  DIDR1 = (1 << AIN1D) | (1 << AIN0D) ; // Disable digital inputs on analog comparator.
}

ISR(ANALOG_COMP_vect) {
  ANALOG_COMP_vect_counter++;
}//end ANALOG_COMP_vect
