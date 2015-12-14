/*
 * Comandos de controle da USR (Unidade Sensoriamento Remoto)
 * 1 => Inicia leitura de dados com graficos offline (modo CSV)
 * 2 => Para leitura de dados no modo CSV
 * 3 => Deleta ultima leitura no modo CSV
 * 4 => Inicia leitura de dados com grafico online (modo HTTP)
 * 5 => Para leitura de dados no modo HTTP
 * 9 => Reinicia USR
 --------------------------------
 Barometro: GY-65
 I2C address is 0x77
 VCC <-> +3.3V
 SDA <-> SDA
 SCL <-> SCL
 XCLR <->
 EOC <-> 
 GND <-> GND
 --------------------------------
 Vibracao: GY-521
 VCC <-> 
 GND <->
 SCL <->
 SDA <->
 XDA <->
 XCL <->
 AD0 <-> +3,3V I2C address is 0x69 | GND I2C address is 0x68
 INT <->
 --------------------------------
 Termopar: TPA
 GND <-> GND
 GND <-> GND
 A3 <-> D7
 SDO <-> 12
 SCK <-> 13
 5V <-> +5V
 --------------------------------
 Amperimetro: AMP-AC
 GND <-> GND
 VCC <-> +5V
 OUT <-> A1
 --------------------------------
 Voltimetro: VLT-AC
 GND <-> GND
 VCC <-> +5V
 OUT <-> A0
 --------------------------------
 Amperimetro: AMP-DC
 GND <-> GND
 VCC <-> +5V
 OUT <-> A2
 --------------------------------
 Voltimetro: VLT-DC
 GND <-> GND
 VCC <-> +5V
 OUT <-> A3
 --------------------------------
 Rotacao: ROT
 GND <-> GND
 VCC <-> +5V
 OUT <-> A4
 --------------------------------
 --------------------------------
 SCL GY-65 (I2C)
 SDA GY-65 (I2C)
 
 D13 TPA (SPI-SCK)
 D12 TPA (SPI-SDO)
 D11~
 D10~
 D9~
 D8
 
 D7
 D6~
 D5~
 D4 
 D3~
 D2 TPA
 D1
 D0
 
 A0 AMP-AC
 A1 VLT-AC
 A2 AMP-DC
 A3 VLT-DC
 A4 ROT
 A5
 */

#include "I2Cdev.h"
#include "MPU6050.h"
#include "BMP085.h"
#include "Wire.h"
#include "SPI.h"
#include "Ethernet.h"
#include "Nanoshield_Thermocouple.h"

//..................................VARIABLES.............................
struct dadosUSR {       
  char msg[50];
  double voltAC;
  double ampAC;
  int rotacao;
  int tempUSR;
  int tempTPAint;
  int tempTPAext;
  int tempBAR;
  int presBAR;
  int altBAR;
  char tempo[50];
}; 

//variável global que contém todos os elementos da estrutura
struct dadosUSR dadosSensores;

Nanoshield_Thermocouple thermocouple;

//Endereco MAC para o shield Ethernet
//byte mac[] = { 0x98, 0x4F, 0xEE, 0x00, 0x25, 0x6F };
byte mac[] = { 
  0x98, 0x4F, 0xEE, 0x01, 0x14, 0x8A };
IPAddress ip(192, 168, 0, 101);
IPAddress serverMySQL(192,168,0,100);  // numeric IP for MySQL server

EthernetClient client;
EthernetServer server(10000);

//Mensagem enviada pela comunicacao ethernet para controlar o experimento
char c = 0;
char command = 0;

//Flags
//Comando recebido com sucesso
boolean runCommand = false;
boolean flagReadSensorsCSV = false;
boolean flagReadSensorsHTTP = false;
boolean flagCreateFile = false;

//temperatura placa galileo
char scale[4];
char raw[4];
char offset[4];

int raw_i;
int scale_i;
int offset_i;

//Barometro
double temperature;
double pressure;
double altitude;
// class default I2C address is 0x77
// specific I2C addresses may be passed as a parameter here
// (though the BMP085 supports only one address)
BMP085 barometer;

//Acelerometro
int32_t lastMicros;
int16_t ax, ay, az;
int16_t gx, gy, gz;

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;
//MPU6050 accelgyro(0x69); // <-- use for AD0 high


