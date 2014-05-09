/*# Stepper motor on Adafruit motor shield
//# Description:
//# turns motor counterclockwise, clockwise, clockwise, counterclockwise... after signal recieved on pin 8
//# outputs state on pins 7, 8, and 9 as HIGH when motor is in the corresponding state
//# Created May 4th, 2014 by Kit Ng
*/
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

void forward(int stepsToMove);
void backward(int stepsToMove);

//30000ms for 30 second loop
const int sleepTime=300;

//polulu motor 1209
const int stepsPerRevolution=200;

//Once pin 8 is given a HIGH signal, 
//it will start. continuous voltage not required
int cutDownPin=8;

//defines output pins on Arduino
int neutralPin=7;
int rightPin=6;
int leftPin=5;
int cutOutPin=2;

Adafruit_MotorShield AFMS = Adafruit_MotorShield();     //creates motorshield object
Adafruit_StepperMotor *ester = AFMS.getStepper(200, 1); //connects motor 200 steps on M1 and M2


void setup() {
  //sets up motorshield
  AFMS.begin();
  Serial.begin(9600);
  ester->setSpeed(10); //10rpm
  
  //accepts pin 8 as input from cutoff
  pinMode(8,INPUT);
  
  pinMode(2,OUTPUT);
  //sets pins 5 through 7 as outputs for status output
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
}

  int go=0;
  int cutCounter=0;
  
void loop(){
  if(digitalRead(cutDownPin) == HIGH) {
    go=1;
    cutCounter=1;
  }  
  //start loop at neutral position after go has been detected.
  while(go==1){
      if(cutCounter==1){
        digitalWrite(2,1);
        inform(0);
      }
      forward(stepsPerRevolution*1.5);
      inform(1);
      delay(sleepTime);
      //now at right position

      backward(stepsPerRevolution*1.5);
      inform(0); 
      delay(sleepTime);
      //now at neutral position

      backward(stepsPerRevolution*1.5);
      inform(2);
      delay(sleepTime);      
      //now at left position

      forward(stepsPerRevolution*1.5);
      inform(0);
      delay(sleepTime);
      //now at neutral position
      
      if(cutCounter==1){
        digitalWrite(2,0);
        cutCounter++;
      }
        
    }
  }


//actually moves the motor
void forward(int stepsToMove){
  ester->step(stepsToMove,FORWARD,DOUBLE);
}

void backward(int stepsToMove){
  ester->step(stepsToMove,BACKWARD,DOUBLE);
}


//Manages status update outputs
void inform(int input){  
  //resets all pins to LOW
  digitalWrite(5,0);
  digitalWrite(6,0);
  digitalWrite(7,0);
  
  switch(input){
    case 0:
      digitalWrite(neutralPin,1);      
    break;
    
    case 1:
      digitalWrite(rightPin,1);      
    break;
    
    case 2:
      digitalWrite(leftPin,1);
    break;
  }
}
