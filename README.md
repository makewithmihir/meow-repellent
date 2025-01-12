# meow-repellent
A raspberry pi pico based device that keeps cats away from certain areas using a spray of water. (Assuming cats don't like to be sprayed with water.)

Parts needed:
Raspberry pi pico
HC-SR04 Ultrasonic Sensor
1602 LCD (I2C)
Submersible pump DC 3V - 5V

Extras:
2 X LEDs
2 X 1k Resistors
Toggle switch
2 X 100nF Capacitor
2 X 1000uF Capacitor


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
Trig - 9
Echo - 8
