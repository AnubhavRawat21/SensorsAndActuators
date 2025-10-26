int step = 7;
int dir = 5;
int led = 13;
float stepsPerRevolution  = 200.0;
float steps = 0;
float rps = 0;
float delayMicro = 0;

void setup(){
  pinMode(step,OUTPUT);
  pinMode(dir,OUTPUT);
  pinMode(led,OUTPUT);

  digitalWrite(dir,HIGH);
  digitalWrite(led,HIGH);

  Serial.begin(9600);
  Serial.println("Enter rotations per second: ");
}
void loop(){
  if(Serial.available()){
    rps = Serial.parseFloat();

    if(rps>0){
     steps = rps*stepsPerRevolution;
     delayMicro = (1.0/steps)*1000000.0;
    }else{
      delayMicro = 0;
    }

    Serial.print("RPS: ");
    Serial.print(rps);
    Serial.print(" Delay Microseconds: ");
    Serial.println(delayMicro);

    for(int i=0;i<=200;i++){
      digitalWrite(step,HIGH);
      delayMicroseconds(delayMicro);
      digitalWrite(step,LOW);
      delayMicroseconds(delayMicro);
    }
    
  }

}