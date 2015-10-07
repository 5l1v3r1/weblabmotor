
#include <Servo.h>

Servo myservo;  // create servo object to control a servo

/*working variables*/
unsigned long lastTime;
double Input, Output;
double Setpoint;
double ITerm, lastInput;
double kp, ki, kd;
int SampleTime;
int controllerDirection;
double outMin, outMax;
bool inAuto;

#define MANUAL 0
#define AUTOMATIC 1

#define DIRECT 0
#define REVERSE 1

#define PIN_INPUT 0
#define PIN_OUTPUT 3

void setup(){
  Serial.begin(9600);
  myservo.attach(PIN_OUTPUT);  // attaches the servo on pin 9 to the servo object
  SetSampleTime(100);
  SetOutputLimits(0,180);
  SetControllerDirection(DIRECT);
  SetTunings(1,1,1);//kp,ki,kd
  Setpoint = 300;
  SetMode(AUTOMATIC);
  
}

void loop(){
  Serial.print("Input: ");
  Serial.print(Input);

  Input = analogRead(PIN_INPUT);
  Compute();
  //analogWrite(PIN_OUTPUT, Output);
  myservo.write(Output); 

  Serial.print(" Output:");
  Serial.println(Output);

  //SetSampleTime(100);
  //SetOutputLimits(0,180);
  //SetTunings(2,4,1);//kp,ki,kd
  //Setpoint = 300;
  
  delay(20);
}

void Compute()
{
   if(!inAuto) return;
   unsigned long now = millis();
   int timeChange = (now - lastTime);
   if(timeChange>=SampleTime)
   {
      /*Compute all the working error variables*/
      double error = Setpoint - Input;
      //Serial.print("Erro:");
      //Serial.println(error);
      ITerm+= (ki * error);
      if(ITerm > outMax) ITerm= outMax;
      else if(ITerm < outMin) ITerm= outMin;
      double dInput = (Input - lastInput);

      /*Compute PID Output*/
      Output = kp * error + ITerm- kd * dInput;
      if(Output > outMax) Output = outMax;
      else if(Output < outMin) Output = outMin;

      /*Remember some variables for next time*/
      lastInput = Input;
      lastTime = now;
   }
}

void SetTunings(double Kp, double Ki, double Kd)
{
   if (Kp<0 || Ki<0|| Kd<0) return;

  double SampleTimeInSec = ((double)SampleTime)/1000;
   kp = Kp;
   ki = Ki * SampleTimeInSec;
   kd = Kd / SampleTimeInSec;

  if(controllerDirection == REVERSE)
   {
      kp = (0 - kp);
      ki = (0 - ki);
      kd = (0 - kd);
   }
}

void SetSampleTime(int NewSampleTime)
{
   if (NewSampleTime > 0)
   {
      double ratio  = (double)NewSampleTime
                      / (double)SampleTime;
      ki *= ratio;
      kd /= ratio;
      SampleTime = (unsigned long)NewSampleTime;
   }
}

void SetOutputLimits(double Min, double Max)
{
   if(Min > Max) return;
   outMin = Min;
   outMax = Max;

   if(Output > outMax) Output = outMax;
   else if(Output < outMin) Output = outMin;

   if(ITerm > outMax) ITerm= outMax;
   else if(ITerm < outMin) ITerm= outMin;
}

void SetMode(int Mode)
{
	bool newAuto = (Mode == AUTOMATIC);
	if(newAuto == !inAuto)
	{  /*we just went from manual to auto*/
		Initialize();
	}
	inAuto = newAuto;
}

void Initialize()
{
   lastInput = Input;
   ITerm = Output;
   if(ITerm > outMax) ITerm= outMax;
   else if(ITerm < outMin) ITerm= outMin;
}

void SetControllerDirection(int Direction)
{
   controllerDirection = Direction;
}

