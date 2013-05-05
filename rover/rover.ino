#include <Servo.h>

#define SENSOR_INTERVAL 50

#define MAX_DISTANCE 300
#define MIN_DISTANCE_CENTER 30
#define PREF_DISTANCE_CENTER 60
#define MIN_DISTANCE_SIDE MIN_DISTANCE_CENTER
#define PREF_DISTANCE_SIDE PREF_DISTANCE_CENTER

#define FRONT_ANGLE 90
#define LEFT_ANGLE 180
#define RIGHT_ANGLE 0

#define MAX_SPEED 255
#define MIN_SPEED 150

#define STATE_IDLE 0
#define STATE_FORWARD 1
#define STATE_BACKWARD 2
#define STATE_TURN_LEFT 3
#define STATE_TURN_RIGHT 4
#define STATE_INITIAL STATE_IDLE

// Motor A
int ENA = 5;  // connected to Arduino's port 5 (output pwm)
int IN1 = 2;  // connected to Arduino's port 2
int IN2 = 3;  // connected to Arduino's port 3

// Motor B
int ENB = 6;  // connected to Arduino's port 6 (output pwm)
int IN3 = 4;  // connected to Arduino's port 4
int IN4 = 7;  // connected to Arduino's port 7

// PING
int TRIG_PIN = 13;  // connected to Arduino's port 13
int ECHO_PIN = 12;  // connected to Arduino's port 12

// Servos
int HEAD_SERVO_PIN = 10;
Servo headServo;
int rangeCenter = 0;
int rangeLeft = 0;
int rangeRight = 0;

unsigned long prevMillis;
boolean forward = true;
int currentState = STATE_INITIAL;
int leftSpeed = 0;
int rightSpeed = 0;

void stopMotors() {
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
}

void runMotors() {
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
}

void motorsForward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void motorsBackward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void motorsTurnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void motorsTurnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

int computeDistance() {
  int duration, distance;

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(1000);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration / 2) / 29.1;

  if (distance <= 0) {
    // "0" means no signal (lowest distance is 2cm)
    // assuming more than 3m
    return MAX_DISTANCE;
  }

  return distance;
}

void headTurnRight() {
  headServo.write(RIGHT_ANGLE);
  delay(500);
}

void headTurnCenter() {
  headServo.write(FRONT_ANGLE);
  delay(500);
}

void headTurnLeft() {
  headServo.write(LEFT_ANGLE);
  delay(500);
}

void computeFrontRange() {
  rangeCenter = computeDistance();
  //Serial.print("C: ");
  //Serial.print(rangeCenter);
  //Serial.print("\t");
}

void rangeSweep() {
  stopMotors();
  
  headTurnCenter();
  rangeCenter = computeDistance();
  
  headTurnLeft();
  rangeLeft = computeDistance();

  headTurnRight();  
  rangeRight = computeDistance();
  
  headTurnCenter();
}

void printState() {
  switch (currentState) {
    case STATE_FORWARD:
      Serial.print("Forward");
      break;
    case STATE_BACKWARD:
      Serial.print("Backward");
      break;
    case STATE_TURN_LEFT:
      Serial.print("Left");
      break;
    case STATE_TURN_RIGHT:
      Serial.print("Right");
      break;
    default:
      Serial.print("Idle");
      break;
  }
  
  Serial.print(" C=");
  Serial.print(rangeCenter);
  Serial.print("cm");
  
  Serial.print(" L=");
  Serial.print(rangeLeft);
  Serial.print("cm");
  
  Serial.print(" R=");
  Serial.print(rangeLeft);
  Serial.print("cm");
  
  Serial.println("");
}

void setup() {
  Serial.begin(9600);
  
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  runMotors();
  
  // setup head
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  headServo.attach(HEAD_SERVO_PIN);

  prevMillis = millis();
}

void loop() {
  switch (currentState) {
    case STATE_FORWARD: {
      Serial.println("Forward");
      stopMotors();
      delay(500);
      motorsForward();
      runMotors();
      analogWrite(ENA, MAX_SPEED);
      analogWrite(ENB, MAX_SPEED);
      delay(500);
      currentState = STATE_IDLE;
      break;
    }
    case STATE_BACKWARD: {
      Serial.println("Backward");
      stopMotors();
      delay(500);
      motorsBackward();
      runMotors();
      analogWrite(ENA, MAX_SPEED);
      analogWrite(ENB, MAX_SPEED);
      delay(500);
      currentState = STATE_IDLE;
      break;
    }
    case STATE_TURN_LEFT: {
      Serial.println("Turn left");
      stopMotors();
      delay(500);
      motorsTurnLeft();
      runMotors();
      analogWrite(ENA, MAX_SPEED);
      analogWrite(ENB, MAX_SPEED);
      delay(250);
      currentState = STATE_IDLE;
      break;
    }
    case STATE_TURN_RIGHT: {
      Serial.println("Turn right");
      stopMotors();
      delay(500);
      motorsTurnRight();
      runMotors();
      analogWrite(ENA, MAX_SPEED);
      analogWrite(ENB, MAX_SPEED);
      delay(250);
      currentState = STATE_IDLE;
      break;
    }
    case STATE_IDLE:
    default: {
      Serial.println("Idle");
      stopMotors();
      delay(500);
      
      rangeSweep();
      
      Serial.println(rangeCenter);
      
      if (MIN_DISTANCE_CENTER <= rangeCenter) {
        currentState = STATE_FORWARD;
      }
      else if (PREF_DISTANCE_SIDE <= rangeLeft) {
        currentState = STATE_TURN_LEFT;
      }
      else if (PREF_DISTANCE_SIDE <= rangeRight) {
        currentState = STATE_TURN_RIGHT;
      }
      else {
        currentState = STATE_BACKWARD;
      }
      break;
    }
  }
}


