
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

byte names[] = {'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C'};  
int tones[] = {1915, 1700, 1519, 1432, 1275, 1136, 1014, 956};
//byte melody[] = "2d2a1f2c2d2a2d2c2f2d2a2c2d2a1f2c2d2a2a2g2p8p8p8p";
//byte melody[] = "2d2a2d2a2d2a2d2a2b2a2b2a2b2a2d2a2b2a2b2a2b2a2b2a";
int count = 0;
int count2 = 0;
//int count3 = 0;
int MAX_COUNT = 10;

#define BUFFER_SIZE 5
uint8_t buffer[BUFFER_SIZE];

#define  AFTER_RUN_BLINK_SEC  130
uint32_t after_run1_sec  = 10;
uint32_t after_run2_sec  = 10;

void setup() {
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);

  man.setupReceive(RX_PIN, MAN_1200);
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
            /*if (buffer[4] == 10)*/
            //moo = ++moo % 2;
            //digitalWrite(LED_PIN1, moo);
            //digitalWrite(LED_PIN1, HIGH);  
            
            play_run(LED_PIN1);
            
            after_run1_sec = AFTER_RUN_BLINK_SEC;
          }
          break;
        case 1:
          {
            /*if (buffer[4] == 10)*/
            //moo = ++moo % 2;
            //digitalWrite(LED_PIN2, moo);
            //digitalWrite(LED_PIN2, HIGH);  
            
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
      preview_run(LED_PIN1);
    }
  
    if (0 < after_run2_sec)
    {
      --after_run2_sec;
      preview_run(LED_PIN2);
    }
  }
}

void play_run(uint8_t RunPin)
{
  analogWrite(BUZZER_PIN, 0);     
  for (count = 0; count < MAX_COUNT; count++) {
      digitalWrite(RunPin, HIGH);
    //for (count3 = 0; count3 <= (melody[count*2] - 48) * 30; count3++) {
      for (count2=0;count2<8;count2++) {
        //if (names[count2] == melody[count*2 + 1]) {
          analogWrite(BUZZER_PIN,500);
          delayMicroseconds(tones[count2]);
          analogWrite(BUZZER_PIN, 0);
          delayMicroseconds(tones[count2]);
        //} 
      }
    //}
    digitalWrite(RunPin, LOW);    
  }
  
  digitalWrite(RunPin, LOW);
}

void preview_run(uint8_t RunPin)
{
  digitalWrite(RunPin, HIGH);
  delay(1600);
  digitalWrite(RunPin, LOW);
}
