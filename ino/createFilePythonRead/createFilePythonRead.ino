 

 

 

/*
This example uses Linux system calls to create a python script which writes
number 0-9 to a file, log.txt, one number per second. Then execute the
python script in the background, and regularly read the contents of the logfile
in the sketch while the python script is updating it.
*/

char output[3];

void setup() {
 
  Serial.begin(115200);
 
  system("echo '#!/usr/bin/python' > myScript.py");
  system("echo 'import time' >> myScript.py");
  system("echo 'for i in range(10):' >> myScript.py");
  system("echo '    with open(\"log.txt\", \"w\") as fh:' >> myScript.py");
  system("echo '        fh.write(\"{0}\".format(i))' >> myScript.py");
  system("echo '    time.sleep(1)' >> myScript.py");
  system("chmod a+x myScript.py");
  system("./myScript.py &");    
}

void loop() {
 
  FILE *fp;
  fp = fopen("log.txt", "r");
  fgets(output, 2, fp);
  fclose(fp);
  Serial.println(output);
  delay(1000);
 
}

