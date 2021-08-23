void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(analogRead(A0));
  delay(100);
}

/*
0 to 1024
Anemometer: 80 to 350 (powered at 9V and 5V)


 
 */
