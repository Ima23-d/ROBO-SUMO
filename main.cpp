// Define os pinos dos LEDs

// MOTORES
int a1a = 6; 
int a1b = 9;

int b1a = 3;
int b1b = 5;

// SENSOR ULTRASONICO
const int trig  = 10;
const int echo = 11;

// SENSOR DE REFLETANCIA
int refle1 = 12;
int refle2 = 13;


void setup() {

  pinMode(b1a,OUTPUT);
  pinMode(b1b,OUTPUT);

  pinMode(a1a,OUTPUT);
  pinMode(a1b,OUTPUT);

  pindeMode(trig,OUTPUT);
  pinMode(echo,INPUT);

  pinMode(refle1,INPUT);
  pinMode(refle2,INPPUT);


}
void parado(){
  digitalWrite(b1a,LOW);
  digitalWrite(b1b,LOW);

  digitalWrite(a1a,LOW);
  digitalWrite(a1b,LOW);

 }

void frente(){
  digitalWrite(b1a,LOW);
  digitalWrite(b1b,HIGH);

  digitalWrite(a1a,LOW);
  digitalWrite(a1b,HIGH);

 }

void re(){
  digitalWrite(b1a,HIGH);
  digitalWrite(b1b,LOW);

  digitalWrite(a1a,HIGH);
  digitalWrite(b1b,LOW);

  }

void esquerdafrente(){
  digitalWrite(b1a,LOW);
  digitalWrite(b1b,HIGH);

  digitalWrite(a1a,LOW);
  digitalWrite(b1b,LOW);

}

void direitafrente(){
  digitalWrite(b1a,LOW);
  digitalWrite(b1b,LOW);
 
  digitalWrite(a1a,LOW);
  digitalWrite(b1b,HIGH);
 
}

void direitare(){
  digitalWrite(b1a,LOW);
  digitalWrite(b1b,LOW);

  digitalWrite(a1a,HIGH);
  digitalWrite(b1b,LOW);
  
}

void esquerdare(){
  digitalWrite(b1a,HIGH);
  digitalWrite(b1b,LOW);

  digitalWrite(a1a,LOW);
  digitalWrite(b1b,LOW);

}


void loop() {

  digitalWrite(trig,HIGH);
  delayMicosseconds(2);
  digitalWrite(trig,LOW);
  delayMicrosseconds(10);
  digitalWrtie(tri,LOW);

  int duration = pulseIn(echo,HIGH);
  int distance = duration*0.034/2

  frente();
  delay(2000);
  parado();
  delay(3000);
  re();
  delay(1000);
  
  Serial.println(distance);
}


