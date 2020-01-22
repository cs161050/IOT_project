//BASE LIBRARIES
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Wire.h> 
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
//BASIC STRUCT THAT STORES THE DATA FROM THE SLAVE NODES
struct data{
  float temp;
  float hum;
  int id;
  int alarm;
};
    //MATRIX KEYPAD INITIALIZER--DEFINING ROWS AND COLUMNS
    const byte ROWS = 4; // four rows
    const byte COLS = 4; // four columns
    char keys[ROWS][COLS] = {
     {'1','2','3','A'},
     {'4','5','6','B'},
     {'7','8','9','C'},
     {'*','0','#','D'}
    };
       String code="12345";//USER PASSWORD FOR THE ALARM SYSTEM
    char input[6];//KEYPAD INPUT STRING
    int count=0;
    int i=0;
    String error="FALSE PASSWORD";//ERROR MESSAGE FOR THE ALARM SYSTEM
    // DEFINING PINS FOR 4X4 KEYPAD 
    byte rowPins[ROWS] = {5,4,3,2}; // connect to the row pinouts of the keypad
    byte colPins[COLS] = {A0,A1,A2,A3}; // connect to the column pinouts of the keypad
    Keypad customKeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);//KEYPAD INITIALIZER

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); //LCD SCREEN INITIALIZER BASED ON THE INSTALLED LIBRARY(NOT THE STANDARD ADAFRUIT VERSION)
//CREATION OF THE NRF24 OBJECT 
RF24 radio(9, 8);  // CE, CSN PINS

//ADDRESS THROUGH WHICH THE MODULES COMMUNICATE
const byte address[6] = "00001";
const byte address2[6] = "00002";
void Alarm();
void setup()
{
  
  Serial.begin(9600);//INITIALISING THE SERIAL PORT
  
  radio.begin();//RADIO STARTS
  
  //SETTING THE PIPE THROUGH WHICH THE MODULES COMMUNICATE
  radio.openReadingPipe(0, address);

  //BASE IS SET AS A RECEIVER
  radio.startListening();
  lcd.begin(16,2);//LCD BEGINS WITH 2 ROWS AND 16 COLUMNS
  lcd.backlight();//BLACKLIGHT IS STARTING
}

void loop()
{
  menu();//THE BASIC FUNCTION OF THE BASE IS STARTING
 
}


void menu()//MENU FUNCTION
{
while(true)
  {
  char ch;//CHOICE CHARACTER
  struct data info; //STRUCT THAT STORES THE DATA COMING FROM THE TRANSMITTERS
  lcd.setCursor(0,0);
  lcd.print("Hit 1 for data");
  lcd.setCursor(0,1);
  lcd.print("Hit 2 for alarm");

              if (radio.available())//IF THE TRANSMITTERS ARE ON START COLLECTING DATA AND PRINT IT THROUGH THE SERIAL PORT SO THAT THE RASPBERRY CAN FETCH IT
              {
                radio.read(&info, sizeof(info));//READS THE STRUCT SENT FROM THE TRANSMITTERS AND STORES THE DATA
                Serial.print(info.id);
                Serial.print(" ");
                Serial.print(info.temp);
                Serial.print(" ");
                Serial.println(info.hum);
                
              }
                
    char customKey = customKeypad.getKey();//KEYPAD INPUT
      if(customKey)
      {
        ch=customKey;
        Serial.println(ch);
        switch(ch)
        {
          case '1':
          Data();// IF THE 1 IS PRESSED THEN PRINT TEMPERATURES AND HUMIDITY OF THE 2 ROOMS
          break;

          case '2':
          lcd.clear();
          Alarm();//IF 2 IS PRESSED THEN SET UP THE ALARM
          delay(3000);
          break;
          
        }
      }
  }

  
  
}



