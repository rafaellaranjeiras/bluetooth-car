#include <SoftwareSerial.h>  
#include <AFMotor.h>

//=====BLUETOOTH============
const int pinoRX = 52;
const int pinoTX = 48;
const int btgnd = 51;
char comm = 'S';
SoftwareSerial bluetooth(pinoRX, pinoTX);

//======MOTORS======
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);
const int relay = 25;
const int relaygnd = 24;

//=====HORN===============
const int hornPin = 19;
const int hornGnd = 21;
boolean hornon = false;

//=====LIGHTS============
const int alertled = 15;
const int alertgnd = 14;
const int frontlightled = 53;
const int backlightled = 23;
const int backlightgnd = 22;
boolean alert = false;
unsigned long ellapsedalert = 0;
unsigned long startedalert = 0;
const unsigned long millisalert = 500;

void setup() {
  bluetooth.begin(9600);  
  bluetooth.print("$");  
  bluetooth.print("$");  
  bluetooth.print("$");  
  delay(100);     
  pinMode(btgnd, OUTPUT);        
  pinMode(alertled, OUTPUT);
  pinMode(alertgnd, OUTPUT);
  pinMode(frontlightled, OUTPUT);
  pinMode(backlightled, OUTPUT);
  pinMode(backlightgnd, OUTPUT);
  pinMode(hornPin, OUTPUT);
  pinMode(hornGnd, OUTPUT);
  pinMode(relaygnd, OUTPUT);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
}

void loop() {
  if (bluetooth.available()) {
    comm = bluetooth.read();
    if(move() || speed() || lights() || horn()) {
    }
  }
  continous();
}

boolean move() {
  switch (comm) {
    case 'S':
      stop();
      return true;
    case 'F':
      moveForward();
      return true;
    case 'B':
      moveBack();
      return true;
    case 'L':
      turnLeft();
      return true;
    case 'R':
      turnRight();
      return true;
    case 'G':
      forwardLeft();
      return true;
    case 'H':
      backLeft();
      return true;
    case 'I':
      forwardRight();
      return true;
    case 'J':
      backRight();
      return true;
  }
  return false;
}

boolean speed() {
  switch (comm) {
    case '0':
      changeSpeed(0);
      return true;
    case '1':
      changeSpeed(150);
      return true;
    case '2':
      changeSpeed(165);
      return true;
    case '3':
      changeSpeed(180);
      return true;
    case '4':
      changeSpeed(190);
      return true;
    case '5':
      changeSpeed(205);
      return true;
    case '6':
      changeSpeed(220);
      return true;
    case '7':
      changeSpeed(230);
      return true;
    case '8':
      changeSpeed(240);
      return true;
    case '9':
      changeSpeed(250);
      return true;
    case 'q':
      changeSpeed(255);
      return true;
  }
  return false;
}

boolean lights() {
  switch (comm) {
    case 'W':
      digitalWrite(frontlightled, HIGH);
      return true;
    case 'w':
      digitalWrite(frontlightled, LOW);
      return true;
    case 'U':
      digitalWrite(backlightled, HIGH);
      return true;
    case 'u':
      digitalWrite(backlightled, LOW);
      return true;
    case 'X':
      alert = true;
      return true;
    case 'x':
      alert = false;
      return true;
  }
  return false;
}

boolean horn() {
  switch (comm) {
    case 'V':
      hornon = true;
      return true;
    case 'v':
      hornon = false;
      return true;    
  }  
  return false;
}

void continous() {
  doHorn();
  if(alert) {
    ellapsedalert = millis() - startedalert;
    if(startedalert == 0 || ellapsedalert >= millisalert)  {      
      digitalWrite(alertled, !digitalReadOutputPin(alertled));
      startedalert = millis();
    }   
  } else {
    digitalWrite(alertled, LOW);
    ellapsedalert = 0;
    startedalert = 0;
  }
}

void doHorn() {
  if(hornon) {
    digitalWrite(hornPin, HIGH);
    delayMicroseconds(800);
    digitalWrite(hornPin, LOW);
    delayMicroseconds(800);
  }else {
    digitalWrite(hornPin, LOW);
  }
}

int digitalReadOutputPin(uint8_t pin) {
  uint8_t bit = digitalPinToBitMask(pin);
  uint8_t port = digitalPinToPort(pin);
  if (port == NOT_A_PIN) 
    return LOW;

  return (*portOutputRegister(port) & bit) ? HIGH : LOW;
}

void stop() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

void moveForward() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void moveBack() {
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

void turnLeft() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

void turnRight() {
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void forwardLeft() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(FORWARD);
}

void forwardRight() {
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void backRight() {
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

void backLeft() {
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(BACKWARD);
}

void changeSpeed(int s) {
  motor1.setSpeed(s); 
  motor2.setSpeed(s); 
  motor3.setSpeed(s); 
  motor4.setSpeed(s); 
}