#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <Servo.h>
#define button 2
#define trigPin 13
#define echoPin 12
#define IRSMALL A0

// Initiate servo
int servo_pin = 10;
Servo servo;
int open_angle = 70;
int closed_angle = 150;

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Select ports
Adafruit_DCMotor *L_motor = AFMS.getMotor(1);
Adafruit_DCMotor *R_motor = AFMS.getMotor(2);
Adafruit_DCMotor *S_motor = AFMS.getMotor(3);
int count = 0;
int row_count = 0;
double duration, distance;

void setMotorSpeed(Adafruit_DCMotor *motor, int motor_speed) {
  motor->setSpeed(abs(motor_speed));
  if (motor_speed >= 0) {
    motor->run(FORWARD);
  }
  else if (motor_speed < 0) {
    motor->run(BACKWARD);
  }
}

void setMotorSpeeds(int L_speed, int R_speed) {
  setMotorSpeed(L_motor, L_speed);
  setMotorSpeed(R_motor, R_speed);
}

void setSweeperSpeed(int S_speed) {
  setMotorSpeed(S_motor, S_speed);
}

void turn(void){
// Turn
/* Original Turn Mechanism, redundant now
  setMotorSpeeds(-255, 50);
  delay(800);
  setMotorSpeeds(200,200);
  delay(900);
  setMotorSpeeds(210,255);
  delay(500);
  */
  // Updated turn
  setMotorSpeeds(-200,0);
  delay(1000);
  setMotorSpeeds(0,150);
  delay(600);
  setMotorSpeeds(200,200);
  delay(600);
  //setMotorSpeeds(0,0);
  //delay(49000);
  
}
void turn_180(void){
  if (row_count%2) {
    setMotorSpeeds(-250,0);
    delay(3800);
    setMotorSpeeds(-200,-200);
    delay(1000);
    ++row_count;
  }
  else {
    setMotorSpeeds(0,-250);
    delay(3800);
    setMotorSpeeds(-200,-200);
    delay(1000);
    ++row_count;
  }
}
void detectHES(void) {
  // take HES reading
  if (not port is high i.e. not magnetic) {
    // potentially reverse
    setSweeperSpeed(-200);
    delay(100);
    setMotorSpeeds(100, 100);
    delay(100);
    setSweeperSpeed(200);
    delay(500);
    setSweeperSpeed(0);
  }
}
void detectIR(void) {
  if (analogRead(IRSMALL) > 10) {
    setMotorSpeeds(0,0);
    int iter = 0;
    while (iter < 10 and analogRead(IRSMALL) > 10) {
      iter+=1;
      Serial.println("positive reading");
    }
    if (iter>9) {
      Serial.println("Block Detected");
      detectHES();
    }
  }
  
}
void shelf_park(void) {
  setMotorSpeeds(-255, -250);
  delay(1500);
  setMotorSpeeds(150, 255);
  delay(1500);
  setMotorSpeeds(255,150);
  delay(1500);
  setMotorSpeeds(150, 100);
  delay(100000);
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);// set up Serial library at 9600 bps
  pinMode(button,INPUT);
  Serial.println("Adafruit Motorshield v2 - DC Motor test!");
  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(IRSMALL, INPUT);
  setMotorSpeeds(0, 0);
  setSweeperSpeed(100);
  delay(300);
  setSweeperSpeed(0);
}
void loop() {
  // if IR: detectIR()
  setMotorSpeeds(255,240);
  detectIR();
  
  if (digitalRead(button)){
    if (count<1){
      turn();
    }
    else if (count==1) {
      shelf_park();
    }
    ++count;
  }
}
