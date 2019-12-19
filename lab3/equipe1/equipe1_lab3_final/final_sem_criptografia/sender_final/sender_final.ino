#include <RH_RF95.h>

RH_RF95 rf95;

void setup() {

  Serial.begin(9600);
  while (!Serial);
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

void loop() {
  
  float valorsensor = analogRead(0);
  String aux = "V-" + String(valorsensor,2);
  char copy[10];
  aux.toCharArray(copy, aux.length()+1);

  uint8_t data[aux.length()+1] = {0};
  for(uint8_t i = 0; i<= aux.length(); i++) data[i] = (uint8_t)copy[i];
  rf95.send(data, sizeof(data)); 
  Serial.print("Sent: ");
  Serial.println((char *)&data);
        
  rf95.waitPacketSent(); 
  
  rf95.setModeIdle();
  delay(2000);
  
}
