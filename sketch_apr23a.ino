#include <LiquidCrystal.h>
#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>

MFRC522 mfrc522(10, 9);
int angle = 90;
/*
 * Initialize.
 */
//Define the ultrasonic sensor pins
#define trigPin A0
#define echoPin A1

//Define the servo motor pin
#define servoPin A2


//Define the maximum distance the sensor can detect
#define maxDistance 15
bool flag = true;
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
Servo myservo;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();


  //Set the trig pin as an output and the echo pin as an input
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //Attach the servo to the servo pin
  myservo.attach(servoPin);

  //Initialize the LCD
  lcd.begin(16, 2);
}

void loop() {
  //Send a 10 microsecond pulse to the ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  //Measure the time it takes for the pulse to return
  float duration = pulseIn(echoPin, HIGH);

  //Calculate the distance based on the speed of sound
  float distance = duration * 0.034 / 2;


  //If the distance is less than or equal to the maximum distance, open the servo
  if (distance <= maxDistance) {
    if (flag) {
      printOnScreen(" hi car number  ", " user your card ");
       Serial.println("car detected");
    }
      
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial() && flag) {
      printOnScreen("   thank you    ", "enjoy your trip ");
      openGate();
      flag = false;
    }
    if (Serial.available()) {    //wait for data available
          String str = Serial.readString();  //read until timeout
          str.trim();                        // remove any \r \n whitespace at the end of the String
          if (str == "open gate") {
            openGate();
          } else if(str == "ticket not found" ) {
            Serial.println("car detected");
          }else if(str == "error"){
            Serial.println("car detected");
          }
      }
    delay(500);
  }else if(distance >=70){
    
  
  
  }else {//If the distance is greater than the maximum distance, close the servo
    if(flag == false){
      delay(3000);
      closeGate();
    }
    flag = true;
    printOnScreen("   welcome to   ", "   JANUS gates  ");
    delay(500);
  }
}


void openGate() {
  for (; angle > 0; angle--) {
    myservo.write(angle);
    delay(5);
  }
}
void closeGate() {
  for (; angle < 91; angle++) {
    myservo.write(angle);
    delay(5);
  }
}
void printOnScreen(String firstLine, String secondLine) {
  lcd.setCursor(0, 0);
  lcd.print(firstLine);
  lcd.setCursor(0, 1);
  lcd.print(secondLine);
}
