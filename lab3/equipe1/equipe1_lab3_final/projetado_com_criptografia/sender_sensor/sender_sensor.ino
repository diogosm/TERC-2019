#include <RH_RF95.h>
#include <RHEncryptedDriver.h>
#include <Speck.h>


RH_RF95 rf95;     // Instanciate a LoRa driver
Speck myCipher;   // Instanciate a Speck block ciphering
RHEncryptedDriver myDriver(rf95, myCipher); // Instantiate the driver with those two

unsigned char encryptkey[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}; // The very secret key


int iniciando = 0;
int esperando = 1;
int enviando = 2;
int estado = iniciando;


int contadorespera = 1;

void setup()
{  
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600); 
  
  while (!Serial);
  Serial.println("CLiente criptografado LORA");
  if (!rf95.init())
    Serial.println("Falha na inicializacao");
  rf95.setFrequency(915.0);
  rf95.setTxPower(20);
  
  rf95.setSignalBandwidth(500000);
  rf95.setSpreadingFactor(9); //7 - 12
  rf95.setCodingRate4(6);  

  
  myCipher.setKey(encryptkey, sizeof(encryptkey));
  
  Serial.println("Esperando configuracao do Radio");
  delay(1000);
  Serial.println("Configuracao finalizada");
}

void loop(){

  if(estado == iniciando){
  
    //Serial.println("INICIANDO-1");
    String aux = "ON";
    char copy[10];
    aux.toCharArray(copy, aux.length()+1);
    
    uint8_t data[aux.length()+1] = {0};
    
    for(uint8_t i = 0; i<= aux.length(); i++) data[i] = (uint8_t)copy[i];
    myDriver.send(data, sizeof(data)); 
    Serial.print("Sent: ");
    Serial.println((char *)&data);
    
    myDriver.waitPacketSent();

    estado = esperando;
    //Serial.println("INICIANDO-2");
       
  }else if(estado == esperando){
    //Serial.println("ESPERANDO-1");
    
    if (myDriver.available()) {
      // Should be a message for us now   
      uint8_t buf[myDriver.maxMessageLength()];
      uint8_t len = sizeof(buf);
      
      if (myDriver.recv(buf, &len)) {
        delay(1000);
        Serial.print("Received: ");
        Serial.println((char *)&buf);
        estado = enviando;
        
      }else{
          Serial.println("recv failed");
          estado = iniciando;
      }
      
    }
    
    //Serial.println("ESPERANDO-2");
    
  }else if(estado == enviando){
        //Serial.println("ENVIANDO-1");  
        delay(500);    
        digitalWrite(LED_BUILTIN, HIGH);

        float valorsensor = analogRead(0);
        String aux = "V-" + String(valorsensor,2);
        char copy[10];
        aux.toCharArray(copy, aux.length()+1);
        
        uint8_t data[aux.length()+1] = {0};
        
        for(uint8_t i = 0; i<= aux.length(); i++) data[i] = (uint8_t)copy[i];
        myDriver.send(data, sizeof(data)); 
        Serial.print("Sent: ");
        Serial.println((char *)&data);
        
        myDriver.waitPacketSent();        

        

        rf95.setModeIdle();
        delay(2000);
        digitalWrite(LED_BUILTIN, LOW);
        estado=iniciando;
        //Serial.println("ENVIANDO-2");
       
     }
  
}
