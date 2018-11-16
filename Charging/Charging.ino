boolean charging = false;
boolean light = false;
int last;
int last2;

void setup(){
  pinMode(13,OUTPUT);
  pinMode(12,INPUT);
  pinMode(10,INPUT);
  pinMode(11,OUTPUT);
  digitalWrite(13,LOW);
  digitalWrite(11,LOW);
  charging = false;
  light = false;
  
  Serial.begin(115200);
}

void loop(){
  if(digitalRead(12) == HIGH && last != HIGH){
    if(charging == false){
      Serial.write("");
      digitalWrite(13,HIGH);
      charging = true;
    }else if(charging == true){
      Serial.write("");
      digitalWrite(13,LOW);
      charging = false;
    }
  }
  if(digitalRead(10) == HIGH && last2 != HIGH){
    if(light == false){
      Serial.write("");
      digitalWrite(11,HIGH);
      light = true;
    }else if(light == true){
      Serial.write("");
      digitalWrite(11,LOW);
      light = false;
    }
  }
  
  last = digitalRead(12);
  last2 = digitalRead(10);
  
  if(Serial.available() > 0){
    String str = "";
    
    for(int i=0;i<Serial.available();i++){
      str += (char) Serial.read();
    }
    
    if(str == "1" && charging == true){
      digitalWrite(13,LOW);
      charging = false;
    }
    if(str == "0" && charging == false){
      digitalWrite(13,HIGH)  ;
      charging = true;
    }
    if(str == "2" && light == true){
      digitalWrite(11,LOW);
      light = false;
    }
    if(str == "3" && light == false){
      digitalWrite(11,HIGH);
      light = true;
    }
  }
}
