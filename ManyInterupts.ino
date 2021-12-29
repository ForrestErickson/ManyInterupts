/* ManyInterupts
 *  A program to exersise many interupts
 *  Author: Forrest Lee Erickson
 *  Date: 20211228
 *  License: Public Domain, 
 *  Warrantee: This program is designed to kill you but is not guarenteed to do so. 
 *  It will also end all life on earth but may take a billion years or more.
 *  
 */

const int BAUDRATE = 115200;

long ANALOG_COMP_vect_counter = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(BAUDRATE);
  Serial.println("Beginning ManyInterupts");

  setup_ANALOG_COMP();

}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  Serial.println("ANALOG_COMP_vect_counter= " + String(ANALOG_COMP_vect_counter));

}


void setup_ANALOG_COMP(void){
  //Set up Analog Comparator Status Register
  ACSR =
    (0 << ACD) | // Analog Comparator: Enabled
    (0 << ACBG) | // Analog Comparator Bandgap Select: AIN0 is applied to the positive input
    (0 << ACO) | // Analog Comparator Output: Off
    (1 << ACI) | // Analog Comparator Interrupt Flag: Clear Pending Interrupt
    (1 << ACIE) | // Analog Comparator Interrupt: Enabled
    (0 << ACIC) | // Analog Comparator Input Capture: Disabled
    (1 << ACIS1) | (1 < ACIS0); // Analog Comparator Interrupt Mode: Comparator Interrupt on Rising Output Edge

  DIDR1 = (1<<AIN1D) | (1<<AIN0D) ; // Disable digital inputs on analog comparator.
}

ISR(ANALOG_COMP_vect){
  ANALOG_COMP_vect_counter++;
}//end ANALOG_COMP_vect
