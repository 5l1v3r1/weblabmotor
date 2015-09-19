#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0x98, 0x4F, 0xEE, 0x01, 0x14, 0x8A };

IPAddress server(192,168,2,3);  // numeric IP for MySQL server

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
    // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("connected");
    // Make a HTTP request:
    client.println("GET /home/cgi-bin/python/mysql/database-insert.py?a0=1;a1=2;a2=3;a3=2;a4=2;a5=3 HTTP/1.1");
    client.println("Host: 192.168.2.3");
    client.println("Connection: close");
    client.println();
    Serial.println("disconnecting.");
    client.stop();
  }
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}


