/*
Meow Repellent
by Mihir Patel

19 Jan 2024

Using dual core functionality of the raspberry pi pico. 
core 0 - screen update, switch state, pump, check pump trigger distance.
core 1 - distance measurement.

5 November 2023

This gizmo uses an HC-SR04 Ultrasonic sensor to suss out if there's a cat hanging around.
If the sensor sniffs out something within 60cm, it sets off a pump that gives 'em a spritz.
The aim of this contraption is to give kitties the nudge to steer clear using a water spray.

Connections: 
1602 LCD(I2C)  -  RPI PICO
GND - GND
VCC - VCC
SDA - 4
SCL - 5

PUMP MOSFET - RPI PICO
GATE - 27

TOGGLE SWITCH - RPI PICO PIN 17

HC-SR04  -  RPI PICO
VCC - 5V
GND - GND
Trig1 - 3
Echo1 - 2
Trig2 - 9
Echo2 - 8
Trig3 - 13
Echo3 - 12

*/


#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address
#define I2C_ADDR    0x27 // You may need to change this address to match your LCD module

LiquidCrystal_I2C lcd(I2C_ADDR,16,2);

#define TRIGGER_2 9   // Trigger for sensor 2 on GPIO 9 (Pin 9) on Raspberry Pi Pico
#define ECHO_2 8      // Echo for sensor 2 on GPIO 8 (Pin 8) on Raspberry Pi Pico

#define PUMP 27      // Pump on GPIO 27 (Pin 27) on Raspberry Pi Pico

#define LED_1 15 
#define LED_2 16

#define TOGGLE 17

int distance2;
unsigned long duration;

unsigned long spraycount = 0;

bool switchState;
bool updateDisplay = true;
bool ARMcount = true;

void setup() {
    
  lcd.init();  // Initialize the LCD
  lcd.backlight(); // Turn on the backlight

  //Serial.begin(115200);
  delay(100);

  /*Serial.println(" ");
  Serial.println("Serial debug for Meow Repellent");
  Serial.println(" ");
  Serial.println("Designed by Mihir Patel");
  Serial.println(" ");
  Serial.println("If you are reading this, I hope you fix whatever you're trying to fix :)");
  Serial.println(" ");
  Serial.println(" "); */


  pinMode(TRIGGER_2, OUTPUT);
  pinMode(ECHO_2, INPUT);


  pinMode(PUMP, OUTPUT);  // Set Pump as an output
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(TOGGLE, INPUT);


  digitalWrite(TRIGGER_2, LOW);

  digitalWrite(PUMP, LOW);  // Ensure the Pump is initially low
  digitalWrite(LED_1, LOW);
  digitalWrite(LED_2, LOW);

  lcd.clear();
  lcd.setCursor(1, 0); // Set the cursor to the first column and first row
  lcd.print("Meow Repellent");
  delay(1000);

  lcd.setCursor(1, 1); 
  lcd.print("by Mihir Patel");
  delay(2000);
  lcd.clear();
}

void setup1 () {

  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  //Serial.print("Setup1");

}

void loop() {

  digitalWrite(PUMP, LOW);
  digitalWrite(LED_1, LOW);

  switchState = digitalRead(TOGGLE);

  if (switchState == LOW) {

   if (ARMcount == true){
    ARMcounter();
   }

   digitalWrite(LED_2, HIGH);

    //Check if the display needs to be updated
    if (updateDisplay == true){    

     lcd.clear();
     lcd.setCursor(2, 0); 
     lcd.print("!! ARMED !!");
     lcd.setCursor(4, 1); 
     lcd.print("Count:");
     lcd.setCursor(10, 1); 
     lcd.print(spraycount);
     updateDisplay = false;

    }
   
   // Check if the distance is less than 60cm
   if (distance2 < 60) {
     pump();
     //delay(10);
    }

   //delay(50);  
  }

  if (switchState == HIGH){
   updateDisplay = true;
   ARMcount = true;
   digitalWrite(LED_2, LOW);

   lcd.clear();
   lcd.setCursor(4, 0); 
   lcd.print("DISARMED");

   lcd.setCursor(6, 1); 
   lcd.print(distance2);
   delay(500);

  }

}



void loop1 (){

  // Send a 10us pulse to trigger the HC-SR04 sensor
  digitalWrite(TRIGGER_2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_2, LOW);

  // Measure the pulse duration on the echo pin
  duration = pulseIn(ECHO_2, HIGH);

  // Calculate the distance in centimeters
  distance2 = (duration / 2) / 29.1;  // The speed of sound is approximately 29.1 microseconds per centimeter

}

void pump(){

  digitalWrite(LED_1, HIGH);
  spraycount++;
  updateDisplay = true; //Update the display
  //Serial.println(" ");
  //Serial.println("Pump ON");

  digitalWrite(PUMP, HIGH);
  delay(1000);
  digitalWrite(PUMP, LOW);
  delay(800);

  //Serial.println(" ");
  //Serial.println("Pump OFF");

}

void ARMcounter(){

  updateDisplay = true;

  lcd.clear();
  lcd.setCursor(4, 0); 
  lcd.print("Arming in");
  lcd.setCursor(4, 1); 
  lcd.print("5");
  delay(1000);

  lcd.setCursor(6, 1); 
  lcd.print("4");
  delay(1000);

  lcd.setCursor(8, 1); 
  lcd.print("3");
  delay(1000);

  lcd.setCursor(10, 1); 
  lcd.print("2");
  delay(1000);

  lcd.setCursor(12, 1); 
  lcd.print("1");
  delay(1000);

  ARMcount = false;

  lcd.clear();
  lcd.setCursor(6, 0); 
  lcd.print("Meow");
  delay(400);

}