// uncomment "OUTPUT_READABLE_ACCELGYRO" if you want to see a tab-separated
// list of the accel X/Y/Z and then gyro X/Y/Z values in decimal. Easy to read,
// not so easy to parse, and slow(er) over UART.
#define OUTPUT_READABLE_ACCELGYRO

// uncomment "OUTPUT_BINARY_ACCELGYRO" to send all 6 axes of data as 16-bit
// binary, one right after the other. This is very fast (as fast as possible
// without compression or data loss), and easy to parse, but impossible to read
// for a human.
//#define OUTPUT_BINARY_ACCELGYRO

const int sensorIn = 1;
int mVperAmp = 66; // use 100 for 20A Module and 66 for 30A Module

double Voltage = 0;
double VRMS = 0;
double AmpsRMS = 0;

const int voltimetroPin = 0;

volatile int half_revolutions;

double rpm;
int a;

unsigned long timeold;

boolean startInterrupt;

//..................................END VARIABLES.............................

//..................................MAIN.....................................
//..................................SETUP.....................................
void setup() {

  Serial.begin(115200);

  setupRede();
  setupI2C();
  setupBarometro();
  setupTermopar();
  setupRotacao();
}
//..................................END SETUP.................................
//..................................LOOP.....................................
void loop() {

  client = server.available();

  verificarComandos(client);

  if(runCommand){
    runCommand = false;
    executarComandoRecebido();
  }

  if(flagReadSensorsCSV){
    delay(1000);
    readSensorsCSV();    
  }

  if(flagReadSensorsHTTP){
    delay(1000);
    readSensorsHTTP();    
  }

  delay(1000);
}
//..................................END LOOP.....................................
//..................................END MAIN...................................

//.......................................SETUP........................................
void setupTermopar(){

  Serial.println("-------------------------------");
  Serial.println(" Nanoshield Serial Thermometer");
  Serial.println("-------------------------------");
  Serial.println("");

  // Initialize the thermocouple
  // The CS pin can be passed as a parameter if different than pin D8,
  //  e.g. thermocouple.begin(7)
  thermocouple.begin(7);
}

void setupRede(){

  if(!Ethernet.begin(mac)){
    Serial.println("conectando com a rede...");
    Ethernet.begin(mac,ip);
    delay(1000);
  }

  delay(200);
  system("/etc/init.d/networking restart");
  delay(200);
  server.begin();
  delay(200);
  Serial.print("IP: ");
  Serial.println(Ethernet.localIP());

}

void setupI2C(){
  // join I2C bus (I2Cdev library doesn't do this automatically)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif

  // initialize serial communication
  // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
  // it's really up to you depending on your project)

  // initialize device
  Serial.println("Initializing I2C devices...");

}

void setupBarometro() {
  barometer.initialize();

  // verify connection
  Serial.println(barometer.testConnection() ? "BMP085 connection successful" : "BMP085 connection failed");

}

void setupVibracao() {

  accelgyro.initialize();

  // verify connection
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
}

void setupRotacao()
{
  half_revolutions = 0;
  rpm = 0.0;
  timeold = 0;
  startInterrupt = true;
}

//.....................................END SETUP................................

//.....................................FUNCTIONS...............................
void executarComandoRecebido(){

  Serial.print("Executando comando [");
  Serial.print(command);
  Serial.println("] enviado...");

  switch(command){

  case '1':
    flagReadSensorsCSV = true;
    flagCreateFile = true;
    client.stop();
    break;

  case '2':
    flagReadSensorsCSV = false;
    client.stop();
    break;

  case '3':
    system("rm /opt/weblabmotor/web/database/*.csv");
    client.stop();
    break;

  case '4':
    flagReadSensorsHTTP = true;
    client.stop();
    break;

  case '5':
    flagReadSensorsHTTP = false;
    client.stop();
    break;

  case '9':
    Serial.println("Reiniciando experimento...");
    client.stop();
    setup();
    break;

  default:
    Serial.println("Comando nao cadastrado");
    client.stop(); // Fechar qualquer conexao
    break;

  }


}

