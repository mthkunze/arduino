#define PWM_A     3
#define DIR_A     12
#define BRAKE_A   9
#define PWM_B     11
#define DIR_B     13
#define BRAKE_B   8
#define TRIG      4
#define ECHO      2
#define FRONT     0
#define REAR      1
#define SPEED     255

#include <Servo.h>

const int trigPin = 4;      
const int echoPin = 2;
const int LEDPin = 13; // Onboard LED

long dist[1][3];

Servo servoFront;

void setup()
{
 pinMode(BRAKE_A, OUTPUT);   // Brake pin on channel A
 pinMode(DIR_A, OUTPUT);     // Direction pin on channel A
 pinMode(BRAKE_B, OUTPUT);   // Brake pin on channel B
 pinMode(DIR_B, OUTPUT);     // Direction pin on channel B
 
 // sensor init
  servoFront.attach(5); // Attach Servo pins
 
  Serial.begin (9600);
 pinMode(LEDPin, OUTPUT); 
 
}

void loop()
{
 driveHalt();
 
 // Serial.println(getDistance());
 
 scanDistances();
 
 if(dist[0][1] > 100) // Front distance check
 {
   digitalWrite(LEDPin, HIGH);  
   driveForward();
 }
 else if(dist[0][2] > 80) // Front-Left distance check
 {
   driveForwardLeft();
 }
 else if(dist[0][0] > 80) // Front-Right distance check
 {
   driveForwardRight();
 }
}

void driveForward()
{
 Serial.print("driveForward ");
  
 servoFront.write(85);
 
 delay(700);
 
 while(getDistance() > 100)
 {
   digitalWrite(BRAKE_A, LOW);
   digitalWrite(DIR_A, HIGH);
   
   analogWrite(PWM_A, SPEED);
   
   digitalWrite(BRAKE_B, LOW); // Breake OFF
  
   analogWrite(PWM_B, 0); // No Energy to MotorServo }
 }
 driveHalt();
}

void driveForwardLeft()
{
 Serial.print("driveForwardLeft ");
  
  servoFront.write(120);

 delay(800);
 
 while(getDistance() > 40)
 {
   digitalWrite(BRAKE_A, LOW);
   digitalWrite(DIR_A, HIGH);
   
   analogWrite(PWM_A, SPEED);
   
   digitalWrite(BRAKE_B, LOW); // Left Steering
   digitalWrite(DIR_B, HIGH);  // Left Steering
   
   analogWrite(PWM_B, 255);
 }
 
 driveHalt();
}

void driveForwardRight()
{
  Serial.print("driveForwardRight ");
  
  servoFront.write(58);
 
 delay(800);  
 
 while(getDistance() > 40)
 {
   digitalWrite(BRAKE_A, LOW);
   digitalWrite(DIR_A, HIGH);
   
   analogWrite(PWM_A, 255);
  
   digitalWrite(BRAKE_B, LOW); // Right Steering   
   digitalWrite(DIR_B, LOW);  // Right Steering
    
   analogWrite(PWM_B, 255);
 }
 
 driveHalt();
}

void driveHalt()
{
  
 digitalWrite(DIR_A, LOW);
 digitalWrite(BRAKE_A, HIGH);
 
 delay(1000);
 
 digitalWrite(BRAKE_A, LOW);
 analogWrite(PWM_A, 0);
     
 digitalWrite(BRAKE_B, LOW);
 analogWrite(PWM_B, 0);
 
 delay(200);  
}

void scanDistances()
{
 servoFront.write(58);
 //servoRear.write(0);
 
 delay(1000);
 
 dist[0][0] = getDistance();
 //getDistance(Right);
 Serial.print("Rechts: ");
 Serial.println(dist[0][0]);

 servoFront.write(120);
 //servoRear.write(179);
   digitalWrite(BRAKE_B, LOW); // Right Steering   
   digitalWrite(DIR_B, HIGH);  // Right Steering
    
   analogWrite(PWM_B, SPEED);
 
 delay(1000);
 
 dist[0][2] = getDistance();
 //getDistance(LEFT);
 Serial.print("Links: ");
 Serial.println(dist[0][2]);
 
 servoFront.write(85);
 //servoRear.write(90);
   digitalWrite(BRAKE_B, LOW); // Right Steering   
   digitalWrite(DIR_B, HIGH);  // Right Steering
    
   analogWrite(PWM_B, SPEED);
 
 delay(1000);
 
 dist[0][1] = getDistance();
 //getDistance(MIDDLE);
 Serial.print("Mitte: ");
 Serial.println(dist[0][1]);
}

long getDistance()
{  
 // establish variables for duration of the ping,
 // and the distance result in inches and centimeters:
 long duration, inches, cm;
 
 // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
 // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
 pinMode(trigPin, OUTPUT);
 digitalWrite(trigPin, LOW);
 delayMicroseconds(2);
 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10);
 digitalWrite(trigPin, LOW);
 
 // Read the signal from the sensor: a HIGH pulse whose
 // duration is the time (in microseconds) from the sending
 // of the ping to the reception of its echo off of an object.
 pinMode(echoPin, INPUT);
 duration = pulseIn(echoPin, HIGH);
 
 // convert the time into a distance
 inches = microsecondsToInches(duration);
 cm = microsecondsToCentimeters(duration);
  
 return cm;
}

long microsecondsToInches(long microseconds)
{
 // According to Parallax's datasheet for the PING))), there are
 // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
 // second). This gives the distance travelled by the ping, outbound
 // and return, so we divide by 2 to get the distance of the obstacle.
 // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
 return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds)
{
 // The speed of sound is 340 m/s or 29 microseconds per centimeter.
 // The ping travels out and back, so to find the distance of the
 // object we take half of the distance travelled.
 return microseconds / 29 / 2;
}
