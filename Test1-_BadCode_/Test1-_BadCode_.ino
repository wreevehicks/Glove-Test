int flexSensorPin0 = A0, flexSensorPin1 = A1, flexSensorPin2 = A2, flexSensorPin3 = A3; 

int base0 = 0, base1 = 0, base2 = 0, base3 = 0, upper0 = 0, upper1 = 0, upper2 = 0, upper3 = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (base0 == 0) {
    Serial.println("Calibration: Hold your hand flat for three seconds.");
    delay(1500);
    base0 = analogRead(flexSensorPin0);
    base1 = analogRead(flexSensorPin1);
    base2 = analogRead(flexSensorPin2);
    base3 = analogRead(flexSensorPin3);
    delay(1500);
    Serial.println("Close your hand fully for three seconds");
    delay(1500);
    upper0 = analogRead(flexSensorPin0);
    upper1 = analogRead(flexSensorPin1);
    upper2 = analogRead(flexSensorPin2);
    upper3 = analogRead(flexSensorPin3);
    delay(1500);
    Serial.println("Calibration complete!");
  } else {
    int flexSensor0 = analogRead(flexSensorPin0);
    delay(10);
    int flexSensor1 = analogRead(flexSensorPin1);
    delay(10);
    int flexSensor2 = analogRead(flexSensorPin2);
    delay(10);
    int flexSensor3 = analogRead(flexSensorPin3);
    delay(10);

    int flex0_0to100 = map(flexSensor0, base0, upper0, 0, 100);
    int flex1_0to100 = map(flexSensor1, base1, upper1, 0, 100);
    int flex2_0to100 = map(flexSensor2, base2, upper2, 0, 100);
    int flex3_0to100 = map(flexSensor3, base3, upper3, 0, 100);

    Serial.print(flex0_0to100);
    Serial.print('\t');
    Serial.print(flex1_0to100);
    Serial.print('\t');
    Serial.print(flex2_0to100);
    Serial.print('\t');
    Serial.println(flex3_0to100);


    //In my tests I was getting a reading on the arduino between 512, and 614.
    //Using map(), you can convert that to a larger range like 0-100.
    //  int flex0to100 = map(flexSensorReading, 512, 614, 0, 100);
    //  Serial.println(flex0to100);

    delay(500); //just here to slow down the output for easier reading
  }
}