void readSensorsCSV(){

  tempoAtual();
  temperaturaUSR();
  temperaturaTermopar();
  barometro();

  FILE * pFile;
  pFile = fopen ("/opt/weblabmotor/web/database/main.csv", "a+");
  char sensorBuffer [25];    

  if(flagCreateFile){
    char headerBuffer [51];    
    sprintf (headerBuffer,"Tempo,Termopar,TempBar,PresAt,Altitude,Processador\n"); 
    fwrite (headerBuffer , sizeof(char), sizeof(headerBuffer), pFile);
    flagCreateFile = false;
  }

  sprintf (sensorBuffer,"%s,%d,%d,%d,%d,%d\n", dadosSensores.tempo, dadosSensores.tempTPAext, dadosSensores.tempBAR, dadosSensores.presBAR, dadosSensores.altBAR, dadosSensores.tempUSR); 

  Serial.print(sensorBuffer); 
  fwrite (sensorBuffer , sizeof(char), sizeof(sensorBuffer), pFile);
  fclose(pFile);

  memset(sensorBuffer, 0, 50);
  memset(dadosSensores.tempo, 0, 50);
  dadosSensores.tempTPAext = 0;
  dadosSensores.tempTPAint = 0;
  dadosSensores.tempBAR = 0;
  dadosSensores.presBAR = 0;
  dadosSensores.altBAR = 0;  
}

void readSensorsHTTP(){

  EthernetClient clientMySQL;

  temperaturaUSR();
  temperaturaTermopar();
  barometro();
  correnteAC();
  voltimetroAC();
  rotacao();

  //char sensorBuffer [100];
  //sprintf (sensorBuffer,"a0=%d&a1=%d&a2=%d&a3=%d&a4=%d&a5=%d&a6=%d&a7=%f&a8=%f",
  //dadosSensores.tempTPAint,
  //dadosSensores.tempTPAext,
  //dadosSensores.tempBAR,
  //dadosSensores.presBAR,
  //dadosSensores.altBAR,
  //dadosSensores.tempUSR,
  //dadosSensores.rotacao,
  //dadosSensores.voltAC,
  //dadosSensores.ampAC);

  //Serial.println(sensorBuffer);

  //memset(sensorBuffer, 0, 100);

  if (clientMySQL.connect(serverMySQL, 80)) {

    Serial.println("database connected");

    char headerBuffer [87];
    sprintf (headerBuffer,"GET /home/cgi-bin/php/mysql/database-insert.php?a0=%d&a1=%d&a2=%d&a3=%d&a4=%d&a5=%d HTTP/1.1",
    dadosSensores.tempTPAint,
    dadosSensores.tempTPAext,
    dadosSensores.tempBAR,
    dadosSensores.presBAR,
    dadosSensores.altBAR,
    dadosSensores.tempUSR);

    Serial.println(headerBuffer);
    clientMySQL.println(headerBuffer);
    clientMySQL.println("Host: 192.168.0.100");
    clientMySQL.println("Connection: close");
    clientMySQL.println();
    Serial.println("database disconnecting.");

    //limpar dados remanescentes e fechar conexao
    memset(headerBuffer, 0, 87);
    clientMySQL.flush();
    clientMySQL.stop();
  } 
  else {
    Serial.println("database connection failed");
  }

  if (clientMySQL.connect(serverMySQL, 80)) {

    Serial.println("database connected");

    char headerBuffer [87];
    sprintf (headerBuffer,"GET /home/cgi-bin/php/mysql/database-insert.php?a6=%d&a7=%f&a8=%f HTTP/1.1",
    dadosSensores.rotacao,
    dadosSensores.voltAC,
    dadosSensores.ampAC);

    Serial.println(headerBuffer);
    clientMySQL.println(headerBuffer);
    clientMySQL.println("Host: 192.168.0.100");
    clientMySQL.println("Connection: close");
    clientMySQL.println();
    Serial.println("database disconnecting.");

    //limpar dados remanescentes e fechar conexao
    memset(headerBuffer, 0, 87);
    clientMySQL.flush();
    clientMySQL.stop();
  } 
  else {
    Serial.println("database connection failed");
  }

  dadosSensores.tempTPAint = 0;
  dadosSensores.tempTPAext = 0;
  dadosSensores.tempBAR = 0;
  dadosSensores.presBAR = 0;
  dadosSensores.altBAR = 0;
  dadosSensores.tempUSR = 0;
  dadosSensores.rotacao = 0;
  dadosSensores.voltAC = 0;
  dadosSensores.ampAC = 0;

}

