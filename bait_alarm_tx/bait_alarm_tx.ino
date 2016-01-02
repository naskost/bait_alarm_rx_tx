  /*
 * Wireless bite alarm TRANSMITER
 */
 
#include <avr/sleep.h>
#include <Manchester.h>

#define TX_PIN PB0  //pin where transmitter is connected

#define INTERRUPTPIN1 PCINT3 //on runnn ALARM 1
#define DATADIRECTIONPIN1 DDB3 //on runnn ALARM 1
#define PORTPIN1 PB3  //on runnn ALARM 1
#define READPIN1 PINB3  //on runnn ALARM 1

#define INTERRUPTPIN2 PCINT4 //on runnn ALARM 2
#define DATADIRECTIONPIN2 DDB4 //on runnn ALARM 2
#define PORTPIN2 PB4 //on runnn ALARM 2
#define READPIN2 PINB4 //on runnn ALARM 2

//uint8_t data_pb3_L[5] = {78, 64, 115, 0, 10}; // {N, @, s, rand, rand}
uint8_t data_pb3_H[5] = {78, 64, 115, 0, 30}; // {N, @, s, rand, rand}
//uint8_t data_pb4_L[5] = {78, 64, 115, 1, 11}; // {N, @, s, rand, rand}
uint8_t data_pb4_H[5] = {78, 64, 115, 1, 31}; // {N, @, s, rand, rand}

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

void setup(){
  cli();//disable interrupts during setup

  man.workAround1MhzTinyCore(); //add this in order for transmitter to work with 1Mhz Attiny85/84
  man.setupTransmit(TX_PIN, MAN_600);  
  
  PCMSK |= (1 << INTERRUPTPIN1);
  PCMSK |= (1 << INTERRUPTPIN2);
  GIMSK |= (1 << PCIE);   // enable PCINT interrupt in the general interrupt mask //SBI

  DDRB &= ~(1 << DATADIRECTIONPIN1); //cbi(DDRB, DATADIRECTIONPIN);//  set up as input  - pinX clear bit  - set to zero
  PORTB |= (1<< PORTPIN1); //cbi(PORTB, PORTPIN);// disable pull-up. hook up pulldown resistor. - set to zero
  
  DDRB &= ~(1 << DATADIRECTIONPIN2); //cbi(DDRB, DATADIRECTIONPIN);//  set up as input  - pinX clear bit  - set to zero
  PORTB |= (1<< PORTPIN2); //cbi(PORTB, PORTPIN);// disable pull-up. hook up pulldown resistor. - set to zero
  
  sei(); //last line of setup - enable interrupts after setup
}

//#define RANGE_TEST

void loop(){

#ifdef RANGE_TEST
  wait_for_second();
  man.transmitArray(5, data_pb3_H);
  wait_for_second();
  man.transmitArray(5, data_pb3_H);
  wait_for_second();
  man.transmitArray(5, data_pb3_H);
  //man.transmitArray(5, data_pb3_H);
  //man.transmitArray(5, data_pb3_H);
  //man.transmitArray(5, data_pb3_H);
  //man.transmitArray(5, data_pb3_H);  
  //man.transmitArray(5, data_pb3_H);
  //man.transmitArray(5, data_pb3_H);    
#else
    system_sleep();
#endif
}

// From http://interface.khm.de/index.php/lab/experiments/sleep_watchdog_battery/
void system_sleep() {
  cbi(ADCSRA,ADEN); // Switch Analog to Digital converter OFF
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Set sleep mode
  sleep_mode(); // System sleeps here
  sbi(ADCSRA,ADEN);  // Switch Analog to Digital converter ON
}

ISR(PCINT0_vect)
{
  //Since the PCINTn triggers on both rising and falling edge let's just looks for rising edge
  //i.e. pin goes to 5v
  byte pinState1, pinState2;
  pinState1 = (PINB >> READPIN1)& 1; //PINB is the register to read the state of the pins
  pinState2 = (PINB >> READPIN2)& 1; //PINB is the register to read the state of the pins  
  
  if (pinState1 > 0) {//look at the pin state on the pin PINB register- returns 1 if high
    man.transmitArray(5, data_pb3_H);
/*    wait_for_second();
    man.transmitArray(5, data_pb3_H);
    wait_for_second();
    man.transmitArray(5, data_pb3_H);
*/
  }
 
  if (pinState2 > 0) {//look at the pin state on the pin PINB register- returns 1 if high
    man.transmitArray(5, data_pb4_H);
/*    wait_for_second();
    man.transmitArray(5, data_pb4_H);
    wait_for_second();
    man.transmitArray(5, data_pb4_H);
*/    
  }  
}

void wait_for_second()
{
  for (int i=0; i < 10; ++i) 
  {
    delayMicroseconds(16383);
  }    
}
