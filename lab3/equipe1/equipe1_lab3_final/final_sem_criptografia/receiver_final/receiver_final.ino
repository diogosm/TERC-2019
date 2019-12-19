#include <RH_RF95.h>

RH_RF95 rf95;
 
int ledpin = 7;

void setup() 
{
  pinMode(ledpin, OUTPUT);
  digitalWrite(ledpin, HIGH);
       
  Serial.begin(9600);
  while (!Serial) ; // Wait for serial port to be available
  if (!rf95.init())
    Serial.println("Falha na inicializacao");  

  rf95.setFrequency(915.0);
  rf95.setTxPower(20);
  rf95.setSignalBandwidth(500000);
  rf95.setSpreadingFactor(9);
  rf95.setCodingRate4(6);

  Serial.println("Esperando configuracao do Radio");
  delay(1000);
  Serial.println("Configuracao finalizada");
}

void loop()
{
  if (rf95.available())
  {
    // Should be a message for us now   
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    
    if (rf95.recv(buf, &len))
    {
      delay(500);
//      RH_RF95::printBuffer("request: ", buf, len);
      Serial.print("got request: ");
      Serial.println((char*)buf);
//      Serial.print("RSSI: ");
//      Serial.println(rf95.lastRssi(), DEC);

      String recepcao = (char *)&buf;
      Serial.println(recepcao);
      Serial.println(recepcao[4]);
      char estadoled = recepcao[4];

      if(estadoled=='0'){
        digitalWrite(ledpin, LOW);
      }else if(estadoled=='1'){
        digitalWrite(ledpin, HIGH);
      }

      
    }
    else{
      Serial.println("recv failed");
    }
  }
}
