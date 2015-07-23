#include <LiquidCrystal.h>

char buf[150];
char bufmac[100];
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
    system("./weblabmotor/sh/eth.sh");
    lcd.begin(16,2);
 }

void loop() {
  // put your main code here, to run repeatedly:
  
  lcd.clear();
  
  FILE *fp;
  fp = fopen("/home/root/ip.txt", "r");
  fgets(buf, 150, fp);
  fclose(fp);
 
  Serial.print("The IP is ");
  Serial.println(buf);
  lcd.print(buf);
  
  lcd.setCursor(0,1);
  
  FILE *fpmac;
  fpmac = fopen("/home/root/mac.txt", "r");
  fgets(bufmac, 150, fpmac);
  fclose(fpmac);
 
  Serial.print("The IP is ");
  Serial.println(bufmac);
  lcd.print(bufmac);
  
  
  delay(1000);

}
