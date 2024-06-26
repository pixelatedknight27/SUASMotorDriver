#include <SPI.h>
#include <RH_NRF24.h>
#include <L298N.h>
#include <String.h>

// Singleton instance of the radio driver
#define CE_PIN 7
#define CSN_PIN 8
RH_NRF24 nrf24(CE_PIN, CSN_PIN);

L298N myMotors[5] = {
    L298N(3, 29, 28),   // driver1
    L298N(2, 34, 35),   // driver2
    L298N(5, 30, 31),   // driver3
    L298N(11, 32, 33),  // driver4
    L298N(13, 24, 25)   // driver5
};

bool isRunning[] = {false, false, false, false, false};

void setup() {
    Serial.begin(9600);
    while (!Serial) ; // Wait for serial port to connect. Needed for Leonardo only

    for (L298N &motor : myMotors) {
        motor.stop();
    }

    if (!nrf24.init()) {
        Serial.println("init failed");
    } else {
        Serial.println("nice");
    }
    
    if (!nrf24.setChannel(1)) {
        Serial.println("setChannel failed");
    } else {
        Serial.println("okay");
    }
    
    if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm)) {
        Serial.println("setRF failed");
    } else {
        Serial.println("perfect");
    }
}

void getMotorAction(L298N &motor, int motorNum) {
    if (isRunning[motorNum]) {
        motor.stop();
        isRunning[motorNum] = false;
    } else {
        motor.forward();
        isRunning[motorNum] = true;
    }
}

void sendConfirmation(const char* confirmationMessage) {
    if (!nrf24.send((uint8_t*)confirmationMessage, strlen(confirmationMessage))) {
        Serial.println("Confirmation send failed");
    }
    nrf24.waitPacketSent();
}

void loop() {
    if (nrf24.available()) {
        uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);

        if (nrf24.recv(buf, &len)) {
            String msg = (char*)buf;
            Serial.print("Got request: ");
            Serial.println(msg);

            if (msg == "Button1") {
                getMotorAction(myMotors[0], 0);
                sendConfirmation("Button1_OK");
            } else if (msg == "Button2") {
                getMotorAction(myMotors[1], 1);
                sendConfirmation("Button2_OK");
            } else if (msg == "Button3") {
                getMotorAction(myMotors[2], 2);
                sendConfirmation("Button3_OK");
            } else if (msg == "Button4") {
                getMotorAction(myMotors[3], 3);
                sendConfirmation("Button4_OK");
            } else if (msg == "Button5") {
                getMotorAction(myMotors[4], 4);
                sendConfirmation("Button5_OK");
            } else {
                Serial.println("Invalid command");
            }
        } else {
            Serial.println("Receive failed");
        }
    }
}
