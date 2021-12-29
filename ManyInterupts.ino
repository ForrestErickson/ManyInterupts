/* ManyInterupts
    A program to exercise many interrupts on UNO
    Author: Forrest Lee Erickson
    Date: 20211228
    License: Public Domain,
    Warrantee: This program is designed to kill you but is not guarenteed to do so.
    It will also end all life on earth but may take a billion years or more.

  From data sheet ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061B.pdf page 74.
  VectorNo. Program Address(2) Source Interrupt Definition
  1 0x0000(1) RESET External Pin, Power-on Reset, Brown-out Reset and Watchdog System Reset
  2 0x0002 INT0 External Interrupt Request 0
  3 0x0004 INT1 External Interrupt Request 1
  4 0x0006 PCINT0 Pin Change Interrupt Request 0
  5 0x0008 PCINT1 Pin Change Interrupt Request 1
  6 0x000A PCINT2 Pin Change Interrupt Request 2
  7 0x000C WDT Watchdog Time-out Interrupt
  8 0x000E TIMER2_COMPA Timer/Counter2 Compare Match A
  9 0x0010 TIMER2_COMPB Timer/Counter2 Compare Match B
  10 0x0012 TIMER2_OVF Timer/Counter2 Overflow
  11 0x0014 TIMER1_CAPT Timer/Counter1 Capture Event
  12 0x0016 TIMER1_COMPA Timer/Counter1 Compare Match A
  13 0x0018 TIMER1_COMPB Timer/Counter1 Compare Match B
  14 0x001A TIMER1_OVF Timer/Counter1 Overflow
  15 0x001C TIMER0_COMPA Timer/Counter0 Compare Match A
  16 0x001E TIMER0_COMPB Timer/Counter0 Compare Match B
  17 0x0020 TIMER0_OVF Timer/Counter0 Overflow
  18 0x0022 SPI_STC SPI Serial Transfer Complete
  19 0x0024 USART_RX USART Rx Complete
  20 0x0026 USART_UDRE USART, Data Register Empty
  21 0x0028 USART_TX USART, Tx Complete
  22 0x002A ADC ADC Conversion Complete
  23 0x002C EE_READY EEPROM Ready
  24 0x002E ANALOG_COMP Analog Comparator
  25 0x0030 TWI 2-wire Serial Interface
  26 0x0032 SPM_Ready Store Program Memory Ready

  Hardware setup:
  Function generator with 4Vp-p square wave into D2, D3, D4 and D6
*/

#define BAUDRATE  115200  // Baud rate of UART in bps

//Wink constants
const int HIGH_TIME_LED = 900;
const int LOW_TIME_LED = 100;
long lastLEDtime = 0;
long nextLEDchange = 100; //time in ms.

//Main loop print constants
long lastPRINTtime = 0;
long nextPRINTchange = 1000; //time in ms.

//Variables for ISRs. Volatile so compilers will not eat them.
volatile long INT0_vect_counter = 0;
volatile long INT1_vect_counter = 0;
volatile long PCINT2_vect_counter = 0;    // PCINT20 aka For Pin D4
volatile long ANALOG_COMP_vect_counter = 0;

//Functions

//Wink the build in LED
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
  Serial.println("\n\n\n\nBeginning ManyInterupts");
  //  Serial.println("INT0= " + String(INT0));
  //  Serial.println("INT1= " + String(INT1));
  //  Serial.println("PCINT0= " + String(PCINT0));
  //  Serial.println("ANALOG_COMP= " + String(ANALOG_COMP));

  setup_INT0();       //Actualy sets both INT0 and INT1 up and enables
  //  setup_INT1();       //Todo
  setup_ANALOG_COMP();

  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off
}

void loop() {
  // put your main code here, to run repeatedly:

  //Print out the ISR count(s) every second.
  if (((millis() - lastPRINTtime) > nextPRINTchange) || (millis() < lastPRINTtime)) {
    lastPRINTtime = millis();
    Serial.println("ANALOG_COMP_vect_counter= " + String(ANALOG_COMP_vect_counter));
    Serial.println("INT0_vect_counter= " + String(INT0_vect_counter));
    Serial.println("INT1_vect_counter= " + String(INT1_vect_counter));
    Serial.println("PCINT2_vect_counter= " + String(PCINT2_vect_counter));
  }//Print once in a while

  wink();
}// end loop()

