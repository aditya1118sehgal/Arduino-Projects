#include <LiquidCrystal.h>
//LiquidCrystal lcd(2, 3, 8, 9, 10, 11);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int sensorPin= 1;
int liquid_level;

const int CYCLE = 5000;
boolean toBuzz = true;
int counter = 0;
const int ALERT_DELAY = 10;
int ThermistorPin = 0;
int Vo;
float R1 = 10000;
float logR2, R2, T, Tc, Tf;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
int BUZZER = 12;
int LED_ON = 13;
int LED_HOT = 6;
int LED_COLD = 5;
int LED_NORMAL = 7;
int LED_MOTION=4;
float HOT_THRESHOLD = 28.00;
float COLD_THRESHOLD = 25.00;

void setup() {
  pinMode(LED_HOT, OUTPUT);
  digitalWrite(LED_HOT, HIGH);
  delay(1000);
  //init:
  toBuzz = true;
  counter = 0;
  Serial.begin(9600);
  //water sesnor:
  pinMode(sensorPin, INPUT);
  pinMode(12, OUTPUT);
  //blink_led(BUZZER, 100);
  //LEDs
  pinMode(LED_HOT, OUTPUT);
  pinMode(LED_ON, OUTPUT);
  pinMode(LED_COLD, OUTPUT);
  pinMode(LED_NORMAL, OUTPUT);
  pinMode(LED_MOTION, OUTPUT);
  digitalWrite(LED_ON, HIGH);
  delay(500);
  digitalWrite(LED_COLD, HIGH);
  delay(250);
  digitalWrite(LED_HOT, HIGH);
  digitalWrite(LED_COLD, LOW);
  delay(250);
  digitalWrite(LED_HOT, LOW);
  digitalWrite(LED_NORMAL, HIGH);
  delay(250);
  digitalWrite(LED_HOT, HIGH);
  digitalWrite(LED_COLD, HIGH);
  digitalWrite(LED_NORMAL, HIGH);
  delay(500);
  digitalWrite(LED_HOT, LOW);
  digitalWrite(LED_COLD, LOW);
  digitalWrite(LED_NORMAL, LOW);
  
  lcd.begin(16, 2);
}


void loop() {
  if(counter == CYCLE) {
    // update temp
    float saveT = getT();
    Tc = getTempC(saveT);
    Tf = getTempF(Tc);
    SERIAL_LED(Tc, Tf, liquid_level);
    updateLEDs(Tc);
    // reinit:
    toBuzz = true;
    counter = 0;
  }
  else{
    counter ++;
  }
  liquid_level= readWaterLevel();
  doSerial();
}
float getTempF(float Tc) {
  return ((Tc * 9.0)/ 5.0 + 32.0); 
}
float getTempC(float T) {
  return (T - 273.15);
}
float getT() {
    Vo = readThermistor();
    R2 = R1 * (1023.0 / (float)Vo - 1.0);
    logR2 = log(R2);
    T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
    return T;
}
int readThermistor() {
  return analogRead(ThermistorPin);
}

int readWaterLevel() {
  return analogRead(sensorPin);
}

void updateLEDs(float Tc) {
  if(Tc >= HOT_THRESHOLD) {
    digitalWrite(LED_HOT, HIGH);
    digitalWrite(LED_COLD, LOW);
    digitalWrite(LED_NORMAL, LOW);
  }
  else {
    if(Tc >= COLD_THRESHOLD) {
      digitalWrite(LED_HOT, LOW);
      digitalWrite(LED_COLD, LOW);
      digitalWrite(LED_NORMAL, HIGH);
    }
    else {
      digitalWrite(LED_HOT, LOW);
      digitalWrite(LED_COLD, HIGH);
      digitalWrite(LED_NORMAL, LOW);
    }
  }
}

void doSerial() {
   if(Serial.available()>0){
        int inputCharacter = Serial.read();
        if(inputCharacter != 10) {
         Serial.println(inputCharacter);
         if(inputCharacter == 97) {
            writeMotion();
            motion_alert();
         }
        }
    }
}

void motion_alert() {
  for(int i = 0; i < 10; i ++) {
    digitalWrite(LED_MOTION, HIGH);
    if(toBuzz) {
      digitalWrite(BUZZER, HIGH);
    }
    delay(ALERT_DELAY);
    if(toBuzz) {
      digitalWrite(BUZZER, LOW);
      toBuzz = false;
    }
    digitalWrite(LED_MOTION, LOW);
  }
}

void writeMotion() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MOTION !!!");
}

void writeToLCD(float t, float tt, int h2o) {
  lcd.clear(); 
  lcd.setCursor(0, 0); 
  lcd.print( String(t)+" C, "+String(tt) + " F");
  lcd.setCursor(0,1);
  lcd.print("h2o = " + String(h2o) + "cm");
}



