/*
  This code is to read the IMU data through BLE
*/

#include <ArduinoBLE.h>
#include <Arduino_LSM6DS3.h>

const int ledPin = LED_BUILTIN; // set ledPin to on-board LED
const int buttonPin = 4; // set buttonPin to digital pin 4

BLEService imuService("19B10010-E8F2-537E-4F6C-D104768A1214"); // create service

// create button characteristic and allow remote device to get notifications
BLEByteCharacteristic imuCharacteristic("19B10012-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);

void setup() {
  //Initialize the IMU
  if (!IMU.begin())
  {
    Serial.println("Failed to initialize IMU!");
    exit(1);
  }
  Serial.println("Initialized the IMU");

  //Initialize the Bluetooth
  Serial.begin(9600);
  while (!Serial);

  //pinMode(ledPin, OUTPUT); // use the LED as an output
  //pinMode(buttonPin, INPUT); // use button pin as an input

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  // set the local name peripheral advertises
  BLE.setLocalName("smoon");
  // set the UUID for the service this peripheral advertises:
  BLE.setAdvertisedService(imuService);
  
  // add the characteristics to the service
  imuService.addCharacteristic(imuCharacteristic);
  

  // add the service
  BLE.addService(imuService);

  imuCharacteristic.writeValue(0);
  // buttonCharacteristic.writeValue(0);

  // start advertising
  BLE.advertise();

  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  // poll for BLE events
  BLE.poll();
float x, y, z, delta = 10;

  if (IMU.gyroscopeAvailable())
  {
    IMU.readGyroscope(x, y, z);

    if(y < -delta)
    {
      Serial.println("Flicked down");
      imuCharacteristic.writeValue(y);
    }
    else if(y > delta)
    {
      Serial.println("Flicked up");
      imuCharacteristic.writeValue(y);
    }
  }
  delay(50);


  
}
