//output pin numbers for the motors
 int motorL = 6; //left motor
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



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

 //declare pins as output
  pinMode(motorR, OUTPUT);
  pinMode(motorL, OUTPUT);
  
  
}

void loop() {
 
  //store the position of the joystick in rectangular coordinate
  //values will range from 0 to 1023
  x_coordinate = analogRead(xJPin);
  y_coordinate = analogRead(yJPin);
  
  delay(10);

  //map x and y coordinate so that they range from -500 to 500, instead of 0 to  1023
   //this will make the calculations easier
  x_coordinate = map(x_coordinate, 0, 1023, -500, 500);
  y_coordinate = map(y_coordinate, 0, 1023, -500, 500);

  //calculate the radius
  radius = sqrt(sq(x_coordinate) + sq(y_coordinate));   
 
    if(radius < 28) { //threshhold, so small movement doesn't cause device to start by accident
      theta_rad = 0;
    }
    else {
      theta_rad = atan2(y_coordinate,x_coordinate); //calculate the degrees in radians
    }
    if(radius < 27) {  //threshhold, so small movement doesn't cause device to start by accident
      radius = 0;
    }
    else if(radius >= 400) { //prevent larger value from occuring
      radius = 400;
    }
  
 //convert angle from radians to degrees for easier calculations
  theta_deg = theta_rad * 4068/71;
  
 
 //we will divide the joystick into four parts: forward, right, left, and backwards
 
    if((theta_deg > 75) && (theta_deg < 105)) { //forward
     //maps the speed based on the radius, the larger the radius the larger the speed
     //radius varies from 0 to 500
     //speed varies from 65 to 200, but we only want it to go from 75 to 190 in case of discrepancies
     speed = map(radius, 0, 500, 75, 190);
     //send same speed t
      analogWrite(motorL, speed);
      analogWrite(motorR, speed);
      
    }
    else if((theta_deg > -105) && (theta_deg < -75)) { //backward
      

    }
    else if(((theta_deg < 75) && (theta_deg > 0)) || ((theta_deg > -75) && (theta_deg < 0))) { //left
     //only move right wheel to turn left
     analogWrite(motorR, 110);
      analogWrite(motorL, 0);
    
    }
    else if(((theta_deg > 105) && (theta_deg < 180)) || ((theta_deg > -105) && (theta_deg < -180))) { //right   
    
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
