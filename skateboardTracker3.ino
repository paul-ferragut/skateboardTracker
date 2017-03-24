//#define DEBUG

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_10DOF.h>
#include <VL6180X.h>

/* Assign a unique ID to the sensors */
Adafruit_10DOF                dof   = Adafruit_10DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
//
VL6180X sensor;
//
#define pinLed 10
#define pinHall 11
#define pinButton 13

//#include <elapsedMillis.h>
//elapsedMillis timeElapsed; //declare global if you don't want it reset every time loop runs

bool record=false;

int buttonCounter=0;
int hallCounter=0;


void setup() {

#ifdef DEBUG
Serial.begin(115200);
#endif
  
 
  Wire.begin();
  
  
  pinMode(pinLed,OUTPUT);
  pinMode(pinHall,INPUT);
  pinMode(pinButton,INPUT);

  init10Dof();
  initTof();

}

void loop() {

//RECORD STATES
  
    bool buttonState = digitalRead(pinButton);

    if(buttonState==HIGH){
      buttonDown();
    }
    if(buttonState==LOW && buttonCounter>0){
     buttonReleased();
     // we trigger start and stop record on button release
      
     //
   }

   if (record == true) {
    // turn LED on:
    digitalWrite(pinLed, HIGH);
   }else{
    digitalWrite(pinLed, LOW);
   }

   
//SENSORS

   //DOF
   sensors_vec_t orientation=loop10Dof();
   //
   
   //HALL
   bool hallVar=loopHall();
   //
   
   //TOF
   float height=loopTof();  
   //

  String str="roll:"+String(orientation.roll,DEC)+";pitch:"+String(orientation.pitch,DEC)+";heading:"+String(orientation.heading,DEC)+";hall:"+String(hallVar,DEC)+";altitude:"+String(height,DEC)+";time:"+String(millis(),DEC);

  #ifdef DEBUG
  Serial.println(str);
  #endif

}

void buttonPressed(){
  //Serial.println("button pressed");
}

void buttonDown(){
  if(buttonCounter==0){
    buttonPressed();
  }
  buttonCounter++;
  //Serial.println("button down");
}

void buttonReleased(){
  //Serial.println("button released");
  buttonCounter=0;

  record=!record;
}


void init10Dof(){
  if(!accel.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    #ifdef DEBUG
    Serial.println(F("Ooops, no LSM303 detected ... Check your wiring!"));
    #endif
    while(1);
  }
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    #ifdef DEBUG
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    #endif
    while(1);
  }
}

sensors_vec_t loop10Dof(){
  
  sensors_event_t accel_event;
  sensors_event_t mag_event;
  sensors_vec_t   orientation;

  /* Calculate pitch and roll from the raw accelerometer data */
  accel.getEvent(&accel_event);
  if (dof.accelGetOrientation(&accel_event, &orientation))
  {
    /*
    Serial.print(F("Roll: "));
    Serial.print(orientation.roll);
    Serial.print(F("; "));
    Serial.print(F("Pitch: "));
    Serial.print(orientation.pitch);
    Serial.print(F("; "));
    */
  }
  
  /* Calculate the heading using the magnetometer */
  mag.getEvent(&mag_event);
  if (dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation))
  {
    /*
    Serial.print(F("Heading: "));
    Serial.print(orientation.heading);
    Serial.print(F("; "));
    */
  }
  return orientation;
}

void initTof(){
  sensor.init();
  sensor.configureDefault();
  sensor.setTimeout(500);
}

float loopTof(){

float range=sensor.readRangeSingleMillimeters();

  if (sensor.timeoutOccurred()) { 
    #ifdef DEBUG
    Serial.print(" TIMEOUT"); 
    #endif
    }
  return range;
}

bool loopHall(){

  bool hallState = digitalRead(pinHall);

  if(hallState==HIGH){
    hallOn();
  }
  if(hallState==LOW && hallCounter>0){
    hallFinished();  
    return true;
  }
  
  return false;
}

void hallStart(){
  //Serial.println("button pressed");
}
void hallOn(){
  if(hallCounter==0){
    hallStart();
  }
  hallCounter++;
  //Serial.println("button down");
}
void hallFinished(){
  //Serial.println("button released");
  hallCounter=0;
}

//
void initSD(){
  
}

void loopSD(){
  
}