void Alarm()//ALARM FUNCTION
{
     int flag=0;//FLAG TO CHECK IF 5 CHARACTERS ARE ENTERED
     int up=0;//CHECKS IF THE PASSWORD IS CORRECT TO SET THE ALARM
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Enter password");
    lcd.setCursor(0,1); 
    while(flag==0)//WHILE CHARS LESS THAN 5 GET INPUT FROM KEYPAD
    {
    char customKey = customKeypad.getKey();//GET INPUT
      if(customKey)//IF THERE IS AN INPUT CHAR ADD IT TO THE ARRAY
      {
        input[count]=customKey;
        lcd.print(input[count]);//PRINT THE CHAR
        count++;//UPDATE THE COUNTER
        if(count==5)//WHEN 5 CHAR ARE ENTERED CHECK IF THE PASSWORD IS CORRECT
        {
          flag=1;
          input[5]=0;//THE LAST POSITION IS ZERO SO THAT THE STRING IS CORRECT
          String usecode= String(input);
          Serial.print(usecode);
            if(usecode.equals(code))//IF THE CODE IS CORRECT PRINT THE APROPRIATE MESSAGE AND PROCEDE TO ACTIVATE THE ALARM ELSE RETURN TO THE MENU
            {
             lcd.clear();
         
             lcd.print("CORRECT PASSWORD"); 
             up=1;
             delay(2000);
            }
            else
            {
              lcd.clear();
             lcd.print(error);//PRINT THE ERROR MESSAGE TO THE LCD
                      
            }
             
          count=0;
          
        }
       
      
      }
    }
        if(up==1)
        {
          lcd.clear();
          up=0;
          int alarm=0;
          struct data info;//STRUCT THAT STORES THE DATA SO THAT TEMP AND HUMIDITY INFO ARE SENT WHILE THE ALARM WORKS
          while(alarm==0)
          {
              
              lcd.setCursor(0,0);
              lcd.print("ALARM ON");//PRINT TO THE LCD THAT THE ALARM IS ACTIVATED
             
  
    char customKey = customKeypad.getKey();//ENTER THE PASSWORD AGAIN TO DEACTIVATE THE ALARM
      if(customKey)
      {
        input[count]=customKey;
        lcd.print(input[count]);
        count++;
        if(count==5)
        {
          flag=1;
          input[5]=0;
          String usecode= String(input);
          Serial.print(usecode);
            if(usecode.equals(code))
            {
             lcd.clear();
         
             lcd.print("ALARM OFF"); 
             alarm=1;
             delay(2000);
            }
            else
            {
              lcd.clear();
             lcd.print(error);
                      
            }
             
          count=0;
          
        }
       
      
      }
    
              
              if (radio.available())//LISTEN TO THE TRANSMITTERS WHILE THE ALARM IS ON
              {
                radio.read(&info, sizeof(info));
                Serial.print(info.id);
                Serial.print(" ");
                Serial.print(info.temp);
                Serial.print(" ");
                Serial.println(info.hum);
                
                int code = 1337;
                  if(info.alarm==1)
                  {
                    alarm=1;
                    int i=0;
                    for(i=0;i<50;i++)
                    {
                      Serial.println(code);
                      delay(200);
                    }
                  }
              }
          }
        }





      
}


void Data()// THE FUNCTION THAT PRINTS THE DATA TO THE LCD WHEN THE USER CHOOSES IT
{
  lcd.clear();
          struct data info;//STRUCT THAT STORES THE TRANSMITTER DATA
           struct data room1;//STRUCTS THAT SAVE THE DATA BASED ON THE ID
            struct data room2;
          int f1 = 0;//FLAGS THAT CHECK IF THE DATA ARE STORED TO THE CORRECT STRUCT
          int f2 = 0;
            while(f1==0&&f2==0)
            {
              if (radio.available())//LISTEN FOR THE DATA AND CHANGE THE FLAGS
                {
                  radio.read(&info, sizeof(info));
                  if(info.id==1&&f1==0)
                  {
                    room1.temp=info.temp;
                    room1.hum=info.hum;
                    f1=1;
                  }
                  else if(info.id==2&&f2==0)
                  {
                    room2.temp=info.temp;
                    room2.hum=info.hum;
                    f2=1;
                  }
                }
              
            }


                    // PRINT THE DATA TO THE LCD
                    lcd.setCursor(0,0);
                    lcd.print("T1=");
                    lcd.print(room1.temp);
                    lcd.print("H1=");
                    lcd.print(room1.hum);
          

                    lcd.setCursor(0,1);
                    lcd.print("T =");
                    lcd.print(room2.temp);
                    lcd.print("H2=");
                    lcd.print(room2.hum);  

          delay(3000);
          lcd.clear();
}
