#include <RH_RF95.h>
#include <RHEncryptedDriver.h>
#include <Speck.h>

RH_RF95 rf95;     // Instanciate a LoRa driver
Speck myCipher;   // Instanciate a Speck block ciphering
RHEncryptedDriver myDriver(rf95, myCipher); // Instantiate the driver with those two

float frequency = 915.0; // Change the frequency here. 
unsigned char encryptkey[16]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}; // The very secret key 


int esperandocom = 1;
int esperandoacao = 2;
int estado = esperandocom;

int ledpin = 7;

void setup() {

  pinMode(ledpin, OUTPUT);
  digitalWrite(ledpin, HIGH);
  
  Serial.begin(9600);
  while (!Serial) ; // Wait for serial port to be available
  Serial.println("LoRa Simple_Encrypted Server");
  if (!rf95.init())
    Serial.println("LoRa init failed");
  // Setup ISM frequency
  rf95.setFrequency(frequency);
  // Setup Power,dBm
  rf95.setTxPower(20);

  rf95.setSignalBandwidth(500000);
  rf95.setSpreadingFactor(9); //7 - 12
  rf95.setCodingRate4(6);
  
  myCipher.setKey(encryptkey, 16);
  delay(4000);
  Serial.println("Setup completed");
}

void loop() {

  if(estado == esperandocom){
    if (myDriver.available()) {
    // Should be a message for us now   
    uint8_t buf[myDriver.maxMessageLength()];
    uint8_t len = sizeof(buf);
    
    if (myDriver.recv(buf, &len)) {
      delay(1000);
      Serial.print("Received: ");
      Serial.println((char *)&buf);

      String aux = "ON-OK";
      char copy[10];
      aux.toCharArray(copy, aux.length()+1);
      uint8_t data[aux.length()+1] = {0};
      for(uint8_t i = 0; i<= aux.length(); i++) data[i] = (uint8_t)copy[i];
      myDriver.send(data, sizeof(data)); 
      myDriver.waitPacketSent();

      estado = esperandoacao;
    }
    else
    {
        Serial.println("recv failed");
    }
  }
  }else if(estado == esperandoacao){
    
    if (myDriver.available()) {
    // Should be a message for us now   
    uint8_t buf[myDriver.maxMessageLength()];
    uint8_t len = sizeof(buf);
    
    if (myDriver.recv(buf, &len)) {
      delay(1000);
      Serial.print("Received2: ");
      Serial.println((char *)&buf);

      String recepcao = (char *)&buf;
      Serial.println(recepcao);
      Serial.println(recepcao[4]);

      char estadoled = recepcao[4];
      
      
      if(estadoled=='0'){
        digitalWrite(ledpin, LOW);
      }else if(estadoled=='1'){
        digitalWrite(ledpin, HIGH);
      }

      estado = esperandocom;
      
    }
    else
    {
        Serial.println("recv failed");
    }
    
  }
}
  
}
