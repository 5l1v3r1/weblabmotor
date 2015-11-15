/*
 * Comandos de controle da UCS (Unidade de Controle Remoto)
 * motor => Liga o motor se desligado, caso contrario desliga
 * reset => Reinicia a UCS
 * pwm:123 => Atribui valor PWM em porta pre configurada
 * pid:on e off => Liga/Desliga controle PID
 * k:1,2,3 => Atribui valores de Kp, Ki e kd
--------------------------------
D13  ETH (SPI-SCK)
D12  ETH (SPI-SDO)
D11~ ETH
D10~ ETH
D9~
D8
D7  CARGA (100%)
D6~ CARGA (75%)
D5~ CARGA (50%)
D4  CARGA (25%)
D3~ SERVO (ACEL.)
D2  MOTOR (ON/OFF)
D1
D0

A0 ROT
A1        (OPEN)
A2
A3
A4
A5
*/

#include "SPI.h"
#include "Ethernet.h"
#include "Servo.h"

Servo myservo;  // create servo object to control a servo

//Endereco MAC para o shield Ethernet
//byte mac[] = { 0x98, 0x4F, 0xEE, 0x00, 0x25, 0x6F };
byte mac[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x3A };
IPAddress ip(192, 168, 0, 102);

EthernetClient client;
EthernetServer server(10000);

//Pino de controle do motor
int motor = 2;

//Controle da carga
int carga_25 = 4;
int carga_50 = 5;
int carga_75 = 6;
int carga_100 = 7;

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

//..................................MAIN.....................................
//..................................SETUP.....................................
void setup() {

  Serial.begin(115200);
  setupRede();
  setupPID();
  setupServo();
  setupPin();
}
//..................................END SETUP.................................
//..................................LOOP.....................................
void loop() {

  client = server.available();

  verificarComandos(client);

  if (runCommand) {
    runCommand = false;
    executarComandoRecebido();
  }

  if (runPID) {
    ComputePID();
  }

}
//..................................END LOOP.....................................
//..................................END MAIN...................................

//.......................................SETUP........................................
void setupPID() {
  SetSampleTime(250);
  SetOutputLimits(35, 105);
  SetControllerDirection(REVERSE);
  SetTunings(1, 0, 0); //kp,ki,kd
  Setpoint = 600;
  SetMode(AUTOMATIC);
}

void setupServo() {
  myservo.attach(PIN_OUTPUT);
  myservo.write(105);
}

