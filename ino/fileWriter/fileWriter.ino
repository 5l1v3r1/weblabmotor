char buf[9];
void setup() {

  Serial.begin(9600);
  //FILE * pFile;
  //char buffera[64];
  //root.printTo(buffera, sizeof(buffera));
  //pFile = fopen ("/home/root/myfile.csv", "a");
  //fwrite (buffera , sizeof(char), sizeof(buffera), pFile);

  char bffr [10000];
  int a0 = 1;
  int a1 = 2;
  int a3 = 3;
  
  sprintf(bffr,"%d,a1,a2,tempo",a0);
  Serial.println(bffr);

}

void loop() {
  // put your main code here, to run repeatedly:
  system("date '+%H:%M:%S' > /home/root/time.txt");  //get current time in the format- hours:minutes:secs //and save in text file time.txt located in /home/root

  FILE *fp;

  fp = fopen("/home/root/time.txt", "r");

  fgets(buf, 9, fp);

  fclose(fp);

  Serial.print("The current time is ");

  Serial.println(buf);

  delay(1000);

}
