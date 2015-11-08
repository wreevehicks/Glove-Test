
#include <XBee.h>
#include <SoftwareSerial.h>

//create XBee object
XBee xbee = XBee();

uint8_t payload[] = { 0, 0 };

// SH + SL Address of receiving XBee
XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x4079b2e6);
ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
ZBTxStatusResponse txStatus = ZBTxStatusResponse();

SoftwareSerial xbeeSerial(2, 3);

int pin5 = 0;

int statusLed = 13;
int errorLed = 13;

int LEDPin_CalibrationPin = 12;

void flashLed(int pin, int times, int wait) {

  for (int i = 0; i < times; i++) {
    digitalWrite(pin, HIGH);
    delay(wait);
    digitalWrite(pin, LOW);

    if (i + 1 < times) {
      delay(wait);
    }
  }
}

int flexSensorPin0 = A0, flexSensorPin1 = A1, flexSensorPin2 = A2, flexSensorPin3 = A3;
int base0 = 0, base1 = 0, base2 = 0, base3 = 0, upper0 = 0, upper1 = 0, upper2 = 0, upper3 = 0;
int baseRead1 = 0, upperRead1 = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(statusLed, OUTPUT);
  pinMode(errorLed, OUTPUT);
  pinMode(LEDPin_CalibrationPin, OUTPUT);
  digitalWrite(LEDPin_CalibrationPin, LOW);

  // start serial
  Serial.begin(9600);
  xbeeSerial.begin(9600);
  xbee.setSerial(xbeeSerial);
  Serial.println("Initializing Xbee Vehicle Mote...");

  //Calibration
  // Step 1 Flat Fingers
  digitalWrite(LEDPin_CalibrationPin, HIGH);
  delay(100);
  digitalWrite(LEDPin_CalibrationPin, LOW);
  delay(2000);
  for (int j = 0; j < 5; j++) {
    baseRead1 = analogRead(flexSensorPin1);
    base1 += baseRead1;
    delay(500);
  }
  base1 = base1 / 5;
  delay(500);

  //Step 2 Closed Fingers
  for (int j = 0; j < 2; j++) {
    digitalWrite(LEDPin_CalibrationPin, HIGH);
    delay(100);
    digitalWrite(LEDPin_CalibrationPin, LOW);
    delay(100);
  }
  delay(2000);
  for (int j = 0; j < 5; j++) {
    upperRead1 = analogRead(flexSensorPin1);
    upper1 += upperRead1;
    delay(200);
  }
  upper1 = upper1 / 5;


  //Calibration Complete

}

void loop() {

  Serial.println(base1);
  int flexSensor1 = analogRead(flexSensorPin1);
  int flex1 = map(flexSensor1, base1, upper1, 0, 100);
  Serial.print("Flex Reading: ");
  Serial.println(flexSensor1);
  Serial.print("Mapped Flex: ");
  Serial.print(flex1);

  // break down 10-bit reading into two bytes and place in payload
  payload[0] = flex1 >> 8 & 0xff;
  payload[1] = flex1 & 0xff;

  xbee.send(zbTx);

  // flash TX indicator
  flashLed(statusLed, 1, 10);

  // after sending a tx request, we expect a status response
  // wait up to half second for the status response
  if (xbee.readPacket(100)) {
    // got a response!

    // should be a znet tx status
    if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
      xbee.getResponse().getZBTxStatusResponse(txStatus);

      // get the delivery status, the fifth byte
      if (txStatus.getDeliveryStatus() == SUCCESS) {
        // success.  time to celebrate
        flashLed(statusLed, 5, 10);
      } else {
        // the remote XBee did not receive our packet. is it powered on?
        flashLed(errorLed, 3, 10);
      }
    }
  } else if (xbee.getResponse().isError()) {
    //nss.print("Error reading packet.  Error code: ");
    //nss.println(xbee.getResponse().getErrorCode());
  } else {
    // local XBee did not provide a timely TX Status Response -- should not happen
    flashLed(errorLed, 2, 10);
  }

  delay(500);


}

