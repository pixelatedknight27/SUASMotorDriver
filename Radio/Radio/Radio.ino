#include <RadioHead.h>

// Transmitter 
#include <SPI.h>
#include <nRF24L01.h>
#include <RH_NRF24.h>
 
// Singleton instance of the radio driver
RH_NRF24 nrf24;
// RH_NRF24 nrf24(8, 7); // use this to be electrically compatible with Mirf
// RH_NRF24 nrf24(8, 10);// For Leonardo, need explicit SS pin
// RH_NRF24 nrf24(8, 7); // For RFM73 on Anarduino Mini

#define buttonPin1 6
#define buttonPin2 5
#define buttonPin3 4
#define buttonPin4 3
#define buttonPin5 2

int buttonState1;
int buttonState2;
int buttonState3;
int buttonState4;
int buttonState5;


 
void setup() 
{

  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  pinMode(buttonPin4, INPUT_PULLUP);
  pinMode(buttonPin5, INPUT_PULLUP);

 Serial.begin(9600);
  while (!Serial) ; // Wait for serial port to connect. Needed for Leonardo only

  if (!nrf24.init()) {
    Serial.println("init failed");
  }
  
  if (!nrf24.setChannel(1)) {
    Serial.println("setChannel failed");
  }
  
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm)) {
    Serial.println("setRF failed");
  }
}

void loop() {
  checkButtonPress(buttonPin1, "Button1", &buttonState1);
  checkButtonPress(buttonPin2, "Button2", &buttonState2);
  checkButtonPress(buttonPin3, "Button3", &buttonState3);
  checkButtonPress(buttonPin4, "Button4", &buttonState4);
  checkButtonPress(buttonPin5, "Button5", &buttonState5);

  delay(50); // Debounce delay
}

void checkButtonPress(int buttonPin, const char* buttonName, int* lastState) {
  int currentState = digitalRead(buttonPin);
  if (currentState == LOW && *lastState != LOW) {
    Serial.print(buttonName);
    Serial.println(" Pressed");
    uint8_t data[20];
    strcpy((char*)data, buttonName);
    nrf24.send(data, strlen((char*)data));
    nrf24.waitPacketSent();

    // Now wait for the confirmation
    uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    unsigned long startTime = millis();
    while (millis() - startTime < 1000) {  // wait for up to 1000 ms for the confirmation
      if (nrf24.available()) {
        if (nrf24.recv(buf, &len)) {
          String confirmation = (char*)buf;
          if (confirmation == String(buttonName) + "_OK") {
            Serial.println("Confirmation received");
            break;
          }
        }
      }
    }
  }
  *lastState = currentState;
}