//Setup for ISRs and ISRs.

void setup_INT0(void) {
  //Set up INT0 for rise edge Also set up INT1 for falling
  //Set up EICRA ( External Interrupt Control Register A)
  EICRA =
    (1 << ISC00) | // INT0 LSB
    (1 << ISC01) | // INT0 MSB
    (0 << ISC10) | // INT0 LSB
    (1 << ISC11) & // INT0 LSB
    0x0F;                           //Upper nibble zero.

  // EIMSK – External Interrupt Mask Register
  //Enable both INT0 and INT1
  EIMSK =
    (1 << INT0) | // Mask for INT0
    (1 << INT1) & // Mask for INT1
    0x03;                           //Upper 6 MSB zero.
}//end setup_INT0

//Todo Make for INT1 and not affect INT0. This is just a place holder.
void setup_INT1(void) {
  //Set up INT1 for rise edge Also set up INT1 for falling
  //Set up EICRA for
  EICRA =
    (1 << ISC00) | // INT0 LSB
    (1 << ISC01) | // INT0 MSB
    (0 << ISC10) | // INT0 LSB
    (1 << ISC11) & // INT0 LSB
    0x0F;                           //Upper nibble zero.

  // EIMSK – External Interrupt Mask Register
  //Enable both INT0 and INT1
  EIMSK =
    (1 << INT0) | // Mask for INT0
    (1 << INT1) & // Mask for INT1
    0x03;                           //Upper 6 MSB zero.
}//end setup_INT1

ISR(INT0_vect) {
  INT0_vect_counter++;
}//end INT0_vect

ISR(INT1_vect) {
  INT1_vect_counter++;
}//end INT1_vect

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
}//end setup_ANALOG_COMP

ISR(ANALOG_COMP_vect) {
  ANALOG_COMP_vect_counter++;
}//end ANALOG_COMP_vect


//Setup PCINT2 for only PD4 interrupt on change.
void setup_PCINT2(void) {
  //Set up PCMSK2 – Pin Change Mask Register 2  
  PCMSK2 =
    (0 << PCINT16) | // Pin PD0
    (0 << PCINT17) | // Pin PD1
    (0 << PCINT18) | // Pin PD2
    (0 << PCINT19) | // Pin PD3
    (1 << PCINT20) | // Pin PD4
    (0 << PCINT21) | // Pin PD5
    (0 << PCINT22) | // Pin PD6
    (0 << PCINT23);  // Pin PD7

    //PCICR – Pin Change Interrupt Control Register
  PCICR = (1 << PCIE2) |
    0x00 ; // Only PCIN2 on PCIF2

    // Status Register (SREG) is set (one)
    bitSet(SREG, 7); //Eable interups.
    
}//end setup_PCINT2

ISR(PCINT2_vect) {
  PCINT2_vect_counter++;
}//end PCINT2_vect



// A template place holder for ISR setup and ISR
/*
  void setup_PLACE_HOLDER(void) {
  //Set up PLACE_HOLDER Status Register
  PLACESR =
    (0 << ACD) | // Analog Comparator: Enabled
    (0 << ACBG) | // Analog Comparator Bandgap Select: AIN0 is applied to the positive input
    (0 << ACO) | // Analog Comparator Output: Off
    (1 << ACI) | // Analog Comparator Interrupt Flag: Clear Pending Interrupt
    (1 << ACIE) | // Analog Comparator Interrupt: Enabled
    (0 << ACIC) | // Analog Comparator Input Capture: Disabled
    (1 << ACIS1) | (1 < ACIS0); // Analog Comparator Interrupt Mode: Comparator Interrupt on Rising Output Edge

  DIDR1 = (1 << AIN1D) | (1 << AIN0D) ; // Disable digital inputs on analog comparator.

   EIMSK =
    (1 << INT0) | // Mask for INT0
    (1 << INT1) | // Mask for INT1
    0x03;                           //Upper 6 MSB zero.
  }//end setup_ANALOG_COMP

  ISR(PLACE_HOLDER_vect) {
  PLACE_HOLDER_vect_counter++;
  }//end PLACE_HOLDER_vect

*/
