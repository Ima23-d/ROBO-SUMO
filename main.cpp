// Define os pinos dos LEDs
int a1a = 6; 
int a1b = 9;

int b1a = 3;
int b1b = 5;



void setup() {

  pinMode(b1a,OUTPUT);
  pinMode(b1b,OUTPUT);

  pinMode(a1a,OUTPUT);
  pinMode(a1b,OUTPUT);

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
  frente();
  delay(2000);
  parado();
  delay(3000);
  re();
  // re();
  // parado();
  delay(1000);
}


