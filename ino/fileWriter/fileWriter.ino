
void setup() {
  Serial.begin(9600);
  //FILE * pFile;
  //char buffera[64];
  //root.printTo(buffera, sizeof(buffera));
  //pFile = fopen ("/home/root/myfile.json", "a");
  //fwrite (buffera , sizeof(char), sizeof(buffera), pFile);
  
//[{"DATA": ["2015-07-27 21:16:05", "2015-07-27 21:16:05", "2015-07-27 21:16:06", "2015-07-27 21:16:06"]}, {"A0": [0, 0, 0, 0]}, {"A1": [2, 2, 2, 2]}]

  char bffr [10000];
  
  
  sprintf(bffr,"[{\"DATA\": [\"2015-07-27 21:16:05\", \"2015-07-27 21:16:05\", \"2015-07-27 21:16:06\", \"2015-07-27 21:16:06\"]}, {\"A0\": [0, 0, 0, 0]}, {\"A1\": [2, 2, 2, 2]}]"); 
  Serial.println(bffr);

}

void loop() {
  // put your main code here, to run repeatedly:

}
