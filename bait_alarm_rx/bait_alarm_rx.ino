
#include <Manchester.h>

/*

  Manchester Receiver example
  
  In this example receiver will receive array of 10 bytes per transmittion

  try different speeds using this constants, your maximum possible speed will 
  depend on various factors like transmitter type, distance, microcontroller speed, ...

  MAN_300 0
  MAN_600 1
  MAN_1200 2
  MAN_2400 3
  MAN_4800 4
  MAN_9600 5
  MAN_19200 6
  MAN_38400 7

*/

#define RX_PIN PB1
#define LED_PIN1 PB3
#define LED_PIN2 PB4
#define BUZZER_PIN  PB0

#define BUFFER_SIZE 5
uint8_t buffer[BUFFER_SIZE];

#define  AFTER_RUN_BLINK_SEC  4700
uint32_t after_run1_sec  = 0;
uint32_t after_run2_sec  = 0;

uint8_t count;

void setup() {
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  
  digitalWrite(LED_PIN1, HIGH);
  digitalWrite(LED_PIN2, HIGH);
  analogWrite(BUZZER_PIN, 500);
  delay(100);
  analogWrite(BUZZER_PIN, 0);
  delay(50);
  analogWrite(BUZZER_PIN, 500);  
  delay(50);
  analogWrite(BUZZER_PIN, 0);
  delay(300);
  digitalWrite(LED_PIN1, LOW);
  digitalWrite(LED_PIN2, LOW);

  man.setupReceive(RX_PIN, MAN_600);
  man.beginReceiveArray(BUFFER_SIZE, buffer);
}

void loop() {
  if (man.receiveComplete()) {
    
    // standart first symbols
    if (buffer[0] == 78 && 
        buffer[1] == 64 &&
        buffer[2] ==  115)
    {
      switch(buffer[3])
      {
        case 0:
          {
            play_run(LED_PIN1);
            
            after_run1_sec = AFTER_RUN_BLINK_SEC;
          }
          break;
        case 1:
          {
            play_run(LED_PIN2);
            
            after_run2_sec = AFTER_RUN_BLINK_SEC;
          }
          break;          
      }// switch
    }
    
    man.beginReceiveArray(BUFFER_SIZE, buffer);
  }
  else
  {
    if (0 < after_run1_sec)
    {
      --after_run1_sec;

      if (1 == after_run1_sec) {
        digitalWrite(LED_PIN1, LOW);
      }
    }
  
    if (0 < after_run2_sec)
    {
      --after_run2_sec;

      if (1 == after_run2_sec) {
        digitalWrite(LED_PIN2, LOW);
      }      
    }    
  }
  
}

void play_run(uint8_t RunPin)
{
  digitalWrite(RunPin, HIGH);

  for (count = 0; count < 100; count++) {
    analogWrite(BUZZER_PIN, 500);  
    delay(9);
    analogWrite(BUZZER_PIN, 0);        
    delay(10);
    analogWrite(BUZZER_PIN, 20);  
    delay(2);
    analogWrite(BUZZER_PIN, 0);    
  }
}
