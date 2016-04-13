#include <Arduino.h>


//Our LEDS
static const int RED_LED_PIN = 9;
static const int BLUE_LED_PIN = 10;
static const int YELLOW_LED_PIN = 11;
static const int GREEN_LED_PIN = 6;

//Our Buttons
static const int RED_BUTTON_PIN = 7;
static const int BLUE_BUTTON_PIN = 8;
static const int YELLOW_BUTTON_PIN = 12;
static const int GREEN_BUTTON_PIN = 13;

static const int SPEAKER_PIN = 3;

static const int NOTE_G4 = 392; //Blue
static const int NOTE_C4 = 277; //Yellow
static const int NOTE_A4 = 440; //Red
static const int NOTE_G5 = 784; //Green
void setup() {

  //Setup LED pins as OUTPUT
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);

  //Setup buttons pins as INPUT_PULLUP
  pinMode(RED_BUTTON_PIN, INPUT_PULLUP);
  pinMode(BLUE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(YELLOW_BUTTON_PIN, INPUT_PULLUP);
  pinMode(GREEN_BUTTON_PIN, INPUT_PULLUP);

  Serial.begin(9600);  //turn on serial communication
}

// the loop function runs over and over again forever
void loop() {
  // Serial.println("And ON");
  // digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  // delay(5000);              // wait for a second
  // Serial.println("And OFF");
  // digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  // delay(1000);              // wait for a second

  digitalWrite(RED_LED_PIN, !digitalRead(RED_BUTTON_PIN));
  digitalWrite(BLUE_LED_PIN, !digitalRead(BLUE_BUTTON_PIN));
  digitalWrite(YELLOW_LED_PIN, !digitalRead(YELLOW_BUTTON_PIN));
  digitalWrite(GREEN_LED_PIN, !digitalRead(GREEN_BUTTON_PIN));

  if(!digitalRead(RED_BUTTON_PIN))
  {
      tone(SPEAKER_PIN, NOTE_A4, 100);
  }
  else if(!digitalRead(BLUE_BUTTON_PIN))
  {
      tone(SPEAKER_PIN, NOTE_G4, 100);
  }
  else if(!digitalRead(YELLOW_BUTTON_PIN))
  {
      tone(SPEAKER_PIN, NOTE_C4, 100);
  }
  else if(!digitalRead(GREEN_BUTTON_PIN))
  {
      tone(SPEAKER_PIN, NOTE_G5, 100);
  }

}
