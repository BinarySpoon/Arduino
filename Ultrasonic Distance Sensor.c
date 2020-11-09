// Libraries Used --> //
#include <LiquidCrystal.h> 
#include <HCSR04.h>
#include <Servo.h>
#include <SoftwareSerial.h>

// Pins Used --> //
#define Trig 6
#define Echo 7
#define MA 7

// Declaring Variables --> //
int measurements[MA];
int RANGE_START = 30;
int RANGE_FINISH = 150;
int Front_D = 0;
int Left_D = 0;
int Right_D = 0;
int closest_angle;
int distance;
int closest_dist = 500;
int min_distance = 10; 

// Sensor setup -->
LiquidCrystal MyLCD(12,11,5,4,3,2);   // SETUP LCD //
UltraSonicDistanceSensor IR_DIST_SENS(Trig, Echo);  // SETUP SENSOR //
Servo scanner_servo; // SETUP SERVO MOTOR //
SoftwareSerial BluetoothModule(9,8); // SETUP BLM //

void setup() {
    // STARTUP LED MESSAGE //
    scanner_servo.attach(8);
    MyLCD.begin(16,2);
    MyLCD.setCursor(0,0);
    MyLCD.print("UltraSonicDistanceSensor");
    MyLCD.setCursor(0, 1);
    MyLCD.print("!!(( ^ . ^ ))!!");
    delay(1000);
    MyLCD.clear();
    default_State();

    // SETUP SENSOR //
    Serial.begin(115200);

    byte counter = 0;   //setup byte var
    while (counter < MA)
    {
        MeasureDist();
        counter++;
    }
}

void loop()
{

  if (BluetoothModule.available() > 0) {
      char message = BluetoothModule.read();
      DisplayLCD(message);
      }
  
    for (int i = RANGE_START; i < RANGE_FINISH; i++)
    {
      scan(i);
    }

    for (int i = RANGE_FINISH; i > RANGE_START; i--)
    {
      scan(i);
    }
    closest_dist = 500;
}

void default_State() 
{
  MyLCD.setCursor(0,0);
  MyLCD.setCursor(3,0);
  MyLCD.print("DISTANCE");
}

// Measure Distance --> //
int MeasureDist() {
    static int measurements[MA]; //Static ints persists even after loop is over //
    long mean = 0;
    for (int i = 0; i < MA; i++)
    {
        mean += measurements[i];
    }

    measurements[MA-1] = IR_DIST_SENS.measureDistanceCm();
    for (int i = 0; i < MA; i++)
    {
      mean += measurements[i];
    }
    
    return mean /= MA;

}

// Depth Scanner --> //
void scan(int angle)
{
  scanner_servo.write(angle);
  distance = MeasureDist();
  MyLCD.setCursor(6,1);
  MyLCD.print(distance);
  delay(20);

  // PRINT TO SERIAL LOG //
  Serial.print(distance);
  Serial.print("\t");
  Serial.print(closest_dist);
  Serial.print("\t");
  Serial.println(angle);

  if (closest_dist > distance && distance != 1)
  {
    closest_dist = distance;
    closest_angle = angle;
  }
  if (distance <= min_distance)
  {
    MyLCD.clear();
    MyLCD.setCursor(0,0);
    MyLCD.print("OBJ DETECTED!");
    delay(300);
    MyLCD.clear();
  }
  else 
  { 
    default_State();
  } 

}

// Display Driver --> //
char DisplayLCD(char message) {
  MyLCD.clear();
  MyLCD.setCursor(0,0);
  MyLCD.print(message);
}
