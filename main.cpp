// =======================================
// CONFIGURAÇÃO DE PINOS
// =======================================

// Motores
int a1a = 6;
int a1b = 9;
int b1a = 3;
int b1b = 5;

// Sensor ultrassônico
const int trig = 10;
const int echo = 11;

// Sensores de refletância (borda do ringue)
int refle1 = 12;
int refle2 = 13;

// =======================================
// CONFIGURAÇÃO INICIAL
// =======================================
void setup() {
  Serial.begin(9600);

  // Motores
  pinMode(a1a, OUTPUT);
  pinMode(a1b, OUTPUT);
  pinMode(b1a, OUTPUT);
  pinMode(b1b, OUTPUT);

  // Sensor ultrassônico
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  // Sensores de refletância
  pinMode(refle1, INPUT);
  pinMode(refle2, INPUT);

  // Inicializa parado
  parado();
  delay(1000);
}

// =======================================
// FUNÇÕES DE MOVIMENTO
// =======================================
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

void girarDireita() {
  digitalWrite(a1a, LOW);
  digitalWrite(a1b, HIGH);
  digitalWrite(b1a, HIGH);
  digitalWrite(b1b, LOW);
}

void girarEsquerda() {
  digitalWrite(a1a, HIGH);
  digitalWrite(a1b, LOW);
  digitalWrite(b1a, LOW);
  digitalWrite(b1b, HIGH);
}

// =======================================
// LEITURA DO ULTRASSÔNICO
// =======================================
int lerDistancia() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  int duracao = pulseIn(echo, HIGH);
  int distancia = duracao * 0.034 / 2; // cm
  return distancia;
}

// =======================================
// LOOP PRINCIPAL - LÓGICA DE BATALHA
// =======================================
void loop() {
  int distancia = lerDistancia();
  int refleValor1 = digitalRead(refle1);
  int refleValor2 = digitalRead(refle2);

  // ======= EVITAR SAIR DO RINGUE =======
  // (Assumindo que o branco reflete mais — ou seja, HIGH = fora do ringue)
  if (refleValor1 == HIGH || refleValor2 == HIGH) {
    Serial.println("Borda detectada! Recuando...");
    re();
    delay(400);
    if (refleValor1 == HIGH) {
      girarDireita();
      delay(400);
    } else if (refleValor2 == HIGH) {
      girarEsquerda();
      delay(400);
    }
  }

  // ======= DETECTAR E ATACAR INIMIGO =======
  else if (distancia > 0 && distancia < 25) { // inimigo detectado
    Serial.print("Inimigo detectado a ");
    Serial.print(distancia);
    Serial.println(" cm — ATACAR!");
    frente();
    delay(150);
  }

  // ======= BUSCAR INIMIGO (SCANNER) =======
  else {
    Serial.println("Procurando inimigo...");
    parado();
    delay(100);
    girarDireita();
    delay(300);
  }

  // Exibe dados no monitor serial
  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.print(" cm | Refle1: ");
  Serial.print(refleValor1);
  Serial.print(" | Refle2: ");
  Serial.println(refleValor2);

  delay(50);
}
