#include <CurieBLE.h>

BLEService heartRateService("180D"); // BLE Heart Rate Service
BLEService batteryService("180F"); // BLE Battery Service

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


      // Try to connect 
      if (peripheral.connect()) {
        Serial.println("Connected");
        digitalWrite(13, HIGH);      // Once connected, make builtin LED lighten
        HeartRate(peripheral);
      } else {
        Serial.println("Failed to connect!");
      }
   
      digitalWrite(13, LOW);   // Disconnect signal
    }

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

  BLECharacteristic heartrate = peripheral.characteristic("2A37");  // UUID of heartrate service is 2A37
  BLECharacteristic battery   = peripheral.characteristic("2A19");  // UUID of battery service is 2A19

  if( !heartrate || !battery ) return ;   // Service not exist

  if( !heartrate.subscribe() || !battery.subscribe() ) return ;   // Cannot subscribe

  while( peripheral.connected() ){

    if( battery.valueUpdated() )
    {
      Serial.print("Battery : ");
      printData(battery.value(), battery.valueLength());
      Serial.println(" %");
    }
    if( heartrate.valueUpdated() )
    {
      Serial.print("Heartrate : ");
      printData(heartrate.value(), heartrate.valueLength());
      Serial.println(" bpm");
    }
    
  }
}

void printData(const unsigned char data[], int length) {
  for (int i = 0; i < length; i++) {
    unsigned char b = data[i];

    if (b < 16) {
      Serial.print("0");
    }

    Serial.print(b);
  }
}

