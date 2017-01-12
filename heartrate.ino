#include <CurieBLE.h>

BLEPeripheral blePeripheral;       // BLE Peripheral Device (the board you're programming)
BLEService heartRateService("180D"); // BLE Heart Rate Service
BLEService batteryService("180F"); // BLE Battery Service

// BLE Heart Rate Measurement Characteristic"
BLECharacteristic heartRateChar("2A37",  // standard 16-bit characteristic UUID
    BLERead | BLENotify, 2);  // remote clients will be able to get notifications if this characteristic changes
BLECharacteristic batteryLevelChar("2A19",  // standard 16-bit characteristic UUID
    BLERead | BLENotify, 2 );  // detect battery level

const char localName[30] = "HeartRate_Arduino101" ;
int i = 0 ;

void setup() {
  Serial.begin(9600);
  while(!Serial.available()) ;
  pinMode(13, OUTPUT);   // indicate when a central is connected

  BLE.begin();
  
  Serial.println("BLE Central begin ...");
  Serial.println("Make sure to turn on the device.");
  
  BLE.scan();
}

// the loop function runs over and over again forever
void loop() {
  BLEDevice peripheral = BLE.available();
   if (peripheral) {
      // discovered a peripheral
      Serial.println("Discovered a peripheral");
      Serial.println("-----------------------");

      // print address
      Serial.print("Address: ");
      Serial.println(peripheral.address());

      if (peripheral.hasLocalName()) {
        Serial.print("Local Name: ");
        Serial.println(peripheral.localName());
      }
      if (peripheral.hasAdvertisedServiceUuid()) {
        Serial.print("Service UUID's: ");
        for (int i = 0; i < peripheral.advertisedServiceUuidCount(); i++) {
          Serial.print(peripheral.advertisedServiceUuid(i));
          Serial.print(" ");
        }
        Serial.println();
      }


      // try to connect 
      if (peripheral.connect()) {
        Serial.println("Connected");
        HeartRate(peripheral);
      } else {
        Serial.println("Failed to connect!");
      }
   
      
    }
    if(i) digitalWrite(13, LOW);
    else digitalWrite(13, HIGH);
    i = 1-i;
    delay(3000);
}

void HeartRate(BLEDevice peripheral) {

  Serial.println("Discovering attributes ...");     // Get attributes
  if (peripheral.discoverAttributes()) {
    Serial.println("Attributes discovered");
  } else {
    Serial.println("Attribute discovery failed!");
    peripheral.disconnect();
    return;
  }

  delay(2000);
}



