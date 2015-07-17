#include <SPI.h>
#include <Ethernet.h>

//Endereco MAC para o shield Ethernet
byte mac[] = { 0x98, 0x4F, 0xEE, 0x00, 0x25, 0x6F };

//Pino de controle do motor
int motor = 6;
int statusMotor;

//Mensagem enviada pela comunicacao ethernet para controlar o experimento
char c = 0;
char command = 0;

//Flags
//Comando recebido com sucesso
boolean runCommand = false;
boolean runReadSensors = false;

//Gets a client that is connected to the server and has data available for reading
EthernetClient client;

// Acesso ao shield ethernet pela porta 10000
EthernetServer server(10000);

void setup() {

  // Inicia-se a comunicaço serial e aguarda uma resposta
  Serial.begin(9600);

  pinSetup();
  conectarRede();

}


void loop() {

  client = server.available();
  
  // Verificar clients e receber comandos via ethernet.
  verificarComandos(client);
  
  comandos();
  
  if(runReadSensors){
    comandos();
    readSensors(client);    
  }
}


void pinSetup(){
  
  pinMode(motor,OUTPUT);
  digitalWrite(motor,LOW);

}

void comandos(){
  
  if(runCommand){
  
  runCommand = false;
    
  Serial.print("Executando comando [");
  Serial.print(command);
  Serial.println("] enviado...");
  
    switch(command){
    
    case '1':
    controlarMotor();
    //client.flush(); //Exclusao de qualquer dado remanescente dos clients
    //client.stop(); // Fechar qualquer conexao
    break;
    
    case '2':
    runReadSensors = true;
    //client.flush(); //Exclusao de qualquer dado remanescente dos clients
    //client.stop(); // Fechar qualquer conexao 
    break;
    
    case '3':
    runReadSensors = false;
    //client.flush(); //Exclusao de qualquer dado remanescente dos clients
    //client.stop(); // Fechar qualquer conexao 
    break;
    
    case '9':
    resetExperimento();
    //client.flush(); //Exclusao de qualquer dado remanescente dos clients
    //client.stop(); // Fechar qualquer conexao
    break;
    
    default:
    Serial.println("Comando nao cadastrado");
    client.flush(); //Exclusao de qualquer dado remanescente dos clients
    client.stop(); // Fechar qualquer conexao
    break;
    
    }
  
  }
}

void readSensors(EthernetClient client){
  temperaturaBloco(client);
}

void verificarComandos(EthernetClient client){
  
  //Verifica se existe um cliente conectado
  if (client) {
  
  Serial.println("Cliente Conectado");
  
  //Verifica se existem dados remanescentes do cliente, conectado ou nao
  if(client.connected()) {
  
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

void temperaturaBloco(EthernetClient client){
  
  int i;
  int calotaBrancaAnalog = A0;
  
  //Coleta 5 amostas
  for (i = 0; i < 5; i++){
  calotaBrancaAnalog = calotaBrancaAnalog + analogRead(A0);
  }
  
  //Media das 5 amostras
  calotaBrancaAnalog = calotaBrancaAnalog/5;
  
  float temperaturaCalotaBranca=(5*calotaBrancaAnalog*100)/1023;
  
  Serial.println(temperaturaCalotaBranca);
  delay(500);
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

void conectarRede(){
  
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

void resetExperimento(){

  Serial.println("Reiniciando experimento...");
  conectarRede();
  digitalWrite(motor,LOW);
  
}

