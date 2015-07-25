/*

                     Arduino Pin Mapping:
   ----------------------------------------------------------
                 Use  Pin       Pin     Use
                      Power     SCL     MPU-6050 SCL
                 3.3V IOREF     SDS     MPU-6050 SDA
                      Reset     AREF
        Device power  3.3V      GND
                      5V        D13     Motor DirA
                      GND       D12     Motor DirB
                      GND       D11~    Motor PWMB
        Cut on motor  Vin       D10~
                                D9~     Motor BrakeA
          Motor CurA  A0        D8      Motor BrakeB
          Motor CurB  A1
     Battery Voltage  A2        D7      LED Heart Beat
                      A3        D6~     LED Upright
                      A4        D5~     
                      A5        D4
                                D3~     Motor PWMA
                                D2
                                D1 Tx   BT Rx
                                D0 Rx   BT Tx w/ 10K pull-up
   ----------------------------------------------------------
   
*/
#include "I2Cdev.h"
#include "MPU6050.h"
#include "BMP085.h"
#include "Wire.h"
#include "SPI.h"
#include "Ethernet.h"

//Endereco MAC para o shield Ethernet
byte mac[] = { 0x98, 0x4F, 0xEE, 0x00, 0x25, 0x6F };
EthernetClient client;
EthernetServer server(10000);

//Mensagem enviada pela comunicacao ethernet para controlar o experimento
char c = 0;
char command = 0;

//Pino de controle do motor
int motor = 6;
int statusMotor;

//Flags
//Comando recebido com sucesso
boolean runCommand = false;
boolean flagReadSensors = false;

//temperatura placa
char scale[4];
char raw[4];
char offset[4];

int raw_i;
int scale_i;
int offset_i;

//Barometro
float temperature;
float pressure;
float altitude;
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


//..................................MAIN.....................................
void setup() {

  // Inicia-se a comunicaço serial e aguarda uma resposta
  Serial.begin(9600);

  setupPin();
  setupRede();

}


void loop() {

  client = server.available();
  
  verificarComandos(client);
  
  executarComandoRecebido();
  
  if(flagReadSensors){
    readSensors();    
  }
}
//....................................MAIN...................................

//.......................................SETUP........................................

void setupPin(){
  
  pinMode(motor,OUTPUT);
  digitalWrite(motor,LOW);

}

void setupRede(){
  
  if(!Ethernet.begin(mac)){
    Serial.println("conectando com a rede...");
    Ethernet.begin(mac);
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
    Serial.begin(115200);

    // initialize device
    Serial.println("Initializing I2C devices...");
    
}

void setupBarometro() {
    barometer.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(barometer.testConnection() ? "BMP085 connection successful" : "BMP085 connection failed");

}

void setupMovimento() {
  
    accelgyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
}
//.....................................SETUP...........................................

//..................................................
void executarComandoRecebido(){
  
  if(runCommand){
  
  runCommand = false;
    
  Serial.print("Executando comando [");
  Serial.print(command);
  Serial.println("] enviado...");
  
    switch(command){
    
    case '1':
    controlarMotor();
    client.stop();
    break;
    
    case '2':
    flagReadSensors = true;
    client.stop();
    break;
    
    case '3':
    flagReadSensors = false;
    client.stop();
    break;
    
    case '9':
    resetExperimento();
    client.stop();
    break;
    
    default:
    Serial.println("Comando nao cadastrado");
    client.stop(); // Fechar qualquer conexao
    break;
    
    }
  
  }
}

void readSensors(){

  char bffr [1000];
  int tcsr = temperaturaCSR();
  int tblc = temperaturaBloco();
  
  sprintf (bffr,"python /opt/weblabmotor/python/database-insert.py /tmp/weblabmotor/temp.db %d 1 2 3 4 4 2 3", tcsr,tblc);
  Serial.println(bffr); 
  
  system(bffr);  

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
        
    }
  
  }

}

int temperaturaBloco(){
  
  int i;
  int calotaBrancaAnalog = A0;
  
  //Coleta 5 amostas
  for (i = 0; i < 5; i++){
  calotaBrancaAnalog = calotaBrancaAnalog + analogRead(A0);
  }
  
  //Media das 5 amostras
  calotaBrancaAnalog = calotaBrancaAnalog/5;
  
  float temperaturaCalotaBranca=(5*calotaBrancaAnalog*100)/1023;
  
  return(temperaturaCalotaBranca);
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

void resetExperimento(){

  Serial.println("Reiniciando experimento...");
  setup();
  
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
    Serial.print("T/P/A\t");
    Serial.print(temperature); Serial.print("\t");
    Serial.print(pressure); Serial.print("\t");
    Serial.print(altitude);
    Serial.println("");
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
        Serial.print(ax); Serial.print("\t");
        Serial.print(ay); Serial.print("\t");
        Serial.print(az); Serial.print("\t");
        Serial.print(gx); Serial.print("\t");
        Serial.print(gy); Serial.print("\t");
        Serial.println(gz);
    #endif

    #ifdef OUTPUT_BINARY_ACCELGYRO
        Serial.write((uint8_t)(ax >> 8)); Serial.write((uint8_t)(ax & 0xFF));
        Serial.write((uint8_t)(ay >> 8)); Serial.write((uint8_t)(ay & 0xFF));
        Serial.write((uint8_t)(az >> 8)); Serial.write((uint8_t)(az & 0xFF));
        Serial.write((uint8_t)(gx >> 8)); Serial.write((uint8_t)(gx & 0xFF));
        Serial.write((uint8_t)(gy >> 8)); Serial.write((uint8_t)(gy & 0xFF));
        Serial.write((uint8_t)(gz >> 8)); Serial.write((uint8_t)(gz & 0xFF));
    #endif
}


int temperaturaCSR(){

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
  return temp; 
 
}


