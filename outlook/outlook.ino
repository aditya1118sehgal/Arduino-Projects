#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int RED_LED = 10;
int GREEN_LED = 9;
int SERIAL_LED = 13;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);a
  pinMode(RED_LED, OUTPUT);
  pinMode(SERIAL_LED ,OUTPUT);
  pinMode(GREEN_LED ,OUTPUT);
  lcd.clear();
}

void loop(){
  performSerial();
} 

void performSerial() {
   if(Serial.available()>0){
    digitalWrite(SERIAL_LED, HIGH);
        int inputCharacter = Serial.read();
        if(inputCharacter != 10) {
         Serial.println(inputCharacter);
         lcd.print(inputCharacter);
         if(inputCharacter == 97) {
            digitalWrite(RED_LED, HIGH);
            digitalWrite(GREEN_LED, LOW);
            lcd.clear();
            String tred=String(Serial.parseInt());
            String  tgreen=String(Serial.parseInt());
            String tblue=String(Serial.parseInt());
            lcd.print("BUSY until");
            lcd.setCursor(0,1);
            lcd.print("" + tred + ":" + tgreen + ":" + tblue);
         }
         else {
            digitalWrite(RED_LED, LOW);
            digitalWrite(GREEN_LED, HIGH);
            lcd.clear();
            lcd.print("available");
         }
        }
    }
    digitalWrite(SERIAL_LED, LOW);
}

