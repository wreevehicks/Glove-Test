#include <QueueArray.h>
#include <XBee.h>

#include <Xbee.h>
#include "XbeeApiStream.h"

//const int button_pin = 5;

//unsigned long debounce_timestamp = 0, debounce_delay = 50;
//int button_state = LOW, last_button_state = LOW;
//bool state_paused = true;

int flexSensorPin0 = A0, flexSensorPin1 = A1, flexSensorPin2 = A2, flexSensorPin3 = A3;
int base0 = 0, base1 = 0, base2 = 0, base3 = 0, upper0 = 0, upper1 = 0, upper2 = 0, upper3 = 0;
int state = 0;

int LEDpin = 8;

XbeeApiStream xbeeStream = XbeeApiStream();

void setup() {
  Serial.begin(9600);
  xbeeStream.begin(Serial);
  Serial.print("begin");
  //  pinMode(button_pin, INPUT);
  //  digitalWrite(button_pin, HIGH);
  //  pinMode(LED_BUILTIN, OUTPUT);
  //  digitalWrite(LED_BUILTIN, !state_paused);
  pinMode(LEDpin, OUTPUT);
  digitalWrite(LEDpin, HIGH);
}

void loop() {
  if (base0 == 0) {
//    Serial.println("Calibration: Hold your hand flat for three seconds.");
    delay(2500);
    base0 = analogRead(flexSensorPin0);
//    Serial.print(base0);
    base1 = analogRead(flexSensorPin1);
    base2 = analogRead(flexSensorPin2);
    base3 = analogRead(flexSensorPin3);
    delay(500);
//    Serial.println("Close your hand fully for three seconds");
    delay(2500);
    upper0 = analogRead(flexSensorPin0);
    upper1 = analogRead(flexSensorPin1);
    upper2 = analogRead(flexSensorPin2);
    upper3 = analogRead(flexSensorPin3);
    delay(500);
//        Serial.println("Calibration complete!");
  } else {
    int flexSensor0 = analogRead(flexSensorPin0);
    delay(10);
    int flexSensor1 = analogRead(flexSensorPin1);
    delay(10);
    int flexSensor2 = analogRead(flexSensorPin2);
    delay(10);
    int flexSensor3 = analogRead(flexSensorPin3);
    delay(10);

    int flex0 = map(flexSensor0, base0, upper0, 0, 100);
    int flex1 = map(flexSensor1, base1, upper1, 0, 100);
    int flex2 = map(flexSensor2, base2, upper2, 0, 100);
    int flex3 = map(flexSensor3, base3, upper3, 0, 100);

//    Serial.print(flex0);
//    Serial.print('\t');
//    Serial.print(flex1);
//    Serial.print('\t');
//    Serial.print(flex2);
//    Serial.print('\t');
//    Serial.println(flex3);

    int reading = flex1;
    if (reading > 50 && state == 1) {
      state = 0;
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      xbeeStream.write((uint8_t)0x00);
      digitalWrite(LEDpin, HIGH);
    } else if (reading < 50 && state == 0) {
      state = 1;
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      xbeeStream.write((uint8_t)0x00);
      digitalWrite(LEDpin, LOW);
    }
    delay(10);


    //    int reading = digitalRead(button_pin);
    //    if (reading != last_button_state)	debounce_timestamp = millis();
    //    if (millis() - debounce_timestamp > debounce_delay) {
    //      if (reading != button_state) {
    //        button_state = reading;
    //        if (button_state == LOW) {
    //          digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    //          xbeeStream.write((uint8_t)0x00);
  }
}
//  last_button_state = reading;


