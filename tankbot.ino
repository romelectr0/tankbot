#include <AFMotor.h> 
#include <math.h>
AF_Stepper stepper(200,2);
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
int ecopinleft = A2;
int echopinright = A3;
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
  motor.setSpeed(motorspeed);   
  motor2.setSpeed(motorspeed);
  stepper.setSpeed(20);
}
 
void loop() {
  //servo.write(90);
  int timesince = millis();
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
    delay(1300);
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
  if (look && timesince > 10000) {
    stepper.step(100,BACKWARD,SINGLE);
    stepper.release();
    timesince = 0;
  }
  look = false;
  //stayparralel(trig);
 }     
}

void stayparralel (boolean side) {
  int pintoecho;
  if (side) pintoecho = leftechopin;
  if (!side) pintoecho = rightechopin;
  motor.run(FORWARD);  
  motor2.run(FORWARD);
  digitalWrite(trigpin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin,LOW);
  timer = pulseIn(pintoecho,HIGH);
  dist[0] = timer/58;
  delay(500);
  digitalWrite(trigpin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin,LOW);
  timer = pulseIn(pintoecho,HIGH);
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
  int pintoecho;
  if (side) pintoecho = leftechopin;
  if (!side) pintoecho = rightechopin;
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
      timer = pulseIn(pintoecho,HIGH);
      dist[0] = timer/58;
      delay(500);
      digitalWrite(trigpin,HIGH);
      delayMicroseconds(10);
      digitalWrite(trigpin,LOW);
      timer = pulseIn(pintoecho,HIGH);
      dist[1] = timer/58;
}
void avoidside(boolean side) {
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
void gotopoint(float x,float y) {;
  float destination[2];
  float polar[2];
  int timetorelease;
  int actualtime;
  destination[0] = x;
  destination[1] = y;
  polar[0] = sqrt((destination[0]*destination[0])+(destination[1]*destination[1]));
  polar[1] = acos(destination[0]/polar[0]);
  robotangle(polar[1]);
  timetorelease = robotgo(polar[1]);
  actulatime = millis();
  while(true) {
  digitalWrite(trigpin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin,LOW);
  timer = pulseIn(echopin,HIGH);
  dist[0] = timer/58;
  if (actualtime == timetorelease) {
    motor.run(RELEASE);
    motor2.run(RELEASE);
    break;
  }
  if ((actualtime-1000 < timetorelease) && (dist[0] < 18)) {
    polar[0] = actualtime*0.002*deplacementspeed;
    destination[0] = destination[0]-(polar[0]*cos(polar[1]));
    destination[1] = destination[1]-(polar[0]*sin(polar[1]));
    robotangle(360-polar[1]);
        
  }
 }
 actualtime = 0;
 
}
int robotangle(float angletodo) {
}
void robotgo(float distancetotravel) {
}
