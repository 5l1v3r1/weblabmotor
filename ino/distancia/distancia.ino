int trig = 3;
int echo = 4;
int tempoInicial = 0;
int tempoFinal = 0;

void setup() {
  Serial.begin(115200);
  
  pinMode(trig,OUTPUT_FAST);
  pinMode(echo,INPUT_FAST);

}

void loop() {
    
  digitalWrite(trig,LOW);
  delayMicroseconds(2);
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
  
  while(!digitalRead(echo)){
    if(!tempoInicial) tempoInicial = micros();
    tempoFinal = micros();
    //Serial.println(micros());
  }
  Serial.println(tempoInicial);
  Serial.println(tempoFinal);
  
  tempoInicial = 0;
  tempoFinal = 0;
  
  delay(1000);
}
