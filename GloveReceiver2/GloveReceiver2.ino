#include <XBee.h>
#include <Servo.h>
#include <SoftwareSerial.h>



XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
// create reusable response objects for responses we expect to handle
ZBRxResponse rx = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();

SoftwareSerial xbeeSerial(2, 3);

int statusLed = 13;
int errorLed = 13;
int dataLed = 13;

Servo myservo;
int pos = 0;
int posMap;

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

void setup() {
  pinMode(statusLed, OUTPUT);
  pinMode(errorLed, OUTPUT);
  pinMode(dataLed,  OUTPUT);

  // start serial
  Serial.begin(9600);
  xbeeSerial.begin(9600);
  xbee.setSerial(xbeeSerial);
  Serial.println("Initializing Xbee Vehicle Mote...");

  flashLed(statusLed, 3, 50);

  myservo.attach(8);
}

// continuously reads packets, looking for ZB Receive or Modem Status
void loop() {

  xbee.readPacket();

  if (xbee.getResponse().isAvailable()) {
    // got something

    if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
      // got a zb rx packet

      // now fill our zb rx class
      xbee.getResponse().getZBRxResponse(rx);

      if (rx.getOption() == ZB_PACKET_ACKNOWLEDGED) {
        // the sender got an ACK
        flashLed(statusLed, 10, 10);
      } else {
        // we got it (obviously) but sender didn't get an ACK
        flashLed(errorLed, 2, 50);
      }
      // set dataLed PWM to value of the first byte in the data
      int servoVal = rx.getData(1) + (rx.getData(0) << 8);
      Serial.println(servoVal);
      myservo.write(map(servoVal, 0, 100, 0, 180));
//      myservo.write(map(pos, 0, 100, 0, 180));
    } else if (xbee.getResponse().getApiId() == MODEM_STATUS_RESPONSE) {
      xbee.getResponse().getModemStatusResponse(msr);
      // the local XBee sends this response on certain events, like association/dissociation

      if (msr.getStatus() == ASSOCIATED) {
        // yay this is great.  flash led
        flashLed(statusLed, 10, 10);
      } else if (msr.getStatus() == DISASSOCIATED) {
        // this is awful.. flash led to show our discontent
        flashLed(errorLed, 10, 10);
      } else {
        // another status
        flashLed(statusLed, 5, 10);
      }
    } else {
      // not something we were expecting
      flashLed(errorLed, 1, 25);
    }
  } else if (xbee.getResponse().isError()) {
    //nss.print("Error reading packet.  Error code: ");
    //nss.println(xbee.getResponse().getErrorCode());
  }
}
