#include <CurieBLE.h>

BLEPeripheral blePeripheral;       // BLE Peripheral Device (the board you're programming)
BLEService heartRateService("180D"); // BLE Heart Rate Service
BLEService batteryRateService("180F"); // BLE battery level Service

// BLE Heart Rate Measurement Characteristic"
BLECharacteristic heartRateChar("2A37",  // standard 16-bit characteristic UUID
    BLERead | BLENotify, 2);  // remote clients will be able to get notifications if this characteristic changes
BLECharacteristic heartRateChar("2A19",  // standard 16-bit characteristic UUID
    BLERead | BLENotify, 2);  // detect battery level


void setup() {
  Serial.begin(9600);    
  pinMode(13, OUTPUT);   // indicate when a central is connected


  
  blePeripheral.begin();
}

// the loop function runs over and over again forever
void loop() {

  digitalWrite(13, LOW);

  
}
