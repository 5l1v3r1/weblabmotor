
#include "SPI.h"
#include "Ethernet.h"

//Endereco MAC para o shield Ethernet
byte mac[] = { 0x98, 0x4F, 0xEE, 0x00, 0x25, 0x6F };
EthernetClient client;
EthernetServer server(10000);

//Mensagem enviada pela comunicacao ethernet para controlar o experimento
char c;
String command; //string for fetching data from address
int i;

//Flags
boolean runCommand = false;
boolean runReadSensors = false;

void setup() {
  // put your setup code here, to run once:

  // Inicia-se a comunicaço serial e aguarda uma resposta
  Serial.begin(9600);
  setupRede();
  Serial.println("Inicializando...");
}

void loop() {
  // put your main code here, to run repeatedly

  client = server.available();

  // Verificar clients e receber comandos via ethernet.
  if (!runCommand) {
    verificarComandos(client);
  }
  
  if(runReadSensors){
    readSensors();
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
      command = "";
    } else if ( command.substring(0, x) == "pwm") {
      Serial.print("Dados pwm: ");
      Serial.println(command.substring(x + 1, t));
      client.stop();
      command = "";
    } else if (command.substring(0, x) == "dados") {
       Serial.println("controlar fluxo de dados ");
      if( command.substring(x + 1, t) == "on"){
        Serial.println("fluxo de dados ligado");
        runReadSensors=true;
      }else if(command.substring(x + 1, t) == "off"){
        Serial.println("fluxo de dados desligado");
        runReadSensors=false;
      }
    }
      client.stop();
      command = "";
  }

  Serial.println(i++);
  delay(1000);

}

void setupRede() {

  if (!Ethernet.begin(mac)) {
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
      }

    }
  }
}

void readSensors() {
  Serial.println("enviando dados...");
  client.println("dados");
}