void tempoAtual(){

  char buf[20];

  system("date '+%s%3N' > /home/root/time.txt");

  FILE *fp;

  fp = fopen("/home/root/time.txt", "r");

  fgets(buf, 20, fp);

  fclose(fp);

  buf[strlen(buf) - 1] = '\0';

  strcpy(dadosSensores.tempo,buf);

  //Serial.print("The current time is ");
  //Serial.println(buf);

}

void verificarComandos(EthernetClient client){

  //Verifica se existe um cliente conectado
  if (client) {

    //Serial.println("Cliente Conectado");

    //Verifica se existem dados remanescentes do cliente, conectado ou nao
    if(client.available()) {

      c = client.read();//Faz a leitura de um caracter
      client.flush();
      command = c ;
      Serial.print("Caracter recebido [");
      Serial.print(c);
      Serial.println("]");

      runCommand = true;
      /*
     if ( c >= 0 & c <= 127 ){// Testa se o caracter é valido. (client.read irá enviar -1 para sinalizar que não existem mais dados)
       
       Serial.println("Caracter valido");
       
       }else{
       
       Serial.println("Caracter nao reconhecido na tabela ASCII, finalizando conexao");
       client.flush(); //Exclusao de qualquer dado remanescente dos clients
       client.stop(); // Fechar qualquer conexao
       runCommand = false; 
       }
       
       //checar sinalizacao do final do comando
       if(c == 13 || c == 10 || c== -1){
       Serial.println("Caracter de finalização detectado, encerrando conexao");
       client.flush(); //Exclusao de qualquer dado remanescente dos clients
       client.stop(); // Fechar qualquer conexao
       runCommand = false; 
       }
       */
    }

  }

}

void temperaturaTermopar(){

  // Read thermocouple data
  thermocouple.read();

  if (thermocouple.isShortedToVcc()) {
    Serial.println("Thermocouple is Shorted to VCC");
    dadosSensores.tempTPAext = 0 ;
  } 
  else if (thermocouple.isShortedToGnd()) {
    Serial.println("Thermocouple is Shorted to GND");
    dadosSensores.tempTPAext = 0 ;
  } 
  else if (thermocouple.isOpen()) {
    Serial.println("Thermocouple Open circuit");
    dadosSensores.tempTPAext = 0 ;
  }
  else{
    dadosSensores.tempTPAext = thermocouple.getExternal();    
  }

  dadosSensores.tempTPAint = thermocouple.getInternal();

}

void barometro() {
  // request temperature
  barometer.setControl(BMP085_MODE_TEMPERATURE);

  // wait appropriate time for conversion (4.5ms delay)
  lastMicros = micros();
  while (micros() - lastMicros < barometer.getMeasureDelayMicroseconds());

  // read calibrated temperature value in degrees Celsius
  temperature = barometer.getTemperatureC();

  // request pressure (3x oversampling mode, high detail, 23.5ms delay)
  barometer.setControl(BMP085_MODE_PRESSURE_3);
  while (micros() - lastMicros < barometer.getMeasureDelayMicroseconds());

  // read calibrated pressure value in Pascals (Pa)
  pressure = barometer.getPressure();

  // calculate absolute altitude in meters based on known pressure
  // (may pass a second "sea level pressure" parameter here,
  // otherwise uses the standard value of 101325 Pa)
  altitude = barometer.getAltitude(pressure);

  // display measured values if appropriate
  //Serial.print("T/P/A\t");
  //Serial.print(temperature); Serial.print("\t");
  //Serial.print(pressure); Serial.print("\t");
  //Serial.print(altitude);
  //Serial.println("");

  dadosSensores.tempBAR = temperature;
  dadosSensores.presBAR = pressure;
  dadosSensores.altBAR = altitude;
}

