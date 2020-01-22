#!/usr/bin/python

import serial,string, time

from pushbullet import Pushbullet


room1=[0,0] #LISTS THAT STORES THE DATA FROM EACH ROOM

room2=[0,0]

pb = Pushbullet("o.Zg1TUbOapFyqotaPLAHzGwCCpmeKgToi")#API KEY FROM PUSH BULLET APP

print(pb.devices)#PRINT OYR DEVICES


dev = pb.get_device('Xiaomi Redmi Note 4')#DEVICE WHERE THE INTRUDER MESSSAGE WILL BE SENT

	while True:	#WHILE LOOP THAT CHECKS THE INPUT AND DETERMINES IF THE ALARM IS SET AND INTRUDERS HAVE BEEN DETECTED
    
	   ser = serial.Serial('/dev/ttyUSB0',9600)#SERIAL PORT WHERE WE GET THE DATA FROM
    
	   data = ser.readline()
    
	   data = data.split()#SPLIT THE DATA AND PUT IT ON A LIST
    
	   size = len(data)
    
           print(data)
    
	   if size == 1:#IF THE SIZE IS ONE THEN WE HAVE AN ALARM
	
		if data[0] == "1337":#SPECIAL CODE THAT IDENTIFIES IF THE ALARM HAS GONE OFF
	 
		  print("INTRUDER ON") 
	
 		  push = dev.push_note("ALERT FROM HOME","INTRUDER DETECTED")#SEND THE MESSAGE TO THE DEVICE WE CHOSE
	 
		  time.sleep(3)
	
	   	else:
	  
	          pass
    
	   elif size == 3:#ID THE SIZE IS 3 THEN WE HAVE TEMP AND HUMIDITY DATA TO PRINT
       
	      if data[0]=='1':
	 
 	         room1[0]=data[1]
	  
		 room1[1]=data[2]
       
	      elif data[0]=='2':
	  
		 room2[0]=data[1]
	  
		 room2[1]=data[2]
		 
t = time.localtime()#GET TIME AND SEND DATA WHEN THE HOUR IS RIGHT
		 
current_time = time.strftime("%H:%M", t)
		 
        if current_time == "12:00"
:			
              push = dev.push_note("DATA FROM room1",room1 )
			
              push = dev.push_note("DATA FROM room2",room2 )
		 
        elif current_time == "18:00"
			
              push = dev.push_note("DATA FROM room1",room1 )
			
              push = dev.push_note("DATA FROM room2",room2 )
			
 print(room1)
    
 print(room2)


