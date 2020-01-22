#include "DHT.h"
#define DHTPIN A0          // DHT11 DIGITAL PIN
#define DHTTYPE DHT11     // DHT11
DHT dht(DHTPIN, DHTTYPE);// INITIALISE THE DHT11
//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
int pir = 7;
int pir_status;
struct data{//STRUCT THAT STORES THE DATA TO BE SENT
  float temp;
  float hum;
  int id;
  int alarm;
};
//create an RF24 object
RF24 radio(9, 8);  // CE, CSN//START THE RADIO

//address through which two modules communicate.
const byte address[6] = "00001";

void setup()
{
  pinMode(pir, INPUT); //SET THE MOTION SENDOR PIN AS INPUT AND START THE RADIO AND THE DHT11 SENSOR
  radio.begin();
  dht.begin();
  //set the address
  radio.openWritingPipe(address);
  Serial.begin(9600);
  //Set module as transmitter
  radio.stopListening();
}
void loop()
{
  struct data info;// READ THE TEMPERATURES AND THE MOTION AND SEND THE DATA TO THE BASE
  info.id= 1;
  info.alarm = digitalRead(pir);
  delay(400);
  info.hum = dht.readHumidity();
  info.temp = dht.readTemperature();
  radio.write(&info, sizeof(info));

 
 
  
  
 

  
  
}
