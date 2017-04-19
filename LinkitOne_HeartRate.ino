#include <LGATTClient.h>

LGATTUUID UUID("E1E9D802-066A-11E7-93AE-92361F002671");
LGATTClient client;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(8000);

  Serial.println("Start BLE service...");
  client.begin(UUID);
  
}

void loop() {
    int numberOfDevices = client.scan(3); // Scan Device
    Serial.println(numberOfDevices);  
    LGATTDeviceInfo info = {0};
  
    for(int i = 0; i < numberOfDevices; ++i){     // Connect all devices
        client.getScanResult(i, info);

        int32_t rssi = 0;
        client.readRemoteRSSI(info.bd_addr, rssi);
        Serial.printf("[LGATTC ino] address : [%x:%x:%x:%x:%x:%x] rssi [%d]", 
                info.bd_addr.addr[5], info.bd_addr.addr[4], info.bd_addr.addr[3], info.bd_addr.addr[2], info.bd_addr.addr[1], info.bd_addr.addr[0],
                rssi);
        Serial.println();
        //if( rssi == 0 ) continue;
        
        if(client.connect(info.bd_addr)){
            Serial.println("Connected");
  
            heartrate_service();
            client.disconnect(info.bd_addr);
            Serial.println("Disconnect");   
            while(1); 
        }       
    }
}

void heartrate_service(){
      LGATTUUID heartrateServiceUUID = 0x180D;
      LGATTUUID heartrateCharUUID = 0x2A37;  
      LGATTUUID notificationUUID ;

      LGATTUUID batteryServiceUUID = 0x180F;
      LGATTUUID batteryCharUUID = 0x2A19;   


      client.getDescriptorInfo(heartrateServiceUUID, true, heartrateCharUUID, notificationUUID);
      Serial.println(notificationUUID);
  


      LGATT_CHAR_PROPERTIES pro;
      client.getCharacteristicInfo(heartrateServiceUUID, true, heartrateCharUUID, pro);
      Serial.print("h Properties : ");
      Serial.println(pro);

      client.getCharacteristicInfo(batteryServiceUUID, true, batteryCharUUID, pro);
      Serial.print("b Properties : ");
      Serial.println(pro);

      if( client.setupNotification(true, heartrateServiceUUID, true, heartrateCharUUID))
          Serial.println("Subscribed");
              
      LGATTAttributeValue attrValue ;

      client.readDescriptor(heartrateServiceUUID, true, heartrateCharUUID, notificationUUID, attrValue);
      Serial.print("Descriptor : ");
      for( int i = 0; i < attrValue.len; ++i)
          Serial.print(attrValue.value[i]);
      Serial.println();

      
      while( true )
      {
         if( client.readCharacteristic(batteryServiceUUID, true, batteryCharUUID, attrValue))
          {
              Serial.print("Battery : ");
              for( int i = 0; i < attrValue.len; ++i)
                Serial.print(attrValue.value[i]);
              Serial.println();
              delay(5000) ;
          }
          // queryNotification() is unusable
          if( client.queryNotification(heartrateServiceUUID, true, heartrateCharUUID, attrValue))
          {
              for( int i = 0; i < attrValue.len; ++i)
                Serial.print(attrValue.value[i]);
              Serial.println();
              delay(5000);
          }
          else
             client.setupNotification(true, heartrateServiceUUID, true, heartrateCharUUID);
      }

}

