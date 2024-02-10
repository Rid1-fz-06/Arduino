#include <Servo.h>

Servo servoMotor1;
Servo servoMotor2;

#define SONU_TRIG 8
#define SONU_ECHO 7
#define SERVO1_PORT 9
#define SERVO2_PORT 10
#define RANGE 40 // in cm

unsigned int readUltrasonicDistance(byte triggerPin, byte echoPin)
{
    pinMode(triggerPin, OUTPUT); // Clear the trigger
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    // Sets the trigger pin to HIGH state for 10 microseconds
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);
    pinMode(echoPin, INPUT);
    return 0.01723 * pulseIn(echoPin, HIGH); // return length in cm
}

void setup()
{
    Serial.begin(9600);
    pinMode(4, OUTPUT);
    servoMotor1.attach(SERVO1_PORT);
    servoMotor2.attach(SERVO2_PORT);
    servoMotor1.write(90);
    servoMotor2.write(90);
}

void loop()
{
    static bool servoStatus = false;

    unsigned int x1 = readUltrasonicDistance(SONU_TRIG, SONU_ECHO);
    // Serial.print("x1 = ");
    // Serial.println(x1);
    delay(500);
    unsigned int x2 = readUltrasonicDistance(SONU_TRIG, SONU_ECHO);
    // Serial.print("x2 = ");
    // Serial.println(x2);

    if (x1 < RANGE && servoStatus == false && x1 > x2 && abs(x1 - x2) >= 5)
    {
        digitalWrite(4, HIGH);
        delay(1500);
        servoMotor1.write(180);
        servoMotor2.write(0);
        servoStatus = true;
    }
    else if ((((x1 <= x2) && abs(x1 - x2) <= 5) || x2 >= RANGE) && servoStatus == true)
    {
        delay(5000);
        digitalWrite(4, LOW);
        servoMotor1.write(90);
        servoMotor2.write(90);
        servoStatus = false;
    }
    delay(50);
}