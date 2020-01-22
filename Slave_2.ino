#include "DHT.h"
#define DHTPIN A0          // what digital pin we're connected to
#define DHTTYPE DHT11     // DHT11
DHT dht(DHTPIN, DHTTYPE);
//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
int pir = 7;
int pir_status;

struct data{
  float temp;
  float hum;
  int id;
  int alarm;
};
//create an RF24 object
RF24 radio(9, 8);  // CE, CSN

//address through which two modules communicate.
const byte address2[6] = "00001";

void setup()
{
  pinMode(pir, INPUT);
  radio.begin();
  dht.begin();
  //set the address
  radio.openWritingPipe(address2);
  Serial.begin(9600);
  //Set module as transmitter
  radio.stopListening();
}
void loop()
{
  int command = 0;
  struct data info;
  info.id= 2;
  info.alarm = digitalRead(pir);
  delay(200);
  info.hum = dht.readHumidity();
  info.temp = dht.readTemperature();
  radio.write(&info, sizeof(info));

 
 
  
  
 

  

}
