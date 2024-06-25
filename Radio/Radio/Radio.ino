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

  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);
  buttonState3 = digitalRead(buttonPin3);
  buttonState4 = digitalRead(buttonPin4);
  buttonState5 = digitalRead(buttonPin5);
   
  Serial.begin(9600);
  Serial.println("Starting");
  while (!Serial) 
    ; // wait for serial port to connect. Needed for Leonardo only
  if (!nrf24.init())
    Serial.println("init failed");
  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!nrf24.setChannel(1))
    Serial.println("setChannel failed");
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    Serial.println("setRF failed");
}
 
 
 void loop() {
  // Read the state of the button
  int currentButtonState1 = digitalRead(buttonPin1);
  int currentButtonState2 = digitalRead(buttonPin2);
  int currentButtonState3 = digitalRead(buttonPin3);
  int currentButtonState4 = digitalRead(buttonPin4);
  int currentButtonState5 = digitalRead(buttonPin5);

  // Check if each button is pressed (assuming they are active low)
  if (currentButtonState1 == LOW && buttonState1 != LOW) {
    Serial.println("Button 1 Pressed");
    uint8_t data[] = "Button1";
    nrf24.send(data, sizeof(data));
  }
  if (currentButtonState2 == LOW && buttonState2 != LOW) {
    Serial.println("Button 2 Pressed");
    uint8_t data[] = "Button2";
    nrf24.send(data, sizeof(data));
  }
  if (currentButtonState3 == LOW && buttonState3 != LOW) {
    Serial.println("Button 3 Pressed");
    uint8_t data[] = "Button3";
    nrf24.send(data, sizeof(data));
  }
  if (currentButtonState4 == LOW && buttonState4 != LOW) {
    Serial.println("Button 4 Pressed");
    uint8_t data[] = "Button4";
    nrf24.send(data, sizeof(data));
  }
  if (currentButtonState5 == LOW && buttonState5 != LOW) {
    Serial.println("Button 5 Pressed");
    uint8_t data[] = "Button5";
    nrf24.send(data, sizeof(data));
  }
  
  // Update button states for the next iteration
  buttonState1 = currentButtonState1;
  buttonState2 = currentButtonState2;
  buttonState3 = currentButtonState3;
  buttonState4 = currentButtonState4;
  buttonState5 = currentButtonState5;

  // Delay to debounce the buttons
  delay(50);
}