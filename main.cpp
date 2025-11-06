// ===============================
// DEFINIÇÃO DE PINOS
// ===============================

// MOTORES
int a1a = 6; 
int a1b = 9;

int b1a = 3;
int b1b = 5;

// SENSOR ULTRASSÔNICO
const int trig = 10;
const int echo = 11;

// SENSORES DE REFLETÂNCIA
int refle1 = 12;
int refle2 = 13;

void setup() {
  Serial.begin(9600);

  // Configuração dos pinos dos motores
  pinMode(a1a, OUTPUT);
  pinMode(a1b, OUTPUT);
  pinMode(b1a, OUTPUT);
  pinMode(b1b, OUTPUT);

  // Configuração do sensor ultrassônico
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  // Configuração dos sensores de refletância
  pinMode(refle1, INPUT);
  pinMode(refle2, INPUT);
}

// ===============================
// FUNÇÕES DE MOVIMENTO
// ===============================
void parado() {
  digitalWrite(a1a, LOW);
  digitalWrite(a1b, LOW);
  digitalWrite(b1a, LOW);
  digitalWrite(b1b, LOW);
}

void frente() {
  digitalWrite(a1a, LOW);
  digitalWrite(a1b, HIGH);
  digitalWrite(b1a, LOW);
  digitalWrite(b1b, HIGH);
}

void re() {
  digitalWrite(a1a, HIGH);
  digitalWrite(a1b, LOW);
  digitalWrite(b1a, HIGH);
  digitalWrite(b1b, LOW);
}

void esquerdafrente() {
  digitalWrite(a1a, LOW);
  digitalWrite(a1b, LOW);
  digitalWrite(b1a, LOW);
  digitalWrite(b1b, HIGH);
}

void direitafrente() {
  digitalWrite(a1a, LOW);
  digitalWrite(a1b, HIGH);
  digitalWrite(b1a, LOW);
  digitalWrite(b1b, LOW);
}

void direitare() {
  digitalWrite(a1a, HIGH);
  digitalWrite(a1b, LOW);
  digitalWrite(b1a, LOW);
  digitalWrite(b1b, LOW);
}

void esquerdare() {
  digitalWrite(a1a, LOW);
  digitalWrite(a1b, LOW);
  digitalWrite(b1a, HIGH);
  digitalWrite(b1b, LOW);
}

// ===============================
// LOOP PRINCIPAL
// ===============================
void loop() {
  // ======== SENSOR ULTRASSÔNICO ========
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  int duration = pulseIn(echo, HIGH);
  int distance = duration * 0.034 / 2; // Distância em cm

  // ======== SENSORES DE REFLETÂNCIA ========
  int valorRefle1 = digitalRead(refle1);
  int valorRefle2 = digitalRead(refle2);

  // ======== LÓGICA DE CONTROLE (exemplo) ========
  if (distance < 10) { 
    parado(); // Para se obstáculo estiver perto
  } else if (valorRefle1 == LOW && valorRefle2 == LOW) {
    frente(); // Linha detectada pelos dois sensores
  } else if (valorRefle1 == LOW && valorRefle2 == HIGH) {
    esquerdafrente(); // Corrige para a esquerda
  } else if (valorRefle1 == HIGH && valorRefle2 == LOW) {
    direitafrente(); // Corrige para a direita
  } else {
    frente(); // Caminho livre
  }

  // Exibe informações no monitor serial
  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.print(" cm | Refletância1: ");
  Serial.print(valorRefle1);
  Serial.print(" | Refletância2: ");
  Serial.println(valorRefle2);

  delay(100);
}
