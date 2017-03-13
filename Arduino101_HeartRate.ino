#include <CurieBLE.h>

BLEService heartRateService("180D"); // BLE Heart Rate Service
BLEService batteryService("180F"); // BLE Battery Service

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);   // indicate when a central is connected
  delay(10000);

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
      
      Serial.println("Disconnect");
      digitalWrite(13, LOW);   // Disconnect signal
    }

    delay(3000);
}
int timer = 0 ;
void HeartRate(BLEDevice peripheral) {

  // discover peripheral attributes
  Serial.println("Discovering attributes of service 0x180D ...");
  if (peripheral.discoverAttributesByService("180D")) {
      Serial.println("Attributes discovered");
      if(!peripheral.discoverAttributesByService("180F"))
          Serial.println("Battery Attributes not discoverd");
  } 
  else {
    Serial.println("Attribute discovery failed.");
    peripheral.disconnect();
    return;
  }

  BLECharacteristic heartrate = peripheral.characteristic("2A37");  // UUID of heartrate service is 2A37
  BLECharacteristic battery   = peripheral.characteristic("2A19");  // UUID of battery service is 2A19

  if( !heartrate || !battery )   // Service not exist
  {
      Serial.println("No HeartRate or Battery Service...");  
      return ;
  }

  if( !heartrate.subscribe() || !battery.subscribe() )
  {
      Serial.println("Cannot Subscribe HeartRate or Battery Service...");
      return ;
  }

  while( peripheral.connected() ){

      timer ++ ;
      if(  battery.valueUpdated() )
      {
          timer = 0;
          Serial.print("Battery : ");
          printData(battery.value(), battery.valueLength());
          Serial.println(" %");
      }
      if( heartrate.valueUpdated() )
      {
          Serial.print("Heartrate : ");
          Serial.print((int)((char*)heartrate.value())[1]);
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
    Serial.print(' ');
  }
}
