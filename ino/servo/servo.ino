int tensaoVoltimetro;
int tensaoCC;
int tensaoCA;

int coeficienteVoltimetro = 44;

int voltimetro = 0;
int gap = 200;

void setup() {
  Serial.begin(9600);

  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
}

void loop() {

  tensaoCC = (5 * analogRead(voltimetro))/1023;
  tensaoCA = tensaoCC * coeficienteVoltimetro;

  if(tensaoCA >= 120){
     sentidoHorario(gap);
  }else if(tensaoCA <= 100){
     sentidoAntiHorario(gap);    
  }

}

void sentidoHorario(int tempo){
  
  digitalWrite(12, LOW);   
  digitalWrite(11, LOW);    
  digitalWrite(10, LOW);    
  digitalWrite(9, HIGH);
  
  delay(tempo);  
  
  digitalWrite(12, LOW);   
  digitalWrite(11, HIGH);  
  digitalWrite(10, LOW);   
  digitalWrite(9, LOW);    

  delay(tempo);
  
  digitalWrite(12, LOW);   
  digitalWrite(11, LOW);   
  digitalWrite(10, HIGH);  
  digitalWrite(9, LOW);    
  
  delay(tempo);
  
  digitalWrite(12, HIGH);  
  digitalWrite(11, LOW);
  digitalWrite(10, LOW);
  digitalWrite(9, LOW); 
  
  delay(tempo);  
}

void sentidoAntiHorario(int tempo){
  
  digitalWrite(12, HIGH);   
  digitalWrite(11, LOW);    
  digitalWrite(10, LOW);    
  digitalWrite(9,  LOW);
  
  delay(tempo);  
  
  digitalWrite(12, LOW);   
  digitalWrite(11, LOW);  
  digitalWrite(10, HIGH);   
  digitalWrite(9, LOW);    

  delay(tempo);
  
  digitalWrite(12, LOW);   
  digitalWrite(11, HIGH);   
  digitalWrite(10, LOW);  
  digitalWrite(9, LOW);    
  
  delay(tempo);
  
  digitalWrite(12, LOW);  
  digitalWrite(11, LOW);
  digitalWrite(10, LOW);
  digitalWrite(9, HIGH); 
  
  delay(tempo);  
}
