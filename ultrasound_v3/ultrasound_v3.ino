// defines pins numbers
const int trigPin1 = 3;
const int echoPin1 = 2;
const int trigPin2 = 5;
const int echoPin2 = 4;

// defines variables
int hit1 = 0, hit2 = 0;
int numEntries = 0, numExits = 0;
long stime1 = 0;
long stime2 = 0;

// include the library code:
//#include <LiquidCrystal.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
 
// initialize the library with the numbers of the interface pins
//LiquidCrystal lcd(12, 11, 10, 9, 8, 7);
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

void setup() {
  Serial.begin(9600); // Starts the serial communication
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  //Print a message to the LCD.
  lcd.print("In / Out:");
 
  //sensors
  pinMode(trigPin1, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin2, INPUT); // Sets the echoPin as an Input
 
}
 
void loop() {
  // set the cursor to column 0, line 1
  lcd.setCursor(0, 1);
 
 
  //check sensor1
  int duration1, distance1;
  digitalWrite(trigPin1, LOW);
  delayMicroseconds (2);
  digitalWrite (trigPin1, HIGH);
  delayMicroseconds (5);
  digitalWrite (trigPin1, LOW);
  duration1 = pulseIn (echoPin1, HIGH);
  distance1 = duration1*0.034/2;
 
  if(distance1 < 150){
    Serial.println("Entering...");
    hit1 = 1;
    stime1 = millis();
  }
 
  //check sensor2
  int duration2, distance2;
  digitalWrite(trigPin2, LOW);
  delayMicroseconds (2);
  digitalWrite (trigPin2, HIGH);
  delayMicroseconds (5);
  digitalWrite (trigPin2, LOW);
 
  duration2 = pulseIn (echoPin2, HIGH);
  distance2 = duration2*0.034/2;
 
  if(distance2 < 150){
    Serial.println("Exitting...");
    hit2 = 1;
    stime2 = millis();
  }
 
  //entry was triggered, scan exit until exit is hit
  while(hit1 == 1 && hit2 == 0){
    //check sensor2
    int dur, dist;
    digitalWrite(trigPin2, LOW);
    delayMicroseconds (2);
    digitalWrite (trigPin2, HIGH);
    delayMicroseconds (5);
    digitalWrite (trigPin2, LOW);
   
    dur = pulseIn (echoPin2, HIGH);
    dist = dur*0.034/2;
   
    if((millis() - stime1) > 5000 || dist < 150){
      //reset the two values
      hit1 = 0;
      hit2 = 0;
      numEntries++;
      Serial.println("\tENTERED");
      delay(25);
      Serial.println(millis() - stime1);
      break;
    } 
  }
 
 
  //exit was triggered, scan entry until entry is hit
  while(hit1 == 0 && hit2 == 1){
    //check sensor2
    int dur, dist;
    digitalWrite(trigPin1, LOW);
    delayMicroseconds (2);
    digitalWrite (trigPin1, HIGH);
    delayMicroseconds (5);
    digitalWrite (trigPin1, LOW);
   
    dur = pulseIn (echoPin1, HIGH);
    dist = dur*0.034/2;
   
    if((millis() - stime2) > 5000 || dist < 150){
      //reset the two values
      hit1 = 0;
      hit2 = 0;
      numExits++;
      Serial.println("\tEXITED");
      delay(25);
      Serial.println(millis() - stime2);
      break;
    }
  }
 if(hit1 == 1 && hit2 == 1){
   //reset the two values
      hit1 = 0;
      hit2 = 0;
 }
  /* DEBUGGING
  Serial.print(distance);
  Serial.print("-----");
  Serial.print(distance2);
  Serial.println();
  */
  Serial.print(distance1);
  Serial.print("-----");
  Serial.print(distance2);
  Serial.println();
  Serial.print(numEntries);
  Serial.print("-----");
  Serial.print(numExits);
  Serial.println();
 
  // print the number of in and out:
  lcd.print(numEntries);
  lcd.print("  /  ");
  lcd.print(numExits);
 
  delay(300);
}
