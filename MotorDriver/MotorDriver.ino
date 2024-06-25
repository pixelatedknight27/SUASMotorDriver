#include <SPI.h>
#include <RH_NRF24.h>
#include <L298N.h>

// Singleton instance of the radio driver
#define CE_PIN 7
#define CSN_PIN 8

RH_NRF24 nrf24(CE_PIN, CSN_PIN);

L298N myMotors[5] = {
  L298N(5, 30, 31),   // driver1
  L298N(3, 28, 29),   // driver2
  L298N(9, 34, 35),   // driver3
  L298N(11, 32, 33),  // driver4
  L298N(13, 24, 25)   // driver5
};

void setup() {
  // put your setup code here, to run once:
  for(L298N driver : myMotors){
    driver.stop();
  }

  Serial.begin(9600);
  while (!Serial) ; // Wait for serial port to connect. Needed for Leonardo only
  
  if(!nrf24.init()){
    Serial.println("init failed");
  } 
  else{
    Serial.println("nice");
  }
    
  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!nrf24.setChannel(1)){
    Serial.println("setChannel failed");
  } 
  else{
    Serial.println("okay");
  }
    
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm)) { 
    Serial.println("setRF failed");
  }
  else{ 
    Serial.println("perfect");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (nrf24.available())
  {
    // Should be a message for us now
    uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    Serial.println("we are live");

    if (nrf24.recv(buf, &len)){

      String msg = (char*)buf;

      Serial.print("Got request: " + msg);
      Serial.println((char*)buf);

      switch((int)msg[6]){
        case 1:
          myMotors[0].forward();
          break;
        case 2:
          myMotors[1].forward();
          break;
        case 3:
          myMotors[2].forward();
          break;
        case 4:
          myMotors[3].forward();
          break;
        case 5:
          myMotors[4].forward();
          break;
        default:
          Serial.println("Invalid command");
          break;
      }
    }
    else
    {
      Serial.println("recv failed");
    }
  }
}
