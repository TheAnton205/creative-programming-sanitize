//LIBRARIES
#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

//Declare servo variables
Servo servo1;
Servo servo2;

//declare ultrasonic sensor pins
const int pingPin = 4;
const int echoPin = 5;

//declare an EEPROM value
int left = 118;

//declare LCD pins (Liquid Crystal Display)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() //runs once on initialize (startup)
{
    lcd.init(); //initialize the lcd
    lcd.backlight(); //open the backlight
    servo1.attach(7); //declare signal pin servo1
    servo2.attach(8); //declare singal pin servo2
    Serial.begin(9600); //initliaze serial monitor (9600 because baud rate)
    left = EEPROM.read(0); //read what's on bit one for EEPROM
    EEPROM.write(0,left); //saves the current value for left at bit 1 (0)
    
}

void servoRotate() {
       for(int i = 0; i < 180; i++) {
        servo1.write(i);
        servo2.write(180-i);
        delay(10);
        lcd.setCursor(3, 0);
    }
}

void loop()
{
  lcd.clear();
  lcd.print("Dispenses: ");
  lcd.print(left);
  delay(500);
  
   long duration, inches, cm;
   pinMode(pingPin, OUTPUT);
   digitalWrite(pingPin, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin, LOW);
   pinMode(echoPin, INPUT);
   duration = pulseIn(echoPin, HIGH);
   inches = microsecondsToInches(duration);
   cm = microsecondsToCentimeters(duration);
   Serial.print(inches);
   Serial.print("in, ");
   Serial.print(cm);
   Serial.print("cm");
   Serial.println();
   delay(100);

   if (inches <= 2) {
    Serial.println("DISPENSING");
    servoRotate();
    delay(10);
    left = --left;
    EEPROM.write(0,left);
   } 

}

long microsecondsToInches(long microseconds) {
   return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}
