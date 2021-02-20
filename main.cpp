



#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DNSServer.h>
//#include <SimpleTimer.h>
BlynkTimer timer;

//BlynkTimer timer;

char auth[] = "70jLqZ_WIgoghZDbeBGL2B8Gg2FJ39dM";
char ssid[] = "RoboLand";
char pass[] = "P0pShop9724";

const int ledPin = 5; //physical pin on ESP
const unsigned long minutes = 3000; //millis @60000/minute. Set lower for testing

//Virtual function pins incremented on Blynk app
int cycleCount; //incremented by V2
int restTime; //incremented by V3
int mixTime; //incremented by V4

//Used for duration of respective time
unsigned long stirTime; // =(mix time * minutes) interval (millis)
unsigned long waitTime; // =(rest time * minutes) interval (millis)

void setup() {
  Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,165),8080);
  Serial.begin(9600);
  pinMode(5, OUTPUT);

}

//Establish Cycle parameters: Cycles,mix time, rest time
  
    BLYNK_WRITE(V2){    //CYCLES this is the number of cycles button pin
    if(param.asInt() == 1) { //this is a Blynk function: assigning incoming value from pin V1 to a variable
      cycleCount++;            // is this the param?
        if(cycleCount > 10){
          cycleCount = 1;
        }      
      }
    Blynk.virtualWrite(1,cycleCount); // 1 is Blynk widget that increments count with button push
    Serial.print("Cycle count = "); Serial.print(cycleCount); Serial.println(" cycles.");
    //Blynk.virtualWrite(3, 127); //blynk LED blob, this has to be a value 0-255;
    }

  BLYNK_WRITE(V3){    //REST is the time between mix periods
    if(param.asInt() == 1) { //this is a Blynk function: assigning incoming value from pin V1 to a variable
      restTime++;            // is this the param?
        if(restTime > 10){
          restTime = 1;
        }            
      }
    Blynk.virtualWrite(6,restTime); // 1 is Blynk widget that increments count with button push
    Serial.print("Rest time = "); Serial.print(restTime); Serial.println(" minutes.");   
  } 

  BLYNK_WRITE(V4){ //MIX is the duration of mix period
    if(param.asInt() == 1) {//this is a Blynk function: assigning incoming value from pin V4 to a variable
      mixTime++; 
        if(mixTime > 10){
          mixTime = 1;//Serial.print (mixTime);
        }
    }
      Blynk.virtualWrite(5,mixTime); 
      Serial.print("Mix time = "); Serial.print(mixTime); Serial.print(" minutes.\n");     
  } 

  BLYNK_WRITE(V0){ //this is the program start button
    if (param.asInt()==1){
        waitTime = restTime * minutes;
        stirTime = mixTime * minutes;
      for (int i = cycleCount; i > 0; i--)
      {
      
        digitalWrite(5,HIGH); 
        Blynk.virtualWrite(7, 127); //blynk LED blob, this has to be a value 0-255;
        delay(stirTime);      

        Serial.print("Cycle "); Serial.println(cycleCount);
        
        digitalWrite(5,LOW);
        delay(waitTime);
        
        cycleCount --;
        Blynk.virtualWrite(1,cycleCount);
        Serial.print("Mix cycles remaining " ); Serial.println(cycleCount);
        if(cycleCount == 0){
        Blynk.virtualWrite(7,0);
        Blynk.virtualWrite(8,255);
        }
      }
    }
  }
     
    
void loop() {
  Blynk.run();
  //timer.run();
}