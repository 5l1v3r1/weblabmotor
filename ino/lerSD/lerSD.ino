
#include <LiquidCrystal.h>
#include <SD.h>

File ipnum;
File macnum;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup()
{
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
  
  lcd.begin(16, 2);

  system("/home/root/eth.sh");
  
  // re-open the file for reading:
  ipnum = SD.open("/dados/ip.txt");
  macnum = SD.open("/dados/mac.txt");

  if (ipnum) {
    Serial.println("/dados/ip.txt:");
    
    int i = 0;
    char ip[150];
    
    // read from the file until there's nothing else in it:
    while (ipnum.available()) {
       ip[i] = ipnum.read();
       i++;
    }     
    // close the file:
    ipnum.close();
    
    Serial.print(ip);
    lcd.clear();
    lcd.print(ip);
    
  } else {
  	// if the file didn't open, print an error:
    Serial.println("error opening file");
  }
  
  if (macnum) {
    Serial.println("/dados/mac.txt:");
    
    int i = 0;
    char mac[100];
    
    // read from the file until there's nothing else in it:
    while (macnum.available()) {
       mac[i] = macnum.read();
       i++;
    }
    // close the file:
    macnum.close();
    
    Serial.print(mac);
    lcd.clear();
    lcd.print(mac);
    
  } else {
  	// if the file didn't open, print an error:
    Serial.println("error opening file");
  }
  
}

void loop()
{
	// nothing happens after setup
}


