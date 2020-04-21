//newest one
// number of analog samples to take per reading
#define NUM_SAMPLES 10

//output pin numbers for the motors
int motorL = 6; //left motor
int motorLrev = 2; //leftreverse
int motorR = 5; //right motor

//input pins from the Joystick
int xJPin = A0; //receive x coordinate
int yJPin = A1; //receive y coordinate

//store value of x and y coordinate of the joystick
float x_coordinate;
float y_coordinate;



//store the speed of the wheels (range from approximately 65 to 200 on analogWrite)
float speed;

//parameters used to convert rectangular to polar coordinate
float theta_rad; //angle in radians
float theta_deg; //angle in degrees
float radius; //radius

int num = 1;

void setup()
{
  Serial.begin(9600);
  //pinMode(motorL, OUTPUT);
  //pinMode(motorLrev, OUTPUT);
  //digitalWrite(motorL, HIGH);
  //digitalWrite(motorLrev, LOW);
}

void loop()
{
  /*while(num == 1){
    digitalWrite(motorL, HIGH);
    delay(1000);
    digitalWrite(motorL, LOW);
    delay(500);
    digitalWrite(motorLrev, HIGH);
    delay(1000);
    digitalWrite(motorLrev, LOW);
    delay(500);
  }*/
  
  
  //Code is checking to see if the board is charging
  const unsigned long threeSeconds = 3 * 1000UL;
  static unsigned long lastSampleTime = 0 - threeSeconds;  // initialize such that a reading is due the first time through loop()

  unsigned long now = millis();
  if (now - lastSampleTime >= threeSeconds)
  {
    lastSampleTime += threeSeconds;
    if (voltageCalculator() == 1)
    {
      //Serial.print("\nCHARGING\n");
    }
    else if (voltageCalculator() == 0)
    {
      //Serial.print("\nNOT CHARGING\n");
    }


  }
  //store the position of the joystick in rectangular coordinate
  //values will range from 0 to 1023
  x_coordinate = analogRead(xJPin);
  y_coordinate = analogRead(yJPin);
/*  Serial.print("X = ");
  Serial.println(x_coordinate);
  //delay(1000);
  Serial.print("Y = ");
  Serial.println(y_coordinate);
*/

  delay(10);

  //map x and y coordinate so that they range from -500 to 500, instead of 0 to  1023
  //this will make the calculations easier
  x_coordinate = map(x_coordinate, 0, 1023, -500, 500);
  y_coordinate = map(y_coordinate, 0, 1023, -500, 500);

  //calculate the radius
  radius = sqrt(sq(x_coordinate) + sq(y_coordinate));

  if (radius < 28) { //threshhold, so small movement doesn't cause device to start by accident
    theta_rad = 0;
  }
  else {
    theta_rad = atan2(y_coordinate, x_coordinate); //calculate the degrees in radians
  }
  if (radius < 27) { //threshhold, so small movement doesn't cause device to start by accident
    radius = 0;
  }
  else if (radius >= 400) { //prevent larger value from occuring
    radius = 400;
  }

  //convert angle from radians to degrees for easier calculations
  theta_deg = theta_rad * 4068 / 71;


  //we will divide the joystick into four parts: forward, right, left, and backwards

  if ((theta_deg > 75) && (theta_deg < 105) && voltageCalculator() == 0) { //forward
    //maps the speed based on the radius, the larger the radius the larger the speed
    //radius varies from 0 to 500
    //speed varies from 65 to 200, but we only want it to go from 75 to 190 in case of discrepancies
    speed = map(radius, 0, 500, 75, 190);  // 0, 500, 100, 110
    //send same speed t
    analogWrite(motorL, speed);
    analogWrite(motorR, speed);

  }
  else if ((theta_deg > -105) && (theta_deg < -75) && voltageCalculator() == 0) { //backward
    //backwards
    Serial.print(speed);
    analogWrite(motorLrev, 110);
    //analogWrite(motorR, speed);
  }
  else if (((theta_deg < 75) && (theta_deg > 0)) && voltageCalculator() == 0 || ((theta_deg > -75) && (theta_deg < 0)) && voltageCalculator() == 0) { //left
    //only move right wheel to turn left
    analogWrite(motorR, 110);
    analogWrite(motorL, 0);

  }
  else if (((theta_deg > 105) && (theta_deg < 180)) && voltageCalculator() == 0 || ((theta_deg > -105) && (theta_deg < -180)) && voltageCalculator() == 0) { //right

    //only turn left wheel when turning right
    analogWrite(motorR, 0);
    analogWrite(motorL, 110);
  }
  else
  {
    //default is none of the motors moving
    analogWrite(motorL, 0);
    analogWrite(motorR, 0);
  }
}

int voltageCalculator()
{
  Serial.begin(230400);
  int sum = 0;                    // sum of samples taken
  unsigned char sample_count = 0; // current sample number
  float voltage = 0.0;            // calculated voltage

  // take a number of analog samples and add them up
  while (sample_count < NUM_SAMPLES) {
    sum += analogRead(A3);
    sample_count++;
    delay(10);
  }
  // calculate the voltage
  // use 5.0 for a 5.0V ADC reference voltage
  voltage = ((float)sum / (float)NUM_SAMPLES * 5.0) / 1024.0;
  // send voltage for display on Serial Monitor
  // value
//  Serial.print(voltage);
//  Serial.println (" V");
  if (voltage > .32)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}
