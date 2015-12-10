/*
 * Pin Change Interrupt Example
 * Version: 1.0
 * Author: Alex from Inside Gadgets (http://www.insidegadgets.com)
 * Created: 25/02/2011
 *
 * Demonstration of the pin change interrupt
 * LED on digital pin 0
 * Interrupt on digital pin 1
 * 10k resistor on digital pin 1 to GND
 *
 */
 
#include <avr/sleep.h>
#include <Manchester.h>

#define TX_PIN PB0  //pin where your transmitter is connected
//#define LED_PIN PB2 //pin for blinking LED
#define RUN_INTERRUPT1 PCINT3//on runnn
#define RUN_INTERRUPT2 PCINT4//on runnn

#define RUN_PIN1 PB3//on runnn
#define RUN_PIN2 PB4//on runnn

//uint8_t data_pb3_L[5] = {78, 64, 115, 0, 10}; // {N, @, s, rand, rand}
uint8_t data_pb3_H[5] = {78, 64, 115, 0, 30}; // {N, @, s, rand, rand}
//uint8_t data_pb4_L[5] = {78, 64, 115, 1, 11}; // {N, @, s, rand, rand}
uint8_t data_pb4_H[5] = {78, 64, 115, 1, 31}; // {N, @, s, rand, rand}
//int value_1 = 0;
//int value_1_1 = 0;
//int value_2 = 1;
int sec_20 = 1;

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

void setup(){
  cli();//disable interrupts during setup
 // pinMode(LED_PIN, OUTPUT);
  pinMode(PB1, OUTPUT);
  pinMode(RUN_INTERRUPT1, INPUT);
  pinMode(RUN_INTERRUPT2, INPUT);  
  
  //man.workAround1MhzTinyCore(); //add this in order for transmitter to work with 1Mhz Attiny85/84
  man.setupTransmit(TX_PIN, MAN_1200);  
  
  sbi(PCMSK, PCINT3); // Which pins are affected by the interrupt
  sbi(PCMSK, PCINT4); // Which pins are affected by the interrupt
  sbi(GIMSK, PCIE); // Turn on Pin Change interrupt  
}

void loop(){

  value_1 = analogRead(RUN_INTERRUPT1);
  value_2 = analogRead(RUN_INTERRUPT2);  
  
  --sec_20;

  if (value_1 > 0)  
  {
    sec_20 = 30000;
      
    if (value_1 <= 623) {
      digitalWrite(LED_PIN, HIGH);
      //delay(200);  // if is in ISR(PCINT0_vect), NOT DELAY
      digitalWrite(LED_PIN, LOW);
      
      man.transmitArray(5, data_pb3_L);
    }
    else {
      digitalWrite(PB1, HIGH);
      //delay(200); // if is in ISR(PCINT0_vect), NOT DELAY
      digitalWrite(PB1, LOW);      
      
      man.transmitArray(5, data_pb3_H);
    }
  }
      
  if (value_2 > 0)
  {
    sec_20 = 30000;
    
    if (value_2 < 623) {
      man.transmitArray(5, data_pb4_L);
    }
    else {
      man.transmitArray(5, data_pb4_H);
    }
  }  

  if (0 >= sec_20) {
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(PB1, HIGH);
    delay(400);
    digitalWrite(LED_PIN, LOW);
    digitalWrite(PB1, LOW);    
    
    system_sleep();
  }
}

// From http://interface.khm.de/index.php/lab/experiments/sleep_watchdog_battery/
void system_sleep() {
  cbi(ADCSRA,ADEN); // Switch Analog to Digital converter OFF
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Set sleep mode
  sleep_mode(); // System sleeps here
  sbi(ADCSRA,ADEN);  // Switch Analog to Digital converter ON
}

ISR(PCINT0_vect) {
}
