#include <AFMotor.h> 
#include <Servo.h>
#include <math.h>
AF_Stepper stepper(200,2);
Servo servo;
AF_DCMotor motor2(2, MOTOR12_8KHZ); 
AF_DCMotor motor(1, MOTOR12_8KHZ);
float pi = 3.14159265359;
int motorspeed = 255;
float deplacementspeed = 4.29*(motorspeed/255);
double angularecart = 1;
double cosangularecart = cos(angularecart);
long ecartmax = cosangularecart*deplacementspeed;
int trigpin = A0;
int echopin = A1;
long timer;
long dist[2];
boolean trig = true;
int angle;
long vitesse;
boolean look = true;
void setup() {
  pinMode(A0,OUTPUT);
  digitalWrite(trigpin,LOW);
  pinMode(A1,INPUT);
  servo.attach(A2);
  motor.setSpeed(motorspeed);   
  motor2.setSpeed(motorspeed);
  stepper.setSpeed(20);
}
 
void loop() {
  //servo.write(90);
  int timer = millis();
  trig = !trig;
  digitalWrite(trigpin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin,LOW);
  timer = pulseIn(echopin,HIGH);
  dist[0] = timer/58;
  if ((dist[0] < 18) && (dist[0] > 0)) {
    motor.run(RELEASE); 
    motor2.run(RELEASE);
    motor.run(BACKWARD); 
    motor2.run(BACKWARD);
    delay(1000);
    motor.run(FORWARD);
    delay(1400);
    motor.run(RELEASE);
    delay(10);
    look = true;
  }
  else if ((dist[0] > 18) && (dist[0] < 50)) {
  motor.run(FORWARD);  
  motor2.run(FORWARD);
  }
  else {
  motor.run(FORWARD);
  motor2.run(FORWARD);
  if (look && timer > 10000) {
    stepper.step(100,BACKWARD,SINGLE);
    stepper.release();
    timer = 0;
  }
  look = false;
  //stayparralel(trig);
 }     
}

void stayparralel (boolean side) {
  if (side) angle = 0;
  if (!side) angle = 180;
  motor.run(FORWARD);  
  motor2.run(FORWARD);
  servo.write(angle);
  digitalWrite(trigpin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin,LOW);
  timer = pulseIn(echopin,HIGH);
  dist[0] = timer/58;
  delay(500);
  digitalWrite(trigpin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin,LOW);
  timer = pulseIn(echopin,HIGH);
  dist[1] = timer/58;
  float timeforparralel = ((((pi/2)-acos((sqrt((dist[0]-dist[1])*(dist[0]-dist[1])))/deplacementspeed)))/deplacementspeed)*1000;
  int timeabs = fabs(timeforparralel);
  avoidside(side);
  while ((dist[0] - dist[1]) > ecartmax) {
    parralelfunction(false,side,timeabs);     
  }
  while ((dist[1] - dist[0]) > ecartmax) {
    parralelfunction(true,side,timeabs);     
  }  
}
void parralelfunction(boolean invert,boolean side,int timeabs) {
  boolean state;
  if (!invert) state = side;
  else state = !side;
      motor.run(RELEASE); 
      motor2.run(RELEASE);
      if (state) {
        motor.run(FORWARD);        

      }
      else {
        motor2.run(BACKWARD);
      }
      delay(timeabs);
      motor.run(RELEASE); 
      motor2.run(RELEASE);
      motor.run(FORWARD);  
      motor2.run(FORWARD);
      digitalWrite(trigpin,HIGH);
      delayMicroseconds(10);
      digitalWrite(trigpin,LOW);
      timer = pulseIn(echopin,HIGH);
      dist[0] = timer/58;
      delay(500);
      digitalWrite(trigpin,HIGH);
      delayMicroseconds(10);
      digitalWrite(trigpin,LOW);
      timer = pulseIn(echopin,HIGH);
      dist[1] = timer/58;  
}
void avoidside(boolean side) {
  if (dist[1] < 40) {
  if (dist[1] < 5) {
      motor.run(RELEASE); 
      motor2.run(RELEASE);
      if (side) {
        motor.run(FORWARD);
      }
      else {
        motor2.run(BACKWARD);
      }
      delay(500);      
  }
 }  
}