void movimento() {
  // read raw accel/gyro measurements from device
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // these methods (and a few others) are also available
  //accelgyro.getAcceleration(&ax, &ay, &az);
  //accelgyro.getRotation(&gx, &gy, &gz);

#ifdef OUTPUT_READABLE_ACCELGYRO
  // display tab-separated accel/gyro x/y/z values
  Serial.print("a/g:\t");
  Serial.print(ax); 
  Serial.print("\t");
  Serial.print(ay); 
  Serial.print("\t");
  Serial.print(az); 
  Serial.print("\t");
  Serial.print(gx); 
  Serial.print("\t");
  Serial.print(gy); 
  Serial.print("\t");
  Serial.println(gz);
#endif

#ifdef OUTPUT_BINARY_ACCELGYRO
  Serial.write((uint8_t)(ax >> 8)); 
  Serial.write((uint8_t)(ax & 0xFF));
  Serial.write((uint8_t)(ay >> 8)); 
  Serial.write((uint8_t)(ay & 0xFF));
  Serial.write((uint8_t)(az >> 8)); 
  Serial.write((uint8_t)(az & 0xFF));
  Serial.write((uint8_t)(gx >> 8)); 
  Serial.write((uint8_t)(gx & 0xFF));
  Serial.write((uint8_t)(gy >> 8)); 
  Serial.write((uint8_t)(gy & 0xFF));
  Serial.write((uint8_t)(gz >> 8)); 
  Serial.write((uint8_t)(gz & 0xFF));
#endif
}


void temperaturaUSR(){

  FILE *fp_raw;
  fp_raw = fopen("/sys/bus/iio/devices/iio:device0/in_temp0_raw", "r");     //read the values from scale, raw and offset files.
  fgets(raw, 4, fp_raw);                                                    //we need all three values, because the formula for
  fclose(fp_raw);                                                           //calulating the actual temperature in milli-degrees Celcius
  //is: TEMP = (RAW + OFFSET) * SCALE
  FILE *fp_scale;
  fp_scale = fopen("/sys/bus/iio/devices/iio:device0/in_temp0_scale", "r");
  fgets(scale, 4, fp_scale);
  fclose(fp_scale);

  FILE *fp_offset;
  fp_offset = fopen("/sys/bus/iio/devices/iio:device0/in_temp0_offset", "r");
  fgets(offset, 4, fp_offset);
  fclose(fp_offset);

  raw_i = atoi(raw);         //we have the values now, but they are in ASCII form-                                                      
  scale_i = atoi(scale);     //we need them as integers so we can use them for calculations.
  offset_i = atoi(offset);

  int temp = (raw_i + offset_i) * scale_i;  //Calculate temperature in milli-degrees celcius
  temp /= 1000;                         //divide by 1000 to convert to degrees celcius

  dadosSensores.tempUSR = temp;

}

void correnteAC(){

  Voltage = getVPP();
  VRMS = (Voltage/2.0) *0.707; 
  AmpsRMS = (VRMS * 1000)/mVperAmp;
  dadosSensores.ampAC = AmpsRMS;
  //Serial.print(AmpsRMS);
  //Serial.println(" Amps RMS");

}

double getVPP()
{
  double result;

  int readValue;             //value read from the sensor
  int maxValue = 0;          // store max value here
  int minValue = 1023;          // store min value here

  uint32_t start_time = millis();
  while((millis()-start_time) < 1000) //sample for 1 Sec
  {
    readValue = analogRead(sensorIn);
    // see if you have a new maxValue
    if (readValue > maxValue) 
    {
      /*record the maximum sensor value*/
      maxValue = readValue;
    }
    if (readValue < minValue) 
    {
      /*record the maximum sensor value*/
      minValue = readValue;
    }
  }

  // Subtract min from max
  result = ((maxValue - minValue) * 5.0)/1023.0;

  return result;
}

void rotacao(){

  int start_time = millis();

  startInterrupt = true;

  while((millis()-start_time) < 500){

    if(startInterrupt){
      attachInterrupt(3, rpm_fun, CHANGE);
      startInterrupt = false;
    }
    calculaRotacao();  
  }
  detachInterrupt(3);
}

void calculaRotacao(){

  if (half_revolutions >= 12) {

    double a = (millis() - timeold)/1000.0;
    double b = (a/half_revolutions);
    double c = 2.0*b;
    double d = 1.0/c;
    rpm = 60.0*d;
    timeold = millis();
    a = (int) rpm;

    half_revolutions = 0;
    //Serial.println(rpm);
    dadosSensores.rotacao = a;

  }
}

void rpm_fun()
{
  half_revolutions++;
}

void voltimetroAC() {

  int sensorValue = analogRead(voltimetroPin);
  double b = (sensorValue*5.0)/1023.0;
  double a = (b+0.13)/0.03;
  dadosSensores.voltAC = a;
  //Serial.print("Vrms= ");
  //Serial.println(a);
}
//.....................................END FUNCTIONS...............................



