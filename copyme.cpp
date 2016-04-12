#include <Arduino.h>
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);

  Serial.begin(9600);  //turn on serial communication

}

// the loop function runs over and over again forever
void loop() {
  Serial.println("And ON");
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(5000);              // wait for a second
  Serial.println("And OFF");
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second
}
