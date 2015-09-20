#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0x98, 0x4F, 0xEE, 0x01, 0x14, 0x8A };

IPAddress server(192,168,1,38);  // numeric IP for MySQL server

EthernetClient client;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  setupRede();
  

}

void loop()
{

  sendData();
  delay(1000);
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

  delay(200);
  Serial.print("IP: ");
  Serial.println(Ethernet.localIP());
  
}

void sendData(){
  
  if (client.connect(server, 80)) {
   
    Serial.println("connected");
     int a0=1;
     int a1=1;
     int a2=1;
     int a3=1;
     int a4=1;
     int a5=1;

    
    // Make a HTTP request:
    char headerBuffer [87];
    sprintf (headerBuffer,"GET /home/cgi-bin/python/mysql/database-insert.py?a0=%d;a1=%d;a2=%d;a3=%d;a4=%d;a5=%d HTTP/1.1",a0,a1,a2,a3,a4,a5);
    //Serial.println(headerBuffer);
    client.println(headerBuffer);
    client.println("Host: 192.168.1.38");
    client.println("Connection: close");
    client.println();
    Serial.println("disconnecting.");

    //limpar dados remanescentes e fechar conexao
    client.flush();
    client.stop();
  }
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}


