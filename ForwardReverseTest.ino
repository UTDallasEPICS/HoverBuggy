int motorL = 6; //left motor
int motorLrev = 3; //leftreverse

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //pinMode(motorL, OUTPUT);
  //pinMode(motorLrev, OUTPUT);
  //analogWrite(motorLrev,0);
  analogWrite(motorL, 400);
  digitalWrite(motorLrev, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(motorLrev,LOW);
  delay(5000);
  analogWrite(motorL,0);
  delay(100);
  analogWrite(motorL,400);
  digitalWrite(motorLrev,HIGH);
  delay(5000);
  analogWrite(motorL,0);
  delay(100);
  
  analogWrite(motorL,400);
    /*analogWrite(motorL, HIGH);
    delay(1000);
    analogWrite(motorL, LOW);
    delay(500);
    analogWrite(motorLrev, HIGH);
    delay(1000);
    analogWrite(motorLrev, LOW);
    delay(500);*/
      

}
