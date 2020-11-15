// Libraries Used --> 
#include <LiquidCrystal.h> 
#include <HCSR04.h>

// Pins Used --> 
#define Trig 6
#define Echo 7
#define MA 7
#define Y_LED 8

// Declaring Variables --> 
static int measurements[MA]; //Static ints persists even after loop is over //
int closest_dist;
int min_distance = 3; 

// Sensor setup -->
LiquidCrystal MyLCD(12,11,5,4,3,2);   // SETUP LCD //
UltraSonicDistanceSensor DIST_SENS(Trig, Echo);  // SETUP SENSOR //

void setup() {
    // Startup LCD message -->
    MyLCD.begin(16,2);
    MyLCD.setCursor(0,0);
    MyLCD.print("UltraSonicDistanceSensor");
    MyLCD.setCursor(0, 1);
    MyLCD.print("By AkashP");
    delay(1000);
    MyLCD.clear();
    default_State();

    // Setup Serial --> 
    Serial.begin(115200);
}

void loop() {  

  // Every 25ms --> 
  delay(25);
  int mean_distance = MeasureDist();
  Object_Alert(mean_distance);
  
  // Update Distance To LCD --> 
  MyLCD.setCursor(6,1);
  MyLCD.print(mean_distance);
    
  // Update Distance To Serial --> 
  Serial.print(mean_distance);
  Serial.print("\t");
  
}

// Additional Functions --> 
// ------------------------ 

// Mesaure Distance --> 
int MeasureDist() {
    long mean = 0;
    for (int i = 0; i < MA; i++)
    {
        mean += measurements[i];
    }

    measurements[MA-1] = DIST_SENS.measureDistanceCm();    
    for (int i = 0; i < MA; i++)
    {
        mean += measurements[i];
    }    
    return mean /= MA;

}

// Object Alert --> 
int Object_Alert(int object_distance)
{  
  if (closest_dist > object_distance && object_distance != 1)
  {
    closest_dist = object_distance;
  }
  if (object_distance <= min_distance)
  {
    MyLCD.clear();
    MyLCD.setCursor(0,0);
    MyLCD.print("OBJ DETECTED!");
    digitalWrite(Y_LED, HIGH); 
    delay(300);
    digitalWrite(Y_LED, LOW); 
    MyLCD.clear();
  }
  else 
  {
    default_State();
  }

}

// Default_State --> 
void default_State() 
{
  MyLCD.setCursor(0,0);
  MyLCD.setCursor(3,0);
  MyLCD.print("DISTANCE");
}
