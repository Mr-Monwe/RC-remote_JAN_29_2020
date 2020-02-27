// Written by Nnamdi Monwe 3/17/2019
// Courtesy of Wayne Robotics Club

// Outputs (###### Remeber to assign appropriate pins ######)
#define DIR_LEFT 2 // Direction pin for front right motor (Use digital pin)
#define DIR_RIGHT 3 // Direction pin for back right motor (Use digital pin)
#define PWM_LEFT 6 // PWM pin for front left motor (Use analog pin)
#define PWM_RIGHT 5 // PWM pin for front right motor (Use analog pin)

// Inputs (###### Remeber to assign appropriate pins ######)
#define rc_channel_1 A0 // Steering input from receiver (Use pwm pin) 
#define rc_channel_2 A1 // Throttle input from receiver (Use pwm pin)
int speedright = 0;
int speedleft = 0;
int pwm = 0;
const int forwardDeadZone = 1738;
const int reverseDeadZone = 1200;
const int leftDeadZone = 1333;
const int rightDeadZone = 1638;
const int fowardMapLimit = 2070;
const int reverseMapLimit = 985;
const int rightMapLimit = 1770;
const int leftMapLimit = 1175;
void setPWM(int pwm) {
  analogWrite(PWM_LEFT, pwm); // PWM value output for motor
  analogWrite(PWM_RIGHT, pwm); // PWM value output for motor
  Serial.print("PWM is ");
  Serial.println(pwm);
}
void setThrottle(int throttleValue) {
  Serial.println(throttleValue);
  if (throttleValue == 0) { // If no value is received from the receiver then output nothing
    Serial.println(" no signal");
  }
  else if (throttleValue > forwardDeadZone) { //Forward
    pwm = map(throttleValue, forwardDeadZone, fowardMapLimit, 0, 255); //map our speed to 0-255 range
    pwm = constrain(pwm, 0, 255); //make sure we're between 0-255
    digitalWrite(DIR_LEFT, HIGH); // Direction of motor(To change rotation change from HIGH-LOW or vise versa)
    digitalWrite(DIR_RIGHT, LOW); // Direction of motor
    setPWM(pwm);
    Serial.print("Forward Speed: ");
    Serial.println(pwm);
  }
  else if (throttleValue < reverseDeadZone) { //Reverse
    pwm = map(throttleValue, reverseDeadZone, reverseMapLimit, 0, 255); //map our speed to 0-255 range
    pwm = constrain(pwm, 0, 255); //make sure we're between 0-255
    digitalWrite(DIR_LEFT, LOW); // Direction of motor
    digitalWrite(DIR_RIGHT, HIGH); // Direction of motor
    setPWM(pwm);
    Serial.print("Reverse Speed: ");
    Serial.println(pwm);
  }
  else {
    Serial.println("Stopping throttle");
    pwm = 0;
    setPWM(pwm);
  }
}

void setDirectionalSteering(int steeringValue) {
  if (steeringValue > rightDeadZone ) { //Right
    pwm = map(steeringValue, rightDeadZone, rightMapLimit, 0, 255); //map our speed to 0-255 range
    pwm = constrain(pwm, 0, 255); //make sure we're between 0-255
    digitalWrite(DIR_LEFT, LOW); // Direction of motor
    digitalWrite(DIR_RIGHT, LOW); // Direction of motor
    setPWM(pwm);
    Serial.print("Right Speed: ");
    Serial.println(pwm);
  }
  else if (steeringValue < leftDeadZone ) { //Left
    pwm = map(steeringValue, leftDeadZone, leftMapLimit, 0, 255); //map our speed to 0-255 range
    pwm = constrain(pwm, 0, 255); //make sure we're between 0-255
    digitalWrite(DIR_LEFT, HIGH); // Direction of motor
    digitalWrite(DIR_RIGHT, HIGH); // Direction of motor
    setPWM(pwm);
    Serial.print("Left Speed: ");
    Serial.println(pwm);
  } else { // If no value is received from the receiver then output nothing
    Serial.println("wheel centered");
    pwm = 0;
    setPWM(pwm);
    //analogWrite(PWM_LEFT, 0);
    //analogWrite(PWM_RIGHT, 0);

  }
}

void setup() { // Declare pins as INPUT/OUTPUT

  pinMode(rc_channel_1, INPUT);
  pinMode(rc_channel_2, INPUT);

  pinMode(DIR_RIGHT, OUTPUT);
  pinMode(DIR_LEFT, OUTPUT);
  pinMode(PWM_LEFT, OUTPUT);
  pinMode(PWM_RIGHT, OUTPUT);

  Serial.begin(9600);// Using Serial Monitor as display
}

void loop() {

  int steeringValue = pulseIn(rc_channel_1, HIGH);//, 25000);
  int throttleValue = pulseIn(rc_channel_2, HIGH);//, 25000);

  Serial.print("steering Values: ");
  Serial.println(steeringValue);
  Serial.print("throttle Values: ");
  Serial.println(throttleValue);

  //If both steering and throttle are in action this function calls and the car can turn
  // if (steeringValue < 1420 || steeringValue > 1540 && throttleValue > 1570 || throttleValue < 1460
  //   steerAndThrottle(steeringValue, throttleValue);
  //If not steering
  if (throttleValue < 1420 || throttleValue > 1540){
    Serial.println("Throttle");
    setThrottle(throttleValue);
  }else if (steeringValue > 1570 || steeringValue < 1460){
    Serial.println("Steering");
    setDirectionalSteering(steeringValue);
  }else{
    setPWM(0);
  }
  Serial.println();

  delay(10); // Just a tiny delay to keep things in order
}
