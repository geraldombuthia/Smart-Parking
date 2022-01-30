#include <Wire.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

#define echoPin 2//parkingUltraOne
#define trigPin 3//parkingUltraOne

#define echoPin2 4//parkingUltraTwo
#define trigPin2 5//parkingUltraTwo

#define echoPin3 6//gateultra
#define trigPin3 7//gateultra

long duration, duration2, duration3;
int distance, distance2, distance3;
int servoPin = 9;

boolean parkingSpot1 = false; // false means unoccupied
boolean parkingSpot2 = false; // false means unoccupied
boolean parkingFull = false;

Servo Servo1;
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // put your setup code here, to run once:
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  lcd.init();
  lcd.begin(16, 2);
  lcd.backlight();
  Servo1.attach(servoPin);
  Serial.begin(9600);
  Serial.println("Smart Parking Started");
  Serial.println("...");

}
void loop() {
  testDisplay();
  lcdPrint("Smart Parking", 0, 0, true);
  setSpotStates();
  letInFunction();
  delay(1000);
}
void testDisplay(){
  Serial.println("Readings");
  Serial.println(parkingUltraTwo());
  Serial.println("gate");
  Serial.println(gateUltra());
  Serial.println("parking ultra one");
  Serial.println(parkingUltraOne());
  Serial.println("parking ultra two");
  Serial.println(parkingUltraTwo());
  Serial.println("Spot 1");
  Serial.println(parkingSpot1);
  Serial.println("Spot 2");
  Serial.println(parkingSpot2);
}
void letInFunction() {
  if (gateUltra() < 10 && !parkingSpot1 && !parkingSpot2) {
    lcdPrint("Welcome", 0, 0, true);
    lcdPrint("2 of 2 free", 0, 1, false);
    servoTurnUp();
    delay(5000);//A signal trigger can be used to close door
    servoTurnDown();
    parkingFull = false;
    delay(1000);
    return;
  } else if (gateUltra() < 10 && parkingSpot1 && !parkingSpot2) {
    lcdPrint("Spot 2 free", 0, 0, true);
    lcdPrint("1 of 2 free", 0, 1, false);
    servoTurnUp();
    delay(5000);//A signal trigger can be used to close door
    servoTurnDown();
    parkingFull = false;
    delay(1000);
    return;
  } else if (gateUltra() < 10 && !parkingSpot1 && parkingSpot2) {
    lcdPrint("Spot 1 free", 0, 0, true);
    lcdPrint("1 of 2 spots free", 0, 1, false);
    servoTurnUp();
    delay(5000);//A signal trigger can be used to close door
    servoTurnDown();
    parkingFull = false;
    delay(1000);
    return;
  } else if (gateUltra() < 10 && parkingSpot1 && parkingSpot2) {
    lcdPrint("Parking Full", 0, 0, true);
    lcdPrint("0 of 2 spots free", 0, 1, false);
    servoTurnDown();
    parkingFull = true;
    delay(1000);
    return;
  }

}
void setSpotStates() {
  if (gateUltra() > 10 && parkingUltraOne() < 10) {
    parkingSpot1 = true;
  } else if (gateUltra() > 10 && parkingUltraOne() > 10) {
    parkingSpot1 = false;
  }
  if (gateUltra() > 10 && parkingUltraTwo() < 10) {
    parkingSpot2 = true;
  } else if (gateUltra() > 10 && parkingUltraTwo() > 10) {
    parkingSpot2 = false;
  }
}
int parkingUltraOne() {
  //Gets data from ultrasonic of parking spot one
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  return distance = duration * 0.034 / 2;
}
int parkingUltraTwo() {
  //Gets data from ultrasonic of parking spot two
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  return distance2 = duration2 * 0.034 / 2;
}

int gateUltra() {
  //Gets data from gate ultrasonic
  digitalWrite(trigPin3, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin3, LOW);
  duration3 = pulseIn(echoPin3, HIGH);
  return distance3 = duration3 * 0.034 / 2;
}
void servoTurnUp() {
  //moves servo by 90 deg
  for (int i = 0; i <= 90; i++) {
    Servo1.write(i);
    delay(20);
  }
}
void servoTurnDown() {
  //moves servo to 0 deg
  for (int i = 90; i >= 0; i--) {
    Servo1.write(i);
    delay(20);
  }
}
void lcdPrint(String message, int row, int col, bool clearLcd) {
  //prints to lcd
  if (clearLcd) {
    lcd.clear();
  }
  lcd.setCursor(row, col);
  lcd.print(message);
}
