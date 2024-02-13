#include <Servo.h>

Servo servoMotor;

#define SONU1_TRIG 3
#define SONU1_ECHO 2
#define SONU2_TRIG 6
#define SONU2_ECHO 5
#define SERVO_PORT 11
#define SERVO_ANGLE 90 // in degrees
#define SPEED_LIMIT 30 // in cm/s
#define RANGE 20       // in cm

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
    return 0.01723 * pulseIn(echoPin, HIGH); // length in cm
}

int getVelocity(byte triggerPin, byte echoPin)
{
    unsigned int x1 = readUltrasonicDistance(triggerPin, echoPin);
    // if anything is found further than RANGE cm, just ignore
    if (x1 > RANGE)
        return 0;

    delay(50);
    unsigned int x2 = readUltrasonicDistance(triggerPin, echoPin);

    return (int)((x1 - x2) / 0.05); // calculate and return velocity
}

void setup()
{
    Serial.begin(9600);
    servoMotor.attach(SERVO_PORT);
    servoMotor.write(0);
}

void loop()
{
    int velocity1 = getVelocity(SONU1_TRIG, SONU1_ECHO);
    int velocity2 = getVelocity(SONU2_TRIG, SONU2_ECHO);
    delay(50); // let everything return to stable state

    // if neither of the velocities crosses SPEED_LIMIT, just restart the function
    if (velocity1 < SPEED_LIMIT && velocity2 < SPEED_LIMIT)
        return;

    servoMotor.write(SERVO_ANGLE);

    // do not open  the barrier until the car slows down
    while (velocity1 >= SPEED_LIMIT)
    {
        velocity1 = getVelocity(SONU1_TRIG, SONU1_ECHO);
        delay(50);
    }

    while (velocity2 >= SPEED_LIMIT)
    {
        velocity2 = getVelocity(SONU2_TRIG, SONU2_ECHO);
        delay(50);
    }

    delay(5000); // wait 5 seconds before opening barrier
    servoMotor.write(0);
}