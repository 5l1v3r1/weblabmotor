int proporcional;

void setup() {
  Serial.begin(9600);

  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);

}

void loop() {
        
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