void setupRede() {

  if (!Ethernet.begin(mac)) {
    Serial.println("conectando com a rede...");
    Ethernet.begin(mac, ip);
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

void setupPin() {

  pinMode(motor, OUTPUT);
  pinMode(carga_25, OUTPUT);
  pinMode(carga_50, OUTPUT);
  pinMode(carga_75, OUTPUT);
  pinMode(carga_100, OUTPUT);
  digitalWrite(motor, LOW);
  digitalWrite(carga_25, LOW);
  digitalWrite(carga_50, LOW);
  digitalWrite(carga_75, LOW);
  digitalWrite(carga_100, LOW);

}
//.....................................END SETUP................................

//.....................................FUNCTIONS...............................
void verificarComandos(EthernetClient client) {

  //Serial.println("Waiting data...");

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

void executarComandoRecebido() {

  Serial.print("executar comando [");
  Serial.print(command);
  Serial.println("]");
  int x = command.indexOf(':');
  int t = command.length();

  if (command == "reset") {
    resetComando();
    setup();
    
  } else if ( command.substring(0, x) == "pwm") {
    Serial.print("Dados pwm: ");
    Serial.println(command.substring(x + 1, t));
    resetComando();

  } else if (command.substring(0, x) == "pid") {

    if ( command.substring(x + 1, t) == "on") {
      Serial.println("ativando PID");
      runPID = true;
    } else if (command.substring(x + 1, t) == "off") {
      Serial.println("desativando PID");
      runPID = false;
    }

  } else if (command.substring(0, x) == "motor") {

    if ( command.substring(x + 1, t) == "on") {
      Serial.println("ativando MOTOR");
      ligarMotor();
    } else if (command.substring(x + 1, t) == "off") {
      Serial.println("desativando MOTOR");
      desligarMotor();
    }
    
    resetComando();

  } else if (command.substring(0, x) == "k") {
    Serial.println(command.substring(x + 1, 5)); //kp
    String kp = command.substring(x + 1, 5);
    //Serial.println(command.substring(5, 6)); //ki
    //Serial.println(command.substring(9, 10)); //kd
    //SetSampleTime(100);
    //SetOutputLimits(0,180);
    SetTunings(kp.toInt(),0,0);//kp,ki,kd
    //Setpoint = 300;
    resetComando();

  } else if (command.substring(0, x) == "carga") {

    if ( command.substring(x + 1, t) == "0") {
      Serial.println("desativar carga...");
      digitalWrite(carga_25, LOW);
      digitalWrite(carga_50, LOW);
      digitalWrite(carga_75, LOW);
      digitalWrite(carga_100, LOW);
      resetComando();
    } else if (command.substring(x + 1, t) == "25") {
      Serial.println("25% de carga...");
      digitalWrite(carga_25, HIGH);
      resetComando();
    } else if (command.substring(x + 1, t) == "50") {
      Serial.println("50% de carga...");
      digitalWrite(carga_50, HIGH);
      resetComando();
    } else if (command.substring(x + 1, t) == "75") {
      Serial.println("75% de carga...");
      digitalWrite(carga_75, HIGH);
      resetComando();
    } else if (command.substring(x + 1, t) == "100") {
      Serial.println("100% de carga...");
      digitalWrite(carga_100, HIGH);
      resetComando();
    }

    resetComando();

  }

  resetComando();

}

void resetComando() {
  client.stop();
  command = "";
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
  if (!inAuto) return;
  unsigned long now = millis();
  int timeChange = (now - lastTime);
  if (timeChange >= SampleTime)
  {
    /*Compute all the working error variables*/
    double error = Setpoint - Input;
    //Serial.print("Erro:");
    //Serial.println(error);
    ITerm += (ki * error);
    if (ITerm > outMax) ITerm = outMax;
    else if (ITerm < outMin) ITerm = outMin;
    double dInput = (Input - lastInput);

    /*Compute PID Output*/
    Output = kp * error + ITerm - kd * dInput;
    if (Output > outMax) Output = outMax;
    else if (Output < outMin) Output = outMin;

    /*Remember some variables for next time*/
    lastInput = Input;
    lastTime = now;
  }
}

void SetTunings(double Kp, double Ki, double Kd)
{
  if (Kp < 0 || Ki < 0 || Kd < 0) return;

  double SampleTimeInSec = ((double)SampleTime) / 1000;
  kp = Kp;
  ki = Ki * SampleTimeInSec;
  kd = Kd / SampleTimeInSec;

  if (controllerDirection == REVERSE)
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
  if (Min > Max) return;
  outMin = Min;
  outMax = Max;

  if (Output > outMax) Output = outMax;
  else if (Output < outMin) Output = outMin;

  if (ITerm > outMax) ITerm = outMax;
  else if (ITerm < outMin) ITerm = outMin;
}

void SetMode(int Mode)
{
  bool newAuto = (Mode == AUTOMATIC);
  if (newAuto == !inAuto)
  { /*we just went from manual to auto*/
    Initialize();
  }
  inAuto = newAuto;
}

void Initialize()
{
  lastInput = Input;
  ITerm = Output;
  if (ITerm > outMax) ITerm = outMax;
  else if (ITerm < outMin) ITerm = outMin;
}

void SetControllerDirection(int Direction)
{
  controllerDirection = Direction;
}

void ligarMotor() {

  Serial.println("Ligando motor...");
  
  myservo.write(105);
  digitalWrite(motor, HIGH);
  delay(1500);
  digitalWrite(motor, LOW);  

}

void desligarMotor() {

  Serial.println("Desligando motor...");
  
  runPID = false ;
  myservo.write(35);
  
}
