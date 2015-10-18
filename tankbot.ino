#include <AFMotor.h>
 
AF_DCMotor motor2(2, MOTOR12_64KHZ);
AF_DCMotor motor(1, MOTOR12_64KHZ);
int trigpin = A0;
int echpin = A1;
int timer;
int dist;
void setup() {
  pinMode(A0,OUTPUT);
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
  if (dist < 10) {
    motor.run(FORWARD);
    delay(5000);
    motor.run(RELEASE);
    delay(10);
  }
  else {
  motor.run(FORWARD);    
  motor2.run(FORWARD);
  }
}
