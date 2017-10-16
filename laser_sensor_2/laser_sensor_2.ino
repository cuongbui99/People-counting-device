// defines pins numbers
const int lPin1 = 2;
const int lPin2 = 3;

// defines variables
int hit1 = 0, hit2 = 0;
int numEntries = 0, numExits = 0;
long stime1 = 0;
long stime2 = 0;

// include the library code:
//#include <LiquidCrystal.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD I2C address
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  


void setup() {
  Serial.begin(9600);
  // laser sensors
  pinMode(lPin1, INPUT);
  pinMode(lPin2, INPUT);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  //Print a message to the LCD.
  lcd.print("In / Out:");
  
}

void loop() {
  // set the cursor to column 0, line 1
  lcd.setCursor(0, 1);
 
  //check sensor1
  int val1 = digitalRead(lPin1);
  if(val1 == 0) {
    Serial.println("Entering...");
    hit1 = 1;
    stime1 = millis();
  }
  
    //check sensor1
  int val2 = digitalRead(lPin2);
  if(val2 == 0) {
    Serial.println("Exiting...");
    hit2 = 1;
    stime2 = millis();
  }
  
  //entry was triggered, scan exit until exit is hit
  while(hit1 == 1 && hit2 == 0){
    //check sensor2
    int tempVal2 = digitalRead(lPin2);
 
    if((millis() - stime1) > 5000){
      //reset the two values
      hit1 = 0;
      hit2 = 0;
      numEntries++;
      Serial.println("\tENTERED");
      delay(25);
      break;
    } else if(tempVal2 == 0) {
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
    //check sensor1
    int tempVal1 = digitalRead(lPin1);
    
     if((millis() - stime2) > 5000){
      //reset the two values
      hit1 = 0;
      hit2 = 0;
      numExits++;
      Serial.println("\tEXITED");
      delay(25);
      break;
    } else if(tempVal1 == 0) {
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

  Serial.print(numEntries);
  Serial.print("-----");
  Serial.print(numExits);
  Serial.println();
 
  // print the number of in and out:
  lcd.print(numEntries);
  lcd.print("  /  ");
  lcd.print(numExits);
 
  delay(500);
}
