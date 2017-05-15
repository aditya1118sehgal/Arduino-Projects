int LED = 12;
int buzzer = 11;

void setup() {
  Serial.begin(9600);
pinMode(LED, OUTPUT); 
pinMode(buzzer, OUTPUT);
}

void loop(){
  int r = analogRead(0);
  digitalWrite(buzzer, LOW);
  Serial.println(r);
  if(r < 1021){ 
    digitalWrite(LED, HIGH); 
  } 
  else {
    digitalWrite(LED, LOW);
    digitalWrite(buzzer, HIGH);
  }
}
