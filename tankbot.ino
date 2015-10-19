#include <AFMotor.h>
 
AF_DCMotor motor2(2, MOTOR12_64KHZ); 
AF_DCMotor motor(1, MOTOR12_64KHZ);
int trigpin = A0;
int echopin = A1;
long timer;
long dist;
void setup() {
  //Serial.begin(9600);
  pinMode(A0,OUTPUT);
  digitalWrite(trigpin,LOW);
  pinMode(A1,INPUT);
  motor.setSpeed(255);   
  motor2.setSpeed(255);
}
 
void loop() {
  digitalWrite(trigpin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin,LOW);
  timer = pulseIn(echopin,HIGH);
  dist = timer/58;
  /*Serial.print("distance is : ");
  Serial.print(dist);
  Serial.println(" cm ");
  delay(300);*/
  if ((dist < 18) && (dist > 0)) {
    motor.run(RELEASE); 
    motor2.run(RELEASE);
    motor.run(BACKWARD); 
    motor2.run(BACKWARD);
    delay(1000);
    motor.run(FORWARD);
    delay(1400);
    motor.run(RELEASE);
    delay(10);
  }
  else {
  motor.run(FORWARD);  
  motor2.run(FORWARD);
  }
}
