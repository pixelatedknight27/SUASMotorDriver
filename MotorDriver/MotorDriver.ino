#include <SPI.h>
#include <RH_NRF24.h>
#include <L298N.h>

// Singleton instance of the radio driver
#define CE_PIN 7
#define CSN_PIN 8

RH_NRF24 nrf24(CE_PIN, CSN_PIN);

L298N myMotors[5] =
{
  L298N(5, 30, 31),   // driver1
  L298N(3, 28, 29),   // driver2
  L298N(9, 34, 35),   // driver3
  L298N(11, 32, 33),  // driver4
  L298N(13, 24, 25)   // driver5
};

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
