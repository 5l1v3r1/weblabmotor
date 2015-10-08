#include "SPI.h"
#include "Ethernet.h"
#include <Servo.h>

Servo myservo;  // create servo object to control a servo


//Endereco MAC para o shield Ethernet
//byte mac[] = { 0x98, 0x4F, 0xEE, 0x00, 0x25, 0x6F };
byte mac[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x3A };
IPAddress ip(192, 168, 0, 102);

EthernetClient client;
EthernetServer server(10000);

//Pino de controle do motor
int motor = 6;
int statusMotor;

//Mensagem enviada pela comunicacao ethernet para controlar o experimento
char c;
String command; //string for fetching data from address
int i;

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

//Flags
boolean runCommand = false;
boolean runPID = false;

void setup() {
  // put your setup code here, to run once:

  // Inicia-se a comunicaço serial e aguarda uma resposta
  Serial.begin(9600);
  setupRede();
  setupPID();
  setupServo();
  setupPin();
  Serial.println("Inicializando...");
}

void setupPID(){
  SetSampleTime(100);
  SetOutputLimits(0,180);
  SetControllerDirection(DIRECT);
  SetTunings(1,1,1);//kp,ki,kd
  Setpoint = 300;
  SetMode(AUTOMATIC);
}

void setupServo(){
   myservo.attach(PIN_OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly

  client = server.available();

  // Verificar clients e receber comandos via ethernet.
  if (!runCommand) {
    verificarComandos(client);
  }
  
  if(runPID){
    ComputePID();
      //SetSampleTime(100);
      //SetOutputLimits(0,180);
      //SetTunings(2,4,1);//kp,ki,kd
      //Setpoint = 300;
  }

  if (runCommand && command) {
    runCommand = false;
    Serial.print("rodar comando [");
    Serial.print(command);
    Serial.println("]");
    int x = command.indexOf(':');
    int t = command.length();

    if (command == "quit") {
      client.stop();
      setup();
      command = "";
    } else if ( command.substring(0, x) == "pwm") {
      Serial.print("Dados pwm: ");
      Serial.println(command.substring(x + 1, t));
      client.stop();
      command = "";
    } else if (command.substring(0, x) == "pid") {
       Serial.println("controlando ativando pid ");
      if( command.substring(x + 1, t) == "on"){
        Serial.println("ativando pid");
        runPID=true;
      }else if(command.substring(x + 1, t) == "off"){
        Serial.println("desativando pid");
        runPID=false;
      }
    } else if(command.substring(0, x) == "motor"){
      Serial.print("controlando motor");
      controlarMotor();
      client.stop();
      command = "";
    } else if(command.substring(0, x) == "k"){
      Serial.println(command.substring(x + 1, 5)); //kp
      //Serial.println(command.substring(5, 6)); //ki
      //Serial.println(command.substring(9, 10)); //kd
      client.stop();
      command = "";
    }
      client.stop();
      command = "";
  }

  //Serial.println(i++);
  //delay(1000);

}

void setupRede() {

  if (!Ethernet.begin(mac)) {
    Serial.println("conectando com a rede...");
    Ethernet.begin(mac,ip);
    delay(1000);
  }
  delay(200);
  //system("/etc/init.d/networking restart");
  delay(200);
  server.begin();
  delay(200);
  Serial.print("IP: ");
  Serial.println(Ethernet.localIP());

}


void setupPin(){
  
  pinMode(motor,OUTPUT);
  digitalWrite(motor,LOW);

}

void verificarComandos(EthernetClient client) {

  Serial.println("Waiting data...");

  if (client) {

    Serial.println("Cliente conectado");

    while (client.available() && !runCommand) {

      char c = client.read();

      //read char by char HTTP request
      if (command.length() < 100) {
        //store characters to string
        command += c;
      }

      if (c == '\n') {
        command.trim();
        Serial.print("comando recebido [");
        Serial.print(command);
        Serial.println("]");
        runCommand = true;
        client.stop();
      }

    }
  }
}

void ComputePID() {
 
  Serial.print("Input: ");
  Serial.print(Input);

  Input = analogRead(PIN_INPUT);
  Compute();
  //analogWrite(PIN_OUTPUT, Output);
  myservo.write(Output); 

  Serial.print(" Output:");
  Serial.println(Output);

  Serial.print(kp);
  Serial.print(",");
  Serial.print(ki);
  Serial.print(",");
  Serial.println(kd);
  
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

void controlarMotor(){

statusMotor = digitalRead(motor);
 
  if(statusMotor == 1){
    Serial.println("Motor ligado, desligando motor...");
    digitalWrite(motor,LOW);
  }else{
    Serial.println("Motor desligado, Ligando motor...");
    digitalWrite(motor,HIGH);
  }
}
